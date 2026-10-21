"""Export owned Halo decal artwork as a bounded N64 material bank.

The stock port omitted native decals. This bank keeps the source silhouettes
and ink colors; Xbox multiply/max/add are approximated with alpha-over ink.
No map tag bytes, damage, projectile settings, or source pixels are modified.

Each material (version 2) is IA8, 32x32 -- the ink's intensity and its
coverage, sixteen levels each, as paintball's I4 masks have -- then its
tint (the ink's colour: the renderer draws intensity x tint x the decal's
colour) and whether it darkens (Halo's multiply and double multiply: there
the decal's colour sets how strongly, not the ink's colour). The first
bank was CI4 with one bit of alpha, dithered: blood came out as a scatter
of dots, a bullet hole as a few black ones.
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
VERSION = 2
# IA8 texels, then the tint (r, g, b, darkens) and padding to 16 bytes
PAYLOAD = CELL * CELL + 16
# Halo's framebuffer blend functions that darken (multiply, double multiply)
DARKENING = (1, 2)


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
    image = decode(xmap, images[index], 128).convert('RGBA')
    w, h = image.size
    bounds = (max(0, int(box[0] * w)), max(0, int(box[1] * h)),
              min(w, math.ceil(box[2] * w)), min(h, math.ceil(box[3] * h)))
    if bounds[2] <= bounds[0] or bounds[3] <= bounds[1]:
        raise ValueError(f'Empty decal sprite: {bitmap.name}')
    # (the colour and the alpha resized apart: PIL resizes RGBA premultiplied,
    # which turned the colour black wherever the alpha was 0 -- and the
    # multiplying decals' alpha is not theirs to use: material_ink)
    sprite = image.crop(bounds)
    rgb = sprite.convert('RGB').resize((CELL, CELL), Image.Resampling.LANCZOS)
    alpha = sprite.getchannel('A').resize((CELL, CELL), Image.Resampling.LANCZOS)
    return Image.merge('RGBA', (*rgb.split(), alpha))


def material_ink(image, blend):
    """a decal sprite as ink over the surface: (ink colour, coverage). Halo's
    Xbox decal shader (rasterizer_xbox_decals.c) takes the texture's alpha in
    its alpha blend alone; multiply and double multiply darken by the colour
    (white, or 0.5 grey doubled, leaves the surface as it is), add and max
    brighten by it. Plasma burns and the sniper's mark, their alpha 0 where
    they darken, came out empty when it counted"""
    result = []
    for r, g, b, alpha in image.getdata():
        if blend in DARKENING:
            scale = 2 if blend == 2 else 1
            rgb = [min(255, value * scale) for value in (r, g, b)]
            coverage = 255 - min(rgb)
            ink = [max(0, min(255, round((value - (255 - coverage)) * 255 / coverage)))
                   if coverage else 0 for value in rgb]
            result.append((*ink, coverage))
        elif blend in (3, 6):
            coverage = max(r, g, b)
            ink = [min(255, value * 255 // coverage) if coverage else 0 for value in (r, g, b)]
            result.append((*ink, coverage))
        elif blend == 7:
            # (alpha multiply add: the surface times 1 - alpha, plus the colour)
            coverage = max(alpha, r, g, b)
            ink = [min(255, value * 255 // coverage) if coverage else 0 for value in (r, g, b)]
            result.append((*ink, coverage))
        else:
            result.append((r, g, b, alpha))
    out = Image.new('RGBA', image.size)
    out.putdata(result)
    return out


def pack_material(image, darkens=False):
    """IA8 + tint: each texel's ink brightness and coverage at sixteen levels
    (as paintball's masks), the ink's hue on average as the tint (coverage
    and brightness weighted; white for black ink). Transparent texels take
    the visible ones' mean brightness, so the RDP's filter brings no dark
    fringe in at the edges. Returns the payload, a preview and the count of
    transparent texels"""
    pixels = list(image.getdata())
    weights, hue = 0.0, [0.0, 0.0, 0.0]
    for r, g, b, alpha in pixels:
        value = max(r, g, b)
        if value:
            for k, c in enumerate((r, g, b)):
                hue[k] += c / value * alpha * value
            weights += alpha * value
    tint = [255, 255, 255]
    if weights:
        top = max(hue) or 1.0
        tint = [max(0, min(255, round(c / top * 255))) for c in hue]
    # (each texel's brightness: its largest channel, as the tint's is 255)
    levels = [(min(15, round(max(p[:3]) / 255 * 15)), min(15, round(p[3] / 255 * 15))) for p in pixels]
    visible = [i for i, a in levels if a]
    fill = round(sum(visible) / len(visible)) if visible else 0
    levels = [(i if a else fill, a) for i, a in levels]
    packed = bytes((i << 4) | a for i, a in levels)
    packed += bytes((*tint, 1 if darkens else 0)) + bytes(PAYLOAD - CELL * CELL - 4)
    preview = Image.new('RGBA', (CELL, CELL))
    preview.putdata([(*(round(i * 17 * t / 255) for t in tint), a * 17) for i, a in levels])
    assert len(packed) == PAYLOAD
    return packed, preview, sum(1 for _, a in levels if not a)


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
            payload, preview, transparent = pack_material(material_ink(sprite_image(xmap, bitmap, defs), blend),
                                                          darkens=blend in DARKENING)
            records[key] = dict(map=name, name=tag.name, bitmap=bitmap.name, blend=blend, payload=payload,
                                preview=preview, transparent_texels=transparent)
            count += 1
        maps.append(dict(name=name, authored_materials=count))
    header = b'NDEC' + struct.pack('>II', VERSION, len(records))
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
    return bank, records, dict(maps=maps, materials=entries, cache_bytes=8 * PAYLOAD,
        shared_marks=32, cell=CELL, format='IA8 + tint (r, g, b, darkens)',
        approximation='Source ink alpha-over; multiply neutral white/double-multiply gray removed; first sprite only',
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
