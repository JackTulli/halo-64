"""Obstacle stand-ins: what the N64 draws for an obstacle whose model it has
no room for (port/n64/src/n64_render.c, standin_queue), so that a player
never runs into something unseen.

An obstacle is an object the map places -- scenery, a machine, a control
or a light fixture -- whose definition has a collision model. For each
collision node and each of its BSPs (one per permutation of the node's
region: a boulder's five shapes) the stand-in is the convex hull of at most
HULL_POINTS of the BSP's vertices, in the node's space, so that it follows
the shape the object collides as, not the model drawn. A BSP's surfaces of
the "leaves" material (a tree's canopy) make a hull of their own, drawn
see-through: the canopy stops players, vehicles and shots, but not sight.

The colour is the model's: each part's texture average times its vertex
colour (where the converter moved a greyscale texture's colour), weighted
by triangles, times CALIBRATION -- the RDP's drawn result is brighter than
that product (a boulder's drawn pixels average 0.36, the product 0.27, and
the hull's own sky shading averages ~0.78). The solid parts take the
model's other batches' colour, the see-through ones its two-sided (leaf
card) batches'.

The file, levels/<map>.sti (big-endian):

    header     'H64S', version, type count, part count, vertex count,
               triangle count (16 bytes)
    types[]    the obstacle definition's absolute tag index, first part,
               part count, 0 (8 bytes)
    parts[]    node, region (-1: none), BSP, the node's BSP count, flags
               (1: see-through), vertex count, triangle count, 0, first
               vertex, first triangle, colour r g b, 0 (16 bytes)
    vertices[] x y z (node space * 1024), shade, 0 (8 bytes)
    triangles[] three vertex numbers of the part (3 bytes)

A part is drawn for an object whose permutation of the part's region,
pinned to the node's BSP count as the engine's collision is
(source/physics/collision_models.c), is the part's BSP. Types are keyed by
the definition, not the render model: the collision model is the
definition's, and two definitions may share a render model."""

import math
import struct
from pathlib import Path

from .repack import NONE

STANDIN_VERSION = 1
HULL_POINTS = 10
# the most parts a type has (the game's STANDIN_TYPE_PARTS: a queued stand-in's parts are a 32-bit mask)
TYPE_PARTS = 32
LEAVES = 29
CALIBRATION = 1.7
GREY = (0.55, 0.55, 0.55)
SKY = (0.3, 0.2, 0.93)
OBSTACLE_PALETTES = (("scenery palette", "scenery", "ScenarioScenery"),
                     ("machine palette", "machines", "ScenarioMachine"),
                     ("control palette", "controls", "ScenarioControl"),
                     ("light fixture palette", "light fixtures", "ScenarioLightFixture"))


def _sub(a, b): return (a[0] - b[0], a[1] - b[1], a[2] - b[2])
def _cross(a, b): return (a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0])
def _dot(a, b): return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]


def _norm(a):
    length = math.sqrt(_dot(a, a)) or 1.0
    return (a[0] / length, a[1] / length, a[2] / length)


_SKY = _norm(SKY)


def hull(points):
    """the convex hull (incremental): its vertices and outward triangles"""
    pts = list(dict.fromkeys(tuple(round(c, 5) for c in p) for p in points))
    if len(pts) < 4:
        return [], []
    a = 0
    b = max(range(len(pts)), key=lambda i: _dot(_sub(pts[i], pts[a]), _sub(pts[i], pts[a])))
    edge = _sub(pts[b], pts[a])
    c = max(range(len(pts)), key=lambda i: _dot(_cross(edge, _sub(pts[i], pts[a])), _cross(edge, _sub(pts[i], pts[a]))))
    normal = _cross(edge, _sub(pts[c], pts[a]))
    d = max(range(len(pts)), key=lambda i: abs(_dot(normal, _sub(pts[i], pts[a]))))
    if abs(_dot(normal, _sub(pts[d], pts[a]))) < 1e-9:
        return [], []
    centre = tuple(sum(pts[i][k] for i in (a, b, c, d)) / 4 for k in range(3))

    def outward(f):
        p0, p1, p2 = (pts[i] for i in f)
        return f if _dot(_cross(_sub(p1, p0), _sub(p2, p0)), _sub(p0, centre)) > 0 else (f[0], f[2], f[1])
    faces = [outward(f) for f in ((a, b, c), (a, c, d), (a, d, b), (b, d, c))]
    for i, p in enumerate(pts):
        if i in (a, b, c, d):
            continue
        visible = [f for f in faces if _dot(_cross(_sub(pts[f[1]], pts[f[0]]), _sub(pts[f[2]], pts[f[0]])),
                                            _sub(p, pts[f[0]])) > 1e-9]
        if not visible:
            continue
        edges = set()
        for f in visible:
            edges.update(((f[0], f[1]), (f[1], f[2]), (f[2], f[0])))
        seen = set(visible)
        faces = [f for f in faces if f not in seen] + [(e[0], e[1], i) for e in edges if (e[1], e[0]) not in edges]
    used = sorted({j for f in faces for j in f})
    remap = {j: k for k, j in enumerate(used)}
    return [pts[j] for j in used], [tuple(remap[j] for j in f) for f in faces]


def lean_points(points, count=HULL_POINTS):
    """at most count of the points: the lowest, then each the farthest from
    those already taken (the hull of these is the stand-in)"""
    pts = list(dict.fromkeys(tuple(p) for p in points))
    if len(pts) <= count:
        return pts
    first = min(pts, key=lambda p: p[2])
    chosen = [first]
    distance = [_dot(_sub(p, first), _sub(p, first)) for p in pts]
    while len(chosen) < count:
        i = max(range(len(pts)), key=lambda i: distance[i])
        chosen.append(pts[i])
        for j, p in enumerate(pts):
            distance[j] = min(distance[j], _dot(_sub(p, pts[i]), _sub(p, pts[i])))
    return chosen


def vertex_shades(vertices, triangles):
    """the converter's sky light (models.py), on each vertex's mean normal"""
    sums = [(0.0, 0.0, 0.0)] * len(vertices)
    for t in triangles:
        p0, p1, p2 = (vertices[i] for i in t)
        n = _cross(_sub(p1, p0), _sub(p2, p0))
        for i in t:
            sums[i] = (sums[i][0] + n[0], sums[i][1] + n[1], sums[i][2] + n[2])
    return [min(1.0, 0.5 + 0.5 * max(0.0, _dot(_norm(s), _SKY))) for s in sums]


def _block(xmap, address):
    return struct.unpack("<iI", xmap.read(address, 8))


def _datum(xmap, address):
    return struct.unpack("<I", xmap.read(address + 12, 4))[0]


def obstacle_types(xmap, defs):
    """(definition tag, render model tag index, collision model tag) for each
    obstacle type the scenario places"""
    sf = {fl.name: o for o, fl in defs.fields("Scenario")}
    of = {fl.name: o for o, fl in defs.fields("Object")}
    out, seen = [], set()
    for palette_field, instance_field, instance_struct in OBSTACLE_PALETTES:
        pc, pp = _block(xmap, xmap.scenario.address + sf[palette_field])
        ic, ip = _block(xmap, xmap.scenario.address + sf[instance_field])
        psize = defs.size(instance_struct + "Palette")
        isize = defs.size(instance_struct)
        placed = set()
        for i in range(max(ic, 0)):
            placed.add(struct.unpack("<h", xmap.read(ip + i * isize, 2))[0])
        for i in range(max(pc, 0)):
            if i not in placed:
                continue
            tag = xmap.tag(_datum(xmap, pp + i * psize))
            # (a campaign map leaves some kinds out: their group is NONE and
            # their placements are not made, so they get no stand-in)
            if not tag or tag.index in seen or tag.group == NONE:
                continue
            model = xmap.tag(_datum(xmap, tag.address + of["model"]))
            collision = xmap.tag(_datum(xmap, tag.address + of["collision model"]))
            if model and collision:
                seen.add(tag.index)
                out.append((tag, model.index, collision))
    return out


def collision_parts(xmap, defs, collision):
    """each node's BSPs' vertices by material group: (node, region, BSP, the
    node's BSP count, leaves, points)"""
    cf = {fl.name: o for o, fl in defs.fields("ModelCollisionGeometry")}
    mf = {fl.name: o for o, fl in defs.fields("ModelCollisionGeometryMaterial")}
    nf = {fl.name: o for o, fl in defs.fields("ModelCollisionGeometryNode")}
    bf = {fl.name: o for o, fl in defs.fields("ModelCollisionGeometryBSP")}
    msize, nsize, bsize = (defs.size(s) for s in ("ModelCollisionGeometryMaterial", "ModelCollisionGeometryNode",
                                                   "ModelCollisionGeometryBSP"))
    vsize, ssize, esize = (defs.size(s) for s in ("ModelCollisionGeometryBSPVertex", "ModelCollisionGeometryBSPSurface",
                                                   "ModelCollisionGeometryBSPEdge"))
    a = collision.address
    mc, mp = _block(xmap, a + cf["materials"])
    materials = [struct.unpack("<h", xmap.read(mp + i * msize + mf["material type"], 2))[0] for i in range(max(mc, 0))]
    nc, np_ = _block(xmap, a + cf["nodes"])
    parts = []
    for n in range(max(nc, 0)):
        region = struct.unpack("<h", xmap.read(np_ + n * nsize + nf["region"], 2))[0]
        if region < 0:
            continue  # (a node of no region has no collision: collision_models.c)
        bc, bp = _block(xmap, np_ + n * nsize + nf["bsps"])
        for b in range(max(bc, 0)):
            base = bp + b * bsize
            vc, vp = _block(xmap, base + bf["vertices"])
            sc, sp = _block(xmap, base + bf["surfaces"])
            ec, ep = _block(xmap, base + bf["edges"])
            vertices = [struct.unpack("<3f", xmap.read(vp + v * vsize, 12)) for v in range(max(vc, 0))]
            edges = [struct.unpack("<6i", xmap.read(ep + e * esize, 24)) for e in range(max(ec, 0))]
            groups = {False: set(), True: set()}
            for s in range(max(sc, 0)):
                _plane, first, _flags, _breakable, material = struct.unpack("<iiBbh", xmap.read(sp + s * ssize, 12))
                leaves = 0 <= material < len(materials) and materials[material] == LEAVES
                e, guard = first, 0
                # (the surface's edge ring: its vertices)
                while 0 <= e < len(edges) and guard < 64:
                    v0, v1, n0, n1, s0, _s1 = edges[e]
                    if s0 == s:
                        groups[leaves].add(v0)
                        e = n0
                    else:
                        groups[leaves].add(v1)
                        e = n1
                    guard += 1
                    if e == first:
                        break
            for leaves, indices in groups.items():
                if len(indices) >= 4:
                    parts.append((n, region, b, bc, leaves, [vertices[i] for i in sorted(indices) if i < len(vertices)]))
    return parts


def _dca(data):
    """libdragon's asset compression (DCA5, LZ4) undone; raw data as it is"""
    if data[:4] != b"DCA5":
        return data
    cursor, values = 5, []
    for _ in range(3):
        value = shift = 0
        while True:
            b = data[cursor]
            cursor += 1
            value |= (b & 127) << shift
            if not b & 128:
                break
            shift += 7
        values.append(value)
    cursor = (cursor + 1) & ~1
    size, original, _ = values
    block = data[cursor:cursor + size]
    out, p = bytearray(), 0

    def length(base):
        nonlocal p
        if base == 15:
            while True:
                extra = block[p]
                p += 1
                base += extra
                if extra < 255:
                    break
        return base
    while p < len(block):
        token = block[p]
        p += 1
        literals = length(token >> 4)
        out += block[p:p + literals]
        p += literals
        if p == len(block):
            break
        offset = int.from_bytes(block[p:p + 2], "little")
        p += 2
        for _ in range(length(token & 15) + 4):
            out.append(out[-offset])
    return bytes(out[:original])


def _rgba16(v):
    return ((v >> 11) & 31) / 31.0, ((v >> 6) & 31) / 31.0, ((v >> 1) & 31) / 31.0, v & 1


def sprite_average(path):
    """a converted sprite's average colour over its opaque texels, or None"""
    try:
        d = _dca(Path(path).read_bytes())
    except OSError:
        return None
    w, h, _, flags = struct.unpack_from(">HHBB", d, 0)
    fmt, px, n = flags & 0x1F, d[8:], w * h
    texels = []
    if fmt == 2:  # RGBA16
        texels = [_rgba16(struct.unpack_from(">H", px, 2 * i)[0]) for i in range(n)]
    elif fmt == 3:  # RGBA32
        texels = [(px[4 * i] / 255, px[4 * i + 1] / 255, px[4 * i + 2] / 255, px[4 * i + 3] >= 128) for i in range(n)]
    elif fmt in (8, 9):  # CI4, CI8: the palette after the pixels (or where the extended header says)
        bits = 4 if fmt == 8 else 8
        at = (8 + (n * bits + 7) // 8 + 7) & ~7
        if flags & 0x80:
            at = struct.unpack_from(">I", d, at + 4)[0]
        palette = [_rgba16(struct.unpack_from(">H", d, at + 2 * k)[0]) for k in range(16 if bits == 4 else 256)
                   if at + 2 * k + 2 <= len(d)]
        for i in range(n):
            k = ((px[i // 2] >> 4) if i % 2 == 0 else (px[i // 2] & 15)) if bits == 4 else px[i]
            if k < len(palette):
                texels.append(palette[k])
    elif fmt in (12, 13, 14, 16, 17):  # IA4, IA8, IA16, I4, I8
        for i in range(n):
            if fmt == 17:
                v, a = px[i] / 255, 1
            elif fmt == 16:
                v, a = ((px[i // 2] >> 4) if i % 2 == 0 else (px[i // 2] & 15)) / 15, 1
            elif fmt == 13:
                v, a = (px[i] >> 4) / 15, (px[i] & 15) >= 8
            elif fmt == 12:
                nib = (px[i // 2] >> 4) if i % 2 == 0 else (px[i // 2] & 15)
                v, a = (nib >> 1) / 7, nib & 1
            else:
                v, a = px[2 * i] / 255, px[2 * i + 1] >= 128
            texels.append((v, v, v, a))
    opaque = [t for t in texels if t[3]] or texels
    if not opaque:
        return None
    return tuple(sum(t[k] for t in opaque) / len(opaque) for k in range(3))


def model_colours(filesystem, mdl_path, model_tag_index):
    """(solid, leaves) colours of a converted model, from its table, data
    and textures as written (models.py), or None where it has none"""
    try:
        d = Path(mdl_path).read_bytes()
    except OSError:
        return None, None
    _m, _v, tc, mc, _rc, _pc, _gc, _bc, _sc, _x, to, mo, _ro, _po, _go, bo, so, _vo, _io = \
        struct.unpack_from(">4sIHHHHHHHHIIIIIIIII", d, 0)
    names = [d[to + 28 * i: to + 28 * i + 24].split(b"\0")[0].decode() for i in range(tc)]
    for m in range(mc):
        tag, _n, _r, _fr, _x2, fb, count, data_name, vb, _ib, near, _fc = struct.unpack_from(">HBBHHHH16sIIHH", d, mo + 40 * m)
        if tag != model_tag_index:
            continue
        try:
            data = (Path(filesystem) / "mdl" / f"{data_name.split(b'\0')[0].decode()}.bin").read_bytes()
        except OSError:
            return None, None
        sums = {False: [0.0, 0.0, 0.0, 0], True: [0.0, 0.0, 0.0, 0]}
        for b in range(fb, fb + near):
            texture, flags, segments, triangles, first_segment, _index = struct.unpack_from(">hHBBHI", d, bo + 12 * b)
            if texture < 0 or not triangles:
                continue
            average = sprite_average(Path(filesystem) / "tex" / f"{names[texture]}.sprite")
            if average is None:
                continue
            shade, count_v = [0.0, 0.0, 0.0], 0
            for g in range(first_segment, first_segment + segments):
                _node, _slot, vertices, _x3, pair = struct.unpack_from(">BBBBI", d, so + 8 * g)
                for k in range(vertices):
                    o = (pair + k // 2) * 32 + 16 + 4 * (k % 2)
                    for c in range(3):
                        shade[c] += data[o + c] / 255
                    count_v += 1
            if not count_v:
                continue
            group = sums[bool(flags & 2)]  # (MATERIAL_TWO_SIDED: the leaf cards)
            for c in range(3):
                group[c] += average[c] * shade[c] / count_v * triangles
            group[3] += triangles

        def colour(g):
            return tuple(min(1.0, g[c] / g[3] * CALIBRATION) for c in range(3)) if g[3] else None
        solid, leaves = colour(sums[False]), colour(sums[True])
        return solid or leaves, leaves or solid
    return None, None


def write(xmap, defs, filesystem, map_name, path, mdl_path=None):
    """levels/<map>.sti for the map; mdl_path: its model table as written
    (for the colours; grey without)"""
    filesystem = Path(filesystem)
    mdl_path = Path(mdl_path) if mdl_path else filesystem / "levels" / f"{map_name}.mdl"
    types, parts, vertices, triangles = [], [], [], []
    for definition, model_index, collision in obstacle_types(xmap, defs):
        solid_colour, leaf_colour = model_colours(filesystem, mdl_path, model_index)
        first = len(parts)
        for node, region, bsp, bsp_count, leaves, points in collision_parts(xmap, defs, collision):
            hv, ht = hull(lean_points(points))
            if not ht or len(hv) > 255 or len(ht) > 255:
                continue
            shades = vertex_shades(hv, ht)
            colour = (leaf_colour if leaves else solid_colour) or GREY
            parts.append((node, region, bsp, bsp_count, 1 if leaves else 0, len(hv), len(ht), len(vertices),
                          len(triangles), colour))
            vertices += list(zip(hv, shades))
            triangles += ht
        if len(parts) - first > TYPE_PARTS:
            print(f"{map_name}: stand-in of {definition.index}: {len(parts) - first} parts, the first {TYPE_PARTS} kept")
            vertices = vertices[:parts[first + TYPE_PARTS][7]]
            triangles = triangles[:parts[first + TYPE_PARTS][8]]
            del parts[first + TYPE_PARTS:]
        if len(parts) > first:
            types.append((definition.index, first, len(parts) - first))
    out = bytearray(b"H64S" + struct.pack(">HHHHHH", STANDIN_VERSION, len(types), len(parts), len(vertices),
                                          len(triangles), 0))
    for tag_index, first, count in types:
        out += struct.pack(">HHHH", tag_index, first, count, 0)
    for node, region, bsp, bsp_count, flags, nv, nt, fv, ft, colour in parts:
        out += struct.pack(">BbBBBBBxHHBBBx", min(node, 255), max(-1, min(region, 127)), min(bsp, 255),
                           min(bsp_count, 255), flags, nv, nt, fv, ft,
                           *(max(0, min(255, round(c * 255))) for c in colour))
    for p, shade in vertices:
        out += struct.pack(">hhhBx", *(max(-32768, min(32767, round(c * 1024))) for c in p), round(shade * 255))
    for t in triangles:
        out += bytes(t)
    Path(path).parent.mkdir(parents=True, exist_ok=True)
    Path(path).write_bytes(bytes(out))
    return {"types": len(types), "parts": len(parts), "vertices": len(vertices), "triangles": len(triangles),
            "bytes": len(out)}


def main():
    """standins.py <source map> <converted file system> [--pc-expansion --pc-maps <dir>] [--campaign] [--custom]
    [--name <map name>]:
    levels/<map>.sti for a map already converted (its model table and
    textures in the file system)"""
    import argparse
    from .convert import open_map
    from .tagdefs import TagDefinitions
    parser = argparse.ArgumentParser()
    parser.add_argument("map")
    parser.add_argument("filesystem")
    parser.add_argument("--definitions", default="/work/n64/invader/src/tag/hek/definition")
    parser.add_argument("--pc-expansion", action="store_true")
    parser.add_argument("--pc-maps")
    parser.add_argument("--campaign", action="store_true")
    parser.add_argument("--custom", action="store_true")
    parser.add_argument("--name")
    args = parser.parse_args()
    xmap = open_map(args.map, campaign=args.campaign, pc_expansion=args.pc_expansion, pc_maps=args.pc_maps,
                    custom=args.custom)
    name = args.name or xmap.name
    defs = TagDefinitions(args.definitions)
    if args.campaign:
        # (the kinds the campaign conversion leaves out, as convert.py hides them)
        from .campaign import campaign_closure, mask_dropped_tags
        mask_dropped_tags(xmap, campaign_closure(xmap, defs, (0,)).kept)
    stats = write(xmap, defs, args.filesystem, name,
                  Path(args.filesystem) / "levels" / f"{name}.sti")
    print(f"{name} stand-ins: " + ", ".join(f"{k} {v}" for k, v in stats.items()))


if __name__ == "__main__":
    main()
