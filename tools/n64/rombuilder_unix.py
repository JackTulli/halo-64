"""Halo 64's ROM builder on macOS and Linux, with the release's programs.

The release's Halo64RomBuilder.zip carries each cartridge's prebuilt
program (its payload: tools/n64/package_rombuilder.py), the same for every
player; the tools beside them are Windows programs. This takes the zip's
programs, Invader's tag definitions, art and port.json, builds the tools
the ROM builder runs from their sources -- libdragon's mksprite,
audioconv64, mkdfs, n64tool, ed64romconfig and mkasset at LIBDRAGON_COMMIT,
and for Custom Edition maps invader-lite and ogg2wav at INVADER_COMMIT
(tools/n64/invader_lite/build.sh, Linux only) -- and runs
tools/n64/rombuilder.py with that payload (HALO64_PAYLOAD):

    python3 -m tools.n64.rombuilder_unix --release <Halo64RomBuilder.zip>
        <disc image or maps folder> -o <folder>
        [--multiplayer] [--campaign] [--custom-edition <CE map or .zip>...]
        [--ce-tools <folder>] [--jobs N] [--font <Halo.ttf>] [--tools <folder>]

The zip must be the release of the version checked out here: its
port.json must equal port/n64/port.json. The work files go in a new
folder in TMPDIR, removed at the end; only the ROMs stay (and, with
--tools, the tools built, which a later run with the same folder reuses).
JOBS sets how many compilers build the tools at a time (default: one per
CPU).
"""

import argparse
import os
import platform
import shlex
import shutil
import subprocess
import sys
import tempfile
import zipfile
from pathlib import Path, PurePosixPath

from .rombuilder import CARTRIDGES, BuildError
from .version import VERSION

REPO = Path(__file__).resolve().parent.parent.parent
# the libdragon this repository is built with (port/n64/docs/pc-expansion/
# PROVENANCE.md), and so the release's tools
LIBDRAGON_URL = "https://github.com/DragonMinded/libdragon"
LIBDRAGON_COMMIT = "39d0d6096130836a65710da6730290701d060272"
# the Invader that invader-lite is built from (tools/n64/invader_lite/README.md)
INVADER_URL = "https://github.com/SnowyMouse/invader"
INVADER_COMMIT = "696830ff80af227e84e7237c2ef26eb2301ed110"
# libdragon's tools the ROM builder runs (package_rombuilder.TOOLS), where
# libdragon's tools/Makefile makes them
LIBDRAGON_TOOLS = {"mksprite": "mksprite/mksprite", "audioconv64": "audioconv64/audioconv64",
                   "mkdfs": "mkdfs/mkdfs", "n64tool": "n64tool", "ed64romconfig": "ed64romconfig",
                   "mkasset": "mkasset/mkasset"}
# what is taken from the zip's payload (its bin/, ce-tools/ and online-kit/
# are Windows programs)
PAYLOAD_PARTS = ("port.json", "elf/", "invader/", "art/")
# a stamp in each tools folder: the commit its tools were built from
STAMP = "commit.txt"
INSTALL_HINTS = {
    "darwin": "xcode-select --install gives git, make and the C and C++ compilers; Python 3.12 or later with "
              "Pillow from python.org or Homebrew, then python3 -m pip install pillow",
    "linux": "on Debian or Ubuntu: sudo apt install build-essential git python3-pil, and for Custom Edition maps "
             "cmake ninja-build zlib1g-dev libsamplerate0-dev",
}


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


def missing_prerequisites(ce_tools_built):
    """what this machine lacks to build the tools and run the converters"""
    missing = []
    if sys.version_info < (3, 12):
        missing.append(f"Python 3.12 or later (this is {platform.python_version()})")
    try:
        import PIL  # noqa: F401 (the converters' images)
    except ImportError:
        missing.append("Pillow for this Python (python3 -m pip install pillow)")
    compilers = [os.environ.get("CC", "cc"), os.environ.get("CXX", "g++")]
    commands = [("git", "git"), ("make", "make"), (compilers[0], f"a C compiler ({compilers[0]})"),
                (compilers[1], f"a C++ compiler ({compilers[1]})")]
    if ce_tools_built:
        commands += [("cmake", "cmake"), ("ninja", "ninja"), ("bash", "bash")]
    for command, what in commands:
        words = shlex.split(command)
        if not words or not shutil.which(words[0]):
            missing.append(what)
    return missing


def payload_prefix(names):
    """the folder of the release zip's payload (<...>/_internal/payload/)"""
    found = {name[:-len("port.json")] for name in names if name.endswith("/payload/port.json")}
    if len(found) != 1:
        raise BuildError("this is not a Halo 64 ROM Builder release zip (Halo64RomBuilder.zip): "
                         + ("no payload/port.json in it" if not found else "more than one payload in it"))
    return found.pop()


def extract_payload(release, folder, cartridges):
    """the zip's payload without its Windows programs (PAYLOAD_PARTS) in
    folder, once the chosen cartridges' programs are found in it"""
    try:
        archive = zipfile.ZipFile(release)
    except (zipfile.BadZipFile, OSError) as error:
        raise BuildError(f"{release} is not a readable .zip file") from error
    with archive:
        names = set(archive.namelist())
        prefix = payload_prefix(names)
        for cartridge in cartridges:
            program = f"elf/{CARTRIDGES[cartridge]['elf']}/"
            for name in ("halo.elf.stripped", "halo.elf.sym"):
                if prefix + program + name not in names:
                    raise BuildError(f"{release.name} has no {cartridge} program ({program}{name})")
        for member in archive.infolist():
            part = member.filename[len(prefix):]
            if member.is_dir() or not member.filename.startswith(prefix) or not part.startswith(PAYLOAD_PARTS):
                continue
            relative = PurePosixPath(part)
            if relative.is_absolute() or ".." in relative.parts:
                raise BuildError(f"{release.name} has an unsafe file name: {member.filename}")
            target = folder.joinpath(*relative.parts)
            target.parent.mkdir(parents=True, exist_ok=True)
            with archive.open(member) as source, target.open("wb") as stream:
                shutil.copyfileobj(source, stream)


def check_port_json(payload):
    """the release's port.json must be this repository's: its programs and
    these converters are of the same version"""
    if (payload / "port.json").read_bytes() != (REPO / "port" / "n64" / "port.json").read_bytes():
        raise BuildError(f"the release zip is not of the version checked out here (version {VERSION}): its "
                         "port.json differs from port/n64/port.json. Use the Halo64RomBuilder.zip of the release "
                         f"whose tag is checked out (v{VERSION}: git checkout v{VERSION})")


def compile_jobs():
    return os.environ.get("JOBS") or str(os.cpu_count() or 2)


def run(command, log, what, **options):
    """a build command, its output in a log; on failure the log's end"""
    with log.open("ab") as stream:
        code = subprocess.run([str(c) for c in command], stdout=stream, stderr=subprocess.STDOUT,
                              stdin=subprocess.DEVNULL, **options).returncode
    if code != 0:
        tail = log.read_text(encoding="utf-8", errors="replace").splitlines()[-30:]
        raise BuildError(f"{what} failed (code {code}):\n" + "\n".join(tail))


def fetch(url, commit, folder, log):
    """one commit of a git repository, without its history"""
    run(["git", "-c", "init.defaultBranch=main", "init", "-q", folder], log, f"git init {folder}")
    run(["git", "-C", folder, "fetch", "-q", "--depth", "1", url, commit], log, f"fetching {url} at {commit}")
    run(["git", "-C", folder, "-c", "advice.detachedHead=false", "checkout", "-q", "FETCH_HEAD"], log,
        f"checking out {commit}")


def tools_ready(folder, commit, names):
    stamp = folder / STAMP
    return (stamp.is_file() and stamp.read_text(encoding="utf-8").strip() == commit
            and all((folder / name).is_file() for name in names))


def build_libdragon_tools(folder, work):
    """libdragon's tools (LIBDRAGON_TOOLS) in folder, built at LIBDRAGON_COMMIT"""
    if tools_ready(folder, LIBDRAGON_COMMIT, LIBDRAGON_TOOLS):
        print(f"libdragon's tools: {folder} (built before)", flush=True)
        return
    print(f"building libdragon's tools ({LIBDRAGON_COMMIT[:7]})", flush=True)
    source, log = work / "libdragon", work / "libdragon.log"
    fetch(LIBDRAGON_URL, LIBDRAGON_COMMIT, source, log)
    run(["make", "-C", source / "tools", f"-j{compile_jobs()}", *LIBDRAGON_TOOLS], log, "building libdragon's tools")
    shutil.rmtree(folder, ignore_errors=True)
    folder.mkdir(parents=True)
    for name, path in LIBDRAGON_TOOLS.items():
        shutil.copy2(source / "tools" / path, folder / name)
    (folder / STAMP).write_text(LIBDRAGON_COMMIT + "\n", encoding="utf-8")


def build_ce_tools(folder, work):
    """invader-lite and ogg2wav (tools/n64/invader_lite/build.sh) in folder,
    built at INVADER_COMMIT"""
    from .ce_prepare import INVADER_TOOLS
    names = [f"invader-{name}" for name in INVADER_TOOLS] + ["ogg2wav"]
    if tools_ready(folder, INVADER_COMMIT, names):
        print(f"invader-lite: {folder} (built before)", flush=True)
        return
    print(f"building invader-lite ({INVADER_COMMIT[:7]}; a few minutes)", flush=True)
    source, log = work / "invader", work / "invader-lite.log"
    fetch(INVADER_URL, INVADER_COMMIT, source, log)
    build = work / "invader-lite"
    run(["bash", REPO / "tools" / "n64" / "invader_lite" / "build.sh", source, build, "linux"], log,
        "building invader-lite", env=dict(os.environ, JOBS=compile_jobs()))
    shutil.rmtree(folder, ignore_errors=True)
    shutil.copytree(build / "dist", folder)
    (folder / STAMP).write_text(INVADER_COMMIT + "\n", encoding="utf-8")


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


def build(args):
    cartridges = chosen_cartridges(args.multiplayer, args.campaign, bool(args.custom_edition))
    ce_tools_built = builds_ce_tools(cartridges, args.ce_tools)
    missing = missing_prerequisites(ce_tools_built)
    if missing:
        hint = INSTALL_HINTS.get("linux" if sys.platform.startswith("linux") else sys.platform, "")
        raise BuildError("missing: " + "; ".join(missing) + (f" ({hint})" if hint else ""))
    if not args.release.is_file():
        raise BuildError(f"no release zip at {args.release} (Halo64RomBuilder.zip from the v{VERSION} release)")
    if not args.source.exists():
        raise BuildError(f"no disc image or maps folder at {args.source}")
    work = Path(tempfile.mkdtemp(prefix="halo64-rombuilder-"))
    try:
        payload = work / "payload"
        extract_payload(args.release, payload, cartridges)
        check_port_json(payload)
        print(f"{args.release.name}: the v{VERSION} release's programs ({', '.join(cartridges)})", flush=True)
        tools = args.tools.resolve() if args.tools else work / "tools"
        build_libdragon_tools(tools / "bin", work)
        shutil.copytree(tools / "bin", payload / "bin", ignore=shutil.ignore_patterns(STAMP))
        if ce_tools_built:
            build_ce_tools(tools / "ce-tools", work)
            shutil.copytree(tools / "ce-tools", payload / "ce-tools", ignore=shutil.ignore_patterns(STAMP))
        command = rombuilder_command(args, work / "build")
        return subprocess.run(command, cwd=REPO, env=dict(os.environ, HALO64_PAYLOAD=str(payload))).returncode
    finally:
        shutil.rmtree(work, ignore_errors=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--release", type=Path, required=True,
                        help="the release's Halo64RomBuilder.zip (the version checked out here)")
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
    parser.add_argument("--tools", type=Path, help="keep the tools built here in this folder, for the next run")
    args = parser.parse_args()
    try:
        code = build(args)
    except BuildError as error:
        print(f"error: {error}", file=sys.stderr)
        code = 1
    sys.exit(code)


if __name__ == "__main__":
    main()
