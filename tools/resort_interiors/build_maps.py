"""Build resort interiors from existing Emerald metatiles and render previews."""

import argparse
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/frontier_district"))
import build_map as maps
import build_tileset

NAMES = ("ultra_frontie_hub", "Party_eat_time", "Partybathtime", "MiniMiniVilla_Shop")


def room(width, height, floor=0x3201):
    return [[floor] * width for _ in range(height)]


def put(cells, x, y, rows):
    for j, row in enumerate(rows):
        cells[y + j][x:x + len(row)] = row


def rug(cells, x, y, width, height):
    put(cells, x, y, [[0x3298] + [0x3299] * (width - 2) + [0x329A]])
    for j in range(1, height - 1):
        put(cells, x, y + j, [[0x32A0] + [0x32A1] * (width - 2) + [0x32A2]])
    put(cells, x, y + height - 1, [[0x32A8] + [0x32A9] * (width - 2) + [0x32AA]])


def table(cells, x, y, carpet, chairs=True):
    for j, row in enumerate(((0x219, 0x21A, 0x21B, 0x21C),
                             (0x221, 0x222, 0x223, 0x224))):
        for i, tile in enumerate(row):
            if i in (0, 3) and not chairs:
                continue
            solid = 0x400 if i in (1, 2) else 0
            cells[y + j][x + i] = 0x3000 | solid | build_tileset.furniture(tile, carpet)


def plant(cells, x, y):
    put(cells, x, y, [[0x3400 | build_tileset.PLANT_TOP], [0x3400 | build_tileset.PLANT_BOTTOM]])


def hub():
    cells = room(27, 22)
    for x in range(27):
        cells[0][x] = 0x366E
        cells[1][x] = 0x3676
        cells[21][x] = 0x3401
    for y in range(2, 21):
        cells[y][0] = cells[y][26] = 0x3401
        cells[y][1] = 0x3204
    # Preserve all doorway, PC and service coordinates from the existing map.
    put(cells, 16, 0, [[0x3608, 0x3609, 0x360A], [0x3610, 0x3211, 0x3612]])
    put(cells, 22, 0, [[0x360B, 0x360C, 0x360D], [0x3613, 0x3214, 0x3615]])
    cells[1][14] = 0x3404
    for x in (3, 4, 7, 8, 10, 11, 19, 20):
        put(cells, x, 0, [[0x361E], [0x3626]])
    put(cells, 13, 21, [[0x3202, 0x3203]])
    rug(cells, 2, 4, 8, 6)
    # Four service NPCs sit around the existing table; all four outer sides remain accessible.
    table(cells, 4, 5, carpet=True, chairs=False)
    # Waiting area and the guide/BGM lounge are separated by the central aisle.
    rug(cells, 2, 13, 8, 5)
    table(cells, 4, 14, carpet=True)
    rug(cells, 18, 6, 7, 6)
    table(cells, 20, 7, carpet=True)
    rug(cells, 18, 14, 7, 6)
    # Complete TV and cabinet, including their lower halves.
    for j, row in enumerate(((0x240, 0x241), (0x248, 0x249))):
        for i, tile in enumerate(row):
            cells[15 + j][22 + i] = 0x3400 | build_tileset.furniture(tile, True)
    for x in (19, 23):
        cells[18][x] = 0x3000 | build_tileset.furniture(0x219, True)
    rug(cells, 12, 18, 4, 3)
    for x, y in ((2, 2), (11, 2), (24, 2), (2, 10), (24, 12), (2, 19), (24, 19)):
        plant(cells, x, y)
    return cells


def diner():
    cells = room(15, 10)
    for x in range(15):
        cells[0][x] = 0x366E
        cells[9][x] = 0x3401
    for y in range(1, 9):
        cells[y][0] = cells[y][14] = 0x3401
        cells[y][1] = 0x3204
    put(cells, 1, 1, [[0x3630, 0x3631, 0x3632, 0x3633, 0x3634],
                     [0x3638, 0x3639, 0x363A, 0x363B, 0x363C]])
    put(cells, 12, 1, [[0x3630], [0x3638]])
    put(cells, 7, 0, [[0x361E, 0x361E]])
    put(cells, 7, 1, [[0x3626, 0x3626]])
    put(cells, 1, 4, [[0x3648] + [0x3649] * 11 + [0x364A]])
    for x in (2, 11):
        table(cells, x - 1, 7, carpet=False)
    for x in (5, 9):
        plant(cells, x, 7)
    # Keep party sprites on plain floor: a chair top without its lower half
    # looked like a broken object beneath large following Pokemon.
    put(cells, 6, 9, [[0x3202, 0x3203]])
    return cells


def bath():
    cells = room(20, 20, 0x33CC)
    for x in range(20):
        cells[0][x] = 0x37CE
        cells[1][x] = 0x37CF
        cells[2][x] = 0x37D0
        cells[19][x] = 0x37CD
    for y in range(3, 19):
        cells[y][0] = cells[y][19] = 0x37CD
    put(cells, 17, 0, [[0x37D1, 0x37D2, 0x37D3], [0x37D4, 0x33D5, 0x37D6]])
    cells[2][18] = 0x33CC
    for x in (3, 6, 9, 12, 15):
        put(cells, x, 2, [[0x37C0, 0x37C1]])

    def pool(x, y, width, height):
        maps.fill(cells, x, y, width, height, 0x32A3)
        put(cells, x, y, [[0x37D7] + [0x37D8] * (width - 2) + [0x37D9]])
        for j in range(1, height - 1):
            cells[y + j][x] = 0x37DA
            cells[y + j][x + width - 1] = 0x37DB
        put(cells, x, y + height - 1, [[0x37DC] + [0x37DD] * (width - 2) + [0x37DE]])

    pool(2, 6, 6, 6)
    pool(10, 9, 8, 9)
    # Wooden steps connect the water to the dry circulation route.
    for x, y in ((5, 6), (5, 11), (14, 9), (10, 13)):
        cells[y][x] = 0x33CC
    for x, y in ((4, 8), (13, 11), (15, 14)):
        cells[y][x] = 0x32BB
    cells[15][13] = 0x32BC
    return cells


def shop():
    cells = maps.read_map("Mart")
    maps.fill(cells, 4, 3, 6, 4, 0x3201)
    for y in range(4, 7):
        cells[y][10] = 0x320E
    # Display shelves, with an uninterrupted aisle from the doors to both clerks.
    put(cells, 7, 3, [[0x362B, 0x362C], [0x3633, 0x3634], [0x363B, 0x363C]])
    put(cells, 9, 5, [[0x3625], [0x362D]])
    return cells


def generated_maps():
    return {
        "data/layouts/ultra_frontie_hub/map.bin": maps.pack_map(hub()),
        "data/layouts/Party_eat_time/map.bin": maps.pack_map(diner()),
        "data/layouts/Partybathtime/map.bin": maps.pack_map(bath()),
        "data/layouts/Partybathtime/border.bin": maps.pack_map([[0x37CD] * 2] * 2),
        "data/layouts/MiniMiniVilla_Shop/map.bin": maps.pack_map(shop()),
        "data/layouts/MiniMiniVilla_Shop/border.bin": (ROOT / "data/layouts/Mart/border.bin").read_bytes(),
    }


def build(check=False):
    build_tileset.build(check)
    for relative, data in generated_maps().items():
        path = ROOT / relative
        if check:
            if not path.exists() or path.read_bytes() != data:
                raise ValueError(f"outdated interior: {relative}")
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(data)


def atlas(name, output):
    from PIL import Image, ImageDraw

    entry = maps.layout(name)
    directory = ROOT / "data/tilesets" / maps.TILESETS[entry["secondary_tileset"]]
    count = (directory / "metatiles.bin").stat().st_size // 16
    rows = (count + 15) // 16
    cells = [[512 + y * 16 + x if y * 16 + x < count else 0 for x in range(16)] for y in range(rows)]
    maps.render(name, output, cells)
    source = Image.open(output)
    result = Image.new("RGB", (16 * 48, rows * 44), "#303030")
    draw = ImageDraw.Draw(result)
    for y, row in enumerate(cells):
        for x, index in enumerate(row):
            result.paste(source.crop((x * 16, y * 16, x * 16 + 16, y * 16 + 16)).resize((32, 32)),
                         (x * 48, y * 44))
            draw.text((x * 48, y * 44 + 32), f"{index:03X}", fill="white")
    result.save(output)


def main():
    parser = argparse.ArgumentParser(__doc__)
    parser.add_argument("--atlas", action="store_true")
    parser.add_argument("--preview", type=Path)
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    if not args.atlas:
        build(args.check)
    if args.preview:
        args.preview.mkdir(parents=True, exist_ok=True)
        for name in NAMES:
            if args.atlas:
                atlas(name, args.preview / f"atlas_{name}.png")
            else:
                maps.render(name, args.preview / f"{name}.png")


if __name__ == "__main__":
    main()
