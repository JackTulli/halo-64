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
import copy

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
    # Several additive packs (PC content, then Covenant) must not alias the
    # same imported virtual region. Existing tag addresses stay unchanged.
    import_base = IMPORT_BASE
    while any(b <= import_base < b + max(len(d), 0x04000000) for b, d in base.regions):
        import_base += 0x04000000
    left_out = {fourcc(g) for g in LEFT_OUT}
    stand_ins = {fourcc(g) for g in STAND_INS}
    own = {(t.group, t.name): t for t in base.tags}
    own_first = {}
    for t in base.tags:
        own_first.setdefault(t.group, t)

    # the closure: each source tag the roots reach -> the base's tag, None
    # (left out) or "import"
    decided = {}
    scoped_bitmaps = set()
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
            existing = own[(t.group, t.name)]
            # PC Fuel Rod uses sequences 15/16, beyond the Xbox sheet's 15.
            # Give imported HUDs their own full sheet; original weapon HUDs
            # retain both their tag and the original sequence/image indices.
            conflict = False
            if t.group_name == 'bitm' and t.name == r'ui\hud\bitmaps\combined\hud_reticles':
                sequence_offset = next(o for o, f in defs.fields('Bitmap') if f.name == 'bitmap group sequence')
                incoming_count = struct.unpack('<i', source.read(t.address + sequence_offset, 4))[0]
                existing_count = struct.unpack('<i', base.read(existing.address + sequence_offset, 4))[0]
                conflict = incoming_count > existing_count
            if conflict:
                scoped = own.get((t.group, t.name + r'\pc_expansion'))
                if scoped is not None and struct.unpack('<i', base.read(scoped.address + sequence_offset, 4))[0] >= incoming_count:
                    decided[t.index] = scoped
                    continue
                scoped_bitmaps.add(t.index)
            else:
                decided[t.index] = existing
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
        if t.index in scoped_bitmaps:
            n.name += r'\pc_expansion'
        n.address = rebase(source, t.address, import_base)
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
        rewrite(source, defs, t, data, renumber, import_base)
    base.regions.append((import_base, bytes(data)))
    base.tags.extend(new[t.index] for t in order)
    return [new[t.index] for t in order]


def merge_empty_weapon_hud(base, source, weapon_name, defs=None):
    """Add PC HUD art to an empty Xbox placeholder without replacing gameplay.

    The original HUD tag and its bytes remain present. Only this weapon's HUD
    dependency datum changes in the in-memory expansion image. Source files,
    damage/projectiles/physics and every other weapon reference are untouched.
    """
    defs = defs or TagDefinitions()
    weapon_fields = {f.name: o for o, f in defs.fields('Weapon')}
    hud_fields = {f.name: o for o, f in defs.fields('WeaponHUDInterface')}
    def named(xmap, group, name):
        return next((t for t in xmap.tags if t.group_name == group and t.name == name), None)
    weapon = named(base, 'weap', weapon_name)
    incoming_weapon = named(source, 'weap', weapon_name)
    if weapon is None or incoming_weapon is None:
        return None
    reference = weapon.address + weapon_fields['hud interface']
    original = base.tag(struct.unpack('<I', base.read(reference + 12, 4))[0])
    incoming = source.tag(struct.unpack('<I', source.read(incoming_weapon.address + weapon_fields['hud interface'] + 12, 4))[0])
    if original is None or incoming is None:
        return None
    if struct.unpack('<i', base.read(original.address + hud_fields['crosshairs'], 4))[0] != 0:
        return None
    if struct.unpack('<i', source.read(incoming.address + hud_fields['crosshairs'], 4))[0] <= 0:
        return None

    # A shallow in-memory view gives just the root HUD a distinct identity.
    # Its existing child/bitmap references are followed and renumbered by the
    # normal closure walker, including the scoped full PC reticle sheet.
    view = copy.copy(source)
    view.tags = list(source.tags)
    root = copy.copy(incoming)
    root.name += r'\pc_expansion'
    root.map = view
    view.tags[root.index] = root
    merge_tags(base, view, [('wphi', root.name)], defs)
    added = named(base, 'wphi', root.name)
    if added is None:
        raise ValueError('failed to import the additive weapon HUD')
    address, region = base.region(reference + 12)
    rewritten = bytearray(region)
    struct.pack_into('<I', rewritten, reference + 12 - address, added.datum & 0xFFFFFFFF)
    rewritten = bytes(rewritten)
    for i, (start, _) in enumerate(base.regions):
        if start == address:
            base.regions[i] = (start, rewritten)
            break
    if address == TAG_BASE:
        base.tag_data = rewritten
    return added


def rebase(source, address, import_base=IMPORT_BASE):
    """a source tag data address, moved to IMPORT_BASE (others: unchanged)"""
    if TAG_BASE <= address < TAG_BASE + len(source.tag_data):
        return address - TAG_BASE + import_base
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


def rewrite(source, defs, tag, data, renumber, import_base=IMPORT_BASE):
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
                        put(a + 4, rebase(source, pointer, import_base))
                        for e in range(count):
                            visit(pointer + e * defs.size(f.struct), f.struct)
                elif f.kind == "TagDataOffset":
                    size_, flags, file_offset, pointer, _ = struct.unpack_from("<iIIII", blob, o)
                    if size_ > 0 and pointer:
                        put(a + 12, rebase(source, pointer, import_base))
                elif f.kind == "TagDependency":
                    group, name_ptr, nlen, datum = struct.unpack_from("<IIiI", blob, o)
                    if name_ptr:
                        put(a + 4, rebase(source, name_ptr, import_base))
                    put(a + 12, renumber(datum))
                elif f.type == "TagID":
                    put(a, renumber(struct.unpack_from("<I", blob, o)[0]))
                elif f.type == "Pointer":
                    value = struct.unpack_from("<I", blob, o)[0]
                    if value:
                        put(a, rebase(source, value, import_base))
                elif f.type == "uint32" and "offset" in f.name:
                    # addresses kept as plain numbers (a model part's
                    # triangles and vertex buffer); file offsets are far
                    # below the tag data's addresses
                    value = struct.unpack_from("<I", blob, o)[0]
                    if TAG_BASE <= value < TAG_BASE + len(source.tag_data):
                        put(a, rebase(source, value, import_base))
                        if name == "ModelGeometryPart" and f.name == "vertex offset":
                            # its vertex buffer descriptor: the vertices' address at +4
                            vertices = struct.unpack("<I", source.read(value + 4, 4))[0]
                            put(value + 4, rebase(source, vertices, import_base))

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
