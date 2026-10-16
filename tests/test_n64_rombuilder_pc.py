"""Installer contracts exercised without invoking content converters or ROM tools."""
import json
from pathlib import Path
import queue
import struct
import sys
import tempfile
import threading
import time
import unittest
from unittest.mock import patch

from tools.n64 import rombuilder as rb
from tools.n64.pc_input import PC_MAPS

BASE = ["bloodgulch"] + ["stock" + str(i) for i in range(12)]
CONFIG = {"maps": BASE, "campaign": {"maps": ["a30"]},
          "pc_expansion": {"maps": BASE + list(PC_MAPS), "title": "HALO PC EXPANSION"}}


def cache(path, version=5, salt=0):
    data = bytearray(4096)
    data[:4] = b"daeh"
    struct.pack_into("<5I", data, 4, version, len(data), 0, 2048, 128)
    data[0x20:0x20 + len(path.stem)] = path.stem.encode("ascii")
    data[0x7fc:0x800] = b"toof"
    data[-1] = salt
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)


class RecordingBuilder(rb.RomBuilder):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.calls = []

    def run_worker(self, text, module, arguments):
        self.calls.append((module, list(arguments)))
        if module == "pc_prepare":
            folder = Path(arguments[arguments.index("--work") + 1])
            maps = folder / "generated" / "maps"
            for name in PC_MAPS:
                cache(maps / (name + ".map"))
            bitmap = maps.parent / "ui-tags" / "ui" / "shell" / "bitmaps" / "mp_map_grafix.bitmap"
            bitmap.parent.mkdir(parents=True)
            bitmap.write_bytes(b"fixture")
            (maps.parent / "manifest.json").write_text(json.dumps({"status": "complete", "outputs": []}))
            (folder / "result.json").write_text(json.dumps({"maps": str(maps), "ui_bitmap": str(bitmap)}))
        elif module == "fx_pool":
            folder = Path(arguments[arguments.index("--output") + 1])
            folder.mkdir()
            folder.with_name(folder.name + ".fx-pool.json").write_text(json.dumps({"source_unchanged": True}))
        elif module == "music":
            (Path(arguments[arguments.index("--filesystem") + 1]) / "music").mkdir()

    def run_tool(self, text, command):
        self.calls.append(("tool", list(command)))
        if "--output" in command:
            Path(command[command.index("--output") + 1]).write_bytes(b"ROM-FIXTURE")


class BuilderTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.xbox = self.root / "xbox"
        self.pc = self.root / "pc"
        for name in [*BASE, "ui", "a30"]:
            cache(self.xbox / (name + ".map"))
        for name in [*PC_MAPS, "ui"]:
            cache(self.pc / (name + ".map"), 7)
        self.elf = self.root / "elf"
        self.elf.write_bytes(b"code")
        self.tools = self.root / "trusted-tools"
        self.tools.mkdir()
        self.mkasset = self.root / "mkasset.exe"
        self.mkasset.write_bytes(b"trusted tool fixture, never executed")
        self.addCleanup(patch.stopall)
        patch.object(rb, "port_config", return_value=CONFIG).start()
        patch.object(rb, "elf_files", return_value=(self.elf, self.elf)).start()
        patch.object(rb, "tool", side_effect=lambda name: str(self.mkasset) if name == "mkasset" else name).start()
        patch.object(rb, "halo_font", return_value=None).start()

    def builder(self, selections, **kwargs):
        return RecordingBuilder(self.xbox, self.root / "out", selections,
                                work=self.root / "work", jobs=1, log=lambda _: None, **kwargs)

    def test_vanilla_default_sources_have_no_pc_requirement_or_flags(self):
        builder = self.builder(["multiplayer", "campaign"], pc_source=self.root / "missing")
        with patch("tools.n64.pc_prepare.validate_tools", side_effect=AssertionError("PC was touched")):
            outputs = builder.run()
        self.assertEqual([p.name for p in outputs], ["halo-n64.z64", "halo-campaign.z64"])
        conversions = [a for m, a in builder.calls if m == "convert"]
        self.assertEqual(len(conversions), 14)
        self.assertFalse(any("--pc-expansion" in a or "--pc-maps" in a for a in conversions))
        self.assertFalse(any(m in ("pc_prepare", "pc_thumbnails", "compress_map", "fx_pool") for m, _ in builder.calls))
        self.assertFalse(builder.input_stage.exists())
        for module, args in builder.calls:
            if module == "tool" and args[0] == "mkdfs":
                self.assertEqual(Path(args[-1]).name, "filesystem")

    def test_pc_separate_19_map_pipeline_explicit_sources_and_ui(self):
        builder = self.builder(["pc_expansion"], pc_source=self.pc, pc_tools=self.tools)
        with patch("tools.n64.pc_prepare.validate_tools", return_value=self.tools):
            outputs = builder.run()
        self.assertEqual([p.name for p in outputs], ["halo-pc-expansion.z64"])
        conversions = [a for m, a in builder.calls if m == "convert"]
        self.assertEqual(len(conversions), 19)
        for args in conversions:
            self.assertIn("--pc-expansion", args)
            self.assertEqual(Path(args[args.index("--pc-maps") + 1]), builder.pc_maps)
            source = Path(args[0])
            self.assertEqual(source.parent, builder.pc_maps if source.stem in PC_MAPS else builder.input_stage / "xbox")
        compressed = [(index, args) for index, (module, args) in enumerate(builder.calls) if module == "compress_map"]
        converted = [index for index, (module, _) in enumerate(builder.calls) if module == "convert"]
        self.assertEqual(len(compressed), 19)
        self.assertLess(max(converted), compressed[0][0])
        self.assertLess(compressed[-1][0], next(i for i, (m, _) in enumerate(builder.calls) if m == "ui"))
        self.assertEqual({Path(args[0]).stem for _, args in compressed}, set(BASE) | set(PC_MAPS))
        self.assertTrue(all(len(args) == 1 for _, args in compressed))
        self.assertEqual(sum(m == "pc_thumbnails" for m, _ in builder.calls), 1)
        self.assertEqual(sum(m == "paint_masks" for m, _ in builder.calls), 1)
        self.assertEqual(sum(m == "decal_bank" for m, _ in builder.calls), 1)
        pool = [(i, a) for i, (m, a) in enumerate(builder.calls) if m == "fx_pool"]
        self.assertEqual(len(pool), 1)
        pool_index, pool_args = pool[0]
        raw_fs = Path(pool_args[pool_args.index("--filesystem") + 1])
        packed_fs = Path(pool_args[pool_args.index("--output") + 1])
        self.assertEqual(pool_args[pool_args.index("--maps") + 1:], BASE + list(PC_MAPS))
        self.assertNotEqual(raw_fs, packed_fs)
        prune_indexes = [i for i, (m, _) in enumerate(builder.calls) if m == "prune"]
        self.assertTrue(prune_indexes and max(prune_indexes) < pool_index)
        self.assertFalse(any(i > pool_index for i in prune_indexes))
        mkdfs_calls = [(i, a) for i, (m, a) in enumerate(builder.calls) if m == "tool" and a[0] == "mkdfs"]
        self.assertEqual(len(mkdfs_calls), 1)
        self.assertGreater(mkdfs_calls[0][0], pool_index)
        self.assertEqual(Path(mkdfs_calls[0][1][-1]), packed_fs)
        self.assertTrue((builder.receipts / "pc-fx-pool.json").is_file())
        manifest = json.loads((builder.receipts / "pc-inputs.json").read_text())
        self.assertTrue(manifest["maps"])
        self.assertFalse(builder.input_stage.exists())

    def test_combined_selection_keeps_main_and_campaign_pc_free(self):
        builder = self.builder(["multiplayer", "campaign", "pc_expansion"], pc_source=self.pc, pc_tools=self.tools)
        with patch("tools.n64.pc_prepare.validate_tools", return_value=self.tools):
            outputs = builder.run()
        self.assertEqual(len(outputs), 3)
        args = [a for m, a in builder.calls if m == "convert"]
        self.assertEqual(sum("--pc-expansion" in a for a in args), 19)
        self.assertEqual(sum("--campaign" in a for a in args), 1)
        self.assertEqual(sum("--pc-expansion" not in a and "--campaign" not in a for a in args), 13)

    def test_pc_missing_source_fails_before_conversion(self):
        builder = self.builder(["pc_expansion"])
        with self.assertRaisesRegex(rb.BuildError, "also needs"):
            builder.run()
        self.assertEqual(builder.calls, [])

    def test_oversize_rom_preserves_final_and_removes_only_owned_temporary(self):
        builder = self.builder(["multiplayer"])
        builder.output.mkdir()
        final = builder.output / "halo-n64.z64"
        final.write_bytes(b"previous known ROM")
        other = builder.output / "halo-n64.tmp"
        other.write_bytes(b"unrelated pre-existing temporary")
        original_tool = builder.run_tool
        def oversized(text, command):
            original_tool(text, command)
            if "--output" in command:
                with Path(command[command.index("--output") + 1]).open("r+b") as stream:
                    stream.truncate(rb.MAX_ROM_BYTES + 1)
        builder.run_tool = oversized
        with self.assertRaisesRegex(rb.BuildError, "maximum ROM size is 64 MiB"):
            builder.run()
        self.assertEqual(final.read_bytes(), b"previous known ROM")
        self.assertEqual(other.read_bytes(), b"unrelated pre-existing temporary")
        self.assertEqual(list(builder.output.glob("halo-n64-*.tmp")), [])
        self.assertFalse(builder.input_stage.exists())

    def test_exact_64mib_boundary_can_publish(self):
        builder = self.builder(["multiplayer"])
        original_tool = builder.run_tool
        def exact_size(text, command):
            original_tool(text, command)
            if "--output" in command:
                with Path(command[command.index("--output") + 1]).open("r+b") as stream:
                    stream.truncate(rb.MAX_ROM_BYTES)
        builder.run_tool = exact_size
        output, = builder.run()
        self.assertEqual(output.stat().st_size, rb.MAX_ROM_BYTES)
        self.assertEqual(list(builder.output.glob("halo-n64-*.tmp")), [])

    def test_actual_fx_pool_worker_keeps_source_and_non_fx_bytes(self):
        from tools.n64 import fx_pool
        raw = self.root / "raw-fs"
        (raw / "fx").mkdir(parents=True)
        (raw / "levels").mkdir()
        (raw / "levels" / "keep.bin").write_bytes(b"unchanged non-FX payload")
        # One zero-emitter event record shared by two independently indexed maps.
        atlas = 48
        record = atlas + fx_pool.ATLAS_BYTES
        explosions = record + 16
        def fixture(tag):
            data = bytearray(explosions + 4)
            struct.pack_into(">4sIHHIIIHB5x", data, 0, b"N64F", 2, 1, 1, 32, atlas, explosions, 1, 0)
            struct.pack_into(">HHI", data, 32, tag, 8, record)
            struct.pack_into(">HHH", data, record, 0, 0, 0)
            struct.pack_into(">HH", data, explosions, tag, 0)
            return bytes(data)
        originals = {"first": fixture(7), "second": fixture(9)}
        for name, data in originals.items():
            (raw / "fx" / (name + ".fx")).write_bytes(data)
        packed = self.root / "packed-fs"
        with patch.object(sys, "argv", ["worker"]):
            self.assertEqual(rb.run_worker(["fx_pool", "--filesystem", str(raw), "--output", str(packed),
                                            "--maps", "first", "second"]), 0)
        for name, data in originals.items():
            self.assertEqual((raw / "fx" / (name + ".fx")).read_bytes(), data)
            fx_pool.verify_map(data, (packed / "fx" / (name + ".fx")).read_bytes(),
                               (packed / fx_pool.BANK_PATH).read_bytes(), name)
        self.assertEqual((packed / "levels" / "keep.bin").read_bytes(), b"unchanged non-FX payload")
        self.assertFalse(any(p.name.endswith((".json", ".stamp")) for p in packed.iterdir()))

    def test_pc_missing_mkasset_fails_before_conversion(self):
        self.mkasset.unlink()
        builder = self.builder(["pc_expansion"], pc_source=self.pc, pc_tools=self.tools)
        with self.assertRaisesRegex(rb.BuildError, "mkasset"):
            builder.run()
        self.assertEqual(builder.calls, [])

    def test_compressor_worker_dispatches_function_without_main_adapter(self):
        with patch("tools.n64.compress_map.compress") as compress:
            self.assertEqual(rb.run_worker(["compress_map", "generated.map"]), 0)
            compress.assert_called_once_with(Path("generated.map"))

    def test_compressor_worker_rejects_missing_or_extra_arguments(self):
        with patch("tools.n64.compress_map.compress") as compress:
            for args in ([], ["generated.map", "extra"]):
                self.assertEqual(rb.run_worker(["compress_map", *args]), 2)
            compress.assert_not_called()

    def test_wrong_xbox_pc_input_and_wrong_pc_xbox_input_rejected(self):
        cache(self.xbox / "bloodgulch.map", 7)
        with self.assertRaisesRegex(rb.BuildError, "version-5"):
            self.builder(["multiplayer"]).run()
        cache(self.xbox / "bloodgulch.map", 5)
        cache(self.pc / "timberland.map", 5)
        with self.assertRaisesRegex(rb.BuildError, "Xbox cache version 5"):
            self.builder(["pc_expansion"], pc_source=self.pc, pc_tools=self.tools).run()

    def test_missing_pc_ui_fails_before_conversion(self):
        (self.pc / "ui.map").unlink()
        with self.assertRaises(rb.BuildError):
            self.builder(["pc_expansion"], pc_source=self.pc, pc_tools=self.tools).run()

    def test_same_sized_changed_xbox_input_is_fresh(self):
        source = rb.MapSource(self.xbox)
        out = self.root / "copied"
        source.provide(["bloodgulch"], out)
        first = (out / "bloodgulch.map").read_bytes()
        cache(self.xbox / "bloodgulch.map", 5, 99)
        source.provide(["bloodgulch"], out)
        self.assertNotEqual(first, (out / "bloodgulch.map").read_bytes())

    def test_case_insensitive_installed_xbox_maps(self):
        path = self.xbox / "bloodgulch.map"
        path.rename(self.xbox / "BloodGulch.MAP")
        builder = self.builder(["multiplayer"])
        builder.run()
        self.assertEqual(len([a for m, a in builder.calls if m == "convert"]), 13)

    def test_cli_defaults_and_pc_selection(self):
        for options, expected in (([], ["multiplayer", "campaign"]),
                                  (["--pc-expansion", "--pc-source", "owned-pc"], ["pc_expansion"])):
            with patch.object(sys, "argv", ["builder", "owned-xbox", "-o", "output", *options]), patch.object(rb, "RomBuilder") as cls:
                rb.main()
                self.assertEqual(cls.call_args.args[2], expected)

    def test_cancellation_stops_a_silent_tool_without_blocking_on_stdout(self):
        builder = self.builder(["multiplayer"])
        timer = threading.Timer(0.3, builder.cancel.set)
        timer.start()
        started = time.monotonic()
        try:
            with self.assertRaises(rb.Cancelled):
                rb.RomBuilder.run_tool(builder, "silent trusted Python fixture",
                                       [sys.executable, "-c", "import time; time.sleep(30)"])
        finally:
            timer.cancel()
        self.assertLess(time.monotonic() - started, 6)
        self.assertEqual(builder.processes, set())

    def test_real_tk_source_gui_constructs_and_toggles_optional_panel(self):
        import tkinter as tk
        from tools.n64 import rombuilder_gui as gui
        root = gui.TkinterDnD.Tk() if gui.TkinterDnD is not None else tk.Tk()
        root.withdraw()
        try:
            with patch.object(gui, "load_settings", return_value={}), patch.object(gui, "payload_root", return_value=None):
                app = gui.App(root)
                root.update_idletasks()
                self.assertTrue(app.choice["multiplayer"].get())
                self.assertTrue(app.choice["campaign"].get())
                self.assertFalse(app.choice["pc_expansion"].get())
                self.assertEqual(app.pc_panel.winfo_manager(), "")
                app.choice["pc_expansion"].set(True)
                app.toggle_pc_inputs()
                root.update_idletasks()
                self.assertEqual(app.pc_panel.winfo_manager(), "grid")
                app.choice["pc_expansion"].set(False)
                app.toggle_pc_inputs()
                self.assertEqual(app.pc_panel.winfo_manager(), "")
                app.source = "owned-xbox"
                app.choice["multiplayer"].set(False)
                app.choice["campaign"].set(False)
                app.choice["pc_expansion"].set(True)
                with patch.object(gui.messagebox, "showinfo") as info:
                    app.build()
                    self.assertIn("Halo PC", info.call_args.args[1])
                    self.assertIsNone(app.builder_thread)
                app.choice["pc_expansion"].set(False)
                app.choice["multiplayer"].set(True)
                app.pc_source = "missing-unused-pc-input"
                app.output.set(str(self.root / "gui-output"))
                with patch.object(app, "remember"), patch.object(gui.threading, "Thread") as thread:
                    app.build()
                    self.assertIsNone(thread.call_args.kwargs["args"][3])
                    thread.return_value.start.assert_called_once()
        finally:
            root.destroy()

    def test_package_without_pc_tools_greys_out_the_pc_cartridge(self):
        import tkinter as tk
        from tools.n64 import rombuilder_gui as gui
        payload = self.root / "payload-without-pc"
        (payload / "elf" / "multiplayer").mkdir(parents=True)
        root = gui.TkinterDnD.Tk() if gui.TkinterDnD is not None else tk.Tk()
        root.withdraw()
        try:
            with patch.object(gui, "load_settings", return_value={"pc_expansion": True}), \
                    patch.object(gui, "payload_root", return_value=payload):
                self.assertFalse(gui.pc_expansion_included())
                app = gui.App(root)
                root.update_idletasks()
                self.assertFalse(app.choice["pc_expansion"].get())
                self.assertEqual(app.pc_panel.winfo_manager(), "")
            (payload / "pc-tools").mkdir()
            (payload / "elf" / "pc-expansion").mkdir()
            with patch.object(gui, "payload_root", return_value=payload):
                self.assertTrue(gui.pc_expansion_included())
        finally:
            root.destroy()

    def test_gui_worker_passes_selected_pc_source_and_tools(self):
        from tools.n64 import rombuilder_gui as gui
        app = object.__new__(gui.App)
        app.messages = queue.Queue()
        app.cancel = threading.Event()
        with patch.object(gui, "RomBuilder") as cls:
            cls.return_value.run.return_value = []
            app.run_build("xbox", "out", ["pc_expansion"], "pc", "trusted")
            self.assertEqual(cls.call_args.kwargs["pc_source"], "pc")
            self.assertEqual(cls.call_args.kwargs["pc_tools"], "trusted")


if __name__ == "__main__":
    unittest.main()
