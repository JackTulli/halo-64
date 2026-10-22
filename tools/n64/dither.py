"""More dithering for the low-colour textures (the texture picker's option):
the 16 colours of a CI4 texture and the 16 (I4) or 8 (IA4) greys. Without it
a texel takes the nearest step, and a smooth gradient between two steps comes
out as bands; with it each texel's rounding error is spread to the texels
right of and below it (Floyd-Steinberg: 7, 3, 5 and 1 sixteenths), so the
shades between the steps come back as a fine pattern, which the RDP's
filtering softens. The palettes are the converter's own (level.py
split_palette, cutout_palette; a median cut for a plain texture): only which
entry each texel takes changes.
"""

from PIL import Image

# (dx, dy, sixteenths)
SPREAD = ((1, 0, 7), (-1, 1, 3), (0, 1, 5), (1, 1, 1))


def dither_values(values, width, steps):
    """values (0..1, row by row) to multiples of 1/steps, the error spread"""
    work = list(values)
    height = len(work) // width
    out = [0.0] * len(work)
    for y in range(height):
        for x in range(width):
            i = y * width + x
            value = work[i]
            q = max(0, min(steps, round(value * steps))) / steps
            out[i] = q
            error = value - q
            for dx, dy, w in SPREAD:
                xx, yy = x + dx, y + dy
                if 0 <= xx < width and yy < height:
                    work[yy * width + xx] += error * w / 16
    return out


def grey_steps(grey, top, steps, dither=False):
    """a grey image (L) stretched to its brightest texel (top) and put on
    steps + 1 levels (15: I4, 7: IA4), as level.py grey_texture does; with
    dither the error spread"""
    if not dither:
        return grey.point(lambda v: int(round(round(v / top * steps) * 255 / steps)))
    values = dither_values([min(1.0, v / top) for v in grey.getdata()], grey.width, steps)
    out = Image.new("L", grey.size)
    out.putdata([int(round(v * 255)) for v in values])
    return out


def dither_indices(pixels, width, palette, candidates):
    """each texel's palette entry, the error spread: pixels (r, g, b) row by
    row; palette [(r, g, b)]; candidates(i) the entries texel i may take, or
    an int: the entry it takes whatever its colour (a cut-out's), its error
    spread to no one"""
    work = [list(p) for p in pixels]
    height = len(work) // width
    fixed = {}
    out = [0] * len(work)
    for y in range(height):
        for x in range(width):
            i = y * width + x
            allowed = candidates(i)
            if isinstance(allowed, int):
                out[i] = allowed
                fixed[i] = True
                continue
            p = work[i]
            best = min(allowed, key=lambda e: sum((palette[e][k] - p[k]) ** 2 for k in range(3)))
            out[i] = best
            error = [p[k] - palette[best][k] for k in range(3)]
            for dx, dy, w in SPREAD:
                xx, yy = x + dx, y + dy
                if 0 <= xx < width and yy < height:
                    j = yy * width + xx
                    for k in range(3):
                        work[j][k] += error[k] * w / 16
    return out


def _palette(image):
    entries = image.getpalette()[:48]
    entries += [0] * (48 - len(entries))
    return [tuple(entries[i:i + 3]) for i in range(0, 48, 3)]


def colours(img, dither=False):
    """img (RGB) in 16 colours (median cut), as a paletted image"""
    rgb = img.convert("RGB")
    quantized = rgb.quantize(colors=16, method=Image.Quantize.MEDIANCUT,
                             dither=Image.Dither.NONE)
    # (16 entries always: an image of fewer colours had a shorter palette,
    # which mksprite kept -- a CI4 load reads 16)
    palette = _palette(quantized)
    used = sorted({index for _, index in quantized.getcolors(256)})
    quantized.putpalette([c for colour in palette for c in colour])
    if not dither:
        return quantized
    # (the entries the median cut made, not the padding)
    indices = dither_indices(list(rgb.getdata()), rgb.width, palette, lambda i: used)
    quantized.putdata(indices)
    return quantized


def split(img, mask, cutout=False, dither=False):
    """level.py split_palette (the player's colour: entries 8-15 inside the
    mask), the entries chosen with the error spread"""
    from .level import split_palette, CUTOUT_ALPHA
    made = split_palette(img, mask, cutout=cutout)
    out, transparency = made if cutout else (made, None)
    if not dither:
        return out, transparency
    palette = _palette(out)
    rgba = list(img.convert("RGBA").getdata())
    inside = [m >= 128 for m in mask.getdata()]

    def candidates(i):
        if cutout and rgba[i][3] < CUTOUT_ALPHA:
            return 7
        if inside[i]:
            return range(8, 16)
        return range(7) if cutout else range(8)
    out.putdata(dither_indices([p[:3] for p in rgba], img.width, palette, candidates))
    return out, transparency


def cutout(img, dither=False):
    """level.py cutout_palette (entry 15 transparent), the entries chosen with
    the error spread"""
    from .level import cutout_palette, CUTOUT_ALPHA
    out, transparency = cutout_palette(img)
    if not dither:
        return out, transparency
    palette = _palette(out)
    rgba = list(img.convert("RGBA").getdata())
    out.putdata(dither_indices([p[:3] for p in rgba], img.width, palette,
                               lambda i: 15 if rgba[i][3] < CUTOUT_ALPHA else range(15)))
    return out, transparency
