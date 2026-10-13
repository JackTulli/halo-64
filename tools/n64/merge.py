"""Tags from another Xbox map, merged into a map before it is converted.

Firefight (port/n64/game/n64_firefight.c) needs the Covenant on the
multiplayer maps, whose cache files hold none: their bipeds, models,
animations, collision and shaders come from a campaign map (a30 has the
grunt, the jackal and the elite). merge_tags() appends the tags a set of
root tags needs to a map (an XboxMap, changed in place):

- a tag the map already has (the same group and name: the plasma weapons,
  most effects) is the map's own;
- groups the N64 does not use are left out (references to them become
  NONE): sound environments, unit HUD interfaces, actors; of a dialogue
  ('udlg') only the vocalizations Firefight plays are followed (the
  game's own pain and death sounds, the AI's few lines), the rest NONE;
- the rest are copied: the source's tag data is mapped at IMPORT_BASE
  instead of TAG_BASE, its block, data and plain pointers moved there, and
  its tag references renumbered to the merged tags.

The converter (convert.py, models.py, level.py's textures, hud.py) then
sees one map.
"""

import struct

from .tagdefs import TagDefinitions
from .xboxmap import TAG_BASE, Tag

IMPORT_BASE = 0x50000000
NONE = 0xFFFFFFFF

# groups whose tags are not imported (references to them are cleared)
LEFT_OUT = {b"snde", b"unhi", b"actv", b"actr", b"DeLa", b"Soul", b"spk!"}
# a dialogue's vocalizations that are imported (unit_dialogue.c: the pain,
# scream and death sounds 6-19 the game plays itself; n64_firefight.c's
# killed enemy 53/55, new combat 108, unexpected enemy 111, grenade throw
# 115 and sighted 116, shooting 149, taunt 154, flee 156/158, melee 179,
# celebration 188)
DIALOG_VOCALIZATIONS = list(range(6, 20)) + [53, 55, 108, 111, 115, 116, 149, 154, 156, 158, 179, 188]
# groups for which the map's own first tag stands in (a biped's material
# effects: its footsteps)
STAND_INS = {b"foot"}


def fourcc(text):
    return int.from_bytes(text, "big")


def merge_tags(base, source, roots, defs=None):
    """imports the (group, name) roots of the source map, and what they
    need, into the base map; returns the imported tags"""
    defs = defs or TagDefinitions()
    left_out = {fourcc(g) for g in LEFT_OUT}
    stand_ins = {fourcc(g) for g in STAND_INS}
    own = {(t.group, t.name): t for t in base.tags}
    own_first = {}
    for t in base.tags:
        own_first.setdefault(t.group, t)

    # the closure: each source tag the roots reach -> the base's tag, None
    # (left out) or "import"
    decided = {}
    order = []
    todo = []
    for group, name in roots:
        found = [t for t in source.tags if t.group_name == group and t.name == name]
        if not found:
            raise KeyError(f"{source.name} has no {group} {name}")
        todo.append(found[0])
    while todo:
        t = todo.pop()
        if t.index in decided:
            continue
        if t.group in left_out:
            decided[t.index] = None
            continue
        if (t.group, t.name) in own:
            decided[t.index] = own[(t.group, t.name)]
            continue
        if t.group in stand_ins and t.group in own_first:
            decided[t.index] = own_first[t.group]
            continue
        decided[t.index] = "import"
        order.append(t)
        if t.group_name == "udlg":
            needs = [struct.unpack("<I", source.read(t.address + 16 + 16 * v + 12, 4))[0]
                     for v in DIALOG_VOCALIZATIONS]
        else:
            needs = dependencies(source, defs, t)
        for datum in needs:
            d = source.tag(datum)
            if d is not None:
                todo.append(d)

    # the imported tags' places in the merged map
    first = len(base.tags)
    salt = max((t.datum >> 16) & 0xFFFF for t in base.tags) + 1
    new = {}
    for k, t in enumerate(order):
        n = Tag()
        n.index = first + k
        n.datum = (((salt + k) & 0x7FFF) << 16) | n.index
        n.group, n.parents, n.name = t.group, t.parents, t.name
        n.address = rebase(source, t.address)
        n.map = base
        n.source = source
        new[t.index] = n

    def renumber(datum):
        if datum in (NONE, -1):
            return NONE
        index = datum & 0xFFFF
        target = decided.get(index, None)
        if index not in decided:
            # (a reference the walk did not follow: a tag left out)
            return NONE
        if target is None:
            return NONE
        if target == "import":
            return new[index].datum & 0xFFFFFFFF
        return target.datum & 0xFFFFFFFF

    data = bytearray(source.tag_data)
    for t in order:
        rewrite(source, defs, t, data, renumber)
    base.regions.append((IMPORT_BASE, bytes(data)))
    base.tags.extend(new[t.index] for t in order)
    return [new[t.index] for t in order]


def rebase(source, address):
    """a source tag data address, moved to IMPORT_BASE (others: unchanged)"""
    if TAG_BASE <= address < TAG_BASE + len(source.tag_data):
        return address - TAG_BASE + IMPORT_BASE
    return address


def dependencies(xmap, defs, tag):
    found = []

    def visit(address, name):
        size = defs.size(name)
        blob = xmap.read(address, size)
        for offset, f in defs.fields(name):
            for i in range(f.count):
                o = offset + i * f.size
                if f.kind == "struct":
                    visit(address + o, f.struct)
                elif f.kind == "TagReflexive":
                    count, pointer, _ = struct.unpack_from("<iII", blob, o)
                    for e in range(max(count, 0)):
                        visit(pointer + e * defs.size(f.struct), f.struct)
                elif f.kind == "TagDependency":
                    found.append(struct.unpack_from("<I", blob, o + 12)[0])
                elif f.type == "TagID":
                    found.append(struct.unpack_from("<I", blob, o)[0])

    visit(tag.address, defs.fourcc_struct(tag.group))
    return found


def rewrite(source, defs, tag, data, renumber):
    """the tag's pointers and references in data (the source's tag data,
    copied): pointers into the tag data moved to IMPORT_BASE, references
    renumbered"""

    def put(address, value):
        o = address - TAG_BASE
        struct.pack_into("<I", data, o, value & 0xFFFFFFFF)

    def visit(address, name):
        size = defs.size(name)
        blob = source.read(address, size)
        for offset, f in defs.fields(name):
            for i in range(f.count):
                o = offset + i * f.size
                a = address + o
                if f.kind == "struct":
                    visit(a, f.struct)
                elif f.kind == "TagReflexive":
                    count, pointer, _ = struct.unpack_from("<iII", blob, o)
                    if count > 0 and pointer:
                        put(a + 4, rebase(source, pointer))
                        for e in range(count):
                            visit(pointer + e * defs.size(f.struct), f.struct)
                elif f.kind == "TagDataOffset":
                    size_, flags, file_offset, pointer, _ = struct.unpack_from("<iIIII", blob, o)
                    if size_ > 0 and pointer:
                        put(a + 12, rebase(source, pointer))
                elif f.kind == "TagDependency":
                    group, name_ptr, nlen, datum = struct.unpack_from("<IIiI", blob, o)
                    if name_ptr:
                        put(a + 4, rebase(source, name_ptr))
                    put(a + 12, renumber(datum))
                elif f.type == "TagID":
                    put(a, renumber(struct.unpack_from("<I", blob, o)[0]))
                elif f.type == "Pointer":
                    value = struct.unpack_from("<I", blob, o)[0]
                    if value:
                        put(a, rebase(source, value))
                elif f.type == "uint32" and "offset" in f.name:
                    # addresses kept as plain numbers (a model part's
                    # triangles and vertex buffer); file offsets are far
                    # below the tag data's addresses
                    value = struct.unpack_from("<I", blob, o)[0]
                    if TAG_BASE <= value < TAG_BASE + len(source.tag_data):
                        put(a, rebase(source, value))
                        if name == "ModelGeometryPart" and f.name == "vertex offset":
                            # its vertex buffer descriptor: the vertices' address at +4
                            vertices = struct.unpack("<I", source.read(value + 4, 4))[0]
                            put(value + 4, rebase(source, vertices))

    visit(tag.address, defs.fourcc_struct(tag.group))


if __name__ == "__main__":
    import sys
    from pathlib import Path
    from .xboxmap import XboxMap
    base = XboxMap(Path(sys.argv[1]))
    base.load_bsp(0)
    source = XboxMap(Path(sys.argv[2]))
    roots = [tuple(r.split(":", 1)) for r in sys.argv[3:]]
    imported = merge_tags(base, source, roots)
    groups = {}
    for t in imported:
        groups[t.group_name] = groups.get(t.group_name, 0) + 1
    print(f"{len(imported)} tags imported:", ", ".join(f"{g} {n}" for g, n in sorted(groups.items())))
