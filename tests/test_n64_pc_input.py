"""Synthetic fixtures only: never requires or modifies commercial assets."""

import hashlib
from pathlib import Path
import struct
import tempfile
import unittest

from tools.n64.pc_input import DEFAULT_REQUIRED, PCInputError, PCMapSource


SECTOR = 2048


def map_bytes(name="dangercanyon", version=7, fill=0x42):
    data = bytearray([fill]) * 4096
    data[:2048] = bytes(2048)
    data[:4] = b"daeh"
    struct.pack_into("<5I", data, 4, version, len(data), 0, 2048, 2048)
    data[32:32 + len(name)] = name.encode("ascii")
    data[2044:2048] = b"toof"
    return bytes(data)


def resource_bytes(kind=1):
    names = b"test\\resource\0"
    return struct.pack("<4I", kind, 20, 20 + len(names), 1) + b"data" + names + struct.pack("<3I", 0, 4, 16)


def dual32(data, offset, value):
    struct.pack_into("<I", data, offset, value)
    struct.pack_into(">I", data, offset + 4, value)


def dual16(data, offset, value):
    struct.pack_into("<H", data, offset, value)
    struct.pack_into(">H", data, offset + 2, value)


def record(name, sector, size, directory=False):
    name = name if isinstance(name, bytes) else name.encode("ascii")
    result = bytearray(33 + len(name) + (len(name) % 2 == 0))
    result[0] = len(result)
    dual32(result, 2, sector)
    dual32(result, 10, size)
    result[25] = 2 if directory else 0
    dual16(result, 28, 1)
    result[32] = len(name)
    result[33:33 + len(name)] = name
    return result


def iso_bytes(entries=None, nested_entries=None, payload=None):
    data = bytearray(30 * SECTOR)
    pvd = bytearray(SECTOR)
    pvd[:7] = b"\x01CD001\x01"
    dual32(pvd, 80, 30)
    dual16(pvd, 128, SECTOR)
    root = record(b"\0", 20, SECTOR, True)
    pvd[156:156 + len(root)] = root
    data[16 * SECTOR:17 * SECTOR] = pvd
    data[17 * SECTOR:17 * SECTOR + 7] = b"\xffCD001\x01"
    if entries is None:
        entries = [record("MAPS", 21, SECTOR, True)]
    root_body = record(b"\0", 20, SECTOR, True) + record(b"\1", 20, SECTOR, True) + b"".join(entries)
    data[20 * SECTOR:20 * SECTOR + len(root_body)] = root_body
    if nested_entries is None:
        nested_entries = [record("DANGERCANYON.MAP;1", 22, 4096)]
    nested_body = record(b"\0", 21, SECTOR, True) + record(b"\1", 20, SECTOR, True) + b"".join(nested_entries)
    data[21 * SECTOR:21 * SECTOR + len(nested_body)] = nested_body
    payload = map_bytes() if payload is None else payload
    data[22 * SECTOR:22 * SECTOR + len(payload)] = payload
    return data


class PCMapSourceTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.maps = self.root / "game" / "mApS"
        self.maps.mkdir(parents=True)
        self.output = self.root / "output"

    def write_map(self, name="dangercanyon", **kwargs):
        path = self.maps / (name.upper() + ".MAP")
        path.write_bytes(map_bytes(name=name, **kwargs))
        return path

    def write_iso(self, data=None):
        path = self.root / "halo.ISO"
        path.write_bytes(iso_bytes() if data is None else data)
        return path

    def test_exact_six_defaults_and_case_insensitive_root_folder(self):
        for name in DEFAULT_REQUIRED:
            self.write_map(name)
        source = PCMapSource(self.maps.parent)
        self.assertEqual(source.names(), sorted(DEFAULT_REQUIRED))
        self.assertEqual(source.list(), source.names())
        self.assertEqual(source.validate(), list(DEFAULT_REQUIRED))

    def test_missing_map_is_actionable(self):
        self.write_map()
        with self.assertRaisesRegex(PCInputError, "Missing required.*deathisland.map"):
            PCMapSource(self.maps).validate()

    def test_rejects_xbox_and_custom_edition(self):
        for version, edition in ((5, "Xbox"), (609, "Custom Edition")):
            with self.subTest(version=version):
                self.write_map(version=version)
                with self.assertRaisesRegex(PCInputError, edition + ".*version.*retail Halo PC"):
                    PCMapSource(self.maps).validate(["dangercanyon"])

    def test_rejects_bad_magic_footer_declared_length_and_tag_bounds(self):
        for offset, replacement in ((0, b"head"), (2044, b"foot"), (8, struct.pack("<I", 4095)), (16, struct.pack("<I", 4090)), (20, struct.pack("<I", 0))):
            with self.subTest(offset=offset):
                data = bytearray(map_bytes())
                data[offset:offset + len(replacement)] = replacement
                (self.maps / "dangercanyon.map").write_bytes(data)
                with self.assertRaises(PCInputError):
                    PCMapSource(self.maps).validate(["dangercanyon"])

    def test_rejects_renamed_and_truncated_maps(self):
        path = self.write_map()
        path.write_bytes(map_bytes("deathisland"))
        with self.assertRaisesRegex(PCInputError, "identifies itself"):
            PCMapSource(self.maps).validate(["dangercanyon"])
        path.write_bytes(b"daeh")
        with self.assertRaisesRegex(PCInputError, "truncated"):
            PCMapSource(self.maps).validate(["dangercanyon"])

    def test_always_refreshes_same_size_output_and_source(self):
        path = self.write_map()
        source = PCMapSource(self.maps)
        result = source.provide(["DANGERCANYON.MAP"], self.output)
        self.assertEqual(result, self.output)
        target = self.output / "dangercanyon.map"
        target.write_bytes(map_bytes(fill=0x19))
        source.provide(["dangercanyon"], self.output)
        self.assertEqual(target.read_bytes(), path.read_bytes())
        path.write_bytes(map_bytes(fill=0x66))
        source.provide(["dangercanyon"], self.output)
        self.assertEqual(target.read_bytes(), path.read_bytes())
        self.assertNotEqual(target.read_bytes(), map_bytes())

    def test_whitelist_copy_progress_provenance_and_no_source_writes(self):
        selected = self.write_map()
        unused = self.write_map("ui")
        before = selected.read_bytes(), selected.stat().st_mtime_ns, unused.read_bytes()
        progress = []
        source = PCMapSource(self.maps)
        source.provide(["dangercanyon"], self.output, lambda *args: progress.append(args))
        self.assertEqual([p.name for p in self.output.iterdir()], ["dangercanyon.map"])
        self.assertEqual(progress[-1], ("dangercanyon", 4096, 4096))
        entry = source.last_manifest["maps"][0]
        self.assertEqual(entry["sha256"], hashlib.sha256(selected.read_bytes()).hexdigest())
        self.assertEqual(entry["size"], 4096)
        self.assertEqual(entry["member"], str(selected.resolve()))
        self.assertEqual(source.last_manifest["source_kind"], "folder")
        self.assertEqual(before, (selected.read_bytes(), selected.stat().st_mtime_ns, unused.read_bytes()))

    def test_source_destination_same_folder_rejected(self):
        path = self.write_map()
        # Use lower-case filename so resolved path comparison is meaningful on all platforms.
        path = path.rename(self.maps / "dangercanyon.map")
        with self.assertRaisesRegex(PCInputError, "output folder must differ"):
            PCMapSource(self.maps).provide(["dangercanyon"], self.maps)
        self.assertEqual(path.read_bytes(), map_bytes())

    def test_explicit_ui_and_shared_resource_cache_dependencies(self):
        self.write_map("ui")
        for name, kind in (("bitmaps", 1), ("sounds", 2)):
            (self.maps / (name + ".map")).write_bytes(resource_bytes(kind))
        source = PCMapSource(self.maps)
        self.assertEqual(source.validate(["ui", "bitmaps", "sounds"]), ["ui", "bitmaps", "sounds"])
        source.provide(source.names(), self.output)
        self.assertEqual((self.output / "bitmaps.map").read_bytes(), resource_bytes(1))
        self.assertEqual((self.output / "sounds.map").read_bytes(), resource_bytes(2))

    def test_resource_cache_wrong_kind_and_index_bounds_rejected(self):
        path = self.maps / "bitmaps.map"
        path.write_bytes(resource_bytes(2))
        with self.assertRaisesRegex(PCInputError, "resource-cache header"):
            PCMapSource(self.maps).validate(["bitmaps"])
        data = bytearray(resource_bytes())
        struct.pack_into("<I", data, len(data) - 4, 9999)
        path.write_bytes(data)
        with self.assertRaisesRegex(PCInputError, "resource index"):
            PCMapSource(self.maps).validate(["bitmaps"])

    def test_requested_traversal_names_rejected(self):
        self.write_map()
        for name in ("../dangercanyon", "C:evil", "/dangercanyon", "a/b", "a\\b", "a\n"):
            with self.subTest(name=name), self.assertRaisesRegex(PCInputError, "Unsafe map name"):
                PCMapSource(self.maps).provide([name], self.output)
        self.assertFalse(self.output.exists())

    def test_iso_loose_nested_map_readonly_provenance(self):
        path = self.write_iso()
        before = path.read_bytes(), path.stat().st_mtime_ns
        source = PCMapSource(path)
        self.assertEqual(source.names(), ["dangercanyon"])
        source.provide(["dangercanyon"], self.output)
        self.assertEqual((self.output / "dangercanyon.map").read_bytes(), map_bytes())
        entry = source.last_manifest["maps"][0]
        self.assertEqual(entry["iso_offset"], 22 * SECTOR)
        self.assertEqual(entry["member"], "MAPS/DANGERCANYON.MAP")
        self.assertEqual(entry["sha256"], hashlib.sha256(map_bytes()).hexdigest())
        self.assertEqual(before, (path.read_bytes(), path.stat().st_mtime_ns))

    def test_iso_rejects_traversal_absolute_drive_control_names(self):
        for name in ("../evil.map;1", "/evil.map;1", "C:evil.map;1", "evil\\a.map;1", "evil\n.map;1", ".."):
            with self.subTest(name=name), self.assertRaisesRegex(PCInputError, "unsafe directory name"):
                PCMapSource(self.write_iso(iso_bytes(nested_entries=[record(name, 22, 4096)])))

    def test_iso_out_of_bounds_truncation_and_endian_disagreement(self):
        bad = iso_bytes(nested_entries=[record("DANGERCANYON.MAP;1", 29, 4096)])
        with self.assertRaisesRegex(PCInputError, "out of bounds"):
            PCMapSource(self.write_iso(bad))
        bad = iso_bytes()
        struct.pack_into(">I", bad, 16 * SECTOR + 84, 31)
        with self.assertRaisesRegex(PCInputError, "endian"):
            PCMapSource(self.write_iso(bad))
        with self.assertRaisesRegex(PCInputError, "volume size"):
            PCMapSource(self.write_iso(iso_bytes()[:-1]))

    def test_iso_cycle_shared_directory_and_duplicate_maps(self):
        for entries in ([record("AGAIN", 20, SECTOR, True)], [record("ONE", 21, SECTOR, True), record("TWO", 21, SECTOR, True)]):
            with self.subTest(entries=entries), self.assertRaisesRegex(PCInputError, "cycle|alias"):
                PCMapSource(self.write_iso(iso_bytes(entries=entries)))
        bad = iso_bytes(nested_entries=[record("DANGERCANYON.MAP;1", 22, 4096), record("dangercanyon.map;1", 22, 4096)])
        with self.assertRaisesRegex(PCInputError, "collision|duplicate"):
            PCMapSource(self.write_iso(bad))

    def test_iso_packed_installer_actionable_no_execution(self):
        bad = iso_bytes(entries=[record("INSTALL.CAB;1", 22, 4096)], nested_entries=[])
        with self.assertRaisesRegex(PCInputError, "Packed CAB/MSI/installer extraction is unsupported.*installed Halo PC Maps folder"):
            PCMapSource(self.write_iso(bad))
        self.assertFalse(self.output.exists())

    def test_iso_directory_depth_limit(self):
        data = iso_bytes()
        data.extend(bytes(20 * SECTOR))
        dual32(data, 16 * SECTOR + 80, 50)
        for sector in range(20, 39):
            body = record(b"\0", sector, SECTOR, True) + record(b"\1", max(20, sector - 1), SECTOR, True)
            body += record("CHILD", sector + 1, SECTOR, True)
            data[sector * SECTOR:(sector + 1) * SECTOR] = body + bytes(SECTOR - len(body))
        with self.assertRaisesRegex(PCInputError, "depth or size"):
            PCMapSource(self.write_iso(data))

    def test_iso_rejects_unsupported_multi_extent_and_bad_directory_record(self):
        entry = record("DANGERCANYON.MAP;1", 22, 4096)
        entry[25] = 0x80
        with self.assertRaisesRegex(PCInputError, "multi-extent"):
            PCMapSource(self.write_iso(iso_bytes(nested_entries=[entry])))
        bad = iso_bytes()
        bad[21 * SECTOR] = 33
        with self.assertRaisesRegex(PCInputError, "truncated"):
            PCMapSource(self.write_iso(bad))

    def test_failed_copy_clears_manifest_and_removes_temporary_file(self):
        self.write_map()
        source = PCMapSource(self.maps)
        source.provide(["dangercanyon"], self.output)
        def fail(*args):
            raise RuntimeError("cancelled")
        with self.assertRaisesRegex(RuntimeError, "cancelled"):
            source.provide(["dangercanyon"], self.output, fail)
        self.assertIsNone(source.last_manifest)
        self.assertFalse(list(self.output.glob(".pc-map-*")))


if __name__ == "__main__":
    unittest.main()
