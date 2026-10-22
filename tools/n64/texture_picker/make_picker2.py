"""The second texture picker: the objects' textures (weapons held and on the
ground, vehicles, characters, scenery, props) and the terrain's (the
ground's detail grain, the rock and snow overlays), each in the formats it
could take, made by mksprite and decoded back as make_picker.py does; the
page saves the picks as texture-choices-objects-terrain.json:

    {"version": 1,
     "objects": {bitmap name: {"option": id, "tint": [r, g, b] for -tint}},
     "grain":   {bitmap name: {"option": id}},
     "overlays": {bitmap name: {"option": id}}}

Objects (tools/n64/models.py), third person:
    colour-32x64, ci4-64x64 (now; a player's colour: the palette split by
    its mask), grey-64x64, grey-128x64, and the greys tinted
  first person (weapons held, hands: an atlas cut into 64x64 windows, each a
  texture load), the same at twice the texels: colour 64x128 in 32x64
  windows, 16 colours 128x128 (now), greys 128x128 and 256x128
Ground grain (level.py detail_texture: light x (0.5 + texel), the colour on
the tile beside it in TMEM, so 2 KB at most):
    now (I4 64x64, drawn at its 32x32 level), grain-i4-64x64,
    grain-i8-32x32, grain-i8-64x32, grain-i4-64x64-micro (the micro detail
    map's grain multiplied in)
Rock and snow overlays (level.py overlay_texture: a pass of its own, which
darkens by its texel):
    now (I4 64x64 with mipmaps), overlay-i4-32x32, overlay-i8-64x32,
    overlay-i8-64x64 and overlay-i4-128x64 (4 KB: no room for mipmaps)

    python3 tools/n64/texture_picker/make_picker2.py <the Xbox maps folder> <output folder>
"""

import json
import struct
import sys
import tempfile
from pathlib import Path

from PIL import Image

sys.path.insert(0, str(Path(__file__).parent))
import make_picker as mp  # noqa: E402
from make_picker import MAPS, mksprite, fit_box, resized, tint_of, data_uri, opaque  # noqa: E402
from tools.n64.xboxmap import XboxMap  # noqa: E402
from tools.n64.repack import Repacker  # noqa: E402
from tools.n64.bitmaps import bitmap_images, decode  # noqa: E402
from tools.n64.level import (LevelBuilder, TERRAIN_REPEAT_UNITS, NATURAL_MATERIAL_TYPES, OVERLAY_MEAN,  # noqa: E402
                             OVERLAY_FLOOR_LEVEL)
from tools.n64.models import ModelConverter  # noqa: E402
from tools.n64 import dither as dithering  # noqa: E402

REFERENCE_SIDE = 192
WINDOW = 64


def decoded(xmap, defs, datum, side=256):
    tag = xmap.tag(datum) if datum != 0xFFFFFFFF else None
    images = bitmap_images(xmap, tag, defs) if tag is not None else []
    try:
        img = decode(xmap, images[0], side) if images else None
    except ValueError:
        return None
    if img is None:
        return None
    if max(img.size) > side:
        k = side / max(img.size)
        img = img.resize((max(1, int(img.width * k)), max(1, int(img.height * k))), Image.LANCZOS)
    return img.convert("RGBA")


def option(id_, label, sub, img):
    return {"id": id_, "label": label, "sub": sub, "img": img}


# ---------- objects

def ci4(img, mask, cutout, work, dither=False):
    """img as the converter's CI4 makes it: the palette split by a player's
    colour mask, transparent texels kept for a cut-out; with dither, the
    entries chosen with more dithering (tools/n64/dither.py)"""
    if mask is not None:
        paletted, transparency = dithering.split(img, mask.resize(img.size, Image.BILINEAR), cutout, dither)
        if transparency is not None:
            paletted.info["transparency"] = transparency
        return mksprite(paletted, "CI4", work)
    if cutout:
        paletted, transparency = dithering.cutout(img, dither)
        paletted.info["transparency"] = transparency
        return mksprite(paletted, "CI4", work)
    if dither:
        return mksprite(dithering.colours(img, True), "CI4", work)
    return mksprite(img.convert("RGB"), "CI4", work)


def grey_options(img, cutout, boxes, work, notes, dither=False):
    """the greys (I4; IA4 cut out) in these boxes, plain and tinted, as
    make_picker.options_for makes them"""
    out = []
    tint = tint_of(img)
    for id_, (long_side, short_side), label in boxes:
        size = fit_box(img, long_side, short_side)
        grey = img.convert("L").resize(size, Image.LANCZOS)
        top = max(grey.getdata()) or 1
        levels = 7 if cutout else 15
        grey = dithering.grey_steps(grey, top, levels, dither)
        if cutout:
            alpha = img.getchannel("A").resize(size, Image.LANCZOS).point(lambda v: 255 if v >= 128 else 0)
            stored = mksprite(Image.merge("LA", (grey, alpha)), "IA4", work)
        else:
            stored = mksprite(grey, "I4", work)
        k = top / 255.0
        plain = stored.copy()
        plain.putdata([(round(p[0] * k),) * 3 + (p[3],) for p in stored.getdata()])
        tinted = stored.copy()
        tinted.putdata([tuple(min(255, round(p[0] * k * t)) for t in tint) + (p[3],) for p in stored.getdata()])
        fmt = "IA4" if cutout else "I4"
        kb = size[0] * size[1] // 2048
        out.append(option(id_, f"Greyscale {label}", f"{fmt} · {kb} KB{notes}", plain))
        out.append(option(id_ + "-tint", f"Greyscale {label}, tinted", f"{fmt} · {kb} KB + tint{notes}", tinted))
    return out, tint


def object_options(img, mask, cutout, first_person, work, dither=False):
    """an object texture's options, the converter's (now) first (dither: the
    16 colours and the greys with more dithering; third person)"""
    lost = " · loses the player's colour" if mask is not None else ""
    if not first_person:
        size = fit_box(img, 64, 32)
        colour = mksprite(resized(img, size) if cutout else opaque(resized(img, size)), "RGBA16", work)
        w, h = img.size
        scale = min(1.0, 64.0 / max(w, h))
        small = resized(img, (max(4, mp.pow2_floor(int(w * scale))), max(4, mp.pow2_floor(int(h * scale)))))
        now = ci4(small, mask, cutout, work, dither)
        greys, tint = grey_options(img, cutout, (("grey-64x64", (64, 64), "64×64"), ("grey-128x64", (128, 64), "128×64")),
                                   work, lost, dither)
        return [option("ci4-64x64", "16 colours 64×64", "CI4 · 2 KB" + (" · 8 + 8 split for the player's colour"
                                                                         if mask is not None else ""), now),
                option("colour-32x64", "Full colour 32×64", "RGBA16 · 4 KB" + lost, colour)] + greys, tint
    # first person: the atlas (128 a side) in 64x64 windows of CI4 now; the
    # others at twice the third person's texels, in windows of their size
    w, h = img.size
    scale = min(1.0, 128.0 / max(w, h))
    atlas = resized(img, (max(1, int(w * scale)), max(1, int(h * scale))))
    now = Image.new("RGBA", atlas.size)
    for y in range(0, atlas.height, WINDOW):
        for x in range(0, atlas.width, WINDOW):
            box = (x, y, min(atlas.width, x + WINDOW), min(atlas.height, y + WINDOW))
            window = atlas.crop(box)
            window_mask = mask.resize(atlas.size, Image.BILINEAR).crop(box) if mask is not None else None
            now.paste(ci4(window, window_mask, cutout, work), (x, y))
    size = fit_box(img, 128, 64)
    colour = mksprite(resized(img, size) if cutout else opaque(resized(img, size)), "RGBA16", work)
    greys, tint = grey_options(img, cutout, (("grey-64x64", (128, 128), "128×128 (64×64 windows)"),
                                             ("grey-128x64", (256, 128), "256×128 (128×64 windows)")), work, lost)
    return [option("ci4-64x64", f"16 colours {atlas.width}×{atlas.height} (64×64 windows)",
                   "CI4 · 2 KB a window" + (" · 8 + 8 split" if mask is not None else ""), now),
            option("colour-32x64", "Full colour 128×64 (64×32 windows)", "RGBA16 · 4 KB a window" + lost, colour)] \
        + greys, tint


def object_textures(maps_folder):
    found = {}
    for map_name, display in MAPS:
        path = maps_folder / f"{map_name}.map"
        if not path.is_file():
            continue
        xmap = XboxMap(path)
        xmap.load_bsp(0)
        repacker = Repacker(xmap, None)
        models = ModelConverter(xmap, repacker.defs, map_name, Path(tempfile.mkdtemp(prefix="picker2-")))
        before = len(found)
        mf = models.f("Model")
        so = models.f("ShaderModel")
        se = models.f("ShaderEnvironment")
        for model in models.object_models():
            if model.index in models.hands:
                role = "first person (hands)"
            elif model.index in models.first_person:
                role = "first person"
            elif model.index in models.units:
                role = "characters & vehicles"
            elif model.index in models.static:
                role = "scenery"
            else:
                role = "items"
            count, pointer = models.block(model.address + mf["shaders"])
            for i in range(count):
                shader = xmap.tag(struct.unpack("<I", xmap.read(pointer + i * 32 + 12, 4))[0])
                if shader is None:
                    continue
                mask = None
                if shader.group_name == "soso":
                    base = struct.unpack("<I", xmap.read(shader.address + so["base map"] + 12, 4))[0]
                    flags = struct.unpack("<H", xmap.read(shader.address + so["shader model flags"], 2))[0]
                    change = struct.unpack("<H", xmap.read(shader.address + so["change color source"], 2))[0]
                    multipurpose = struct.unpack("<I", xmap.read(shader.address + so["multipurpose map"] + 12, 4))[0]
                    cut = bool((not flags & 4 or flags & 8) and models.textures.has_cutout(base))
                    img = decoded(xmap, repacker.defs, base)
                    if img is None:
                        continue
                    if not cut:
                        img = opaque(img)
                    if change and xmap.tag(multipurpose) is not None:
                        mask = models.change_mask(multipurpose, img.size, models.change_channel(shader, flags))
                elif shader.group_name == "senv":
                    base = struct.unpack("<I", xmap.read(shader.address + se["base map"] + 12, 4))[0]
                    env_flags = struct.unpack("<H", xmap.read(shader.address + se["shader environment flags"], 2))[0]
                    img = decoded(xmap, repacker.defs, base)
                    if img is None:
                        continue
                    cut = bool(env_flags & 1)
                    if cut:
                        bump = struct.unpack("<I", xmap.read(shader.address + se["bump map"] + 12, 4))[0]
                        scale = struct.unpack("<f", xmap.read(shader.address + se["bump map scale"], 4))[0] or 1.0
                        img = models.textures.environment_alpha(img, bump, scale)
                    else:
                        img = opaque(img)
                else:
                    # (transparent shaders: glows, screens, glass -- made by
                    # their own rules, models.py glow_texture: not offered)
                    continue
                name = xmap.tag(base).name
                first = role.startswith("first person")
                key = name + (" (first person)" if first else "")
                record = found.get(key)
                if record is None:
                    record = found[key] = {"name": name, "key": key, "image": img, "mask": mask, "cut": cut,
                                           "first": first, "roles": set(), "maps": [], "models": set()}
                record["roles"].add(role)
                # (the model's name past its category, repeats once: "cyborg fp")
                record["models"].add(" ".join(dict.fromkeys(model.name.split("\\")[1:])))
                if display not in record["maps"]:
                    record["maps"].append(display)
        print(f"  {display}: {len(found) - before} new object textures", flush=True)
    return found


# ---------- terrain

def grain_values(detail, size):
    """level.py detail_texture's grain at a size: (texel - 0.5) about the
    map's mean, 0..1 (the renderer draws light x (0.5 + texel))"""
    grey = detail.convert("L").resize(size, Image.LANCZOS)
    values = list(grey.getdata())
    mean = sum(values) / len(values) or 1.0
    return [max(0.0, min(1.0, v / mean - 0.5)) for v in values]


def grain_image(values, size, fmt, work, dither=False):
    levels = 15 if fmt == "I4" else 255
    if dither and fmt == "I4":
        values = dithering.dither_values(values, size[0], levels)
    img = Image.new("L", size)
    img.putdata([int(round(round(v * levels) * 255 / levels)) for v in values])
    stored = mksprite(img, fmt, work)
    return [p[0] / 255.0 for p in stored.getdata()]


def shown_grain(texels, size, colour):
    img = Image.new("RGBA", size)
    img.putdata([tuple(min(255, round(c * 255 * (0.5 + t))) for c in colour) + (255,) for t in texels])
    return img


def grain_options(detail, micro, micro_ratio, colour, work, dither=False):
    """the grain's options (dither: the 16-grey ones with more dithering)"""
    out = []
    # now: I4 64x64, drawn at its 32x32 level (mksprite's BOX level 1)
    values = grain_values(detail, (64, 64))
    level0 = grain_image(values, (64, 64), "I4", work, dither)
    level1 = [sum(level0[(2 * y + dy) * 64 + 2 * x + dx] for dy in (0, 1) for dx in (0, 1)) / 4
              for y in range(32) for x in range(32)]
    level1 = grain_image(level1, (32, 32), "I4", work, dither)
    out.append(option("now", "Now: 16 greys, its 32×32 level", "I4 64×64 + mipmaps · drawn at 32×32",
                      shown_grain(level1, (32, 32), colour)))
    out.append(option("grain-i4-64x64", "16 greys 64×64", "I4 · 2 KB (fits beside the 2 KB colour tile)",
                      shown_grain(level0, (64, 64), colour)))
    for id_, size, label, sub in (("grain-i8-32x32", (32, 32), "256 greys 32×32", "I8 · 1 KB"),
                                  ("grain-i8-64x32", (64, 32), "256 greys 64×32", "I8 · 2 KB")):
        texels = grain_image(grain_values(detail, size), size, "I8", work)
        out.append(option(id_, label, sub, shown_grain(texels, size, colour)))
    if micro is not None and micro_ratio:
        # the micro detail map's grain multiplied in: it repeats micro_ratio
        # times across the grain (Halo's micro function: about its mean)
        k = max(1, min(8, round(micro_ratio)))
        side = max(4, 64 // k)
        m = micro.convert("L").resize((side, side), Image.LANCZOS)
        mv = list(m.getdata())
        mean = sum(mv) / len(mv) or 1.0
        tiled = [mv[(y % side) * side + x % side] / mean for y in range(64) for x in range(64)]
        values = [max(0.0, min(1.0, (0.5 + v) * t - 0.5)) for v, t in zip(grain_values(detail, (64, 64)), tiled)]
        texels = grain_image(values, (64, 64), "I4", work, dither)
        out.append(option("grain-i4-64x64-micro", "16 greys 64×64 + micro detail",
                          f"I4 · 2 KB · the micro grain {k}×{k} in it", shown_grain(texels, (64, 64), colour)))
    return out


def overlay_texels(detail, size, levels, dither=False):
    """level.py overlay_texture's texels at a size: the factor it darkens by,
    its brightest 1, the contrast cut to keep the mean at OVERLAY_MEAN (dither:
    on its 16 greys with more dithering)"""
    grey = detail.convert("L").resize(size, Image.LANCZOS)
    values = list(grey.getdata())
    top = max(values) or 1
    mean = sum(values) / len(values) / top
    cut = min(1.0, (1.0 - OVERLAY_MEAN) / (1.0 - mean)) if mean < 1.0 else 1.0
    floor = OVERLAY_FLOOR_LEVEL / 15.0
    raw = [1.0 - cut * (1.0 - v / top) for v in values]
    stepped = dithering.dither_values(raw, size[0], levels) if dither and levels == 15 else \
        [round(levels * r) / levels for r in raw]
    return [max(floor, s) for s in stepped]


def shown_overlay(texels, size, colour):
    mean = sum(texels) / len(texels) or 1.0
    img = Image.new("RGBA", size)
    img.putdata([tuple(min(255, round(c * 255 * t / mean)) for c in colour) + (255,) for t in texels])
    return img


def overlay_options(detail, colour, work, dither=False):
    """the overlay's options (dither: the 16-grey ones with more dithering)"""
    out = []
    for id_, size, fmt, label, sub in (
            ("now", (64, 64), "I4", "Now: 16 greys 64×64", "I4 · 2 KB + mipmaps"),
            ("overlay-i4-32x32", (32, 32), "I4", "16 greys 32×32", "I4 · 0.5 KB + mipmaps"),
            ("overlay-i8-64x32", (64, 32), "I8", "256 greys 64×32", "I8 · 2 KB + mipmaps"),
            ("overlay-i8-64x64", (64, 64), "I8", "256 greys 64×64", "I8 · 4 KB · no room for mipmaps"),
            ("overlay-i4-128x64", (128, 64), "I4", "16 greys 128×64", "I4 · 4 KB · no room for mipmaps")):
        levels = 15 if fmt == "I4" else 255
        values = overlay_texels(detail, size, levels, dither)
        img = Image.new("L", size)
        img.putdata([int(round(v * 255)) for v in values])
        stored = [p[0] / 255.0 for p in mksprite(img, fmt, work).getdata()]
        out.append(option(id_, label, sub, shown_overlay(stored, size, colour)))
    return out


def detailed(base, detail, function):
    """a base colour with a detail colour on it by Halo's detail functions:
    double/biased multiply, multiply, double/biased add"""
    if function == 0:
        return [min(1.0, b * 2.0 * d) for b, d in zip(base, detail)]
    if function == 1:
        return [b * d for b, d in zip(base, detail)]
    return [max(0.0, min(1.0, b + 2.0 * d - 1.0)) for b, d in zip(base, detail)]


def mean_colour(img):
    rgb = img.convert("RGB")
    n = rgb.width * rgb.height
    return [sum(ch.getdata()) / n / 255.0 for ch in rgb.split()]


def terrain_textures(maps_folder):
    grains, overlays = {}, {}
    for map_name, display in MAPS:
        path = maps_folder / f"{map_name}.map"
        if not path.is_file():
            continue
        xmap = XboxMap(path)
        xmap.load_bsp(0)
        repacker = Repacker(xmap, None)
        builder = LevelBuilder(xmap, repacker.defs, map_name, Path(tempfile.mkdtemp(prefix="picker2-")))
        builder.greyscale = builder.terrain_detail = True
        bsp_tag = xmap.bsps[0][0]
        builder.shader_span = builder.shader_spans(bsp_tag)
        f = builder.fields("ShaderEnvironment")
        bo = builder.fields("ScenarioStructureBSP")
        lo = builder.fields("ScenarioStructureBSPLightmap")
        mo = builder.fields("ScenarioStructureBSPMaterial")
        count, ptr = struct.unpack("<iI", xmap.read(bsp_tag.address + bo["lightmaps"], 8))
        lsize, msize = repacker.defs.size("ScenarioStructureBSPLightmap"), repacker.defs.size("ScenarioStructureBSPMaterial")
        shaders = {}
        for li in range(count):
            mc, mp_ = struct.unpack("<iI", xmap.read(ptr + li * lsize + lo["materials"], 8))
            for mi in range(mc):
                raw = xmap.read(mp_ + mi * msize, msize)
                shader = struct.unpack_from("<I", raw, mo["shader"] + 12)[0]
                shaders[shader] = shaders.get(shader, 0) + max(0, struct.unpack_from("<ii", raw, mo["surfaces"])[1])
        before = (len(grains), len(overlays))
        for shader, surfaces in shaders.items():
            tag = xmap.tag(shader)
            if tag is None or tag.group_name != "senv":
                continue

            def read(field, fmt, tag=tag):
                return struct.unpack(fmt, xmap.read(tag.address + f[field], struct.calcsize(fmt)))[0]

            def datum(field, tag=tag):
                return struct.unpack("<I", xmap.read(tag.address + f[field] + 12, 4))[0]
            base = datum("base map")
            cut = read("shader environment flags", "<H") & 1
            span = builder.shader_span.get(shader, 0.0)
            base_img = decoded(xmap, repacker.defs, base)
            if base_img is None:
                continue
            colour = mean_colour(base_img)
            terrain = not cut and span >= TERRAIN_REPEAT_UNITS and builder.terrain_materials(tag, base, span) is not None
            if terrain:
                blended = read("shader environment type", "<H") in (1, 2)
                micro_datum = datum("micro detail map")
                micro = decoded(xmap, repacker.defs, micro_datum) if micro_datum != 0xFFFFFFFF else None
                for field in ("primary detail map", "secondary detail map") if blended else ("primary detail map",):
                    d = datum(field)
                    scale = read(field + " scale", "<f")
                    if d == 0xFFFFFFFF or scale <= 0:
                        continue
                    name = xmap.tag(d).name
                    record = grains.get(name)
                    if record is None:
                        img = decoded(xmap, repacker.defs, d, 64)
                        if img is None:
                            continue
                        micro_scale = read("micro detail map scale", "<f") if micro is not None else 0.0
                        # (the ground under it: the base map's colour times
                        # the detail's own on average, by Halo's detail
                        # function, as the colour tiles have it)
                        record = grains[name] = {"name": name, "image": img,
                                                 "colour": detailed(colour, mean_colour(img),
                                                                    read("detail map function", "<H")),
                                                 "micro": micro,
                                                 "micro_ratio": micro_scale / scale if micro_scale > 0 else 0.0,
                                                 "maps": [], "surfaces": 0, "shaders": set()}
                    record["surfaces"] += surfaces
                    record["shaders"].add(tag.name)
                    if display not in record["maps"]:
                        record["maps"].append(display)
            elif not cut and read("material type", "<H") in NATURAL_MATERIAL_TYPES:
                d = datum("primary detail map")
                if d == 0xFFFFFFFF or read("primary detail map scale", "<f") <= 0:
                    continue
                name = xmap.tag(d).name
                record = overlays.get(name)
                if record is None:
                    img = decoded(xmap, repacker.defs, d, 128)
                    if img is None:
                        continue
                    record = overlays[name] = {"name": name, "image": img, "colour": colour, "maps": [],
                                               "surfaces": 0, "shaders": set()}
                record["surfaces"] += surfaces
                record["shaders"].add(tag.name)
                if display not in record["maps"]:
                    record["maps"].append(display)
        print(f"  {display}: {len(grains) - before[0]} new grains, {len(overlays) - before[1]} new overlays", flush=True)
    return grains, overlays


def reference(img):
    if max(img.size) > REFERENCE_SIDE:
        k = REFERENCE_SIDE / max(img.size)
        img = resized(img, (max(1, round(img.width * k)), max(1, round(img.height * k))))
    return data_uri(img)


def main():
    maps_folder, out_folder = Path(sys.argv[1]), Path(sys.argv[2])
    out_folder.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="picker2-sprites-"))
    records = []
    print("Finding the objects' textures", flush=True)
    objects = object_textures(maps_folder)
    print(f"{len(objects)} object textures; making their options", flush=True)
    for i, r in enumerate(sorted(objects.values(), key=lambda r: (not r["first"], -len(r["maps"]), r["name"]))):
        options, tint = object_options(r["image"], r["mask"], r["cut"], r["first"], work)
        kind = "cut-out" if r["cut"] else "opaque"
        chips = [kind] + (["player colour"] if r["mask"] is not None else []) + sorted(r["roles"])
        records.append({"section": "objects", "key": r["key"], "name": r["name"], "current": "ci4-64x64",
                        "chips": chips, "detail": ", ".join(sorted(r["models"])[:6]) +
                        (" …" if len(r["models"]) > 6 else ""), "maps": r["maps"], "tint": [round(t, 3) for t in tint],
                        "size": list(r["image"].size), "original": reference(r["image"]),
                        "options": [{"id": o["id"], "label": o["label"], "sub": o["sub"], "size": list(o["img"].size),
                                     "src": data_uri(o["img"])} for o in options]})
        if (i + 1) % 25 == 0:
            print(f"  {i + 1}/{len(objects)}", flush=True)
    print("Finding the terrain's textures", flush=True)
    grains, overlays = terrain_textures(maps_folder)
    for section, found in (("grain", grains), ("overlays", overlays)):
        for r in sorted(found.values(), key=lambda r: -r["surfaces"]):
            if section == "grain":
                options = grain_options(r["image"], r["micro"], r["micro_ratio"], r["colour"], work)
            else:
                options = overlay_options(r["image"], r["colour"], work)
            records.append({"section": section, "key": r["name"], "name": r["name"], "current": "now",
                            "chips": [f"{r['surfaces']:,} triangles"], "detail": ", ".join(
                                s.split("\\")[-1] for s in sorted(r["shaders"])[:6]),
                            "maps": r["maps"], "tint": None, "size": list(r["image"].size),
                            "original": reference(r["image"]),
                            "options": [{"id": o["id"], "label": o["label"], "sub": o["sub"], "size": list(o["img"].size),
                                         "src": data_uri(o["img"])} for o in options]})
    template = (Path(__file__).parent / "picker2_template.html").read_text(encoding="utf-8")
    data = json.dumps({"maps": [d for _, d in MAPS], "textures": records}).replace("</", "<\\/")
    target = out_folder / "texture-picker-objects-terrain.html"
    target.write_text(template.replace("/*DATA*/null", data), encoding="utf-8")
    counts = {s: sum(1 for r in records if r["section"] == s) for s in ("objects", "grain", "overlays")}
    print(f"{target}: {target.stat().st_size / 1e6:.1f} MB, {counts}")


if __name__ == "__main__":
    main()
