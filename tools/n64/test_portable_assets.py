"""Format/coverage tests for the portable material and verified map helpers."""
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from PIL import Image
from .compress_map import compress, decode
from .decal_bank import material_ink, pack_material
from .paint_masks import padded_mask, pack_i4, multiply_coverage


class PortableAssetTests(unittest.TestCase):
    def test_paint_masks_keep_budget_and_transparent_border(self):
        source = Image.new('L', (64, 64), 0)
        for y in range(12, 52):
            for x in range(10, 49):
                if (x-30)**2 + (y-31)**2 < 390:
                    source.putpixel((x,y),255)
        mask = padded_mask(source)
        self.assertEqual(len(pack_i4([mask]*3)),1536)
        self.assertTrue(all(mask.getpixel((x,y)) == 0 for y in range(32) for x in range(32)
                            if x<2 or y<2 or x>=30 or y>=30))
        with self.assertRaises(ValueError):
            padded_mask(Image.new('L',(32,32),0))

    def test_multiply_white_is_transparent_and_dark_ink_visible(self):
        source = Image.new('RGBA',(2,1)); source.putdata([(255,255,255,255),(15,80,160,255)])
        self.assertEqual(list(multiply_coverage(source).getdata()),[0,240])
        ink = material_ink(source,1)
        self.assertEqual(ink.getpixel((0,0))[3],0)
        self.assertEqual(ink.getpixel((1,0))[3],240)

    def test_ia8_material_budget_tint_and_silhouette(self):
        image = Image.new('RGBA',(32,32),(0,0,0,0))
        for y in range(6,26):
            for x in range(4,28):
                image.putpixel((x,y),(120,20,45,255))
        payload, preview, transparent = pack_material(image, darkens=True)
        self.assertEqual(len(payload),1040)
        # (the ink's hue, its largest channel 255, and the darkening flag)
        self.assertEqual(payload[1024],255)
        self.assertAlmostEqual(payload[1025],42,delta=1)
        self.assertAlmostEqual(payload[1026],96,delta=1)
        self.assertEqual(payload[1027],1)
        self.assertEqual(transparent,1024-480)
        # a visible texel: brightness 120/255 at full coverage
        self.assertEqual(payload[10*32+10],(7<<4)|15)
        # a transparent one takes the visible ones' brightness (no dark fringe)
        self.assertEqual(payload[0],7<<4)
        self.assertEqual(preview.getpixel((0,0))[3],0)
        self.assertEqual(preview.getpixel((10,10))[3],255)

    def test_double_multiply_ignores_the_texture_alpha(self):
        # (Halo's shader darkens by the colour alone: a plasma burn's dark
        # centre has alpha 0)
        source = Image.new('RGBA',(2,1)); source.putdata([(128,128,128,0),(20,20,20,0)])
        ink = material_ink(source,2)
        self.assertEqual(ink.getpixel((0,0))[3],0)
        self.assertEqual(ink.getpixel((1,0)),(0,0,0,215))

    def test_decoder_rejects_truncated_or_invalid_packed_data(self):
        for data in (b'',b'DCA5',b'DCA5\x15\x80',b'DCA5\x15\x01\x20\0\0\xf0'):
            with self.subTest(data=data):
                with self.assertRaises(ValueError):
                    decode(data)

    def test_failed_packer_never_replaces_the_original_map(self):
        with tempfile.TemporaryDirectory() as folder:
            path=Path(folder)/'owned map.map'; original=b'H64M'+b'protected payload'*16
            path.write_bytes(original)
            def bad_tool(command,**kwargs):
                output=Path(command[command.index('-o')+1])/path.name
                output.write_bytes(b'bad packed data')
            with patch('tools.n64.toolpaths.run',side_effect=bad_tool):
                with self.assertRaises(ValueError):
                    compress(path)
            self.assertEqual(path.read_bytes(),original)


if __name__=='__main__':
    unittest.main()
