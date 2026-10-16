"""Converting a structure BSP's render geometry for the N64 renderer.

The level is drawn as tiny3d vertex batches (port/n64/src/n64_render.c):
each batch is up to 70 vertices (the RSP's vertex cache) and the triangles
that use them, with one material. Batches are grouped by BSP cluster, so a
view can skip clusters outside its frustum.

There are no lightmaps (the first milestone leaves them out): each vertex
is lit by its material's own lighting summary from the lightmapper (ambient
colour and two distant lights), against the vertex normal.

The output file (big-endian):

    header         'H64L', version, counts and offsets (struct below)
    textures[]     name[24] (rom:/tex/<map>/<name>.sprite), width, height
    materials[]    texture index, flags
    clusters[]     bounds min[3], max[3] (floats, world units), first batch, batch count,
                   first owner, owner count: drawn when any of its owners (BSP clusters,
                   in owners[]) is in the eye's potentially visible set; none, always
    owners[]       u16 BSP cluster indices, padded to 4 bytes
    batches[]      material, vertex count, triangle count, vertex pair offset, index offset,
                   bounds min[3], max[3] (s16, N64 units)
    vertices       tiny3d T3DVertPacked, two vertices per 32 bytes
    indices        each batch's triangles as tiny3d strips (strip_chunks), in chunks
                   of an 8-byte head (u16 index count, u16 another chunk follows)
                   and u16 indices, 8-byte aligned

Positions are stored in fixed point, world units * POSITION_SCALE.
"""

import math
import os
import struct
import subprocess
from pathlib import Path

from PIL import Image

from . import toolpaths
from .bitmaps import bitmap_images, decode

POSITION_SCALE = 64.0
TEXTURE_FORMAT = os.environ.get("N64_TEXTURE_FORMAT", "CI4")
# the RSP's vertex cache holds 70; a batch's strips' indices are read into
# the slots it leaves (six of 36 bytes: STRIP_CHUNK_INDICES at a time)
MAXIMUM_BATCH_VERTICES = 64
STRIP_CHUNK_INDICES = 104
MAXIMUM_UV_EXTENT = 512.0  # texels; s10.5 holds +-1024, keep well inside it
# a greyscale level texture (LevelBuilder.greyscale): 4 bits a texel, all
# 4 KB of TMEM (no palette), twice a colour texture's texels
GREYSCALE_TEXELS = 8192
# its batches' uv extent: twice the texels a side would double the batches
# at MAXIMUM_UV_EXTENT; a batch's coordinates are centred on 0 (write), so
# they stay within half of it and half a repeat, inside s10.5's +-1024 (a
# batch's first triangle is taken whatever its extent: up to ~2000 texels)
GREYSCALE_UV_EXTENT = 832.0
VERSION = 4
# a streamed level's chunks hold this much at most (the N64 reads one in
# ~2 ms; a view needs ~90 KB of them, 230 KB at most in nine places of ten)
CHUNK_BYTES = 8 * 1024

MATERIAL_ALPHA_TEST = 1
MATERIAL_TWO_SIDED = 2
MATERIAL_TRANSPARENT = 4
MATERIAL_UNTEXTURED = 8
# a model batch whose texture's palette is split for a player's colour
# (split_palette)
MATERIAL_CHANGE_COLOR = 16
# a transparent model shader drawn as a cut-out glow (models.py glow_texture)
MATERIAL_GLOW = 32
# natural terrain: the texture is a detail map, the vertices carry the base
# map's colour (LevelBuilder.terrain_materials)
MATERIAL_DETAIL = 128
# a greyscale texture (grey_texture): the renderer doubles texture x light
# in a second cycle, Halo's doubling
MATERIAL_DOUBLE = 256
# a natural material's detail, drawn over it (overlay_material)
MATERIAL_OVERLAY = 512
# a MATERIAL_DOUBLE vertex's light at most (per channel, of 255): the RDP's
# combiner does not clamp -- the doubling cycle's 1.0-1.5 clips to white but
# 1.5-2.0 wraps to black -- so texture x light must stay under 0.75. Above
# it Sidewinder's bright snow (the cliffs' light x the stretch x the
# overlay's brightening, up to ~1.1) was speckled black, blue and cyan where
# one channel or all wrapped. Halo's own min(1, 2 x texture x light) is kept
# up to 0.75, 1.5 x texture past it (the brightest texels clip to white)
DOUBLE_SHADE_LIMIT = 191
# Halo's water (shader_transparent_water): untextured, its vertices one flat
# colour (water_colour), blended at the renderer's WATER_ALPHA -- the
# cheapest transparent surface: no texture load, no lightmap splits. (Its
# base map's alpha, ~0.2, was lost in CI4's one-bit palette alpha, and the
# vertices of a surface without a lightmap had no light: the creek drew black)
MATERIAL_WATER = 1024
# a model glow of one emitted colour (models.py glow_texture: a teleporter's
# energy), drawn translucent (the renderer's GLOW_ALPHA)
MATERIAL_BLEND = 2048
# natural terrain whose colour map is in tiles of its own (terrain_tiles.py:
# a tile a batch, the lightmap baked in), the detail map drawn over them
MATERIAL_TILED = 4096
# the water's colour: its perpendicular and parallel tints' mean (by their
# brightness), dimmed about as far as its reflection map darkens it
WATER_SHADE = 0.7
# a water shader without a tint of its own
WATER_DEFAULT_COLOUR = (0.12, 0.16, 0.45)
# chicago shaders drawn as water where LevelBuilder.chicago_water is set
WATER_NAMES = ("water", "river", "stream")
# and there, water whose own colour is grey (a30's: Halo's blue is the sky
# in its reflection, which the N64 has not -- the user: "the water looks
# grey not blue") is a river's blue, or a waterfall's paler one
CAMPAIGN_WATER_COLOUR = (0.16, 0.30, 0.44)
CAMPAIGN_WATERFALL_COLOUR = (0.42, 0.53, 0.62)
WATER_GREY_SPREAD = 0.06

# rock, earth, snow, wood (Halo's material types: dirt, sand, stone, snow,
# wood, leaves, ice) with a detail map have it drawn over them in a second
# pass (overlay_material): one pass cannot take a colour texture, a detail
# texture and the vertices' light (three products, two combiner cycles). The
# overlay darkens what the first drew by its texel (the RDP's blender wraps
# brighter), the first drawn brighter by the overlay's mean (at least
# OVERLAY_MEAN: the detail's contrast is cut to keep it there; at 0.75 the
# brightening saturated bright rock, whose colour then went grey)
NATURAL_MATERIAL_TYPES = {0, 1, 2, 3, 4, 29, 31}
OVERLAY_MEAN = 0.88
# the overlay's darkest level (of 15): never black -- consoles showed black
# smudges on these passes away from the eye; whatever goes wrong there, a
# texel darkens by a third at most
OVERLAY_FLOOR_LEVEL = 5

# natural terrain: an environment shader with detail maps whose base map
# spans at least TERRAIN_REPEAT_UNITS world units a repeat -- a colour map
# laid over the ground (Blood Gulch's spans the map), the grain of grass and
# sand left to the detail maps. The N64 cannot sample both well (the detail
# needs coordinates a hundred times finer than s10.5 carries over the base
# map's span), so the base map's colour goes on the vertices, sampled from
# it at TERRAIN_SAMPLES a side (the ground's vertices are about that far
# apart; edges are also split where its colour changes by more than
# TERRAIN_SPLIT_DIFFERENCE), and the texture is the detail map, greyscale,
# DETAIL_SIZE a side with mipmaps (they fade it to grey in the distance)
TERRAIN_REPEAT_UNITS = 40.0
TERRAIN_SAMPLES = 64
TERRAIN_SPLIT_DIFFERENCE = 0.08
DETAIL_SIZE = 64
# an environment texture keeps its colour (needs_colour) with a mean
# saturation (HSV, 0-255) or a mean colour's spread of channels at least
# these: the Covenant's purples and blues, rock and earth, tinted metal
COLOUR_SATURATION = 38
COLOUR_TINT = 25

from .toolpaths import tool  # noqa: E402

MKSPRITE = tool("mksprite")


def unpack_normal(value):
    """Halo's compressed normal: 11, 11 and 10 signed bits"""
    def signed(v, bits):
        return v - (1 << bits) if v & (1 << (bits - 1)) else v
    x = signed(value & 0x7FF, 11) / 1023.0
    y = signed((value >> 11) & 0x7FF, 11) / 1023.0
    z = signed((value >> 22) & 0x3FF, 10) / 511.0
    return x, y, z


def pack_normal(n):
    """tiny3d's 5.6.5 normal"""
    x, y, z = n
    length = math.sqrt(x * x + y * y + z * z) or 1.0
    x, y, z = x / length, y / length, z / length
    xi = max(-16, min(15, int(round(x * 15.5))))
    yi = max(-32, min(31, int(round(y * 31.5))))
    zi = max(-16, min(15, int(round(z * 15.5))))
    return ((xi & 31) << 11) | ((yi & 63) << 5) | (zi & 31)


class Texture:
    def __init__(self, name, width, height, colour=(1.0, 1.0, 1.0)):
        self.name, self.width, self.height = name, width, height
        # the texture's average colour (0..1), for drawing it untextured
        self.colour = colour


# an alpha-tested texture (cutout_texture): texels under this alpha are cut
# out (Halo's test is at 0x7F); one with fewer than this share of them is
# drawn whole
CUTOUT_ALPHA = 128
CUTOUT_MINIMUM_SHARE = 0.005


# the colours Halo fills a lightmap page's unused texels with
LIGHTMAP_FILL = ((255, 255, 0), (255, 0, 0))
# a level triangle is split where its lightmap changes across it (shadows,
# pools of light), an edge at most this many times: at an edge of more than
# LIGHTMAP_SPLIT_TEXELS texels whose middle differs from its ends' average
# by more than LIGHTMAP_SPLIT_DIFFERENCE. Each triangle costs the RSP and RDP:
# Wizard's 2,972 become 3,649 (depth 3, 2 texels, 0.06 made 5,090 and cost
# ~10 ms a frame)
LIGHTMAP_SPLIT_DEPTH = int(os.environ.get("N64_LIGHTMAP_SPLIT_DEPTH", "2"))
LIGHTMAP_SPLIT_TEXELS = 4.0
LIGHTMAP_SPLIT_DIFFERENCE = 0.1


def sample_lightmap(page, u, v):
    """the lightmap's colour (0..1) around (u, v): the texels within a texel
    and a half, weighted by distance, the page's unused texels left out
    (a vertex is usually on the edge of its chart); None if there are none"""
    width, height, pixels = page
    x, y = u * width - 0.5, v * height - 0.5
    for radius in (1.5, 3.0):
        total = [0.0, 0.0, 0.0]
        weight_sum = 0.0
        for ty in range(int(math.floor(y - radius)), int(math.ceil(y + radius)) + 1):
            for tx in range(int(math.floor(x - radius)), int(math.ceil(x + radius)) + 1):
                if not (0 <= tx < width and 0 <= ty < height):
                    continue
                d = math.hypot(tx - x, ty - y)
                if d > radius:
                    continue
                texel = pixels[ty * width + tx]
                if texel in LIGHTMAP_FILL:
                    continue
                w = radius - d + 0.25
                total = [t + c * w for t, c in zip(total, texel)]
                weight_sum += w
        if weight_sum > 0:
            return [t / weight_sum / 255.0 for t in total]
    return None


def triangle_strips(triangles):
    """strips (lists of vertex numbers) drawing these triangles with their
    winding, as tiny3d draws a strip: triangle k is (s[k], s[k+1], s[k+2]),
    every other one turned round (s[k+1], s[k], s[k+2])"""
    by_edge = {}
    for i, (a, b, c) in enumerate(triangles):
        for edge, third in (((a, b), c), ((b, c), a), ((c, a), b)):
            by_edge.setdefault(edge, []).append((i, third))
    used = [False] * len(triangles)

    def extend(strip, taken):
        while True:
            n = len(strip)
            edge = (strip[n - 2], strip[n - 1]) if (n - 2) % 2 == 0 else (strip[n - 1], strip[n - 2])
            for i, third in by_edge.get(edge, ()):
                if not used[i] and i not in taken:
                    taken.add(i)
                    strip.append(third)
                    break
            else:
                return strip

    out = []
    for i, triangle in enumerate(triangles):
        if used[i]:
            continue
        best = None
        for r in range(3):
            taken = {i}
            strip = extend(list(triangle[r:] + triangle[:r]), taken)
            if best is None or len(strip) > len(best[0]):
                best = (strip, taken)
        for t in best[1]:
            used[t] = True
        out.append(best[0])
    return out


def strip_chunks(triangles):
    """a batch's triangles as tiny3d strip index buffers (the level file's
    indices): chunks of at most STRIP_CHUNK_INDICES, each strip after a
    chunk's first marked by bit 15 on its first index; a strip too long
    is cut after an even number of triangles (the next piece starts as its
    first, unturned)"""
    pieces = []
    for strip in triangle_strips(triangles):
        while len(strip) > STRIP_CHUNK_INDICES:
            cut = STRIP_CHUNK_INDICES - (STRIP_CHUNK_INDICES % 2)
            pieces.append(strip[:cut])
            strip = strip[cut - 2:]
        pieces.append(strip)
    chunks = [[]]
    for piece in pieces:
        if chunks[-1] and len(chunks[-1]) + len(piece) > STRIP_CHUNK_INDICES:
            chunks.append([])
        chunk = chunks[-1]
        chunk.extend([piece[0] | (0x8000 if chunk else 0)] + piece[1:])
    out = bytearray()
    for i, chunk in enumerate(chunks):
        out += struct.pack(">HH4x", len(chunk), 1 if i + 1 < len(chunks) else 0)
        out += struct.pack(f">{len(chunk)}H", *chunk)
        out += b"\0" * ((-len(out)) % 8)
    return bytes(out)


def needs_colour(img):
    """whether a texture would lose something in greyscale"""
    rgb = img.convert("RGB")
    n = rgb.width * rgb.height
    saturation = sum(rgb.convert("HSV").getchannel("S").getdata()) / n
    mean = [sum(channel.getdata()) / n for channel in rgb.split()]
    return saturation >= COLOUR_SATURATION or max(mean) - min(mean) >= COLOUR_TINT


class BaseSampler:
    """a terrain's base map, TERRAIN_SAMPLES a side: its colour (RGBA,
    0..1) at a texture coordinate, bilinear and repeating"""

    def __init__(self, img):
        # (the colour and the alpha scaled apart: PIL scales RGBA
        # premultiplied, and where the alpha is 0 the colour went black)
        img = img.convert("RGBA")
        size = (TERRAIN_SAMPLES, TERRAIN_SAMPLES)
        small = img.convert("RGB").resize(size, Image.LANCZOS)
        small.putalpha(img.getchannel("A").resize(size, Image.LANCZOS))
        self.pixels = [tuple(c / 255.0 for c in p) for p in small.getdata()]

    def sample(self, uv):
        n = TERRAIN_SAMPLES
        x, y = uv[0] * n - 0.5, uv[1] * n - 0.5
        x0, y0 = math.floor(x), math.floor(y)
        fx, fy = x - x0, y - y0
        out = [0.0, 0.0, 0.0, 0.0]
        for dy, wy in ((0, 1 - fy), (1, fy)):
            for dx, wx in ((0, 1 - fx), (1, fx)):
                p = self.pixels[((y0 + dy) % n) * n + (x0 + dx) % n]
                for k in range(4):
                    out[k] += p[k] * wx * wy
        return out


class Terrain:
    """a natural terrain shader's N64 materials: its detail maps' (the
    primary where the base map's alpha is high, the secondary where low,
    as Halo blends them) and the base map's colours"""

    def __init__(self, sampler, variants):
        self.sampler = sampler
        self.variants = variants
        # the base map whole (RGBA), for its tiles (LevelBuilder.colour_tiles),
        # and its detail maps' effect on it on average (detail_tint)
        self.image = None
        self.tint = None

    def tinted(self, colour):
        """a base map colour (RGBA, 0..1) with its detail maps' average
        (detail_tint): the primary's where its alpha is high, the
        secondary's where low, then the micro detail's"""
        (pm, pa), (sm, sa), (mm, ma) = self.tint
        a = colour[3]
        return [(c * (sm[k] + (pm[k] - sm[k]) * a) + sa[k] + (pa[k] - sa[k]) * a) * mm[k] + ma[k]
                for k, c in enumerate(colour[:3])]


def share_file(path, directory, suffix):
    """moves a made file to directory/<hash of its contents><suffix> (the
    same contents made for another map is one file) and returns the hash"""
    import hashlib
    data = Path(path).read_bytes()
    name = hashlib.sha1(data).hexdigest()[:16]
    target = Path(directory) / f"{name}{suffix}"
    if target.exists() and target.read_bytes() == data:
        Path(path).unlink()
    else:
        os.replace(path, target)
    return name


def write_list(path, names):
    """a map's list of the shared files it uses (tools/n64/prune.py)"""
    Path(path).parent.mkdir(parents=True, exist_ok=True)
    # (newlines alone, on Windows too: the game reads hud/<map>.list in
    # 16-character records and maps/<map>.anims a name a line)
    Path(path).write_text("".join(f"{n}\n" for n in names), newline="\n")


def split_palette(img, mask, cutout=False):
    """img (RGBA) as a 16-colour paletted image: the texels where mask < 128
    take entries 0-7, the others 8-15, each half the image's own colours
    there (median cut). Cut out (alpha-tested), the texels of alpha under
    CUTOUT_ALPHA take entry 7, transparent (its colour the lower half's
    mean), the lower half's colours the 7 others; returns the image and its
    PNG transparency then (None without)"""
    rgba = list(img.convert("RGBA").getdata())
    pixels = [p[:3] for p in rgba]
    inside = [m >= 128 for m in mask.getdata()]
    cut = [cutout and p[3] < CUTOUT_ALPHA for p in rgba]
    palette = []
    for half in (False, True):
        count = 7 if cutout and not half else 8
        colours = [p for p, m, c in zip(pixels, inside, cut) if m == half and not c] or [(0, 0, 0)]
        sample = Image.new("RGB", (len(colours), 1))
        sample.putdata(colours)
        quantized = sample.quantize(colors=count, method=Image.Quantize.MEDIANCUT)
        entries = quantized.getpalette()[:3 * count]
        entries += entries[-3:] * ((3 * count - len(entries)) // 3)
        palette.append([tuple(entries[i:i + 3]) for i in range(0, 3 * count, 3)])
        if count == 7:
            palette[-1].append(tuple(int(sum(c[k] for c in colours) / len(colours)) for k in range(3)))
    out = Image.new("P", img.size)
    out.putpalette([c for half in palette for colour in half for c in colour])
    cache = {}
    indices = []
    for p, m, c in zip(pixels, inside, cut):
        if c:
            indices.append(7)
            continue
        key = (p, m)
        index = cache.get(key)
        if index is None:
            half = palette[1 if m else 0]
            choices = range(7) if cutout and not m else range(8)
            best = min(choices, key=lambda i: (half[i][0] - p[0]) ** 2 + (half[i][1] - p[1]) ** 2 + (half[i][2] - p[2]) ** 2)
            index = cache[key] = best + (8 if m else 0)
        indices.append(index)
    out.putdata(indices)
    if cutout:
        return out, bytes([255] * 7 + [0] + [255] * 8)
    return out


def cutout_palette(img):
    """img (RGBA) as a 16-colour paletted image for an alpha-tested texture
    and its PNG transparency: the texels of alpha under CUTOUT_ALPHA take
    entry 15, transparent, its colour the others' mean (the RDP filters the
    palette's colours: the edges mix it in), the others entries 0-14 (median
    cut of their colours)"""
    pixels = list(img.convert("RGBA").getdata())
    opaque = [p[:3] for p in pixels if p[3] >= CUTOUT_ALPHA] or [(0, 0, 0)]
    sample = Image.new("RGB", (len(opaque), 1))
    sample.putdata(opaque)
    entries = sample.quantize(colors=15, method=Image.Quantize.MEDIANCUT).getpalette()[:45]
    entries += entries[-3:] * ((45 - len(entries)) // 3)
    palette = [tuple(entries[i:i + 3]) for i in range(0, 45, 3)]
    mean = tuple(int(sum(c[k] for c in opaque) / len(opaque)) for k in range(3))
    cache = {}
    indices = []
    for p in pixels:
        if p[3] < CUTOUT_ALPHA:
            indices.append(15)
            continue
        index = cache.get(p[:3])
        if index is None:
            index = cache[p[:3]] = min(range(15), key=lambda i: sum((palette[i][k] - p[k]) ** 2 for k in range(3)))
        indices.append(index)
    out = Image.new("P", img.size)
    out.putpalette([c for colour in palette + [mean] for c in colour])
    out.putdata(indices)
    return out, bytes([255] * 15 + [0])


def is_light_volume(shader_name):
    """a light cone or volume's shader by its name (the maps' spotlights'
    b40_volumelight_nolight, Derelict's and Damnation's conelights): the
    N64 leaves them out, level and models alike"""
    name = shader_name.lower()
    return any(k in name for k in ("volumelight", "conelight", "light cone", "lightcone", "light volume"))


class LevelBuilder:
    def __init__(self, xmap, defs, map_name, out_dir, split_depth=None):
        self.map = xmap
        # how many times a triangle may be halved where the lightmap changes
        # across it (0: the BSP's own triangles, for 3-4 player games)
        self.split_depth = LIGHTMAP_SPLIT_DEPTH if split_depth is None else split_depth
        self.defs = defs
        self.map_name = map_name
        self.out_dir = Path(out_dir)
        self.textures = []
        # the shared textures this map uses (tex/<map>.list)
        self.used_textures = set()
        self.texture_by_bitmap = {}
        self.materials = []
        self.material_by_shader = {}
        self.texture_prefix = ""
        # Halo's environment shader doubles texture x light; the N64 draws
        # the level in one RDP cycle, so the doubling is in the texture
        self.texture_brightness = 2.0
        # the level's environment textures in greyscale from the larger
        # original images (grey_texture), the colour left to the lighting
        self.greyscale = False
        self.grey_by_bitmap = {}
        # each material's factor on its vertices' light (grey_texture)
        self.material_shade = []
        # natural terrain (terrain_materials): detail maps for textures,
        # the base map's colour on the vertices
        self.terrain_detail = False
        self.colour_by_bitmap = {}
        self.detail_by_bitmap = {}
        # each material's Terrain (or None) and its texture coordinates'
        # scale (a detail map's)
        self.material_terrain = []
        self.material_uv_scale = []
        # shader datum: its base map's repeat in world units (shader_spans)
        self.shader_span = {}
        # materials left out (dropped_shader)
        self.dropped = set()
        # water materials: their colour (MATERIAL_WATER)
        self.water_colour = {}
        # natural materials' detail drawn over them: material -> overlay
        self.overlay_detail = False
        self.overlay_of = {}
        self.overlay_by_bitmap = {}
        self.split_terrain = None
        # natural terrain's colour map in tiles, a tile a batch with the
        # light baked in (terrain_tiles.py), not on its vertices
        self.colour_tiles = False
        # a campaign level's triangles simplified (decimate.py) within this
        # many world units of where they were: a function of the position
        # (campaign.py: closer where the game is played), or None
        self.decimate_tolerance = None
        self.decimated = [0, 0]
        # alpha-blended chicago shaders named as water (WATER_NAMES) drawn as
        # water (MATERIAL_WATER), their colour their colour maps' mean: a30's
        # rivers and waterfalls (a mask as their first map, the colour in
        # their second) came out as slabs, as Battle Creek's creek did before
        # its water was flat (the user: "fix all the water on the level like
        # we did on beaver creek"); the campaign's level only
        self.chicago_water = False

    def fields(self, struct_name):
        return {f.name: o for o, f in self.defs.fields(struct_name)}

    # ---------- textures

    def texture(self, bitmap_datum):
        if bitmap_datum in self.texture_by_bitmap:
            return self.texture_by_bitmap[bitmap_datum]
        tag = self.map.tag(bitmap_datum)
        index = -1
        if tag is not None:
            images = bitmap_images(self.map, tag, self.defs)
            if images:
                try:
                    img = decode(self.map, images[0], 64)
                except ValueError:
                    img = None
                if img is not None:
                    index = self.write_texture(img, tag.name)
        self.texture_by_bitmap[bitmap_datum] = index
        return index

    def has_cutout(self, bitmap_datum):
        """whether a bitmap has transparent texels to cut out (at least
        CUTOUT_MINIMUM_SHARE of them under CUTOUT_ALPHA)"""
        key = ("holes", bitmap_datum)
        if key not in self.texture_by_bitmap:
            tag = self.map.tag(bitmap_datum)
            images = bitmap_images(self.map, tag, self.defs) if tag is not None else []
            try:
                img = decode(self.map, images[0], 64) if images else None
            except ValueError:
                img = None
            alpha = list(img.getchannel("A").getdata()) if img is not None else []
            self.texture_by_bitmap[key] = bool(alpha) and \
                sum(1 for a in alpha if a < CUTOUT_ALPHA) >= CUTOUT_MINIMUM_SHARE * len(alpha)
        return self.texture_by_bitmap[key]

    def cutout_texture(self, bitmap_datum):
        """an alpha-tested texture (Halo's model shaders test the base map's
        alpha unless told not to): its transparent texels kept (write_texture
        cutout), or the plain texture if it has none (has_cutout). Returns
        (texture, whether it is cut out)"""
        key = ("cutout", bitmap_datum)
        if key in self.texture_by_bitmap:
            return self.texture_by_bitmap[key]
        result = (self.texture(bitmap_datum), False)
        if self.has_cutout(bitmap_datum):
            tag = self.map.tag(bitmap_datum)
            img = decode(self.map, bitmap_images(self.map, tag, self.defs)[0], 64)
            result = (self.write_texture(img, tag.name, cutout=True), True)
        self.texture_by_bitmap[key] = result
        return result

    def environment_alpha(self, img, bump_datum, bump_scale=1.0):
        """Environment alpha testing uses the bump map's alpha, not the
        base map's (which can be an entirely opaque DXT1 specular map).
        Keep it with the RGB before CI4/IA4 quantization. With no bump map,
        retain the base alpha as a conservative fallback.
        """
        tag = self.map.tag(bump_datum)
        images = bitmap_images(self.map, tag, self.defs) if tag is not None else []
        if not images:
            return img
        alpha = decode(self.map, images[0], max(img.size)).getchannel("A")
        scale = bump_scale or 1.0
        if not math.isfinite(scale) or scale <= 0.0:
            raise ValueError("invalid environment bump map scale")
        if scale == 1.0:
            alpha = alpha.resize(img.size, Image.LANCZOS)
        else:
            # The bump map repeats in base UV space. Sample pixel centres;
            # wrapping also keeps a non-integral scale's last repeat correct.
            source = alpha.load()
            w, h = alpha.size
            values = []
            for y in range(img.height):
                v = ((y + 0.5) / img.height * scale % 1.0) * h - 0.5
                y0 = math.floor(v); fy = v - y0
                for x in range(img.width):
                    u = ((x + 0.5) / img.width * scale % 1.0) * w - 0.5
                    x0 = math.floor(u); fx = u - x0
                    values.append(round(sum(source[(x0 + dx) % w, (y0 + dy) % h] *
                                            (fx if dx else 1 - fx) * (fy if dy else 1 - fy)
                                            for dx in (0, 1) for dy in (0, 1))))
            alpha = Image.new("L", img.size)
            alpha.putdata(values)
        img = img.copy()
        img.putalpha(alpha)
        return img

    def environment_texture(self, base, bump, bump_scale=1.0):
        """A coloured alpha-tested environment texture, keyed by its mask
        as well as its RGB map; the same base can have different masks.
        """
        key = ("environment-cutout", base, bump, bump_scale or 1.0)
        if key not in self.texture_by_bitmap:
            tag = self.map.tag(base)
            images = bitmap_images(self.map, tag, self.defs) if tag is not None else []
            img = decode(self.map, images[0], 64) if images else None
            self.texture_by_bitmap[key] = self.write_texture(
                self.environment_alpha(img, bump, bump_scale), tag.name, cutout=True) if img is not None else -1
        return self.texture_by_bitmap[key]

    def write_texture(self, img, tag_name, mask=None, fmt=None, cutout=False):
        """a texture from an image; with a mask (L, the image's size: a
        player's colour change), a CI4 texture's palette is split: entries
        0-7 for the texels outside the mask, 8-15 for those in it (the
        renderer tints the upper half); cut out (alpha-tested), a CI4
        texture's transparent texels keep their own palette entry
        (cutout_palette: mksprite's quantizer drops the alpha)"""
        # at most 64 texels a side and 4 KB of TMEM with mipmaps: CI4 64x64
        # is 2 KB, its mipmaps about 0.7 KB more
        fmt = fmt or TEXTURE_FORMAT
        w, h = img.size
        limit = 32.0 if fmt == "RGBA16" else 64.0
        scale = min(1.0, limit / max(w, h))
        w2 = max(4, 1 << int(math.log2(max(1, int(w * scale)))))
        h2 = max(4, 1 << int(math.log2(max(1, int(h * scale)))))
        img = img.resize((w2, h2), Image.LANCZOS)
        if mask is not None:
            mask = mask.resize((w2, h2), Image.BILINEAR)
        if self.texture_brightness != 1.0:
            k = self.texture_brightness
            r, g, b, a = img.split()
            r, g, b = (ch.point(lambda v: min(255, int(v * k))) for ch in (r, g, b))
            img = Image.merge("RGBA", (r, g, b, a))
        index = len(self.textures)
        # made in a directory of the map's own, then shared by content:
        # rom:/tex/<hash>.sprite (every map's weapons are the same)
        work_dir = self.out_dir / "tex" / f".{self.map_name}"
        work_dir.mkdir(parents=True, exist_ok=True)
        png = work_dir / f"{self.texture_prefix}{index:02d}.png"
        if mask is not None and fmt == "CI4" and cutout:
            paletted, transparency = split_palette(img, mask, cutout=True)
            paletted.save(png, transparency=transparency)
        elif mask is not None and fmt == "CI4":
            split_palette(img, mask).save(png)
        elif cutout and fmt == "CI4":
            paletted, transparency = cutout_palette(img)
            paletted.save(png, transparency=transparency)
        else:
            img.save(png)
        painted_png = png.read_bytes() if tag_name.startswith("weapons\\flamethrower\\bitmaps\\flamethrower window") else None
        name = self.make_sprite(png, fmt)
        if painted_png is not None:
            # Exact quantized inputs supplied to mksprite, outside DragonFS.
            audit = self.out_dir.parent / "texture-audit"
            audit.mkdir(exist_ok=True)
            (audit / f"{name}.png").write_bytes(painted_png)
        rgb = img.convert("RGB")
        alpha = list(img.getchannel("A").getdata()) if img.mode == "RGBA" else None
        pixels = [p for i, p in enumerate(rgb.getdata()) if alpha is None or alpha[i] >= 128] or list(rgb.getdata())
        colour = tuple(sum(p[k] for p in pixels) / len(pixels) / 255.0 for k in range(3))
        self.textures.append(Texture(name, w2, h2, colour))
        return index

    def make_sprite(self, png, fmt, mipmaps=False):
        """the sprite of a saved image, shared by content: its name"""
        # (no mipmaps but on the detail maps: the renderer's doubling
        # combiner needs both RDP cycles, and interpolated mipmapping would
        # need one of them)
        toolpaths.run([MKSPRITE, "--format", fmt, "--mipmap", "BOX" if mipmaps else "NONE",
                        "--compress", "1", "-o", str(png.parent), str(png)], check=True, capture_output=True)
        png.unlink()
        name = share_file(png.with_suffix(".sprite"), self.out_dir / "tex", ".sprite")
        self.used_textures.add(name)
        return name

    def grey_texture(self, bitmap_datum, alpha_test, alpha_source=None):
        """an environment texture in greyscale (I4; IA4 with a cut-out):
        GREYSCALE_TEXELS from an original image of up to 256 a side, as
        wide and high as tells the most of it (within twice the original's
        proportions: further, it streaks), its greys stretched to the full
        range. Returns the texture and the factor for its vertices' light
        (the stretch undone; the renderer doubles, MATERIAL_DOUBLE: in the
        texture the doubling turned bright textures white, on the vertices
        bright light dark)."""
        key = (bitmap_datum, alpha_test, alpha_source)
        if key in self.grey_by_bitmap:
            return self.grey_by_bitmap[key]
        tag = self.map.tag(bitmap_datum)
        images = bitmap_images(self.map, tag, self.defs) if tag is not None else []
        try:
            img = decode(self.map, images[0], 256) if images else None
        except ValueError:
            img = None
        if img is None:
            self.grey_by_bitmap[key] = (-1, 1.0)
            return self.grey_by_bitmap[key]
        if alpha_test and alpha_source is not None:
            img = self.environment_alpha(img, *alpha_source)
        if max(img.size) > 256:
            k = 256.0 / max(img.size)
            img = img.resize((max(1, int(img.width * k)), max(1, int(img.height * k))), Image.LANCZOS)
        lum = img.convert("L")
        width, height = lum.size
        texels = min(GREYSCALE_TEXELS, 1 << int(math.log2(max(16, width * height))))
        aspect = width / height
        best = None
        w = 4
        while w <= texels // 4:
            h = texels // w
            if 0.5 <= (w / h) / aspect <= 2.0:
                down = lum.resize((w, h), Image.LANCZOS)
                back = down.resize(lum.size, Image.BILINEAR)
                error = sum((a - b) ** 2 for a, b in zip(lum.getdata(), back.getdata()))
                if best is None or error < best[0]:
                    best = (error, down, w, h)
            w *= 2
        _, grey, w, h = best
        top = max(grey.getdata()) or 1
        levels = 7 if alpha_test else 15
        grey = grey.point(lambda v: int(round(round(v / top * levels) * 255 / levels)))
        index = len(self.textures)
        work_dir = self.out_dir / "tex" / f".{self.map_name}"
        work_dir.mkdir(parents=True, exist_ok=True)
        png = work_dir / f"{self.texture_prefix}g{index:02d}.png"
        if alpha_test:
            alpha = img.getchannel("A").resize((w, h), Image.LANCZOS).point(lambda v: 255 if v >= 128 else 0)
            Image.merge("LA", (grey, alpha)).save(png)
            fmt = "IA4"
        else:
            grey.save(png)
            fmt = "I4"
        name = self.make_sprite(png, fmt)
        shade = top / 255.0
        mean = sum(grey.getdata()) / (w * h) / 255.0 * shade * self.texture_brightness
        self.textures.append(Texture(name, w, h, (mean, mean, mean)))
        self.grey_by_bitmap[key] = (index, shade)
        return self.grey_by_bitmap[key]

    # ---------- shaders

    def material(self, shader_datum):
        if shader_datum in self.material_by_shader:
            return self.material_by_shader[shader_datum]
        tag = self.map.tag(shader_datum)
        texture, flags, shade = -1, 0, 1.0
        if tag is not None:
            group = tag.group_name
            if group == "senv":
                f = self.fields("ShaderEnvironment")
                base = struct.unpack("<I", self.map.read(tag.address + f["base map"] + 12, 4))[0]
                env_flags = struct.unpack("<H", self.map.read(tag.address + f["shader environment flags"], 2))[0]
                bump = struct.unpack("<I", self.map.read(tag.address + f["bump map"] + 12, 4))[0]
                bump_scale = struct.unpack("<f", self.map.read(tag.address + f["bump map scale"], 4))[0] or 1.0
                if env_flags & 1:
                    flags |= MATERIAL_ALPHA_TEST
                span = self.shader_span.get(shader_datum, 0.0)
                # Terrain detail has no alpha channel. Alpha-tested grates
                # and foliage must stay on the composed cutout path even
                # when their BSP UV span would qualify as broad terrain.
                if self.terrain_detail and not env_flags & 1 and span >= TERRAIN_REPEAT_UNITS:
                    first = self.terrain_materials(tag, base, span)
                    if first is not None:
                        self.material_by_shader[shader_datum] = first
                        return first
                if self.greyscale and not self.base_needs_colour(base):
                    texture, shade = self.grey_texture(base, bool(env_flags & 1),
                                                       (bump, bump_scale) if env_flags & 1 else None)
                    if texture >= 0:
                        flags |= MATERIAL_DOUBLE
                else:
                    texture = self.environment_texture(base, bump, bump_scale) if env_flags & 1 else self.texture(base)
                overlay = self.overlay_material(tag, base) if texture >= 0 and not env_flags & 1 else None
                if overlay is not None:
                    shade /= overlay[1]
                    index = self.add_material(texture, flags, shade)
                    self.overlay_of[index] = overlay[0]
                    self.material_by_shader[shader_datum] = index
                    return index
            elif group == "swat":
                index = self.add_material(-1, MATERIAL_TRANSPARENT | MATERIAL_WATER | MATERIAL_UNTEXTURED)
                self.water_colour[index] = self.campaign_water(tag, self.water_tint(tag))
                self.material_by_shader[shader_datum] = index
                return index
            else:
                # transparent shaders: their first map, blended
                flags |= MATERIAL_TRANSPARENT
                if self.dropped_shader(tag):
                    index = self.add_material(-1, flags | MATERIAL_UNTEXTURED)
                    self.dropped.add(index)
                    self.material_by_shader[shader_datum] = index
                    return index
                if self.chicago_water and group == "schi" and any(
                        word in tag.name.split("\\")[-1].lower() for word in WATER_NAMES):
                    index = self.add_material(-1, MATERIAL_TRANSPARENT | MATERIAL_WATER | MATERIAL_UNTEXTURED)
                    self.water_colour[index] = self.campaign_water(tag, self.chicago_water_tint(tag))
                    self.material_by_shader[shader_datum] = index
                    return index
                texture = self.first_map(tag)
        if texture < 0:
            flags |= MATERIAL_UNTEXTURED
        index = self.add_material(texture, flags, shade)
        self.material_by_shader[shader_datum] = index
        return index

    def overlay_material(self, tag, base):
        """a natural material's detail map drawn over it: (the overlay
        material, the mean of its texels -- the first pass's brightening),
        or None"""
        if not self.overlay_detail:
            return None
        f = self.fields("ShaderEnvironment")
        read = lambda field, fmt: struct.unpack(fmt, self.map.read(tag.address + f[field], struct.calcsize(fmt)))[0]
        if read("material type", "<H") not in NATURAL_MATERIAL_TYPES:
            return None
        datum = struct.unpack("<I", self.map.read(tag.address + f["primary detail map"] + 12, 4))[0]
        scale = read("primary detail map scale", "<f")
        if datum == 0xFFFFFFFF or scale <= 0:
            return None
        if datum not in self.overlay_by_bitmap:
            self.overlay_by_bitmap[datum] = self.overlay_texture(datum)
        texture, size, mean = self.overlay_by_bitmap[datum]
        if texture < 0:
            return None
        su = sv = scale
        base_tag = self.map.tag(base)
        images = bitmap_images(self.map, base_tag, self.defs) if base_tag is not None else []
        if read("diffuse flags", "<H") & 1 and images and size:
            su *= images[0].width / size[0]
            sv *= images[0].height / size[1]
        return self.add_material(texture, MATERIAL_OVERLAY, uv_scale=(su, sv)), mean

    def overlay_texture(self, bitmap_datum):
        """a detail map as an overlay's I4 texture with mipmaps, DETAIL_SIZE
        a side: the factor it darkens by, its brightest 1 (Halo's detail
        multiplies by the texel over the map's mean; here over its
        brightest, the contrast cut to keep the mean at OVERLAY_MEAN).
        Returns the texture, the image's size and the texels' mean"""
        tag = self.map.tag(bitmap_datum)
        images = bitmap_images(self.map, tag, self.defs) if tag is not None else []
        try:
            img = decode(self.map, images[0], DETAIL_SIZE) if images else None
        except ValueError:
            img = None
        if img is None:
            return -1, None, 1.0
        grey = img.convert("L").resize((DETAIL_SIZE, DETAIL_SIZE), Image.LANCZOS)
        values = list(grey.getdata())
        top = max(values) or 1
        mean = sum(values) / len(values) / top
        cut = min(1.0, (1.0 - OVERLAY_MEAN) / (1.0 - mean)) if mean < 1.0 else 1.0
        grey = grey.point(lambda v: max(OVERLAY_FLOOR_LEVEL, int(round(15 * (1.0 - cut * (1.0 - v / top))))) * 17)
        levels = list(grey.getdata())
        index = len(self.textures)
        work_dir = self.out_dir / "tex" / f".{self.map_name}"
        work_dir.mkdir(parents=True, exist_ok=True)
        png = work_dir / f"{self.texture_prefix}o{index:02d}.png"
        grey.save(png)
        name = self.make_sprite(png, "I4", mipmaps=True)
        self.textures.append(Texture(name, DETAIL_SIZE, DETAIL_SIZE, (1.0, 1.0, 1.0)))
        return index, (images[0].width, images[0].height), sum(levels) / len(levels) / 255.0

    def dropped_shader(self, tag):
        """a transparent level shader the N64 cannot draw: Halo's additive
        ones (light strips' glow, light cones and volumes, most of them
        black -- invisible added, a black slab blended), light volumes
        whatever their blending (is_light_volume) and glass (its first map
        the dark reflection cube): only alpha-blended generic and chicago
        shaders and water are drawn"""
        group = tag.group_name
        if is_light_volume(tag.name):
            return True
        if group in ("sotr", "schi", "scex"):
            blend = struct.unpack("<H", self.map.read(tag.address + 44, 2))[0]
            return blend != 0
        return group != "swat"

    def campaign_water(self, tag, colour):
        """a water colour as the level has it: a grey one blue in a
        campaign level (chicago_water; CAMPAIGN_WATER_COLOUR)"""
        if not self.chicago_water or max(colour) - min(colour) >= WATER_GREY_SPREAD:
            return colour
        return CAMPAIGN_WATERFALL_COLOUR if "waterfall" in tag.name.lower() else CAMPAIGN_WATER_COLOUR

    def chicago_water_tint(self, tag):
        """a chicago water shader's colour: its maps' after the first (the
        mask) averaged, x WATER_SHADE"""
        f = self.fields("ShaderTransparentChicago")
        mf = self.fields("ShaderTransparentChicagoMap")
        size = self.defs.size("ShaderTransparentChicagoMap")
        count, pointer, _ = struct.unpack("<iII", self.map.read(tag.address + f["maps"], 12))
        means = []
        for i in range(1, max(count, 0)):
            datum = struct.unpack("<I", self.map.read(pointer + i * size + mf["map"] + 12, 4))[0]
            bitmap = self.map.tag(datum) if datum != 0xFFFFFFFF else None
            images = bitmap_images(self.map, bitmap, self.defs) if bitmap is not None else []
            try:
                img = decode(self.map, images[0], 1 << 16).convert("RGB") if images else None
            except ValueError:
                img = None
            if img is not None:
                n = img.width * img.height
                means.append([sum(channel.getdata()) / n / 255.0 for channel in img.split()])
        if not means:
            return WATER_DEFAULT_COLOUR
        colour = tuple(min(1.0, sum(m[k] for m in means) / len(means) * WATER_SHADE) for k in range(3))
        return colour if max(colour) > 0.02 else WATER_DEFAULT_COLOUR

    def water_tint(self, tag):
        """a water shader's colour (MATERIAL_WATER)"""
        f = self.fields("ShaderTransparentWater")
        read = lambda field, fmt: struct.unpack(fmt, self.map.read(tag.address + f[field], struct.calcsize(fmt)))
        tints = [[c * read(view + " brightness", "<f")[0] for c in read(view + " tint color", "<3f")]
                 for view in ("view perpendicular", "view parallel")]
        colour = tuple(min(1.0, (a + b) / 2 * WATER_SHADE) for a, b in zip(*tints))
        return colour if max(colour) > 0.02 else WATER_DEFAULT_COLOUR

    def add_material(self, texture, flags, shade=1.0, terrain=None, uv_scale=(1.0, 1.0)):
        self.materials.append((texture, flags))
        self.material_shade.append(shade)
        self.material_terrain.append(terrain)
        self.material_uv_scale.append(uv_scale)
        return len(self.materials) - 1

    def base_needs_colour(self, bitmap_datum):
        if bitmap_datum not in self.colour_by_bitmap:
            tag = self.map.tag(bitmap_datum)
            images = bitmap_images(self.map, tag, self.defs) if tag is not None else []
            try:
                img = decode(self.map, images[0], 64) if images else None
            except ValueError:
                img = None
            self.colour_by_bitmap[bitmap_datum] = img is None or needs_colour(img)
        return self.colour_by_bitmap[bitmap_datum]

    def terrain_materials(self, tag, base, span):
        """a natural terrain shader's materials (Terrain): their first,
        or None (no detail maps)"""
        f = self.fields("ShaderEnvironment")
        read = lambda field, fmt: struct.unpack(fmt, self.map.read(tag.address + f[field], struct.calcsize(fmt)))[0]
        base_tag = self.map.tag(base)
        images = bitmap_images(self.map, base_tag, self.defs) if base_tag is not None else []
        try:
            img = decode(self.map, images[0], TERRAIN_SAMPLES) if images else None
        except ValueError:
            img = None
        if img is None:
            return None
        sampler = BaseSampler(img)
        # Halo's "rescale detail maps": a detail map's scale is also in
        # base map texels to its own
        rescale = read("diffuse flags", "<H") & 1
        details = []
        for field in ("primary detail map", "secondary detail map"):
            datum = struct.unpack("<I", self.map.read(tag.address + f[field] + 12, 4))[0]
            scale = read(field + " scale", "<f")
            if datum == 0xFFFFFFFF or scale <= 0:
                details.append(None)
                continue
            texture, size = self.detail_texture(datum)
            if texture < 0:
                details.append(None)
                continue
            su = sv = scale
            if rescale and size:
                su *= images[0].width / size[0]
                sv *= images[0].height / size[1]
            details.append((texture, (su, sv)))
        blended = read("shader environment type", "<H") in (1, 2)
        if not blended:
            details[1] = None
        available = [d for d in details if d is not None]
        if not available:
            return None
        terrain = Terrain(sampler, [])
        flags = MATERIAL_DETAIL
        terrain.tint = self.detail_tint(tag, blended)
        if self.colour_tiles:
            flags |= MATERIAL_TILED
            terrain.image = decode(self.map, images[0], 1 << 16).convert("RGBA")
        for d in (details[0] or available[0], details[1] or available[0]):
            terrain.variants.append(self.add_material(d[0], flags, terrain=terrain, uv_scale=d[1]))
        return terrain.variants[0]

    def detail_tint(self, tag, blended):
        """what a terrain shader's detail maps do to its base map's colour on
        average, Halo's functions over each map's mean colour: (primary,
        secondary, micro), each (multiplier, addend) per channel -- the
        primary where the base map's alpha is high, the secondary where low
        (the primary's where the shader is not blended). Blood Gulch's grass
        detail is green, (0.22, 0.39, 0.16): twice it greens and darkens the
        grass; its sand and micro details brighten by 0.93 and 1.21. The
        tiles take this (terrain_tiles.py), the detail texture drawn over
        them only its grain (detail_texture: about its own mean)"""
        f = self.fields("ShaderEnvironment")
        read = lambda field, fmt: struct.unpack(fmt, self.map.read(tag.address + f[field], struct.calcsize(fmt)))[0]

        def effect(field, function):
            datum = struct.unpack("<I", self.map.read(tag.address + f[field] + 12, 4))[0]
            detail = self.map.tag(datum) if datum != 0xFFFFFFFF else None
            images = bitmap_images(self.map, detail, self.defs) if detail is not None else []
            try:
                img = decode(self.map, images[0], 1 << 16).convert("RGB") if images else None
            except ValueError:
                img = None
            if img is None:
                return None
            n = img.width * img.height
            mean = [sum(channel.getdata()) / n / 255.0 for channel in img.split()]
            # Halo's detail functions: double/biased multiply, multiply,
            # double/biased add
            if function == 0:
                return [2.0 * m for m in mean], [0.0, 0.0, 0.0]
            if function == 1:
                return mean, [0.0, 0.0, 0.0]
            return [1.0, 1.0, 1.0], [2.0 * m - 1.0 for m in mean]
        identity = ([1.0, 1.0, 1.0], [0.0, 0.0, 0.0])
        function = read("detail map function", "<H")
        primary = effect("primary detail map", function)
        secondary = effect("secondary detail map", function) if blended else None
        micro = effect("micro detail map", read("micro detail map function", "<H"))
        primary = primary or secondary or identity
        return primary, secondary or primary, micro or identity

    def detail_texture(self, bitmap_datum):
        """a detail map as an I4 texture with mipmaps, DETAIL_SIZE a side,
        about grey: the renderer draws light x (0.5 + texel), so a texel
        of 0.5 leaves the base map's colour as it is; Halo's detail maps
        multiply by twice the texel (biased), here over the map's mean,
        the base map's colour unchanged on average. Returns the texture and
        the original image's size"""
        if bitmap_datum in self.detail_by_bitmap:
            return self.detail_by_bitmap[bitmap_datum]
        result = (-1, None)
        tag = self.map.tag(bitmap_datum)
        images = bitmap_images(self.map, tag, self.defs) if tag is not None else []
        try:
            img = decode(self.map, images[0], DETAIL_SIZE) if images else None
        except ValueError:
            img = None
        if img is not None:
            grey = img.convert("L").resize((DETAIL_SIZE, DETAIL_SIZE), Image.LANCZOS)
            mean = sum(grey.getdata()) / (DETAIL_SIZE * DETAIL_SIZE) or 1.0
            grey = grey.point(lambda v: int(round(round(max(0.0, min(1.0, v / mean - 0.5)) * 15) * 17)))
            index = len(self.textures)
            work_dir = self.out_dir / "tex" / f".{self.map_name}"
            work_dir.mkdir(parents=True, exist_ok=True)
            png = work_dir / f"{self.texture_prefix}d{index:02d}.png"
            grey.save(png)
            name = self.make_sprite(png, "I4", mipmaps=True)
            self.textures.append(Texture(name, DETAIL_SIZE, DETAIL_SIZE, (0.5, 0.5, 0.5)))
            result = (index, (images[0].width, images[0].height))
        self.detail_by_bitmap[bitmap_datum] = result
        return result

    def triangle_material(self, material, triangle):
        """a terrain triangle's material: the detail map its base map's
        alpha picks (Halo's blend: primary where high)"""
        terrain = self.material_terrain[material]
        if terrain is None or terrain.variants[0] == terrain.variants[1]:
            return material
        alpha = sum(terrain.sampler.sample(v[2])[3] for v in triangle) / 3
        return terrain.variants[0] if alpha >= 0.5 else terrain.variants[1]

    def shader_spans(self, bsp_tag):
        """each environment shader's base map repeat in world units (area
        over texture coordinate area, the BSP's surfaces of it)"""
        x = self.map
        bo = self.fields("ScenarioStructureBSP")
        lo = self.fields("ScenarioStructureBSPLightmap")
        mo = self.fields("ScenarioStructureBSPMaterial")
        base = bsp_tag.address
        surfaces_count, surfaces_ptr = struct.unpack("<iI", x.read(base + bo["surfaces"], 8))
        surfaces = [struct.unpack("<hhh", x.read(surfaces_ptr + i * 6, 6)) for i in range(surfaces_count)]
        lightmap_count, lightmap_ptr = struct.unpack("<iI", x.read(base + bo["lightmaps"], 8))
        lsize, msize = self.defs.size("ScenarioStructureBSPLightmap"), self.defs.size("ScenarioStructureBSPMaterial")
        areas = {}
        for li in range(lightmap_count):
            mc, mp = struct.unpack("<iI", x.read(lightmap_ptr + li * lsize + lo["materials"], 8))
            for mi in range(mc):
                raw = x.read(mp + mi * msize, msize)
                shader_datum = struct.unpack_from("<I", raw, mo["shader"] + 12)[0]
                first, count = struct.unpack_from("<ii", raw, mo["surfaces"])
                rv_count = struct.unpack_from("<I", raw, mo["rendered vertices count"])[0]
                cv_ptr = struct.unpack_from("<iIIII", raw, mo["compressed vertices"])[3]
                if count <= 0 or rv_count == 0 or not cv_ptr:
                    continue
                data = x.read(cv_ptr, rv_count * 32)
                verts = [struct.unpack_from("<3f12x2f", data, v * 32) for v in range(rv_count)]
                world = uv = 0.0
                for si in range(first, first + count):
                    a, b, c = (verts[i] for i in surfaces[si])
                    e1 = [b[k] - a[k] for k in range(3)]
                    e2 = [c[k] - a[k] for k in range(3)]
                    cross = (e1[1] * e2[2] - e1[2] * e2[1], e1[2] * e2[0] - e1[0] * e2[2], e1[0] * e2[1] - e1[1] * e2[0])
                    world += math.sqrt(sum(k * k for k in cross)) / 2
                    uv += abs((b[3] - a[3]) * (c[4] - a[4]) - (c[3] - a[3]) * (b[4] - a[4])) / 2
                total = areas.setdefault(shader_datum, [0.0, 0.0])
                total[0] += world
                total[1] += uv
        return {datum: math.sqrt(w / u) for datum, (w, u) in areas.items() if w > 0 and u > 0}

    def first_map(self, tag):
        """the first bitmap any dependency of the shader names"""
        found = []

        def on_dependency(t, address, f, path, group, datum):
            if group == 0x6269746D and datum != 0xFFFFFFFF and not found:  # 'bitm'
                found.append(datum)

        from .walk import Walker
        Walker(self.map, self.defs).walk(tag, on_dependency=on_dependency)
        return self.texture(found[0]) if found else -1

    # ---------- geometry

    def build(self, bsp_tag):
        x = self.map
        bo = self.fields("ScenarioStructureBSP")
        lo = self.fields("ScenarioStructureBSPLightmap")
        mo = self.fields("ScenarioStructureBSPMaterial")
        co = self.fields("ScenarioStructureBSPCluster")
        base = bsp_tag.address
        surfaces_count, surfaces_ptr = struct.unpack("<iI", x.read(base + bo["surfaces"], 8))
        surfaces = [struct.unpack("<hhh", x.read(surfaces_ptr + i * 6, 6)) for i in range(surfaces_count)]

        if self.terrain_detail and not self.shader_span:
            self.shader_span = self.shader_spans(bsp_tag)
        # every surface (triangle) with its world vertices, uvs, colours and material
        surface_info = [None] * surfaces_count
        # split decisions by edge (its two ends' positions), shared by the
        # triangles on both sides so that no crack opens between them
        self.edge_splits = {}
        self.midpoints = {}
        lightmap_count, lightmap_ptr = struct.unpack("<iI", x.read(base + bo["lightmaps"], 8))
        lsize, msize = self.defs.size("ScenarioStructureBSPLightmap"), self.defs.size("ScenarioStructureBSPMaterial")
        lightmap_pages = self.lightmap_pages(bsp_tag, bo)
        for li in range(lightmap_count):
            la = lightmap_ptr + li * lsize
            page_index = struct.unpack("<h", x.read(la + lo["bitmap"], 2))[0]
            page = lightmap_pages[page_index] if 0 <= page_index < len(lightmap_pages) else None
            mc, mp = struct.unpack("<iI", x.read(la + lo["materials"], 8))
            for mi in range(mc):
                ma = mp + mi * msize
                raw = x.read(ma, msize)
                shader_datum = struct.unpack_from("<I", raw, mo["shader"] + 12)[0]
                first, count = struct.unpack_from("<ii", raw, mo["surfaces"])
                rv_count = struct.unpack_from("<I", raw, mo["rendered vertices count"])[0]
                ambient = struct.unpack_from("<3f", raw, mo["ambient color"])
                light_count = struct.unpack_from("<h", raw, mo["distant light count"])[0]
                lights = [(struct.unpack_from("<3f", raw, mo["distant light 0 color"]),
                           struct.unpack_from("<3f", raw, mo["distant light 0 direction"])),
                          (struct.unpack_from("<3f", raw, mo["distant light 1 color"]),
                           struct.unpack_from("<3f", raw, mo["distant light 1 direction"]))][:max(0, light_count)]
                cv_size, _, _, cv_ptr, _ = struct.unpack_from("<iIIII", raw, mo["compressed vertices"])
                if count <= 0 or rv_count == 0 or not cv_ptr:
                    continue
                material = self.material(shader_datum)
                if material in self.dropped:
                    continue
                verts = []
                same_vertex = {}
                data = x.read(cv_ptr, rv_count * 32)
                # the lightmap's texture coordinates follow the rendered
                # vertices: a compressed normal and u, v over 32767
                lv_count = struct.unpack_from("<I", raw, mo["lightmap vertices count"])[0]
                lightmap_uvs = []
                if page is not None and lv_count >= rv_count:
                    lv_data = x.read(cv_ptr + rv_count * 32, rv_count * 8)
                    lightmap_uvs = [struct.unpack_from("<Ihh", lv_data, v * 8)[1:] for v in range(rv_count)]
                for v in range(rv_count):
                    px, py, pz, normal, binormal, tangent, u, vv = struct.unpack_from("<3fIII2f", data, v * 32)
                    n = unpack_normal(normal)
                    color = None
                    if lightmap_uvs:
                        lu, lv = lightmap_uvs[v]
                        color = sample_lightmap(page, lu / 32767.0, lv / 32767.0)
                    if color is None:
                        # no lightmap: the material's lighting summary
                        color = list(ambient)
                        for (lc, ld) in lights:
                            d = -(n[0] * ld[0] + n[1] * ld[1] + n[2] * ld[2])
                            if d > 0:
                                color = [c + l * d for c, l in zip(color, lc)]
                    lightmap = (page, lightmap_uvs[v][0] / 32767.0, lightmap_uvs[v][1] / 32767.0) if lightmap_uvs else None
                    if material in self.water_colour:
                        # water: its flat colour, never split by light
                        color, lightmap = list(self.water_colour[material]), None
                    vertex =((px, py, pz), n, (u, vv), tuple(min(1.0, max(0.0, c)) for c in color), lightmap)
                    # the same vertex twice (position, texture coordinate,
                    # light): one, so that batches share it
                    key = (tuple(round(c, 4) for c in vertex[0]), tuple(round(c, 5) for c in vertex[2]),
                           tuple(round(c, 5) for c in lightmap[1:]) if lightmap else None)
                    verts.append(same_vertex.setdefault(key, vertex))
                self.split_terrain = self.material_terrain[material]
                if self.materials[material][1] & MATERIAL_TILED:
                    self.split_terrain = None
                for s in range(first, first + count):
                    a, b, c = surfaces[s]
                    # Halo's front faces wind clockwise (Direct3D), tiny3d's
                    # counter-clockwise
                    triangles = self.split_by_lightmap((verts[a], verts[c], verts[b]))
                    surface_info[s] = [(self.triangle_material(material, t),) + t for t in triangles]
                self.split_terrain = None

        # clusters: their surfaces (surface indices), batched per material
        cluster_count, cluster_ptr = struct.unpack("<iI", x.read(base + bo["clusters"], 8))
        csize = self.defs.size("ScenarioStructureBSPCluster")
        listed = []
        so = self.fields("ScenarioStructureBSPSubcluster")
        ssize = self.defs.size("ScenarioStructureBSPSubcluster")
        for ci in range(cluster_count):
            ca = cluster_ptr + ci * csize
            ic, ip = struct.unpack("<iI", x.read(ca + co["surface indices"], 8))
            indices = list(struct.unpack(f"<{ic}i", x.read(ip, 4 * ic))) if ic > 0 else []
            # the Xbox's maps list a cluster's surfaces by subcluster
            sc, sp = struct.unpack("<iI", x.read(ca + co["subclusters"], 8))
            for si in range(max(sc, 0)):
                sic, sip = struct.unpack("<iI", x.read(sp + si * ssize + so["surface indices"], 8))
                if sic > 0:
                    indices += struct.unpack(f"<{sic}i", x.read(sip, 4 * sic))
            seen = set()
            members = []
            for i in indices:
                if 0 <= i < surfaces_count and surface_info[i] and i not in seen:
                    seen.add(i)
                    members.append(i)
            listed.append(members)
        return self.surface_homes(listed, self.cluster_pvs(bsp_tag, bo, cluster_count), surfaces_count,
                                  surface_info), surface_info

    def cluster_pvs(self, bsp_tag, bo, cluster_count):
        """the BSP's potentially visible sets: visible(eye, cluster), or None"""
        size, _, _, pointer, _ = struct.unpack("<iIIII", self.map.read(bsp_tag.address + bo["cluster data"], 20))
        words = (cluster_count + 31) // 32
        if cluster_count <= 0 or not pointer or size < cluster_count * words * 4:
            return None
        data = self.map.read(pointer, cluster_count * words * 4)

        def visible(eye, cluster):
            word = struct.unpack_from("<I", data, (eye * words + (cluster >> 5)) * 4)[0]
            return (word >> (cluster & 31)) & 1
        return visible

    def surface_homes(self, listed, visible, surfaces_count, surface_info):
        """each BSP cluster's surfaces for the level file: [(surfaces, the BSP
        clusters whose visibility draws them)]. A surface several clusters
        list (where it crosses between them) goes to one of them that every
        eye seeing any of them sees too; if none is, to a group of its own
        drawn when any of them is visible -- given to the first that listed it,
        it was not drawn from where the potentially visible set has another
        of them and not that one: holes in Sidewinder's ground and cliffs
        (188 surfaces; Blood Gulch 75). Surfaces no cluster lists (rare): a
        last group drawn always"""
        owners = {}
        for ci, members in enumerate(listed):
            for i in members:
                owners.setdefault(i, []).append(ci)
        count = len(listed)
        out = [([], (ci,)) for ci in range(count)]
        groups = {}

        def covers(home, owned):
            return all(visible(eye, home) or not any(visible(eye, o) for o in owned) for eye in range(count))
        placed = set()
        for members in listed:
            for i in members:
                if i in placed:
                    continue
                placed.add(i)
                owned = owners[i]
                if len(owned) == 1 or visible is None:
                    out[owned[0]][0].append(i)
                    continue
                home = next((o for o in owned if covers(o, owned)), None)
                if home is not None:
                    out[home][0].append(i)
                else:
                    groups.setdefault(tuple(owned), []).append(i)
        for owned, members in groups.items():
            out.append((members, owned))
        leftover = [i for i in range(surfaces_count) if surface_info[i] and i not in placed]
        if leftover:
            out.append((leftover, ()))
        return out

    def fit_detail_scales(self, surface_info):
        """halves a detail map's scale until every triangle of it fits a
        batch's coordinates (GREYSCALE_UV_EXTENT, less one repeat): Blood
        Gulch's sand repeats every 1.4 units, its triangles span 14"""
        limit = GREYSCALE_UV_EXTENT - DETAIL_SIZE
        widest = {}
        for triangles in surface_info:
            for t in triangles or ():
                material = t[0]
                if material in self.overlay_of:
                    material = self.overlay_of[material]
                if self.material_uv_scale[material] == (1.0, 1.0):
                    continue
                for k in range(2):
                    values = [v[2][k] for v in t[1:]]
                    widest[(material, k)] = max(widest.get((material, k), 0.0), max(values) - min(values))
        for material, scale in enumerate(self.material_uv_scale):
            if scale == (1.0, 1.0):
                continue
            su, sv = scale
            while max(widest.get((material, 0), 0.0) * su, widest.get((material, 1), 0.0) * sv) * DETAIL_SIZE > limit:
                su, sv = su / 2, sv / 2
            self.material_uv_scale[material] = (su, sv)

    def split_by_lightmap(self, triangle, depth=0):
        """a level triangle (three vertices) split where the lightmap changes
        across it, as a list of triangles"""
        if depth >= self.split_depth:
            return [triangle]
        split = []
        for i in range(3):
            p, q = triangle[i], triangle[(i + 1) % 3]
            key = tuple(sorted((tuple(round(c, 4) for c in p[0]), tuple(round(c, 4) for c in q[0]))))
            decision = self.edge_splits.get(key)
            if decision is None:
                decision = self.edge_wants_split(p, q)
                self.edge_splits[key] = decision
            split.append(decision)
        if not any(split):
            return [triangle]
        a, b, c = triangle
        mab = self.midpoint(a, b) if split[0] else None
        mbc = self.midpoint(b, c) if split[1] else None
        mca = self.midpoint(c, a) if split[2] else None
        count = sum(split)
        if count == 3:
            children = [(a, mab, mca), (mab, b, mbc), (mca, mbc, c), (mab, mbc, mca)]
        elif count == 1:
            if mab:
                children = [(a, mab, c), (mab, b, c)]
            elif mbc:
                children = [(a, b, mbc), (a, mbc, c)]
            else:
                children = [(a, b, mca), (mca, b, c)]
        else:
            # two edges: the corner between them, and the rest in two
            if not mab:
                children = [(c, mca, mbc), (a, b, mbc), (a, mbc, mca)]
            elif not mbc:
                children = [(a, mab, mca), (b, c, mca), (b, mca, mab)]
            else:
                children = [(b, mbc, mab), (c, a, mab), (c, mab, mbc)]
        out = []
        for child in children:
            out.extend(self.split_by_lightmap(child, depth + 1))
        return out

    def edge_wants_split(self, p, q):
        return self.lightmap_wants_split(p, q) or self.terrain_wants_split(p, q)

    def terrain_wants_split(self, p, q):
        """a terrain edge across which the base map's colour changes"""
        terrain = self.split_terrain
        if terrain is None:
            return False
        if math.hypot(p[2][0] - q[2][0], p[2][1] - q[2][1]) * TERRAIN_SAMPLES <= 1.5:
            return False
        a, b = terrain.sampler.sample(p[2]), terrain.sampler.sample(q[2])
        middle = terrain.sampler.sample(((p[2][0] + q[2][0]) / 2, (p[2][1] + q[2][1]) / 2))
        return max(abs(m - (x + y) / 2) for m, x, y in zip(middle[:3], a[:3], b[:3])) > TERRAIN_SPLIT_DIFFERENCE

    def lightmap_wants_split(self, p, q):
        if p[4] is None or q[4] is None or p[4][0] is not q[4][0]:
            return False
        page, pu, pv = p[4]
        _, qu, qv = q[4]
        width, height, _ = page
        if math.hypot((pu - qu) * width, (pv - qv) * height) <= LIGHTMAP_SPLIT_TEXELS:
            return False
        middle = sample_lightmap(page, (pu + qu) / 2, (pv + qv) / 2)
        if middle is None:
            return False
        return max(abs(m - (a + b) / 2) for m, a, b in zip(middle, p[3], q[3])) > LIGHTMAP_SPLIT_DIFFERENCE

    def midpoint(self, p, q):
        """the vertex halfway along p-q (one per edge and side), lit from the
        lightmap there"""
        key = (id(p), id(q)) if id(p) < id(q) else (id(q), id(p))
        vertex = self.midpoints.get(key)
        if vertex is None:
            position = tuple((a + b) / 2 for a, b in zip(p[0], q[0]))
            n = [a + b for a, b in zip(p[1], q[1])]
            length = math.sqrt(sum(c * c for c in n)) or 1.0
            uv = ((p[2][0] + q[2][0]) / 2, (p[2][1] + q[2][1]) / 2)
            lightmap = None
            color = tuple((a + b) / 2 for a, b in zip(p[3], q[3]))
            if p[4] is not None and q[4] is not None and p[4][0] is q[4][0]:
                lightmap = (p[4][0], (p[4][1] + q[4][1]) / 2, (p[4][2] + q[4][2]) / 2)
                sampled = sample_lightmap(lightmap[0], lightmap[1], lightmap[2])
                if sampled is not None:
                    color = tuple(min(1.0, max(0.0, c)) for c in sampled)
            vertex = (position, tuple(c / length for c in n), uv, color, lightmap)
            self.midpoints[key] = vertex
        return vertex

    def lightmap_pages(self, bsp_tag, bo):
        """the BSP's lightmap bitmap: each page as (width, height, pixels)"""
        datum = struct.unpack("<I", self.map.read(bsp_tag.address + bo["lightmaps bitmap"] + 12, 4))[0]
        tag = self.map.tag(datum)
        pages = []
        if tag is None:
            return pages
        for image in bitmap_images(self.map, tag, self.defs):
            try:
                img = decode(self.map, image, 4096).convert("RGB")
            except ValueError:
                pages.append(None)
                continue
            pages.append((img.width, img.height, list(img.getdata())))
        return pages

    def batches(self, triangles):
        """splits (material, v0, v1, v2) triangles of one material into
        batches of at most 70 vertices with a bounded uv extent. A batch
        grows from a triangle by the triangle that adds the fewest new
        vertices: the RSP transforms every vertex a batch loads, and a
        compact batch is left out whole when it is out of view"""
        out = []
        material = triangles[0][0] if triangles else 0
        texture, _ = self.materials[material]
        size = (self.textures[texture].width, self.textures[texture].height) if texture >= 0 else (32, 32)
        uv_limit = GREYSCALE_UV_EXTENT if self.greyscale else MAXIMUM_UV_EXTENT
        scale = self.material_uv_scale[material]
        # (a material with an overlay: its batches are the overlay's too,
        # in the overlay's coordinates, the larger)
        if material in self.overlay_of:
            overlay_scale = self.material_uv_scale[self.overlay_of[material]]
            scale = (max(scale[0], overlay_scale[0]), max(scale[1], overlay_scale[1]))
        by_vertex = {}
        for t, tri in enumerate(triangles):
            for v in tri[1:]:
                by_vertex.setdefault(id(v), []).append(t)
        remaining = set(range(len(triangles)))

        while remaining:
            current_vertices, current_map, current_tris = [], {}, []
            uv_min = [math.inf, math.inf]
            uv_max = [-math.inf, -math.inf]
            candidates = set()
            seed = min(remaining)
            while True:
                if current_tris:
                    pool = candidates & remaining
                else:
                    pool = {seed}
                best, best_new = None, None
                for t in sorted(pool):
                    new = sum(1 for v in triangles[t][1:] if id(v) not in current_map)
                    if best is None or new < best_new:
                        best, best_new = t, new
                        if new == 0:
                            break
                if best is None:
                    # nothing adjacent left: start over from any triangle
                    # that still fits
                    for t in sorted(remaining)[:64]:
                        new = sum(1 for v in triangles[t][1:] if id(v) not in current_map)
                        if len(current_vertices) + new <= MAXIMUM_BATCH_VERTICES:
                            best, best_new = t, new
                            break
                if best is None:
                    break
                tri = triangles[best]
                n_min = list(uv_min)
                n_max = list(uv_max)
                for v in tri[1:]:
                    for k in range(2):
                        n_min[k] = min(n_min[k], v[2][k] * scale[k])
                        n_max[k] = max(n_max[k], v[2][k] * scale[k])
                extent = max((n_max[0] - n_min[0]) * size[0], (n_max[1] - n_min[1]) * size[1])
                if len(current_vertices) + best_new > MAXIMUM_BATCH_VERTICES or (current_tris and extent > uv_limit):
                    # the best does not fit: try the others once, else close
                    candidates.discard(best)
                    if not candidates & remaining:
                        break
                    continue
                uv_min, uv_max = n_min, n_max
                idx = []
                for v in tri[1:]:
                    key = id(v)
                    if key not in current_map:
                        current_map[key] = len(current_vertices)
                        current_vertices.append(v)
                        candidates.update(by_vertex[key])
                    idx.append(current_map[key])
                current_tris.append(tuple(idx))
                remaining.discard(best)
            out.append((material, list(current_vertices), list(current_tris)))
        return out

    def decimate_triangles(self, triangles):
        """one material's triangles of a cluster, simplified within
        decimate_tolerance (its edges with other materials and clusters
        stay: decimate.py moves no vertex on an open edge)"""
        from .decimate import decimate
        index = {}
        vertices = []
        faces = []
        for t in triangles:
            face = []
            for v in t[1:]:
                key = id(v)
                if key not in index:
                    index[key] = len(vertices)
                    vertices.append((0, v[0], v[2], v))
                face.append(index[key])
            faces.append(tuple(face))
        kept = decimate(vertices, faces, 0, max_deviation=self.decimate_tolerance)
        self.decimated[0] += len(faces)
        self.decimated[1] += len(kept)
        material = triangles[0][0]
        return [(material,) + tuple(vertices[i][3] for i in face) for face in kept]

    def chunk_batches(self, batches, limit=None):
        """a streamed level cluster's batches in chunks of near ones (each
        read when it comes into view): halved across their longest extent
        until a chunk's vertices and indices are at most CHUNK_BYTES"""
        limit = limit or CHUNK_BYTES
        items = []
        for b in batches:
            verts, tris = b[1], b[2]
            size = (len(verts) + 1) // 2 * 32 + len(strip_chunks(tris))
            centre = [sum(v[0][k] for v in verts) / len(verts) for k in range(3)] if verts else [0.0, 0.0, 0.0]
            items.append((b, size, centre))

        def split(group):
            total = sum(i[1] for i in group)
            if total <= limit or len(group) <= 1:
                return [[i[0] for i in group]]
            extents = [max(i[2][k] for i in group) - min(i[2][k] for i in group) for k in range(3)]
            axis = extents.index(max(extents))
            group = sorted(group, key=lambda i: i[2][axis])
            running, cut = 0, 1
            for n, i in enumerate(group):
                running += i[1]
                if running >= total / 2:
                    cut = max(1, min(len(group) - 1, n + 1))
                    break
            return split(group[:cut]) + split(group[cut:])
        return split(items) if items else []

    def write(self, path, bsp_tag):
        clusters, surface_info = self.build(bsp_tag)
        self.fit_detail_scales(surface_info)
        streamed = getattr(self, "streamed", False)
        # natural terrain's tiles: every terrain triangle, for the light
        # under each tile texel (the batches' own and their neighbours')
        tiles = None
        if self.colour_tiles:
            from .terrain_tiles import TerrainTiles
            tiles = TerrainTiles()
            for triangles in surface_info:
                for t in triangles or ():
                    if self.materials[t[0]][1] & MATERIAL_TILED:
                        tiles.light_of(self.material_terrain[t[0]]).add(t[1:])
        out_batches = []
        out_clusters = []
        out_owners = []
        # a streamed level's chunks: (first batch, batch count), each
        # cluster's in a row
        chunks = []
        cluster_chunks = []
        for indices, owned in clusters:
            first = len(out_batches)
            by_material = {}
            for i in indices:
                for info in surface_info[i]:
                    by_material.setdefault(info[0], []).append(info)
            lo = [math.inf] * 3
            hi = [-math.inf] * 3
            for material in sorted(by_material):
                triangles = by_material[material]
                if self.decimate_tolerance is not None and len(triangles) > 2:
                    triangles = self.decimate_triangles(triangles)
                for b in self.batches(triangles):
                    out_batches.append(b)
                    # a natural material's detail drawn over it: the same
                    # batch (the same culling: drawn only where it is)
                    if material in self.overlay_of:
                        out_batches.append((self.overlay_of[material],) + b[1:])
                    for v in b[1]:
                        for k in range(3):
                            lo[k] = min(lo[k], v[0][k])
                            hi[k] = max(hi[k], v[0][k])
            if lo[0] == math.inf:
                lo = hi = [0.0, 0.0, 0.0]
            if streamed:
                # (the cluster's batches in chunks of near ones: chunk_batches)
                ordered = self.chunk_batches(out_batches[first:])
                out_batches[first:] = [b for chunk in ordered for b in chunk]
                cluster_chunks.append((len(chunks), len(ordered)))
                start = first
                for chunk in ordered:
                    chunks.append((start, len(chunk)))
                    start += len(chunk)
            out_clusters.append((lo, hi, first, len(out_batches) - first, len(out_owners), len(owned)))
            out_owners.extend(owned)

        vertex_data = bytearray()
        index_data = bytearray()
        batch_records = []
        for material, verts, tris in out_batches:
            texture, flags = self.materials[material]
            tw, th = (self.textures[texture].width, self.textures[texture].height) if texture >= 0 else (32, 32)
            shade = self.material_shade[material]
            # the vertices' light at most DOUBLE_SHADE_LIMIT where the
            # renderer doubles texture x light
            top = DOUBLE_SHADE_LIMIT if flags & MATERIAL_DOUBLE else 255
            terrain = self.material_terrain[material]
            su, sv = self.material_uv_scale[material]
            # a tiled terrain batch's coordinates in its detail map's texels
            # at the level the renderer draws (terrain_tiles.py)
            tiled = tiles is not None and flags & MATERIAL_TILED
            if tiled:
                from .terrain_tiles import TILE_DETAIL_LEVEL
                tw, th = tw >> TILE_DETAIL_LEVEL, th >> TILE_DETAIL_LEVEL
            # the batch's coordinates about 0 (whole repeats off): s10.5 holds
            # +-1024 texels, so a triangle may span up to ~2000 (from its
            # lowest whole coordinate, 1024 at most: Sidewinder's long trims
            # were clamped there, their texture smeared)
            u0 = round((min(v[2][0] * su for v in verts) + max(v[2][0] * su for v in verts)) / 2)
            v0 = round((min(v[2][1] * sv for v in verts) + max(v[2][1] * sv for v in verts)) / 2)
            if tiled:
                tiles.make(len(batch_records), terrain, verts, su, sv, u0, v0, tw, shade)
            if len(verts) % 2:
                verts = verts + [verts[-1]]
            pair_offset = len(vertex_data) // 32
            for a, b in zip(verts[0::2], verts[1::2]):
                def pos(v):
                    return [max(-32768, min(32767, int(round(c * POSITION_SCALE)))) for c in v[0]]

                def st(v):
                    return [max(-32768, min(32767, int(round((v[2][0] * su - u0) * tw * 32)))),
                            max(-32768, min(32767, int(round((v[2][1] * sv - v0) * th * 32))))]

                def rgba(v):
                    light = v[3]
                    if terrain is not None:
                        # the base map's colour with its detail maps' average
                        # (detail_tint: not doubled as the other textures
                        # are -- Halo's detail maps double), times the light
                        colour = terrain.tinted(terrain.sampler.sample(v[2]))
                        light = [min(1.0, max(0.0, l * c)) for l, c in zip(light, colour)]
                    r, g, b_, = (min(top, int(round(c * shade * 255))) for c in light)
                    return (r << 24) | (g << 16) | (b_ << 8) | 255
                vertex_data += struct.pack(">3hH3hHII2h2h", *pos(a), pack_normal(a[1]), *pos(b), pack_normal(b[1]),
                                           rgba(a), rgba(b), *st(a), *st(b))
            index_offset = len(index_data)
            index_data += strip_chunks(tris)
            lo = [max(-32768, min(32767, int(math.floor(min(v[0][k] for v in verts) * POSITION_SCALE)))) for k in range(3)]
            hi = [max(-32768, min(32767, int(math.ceil(max(v[0][k] for v in verts) * POSITION_SCALE)))) for k in range(3)]
            batch_records.append((material, len(verts), len(tris), pair_offset, index_offset, lo, hi))

        # a streamed level (version 5: a campaign level, too big to keep in
        # memory -- n64_render.c reads a chunk's vertices and indices when it
        # comes into view): after the batches, the chunk count, each
        # cluster's (first chunk, chunk count), then each chunk's bounds
        # (world units), (first batch, batch count) and (vertex bytes from,
        # vertex bytes, index bytes from, index bytes). A chunk's batches are
        # one run of each (written in order)
        chunk_records = []
        index_ends = []
        for b, record in enumerate(batch_records):
            following = batch_records[b + 1][4] if b + 1 < len(batch_records) else len(index_data)
            index_ends.append(following)
        for first, count in chunks:
            last = first + count - 1
            v_from = batch_records[first][3] * 32
            v_to = (batch_records[last][3] + (batch_records[last][1] + 1) // 2) * 32
            i_from = batch_records[first][4]
            i_to = index_ends[last]
            positions = [v[0] for b in out_batches[first:first + count] for v in b[1]]
            lo = [min(p[k] for p in positions) for k in range(3)]
            hi = [max(p[k] for p in positions) for k in range(3)]
            chunk_records.append((lo, hi, first, count, v_from, v_to - v_from, i_from, i_to - i_from))
        streamed = getattr(self, "streamed", False)
        header_size = 64
        textures_size = len(self.textures) * 28
        materials_size = len(self.materials) * 4
        clusters_size = len(out_clusters) * 32
        owners_size = (len(out_owners) * 2 + 3) & ~3
        batches_size = len(batch_records) * 24
        offset = header_size
        textures_offset = offset; offset += textures_size
        materials_offset = offset; offset += materials_size
        clusters_offset = offset; offset += clusters_size
        owners_offset = offset; offset += owners_size
        batches_offset = offset; offset += batches_size
        cluster_data_offset = 0
        if streamed:
            cluster_data_offset = offset
            offset += 4 + len(cluster_chunks) * 4 + len(chunk_records) * 44
        offset = (offset + 7) & ~7
        vertices_offset = offset; offset += len(vertex_data)
        indices_offset = offset; offset += len(index_data)

        out = bytearray()
        out += b"H64L" + struct.pack(">I f HHHH II II II II IHxx", 5 if streamed else VERSION, 1.0 / POSITION_SCALE,
                                     len(self.textures), len(self.materials), len(out_clusters), len(batch_records),
                                     textures_offset, materials_offset, clusters_offset, batches_offset,
                                     vertices_offset, len(vertex_data), indices_offset, len(index_data),
                                     owners_offset, len(out_owners))
        out += struct.pack(">I", cluster_data_offset)
        out += b"\0" * (header_size - len(out))
        for t in self.textures:
            name = t.name.encode()
            out += name + b"\0" * (24 - len(name)) + struct.pack(">HH", t.width, t.height)
        for texture, flags in self.materials:
            out += struct.pack(">hH", texture, flags)
        for lo, hi, first, count, owner_first, owner_count in out_clusters:
            out += struct.pack(">3f3fHHHH", *lo, *hi, first, count, owner_first, owner_count)
        out += struct.pack(f">{len(out_owners)}H", *out_owners)
        out += b"\0" * (batches_offset - len(out))
        for material, nv, nt, pair_offset, index_offset, lo, hi in batch_records:
            out += struct.pack(">HBBII3h3h", material, nv, nt, pair_offset, index_offset, *lo, *hi)
        if streamed:
            out += struct.pack(">I", len(chunk_records))
            for record in cluster_chunks:
                out += struct.pack(">HH", *record)
            for lo, hi, first, count, v_from, v_size, i_from, i_size in chunk_records:
                out += struct.pack(">3f3fHH4I", *lo, *hi, first, count, v_from, v_size, i_from, i_size)
        out += b"\0" * (vertices_offset - len(out))
        out += vertex_data
        out += index_data
        Path(path).parent.mkdir(parents=True, exist_ok=True)
        Path(path).write_bytes(out)
        stats = {
            "textures": len(self.textures), "materials": len(self.materials), "clusters": len(out_clusters),
            "batches": len(batch_records), "vertices": len(vertex_data) // 16,
            "triangles": len(index_data) // 3, "bytes": len(out),
        }
        if tiles is not None:
            for k, v in tiles.write(Path(path).with_suffix(".tiles")).items():
                stats["terrain " + k] = v
        return stats
