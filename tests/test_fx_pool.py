"""Private-data-free FX pooling regression tests.

Run from the repository root: python -m unittest discover -s tests -p test_fx_pool.py
"""
from pathlib import Path
import json
import struct
import tempfile
import unittest
from tools.n64 import fx_pool

def source_fx(tag_base=10, tint=255):
    """Two logical effects with identical synthetic one-emitter bodies."""
    body = struct.pack('>4H', 1, 1, 0, 1) + bytes(80)
    padded = body + bytes(8)
    atlas = 48
    start = atlas + fx_pool.ATLAS_BYTES
    explosion = start + 2 * len(padded)
    header = bytearray(32)
    struct.pack_into('>4sIHHIIIH', header, 0, b'N64F', 2, 2, 1, 32, atlas, explosion, 1)
    index = b''.join(struct.pack('>HHI', tag_base + i, len(body), start + i * len(padded)) for i in range(2))
    colours = bytes([tint, 10, 20, 255]) * 32
    return bytes(header) + index + bytes(4096) + colours + padded * 2 + struct.pack('>HBB', tag_base, 1, 0)

class FXPoolTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.base = Path(self.temp.name)
        self.source = self.base / 'raw'
        (self.source / 'fx').mkdir(parents=True)
        self.maps = {'alpha': source_fx(), 'beta': source_fx(20, 128)}
        for name, raw in self.maps.items():
            (self.source / 'fx' / (name + '.fx')).write_bytes(raw)
        (self.source / 'other.bin').write_bytes(b'unchanged content\0')
        self.output = self.base / 'packed'

    def prepare(self):
        return fx_pool.prepare_filesystem(self.source, self.output, list(self.maps))

    def test_all_logical_bodies_and_distinct_map_metadata_are_preserved(self):
        made, bank, proof = fx_pool.pool_maps(self.maps)
        self.assertEqual((proof['logical_records'], proof['unique_records']), (4, 1))
        self.assertEqual(len(bank), 128)
        for name in self.maps:
            self.assertEqual(fx_pool.verify_map(self.maps[name], made[name], bank)['status'], 'EXACT')
        self.assertNotEqual(made['alpha'], made['beta'])
        self.assertEqual((made, bank, proof), fx_pool.pool_maps(dict(reversed(list(self.maps.items())))))

    def test_source_nonfx_and_noop_mtimes(self):
        before = {p: (p.read_bytes(), p.stat().st_mtime_ns) for p in self.source.rglob('*') if p.is_file()}
        receipt = self.prepare()
        self.assertTrue(receipt['source_unchanged'])
        self.assertEqual((self.output/'other.bin').read_bytes(), b'unchanged content\0')
        tracked = [p for p in self.base.rglob('*') if p.is_file()]
        times = {p: p.stat().st_mtime_ns for p in tracked}
        self.assertEqual(receipt, self.prepare())
        self.assertEqual(times, {p: p.stat().st_mtime_ns for p in tracked})
        self.assertEqual(before, {p: (p.read_bytes(), p.stat().st_mtime_ns) for p in before})

    def test_ninja_empty_directory_and_durable_clean_repair(self):
        (self.output/'fx').mkdir(parents=True)
        receipt = self.prepare()
        for p in self.output.rglob('*'):
            if p.is_file():p.unlink()
        self.output.with_name('packed.fx-pool.json').unlink()
        self.output.with_name('packed.fx-pool.stamp').unlink()
        self.assertTrue(self.output.with_name('packed.fx-pool.owner.json').is_file())
        self.assertEqual(self.prepare()['output_files'], receipt['output_files'])

    def test_edited_or_unknown_outputs_are_retained_and_refused(self):
        self.prepare()
        path = self.output/'other.bin'
        path.write_bytes(b'user edit')
        with self.assertRaises(ValueError):self.prepare()
        self.assertEqual(path.read_bytes(), b'user edit')
        path.write_bytes(b'unchanged content\0')
        extra = self.output/'unknown'
        extra.write_bytes(b'user file')
        with self.assertRaises(ValueError):self.prepare()
        self.assertEqual(extra.read_bytes(), b'user file')

    def test_receipt_accepts_equivalent_absolute_path_spelling(self):
        self.prepare()
        owner = self.output.with_name('packed.fx-pool.owner.json')
        receipt = json.loads(owner.read_text())
        # Forward slashes are also valid on Windows, where native str(Path)
        # uses backslashes. A dot component exercises normalization elsewhere.
        receipt['packed_fs'] = self.output.parent.as_posix() + '/./' + self.output.name
        owner.write_text(json.dumps(receipt))
        self.assertTrue(self.prepare()['source_unchanged'])

    def test_receipt_rejects_different_relative_or_malformed_roots(self):
        self.prepare()
        owner = self.output.with_name('packed.fx-pool.owner.json')
        receipt = json.loads(owner.read_text())
        for root in (str(self.base/'different'), 'packed', '', None, 42):
            with self.subTest(root=root):
                receipt['packed_fs'] = root
                owner.write_text(json.dumps(receipt))
                with self.assertRaisesRegex(ValueError, 'ownership receipt mismatch'):
                    self.prepare()
                self.assertEqual((self.output/'other.bin').read_bytes(), b'unchanged content\0')

    def test_unowned_nonempty_or_overlapping_output_is_refused(self):
        self.output.mkdir();(self.output/'keep').write_bytes(b'x')
        with self.assertRaises(ValueError):self.prepare()
        for output in (self.source, self.source/'child', self.base):
            with self.assertRaises(ValueError):fx_pool.prepare_filesystem(self.source, output, list(self.maps))
        with self.assertRaises(ValueError):fx_pool.prepare_filesystem(self.source, self.base/'fresh', ['alpha'])

    def test_bank_corruption_and_padded_index_overrun_are_rejected(self):
        made, bank, _ = fx_pool.pool_maps(self.maps)
        bad = bytearray(bank);bad[-1]^=1
        with self.assertRaises(ValueError):fx_pool.verify_bank(bad)
        with self.assertRaises(ValueError):fx_pool.verify_bank(bank[:-1])
        bad = bytearray(made['alpha']);struct.pack_into('>I', bad, 36, len(bank)-16)
        with self.assertRaises(ValueError):fx_pool.verify_map(self.maps['alpha'], bad, bank)

    def test_pooled_or_invalid_record_input_is_rejected(self):
        bad = bytearray(self.maps['alpha']);struct.pack_into('>I', bad, 4, 3)
        with self.assertRaises(ValueError):fx_pool.parse_map(bad)
        bad = bytearray(self.maps['alpha']);offset=struct.unpack_from('>I', bad, 36)[0]
        struct.pack_into('>H', bad, offset+6, 2)
        with self.assertRaises(ValueError):fx_pool.parse_map(bad)

if __name__ == '__main__':
    unittest.main()
