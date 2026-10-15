"""Decoding the Xbox game's bitmaps (bitmap tags' pixel data in the map).

A bitmap tag lists its images in "bitmap data"; each image's pixels are in
the cache file at its "pixel data offset", all mipmaps one after another.
DXT1/3/5 are decoded by PIL (through a DDS header); the uncompressed
formats are swizzled on the Xbox (the GPU's Morton order) unless the image
is linear, and are unswizzled here.
"""

import io
import struct
from functools import lru_cache

from PIL import Image

from .tagdefs import DEFINITIONS

FORMATS = ['a8', 'y8', 'ay8', 'a8y8', 'unused1', 'unused2', 'r5g6b5', 'unused3', 'a1r5g5b5',
           'a4r4g4b4', 'x8r8g8b8', 'a8r8g8b8', 'unused4', 'unused5', 'dxt1', 'dxt3', 'dxt5', 'p8', 'bc7']
BYTES_PER_PIXEL = {'a8': 1, 'y8': 1, 'ay8': 1, 'a8y8': 2, 'r5g6b5': 2, 'a1r5g5b5': 2, 'a4r4g4b4': 2,
                   'x8r8g8b8': 4, 'a8r8g8b8': 4, 'p8': 1}
FLAG_SWIZZLED = 1 << 3
FLAG_LINEAR = 1 << 4


@lru_cache(maxsize=1)
def _p8_palette():
    """Halo's fixed bump-normal palette (ARGB in Invader's source data).
    Its last two entries have zero alpha; treating indices as greyscale
    loses the cutouts on Xbox environment textures such as ladder grates.
    """
    path = DEFINITIONS.parents[2] / "bitmap" / "p8" / "p8_palette"
    data = path.read_bytes()
    if len(data) != 1024:
        raise ValueError("Halo P8 palette must contain 256 ARGB entries")
    return tuple(bytes((data[i + 1], data[i + 2], data[i + 3], data[i]))
                 for i in range(0, len(data), 4))


class BitmapImage:
    """one image of a bitmap tag"""

    def __init__(self, raw, data=None):
        # the cache file holding the pixels (an imported tag's own: merge.py)
        self.data = data
        (self.signature, self.width, self.height, self.depth, self.type, fmt, self.flags,
         rx, ry, self.mipmaps, self.offset, self.size) = struct.unpack_from("<4sHHHHHHhhHxxII", raw, 0)
        self.format = FORMATS[fmt] if fmt < len(FORMATS) else str(fmt)


def bitmap_images(xmap, tag, defs):
    """the images of a bitmap tag"""
    fields = {f.name: o for o, f in defs.fields("Bitmap")}
    count, pointer = struct.unpack("<iI", xmap.read(tag.address + fields["bitmap data"], 8))
    size = defs.size("BitmapData")
    source = getattr(tag, "source", None)
    data = source.data if source is not None else None
    return [BitmapImage(xmap.read(pointer + i * size, size), data) for i in range(max(count, 0))]


def _unswizzle(data, width, height, bpp):
    """Xbox swizzle: the bits of x and y interleaved, x first, while both
    dimensions last; the larger one's remaining bits on top"""
    mask_x = mask_y = 0
    i = j = 1
    while i < width or i < height:
        if i < width:
            mask_x |= j
            j <<= 1
        if i < height:
            mask_y |= j
            j <<= 1
        i <<= 1

    def spread(value, mask):
        result = 0
        bit = 1
        while mask:
            low = mask & -mask
            if value & bit:
                result |= low
            bit <<= 1
            mask &= mask - 1
        return result

    xs = [spread(x, mask_x) for x in range(width)]
    ys = [spread(y, mask_y) for y in range(height)]
    out = bytearray(width * height * bpp)
    for y in range(height):
        row = ys[y]
        for x in range(width):
            s = (row | xs[x]) * bpp
            d = (y * width + x) * bpp
            out[d:d + bpp] = data[s:s + bpp]
    return bytes(out)


def _dds(fourcc, width, height, data):
    header = struct.pack("<4sIIIIIII44xII4sIIIIIIIIII", b"DDS ", 124, 0x1007, height, width, len(data), 0, 1,
                         32, 4, fourcc, 0, 0, 0, 0, 0, 0x1000, 0, 0, 0, 0)
    return Image.open(io.BytesIO(header + data)).convert("RGBA")


def _mip_size(fmt, width, height):
    if fmt in ('dxt1',):
        return max(1, (width + 3) // 4) * max(1, (height + 3) // 4) * 8
    if fmt in ('dxt3', 'dxt5'):
        return max(1, (width + 3) // 4) * max(1, (height + 3) // 4) * 16
    return width * height * BYTES_PER_PIXEL.get(fmt, 4)


def decode(xmap, image, target=64):
    """the image as a PIL RGBA image, from the smallest mipmap that is still
    at least `target` texels on its larger side (or the full image)"""
    width, height = image.width, image.height
    offset = image.offset
    level = 0
    while level < image.mipmaps and max(width, height) // 2 >= target:
        offset += _mip_size(image.format, width, height)
        width, height = max(1, width // 2), max(1, height // 2)
        level += 1
    size = _mip_size(image.format, width, height)
    data = (image.data if image.data is not None else xmap.data)[offset:offset + size]
    fmt = image.format
    if fmt in ('dxt1', 'dxt3', 'dxt5'):
        w4, h4 = max(4, width), max(4, height)
        img = _dds({'dxt1': b"DXT1", 'dxt3': b"DXT3", 'dxt5': b"DXT5"}[fmt], w4, h4, data)
        return img.crop((0, 0, width, height))
    bpp = BYTES_PER_PIXEL.get(fmt)
    if bpp is None:
        raise ValueError(f"bitmap format {fmt} is not decoded")
    if image.flags & FLAG_SWIZZLED and not image.flags & FLAG_LINEAR:
        data = _unswizzle(data, width, height, bpp)
    if fmt in ('a8r8g8b8', 'x8r8g8b8'):
        img = Image.frombytes("RGBA", (width, height), data, "raw", "BGRA")
        if fmt == 'x8r8g8b8':
            img.putalpha(255)
        return img
    if fmt == 'r5g6b5':
        return Image.frombytes("RGB", (width, height), data, "raw", "BGR;16").convert("RGBA")
    if fmt == 'a1r5g5b5':
        return Image.frombytes("RGBA", (width, height), data, "raw", "BGRA;15")
    if fmt == 'a4r4g4b4':
        out = bytearray()
        for i in range(0, len(data), 2):
            v = data[i] | (data[i + 1] << 8)
            out += bytes((((v >> 8) & 15) * 17, ((v >> 4) & 15) * 17, (v & 15) * 17, ((v >> 12) & 15) * 17))
        return Image.frombytes("RGBA", (width, height), bytes(out))
    if fmt == 'a8':
        img = Image.new("RGBA", (width, height), (255, 255, 255, 255))
        img.putalpha(Image.frombytes("L", (width, height), data))
        return img
    if fmt == 'p8':
        palette = _p8_palette()
        return Image.frombytes("RGBA", (width, height), b"".join(palette[v] for v in data))
    if fmt == 'y8':
        return Image.frombytes("L", (width, height), data).convert("RGBA")
    if fmt == 'ay8':
        g = Image.frombytes("L", (width, height), data)
        return Image.merge("RGBA", (g, g, g, g))
    if fmt == 'a8y8':
        img = Image.frombytes("LA", (width, height), data)
        return img.convert("RGBA")
    raise ValueError(f"bitmap format {fmt} is not decoded")
