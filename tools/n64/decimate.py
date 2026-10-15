"""Simplifying a skinned mesh for the N64 (quadric error edge collapse).

Halo's first-person models are 1,000 to 2,300 triangles each, made for the
Xbox; the N64 draws them in every view. This collapses edges by the error
they add (Garland and Heckbert's quadrics), with constraints that keep the
result drawable as it was:

- vertices are welded by position within their node first, so a texture
  seam (two vertices, one position) moves as one and never opens a crack;
- with max_deviation, a collapse that moves the surface further than that
  off any of the moved vertex's triangles' planes is refused: the budget is
  a most, the shape comes first (a budget alone flattened the front of the
  first-person pistol's slide, its muzzle);
- with cross_nodes, an edge between vertices of two nodes collapses too,
  at CROSS_NODE_COST times its error (after the edges within a node): the
  moved vertex's triangles take the other node's vertex, as rigid skinning
  stretches a joint anyway. Not for first-person models: it melted the
  hands' fingers (36 nodes) together;
- a vertex moves onto its neighbour's position (half-edge collapse), and
  each corner of a remaining triangle takes, from the vertices at that
  position, the one its own vertex already shares a triangle with: the same
  island of the texture (a collapse where some corner has none is
  refused, so a vertex on a texture seam moves only along the seam);
- a collapse that would turn a triangle more than 60 degrees is refused (it
  would fold over its neighbours, and a folded triangle faces away), as is
  one that would join the mesh to itself (g and h may share no neighbour
  but the far corners of the triangles on their edge);
- a vertex on an open edge (where the part meets another part, or the
  mesh simply ends) or where two nodes meet (the same position in two
  groups) never moves: the meshes would open there. A model's parts are
  simplified one by one, so their shared edges must stay where they are.

decimate(vertices, triangles, target, cross_nodes) takes vertices as (node, position,
uv, ...) tuples and triangles as vertex index triples, and returns the
simplified triangles (still indices into the same vertices).
"""

import heapq
import math


def _plane_quadric(a, b, c):
    ux, uy, uz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    vx, vy, vz = c[0] - a[0], c[1] - a[1], c[2] - a[2]
    nx, ny, nz = uy * vz - uz * vy, uz * vx - ux * vz, ux * vy - uy * vx
    length = math.sqrt(nx * nx + ny * ny + nz * nz)
    if length < 1e-12:
        return None
    area = length * 0.5
    nx, ny, nz = nx / length, ny / length, nz / length
    d = -(nx * a[0] + ny * a[1] + nz * a[2])
    # the symmetric 4x4 as its 10 distinct terms, weighted by area
    return [area * v for v in (nx * nx, nx * ny, nx * nz, nx * d, ny * ny, ny * nz, ny * d, nz * nz, nz * d, d * d)]


def _error(q, p):
    x, y, z = p
    return (q[0] * x * x + 2 * q[1] * x * y + 2 * q[2] * x * z + 2 * q[3] * x
            + q[4] * y * y + 2 * q[5] * y * z + 2 * q[6] * y
            + q[7] * z * z + 2 * q[8] * z + q[9])


def _normal(a, b, c):
    ux, uy, uz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    vx, vy, vz = c[0] - a[0], c[1] - a[1], c[2] - a[2]
    return (uy * vz - uz * vy, uz * vx - ux * vz, ux * vy - uy * vx)


CROSS_NODE_COST = 8.0


def decimate(vertices, triangles, target, cross_nodes=False, max_deviation=None):
    if len(triangles) <= target:
        return list(triangles)
    # weld by node and position
    group_of = []
    groups = {}
    members = []
    positions = []
    nodes = []
    for v in vertices:
        key = (v[0], round(v[1][0], 5), round(v[1][1], 5), round(v[1][2], 5))
        g = groups.get(key)
        if g is None:
            g = len(members)
            groups[key] = g
            members.append([])
            positions.append(v[1])
            nodes.append(v[0])
        group_of.append(g)
        members[g].append(len(group_of) - 1)

    tris = []  # [corner vertices, corner groups, alive]
    by_group = [set() for _ in members]
    quadric = [[0.0] * 10 for _ in members]
    for a, b, c in triangles:
        ga, gb, gc = group_of[a], group_of[b], group_of[c]
        if ga == gb or gb == gc or ga == gc:
            continue
        t = len(tris)
        tris.append([[a, b, c], [ga, gb, gc], True])
        q = _plane_quadric(positions[ga], positions[gb], positions[gc])
        for g in (ga, gb, gc):
            by_group[g].add(t)
            if q:
                quadric[g] = [x + y for x, y in zip(quadric[g], q)]
    alive_count = len(tris)
    # vertices that must not move: on open edges, and where nodes meet
    edge_use = {}
    for _, corners, _ in tris:
        for x, y in ((corners[0], corners[1]), (corners[1], corners[2]), (corners[2], corners[0])):
            key = (x, y) if x < y else (y, x)
            edge_use[key] = edge_use.get(key, 0) + 1
    locked = set()
    for (x, y), n in edge_use.items():
        if n == 1:
            locked.add(x)
            locked.add(y)
    groups_at = {}
    for g, position in enumerate(positions):
        groups_at.setdefault(tuple(round(c, 5) for c in position), []).append(g)
    for shared in groups_at.values():
        if len(shared) > 1:
            locked.update(shared)
    # which vertices share a triangle (for a corner's replacement)
    adjacent = {}
    for corners, _, _ in tris:
        for x in corners:
            adjacent.setdefault(x, set()).update(corners)
    alive_group = [True] * len(members)
    version = [0] * len(members)
    heap = []

    def push_edges(g):
        neighbours = set()
        for t in by_group[g]:
            for h in tris[t][1]:
                if h != g and (cross_nodes or nodes[h] == nodes[g]):
                    neighbours.add(h)
        for h in neighbours:
            q = [x + y for x, y in zip(quadric[g], quadric[h])]
            k = 1.0 if nodes[h] == nodes[g] else CROSS_NODE_COST
            # (g moves onto h, and h onto g, unless the mover is locked)
            if g not in locked:
                heapq.heappush(heap, (k * _error(q, positions[h]), g, h, version[g], version[h]))
            if h not in locked:
                heapq.heappush(heap, (k * _error(q, positions[g]), h, g, version[h], version[g]))

    for g in range(len(members)):
        push_edges(g)

    def replacement(vertex, target_group):
        """the vertex of target_group that vertex shares a triangle with
        (nearest in texture space if several), or None"""
        best, best_d = None, None
        uv = vertices[vertex][2]
        for v in members[target_group]:
            if v not in adjacent.get(vertex, ()):
                continue
            d = (vertices[v][2][0] - uv[0]) ** 2 + (vertices[v][2][1] - uv[1]) ** 2
            if best is None or d < best_d:
                best, best_d = v, d
        return best

    while alive_count > target and heap:
        cost, g, h, vg, vh = heapq.heappop(heap)
        if not alive_group[g] or not alive_group[h] or version[g] != vg or version[h] != vh:
            continue
        # refuse a collapse that turns a remaining triangle too far, or moves
        # the surface too far off its planes
        flips = False
        move = [positions[h][k] - positions[g][k] for k in range(3)]
        # (max_deviation may be a function of the position: a level's, by
        # how near the players go)
        deviation = (min(max_deviation(positions[g]), max_deviation(positions[h])) if callable(max_deviation)
                     else max_deviation)
        for t in by_group[g]:
            corners = tris[t][1]
            before = _normal(*(positions[c] for c in corners))
            if deviation is not None:
                length = math.sqrt(before[0] ** 2 + before[1] ** 2 + before[2] ** 2)
                if length > 1e-20 and abs(before[0] * move[0] + before[1] * move[1] + before[2] * move[2]) > deviation * length:
                    flips = True
                    break
            if h in corners:
                continue
            after = _normal(*(positions[h] if c == g else positions[c] for c in corners))
            dot = before[0] * after[0] + before[1] * after[1] + before[2] * after[2]
            lengths = math.sqrt((before[0] ** 2 + before[1] ** 2 + before[2] ** 2) *
                                (after[0] ** 2 + after[1] ** 2 + after[2] ** 2))
            if lengths < 1e-20 or dot < 0.5 * lengths:
                flips = True
                break
        if flips:
            continue
        # the link condition: the only neighbours g and h share are the far
        # corners of the triangles on their edge
        shared_triangles = by_group[g] & by_group[h]
        far = {c for t in shared_triangles for c in tris[t][1] if c != g and c != h}
        g_neighbours = {c for t in by_group[g] for c in tris[t][1]} - {g}
        h_neighbours = {c for t in by_group[h] for c in tris[t][1]} - {h}
        if (g_neighbours & h_neighbours) - far:
            continue
        # every moved corner must find h in its own texture island
        replacements = {}
        for t in by_group[g]:
            tri = tris[t]
            if h in tri[1]:
                continue
            for i in range(3):
                if tri[1][i] == g and tri[0][i] not in replacements:
                    replacements[tri[0][i]] = replacement(tri[0][i], h)
        if any(r is None for r in replacements.values()):
            continue
        for t in list(by_group[g]):
            tri = tris[t]
            if h in tri[1]:
                tri[2] = False
                alive_count -= 1
                for c in tri[1]:
                    by_group[c].discard(t)
                continue
            for i in range(3):
                if tri[1][i] == g:
                    tri[1][i] = h
                    tri[0][i] = replacements[tri[0][i]]
            by_group[h].add(t)
            # the triangle's vertices now share it
            for x in tri[0]:
                adjacent.setdefault(x, set()).update(tri[0])
        by_group[g] = set()
        alive_group[g] = False
        quadric[h] = [x + y for x, y in zip(quadric[h], quadric[g])]
        version[h] += 1
        push_edges(h)

    return [tuple(t[0]) for t in tris if t[2]]
