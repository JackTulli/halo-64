"""Synthetic tag files for the Custom Edition repairs; no game data or external tools required.

The layouts are Invader's definitions (INVADER_DEFINITIONS, as the converter reads them); without them the tests
are skipped.
"""
import struct
import tempfile
import unittest
import wave
import zlib
from unittest import mock
from pathlib import Path
from types import SimpleNamespace

from tools.n64 import ce_prepare
from tools.n64.tagdefs import DEFINITIONS, TagDefinitions
from tools.n64.tagfile import Block, TagFile

try:
    import PIL  # noqa: F401 (bitmaps.py decodes with PIL)
    HAVE_PIL = True
except ImportError:
    HAVE_PIL = False


def tag_file(defs, group, fourcc, root, children):
    """a tag file's bytes: its header (its checksum as Invader writes it), the root struct (fields set by name) and
    what follows it"""
    body = struct_bytes(defs, group, root) + children
    header = bytearray(64)
    header[36:40] = fourcc
    struct.pack_into(">I", header, 40, ~zlib.crc32(body) & 0xFFFFFFFF)
    struct.pack_into(">IHH", header, 44, 64, 1, 255)
    header[60:64] = b"blam"
    return bytes(header) + body


def struct_bytes(defs, name, values):
    raw = bytearray(defs.size(name))
    layout = {f.name: (offset, f) for offset, f in defs.fields(name)}
    for key, value in values.items():
        offset, f = layout[key]
        code = {"float": "f"}.get(f.type, {2: "H", 4: "I"}[f.size if f.kind == "prim" else 4])
        struct.pack_into((">" if f.raw.get("endian") != "little" else "<") + code, raw, offset, value)
    return bytes(raw)


def sound(defs, sound_format, permutations):
    """a one-pitch-range sound tag: [(format, samples)] permutations, mono 22,050 Hz"""
    pitch = struct_bytes(defs, "SoundPitchRange", {"permutations": len(permutations)})
    perms = b"".join(struct_bytes(defs, "SoundPermutation", {"format": f, "samples": len(s),
                                                                "buffer size": len(s) if f == 0 else 0})
                     for f, s in permutations)
    return tag_file(defs, "Sound", b"snd!", {"format": sound_format, "pitch ranges": 1, "sound class": 4},
                    pitch + perms + b"".join(s for _, s in permutations))


@unittest.skipUnless(DEFINITIONS.is_dir(), "Invader's tag definitions (INVADER_DEFINITIONS) not found")
class CERepairTests(unittest.TestCase):
    def setUp(self):
        self.defs = TagDefinitions()
        self.work = tempfile.TemporaryDirectory()
        root = Path(self.work.name)
        self.space = SimpleNamespace(tags=root / "tags", audio=root / "audio")
        self.space.tags.mkdir()

    def tearDown(self):
        self.work.cleanup()

    def write(self, name, data):
        path = self.space.tags / name
        path.write_bytes(data)
        return path

    def test_tag_file_written_back_byte_for_byte(self):
        path = self.write("a.sound", sound(self.defs, 1, [(0, bytes(range(200)) * 2), (1, bytes(72))]))
        before = path.read_bytes()
        tag = TagFile(path, self.defs)
        tag.root.blocks("pitch ranges")[0].blocks("permutations")[1].set_data("samples", bytes(36))
        tag.save()
        changed = path.read_bytes()
        self.assertEqual(struct.unpack_from(">I", changed, 40)[0], ~zlib.crc32(changed[64:]) & 0xFFFFFFFF)
        TagFile(path, self.defs).save()
        self.assertEqual(path.read_bytes(), changed)
        path.write_bytes(before)
        TagFile(path, self.defs).save()
        self.assertEqual(path.read_bytes(), before)

    def test_permutations_of_another_format_made_pcm(self):
        pcm = struct.pack(">4h", 1000, -1000, 2000, -2000)
        path = self.write("a.sound", sound(self.defs, 1, [(0, pcm)]))
        whole, text = ce_prepare.repair_sound(self.space, "a.sound")
        self.assertTrue(whole, text)
        root = TagFile(path, self.defs).root
        permutation = root.blocks("pitch ranges")[0].blocks("permutations")[0]
        self.assertEqual((root.get("format"), permutation.get("format")), (0, 0))
        self.assertEqual(permutation.data("samples"), pcm)
        self.assertEqual(permutation.get("buffer size"), len(pcm))

    def test_xbox_adpcm_decoded_to_big_endian_pcm_beside_pcm(self):
        block = struct.pack("<hBB", 1234, 0, 0) + bytes(32)
        path = self.write("a.sound", sound(self.defs, 0, [(0, struct.pack(">h", 7)), (1, block)]))
        whole, text = ce_prepare.repair_sound(self.space, "a.sound")
        self.assertTrue(whole, text)
        permutations = TagFile(path, self.defs).root.blocks("pitch ranges")[0].blocks("permutations")
        decoded = permutations[1].data("samples")
        self.assertEqual(permutations[1].get("format"), 0)
        self.assertEqual(len(decoded), 64 * 2)
        self.assertEqual(struct.unpack_from(">h", decoded)[0], 1234)

    def test_bad_size_cut_to_the_last_whole_block(self):
        path = self.write("a.sound", sound(self.defs, 1, [(1, bytes(36 * 3 + 12))]))
        whole, text = ce_prepare.repair_sound(self.space, "a.sound")
        self.assertTrue(whole, text)
        permutation = TagFile(path, self.defs).root.blocks("pitch ranges")[0].blocks("permutations")[0]
        self.assertEqual((permutation.get("format"), len(permutation.data("samples"))), (1, 108))

    def test_ima_adpcm_left_to_the_stand_in(self):
        self.write("a.sound", sound(self.defs, 1, [(2, bytes(36))]))
        whole, text = ce_prepare.repair_sound(self.space, "a.sound")
        self.assertFalse(whole)
        self.assertIn("IMA ADPCM", text)

    def test_failed_ogg_decode_left_to_the_stand_in(self):
        def decode_ogg(ogg, destination):
            destination.write_bytes(b"RIFF")
            raise ce_prepare.CEPrepareError("ogg2wav failed (1)")
        self.space.decode_ogg = decode_ogg
        self.write("a.sound", sound(self.defs, 0, [(0, bytes(4)), (3, b"OggS" + bytes(60))]))
        whole, text = ce_prepare.repair_sound(self.space, "a.sound")
        self.assertFalse(whole)
        self.assertIn("ogg2wav failed", text)
        self.assertFalse((self.space.audio / "decoded" / "1.wav").exists())

    def test_ogg_stream_of_other_channels_left_to_the_stand_in(self):
        def decode_ogg(ogg, destination):
            with wave.open(str(destination), "wb") as out:
                out.setnchannels(2)
                out.setsampwidth(2)
                out.setframerate(22050)
                out.writeframes(bytes(8))
        self.space.decode_ogg = decode_ogg
        self.write("a.sound", sound(self.defs, 0, [(0, bytes(4)), (3, b"OggS" + bytes(60))]))
        whole, text = ce_prepare.repair_sound(self.space, "a.sound")
        self.assertFalse(whole)
        self.assertIn("2 channels", text)
        self.assertFalse((self.space.audio / "decoded" / "1.wav").exists())

    def test_unreadable_sound_left_to_the_stand_in(self):
        data = bytearray(sound(self.defs, 1, [(0, bytes(8))]))
        self.write("a.sound", bytes(data[:-3]))
        whole, text = ce_prepare.repair_sound(self.space, "a.sound")
        self.assertFalse(whole)
        self.assertIn("TagFileError", text)

    def bitmap(self, width, height, mipmaps, pixels):
        image = struct_bytes(self.defs, "BitmapData", {"width": width, "height": height, "depth": 1, "type": 0,
                                                        "format": 11, "flags": 1, "mipmap count": mipmaps})
        return self.write("a.bitmap", tag_file(self.defs, "Bitmap", b"bitm",
                                               {"processed pixel data": len(pixels), "bitmap data": 1},
                                               pixels + image))

    @unittest.skipUnless(HAVE_PIL, "PIL not installed")
    def test_wide_image_halved_each_channel_alone(self):
        # (4096x2 A8R8G8B8: the pixel pairs average to B 10, G 20, R 30 with an alpha of 1)
        row = (bytes((0, 0, 0, 0)) + bytes((20, 40, 60, 2))) * 2048
        path = self.bitmap(4096, 2, 0, row * 2)
        shrunk, text = ce_prepare.shrink_bitmap(self.space, "a.bitmap")
        self.assertTrue(shrunk, text)
        root = TagFile(path, self.defs).root
        image = root.blocks("bitmap data")[0]
        self.assertEqual((image.get("width"), image.get("height"), image.get("format")), (2048, 1, 11))
        self.assertEqual(root.data("processed pixel data"), bytes((10, 20, 30, 1)) * 2048)

    @unittest.skipUnless(HAVE_PIL, "PIL not installed")
    def test_larger_mipmaps_dropped(self):
        sizes = [(4096 >> m) * max(1, 4 >> m) * 4 for m in range(3)]
        pixels = b"".join(bytes([m]) * size for m, size in enumerate(sizes))
        path = self.bitmap(4096, 4, 2, pixels)
        shrunk, text = ce_prepare.shrink_bitmap(self.space, "a.bitmap")
        self.assertTrue(shrunk, text)
        root = TagFile(path, self.defs).root
        image = root.blocks("bitmap data")[0]
        self.assertEqual((image.get("width"), image.get("height"), image.get("mipmap count")), (2048, 2, 1))
        self.assertEqual(root.data("processed pixel data"), pixels[sizes[0]:])

    @unittest.skipUnless(HAVE_PIL, "PIL not installed")
    def test_bitmap_that_will_not_decode_left_to_the_stand_in(self):
        # (the decoder fails as it does without the P8 palette's file)
        self.bitmap(4096, 2, 0, bytes(4096 * 2 * 4))
        with mock.patch("tools.n64.bitmaps.decode", side_effect=OSError("no palette")):
            shrunk, text = ce_prepare.shrink_bitmap(self.space, "a.bitmap")
        self.assertFalse(shrunk)
        self.assertIn("no palette", text)

    def element(self, name, values=None):
        return Block(self.defs, name, struct_bytes(self.defs, name, values or {}))

    def new_tag(self, name, group, fourcc):
        path = self.space.tags / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(tag_file(self.defs, group, fourcc, {}, b""))
        return TagFile(path, self.defs)

    def test_bsp_with_the_spawns_kept_and_indices_moved(self):
        # (two BSPs, the second around both spawns: it is kept, as the first; the first BSP erased)
        for name, low, high in (("bsp_a", 0.0, 10.0), ("bsp_b", 100.0, 200.0)):
            bsp = self.new_tag(f"levels/test/{name}.scenario_structure_bsp", "ScenarioStructureBSP", b"sbsp")
            for axis in "xyz":
                struct.pack_into(">2f", bsp.root.raw, bsp.root.layout[f"world bounds {axis}"][0], low, high)
            bsp.save()
        scenario = self.new_tag("levels/test/test.scenario", "Scenario", b"scnr")
        root = scenario.root
        bsps = []
        for name in ("bsp_a", "bsp_b"):
            element = self.element("ScenarioBSP")
            offset = element.layout["structure bsp"][0]
            path = f"levels\\test\\{name}".encode()
            element.raw[offset:offset + 4] = b"sbsp"
            struct.pack_into(">I", element.raw, offset + 8, len(path))
            element.tails[offset] = path + b"\0"
            bsps.append(element)
        root.tails[root.layout["structure bsps"][0]] = bsps
        spawns = []
        for index in (1, 0):
            spawn = self.element("ScenarioPlayerStartingLocation", {"bsp index": index})
            struct.pack_into(">3f", spawn.raw, spawn.layout["position"][0], 150.0, 150.0, 150.0)
            spawns.append(spawn)
        root.tails[root.layout["player starting locations"][0]] = spawns
        root.tails[root.layout["encounters"][0]] = [self.element("ScenarioEncounter", {"manual bsp index": i})
                                                    for i in (1, 0, 0xFFFF)]
        root.tails[root.layout["command lists"][0]] = [self.element("ScenarioCommandList", {"manual bsp index": 1})]
        scenario.save()
        edits, lines = [], []
        self.space.edit = lambda tag, *args: edits.append((tag, *args))
        ce_prepare.keep_one_bsp(self.space, "levels\\test\\test", lines.append)
        tag = "levels/test/test.scenario"
        self.assertEqual(edits, [(tag, "-E", "structure_bsps[0-0]"),
                                 (tag, "-S", "player_starting_locations[0].bsp_index", "0"),
                                 (tag, "-S", "encounters[0].manual_bsp_index", "0"),
                                 (tag, "-S", "encounters[1].manual_bsp_index", "1"),
                                 (tag, "-S", "command_lists[0].manual_bsp_index", "0")])
        self.assertEqual(lines[-1], "kept structure BSP #1 of 2 (levels/test/bsp_b)")

    def test_one_bsp_left_alone(self):
        scenario = self.new_tag("levels/test/test.scenario", "Scenario", b"scnr")
        scenario.root.tails[scenario.root.layout["structure bsps"][0]] = [self.element("ScenarioBSP")]
        scenario.save()
        edits = []
        self.space.edit = lambda tag, *args: edits.append((tag, *args))
        ce_prepare.keep_one_bsp(self.space, "levels\\test\\test", print)
        self.assertEqual(edits, [])


if __name__ == "__main__":
    unittest.main()
