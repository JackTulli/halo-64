"""Optional PC pickups on original multiplayer maps; original rows stay intact.

The native engine owns item spawning/expiry and scenario vehicle lifetime.
Markers gate only appended rows. No parallel respawn/object manager is added.
Coordinates come from authored anchors and a conservative BSP body-volume
check; native traversal, settling and gameplay balance still need validation.
"""
import struct
from .merge import merge_tags
from .tagdefs import TagDefinitions
from .xboxmap import TAG_BASE
from .pc_placements import PLACEMENTS
from .pc_placement_geometry import resolve_placements

EQUIPMENT_FLAG = 0x40000000
VEHICLE_FLAG = 0x4000
COLLECTIONS = {
    'Flamethrower': r'item collections\single weapons\flame thrower',
    'Fuel Rod': r'item collections\single weapons\plasma_cannon',
}
ROCKET_HOG = r'vehicles\rwarthog\rwarthog'


def add_original_map_content(m, donor):
    """Append marked rows in memory; no source map is rewritten on disk."""
    if m.map_type != 1 or m.name not in PLACEMENTS:
        return
    defs = TagDefinitions()
    fields = lambda kind: {f.name: o for o, f in defs.fields(kind)}
    sf = fields('Scenario')
    scenario = m.scenario.address
    def block(address):
        count, pointer = struct.unpack('<iI', m.read(address, 8))
        if count < 0:
            raise ValueError('negative scenario block size')
        return count, pointer
    def old_rows(address, kind):
        count, pointer = block(address)
        size = defs.size(kind)
        return [m.read(pointer + i * size, size) for i in range(count)]
    eq = old_rows(scenario + sf['netgame equipment'], 'ScenarioNetgameEquipment')
    vehicles = old_rows(scenario + sf['vehicles'], 'ScenarioVehicle')
    # Repeated application is an error: never quietly double the population.
    if any(struct.unpack_from('<I', r)[0] & EQUIPMENT_FLAG for r in eq) or any(
            struct.unpack_from('<H', r, 4)[0] & VEHICLE_FLAG for r in vehicles):
        raise ValueError('reserved PC placement marker already present')
    merge_tags(m, donor, [('itmc', name) for name in COLLECTIONS.values()], defs)
    named = {(t.group_name, t.name): t for t in m.tags}
    def reference(group, name):
        t = named[group, name]
        # Runtime/repacker references resolve the salted datum. Name strings
        # are emitted by the regular tag-instance path, not scenario rows.
        return struct.pack('<IIII', t.group, 0, 0, t.datum & 0xffffffff)
    def write(address, raw):
        start, original = m.region(address)
        value = bytearray(original)
        offset = address - start
        if offset + len(raw) > len(value):
            raise ValueError('write outside region')
        value[offset:offset + len(raw)] = raw
        rewritten = bytes(value)
        m.regions = [(a, rewritten if a == start else d) for a, d in m.regions]
        if start == TAG_BASE:
            m.tag_data = rewritten
    def append(address, kind, added):
        original = old_rows(address, kind)
        size = defs.size(kind)
        if not added:
            return
        if any(len(r) != size for r in added):
            raise ValueError('incorrect appended row size')
        base = 0x70000000
        raw = b''.join(original + added)
        while any(base < a + len(d) and a < base + len(raw) for a, d in m.regions):
            base += 0x01000000
        m.regions.append((base, raw))
        write(address, struct.pack('<iI', len(original) + len(added), base))
    additions = resolve_placements(m, donor, defs, PLACEMENTS[m.name])
    # Copy actual donor row defaults, including its runtime datum sentinel.
    dsf = fields('Scenario')
    count, ptr = struct.unpack('<iI', donor.read(donor.scenario.address + dsf['netgame equipment'], 8))
    templates = {}
    for i in range(count):
        raw = donor.read(ptr + i * defs.size('ScenarioNetgameEquipment'), defs.size('ScenarioNetgameEquipment'))
        tag = donor.tag(struct.unpack_from('<I', raw, 92)[0])
        if tag and tag.name in COLLECTIONS.values():
            templates[tag.name] = raw
    added = []
    for row in additions['weapons']:
        collection = COLLECTIONS[row['kind']]
        raw = bytearray(templates[collection])
        struct.pack_into('<I4hhh', raw, 0, struct.unpack_from('<I', raw)[0] | EQUIPMENT_FLAG, *row['game_types'], 0, row['respawn_seconds'])
        struct.pack_into('<3ff', raw, 64, *row['position'], row['yaw'])
        raw[80:96] = reference('itmc', collection)
        added.append(bytes(raw))
    append(scenario + sf['netgame equipment'], 'ScenarioNetgameEquipment', added)
    if additions['vehicles']:
        palette_address = scenario + sf['vehicle palette']
        palette = old_rows(palette_address, 'ScenarioVehiclePalette')
        existing = [m.tag(struct.unpack_from('<I', r, 12)[0]).name for r in palette]
        if ROCKET_HOG in existing:
            palette_index = existing.index(ROCKET_HOG)
        else:
            palette_index = len(palette)
            append(palette_address, 'ScenarioVehiclePalette',
                   [reference('vehi', ROCKET_HOG) + bytes(defs.size('ScenarioVehiclePalette') - 16)])
        globals_tag = next(t for t in m.tags if t.group_name == 'matg')
        count, mp = block(globals_tag.address + fields('Globals')['multiplayer information'])
        if count != 1:
            raise ValueError('expected one multiplayer globals entry')
        address = mp + fields('GlobalsMultiplayerInformation')['vehicles']
        entries = old_rows(address, 'GlobalsVehicle')
        if not any(m.tag(struct.unpack_from('<I', r, 12)[0]).name == ROCKET_HOG for r in entries):
            append(address, 'GlobalsVehicle', [reference('vehi', ROCKET_HOG)])
        added = []
        for row in additions['vehicles']:
            raw = bytearray(vehicles[row['anchor_row']])
            struct.pack_into('<hhH', raw, 0, palette_index, -1,
                             struct.unpack_from('<H', raw, 4)[0] | VEHICLE_FLAG)
            struct.pack_into('<3f3f', raw, 8, *row['position'], row['yaw'], 0, 0)
            struct.pack_into('<b', raw, 88, row['team_base'])
            added.append(bytes(raw))
        append(scenario + sf['vehicles'], 'ScenarioVehicle', added)
