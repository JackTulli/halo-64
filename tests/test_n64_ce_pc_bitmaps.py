"""The Custom Edition cartridge's shared bitmaps that no Xbox map has, from the retail Halo PC maps (ce_prepare.py
--pc-maps, the ROM Builder's staging of them); synthetic files only, no game data or external tools."""
import struct
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

from tools.n64 import ce_prepare
from tools.n64 import pc_input
from tools.n64 import rombuilder as rb


def tag_map(path, tags, version=7):
    """a cache file with the retail PC header (version) whose tag table names tags [(group, name)]"""
    names, offsets = b"", []
    for _, name in tags:
        offsets.append(len(names))
        names += name.encode("latin1") + b"\0"
    table = 40
    strings = table + 32 * len(tags)
    data = bytearray(strings + len(names))
    struct.pack_into("<IIII", data, 0, ce_prepare.CE_TAG_BASE + table, 0xE1740000, 0, len(tags))
    for i, ((group, _), offset) in enumerate(zip(tags, offsets)):
        struct.pack_into("<4s4s4sIIIII", data, table + 32 * i, group.encode("latin1")[::-1], b"\xff" * 4, b"\xff" * 4,
                         0xE1740000 + i, ce_prepare.CE_TAG_BASE + strings + offset, 0, 0, 0)
    data[strings:] = names
    header = bytearray(0x800)
    header[:4], header[-4:] = b"daeh", b"toof"
    struct.pack_into("<5I", header, 4, version, 0x800 + len(data), 0, 0x800, len(data))
    header[0x20:0x20 + len(path.stem)] = path.stem.encode("ascii")
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(bytes(header) + bytes(data))


def bitmaps_map(path):
    """a retail-PC-shaped bitmaps.map: one resource"""
    names = b"x_0\0"
    data = struct.pack("<4I", 1, 20, 20 + len(names), 1) + b"\0" * 4 + names + struct.pack("<3I", 0, 4, 16)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)


class PCTagNamesTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)

    def test_tag_table_read(self):
        tags = [("bitm", "ui\\shell\\bitmaps\\a"), ("snd!", "sound\\b"), ("bitm", "powerups\\c")]
        tag_map(self.root / "ui.map", tags)
        self.assertEqual(ce_prepare.pc_tag_names(self.root / "ui.map"), set(tags))

    def test_other_cache_versions_rejected(self):
        for version in (5, ce_prepare.CE_VERSION):
            tag_map(self.root / "ui.map", [("bitm", "a")], version)
            with self.assertRaises(ce_prepare.CEPrepareError):
                ce_prepare.pc_tag_names(self.root / "ui.map")

    def test_bounds_checked(self):
        tag_map(self.root / "ui.map", [("bitm", "a")])
        data = bytearray((self.root / "ui.map").read_bytes())
        struct.pack_into("<I", data, 0x800 + 12, 70000)
        (self.root / "ui.map").write_bytes(bytes(data))
        with self.assertRaises(ce_prepare.CEPrepareError):
            ce_prepare.pc_tag_names(self.root / "ui.map")
        tag_map(self.root / "ui.map", [("bitm", "a")])
        data = bytearray((self.root / "ui.map").read_bytes())
        struct.pack_into("<I", data, 0x800 + 40 + 16, ce_prepare.CE_TAG_BASE + (1 << 20))
        (self.root / "ui.map").write_bytes(bytes(data))
        with self.assertRaises(ce_prepare.CEPrepareError):
            ce_prepare.pc_tag_names(self.root / "ui.map")


class ResourceSourceTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.xbox, self.pc = self.root / "xbox", self.root / "pc"
        self.xbox.mkdir()
        (self.xbox / "bloodgulch.map").write_bytes(b"")
        patch = mock.patch.object(ce_prepare, "xbox_tag_names", return_value={("bitm", "a")})
        patch.start()
        self.addCleanup(patch.stop)
        self.tags = [("bitm", "a", True), ("bitm", "b", True), ("bitm", "c", True), ("snd!", "s", True),
                     ("bitm", "own", False)]
        tag_map(self.pc / "ui.map", [("bitm", "a"), ("bitm", "b")])
        tag_map(self.pc / "dangercanyon.map", [("bitm", "b"), ("bitm", "c"), ("snd!", "s")])
        self.log = []

    def test_xbox_maps_first_then_the_pc_maps_bitmaps_in_order(self):
        found, from_pc = ce_prepare.resource_sources(self.tags, self.xbox, self.log.append, self.pc)
        self.assertEqual(found, {("bitm", "a"): self.xbox / "bloodgulch.map"})
        self.assertEqual(from_pc, {("bitm", "b"): self.pc / "ui.map", ("bitm", "c"): self.pc / "dangercanyon.map"})
        self.assertIn("2 of the 2 other shared bitmaps found in the Halo PC maps", self.log)

    def test_without_the_pc_maps_as_before(self):
        found, from_pc = ce_prepare.resource_sources(self.tags, self.xbox, self.log.append)
        self.assertEqual(found, {("bitm", "a"): self.xbox / "bloodgulch.map"})
        self.assertEqual(from_pc, {})
        self.assertEqual(self.log, ["1 of 4 shared Custom Edition tags found in the Xbox maps"])

    def test_unreadable_pc_map_left_out(self):
        (self.pc / "ui.map").write_bytes(b"not a map" * 300)
        _, from_pc = ce_prepare.resource_sources(self.tags, self.xbox, self.log.append, self.pc)
        self.assertEqual(from_pc[("bitm", "b")], self.pc / "dangercanyon.map")
        self.assertTrue(any(line.startswith("ui.map of the Halo PC maps not used") for line in self.log))


class ExtractTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.calls = []
        self.fail = False

        def invoke(tool, *args):
            self.calls.append((tool, args))
            if self.fail and "-m" in args:
                raise ce_prepare.CEPrepareError("invader-extract failed (1): no\nmore")
            return ""

        self.space = SimpleNamespace(tags=self.root / "tags", invoke=invoke)
        self.xbox, self.pc = self.root / "xbox", self.root / "pc"
        self.found = {("bitm", "a\\x"): self.xbox / "bloodgulch.map"}
        self.from_pc = {("bitm", "b\\y"): self.pc / "ui.map"}

    def test_pc_bitmaps_extracted_after_the_xbox_ones_with_bitmaps_map(self):
        ce_prepare.extract(self.space, self.root / "ce.map", self.xbox, self.found, self.from_pc, self.pc,
                           lambda _: None)
        self.assertEqual([args[-1] for _, args in self.calls],
                         [self.xbox / "bloodgulch.map", self.root / "ce.map", self.xbox / "bloodgulch.map",
                          self.pc / "ui.map"])
        last = self.calls[-1][1]
        self.assertEqual(last[last.index("-m") + 1], self.pc)
        self.assertIn("b/y.bitmap", last)
        self.assertNotIn("-O", last)
        self.assertTrue(all("-m" not in args for _, args in self.calls[:-1]))

    def test_failed_pc_extraction_left_to_the_stand_ins(self):
        self.fail = True
        log = []
        ce_prepare.extract(self.space, self.root / "ce.map", self.xbox, self.found, self.from_pc, self.pc, log.append)
        self.assertEqual(log, ["the bitmaps of ui.map (Halo PC) not extracted: invader-extract failed (1): no"])

    def test_pc_sources_marked_not_found_by_their_folder(self):
        # (an Xbox map in the PC folder is still extracted as an Xbox map)
        found = {("bitm", "a\\x"): self.pc / "beavercreek.map"}
        ce_prepare.extract(self.space, self.root / "ce.map", self.pc, found, self.from_pc, self.pc, lambda _: None)
        self.assertNotIn("-m", self.calls[2][1])
        self.assertEqual(self.calls[2][1][-1], self.pc / "beavercreek.map")
        self.assertIn("-m", self.calls[3][1])

    def test_without_pc_sources_the_calls_of_before(self):
        ce_prepare.extract(self.space, self.root / "ce.map", self.xbox, self.found)
        self.assertEqual(len(self.calls), 3)
        self.assertTrue(all("-m" not in args for _, args in self.calls))

    def test_pc_folder_without_bitmaps_map_refused(self):
        self.pc.mkdir()
        with self.assertRaises(ce_prepare.CEPrepareError):
            ce_prepare.prepare_ce_map(self.root / "ce.map", self.xbox, self.root, self.root / "work",
                                      pc_maps=self.pc)


class BuilderStagingTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.pc = self.root / "pc"
        for name in ce_prepare.PC_ORDER:
            tag_map(self.pc / f"{name}.map", [("bitm", name)])
        bitmaps_map(self.pc / "bitmaps.map")
        (self.pc / "sounds.map").write_bytes(b"left out")
        tag_map(self.root / "xbox" / "bloodgulch.map", [("bitm", "a")], 5)
        self.log = []

    def builder(self, pc_source):
        builder = rb.RomBuilder(self.root / "xbox", self.root / "out", ["custom"], work=self.root / "work", jobs=1,
                                log=self.log.append, pc_source=pc_source, custom_maps=[self.root / "a.map"])
        builder.input_stage = self.root / "stage"
        return builder

    def test_staged_without_sounds_map(self):
        folder = self.builder(self.pc).stage_pc_bitmaps()
        self.assertEqual(sorted(p.name for p in folder.iterdir()),
                         sorted(["bitmaps.map", *[f"{name}.map" for name in ce_prepare.PC_ORDER]]))

    def test_no_pc_data_logged(self):
        builder = self.builder(None)
        self.assertIsNone(builder.stage_pc_bitmaps())
        self.assertTrue(self.log[-1].startswith("no Halo PC data given"))
        self.assertEqual(builder.notes, [])

    def test_absent_map_logged_and_the_rest_staged(self):
        (self.pc / "icefields.map").unlink()
        folder = self.builder(self.pc).stage_pc_bitmaps()
        self.assertFalse((folder / "icefields.map").exists())
        self.assertTrue((folder / "ui.map").is_file())
        self.assertTrue(any("icefields.map: not there" in line for line in self.log))

    def test_bad_map_left_out_and_the_rest_staged(self):
        tag_map(self.pc / "icefields.map", [("bitm", "icefields")], ce_prepare.CE_VERSION)
        builder = self.builder(self.pc)
        folder = builder.stage_pc_bitmaps()
        self.assertFalse((folder / "icefields.map").exists())
        self.assertTrue((folder / "bitmaps.map").is_file() and (folder / "timberland.map").is_file())
        self.assertTrue(any("icefields.map: " in line and "609" in line for line in self.log))
        self.assertEqual(builder.notes, [])

    def test_failing_copy_of_one_map_left_out(self):
        provide = pc_input.PCMapSource.provide

        def failing(source, names, folder, progress=None):
            if names == ["ui"]:
                raise OSError("disk full")
            return provide(source, names, folder, progress)

        with mock.patch.object(pc_input.PCMapSource, "provide", failing):
            folder = self.builder(self.pc).stage_pc_bitmaps()
        self.assertFalse((folder / "ui.map").exists())
        self.assertTrue((folder / "dangercanyon.map").is_file())
        self.assertTrue(any("ui.map: disk full" in line for line in self.log))

    def test_failing_copy_of_bitmaps_map_not_used_and_noted(self):
        provide = pc_input.PCMapSource.provide

        def failing(source, names, folder, progress=None):
            if names == ["bitmaps"]:
                raise OSError("disk full")
            return provide(source, names, folder, progress)

        builder = self.builder(self.pc)
        with mock.patch.object(pc_input.PCMapSource, "provide", failing):
            self.assertIsNone(builder.stage_pc_bitmaps())
        self.assertFalse((builder.input_stage / "pc-bitmaps").exists())
        self.assertEqual(len(builder.notes), 1)

    def test_cancel_stops_the_staging(self):
        builder = self.builder(self.pc)
        builder.cancel.set()
        with self.assertRaises(rb.Cancelled):
            builder.stage_pc_bitmaps()

    def test_without_bitmaps_map_not_used_and_noted(self):
        (self.pc / "bitmaps.map").unlink()
        builder = self.builder(self.pc)
        self.assertIsNone(builder.stage_pc_bitmaps())
        self.assertFalse((builder.input_stage / "pc-bitmaps").exists())
        self.assertEqual(len(builder.notes), 1)

    def test_preparations_given_the_folder(self):
        builder = self.builder(self.pc)
        (self.root / "a.map").write_bytes(b"")
        tasks = []

        def run_parallel(given, keep_going=False, **_):
            # (each preparation failed: prepare_custom stops with nothing to put on the cartridge)
            tasks.extend(given)
            return [(task, rb.BuildError("error: not run in this test")) for task in given]

        with mock.patch("tools.n64.ce_prepare.read_ce_map", return_value=("a", "scenario", [])), \
                mock.patch.object(builder, "run_parallel", side_effect=run_parallel):
            with self.assertRaises(rb.BuildError):
                builder.prepare_custom(self.root / "xbox")
        arguments = tasks[0][2]
        self.assertEqual(arguments[arguments.index("--pc-maps") + 1], str(builder.input_stage / "pc-bitmaps"))


class GUITests(unittest.TestCase):
    def test_pc_panel_and_source_also_for_the_custom_cartridge(self):
        try:
            import tkinter as tk
        except ImportError as error:  # (no Tk here)
            self.skipTest(f"tkinter: {error}")
        from tools.n64 import rombuilder_gui as gui
        tmp = tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        try:
            root = gui.TkinterDnD.Tk() if gui.TkinterDnD is not None else tk.Tk()
        except tk.TclError as error:  # (no display)
            self.skipTest(f"Tk: {error}")
        root.withdraw()
        try:
            with mock.patch.object(gui, "load_settings", return_value={}), \
                    mock.patch.object(gui, "payload_root", return_value=None):
                app = gui.App(root)
                root.update_idletasks()
                self.assertEqual(app.pc_panel.winfo_manager(), "")
                app.choice["custom"].set(True)
                app.toggle_pc_inputs()
                self.assertEqual(app.pc_panel.winfo_manager(), "grid")
                app.source = "owned-xbox"
                app.choice["multiplayer"].set(False)
                app.choice["campaign"].set(False)
                app.custom_maps = ["a.map"]
                app.pc_source = "pc-maps"
                app.output.set(str(Path(tmp.name) / "out"))
                with mock.patch.object(app, "remember"), mock.patch.object(gui.threading, "Thread") as thread:
                    app.build()
                    self.assertEqual(thread.call_args.kwargs["args"][3], "pc-maps")
                app.choice["custom"].set(False)
                app.toggle_pc_inputs()
                self.assertEqual(app.pc_panel.winfo_manager(), "")
        finally:
            root.destroy()

    def test_pc_source_without_bitmaps_map_ticked_with_a_note(self):
        try:
            import tkinter as tk
        except ImportError as error:  # (no Tk here)
            self.skipTest(f"tkinter: {error}")
        from tools.n64 import rombuilder_gui as gui
        tmp = tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        pc = Path(tmp.name) / "pc"
        for name in (*pc_input.PC_MAPS, "ui"):
            tag_map(pc / f"{name}.map", [("bitm", name)])
        try:
            root = gui.TkinterDnD.Tk() if gui.TkinterDnD is not None else tk.Tk()
        except tk.TclError as error:  # (no display)
            self.skipTest(f"Tk: {error}")
        root.withdraw()
        try:
            with mock.patch.object(gui, "load_settings", return_value={}), \
                    mock.patch.object(gui, "payload_root", return_value=None):
                app = gui.App(root)
                app.set_pc_source(str(pc))
                self.assertEqual(app.pc_source, str(pc))
                self.assertIn("no bitmaps.map", app.pc_source_label.cget("text"))
                bitmaps_map(pc / "bitmaps.map")
                app.set_pc_source(str(pc))
                self.assertNotIn("no bitmaps.map", app.pc_source_label.cget("text"))
        finally:
            root.destroy()


if __name__ == "__main__":
    unittest.main()
