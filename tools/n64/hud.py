"""Converting the HUD's bitmaps for the N64 (the game's own HUD draws them,
port/n64/game/n64_hud_game.c).

Every image of every bitmap a HUD tag uses (unit, weapon and grenade HUD
interfaces, the HUD globals, HUD numbers) becomes a sprite at half its size
(the game lays the HUD out on 640x480; the N64 shows 320x240), in IA8
(intensity and alpha: the HUD's bitmaps are grey, tinted by the HUD's
colours), IA16 for the meters (intensity = each texel's fill order, alpha =
the shape; 4 bits are too coarse) or RGBA16 for the rare coloured one. The
HUD's bitmaps are premultiplied; all but the meters are stored straight. The image's bitmap data
(in the converted tags) carries the sprite's number + 1 in pixels_offset:
rom:/hud/<hash>.sprite, named in number order by rom:/hud/<map>.list.
"""

import struct
import subprocess
from pathlib import Path

from PIL import Image

from . import toolpaths
from .bitmaps import bitmap_images, decode
from .level import MKSPRITE, share_file, write_list
from .walk import Walker

HUD_GROUPS = ("unhi", "wphi", "grhi", "hudg", "hud#")
# full-screen images the first milestone leaves out (the postgame report,
# the scope masks)
SKIPPED_NAMES = ("postgame_carnage_report", "scope_mask")


def unpremultiply(img):
    """the HUD blends premultiplied (colour + framebuffer * (1 - alpha)); the
    N64 blends straight, so colour / alpha (colour brighter than its alpha,
    a glow, saturates)"""
    r, g, b, a = img.split()
    alpha = a.tobytes()

    def divide(channel):
        return Image.frombytes("L", img.size, bytes(
            0 if not k else min(255, v * 255 // k) for v, k in zip(channel.tobytes(), alpha)))

    return Image.merge("RGBA", (divide(r), divide(g), divide(b), a))


def halve(img, premultiplied):
    """an image at half size, each texel its area's mean (Lanczos rang: its
    undershoot, divided by a small alpha, left dark texels along every thin
    outline). Premultiplied channels are averaged each on its own; a meter's
    fill order is weighted by its shape (PIL weights colour by alpha)"""
    size = (max(1, img.width // 2), max(1, img.height // 2))
    if not premultiplied:
        return img.resize(size, Image.BOX)
    return Image.merge("RGBA", [c.resize(size, Image.BOX) for c in img.split()])


def bleed(img, passes=2):
    """the colour of the transparent texels next to a shape: the mean of its
    neighbours' (bilinear filtering mixes each texel with its neighbours, and
    transparent black ones darkened the shapes' edges)"""
    w, h = img.size
    px = bytearray(img.tobytes())
    known = bytearray(1 if px[i * 4 + 3] else 0 for i in range(w * h))
    for _ in range(passes):
        filled = []
        for i in range(w * h):
            if known[i]:
                continue
            x, y = i % w, i // w
            r = g = b = n = 0
            for yy in range(max(0, y - 1), min(h, y + 2)):
                for xx in range(max(0, x - 1), min(w, x + 2)):
                    j = yy * w + xx
                    if known[j]:
                        r += px[j * 4]
                        g += px[j * 4 + 1]
                        b += px[j * 4 + 2]
                        n += 1
            if n:
                filled.append((i, r // n, g // n, b // n))
        for i, r, g, b in filled:
            px[i * 4:i * 4 + 3] = bytes((r, g, b))
            known[i] = 1
    return Image.frombytes("RGBA", img.size, bytes(px))


class HudExporter:
    def __init__(self, xmap, defs, map_name, out_dir):
        self.map = xmap
        self.defs = defs
        self.map_name = map_name
        self.out_dir = Path(out_dir)
        # the Xbox address of an image's bitmap data -> sprite number
        self.sprite_of = {}
        self.count = 0
        self.names = []

    def collect(self):
        bitmaps = []
        walker = Walker(self.map, self.defs)

        def on_dependency(tag, address, f, path, group, datum):
            t = self.map.tag(datum)
            if t is not None and t.group_name == "bitm" and t not in bitmaps:
                bitmaps.append(t)

        for tag in self.map.tags:
            if tag.group_name in HUD_GROUPS:
                walker.walk(tag, on_dependency=on_dependency)
        out = self.out_dir / "hud" / f".{self.map_name}"
        out.mkdir(parents=True, exist_ok=True)
        bf = {fl.name: o for o, fl in self.defs.fields("Bitmap")}
        size = self.defs.size("BitmapData")
        for tag in bitmaps:
            if any(name in tag.name for name in SKIPPED_NAMES):
                continue
            count, pointer = struct.unpack("<iI", self.map.read(tag.address + bf["bitmap data"], 8))
            images = bitmap_images(self.map, tag, self.defs)
            for i, image in enumerate(images):
                if image.type != 0:  # 2D images only
                    continue
                try:
                    img = decode(self.map, image, max(image.width, image.height))
                except ValueError:
                    continue
                number = self.count
                self.write(img, out / f"{number}.png", "meter" in tag.name)
                self.names.append(share_file(out / f"{number}.sprite", self.out_dir / "hud", ".sprite"))
                self.sprite_of[pointer + i * size] = number
                self.count += 1
        write_list(self.out_dir / "hud" / f"{self.map_name}.list", self.names)
        return self.count

    def write(self, img, png, meter):
        img = halve(img.convert("RGBA"), not meter)
        if not meter:
            img = bleed(unpremultiply(img))
        r, g, b, a = img.split()
        grey = max(abs(x - y) for x, y in zip(r.getextrema(), g.getextrema())) < 24 and \
            max(abs(x - y) for x, y in zip(g.getextrema(), b.getextrema())) < 24
        if meter:
            fmt = "IA16"
        elif grey:
            fmt = "IA8"
        else:
            fmt = "RGBA16"
        img.save(png)
        toolpaths.run([MKSPRITE, "--format", fmt, "--compress", "1", "-o", str(png.parent), str(png)],
                       check=True, capture_output=True)
        png.unlink()
