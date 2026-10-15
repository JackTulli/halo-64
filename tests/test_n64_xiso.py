"""Small synthetic XDVDFS fixtures; no copyrighted assets or external tools."""

from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch

from tools.n64.xiso import DIRECTORY, MAGIC, PARTITIONS, SECTOR, XboxDisc


PAYLOAD = b"synthetic Halo map payload\n" * 7


def node(name, sector=36, size=len(PAYLOAD), attributes=0, left=0, right=0):
    name = name if isinstance(name, bytes) else name.encode("latin-1")
    data = struct.pack("<HHIIBB", left, right, sector, size, attributes, len(name)) + name
    return data + bytes((-len(data)) % 4)


class XboxDiscTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)

    def image(self, *, partition=0, root=None, child=None, root_size=None, sectors=40):
        path = self.root / "fixture.iso"
        header = bytearray(SECTOR)
        header[:20] = MAGIC
        header[0x7EC:] = MAGIC
        root = node("Maps", 35, SECTOR, DIRECTORY) if root is None else root
        child = node("A10.MAP") if child is None else child
        struct.pack_into("<II", header, 20, 34, SECTOR if root_size is None else root_size)
        with path.open("wb") as stream:
            stream.seek(partition + sectors * SECTOR - 1)
            stream.write(b"\0")
            for sector, content in ((32, header), (34, root), (35, child), (36, PAYLOAD)):
                stream.seek(partition + sector * SECTOR)
                stream.write(content)
        return path

    def test_valid_trimmed_and_supported_nonzero_partition_exact_extraction(self):
        for partition in (0, PARTITIONS[-1]):
            with self.subTest(partition=partition):
                path = self.image(partition=partition)
                before = path.stat().st_size, path.stat().st_mtime_ns
                disc = XboxDisc(path)
                try:
                    self.assertEqual(disc.partition, partition)
                    self.assertEqual(disc.files, {"maps/a10.map": (36, len(PAYLOAD))})
                    self.assertEqual(disc.names, {"maps/a10.map": "Maps/A10.MAP"})
                    target = self.root / "output.map"
                    progress = []
                    disc.extract("MAPS/A10.MAP", target, lambda *args: progress.append(args))
                    self.assertEqual(target.read_bytes(), PAYLOAD)
                    self.assertEqual(progress[-1], (len(PAYLOAD), len(PAYLOAD)))
                    self.assertEqual(before, (path.stat().st_size, path.stat().st_mtime_ns))
                finally:
                    disc.close()
                self.assertTrue(disc.file.closed)

    def test_zero_tree_children_are_sentinels_and_valid_binary_tree(self):
        first = node("A10.MAP", right=24 // 4)
        self.assertEqual(len(first), 24)
        second = node("A30.MAP")
        disc = XboxDisc(self.image(child=first + second))
        self.addCleanup(disc.close)
        self.assertEqual(sorted(disc.files), ["maps/a10.map", "maps/a30.map"])

    def test_constructor_closes_open_file_on_parse_error(self):
        path = self.image(root_size=3)
        original_open = open
        handles = []
        def capture(*args, **kwargs):
            handle = original_open(*args, **kwargs)
            handles.append(handle)
            return handle
        with patch("tools.n64.xiso.open", side_effect=capture, create=True):
            with self.assertRaisesRegex(ValueError, "root directory"):
                XboxDisc(path)
        self.assertEqual(len(handles), 1)
        self.assertTrue(handles[0].closed)

    def test_truncated_or_invalid_descriptor_rejected(self):
        path = self.image()
        with path.open("r+b") as stream:
            stream.truncate(32 * SECTOR + 30)
        with self.assertRaisesRegex(ValueError, "complete Xbox disc image"):
            XboxDisc(path)
        path = self.image()
        with path.open("r+b") as stream:
            stream.seek(32 * SECTOR + 0x7EC)
            stream.write(b"broken")
        with self.assertRaisesRegex(ValueError, "complete Xbox disc image"):
            XboxDisc(path)

    def test_root_child_directory_and_file_extents_outside_image(self):
        for kwargs in ({"root_size": 100 * SECTOR}, {"root": node("Maps", 100, SECTOR, DIRECTORY)}, {"child": node("A10.MAP", 39, 2 * SECTOR)}):
            with self.subTest(kwargs=kwargs), self.assertRaisesRegex(ValueError, "outside the image"):
                XboxDisc(self.image(**kwargs))

    def test_truncated_entry_header_name_and_tree_pointer(self):
        for kwargs in ({"root_size": 15, "root": node("Maps")}, {"root_size": 14}, {"root": node("Maps", right=0xFFFF)}):
            with self.subTest(kwargs=kwargs), self.assertRaisesRegex(ValueError, "truncated"):
                XboxDisc(self.image(**kwargs))

    def test_empty_name_and_padding_pointer_rejected(self):
        for content in (node(b""), b"\xff" * SECTOR):
            with self.subTest(content=content[:20]), self.assertRaisesRegex(ValueError, "name is empty|padding"):
                XboxDisc(self.image(root=content))

    def test_unsafe_control_path_separator_drive_and_dot_names(self):
        for name in ("..", ".", "../map", "/map", "a/b", "a\\b", "C:map", "a\x00b", "a\x1fb", "a\x7fb", "a\x85b", "map.", "map "):
            with self.subTest(name=name), self.assertRaisesRegex(ValueError, "unsafe entry name"):
                XboxDisc(self.image(child=node(name)))

    def test_case_collisions_and_overlapping_tree_entries_rejected(self):
        first = node("A10.MAP", right=6)
        second = node("a10.map")
        with self.assertRaisesRegex(ValueError, "collision"):
            XboxDisc(self.image(child=first + second))
        # A pointer into the existing node must not silently create a new entry.
        first = node("A10.MAP", right=1)
        with self.assertRaises(ValueError):
            XboxDisc(self.image(child=first))

    def test_binary_tree_cycle_and_shared_pointer_rejected(self):
        first = node("A10.MAP", right=6)
        second = node("A30.MAP", right=6)
        with self.assertRaisesRegex(ValueError, "cycle|shared pointer"):
            XboxDisc(self.image(child=first + second))
        first = node("A10.MAP", left=6, right=6)
        second = node("A30.MAP")
        with self.assertRaisesRegex(ValueError, "cycle|shared pointer"):
            XboxDisc(self.image(child=first + second))

    def test_global_directory_cycle_and_shared_directory_rejected(self):
        with self.assertRaisesRegex(ValueError, "directory cycle"):
            XboxDisc(self.image(root=node("Again", 34, SECTOR, DIRECTORY)))
        first = node("Maps", 35, SECTOR, DIRECTORY, right=5)
        self.assertEqual(len(first), 20)
        second = node("Other", 35, SECTOR, DIRECTORY)
        with self.assertRaisesRegex(ValueError, "shared-directory alias"):
            XboxDisc(self.image(root=first + second))

    def test_directory_depth_size_total_bytes_and_entry_limits(self):
        path = self.image()
        with patch("tools.n64.xiso.MAX_DEPTH", 0):
            with self.assertRaisesRegex(ValueError, "depth or table size"):
                XboxDisc(path)
        with patch("tools.n64.xiso.MAX_DIRECTORY_SIZE", 100):
            with self.assertRaisesRegex(ValueError, "depth or table size"):
                XboxDisc(path)
        with patch("tools.n64.xiso.MAX_DIRECTORY_BYTES", SECTOR):
            with self.assertRaisesRegex(ValueError, "total directory bytes"):
                XboxDisc(path)
        with patch("tools.n64.xiso.MAX_ENTRIES", 1):
            with self.assertRaisesRegex(ValueError, "entry count"):
                XboxDisc(path)

    def test_truncation_after_parse_fails_before_output_creation(self):
        path = self.image()
        disc = XboxDisc(path)
        self.addCleanup(disc.close)
        with path.open("r+b") as stream:
            stream.truncate(36 * SECTOR)
        target = self.root / "output.map"
        with self.assertRaisesRegex(ValueError, "outside the image"):
            disc.extract("maps/a10.map", target)
        self.assertFalse(target.exists())

    def test_extract_cannot_overwrite_source_image(self):
        path = self.image()
        disc = XboxDisc(path)
        self.addCleanup(disc.close)
        with self.assertRaisesRegex(ValueError, "must differ from the source"):
            disc.extract("maps/a10.map", path)
        self.assertEqual(path.stat().st_size, 40 * SECTOR)


if __name__ == "__main__":
    unittest.main()
