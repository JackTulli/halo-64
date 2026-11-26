"""The looks of the geometry Halo builds each frame (port/n64/src/n64_dynamic.c).

Halo draws two things of a multiplayer game as geometry it makes every frame
in its rasterizer's dynamic buffers, which the N64 does not have: King of the
Hill's band round the hill (game_engine_king.c, the globals' hill shader) and
the cloth of Capture the Flag's flags (objects/widgets/flags.c, the flag tag's
red and blue shaders). Their shaders' bitmaps are left out of the image like
every shader's (convert.py bitmap_stubs); their looks are written here, small,
for each map:

    fx/<map>.dyn
        header (16 bytes): 'N64D', u16 version, u16 entry count, 8 bytes 0
        entries (32 bytes each, by shader tag index):
            u16 the shader's tag index, u8 kind (1 the hill's band, 2 a
                flag's cloth), u8 format (0 I4, 1 RGBA16), u8 width, u8
                height (texels), u8 the scroll's function, u8 the flicker's
                function (Halo's periodic functions; 0 one: none)
            u8 colour r g b a (I4: the texels' hue at full brightness, the
                brightest texel's alpha; RGBA16: white, 255)
            f32 the scroll's period (s; along the texture's width, a repeat
                each period), its scale (repeats), the flicker's period (s),
                its lower bound (a share of its upper)
            u32 the texels' offset in the file

The band's texture is the hill shader's colour (its stages evaluated on the
CPU, stages.py) from v 0.2 to 1 -- the part of the bitmap the band shows --
as an I4 intensity of one hue: the shader adds its colour to the frame, which
the N64 draws as that hue at the texel's intensity (an alpha blend: the RDP's
additive blend wraps). A flag's is its shader's bitmap, small, in RGBA16.

    python3 -m tools.n64.dynamic <map> --filesystem <file tree>
"""

import struct
from pathlib import Path

from PIL import Image

from .bitmaps import bitmap_images, decode

MAGIC = b"N64D"
VERSION = 1
KIND_HILL, KIND_FLAG = 1, 2
FORMAT_I4, FORMAT_RGBA16 = 0, 1
HILL_SIZE = (64, 32)
FLAG_SIZE = (32, 16)
# the band's v: from 1 at the ground to 0.2 at its top (game_engine_king.c)
HILL_V_TOP = 0.2
ENTRY = struct.Struct(">HBBBBBB4B4fI")
assert ENTRY.size == 32


def _offsets(defs, name):
    return {f.name: o for o, f in defs.fields(name)}


def _reference(xmap, address):
    """the tag a TagDependency at address names, or None"""
    return xmap.tag(struct.unpack("<I", xmap.read(address + 12, 4))[0])


def _block(xmap, address):
    return struct.unpack("<iI", xmap.read(address, 8))


def hill_shader(xmap, defs):
    """the globals' hill shader (the first multiplayer information's), or None"""
    matg = next((t for t in xmap.tags if t.group_name == "matg"), None)
    if matg is None:
        return None
    count, pointer = _block(xmap, matg.address + _offsets(defs, "Globals")["multiplayer information"])
    if count < 1:
        return None
    struct_name = next(f.struct for o, f in defs.fields("Globals") if f.name == "multiplayer information")
    return _reference(xmap, pointer + _offsets(defs, struct_name)["hill shader"])


def flag_shaders(xmap, defs):
    """the red and blue shaders of the cloths the N64 makes: the flag
    widgets of the globals' multiplayer flag (objects/widgets/widgets.c makes
    no other: a scenery cloth's shaders are not read)"""
    fields = _offsets(defs, "Flag")
    found = []
    matg = next((t for t in xmap.tags if t.group_name == "matg"), None)
    if matg is None:
        return found
    count, pointer = _block(xmap, matg.address + _offsets(defs, "Globals")["multiplayer information"])
    if count < 1:
        return found
    struct_name = next(f.struct for o, f in defs.fields("Globals") if f.name == "multiplayer information")
    flag_object = _reference(xmap, pointer + _offsets(defs, struct_name)["flag"])
    if flag_object is None or flag_object.address is None:
        return found
    count, pointer = _block(xmap, flag_object.address + _offsets(defs, "Object")["widgets"])
    reference = _offsets(defs, "ObjectWidget")["reference"]
    widgets = [_reference(xmap, pointer + i * defs.size("ObjectWidget") + reference) for i in range(max(0, count))]
    for t in widgets:
        if t is None or t.group_name != "flag" or t.address is None:
            continue
        for name in ("red flag shader", "blue flag shader"):
            shader = _reference(xmap, t.address + fields[name])
            if shader is not None and shader not in found:
                found.append(shader)
    return found


# the transparent shaders whose maps are read here: the Xbox's generic ones,
# and the chicago ones the PC's maps have in their place (pc_prepare.py)
SHADER_MAPS = {"sotr": ("ShaderTransparentGeneric", "ShaderTransparentGenericMap"),
               "schi": ("ShaderTransparentChicago", "ShaderTransparentChicagoMap")}


def _maps(xmap, defs, shader):
    """a transparent shader's maps: each one's bitmap and its u animation
    (function, period, phase, scale); [] for another group"""
    if shader is None or shader.group_name not in SHADER_MAPS:
        return []
    shader_struct, map_struct = SHADER_MAPS[shader.group_name]
    mf = _offsets(defs, map_struct)
    count, pointer = _block(xmap, shader.address + _offsets(defs, shader_struct)["maps"])
    maps = []
    for i in range(max(0, min(count, 4))):
        address = pointer + i * defs.size(map_struct)
        function = struct.unpack("<h", xmap.read(address + mf["u animation function"], 2))[0]
        period, phase, scale = struct.unpack("<3f", xmap.read(address + mf["u animation period"], 12))
        maps.append((_reference(xmap, address + mf["map"]), (function, period, phase, scale)))
    return maps


def _first_map(xmap, defs, shader):
    """a transparent shader's first map's bitmap and its u animation"""
    maps = _maps(xmap, defs, shader)
    return maps[0] if maps else (None, (0, 1.0, 0.0, 0.0))


def _chicago_look(xmap, defs, shader, size):
    """a chicago shader's colour (the PC's hill shader: the generic one's
    maps, without its stages), as the generic one's stages make it: the first
    map's colour, and the second's where the first's alpha is"""
    images = []
    for bitmap, _ in _maps(xmap, defs, shader)[:2]:
        found = bitmap_images(xmap, bitmap, defs) if bitmap is not None else []
        if not found:
            return None
        try:
            images.append(decode(xmap, found[0], size).convert("RGBA").resize((size, size), Image.BILINEAR))
        except ValueError:
            return None
    if not images:
        return None
    first = list(images[0].getdata())
    second = list(images[1].getdata()) if len(images) > 1 else [(0, 0, 0, 0)] * len(first)
    look = Image.new("RGB", (size, size))
    look.putdata([tuple(min(255, a[k] + a[3] * b[k] // 255) for k in range(3)) for a, b in zip(first, second)])
    return look


def _flicker(xmap, defs, shader):
    """the last stage's animated constant colour (function, period, lower bound
    as a share of the upper), when it scales the result; else none (a chicago
    shader has no stages)"""
    if shader.group_name != "sotr":
        return 0, 1.0, 1.0
    gf = _offsets(defs, "ShaderTransparentGeneric")
    sf = _offsets(defs, "ShaderTransparentGenericStage")
    count, pointer = _block(xmap, shader.address + gf["stages"])
    if count < 1:
        return 0, 1.0, 1.0
    last = pointer + (count - 1) * defs.size("ShaderTransparentGenericStage")
    function = struct.unpack("<h", xmap.read(last + sf["color0 animation function"], 2))[0]
    period = struct.unpack("<f", xmap.read(last + sf["color0 animation period"], 4))[0]
    lower = max(struct.unpack("<4f", xmap.read(last + sf["color0 animation lower bound"], 16))[1:])
    upper = max(struct.unpack("<4f", xmap.read(last + sf["color0 animation upper bound"], 16))[1:])
    if function == 0 or upper <= 0.0 or lower <= 0.0 or lower >= upper or period <= 0.0:
        return 0, 1.0, 1.0
    return function, period, lower / upper


def hill_entry(xmap, defs, shader):
    """the band's entry and texels, or None (the shader's look not found)"""
    from .stages import evaluate
    if shader is None or shader.group_name not in SHADER_MAPS:
        return None
    look = evaluate(xmap, defs, shader, 256) if shader.group_name == "sotr" else _chicago_look(xmap, defs, shader, 256)
    if look is None:
        return None
    width, height = HILL_SIZE
    # (as RGB: the look's alpha is not the band's, and Pillow shrinks RGBA
    # premultiplied)
    look = look.convert("RGB").crop((0, int(256 * HILL_V_TOP), 256, 256)).resize((width, height), Image.BOX)
    pixels = list(look.getdata())
    peak = max(max(p) for p in pixels)
    if peak < 8:
        return None
    # the hue: the texels' colour, each as bright as it is
    total = [sum(p[k] for p in pixels) for k in range(3)]
    top = max(total)
    colour = tuple(max(0, min(255, round(255 * c / top))) for c in total)
    levels = [min(15, round(15 * max(p) / peak)) for p in pixels]
    texels = bytes(levels[i] << 4 | levels[i + 1] for i in range(0, len(levels), 2))
    flicker_function, flicker_period, flicker_lower = _flicker(xmap, defs, shader)
    # (the look is evaluated at the flicker's lower bound: the brightest
    # texel's alpha at its upper)
    alpha = min(255, round(peak / flicker_lower))
    _, (u_function, u_period, u_phase, u_scale) = _first_map(xmap, defs, shader)
    if u_period <= 0.0:
        u_function = 0
    return (KIND_HILL, FORMAT_I4, width, height, u_function, flicker_function, colour + (alpha,),
            (u_period if u_function else 0.0, u_scale if u_function else 0.0, flicker_period, flicker_lower)), texels


def flag_entry(xmap, defs, shader):
    """a flag cloth's entry and texels, or None"""
    if shader is None or shader.group_name not in SHADER_MAPS:
        return None
    bitmap, _ = _first_map(xmap, defs, shader)
    images = bitmap_images(xmap, bitmap, defs) if bitmap is not None else []
    if not images:
        return None
    width, height = FLAG_SIZE
    try:
        img = decode(xmap, images[0], max(width, height)).convert("RGBA")
    except ValueError:
        return None
    # (Pillow shrinks RGBA premultiplied: no dark fringe at the torn edges)
    small = img.resize((width, height), Image.BOX)
    texels = bytearray()
    for r, g, b, a in small.getdata():
        value = (r >> 3) << 11 | (g >> 3) << 6 | (b >> 3) << 1 | (1 if a >= 128 else 0)
        texels += struct.pack(">H", value)
    return (KIND_FLAG, FORMAT_RGBA16, width, height, 0, 0, (255, 255, 255, 255), (0.0, 0.0, 0.0, 1.0)), bytes(texels)


def write(xmap, defs, path):
    """fx/<map>.dyn; returns its statistics"""
    entries = {}
    shader = hill_shader(xmap, defs)
    made = hill_entry(xmap, defs, shader)
    if made is not None:
        entries[shader.index] = made
    for shader in flag_shaders(xmap, defs):
        made = flag_entry(xmap, defs, shader)
        if made is not None and shader.index not in entries:
            entries[shader.index] = made
    indices = sorted(entries)
    offset = 16 + ENTRY.size * len(indices)
    table = bytearray()
    body = bytearray()
    for index in indices:
        (kind, fmt, width, height, u_function, flicker_function, colour, floats), texels = entries[index]
        table += ENTRY.pack(index, kind, fmt, width, height, u_function, flicker_function, *colour, *floats,
                            offset + len(body))
        body += texels
        body += b"\0" * ((-len(body)) % 8)
    out = MAGIC + struct.pack(">HH", VERSION, len(indices)) + b"\0" * 8 + table + body
    Path(path).parent.mkdir(parents=True, exist_ok=True)
    Path(path).write_bytes(out)
    return {"hill": sum(1 for e in entries.values() if e[0][0] == KIND_HILL),
            "flag shaders": sum(1 for e in entries.values() if e[0][0] == KIND_FLAG), "bytes": len(out)}


def main():
    import argparse
    from .tagdefs import TagDefinitions
    from .xboxmap import XboxMap
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("map", type=Path)
    parser.add_argument("--filesystem", type=Path, required=True)
    args = parser.parse_args()
    xmap = XboxMap(args.map)
    stats = write(xmap, TagDefinitions(), args.filesystem / "fx" / f"{xmap.name}.dyn")
    print(f"{xmap.name} dynamic geometry: " + ", ".join(f"{k} {v}" for k, v in stats.items()))


if __name__ == "__main__":
    main()
