from collections import deque
import json
from pathlib import Path
import struct
import unittest

import build_maps as inn

ROOT = inn.ROOT


class InnMapTest(unittest.TestCase):
    def setUp(self):
        self.layouts = {l['name'].removesuffix('_Layout'): l for l in json.loads((ROOT/'data/layouts/layouts.json').read_text())['layouts'] if l}
        self.maps = {n: json.loads((ROOT/f'data/maps/{n}/map.json').read_text()) for n in inn.MAPS}
        self.grid = {n: [list(row) for row in zip(*[iter(struct.unpack('<'+'H'*(len(raw)//2), raw))]*self.layouts[n]['width'])]
                     for n in inn.MAPS for raw in [(ROOT/self.layouts[n]['blockdata_filepath']).read_bytes()]}

    def test_generated_files_match(self):
        for relative, data in inn.generated().items():
            self.assertEqual((ROOT/relative).read_bytes(), data, relative)

    def test_registry_ids_unique_and_new_group_is_appended(self):
        groups = json.loads((ROOT/'data/maps/map_groups.json').read_text())
        self.assertEqual(groups['group_order'][-1], 'gMapGroup_RestorationInn')
        self.assertEqual(groups['gMapGroup_RestorationInn'], inn.MAPS)
        names = [n for g in groups['group_order'] for n in groups[g]]
        self.assertEqual(len(names), len(set(names)))
        ids = [l['id'] for l in self.layouts.values()]
        self.assertEqual(len(ids), len(set(ids)))

    def test_dimensions_and_all_metatiles_are_valid(self):
        for n, grid in self.grid.items():
            layout = self.layouts[n]
            self.assertEqual((len(grid[0]),len(grid)),(layout['width'],layout['height']))
            self.assertEqual(len((ROOT/layout['border_filepath']).read_bytes()), 8)
            for row in grid:
                for value in row:
                    self.assertTrue(512 <= value & 1023 < 579)
                    self.assertEqual(value>>12, 3)

    def test_tilesets_fit_hardware_and_have_no_unrelated_animation(self):
        for kind in ['exterior','interior']:
            folder = ROOT/f'data/tilesets/secondary/restoration_inn_{kind}'
            png = (folder/'tiles.png').read_bytes()
            width,height = struct.unpack('>II',png[16:24])
            self.assertEqual((png[24],png[25]),(4,3))
            self.assertEqual(width%8,0); self.assertEqual(height%8,0)
            self.assertLessEqual(width*height//64,512)
            tiles = width*height//64
            words = struct.unpack('<'+'H'*536,(folder/'metatiles.bin').read_bytes())
            for value in words:
                self.assertEqual(value>>12,6)
                self.assertTrue(512 <= value&1023 < 512+tiles)
            attrs = struct.unpack('<67H',(folder/'metatile_attributes.bin').read_bytes())
            self.assertEqual(attrs[64],0x60)
            self.assertEqual(attrs[65],0x65)

    def test_all_warps_have_passable_arrival_and_approach_tiles(self):
        for name, m in self.maps.items():
            for w in m['warp_events']:
                x,y = w['x'],w['y']
                self.assertFalse(self.grid[name][y][x]&0xC00)
                tile = (self.grid[name][y][x]&1023)-512
                self.assertIn(tile,[64,65])
                dy = 1 if tile==64 else -1
                self.assertFalse(self.grid[name][y+dy][x]&0xC00,(name,x,y))
                target = next(t for t in self.maps.values() if t['id']==w['dest_map'])
                matches = [v for i,v in enumerate(target['warp_events'])
                           if v.get('warp_id',str(i))==w['dest_warp_id']]
                self.assertEqual(len(matches),1,w)

    def test_every_walkable_tile_is_reachable_from_an_entrance(self):
        for n, g in self.grid.items():
            start = self.maps[n]['warp_events'][0]
            seen={(start['x'],start['y'])}; q=deque(seen)
            while q:
                x,y=q.popleft()
                for dx,dy in [(0,1),(0,-1),(1,0),(-1,0)]:
                    p=(x+dx,y+dy)
                    if 0<=p[0]<len(g[0]) and 0<=p[1]<len(g) and not g[p[1]][p[0]]&0xC00 and p not in seen:
                        seen.add(p);q.append(p)
            walkable={(x,y) for y,row in enumerate(g) for x,c in enumerate(row) if not c&0xC00}
            self.assertEqual(seen,walkable, n+' contains an inaccessible floor pocket')

    def test_no_ferry_npcs_or_outside_world_connection(self):
        for name,m in self.maps.items():
            self.assertEqual(m['object_events'],[])
            self.assertEqual(m['coord_events'],[])
            self.assertIsNone(m['connections'])
            self.assertFalse(m['allow_escaping'])
            for w in m['warp_events']:
                self.assertIn(w['dest_map'],[t['id'] for t in self.maps.values()])
            grid=self.grid[name]
            for x in range(len(grid[0])): self.assertTrue(grid[0][x]&0xC00); self.assertTrue(grid[-1][x]&0xC00)
            for row in grid: self.assertTrue(row[0]&0xC00); self.assertTrue(row[-1]&0xC00)


if __name__=='__main__': unittest.main()
