#!/usr/bin/env python3
"""Converting an Xbox multiplayer map for the Nintendo 64 build.

    python3 -m tools.n64.convert build/xbox/disc/maps/chillout.map -o build/n64/maps/chillout.map

The output (N64 map) is one file the game copies from the cartridge into
RAM and relocates (port/n64/src/n64_cache.c):

    header (64 bytes, big-endian)
        'H64M', version, name[32], image size, relocation count,
        tag header offset, BSP header offset, format kind ('MP32'/'CP16'), reserved
    image (tag data, big-endian; pointers are image offsets)
    relocations (image offsets of every non-null pointer, u32 each)

Tag data the N64 draws or plays differently (bitmaps, sounds, render
geometry) is left out here and converted separately.
"""

import argparse
import struct
import sys
from pathlib import Path

from .repack import NONE, External, Policy, Repacker, swap_array
from .xboxmap import XboxMap

MAGIC = b"H64M"
VERSION = 2
MAP_KIND_MULTIPLAYER = 0x4D503332  # MP32: original collision, native cluster-only leaves
MAP_KIND_CAMPAIGN = 0x43503136     # CP16: upstream campaign's compact collision
PC_MAPS = {"dangercanyon", "deathisland", "gephyrophobia", "icefields", "infinity", "timberland"}


def validate_pc_maps(pc_expansion=False, pc_maps=None, campaign=False):
    """The optional expansion reads only an explicitly staged Xbox-v5 set.

    Retail PC-v7 inputs must first pass the installer's Invader normalization;
    a sibling Timberland file never enables imports for a vanilla cartridge.
    """
    if not pc_expansion:
        if pc_maps is not None:
            raise ValueError("--pc-maps requires --pc-expansion")
        return None
    if campaign:
        raise ValueError("campaign and PC expansion are separate cartridges")
    if pc_maps is None:
        raise ValueError("PC expansion requires --pc-maps with all six normalized Xbox-v5 maps")
    folder = Path(pc_maps).expanduser().resolve()
    for name in sorted(PC_MAPS):
        path = folder / f"{name}.map"
        with path.open("rb") as stream:
            header = stream.read(0x800)
        if (len(header) != 0x800 or header[:4] != b"daeh" or
                struct.unpack_from("<I", header, 4)[0] != 5 or
                header[0x20:0x40].split(b"\0")[0] != name.encode("ascii") or
                struct.unpack_from("<H", header, 0x60)[0] != 1):
            raise ValueError(f"{path}: expected a normalized Xbox-v5 multiplayer map named {name}")
    return folder

# Firefight's Covenant (port/n64/game/n64_firefight.c), imported into every
# multiplayer map from a campaign map beside it (merge.py)
# the view models' qualities (the menu's settings): their number (the
# model set's suffix, -fp<n>; 0 the plain set) and the first-person
# models' triangle budget's scale
VIEW_MODEL_QUALITIES = ((0, 1.0), (1, 1.25), (2, 2.0))
FIREFIGHT_SOURCE = "a30"
FIREFIGHT_ROOTS = [("bipd", "characters\\grunt\\grunt"), ("bipd", "characters\\jackal\\jackal"),
                   ("bipd", "characters\\elite\\elite")]

# tag groups the N64 game never loads: the Xbox menus and input devices,
# and sound environments (the N64 has no reverb); sounds and looping
# sounds are kept in a form of the N64's own (sounds.py) or left out
DROPPED_GROUPS = {b"DeLa", b"Soul", b"vcky", b"mply", b"devc", b"ngpr", b"snde",
                  # particles, particle systems, contrails, antennas, cloth
                  # flags and glows: the N64 makes none of them (its own
                  # simple particles come from effects.py's records)
                  b"part", b"pctl", b"cont", b"ant!", b"flag", b"glw!"}
SOUND_GROUPS = {int.from_bytes(g, "big") for g in (b"snd!", b"lsnd")}


def fourcc(text):
    return int.from_bytes(text, "big")


DROPPED = {fourcc(g) for g in DROPPED_GROUPS}


def swap_animation(raw, struct_raw, defs):
    """the frame data / default data of an uncompressed animation: per node,
    a rotation (4 shorts), a translation (3 floats) and a scale (float),
    each either per frame (the node's flag set) or in the default data
    (model_animations.c animation_get_node_orientations)"""
    return raw


class N64Policy(Policy):
    def __init__(self, xmap, defs):
        self.map = xmap
        self.defs = defs
        offs = {f.name: o for o, f in defs.fields("ModelAnimationsAnimation")}
        self.anim = offs
        # animation frames stream from the cartridge: a file for each
        # animation graph, shared by content (anim/<hash>.anim, the map's
        # list in maps/<map>.anims); a frame data's offset carries its graph's
        # place in that list in its top 8 bits
        self.streams = {}  # graph tag index -> bytearray
        self.graphs = []   # graph tag indices in list order
        self.hud = None
        # the gameplay sounds (sounds.py; None: none)
        self.sounds = None

    @property
    def stream_size(self):
        return sum(len(s) for s in self.streams.values())

    def keep_tag(self, tag):
        if tag.group in SOUND_GROUPS:
            return self.sounds is not None and self.sounds.keep(tag)
        return tag.group not in DROPPED

    def custom_tag(self, tag):
        if tag.group in SOUND_GROUPS:
            return self.sounds.blob(tag)
        return None

    def array_data(self, tag, address, name, count, path):
        if name == "ScenarioStructureBSPLeaf":
            size = self.defs.size(name)
            cluster = next(o for o, f in self.defs.fields(name) if f.name == "cluster")
            return b"".join(struct.pack(">h", struct.unpack("<h", self.map.read(address + i * size + cluster, 2))[0])
                            for i in range(count))
        return None

    # render data: the N64 draws its own level and models (level.py,
    # models.py); only Halo's renderer and lightmap lookups (left out) read
    # these
    RENDER_BLOCKS = ("ScenarioStructureBSP.nodes[]", "ScenarioStructureBSP.leaf surfaces[]", "ScenarioStructureBSP.surfaces[]",
                     "ScenarioStructureBSP.clusters[].subclusters[].surface indices[]",
                     "ScenarioStructureBSP.clusters[].surface indices[]", "ScenarioStructureBSP.lightmaps[].materials[]",
                     # the models' geometry: the N64's own (models.py)
                     "Model.geometries[].parts[]",
                     # effects' particles: the N64 draws none (437 KB of Sidewinder's
                     # tags, and their simulation)
                     "Effect.events[].particles[]")

    def keep_block(self, tag, path, count):
        return 0 if path in self.RENDER_BLOCKS else count

    def override(self, tag, struct_name, field_name, address):
        # a HUD image's sprite (tools/n64/hud.py): its number + 1
        if struct_name == "BitmapData" and field_name == "pixel data offset" and self.hud is not None:
            number = self.hud.sprite_of.get(address)
            return number + 1 if number is not None else 0
        return None

    def data(self, tag, path, parent, raw):
        address, struct_name, struct_raw = parent
        if path.endswith("animations[].frame data"):
            data = self.animation_data(struct_raw, raw, False)
            if tag.index not in self.streams:
                self.streams[tag.index] = bytearray()
                self.graphs.append(tag.index)
            stream = self.streams[tag.index]
            stream += b"\0" * ((-len(stream)) % 8)
            offset = len(stream)
            stream += data
            if offset >= 1 << 24 or len(self.graphs) > 255:
                raise ValueError("an animation graph past 16 MB, or 256 graphs in a map")
            return External(offset | (self.graphs.index(tag.index) << 24), len(data))
        # an animation's default pose and its per-frame root motion stream
        # too (model_animation_definitions.h/.c, HALO_N64): 190 KB of the
        # Covenant's graphs, and of every map's own
        if path.endswith("animations[].default data"):
            data = self.animation_data(struct_raw, raw, True)
            # (read whole: it must fit one of the stream cache's 4 KB chunks)
            return self.stream(tag, data) if len(data) <= 4096 else data
        if path.endswith("animations[].frame info"):
            return self.stream(tag, swap_array(raw, 4))
        if path.endswith("UnicodeStringList.strings[].string") or path.endswith("HUDMessageText.text data"):
            return swap_array(raw, 2)
        if path.endswith("StringList.strings[].string") or path.endswith("Scenario.script string data"):
            return raw
        if path.endswith("Meter.encoded stencil") or path.endswith("permutations[].mouth data"):
            return raw
        if path.endswith("ScenarioStructureBSP.cluster data"):
            return swap_array(raw, 4)
        if path.endswith("ScenarioStructureBSP.sound pas data"):
            return raw
        return None

    def stream(self, tag, data):
        """data that streams from the cartridge with the graph's frames"""
        if tag.index not in self.streams:
            self.streams[tag.index] = bytearray()
            self.graphs.append(tag.index)
        stream = self.streams[tag.index]
        stream += b"\0" * ((-len(stream)) % 8)
        offset = len(stream)
        stream += data
        if offset >= 1 << 24 or len(self.graphs) > 255:
            raise ValueError("an animation graph past 16 MB, or 256 graphs in a map")
        return External(offset | (self.graphs.index(tag.index) << 24), len(data))

    def animation_data(self, struct_raw, raw, default):
        o = self.anim
        frame_count, frame_size = struct.unpack_from("<hh", struct_raw, o["frame count"])
        node_count = struct.unpack_from("<h", struct_raw, o["node count"])[0]
        flags = struct.unpack_from("<H", struct_raw, o["flags"])[0]
        if flags & 1:
            raise ValueError("compressed animations are not converted yet")
        rot = struct.unpack_from("<2I", struct_raw, o["node rotation flag data"])
        trans = struct.unpack_from("<2I", struct_raw, o["node transform flag data"])
        scale = struct.unpack_from("<2I", struct_raw, o["node scale flag data"])

        def flag(bits, n):
            return (bits[n >> 5] >> (n & 31)) & 1

        layout = []  # element sizes of one frame (or of the default data)
        for n in range(node_count):
            want = 0 if default else 1
            if flag(rot, n) == want:
                layout += [2, 2, 2, 2]
            if flag(trans, n) == want:
                layout += [4, 4, 4]
            if flag(scale, n) == want:
                layout += [4]
        unit = sum(layout)
        out = bytearray(raw)
        if unit == 0:
            return bytes(out)
        frames = 1 if default else frame_count
        if unit * frames != len(raw):
            raise ValueError(f"animation data is {len(raw)} bytes, layout {unit}x{frames}")
        pos = 0
        for _ in range(frames):
            for size in layout:
                out[pos:pos + size] = raw[pos:pos + size][::-1]
                pos += size
        return bytes(out)


class CampaignPolicy(N64Policy):
    """a campaign level's (tools/n64/campaign.py): only the tags its closure
    keeps, without the blocks the N64 does not use"""

    def __init__(self, xmap, defs, kept, single_bsp):
        super().__init__(xmap, defs)
        from .campaign import CAMPAIGN_DROPPED_BLOCKS
        self.kept = kept
        self.dropped_blocks = set(CAMPAIGN_DROPPED_BLOCKS)
        if single_bsp:
            # (one BSP on the cartridge: nothing to switch to)
            self.dropped_blocks.add("Scenario.bsp switch trigger volumes[]")

    def keep_tag(self, tag):
        return tag.index in self.kept and super().keep_tag(tag)

    def keep_block(self, tag, path, count):
        if path in self.dropped_blocks:
            return 0
        return super().keep_block(tag, path, count)

    # the level's collision (campaign.py level_collision), and what is
    # indexed by its nodes, leaves and surfaces; every other collision BSP
    # (the objects' collision models) in the compact layout as it is
    LEVEL = "ScenarioStructureBSP.collision bsp[]."
    level_collision = None
    model_collision = {}

    def override(self, tag, struct_name, field_name, address):
        lc = self.level_collision
        if lc is not None and struct_name == "ScenarioStructureBSPClusterPortal" and field_name == "plane index":
            return lc["portal_planes"][address]
        from .campaign import PLACEMENT_STRUCTS, placement_not_placed, placement_type
        if struct_name in PLACEMENT_STRUCTS and field_name == "type":
            return placement_type(self.map, self.defs, self.placements, struct_name, address)
        if struct_name in PLACEMENT_STRUCTS and field_name == "not placed":
            return placement_not_placed(self.placements, address)
        return super().override(tag, struct_name, field_name, address)

    def override_bytes(self, tag, struct_name, field_name, address):
        # a warthog moved near the start (campaign.py Placements.start_warthog)
        if struct_name == "ScenarioVehicle" and field_name == "position" and address in self.placements.moved:
            return struct.pack(">3f", *self.placements.moved[address])
        return None

    def replace_block(self, tag, path, count, pointer):
        from .collision import BLOCK_NAMES, compact_block_from_map
        lc = self.level_collision
        if lc is not None and path.startswith(self.LEVEL):
            return lc["blocks"][BLOCK_NAMES[path[len(self.LEVEL):-2]]]
        if lc is not None and path == "ScenarioStructureBSP.nodes[]":
            # (the nodes' bounds: the Xbox renderer's alone)
            return 0, b""
        if lc is not None and path == "ScenarioStructureBSP.leaves[]":
            # (a leaf's cluster, nothing else: structure_bsp_definitions.h,
            # HALO_N64_CAMPAIGN)
            return len(lc["clusters"]), struct.pack(f">{len(lc['clusters'])}h", *lc["clusters"])
        if lc is not None and path == "ScenarioStructureBSP.pathfinding surfaces[]":
            return len(lc["pathfinding"]), lc["pathfinding"]
        if lc is not None and path in ("ScenarioStructureBSP.pathfinding edges[]",
                                       "ScenarioStructureBSP.breakable surfaces[]",
                                       "ScenarioStructureBSP.leaf map leaves[]",
                                       "ScenarioStructureBSP.leaf map portals[]"):
            return 0, b""
        if ".bsps[]." in path and tag.group_name == "coll":
            field = path.rsplit(".", 1)[1][:-2]
            if field in BLOCK_NAMES:
                # (the scenery's simplified: campaign.py model_collisions)
                if count > 0 and pointer in self.model_collision:
                    return self.model_collision[pointer]
                return compact_block_from_map(self.map, BLOCK_NAMES[field], count, pointer)
        return None

    def data(self, tag, path, parent, raw):
        # (the scripts are not run)
        if path.endswith("Scenario.script string data"):
            return None
        return super().data(tag, path, parent, raw)


def open_map(path, *, campaign=False, pc_expansion=False, pc_maps=None):
    """Load a map and its explicitly selected additive content for exporters."""
    pc_folder = validate_pc_maps(pc_expansion, pc_maps, campaign)
    xmap = XboxMap(path)
    xmap.load_bsp(0)
    if pc_expansion and xmap.map_type != 1:
        raise ValueError("PC expansion imports require a multiplayer target map")
    if pc_folder is not None and xmap.name != "timberland":
        from .merge import merge_tags, merge_empty_weapon_hud
        pc_map = XboxMap(pc_folder / "timberland.map")
        imported = merge_tags(xmap, pc_map, [
            ("weap", "weapons\\flamethrower\\flamethrower"),
            ("weap", "weapons\\plasma_cannon\\plasma_cannon"),
            ("vehi", "vehicles\\rwarthog\\rwarthog")])
        print(f"{xmap.name}: {len(imported)} additive PC tags")
        hud = merge_empty_weapon_hud(xmap, pc_map, r"weapons\flamethrower\flamethrower")
        if hud is not None:
            print(f"{xmap.name}: additive PC Flamethrower HUD {hud.name}")
    source = Path(path).parent / f"{FIREFIGHT_SOURCE}.map"
    if (xmap.map_type == 1 and source.exists() and not campaign and
            not (pc_expansion and xmap.name in PC_MAPS)):
        from .merge import merge_tags
        imported = merge_tags(xmap, XboxMap(source), FIREFIGHT_ROOTS)
        print(f"{xmap.name}: {len(imported)} Covenant tags from {FIREFIGHT_SOURCE}")
    if pc_folder is not None:
        from .pc_placements import AIRCRAFT_PLACEMENTS, map_placements
        if map_placements(xmap.name) is not None:
            from .pc_additions import add_original_map_content
            donor = XboxMap(pc_folder / "timberland.map")
            aircraft = (XboxMap(pc_folder / "deathisland.map")
                        if xmap.name in AIRCRAFT_PLACEMENTS else None)
            add_original_map_content(xmap, donor, aircraft)
    return xmap


def map_header(name, image_size, relocation_count, tag_header, bsp_header, campaign=False):
    """The byte56 kind binds the payload to its collision/leaf compile ABI."""
    name = name.encode()[:31]
    kind = MAP_KIND_CAMPAIGN if campaign else MAP_KIND_MULTIPLAYER
    return (MAGIC + struct.pack(">I", VERSION) + name + b"\0" * (32 - len(name)) +
            struct.pack(">IIIIII", image_size, relocation_count, tag_header, bsp_header, kind, 0))


def convert(path, output, filesystem=None, campaign=False, pc_expansion=False, pc_maps=None):
    xmap = open_map(path, campaign=campaign, pc_expansion=pc_expansion, pc_maps=pc_maps)
    bsp_tag, bsp_base, bsp_size = xmap.bsps[0]
    repacker = Repacker(xmap, None)
    if campaign:
        from .campaign import campaign_closure, encounter_table, mask_dropped_tags
        closure = campaign_closure(xmap, repacker.defs, (0,))
        kept = closure.kept
        # (the encounters read before the tags they name are hidden)
        table, kinds, squads, locations = encounter_table(xmap, repacker.defs, kept, closure.placements.inside)
        print(f"{xmap.name}: {len(kept)} of {len(xmap.tags)} tags kept; {len(kinds)} actor kinds, "
              f"{len(squads)} squads, {len(locations)} starting locations; {len(closure.placements.kept)} "
              f"placements kept, {closure.placements.dropped} left out ({closure.placements.thinned} "
              f"scenery thinned)")
        # (the user: "ensure the flying banshees don't spawn at all": no
        # banshee on the cartridge, whatever the placements and squads)
        flying = [t.name for t in xmap.tags if t.index in kept and t.group_name == "vehi" and "banshee" in t.name]
        if flying:
            raise ValueError(f"campaign: banshees kept ({', '.join(flying)})")
        from .campaign import level_collision, model_collisions
        collision = level_collision(xmap, repacker.defs, bsp_tag)
        models = model_collisions(xmap, repacker.defs, kept)
        mask_dropped_tags(xmap, kept)
        repacker.policy = CampaignPolicy(xmap, repacker.defs, kept, single_bsp=True)
        repacker.policy.encounters = table
        repacker.policy.level_collision = collision
        repacker.policy.placements = closure.placements
        repacker.policy.model_collision = models
    else:
        repacker.policy = N64Policy(xmap, repacker.defs)
    if filesystem is not None:
        from .hud import HudExporter
        repacker.policy.hud = HudExporter(xmap, repacker.defs, xmap.name, filesystem)
        print(f"{xmap.name} hud: {repacker.policy.hud.collect()} images")
        from .sounds import SoundConverter
        repacker.policy.sounds = SoundConverter(xmap, repacker.defs, Path(filesystem).parent / "sound-cache")
    image = repacker.image
    image.alloc(0)

    tags = list(xmap.tags)
    roots = repacker.repack(tags)

    # the tag header and instance array (cache_files.c)
    instances = image.alloc(32 * len(tags))
    for t in tags:
        o = instances + 32 * t.index
        kept = t.index in roots
        group = t.group if kept else NONE
        image.put(o, struct.pack(">IIII", group, t.parents[0], t.parents[1], t.datum & 0xFFFFFFFF))
        image.put_pointer(o + 16, repacker.name_offset(t.name))
        # the BSP's instance has no address until the game loads the BSP
        # (cache_files.c scenario_structure_bsp_load asserts it)
        image.put_pointer(o + 20, None if t.index == bsp_tag.index else roots.get(t.index))
        image.put(o + 24, b"\0" * 8)
    header = image.alloc(36)
    image.put_pointer(header, instances)
    image.put(header + 4, struct.pack(">iIiIIII", xmap.scenario_datum, xmap.tags_checksum, len(tags), 0, 0, 0, 0))
    image.put(header + 32, b"tags")

    # the structure BSP's header (cache_file_structure_bsp_header) and the
    # scenario's reference to it
    bsp_header = image.alloc(24)
    image.put_pointer(bsp_header, roots.get(bsp_tag.index))
    image.put(bsp_header + 4, struct.pack(">IIII", 0, 0, 0, 0) + b"sbsp")
    scenario_root = roots[xmap.scenario.index]
    count, refs = struct.unpack_from(">iI", image.data, scenario_root + 0x5A4)
    if count > 0:
        image.put_pointer(refs + 8, bsp_header)

    relocs = sorted(set(image.relocs))
    out = bytearray()
    out += map_header(xmap.name, len(image.data), len(relocs), header, bsp_header, campaign)
    out += image.data
    out += b"\0" * ((-len(out)) % 8)
    out += struct.pack(f">{len(relocs)}I", *relocs)
    Path(output).parent.mkdir(parents=True, exist_ok=True)
    Path(output).write_bytes(out)
    # the animation graphs' frames, shared by content
    from .level import share_file, write_list
    anim_dir = (Path(filesystem) if filesystem is not None else Path(output).parent) / "anim"
    anim_dir.mkdir(parents=True, exist_ok=True)
    names = []
    for i, graph in enumerate(repacker.policy.graphs):
        made = anim_dir / f".{xmap.name}-{i}.anim"
        made.write_bytes(bytes(repacker.policy.streams[graph]))
        names.append(share_file(made, anim_dir, ".anim"))
    write_list(Path(output).with_suffix(".anims"), names)
    # the sounds' files (soundbank.py packs them)
    sounds = repacker.policy.sounds
    if sounds is not None:
        files, made = sounds.write(Path(filesystem).parent / "sound" / f"{xmap.name}.slist")
        print(f"{xmap.name} sounds: {files} files ({made} new), {sounds.seconds:.0f} s")
    # a campaign level's encounters (campaign.py encounter_table)
    if campaign:
        enc = (Path(filesystem) if filesystem is not None else Path(output).parent) / "levels" / f"{xmap.name}.enc"
        enc.parent.mkdir(parents=True, exist_ok=True)
        enc.write_bytes(repacker.policy.encounters)
    return xmap, repacker, len(out), relocs


# campaign scenery the players go inside: its highest level of detail (the
# lower ones are its outside alone), with this many triangles
INTERIOR_MODELS = ("lifepod_crashed",)
INTERIOR_BUDGET = 2000


def write_campaign_assets(xmap, repacker, filesystem):
    """a campaign level's render geometry, models and particles: the level
    streamed by cluster (level.py, version 5: too big to keep in memory),
    lit at its triangles' corners; the characters at their lowest level of
    detail; one player's sets only (no lighter set for split screen)"""
    from .effects import EffectParticles
    from .level import LevelBuilder, write_list
    from .models import ModelConverter
    builder = LevelBuilder(xmap, repacker.defs, xmap.name, filesystem, split_depth=0)
    builder.greyscale = builder.terrain_detail = True
    builder.chicago_water = True
    # (no colour tiles, terrain_tiles.py: ~85 KB of a campaign level's heap,
    # which its models' cache and Halo's objects need -- the ground's colour
    # with its detail maps' tint on its vertices instead)
    # (no detail drawn over rock and earth: a second pass of each, three
    # times the batches)
    builder.streamed = True
    # (an earlier build's tiles would still be loaded beside the level)
    (filesystem / "levels" / f"{xmap.name}.tiles").unlink(missing_ok=True)
    stats = builder.write(filesystem / "levels" / f"{xmap.name}.lvl", xmap.bsps[0][0])
    print(f"{xmap.name} level: " + ", ".join(f"{k} {v}" for k, v in stats.items()))
    characters = [t.index for t in xmap.tags if t.group_name == "mode" and t.name.startswith("characters\\")
                  and "\\fp\\" not in t.name]
    lowest = ("super low", "low", "medium", "high", "super high")
    # scenery the players stand in (a30's crashed lifepods: the level
    # starts inside one): only the highest level of detail has the inside
    highest = ("super high", "high", "medium", "low", "super low")
    insides = [t.index for t in xmap.tags if t.group_name == "mode" and t.name.endswith(INTERIOR_MODELS)]
    model_sets = []
    for quality, view_scale in VIEW_MODEL_QUALITIES:
        suffix = f"-fp{quality}" if quality else ""
        models = ModelConverter(xmap, repacker.defs, xmap.name + suffix, filesystem)
        models.lod_preferences = {i: lowest for i in characters}
        models.budget_overrides = {i: 250 for i in characters}
        models.lod_preferences.update({i: highest for i in insides})
        models.budget_overrides.update({i: INTERIOR_BUDGET for i in insides})
        models.first_person_budget = int(models.first_person_budget * view_scale)
        stats = models.write(filesystem / "levels" / f"{xmap.name}{suffix}.mdl")
        print(f"{xmap.name}{suffix} models: " + ", ".join(f"{k} {v}" for k, v in stats.items()))
        model_sets.append(models)
    stats = EffectParticles(xmap, repacker.defs).write(filesystem / "fx" / f"{xmap.name}.fx")
    print(f"{xmap.name} particles: " + ", ".join(f"{k} {v}" for k, v in stats.items()))
    write_list(filesystem / "tex" / f"{xmap.name}.list",
               sorted(builder.used_textures.union(*(m.textures.used_textures for m in model_sets))))


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("map", type=Path)
    parser.add_argument("-o", "--output", type=Path, required=True)
    parser.add_argument("--stats", action="store_true")
    parser.add_argument("--filesystem", type=Path,
                        help="the cartridge's file tree: the level's render geometry goes to "
                        "levels/<map>.lvl and its textures to tex/<map>/")
    parser.add_argument("--campaign", action="store_true",
                        help="a campaign level for a cartridge of its own (tools/n64/campaign.py)")
    parser.add_argument("--pc-expansion", action="store_true",
                        help="the separate PC multiplayer expansion cartridge")
    parser.add_argument("--pc-maps", type=Path,
                        help="explicit folder containing all six normalized Xbox-v5 PC maps")
    parser.add_argument("--tags-only", action="store_true", help="the tag image alone (measuring)")
    args = parser.parse_args()
    xmap, repacker, size, relocs = convert(args.map, args.output, None if args.tags_only else args.filesystem,
                                           campaign=args.campaign, pc_expansion=args.pc_expansion,
                                           pc_maps=args.pc_maps)
    print(f"{xmap.name}: {size / 1e6:.2f} MB, {len(relocs)} relocations, "
          f"{repacker.policy.stream_size / 1e6:.2f} MB of animation frames streamed")
    if args.filesystem and not args.tags_only and args.campaign:
        write_campaign_assets(xmap, repacker, args.filesystem)
    elif args.filesystem and not args.tags_only:
        from .level import LevelBuilder
        builder = LevelBuilder(xmap, repacker.defs, xmap.name, args.filesystem)
        # textures without colour of their own in greyscale from the larger
        # original images (level.py grey_texture), natural terrain drawn with
        # its detail maps (terrain_materials)
        builder.greyscale = builder.terrain_detail = True
        # rock, earth, snow: their detail maps drawn over them (not in the
        # lighter level of 3-4 player games)
        builder.overlay_detail = True
        # the ground's colour map in tiles, the light baked in
        # (terrain_tiles.py; not in the lighter level either)
        builder.colour_tiles = True
        stats = builder.write(args.filesystem / "levels" / f"{xmap.name}.lvl", xmap.bsps[0][0])
        print(f"{xmap.name} level: " + ", ".join(f"{k} {v}" for k, v in stats.items()))
        # the lighter level of 3-4 player games: the BSP's own triangles,
        # lit at their corners
        low = LevelBuilder(xmap, repacker.defs, xmap.name, args.filesystem, split_depth=0)
        low.greyscale = low.terrain_detail = True
        stats = low.write(args.filesystem / "levels" / f"{xmap.name}-low.lvl", xmap.bsps[0][0])
        print(f"{xmap.name} low level: " + ", ".join(f"{k} {v}" for k, v in stats.items()))
        from .models import ModelConverter
        # Firefight's Covenant (imported: merge.py): always their lowest
        # level of detail, few triangles
        covenant = [t.index for t in xmap.tags if getattr(getattr(t, "source", None), "name", None) == FIREFIGHT_SOURCE
                    and t.group_name == "mode"]
        lowest = ("super low", "low", "medium", "high", "super high")
        model_sets = []
        for quality, view_scale in VIEW_MODEL_QUALITIES:
            suffix = f"-fp{quality}" if quality else ""
            models = ModelConverter(xmap, repacker.defs, xmap.name + suffix, args.filesystem)
            models.lod_preferences = {i: lowest for i in covenant}
            models.budget_overrides = {i: 250 for i in covenant}
            models.first_person_budget = int(models.first_person_budget * view_scale)
            stats = models.write(args.filesystem / "levels" / f"{xmap.name}{suffix}.mdl")
            print(f"{xmap.name}{suffix} models: " + ", ".join(f"{k} {v}" for k, v in stats.items()))
            # the lighter models of 3-4 player games (the objects at half
            # their triangles, the first-person views at 350)
            low_models = ModelConverter(xmap, repacker.defs, f"{xmap.name}-low{suffix}", args.filesystem)
            low_models.budget_scale = 0.5
            low_models.first_person_budget = int(350 * view_scale)
            low_models.first_person_deviation = 0.06
            low_models.detail_atlas = 64
            low_models.lod_preferences = {i: lowest for i in covenant}
            low_models.budget_overrides = {i: 150 for i in covenant}
            stats = low_models.write(args.filesystem / "levels" / f"{xmap.name}-low{suffix}.mdl")
            print(f"{xmap.name}-low{suffix} models: " + ", ".join(f"{k} {v}" for k, v in stats.items()))
            model_sets += [models, low_models]
        # the effects' particles (port/n64/src/n64_particles.c)
        from .effects import EffectParticles
        stats = EffectParticles(xmap, repacker.defs).write(args.filesystem / "fx" / f"{xmap.name}.fx")
        print(f"{xmap.name} particles: " + ", ".join(f"{k} {v}" for k, v in stats.items()))
        # the shared files the map uses (tools/n64/prune.py removes the rest)
        from .level import write_list
        write_list(args.filesystem / "tex" / f"{xmap.name}.list",
                   sorted(builder.used_textures.union(*(m.textures.used_textures for m in model_sets))))
    if args.stats:
        for g, s in sorted(repacker.stats.items(), key=lambda x: -x[1])[:25]:
            print(f"  {g} {s / 1024:8.1f} KB")


if __name__ == "__main__":
    main()
