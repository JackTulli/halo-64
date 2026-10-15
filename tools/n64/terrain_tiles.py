"""Natural terrain's colour map in tiles: a megatexture for the N64.

Halo lays one big colour map over its natural ground (Blood Gulch's,
512x512, spans the map) and multiplies it by detail maps that repeat every
metre or two. The N64's RDP takes two textures a pixel at most, and the
detail map's mipmaps take both; so the colour map went on the vertices,
sampled at 64x64 seven units apart, and the ground's grass, dirt and paths
blurred into beige (level.py TERRAIN_SAMPLES).

Here each terrain batch of a level gets a tile of its own: its piece of
the colour map at up to the colour map's own density, RGBA16, at most
TILE_MAX_TEXELS texels (2 KB of TMEM beside the detail map's 512 bytes),
with its detail maps' average colour (level.py detail_tint: Halo
multiplies by twice a detail texel, and Blood Gulch's grass detail is
green) and the lightmap baked in, since the second texture leaves no room
for the vertices' light. (The first tiles doubled colour x light, as the
level's other textures are: in Blood Gulch's sun that clipped the sand to
pale yellow, and the grass lost its detail's green.) The
renderer (port/n64/src/n64_render.c, MATERIAL_TILED) draws tile x (0.5 +
detail) in two cycles, the detail map's TILE_DETAIL_LEVEL alone, faded out
by the RDP's LOD fraction where its texels get smaller than the pixels
(the batch's texture coordinates are in that level's texels).

A tile is addressed with the batch's own detail coordinates, shifted right
(the RDP's tile shift: one tile texel is 2^k detail texels) and wrapped
(the tile's mask): the tile texel at coordinate x is stored at x mod the
tile's width, so a batch whose coordinates span fewer texels than its tile
(with one more for the filter) needs no offset of its own. Tiles of more
than TILE_RESIDENT_TEXELS are read from the cartridge as the eye comes
near; the same tile a quarter a side (TILE_SMALL_SHIFT) stays in memory for
the distance. The file, levels/<map>.tiles (big-endian):

    header    'H64T', version, entry count, the detail map's level drawn, the
              entries' offset, the resident tiles' offset and size, the big tiles'
              offset, the largest big tile's bytes
    entries   (one a tiled batch, in batch order) u16 batch; the resident tile's
              shift s, shift t and size (log2 width << 4 | log2 height); the big
              tile's (size 0: none); u32 offset of the resident tile, of the big one
    tiles     the resident ones, then the big ones: RGBA 5551, row by row
"""

import math
import struct
from pathlib import Path

from PIL import Image

from .level import sample_lightmap

VERSION = 1
# the detail map's mipmap level drawn over a tile (64x64 >> 1: 32x32 I4,
# 512 bytes of TMEM): its texels are the batch's texture coordinates, so
# the RDP's LOD fraction -- 0 where a texel covers a pixel or more, 1 where
# two or more texels share one -- fades it out before it would shimmer
TILE_DETAIL_LEVEL = 1
# a tile's texels at most (RGBA16: 2 KB of TMEM), and a side's
TILE_MAX_TEXELS = 1024
TILE_MAX_SIDE = 64
# tiles of up to this many texels are kept in memory whole; bigger ones are
# read when near, and kept for the distance at most TILE_SMALL_SIDE a side
# (coarser by a power of two each way)
TILE_RESIDENT_TEXELS = 256
TILE_SMALL_SIDE = 8
# a tile's sides at least (a TMEM row: 8 bytes)
TILE_MINIMUM_SIDE = 4
# the coordinates' reach past a batch's vertices (the RDP's arithmetic)
TILE_MARGIN = 0.25
TILE_ALIGN = 16
# the colour map's light lookups: the terrain's triangles in cells of its
# texture coordinates
CELL = 1.0 / 32.0


def rgba5551(r, g, b):
    return (min(31, int(r * 31 + 0.5)) << 11) | (min(31, int(g * 31 + 0.5)) << 6) | \
        (min(31, int(b * 31 + 0.5)) << 1) | 1


class TerrainLight:
    """a terrain's triangles in its colour map's texture coordinates: the
    light at a point of the map (the lightmap there, as level.py lights a
    vertex), from the triangle over it"""

    def __init__(self):
        self.cells = {}
        self.triangles = []

    def add(self, triangle):
        a, b, c = triangle
        (ax, ay), (bx, by), (cx, cy) = a[2], b[2], c[2]
        det = (by - cy) * (ax - cx) + (cx - bx) * (ay - cy)
        if abs(det) < 1e-14:
            return
        index = len(self.triangles)
        self.triangles.append((triangle, det))
        x0, x1 = min(ax, bx, cx), max(ax, bx, cx)
        y0, y1 = min(ay, by, cy), max(ay, by, cy)
        for cy_ in range(math.floor(y0 / CELL), math.floor(y1 / CELL) + 1):
            for cx_ in range(math.floor(x0 / CELL), math.floor(x1 / CELL) + 1):
                self.cells.setdefault((cx_, cy_), []).append(index)

    def light(self, u, v, prefer):
        """the light (r, g, b, 0..1) at texture coordinate (u, v), or None
        where no triangle is; a triangle of prefer (vertex ids) first where
        the ground folds over itself"""
        found = None
        for index in self.cells.get((math.floor(u / CELL), math.floor(v / CELL)), ()):
            triangle, det = self.triangles[index]
            a, b, c = triangle
            (ax, ay), (bx, by), (cx, cy) = a[2], b[2], c[2]
            w0 = ((by - cy) * (u - cx) + (cx - bx) * (v - cy)) / det
            w1 = ((cy - ay) * (u - cx) + (ax - cx) * (v - cy)) / det
            w2 = 1.0 - w0 - w1
            if w0 < -1e-6 or w1 < -1e-6 or w2 < -1e-6:
                continue
            found = (triangle, (w0, w1, w2))
            if all(id(p) in prefer for p in triangle):
                break
        if found is None:
            return None
        triangle, weights = found
        maps = [p[4] for p in triangle]
        if all(m is not None for m in maps) and maps[0][0] is maps[1][0] is maps[2][0]:
            lu = sum(w * m[1] for w, m in zip(weights, maps))
            lv = sum(w * m[2] for w, m in zip(weights, maps))
            sampled = sample_lightmap(maps[0][0], lu, lv)
            if sampled is not None:
                return [min(1.0, max(0.0, s)) for s in sampled]
        return [sum(w * p[3][k] for w, p in zip(weights, triangle)) for k in range(3)]


def _span(values):
    """the tile texels a batch's coordinates (tile texels) reach, with the
    filter's next one: (first, how many)"""
    first = math.floor(min(values) - TILE_MARGIN)
    return first, math.floor(max(values) + TILE_MARGIN) + 1 - first + 1


def _side(need):
    side = TILE_MINIMUM_SIDE
    while side < need:
        side *= 2
    return side


def _fit(coords, k, limit):
    """the shift (from k) and tile side at which a batch's coordinates
    (detail texels, one axis) fit at most limit texels: (k, first, side)"""
    while True:
        first, need = _span([c / (1 << k) for c in coords])
        if _side(need) <= limit:
            return k, first, _side(need)
        k += 1


class TerrainTiles:
    """a level's terrain tiles (one per tiled batch) and its file"""

    def __init__(self):
        self.entries = []
        self.lights = {}
        self.tiled_images = {}
        self.stats = {"tiles": 0, "big": 0, "texels": 0}

    def light_of(self, terrain):
        if id(terrain) not in self.lights:
            self.lights[id(terrain)] = TerrainLight()
        return self.lights[id(terrain)]

    def make(self, batch, terrain, verts, su, sv, u0, v0, texels, shade):
        """a batch's tile: its coordinates are (u x su - u0) x texels detail
        texels (level.py write, quantised as there)"""
        coords = [(round((v[2][0] * su - u0) * texels * 32) / 32.0,
                   round((v[2][1] * sv - v0) * texels * 32) / 32.0) for v in verts]
        base_w, base_h = terrain.image.size
        # a tile texel no finer than the colour map's, the tile at most
        # TILE_MAX_TEXELS (the longer side coarser)
        ks = max(0, math.ceil(math.log2(texels * su / base_w) - 1e-6))
        kt = max(0, math.ceil(math.log2(texels * sv / base_h) - 1e-6))
        us = [c[0] for c in coords]
        vs = [c[1] for c in coords]
        ks, xa, w = _fit(us, ks, TILE_MAX_SIDE)
        kt, ya, h = _fit(vs, kt, TILE_MAX_SIDE)
        while w * h > TILE_MAX_TEXELS:
            if w >= h:
                ks, xa, w = _fit(us, ks + 1, TILE_MAX_SIDE)
            else:
                kt, ya, h = _fit(vs, kt + 1, TILE_MAX_SIDE)
        # a big tile's copy for the distance: coarser by a power of two each
        # way, at most TILE_SMALL_SIDE a side
        small = None
        if w * h > TILE_RESIDENT_TEXELS:
            sks, sxa, sw = _fit(us, ks + 1, TILE_SMALL_SIDE)
            skt, sya, sh = _fit(vs, kt + 1, TILE_SMALL_SIDE)
            small = (sks, skt, sxa, sya, sw, sh)
        assert max(ks, kt, small[0] if small else 0, small[1] if small else 0) <= 10, "tile shift past the RDP's"
        # the texels: a canvas over the big tile and the small one's
        # neighbourhood (each of its texels filters the big ones about it)
        cx0, cy0, cx1, cy1 = xa, ya, xa + w, ya + h
        if small is not None:
            sks, skt, sxa, sya, sw, sh = small
            fx, fy = 1 << (sks - ks), 1 << (skt - kt)
            cx0 = min(cx0, sxa * fx - fx // 2)
            cy0 = min(cy0, sya * fy - fy // 2)
            cx1 = max(cx1, (sxa + sw - 1) * fx + fx // 2 + 1)
            cy1 = max(cy1, (sya + sh - 1) * fy + fy // 2 + 1)
        canvas = self.canvas(terrain, verts, cx0, cy0, cx1 - cx0, cy1 - cy0, ks, kt, su, sv, u0, v0, texels, shade)

        def texel(x, y):
            return canvas[(y - cy0) * (cx1 - cx0) + (x - cx0)]

        big = [[0.0, 0.0, 0.0] for _ in range(w * h)]
        for y in range(ya, ya + h):
            for x in range(xa, xa + w):
                big[(y % h) * w + (x % w)] = texel(x, y)
        resident = (ks, kt, w, h, big)
        big_tile = None
        if small is not None:
            weights_x = [(d - fx // 2, 0.5 if d in (0, fx) else 1.0) for d in range(fx + 1)]
            weights_y = [(d - fy // 2, 0.5 if d in (0, fy) else 1.0) for d in range(fy + 1)]
            pixels = [[0.0, 0.0, 0.0] for _ in range(sw * sh)]
            for j in range(sya, sya + sh):
                for i in range(sxa, sxa + sw):
                    total = [0.0, 0.0, 0.0]
                    for dy, wy in weights_y:
                        for dx, wx in weights_x:
                            t = texel(i * fx + dx, j * fy + dy)
                            for k in range(3):
                                total[k] += t[k] * wx * wy
                    pixels[(j % sh) * sw + (i % sw)] = [t / (fx * fy) for t in total]
            big_tile = resident
            resident = (sks, skt, sw, sh, pixels)
        self.entries.append((batch, resident, big_tile))
        self.stats["tiles"] += 1
        self.stats["big"] += big_tile is not None
        self.stats["texels"] += w * h

    def tiled_image(self, terrain, repeats_x, repeats_y):
        """the base map repeated, its colour and its alpha apart: PIL scales
        an RGBA image premultiplied, and where the alpha is 0 (Blood Gulch's
        grass: its secondary detail) the colour came out black"""
        key = (id(terrain), repeats_x, repeats_y)
        if key not in self.tiled_images:
            base = terrain.image
            bw, bh = base.size
            img = Image.new("RGBA", (bw * repeats_x, bh * repeats_y))
            for ry in range(repeats_y):
                for rx in range(repeats_x):
                    img.paste(base, (rx * bw, ry * bh))
            self.tiled_images[key] = (img.convert("RGB"), img.getchannel("A"))
        return self.tiled_images[key]

    def canvas(self, terrain, verts, x0, y0, n, m, ks, kt, su, sv, u0, v0, texels, shade):
        """the tile texels (x0..x0+n, y0..y0+m): the colour map with its
        detail maps' average (the primary's where its alpha is high, the
        secondary's where low), times the light, held at white (lists of r,
        g, b, 0..1)"""
        base_w, base_h = terrain.image.size

        def uv(x, y):
            return ((x * (1 << ks) / texels + u0) / su, (y * (1 << kt) / texels + v0) / sv)

        # the colour map over the canvas, each texel the average of its
        # footprint (PIL's box filter over the texel's square, about its
        # coordinate)
        fx = (1 << ks) / texels / su * base_w
        fy = (1 << kt) / texels / sv * base_h
        u_first, v_first = uv(x0, y0)
        left, top = u_first * base_w - 0.5 * fx, v_first * base_h - 0.5 * fy
        ox, oy = math.floor(left / base_w) * base_w, math.floor(top / base_h) * base_h
        left, top = left - ox, top - oy
        repeats_x = math.ceil((left + n * fx) / base_w) + 1
        repeats_y = math.ceil((top + m * fy) / base_h) + 1
        rgb, alpha = self.tiled_image(terrain, repeats_x, repeats_y)
        resample = Image.BOX if min(fx, fy) >= 1.0 else Image.BILINEAR
        box = (left, top, left + n * fx, top + m * fy)
        colour = rgb.resize((n, m), resample, box=box)
        colour.putalpha(alpha.resize((n, m), resample, box=box))
        colours = list(colour.getdata())
        # the light, from the terrain's triangles over each texel; texels
        # past the ground take their neighbours'
        light_of = self.lights.get(id(terrain))
        prefer = {id(v) for v in verts}
        lights = [None] * (n * m)
        for j in range(m):
            for i in range(n):
                u, v = uv(x0 + i, y0 + j)
                lights[j * n + i] = light_of.light(u, v, prefer) if light_of else None
        if all(l is None for l in lights):
            mean = [sum(v[3][k] for v in verts) / len(verts) for k in range(3)]
            lights = [mean] * (n * m)
        while any(l is None for l in lights):
            filled = list(lights)
            for j in range(m):
                for i in range(n):
                    if lights[j * n + i] is not None:
                        continue
                    near = [lights[jj * n + ii] for jj in range(max(0, j - 1), min(m, j + 2))
                            for ii in range(max(0, i - 1), min(n, i + 2)) if lights[jj * n + ii] is not None]
                    if near:
                        filled[j * n + i] = [sum(l[k] for l in near) / len(near) for k in range(3)]
            lights = filled
        out = []
        for i in range(n * m):
            colour = terrain.tinted([c / 255.0 for c in colours[i]])
            out.append([min(1.0, max(0.0, c * l * shade)) for c, l in zip(colour, lights[i])])
        return out

    def write(self, path, detail_level=TILE_DETAIL_LEVEL):
        """levels/<map>.tiles (an empty one where no batch is tiled)"""
        header_size = 32
        entries_offset = header_size
        resident = bytearray()
        big = bytearray()
        records = []
        biggest = 0

        def pack(tile):
            _, _, w, h, pixels = tile
            return b"".join(struct.pack(">H", rgba5551(*p)) for p in pixels)

        def size_code(w, h):
            return (int(math.log2(w)) << 4) | int(math.log2(h))

        for batch, small, large in self.entries:
            data = pack(small)
            small_offset = len(resident)
            resident += data + b"\0" * (-len(data) % TILE_ALIGN)
            large_offset = 0xFFFFFFFF
            large_code = (0, 0, 0)
            if large is not None:
                data = pack(large)
                large_offset = len(big)
                big += data + b"\0" * (-len(data) % TILE_ALIGN)
                biggest = max(biggest, len(data))
                large_code = (large[0], large[1], size_code(large[2], large[3]))
            records.append(struct.pack(">HBBBBBBII", batch, small[0], small[1], size_code(small[2], small[3]),
                                       *large_code, small_offset, large_offset))
        resident_offset = entries_offset + len(records) * 16
        resident_offset += -resident_offset % TILE_ALIGN
        big_offset = resident_offset + len(resident)
        out = bytearray(struct.pack(">4sIHHIIIII", b"H64T", VERSION, len(records), detail_level, entries_offset,
                                    resident_offset, len(resident), big_offset, biggest))
        out += b"".join(records)
        out += b"\0" * (resident_offset - len(out))
        out += resident
        out += big
        Path(path).parent.mkdir(parents=True, exist_ok=True)
        Path(path).write_bytes(out)
        return {"tiles": len(records), "big": sum(1 for e in self.entries if e[2] is not None),
                "resident KB": round(len(resident) / 1024, 1), "big KB": round(len(big) / 1024, 1)}
