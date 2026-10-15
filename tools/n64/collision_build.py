"""Building a Halo collision BSP from a closed polygon mesh (collision.py
describes what the game expects).

The mesh's polygons are convex, wind counter-clockwise about their normals
(which point into empty space) and close up: solid behind every one-sided
polygon. Two-sided polygons (sheets in empty space) may stand anywhere.

build_bsp() makes the 3D BSP the classic way: a polygon's plane splits the
rest (those across it are cut, as fragments of the same surface); the
polygons on the plane stay with its node, and a side with no polygon left
is a leaf: empty when a polygon on the node faces it, else solid. Then each
polygon kept at a node is pushed down its empty side(s) to the leaves it
touches, and each leaf gets, for every plane with polygons on its boundary,
a 2D BSP of those fragments (cut by their edges) that names the surface
under a point.
"""

import math
import random

from .collision import (LEAF_TWO_SIDED, NONE, PROJECTION, SIGN, SURFACE_TWO_SIDED, CollisionBSP,
                        polygon_normal, projection_axis)

EPSILON = 1.0 / 2048.0
# a split candidate's cost: cutting a polygon in two against putting
# polygons on the bigger side
SPLIT_COST = 4.0
CANDIDATES = 24
SAMPLE = 600


class Surface:
    """a mesh polygon: its corners (vertex indices, counter-clockwise), its
    plane index and whether it faces the plane's normal (designator sign),
    flags, breakable index, material"""
    __slots__ = ("corners", "plane", "flipped", "flags", "breakable", "material", "pathfinding")

    def __init__(self, corners, plane, flipped, flags, breakable, material, pathfinding=0):
        self.corners = corners
        self.plane = plane
        self.flipped = flipped
        self.flags = flags
        self.breakable = breakable
        self.material = material
        self.pathfinding = pathfinding


class Fragment:
    """a piece of a surface (its points) on its way down the BSP"""
    __slots__ = ("surface", "points")

    def __init__(self, surface, points):
        self.surface = surface
        self.points = points


def plane_distance(plane, p):
    return plane[0] * p[0] + plane[1] * p[1] + plane[2] * p[2] - plane[3]


def clip(points, plane, keep_front):
    """the part of a convex polygon on one side of a plane (None if none)"""
    out = []
    count = len(points)
    distances = [plane_distance(plane, p) for p in points]
    for i in range(count):
        a, b = points[i], points[(i + 1) % count]
        da, db = distances[i], distances[(i + 1) % count]
        a_in = da >= -EPSILON if keep_front else da <= EPSILON
        b_in = db >= -EPSILON if keep_front else db <= EPSILON
        if a_in:
            out.append(a)
        if (da > EPSILON and db < -EPSILON) or (da < -EPSILON and db > EPSILON):
            t = da / (da - db)
            out.append((a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t))
    # (degenerate remains: fewer than three corners, or no area)
    if len(out) < 3:
        return None
    n = polygon_normal(out)
    if n[0] * n[0] + n[1] * n[1] + n[2] * n[2] < 1e-12:
        return None
    return out


def classify(points, plane):
    """0 on the plane, 1 in front, -1 behind, 2 across"""
    front = back = False
    for p in points:
        d = plane_distance(plane, p)
        if d > EPSILON:
            front = True
        elif d < -EPSILON:
            back = True
    if front and back:
        return 2
    if front:
        return 1
    if back:
        return -1
    return 0


class Builder:
    def __init__(self, vertices, planes, surfaces, seed=7):
        """vertices: [(x, y, z)]; planes: [(i, j, k, d)] unit normals;
        surfaces: [Surface]"""
        self.vertices = vertices
        self.planes = planes
        self.surfaces = surfaces
        self.rng = random.Random(seed)
        self.bsp = CollisionBSP()
        # per node: [(surface index, fragment points, faces the front)]
        self.on_node = []
        self.stats = {"splits": 0, "depth": 0}

    # ---------- the 3D BSP

    def choose(self, fragments):
        """the plane to split on: of a sample of the fragments' planes, the
        one that cuts fewest and balances best"""
        planes = list({self.surfaces[f.surface].plane for f in fragments})
        # (one-sided planes first: a two-sided sheet's plane decides no
        # solidity)
        one_sided = [p for p in planes
                     if any(self.surfaces[f.surface].plane == p and not self.surfaces[f.surface].flags & SURFACE_TWO_SIDED
                            for f in fragments)] if len(planes) < 64 else planes
        if one_sided:
            planes = one_sided
        if len(planes) > CANDIDATES:
            planes = self.rng.sample(planes, CANDIDATES)
        sample = fragments if len(fragments) <= SAMPLE else self.rng.sample(fragments, SAMPLE)
        best, best_cost = None, None
        for p in planes:
            plane = self.planes[p]
            front = back = splits = 0
            for f in sample:
                c = classify(f.points, plane)
                if c == 1:
                    front += 1
                elif c == -1:
                    back += 1
                elif c == 2:
                    splits += 1
            cost = SPLIT_COST * splits + abs(front - back)
            if best is None or cost < best_cost:
                best, best_cost = p, cost
        return best

    def new_leaf(self):
        self.bsp.leaves.append([0, 0, 0])
        return SIGN | (len(self.bsp.leaves) - 1)

    def build(self, fragments, depth=0):
        """a node for the fragments (a cell with polygons in it)"""
        self.stats["depth"] = max(self.stats["depth"], depth)
        p = self.choose(fragments)
        plane = self.planes[p]
        node = len(self.bsp.nodes)
        self.bsp.nodes.append([p, NONE, NONE])
        self.on_node.append([])
        front, back = [], []
        faces_front = faces_back = False
        for f in fragments:
            s = self.surfaces[f.surface]
            # (a surface lies on its own plane, whatever rounding says: a
            # quad off its plane by more than the epsilon was cut by it and
            # never stayed with its node)
            c = 0 if s.plane == p else classify(f.points, plane)
            if c == 0:
                # (a fragment of another plane within the epsilon of this one
                # lies on it too: a sliver cut off at a corner where several
                # surfaces meet, sent to one side, once stood alone in a big
                # cell and made all of it solid)
                if s.plane == p:
                    facing = not s.flipped
                else:
                    n = self.planes[s.plane]
                    facing = (n[0] * plane[0] + n[1] * plane[1] + n[2] * plane[2] > 0) != s.flipped
                self.on_node[node].append((f.surface, f.points, facing))
                if s.flags & SURFACE_TWO_SIDED:
                    faces_front = faces_back = True
                elif facing:
                    faces_front = True
                else:
                    faces_back = True
            elif c == 1:
                front.append(f)
            elif c == -1:
                back.append(f)
            else:
                self.stats["splits"] += 1
                a = clip(f.points, plane, True)
                b = clip(f.points, plane, False)
                if a:
                    front.append(Fragment(f.surface, a))
                if b:
                    back.append(Fragment(f.surface, b))
        if front:
            self.bsp.nodes[node][2] = self.build(front, depth + 1)
        else:
            self.bsp.nodes[node][2] = self.new_leaf() if faces_front else NONE
        if back:
            self.bsp.nodes[node][1] = self.build(back, depth + 1)
        else:
            self.bsp.nodes[node][1] = self.new_leaf() if faces_back else NONE
        return node

    # ---------- the leaves' surfaces

    def push(self, child, plane_index, behind, surface, points, refs):
        """a fragment on a node's plane, down the side `child`: the empty
        leaves it touches get (plane, surface, points)"""
        if child == NONE:
            return
        if child & SIGN:
            leaf = child & 0x7FFFFFFF
            refs.setdefault(leaf, {}).setdefault((plane_index, behind), []).append((surface, points))
            return
        p, back, front = self.bsp.nodes[child]
        plane = self.planes[p]
        c = classify(points, plane)
        if c == 1:
            self.push(front, plane_index, behind, surface, points, refs)
        elif c == -1:
            self.push(back, plane_index, behind, surface, points, refs)
        elif c == 0:
            # (on this plane too: lies on both sides' boundary)
            self.push(front, plane_index, behind, surface, points, refs)
            self.push(back, plane_index, behind, surface, points, refs)
        else:
            a = clip(points, plane, True)
            b = clip(points, plane, False)
            if a:
                self.push(front, plane_index, behind, surface, a, refs)
            if b:
                self.push(back, plane_index, behind, surface, b, refs)

    def build_2d(self, fragments, axis, sign):
        """a 2D BSP over (surface, 2D polygon) fragments: a node index, or a
        surface with the sign bit"""
        surfaces = {s for s, _ in fragments}
        if len(surfaces) == 1:
            return SIGN | fragments[0][0]
        # the first edge that leaves fragments on both sides
        best = None
        for s, poly in fragments:
            count = len(poly)
            for i in range(count):
                a, b = poly[i], poly[(i + 1) % count]
                ex, ey = b[0] - a[0], b[1] - a[1]
                length = math.hypot(ex, ey)
                if length < 1e-6:
                    continue
                # (the line's normal, pointing out of this polygon for a
                # counter-clockwise one: either way works)
                ni, nj = ey / length, -ex / length
                d = ni * a[0] + nj * a[1]
                front = back = 0
                for _, q in fragments:
                    side = side_2d(q, ni, nj, d)
                    if side >= 0:
                        front += 1
                    if side <= 0:
                        back += 1
                if front and back and (front < len(fragments) or back < len(fragments)):
                    score = abs(front - back) + (front + back - len(fragments)) * 2
                    if best is None or score < best[0]:
                        best = (score, ni, nj, d)
        if best is None:
            # (overlapping fragments no line separates: the first surface)
            return SIGN | fragments[0][0]
        _, ni, nj, d = best
        front, back = [], []
        for s, q in fragments:
            side = side_2d(q, ni, nj, d)
            if side > 0:
                front.append((s, q))
            elif side < 0:
                back.append((s, q))
            else:
                a = clip_2d(q, ni, nj, d, True)
                b = clip_2d(q, ni, nj, d, False)
                if a:
                    front.append((s, a))
                if b:
                    back.append((s, b))
        node = len(self.bsp.nodes2d)
        self.bsp.nodes2d.append([ni, nj, d, NONE, NONE])
        # (an empty side: the other side's surface, never NONE -- a point the
        # game asks about is always on a fragment, give or take rounding)
        back_child = self.build_2d(back, axis, sign) if back else SIGN | front[0][0]
        front_child = self.build_2d(front, axis, sign) if front else SIGN | back[0][0]
        self.bsp.nodes2d[node][3] = back_child
        self.bsp.nodes2d[node][4] = front_child
        return node

    def references(self):
        refs = {}
        for node, items in enumerate(self.on_node):
            p, back, front = self.bsp.nodes[node]
            for surface, points, facing in items:
                s = self.surfaces[surface]
                if s.flags & SURFACE_TWO_SIDED:
                    self.push(front, p, False, surface, points, refs)
                    self.push(back, p, True, surface, points, refs)
                elif facing:
                    self.push(front, p, False, surface, points, refs)
                else:
                    self.push(back, p, True, surface, points, refs)
        # (points inside each leaf: its fragments' middles, a little off
        # their plane on the leaf's side -- campaign.py finds the leaf's
        # cluster with them)
        self.leaf_points = [[] for _ in self.bsp.leaves]
        for leaf in range(len(self.bsp.leaves)):
            planes = refs.get(leaf, {})
            first = len(self.bsp.references)
            flags = 0
            for (p, behind), fragments in sorted(planes.items()):
                n = self.planes[p]
                side = -1.0 if behind else 1.0
                normal = (n[0] * side, n[1] * side, n[2] * side)
                for _, pts in fragments[:3]:
                    middle = [sum(q[k] for q in pts) / len(pts) for k in range(3)]
                    self.leaf_points[leaf].append((middle, normal))
                    # (and its corners, drawn in towards its middle)
                    for q in pts[:4]:
                        self.leaf_points[leaf].append(
                            ([middle[k] + (q[k] - middle[k]) * 0.8 for k in range(3)], normal))
                axis = projection_axis(n)
                sign = (n[axis] > 0) != behind
                u, v = PROJECTION[axis][1 if sign else 0]
                flat = [(s, [(q[u], q[v]) for q in pts]) for s, pts in fragments]
                for s, _ in fragments:
                    if self.surfaces[s].flags & SURFACE_TWO_SIDED:
                        flags |= LEAF_TWO_SIDED
                root = self.build_2d(flat, axis, sign)
                self.bsp.references.append([(SIGN | p) if behind else p, root])
            self.bsp.leaves[leaf] = [flags, len(self.bsp.references) - first, first]

    # ---------- surfaces, edges, vertices

    def topology(self):
        """the winged edges of the mesh's polygons. Every edge has a surface
        on each side (collision_features.c reads both unchecked): one-sided
        surfaces are joined first (they close up by themselves), then the
        two-sided sheets -- each face of a sheet to its neighbours, never to
        its own back face (the same corners the other way round) while
        another is there; edges left with one surface are joined with one
        going the other way (a sheet's free border: its two faces)"""
        bsp = self.bsp
        bsp.vertices = [[v[0], v[1], v[2], NONE] for v in self.vertices]
        bsp.edges = []
        bsp.surfaces = [None] * len(self.surfaces)
        by_direction = {}  # (a, b) as stored -> edges with surface1 free
        corner_sets = [frozenset(s.corners) for s in self.surfaces]
        rings = [None] * len(self.surfaces)
        order = sorted(range(len(self.surfaces)), key=lambda i: bool(self.surfaces[i].flags & SURFACE_TWO_SIDED))
        for index in order:
            s = self.surfaces[index]
            corners = s.corners
            count = len(corners)
            ring = []
            for i in range(count):
                a, b = corners[i], corners[(i + 1) % count]
                candidates = by_direction.get((b, a), [])
                chosen = None
                for e in candidates:
                    owner = bsp.edges[e][4]
                    if owner != index and corner_sets[owner] != corner_sets[index]:
                        chosen = e
                        break
                if chosen is not None:
                    candidates.remove(chosen)
                    bsp.edges[chosen][5] = index
                    ring.append([chosen, False])
                else:
                    e = len(bsp.edges)
                    bsp.edges.append([a, b, NONE, NONE, index, NONE])
                    by_direction.setdefault((a, b), []).append(e)
                    ring.append([e, True])
            rings[index] = ring
        # edges still with one surface: joined in pairs going opposite ways
        dead = set()
        for (a, b), edges in list(by_direction.items()):
            for e in list(edges):
                if bsp.edges[e][5] != NONE or e in dead:
                    continue
                others = [f for f in by_direction.get((b, a), []) if f not in dead and bsp.edges[f][5] == NONE and f != e]
                if not others:
                    raise ValueError(f"an open edge {a}-{b} (surface {bsp.edges[e][4]})")
                f = others[0]
                owner = bsp.edges[f][4]
                bsp.edges[e][5] = owner
                dead.add(f)
                for item in rings[owner]:
                    if item[0] == f:
                        item[0], item[1] = e, False
        # renumbered without the dead edges
        renumber = {}
        kept = []
        for e, edge in enumerate(bsp.edges):
            if e not in dead:
                renumber[e] = len(kept)
                kept.append(edge)
        bsp.edges = kept
        for index, ring in enumerate(rings):
            ring = [[renumber[e], forward] for e, forward in ring]
            count = len(ring)
            for i, (e, forward) in enumerate(ring):
                following = ring[(i + 1) % count][0]
                bsp.edges[e][2 if forward else 3] = following
                v = bsp.edges[e][0] if forward else bsp.edges[e][1]
                if bsp.vertices[v][3] == NONE:
                    bsp.vertices[v][3] = e
            s = self.surfaces[index]
            designator = (SIGN | s.plane) if s.flipped else s.plane
            bsp.surfaces[index] = [designator, ring[0][0], s.flags, s.breakable, s.material]

    def run(self, topology_from=None):
        """the BSP; with topology_from (the BSP the mesh came from, all its
        surfaces kept in order), its surfaces, edges and vertices as they
        were"""
        fragments = []
        for index, s in enumerate(self.surfaces):
            fragments.append(Fragment(index, [self.vertices[c] for c in s.corners]))
        self.build(fragments)
        self.references()
        if topology_from is not None:
            self.bsp.surfaces = [list(x) for x in topology_from.surfaces]
            self.bsp.edges = [list(x) for x in topology_from.edges]
            self.bsp.vertices = [list(x) for x in topology_from.vertices]
        else:
            self.topology()
        self.bsp.planes = [list(p) for p in self.planes]
        return self.bsp


def side_2d(poly, ni, nj, d):
    front = back = False
    for x, y in poly:
        v = ni * x + nj * y - d
        if v > 1e-5:
            front = True
        elif v < -1e-5:
            back = True
    if front and back:
        return 0
    return 1 if front else -1 if back else 1


def clip_2d(poly, ni, nj, d, keep_front):
    out = []
    count = len(poly)
    values = [ni * x + nj * y - d for x, y in poly]
    for i in range(count):
        a, b = poly[i], poly[(i + 1) % count]
        va, vb = values[i], values[(i + 1) % count]
        a_in = va >= -1e-5 if keep_front else va <= 1e-5
        if a_in:
            out.append(a)
        if (va > 1e-5 and vb < -1e-5) or (va < -1e-5 and vb > 1e-5):
            t = va / (va - vb)
            out.append((a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t))
    return out if len(out) >= 3 else None


def mesh_from_bsp(bsp, pathfinding=None, drop_breakable=True):
    """a collision BSP's own polygons as a mesh (vertices, planes, surfaces)"""
    vertices = [tuple(v[:3]) for v in bsp.vertices]
    planes = [tuple(p) for p in bsp.planes]
    surfaces = []
    for index, (designator, _, flags, breakable, material) in enumerate(bsp.surfaces):
        if drop_breakable and flags & 8:
            continue
        corners = bsp.surface_polygon(index)
        flipped = (designator & SIGN) != 0 if designator >= 0 else True
        surfaces.append(Surface(corners, designator & 0x7FFFFFFF, flipped, flags & ~8, 0xFF, material,
                                pathfinding[index] if pathfinding else 0))
    return vertices, planes, surfaces
