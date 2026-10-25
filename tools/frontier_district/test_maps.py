"""Static regression checks for the Frontier district and its entrances."""

from collections import deque
import json
import unittest

import build_map

ROOT = build_map.ROOT
SOUTH = "BattleFrontier_OutsideSouth"
GATE = "PartyResortGate"


def map_data(name):
    return json.loads((ROOT / f"data/maps/{name}/map.json").read_text(encoding="utf-8"))


def warp_index(data, name):
    if name.isdigit():
        return int(name)
    return next(i for i, warp in enumerate(data["warp_events"]) if warp.get("warp_id") == name)


def south_warp(name):
    return "WARP_ID_FRONTIER_SOUTH_" + name


def walkable(cell):
    return cell & 0xC00 == 0 and cell & 1023 != 0x170


class FrontierDistrictTest(unittest.TestCase):
    def test_generated_maps_are_current(self):
        build_map.build(check=True)

    def test_appended_ids_and_dimensions(self):
        groups = json.loads((ROOT / "data/maps/map_groups.json").read_text())
        self.assertEqual(groups["gMapGroup_TownsAndRoutes"][-2:], ["MiniMiniVilla", SOUTH])
        self.assertEqual(groups["gMapGroup_IndoorMiniMiniVilla"][-1], GATE)
        layouts = json.loads((ROOT / "data/layouts/layouts.json").read_text())["layouts"]
        names = [entry["name"] for entry in layouts]
        self.assertGreater(names.index(SOUTH + "_Layout"), names.index("Partybathtime_Layout"))
        self.assertEqual(names[-1], GATE + "_Layout")
        for entry in layouts:
            if entry["name"] in (SOUTH + "_Layout", "BattleFrontier_OutsideWest_Layout",
                                 "party_roamer_park_Layout", GATE + "_Layout"):
                with self.subTest(layout=entry["name"]):
                    self.assertEqual((ROOT / entry["blockdata_filepath"]).stat().st_size,
                                     entry["width"] * entry["height"] * 2)

    def test_reciprocal_connections(self):
        names = {map_data(name)["id"]: name for name in
                 (SOUTH, "BattleFrontier_OutsideWest", "BattleFrontier_OutsideEast", "PartyRoamerPark", "MiniMiniVilla")}
        opposite = {"up": "down", "down": "up", "left": "right", "right": "left"}
        for name in names.values():
            data = map_data(name)
            for connection in data["connections"]:
                with self.subTest(source=name, target=connection["map"]):
                    target = map_data(names[connection["map"]])
                    self.assertIn({"map": data["id"], "direction": opposite[connection["direction"]],
                                   "offset": -connection["offset"]}, target["connections"])

    def test_connection_previews_use_shared_tiles(self):
        south = build_map.read_map(SOUTH)
        west = build_map.read_map("BattleFrontier_OutsideWest")
        strips = (south[:7], west[-7:])
        for index, strip in enumerate(strips):
            with self.subTest(strip=index):
                self.assertTrue(all(cell & 1023 < 512 for row in strip for cell in row))
        for x in range(24, 30):
            self.assertTrue(walkable(west[-1][x]) and walkable(south[0][x]))

    def test_resort_is_entered_through_the_gate(self):
        # Walking straight in kept the follower out beside the roaming party.
        south = build_map.read_map(SOUTH)
        park = build_map.read_map("party_roamer_park")
        self.assertNotIn("MAP_PARTY_ROAMER_PARK", [c["map"] for c in map_data(SOUTH)["connections"]])
        self.assertNotIn(map_data(SOUTH)["id"], [c["map"] for c in map_data("PartyRoamerPark")["connections"]])
        # The rows of the former road are closed on both sides.
        self.assertFalse(any(walkable(row[-1]) for row in south[30:34]))
        self.assertFalse(any(walkable(row[0]) for row in park[17:21]))

    def test_resort_gate_round_trips(self):
        south, gate, park = map_data(SOUTH), map_data(GATE), map_data("PartyRoamerPark")
        entrance = south["warp_events"][warp_index(south, south_warp("RESORT_GATE"))]
        self.assertEqual(entrance["dest_map"], gate["id"])
        frontier_side = warp_index(gate, entrance["dest_warp_id"])
        exits = [warp for warp in gate["warp_events"] if warp["dest_map"] == south["id"]]
        self.assertEqual(len(exits), 2)
        self.assertIn(gate["warp_events"][frontier_side], exits)
        self.assertTrue(all(warp["dest_warp_id"] == south_warp("RESORT_GATE") for warp in exits))
        camp_side = gate["warp_events"][warp_index(gate, "WARP_ID_RESORT_GATE_CAMP")]
        self.assertEqual(camp_side["dest_map"], park["id"])
        park_door = park["warp_events"][warp_index(park, camp_side["dest_warp_id"])]
        self.assertEqual((park_door["dest_map"], park_door["dest_warp_id"]), (gate["id"], "WARP_ID_RESORT_GATE_CAMP"))

    def test_resort_gate_interior_tiles(self):
        cells = build_map.resort_gate()
        gate = map_data(GATE)
        self.assertEqual((len(cells[0]), len(cells)), (16, 11))
        doors = {(warp["x"], warp["y"]): cells[warp["y"]][warp["x"]] & 1023 for warp in gate["warp_events"]}
        self.assertEqual(doors, {(7, 10): 0x202, (8, 10): 0x203, (7, 1): 0x214})
        # The receptionist stands beside the aisle, which runs unblocked between the doors.
        receptionist = gate["object_events"][0]
        self.assertTrue(walkable(cells[receptionist["y"]][receptionist["x"]]))
        for y in range(1, 11):
            self.assertTrue(walkable(cells[y][7]))

    def test_plaza_reaches_each_entrance(self):
        cells = build_map.read_map(SOUTH)
        seen = {(26, 2)}
        queue = deque(seen)
        while queue:
            x, y = queue.popleft()
            for nx, ny in ((x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)):
                if (0 <= ny < len(cells) and 0 <= nx < len(cells[ny])
                        and (nx, ny) not in seen and walkable(cells[ny][nx])):
                    seen.add((nx, ny))
                    queue.append((nx, ny))
        for point in ((9, 12), (37, 14), (38, 26), (39, 26), (50, 30), (50, 29)):
            with self.subTest(point=point):
                self.assertIn(point, seen)

    def test_building_warps_match_original_door_tiles(self):
        cells = build_map.read_map(SOUTH)
        original = build_map.read_map("LilycoveCity")
        for (x, y), (sx, sy) in zip(((9, 11), (37, 13), (38, 25), (39, 25), (50, 29)),
                                  ((37, 24), (23, 24), (11, 5), (12, 5), (12, 32))):
            with self.subTest(door=(x, y)):
                self.assertEqual(cells[y][x] & 0xFFF, original[sy][sx] & 0xFFF)
        # The wooden entrance needs the existing Lilycove door animation table.
        self.assertEqual(cells[11][9] & 1023, 0x28E)

    def test_district_interior_round_trips(self):
        district = map_data(SOUTH)
        destinations = (("ultra_frontie_hub", "HUB", (0, 1, 2)),
                        ("LilycoveCity_ContestLobby", "CONTEST", (0, 1)),
                        ("LilycoveCity_LilycoveMuseum_1F", "MUSEUM_LEFT", (0,)),
                        ("LilycoveCity_LilycoveMuseum_1F", "MUSEUM_RIGHT", (1,)))
        for name, entrance, exits in destinations:
            with self.subTest(interior=name, entrance=entrance):
                data = map_data(name)
                incoming = district["warp_events"][warp_index(district, south_warp(entrance))]
                self.assertEqual(incoming["dest_map"], data["id"])
                self.assertLess(warp_index(data, incoming["dest_warp_id"]), len(data["warp_events"]))
                for index in exits:
                    self.assertEqual(data["warp_events"][index]["dest_map"], district["id"])
                    self.assertEqual(data["warp_events"][index]["dest_warp_id"], south_warp(entrance))

    def test_facilities_exit_to_their_outdoor_entrance(self):
        for facility, side, index in (("Tower", "East", 0), ("Dome", "West", 1),
                                      ("Factory", "West", 2), ("Palace", "East", 2),
                                      ("Arena", "East", 1), ("Pike", "West", 0), ("Pyramid", "East", 3)):
            with self.subTest(facility=facility):
                lobby = map_data(f"BattleFrontier_Battle{facility}Lobby")
                outdoors = map_data(f"BattleFrontier_Outside{side}")
                self.assertEqual(outdoors["warp_events"][index]["dest_map"], lobby["id"])
                exits = [warp for warp in lobby["warp_events"] if warp["dest_map"] == outdoors["id"]]
                self.assertTrue(exits)
                self.assertTrue(all(warp["dest_warp_id"] == str(index) for warp in exits))
                self.assertNotIn("MAP_ULTRA_FRONTIE_HUB", [warp["dest_map"] for warp in lobby["warp_events"]])

    def test_resort_exit_is_fixed_not_dynamic(self):
        exit_warp = map_data("PartyRoamerPark")["warp_events"][0]
        self.assertEqual(exit_warp["dest_map"], map_data(GATE)["id"])
        self.assertEqual(exit_warp["dest_warp_id"], "WARP_ID_RESORT_GATE_CAMP")
        source = (ROOT / "src/party_roamer_park.c").read_text(encoding="utf-8")
        start = source.index("static void FieldCallback_EnterPartyRoamerPark(void)\n{")
        body = source[start:source.index("\n}", start)]
        self.assertNotIn("SetDynamicWarp", body)
        self.assertIn("PARTY_ROAMER_PARK_ENTRY_X", body)


if __name__ == "__main__":
    unittest.main()
