"""Package recipe smoke tests; PyInstaller and all native binaries are stubbed."""
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

from tools.n64 import package_rombuilder as package


class PackageTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.repo = self.root / "repo"
        self.tools = self.root / "n64tools"
        self.elf = self.root / "elf"
        self.invader = self.root / "invader"
        self.pc_tools = self.root / "pctools"
        self.pc_elf = self.root / "pcelf"
        self.notices = self.root / "pcnotices"
        self.output = self.root / "output"
        for folder in [self.tools, self.elf / "campaign", self.pc_tools, self.pc_elf, self.notices,
                       self.repo / "tools" / "n64", self.repo / "assets" / "paintball",
                       self.repo / "port" / "n64", self.invader / "src" / "tag" / "hek" / "definition",
                       self.invader / "include" / "invader" / "hek", self.invader / "src" / "bitmap" / "p8"]:
            folder.mkdir(parents=True, exist_ok=True)
        for name in (*package.TOOLS, *package.PC_ASSET_TOOLS):
            (self.tools / (name + ".exe")).write_bytes(b"never executed fixture")
        for folder in [self.elf, self.elf / "campaign", self.pc_elf]:
            for name in ["halo.elf.stripped", "halo.elf.sym"]:
                (folder / name).write_bytes(b"program fixture")
        for name in package.PC_TOOLS:
            (self.pc_tools / (name + ".exe")).write_bytes(b"never executed PC fixture")
        (self.pc_tools / "dependency.dll").write_bytes(b"supplied dependency fixture")
        (self.notices / "LICENSE.txt").write_text("matching supplied tool licence fixture")
        (self.repo / "tools" / "n64" / "rombuilder_main.py").write_text("# entry point")
        (self.repo / "assets" / "paintball" / "paint-splatter-original.png").write_bytes(b"original art fixture")
        (self.repo / "port" / "n64" / "port.json").write_text("{}")
        (self.invader / "include" / "invader" / "hek" / "fourcc.hpp").write_text("// definitions")
        (self.invader / "LICENSE.md").write_text("Invader GPL licence fixture")
        self.palette_bytes = b"".join(bytes((0 if index >= 254 else 255, index, 255 - index, index ^ 0x55))
                                      for index in range(256))
        (self.invader / "src" / "bitmap" / "p8" / "p8_palette").write_bytes(self.palette_bytes)
        self.staged_palette = None
        self.addCleanup(patch.stopall)
        patch.object(package, "REPO", self.repo).start()
        patch.object(package, "make_icon", side_effect=self.icon).start()
        patch.object(package.subprocess, "run", side_effect=self.pyinstaller).start()
        self.payload = {}

    def icon(self, folder):
        png, ico = folder / "icon.png", folder / "icon.ico"
        png.write_bytes(b"png fixture")
        ico.write_bytes(b"ico fixture")
        return png, ico

    def pyinstaller(self, command, check):
        self.assertEqual(command[:3], [sys.executable, "-m", "PyInstaller"])
        stage = Path(command[command.index("--distpath") + 1]).parent
        payload = stage / "payload"
        self.payload = {p.relative_to(payload).as_posix(): p.read_bytes()
                        for p in payload.rglob("*") if p.is_file()}
        # Exercise the real loader while the staged payload still exists.
        # No external Invader tree or cached palette may satisfy this read.
        from tools.n64 import bitmaps
        bitmaps._p8_palette.cache_clear()
        try:
            with patch.object(bitmaps, "DEFINITIONS", payload / "invader" / "src" / "tag" / "hek" / "definition"):
                self.staged_palette = bitmaps._p8_palette()
        finally:
            bitmaps._p8_palette.cache_clear()
        built = stage / "dist" / package.NAME
        built.mkdir(parents=True)
        (built / "Halo64RomBuilder.exe").write_bytes(b"not a real executable")

    def run_package(self, extra=()):
        argv = ["package", "--tools", str(self.tools), "--elf", str(self.elf),
                "--invader", str(self.invader), "--out", str(self.output), *extra]
        with patch.object(sys, "argv", argv):
            package.main()

    def test_vanilla_package_needs_no_pc_tools_or_assets(self):
        self.run_package()
        self.assertIn("elf/multiplayer/halo.elf.stripped", self.payload)
        self.assertIn("elf/campaign/halo.elf.stripped", self.payload)
        self.assertFalse(any(k.startswith("pc-tools/") or "pc-expansion" in k for k in self.payload))
        self.assertIn("art/paint-splatter-original.png", self.payload)
        self.assertNotIn("bin/mkasset.exe", self.payload)
        self.assertFalse(any(k.endswith(".map") for k in self.payload))

    def test_optional_pc_payload_is_distinct_hashed_and_notice_bound(self):
        self.run_package(["--pc-expansion-elf", str(self.pc_elf), "--pc-tools", str(self.pc_tools),
                          "--pc-tool-notices", str(self.notices)])
        self.assertIn("elf/pc-expansion/halo.elf.stripped", self.payload)
        self.assertIn("bin/mkasset.exe", self.payload)
        self.assertIn("compress_map", package.CONVERTERS)
        self.assertIn("fx_pool", package.CONVERTERS)
        manifest = json.loads(self.payload["pc-tools.json"])
        self.assertEqual(len(manifest), len(package.PC_TOOLS) + 1)
        self.assertTrue(all(len(item["sha256"]) == 64 for item in manifest))
        self.assertTrue((self.output / package.NAME / "pc-tools" / "LICENSE.txt").is_file())
        self.assertFalse(any(k.endswith(".map") for k in self.payload))

    def test_pc_package_missing_mkasset_fails_before_pyinstaller(self):
        (self.tools / "mkasset.exe").unlink()
        with self.assertRaises(SystemExit) as context:
            self.run_package(["--pc-expansion-elf", str(self.pc_elf), "--pc-tools", str(self.pc_tools),
                              "--pc-tool-notices", str(self.notices)])
        self.assertEqual(context.exception.code, 2)
        self.assertEqual(self.payload, {})

    def test_actual_p8_loader_reads_staged_selected_invader_palette(self):
        self.run_package()
        self.assertEqual(self.payload["invader/src/bitmap/p8/p8_palette"], self.palette_bytes)
        self.assertEqual(len(self.staged_palette), 256)
        self.assertEqual(self.staged_palette[0], bytes((0, 255, 0x55, 255)))
        self.assertEqual(self.staged_palette[253], bytes((253, 2, 253 ^ 0x55, 255)))
        self.assertEqual(self.staged_palette[254], bytes((254, 1, 254 ^ 0x55, 0)))
        self.assertEqual(self.staged_palette[255], bytes((255, 0, 255 ^ 0x55, 0)))

    def test_missing_or_wrong_size_palette_rejected_before_packaging(self):
        palette = self.invader / "src" / "bitmap" / "p8" / "p8_palette"
        for data in (None, b"short", self.palette_bytes + b"excess"):
            if data is None:
                palette.unlink(missing_ok=True)
            else:
                palette.write_bytes(data)
            with self.assertRaises(SystemExit) as context:
                self.run_package()
            self.assertEqual(context.exception.code, 2)
            self.assertEqual(self.payload, {})

    def test_incomplete_pc_package_rejected_before_execution(self):
        with self.assertRaises(SystemExit) as context:
            self.run_package(["--pc-tools", str(self.pc_tools)])
        self.assertEqual(context.exception.code, 2)
        self.assertEqual(self.payload, {})


if __name__ == "__main__":
    unittest.main()
