"""Generate the isolated inn maps. No ferry, story flags, NPCs or existing maps are changed."""
import argparse
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[2]
BASE = 512
BLOCKED = 0x400
ELEVATION = 0x3000
MAPS = ['RestorationInn_Outside', 'RestorationInn_Lobby', 'RestorationInn_EastWing', 'RestorationInn_WestWing']


def cell(tile, blocked=False):
    return ELEVATION | (BLOCKED if blocked else 0) | (BASE + tile)


def fill(grid, x, y, w, h, tile, blocked=False):
    for row in grid[y:y + h]:
        row[x:x + w] = [cell(tile, blocked)] * w


def outside():
    w, h = 40, 36
    g = [[cell(0)] * w for _ in range(h)]
    # Bamboo around a small coast-side garden, with a future arrival pier to the south.
    for y in range(0, 30, 2):
        for x in (0, 2, 36, 38):
            for dy in range(2):
                for dx in range(2): g[y + dy][x + dx] = cell(48 + dy * 2 + dx, True)
    for y in (0, 2):
        for x in range(0, w, 2):
            for dy in range(2):
                for dx in range(2): g[y + dy][x + dx] = cell(48 + dy * 2 + dx, True)
    fill(g, 0, 28, w, 8, 6, True)
    fill(g, 0, 34, 40, 2, 63, True)
    fill(g, 17, 28, 7, 6, 4)
    for y in range(28, 34):
        g[y][17] = cell(56, True)
        g[y][23] = cell(58, True)
    g[32][18] = cell(60, True)
    g[32][22] = cell(60, True)
    fill(g, 6, 19, 28, 9, 3)
    fill(g, 17, 18, 7, 10, 2)
    fill(g, 10, 22, 21, 3, 2)
    # Broad main roof, second-storey windows, then the lower eave and entrance.
    left, right = 6, 33
    for x in range(left, right + 1):
        g[4][x] = cell(11 if x == left else 13 if x == right else 12, True)
        for y in range(5, 8): g[y][x] = cell(15 if (x + y) % 9 == 0 else 14, True)
        g[8][x] = cell(8 if x == left else 10 if x == right else 9, True)
        g[9][x] = cell(16 if x == left else 18 if x == right else 17, True)
        g[10][x] = cell(26 if x % 4 == 0 else 27 if x % 4 == 1 else 24, True)
        g[11][x] = cell(28, True)
        g[12][x] = cell(8 if x == left else 10 if x == right else 9, True)
        g[13][x] = cell(24 if x % 4 < 2 else 26 if x % 4 == 2 else 27, True)
        g[14][x] = cell(28, True)
        g[15][x] = cell(28, True)
        g[16][x] = cell(4)
        g[17][x] = cell(40 if x == left else 42 if x == right else 41, True)
    for dx, tile in enumerate([32, 64, 34]): g[15][19 + dx] = cell(tile, dx != 1)
    for x in (19, 20, 21):
        g[17][x] = cell(4)
        g[18][x] = cell(39)
    # The central gable breaks up the large tiled roof, without changing elevation.
    for dx, tile in enumerate([19, 20, 21]): g[8][19 + dx] = cell(tile, True)
    for x, y, tile in [(8, 22, 44), (8, 23, 45), (31, 22, 44), (31, 23, 45),
                        (10, 26, 53), (28, 26, 53), (12, 25, 54), (30, 20, 47), (25, 19, 46)]:
        g[y][x] = cell(tile, True)
    # The map has no outside-world warp and no walkable boundary.
    for y in range(h): g[y][0] = g[y][-1] = cell(63, True)
    for x in range(w): g[0][x] = g[-1][x] = cell(63, True)
    return g


def room_base(w, h):
    g = [[cell(66, True)] * w for _ in range(h)]
    fill(g, 1, 2, w - 2, h - 3, 0)
    fill(g, 1, 1, w - 2, 1, 8, True)
    fill(g, 1, h - 1, w - 2, 1, 9, True)
    return g


def lobby():
    g = room_base(32, 22)
    fill(g, 13, 4, 16, 12, 2)
    for y in range(4, 16):
        g[y][13] = cell(4)
        g[y][28] = cell(5)
    for x in (3, 11, 20, 28):
        g[2][x] = cell(12, True); g[2][x + 1] = cell(13, True)
    for dx, tile in enumerate([24, 25, 25, 25, 26]): g[6][3 + dx] = cell(tile, True)
    for x in (3, 8, 10): g[3][x] = cell(43, True)
    for x, y in [(17, 7), (24, 7), (17, 12), (24, 12)]:
        g[y][x] = cell(34, True)
        g[y - 1][x] = cell(40)
        g[y + 1][x] = cell(41)
    for dx, tile in enumerate([52, 53, 54]): g[3][19 + dx] = cell(tile, True)
    g[9][4] = cell(47, True); g[15][29] = cell(58, True)
    g[18][3] = cell(63, True); g[18][28] = cell(63, True)
    for x in (6, 25): g[2][x] = cell(64)
    fill(g, 13, 18, 6, 3, 6)
    g[20][15] = g[20][16] = cell(65)
    return g


def wing(west=False):
    g = room_base(34, 24)
    fill(g, 1, 10, 32, 4, 0)
    fill(g, 16, 2, 2, 21, 0)
    for left in (2, 19):
        for top in (2, 15):
            fill(g, left, top, 13, 7, 2)
            fill(g, left, top - 1, 13, 1, 8, True)
            fill(g, left, top + 7, 13, 1, 9, True)
            for y in range(top, top + 7):
                g[y][left - 1] = g[y][left + 13] = cell(10, True)
            alcove_x = left + (5 if top == 2 else 1)
            for dx, tile in enumerate([52, 53, 54]): g[top][alcove_x + dx] = cell(tile, True)
            for bx in (left + 1, left + 5, left + 9):
                g[top + 4][bx] = cell(32, True)
                g[top + 5][bx] = cell(33, True)
            g[top + 2][left + 7] = cell(34, True)
            g[top + 3][left + 7] = cell(41 if west else 40)
            door_y = top + 7 if top == 2 else top - 1
            g[door_y][left + 6] = cell(18)
            g[door_y][left + 7] = cell(19)
    for x in (2, 30): g[12][x] = cell(47, True)
    fill(g, 1, 22, 32, 1, 9, True)
    g[22][16] = g[22][17] = cell(65)
    return g


def warp(x, y, destination, destination_warp, name=None):
    event = dict(x=x, y=y, elevation=0, dest_map='MAP_' + destination, dest_warp_id=destination_warp)
    if name: event = {'warp_id': name, **event}
    return event


def map_json(name, warps):
    outside_map = name.endswith('Outside')
    constants = {'RestorationInn_Outside':'OUTSIDE', 'RestorationInn_Lobby':'LOBBY',
                 'RestorationInn_EastWing':'EAST_WING', 'RestorationInn_WestWing':'WEST_WING'}
    token = 'RESTORATION_INN_' + constants[name]
    return dict(id='MAP_' + token, name=name, layout='LAYOUT_' + token, music='MUS_VERDANTURF',
                region_map_section='MAPSEC_SPECIAL_AREA', requires_flash=False, weather='WEATHER_NONE',
                map_type='MAP_TYPE_TOWN' if outside_map else 'MAP_TYPE_INDOOR', allow_cycling=False,
                allow_escaping=False, allow_running=True, show_map_name=False,
                battle_scene='MAP_BATTLE_SCENE_NORMAL', connections=None, object_events=[],
                warp_events=warps, coord_events=[], bg_events=[])


def generated():
    grids = dict(zip(MAPS, [outside(), lobby(), wing(), wing(True)]))
    warps = [
        [warp(20,15,'RESTORATION_INN_LOBBY','WARP_ID_INN_LOBBY_EXIT','WARP_ID_INN_OUTSIDE_ENTRY')],
        [warp(15,20,'RESTORATION_INN_OUTSIDE','WARP_ID_INN_OUTSIDE_ENTRY','WARP_ID_INN_LOBBY_EXIT'),
         warp(16,20,'RESTORATION_INN_OUTSIDE','WARP_ID_INN_OUTSIDE_ENTRY'),
         warp(25,2,'RESTORATION_INN_EAST_WING','WARP_ID_INN_EAST_EXIT','WARP_ID_INN_LOBBY_EAST'),
         warp(6,2,'RESTORATION_INN_WEST_WING','WARP_ID_INN_WEST_EXIT','WARP_ID_INN_LOBBY_WEST')],
        [warp(16,22,'RESTORATION_INN_LOBBY','WARP_ID_INN_LOBBY_EAST','WARP_ID_INN_EAST_EXIT'),
         warp(17,22,'RESTORATION_INN_LOBBY','WARP_ID_INN_LOBBY_EAST')],
        [warp(16,22,'RESTORATION_INN_LOBBY','WARP_ID_INN_LOBBY_WEST','WARP_ID_INN_WEST_EXIT'),
         warp(17,22,'RESTORATION_INN_LOBBY','WARP_ID_INN_LOBBY_WEST')],
    ]
    files = {}
    for name, ws in zip(MAPS, warps):
        grid = grids[name]
        values = [v for row in grid for v in row]
        files[f'data/layouts/{name}/map.bin'] = struct.pack('<' + 'H' * len(values), *values)
        files[f'data/layouts/{name}/border.bin'] = struct.pack('<4H', *([cell(63 if name.endswith('Outside') else 66, True)] * 4))
        files[f'data/maps/{name}/map.json'] = (json.dumps(map_json(name,ws),ensure_ascii=False,indent=2)+'\n').encode()
        files[f'data/maps/{name}/scripts.inc'] = (name+'_MapScripts::\n\t.byte 0\n').encode()
    return files


def main():
    parser = argparse.ArgumentParser(__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    for relative, data in generated().items():
        p = ROOT / relative
        if args.check:
            assert p.read_bytes() == data, 'Outdated generated inn map: ' + relative
        else:
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_bytes(data)
    print('Validated' if args.check else 'Generated', len(MAPS), 'isolated inn maps; no ferry or NPCs')


if __name__ == '__main__':
    main()
