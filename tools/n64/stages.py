"""A transparent generic shader's look ('sotr'), evaluated on the CPU.

Halo draws such a shader with up to seven register-combiner stages over its
maps; the N64 draws one texture. For shaders whose colour lives in the
stages (the overshield's and active camouflage's energy: grey noise maps,
coloured by the stages' constant colours) the stages are evaluated here,
texel by texel, into that texture: the maps sampled where they lie (their
animations still), vertex colours and fades full, the object's functions 0
(a constant colour "controlled by A out" is its lower bound), outputs as
the combiners give them (mux on scratch alpha 0, scale and bias, clamped to
-1..1). The final colour is scratch colour 0, its alpha scratch alpha 0.
"""

import struct

from PIL import Image

from .bitmaps import bitmap_images, decode

ZERO_ONE = (0.0, 1.0, 0.5, -1.0, -0.5)
OUTPUT_SCALE = (1.0, 0.5, 2.0, 4.0, 1.0, 1.0)
OUTPUT_BIAS = (0.0, 0.0, 0.0, 0.0, -0.5, 0.0)


def _clamp(x, lo=0.0, hi=1.0):
    return lo if x < lo else hi if x > hi else x


def _mapping(x, kind):
    """an input's mapping: clamp(x), 1 - clamp(x), expand, negated expand,
    half bias, negated half bias, signed, signed negated"""
    c = _clamp(x)
    return (c, 1.0 - c, 2.0 * c - 1.0, 1.0 - 2.0 * c, c - 0.5, 0.5 - c, x, -x)[kind]


def evaluate(xmap, defs, tag, size):
    """the shader's colour (RGB) and alpha (A) as a size x size image, or None"""
    gf = {f.name: o for o, f in defs.fields("ShaderTransparentGeneric")}
    mf = {f.name: o for o, f in defs.fields("ShaderTransparentGenericMap")}
    map_size = defs.size("ShaderTransparentGenericMap")
    stage_size = defs.size("ShaderTransparentGenericStage")
    count, pointer = struct.unpack("<iI", xmap.read(tag.address + gf["maps"], 8))
    maps = []
    for i in range(min(count, 4)):
        datum = struct.unpack("<I", xmap.read(pointer + i * map_size + mf["map"] + 12, 4))[0]
        bitmap = xmap.tag(datum)
        images = bitmap_images(xmap, bitmap, defs) if bitmap is not None else []
        if not images:
            return None
        try:
            img = decode(xmap, images[0], size)
        except ValueError:
            return None
        maps.append(img.convert("RGBA").resize((size, size), Image.BILINEAR).load())
    count, pointer = struct.unpack("<iI", xmap.read(tag.address + gf["stages"], 8))
    stages = [xmap.read(pointer + i * stage_size, stage_size) for i in range(count)]
    if not maps or not stages:
        return None

    out = Image.new("RGBA", (size, size))
    pixels = out.load()
    for y in range(size):
        for x in range(size):
            texel = [tuple(c / 255.0 for c in m[x, y]) for m in maps]
            while len(texel) < 4:
                texel.append((0.0, 0.0, 0.0, 0.0))
            scratch_color = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]]
            scratch_alpha = [0.0, 0.0]
            for raw in stages:
                flags = struct.unpack_from("<H", raw, 0)[0]
                lower = struct.unpack_from("<4f", raw, 0x0C)  # a r g b
                c1 = struct.unpack_from("<4f", raw, 0x2C)
                constant_color = ((lower[1], lower[2], lower[3]), (c1[1], c1[2], c1[3]))
                constant_alpha = (lower[0], c1[0])

                def color_input(i):
                    if i < 5:
                        return [ZERO_ONE[i]] * 3
                    if i < 9:
                        return list(texel[i - 5][:3])
                    if i in (9, 10):
                        return [1.0] * 3
                    if i in (11, 12):
                        return list(scratch_color[i - 11])
                    if i in (13, 14):
                        return list(constant_color[i - 13])
                    if i < 19:
                        return [texel[i - 15][3]] * 3
                    if i in (19, 20):
                        return [1.0] * 3
                    if i in (21, 22):
                        return [scratch_alpha[i - 21]] * 3
                    return [constant_alpha[i - 23]] * 3

                def alpha_input(i):
                    if i < 5:
                        return ZERO_ONE[i]
                    if i < 9:
                        return texel[i - 5][3]
                    if i in (9, 10):
                        return 1.0
                    if i in (11, 12):
                        return scratch_alpha[i - 11]
                    if i in (13, 14):
                        return constant_alpha[i - 13]
                    if i < 19:
                        return texel[i - 15][2]
                    if i in (19, 20):
                        return 1.0
                    if i in (21, 22):
                        return scratch_color[i - 21][2]
                    return constant_color[i - 23][2]

                color_in = struct.unpack_from("<8H", raw, 0x3C)
                color_out = struct.unpack_from("<6H", raw, 0x4C)
                alpha_in = struct.unpack_from("<8H", raw, 0x58)
                alpha_out = struct.unpack_from("<4H", raw, 0x68)
                c = [[_mapping(v, color_in[2 * j + 1]) for v in color_input(color_in[2 * j])] for j in range(4)]
                a = [_mapping(alpha_input(alpha_in[2 * j]), alpha_in[2 * j + 1]) for j in range(4)]
                mux_high = scratch_alpha[0] >= 0.5

                # colour: A*B (or A.B), C*D (or C.D), their sum or mux
                if color_out[1] == 1:
                    ab = [sum(c[0][k] * c[1][k] for k in range(3))] * 3
                else:
                    ab = [c[0][k] * c[1][k] for k in range(3)]
                if color_out[3] == 1:
                    cd = [sum(c[2][k] * c[3][k] for k in range(3))] * 3
                else:
                    cd = [c[2][k] * c[3][k] for k in range(3)]
                total = (cd if mux_high else ab) if flags & 1 else [ab[k] + cd[k] for k in range(3)]
                cm = min(color_out[5], 5)
                fix = lambda v, m=cm: _clamp((v + OUTPUT_BIAS[m]) * OUTPUT_SCALE[m], -1.0, 1.0)
                ab, cd, total = [fix(v) for v in ab], [fix(v) for v in cd], [fix(v) for v in total]
                # alpha
                aab, acd = a[0] * a[1], a[2] * a[3]
                asum = (acd if mux_high else aab) if flags & 2 else aab + acd
                am = min(alpha_out[3], 5)
                afix = lambda v, m=am: _clamp((v + OUTPUT_BIAS[m]) * OUTPUT_SCALE[m], -1.0, 1.0)
                aab, acd, asum = afix(aab), afix(acd), afix(asum)

                new_color = [list(scratch_color[0]), list(scratch_color[1])]
                new_alpha = list(scratch_alpha)
                for target, value in ((color_out[0], ab), (color_out[2], cd), (color_out[4], total)):
                    if target in (1, 2):
                        new_color[target - 1] = value
                for target, value in ((alpha_out[0], aab), (alpha_out[1], acd), (alpha_out[2], asum)):
                    if target in (1, 2):
                        new_alpha[target - 1] = value
                scratch_color, scratch_alpha = new_color, new_alpha
            pixels[x, y] = tuple(int(_clamp(v) * 255) for v in scratch_color[0]) + (int(_clamp(scratch_alpha[0]) * 255),)
    return out
