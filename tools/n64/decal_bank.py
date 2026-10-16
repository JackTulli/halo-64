"""Export owned Halo decal artwork as a bounded N64 CI4 material bank.

The stock port omitted native decals. This bank keeps the source silhouettes
and ink colors; Xbox multiply/max/add are approximated with alpha-over ink.
No map tag bytes, damage, projectile settings, or source pixels are modified.
"""
import argparse
import hashlib
import json
import math
from pathlib import Path
import struct

from PIL import Image

from .paint_masks import configure

CELL = 32
PAYLOAD = CELL * CELL // 2 + 32
BAYER = ((0, 8, 2, 10), (12, 4, 14, 6), (3, 11, 1, 9), (15, 7, 13, 5))


def name_hash(name):
    value = 2166136261
    for byte in name.encode('ascii'):
        value = ((value ^ byte) * 16777619) & 0xffffffff
    return value


def sprite_image(xmap, bitmap, defs):
    fields = {f.name: o for o, f in defs.fields('Bitmap')}
    sequences = {f.name: o for o, f in defs.fields('BitmapGroupSequence')}
    sprites = {f.name: o for o, f in defs.fields('BitmapGroupSprite')}
    count, pointer = struct.unpack('<iI', xmap.read(bitmap.address + fields['bitmap group sequence'], 8))
    index, box = 0, (0., 0., 1., 1.)
    if count > 0:
        sc, sp = struct.unpack('<iI', xmap.read(pointer + sequences['sprites'], 8))
        if sc > 0:
            raw = xmap.read(sp, defs.size('BitmapGroupSprite'))
            index = struct.unpack_from('<h', raw, sprites['bitmap index'])[0]
            left, right, top, bottom = (struct.unpack_from('<f', raw, sprites[k])[0]
                                       for k in ('left', 'right', 'top', 'bottom'))
            box = left, top, right, bottom
        else:
            index = struct.unpack('<h', xmap.read(pointer + sequences['first bitmap index'], 2))[0]
    from .bitmaps import bitmap_images, decode
    images = bitmap_images(xmap, bitmap, defs)
    if not 0 <= index < len(images) or not all(math.isfinite(v) for v in box):
        raise ValueError(f'Invalid decal sprite: {bitmap.name}')
    image = decode(xmap, images[index], 128)
    w, h = image.size
    bounds = (max(0, int(box[0] * w)), max(0, int(box[1] * h)),
              min(w, math.ceil(box[2] * w)), min(h, math.ceil(box[3] * h)))
    if bounds[2] <= bounds[0] or bounds[3] <= bounds[1]:
        raise ValueError(f'Empty decal sprite: {bitmap.name}')
    return image.crop(bounds).resize((CELL, CELL), Image.Resampling.LANCZOS).convert('RGBA')


def material_ink(image, blend):
    result = []
    for r, g, b, alpha in image.getdata():
        if blend in (1, 2):
            scale = 2 if blend == 2 else 1
            rgb = [min(255, value * scale) for value in (r, g, b)]
            coverage = 255 - min(rgb)
            ink = [max(0, min(255, round((value - (255 - coverage)) * 255 / coverage)))
                   if coverage else 0 for value in rgb]
            result.append((*ink, coverage * alpha // 255))
        elif blend in (3, 6, 7):
            coverage = max(r, g, b)
            ink = [min(255, value * 255 // coverage) if coverage else 0 for value in (r, g, b)]
            result.append((*ink, coverage * alpha // 255))
        else:
            result.append((r, g, b, alpha))
    out = Image.new('RGBA', image.size)
    out.putdata(result)
    return out


def pack_material(image):
    """CI4 + RGBA16: transparent entry 0, fifteen original-ink colors.

    A fixed ordered alpha dither approximates soft original coverage within
    RGBA16's one-bit alpha; no opaque neutral-white/gray cards are introduced.
    """
    pixels = list(image.getdata())
    visible_rgb = [rgb[:3] for rgb in pixels if rgb[3] > 8]
    if not visible_rgb:
        raise ValueError('Empty authored decal material')
    sample = Image.new('RGB', (len(visible_rgb), 1))
    sample.putdata(visible_rgb)
    quantized = sample.quantize(colors=15, method=Image.Quantize.MEDIANCUT)
    palette_rgb = quantized.getpalette()[:45]
    palette_rgb += [0] * (45 - len(palette_rgb))
    colors = [tuple(palette_rgb[i:i + 3]) for i in range(0, 45, 3)]
    palette = [0] + [((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | 1 for r, g, b in colors]
    indices = []
    for index, (r, g, b, alpha) in enumerate(pixels):
        threshold = (BAYER[(index // CELL) & 3][index & 3] + .5) * 16
        if alpha <= threshold:
            indices.append(0)
        else:
            indices.append(1 + min(range(15), key=lambda c: sum((a - z) ** 2 for a, z in zip((r, g, b), colors[c]))))
    packed = bytes((indices[i] << 4) | indices[i + 1] for i in range(0, len(indices), 2))
    packed += struct.pack('>16H', *palette)
    preview = Image.new('RGBA', (CELL, CELL))
    preview.putdata([((palette[i] >> 11) * 255 // 31, ((palette[i] >> 6) & 31) * 255 // 31,
                      ((palette[i] >> 1) & 31) * 255 // 31, 255 if i else 0) for i in indices])
    assert len(packed) == PAYLOAD
    return packed, preview, indices.count(0)


def build(paths, *, definitions=None, campaign=False, pc_expansion=False, pc_maps=None):
    defs = configure(definitions)
    from .convert import open_map
    fields = {f.name: o for o, f in defs.fields('Decal')}
    records, seen_names, maps = {}, {}, []
    for path in paths:
        name = Path(path).stem
        xmap = open_map(path, campaign=campaign, pc_expansion=pc_expansion, pc_maps=pc_maps)
        count = 0
        for tag in xmap.tags:
            if tag.group_name != 'deca':
                continue
            raw = xmap.read(tag.address, defs.size('Decal'))
            bitmap = xmap.tag(struct.unpack_from('<I', raw, fields['map'] + 12)[0])
            if bitmap is None or bitmap.group_name != 'bitm':
                continue
            blend = struct.unpack_from('<h', raw, fields['framebuffer blend function'])[0]
            if blend not in (0, 1, 2, 3, 6, 7):
                raise ValueError(f'Unsupported native decal blend {blend}: {tag.name}')
            material_name = name + '|' + tag.name
            key = name_hash(material_name)
            if key in seen_names and seen_names[key] != material_name:
                raise ValueError(f'Decal name hash collision: {tag.name}')
            seen_names[key] = material_name
            payload, preview, transparent = pack_material(material_ink(sprite_image(xmap, bitmap, defs), blend))
            records[key] = dict(map=name, name=tag.name, bitmap=bitmap.name, blend=blend, payload=payload,
                                preview=preview, transparent_texels=transparent)
            count += 1
        maps.append(dict(name=name, authored_materials=count))
    header = b'NDEC' + struct.pack('>II', 1, len(records))
    index, data, entries = bytearray(), bytearray(), []
    shared = {}
    for key, material in sorted(records.items()):
        digest = hashlib.sha256(material['payload']).hexdigest()
        offset = shared.get(digest)
        if offset is None:
            offset = len(header) + len(records) * 8 + len(data)
            shared[digest] = offset
            data.extend(material['payload'])
        index.extend(struct.pack('>II', key, offset))
        entries.append(dict(hash=f'{key:08x}', map=material['map'], name=material['name'], bitmap=material['bitmap'],
                            blend=material['blend'], transparent_texels=material['transparent_texels'], offset=offset,
                            sha256=hashlib.sha256(material['payload']).hexdigest()))
    bank = header + index + data
    return bank, records, dict(maps=maps, materials=entries, cache_bytes=4 * PAYLOAD,
        shared_marks=32, cell=CELL, format='CI4 + 16-entry RGBA16 palette',
        approximation='Source ink alpha-over; multiply neutral white/double-multiply gray removed; ordered alpha dither; first sprite only',
        bank_bytes=len(bank), sha256=hashlib.sha256(bank).hexdigest())


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--maps", type=Path, nargs="+", required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--definitions", type=Path)
    parser.add_argument("--campaign", action="store_true")
    parser.add_argument("--pc-expansion", action="store_true")
    parser.add_argument("--pc-maps", type=Path)
    parser.add_argument("--audit", type=Path)
    args = parser.parse_args()
    bank, records, audit = build(args.maps, definitions=args.definitions, campaign=args.campaign,
                                 pc_expansion=args.pc_expansion, pc_maps=args.pc_maps)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(bank)
    if args.audit:
        args.audit.mkdir(parents=True, exist_ok=True)
        for key, material in records.items():
            material['preview'].save(args.audit / f'{key:08x}.png')
        (args.audit / 'materials.json').write_text(json.dumps(audit, indent=2))
    print(f'Native decals: {len(records)} source materials across {len(args.maps)} maps; {len(bank)} bytes ROM, {audit["cache_bytes"]} bytes texture cache')


if __name__ == '__main__':
    main()
