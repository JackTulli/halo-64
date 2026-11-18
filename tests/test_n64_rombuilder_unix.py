"""The ROM builder on macOS and Linux (tools/n64/rombuilder_unix.py) without
the network, compilers, the release or game data: the release zip's payload,
the version check, the prerequisites, the tools folder and the ROM builder's
command line."""
import argparse
import os
from pathlib import Path
import sys
import tempfile
import unittest
import zipfile
from unittest.mock import patch

from tools.n64 import package_rombuilder as package
from tools.n64 import rombuilder_unix as unix
from tools.n64.rombuilder import BuildError

REPO = Path(__file__).resolve().parent.parent
PREFIX = "Halo64RomBuilder/_internal/payload/"
PORT = b'{"maps": ["bloodgulch"]}'


def release_zip(path, port=PORT, programs=("multiplayer", "campaign", "custom"), extra=()):
    """a release zip's layout, its files fixtures"""
    with zipfile.ZipFile(path, "w") as archive:
        archive.writestr("Halo64RomBuilder/Halo64RomBuilder.exe", b"MZ fixture")
        archive.writestr(PREFIX + "port.json", port)
        for program in programs:
            for name in ("halo.elf.stripped", "halo.elf.sym", "halo.msym", "hud64.dso"):
                archive.writestr(f"{PREFIX}elf/{program}/{name}", f"{program} {name} fixture")
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


class UnixBuilderTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.repo = self.root / "repo"
        (self.repo / "port" / "n64").mkdir(parents=True)
        (self.repo / "port" / "n64" / "port.json").write_bytes(PORT)

    def test_payload_without_the_windows_programs(self):
        release = release_zip(self.root / "Halo64RomBuilder.zip")
        payload = self.root / "payload"
        unix.extract_payload(release, payload, ["multiplayer", "campaign"])
        files = sorted(p.relative_to(payload).as_posix() for p in payload.rglob("*") if p.is_file())
        self.assertIn("port.json", files)
        self.assertIn("elf/campaign/halo.elf.stripped", files)
        self.assertIn("elf/custom/hud64.dso", files)
        self.assertIn("invader/src/tag/hek/definition/actor.json", files)
        self.assertIn("art/hud64/c000.bin", files)
        self.assertFalse([f for f in files if f.startswith(("bin/", "ce-tools/", "online-kit/")) or f.endswith(".exe")])

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
        with self.assertRaisesRegex(BuildError, "not a Halo 64 ROM Builder release zip"):
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

    def test_the_release_must_be_the_checked_out_version(self):
        payload = self.root / "payload"
        payload.mkdir()
        (payload / "port.json").write_bytes(PORT)
        with patch.object(unix, "REPO", self.repo):
            unix.check_port_json(payload)
            (payload / "port.json").write_bytes(PORT + b"\n")
            with self.assertRaisesRegex(BuildError, r"port.json differs .* git checkout v"):
                unix.check_port_json(payload)

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

        with patch.object(unix.shutil, "which", side_effect=which), \
                patch.dict(os.environ, {"CC": "clang-99"}):
            self.assertIn("a C compiler (clang-99)", unix.missing_prerequisites(False))
            self.assertNotIn("cmake", unix.missing_prerequisites(False))
            self.assertIn("cmake", unix.missing_prerequisites(True))
        with patch.object(unix.shutil, "which", return_value="/usr/bin/tool"), \
                patch.object(unix.sys, "version_info", (3, 11, 9)):
            self.assertTrue(any(m.startswith("Python 3.12 or later") for m in unix.missing_prerequisites(False)))

    def test_the_tools_are_the_ones_the_windows_package_carries(self):
        self.assertEqual(set(unix.LIBDRAGON_TOOLS), set(package.TOOLS))

    def test_the_commits_are_the_ones_the_repository_names(self):
        lite = REPO / "tools" / "n64" / "invader_lite"
        self.assertIn(unix.INVADER_COMMIT, (lite / "README.md").read_text(encoding="utf-8"))
        self.assertIn(unix.INVADER_COMMIT, (lite / "NOTICE").read_text(encoding="utf-8"))
        provenance = REPO / "port" / "n64" / "docs" / "pc-expansion" / "PROVENANCE.md"
        self.assertIn(f"base `{unix.LIBDRAGON_COMMIT}`", provenance.read_text(encoding="utf-8"))

    def test_tools_built_before_are_reused_and_others_rebuilt(self):
        folder = self.root / "tools" / "bin"
        folder.mkdir(parents=True)
        for name in unix.LIBDRAGON_TOOLS:
            (folder / name).write_bytes(b"tool fixture")
        (folder / unix.STAMP).write_text(unix.LIBDRAGON_COMMIT + "\n")
        with patch.object(unix, "fetch", side_effect=AssertionError("fetched again")):
            unix.build_libdragon_tools(folder, self.root)
        (folder / unix.STAMP).write_text("0" * 40 + "\n")
        made = []

        def make(command, log, what, **options):
            if command[0] == "make":
                tools = Path(command[2])
                for path in unix.LIBDRAGON_TOOLS.values():
                    (tools / path).parent.mkdir(parents=True, exist_ok=True)
                    (tools / path).write_bytes(b"new tool fixture")
                made.append(command)

        with patch.object(unix, "fetch") as fetch, patch.object(unix, "run", side_effect=make):
            unix.build_libdragon_tools(folder, self.root)
        fetch.assert_called_once_with(unix.LIBDRAGON_URL, unix.LIBDRAGON_COMMIT, self.root / "libdragon",
                                      self.root / "libdragon.log")
        self.assertEqual(made[0][4:], list(unix.LIBDRAGON_TOOLS))
        self.assertEqual((folder / "mkdfs").read_bytes(), b"new tool fixture")
        self.assertEqual((folder / unix.STAMP).read_text().strip(), unix.LIBDRAGON_COMMIT)

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

    def test_build_runs_the_rom_builder_with_the_payload_and_removes_its_work(self):
        release_zip(self.root / "Halo64RomBuilder.zip")
        (self.root / "disc.iso").write_bytes(b"disc fixture")
        work = self.root / "work"
        seen = {}

        def tools(folder, _work):
            folder.mkdir(parents=True)
            for name in unix.LIBDRAGON_TOOLS:
                (folder / name).write_bytes(b"tool fixture")
            (folder / unix.STAMP).write_text(unix.LIBDRAGON_COMMIT)

        def rombuilder(command, cwd, env):
            payload = Path(env["HALO64_PAYLOAD"])
            seen["command"], seen["cwd"] = command, cwd
            seen["files"] = sorted(p.relative_to(payload).as_posix() for p in payload.rglob("*") if p.is_file())
            return unix.subprocess.CompletedProcess(command, 3)

        with patch.object(unix, "REPO", self.repo), \
                patch.object(unix, "missing_prerequisites", return_value=[]), \
                patch.object(unix.tempfile, "mkdtemp", side_effect=lambda **_: (work.mkdir(), str(work))[1]), \
                patch.object(unix, "build_libdragon_tools", side_effect=tools), \
                patch.object(unix, "build_ce_tools", side_effect=AssertionError("no Custom Edition maps")), \
                patch.object(unix.subprocess, "run", side_effect=rombuilder):
            code = unix.build(arguments(self.root))
        self.assertEqual(code, 3)
        self.assertEqual(seen["cwd"], self.repo)
        self.assertIn(str(work / "build"), seen["command"])
        self.assertIn("bin/mkdfs", seen["files"])
        self.assertIn("elf/multiplayer/halo.elf.sym", seen["files"])
        self.assertNotIn("bin/" + unix.STAMP, seen["files"])
        self.assertFalse(work.exists())


if __name__ == "__main__":
    unittest.main()
