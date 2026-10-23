"""Build and inspect the Frontier south district using original map metatiles."""

import argparse
from functools import lru_cache
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[2]
WIDTH, HEIGHT = 56, 42
TILESETS = {
    "gTileset_General": "primary/general",
    "gTileset_Lilycove": "secondary/lilycove",
    "gTileset_BattleFrontierOutsideWest": "secondary/battle_frontier_outside_west",
    "gTileset_BattleFrontierOutsideEast": "secondary/battle_frontier_outside_east",
    "gTileset_Fortree": "secondary/fortree",
}


def layout(name):
    layouts = json.loads((ROOT / "data/layouts/layouts.json").read_text())["layouts"]
    return next(entry for entry in layouts if entry["name"] == name + "_Layout")


def read_map(name, old_height=None):
    entry = layout(name)
    raw = (ROOT / entry["blockdata_filepath"]).read_bytes()
    width, height = entry["width"], entry["height"]
    if old_height is not None and len(raw) == width * old_height * 2:
        height = old_height
    assert len(raw) == width * height * 2
    cells = struct.unpack("<" + "H" * (width * height), raw)
    return [list(cells[y * width:(y + 1) * width]) for y in range(height)]


def pack_map(cells):
    values = [cell for row in cells for cell in row]
    return struct.pack("<" + "H" * len(values), *values)


def fill(cells, x, y, width, height, value):
    for row in cells[y:y + height]:
        row[x:x + width] = [value] * width


def copy_building(cells, source, rect, destination):
    x, y, width, height = rect
    dx, dy = destination
    for j in range(height):
        for i in range(width):
            # Keep the source collision/metatile, with one ground elevation.
            cells[dy + j][dx + i] = (source[y + j][x + i] & 0xFFF) | 0x3000


def generated_maps():
    city = read_map("LilycoveCity")
    cells = [[0x3001] * WIDTH for _ in range(HEIGHT)]
    tree = ((0x35D4, 0x35D5), (0x35DC, 0x35DD))

    def trees(x, y, width, height):
        for j in range(height):
            for i in range(width):
                cells[y + j][x + i] = tree[j % 2][i % 2]

    trees(0, 0, WIDTH, 2)
    trees(0, HEIGHT - 2, WIDTH, 2)
    trees(0, 2, 2, HEIGHT - 4)
    trees(WIDTH - 2, 2, 2, HEIGHT - 4)
    trees(4, 18, 6, 12)
    trees(44, 6, 6, 10)
    trees(34, 34, 16, 4)
    fill(cells, 24, 0, 6, 38, 0x305E)
    fill(cells, 9, 12, 18, 3, 0x305E)
    fill(cells, 27, 14, 11, 3, 0x305E)
    fill(cells, 17, 18, 14, 10, 0x305E)
    fill(cells, 27, 29, 13, 3, 0x305E)
    fill(cells, 18, 30, WIDTH - 18, 4, 0x305E)
    copy_building(cells, city, (36, 20, 7, 5), (8, 7))
    copy_building(cells, city, (20, 18, 7, 7), (34, 7))
    copy_building(cells, city, (7, 0, 10, 9), (34, 20))
    for x in (34, 35, 42, 43):
        cells[20][x] = 0x3001
    for x, y in ((8, 12), (40, 14), (45, 29), (22, 4), (52, 29)):
        cells[y][x] = 0x3403
    for x, y in ((19, 20), (28, 20), (19, 25), (28, 25)):
        cells[y][x] = 0x3004

    # Seven preview rows on either side of a connection must use the shared
    # primary tileset; the Frontier gate itself stays on the original map.
    west = read_map("BattleFrontier_OutsideWest", old_height=72)
    west.extend([[0x0170] * 56 for _ in range(80 - len(west))])
    fill(west, 24, 68, 6, 12, 0x005E)
    park = read_map("party_roamer_park")
    fill(park, 0, 17, 5, 4, 0x305E)
    park[12][3] = park[13][3] = 0x3001
    return {
        "data/layouts/BattleFrontier_OutsideSouth/map.bin": pack_map(cells),
        "data/layouts/BattleFrontier_OutsideSouth/border.bin": struct.pack("<4H", 0x35D4, 0x35D5, 0x35DC, 0x35DD),
        "data/layouts/BattleFrontier_OutsideWest/map.bin": pack_map(west),
        "data/layouts/PartyRoamerPark/map.bin": pack_map(park),
    }


def build(check=False):
    for relative, data in generated_maps().items():
        path = ROOT / relative
        if check:
            if not path.exists() or path.read_bytes() != data:
                raise ValueError(f"outdated generated map: {relative}")
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(data)


def render(name, output):
    from PIL import Image

    entry = layout(name)
    directories = [ROOT / "data/tilesets" / TILESETS[entry[key]]
                   for key in ("primary_tileset", "secondary_tileset")]
    sheets = [Image.open(directory / "tiles.png") for directory in directories]
    metatiles = [directory.joinpath("metatiles.bin").read_bytes() for directory in directories]
    palettes = []
    for bank in range(16):
        path = directories[bank >= 6] / f"palettes/{bank:02}.pal"
        colors = [tuple(map(int, line.split())) for line in path.read_text().splitlines()[3:19]]
        palettes.append(colors)

    @lru_cache(None)
    def metatile(index):
        raw = metatiles[index >= 512]
        words = struct.unpack_from("<8H", raw, (index % 512) * 16)
        image = Image.new("RGB", (16, 16), palettes[0][0])
        for i, word in enumerate(words):
            tile = word & 1023
            sheet = sheets[tile >= 512]
            tile %= 512
            columns = sheet.width // 8
            pixels = sheet.crop((tile % columns * 8, tile // columns * 8,
                                 tile % columns * 8 + 8, tile // columns * 8 + 8))
            if word & 0x400:
                pixels = pixels.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
            if word & 0x800:
                pixels = pixels.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
            for y in range(8):
                for x in range(8):
                    color = pixels.getpixel((x, y)) % 16
                    if i < 4 or color:
                        image.putpixel((i % 2 * 8 + x, i % 4 // 2 * 8 + y), palettes[word >> 12][color])
        return image

    cells = read_map(name)
    result = Image.new("RGB", (len(cells[0]) * 16, len(cells) * 16))
    for y, row in enumerate(cells):
        for x, cell in enumerate(row):
            result.paste(metatile(cell & 1023), (x * 16, y * 16))
    output = Path(output)
    output.parent.mkdir(parents=True, exist_ok=True)
    result.save(output)


def main():
    parser = argparse.ArgumentParser(__doc__)
    parser.add_argument("--render", metavar="MAP")
    parser.add_argument("--output", type=Path)
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    if args.render:
        if not args.output:
            parser.error("--render requires --output")
        render(args.render, args.output)
    else:
        build(args.check)


if __name__ == "__main__":
    main()
