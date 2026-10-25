"""Interior layout and regression checks; no emulator or user save is required."""

from collections import deque
import json
import struct
import unittest

import build_maps as interiors

ROOT = interiors.ROOT


def map_data(name):
    return json.loads((ROOT / f"data/maps/{name}/map.json").read_text(encoding="utf-8"))


def cells(name):
    return interiors.maps.read_map(name)


def walkable(cell):
    return not cell & 0xC00


def reachable(grid, start, occupied=()):
    seen = {start}
    queue = deque(seen)
    while queue:
        x, y = queue.popleft()
        for point in ((x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)):
            nx, ny = point
            if (0 <= ny < len(grid) and 0 <= nx < len(grid[ny]) and point not in seen
                    and point not in occupied and walkable(grid[ny][nx])):
                seen.add(point)
                queue.append(point)
    return seen


class ResortInteriorTest(unittest.TestCase):
    def test_generated_maps_match(self):
        interiors.build(check=True)

    def test_private_furniture_keeps_original_objects_and_matching_floors(self):
        assets = interiors.build_tileset
        raw = (assets.SOURCE / "metatiles.bin").read_bytes()
        private = (assets.OUTPUT / "metatiles.bin").read_bytes()
        for carpet, floor in ((False, 0x201), (True, 0x2A1)):
            for source in assets.FURNITURE:
                offset = (assets.furniture(source, carpet) - 512) * 16
                self.assertEqual(private[offset:offset + 8], raw[(floor - 512) * 16:(floor - 512) * 16 + 8])
                self.assertEqual(private[offset + 8:offset + 16], raw[(source - 512) * 16 + 8:(source - 511) * 16])

    def test_imported_plants_do_not_replace_house_graphics_or_palettes(self):
        assets = interiors.build_tileset
        raw = (assets.SOURCE / "metatiles.bin").read_bytes()
        words = struct.unpack(f"<{len(raw) // 2}H", raw)
        self.assertTrue(all(word & 1023 < 992 for word in words))
        self.assertNotIn(12, {word >> 12 for word in words})
        private = (assets.OUTPUT / "metatiles.bin").read_bytes()
        for index in (assets.PLANT_TOP, assets.PLANT_BOTTOM):
            foreground = struct.unpack_from("<4H", private, (index - 512) * 16 + 8)
            self.assertTrue(all(word >> 12 == 12 for word in foreground))
            self.assertTrue(all(992 <= word & 1023 < 1000 for word in foreground))

    def test_dimensions_and_metatile_ranges(self):
        for name in interiors.NAMES:
            with self.subTest(map=name):
                entry = interiors.maps.layout(name)
                grid = cells(name)
                self.assertEqual(len(grid), entry["height"])
                self.assertTrue(all(len(row) == entry["width"] for row in grid))
                primary, secondary = [ROOT / "data/tilesets" / interiors.maps.TILESETS[entry[key]]
                                      for key in ("primary_tileset", "secondary_tileset")]
                limits = [(path / "metatiles.bin").stat().st_size // 16 for path in (primary, secondary)]
                for row in grid:
                    for cell in row:
                        index = cell & 1023
                        self.assertLess(index % 512, limits[index >= 512])

    def test_all_npc_spawns_remain_walkable(self):
        # Pecharunt sleeps on the hub's bed on purpose.
        on_furniture = {"LOCALID_HUB_PECHARUNT"}
        for name in interiors.NAMES:
            grid = cells(name)
            for npc in map_data(name)["object_events"]:
                if npc.get("local_id") in on_furniture:
                    continue
                with self.subTest(map=name, npc=npc["graphics_id"]):
                    self.assertTrue(walkable(grid[npc["y"]][npc["x"]]))

    def test_hub_services_and_pc_are_reachable(self):
        data = map_data("ultra_frontie_hub")
        occupied = {(npc["x"], npc["y"]) for npc in data["object_events"]}
        seen = reachable(cells("ultra_frontie_hub"), (13, 19), occupied)
        for x, y in occupied:
            with self.subTest(npc=(x, y)):
                self.assertTrue(any(point in seen for point in ((x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1))))
        for point in ((14, 2), (17, 1), (23, 1), (13, 21), (14, 21), (5, 7)):
            self.assertIn(point, seen)

    def test_diner_counter_seats_and_exit_are_reachable(self):
        grid = cells("Party_eat_time")
        occupied = {(npc["x"], npc["y"]) for npc in map_data("Party_eat_time")["object_events"]}
        seen = reachable(grid, (7, 7), occupied)
        self.assertIn((7, 5), seen)
        self.assertEqual(grid[4][7] & 1023, 0x249)
        for x, y in occupied - {(7, 3)}:
            self.assertIn((x, y + 1), seen)
        self.assertIn((6, 9), seen)
        self.assertIn((7, 9), seen)

    def test_bath_gallery_and_all_roamers_are_reachable(self):
        grid = cells("Partybathtime")
        seen = reachable(grid, (18, 4))
        for x in (3, 6, 9, 12, 15):
            self.assertEqual(grid[2][x] & 1023, 0x3C0)
            self.assertEqual(grid[2][x + 1] & 1023, 0x3C1)
            self.assertIn((x, 3), seen)
            self.assertIn((x + 1, 3), seen)
        for npc in map_data("Partybathtime")["object_events"]:
            self.assertIn((npc["x"], npc["y"]), seen)
        self.assertIn((18, 1), seen)

    def test_bath_deck_uses_private_safe_graphics(self):
        directory = ROOT / "data/tilesets/secondary/party_bath"
        raw = (directory / "metatiles.bin").read_bytes()
        deck = struct.unpack_from("<8H", raw, (0x3CC - 512) * 16)
        self.assertTrue(all(word >> 12 == 6 for word in deck[:4]))
        self.assertEqual(deck[4:], (0, 0, 0, 0))
        used = set(range(672, 676)) | set(range(800, 808))
        for index in (*range(0x299, 0x2C0), *range(0x3C0, 0x3CC)):
            used.update(word & 1023 for word in struct.unpack_from("<8H", raw, (index - 512) * 16))
        self.assertFalse({word & 1023 for word in deck[:4]} & used)
        attrs = (directory / "metatile_attributes.bin").read_bytes()
        self.assertEqual(struct.unpack_from("<H", attrs, (0x3CC - 512) * 2)[0], 0)

    def test_shop_uses_a_private_layout_and_clear_aisle(self):
        self.assertEqual(map_data("MiniMiniVilla_Shop")["layout"], "LAYOUT_MINI_MINI_VILLA_SHOP")
        grid = cells("MiniMiniVilla_Shop")
        seen = reachable(grid, (4, 6), ((1, 3), (3, 2)))
        for point in ((3, 3), (3, 7), (4, 7), (5, 2), (9, 3)):
            self.assertIn(point, seen)
        self.assertNotEqual(grid, cells("Mart"))

    def test_restore_special_is_scoped_and_before_paintings(self):
        source = (ROOT / "src/resort_interiors.c").read_text(encoding="utf-8")
        self.assertNotIn("MAP_MINI_MINI_VILLA_LOCAL_ROOM", source)
        self.assertNotIn("secretBases", source)
        for name in interiors.NAMES:
            script = (ROOT / f"data/maps/{name}/scripts.inc").read_text(encoding="utf-8")
            self.assertIn("special Special_RestoreResortInterior", script)
        script = (ROOT / "data/maps/Partybathtime/scripts.inc").read_text(encoding="utf-8")
        self.assertLess(script.index("special Special_RestoreResortInterior"), script.index("FLAG_COOL_PAINTING_MADE"))


if __name__ == "__main__":
    unittest.main()
