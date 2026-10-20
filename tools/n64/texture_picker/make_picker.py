"""Halo 64's level textures in every format the cartridge could store them
in, side by side in one HTML page (texture-picker.html) where a format is
picked for each; the page saves the picks as JSON (texture-choices.json).

Every non-terrain texture of the levels (environment shaders' base maps, and
the transparent shaders the N64 draws), on the 13 multiplayer maps and the
campaign's level, found as tools/n64/level.py's LevelBuilder finds them.
Each option is made by libdragon's mksprite as the converter makes its
textures, then decoded from the sprite, so the page shows the very texels a
cartridge would hold:

    colour-32x64      RGBA16 (15-bit colour), 32x64 (4 KB: all of TMEM)
    ci4-64x64         CI4, 16 colours, 64x64 at most, the image's proportions
                      (2 KB + palette: what the colour textures are now)
    grey-64x64        I4, 16 greys (IA4, 8 greys + cut-out, if alpha-tested), 64x64 (2 KB)
    grey-128x64       the same, 128x64 (4 KB: what the grey textures are now)
    ...-tint          a grey option times one colour, the least-squares fit of
                      the original's colours (the renderer can tint a grey
                      texture for free through its vertex colours)

Shown at the original's brightness (the cartridge doubles the colour
textures, Halo's doubling, and undoes the greys' stretch on the vertices).

    python3 tools/n64/texture_picker/make_picker.py <the Xbox maps folder> <output folder>

The page holds the game's textures: keep it to yourself. (make_picker2.py
makes the second page, the objects' and the terrain's.)
"""

import base64
import io
import json
import math
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

from PIL import Image

# (the repository: tools/n64/texture_picker/ is three folders in)
sys.path.insert(0, str(Path(__file__).resolve().parents[3]))
from tools.n64.xboxmap import XboxMap  # noqa: E402
from tools.n64.repack import Repacker  # noqa: E402
from tools.n64.bitmaps import bitmap_images, decode  # noqa: E402
from tools.n64 import level as level_module  # noqa: E402
from tools.n64.level import LevelBuilder, TERRAIN_REPEAT_UNITS, cutout_palette  # noqa: E402
from tools.n64.toolpaths import tool  # noqa: E402

MKSPRITE = tool("mksprite")
MAPS = [("bloodgulch", "Blood Gulch"), ("beavercreek", "Battle Creek"), ("sidewinder", "Sidewinder"),
        ("damnation", "Damnation"), ("ratrace", "Rat Race"), ("prisoner", "Prisoner"),
        ("hangemhigh", "Hang 'em High"), ("chillout", "Chill Out"), ("carousel", "Derelict"),
        ("boardingaction", "Boarding Action"), ("wizard", "Wizard"), ("putput", "Chiron TL-34"),
        ("longest", "Longest"), ("a30", "Halo (campaign)")]
ORIGINAL_SIDE = 256
REFERENCE_SIDE = 192

FORMAT_CODES = {2: "RGBA16", 8: "CI4", 9: "CI8", 12: "IA4", 13: "IA8", 16: "I4", 17: "I8"}


# ---------- libdragon sprites

def decode_sprite(data):
    """a libdragon .sprite (uncompressed) as an RGBA image"""
    width, height, _, flags = struct.unpack_from(">HHBB", data, 0)
    fmt = FORMAT_CODES[flags & 0x1F]
    bits = {"RGBA16": 16, "CI4": 4, "CI8": 8, "IA4": 4, "IA8": 8, "I4": 4, "I8": 8}[fmt]
    stride = (width * bits + 7) // 8
    pixels_at = 8
    palette = None
    if flags & 0x80:
        ext_at = 8 + ((stride * height + 7) & ~7) if not flags & 0x40 else 8
        pal_pos = struct.unpack_from(">I", data, ext_at + 4)[0]
        if flags & 0x40:
            pixels_at = struct.unpack_from(">I", data, ext_at + 124)[0]
        if fmt in ("CI4", "CI8") and pal_pos:
            count = 16 if fmt == "CI4" else 256
            palette = [rgba16(struct.unpack_from(">H", data, pal_pos + 2 * i)[0]) for i in range(count)]
    out = []
    for y in range(height):
        row = data[pixels_at + y * stride:pixels_at + (y + 1) * stride]
        for x in range(width):
            if bits == 16:
                out.append(rgba16(struct.unpack_from(">H", row, x * 2)[0]))
                continue
            if bits == 8:
                v = row[x]
            else:
                byte = row[x // 2]
                v = byte >> 4 if x % 2 == 0 else byte & 15
            if fmt in ("CI4", "CI8"):
                out.append(palette[v])
            elif fmt == "I4":
                out.append((v * 17,) * 3 + (255,))
            elif fmt == "I8":
                out.append((v,) * 3 + (255,))
            elif fmt == "IA4":
                i = (v >> 1) * 255 // 7
                out.append((i, i, i, 255 if v & 1 else 0))
            elif fmt == "IA8":
                out.append(((v >> 4) * 17,) * 3 + ((v & 15) * 17,))
    img = Image.new("RGBA", (width, height))
    img.putdata(out)
    return img


def rgba16(v):
    def five(x):
        return (x << 3) | (x >> 2)
    return (five((v >> 11) & 31), five((v >> 6) & 31), five((v >> 1) & 31), 255 if v & 1 else 0)


def mksprite(img, fmt, work):
    """img through mksprite in this format, decoded back"""
    png = work / "option.png"
    img.save(png, transparency=img.info.get("transparency")) if img.mode == "P" else img.save(png)
    subprocess.run([MKSPRITE, "--format", fmt, "--mipmap", "NONE", "--compress", "0", "-o", str(work), str(png)],
                   check=True, capture_output=True)
    return decode_sprite((work / "option.sprite").read_bytes())


# ---------- the options

def pow2_floor(n):
    return 1 << int(math.log2(max(1, n)))


def fit_box(img, long_side, short_side):
    """the size of a box long x short (its long side the better way round:
    the one that loses least, as level.py's grey_texture chooses), each side
    no larger than the image's"""
    w, h = img.size
    best = None
    for bw, bh in ((long_side, short_side), (short_side, long_side)):
        size = (min(bw, pow2_floor(w)), min(bh, pow2_floor(h)))
        lum = img.convert("L")
        back = lum.resize(size, Image.LANCZOS).resize(lum.size, Image.BILINEAR)
        error = sum((a - b) ** 2 for a, b in zip(lum.getdata(), back.getdata()))
        if best is None or error < best[0]:
            best = (error, size)
        if long_side == short_side:
            break
    return best[1]


def resized(img, size):
    """RGBA resized with the colour and the alpha apart (PIL resizes RGBA
    premultiplied: transparent texels went black)"""
    rgb = img.convert("RGB").resize(size, Image.LANCZOS)
    rgb.putalpha(img.getchannel("A").resize(size, Image.LANCZOS))
    return rgb


def luminance(img):
    return img.convert("L")


def tint_of(img):
    """the colour t that t x grey fits the image's colours best (least
    squares, opaque texels)"""
    rgb = list(img.convert("RGB").getdata())
    alpha = list(img.getchannel("A").getdata())
    grey = list(img.convert("L").getdata())
    pairs = [(p, g) for p, g, a in zip(rgb, grey, alpha) if a >= 128] or list(zip(rgb, grey))
    gg = sum(g * g for _, g in pairs) or 1
    return [min(4.0, sum(p[k] * g for p, g in pairs) / gg) for k in range(3)]


def options_for(img, cutout, work):
    """{option id: RGBA image as it would look}, and the tint"""
    out = {}
    # full colour: RGBA16 32x64
    size = fit_box(img, 64, 32)
    out["colour-32x64"] = mksprite(resized(img, size).convert("RGBA") if cutout else
                                   resized(img, size).convert("RGB").convert("RGBA"), "RGBA16", work)
    # 16 colours: CI4, the converter's way (level.py write_texture)
    w, h = img.size
    scale = min(1.0, 64.0 / max(w, h))
    size = (max(4, pow2_floor(int(w * scale))), max(4, pow2_floor(int(h * scale))))
    small = resized(img, size)
    if cutout:
        paletted, transparency = cutout_palette(small)
        paletted.info["transparency"] = transparency
        out["ci4-64x64"] = mksprite(paletted, "CI4", work)
    else:
        out["ci4-64x64"] = mksprite(small.convert("RGB"), "CI4", work)
    # greyscale: I4 (IA4 cut out), stretched to the full range as
    # grey_texture does, the stretch undone for showing
    tint = tint_of(img)
    for option, long_side, short_side in (("grey-64x64", 64, 64), ("grey-128x64", 128, 64)):
        size = fit_box(img, long_side, short_side)
        grey = luminance(img).resize(size, Image.LANCZOS)
        top = max(grey.getdata()) or 1
        levels = 7 if cutout else 15
        grey = grey.point(lambda v: int(round(round(v / top * levels) * 255 / levels)))
        if cutout:
            alpha = img.getchannel("A").resize(size, Image.LANCZOS).point(lambda v: 255 if v >= 128 else 0)
            stored = mksprite(Image.merge("LA", (grey, alpha)), "IA4", work)
        else:
            stored = mksprite(grey, "I4", work)
        k = top / 255.0
        plain = stored.copy()
        plain.putdata([(round(p[0] * k),) * 3 + (p[3],) for p in stored.getdata()])
        out[option] = plain
        tinted = stored.copy()
        tinted.putdata([tuple(min(255, round(p[0] * k * t)) for t in tint) + (p[3],) for p in stored.getdata()])
        out[option + "-tint"] = tinted
    return out, tint


# ---------- the levels' textures

def level_textures(maps_folder):
    """{bitmap tag name: record}, every non-terrain texture of every level"""
    found = {}
    for map_name, display in MAPS:
        path = maps_folder / f"{map_name}.map"
        if not path.is_file():
            print(f"  {map_name}: not found, left out")
            continue
        xmap = XboxMap(path)
        xmap.load_bsp(0)
        repacker = Repacker(xmap, None)
        builder = LevelBuilder(xmap, repacker.defs, map_name, Path(tempfile.mkdtemp(prefix="picker-")))
        builder.greyscale = builder.terrain_detail = True
        bsp_tag = xmap.bsps[0][0]
        builder.shader_span = builder.shader_spans(bsp_tag)
        bo = builder.fields("ScenarioStructureBSP")
        lo = builder.fields("ScenarioStructureBSPLightmap")
        mo = builder.fields("ScenarioStructureBSPMaterial")
        surfaces = {}
        lightmap_count, lightmap_ptr = struct.unpack("<iI", xmap.read(bsp_tag.address + bo["lightmaps"], 8))
        lsize, msize = repacker.defs.size("ScenarioStructureBSPLightmap"), repacker.defs.size("ScenarioStructureBSPMaterial")
        for li in range(lightmap_count):
            mc, mp = struct.unpack("<iI", xmap.read(lightmap_ptr + li * lsize + lo["materials"], 8))
            for mi in range(mc):
                raw = xmap.read(mp + mi * msize, msize)
                shader = struct.unpack_from("<I", raw, mo["shader"] + 12)[0]
                count = struct.unpack_from("<ii", raw, mo["surfaces"])[1]
                surfaces[shader] = surfaces.get(shader, 0) + max(0, count)
        before = len(found)
        for shader, count in surfaces.items():
            tag = xmap.tag(shader)
            if tag is None or count <= 0:
                continue
            entry = classify(builder, xmap, tag, shader)
            if entry is None:
                continue
            datum, kind, img, current = entry
            bitmap = xmap.tag(datum)
            name = bitmap.name if bitmap is not None else f"{map_name}:{datum:08x}"
            record = found.get(name)
            if record is None:
                record = found[name] = {"name": name, "image": img, "kind": kind, "current": current,
                                        "maps": [], "surfaces": 0, "shaders": set()}
            if display not in record["maps"]:
                record["maps"].append(display)
            record["surfaces"] += count
            record["shaders"].add(tag.name)
        print(f"  {display}: {len(surfaces)} shaders, {len(found) - before} new textures")
    return found


def classify(builder, xmap, tag, shader):
    """(base bitmap datum, kind, original image, current option) for a
    non-terrain level shader the N64 draws textured, else None"""
    group = tag.group_name
    if group == "senv":
        f = builder.fields("ShaderEnvironment")
        base = struct.unpack("<I", xmap.read(tag.address + f["base map"] + 12, 4))[0]
        env_flags = struct.unpack("<H", xmap.read(tag.address + f["shader environment flags"], 2))[0]
        bump = struct.unpack("<I", xmap.read(tag.address + f["bump map"] + 12, 4))[0]
        bump_scale = struct.unpack("<f", xmap.read(tag.address + f["bump map scale"], 4))[0] or 1.0
        cutout = bool(env_flags & 1)
        span = builder.shader_span.get(shader, 0.0)
        if not cutout and span >= TERRAIN_REPEAT_UNITS and builder.terrain_materials(tag, base, span) is not None:
            return None
        img = original(builder, xmap, base)
        if img is None:
            return None
        # (a base map's alpha is other data -- a specular mask, often all 0:
        # the cut-out's comes from the bump map, as level.py has it)
        img = builder.environment_alpha(img, bump, bump_scale) if cutout else opaque(img)
        grey = not builder.base_needs_colour(base)
        return base, "cut-out" if cutout else "opaque", img, "grey-128x64" if grey else "ci4-64x64"
    if group == "swat" or builder.dropped_shader(tag):
        return None
    if any(word in tag.name.split("\\")[-1].lower() for word in level_module.WATER_NAMES) and group == "schi":
        return None
    first = []

    def on_dependency(t, address, f, path, g, datum):
        if g == 0x6269746D and datum != 0xFFFFFFFF and not first:
            first.append(datum)

    from tools.n64.walk import Walker
    Walker(xmap, builder.defs).walk(tag, on_dependency=on_dependency)
    if not first:
        return None
    img = original(builder, xmap, first[0])
    return (first[0], "transparent", opaque(img), "ci4-64x64") if img is not None else None


def opaque(img):
    return img.convert("RGB").convert("RGBA")


def original(builder, xmap, datum):
    tag = xmap.tag(datum)
    images = bitmap_images(xmap, tag, builder.defs) if tag is not None else []
    try:
        img = decode(xmap, images[0], ORIGINAL_SIDE) if images else None
    except ValueError:
        return None
    if img is None:
        return None
    if max(img.size) > ORIGINAL_SIDE:
        k = ORIGINAL_SIDE / max(img.size)
        img = img.resize((max(1, int(img.width * k)), max(1, int(img.height * k))), Image.LANCZOS)
    return img.convert("RGBA")


def data_uri(img):
    buffer = io.BytesIO()
    img.save(buffer, "PNG", optimize=True)
    return "data:image/png;base64," + base64.b64encode(buffer.getvalue()).decode("ascii")


def main():
    maps_folder, out_folder = Path(sys.argv[1]), Path(sys.argv[2])
    out_folder.mkdir(parents=True, exist_ok=True)
    print("Finding the levels' textures")
    textures = level_textures(maps_folder)
    print(f"{len(textures)} textures; making their options")
    work = Path(tempfile.mkdtemp(prefix="picker-sprites-"))
    records = []
    for i, record in enumerate(sorted(textures.values(), key=lambda r: -r["surfaces"])):
        img = record["image"]
        options, tint = options_for(img, record["kind"] == "cut-out", work)
        reference = img
        if max(img.size) > REFERENCE_SIDE:
            k = REFERENCE_SIDE / max(img.size)
            reference = resized(img, (max(1, round(img.width * k)), max(1, round(img.height * k))))
        records.append({
            "name": record["name"], "kind": record["kind"], "current": record["current"],
            "maps": record["maps"], "surfaces": record["surfaces"], "shaders": sorted(record["shaders"]),
            "size": list(img.size), "tint": [round(t, 3) for t in tint],
            "original": data_uri(reference),
            "options": {key: {"size": list(value.size), "src": data_uri(value)} for key, value in options.items()},
        })
        if (i + 1) % 20 == 0:
            print(f"  {i + 1}/{len(textures)}")
    template = (Path(__file__).parent / "picker_template.html").read_text(encoding="utf-8")
    data = json.dumps({"maps": [d for _, d in MAPS], "textures": records}).replace("</", "<\\/")
    page = template.replace("/*DATA*/null", data)
    target = out_folder / "texture-picker.html"
    target.write_text(page, encoding="utf-8")
    print(f"{target}: {target.stat().st_size / 1e6:.1f} MB, {len(records)} textures")


if __name__ == "__main__":
    main()
