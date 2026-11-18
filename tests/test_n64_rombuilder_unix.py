"""The ROM builder on macOS and Linux (tools/n64/rombuilder_unix.py) without
the network, the release or game data: the release's payload, the version
checks, the prerequisites, the tools folders and their stamps, the
compilers at a time, stopping the processes, the logs of a failed build and
the ROM builder's command line."""
import argparse
import io
import json
import os
from pathlib import Path
import re
import signal
import struct
import subprocess
import sys
import tempfile
import time
import unittest
import zipfile
from unittest.mock import patch

from tools.n64 import package_rombuilder as package
from tools.n64 import rombuilder_unix as unix
from tools.n64.rombuilder import BuildError
from tools.n64.version import VERSION

REPO = Path(__file__).resolve().parent.parent
PREFIX = "Halo64RomBuilder/_internal/payload/"
PORT = b'{"maps": ["bloodgulch"]}'


def lz4(sequences, last):
    """an LZ4 block: sequences of (literals, offset, match length), then the
    last literals"""
    def length(value):
        out = b""
        if value >= 15:
            value -= 15
            while value >= 255:
                out += b"\xff"
                value -= 255
            out += bytes((value,))
        return out
    block = b""
    for literals, offset, match in sequences:
        block += bytes((min(len(literals), 15) << 4 | min(match - 4, 15),)) + length(len(literals)) + literals
        block += struct.pack("<H", offset) + length(match - 4)
    return block + bytes((min(len(last), 15) << 4,)) + length(len(last)) + last


def program(segments):
    """a big-endian 32-bit ELF of loadable segments: [(bytes, flags)]"""
    header_size, entry = 0x34, 0x20
    data = bytearray(header_size + entry * len(segments))
    data[:6] = b"\x7fELF\x01\x02"
    struct.pack_into(">I", data, 0x1C, header_size)
    struct.pack_into(">HH", data, 0x2A, entry, len(segments))
    for i, (body, flags) in enumerate(segments):
        struct.pack_into(">8I", data, header_size + i * entry, 1, len(data), 0, 0, len(body), len(body), flags, 16)
        data += body
    return bytes(data)


def shows(version):
    """a compressed program whose title page shows this version"""
    text = b"Multiplayer\0Firefight\0V" + version.encode() + b"\0"
    return program([(lz4([(b"xyzxyzxyz", 3, 9)], text), unix.PF_N64_COMPRESSED)])


def release_zip(path, port=PORT, programs=("multiplayer", "campaign", "custom"), extra=(), version=VERSION,
                version_txt=None):
    """a release zip's layout: its programs show version on their title page"""
    with zipfile.ZipFile(path, "w") as archive:
        archive.writestr("Halo64RomBuilder/Halo64RomBuilder.exe", b"MZ fixture")
        archive.writestr(PREFIX + "port.json", port)
        if version_txt is not None:
            archive.writestr(PREFIX + "version.txt", version_txt)
        for name in programs:
            archive.writestr(f"{PREFIX}elf/{name}/halo.elf.stripped", shows(version))
            for other in ("halo.elf.sym", "halo.msym", "hud64.dso"):
                archive.writestr(f"{PREFIX}elf/{name}/{other}", f"{name} {other} fixture")
        archive.writestr(PREFIX + "invader/src/tag/hek/definition/actor.json", "{}")
        archive.writestr(PREFIX + "art/hud64/c000.bin", b"art fixture")
        archive.writestr(PREFIX + "bin/mkdfs.exe", b"MZ fixture")
        archive.writestr(PREFIX + "ce-tools/invader-build.exe", b"MZ fixture")
        archive.writestr(PREFIX + "online-kit/play-sc64.bat", b"fixture")
        for name, data in extra:
            archive.writestr(name, data)
    return path


def arguments(root, **values):
    args = argparse.Namespace(release=root / "Halo64RomBuilder.zip", source=root / "disc.iso", output=root / "roms",
                              multiplayer=False, campaign=False, custom_edition=None, ce_tools=None, jobs=None,
                              font=None, tools=None)
    for key, value in values.items():
        setattr(args, key, value)
    return args


def files(folder):
    return sorted(p.relative_to(folder).as_posix() for p in folder.rglob("*") if p.is_file())


class UnixBuilderTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.repo = self.root / "repo"
        (self.repo / "port" / "n64").mkdir(parents=True)
        (self.repo / "port" / "n64" / "port.json").write_bytes(PORT)
        self.addCleanup(patch.stopall)

    # ---------- the release's payload

    def test_payload_without_the_windows_programs(self):
        release = release_zip(self.root / "Halo64RomBuilder.zip", version_txt="x")
        payload = self.root / "payload"
        unix.extract_payload(release, payload, ["multiplayer", "campaign"])
        found = files(payload)
        for name in ("port.json", "version.txt", "elf/campaign/halo.elf.stripped", "elf/custom/hud64.dso",
                     "invader/src/tag/hek/definition/actor.json", "art/hud64/c000.bin"):
            self.assertIn(name, found)
        self.assertFalse([f for f in found if f.startswith(("bin/", "ce-tools/", "online-kit/")) or f.endswith(".exe")])

    def test_the_release_unpacked_in_a_folder(self):
        release = release_zip(self.root / "Halo64RomBuilder.zip")
        folder = self.root / "Downloads" / "Halo64RomBuilder"
        with zipfile.ZipFile(release) as archive:
            archive.extractall(folder.parent)
        unix.extract_payload(release, self.root / "from-zip", ["multiplayer"])
        unix.extract_payload(folder, self.root / "from-folder", ["multiplayer"])
        self.assertEqual(files(self.root / "from-zip"), files(self.root / "from-folder"))
        unix.extract_payload(folder.parent, self.root / "from-downloads", ["multiplayer"])
        self.assertEqual(files(self.root / "from-zip"), files(self.root / "from-downloads"))
        with self.assertRaisesRegex(BuildError, "no online program"):
            unix.extract_payload(folder, self.root / "other", ["online"])

    def test_a_missing_program_is_named_before_anything_is_extracted(self):
        release = release_zip(self.root / "Halo64RomBuilder.zip", programs=("multiplayer",))
        payload = self.root / "payload"
        with self.assertRaisesRegex(BuildError, r"no campaign program \(elf/campaign/halo.elf.stripped\)"):
            unix.extract_payload(release, payload, ["multiplayer", "campaign"])
        self.assertFalse(payload.exists())

    def test_not_a_release_zip(self):
        other = self.root / "other.zip"
        with zipfile.ZipFile(other, "w") as archive:
            archive.writestr("readme.txt", "fixture")
        with self.assertRaisesRegex(BuildError, "not a Halo 64 ROM Builder release"):
            unix.extract_payload(other, self.root / "payload", ["multiplayer"])
        broken = self.root / "broken.zip"
        broken.write_bytes(b"not a zip")
        with self.assertRaisesRegex(BuildError, "not a readable .zip file"):
            unix.extract_payload(broken, self.root / "payload", ["multiplayer"])

    def test_a_member_outside_the_payload_folder_is_refused(self):
        release = release_zip(self.root / "Halo64RomBuilder.zip", extra=[(PREFIX + "art/../../escaped.txt", "x")])
        with self.assertRaisesRegex(BuildError, "unsafe file name"):
            unix.extract_payload(release, self.root / "work" / "payload", ["multiplayer"])
        self.assertFalse(list(self.root.rglob("escaped.txt")))

    def test_conflicting_or_repeated_members_are_a_build_error(self):
        conflicting = release_zip(self.root / "conflicting.zip", extra=[(PREFIX + "art/hud64/c000.bin/x", "x")])
        with self.assertRaisesRegex(BuildError, "cannot be unpacked"):
            unix.extract_payload(conflicting, self.root / "a", ["multiplayer"])
        repeated = release_zip(self.root / "repeated.zip")
        with zipfile.ZipFile(repeated, "a") as archive, self.assertWarns(UserWarning):
            archive.writestr(PREFIX + "port.json", b"{}")
        with self.assertRaisesRegex(BuildError, "the same payload file twice"):
            unix.extract_payload(repeated, self.root / "b", ["multiplayer"])

    # ---------- the version

    def test_lz4_blocks_as_n64elfcompress_writes_them(self):
        literals = bytes(range(40))
        block = lz4([(literals, 40, 300), (b"ab", 2, 7)], b"the end")
        self.assertEqual(unix.lz4_block(block), literals * 8 + literals[:20] + b"ab" + b"abababa" + b"the end")
        with self.assertRaisesRegex(BuildError, "outside its data"):
            unix.lz4_block(lz4([(b"ab", 3, 4)], b""))
        with self.assertRaisesRegex(BuildError, "cut short"):
            unix.lz4_block(block[:-12])

    def test_the_version_a_program_shows_on_its_title_page(self):
        path = self.root / "halo.elf.stripped"
        # (the version made whole only by decompressing: "9\0V0.6." then 4 bytes from 7 back)
        compressed = lz4([(b"9\0V0.6.", 7, 4)], b"end!!")
        self.assertNotIn(b"V0.6.9\0", compressed)
        path.write_bytes(program([(b"V9.9.99\0 V1.2 V", 0), (compressed, unix.PF_N64_COMPRESSED)]))
        self.assertEqual(unix.program_versions(path), {"9.9.99", "0.6.9"})
        path.write_bytes(shows("0.6.8"))
        self.assertEqual(unix.program_versions(path), {"0.6.8"})
        path.write_bytes(b"MZ not an ELF" + bytes(80))
        with self.assertRaisesRegex(BuildError, "not a big-endian ELF"):
            unix.program_versions(path)

    def test_the_release_must_be_of_this_version(self):
        payload = self.root / "payload"
        program_path = payload / "elf" / "multiplayer" / "halo.elf.stripped"
        program_path.parent.mkdir(parents=True)
        program_path.write_bytes(shows(VERSION))
        unix.check_version(payload, ["multiplayer"])
        program_path.write_bytes(shows("0.6.8"))
        with self.assertRaisesRegex(BuildError, rf"the release is version 0\.6\.8 and this checkout is version "
                                                rf"{VERSION}.*git checkout v0\.6\.8"):
            unix.check_version(payload, ["multiplayer"])
        program_path.write_bytes(program([(b"no version", 0)]))
        with self.assertRaisesRegex(BuildError, "shows no version"):
            unix.check_version(payload, ["multiplayer"])
        # (a release's version.txt comes first)
        (payload / "version.txt").write_text(VERSION + "\n")
        unix.check_version(payload, ["multiplayer"])
        (payload / "version.txt").write_text("0.7.0\n")
        program_path.write_bytes(shows(VERSION))
        with self.assertRaisesRegex(BuildError, "the release is version 0.7.0"):
            unix.check_version(payload, ["multiplayer"])

    def test_the_release_port_json_must_be_the_repositorys(self):
        payload = self.root / "payload"
        payload.mkdir()
        (payload / "port.json").write_bytes(PORT)
        with patch.object(unix, "REPO", self.repo):
            unix.check_port_json(payload)
            (payload / "port.json").write_bytes(PORT + b"\n")
            with self.assertRaisesRegex(BuildError, r"port.json differs"):
                unix.check_port_json(payload)

    def test_build_stops_at_another_versions_release_before_any_tool(self):
        release_zip(self.root / "Halo64RomBuilder.zip", version="0.6.8")
        (self.root / "disc.iso").write_bytes(b"disc fixture")
        built = []
        patch.object(unix, "REPO", self.repo).start()
        patch.object(unix, "missing_prerequisites", return_value=[]).start()
        patch.object(unix, "compile_jobs", return_value=2).start()
        patch.object(unix, "build_libdragon_tools", side_effect=lambda *a: built.append(a)).start()
        patch.object(unix, "run_group", side_effect=AssertionError("the ROM builder ran")).start()
        with patch.object(unix.tempfile, "tempdir", str(self.root)):
            with self.assertRaisesRegex(BuildError, "the release is version 0.6.8"):
                unix.build(arguments(self.root))
        self.assertEqual(built, [])
        self.assertFalse(list(self.root.glob("halo64-rombuilder-*")))

    # ---------- what the machine needs

    def test_cartridges_as_the_rom_builder_chooses_them(self):
        self.assertEqual(unix.chosen_cartridges(), ["multiplayer", "campaign"])
        self.assertEqual(unix.chosen_cartridges(campaign=True), ["campaign"])
        self.assertEqual(unix.chosen_cartridges(custom=True), ["custom"])
        self.assertEqual(unix.chosen_cartridges(multiplayer=True, custom=True), ["multiplayer", "custom"])

    def test_invader_lite_is_built_on_linux_only(self):
        self.assertTrue(unix.builds_ce_tools(["custom"], None, "linux"))
        self.assertFalse(unix.builds_ce_tools(["multiplayer", "campaign"], None, "darwin"))
        self.assertFalse(unix.builds_ce_tools(["custom"], Path("ce-tools"), "darwin"))
        with self.assertRaisesRegex(BuildError, "Linux only.*--ce-tools"):
            unix.builds_ce_tools(["custom"], None, "darwin")

    def test_missing_prerequisites_are_named(self):
        def which(name):
            return None if name in ("cmake", "clang-99") else "/usr/bin/" + name

        patch.object(unix, "missing_headers", return_value=[]).start()
        with patch.object(unix.shutil, "which", side_effect=which), patch.dict(os.environ, {"CC": "clang-99"}):
            self.assertIn("a C compiler (clang-99)", unix.missing_prerequisites(False, "linux"))
            self.assertNotIn("cmake", unix.missing_prerequisites(False, "linux"))
            self.assertIn("cmake", unix.missing_prerequisites(True, "linux"))
        with patch.object(unix.shutil, "which", return_value="/usr/bin/tool"), \
                patch.object(unix.sys, "version_info", (3, 11, 9)):
            self.assertTrue(any(m.startswith("Python 3.12 or later") for m in unix.missing_prerequisites(False, "linux")))

    def test_invader_lite_headers_are_checked(self):
        patch.object(unix.shutil, "which", return_value="/usr/bin/tool").start()
        patch.object(unix, "missing_headers", return_value=[("samplerate.h", "libsamplerate0-dev")]).start()
        self.assertIn("samplerate.h (libsamplerate0-dev)", unix.missing_prerequisites(True, "linux"))
        self.assertEqual(unix.missing_prerequisites(False, "linux"), [])
        results = {"zlib.h": 0, "samplerate.h": 1}

        def compile_only(command, input, capture_output):
            header = input.decode().split("<")[1].split(">")[0]
            return subprocess.CompletedProcess(command, results[header])

        with patch.object(unix.subprocess, "run", side_effect=compile_only):
            self.assertEqual(unix.missing_headers("cc"), [("samplerate.h", "libsamplerate0-dev")])

    def test_macos_without_its_developer_tools(self):
        patch.object(unix.shutil, "which", return_value="/usr/bin/git").start()
        with patch.object(unix, "developer_tools_missing", return_value=True):
            missing = unix.missing_prerequisites(False, "darwin")
        self.assertEqual(missing, ["the command line developer tools (xcode-select --install): git, make and the "
                                   "compilers"])
        with patch.object(unix, "developer_tools_missing", return_value=False):
            self.assertEqual(unix.missing_prerequisites(False, "darwin"), [])
        no_tools = subprocess.CompletedProcess(["xcode-select", "-p"], 2, "", "error: unable to get active "
                                                                              "developer directory")
        with patch.object(unix.subprocess, "run", return_value=no_tools):
            self.assertTrue(unix.developer_tools_missing())
        tools = subprocess.CompletedProcess(["xcode-select", "-p"], 0, str(self.root) + "\n", "")
        with patch.object(unix.subprocess, "run", return_value=tools):
            self.assertFalse(unix.developer_tools_missing())

    def test_pillow_hint_names_a_venv_on_macos(self):
        patch.object(unix.shutil, "which", return_value="/usr/bin/tool").start()
        patch.object(unix, "developer_tools_missing", return_value=False).start()
        with patch.dict(sys.modules, {"PIL": None}):
            missing = unix.missing_prerequisites(False, "darwin")
        self.assertEqual(len(missing), 1)
        self.assertIn("python3 -m venv ~/halo-64-venv && ~/halo-64-venv/bin/pip install pillow", missing[0])
        self.assertIn("venv", unix.INSTALL_HINTS["darwin"])

    def test_the_tools_are_the_ones_the_windows_package_carries(self):
        self.assertEqual(set(unix.LIBDRAGON_TOOLS), set(package.TOOLS))

    def test_the_libdragon_patches_are_in_the_repository(self):
        for patch_file in unix.LIBDRAGON_PATCHES:
            text = (REPO / patch_file).read_text(encoding="utf-8")
            self.assertTrue(text.startswith("diff --git a/tools/"), patch_file)
            self.assertIn("S_ISDIR(st.st_mode)", text)

    def test_the_commits_are_the_ones_the_repository_names(self):
        lite = REPO / "tools" / "n64" / "invader_lite"
        self.assertIn(unix.INVADER_COMMIT, (lite / "README.md").read_text(encoding="utf-8"))
        self.assertIn(unix.INVADER_COMMIT, (lite / "NOTICE").read_text(encoding="utf-8"))
        provenance = REPO / "port" / "n64" / "docs" / "pc-expansion" / "PROVENANCE.md"
        self.assertIn(f"base `{unix.LIBDRAGON_COMMIT}`", provenance.read_text(encoding="utf-8"))

    # ---------- the tools folders

    def make_tools(self, made):
        def make(command, log, what, **options):
            made.append(command)
            if command[0] == "make":
                tools = Path(command[2])
                for path in unix.LIBDRAGON_TOOLS.values():
                    (tools / path).parent.mkdir(parents=True, exist_ok=True)
                    (tools / path).write_bytes(b"new tool fixture")
        return make

    def test_tools_built_before_are_reused_and_others_rebuilt(self):
        folder = self.root / "tools" / "libdragon-tools"
        folder.mkdir(parents=True)
        for name in unix.LIBDRAGON_TOOLS:
            (folder / name).write_bytes(b"tool fixture")
        (folder / unix.STAMP).write_text(json.dumps(unix.libdragon_stamp()))
        with patch.object(unix, "fetch", side_effect=AssertionError("fetched again")):
            unix.build_libdragon_tools(folder, self.root, 2)
        # (another commit: built again)
        (folder / unix.STAMP).write_text(json.dumps(dict(unix.libdragon_stamp(), commit="0" * 40)))
        made = []
        with patch.object(unix, "fetch") as fetch, patch.object(unix, "run", side_effect=self.make_tools(made)):
            unix.build_libdragon_tools(folder, self.root, 2)
        fetch.assert_called_once_with(unix.LIBDRAGON_URL, unix.LIBDRAGON_COMMIT, self.root / "libdragon",
                                      self.root / "libdragon.log")
        self.assertEqual(made[:-1], [["git", "-C", self.root / "libdragon", "apply", REPO / patch_file]
                                     for patch_file in unix.LIBDRAGON_PATCHES])
        self.assertEqual(made[-1][4:], list(unix.LIBDRAGON_TOOLS))
        self.assertEqual((folder / "mkdfs").read_bytes(), b"new tool fixture")
        self.assertEqual(unix.read_stamp(folder), unix.libdragon_stamp())

    def test_a_changed_patch_builds_the_tools_again(self):
        repo = self.root / "repo"
        patch_file = repo / unix.LIBDRAGON_PATCHES[0]
        patch_file.parent.mkdir(parents=True)
        patch_file.write_text("diff --git a/tools/x b/tools/x\n-old\n+new\n")
        patch.object(unix, "REPO", repo).start()
        folder = self.root / "tools" / "libdragon-tools"
        made = []
        patch.object(unix, "fetch").start()
        patch.object(unix, "run", side_effect=self.make_tools(made)).start()
        unix.build_libdragon_tools(folder, self.root, 2)
        self.assertEqual(len(made), 2)
        unix.build_libdragon_tools(folder, self.root, 2)
        self.assertEqual(len(made), 2)
        patch_file.write_text("diff --git a/tools/x b/tools/x\n-old\n+newer\n")
        unix.build_libdragon_tools(folder, self.root, 2)
        self.assertEqual(len(made), 4)

    def test_stamps_name_the_machine_and_the_compilers(self):
        with patch.dict(os.environ, {"CC": "clang", "CXX": "clang++", "CFLAGS": "-O3", "CXXFLAGS": "-O2"}):
            libdragon = unix.libdragon_stamp()
            invader = unix.invader_stamp()
        for wanted in (libdragon, invader):
            self.assertEqual(wanted["system"], unix.platform.system())
            self.assertEqual(wanted["machine"], unix.platform.machine())
            self.assertEqual((wanted["CC"], wanted["CXX"], wanted["CFLAGS"], wanted["CXXFLAGS"]),
                             ("clang", "clang++", "-O3", "-O2"))
            self.assertEqual(wanted["flags"], unix.TOOL_FLAGS)
        self.assertEqual(invader["commit"], unix.INVADER_COMMIT)
        lite = self.root / "repo" / "tools" / "n64" / "invader_lite"
        lite.mkdir(parents=True)
        (lite / "build.sh").write_text("one")
        (lite / "README.md").write_text("words")
        with patch.object(unix, "REPO", self.root / "repo"):
            first = unix.invader_stamp()
            (lite / "README.md").write_text("other words")
            self.assertEqual(unix.invader_stamp(), first)
            (lite / "build.sh").write_text("two")
            self.assertNotEqual(unix.invader_stamp(), first)

    def test_a_tools_folder_with_other_files_is_never_emptied(self):
        folder = self.root / "tools" / "libdragon-tools"
        folder.mkdir(parents=True)
        (folder / "notes.txt").write_text("the user's")
        patch.object(unix, "fetch", side_effect=AssertionError("fetched")).start()
        with self.assertRaisesRegex(BuildError, re.escape(f"{folder} holds files this command did not make")):
            unix.build_libdragon_tools(folder, self.root, 2)
        self.assertEqual((folder / "notes.txt").read_text(), "the user's")
        # (the user's own bin/ beside it is not this command's folder)
        own = self.root / "tools" / "bin"
        own.mkdir()
        (own / "mkdfs").write_text("the user's mkdfs")
        (folder / "notes.txt").unlink()
        made = []
        patch.object(unix, "fetch", return_value=None).start()
        patch.object(unix, "run", side_effect=self.make_tools(made)).start()
        unix.build_libdragon_tools(folder, self.root, 2)
        self.assertEqual((own / "mkdfs").read_text(), "the user's mkdfs")
        self.assertEqual(unix.read_stamp(folder), unix.libdragon_stamp())
        # (a folder of this command's, however old, is emptied)
        (folder / unix.STAMP).write_text("{}")
        (folder / "old-tool").write_text("old")
        unix.build_libdragon_tools(folder, self.root, 2)
        self.assertFalse((folder / "old-tool").exists())

    def test_tools_are_built_without_fused_multiply_add(self):
        with patch.dict(os.environ, {"CFLAGS": "-O3"}):
            os.environ.pop("CXXFLAGS", None)
            env = unix.tool_environment(5)
        self.assertEqual(unix.TOOL_FLAGS, "-ffp-contract=off")
        self.assertEqual(env["CFLAGS"], "-O3 -ffp-contract=off")
        self.assertEqual(env["CXXFLAGS"], "-ffp-contract=off")
        self.assertEqual(env["JOBS"], "5")
        made = []

        def make(command, log, what, **options):
            if command[0] == "make":
                self.assertIn(unix.TOOL_FLAGS, options["env"]["CFLAGS"])
                self.assertIn(unix.TOOL_FLAGS, options["env"]["CXXFLAGS"])
            self.make_tools(made)(command, log, what, **options)

        with patch.object(unix, "fetch"), patch.object(unix, "run", side_effect=make):
            unix.build_libdragon_tools(self.root / "tools" / "libdragon-tools", self.root, 3)
        self.assertIn("-j3", made[-1])

    # ---------- compilers at a time

    def test_compilers_at_a_time(self):
        cgroup = self.root / "cgroup"
        cgroup.mkdir()
        meminfo = self.root / "meminfo"
        meminfo.write_text("MemTotal: 33554432 kB\nMemAvailable: 16777216 kB\n")
        patch.object(unix, "CGROUP", cgroup).start()
        patch.object(unix, "MEMINFO", meminfo).start()
        patch.object(unix.os, "cpu_count", return_value=16).start()
        with patch.dict(os.environ, {}):
            os.environ.pop("JOBS", None)
            self.assertEqual(unix.compile_jobs(), 10)  # (16 GiB: 10 compilers of 1.5 GiB)
            (cgroup / "cpu.max").write_text("300000 100000\n")
            (cgroup / "memory.max").write_text(str(4 << 30) + "\n")
            (cgroup / "memory.current").write_text(str(1 << 29) + "\n")
            self.assertEqual(unix.compile_jobs(), 2)  # (3 CPUs, 3.5 GiB left in the container)
            (cgroup / "memory.max").write_text("max\n")
            self.assertEqual(unix.compile_jobs(), 3)
            (cgroup / "cpu.max").write_text("max 100000\n")
            meminfo.write_text("MemAvailable: 1048576 kB\n")
            self.assertEqual(unix.compile_jobs(), 1)
            os.environ["JOBS"] = "7"
            self.assertEqual(unix.compile_jobs(), 7)
            os.environ["JOBS"] = "lots"
            with self.assertRaisesRegex(BuildError, "JOBS must be a whole number"):
                unix.compile_jobs()

    def test_the_memory_hint_only_when_a_compiler_was_killed(self):
        log = self.root / "build.log"
        killed = [sys.executable, "-c", "print('c++: fatal error: Killed signal terminated program cc1plus'); "
                                        "raise SystemExit(1)"]
        with self.assertRaises(BuildError) as context:
            unix.run(killed, log, "building invader-lite")
        self.assertIn("JOBS=1", str(context.exception))
        failed = [sys.executable, "-c", "print('error: no such file'); raise SystemExit(2)"]
        with self.assertRaises(BuildError) as context:
            unix.run(failed, self.root / "other.log", "building invader-lite")
        self.assertIn("no such file", str(context.exception))
        self.assertNotIn("JOBS=1", str(context.exception))

    # ---------- stopping

    def spawn(self):
        """a process in a group of its own that started another: (process, the other's pid)"""
        pid_file = self.root / "child.pid"
        code = ("import subprocess, sys, time; "
                "child = subprocess.Popen([sys.executable, '-c', 'import time; time.sleep(60)']); "
                f"open({str(pid_file)!r}, 'w').write(str(child.pid)); time.sleep(60)")
        return [sys.executable, "-c", code], pid_file

    def wait_for(self, path):
        deadline = time.monotonic() + 20
        while not (path.is_file() and path.read_text()) and time.monotonic() < deadline:
            time.sleep(0.05)
        return int(path.read_text())

    def test_a_stopped_group_leaves_no_process(self):
        command, pid_file = self.spawn()
        process = subprocess.Popen(command, start_new_session=True)
        child = self.wait_for(pid_file)
        self.assertTrue(unix.alive(child))
        unix.stop_group(process, grace=2)
        time.sleep(0.5)
        self.assertFalse(unix.alive(process.pid))
        self.assertFalse(unix.alive(child))

    def test_a_signal_while_waiting_stops_the_whole_group(self):
        command, pid_file = self.spawn()

        def stop(number, _frame):
            raise unix.Stopped(number)

        previous = signal.signal(signal.SIGALRM, stop)
        self.addCleanup(signal.signal, signal.SIGALRM, previous)
        signal.setitimer(signal.ITIMER_REAL, 3)
        self.addCleanup(signal.setitimer, signal.ITIMER_REAL, 0)
        started = time.monotonic()
        with self.assertRaises(unix.Stopped):
            unix.run_group(command)
        self.assertLess(time.monotonic() - started, 30)
        child = int(pid_file.read_text())
        time.sleep(0.5)
        self.assertFalse(unix.alive(child))

    def test_ctrl_c_and_signals_exit_without_a_traceback(self):
        for raised, code, word in ((KeyboardInterrupt(), 130, "Ctrl-C"), (unix.Stopped(signal.SIGTERM), 143, "SIGTERM"),
                                   (unix.Stopped(signal.SIGHUP), 129, "SIGHUP")):
            errors = io.StringIO()
            with patch.object(unix, "build", side_effect=raised), patch.object(sys, "stderr", errors), \
                    patch.object(sys, "argv", ["rombuilder_unix", "--release", "r.zip", "disc.iso", "-o", "out"]):
                with self.assertRaises(SystemExit) as context:
                    unix.main()
            self.assertEqual(context.exception.code, code)
            self.assertIn(word, errors.getvalue())
            self.assertNotIn("Traceback", errors.getvalue())
        for number in (signal.SIGTERM, signal.SIGHUP):
            signal.signal(number, signal.SIG_DFL)

    # ---------- the build

    def test_rom_builder_command_line(self):
        maps = [Path("maps") / "a.map", Path("maps") / "b.zip"]
        args = arguments(Path("relative"), multiplayer=True, custom_edition=maps, jobs=1, font=Path("Halo.ttf"))
        command = unix.rombuilder_command(args, self.root / "work")
        self.assertEqual(command[:3], [sys.executable, "-m", "tools.n64.rombuilder"])
        self.assertEqual(command[3:7], [str(Path("relative/disc.iso").resolve()), "-o",
                                        str(Path("relative/roms").resolve()), "--work"])
        self.assertEqual(command[8:], ["--multiplayer", "--custom-edition", *[str(m.resolve()) for m in maps],
                                       "--jobs", "1", "--font", str(Path("Halo.ttf").resolve())])
        self.assertNotIn("--campaign", unix.rombuilder_command(arguments(self.root), self.root / "work"))

    def built(self, rombuilder, tools=None):
        """build() with stubbed tools and ROM builder: (its result, what the ROM builder saw)"""
        release_zip(self.root / "Halo64RomBuilder.zip")
        (self.root / "disc.iso").write_bytes(b"disc fixture")
        seen = {}

        def libdragon(folder, work, jobs):
            folder.mkdir(parents=True, exist_ok=True)
            for name in unix.LIBDRAGON_TOOLS:
                (folder / name).write_bytes(b"tool fixture")
            (folder / unix.STAMP).write_text("{}")
            (work / "libdragon.log").write_text("make's output")

        def run_group(command, cwd, env):
            payload = Path(env["HALO64_PAYLOAD"])
            seen.update(command=command, cwd=cwd, work=payload.parent, files=files(payload))
            return rombuilder(payload.parent)

        patch.object(unix, "REPO", self.repo).start()
        patch.object(unix, "missing_prerequisites", return_value=[]).start()
        patch.object(unix, "compile_jobs", return_value=2).start()
        patch.object(unix, "build_libdragon_tools", side_effect=libdragon).start()
        patch.object(unix, "build_ce_tools", side_effect=AssertionError("no Custom Edition maps")).start()
        patch.object(unix, "run_group", side_effect=run_group).start()
        with patch.object(unix.tempfile, "tempdir", str(self.root)):
            return unix.build(arguments(self.root, tools=tools)), seen

    def test_build_runs_the_rom_builder_with_the_payload_and_removes_its_work(self):
        code, seen = self.built(lambda work: 0)
        self.assertEqual(code, 0)
        self.assertEqual(seen["cwd"], self.repo)
        self.assertIn(str(seen["work"] / "build"), seen["command"])
        self.assertIn("bin/mkdfs", seen["files"])
        self.assertIn("elf/multiplayer/halo.elf.sym", seen["files"])
        self.assertNotIn("bin/" + unix.STAMP, seen["files"])
        self.assertFalse(seen["work"].exists())
        self.assertFalse((self.root / "roms").exists())

    def test_a_failed_build_keeps_its_logs(self):
        def failing(work):
            logs = work / "build" / "logs"
            logs.mkdir(parents=True)
            (logs / "convert-123.log").write_text("Traceback: the converter's error")
            return 1

        code, seen = self.built(failing)
        self.assertEqual(code, 1)
        self.assertFalse(seen["work"].exists())
        kept = list((self.root / "roms").glob("rombuilder-logs-*"))
        self.assertEqual(len(kept), 1)
        self.assertEqual(files(kept[0]), ["build-logs-convert-123.log", "libdragon.log"])
        self.assertIn("the converter's error", (kept[0] / "build-logs-convert-123.log").read_text())

    def test_ctrl_c_during_the_build_removes_its_work_and_keeps_no_logs(self):
        def interrupted(work):
            (work / "build").mkdir()
            raise KeyboardInterrupt

        with self.assertRaises(KeyboardInterrupt):
            self.built(interrupted)
        self.assertFalse(list(self.root.glob("halo64-rombuilder-*")))
        self.assertFalse((self.root / "roms").exists())


if __name__ == "__main__":
    unittest.main()
