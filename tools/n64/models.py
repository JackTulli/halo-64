"""Converting a map's object models (mode tags) for the N64 renderer.

Every model an object of the map uses is written at one level of detail
("low", or the nearest that exists) as tiny3d batches, like the level's
(level.py), but skinned: each vertex belongs to one node (the heavier of its
two), is stored in that node's space (the node's inverse default transform,
which the cache file keeps with each node, applied ahead of time) and is
loaded under that node's matrix. A batch is a list of segments -- (node,
vertices) -- loaded into the RSP's 70-vertex cache, then triangles that may
use vertices of several nodes, which stretches the skin between them.

A first-person model's texture is usually one atlas for the whole weapon
(512x512), far more than the 64x64 a texture can be in TMEM. Its triangles
are grouped into windows of the atlas (at DETAIL_ATLAS texels a side) that
fit 64x64, each window its own texture with its own palette; triangles too
big for any window keep the whole atlas at 64x64.

There is no lighting on the RSP for models (the first milestone): each
vertex carries a sky light from above, baked against its default-pose
normal, which stays right while a model turns about its up axis.

The output file (big-endian):

    header        'H64O', version, counts, offsets (the tables: everything up to
                  the first model's data, which the N64 keeps in memory)
    textures[]    name[24] (rom:/tex/<map>/<name>.sprite), width, height
    models[]      tag index, node count, region count, first region, first
                  batch, batch count, data name[16] (rom:/mdl/<name>.bin,
                  shared by content), vertex bytes, index bytes,
                  first far batch, far batch count (the far version: the
                  whole model at a quarter of the triangles, untextured,
                  coloured with its textures' average colours; no far
                  version for players and first-person models)
    regions[]     permutation count, first permutation
    permutations[] geometry (index into geometries, -1 for none)
    geometries[]  first batch, batch count
    batches[]     texture, flags, segment count, triangle count, first segment, index offset
    segments[]    node, cache slot, vertex count, vertex pair offset
    model data    per model, a file of its own: its vertices (T3DVertPacked,
                  positions in node space * MODEL_SCALE), then its indices
                  (three u8 per triangle: cache slots); a batch's segments and
                  indices are offsets into its model's data. The N64 reads a
                  model's data from the cartridge when it is first drawn.
"""

import math
import os
import struct
from pathlib import Path

from PIL import Image

from .bitmaps import bitmap_images, decode
from .level import is_light_volume
from .stages import evaluate as evaluate_stages
from .decimate import decimate
from .level import (LevelBuilder, MATERIAL_ALPHA_TEST, MATERIAL_BLEND, MATERIAL_CHANGE_COLOR, MATERIAL_GLOW,
                    MATERIAL_TRANSPARENT, MATERIAL_TWO_SIDED, MATERIAL_UNTEXTURED, GREYSCALE_TEXELS, Texture,
                    pack_normal, unpack_normal, texture_choices)

MODEL_SCALE = 1024.0
MAXIMUM_CACHE = 70
# a batch's texture coordinates span at most this many texels (centred on
# 0: s10.5 holds +-1024), where a part's span more (build_batches)
MODEL_UV_EXTENT = 1792.0
# (the converter's own flag, not written: a part of an alpha-blended decal
# shader, left out of the far version)
DECAL = 0x8000
VERSION = 4
LOD_PREFERENCE = ("low", "super low", "medium", "high", "super high")
OBJECT_GROUPS = ("bipd", "vehi", "weap", "eqip", "garb", "proj", "scen", "mach", "ctrl", "lifi", "plac", "ssce")
SKY_LIGHT = (0.3, 0.2, 0.93)
# triangles a model may keep (tools/n64/decimate.py): the first-person
# models are drawn in every view
FIRST_PERSON_BUDGET = int(os.environ.get("N64_FIRST_PERSON_BUDGET", "500"))
# the first-person hands (their fingers need the triangles)
HANDS_BUDGET = FIRST_PERSON_BUDGET
MODEL_BUDGET = 400
# how far (a share of the part's size) the simplifier may move a
# first-person weapon's surface (with the budget alone the pistol lost its
# muzzle); the others keep their budget alone
FIRST_PERSON_DEVIATION = float(os.environ.get("N64_FIRST_PERSON_DEVIATION", "0.015"))
# a far version keeps this share of the model's triangles (at least
# FAR_MINIMUM)
FAR_SHARE = 0.25
FAR_MINIMUM = 24
# a far version with cut-out parts is kept only below this share of the
# near version's triangles
FAR_CUTOUT_SHARE = 0.75
# by the object's group: players matter most; what lies on the floor or
# flies is small on screen
GROUP_BUDGETS = {"bipd": 600, "vehi": 600, "weap": 250, "eqip": 250, "garb": 250, "proj": 150}
# static objects (scenery, machines...) bigger than this many triangles are
# pieces of the level (Custom Edition maps: Foundry's warehouse shell, pipes
# and roof are machines): they keep this share of their triangles (not a
# prop's budget) and have no far version
STATIC_GROUPS = ("scen", "mach", "ctrl", "lifi", "plac", "ssce")
BIG_STATIC_TRIANGLES = 2000
BIG_STATIC_SHARE = 0.12
# a part the simplifier left above this many times its target is simplified
# again with decimate's open_edges (open shapes: pipes, open boxes)
OPEN_EDGE_RETRY = 1.5
# first-person textures: the atlas at this size (each window is a texture
# load and a batch more: 256 cost the RSP/RDP ~15 ms a frame more than 128),
# cut into windows of at most
# WINDOW texels a side (CI4, 2 KB of TMEM); a window keeps a texel of
# margin for the filtering
DETAIL_ATLAS = int(os.environ.get("N64_DETAIL_ATLAS", "128"))
WINDOW = 64
DETAIL_WINDOWS = os.environ.get("N64_DETAIL_WINDOWS", "1") == "1"
# prepare_pc_content.py builds these owned PC maps into Xbox-format
# intermediates. Invader changes the cache layout, not the bitmap channels.
# Imported tags retain their source map through merge_tags.
PC_INTERMEDIATE_MAPS = frozenset(("dangercanyon", "deathisland", "gephyrophobia", "icefields", "infinity", "timberland"))


def matrix4x3(values):
    """(scale, forward, left, up, position) from 13 floats"""
    return values[0], values[1:4], values[4:7], values[7:10], values[10:13]


def transform_point(m, p):
    scale, f, l, u, t = m
    x, y, z = (c * scale for c in p)
    return (u[0] * z + l[0] * y + f[0] * x + t[0],
            u[1] * z + l[1] * y + f[1] * x + t[1],
            u[2] * z + l[2] * y + f[2] * x + t[2])


# the powerups' energy (stages_texture): brightened by about 1 / the blend's
# alpha (GLOW_ALPHA, n64_render.c: 112 of 255), transparent below the floor
POWERUP_ENERGY_GAIN = 2.25
POWERUP_ENERGY_FLOOR = 20


def is_powerup_energy(shader_name):
    """the overshield's and active camouflage's energy shaders
    (powerups\\over shield\\shaders\\over shield, powerups\\active
    camoflage\\shaders\\active camoflage): not the ammunition's glows"""
    name = shader_name.lower()
    return name.startswith("powerups\\") and "ammo" not in name


def part_size(vertices, triangles):
    """the diagonal of the box around a part's triangles (node space, which
    for a part on few nodes is close enough)"""
    used = {i for t in triangles for i in t}
    if not used:
        return 0.0
    lo = [min(vertices[i][1][k] for i in used) for k in range(3)]
    hi = [max(vertices[i][1][k] for i in used) for k in range(3)]
    return math.sqrt(sum((b - a) ** 2 for a, b in zip(lo, hi)))


def split_painted_faces(vertices, triangles, atlas_size, span=WINDOW - 2):
    """Keep rigid painted panels out of the coarse whole-gun fallback.

    Subdivide only edges on the same bone; interpolating across two local
    bone spaces would deform the weapon. A fixed depth bounds the work.
    """
    out, midpoints = [], {}
    pending = [(tuple(t), 0) for t in triangles]
    while pending:
        tri, depth = pending.pop()
        uv = [vertices[i][2] for i in tri]
        fits = all((max(p[k] for p in uv) - min(p[k] for p in uv)) * atlas_size[k] <= span
                   for k in range(2))
        if fits or depth >= 8 or len({vertices[i][0] for i in tri}) != 1:
            out.append(tri)
            continue
        edges = [(tri[0], tri[1], tri[2]), (tri[1], tri[2], tri[0]), (tri[2], tri[0], tri[1])]
        a, b, c = max(edges, key=lambda e: sum(((vertices[e[0]][2][k] - vertices[e[1]][2][k]) * atlas_size[k])**2
                                               for k in range(2)))
        key = tuple(sorted((a, b)))
        if key not in midpoints:
            va, vb = vertices[a], vertices[b]
            average = lambda x, y: tuple((p + q) * 0.5 for p, q in zip(x, y))
            normal = average(va[4], vb[4])
            length = math.sqrt(sum(n*n for n in normal)) or 1.0
            normal = tuple(n / length for n in normal)
            shade = average(va[3], vb[3]) if isinstance(va[3], tuple) else (va[3] + vb[3]) * 0.5
            midpoints[key] = len(vertices)
            vertices.append((va[0], average(va[1], vb[1]), average(va[2], vb[2]), shade, normal))
        middle = midpoints[key]
        pending.extend((((a, middle, c), depth + 1), ((middle, b, c), depth + 1)))
    return out


def weld_nodes(vertices, world, nodes_inverse):
    """the vertices at one position all on the node most of them have (each
    vertex is on one node: two at a point on different nodes part as the
    model moves, a crack, and the simplifier must leave them be)"""
    by_position = {}
    for i, p in enumerate(world):
        by_position.setdefault(tuple(round(c, 5) for c in p), []).append(i)
    out = list(vertices)
    for members in by_position.values():
        nodes = [vertices[i][0] for i in members]
        if len(set(nodes)) < 2:
            continue
        node = max(set(nodes), key=nodes.count)
        for i in members:
            v = vertices[i]
            out[i] = (node, transform_point(nodes_inverse[node], world[i])) + tuple(v[2:])
    return out


def strip_triangles(indices):
    """a Direct3D triangle strip as triangles, degenerate ones dropped,
    each wound as the strip's first (clockwise front faces)"""
    out = []
    for i in range(len(indices) - 2):
        a, b, c = indices[i], indices[i + 1], indices[i + 2]
        if a == b or b == c or a == c or 0xFFFF in (a, b, c):
            continue
        out.append((a, b, c) if i % 2 == 0 else (b, a, c))
    return out


def vertex_node(n0, n1, weight, node_count):
    """Dominant bone of a Halo compressed vertex.

    The weight uses Halo's signed-normalized 16-bit encoding: 32767 is
    +1, not one half. PC rigid vertices often retain a valid node1=0 with
    zero influence, while Xbox rigid vertices usually mark node1 absent.
    Dividing by 65535 attached nearly all PC meshes to that zero-weight
    second bone, freezing turrets and stretching animated player meshes.
    """
    decoded = (weight & 0x7FFF) / 32767.0 - (1.0 if weight & 0x8000 else 0.0)
    decoded = min(1.0, max(0.0, decoded))
    node0 = n0 // 3 if n0 >= 0 else -1
    node1 = n1 // 3 if n1 >= 0 else -1
    node = node0 if node1 < 0 or (node0 >= 0 and decoded >= 0.5) else node1
    return node if 0 <= node < node_count else 0


class ModelConverter:
    # the reductions for open shapes and big static models (OPEN_EDGE_RETRY,
    # BIG_STATIC_TRIANGLES): Custom Edition maps' (convert.py --custom); the
    # stock maps are converted as they always were
    reduce_open_shapes = False

    def __init__(self, xmap, defs, map_name, out_dir):
        self.map = xmap
        self.defs = defs
        # textures go through the level builder's writer, named m00...
        self.textures = LevelBuilder(xmap, defs, map_name, out_dir)
        self.textures.texture_prefix = "m"
        # Halo doubles a model's colour, its lighting being kept at half;
        # the sky light baked here is whole, so the texture is not doubled
        self.textures.texture_brightness = 1.0
        # (base map, multipurpose map) -> texture with a split palette
        self.masked = {}
        # glow textures of one emitted colour (glow_texture: MATERIAL_BLEND)
        self.solid_glows = set()
        # by model tag index: a factor for its vertices' positions, flags
        # for its batches, no far version (tools/n64/ui.py's sky)
        self.position_scales = {}
        self.extra_flags = {}
        self.no_far = set()
        self.current_scale = 1.0
        self.current_flags = 0
        self.models = []
        # the lighter set of 3-4 player games: the objects' budgets scaled,
        # the first-person models' (weapons and hands) set
        self.budget_scale = 1.0
        self.first_person_budget = FIRST_PERSON_BUDGET
        # how far the first-person weapons' simplifier may move their
        # surface, and their atlas's size (the 3-4 player set's views are a
        # quarter of the screen: a looser bound reaches the budget, one
        # 64-texel atlas is one texture load a part instead of several)
        self.first_person_deviation = FIRST_PERSON_DEVIATION
        self.detail_atlas = DETAIL_ATLAS
        # by model tag index: another order of levels of detail to take
        # (Firefight's Covenant: their lowest)
        self.lod_preferences = {}
        # by model tag index: a budget that overrides its group's
        self.budget_overrides = {}
        # the texture picker's choices of the objects' textures (by bitmap
        # name, "<name> (first person)" for a first-person atlas: level.py
        # texture_choices), and what a grey texture's vertices are
        # multiplied by (its stretch undone, times its tint: by texture)
        self.object_choices = texture_choices("objects")
        self.texture_colour = {}

    def f(self, name):
        return {fl.name: o for o, fl in self.defs.fields(name)}

    def block(self, address):
        return struct.unpack("<iI", self.map.read(address, 8))

    def object_models(self):
        """the model tags the map's objects use"""
        of = self.f("Object")
        found = []
        self.budgets = {}
        self.first_person = set()
        self.hands = set()
        # models static objects use (big ones: BIG_STATIC_TRIANGLES)
        self.static = set()
        # models a unit (a player) uses keep their textures at any distance
        # (a player's colour)
        self.units = set()
        for tag in self.map.tags:
            if tag.group_name not in OBJECT_GROUPS:
                continue
            datum = struct.unpack("<I", self.map.read(tag.address + of["model"] + 12, 4))[0]
            model = self.map.tag(datum)
            if model is not None and model.group_name == "mode":
                if model not in found:
                    found.append(model)
                # a model several groups use keeps the largest budget
                budget = int(GROUP_BUDGETS.get(tag.group_name, MODEL_BUDGET) * self.budget_scale)
                self.budgets[model.index] = max(self.budgets.get(model.index, 0), budget)
                if tag.group_name in ("bipd", "vehi"):
                    self.units.add(model.index)
                if tag.group_name in STATIC_GROUPS:
                    self.static.add(model.index)
        # the first-person views: each weapon's model, and the hands
        wf = self.f("Weapon")
        for tag in self.map.tags:
            if tag.group_name == "weap":
                datum = struct.unpack("<I", self.map.read(tag.address + wf["first person model"] + 12, 4))[0]
                model = self.map.tag(datum)
                if model is not None and model.group_name == "mode" and model not in found:
                    found.append(model)
                if model is not None:
                    self.budgets[model.index] = self.first_person_budget
                    self.first_person.add(model.index)
        gf = self.f("Globals")
        for tag in self.map.tags:
            if tag.group_name == "matg":
                count, pointer = self.block(tag.address + gf["first person interface"])
                if count > 0:
                    datum = struct.unpack("<I", self.map.read(pointer + 12, 4))[0]
                    model = self.map.tag(datum)
                    if model is not None and model.group_name == "mode" and model not in found:
                        found.append(model)
                    if model is not None:
                        self.budgets[model.index] = self.first_person_budget
                        self.first_person.add(model.index)
                        self.hands.add(model.index)
        self.budgets.update(self.budget_overrides)
        return found

    def shader_texture(self, shader_datum):
        """(texture, material flags, base map's bitmap or None) of a model's shader (tools/n64/level.py's
        MATERIAL_* bits): two-sided shaders are drawn without culling"""
        tag = self.map.tag(shader_datum)
        if tag is None:
            return -1, 0, None
        if tag.group_name == "soso":
            so = self.f("ShaderModel")
            base = struct.unpack("<I", self.map.read(tag.address + so["base map"] + 12, 4))[0]
            model_flags = struct.unpack("<H", self.map.read(tag.address + so["shader model flags"], 2))[0]
            flags = MATERIAL_TWO_SIDED if model_flags & 2 else 0
            # Native Xbox multipurpose maps use blue; the PC intermediates
            # retain PC pixels (alpha), even though their cache format is
            # Xbox. Bit 6 explicitly selects Xbox order for PC shaders.
            change_source = struct.unpack("<H", self.map.read(tag.address + so["change color source"], 2))[0]
            multipurpose = struct.unpack("<I", self.map.read(tag.address + so["multipurpose map"] + 12, 4))[0]
            channel = self.change_channel(tag, model_flags)
            # Halo tests the base map's alpha (at 0x7F) unless the shader says
            # "not alpha tested"; an alpha-blended decal (bit 3) is blended
            # by it -- here both cut out, where the base map has transparent
            # texels (Sidewinder's snow pines' needles, Blood Gulch's shrubs
            # and plants were solid cards; the weapons' decals were drawn
            # whole over their bodies)
            cut = (not model_flags & 4 or model_flags & 8) and self.textures.has_cutout(base)
            cut_flags = (MATERIAL_ALPHA_TEST | (DECAL if model_flags & 8 else 0)) if cut else 0
            texture = self.chosen_texture(base, bool(cut), None,
                                          player_colour=bool(change_source and self.map.tag(multipurpose) is not None))
            if texture is not None:
                return texture, flags | cut_flags, (base, None)
            if change_source and self.map.tag(multipurpose) is not None:
                texture = self.masked_texture(base, multipurpose, cut, channel)
                if texture >= 0:
                    return texture, flags | MATERIAL_CHANGE_COLOR | cut_flags, (base, multipurpose, channel)
            if cut:
                return self.textures.cutout_texture(base)[0], flags | cut_flags, (base, None)
            return self.textures.texture(base), flags, (base, None)
        if tag.group_name == "senv":
            so = self.f("ShaderEnvironment")
            base = struct.unpack("<I", self.map.read(tag.address + so["base map"] + 12, 4))[0]
            env_flags = struct.unpack("<H", self.map.read(tag.address + so["shader environment flags"], 2))[0]
            if env_flags & 1:
                bump = struct.unpack("<I", self.map.read(tag.address + so["bump map"] + 12, 4))[0]
                scale = struct.unpack("<f", self.map.read(tag.address + so["bump map scale"], 4))[0] or 1.0
                texture = self.chosen_texture(base, True, (bump, scale))
                if texture is not None:
                    return texture, MATERIAL_ALPHA_TEST, None
                # Keep the composed mask intact; base-only atlas windows
                # would throw the bump alpha away again.
                return self.textures.environment_texture(base, bump, scale), MATERIAL_ALPHA_TEST, None
            texture = self.chosen_texture(base, False, None)
            if texture is not None:
                return texture, 0, (base, None)
            return self.textures.texture(base), 0, (base, None)
        # transparent shaders (glowing screens, sights, meters): meters and
        # numeric displays need Halo's own drawing and are left out (texture
        # None: the part is not converted);
        # the rest are cut-out glows of their first map
        if tag.group_name == "smet":
            return None, 0, None
        # light volumes, left out: the spotlights' cones on Rat Race,
        # Derelict and Damnation (car_spotlight, dam_spotlight:
        # b40_volumelight_nolight), faint glows in Halo, drawn here as solid
        # cones by the glow below -- triangles for nothing
        if is_light_volume(tag.name):
            return None, 0, None
        flags = MATERIAL_TRANSPARENT | MATERIAL_GLOW
        # the transparent shaders' own flags: "two sided" is bit 2, "numeric"
        # bit 7
        for struct_name, field, group in (("ShaderTransparentChicago", "shader transparent chicago flags", "schi"),
                                          ("ShaderTransparentChicagoExtended", "shader transparent chicago extended flags", "scex"),
                                          ("ShaderTransparentGeneric", "shader transparent generic flags", "sotr"),
                                          ("ShaderTransparentGlass", "shader transparent glass flags", "sgla")):
            if tag.group_name == group:
                fields = self.f(struct_name)
                value = self.map.read(tag.address + fields[field], 1)[0] if field in fields else 0
                if value & 4:
                    flags |= MATERIAL_TWO_SIDED
                if value & 0x80 and group != "sgla":
                    return None, 0, None
        # the needler's glass (its needles in first person, the needler
        # ammo's crystals): the glass's first map is its reflection cube, a
        # grey one -- the needles were grey; its colour is its tint, drawn
        # as a translucent glow of it (the other glass, clear or faintly
        # tinted, stays as it was)
        if tag.group_name == "sgla" and "needler" in tag.name.lower():
            texture = self.glass_tint_texture(tag)
            if texture >= 0:
                return texture, flags | MATERIAL_BLEND, None
        # the powerups' glass panels, near clear in Halo (a faint blue tint,
        # faint reflections), left out: cut-outs of their grey reflection
        # cube, they were grey patches over the energy inside
        if tag.group_name == "sgla" and tag.name.lower().endswith("\\powerup glass"):
            return None, 0, None
        # the powerups' energy (the overshield's red, active camouflage's
        # blue: grey noise maps coloured by the shader's stages, added to the
        # frame in Halo): its stages evaluated into a coloured texture, drawn
        # translucent -- its first map alone was a grey cut-out
        if tag.group_name == "sotr" and is_powerup_energy(tag.name):
            texture = self.stages_texture(tag)
            if texture >= 0:
                return texture, flags | MATERIAL_BLEND, None
        texture = self.glow_texture(tag)
        # a glow of one emitted colour (a teleporter's energy) is drawn
        # translucent: solid, Sidewinder's teleporters were opaque green slabs
        if texture in self.solid_glows:
            flags |= MATERIAL_BLEND
        return texture, flags, None

    def object_choice(self, bitmap_datum, first_person=False):
        """the texture picker's (option, tint or None) for an object's
        bitmap (its first-person atlas's), or None"""
        tag = self.map.tag(bitmap_datum) if bitmap_datum != 0xFFFFFFFF else None
        if tag is None:
            return None
        return self.object_choices.get(tag.name.lower() + (" (first person)" if first_person else ""))

    def chosen_texture(self, base, cutout, alpha_source, player_colour=False):
        """an object texture in the format the texture picker chose for it
        (a third-person one), or None to make it as before: greyscale (I4;
        IA4 cut out) with its vertices multiplied by its stretch undone
        and its tint (texture_colour), or full colour (RGBA16 64x32). A
        texture with the player's colour keeps its 16 colours, the palette's
        upper half the player's -- unless the picker chose otherwise, which
        it said would lose the colour"""
        choice = self.object_choice(base)
        if choice is None or choice[0] == "ci4-64x64":
            return None
        if player_colour:
            # (the picker said so: its other options lose the colour change)
            print(f"{self.map.name}: {self.map.tag(base).name}: {choice[0]}, without the player's colour")
        if choice[0] == "colour-32x64":
            tag = self.map.tag(base)
            img = decode(self.map, bitmap_images(self.map, tag, self.defs)[0], 64)
            if cutout and alpha_source is not None:
                img = self.textures.environment_alpha(img, *alpha_source)
            return self.textures.write_texture(img, tag.name, fmt="RGBA16", cutout=cutout, box=(64, 32))
        return self.grey_texture(base, cutout, alpha_source, choice)

    def grey_texture(self, base, cutout, alpha_source, choice):
        """level.py's grey texture of a bitmap (choice: the texture
        picker's, its texels and tint), its vertices' multiplier noted, the
        far version's colour the grey's mean times the tint"""
        option, tint = choice
        limit = GREYSCALE_TEXELS if option.startswith("grey-128x64") else GREYSCALE_TEXELS // 2
        texture, shade = self.textures.grey_texture(base, cutout, alpha_source, limit)
        if texture < 0:
            return None
        tint = (tint or self.textures.grey_tint(base)) if option.endswith("-tint") else (1.0, 1.0, 1.0)
        self.texture_colour[texture] = tuple(shade * t for t in tint)
        t = self.textures.textures[texture]
        t.colour = tuple(min(1.0, c * k) for c, k in zip(t.colour, tint))
        return texture

    def grey_window(self, grey, alpha, name, colour):
        """an atlas window of a grey first-person texture (I4; IA4 cut
        out): its texture, its vertices' multiplier noted"""
        index = len(self.textures.textures)
        work_dir = self.textures.out_dir / "tex" / f".{self.textures.map_name}"
        work_dir.mkdir(parents=True, exist_ok=True)
        png = work_dir / f"{self.textures.texture_prefix}w{index:02d}.png"
        if alpha is not None:
            Image.merge("LA", (grey, alpha)).save(png)
        else:
            grey.save(png)
        sprite = self.textures.make_sprite(png, "IA4" if alpha is not None else "I4")
        mean = sum(grey.getdata()) / (grey.width * grey.height) / 255.0
        self.textures.textures.append(Texture(sprite, grey.width, grey.height,
                                              tuple(min(1.0, mean * c) for c in colour)))
        self.texture_colour[index] = colour
        return index

    def stages_texture(self, shader_tag):
        """a transparent generic shader's stages evaluated (tools/n64/stages.py)
        as an RGBA16 texture for a translucent glow (MATERIAL_BLEND): Halo
        adds the colour to the frame, the N64 blends it at GLOW_ALPHA, so the
        colour is brightened by about as much, and the texels that add next
        to nothing are transparent (blended, they would darken); or -1"""
        key = ("stages", shader_tag.index)
        if key in self.masked:
            return self.masked[key]
        index = -1
        img = evaluate_stages(self.map, self.defs, shader_tag, 32)
        if img is not None:
            rgb = img.convert("RGB").point(lambda v: min(255, int(v * POWERUP_ENERGY_GAIN)))
            seen = [px for px in rgb.getdata() if max(px) >= POWERUP_ENERGY_FLOOR]
            if seen:
                # (the transparent texels take the mean colour: filtered, they
                # would otherwise darken the edges)
                mean = tuple(sum(px[k] for px in seen) // len(seen) for k in range(3))
                out = Image.new("RGBA", rgb.size)
                out.putdata([px + (255,) if max(px) >= POWERUP_ENERGY_FLOOR else mean + (0,)
                             for px in rgb.getdata()])
                index = self.textures.write_texture(out, shader_tag.name, fmt="RGBA16")
        self.masked[key] = index
        return index

    def glass_tint_texture(self, shader_tag):
        """a glass shader's tint seen face on (its perpendicular tint, else
        its background tint) as a solid glow, or -1 if it has none"""
        key = ("glass", shader_tag.index)
        if key in self.masked:
            return self.masked[key]
        index = -1
        fields = self.f("ShaderTransparentGlass")
        for field in ("perpendicular tint color", "background tint color"):
            if field not in fields:
                continue
            tint = struct.unpack("<3f", self.map.read(shader_tag.address + fields[field], 12))
            if max(tint) > 0.05:
                colour = tuple(int(max(0.0, min(1.0, c)) * 255) for c in tint) + (255,)
                index = self.textures.write_texture(Image.new("RGBA", (4, 4), colour), shader_tag.name, fmt="RGBA16")
                self.solid_glows.add(index)
                break
        self.masked[key] = index
        return index

    def glow_texture(self, shader_tag):
        """a transparent shader's first map as a cut-out glow (RGBA16): its
        brighter texels, the rest transparent (the RDP's additive blending
        wraps around)"""
        found = []

        def on_dependency(t, address, f, path, group, datum):
            if group == 0x6269746D and datum != 0xFFFFFFFF and not found:  # 'bitm'
                found.append(datum)

        from .walk import Walker
        Walker(self.map, self.defs).walk(shader_tag, on_dependency=on_dependency)
        if not found:
            return -1
        key = ("glow", found[0], shader_tag.index)
        if key in self.masked:
            return self.masked[key]
        index = -1
        tag = self.map.tag(found[0])
        images = bitmap_images(self.map, tag, self.defs) if tag is not None else []
        if images:
            try:
                img = decode(self.map, images[0], 32)
            except ValueError:
                img = None
            if img is not None:
                r, g, b, a = img.convert("RGBA").split()
                # a grey map (a mask: its colour comes from the shader's
                # other layers, as the teleporters' energy) takes the colour
                # the shader emits, solid
                emitted = struct.unpack("<3f", self.map.read(shader_tag.address + self.f("Shader")["color of emitted light"], 12))
                spread = max(abs(x - y) for x, y in zip(r.getextrema() + g.getextrema(), g.getextrema() + b.getextrema()))
                if spread < 24 and max(emitted) - min(emitted) > 0.2:
                    colour = tuple(int(max(0.0, min(1.0, c)) * 255) for c in emitted) + (255,)
                    img = Image.new("RGBA", (4, 4), colour)
                    index = self.textures.write_texture(img, tag.name, fmt="RGBA16")
                    self.solid_glows.add(index)
                else:
                    brightness = Image.merge("RGB", (r, g, b)).convert("L").point(lambda v: 255 if v >= 48 else 0)
                    index = self.textures.write_texture(Image.merge("RGBA", (r, g, b, brightness)), tag.name, fmt="RGBA16")
        self.masked[key] = index
        return index

    def change_channel(self, shader, flags):
        """Pixel layout follows an imported shader's source, not the map
        receiving it. Native Xbox maps predate the explicit PC Xbox-order
        flag; their multipurpose pixels always use blue.
        """
        source = getattr(shader, "source", None) or self.map
        return "A" if source.name in PC_INTERMEDIATE_MAPS and not flags & 64 else "B"

    def change_mask(self, multipurpose, size, channel="B"):
        """The source shader's colour-change channel at size."""
        tag = self.map.tag(multipurpose)
        images = bitmap_images(self.map, tag, self.defs) if tag is not None else []
        if not images:
            return None
        try:
            img = decode(self.map, images[0], max(size))
        except ValueError:
            return None
        return img.getchannel(channel).resize(size, Image.BILINEAR)

    def masked_texture(self, base, multipurpose, cutout=False, channel="B"):
        """the base map whole at 64x64, its palette split by the colour-change
        mask (and its transparent texels cut out), or -1"""
        key = (base, multipurpose, bool(cutout), channel)
        if key in self.masked:
            return self.masked[key]
        index = -1
        tag = self.map.tag(base)
        images = bitmap_images(self.map, tag, self.defs) if tag is not None else []
        if images:
            try:
                img = decode(self.map, images[0], 64)
            except ValueError:
                img = None
            mask = self.change_mask(multipurpose, img.size, channel) if img is not None else None
            if img is not None:
                index = self.textures.write_texture(img, tag.name, mask=mask, cutout=cutout)
        self.masked[key] = index
        return index

    def part_triangle_count(self, part_address):
        pf = self.f("ModelGeometryPart")
        raw = self.map.read(part_address, self.defs.size("ModelGeometryPart"))
        triangle_count = struct.unpack_from("<I", raw, pf["triangle count"])[0]
        triangle_pointer = struct.unpack_from("<I", raw, pf["triangle offset"])[0]
        if not triangle_pointer:
            return 0
        indices = struct.unpack(f"<{triangle_count + 2}H", self.map.read(triangle_pointer, 2 * (triangle_count + 2)))
        return len(strip_triangles(indices))

    def convert_part(self, part_address, nodes_inverse, u_scale, v_scale, shaders, batches, vertex_data, index_data,
                     keep=1.0, detail=False, hands=False):
        pf = self.f("ModelGeometryPart")
        raw = self.map.read(part_address, self.defs.size("ModelGeometryPart"))
        shader_index = struct.unpack_from("<h", raw, pf["shader index"])[0]
        triangle_count = struct.unpack_from("<I", raw, pf["triangle count"])[0]
        triangle_pointer = struct.unpack_from("<I", raw, pf["triangle offset"])[0]
        vertex_count = struct.unpack_from("<I", raw, pf["vertex count"])[0]
        vertex_entry = struct.unpack_from("<I", raw, pf["vertex offset"])[0]
        if not vertex_count or not triangle_pointer or not vertex_entry:
            return
        vertex_pointer = struct.unpack("<I", self.map.read(vertex_entry + 4, 4))[0]
        data = self.map.read(vertex_pointer, vertex_count * 32)
        indices = struct.unpack(f"<{triangle_count + 2}H", self.map.read(triangle_pointer, 2 * (triangle_count + 2)))
        texture, flags, bitmap = shaders[shader_index] if 0 <= shader_index < len(shaders) else (-1, 8, None)
        if texture is None:
            return
        flags |= self.current_flags

        vertices = []
        world = []
        for v in range(vertex_count):
            px, py, pz, normal, binormal, tangent, tu, tv, n0, n1, w0 = struct.unpack_from("<3fIIIhhbbH", data, v * 32)
            node = vertex_node(n0, n1, w0, len(nodes_inverse))
            local = tuple(c * self.current_scale for c in transform_point(nodes_inverse[node], (px, py, pz)))
            n = unpack_normal(normal)
            light = max(0.0, n[0] * SKY_LIGHT[0] + n[1] * SKY_LIGHT[1] + n[2] * SKY_LIGHT[2])
            shade = min(1.0, 0.5 + 0.5 * light)
            vertices.append((node, local, (tu / 32767.0 * u_scale, tv / 32767.0 * v_scale), shade, n))
            world.append((px, py, pz))
        vertices = weld_nodes(vertices, world, nodes_inverse)

        # tiny3d winds front faces counter-clockwise
        triangles = [(a, c, b) for a, b, c in strip_triangles(indices)
                     if a < vertex_count and b < vertex_count and c < vertex_count]
        if keep < 1.0:
            # (the hands keep their budget alone: the bound kept them at
            # ~1,300 triangles)
            deviation = part_size(vertices, triangles) * self.first_person_deviation if detail and not hands else None
            target = max(8, int(len(triangles) * keep))
            simplified = decimate(vertices, triangles, target, cross_nodes=not detail,
                                  max_deviation=deviation)
            if (self.reduce_open_shapes and not hands and len(simplified) > OPEN_EDGE_RETRY * target and
                    len(triangles) >= 64):
                again = decimate(vertices, triangles, target, cross_nodes=not detail,
                                 max_deviation=deviation, open_edges=True, new_vertices=True)
                if len(again) < len(simplified):
                    simplified = again
            triangles = simplified

        groups = None
        if detail and texture >= 0 and DETAIL_WINDOWS:
            # (cut out: the windows keep the transparent texels; ui.py's
            # override takes no such argument)
            groups = self.texture_windows(bitmap, vertices, triangles, cutout=True) if flags & MATERIAL_ALPHA_TEST \
                else self.texture_windows(bitmap, vertices, triangles)
        if not groups:
            groups = [(texture, None, triangles)]
        for group_texture, st_of, group_triangles in groups:
            self.build_batches(vertices, group_triangles, group_texture, flags, st_of, batches, vertex_data, index_data)

    def convert_far(self, address, gc, gp, nodes_inverse, u_scale, v_scale, shaders, budget, batches, vertex_data,
                    index_data):
        """the model's far version: every region's first permutation at its
        lowest level of detail, one untextured mesh coloured with each part's
        texture's average colour, simplified to a quarter of the budget. Cut
        out (alpha-tested) parts keep their texture, each its share of the
        triangles (a snow pine's needles: untextured, their cards were
        solid green slabs); decals are left out"""
        rf, pf, gf = self.f("ModelRegion"), self.f("ModelRegionPermutation"), self.f("ModelGeometry")
        rsize, psize, gsize = (self.defs.size(s) for s in ("ModelRegion", "ModelRegionPermutation", "ModelGeometry"))
        part_size = self.defs.size("ModelGeometryPart")
        mf = self.f("Model")
        vertices, triangles = [], []
        cut_parts = []
        rc, rp = self.block(address + mf["regions"])
        for r in range(rc):
            pc, pp = self.block(rp + r * rsize + rf["permutations"])
            if pc <= 0:
                continue
            raw = self.map.read(pp, psize)
            gi = -1
            for lod in ("super low", "low", "medium", "high", "super high"):
                gi = struct.unpack_from("<h", raw, pf[lod])[0]
                if 0 <= gi < gc:
                    break
            if not 0 <= gi < gc:
                continue
            parts_count, parts = self.block(gp + gi * gsize + gf["parts"])
            for p in range(parts_count):
                part_vertices, part_triangles, texture, flags = self.part_mesh(parts + p * part_size, nodes_inverse,
                                                                              u_scale, v_scale, shaders)
                if not part_triangles or flags & (MATERIAL_GLOW | DECAL):
                    continue
                if flags & MATERIAL_ALPHA_TEST and texture is not None and texture >= 0:
                    cut_parts.append((part_vertices, part_triangles, texture, flags))
                    continue
                colour = self.textures.textures[texture].colour if texture is not None and texture >= 0 else (0.6, 0.6, 0.6)
                base = len(vertices)
                for v in part_vertices:
                    vertices.append((v[0], v[1], v[2], tuple(min(1.0, c * v[3]) for c in colour), v[4]))
                triangles.extend((a + base, b + base, c + base) for a, b, c in part_triangles)
        total = len(triangles) + sum(len(p[1]) for p in cut_parts)
        if not total:
            return
        target = max(FAR_MINIMUM, int(budget * FAR_SHARE))
        for part_vertices, part_triangles, texture, flags in cut_parts:
            share = max(8, target * len(part_triangles) // total)
            if len(part_triangles) > share:
                part_triangles = decimate(part_vertices, part_triangles, share, cross_nodes=True)
            self.build_batches(part_vertices, part_triangles, texture, flags | self.current_flags, None, batches,
                               vertex_data, index_data)
        if not triangles:
            return
        share = max(8, target * len(triangles) // total)
        if len(triangles) > share:
            triangles = decimate(vertices, triangles, share, cross_nodes=True)
        self.build_batches(vertices, triangles, -1, MATERIAL_UNTEXTURED, lambda uv: (0.0, 0.0), batches, vertex_data,
                           index_data)

    def part_mesh(self, part_address, nodes_inverse, u_scale, v_scale, shaders):
        """a part's vertices (node, position in node space, uv, shade,
        normal) and triangles (tiny3d's winding), its texture and flags"""
        pf = self.f("ModelGeometryPart")
        raw = self.map.read(part_address, self.defs.size("ModelGeometryPart"))
        shader_index = struct.unpack_from("<h", raw, pf["shader index"])[0]
        triangle_count = struct.unpack_from("<I", raw, pf["triangle count"])[0]
        triangle_pointer = struct.unpack_from("<I", raw, pf["triangle offset"])[0]
        vertex_count = struct.unpack_from("<I", raw, pf["vertex count"])[0]
        vertex_entry = struct.unpack_from("<I", raw, pf["vertex offset"])[0]
        texture, flags, _ = shaders[shader_index] if 0 <= shader_index < len(shaders) else (-1, 8, None)
        if texture is None or not vertex_count or not triangle_pointer or not vertex_entry:
            return [], [], texture, flags
        vertex_pointer = struct.unpack("<I", self.map.read(vertex_entry + 4, 4))[0]
        data = self.map.read(vertex_pointer, vertex_count * 32)
        indices = struct.unpack(f"<{triangle_count + 2}H", self.map.read(triangle_pointer, 2 * (triangle_count + 2)))
        vertices = []
        world = []
        for v in range(vertex_count):
            px, py, pz, normal, binormal, tangent, tu, tv, n0, n1, w0 = struct.unpack_from("<3fIIIhhbbH", data, v * 32)
            node = vertex_node(n0, n1, w0, len(nodes_inverse))
            n = unpack_normal(normal)
            light = max(0.0, n[0] * SKY_LIGHT[0] + n[1] * SKY_LIGHT[1] + n[2] * SKY_LIGHT[2])
            vertices.append((node, transform_point(nodes_inverse[node], (px, py, pz)),
                             (tu / 32767.0 * u_scale, tv / 32767.0 * v_scale), min(1.0, 0.5 + 0.5 * light), n))
            world.append((px, py, pz))
        vertices = weld_nodes(vertices, world, nodes_inverse)
        triangles = [(a, c, b) for a, b, c in strip_triangles(indices)
                     if a < vertex_count and b < vertex_count and c < vertex_count]
        return vertices, triangles, texture, flags

    def texture_windows(self, bitmap, vertices, triangles, cutout=False):
        """a first-person part's triangles grouped by window of its atlas:
        [(texture, uv -> texels, triangles)], or None to keep the atlas
        whole (cutout: alpha-tested, the windows' transparent texels kept)"""
        if bitmap is None:
            return None
        bitmap, multipurpose, *channels = bitmap
        channel = channels[0] if channels else "B"
        tag = self.map.tag(bitmap)
        if tag is None:
            return None
        images = bitmap_images(self.map, tag, self.defs)
        if not images or max(images[0].width, images[0].height) <= WINDOW:
            return None
        # (the texture picker's grey for this atlas: twice the texels in
        # windows of 128x64 (4 KB of I4), or the same in windows of 64x64)
        choice = self.object_choice(bitmap, first_person=True)
        grey = choice is not None and choice[0].startswith("grey")
        if grey and multipurpose is not None:
            # (the picker said so: its other options lose the colour change)
            print(f"{self.map.name}: {tag.name} (first person): {choice[0]}, without the player's colour")
            multipurpose = None
        wide = grey and choice[0].startswith("grey-128x64")
        # Preserve the PC Flamethrower's painted dragon face. Only this gun's
        # first-person body gets a 256px source atlas; each RDP window still
        # fits the existing 64px / CI4 TMEM limit. Other weapons keep budgets.
        atlas_limit = (128 if self.detail_atlas <= 64 else 256) if tag.name == "weapons\\flamethrower\\bitmaps\\flamethrower" else self.detail_atlas
        if wide:
            atlas_limit = 2 * self.detail_atlas
        try:
            atlas = decode(self.map, images[0], atlas_limit)
        except ValueError:
            return None
        scale = min(1.0, atlas_limit / max(atlas.size))
        aw, ah = max(1, int(atlas.width * scale)), max(1, int(atlas.height * scale))
        atlas = atlas.resize((aw, ah), Image.LANCZOS)
        mask = self.change_mask(multipurpose, (aw, ah), channel) if multipurpose is not None else None
        grey_atlas = None
        if grey:
            whole = self.grey_texture(bitmap, cutout, None, choice)
            if whole is None:
                return None
            # (one stretch over the whole atlas, so the windows match)
            lum = atlas.convert("L")
            top = max(lum.getdata()) or 1
            levels = 7 if cutout else 15
            lum = lum.point(lambda v: int(round(round(v / top * levels) * 255 / levels)))
            alpha = atlas.getchannel("A").point(lambda v: 255 if v >= 128 else 0) if cutout else None
            tint = (choice[1] or self.textures.grey_tint(bitmap)) if choice[0].endswith("-tint") else (1.0, 1.0, 1.0)
            shapes = ((128, 64), (64, 128)) if wide else ((WINDOW, WINDOW),)
            grey_atlas = (lum, alpha, tuple(top / 255.0 * t for t in tint), shapes)
        elif multipurpose is not None:
            whole = self.masked_texture(bitmap, multipurpose, cutout, channel)
        elif cutout:
            whole = self.textures.cutout_texture(bitmap)[0]
        else:
            whole = self.textures.texture(bitmap)
        if tag.name == "weapons\\flamethrower\\bitmaps\\flamethrower":
            triangles = split_painted_faces(vertices, triangles, (aw, ah))
        return self.atlas_windows(atlas, mask, whole, tag.name, vertices, triangles, cutout, grey_atlas)

    def atlas_windows(self, atlas, mask, whole, name, vertices, triangles, cutout=False, grey=None):
        """texture_windows' grouping over an image already at its size: the
        triangles that fit a window's span get that window, the rest the
        whole texture. grey: (the atlas's greys, its cut-out alpha or None,
        the vertices' multiplier, the windows' shapes) for grey windows"""
        aw, ah = atlas.size
        shapes = grey[3] if grey else ((WINDOW, WINDOW),)

        def fits(w, h):
            return any(w <= a - 2 and h <= b - 2 for a, b in shapes)

        def extent(tri):
            us = [vertices[i][2][0] * aw for i in tri]
            vs = [vertices[i][2][1] * ah for i in tri]
            return min(us), min(vs), max(us), max(vs)

        # windows filled first-fit, the triangles taken in Z order of their
        # centres in the atlas
        def z_order(tri):
            x0, y0, x1, y1 = extent(tri)
            x, y = int(max(0, min(aw - 1, (x0 + x1) / 2))), int(max(0, min(ah - 1, (y0 + y1) / 2)))
            key = 0
            for bit in range(10):
                key |= ((x >> bit) & 1) << (2 * bit) | ((y >> bit) & 1) << (2 * bit + 1)
            return key

        windows = []  # [x0, y0, x1, y1, triangles]
        coarse = []
        for tri in sorted(triangles, key=z_order):
            x0, y0, x1, y1 = extent(tri)
            if not fits(x1 - x0, y1 - y0) or x0 < 0 or y0 < 0 or x1 > aw or y1 > ah:
                coarse.append(tri)
                continue
            for w in windows:
                nx0, ny0, nx1, ny1 = min(w[0], x0), min(w[1], y0), max(w[2], x1), max(w[3], y1)
                if fits(nx1 - nx0, ny1 - ny0):
                    w[0:4] = [nx0, ny0, nx1, ny1]
                    w[4].append(tri)
                    break
            else:
                windows.append([x0, y0, x1, y1, [tri]])

        groups = []
        for x0, y0, x1, y1, tris in windows:
            # a power-of-two window around the triangles, a texel of margin,
            # inside the atlas where it can be
            ww = 1 << max(2, math.ceil(math.log2(max(1.0, x1 - x0 + 2))))
            wh = 1 << max(2, math.ceil(math.log2(max(1.0, y1 - y0 + 2))))
            ox = int(max(0, min(math.floor(x0) - 1, aw - ww)))
            oy = int(max(0, min(math.floor(y0) - 1, ah - wh)))
            box = (ox, oy, ox + ww, oy + wh)
            if grey:
                texture = self.grey_window(grey[0].crop(box), grey[1].crop(box) if grey[1] else None,
                                           f"{name} window", grey[2])
            else:
                texture = self.textures.write_texture(atlas.crop(box), f"{name} window",
                                                      mask=mask.crop(box) if mask else None, cutout=cutout)
            groups.append((texture, lambda uv, ox=ox, oy=oy: (uv[0] * aw - ox, uv[1] * ah - oy), tris))
        if coarse:
            groups.append((whole, None, coarse))
        return groups

    def build_batches(self, vertices, triangles, texture, flags, st_of, batches, vertex_data, index_data):
        """batches of triangles while the per-node groups (padded to pairs)
        fit the vertex cache; st_of maps a texture coordinate to texels of
        the batch's texture (None: the texture's texels from the batch's
        lowest whole coordinate)"""
        # triangles of the same nodes together: fewer segments per batch
        triangles = sorted(triangles, key=lambda t: tuple(sorted({vertices[i][0] for i in t})))
        tw, th = (self.textures.textures[texture].width, self.textures.textures[texture].height) if texture >= 0 else (32, 32)
        # (a grey texture's vertices: times its stretch undone and its tint;
        # no palette for a player's colour)
        colour = self.texture_colour.get(texture)
        if colour is not None:
            flags &= ~MATERIAL_CHANGE_COLOR
        # a part whose texture coordinates span more than s10.5 holds (Blood
        # Gulch's trees: bark repeated ~100 times, 8,000 texels): its batches
        # each within MODEL_UV_EXTENT, the triangles in order of their
        # coordinates within a group of nodes
        wide = False
        if st_of is None and triangles and texture >= 0:
            us = [vertices[i][2][0] for t in triangles for i in t]
            vs = [vertices[i][2][1] for t in triangles for i in t]
            wide = (max(us) - min(us)) * tw > MODEL_UV_EXTENT or (max(vs) - min(vs)) * th > MODEL_UV_EXTENT
        if wide:
            triangles = sorted(triangles, key=lambda t: (tuple(sorted({vertices[i][0] for i in t})),
                                                         min(vertices[i][2][0] for i in t), min(vertices[i][2][1] for i in t)))
        while triangles:
            groups = {}
            taken = []
            rest = []
            full = False
            box = None
            for tri in triangles:
                if full:
                    rest.append(tri)
                    continue
                if wide:
                    tu = [vertices[i][2][0] for i in tri]
                    tv = [vertices[i][2][1] for i in tri]
                    grown = (min(tu), min(tv), max(tu), max(tv)) if box is None else \
                        (min(box[0], *tu), min(box[1], *tv), max(box[2], *tu), max(box[3], *tv))
                    if taken and ((grown[2] - grown[0]) * tw > MODEL_UV_EXTENT or
                                  (grown[3] - grown[1]) * th > MODEL_UV_EXTENT):
                        rest.append(tri)
                        continue
                trial = {k: set(v) for k, v in groups.items()}
                for i in tri:
                    trial.setdefault(vertices[i][0], set()).add(i)
                slots = sum((len(s) + 1) // 2 * 2 for s in trial.values())
                if slots > MAXIMUM_CACHE and taken:
                    rest.append(tri)
                    full = True
                    continue
                groups = trial
                taken.append(tri)
                if wide:
                    box = grown
            triangles = rest
            batch_st = st_of
            if batch_st is None:
                # (centred on 0, whole repeats off: from the lowest whole
                # coordinate, Blood Gulch's trees passed s10.5's 1024 texels)
                us = [vertices[i][2][0] for t in taken for i in t]
                vs = [vertices[i][2][1] for t in taken for i in t]
                u0 = round((min(us) + max(us)) / 2)
                v0 = round((min(vs) + max(vs)) / 2)
                batch_st = lambda uv, u0=u0, v0=v0: ((uv[0] - u0) * tw, (uv[1] - v0) * th)
            slot_of = {}
            segments = []
            slot = 0
            for node in sorted(groups):
                members = sorted(groups[node])
                if len(members) % 2:
                    members.append(members[-1])
                pair_offset = len(vertex_data) // 32
                for i, a in enumerate(members):
                    slot_of.setdefault(a, slot + i)
                for a, b in zip(members[0::2], members[1::2]):
                    vertex_data += self.pack_pair(vertices[a], vertices[b], batch_st, colour)
                segments.append((node, slot, len(members), pair_offset))
                slot += len(members)
            index_offset = len(index_data)
            for tri in taken:
                index_data += bytes(slot_of[i] for i in tri)
            batches.append((texture, flags & ~DECAL, segments, len(taken), index_offset))

    @staticmethod
    def pack_pair(a, b, st_of, colour=None):
        """two vertices (tiny3d's packed pair); colour: what their light is
        multiplied by (a grey texture's: texture_colour)"""
        def pos(v):
            return [max(-32768, min(32767, int(round(c * MODEL_SCALE)))) for c in v[1]]

        def st(v):
            s_, t_ = st_of(v[2])
            return [max(-32768, min(32767, int(round(s_ * 32)))), max(-32768, min(32767, int(round(t_ * 32))))]

        def rgba(v):
            if colour is not None:
                light = v[3] if isinstance(v[3], tuple) else (v[3],) * 3
                r, g, b = (int(round(max(0.0, min(1.0, c * k)) * 255)) for c, k in zip(light, colour))
                return (r << 24) | (g << 16) | (b << 8) | 255
            if isinstance(v[3], tuple):
                r, g, b = (int(round(min(1.0, c) * 255)) for c in v[3])
                return (r << 24) | (g << 16) | (b << 8) | 255
            c = int(round(v[3] * 255))
            return (c << 24) | (c << 16) | (c << 8) | 255
        return struct.pack(">3hH3hHII2h2h", *pos(a), pack_normal(a[4]), *pos(b), pack_normal(b[4]),
                           rgba(a), rgba(b), *st(a), *st(b))

    def write(self, path):
        mf, nf, rf, pf, gf = (self.f(s) for s in ("Model", "ModelNode", "ModelRegion", "ModelRegionPermutation", "ModelGeometry"))
        nsize, rsize, psize, gsize = (self.defs.size(s) for s in ("ModelNode", "ModelRegion", "ModelRegionPermutation", "ModelGeometry"))
        lod_fields = {"super low": "super low", "low": "low", "medium": "medium", "high": "high", "super high": "super high"}
        model_records, region_records, permutation_records, geometry_records = [], [], [], []
        batches = []
        model_data = []
        for tag in self.object_models():
            vertex_data, index_data = bytearray(), bytearray()
            first_batch = len(batches)
            self.current_scale = self.position_scales.get(tag.index, 1.0)
            self.current_flags = self.extra_flags.get(tag.index, 0)
            a = tag.address
            u_scale, v_scale = struct.unpack("<2f", self.map.read(a + mf["base map u scale"], 8))
            u_scale = u_scale or 1.0
            v_scale = v_scale or 1.0
            nc, np_ = self.block(a + mf["nodes"])
            nodes_inverse = []
            for i in range(nc):
                raw = self.map.read(np_ + i * nsize, nsize)
                scale = struct.unpack_from("<f", raw, nf["scale"])[0]
                rot = struct.unpack_from("<9f", raw, nf["rotation"])
                trans = struct.unpack_from("<3f", raw, nf["translation"])
                nodes_inverse.append(matrix4x3((scale,) + rot + trans))
            sc, sp = self.block(a + mf["shaders"])
            shaders = []
            for i in range(sc):
                datum = struct.unpack("<I", self.map.read(sp + i * 32 + 12, 4))[0]
                shaders.append(self.shader_texture(datum))
            gc, gp = self.block(a + mf["geometries"])
            geometry_map = {}

            budget = self.budgets.get(tag.index, MODEL_BUDGET)
            big_static = set()
            if (self.reduce_open_shapes and tag.index in getattr(self, "static", ()) and
                    tag.index not in self.units and tag.index not in self.first_person):
                part_size = self.defs.size("ModelGeometryPart")
                biggest = 0
                for gi in range(max(gc, 0)):
                    pc, pp = self.block(gp + gi * gsize + gf["parts"])
                    biggest = max(biggest, sum(self.part_triangle_count(pp + p * part_size) for p in range(pc)))
                if biggest > BIG_STATIC_TRIANGLES:
                    big_static.add(tag.index)
                    self.no_far.add(tag.index)

            def geometry(gi):
                if gi in geometry_map:
                    return geometry_map[gi]
                first = len(batches)
                pc, pp = self.block(gp + gi * gsize + gf["parts"])
                part_size = self.defs.size("ModelGeometryPart")
                total = sum(self.part_triangle_count(pp + p * part_size) for p in range(pc))
                keep = min(1.0, budget / total) if total else 1.0
                if tag.index in big_static:
                    keep = max(keep, BIG_STATIC_SHARE)
                for p in range(pc):
                    self.convert_part(pp + p * part_size, nodes_inverse, u_scale, v_scale,
                                      shaders, batches, vertex_data, index_data, keep,
                                      detail=tag.index in self.first_person, hands=tag.index in self.hands)
                geometry_map[gi] = len(geometry_records)
                geometry_records.append((first, len(batches) - first))
                return geometry_map[gi]

            rc, rp = self.block(a + mf["regions"])
            first_region = len(region_records)
            for r in range(rc):
                pc, pp = self.block(rp + r * rsize + rf["permutations"])
                first_permutation = len(permutation_records)
                for p in range(max(pc, 0)):
                    raw = self.map.read(pp + p * psize, psize)
                    choice = -1
                    for lod in self.lod_preferences.get(tag.index, LOD_PREFERENCE):
                        gi = struct.unpack_from("<h", raw, pf[lod_fields[lod]])[0]
                        if 0 <= gi < gc:
                            choice = geometry(gi)
                            break
                    permutation_records.append(choice)
                region_records.append((max(pc, 0), first_permutation))
            near_count = len(batches) - first_batch
            far_first = len(batches)
            if tag.index not in self.first_person and tag.index not in self.units and tag.index not in self.no_far:
                marks = len(vertex_data), len(index_data)
                self.convert_far(a, gc, gp, nodes_inverse, u_scale, v_scale, shaders, budget, batches, vertex_data, index_data)
                # a far version with textured cut-out parts no lighter than
                # the near one (a snow pine's needles are cards the simplifier
                # cannot merge) is only the model's data twice over
                first_geometries = {permutation_records[region_records[r][1]] for r in range(first_region, len(region_records))
                                    if region_records[r][0] > 0}
                near_triangles = sum(batches[b][3] for g in first_geometries if g >= 0
                                     for b in range(geometry_records[g][0], geometry_records[g][0] + geometry_records[g][1]))
                far_triangles = sum(b[3] for b in batches[far_first:])
                if any(b[1] & MATERIAL_ALPHA_TEST for b in batches[far_first:]) and \
                        far_triangles > FAR_CUTOUT_SHARE * near_triangles:
                    del batches[far_first:]
                    del vertex_data[marks[0]:]
                    del index_data[marks[1]:]
            model_records.append((tag.index, nc, rc, first_region, first_batch, near_count,
                                  far_first - first_batch, len(batches) - far_first))
            model_data.append((vertex_data, index_data))

        textures = self.textures.textures
        segments = []
        batch_records = []
        for texture, flags, segs, tri_count, index_offset in batches:
            batch_records.append((texture, flags, len(segs), tri_count, len(segments), index_offset))
            segments.extend(segs)

        sizes = [len(textures) * 28, len(model_records) * 40, len(region_records) * 4, len(permutation_records) * 2,
                 len(geometry_records) * 4, len(batch_records) * 12, len(segments) * 8]
        offset = 64
        offsets = []
        for size in sizes:
            offsets.append(offset)
            offset = (offset + size + 3) & ~3
        tables_size = (offset + 15) & ~15
        # each model's data in a file of its own, shared by content
        from .level import share_file, write_list
        mdl_dir = self.textures.out_dir / "mdl"
        mdl_dir.mkdir(parents=True, exist_ok=True)
        data_names = []
        for i, (vertex_data, index_data) in enumerate(model_data):
            made = mdl_dir / f".{self.textures.map_name}-{i}.bin"
            made.write_bytes(bytes(vertex_data) + bytes(index_data))
            data_names.append(share_file(made, mdl_dir, ".bin"))
        write_list(mdl_dir / f"{self.textures.map_name}.list", sorted(set(data_names)))

        out = bytearray()
        out += b"H64O" + struct.pack(">I HHHHHHH xx", VERSION, len(textures), len(model_records), len(region_records),
                                     len(permutation_records), len(geometry_records), len(batch_records), len(segments))
        out += struct.pack(">7I II", *offsets, tables_size, 0)
        out += b"\0" * (64 - len(out))

        def pad_to(o):
            out.extend(b"\0" * (o - len(out)))

        pad_to(offsets[0])
        for t in textures:
            name = t.name.encode()
            out += name + b"\0" * (24 - len(name)) + struct.pack(">HH", t.width, t.height)
        pad_to(offsets[1])
        for (tag_index, nc, rc, first_region, first_batch, batch_count, far_offset, far_count), (vertex_data, index_data), \
                data_name in zip(model_records, model_data, data_names):
            out += struct.pack(">HBBHHHH16sIIHH", tag_index, min(nc, 255), rc, first_region, 0, first_batch, batch_count,
                               data_name.encode(), len(vertex_data), len(index_data), far_offset, far_count)
        pad_to(offsets[2])
        for count, first in region_records:
            out += struct.pack(">HH", count, first)
        pad_to(offsets[3])
        for g in permutation_records:
            out += struct.pack(">h", g)
        pad_to(offsets[4])
        for first, count in geometry_records:
            out += struct.pack(">HH", first, count)
        pad_to(offsets[5])
        for texture, flags, seg_count, tri_count, first_segment, index_offset in batch_records:
            out += struct.pack(">hHBBHI", texture, flags, seg_count, tri_count, first_segment, index_offset)
        pad_to(offsets[6])
        for node, slot, count, pair_offset in segments:
            out += struct.pack(">BBBxI", node, slot, count, pair_offset)
        pad_to(tables_size)
        Path(path).parent.mkdir(parents=True, exist_ok=True)
        Path(path).write_bytes(out)
        return {"models": len(model_records), "geometries": len(geometry_records), "batches": len(batch_records),
                "segments": len(segments), "vertices": sum(len(v) for v, i in model_data) // 16,
                "triangles": sum(len(i) for v, i in model_data) // 3, "textures": len(textures), "bytes": len(out),
                "tables": tables_size}
