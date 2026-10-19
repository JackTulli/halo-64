"""The campaign level editor's data (tools/n64/campaign_editor.html): a
campaign level's ground (its collision BSP's upward surfaces), its player
starts, the placements and the encounters' squads the N64 keeps or leaves
out (tools/n64/campaign.py), as JSON for the page to load:

    python3 -m tools.n64.campaign_editor build/xbox/disc/maps/a30.map -o a30-level.json --html a30-editor.html

(--html: the page with the level in it, to open by itself.)

The file is made from your own map: keep it to yourself (not in the
repository). The page saves the edits to campaign_edits/<map>.json, which
the campaign's conversion reads (campaign.py campaign_edits)."""

import argparse
import json
import struct
import sys
from pathlib import Path

from .campaign import (PLACEMENTS, DROPPED_PLACEMENTS, CAMPAIGN_VEHICLES, Placements, actor_kinds, campaign_closure,
                       encounter_table)
from .repack import Repacker
from .xboxmap import XboxMap

# the kinds the page shows (sound scenery has nothing to see)
SHOWN = ("scenery", "bipeds", "equipment", "weapons", "machines", "controls", "light fixtures", "vehicles")


def ground_mesh(space, open_space):
    """the collision BSP's surfaces that face up into open space (no
    invisible ones, none of the closed shell around the level: a30's lid
    of 10,000-square-unit triangles 118 units up hid the whole valley):
    its vertices (flat x, y, z) and triangles (flat indices, fans);
    open_space(point) says whether a point is in the level's open space"""
    vertices = []
    for x, y, z, _ in space.vertices:
        vertices += [round(x, 2), round(y, 2), round(z, 2)]
    triangles = []
    for s, (plane, first, flags, _, _) in enumerate(space.surfaces):
        if flags & 2:
            continue
        index = plane & 0x7FFFFFFF
        if not 0 <= index < len(space.planes):
            continue
        k = space.planes[index][2] * (-1.0 if plane < 0 else 1.0)
        if k < 0.05:
            continue
        polygon = []
        e = first
        for _ in range(64):
            edge = space.edges[e]
            reverse = edge[5] == s
            polygon.append(edge[1] if reverse else edge[0])
            e = edge[3] if reverse else edge[2]
            if e == first:
                break
        corners = [space.vertices[v] for v in polygon]
        centre = [sum(c[k] for c in corners) / len(corners) for k in range(3)]
        if not open_space((centre[0], centre[1], centre[2] + 0.3)):
            continue
        for i in range(1, len(polygon) - 1):
            triangles += [polygon[0], polygon[i], polygon[i + 1]]
    return vertices, triangles


def export(map_path, output, html=None):
    xmap = XboxMap(map_path)
    xmap.load_bsp(0)
    defs = Repacker(xmap, None).defs
    closure = campaign_closure(xmap, defs, (0,))
    # (the converter's own choices, before the edits: the page shows those)
    placements = Placements(xmap, defs, (0,), apply_edits=False)
    vertices, triangles = ground_mesh(placements.spaces[0], placements.open)
    used = sorted(set(triangles))
    xs = [vertices[3 * i] for i in used]
    ys = [vertices[3 * i + 1] for i in used]
    zs = sorted(vertices[3 * i + 2] for i in used)

    sf = {f.name: (o, f) for o, f in defs.fields("Scenario")}
    o, f = sf["player starting locations"]
    count, pointer, _ = struct.unpack("<iII", xmap.read(xmap.scenario.address + o, 12))
    fo = {g.name: off for off, g in defs.fields(f.struct)}
    starts = []
    for i in range(max(count, 0)):
        a = pointer + i * defs.size(f.struct)
        x, y, z = struct.unpack("<3f", xmap.read(a + fo["position"], 12))
        facing = struct.unpack("<f", xmap.read(a + fo["facing"], 4))[0] if "facing" in fo else 0.0
        starts.append([round(x, 2), round(y, 2), round(z, 2), round(facing, 3)])

    items = []
    for pid, address in placements.by_id.items():
        kind, name, index, position, not_placed = placements.all[address]
        if kind not in SHOWN or any(name.startswith(d) for d in DROPPED_PLACEMENTS):
            continue
        kept = address in placements.kept
        if not kept and not placements.inside(position):
            continue
        keepable = kind != "vehicles" or name in CAMPAIGN_VEHICLES
        shown = placements.moved.get(address, position)
        items.append({"id": pid, "kind": kind, "tag": name, "name": name.split("\\")[-1],
                      "position": [round(c, 2) for c in shown], "kept": kept, "keepable": keepable,
                      "moved": address in placements.moved})

    report = []
    encounter_table(xmap, defs, closure.kept, closure.placements.inside, report=report, apply_edits=False)
    squads = [{"key": r["key"], "kind": r["kind"].split("\\")[-1], "team": r["team"], "normal": r["normal"],
               "insane": r["insane"], "kept": r["kept"], "reason": r["reason"],
               "locations": [[round(c, 3 if i == 3 else 2) for i, c in enumerate(p)] for p in r.get("locations", [])]}
              for r in report]

    data = {"map": xmap.name, "version": 1,
            "bounds": [min(xs), min(ys), zs[0], max(xs), max(ys), zs[-1]],
            # (the heights the page colours across: the ground's 1st to 99th
            # percentile)
            "zrange": [zs[len(zs) // 100], zs[len(zs) * 99 // 100]],
            "vertices": vertices, "triangles": triangles, "starts": starts,
            "placements": items, "squads": squads,
            # (what a squad added in the page can be made of)
            "kinds": [{"palette": i, "name": variant.split("\\")[-1], "unit": unit.split("\\")[-1], "team": team}
                      for i, variant, unit, team in actor_kinds(xmap, defs, closure.kept)]}
    text = json.dumps(data, separators=(",", ":"))
    Path(output).write_text(text, encoding="utf-8")
    if html:
        page = (Path(__file__).with_name("campaign_editor.html")).read_text(encoding="utf-8")
        marker = '<script>\n"use strict";'
        if marker not in page:
            raise ValueError("campaign_editor.html: no main script to put the level before")
        # (the level as a script of its own before the page's; "</" escaped
        # so no string in it ends the script)
        page = page.replace(marker, "<script>window.EMBEDDED_LEVEL = " + text.replace("</", "<\\/") +
                            ";</script>\n" + marker, 1)
        Path(html).write_text(page, encoding="utf-8")
        print(f"{xmap.name}: the editor with the level in it -> {html}")
    print(f"{xmap.name}: {len(triangles) // 3} ground triangles, {len(items)} placements "
          f"({sum(1 for i in items if i['kept'])} kept), {len(squads)} squads "
          f"({sum(1 for s in squads if s['kept'])} kept) -> {output}")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("map", type=Path, help="the campaign level's Xbox map (a30.map)")
    parser.add_argument("-o", "--output", type=Path, required=True)
    parser.add_argument("--html", type=Path, help="also the editor page with the level in it")
    args = parser.parse_args()
    export(args.map, args.output, args.html)


if __name__ == "__main__":
    sys.exit(main())
