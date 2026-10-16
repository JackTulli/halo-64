"""Build I4 coverage masks using the source decal's actual blend mode.

A multiply decal's neutral white means 'leave the wall alone'. Treating it as
particle brightness made that white background into an opaque colored card.
"""
import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import struct

from PIL import Image, ImageChops, ImageOps

from .xboxmap import XboxMap


def configure(definitions=None):
    """Set the explicit Invader source tree before constructing tag layouts."""
    if definitions is not None:
        os.environ["INVADER_DEFINITIONS"] = str(Path(definitions).resolve())
    from . import tagdefs, bitmaps
    directory = Path(definitions or os.environ.get("INVADER_DEFINITIONS", tagdefs.DEFINITIONS)).resolve()
    bitmaps.DEFINITIONS = directory
    bitmaps._p8_palette.cache_clear()
    return tagdefs.TagDefinitions(directory)


CELL = 32
PADDING = 2
ART = Path(__file__).resolve().parents[2] / 'assets/paintball/paint-splatter-original.png'
COVENANT = (r'effects\decals\blood splats\bitmaps\blood splat elite',
            r'effects\decals\blood splats\bitmaps\blood splat hunter')


def multiply_coverage(image):
    """Coverage is departure from multiply's neutral white, times source alpha."""
    r, g, b, alpha = image.convert('RGBA').split()
    darkest = ImageChops.darker(ImageChops.darker(r, g), b)
    return ImageChops.multiply(ImageOps.invert(darkest), alpha)


def padded_mask(coverage):
    """Fit a quantized silhouette inside a bilinear-safe transparent frame.

    Only coverage inside the existing UV square changes; the original source art,
    weapon-defined world radius, clipped geometry and runtime budgets stay intact.
    """
    coverage = coverage.convert('L')
    peak = coverage.getextrema()[1]
    if not peak:
        raise ValueError('Empty paint coverage')
    coverage = coverage.point(lambda value: min(255, value * 255 // peak))
    bounds = coverage.point(lambda value: 255 if value >= 8 else 0).getbbox()
    if not bounds:
        raise ValueError('Paint coverage has no visible silhouette')
    shape = coverage.crop(bounds)
    shape.thumbnail((CELL - 2 * PADDING, CELL - 2 * PADDING), Image.Resampling.LANCZOS)
    result = Image.new('L', (CELL, CELL), 0)
    result.paste(shape, ((CELL - shape.width) // 2, (CELL - shape.height) // 2))
    return result.point(lambda value: (value >> 4) * 17)


def first_sprite(xmap, bitmap, defs):
    fields = {field.name: offset for offset, field in defs.fields('Bitmap')}
    sequences = {field.name: offset for offset, field in defs.fields('BitmapGroupSequence')}
    sprites = {field.name: offset for offset, field in defs.fields('BitmapGroupSprite')}
    count, pointer = struct.unpack('<iI', xmap.read(bitmap.address + fields['bitmap group sequence'], 8))
    image_index, box = 0, (0., 0., 1., 1.)
    if count:
        sc, sp = struct.unpack('<iI', xmap.read(pointer + sequences['sprites'], 8))
        if sc:
            raw = xmap.read(sp, defs.size('BitmapGroupSprite'))
            image_index = struct.unpack_from('<h', raw, sprites['bitmap index'])[0]
            left, right, top, bottom = (struct.unpack_from('<f', raw, sprites[key])[0]
                                        for key in ('left', 'right', 'top', 'bottom'))
            box = left, top, right, bottom
        else:
            image_index = struct.unpack('<h', xmap.read(pointer + sequences['first bitmap index'], 2))[0]
    from .bitmaps import bitmap_images
    images = bitmap_images(xmap, bitmap, defs)
    if not 0 <= image_index < len(images):
        raise ValueError(f'Invalid paint bitmap index: {bitmap.name}')
    from .bitmaps import decode
    image = decode(xmap, images[image_index], 128)
    width, height = image.size
    return image.crop((int(box[0] * width), int(box[1] * height),
                       math.ceil(box[2] * width), math.ceil(box[3] * height)))


def validate_mask(mask):
    pixels = list(mask.getdata())
    border = [pixels[y * CELL + x] for y in range(CELL) for x in range(CELL)
              if x < PADDING or x >= CELL - PADDING or y < PADDING or y >= CELL - PADDING]
    visible = sum(value > 0 for value in pixels)
    if any(border) or not 40 <= visible <= CELL * CELL * .65 or max(pixels) < 204:
        raise ValueError(f'Invalid paint silhouette: {visible} visible texels, border peak {max(border)}')
    return {'visible_texels': visible, 'transparent_texels': pixels.count(0),
            'border_peak': max(border), 'peak': max(pixels)}


def build_masks(source_map, art=ART, definitions=None):
    xmap = XboxMap(source_map)
    defs = configure(definitions)
    art = Path(art)
    fields = {field.name: offset for offset, field in defs.fields('Decal')}
    masks = [padded_mask(Image.open(art).convert('RGBA').getchannel('A'))]
    sources = [{'source': art.as_posix(), 'coverage': 'original artwork alpha',
                'sha256': hashlib.sha256(art.read_bytes()).hexdigest()}]
    for name in COVENANT:
        tag = next(tag for tag in xmap.tags if tag.group_name == 'bitm' and tag.name == name)
        materials = [decal for decal in xmap.tags if decal.group_name == 'deca'
                     and xmap.tag(struct.unpack('<I', xmap.read(decal.address + fields['map'] + 12, 4))[0]) is tag]
        blends = [struct.unpack('<h', xmap.read(decal.address + fields['framebuffer blend function'], 2))[0]
                  for decal in materials]
        if not blends or any(blend != 1 for blend in blends):
            raise ValueError(f'Paint source no longer uses multiply: {name}: {blends}')
        masks.append(padded_mask(multiply_coverage(first_sprite(xmap, tag, defs))))
        sources.append({'source': name, 'coverage': '(255 - min(R,G,B)) * alpha / 255',
                        'framebuffer_blend': 'multiply', 'materials': [material.name for material in materials]})
    return masks, sources


def pack_i4(masks):
    output = bytearray()
    for mask in masks:
        validate_mask(mask)
        pixels = list(mask.getdata())
        output.extend(((pixels[p] >> 4) << 4) | (pixels[p + 1] >> 4) for p in range(0, len(pixels), 2))
    if len(output) != 1536:
        raise ValueError('Paint masks exceeded the existing 1536-byte budget')
    return bytes(output)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--map", dest="source_map", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--art", type=Path, default=ART)
    parser.add_argument("--definitions", type=Path)
    parser.add_argument("--audit", type=Path)
    args = parser.parse_args()
    masks, sources = build_masks(args.source_map, args.art, args.definitions)
    output = pack_i4(masks)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(output)
    if args.audit:
        args.audit.mkdir(parents=True, exist_ok=True)
        for i, mask in enumerate(masks):
            mask.save(args.audit / f"mask-{i}.png")
            sources[i]["quantized"] = validate_mask(mask)
        (args.audit / "source.json").write_text(json.dumps({"patterns": sources, "bytes": len(output),
            "size": [CELL, CELL], "transparent_padding": PADDING,
            "sha256": hashlib.sha256(output).hexdigest()}, indent=2))
    print(f"Paint masks: {len(output)} bytes; original splatter plus material-correct Covenant silhouettes")


if __name__ == "__main__":
    main()
