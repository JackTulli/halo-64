"""Converting the HUD's bitmaps for the N64 (the game's own HUD draws them,
port/n64/game/n64_hud_game.c).

Every image of every bitmap a HUD tag uses (unit, weapon and grenade HUD
interfaces, the HUD globals, HUD numbers) becomes a sprite at half its size
(the game lays the HUD out on 640x480; the N64 shows 320x240), and a second
one at its full width and half its height, for a HUD 640 wide with 240
lines (high detail's: n64_hud_set_wide), in IA8
(intensity and alpha: the HUD's bitmaps are grey, tinted by the HUD's
colours), IA16 for the meters (intensity = each texel's fill order, alpha =
the shape; 4 bits are too coarse) or RGBA16 for the rare coloured one. The
HUD's bitmaps are premultiplied; all but the meters are stored straight. The image's bitmap data
(in the converted tags) carries the sprite's number + 1 in pixels_offset:
rom:/hud/<hash>.sprite, named in number order by rom:/hud/<map>.list (the
full-width ones by rom:/hud/<map>-wide.list, in the same order, after a line
"#<bytes>": the memory they all load into, for the game's room check).

Each image of more than 256x256 texels -- the PC HUD's 512x512 unit
meters, ammo meters and ammo glyphs, mostly empty -- also gets a sprite for
each of its
bitmap's sprite rectangles, cropped from its own sprites (the same texels,
one texel of margin): numbered after the images, named at the end of both
lists, and placed by rom:/hud/<map>.subs, a line "<image> <sprite> <s0>
<t0> <s1> <t1>" each (the half-size set's texels). The HUD draws a quad
inside such a rectangle from that sprite (n64_hud.c): a weapon's bullet row
needs a few KB, not its image's 128 (pack). On a map with such images (the
PC HUD's) the HUD's digits image and the motion sensor's images, drawn on
every frame, each get one more: the digits
cropped to their glyphs' rectangles, the motion sensor's (drawn whole, no
rectangles) to what is not clear, where that is at most half the image (a
line's seventh field, 1: the sprite the image is drawn from).
"""

import math
import struct
from struct import unpack as struct_unpack
import subprocess
import sys
from pathlib import Path

from PIL import Image

from . import toolpaths
from .bitmaps import bitmap_images, decode
from .level import MKSPRITE, share_file, write_list
from .walk import Walker

HUD_GROUPS = ("unhi", "wphi", "grhi", "hudg", "hud#")
# an image of more texels than this is packed (pack): the Xbox maps' HUD
# images have no more, the PC HUD's 512x512 ones four times as many
PACKED_TEXELS = 256 * 256
# the packed sprites the HUD reads a map (n64_hud.c MAXIMUM_PACKED_SPRITES):
# more would be ignored there
PACKED_SPRITES = 64


def crop_box(left, top, right, bottom, width, height):
    """a sprite rectangle (an image's texels) as a packed sprite: its box in
    the half-size image's texels, one texel of margin (s0, t0, s1, t1: the
    quads the game draws in it lie inside, an odd-sized image's last half
    texel too), and the crop of the half-size image (width // 2 by
    height // 2) it is cut from"""
    s0, t0 = max(0, math.floor(left / 2) - 1), max(0, math.floor(top / 2) - 1)
    s1 = min(math.ceil(width / 2), math.ceil(right / 2) + 1)
    t1 = min(math.ceil(height / 2), math.ceil(bottom / 2) + 1)
    return (s0, t0, s1, t1), (s0, t0, min(width // 2, s1), min(height // 2, t1))
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


# the source formats of a meter bitmap that keep the meter's fill order in
# intensity and its shape in alpha (the Xbox maps' meters): what
# n64_hud.c's meter_surface reads from the IA16 sprite
METER_INTENSITY_FORMATS = ("a8y8", "ay8", "y8")


def meter_channels(img, source_format):
    """a meter image with the fill order in intensity and the shape in
    alpha. The PC version's meter bitmaps (DXT5, A8R8G8B8) keep the fill
    order in alpha and the shape in colour, the reverse of the Xbox
    bitmaps': for those, alpha becomes the intensity and the colour's
    luminance the alpha"""
    if source_format in METER_INTENSITY_FORMATS:
        return img
    rgba = img.convert("RGBA")
    order = rgba.getchannel("A")
    shape = rgba.convert("L")
    return Image.merge("RGBA", (order, order, order, shape))


def sprite_bytes(path):
    """the memory a sprite loads into, as the game counts it
    (n64_hud.c sprite_bytes): a compressed asset's original size and
    in-place margin (its "DCA" header's LEB128 fields), else the file's size"""
    data = Path(path).read_bytes()[:16]
    if data[:3] != b"DCA":
        return Path(path).stat().st_size
    fields, p = [], 5
    for _ in range(3):
        value = shift = 0
        while p < len(data):
            byte = data[p]
            p += 1
            value |= (byte & 0x7F) << shift
            shift += 7
            if not byte & 0x80:
                break
        fields.append(value)
    return fields[1] + fields[2]


def halve(img, premultiplied, full_width=False):
    """an image at half size (full_width: half its height alone), each texel
    its area's mean (Lanczos rang: its undershoot, divided by a small alpha,
    left dark texels along every thin outline). Premultiplied channels are
    averaged each on its own; a meter's fill order is weighted by its shape
    (PIL weights colour by alpha)"""
    size = (img.width if full_width else max(1, img.width // 2), max(1, img.height // 2))
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
        self.wide_names = []
        # (number, image, meter, rectangles in its texels) of each image
        # pack may crop
        self.large = []
        # (number, image, meter, box in its texels) of the digits' and the
        # motion sensor's images, which pack may crop to one box
        self.whole = []
        # a bitmap tag's index -> its images' sprite numbers
        self.numbers_of = {}

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
        # (the damage arrows' images: drawn from one sprite rectangle of a
        # large image, cropped to it like the large images, on the maps that
        # pack -- those with large images: the PC maps; the Xbox maps' lists
        # stay as they were)
        arrows = []
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
                if "meter" in tag.name:
                    img = meter_channels(img, image.format)
                number = self.count
                if img.width * img.height > PACKED_TEXELS:
                    self.large.append((number, img, "meter" in tag.name, self.rectangles(tag, i, img)))
                elif "damage_arrows" in tag.name:
                    arrows.append((number, img, False, self.rectangles(tag, i, img)))
                self.numbers_of.setdefault(tag.index, {})[i] = (number, img, "meter" in tag.name, tag)
                self.write(img, out / f"{number}.png", "meter" in tag.name)
                self.names.append(share_file(out / f"{number}.sprite", self.out_dir / "hud", ".sprite"))
                self.write(img, out / f"{number}w.png", "meter" in tag.name, full_width=True)
                self.wide_names.append(share_file(out / f"{number}w.sprite", self.out_dir / "hud", ".sprite"))
                self.sprite_of[pointer + i * size] = number
                self.count += 1
        if self.large:
            self.large.extend(arrows)
        self.find_whole()
        hud_dir = self.out_dir / "hud"
        self.wide_bytes = sum(sprite_bytes(hud_dir / f"{name}.sprite") for name in self.wide_names)
        self.write_lists()
        (hud_dir / f"{self.map_name}.subs").unlink(missing_ok=True)
        images = self.count
        self.pack()
        return images

    def write_lists(self):
        hud_dir = self.out_dir / "hud"
        write_list(hud_dir / f"{self.map_name}.list", self.names)
        # (the full-width set's memory as without the packed sprites: the
        # set is chosen as before)
        (hud_dir / f"{self.map_name}-wide.list").write_text(
            f"#{self.wide_bytes}\n" + "".join(f"{n}\n" for n in self.wide_names), newline="\n")

    def find_whole(self):
        """the digits' images (the HUD numbers' digits bitmap: the box of its
        sprite rectangles) and the motion sensors' (each unit HUD's
        background and foreground sequence, drawn whole: the box of what is
        not clear)"""
        def dependency(tag, field, struct):
            offset = {fl.name: o for o, fl in self.defs.fields(struct)}[field]
            datum = struct_unpack("<I", self.map.read(tag.address + offset + 12, 4))[0]
            target = self.map.tag(datum) if datum != 0xFFFFFFFF else None
            return target.index if target is not None and target.index in self.numbers_of else None

        def add(number, img, meter, box):
            if box and all(number != w[0] for w in self.whole):
                self.whole.append((number, img, meter, box))

        for tag in self.map.tags:
            if tag.group_name == "hud#":
                index = dependency(tag, "digits bitmap", "HUDNumber")
                for i, (number, img, meter, bitmap) in sorted((self.numbers_of.get(index) or {}).items()):
                    boxes = self.rectangles(bitmap, i, img)
                    if boxes:
                        add(number, img, meter, (min(b[0] for b in boxes), min(b[1] for b in boxes),
                                                 max(b[2] for b in boxes), max(b[3] for b in boxes)))
            elif tag.group_name == "unhi":
                uf = {fl.name: o for o, fl in self.defs.fields("UnitHUDInterface")}
                for part in ("motion sensor background", "motion sensor foreground"):
                    index = dependency(tag, part + " interface bitmap", "UnitHUDInterface")
                    if index is None:
                        continue
                    sequence = struct_unpack("<h", self.map.read(tag.address + uf[part + " sequence index"], 2))[0]
                    bitmap = next(iter(self.numbers_of[index].values()))[3]
                    for i in self.sequence_images(bitmap, sequence):
                        number, img, meter, _ = self.numbers_of[index].get(i, (None,) * 4)
                        if number is not None:
                            add(number, img, meter, img.convert("RGBA").split()[3].getbbox())

    def sequence_images(self, tag, sequence):
        """the image indices a bitmap's sequence draws whole (no sprite
        rectangles)"""
        bf = {fl.name: o for o, fl in self.defs.fields("Bitmap")}
        sf = {fl.name: o for o, fl in self.defs.fields("BitmapGroupSequence")}
        count, pointer = struct_unpack("<iI", self.map.read(tag.address + bf["bitmap group sequence"], 8))
        images = struct_unpack("<i", self.map.read(tag.address + bf["bitmap data"], 4))[0]
        if not 0 <= sequence < count:
            return []
        address = pointer + sequence * self.defs.size("BitmapGroupSequence")
        first, n = struct_unpack("<hH", self.map.read(address + sf["first bitmap index"], 4))
        sprites = struct_unpack("<i", self.map.read(address + sf["sprites"], 4))[0]
        # (a sequence with no bitmaps, or past the bitmap's images -- some
        # Custom Edition maps have them -- draws none of them whole)
        if sprites > 0 or first < 0 or n == 0 or first + n > images:
            return []
        return list(range(first, first + n))

    def rectangles(self, tag, index, img):
        """the distinct sprite rectangles of a bitmap's sequences on its image
        index, in the image's texels (left, top, right, bottom)"""
        bf = {fl.name: o for o, fl in self.defs.fields("Bitmap")}
        sf = {fl.name: o for o, fl in self.defs.fields("BitmapGroupSequence")}
        pf = {fl.name: o for o, fl in self.defs.fields("BitmapGroupSprite")}
        count, pointer = struct.unpack("<iI", self.map.read(tag.address + bf["bitmap group sequence"], 8))
        out = []
        for k in range(max(count, 0)):
            sequence = pointer + k * self.defs.size("BitmapGroupSequence")
            n, sprites = struct.unpack("<iI", self.map.read(sequence + sf["sprites"], 8))
            for j in range(max(n, 0)):
                sprite = sprites + j * self.defs.size("BitmapGroupSprite")
                bitmap = struct.unpack("<h", self.map.read(sprite + pf["bitmap index"], 2))[0]
                left, right, top, bottom = struct.unpack("<ffff", self.map.read(sprite + pf["left"], 16))
                box = (left * img.width, top * img.height, right * img.width, bottom * img.height)
                # (a rectangle under one half-size texel, or not a number --
                # corrupt sprite data in some Custom Edition maps: zeros,
                # denormals -- is no sprite's)
                if (bitmap == index and all(math.isfinite(v) for v in box) and box[2] - box[0] >= 2 and
                        box[3] - box[1] >= 2 and box not in out):
                    out.append(box)
        return out

    def pack(self):
        """a sprite for each sprite rectangle of each image of more than
        PACKED_TEXELS texels, and, on a map with such images, one for the box
        of each digits' and motion sensor's image where the box is at most half
        of it (both sets), named after the images in both lists and placed by
        <map>.subs; the number of sprites made"""
        if not self.large:
            return 0
        out = self.out_dir / "hud" / f".{self.map_name}"
        lines = []
        # (the images whose rectangles or box the limits left unpacked: the
        # game draws those from the whole image)
        unpacked = {}
        for number, img, meter, boxes in self.large:
            half = self.prepare(img, meter)
            wide = self.prepare(img, meter, full_width=True)
            fmt = self.format(half, meter)
            for k, box in enumerate(boxes):
                if self.count >= 256 or len(lines) >= PACKED_SPRITES:  # n64_hud.c MAXIMUM_SPRITES
                    unpacked[number] = unpacked.get(number, 0) + len(boxes) - k
                    break
                (s0, t0, s1, t1), crop = crop_box(*box, img.width, img.height)
                sub = self.count
                self.save(half.crop(crop), out / f"{sub}.png", fmt)
                self.names.append(share_file(out / f"{sub}.sprite", self.out_dir / "hud", ".sprite"))
                self.save(wide.crop((2 * crop[0], crop[1], 2 * crop[2], crop[3])), out / f"{sub}w.png", fmt)
                self.wide_names.append(share_file(out / f"{sub}w.sprite", self.out_dir / "hud", ".sprite"))
                lines.append(f"{number} {sub} {s0} {t0} {s1} {t1} 0\n")
                self.count += 1
        for number, img, meter, box in self.whole:
            half = self.prepare(img, meter)
            (s0, t0, s1, t1), crop = crop_box(*box, img.width, img.height)
            if (crop[2] - crop[0]) * (crop[3] - crop[1]) * 2 > half.width * half.height:
                continue
            if self.count >= 256 or len(lines) >= PACKED_SPRITES:
                unpacked[number] = unpacked.get(number, 0) + 1
                continue
            out_png = out / f"{self.count}.png"
            fmt = self.format(half, meter)
            self.save(half.crop(crop), out_png, fmt)
            self.names.append(share_file(out / f"{self.count}.sprite", self.out_dir / "hud", ".sprite"))
            wide = self.prepare(img, meter, full_width=True)
            self.save(wide.crop((2 * crop[0], crop[1], 2 * crop[2], crop[3])), out / f"{self.count}w.png", fmt)
            self.wide_names.append(share_file(out / f"{self.count}w.sprite", self.out_dir / "hud", ".sprite"))
            lines.append(f"{number} {self.count} {s0} {t0} {s1} {t1} 1\n")
            self.count += 1
        if unpacked:
            print(f"warning: {self.map_name}: HUD packing stopped at {len(lines)} packed sprites ({self.count} sprites; "
                  f"the limits are {PACKED_SPRITES} packed and 256 in all): {sum(unpacked.values())} rectangle(s) or "
                  f"box(es) of image(s) {', '.join(str(n) for n in sorted(unpacked))} not packed; the game draws "
                  f"them from the whole image", file=sys.stderr)
        self.write_lists()
        (self.out_dir / "hud" / f"{self.map_name}.subs").write_text("".join(lines), newline="\n")
        return len(lines)

    def write(self, img, png, meter, full_width=False):
        img = self.prepare(img, meter, full_width)
        self.save(img, png, self.format(img, meter))

    @staticmethod
    def prepare(img, meter, full_width=False):
        img = halve(img.convert("RGBA"), not meter, full_width)
        if not meter:
            img = bleed(unpremultiply(img))
        return img

    @staticmethod
    def format(img, meter):
        r, g, b, a = img.split()
        grey = max(abs(x - y) for x, y in zip(r.getextrema(), g.getextrema())) < 24 and \
            max(abs(x - y) for x, y in zip(g.getextrema(), b.getextrema())) < 24
        if meter:
            return "IA16"
        return "IA8" if grey else "RGBA16"

    @staticmethod
    def save(img, png, fmt):
        img.save(png)
        toolpaths.run([MKSPRITE, "--format", fmt, "--compress", "1", "-o", str(png.parent), str(png)],
                       check=True, capture_output=True)
        png.unlink()
