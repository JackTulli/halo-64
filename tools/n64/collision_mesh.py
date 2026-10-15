"""A simpler collision mesh for a campaign level's structure BSP.

The map's collision surfaces (collision.py) form a closed shell: solid
behind every one-sided surface. simplify() collapses its edges by the
error they add (Garland and Heckbert's quadrics) while it stays closed and
the surface moves no further than a tolerance: a few centimetres where the
players and the AI go (play_points: the scenario's placements, starting
locations, firing positions), more away from them (far cliffs, the
mountains around). Two-sided sheets (fences, grates) and the vertices they
share are kept as they are; breakable surfaces (glass) are left out.

Then adjacent triangles on one plane with the same material become one
polygon (at most 8 corners, convex), and every polygon gets its plane and
its pathfinding flags (walkable where its slope is under 45 degrees, as the
map's are).
"""

import heapq
import math

from .collision import SURFACE_BREAKABLE, SURFACE_TWO_SIDED
from .collision_build import Surface

WALKABLE = 0x40
# the map's walkable surfaces all face up at least this much
WALKABLE_Z = 0.707
# a collapse may turn a remaining triangle at most this far (cosine)
FLIP_COSINE = 0.5
# coplanar for merging: normals this close, corners this near the plane
MERGE_COSINE = 0.99995
MERGE_DISTANCE = 0.002


def sub(a, b):
    return (a[0] - b[0], a[1] - b[1], a[2] - b[2])


def cross(a, b):
    return (a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0])


def dot(a, b):
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]


def normalise(v):
    length = math.sqrt(dot(v, v))
    return (v[0] / length, v[1] / length, v[2] / length) if length > 1e-20 else (0.0, 0.0, 0.0)


def triangle_quadric(a, b, c):
    n = cross(sub(b, a), sub(c, a))
    length = math.sqrt(dot(n, n))
    if length < 1e-12:
        return None, 0.0
    area = 0.5 * length
    nx, ny, nz = n[0] / length, n[1] / length, n[2] / length
    d = -(nx * a[0] + ny * a[1] + nz * a[2])
    return [area * v for v in (nx * nx, nx * ny, nx * nz, nx * d, ny * ny, ny * nz, ny * d, nz * nz, nz * d,
                               d * d)], area


def quadric_error(q, p):
    x, y, z = p
    return (q[0] * x * x + 2 * q[1] * x * y + 2 * q[2] * x * z + 2 * q[3] * x
            + q[4] * y * y + 2 * q[5] * y * z + 2 * q[6] * y
            + q[7] * z * z + 2 * q[8] * z + q[9])


class PlaySpace:
    """how near a point is to where the game is played: the distance to the
    nearest play point, by a grid of cells"""

    def __init__(self, points, cell=8.0):
        self.cell = cell
        self.grid = {}
        for p in points:
            key = (int(math.floor(p[0] / cell)), int(math.floor(p[1] / cell)), int(math.floor(p[2] / cell)))
            self.grid.setdefault(key, []).append(p)

    def distance(self, p, limit=64.0):
        cell = self.cell
        cx, cy, cz = (int(math.floor(p[k] / cell)) for k in range(3))
        best = limit * limit
        reach = int(math.ceil(limit / cell))
        for r in range(reach + 1):
            # (rings of cells outward: stop once a ring is past the best)
            if (r - 1) * cell > math.sqrt(best):
                break
            for dx in range(-r, r + 1):
                for dy in range(-r, r + 1):
                    for dz in range(-r, r + 1):
                        if max(abs(dx), abs(dy), abs(dz)) != r:
                            continue
                        for q in self.grid.get((cx + dx, cy + dy, cz + dz), ()):
                            d = (q[0] - p[0]) ** 2 + (q[1] - p[1]) ** 2 + (q[2] - p[2]) ** 2
                            if d < best:
                                best = d
        return math.sqrt(best)


def tolerance_by_distance(near=0.03, far=0.4, near_distance=16.0, far_distance=64.0):
    def tolerance(distance):
        if distance <= near_distance:
            return near
        if distance >= far_distance:
            return far
        t = (distance - near_distance) / (far_distance - near_distance)
        return near + (far - near) * t
    return tolerance


class Mesh:
    """the one-sided triangles (corners, material, source surface), the
    two-sided sheets (polygons, kept), positions"""

    def __init__(self):
        self.positions = []
        self.triangles = []   # [a, b, c, material, flags]
        self.sheets = []      # (corners, material, flags)


def extract(bsp):
    """the mesh of a map's collision BSP: positions welded, polygons fanned
    into triangles, sheets apart"""
    mesh = Mesh()
    index_of = {}
    remap = []
    for v in bsp.vertices:
        key = (round(v[0], 5), round(v[1], 5), round(v[2], 5))
        if key not in index_of:
            index_of[key] = len(mesh.positions)
            mesh.positions.append((v[0], v[1], v[2]))
        remap.append(index_of[key])
    for s, (designator, _, flags, breakable, material) in enumerate(bsp.surfaces):
        if flags & SURFACE_BREAKABLE:
            continue
        corners = [remap[v] for v in bsp.surface_polygon(s)]
        # (welding may have joined two corners of one polygon)
        cleaned = [c for i, c in enumerate(corners) if c != corners[i - 1]]
        if len(cleaned) < 3:
            continue
        if flags & SURFACE_TWO_SIDED:
            mesh.sheets.append((cleaned, material, flags))
            continue
        for i in range(1, len(cleaned) - 1):
            mesh.triangles.append([cleaned[0], cleaned[i], cleaned[i + 1], material, flags])
    return mesh


def simplify(mesh, tolerance_at, stats=None):
    """collapses the one-sided triangles' edges while the error stays under
    tolerance_at(position) (world units); returns the surviving triangles"""
    positions = mesh.positions
    tris = [list(t) for t in mesh.triangles]
    alive = [True] * len(tris)
    by_vertex = [set() for _ in positions]
    for t, tri in enumerate(tris):
        for v in tri[:3]:
            by_vertex[v].add(t)
    quadric = [[0.0] * 10 for _ in positions]
    area = [0.0] * len(positions)
    for tri in tris:
        q, a = triangle_quadric(*(positions[v] for v in tri[:3]))
        if q is None:
            continue
        for v in tri[:3]:
            quadric[v] = [x + y for x, y in zip(quadric[v], q)]
            area[v] += a
    # vertices that never move: the sheets', and those on an edge that is
    # not shared by exactly two triangles going opposite ways
    locked = set()
    for corners, _, _ in mesh.sheets:
        locked.update(corners)
    directed = {}
    for tri in tris:
        a, b, c = tri[:3]
        for x, y in ((a, b), (b, c), (c, a)):
            directed[(x, y)] = directed.get((x, y), 0) + 1
    for (x, y), n in directed.items():
        if n != 1 or directed.get((y, x), 0) != 1:
            locked.add(x)
            locked.add(y)
    tolerance = [tolerance_at(p) for p in positions]
    version = [0] * len(positions)
    heap = []

    def neighbours(g):
        out = set()
        for t in by_vertex[g]:
            out.update(tris[t][:3])
        out.discard(g)
        return out

    def push(g):
        for h in neighbours(g):
            for mover, target in ((g, h), (h, g)):
                if mover in locked:
                    continue
                q = [x + y for x, y in zip(quadric[mover], quadric[target])]
                cost = quadric_error(q, positions[target]) / max(area[mover] + area[target], 1e-9)
                heapq.heappush(heap, (cost, mover, target, version[mover], version[target]))

    for g in range(len(positions)):
        if by_vertex[g] and g not in locked:
            push(g)
    collapses = 0
    while heap:
        cost, g, h, vg, vh = heapq.heappop(heap)
        if version[g] != vg or version[h] != vh or not by_vertex[g] or g in locked:
            continue
        limit = min(tolerance[g], tolerance[h])
        if cost > limit * limit:
            continue
        pg, ph = positions[g], positions[h]
        move = sub(ph, pg)
        shared = by_vertex[g] & by_vertex[h]
        if len(shared) != 2:
            continue
        # the link condition: g and h share no neighbour but the far
        # corners of the two triangles on their edge
        far = {c for t in shared for c in tris[t][:3]} - {g, h}
        if (neighbours(g) & neighbours(h)) - far:
            continue
        ok = True
        for t in by_vertex[g]:
            if t in shared:
                continue
            corners = tris[t][:3]
            before = cross(sub(positions[corners[1]], positions[corners[0]]),
                           sub(positions[corners[2]], positions[corners[0]]))
            lb = math.sqrt(dot(before, before))
            if lb < 1e-12:
                ok = False
                break
            # (the moved corner stays near the triangle's old plane)
            if abs(dot(before, move)) > limit * lb:
                ok = False
                break
            moved = [ph if c == g else positions[c] for c in corners]
            after = cross(sub(moved[1], moved[0]), sub(moved[2], moved[0]))
            la = math.sqrt(dot(after, after))
            if la < 1e-9 or dot(before, after) < FLIP_COSINE * lb * la:
                ok = False
                break
        if not ok:
            continue
        # the collapse
        for t in shared:
            alive[t] = False
            for c in tris[t][:3]:
                if c != g:
                    by_vertex[c].discard(t)
        for t in by_vertex[g] - shared:
            tri = tris[t]
            for k in range(3):
                if tri[k] == g:
                    tri[k] = h
            by_vertex[h].add(t)
        by_vertex[g] = set()
        quadric[h] = [x + y for x, y in zip(quadric[h], quadric[g])]
        area[h] += area[g]
        version[h] += 1
        version[g] += 1
        collapses += 1
        push(h)
    if stats is not None:
        stats["collapses"] = collapses
        stats["locked"] = len(locked)
    return [tris[t] for t in range(len(tris)) if alive[t]]


def plane_of(points):
    n = (0.0, 0.0, 0.0)
    count = len(points)
    for i in range(count):
        a, b = points[i], points[(i + 1) % count]
        n = (n[0] + (a[1] - b[1]) * (a[2] + b[2]), n[1] + (a[2] - b[2]) * (a[0] + b[0]),
             n[2] + (a[0] - b[0]) * (a[1] + b[1]))
    n = normalise(n)
    centre = [sum(p[k] for p in points) / count for k in range(3)]
    return (n[0], n[1], n[2], dot(n, centre))


def convex(points, normal):
    count = len(points)
    for i in range(count):
        a, b, c = points[i - 1], points[i], points[(i + 1) % count]
        if dot(cross(sub(b, a), sub(c, b)), normal) < -1e-7:
            return False
    return True


def merge_coplanar(positions, triangles):
    """adjacent triangles on one plane, of one material and flags, joined
    into convex polygons of at most 8 corners: [(corners, material, flags)]"""
    polys = [{"corners": list(t[:3]), "material": t[3], "flags": t[4], "alive": True} for t in triangles]
    planes = [plane_of([positions[c] for c in p["corners"]]) for p in polys]
    edge_owner = {}
    for i, p in enumerate(polys):
        c = p["corners"]
        for k in range(len(c)):
            edge_owner[(c[k], c[(k + 1) % len(c)])] = i
    changed = True
    while changed:
        changed = False
        for i, p in enumerate(polys):
            if not p["alive"]:
                continue
            c = p["corners"]
            for k in range(len(c)):
                a, b = c[k], c[(k + 1) % len(c)]
                j = edge_owner.get((b, a))
                if j is None or j == i or not polys[j]["alive"]:
                    continue
                q = polys[j]
                if q["material"] != p["material"] or q["flags"] != p["flags"]:
                    continue
                if dot(planes[i][:3], planes[j][:3]) < MERGE_COSINE:
                    continue
                # the joined ring: p's corners from b round to a, then q's
                # from a round to b, without the shared edge
                qc = q["corners"]
                m = qc.index(a)
                ring_q = [qc[(m + r) % len(qc)] for r in range(len(qc))]  # a ... b
                if ring_q[-1] != b:
                    continue
                kb = (k + 1) % len(c)
                ring_p = [c[(kb + r) % len(c)] for r in range(len(c))]  # b ... a
                ring = ring_p[:-1] + ring_q[:-1]
                if len(ring) > 8 or len(set(ring)) != len(ring):
                    continue
                points = [positions[x] for x in ring]
                plane = plane_of(points)
                if any(abs(dot(plane[:3], x) - plane[3]) > MERGE_DISTANCE for x in points):
                    continue
                if not convex(points, plane[:3]):
                    continue
                # (collinear corners dropped later would leave T-junctions:
                # keep every corner)
                q["alive"] = False
                p["corners"] = ring
                planes[i] = plane
                for kk in range(len(ring)):
                    edge_owner[(ring[kk], ring[(kk + 1) % len(ring)])] = i
                changed = True
                break
    return [(p["corners"], p["material"], p["flags"]) for p in polys if p["alive"]]


def surfaces_for(positions, polygons, sheets):
    """vertices, planes and Surfaces (collision_build) for the polygons and
    the sheets: planes shared by coplanar polygons"""
    used = sorted({c for corners, _, _ in polygons for c in corners} | {c for corners, _, _ in sheets for c in corners})
    new_index = {v: i for i, v in enumerate(used)}
    vertices = [positions[v] for v in used]
    planes = []
    plane_index = {}
    surfaces = []

    def find_plane(plane):
        n = plane[:3]
        key = (round(n[0], 4), round(n[1], 4), round(n[2], 4), round(plane[3], 3))
        flipped_key = (round(-n[0], 4), round(-n[1], 4), round(-n[2], 4), round(-plane[3], 3))
        if key in plane_index:
            return plane_index[key], False
        if flipped_key in plane_index:
            return plane_index[flipped_key], True
        plane_index[key] = len(planes)
        planes.append(plane)
        return len(planes) - 1, False

    for corners, material, flags in polygons:
        points = [positions[c] for c in corners]
        plane = plane_of(points)
        p, flipped = find_plane(plane)
        pf = WALKABLE if plane[2] >= WALKABLE_Z else 0
        surfaces.append(Surface([new_index[c] for c in corners], p, flipped, flags, 0xFF, material, pf))
    for corners, material, flags in sheets:
        points = [positions[c] for c in corners]
        plane = plane_of(points)
        p, flipped = find_plane(plane)
        surfaces.append(Surface([new_index[c] for c in corners], p, flipped, flags, 0xFF, material, 0))
    return vertices, planes, surfaces
