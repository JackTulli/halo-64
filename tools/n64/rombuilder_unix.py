"""Halo 64's ROM builder on macOS and Linux, with the release's programs.

The release's Halo64RomBuilder.zip carries each cartridge's prebuilt
program (its payload: tools/n64/package_rombuilder.py), the same for every
player; the tools beside them are Windows programs. This takes the zip's
programs, Invader's tag definitions, art and port.json, builds the tools
the ROM builder runs from their sources -- libdragon's mksprite,
audioconv64, mkdfs, n64tool, ed64romconfig and mkasset at LIBDRAGON_COMMIT
with LIBDRAGON_PATCHES, and for Custom Edition maps invader-lite and
ogg2wav at INVADER_COMMIT (tools/n64/invader_lite/build.sh, Linux only),
all with TOOL_FLAGS -- and runs tools/n64/rombuilder.py with that payload
(HALO64_PAYLOAD):

    python3 -m tools.n64.rombuilder_unix --release <Halo64RomBuilder.zip>
        <disc image or maps folder> -o <folder>
        [--multiplayer] [--campaign] [--custom-edition <CE map or .zip>...]
        [--ce-tools <folder>] [--jobs N] [--font <Halo.ttf>] [--tools <folder>]

--release also takes the zip's folder, unpacked. The release must be of
the version checked out here (tools/n64/version.py): the version its
version.txt names or, in a release without one, the version KNOWN_RELEASES
gives its multiplayer program; and its port.json must be port/n64/port.json.
The work files go in a new folder in TMPDIR, removed at the end (and on
Ctrl-C, SIGTERM or SIGHUP, which stop every process the build started);
only the ROMs stay, and the logs when the build fails. --tools keeps the
tools built in two folders of its own inside it (TOOL_FOLDERS), which a
later run reuses. JOBS sets how many compilers build the tools at a time
(default: one for each CPU; for invader-lite, whose compilers take much
memory, INVADER_JOBS at most).
"""

import argparse
import contextlib
import hashlib
import json
import os
import platform
import re
import shlex
import shutil
import signal
import subprocess
import sys
import tempfile
import time
import zipfile
import zlib
from pathlib import Path, PurePosixPath

from .rombuilder import CARTRIDGES, BuildError
from .version import VERSION

REPO = Path(__file__).resolve().parent.parent.parent
# the libdragon base that port/n64/docs/pc-expansion/PROVENANCE.md names
LIBDRAGON_URL = "https://github.com/DragonMinded/libdragon"
LIBDRAGON_COMMIT = "39d0d6096130836a65710da6730290701d060272"
# and the fixes its tools take here: audioconv64 read a missing path's
# uninitialised stat and, in Linux, wrote "<out>/x.wav//x.wav64"
LIBDRAGON_PATCHES = ("port/n64/devtools/libdragon-audioconv64-stat.patch",)
# the Invader that invader-lite is built from (tools/n64/invader_lite/README.md)
INVADER_URL = "https://github.com/SnowyMouse/invader"
INVADER_COMMIT = "696830ff80af227e84e7237c2ef26eb2301ed110"
INVADER_LITE = Path("tools") / "n64" / "invader_lite"
# the C and C++ compilers' flags for every tool built here: no fused
# multiply-add. The compilers fuse a*b+c on arm64 (Apple silicon) and not on
# x86-64 (as the release's Windows tools are built), and mksprite then made
# another sprite of one texture of the multiplayer maps (libdragon builds
# audioconv64's VADPCM encoder with this flag too)
TOOL_FLAGS = "-ffp-contract=off"
# libdragon's tools the ROM builder runs (package_rombuilder.TOOLS), where
# libdragon's tools/Makefile makes them
LIBDRAGON_TOOLS = {"mksprite": "mksprite/mksprite", "audioconv64": "audioconv64/audioconv64",
                   "mkdfs": "mkdfs/mkdfs", "n64tool": "n64tool", "ed64romconfig": "ed64romconfig",
                   "mkasset": "mkasset/mkasset"}
# the tools' folders in --tools (or in the work folder)
TOOL_FOLDERS = {"libdragon": "libdragon-tools", "invader-lite": "invader-lite"}
# what is taken from the payload (its bin/, ce-tools/ and online-kit/ are
# Windows programs); version.txt: package_rombuilder.py writes it
VERSION_FILE = "version.txt"
PAYLOAD_PARTS = ("port.json", VERSION_FILE, "elf/", "invader/", "art/")
# in each tools folder: what its tools were built from, as JSON. A folder
# with other files and without it is not this command's, and is never emptied
STAMP = "built-from.json"
# releases without version.txt: their multiplayer program's SHA-256
# (elf/multiplayer/halo.elf.stripped)
KNOWN_RELEASES = {"d104c4986ec85f02772b90186d36835af61d929cdb8480bf2e62760fca1a8c67": "0.6.9"}
# invader-lite's compilers at a time by default (its generated parsers take
# much memory: three were once too many for 4 GB)
INVADER_JOBS = 2
# how long a stopped process group has between SIGTERM and SIGKILL
GRACE = 5.0
VENV = "python3 -m venv ~/halo-64-venv && ~/halo-64-venv/bin/pip install pillow, then run this command with " \
       "~/halo-64-venv/bin/python3"
INSTALL_HINTS = {
    "darwin": f"xcode-select --install gives git, make and the C and C++ compilers; Python 3.12 or later with "
              f"Pillow (python.org or Homebrew), for example {VENV}",
    "linux": "on Debian or Ubuntu: sudo apt install build-essential git python3-pil, and for Custom Edition maps "
             "cmake ninja-build zlib1g-dev libsamplerate0-dev",
}


class Stopped(Exception):
    """SIGTERM or SIGHUP: stop as on Ctrl-C"""

    def __init__(self, number):
        super().__init__(f"signal {number}")
        self.number = number


def chosen_cartridges(multiplayer=False, campaign=False, custom=False):
    """the cartridges as tools/n64/rombuilder.py chooses them: the ones asked
    for, else multiplayer and the campaign"""
    chosen = [name for name, wanted in (("multiplayer", multiplayer), ("campaign", campaign)) if wanted]
    if custom:
        chosen.append("custom")
    return chosen or ["multiplayer", "campaign"]


def builds_ce_tools(cartridges, ce_tools, system=None):
    """True if invader-lite is built here: Custom Edition maps without a
    --ce-tools folder. Its build (tools/n64/invader_lite/build.sh) is for
    Linux"""
    if "custom" not in cartridges or ce_tools:
        return False
    if not (system or sys.platform).startswith("linux"):
        raise BuildError("Custom Edition maps need invader-lite, and tools/n64/invader_lite/build.sh builds it on "
                         "Linux only: build the Custom Edition cartridge on Linux, or name a folder of Invader's "
                         "tools and ogg2wav (or ffmpeg) built for this system with --ce-tools")
    return True


# ---------- what the machine needs

def developer_tools_missing():
    """macOS: True without the command line developer tools (/usr/bin/git,
    cc and make are there without them, and only ask for them)"""
    try:
        result = subprocess.run(["xcode-select", "-p"], capture_output=True, text=True, stdin=subprocess.DEVNULL)
    except OSError:
        return True
    return result.returncode != 0 or not Path(result.stdout.strip() or "/nonexistent").is_dir()


def missing_prerequisites(ce_tools_built, system=None):
    """what this machine lacks to build the tools and run the converters"""
    system = system or sys.platform
    missing = []
    if sys.version_info < (3, 12):
        missing.append(f"Python 3.12 or later (this is {platform.python_version()})")
    try:
        import PIL  # noqa: F401 (the converters' images)
    except ImportError:
        missing.append(f"Pillow for this Python ({sys.executable}): {VENV}" if system == "darwin" else
                       f"Pillow for this Python ({sys.executable})")
    compilers = [os.environ.get("CC", "cc"), os.environ.get("CXX", "g++")]
    commands = [("git", "git"), ("make", "make"), (compilers[0], f"a C compiler ({compilers[0]})"),
                (compilers[1], f"a C++ compiler ({compilers[1]})")]
    if system == "darwin" and developer_tools_missing():
        missing.append("the command line developer tools (xcode-select --install): git, make and the compilers")
        commands = []
    if ce_tools_built:
        commands += [("cmake", "cmake"), ("ninja", "ninja"), ("bash", "bash")]
    for command, what in commands:
        words = shlex.split(command)
        if not words or not shutil.which(words[0]):
            missing.append(what)
    return missing


# ---------- the release's payload

def find_payload(names):
    """the payload's folder among a release's file names (<...>/payload/)"""
    found = {name[:-len("port.json")] for name in names
             if name == "payload/port.json" or name.endswith("/payload/port.json")}
    if len(found) != 1:
        raise BuildError("this is not a Halo 64 ROM Builder release (Halo64RomBuilder.zip or its folder): "
                         + ("no payload/port.json in it" if not found else "more than one payload in it"))
    return found.pop()


def check_programs(release, names, prefix, cartridges):
    for cartridge in cartridges:
        program = f"elf/{CARTRIDGES[cartridge]['elf']}/"
        for name in ("halo.elf.stripped", "halo.elf.sym"):
            if prefix + program + name not in names:
                raise BuildError(f"{release.name} has no {cartridge} program ({program}{name})")


def payload_part(prefix, name):
    """a release file's path in the payload (PAYLOAD_PARTS), or None"""
    if not name.startswith(prefix):
        return None
    part = name[len(prefix):]
    if not part.startswith(PAYLOAD_PARTS):
        return None
    relative = PurePosixPath(part)
    if relative.is_absolute() or ".." in relative.parts:
        raise BuildError(f"the release has an unsafe file name: {name}")
    return relative


def extract_payload(release, folder, cartridges):
    """the release's payload without its Windows programs (PAYLOAD_PARTS) in
    folder, once the chosen cartridges' programs are found in it: from the
    zip, or from its folder unpacked"""
    if release.is_dir():
        return copy_payload(release, folder, cartridges)
    try:
        archive = zipfile.ZipFile(release)
    except (zipfile.BadZipFile, OSError) as error:
        raise BuildError(f"{release} is not a readable .zip file") from error
    with archive:
        members = [m for m in archive.infolist() if not m.is_dir()]
        names = [m.filename for m in members]
        prefix = find_payload(names)
        check_programs(release, set(names), prefix, cartridges)
        taken = [(m, payload_part(prefix, m.filename)) for m in members]
        taken = [(m, relative) for m, relative in taken if relative is not None]
        paths = [relative.as_posix() for _, relative in taken]
        if len(set(paths)) != len(paths):
            raise BuildError(f"{release.name} has the same payload file twice")
        try:
            for member, relative in taken:
                target = folder.joinpath(*relative.parts)
                target.parent.mkdir(parents=True, exist_ok=True)
                with archive.open(member) as source, target.open("wb") as stream:
                    shutil.copyfileobj(source, stream)
        except (OSError, zipfile.BadZipFile, zlib.error, EOFError, RuntimeError, ValueError) as error:
            raise BuildError(f"{release.name}: its payload cannot be unpacked ({error})") from error


def copy_payload(release, folder, cartridges):
    names = [p.relative_to(release).as_posix() for p in release.rglob("port.json")]
    prefix = find_payload(names)
    root = release / prefix
    files = [p for p in root.rglob("*") if p.is_file()]
    names = {prefix + p.relative_to(root).as_posix() for p in files}
    check_programs(release, names, prefix, cartridges)
    try:
        for path in files:
            relative = payload_part("", path.relative_to(root).as_posix())
            if relative is None:
                continue
            target = folder.joinpath(*relative.parts)
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(path, target)
    except OSError as error:
        raise BuildError(f"{release}: its payload cannot be copied ({error})") from error


def check_port_json(payload):
    """the release's port.json must be this repository's"""
    if (payload / "port.json").read_bytes() != (REPO / "port" / "n64" / "port.json").read_bytes():
        raise BuildError(f"the release's port.json differs from this repository's port/n64/port.json (version "
                         f"{VERSION}): use the Halo64RomBuilder.zip of the release whose tag is checked out "
                         f"(v{VERSION})")


def release_version(payload):
    """the release's version: its version.txt's (package_rombuilder.py
    writes it) or, in a release without one, the one KNOWN_RELEASES gives
    its multiplayer program; None if neither says"""
    stated = payload / VERSION_FILE
    if stated.is_file():
        return stated.read_text(encoding="utf-8", errors="replace").strip()
    program = payload / "elf" / "multiplayer" / "halo.elf.stripped"
    if program.is_file():
        return KNOWN_RELEASES.get(hashlib.sha256(program.read_bytes()).hexdigest())
    return None


def check_version(payload):
    """the release must be of the version checked out here (port.json alone
    did not change from v0.6.5 to v0.6.9; the converters did)"""
    version = release_version(payload)
    if version is None:
        raise BuildError(f"the release is older than this command or unknown (no version.txt, and not a release "
                         f"it knows): use the Halo64RomBuilder.zip of version {VERSION}, the one checked out here")
    if version != VERSION:
        raise BuildError(f"the release is version {version} and this checkout is version {VERSION}: use version "
                         f"{VERSION}'s Halo64RomBuilder.zip, or check out v{version} (git checkout v{version})")


# ---------- processes: each in a group of its own, stopped whole

def group_alive(group):
    try:
        os.killpg(group, 0)
    except ProcessLookupError:
        return False
    except PermissionError:
        return True
    return True


@contextlib.contextmanager
def signals_ignored():
    """Ctrl-C, SIGTERM and SIGHUP ignored meanwhile: a second Ctrl-C does not
    cut a stop or a cleaning up short (the main thread only can)"""
    try:
        previous = {number: signal.signal(number, signal.SIG_IGN)
                    for number in (signal.SIGINT, signal.SIGTERM, signal.SIGHUP)}
    except ValueError:
        previous = {}
    try:
        yield
    finally:
        for number, handler in previous.items():
            signal.signal(number, handler)


def stop_group(process, grace=None):
    """a process and every process it started (its group): SIGTERM, then
    SIGKILL for what is left after GRACE seconds"""
    with signals_ignored():
        for number in (signal.SIGTERM, signal.SIGKILL):
            try:
                os.killpg(process.pid, number)
            except ProcessLookupError:
                break
            deadline = time.monotonic() + (GRACE if grace is None else grace)
            while time.monotonic() < deadline and group_alive(process.pid):
                process.poll()
                time.sleep(0.1)
            if not group_alive(process.pid):
                break
        process.poll()


def run_group(command, **options):
    """a command in a process group of its own, its exit status: on any
    exception (Ctrl-C, Stopped) the whole group is stopped first. (The
    terminal's Ctrl-C reaches this process alone, which stops the group)"""
    process = subprocess.Popen([str(c) for c in command], start_new_session=True, **options)
    try:
        return process.wait()
    except BaseException:
        stop_group(process)
        raise


# ---------- the tools

def compile_jobs(limit=None):
    """compilers at a time: JOBS, else one for each CPU (limit at most)"""
    value = os.environ.get("JOBS")
    if value:
        if not value.isdigit() or int(value) < 1:
            raise BuildError(f"JOBS must be a whole number of 1 or more (it is {value!r})")
        return int(value)
    cpus = os.cpu_count() or 1
    return min(cpus, limit) if limit else cpus


def tool_environment(jobs):
    """the environment the tools are built in: TOOL_FLAGS added to CFLAGS and
    CXXFLAGS (libdragon's tools/Makefile and CMake add theirs to these)"""
    env = dict(os.environ, JOBS=str(jobs))
    for name in ("CFLAGS", "CXXFLAGS"):
        env[name] = f"{env.get(name, '')} {TOOL_FLAGS}".strip()
    return env


def run(command, log, what, **options):
    """a build command in a group of its own, its output in a log; on
    failure the log's end, and a hint when a compiler was killed"""
    with log.open("ab") as stream:
        code = run_group(command, stdout=stream, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL, **options)
    if code != 0:
        tail = log.read_text(encoding="utf-8", errors="replace").splitlines()[-30:]
        killed = code in (-signal.SIGKILL, 128 + signal.SIGKILL) or any(re.search(r"\bKilled\b", line) for line in tail)
        hint = ("\n(a compiler was killed, most likely for lack of memory: run again with fewer at a time, "
                "JOBS=1)") if killed else ""
        raise BuildError(f"{what} failed (code {code}):\n" + "\n".join(tail) + hint)


def fetch(url, commit, folder, log):
    """one commit of a git repository, without its history"""
    run(["git", "-c", "init.defaultBranch=main", "init", "-q", folder], log, f"git init {folder}")
    run(["git", "-C", folder, "fetch", "-q", "--depth", "1", url, commit], log, f"fetching {url} at {commit}")
    run(["git", "-C", folder, "-c", "advice.detachedHead=false", "checkout", "-q", "FETCH_HEAD"], log,
        f"checking out {commit}")


def files_digest(paths):
    digest = hashlib.sha256()
    for path in paths:
        digest.update(path.relative_to(REPO).as_posix().encode() + b"\0" + path.read_bytes() + b"\0")
    return digest.hexdigest()[:16]


def stamp(tool, **details):
    """what a tools folder's tools are built from: the sources, the machine,
    the compilers and their flags"""
    return {"tool": tool, **details, "flags": TOOL_FLAGS, "system": platform.system(), "machine": platform.machine(),
            **{name: os.environ.get(name, "") for name in ("CC", "CXX", "CFLAGS", "CXXFLAGS")}}


def libdragon_stamp():
    return stamp("libdragon", commit=LIBDRAGON_COMMIT,
                 patches=files_digest([REPO / patch for patch in LIBDRAGON_PATCHES]))


def invader_stamp():
    lite = REPO / INVADER_LITE
    files = sorted(p for p in lite.iterdir() if p.is_file() and p.name != "README.md")
    return stamp("invader-lite", commit=INVADER_COMMIT, build=files_digest(files))


def read_stamp(folder):
    try:
        return json.loads((folder / STAMP).read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return None


def tools_ready(folder, wanted, names):
    return read_stamp(folder) == wanted and all((folder / name).is_file() for name in names)


def own_folder(folder):
    """a tools folder emptied (or made) for this command's tools: one that
    holds anything without this command's stamp is never touched, and the
    stamp goes last (a folder emptied halfway is still this command's)"""
    if folder.is_symlink() or (folder.exists() and not folder.is_dir()):
        raise BuildError(f"{folder} is not a folder this command made: name another --tools folder")
    try:
        if folder.is_dir():
            if any(folder.iterdir()) and not (folder / STAMP).is_file():
                raise BuildError(f"{folder} holds files this command did not make (no {STAMP} in it): name "
                                 "another --tools folder, or move them away")
            for entry in folder.iterdir():
                if entry.name != STAMP:
                    remove_entry(entry)
        else:
            folder.mkdir(parents=True)
        # (a stamp which matches no build: an unfinished folder is this command's too)
        (folder / STAMP).write_text("{}\n", encoding="utf-8")
    except OSError as error:
        raise BuildError(f"{folder} cannot be made ready for the tools: {error}") from error


def remove_entry(path):
    if path.is_dir() and not path.is_symlink():
        shutil.rmtree(path)
    else:
        path.unlink()


def build_libdragon_tools(folder, work, jobs):
    """libdragon's tools (LIBDRAGON_TOOLS) in folder, built at LIBDRAGON_COMMIT
    with LIBDRAGON_PATCHES and TOOL_FLAGS"""
    wanted = libdragon_stamp()
    if tools_ready(folder, wanted, LIBDRAGON_TOOLS):
        print(f"libdragon's tools: {folder} (built before)", flush=True)
        return
    own_folder(folder)
    print(f"building libdragon's tools ({LIBDRAGON_COMMIT[:7]}, {jobs} at a time)", flush=True)
    source, log = work / "libdragon", work / "libdragon.log"
    fetch(LIBDRAGON_URL, LIBDRAGON_COMMIT, source, log)
    for patch in LIBDRAGON_PATCHES:
        run(["git", "-C", source, "apply", REPO / patch], log, f"applying {patch}")
    run(["make", "-C", source / "tools", f"-j{jobs}", *LIBDRAGON_TOOLS], log, "building libdragon's tools",
        env=tool_environment(jobs))
    for name, path in LIBDRAGON_TOOLS.items():
        shutil.copy2(source / "tools" / path, folder / name)
    (folder / STAMP).write_text(json.dumps(wanted, indent=1, sort_keys=True) + "\n", encoding="utf-8")


def build_ce_tools(folder, work, jobs):
    """invader-lite and ogg2wav (tools/n64/invader_lite/build.sh) in folder,
    built at INVADER_COMMIT with TOOL_FLAGS"""
    from .ce_prepare import INVADER_TOOLS
    names = [f"invader-{name}" for name in INVADER_TOOLS] + ["ogg2wav"]
    wanted = invader_stamp()
    if tools_ready(folder, wanted, names):
        print(f"invader-lite: {folder} (built before)", flush=True)
        return
    own_folder(folder)
    print(f"building invader-lite ({INVADER_COMMIT[:7]}, {jobs} at a time; a few minutes)", flush=True)
    source, log = work / "invader", work / "invader-lite.log"
    fetch(INVADER_URL, INVADER_COMMIT, source, log)
    build = work / "invader-lite"
    run(["bash", REPO / INVADER_LITE / "build.sh", source, build, "linux"], log, "building invader-lite",
        env=tool_environment(jobs))
    for path in (build / "dist").iterdir():
        shutil.copy2(path, folder / path.name)
    (folder / STAMP).write_text(json.dumps(wanted, indent=1, sort_keys=True) + "\n", encoding="utf-8")


# ---------- the build

def rombuilder_command(args, work):
    """tools/n64/rombuilder.py's command line for these arguments (paths
    made absolute: it runs from the repository)"""
    command = [sys.executable, "-m", "tools.n64.rombuilder", str(args.source.resolve()),
               "-o", str(args.output.resolve()), "--work", str(work)]
    command += [f"--{name}" for name in ("multiplayer", "campaign") if getattr(args, name)]
    if args.custom_edition:
        command += ["--custom-edition", *[str(path.resolve()) for path in args.custom_edition]]
    if args.ce_tools:
        command += ["--ce-tools", str(args.ce_tools.resolve())]
    if args.jobs is not None:
        command += ["--jobs", str(args.jobs)]
    if args.font:
        command += ["--font", str(args.font.resolve())]
    return command


def keep_logs(work, output):
    """a failed build's logs (the tools' builds, the converters'; no game
    data) copied to a new folder in the output folder: its path, or None"""
    logs = sorted(work.glob("*.log")) + sorted((work / "invader-lite" / "build").glob("*.log")) + \
        sorted((work / "build" / "logs").glob("*.log"))
    if not logs:
        return None
    folder = output / time.strftime("rombuilder-logs-%Y%m%d-%H%M%S")
    try:
        folder.mkdir(parents=True, exist_ok=True)
        for path in logs:
            shutil.copyfile(path, folder / "-".join(path.relative_to(work).parts))
    except OSError as error:
        print(f"the logs could not be kept in {folder}: {error}", file=sys.stderr)
        return None
    return folder


def remove_work(work):
    """the work folder removed; what cannot be is named"""
    failed = []
    shutil.rmtree(work, onexc=lambda _function, path, error: failed.append(f"{path}: {error}"))
    if failed or work.exists():
        print(f"could not remove all of {work} (remove it yourself): " + "; ".join(failed[:3]), file=sys.stderr)


def build(args):
    cartridges = chosen_cartridges(args.multiplayer, args.campaign, bool(args.custom_edition))
    ce_tools_built = builds_ce_tools(cartridges, args.ce_tools)
    missing = missing_prerequisites(ce_tools_built)
    if missing:
        hint = INSTALL_HINTS.get("linux" if sys.platform.startswith("linux") else sys.platform, "")
        raise BuildError("missing: " + "; ".join(missing) + (f" ({hint})" if hint else ""))
    if not args.release.exists():
        raise BuildError(f"no release at {args.release} (Halo64RomBuilder.zip from the v{VERSION} release, or "
                         "its folder)")
    if not args.source.exists():
        raise BuildError(f"no disc image or maps folder at {args.source}")
    if args.tools and args.tools.exists() and not args.tools.is_dir():
        raise BuildError(f"--tools {args.tools} is a file: name a folder")
    jobs = compile_jobs()
    work = Path(tempfile.mkdtemp(prefix="halo64-rombuilder-"))
    failed = False
    try:
        payload = work / "payload"
        extract_payload(args.release, payload, cartridges)
        check_version(payload)
        check_port_json(payload)
        print(f"{args.release.name}: version {VERSION}'s programs ({', '.join(cartridges)})", flush=True)
        tools = args.tools.resolve() if args.tools else work / "tools"
        bin_folder = tools / TOOL_FOLDERS["libdragon"]
        build_libdragon_tools(bin_folder, work, jobs)
        shutil.copytree(bin_folder, payload / "bin", ignore=shutil.ignore_patterns(STAMP))
        if ce_tools_built:
            ce_folder = tools / TOOL_FOLDERS["invader-lite"]
            build_ce_tools(ce_folder, work, compile_jobs(INVADER_JOBS))
            shutil.copytree(ce_folder, payload / "ce-tools", ignore=shutil.ignore_patterns(STAMP))
        command = rombuilder_command(args, work / "build")
        code = run_group(command, cwd=REPO, env=dict(os.environ, HALO64_PAYLOAD=str(payload)))
        failed = code != 0
        return code
    except BuildError:
        failed = True
        raise
    finally:
        with signals_ignored():
            kept = keep_logs(work, args.output.resolve()) if failed else None
            if kept:
                print(f"the logs of this build: {kept}", file=sys.stderr)
            remove_work(work)


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--release", type=Path, required=True,
                        help="the release's Halo64RomBuilder.zip, or its folder unpacked (the version checked out "
                        "here)")
    parser.add_argument("source", type=Path, help="Halo's Xbox disc image, or its maps folder")
    parser.add_argument("-o", "--output", type=Path, required=True)
    parser.add_argument("--multiplayer", action="store_true")
    parser.add_argument("--campaign", action="store_true")
    parser.add_argument("--custom-edition", type=Path, nargs="+", metavar="MAP",
                        help="a cartridge of these Halo Custom Edition maps (.map, or .zip files of them)")
    parser.add_argument("--ce-tools", type=Path,
                        help="Invader's tools and ogg2wav for the Custom Edition maps (default: invader-lite, "
                        "built here; Linux)")
    parser.add_argument("--jobs", type=int, help="conversions at a time")
    parser.add_argument("--font", type=Path)
    parser.add_argument("--tools", type=Path,
                        help="keep the tools built here in this folder (in folders of their own), for the next run")
    args = parser.parse_args()

    def stop(number, _frame):
        raise Stopped(number)

    for number in (signal.SIGTERM, signal.SIGHUP):
        # (not where the caller ignores it: nohup's SIGHUP)
        if signal.getsignal(number) is not signal.SIG_IGN:
            signal.signal(number, stop)
    try:
        code = build(args)
    except BuildError as error:
        print(f"error: {error}", file=sys.stderr)
        code = 1
    except KeyboardInterrupt:
        print("stopped (Ctrl-C): the work folder is removed", file=sys.stderr)
        code = 128 + signal.SIGINT
    except Stopped as stopped:
        print(f"stopped ({signal.Signals(stopped.number).name}): the work folder is removed", file=sys.stderr)
        code = 128 + stopped.number
    sys.exit(code)


if __name__ == "__main__":
    main()
