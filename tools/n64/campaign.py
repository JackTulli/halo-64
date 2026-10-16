"""Converting a campaign level for the N64 (its own cartridge).

A campaign map holds far more than an N64 can: a30 (Halo) is 15 MB of tag
data and two structure BSPs, against the ~3 MB a multiplayer map's image
gets. The scripts, cutscenes, the AI's own data (actors, encounters' firing
positions, command lists, conversations), recorded animations and the
scripted vehicles are left out: the N64 runs none of them. What is kept is
what the rest reaches (closure): the scenario's placed objects, its player
starting locations and profiles, its trigger volumes, the globals and the
strings and fonts the game looks up by name.

The encounters' squads become the N64's own table instead
(encounter_table): which biped and weapon each actor is, how many by
difficulty, where they start. port/n64/game/n64_campaign.c places them as
the players come near and drives them with the simplified AI.
"""

import struct

from .repack import NONE
from .walk import Walker

# tag groups the campaign leaves out on top of the multiplayer maps' (the
# actors and their variants: the AI is the N64's own)
CAMPAIGN_DROPPED_GROUPS = {b"actr", b"actv"}

# scenario blocks the N64 does not use: the AI's, the scripts', the
# cutscenes', the editor's; the decals (the N64 draws none); the structure
# BSP's detail objects (grass, not drawn) and runtime decals. (The vehicles
# stay: CAMPAIGN_VEHICLES)
CAMPAIGN_DROPPED_BLOCKS = {
    "Scenario.child scenarios[]", "Scenario.predicted resources[]", "Scenario.functions[]",
    "Scenario.comments[]", "Scenario.scavenger hunt objects[]", "Scenario.recorded animations[]",
    "Scenario.decals[]", "Scenario.decal palette[]", "Scenario.detail object collection palette[]",
    "Scenario.actor palette[]", "Scenario.encounters[]", "Scenario.command lists[]",
    "Scenario.ai animation references[]", "Scenario.ai script references[]",
    "Scenario.ai recording references[]", "Scenario.ai conversations[]", "Scenario.scripts[]",
    "Scenario.globals[]", "Scenario.references[]", "Scenario.source files[]",
    "Scenario.cutscene flags[]", "Scenario.cutscene camera points[]", "Scenario.cutscene titles[]",
    "ScenarioStructureBSP.detail objects[]", "ScenarioStructureBSP.runtime decals[]",
}

# the scenario's object placements and their palettes: a palette entry is
# followed only if a placement the N64 keeps uses it (Placements)
PLACEMENTS = {"scenery": "scenery palette", "bipeds": "biped palette", "equipment": "equipment palette",
              "weapons": "weapon palette", "machines": "machine palette", "controls": "control palette",
              "light fixtures": "light fixture palette", "sound scenery": "sound scenery palette",
              "vehicles": "vehicle palette"}

# placements a campaign level leaves out, by their palette entry's tag:
# Halo's particle emitters (smoke over the debris, a waterfall's spray:
# objects with nothing else to them, and Halo's particle systems are not
# made on the N64), the cutscenes' props, and a30's crashed lifepods (their
# insides at the highest level of detail did not fit beside the rest in the
# models' cache: invisible craft the players walked into -- the user: "you
# may as well delete their collision")
DROPPED_PLACEMENTS = ("scenery\\emitters\\", "scenery\\waterfalls\\spray_emitter", "cinematics\\",
                      "scenery\\vehicles\\lifepod_crashed\\")
# vehicles Halo's scripts brought that the N64 places at the start, where
# the scenario has them: a30's warthog (the user: "by the base there should
# be a warthog")
CAMPAIGN_VEHICLES = ("vehicles\\warthog\\warthog",)
# the scenery thinned (the user: "tone down the scenery ... less trees
# rocks", then "further reducing the scenery"): of each family, one
# placement in this many kept (0: none), in the scenario's order -- an
# artist places a clump at a time, so one of each few of a clump stays.
# a30: 115 + 30 pines, 93 large, 34 gigantic and 20 small boulders -> about
# 37 pines, 24 large and 17 gigantic boulders, no small ones
SCENERY_THINNING = (("scenery\\trees\\", 4), ("scenery\\rocks\\boulder_granite_large\\", 4),
                    ("scenery\\rocks\\boulder_granite_gigantic\\", 2),
                    ("scenery\\rocks\\boulder_granite_small\\", 0))
# the Covenant alone (the user: "disable all the marines only covenant"): no
# squad of the players' side, no Marine placed as a biped
CAMPAIGN_ALLIES = False
ALLY_WORDS = ("marine", "crewman", "keyes")
# the campaign's one warthog (the user: "it needs to be by the base"; one
# moved near the start went over a cliff and out of the map): on a map
# listed here every vehicle placement the scenario has is left out and one
# warthog placement is moved to flat ground beside the named squad's
# starting locations -- a30's Marine base (first_marine/base_marine)
CAMPAIGN_WARTHOG_BESIDE = {"a30": ("first_marine", "base_marine")}
# its spot: BASE_WARTHOG_RADII from the squad's middle, its ground within
# BASE_WARTHOG_LEVEL of the squad's (no cliff top or foot), its corners
# within BASE_WARTHOG_FLAT, no ledge (BASE_WARTHOG_LEDGE) within 2.5 units, clear of the kept
# objects and the squad's starting locations by BASE_WARTHOG_CLEARANCE
BASE_WARTHOG_RADII = (2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 12.0, 15.0)
BASE_WARTHOG_LEVEL = 1.5
# (a30 base ground rises up to 0.45 across a warthog; a drop of more than
# 1.2 within 2.5 units of it is a ledge)
BASE_WARTHOG_FLAT = 0.45
BASE_WARTHOG_LEDGE = 1.2
BASE_WARTHOG_CLEARANCE = 2.0
# squads that come in a vehicle the N64 has not (Banshee pilots, a jeep's
# crew, a dropship's) are left out: they stood where their vehicle would
# be -- a30's banshee pilots outside the map (the user: "you have enemies
# spawning outside the map")
DROPPED_SQUAD_WORDS = ("banshee", "pilot", "jeep", "dropship", "driver", "gunner", "passenger", "pelican",
                       "wraith")
# placements the N64 makes as the players come near and deletes as they go
# (n64_campaign.c), not at the start: a30's ~300 trees and boulders took
# ~230 KB of Halo's 256 KB of object memory, and the encounters' actors (a
# biped ~7 KB) found no room -- "OUT OF OBJECTS", few Covenant
STREAMED_PLACEMENTS = ("scenery", "sound scenery")

# a dialogue's vocalizations that are kept (merge.py's: the pain, scream
# and death sounds the game plays itself, and the lines the simplified AI
# says)
from .merge import DIALOG_VOCALIZATIONS  # noqa: E402

# looked up by name at run time (tag_loaded): strings, fonts, the HUD's
ROOT_GROUPS = {b"ustr", b"str#", b"font", b"hmt ", b"hud#", b"hudg", b"matg"}


def fourcc(text):
    return int.from_bytes(text, "big")


class Placements:
    """the scenario's placements a campaign level keeps: inside the BSPs
    on the cartridge (open space at or a little above the placement, in
    their collision: a30_b's half is never reached), not left out
    (DROPPED_PLACEMENTS), and placed by the scenario itself or a campaign
    vehicle. kept: placement address -> its "not placed" flags as the N64
    has them (the streamed kinds not placed at the start, the vehicles
    placed); palette_use: palette name -> the entries they use"""

    def __init__(self, xmap, defs, bsp_indices):
        from .collision import CollisionBSP
        self.spaces = []
        bo = {f.name: o for o, f in defs.fields("ScenarioStructureBSP")}
        for b in bsp_indices:
            if b < len(xmap.bsps):
                count, pointer, _ = struct.unpack("<iII", xmap.read(xmap.bsps[b][0].address + bo["collision bsp"], 12))
                if count > 0:
                    self.spaces.append(CollisionBSP.read(xmap, pointer))
        fields = {f.name: (o, f) for o, f in defs.fields("Scenario")}
        scenario = xmap.scenario.address
        self.kept = {}
        self.palette_use = {}
        self.dropped = 0
        self.thinned = 0
        # placements moved (address -> position): convert.py's CampaignPolicy
        self.moved = {}
        # every placement read: address -> (kind, palette name, its index,
        # position, its "not placed" flags)
        self.all = {}
        # (CAMPAIGN_WARTHOG_BESIDE: the squad the one warthog stands by)
        self.beside = CAMPAIGN_WARTHOG_BESIDE.get(xmap.name)
        thinning = {}
        for kind, palette in PLACEMENTS.items():
            if kind not in fields or palette not in fields:
                continue
            po_, pf = fields[palette]
            pcount, ppointer, _ = struct.unpack("<iII", xmap.read(scenario + po_, 12))
            psize = defs.size(pf.struct)
            pfo = {g.name: off for off, g in defs.fields(pf.struct)}
            names = []
            for e in range(max(pcount, 0)):
                t = xmap.tag(struct.unpack("<I", xmap.read(ppointer + e * psize + pfo["name"] + 12, 4))[0])
                names.append(t.name if t is not None else "")
            o, f = fields[kind]
            count, pointer, _ = struct.unpack("<iII", xmap.read(scenario + o, 12))
            size = defs.size(f.struct)
            fo = {g.name: off for off, g in defs.fields(f.struct)}
            entries = set()
            for i in range(max(count, 0)):
                address = pointer + i * size
                raw = xmap.read(address, size)
                index = struct.unpack_from("<h", raw, fo["type"])[0]
                # (the "not placed" flags: placed at the start when bit 0 is clear)
                not_placed = struct.unpack_from("<H", raw, fo["not placed"])[0] if "not placed" in fo else 0
                position = struct.unpack_from("<3f", raw, fo["position"])
                if not 0 <= index < len(names):
                    continue
                name = names[index]
                self.all[address] = (kind, name, index, position, not_placed)
                vehicle = kind == "vehicles"
                ally = kind == "bipeds" and not CAMPAIGN_ALLIES and any(w in name.lower() for w in ALLY_WORDS)
                if ally or (any(name.startswith(d) for d in DROPPED_PLACEMENTS) or
                        (vehicle and (name not in CAMPAIGN_VEHICLES or self.beside is not None))
                        or (not vehicle and not_placed & 1) or not self.inside(position)):
                    self.dropped += 1
                    continue
                family = next((rule for rule in SCENERY_THINNING if name.startswith(rule[0])), None)
                if kind == "scenery" and family is not None:
                    seen = thinning[family[0]] = thinning.get(family[0], 0) + 1
                    if family[1] == 0 or (seen - 1) % family[1]:
                        self.dropped += 1
                        self.thinned += 1
                        continue
                if vehicle:
                    not_placed &= ~1
                if kind in STREAMED_PLACEMENTS:
                    not_placed |= 1
                self.kept[address] = not_placed
                entries.add(index)
            self.palette_use[palette] = entries
        self.base_warthog(xmap, defs)

    def base_warthog(self, xmap, defs):
        """the one warthog, by the squad CAMPAIGN_WARTHOG_BESIDE names"""
        import math
        if self.beside is None:
            return
        points = squad_locations(xmap, defs, *self.beside)
        warthogs = [(a, entry) for a, entry in self.all.items()
                    if entry[0] == "vehicles" and entry[1] in CAMPAIGN_VEHICLES]
        if not points or not warthogs:
            print(f"campaign: no warthog by {'/'.join(self.beside)}")
            return
        heights = sorted(point[2] for point in points)
        centre = (sum(point[0] for point in points) / len(points), sum(point[1] for point in points) / len(points),
                  heights[len(heights) // 2])
        obstacles = [entry[3] for a, entry in self.all.items() if a in self.kept and entry[0] != "sound scenery"]
        spot = self.flat_ground_near(centre, obstacles + list(points))
        if spot is None:
            print(f"campaign: no flat ground for a warthog by {'/'.join(self.beside)}")
            return
        address, (kind, name, index, position, not_placed) = warthogs[0]
        self.kept[address] = not_placed & ~1
        self.moved[address] = spot
        self.palette_use.setdefault("vehicle palette", set()).add(index)
        print(f"campaign: the warthog at {tuple(round(c, 1) for c in spot)}, by {'/'.join(self.beside)} "
              f"({math.dist(spot[:2], centre[:2]):.1f} units from its middle)")

    def open(self, point):
        from .collision import NONE as COLLISION_NONE
        return any(space.test_point(point) != COLLISION_NONE for space in self.spaces)

    def ground(self, x, y, top, bottom):
        """the height of the ground under (x, y) between top (open) and
        bottom, or None"""
        if not self.open((x, y, top)):
            return None
        z = top
        while z > bottom:
            z -= 0.05
            if not self.open((x, y, z)):
                return z + 0.05
        return None

    def flat_ground_near(self, centre, obstacles):
        """a warthog's spot near centre (BASE_WARTHOG_RADII ...)"""
        import math
        if not self.spaces:
            return None
        corners = ((0.9, 0.0), (-0.9, 0.0), (0.0, 0.9), (0.0, -0.9))
        for radius in BASE_WARTHOG_RADII:
            for k in range(16):
                angle = 2 * math.pi * k / 16
                x, y = centre[0] + radius * math.cos(angle), centre[1] + radius * math.sin(angle)
                ground = self.ground(x, y, centre[2] + 3.0, centre[2] - 3.0)
                if ground is None or abs(ground - centre[2]) > BASE_WARTHOG_LEVEL:
                    continue
                heights = [self.ground(x + dx, y + dy, ground + 1.0, ground - 1.0) for dx, dy in corners]
                if any(h is None or abs(h - ground) > BASE_WARTHOG_FLAT for h in heights):
                    continue
                ring = [self.ground(x + 2.5 * math.cos(j * math.pi / 4), y + 2.5 * math.sin(j * math.pi / 4),
                                    ground + 1.5, ground - 1.5) for j in range(8)]
                if any(h is None or abs(h - ground) > BASE_WARTHOG_LEDGE for h in ring):
                    continue
                if not all(self.open((x + dx, y + dy, ground + h)) for dx, dy in ((0.0, 0.0),) + corners
                           for h in (0.3, 0.8, 1.5)):
                    continue
                if any(math.dist((x, y), o[:2]) < BASE_WARTHOG_CLEARANCE for o in obstacles):
                    continue
                return (x, y, ground + 0.3)
        return None

    def inside(self, position):
        """a point in (or a little above, for one on or in the ground) the
        open space of the BSPs on the cartridge"""
        from .collision import NONE as COLLISION_NONE
        x, y, z = position
        return not self.spaces or any(space.test_point((x, y, z + dz)) != COLLISION_NONE
                                      for space in self.spaces for dz in (0.3, 1.0, 3.0, 6.0))


def squad_locations(xmap, defs, encounter, squad):
    """the starting locations of an encounter's squad, by name"""
    sf = {f.name: o for o, f in defs.fields("Scenario")}
    ef = {f.name: o for o, f in defs.fields("ScenarioEncounter")}
    qf = {f.name: o for o, f in defs.fields("ScenarioSquad")}
    lf = {f.name: o for o, f in defs.fields("ScenarioActorStartingLocation")}

    def block(address, offset, name):
        count, pointer, _ = struct.unpack("<iII", xmap.read(address + offset, 12))
        return [pointer + i * defs.size(name) for i in range(max(count, 0))]

    for e in block(xmap.scenario.address, sf["encounters"], "ScenarioEncounter"):
        if xmap.read(e + ef["name"], 32).split(b"\0")[0].decode("latin-1") != encounter:
            continue
        for q in block(e, ef["squads"], "ScenarioSquad"):
            if xmap.read(q + qf["name"], 32).split(b"\0")[0].decode("latin-1") == squad:
                return [struct.unpack("<3f", xmap.read(l + lf["position"], 12))
                        for l in block(q, qf["starting locations"], "ScenarioActorStartingLocation")]
    return []


class Closure:
    """the tags a campaign map keeps: what its roots reach through the blocks
    and references the N64 keeps"""

    def __init__(self, xmap, defs, dropped_groups, dropped_blocks, bsp_indices):
        self.map = xmap
        self.defs = defs
        self.dropped_groups = {fourcc(g) for g in dropped_groups}
        self.dropped_blocks = dropped_blocks
        self.bsp_indices = set(bsp_indices)
        self.kept = set()
        self.placements = Placements(xmap, defs, bsp_indices)
        self.palette_use = self.placements.palette_use
        # animation graphs the converter cannot convert (compressed: the
        # cinematics' own, a30's dead pilot's): a palette entry whose object
        # animates by one is left out
        self.unconvertible = set(compressed_graphs(xmap, defs))
        palettes = {f.name: (o, f) for o, f in defs.fields("Scenario")}
        for palette, entries in self.palette_use.items():
            o, f = palettes[palette]
            count, pointer, _ = struct.unpack("<iII", xmap.read(xmap.scenario.address + o, 12))
            size = defs.size(f.struct)
            fo = {g.name: off for off, g in defs.fields(f.struct)}
            for e in sorted(entries):
                if e >= count:
                    continue
                datum = struct.unpack("<I", xmap.read(pointer + e * size + fo["name"] + 12, 4))[0]
                definition = xmap.tag(datum)
                if definition is not None and any(r.index in self.unconvertible for r in self.references(definition)):
                    entries.discard(e)

    def add(self, tag):
        todo = [tag]
        while todo:
            t = todo.pop()
            if t is None or t.index in self.kept or t.group in self.dropped_groups:
                continue
            if self.defs.fourcc_struct(t.group) is None:
                continue
            # (a structure BSP not mapped: one the converter does not load)
            if t.group_name == "sbsp" and not any(b[0].index == t.index for b in self.map.bsps):
                continue
            self.kept.add(t.index)
            todo.extend(self.references(t))

    def references(self, tag):
        found = []
        xmap, defs = self.map, self.defs
        if tag.group_name == "udlg":
            for v in DIALOG_VOCALIZATIONS:
                datum = struct.unpack("<I", xmap.read(tag.address + 16 + 16 * v + 12, 4))[0]
                found.append(xmap.tag(datum))
            return [t for t in found if t is not None]

        def visit(address, name, path):
            size = defs.size(name)
            blob = xmap.read(address, size)
            for offset, f in defs.fields(name):
                for i in range(f.count):
                    o = offset + i * f.size
                    if f.kind == "struct":
                        visit(address + o, f.struct, f"{path}.{f.name}")
                    elif f.kind == "TagReflexive":
                        sub = f"{path}.{f.name}[]"
                        if sub in self.dropped_blocks:
                            continue
                        count, pointer, _ = struct.unpack_from("<iII", blob, o)
                        if count <= 0 or not pointer:
                            continue
                        elements = range(count)
                        if path == "Scenario" and f.name in self.palette_use:
                            elements = sorted(e for e in self.palette_use[f.name] if e < count)
                        elif sub == "Scenario.structure bsps[]":
                            elements = sorted(e for e in self.bsp_indices if e < count)
                        esize = defs.size(f.struct)
                        for e in elements:
                            visit(pointer + e * esize, f.struct, sub)
                    elif f.kind == "TagDependency":
                        datum = struct.unpack_from("<I", blob, o + 12)[0]
                        found.append(xmap.tag(datum))
                    elif f.type == "TagID":
                        datum = struct.unpack_from("<I", blob, o)[0]
                        found.append(xmap.tag(datum))

        root = defs.fourcc_struct(tag.group)
        try:
            visit(tag.address, root, root)
        except KeyError:
            # (data outside what the converter mapped: a tag only another
            # BSP's objects use)
            pass
        return [t for t in found if t is not None]


def compressed_graphs(xmap, defs):
    """the animation graphs with compressed animations (convert.py takes
    none)"""
    out = []
    mo = {f.name: o for o, f in defs.fields("ModelAnimations")}
    ao = {f.name: o for o, f in defs.fields("ModelAnimationsAnimation")}
    size = defs.size("ModelAnimationsAnimation")
    for t in xmap.tags:
        if t.group_name != "antr":
            continue
        count, pointer, _ = struct.unpack("<iII", xmap.read(t.address + mo["animations"], 12))
        for i in range(max(count, 0)):
            if struct.unpack("<H", xmap.read(pointer + i * size + ao["flags"], 2))[0] & 1:
                out.append(t.index)
                break
    return out


def actor_units(xmap, defs):
    """the bipeds and weapons of the scenario's actor palette (the actors'
    variants are left out: the encounter table names them instead)"""
    sf = {f.name: o for o, f in defs.fields("Scenario")}
    ap = {f.name: o for o, f in defs.fields("ScenarioActorPalette")}
    av = {f.name: o for o, f in defs.fields("ActorVariant")}
    count, pointer, _ = struct.unpack("<iII", xmap.read(xmap.scenario.address + sf["actor palette"], 12))
    size = defs.size("ScenarioActorPalette")
    out = []
    for i in range(max(count, 0)):
        variant = xmap.tag(struct.unpack("<I", xmap.read(pointer + i * size + ap["reference"] + 12, 4))[0])
        if variant is None or variant.group_name != "actv":
            continue
        for field in ("unit", "weapon"):
            t = xmap.tag(struct.unpack("<I", xmap.read(variant.address + av[field] + 12, 4))[0])
            if t is not None:
                out.append(t)
    return out


def campaign_closure(xmap, defs, bsp_indices=(0,)):
    closure = Closure(xmap, defs, CAMPAIGN_DROPPED_GROUPS, CAMPAIGN_DROPPED_BLOCKS, bsp_indices)
    closure.add(xmap.scenario)
    # the encounters' bipeds and their weapons (n64_campaign.c places them)
    for t in actor_units(xmap, defs):
        closure.add(t)
    for t in xmap.tags:
        if t.group in {fourcc(g) for g in ROOT_GROUPS}:
            closure.add(t)
    return closure


# the scenario's placements (struct name: its palette's field)
PLACEMENT_STRUCTS = {"ScenarioScenery": "scenery palette", "ScenarioBiped": "biped palette",
                     "ScenarioEquipment": "equipment palette", "ScenarioWeapon": "weapon palette",
                     "ScenarioMachine": "machine palette", "ScenarioControl": "control palette",
                     "ScenarioLightFixture": "light fixture palette", "ScenarioSoundScenery": "sound scenery palette",
                     "ScenarioVehicle": "vehicle palette"}


def placement_type(xmap, defs, placements, struct_name, address):
    """a placement's palette index as the N64 has it: NONE where it is not
    kept (Placements) or its entry was not followed (its tag is not on the
    cartridge -- objects_place reads every placement's definition, made or
    not: object_types.c)"""
    offset = {f.name: o for o, f in defs.fields(struct_name)}["type"]
    index = struct.unpack("<h", xmap.read(address + offset, 2))[0]
    if index >= 0 and address in placements.kept and index in placements.palette_use[PLACEMENT_STRUCTS[struct_name]]:
        return index & 0xFFFF
    return 0xFFFF


def placement_not_placed(placements, address):
    """a kept placement's "not placed" flags as the N64 has them, or None"""
    return placements.kept.get(address)


def mask_dropped_tags(xmap, kept):
    """the tags a campaign map leaves out, hidden from the converters that
    look for tags by group (models, the HUD, effects, sounds): their group
    becomes NONE (the image writes them so anyway)"""
    for t in xmap.tags:
        if t.index not in kept:
            t.group = NONE


# ---------- the encounters' squads (n64_campaign.c)

ENCOUNTER_MAGIC = b"H64E"
ENCOUNTER_VERSION = 1


def encounter_table(xmap, defs, kept, inside=None):
    """the N64's table of the encounters' squads (big-endian):

        'H64E', version, actor kind count, squad count, location count
        actor kinds[]  u32 unit (biped tag datum), u32 weapon (weapon tag datum),
                       u8 team (0 the players', 1 the Covenant's), u8 pad[3]
        squads[]       name[32], encounter name[32], u16 actor kind,
                       u8 count easy/normal, u8 count heroic/legendary,
                       u16 first location, u16 location count, u16 encounter,
                       u16 flags, s16 wave (0: none; n: the encounter's wave n)
        locations[]    f32 x, y, z, facing

    Squads with no actor kind, no count or no starting location are left
    out (Halo's scripts move actors between those), and with inside (a
    point's test: Placements.inside) those with no starting location in
    the BSPs on the cartridge."""
    walker = Walker(xmap, defs)
    del walker
    sf = {f.name: o for o, f in defs.fields("Scenario")}
    scenario = xmap.scenario.address

    def block(address, name, struct_name):
        count, pointer, _ = struct.unpack("<iII", xmap.read(address + name, 12))
        size = defs.size(struct_name)
        return [pointer + i * size for i in range(max(count, 0))]

    def fields(name):
        return {f.name: o for o, f in defs.fields(name)}

    ap = fields("ScenarioActorPalette")
    av = fields("ActorVariant")
    ef = fields("ScenarioEncounter")
    qf = fields("ScenarioSquad")
    lf = fields("ScenarioActorStartingLocation")

    kinds = []
    kind_of_palette = {}
    for i, address in enumerate(block(scenario, sf["actor palette"], "ScenarioActorPalette")):
        datum = struct.unpack("<I", xmap.read(address + ap["reference"] + 12, 4))[0]
        variant = xmap.tag(datum)
        if variant is None or variant.group_name != "actv":
            continue
        unit = xmap.tag(struct.unpack("<I", xmap.read(variant.address + av["unit"] + 12, 4))[0])
        weapon = xmap.tag(struct.unpack("<I", xmap.read(variant.address + av["weapon"] + 12, 4))[0])
        if unit is None or unit.index not in kept:
            continue
        name = unit.name.lower()
        team = 0 if any(word in name for word in ALLY_WORDS) else 1
        kind_of_palette[i] = len(kinds)
        kinds.append((unit.datum & 0xFFFFFFFF,
                      weapon.datum & 0xFFFFFFFF if weapon is not None and weapon.index in kept else NONE, team,
                      unit.name))

    squads, locations = [], []
    for e, address in enumerate(block(scenario, sf["encounters"], "ScenarioEncounter")):
        ename = xmap.read(address + ef["name"], 32).split(b"\0")[0]
        for q in block(address, ef["squads"], "ScenarioSquad"):
            raw = xmap.read(q, defs.size("ScenarioSquad"))
            qname = raw[qf["name"]:qf["name"] + 32].split(b"\0")[0]
            actor = struct.unpack_from("<h", raw, qf["actor type"])[0]
            normal = struct.unpack_from("<h", raw, qf["normal diff count"])[0]
            insane = struct.unpack_from("<h", raw, qf["insane diff count"])[0]
            flags = struct.unpack_from("<I", raw, qf["flags"])[0]
            starts = block(q, qf["starting locations"], "ScenarioActorStartingLocation")
            if actor not in kind_of_palette or max(normal, insane) <= 0 or not starts:
                continue
            # (the players' side: CAMPAIGN_ALLIES)
            if not CAMPAIGN_ALLIES and kinds[kind_of_palette[actor]][2] == 0:
                continue
            # (a vehicle's crew, with no vehicle: DROPPED_SQUAD_WORDS)
            if any(word in (ename + b" " + qname).decode("latin-1").lower() for word in DROPPED_SQUAD_WORDS):
                continue
            points = []
            for s in starts:
                x, y, z = struct.unpack("<3f", xmap.read(s + lf["position"], 12))
                facing = struct.unpack("<f", xmap.read(s + lf["facing"], 4))[0]
                points.append((x, y, z, facing))
            # (only the starting locations inside: an actor comes at one in
            # turn, and one outside stood outside the map)
            if inside is not None:
                points = [p for p in points if inside(p[:3])]
            if not points:
                continue
            first = len(locations)
            locations.extend(points)
            wave = 0
            text = qname.decode("latin-1")
            if text.startswith("wave_"):
                digits = text[5:].split("_")[0]
                if digits.isdigit():
                    wave = int(digits)
            squads.append((qname[:31], ename[:31], kind_of_palette[actor], max(normal, 0), max(insane, 0),
                           first, len(points), e, flags & 0xFFFF, wave))

    out = bytearray(ENCOUNTER_MAGIC + struct.pack(">IIII", ENCOUNTER_VERSION, len(kinds), len(squads),
                                                    len(locations)))
    for unit, weapon, team, _ in kinds:
        out += struct.pack(">IIB3x", unit, weapon, team)
    for name, encounter, kind, normal, insane, first, count, e, flags, wave in squads:
        out += name.ljust(32, b"\0") + encounter.ljust(32, b"\0")
        out += struct.pack(">HBBHHHHh", kind, min(normal, 255), min(insane, 255), first, count, e, flags, wave)
    for x, y, z, facing in locations:
        out += struct.pack(">4f", x, y, z, facing)
    return bytes(out), kinds, squads, locations


# ---------- where the game is played (collision_mesh.py's tolerances)

def play_points(xmap, defs):
    """points where players and the AI go: the player starting locations,
    the placed objects, the encounters' starting locations and firing
    positions, the trigger volumes' corners and middles"""
    sf = {f.name: (o, f) for o, f in defs.fields("Scenario")}
    scenario = xmap.scenario.address
    points = []

    def elements(address, field_offset, struct_name):
        count, pointer, _ = struct.unpack("<iII", xmap.read(address + field_offset, 12))
        size = defs.size(struct_name)
        return [pointer + i * size for i in range(max(count, 0))]

    def offset(struct_name, field):
        return {f.name: o for o, f in defs.fields(struct_name)}[field]

    def point(address):
        return struct.unpack("<3f", xmap.read(address, 12))

    for name in ("player starting locations", "scenery", "bipeds", "equipment", "weapons", "machines", "controls",
                 "sound scenery", "netgame flags", "netgame equipment"):
        if name not in sf:
            continue
        o, f = sf[name]
        po = offset(f.struct, "position")
        for a in elements(scenario, o, f.struct):
            points.append(point(a + po))
    eo, ef = sf["encounters"]
    eff = {f.name: (o, f) for o, f in defs.fields(ef.struct)}
    for e in elements(scenario, eo, ef.struct):
        fo, ff = eff["firing positions"]
        for a in elements(e, fo, ff.struct):
            points.append(point(a + offset(ff.struct, "position")))
        qo, qf = eff["squads"]
        qff = {f.name: (o, f) for o, f in defs.fields(qf.struct)}
        lo, lf = qff["starting locations"]
        for q in elements(e, qo, qf.struct):
            for a in elements(q, lo, lf.struct):
                points.append(point(a + offset(lf.struct, "position")))
    to, tf = sf["trigger volumes"]
    for t in elements(scenario, to, tf.struct):
        corner = point(t + offset(tf.struct, "starting corner"))
        extent = point(t + offset(tf.struct, "ending corner offset"))
        points.append(corner)
        points.append(tuple(corner[k] + extent[k] for k in range(3)))
        points.append(tuple(corner[k] + extent[k] / 2 for k in range(3)))
    return points


# ---------- the level's collision, simpler and compact (collision*.py)

LEVEL_TOLERANCE = dict(near=0.1, far=0.8, near_distance=8.0, far_distance=40.0)


def level_collision(xmap, defs, bsp_tag, verify_rays=3000, log=print):
    """the structure BSP's collision made anew: its mesh simplified within
    LEVEL_TOLERANCE of where the game is played, built as a BSP of the
    campaign cartridge's compact layout. Returns a dict: 'blocks' (field
    name -> (count, bytes)), 'clusters' (each new leaf's cluster),
    'pathfinding' (each surface's flags), 'bsp' (the CollisionBSP)"""
    import random
    import sys
    from .collision import NONE as COLLISION_NONE
    from .collision import SIGN, BLOCK_NAMES, CollisionBSP, compact_rows, signed
    from .collision_build import Builder
    from .collision_mesh import (PlaySpace, extract, merge_coplanar, simplify, surfaces_for,
                                 tolerance_by_distance)
    sys.setrecursionlimit(max(sys.getrecursionlimit(), 20000))
    bo = {f.name: o for o, f in defs.fields("ScenarioStructureBSP")}
    count, pointer, _ = struct.unpack("<iII", xmap.read(bsp_tag.address + bo["collision bsp"], 12))
    old = CollisionBSP.read(xmap, pointer)
    points = [p for p in play_points(xmap, defs) if old.test_point((p[0], p[1], p[2] + 0.3)) != COLLISION_NONE]
    space = PlaySpace(points)
    tolerance = tolerance_by_distance(**LEVEL_TOLERANCE)
    mesh = extract(old)
    stats = {}
    triangles = simplify(mesh, lambda p: tolerance(space.distance(p)), stats)
    polygons = merge_coplanar(mesh.positions, triangles)
    vertices, planes, surfaces = surfaces_for(mesh.positions, polygons, mesh.sheets)
    builder = Builder(vertices, planes, surfaces)
    new = builder.run()
    problems = new.check()
    if problems:
        raise ValueError("the new collision BSP: " + "; ".join(problems[:5]))
    # each new leaf's cluster: the original's at a point inside it
    lo = struct.unpack("<iII", xmap.read(bsp_tag.address + bo["leaves"], 12))
    leaf_size = defs.size("ScenarioStructureBSPLeaf")
    cluster_offset = {f.name: o for o, f in defs.fields("ScenarioStructureBSPLeaf")}["cluster"]
    old_clusters = [struct.unpack("<h", xmap.read(lo[1] + i * leaf_size + cluster_offset, 2))[0]
                    for i in range(max(lo[0], 0))]
    # the cluster portals' planes (structures.c sphere_intersects_cluster_
    # portal reads them from the collision's planes): after the new BSP's
    # own, each portal's plane index made to point there (CampaignPolicy.
    # override)
    po = {f.name: o for o, f in defs.fields("ScenarioStructureBSPClusterPortal")}
    portal_count, portal_pointer, _ = struct.unpack("<iII", xmap.read(bsp_tag.address + bo["cluster portals"], 12))
    portal_size = defs.size("ScenarioStructureBSPClusterPortal")
    portal_planes = {}
    portals = []
    for i in range(max(portal_count, 0)):
        address = portal_pointer + i * portal_size
        plane = struct.unpack("<i", xmap.read(address + po["plane index"], 4))[0]
        front_cluster, back_cluster = struct.unpack("<hh", xmap.read(address + po["front cluster"], 4))
        portal_planes[address] = len(new.planes)
        portals.append((len(new.planes), front_cluster, back_cluster))
        new.planes.append(list(old.planes[plane & 0x7FFFFFFF]))

    # each new leaf's cluster: the original's at points inside it
    def samples_of(leaf):
        found = []
        for distance in (0.002, 0.01, 0.05, 0.2, 0.6):
            for middle, normal in builder.leaf_points[leaf]:
                p = tuple(middle[k] + normal[k] * distance for k in range(3))
                if new.test_point(p) != leaf:
                    continue
                # (where the simpler surface stands in front of the map's,
                # the map's empty side a little further on)
                for further in (0.0, 0.1, 0.3, 0.8, 2.0):
                    q = tuple(p[k] + normal[k] * further for k in range(3))
                    o = old.test_point(q)
                    if o != COLLISION_NONE and 0 <= o < len(old_clusters):
                        found.append((p, old_clusters[o]))
                        break
            if len(found) >= 12:
                break
        return found

    samples = [samples_of(leaf) for leaf in range(len(new.leaves))]
    clusters = []
    unresolved = 0
    for found in samples:
        if not found:
            unresolved += 1
            clusters.append(0)
            continue
        values = [c for _, c in found]
        clusters.append(max(set(values), key=values.count))

    # a leaf across two clusters split by the portal between them (the
    # original's leaves never are: its clusters are sets of them): a node
    # of the portal's plane over the leaf and a copy of it, both with the
    # leaf's surfaces (its references), each side its own cluster
    parent = {}
    for n, (plane, back, front) in enumerate(new.nodes):
        for side, child in ((1, back), (2, front)):
            c = signed(child)
            if c < 0 and c != COLLISION_NONE:
                parent[c & 0x7FFFFFFF] = (n, side)

    def split(leaf, found, depth):
        values = [c for _, c in found]
        present = set(values)
        if len(present) <= 1:
            return
        best = None
        for plane_index, front_cluster, back_cluster in portals:
            if front_cluster not in present and back_cluster not in present:
                continue
            i, j, k, d = new.planes[plane_index]
            score = 0
            for p, c in found:
                # (a portal's front cluster is on its plane's negative
                # side: so in all 126 of a30_a's portals' tested sides)
                in_front = p[0] * i + p[1] * j + p[2] * k - d >= 0
                if c == (back_cluster if in_front else front_cluster):
                    score += 1
            if best is None or score > best[0]:
                best = (score, plane_index, front_cluster, back_cluster)
        majority = max(values.count(c) for c in present)
        if best is None or best[0] <= majority or depth >= 3:
            return
        _, plane_index, front_cluster, back_cluster = best
        i, j, k, d = new.planes[plane_index]
        node = len(new.nodes)
        copy = len(new.leaves)
        new.leaves.append(list(new.leaves[leaf]))
        new.nodes.append([plane_index, SIGN | copy, SIGN | leaf])
        parent_node, side = parent[leaf]
        new.nodes[parent_node][side] = node
        parent[leaf] = (node, 2)
        parent[copy] = (node, 1)
        front_found = [(p, c) for p, c in found if p[0] * i + p[1] * j + p[2] * k - d >= 0]
        back_found = [(p, c) for p, c in found if p[0] * i + p[1] * j + p[2] * k - d < 0]
        clusters[leaf] = back_cluster if not front_found else max(
            set(c for _, c in front_found), key=[c for _, c in front_found].count)
        clusters.append(front_cluster if not back_found else max(
            set(c for _, c in back_found), key=[c for _, c in back_found].count))
        split(leaf, front_found, depth + 1)
        split(copy, back_found, depth + 1)

    mixed = sum(1 for found in samples if len(set(c for _, c in found)) > 1)
    for leaf in range(len(samples)):
        if len(set(c for _, c in samples[leaf])) > 1:
            split(leaf, samples[leaf], 0)
    blocks = {name: compact_rows(name, getattr(new, name)) for name in BLOCK_NAMES.values()}
    size = sum(len(b) for _, b in blocks.values())
    log(f"{bsp_tag.name}: collision {len(old.surfaces)} -> {len(new.surfaces)} surfaces "
        f"({stats.get('collapses', 0)} collapses), {old.size() / 1024:.0f} KB -> {size / 1024:.0f} KB compact; "
        f"{len(new.nodes)} nodes, {len(new.leaves)} leaves ({unresolved} without a cluster, {mixed} across two "
        f"split by portals into {len(new.leaves) - len(samples)} more)")
    if verify_rays:
        result = compare(old, new, points, verify_rays)
        log(f"{bsp_tag.name}: collision check: " + ", ".join(f"{k} {v}" for k, v in result.items()))
    return {"blocks": blocks, "clusters": clusters, "pathfinding": bytes(s.pathfinding for s in surfaces),
            "bsp": new, "nodes": len(new.nodes), "portal_planes": portal_planes}


def compare(old, new, points, rays, seed=11):
    """the new collision against the old: rays from near the play points"""
    import math
    import random
    from .collision import NONE as COLLISION_NONE
    rng = random.Random(seed)
    result = {"rays": 0, "holes": 0, "phantoms": 0, "far off": 0, "ground holes": 0}
    for _ in range(rays):
        p0 = rng.choice(points)
        p = (p0[0] + rng.uniform(-6, 6), p0[1] + rng.uniform(-6, 6), p0[2] + rng.uniform(0.2, 4))
        if old.test_point(p) == COLLISION_NONE or new.test_point(p) == COLLISION_NONE:
            continue
        v = (rng.uniform(-1, 1) * 30, rng.uniform(-1, 1) * 30, rng.uniform(-1, 0.5) * 30)
        length = math.sqrt(v[0] ** 2 + v[1] ** 2 + v[2] ** 2)
        a = old.test_vector(p, v)
        c = new.test_vector(p, v)
        result["rays"] += 1
        if a and not c:
            result["holes"] += 1
        elif c and not a:
            result["phantoms"] += 1
        elif a and c and abs(a[0] - c[0]) * length > 1.0:
            result["far off"] += 1
    for p in rng.sample(points, min(len(points), 1500)):
        start = (p[0], p[1], p[2] + 0.5)
        if new.test_point(start) == COLLISION_NONE:
            continue
        if old.test_vector(start, (0, 0, -6)) and not new.test_vector(start, (0, 0, -6)):
            result["ground holes"] += 1
    return result


# ---------- the scenery's collision models, simpler (boulders: 600
# surfaces each of their five shapes)

# (a share of the shape's size: a pebble's few centimetres, a crag's a
# little under a metre at most)
MODEL_TOLERANCE_SHARE = 0.04
MODEL_TOLERANCE = (0.04, 0.3)


def model_collisions(xmap, defs, kept, log=print):
    """the kept scenery's collision BSPs simplified and rebuilt (compact):
    {the map address of each original block: (count, bytes)} for
    CampaignPolicy.replace_block"""
    import sys
    from .collision import BLOCK_NAMES, CollisionBSP, compact_rows
    from .collision_build import Builder
    from .collision_mesh import extract, merge_coplanar, simplify, surfaces_for
    sys.setrecursionlimit(max(sys.getrecursionlimit(), 20000))
    cf = {f.name: o for o, f in defs.fields("ModelCollisionGeometry")}
    nf = {f.name: o for o, f in defs.fields("ModelCollisionGeometryNode")}
    bsp_size = defs.size("ModelCollisionGeometryBSP")
    node_size = defs.size("ModelCollisionGeometryNode")
    out = {}
    before = after = 0
    for t in xmap.tags:
        if t.group_name != "coll" or t.index not in kept or not t.name.startswith("scenery\\"):
            continue
        count, pointer, _ = struct.unpack("<iII", xmap.read(t.address + cf["nodes"], 12))
        for n in range(max(count, 0)):
            bc, bp, _ = struct.unpack("<iII", xmap.read(pointer + n * node_size + nf["bsps"], 12))
            for b in range(max(bc, 0)):
                address = bp + b * bsp_size
                old = CollisionBSP.read(xmap, address)
                mesh = extract(old)
                if mesh.sheets or len(mesh.triangles) < 40:
                    continue
                xs = [p[0] for p in mesh.positions]
                ys = [p[1] for p in mesh.positions]
                zs = [p[2] for p in mesh.positions]
                radius = 0.5 * max(max(xs) - min(xs), max(ys) - min(ys), max(zs) - min(zs))
                tolerance = min(MODEL_TOLERANCE[1], max(MODEL_TOLERANCE[0], radius * MODEL_TOLERANCE_SHARE))
                try:
                    triangles = simplify(mesh, lambda p: tolerance)
                    polygons = merge_coplanar(mesh.positions, triangles)
                    vertices, planes, surfaces = surfaces_for(mesh.positions, polygons, [])
                    new = Builder(vertices, planes, surfaces).run()
                except ValueError as error:
                    log(f"{t.name}: collision BSP {n}/{b} kept as it is ({error})")
                    continue
                if new.check() or not same_shape(old, new, radius, tolerance):
                    log(f"{t.name}: collision BSP {n}/{b} kept as it is (the simpler one differs)")
                    continue
                # (the original's blocks, by where each was: a block the
                # original has empty has no place to be told by)
                made = {}
                for i, name in enumerate(BLOCK_NAMES.values()):
                    block_count, block_pointer, _ = struct.unpack("<iII", xmap.read(address + 12 * i, 12))
                    rows = getattr(new, name)
                    if block_count <= 0 and rows:
                        made = None
                        break
                    if block_count > 0:
                        made[block_pointer] = compact_rows(name, rows)
                if made is None:
                    continue
                out.update(made)
                before += len(old.surfaces)
                after += len(new.surfaces)
    log(f"scenery collision models: {before} -> {after} surfaces")
    return out


def same_shape(old, new, radius, tolerance, rays=300, seed=5):
    """a simpler model BSP against its original: rays from around it towards
    its middle hit both or neither, within a few tolerances of each other"""
    import math
    import random
    rng = random.Random(seed)
    xs = [v[0] for v in old.vertices]
    ys = [v[1] for v in old.vertices]
    zs = [v[2] for v in old.vertices]
    middle = ((min(xs) + max(xs)) / 2, (min(ys) + max(ys)) / 2, (min(zs) + max(zs)) / 2)
    bad = 0
    for _ in range(rays):
        theta = rng.uniform(0, 2 * math.pi)
        z = rng.uniform(-1, 1)
        r = math.sqrt(1 - z * z)
        direction = (r * math.cos(theta), r * math.sin(theta), z)
        start = tuple(middle[k] + direction[k] * radius * 2.5 for k in range(3))
        target = tuple(middle[k] + rng.uniform(-0.5, 0.5) * radius for k in range(3))
        vector = tuple(target[k] - start[k] for k in range(3))
        length = math.sqrt(sum(v * v for v in vector))
        a = old.test_vector(start, vector)
        c = new.test_vector(start, vector)
        if (a is None) != (c is None):
            # (a ray grazing an edge may hit one and miss the other)
            bad += 1
        elif a is not None and abs(a[0] - c[0]) * length > 4 * tolerance:
            bad += 1
    return bad <= rays // 50
