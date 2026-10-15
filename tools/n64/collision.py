"""Collision BSPs for the N64: reading Halo's, testing them as the game
does, and building new ones from simpler meshes.

A campaign level's collision is far bigger than the N64 can hold (a30's
first BSP: 19,138 surfaces in 2.4 MB). tools/n64/collision_mesh.py makes a
simpler closed mesh of it; this builds the collision BSP the game's own code
(source/physics/collision_bsp.c) walks:

- a solid-leaf 3D BSP: a node's children are nodes, leaves (sign bit set;
  empty space) or NONE (solid). Its planes are the surfaces' planes; a
  node's front (children[1]) is where the plane's normal points;
- a leaf lists, for each plane its surfaces bound it on, a bsp2d reference:
  the plane (with the sign bit when the leaf is behind it) and a 2D BSP that
  finds, in the plane's projection (global_projection3d_mappings), which
  surface a point is on;
- surfaces are convex polygons (at most 8 corners), winding counter-
  clockwise about their normal, joined by winged edges (each edge's
  surfaces[0] walks it from vertices[0] to vertices[1] and continues with
  edges[0], surfaces[1] the other way with edges[1]).

The game finds a vector's hit by walking the 3D BSP: going from an empty
leaf into solid across a plane, it asks the empty leaf's 2D BSP for that
plane which surface is there. Two-sided surfaces separate two empty
("semi-empty", leaf flag 1) leaves and are tested as polygons.

test_vector()/test_point() here are the game's, to check a new BSP against
the map's own (verify()).
"""

import math
import random
import struct

NONE = -1
SIGN = 0x80000000

SURFACE_TWO_SIDED = 1
SURFACE_INVISIBLE = 2
SURFACE_CLIMBABLE = 4
SURFACE_BREAKABLE = 8

LEAF_TWO_SIDED = 1

# the game's projection of a plane's points to 2D (real_math.c
# global_projection3d_mappings[axis][sign])
PROJECTION = (((2, 1), (1, 2)), ((0, 2), (2, 0)), ((1, 0), (0, 1)))


def projection_axis(n):
    """the axis a plane projects along (collision_leaf_test_vector)"""
    i, j, k = abs(n[0]), abs(n[1]), abs(n[2])
    if k >= j and k >= i:
        return 2
    return 1 if j >= i else 0


def project(p, axis, sign):
    a, b = PROJECTION[axis][1 if sign else 0]
    return (p[a], p[b])


def signed(value):
    value &= 0xFFFFFFFF
    return value - 0x100000000 if value & SIGN else value


class CollisionBSP:
    """a collision BSP's blocks, as lists of tuples (the game's fields):

        nodes      (plane, back child, front child)
        planes     (i, j, k, d)
        leaves     (flags, reference count, first reference)
        references (plane designator, 2D root)
        nodes2d    (i, j, d, child0, child1)
        surfaces   (plane designator, first edge, flags, breakable surface, material)
        edges      (vertex0, vertex1, edge0, edge1, surface0, surface1)
        vertices   (x, y, z, first edge)

    Indices with the sign bit (leaves in nodes, surfaces in nodes2d and
    references' roots, flipped planes in designators) are kept as Python
    ints with the bit: signed(value) < 0."""

    def __init__(self):
        self.nodes = []
        self.planes = []
        self.leaves = []
        self.references = []
        self.nodes2d = []
        self.surfaces = []
        self.edges = []
        self.vertices = []

    # ---------- reading and writing

    BLOCKS = (("nodes", "<3i", ">3i"), ("planes", "<4f", ">4f"), ("leaves", "<Hhi", ">Hhi"),
              ("references", "<ii", ">ii"), ("nodes2d", "<3fii", ">3fii"), ("surfaces", "<iiBBh", ">iiBBh"),
              ("edges", "<6i", ">6i"), ("vertices", "<3fi", ">3fi"))

    @classmethod
    def read(cls, xmap, address):
        """the ModelCollisionGeometryBSP at an address of the map"""
        bsp = cls()
        for i, (name, fmt, _) in enumerate(cls.BLOCKS):
            count, pointer, _ = struct.unpack("<iII", xmap.read(address + 12 * i, 12))
            size = struct.calcsize(fmt)
            data = xmap.read(pointer, size * count) if count > 0 else b""
            rows = [struct.unpack_from(fmt, data, k * size) for k in range(max(count, 0))]
            setattr(bsp, name, rows)
        # (signed fields: keep the sign bit as Python sees it)
        return bsp

    def blocks(self):
        """each block as (count, big-endian bytes), in the struct's order"""
        out = []
        for name, _, fmt in self.BLOCKS:
            rows = getattr(self, name)
            data = b"".join(struct.pack(fmt, *(signed(v) if isinstance(v, int) and v > 0x7FFFFFFF else v
                                              for v in row)) for row in rows)
            out.append((len(rows), data))
        return out

    def size(self):
        return sum(len(data) for _, data in self.blocks())

    # ---------- surfaces

    def surface_polygon(self, s):
        """a surface's corners in order (collision_surface_polygon)"""
        first = self.surfaces[s][1]
        e = first
        points = []
        for _ in range(64):
            edge = self.edges[e]
            reverse = edge[5] == s
            points.append(edge[1] if reverse else edge[0])
            e = edge[3] if reverse else edge[2]
            if e == first:
                break
        else:
            raise ValueError(f"surface {s}: its edges do not close")
        return points

    def plane_of(self, designator):
        i, j, k, d = self.planes[designator & 0x7FFFFFFF]
        if signed(designator) < 0:
            return (-i, -j, -k, -d)
        return (i, j, k, d)

    # ---------- the game's tests

    def test_point(self, p):
        """the leaf a point is in, or NONE (solid): bsp3d_test_point"""
        node = 0
        while True:
            plane, back, front = self.nodes[node]
            i, j, k, d = self.planes[plane]
            node = front if p[0] * i + p[1] * j + p[2] * k - d >= 0 else back
            node = signed(node)
            if node < 0:
                return NONE if node == NONE else node & 0x7FFFFFFF

    def surface_test_point(self, s, axis, sign, p2):
        """collision_surface_test_point (no breakables)"""
        first = self.surfaces[s][1]
        e = first
        for _ in range(64):
            edge = self.edges[e]
            reverse = edge[5] == s
            v0 = self.vertices[edge[1] if reverse else edge[0]]
            v1 = self.vertices[edge[0] if reverse else edge[1]]
            a = project(v0, axis, sign)
            b = project(v1, axis, sign)
            ax, ay = p2[0] - a[0], p2[1] - a[1]
            bx, by = b[0] - a[0], b[1] - a[1]
            if ax * by - ay * bx > 0.0:
                return False
            e = edge[3] if reverse else edge[2]
            if e == first:
                return True
        return False

    def bsp2d_test_point(self, root, p2):
        node = signed(root)
        while node >= 0:
            i, j, d, c0, c1 = self.nodes2d[node]
            node = signed(c1 if i * p2[0] + j * p2[1] - d >= 0 else c0)
        return NONE if node == NONE else node & 0x7FFFFFFF

    def leaf_test_vector(self, point, vector, leaf, plane_index, t, test_surface):
        """collision_leaf_test_vector"""
        flags, count, first = self.leaves[leaf]
        for r in range(first, first + count):
            designator, root = self.references[r]
            if designator & 0x7FFFFFFF != plane_index:
                continue
            n = self.planes[plane_index]
            axis = projection_axis(n)
            sign = (n[axis] > 0) != (signed(designator) < 0)
            hit = (point[0] + vector[0] * t, point[1] + vector[1] * t, point[2] + vector[2] * t)
            p2 = project(hit, axis, sign)
            s = self.bsp2d_test_point(root, p2)
            if not test_surface or (s != NONE and self.surface_test_point(s, axis, sign, p2)):
                return s
        return NONE

    def test_vector(self, point, vector, maximum_t=1.0, ignore_two_sided=False):
        """(t, surface) of the first front-facing surface a vector hits, or
        None: collision_bsp_test_vector_recursive with the front-facing
        surfaces' flag"""
        state = {"last_leaf": NONE, "last_contents": 0, "last_plane": NONE, "result": None,
                 "t": maximum_t}

        def recurse(node, t0, t1):
            node = signed(node)
            if node >= 0:
                plane, back, front = self.nodes[node]
                i, j, k, d = self.planes[plane]
                distance = point[0] * i + point[1] * j + point[2] * k - d
                dot = vector[0] * i + vector[1] * j + vector[2] * k
                d0, d1 = dot * t0 + distance, dot * t1 + distance
                reaches_back = d0 < 0.0 or d1 < 0.0
                reaches_front = d0 >= 0.0 or d1 >= 0.0
                if reaches_back and reaches_front:
                    is_front = dot > 0.0
                    t = -(distance / dot)
                    if recurse(front if not is_front else back, t0, t):
                        return True
                    if state["t"] <= t:
                        return False
                    state["last_plane"] = plane
                    return recurse(front if is_front else back, t, t1)
                return recurse(front if reaches_front else back, t0, t1)
            leaf = NONE
            contents = 3  # solid
            if node != NONE:
                leaf = node & 0x7FFFFFFF
                contents = 2 if self.leaves[leaf][0] & LEAF_TWO_SIDED else 1
            last = state["last_contents"]
            test_leaf, test_surface = NONE, False
            if last in (1, 2) and contents == 3:
                test_leaf = state["last_leaf"]
            elif not ignore_two_sided and last == 2 and contents == 2:
                test_leaf, test_surface = state["last_leaf"], True
            if test_leaf != NONE:
                s = self.leaf_test_vector(point, vector, test_leaf, state["last_plane"], t0, test_surface)
                if s != NONE:
                    state["result"] = (t0, s)
                    state["t"] = t0
                    return True
            state["last_leaf"] = leaf
            state["last_contents"] = contents
            return False

        recurse(0, 0.0, max(0.0, min(1.0, maximum_t)))
        return state["result"]

    # ---------- checks

    def check(self):
        """structural problems (a list of strings; empty when sound)"""
        problems = []
        for s in range(len(self.surfaces)):
            try:
                corners = self.surface_polygon(s)
            except ValueError as e:
                problems.append(str(e))
                continue
            if len(corners) > 8:
                problems.append(f"surface {s}: {len(corners)} corners")
        for n, (plane, back, front) in enumerate(self.nodes):
            for child in (back, front):
                c = signed(child)
                if c >= 0 and c >= len(self.nodes):
                    problems.append(f"node {n}: child {c} past the nodes")
                elif c < 0 and c != NONE and (c & 0x7FFFFFFF) >= len(self.leaves):
                    problems.append(f"node {n}: leaf {c & 0x7FFFFFFF} past the leaves")
        for leaf, (flags, count, first) in enumerate(self.leaves):
            if first + count > len(self.references):
                problems.append(f"leaf {leaf}: references past the end")
        return problems


def polygon_normal(points):
    """Newell's normal of a polygon (unnormalised)"""
    nx = ny = nz = 0.0
    for a, b in zip(points, points[1:] + points[:1]):
        nx += (a[1] - b[1]) * (a[2] + b[2])
        ny += (a[2] - b[2]) * (a[0] + b[0])
        nz += (a[0] - b[0]) * (a[1] + b[1])
    return (nx, ny, nz)


def conventions(bsp, samples=400, seed=1):
    """what the map's BSP says about its conventions: for sampled
    one-sided surfaces, whether the corners wind counter-clockwise about the
    designator's normal, and whether in front of the surface is empty and
    behind it solid"""
    rng = random.Random(seed)
    stats = {"ccw": 0, "cw": 0, "front_empty": 0, "front_solid": 0, "back_solid": 0, "back_empty": 0}
    for _ in range(samples):
        s = rng.randrange(len(bsp.surfaces))
        designator, _, flags, _, _ = bsp.surfaces[s]
        if flags & SURFACE_TWO_SIDED:
            continue
        corners = [bsp.vertices[v][:3] for v in bsp.surface_polygon(s)]
        n = bsp.plane_of(designator)
        newell = polygon_normal(corners)
        stats["ccw" if newell[0] * n[0] + newell[1] * n[1] + newell[2] * n[2] > 0 else "cw"] += 1
        centre = [sum(c[k] for c in corners) / len(corners) for k in range(3)]
        for side, name in ((0.02, "front"), (-0.02, "back")):
            p = [centre[k] + n[k] * side for k in range(3)]
            leaf = bsp.test_point(p)
            stats[f"{name}_{'solid' if leaf == NONE else 'empty'}"] += 1
    return stats


# ---------- the campaign cartridge's compact collision (HALO_N64_CAMPAIGN:
# source/physics/bsp3d.h, collision_bsp_definitions.h, collision_bsp.c)

# the blocks of a ModelCollisionGeometryBSP by their tag definition names
BLOCK_NAMES = {"bsp3d nodes": "nodes", "planes": "planes", "leaves": "leaves", "bsp2d references": "references",
               "bsp2d nodes": "nodes2d", "surfaces": "surfaces", "edges": "edges", "vertices": "vertices"}


def flagged16(value):
    """a 32-bit flagged index (bit 31: a leaf, a surface, a flipped plane;
    NONE) in 16 bits: bit 15 the flag, 0xFFFF NONE"""
    value = signed(value)
    if value == NONE:
        return 0xFFFF
    if value < 0:
        index = value & 0x7FFFFFFF
        if index >= 0x7FFF:
            raise ValueError(f"flagged index {index} past 15 bits")
        return 0x8000 | index
    if value >= 0x8000:
        raise ValueError(f"index {value} past 15 bits")
    return value


def short16(value):
    value = signed(value)
    if value != NONE and not 0 <= value < 0x8000:
        raise ValueError(f"index {value} past 15 bits")
    return value


def compact_rows(name, rows):
    """a block's rows (CollisionBSP's tuples) in the compact layout:
    (count, big-endian bytes)"""
    out = bytearray()
    if name == "nodes":
        for plane, back, front in rows:
            out += struct.pack(">hHH", short16(plane), flagged16(back), flagged16(front))
    elif name == "planes":
        for row in rows:
            out += struct.pack(">4f", *row)
    elif name == "leaves":
        for flags, count, first in rows:
            if count > 255 or first > 0xFFFF:
                raise ValueError(f"a leaf's references ({count} from {first}) past the compact layout")
            out += struct.pack(">BBH", flags & 0xFF, count, first)
    elif name == "references":
        for designator, root in rows:
            out += struct.pack(">HH", flagged16(designator), flagged16(root))
    elif name == "nodes2d":
        for i, j, d, c0, c1 in rows:
            out += struct.pack(">3fii", i, j, d, signed(c0), signed(c1))
    elif name == "surfaces":
        for designator, first, flags, breakable, material in rows:
            out += struct.pack(">iiBBh", signed(designator), signed(first), flags, breakable, material)
    elif name == "edges":
        for row in rows:
            out += struct.pack(">6h", *(short16(v) for v in row))
    elif name == "vertices":
        for x, y, z, first in rows:
            out += struct.pack(">3fi", x, y, z, signed(first))
    else:
        raise KeyError(name)
    return len(rows), bytes(out)


def compact_block_from_map(xmap, name, count, pointer):
    """a ModelCollisionGeometryBSP block of the map, in the compact layout"""
    fmt = dict((n, f) for n, f, _ in CollisionBSP.BLOCKS)[name]
    size = struct.calcsize(fmt)
    data = xmap.read(pointer, size * count) if count > 0 else b""
    rows = [struct.unpack_from(fmt, data, k * size) for k in range(max(count, 0))]
    return compact_rows(name, rows)


def compact_size(bsp):
    return sum(len(compact_rows(name, getattr(bsp, name))[1]) for name in BLOCK_NAMES.values())
