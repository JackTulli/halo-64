"""Synthetic converter controls: no retail maps, Invader definitions or tools.

Run: python -m unittest discover -s tests -p test_n64_pc_additions.py
"""
from pathlib import Path
import struct
import tempfile
from types import SimpleNamespace
import unittest
import math
from unittest.mock import patch

from tools.n64 import pc_additions as pc
from tools.n64.pc_placement_geometry import empty_aabb
from tools.n64.pc_placements import map_placements, AIRCRAFT_PLACEMENTS


class Layouts:
    """Only the block layouts exercised by the append routine."""
    sizes = {'ScenarioNetgameEquipment': 144, 'ScenarioVehicle': 120,
             'ScenarioVehiclePalette': 48, 'GlobalsVehicle': 16}
    offsets = {'Scenario': {'netgame equipment': 0, 'vehicles': 12,
                            'vehicle palette': 24},
               'Globals': {'multiplayer information': 0},
               'GlobalsMultiplayerInformation': {'vehicles': 0},
               'ScenarioStructureBSP': {'world bounds z': 0,
                                        'vehicle floor': 8, 'vehicle ceiling': 12}}

    def fields(self, name):
        return [(offset, SimpleNamespace(name=field))
                for field, offset in self.offsets[name].items()]

    def size(self, name):
        return self.sizes[name]


def reference(tag):
    return struct.pack('<4I', tag.group, 0, 0, tag.datum)


class Map:
    """Small synthetic virtual map with movable block storage and salted IDs."""
    def __init__(self, donor=False, name='bloodgulch', map_type=1):
        self.name, self.map_type = name, map_type
        self.scenario = SimpleNamespace(address=0x100000)
        self.tags = []
        for group, tagname, datum in [
            ('itmc', pc.COLLECTIONS['Flamethrower'], 0x80000001),
            ('itmc', pc.COLLECTIONS['Fuel Rod'], 0x80000002),
            ('vehi', pc.ROCKET_HOG, 0x80000003),
            ('vehi', 'synthetic car', 0x80000004),
            ('vehi', 'synthetic ghost', 0x80000005),
            ('vehi', 'synthetic tank', 0x80000006),
            ('matg', 'synthetic globals', 0x80000007),
            ('vehi', pc.BANSHEE, 0x80000008)]:
            self.tags.append(SimpleNamespace(group_name=group, name=tagname,
                group=int.from_bytes(group.encode(), 'big'), datum=datum,
                address=0x100040 if group=='matg' else 0))
        data = bytearray(1024)
        self.bsps = [(SimpleNamespace(address=0x100300), None)]
        struct.pack_into('<4f', data, 768, -2., 30., 0., 0.)
        def block(offset, count, target):
            struct.pack_into('<3I', data, offset, count, 0x100000+target, 0x12345678)
        block(0, 2 if donor else 1, 128)
        block(12, 1, 448)
        block(24, 1, 576)
        block(64, 1, 80)
        block(80, 3, 640)
        for i in range(2 if donor else 1):
            offset=128+i*144
            struct.pack_into('<I4hhhI', data, offset, 1, 12, 0, 0, 0, 0, 30, 0xffffffff)
            data[offset+80:offset+96]=reference(self.tags[i])
        struct.pack_into('<hhH', data, 448, 0, -1, 0)
        struct.pack_into('<3f', data, 456, 1, 2, 3)
        data[576:592]=reference(self.tags[3])
        for i in range(3):data[640+i*16:656+i*16]=reference(self.tags[3+i])
        self.regions=[(0x100000,bytes(data))]

    def load_bsp(self, index):
        pass

    def region(self, address):
        return next((a,d) for a,d in self.regions if a<=address<a+len(d))

    def read(self, address, size):
        a,d=self.region(address)
        if address+size>a+len(d):raise KeyError(address)
        return d[address-a:address-a+size]

    def tag(self, datum):
        return next((t for t in self.tags if t.datum==datum),None)


def block_rows(m, header, size):
    count,pointer=struct.unpack('<2I',m.read(header,8))
    return [m.read(pointer+i*size,size) for i in range(count)]


class AdditionsTests(unittest.TestCase):
    def setUp(self):
        self.map,self.donor=Map(),Map(donor=True)
        self.plan={'weapons':[
            {'kind':'Flamethrower','position':[10,0,1],'yaw':0,
             'game_types':[1,0,0,0],'respawn_seconds':45},
            {'kind':'Fuel Rod','position':[-10,0,1],'yaw':0,
             'game_types':[12,0,0,0],'respawn_seconds':60}],
            'vehicles':[{'anchor_row':0,'position':[0,10,1],
                         'yaw':0,'team_base':0}]}

    def append(self, aircraft=None):
        with patch.object(pc,'TagDefinitions',return_value=Layouts()), \
             patch.object(pc,'merge_tags') as merge, \
             patch.object(pc,'resolve_placements',return_value=self.plan):
            pc.add_original_map_content(self.map,self.donor,aircraft)
            self.assertEqual(merge.call_args_list[0].args[2],
                             [('itmc',n) for n in pc.COLLECTIONS.values()])

    def test_prefixes_headers_markers_references_and_optional_modes(self):
        m=self.map;old_regions=list(m.regions)
        original={(h,s):block_rows(m,h,s) for h,s in
                  [(0x100000,144),(0x10000c,120),(0x100018,48),(0x100050,16)]}
        self.append()
        for (header,size),rows in original.items():
            self.assertEqual(block_rows(m,header,size)[:len(rows)],rows)
            self.assertEqual(m.read(header+8,4),struct.pack('<I',0x12345678))
        eq=block_rows(m,0x100000,144)
        self.assertEqual(len(eq),3)
        for raw,seconds,mode in zip(eq[-2:],[45,60],[1,12]):
            self.assertEqual(struct.unpack_from('<I',raw)[0],pc.EQUIPMENT_FLAG|1)
            self.assertEqual(struct.unpack_from('<4h',raw,4),(mode,0,0,0))
            self.assertEqual(struct.unpack_from('<h',raw,14)[0],seconds)
            self.assertEqual(struct.unpack_from('<I',raw,16)[0],0xffffffff)
            self.assertEqual(m.tag(struct.unpack_from('<I',raw,92)[0]).group_name,'itmc')
        vehicle=block_rows(m,0x10000c,120)[-1]
        self.assertEqual(struct.unpack_from('<hhH',vehicle),(1,-1,pc.VEHICLE_FLAG))
        self.assertEqual(m.tag(struct.unpack_from('<I',block_rows(m,0x100018,48)[-1],12)[0]).name,pc.ROCKET_HOG)
        self.assertEqual(m.tag(struct.unpack_from('<I',block_rows(m,0x100050,16)[-1],12)[0]).name,pc.ROCKET_HOG)
        self.assertNotEqual(old_regions,m.regions)
        snapshot=list(m.regions)
        with self.assertRaisesRegex(ValueError,'marker already present'):self.append()
        self.assertEqual(m.regions,snapshot)

    def test_existing_rocket_palettes_are_reused(self):
        self.map.tags[3].name=pc.ROCKET_HOG
        self.append()
        self.assertEqual(len(block_rows(self.map,0x100018,48)),1)
        self.assertEqual(len(block_rows(self.map,0x100050,16)),3)

    def test_banshee_and_rocket_palettes_keep_distinct_types_and_native_limits(self):
        self.plan['vehicles'] += [
            {'kind':'Banshee', 'anchor_row':0, 'position':[10,10,1], 'yaw':0, 'team_base':0},
            {'kind':'Banshee', 'anchor_row':0, 'position':[-10,-10,2], 'yaw':1, 'team_base':1}]
        original=block_rows(self.map,0x10000c,120)[0]
        self.append(self.donor)
        palette=block_rows(self.map,0x100018,48)
        rows=block_rows(self.map,0x10000c,120)
        self.assertEqual(rows[0],original)
        kinds=[]
        for row in rows[1:]:
            index=struct.unpack_from('<h',row)[0]
            kinds.append(self.map.tag(struct.unpack_from('<I',palette[index],12)[0]).name)
            self.assertTrue(struct.unpack_from('<H',row,4)[0]&pc.VEHICLE_FLAG)
        self.assertEqual(kinds,[pc.ROCKET_HOG,pc.BANSHEE,pc.BANSHEE])
        self.assertEqual([struct.unpack_from('<b',row,88)[0]for row in rows[-2:]],[0,1])
        self.assertEqual([struct.unpack_from('<H',row,90)[0]for row in rows[-2:]],[0x0f00,0x0f00])
        self.assertEqual(struct.unpack('<2f',self.map.read(0x100308,8)),(-3.,12.))
        self.assertEqual(len(block_rows(self.map,0x100050,16)),5)

    def test_no_vehicle_recipe_keeps_vehicle_and_globals_blocks(self):
        self.plan['vehicles']=[]
        old=[self.map.read(h,12) for h in (0x10000c,0x100018,0x100050)]
        self.append()
        self.assertEqual([self.map.read(h,12) for h in (0x10000c,0x100018,0x100050)],old)

    def test_campaign_pc_map_and_unlisted_map_skip_before_dependencies(self):
        for name,kind in [('bloodgulch',0),('timberland',1),('synthetic',1)]:
            m=Map(name=name,map_type=kind);snapshot=list(m.regions)
            with patch.object(pc,'TagDefinitions',side_effect=AssertionError('unused')):
                pc.add_original_map_content(m,None)
            self.assertEqual(m.regions,snapshot)

    def test_non_pc_open_map_needs_no_pc_donor_or_placement_dependency(self):
        from tools.n64.convert import open_map
        with tempfile.TemporaryDirectory() as directory, \
             patch('tools.n64.convert.XboxMap',return_value=self.map), \
             patch.object(pc,'add_original_map_content',side_effect=AssertionError('unused')):
            self.assertIs(open_map(Path(directory)/'synthetic.map'),self.map)

    def test_reserved_vehicle_marker_fails_before_merge(self):
        a,data=self.map.regions[0];raw=bytearray(data)
        struct.pack_into('<H',raw,452,pc.VEHICLE_FLAG)
        self.map.regions=[(a,bytes(raw))]
        with patch.object(pc,'TagDefinitions',return_value=Layouts()), \
             patch.object(pc,'merge_tags',side_effect=AssertionError('unused')):
            with self.assertRaisesRegex(ValueError,'marker already present'):
                pc.add_original_map_content(self.map,self.donor)

    def test_full_box_traversal_rejects_a_solid_crossing_and_nonfinite(self):
        bsp=SimpleNamespace(nodes=[(0,-1,0x80000000)],leaves=[(0,)],
                            plane_of=lambda _: (1,0,0,0))
        self.assertFalse(empty_aabb(bsp,[[-1,1],[-1,1],[-1,1]]))
        self.assertTrue(empty_aabb(bsp,[[1,2],[-1,1],[-1,1]]))
        self.assertFalse(empty_aabb(bsp,[[float('nan'),2],[-1,1],[-1,1]]))

    def test_aircraft_pairs_are_optional_and_do_not_mutate_original_recipes(self):
        for name in ('bloodgulch', 'sidewinder', 'icefields'):
            plan = map_placements(name)
            aircraft = [r for r in plan['vehicles'] if r.get('kind') == 'Banshee']
            self.assertEqual([r['team_base'] for r in aircraft], [0, 1])
            self.assertEqual(len(aircraft), 2)
            self.assertTrue(all(math.isfinite(x) for r in aircraft for x in r['offset_xy']))
            plan['vehicles'].clear()
            self.assertEqual(len(AIRCRAFT_PLACEMENTS[name]), 2)
        self.assertIsNone(map_placements('timberland'))
        self.assertEqual(len(map_placements('bloodgulch')['vehicles']), 4)

    def test_flight_limits_preserve_authored_values_and_reject_bad_airspace(self):
        self.assertEqual(pc.aircraft_flight_bounds((-2., 30.), [1., 2.]), (-3., 12.))
        self.assertEqual(pc.aircraft_flight_bounds((-2., 30.), [1., 2.], -4., 20.), (-4., 20.))
        for world, heights, floor, ceiling in [
            ((2., 1.), [1.], 0., 0.),
            ((-2., float('inf')), [1.], 0., 0.),
            ((-2., 30.), [float('nan')], 0., 0.),
            ((-2., 30.), [], 0., 0.),
            ((-2., 4.), [2.], 0., 0.),
            ((-2., 30.), [1.], 2., 20.),
        ]:
            with self.subTest(world=world, heights=heights):
                with self.assertRaises(ValueError):
                    pc.aircraft_flight_bounds(world, heights, floor, ceiling)
        floor, ceiling = pc.aircraft_flight_bounds((1., 30.), [3.])
        self.assertNotEqual(floor, 0.)
        self.assertNotEqual(ceiling, 0.)


if __name__=='__main__':unittest.main()
