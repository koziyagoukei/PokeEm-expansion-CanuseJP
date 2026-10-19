"""Build the local room's map and private, behavior-compatible metatile table."""

import argparse
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[2]
WIDTH, HEIGHT = 16, 14

# Append styles only: their indices are stored in existing saves.
FLOOR_SOURCES = (
    ("brendans_mays_house", 0x201),
    ("generic_building", 0x224),
    ("lilycove_museum", 0x269),
    ("seashore_house", 0x224),
    ("pokemon_day_care", 0x201),
    ("pokemon_fan_club", 0x22A),
    ("brendans_mays_house", 0x2A4),
)
WALL_SOURCES = (
    ("brendans_mays_house", 0x276, 0x26E),
    ("brendans_mays_house", 0x276, 0x26E),
    ("seashore_house", 0x20A, 0x20A),
    ("pokemon_fan_club", 0x21D, 0x21C),
    ("generic_building", 0x32C, 0x32C),
    ("pokemon_day_care", 0x209, 0x208),
    ("generic_building", 0x20D, 0x218),
)


def indexed_pixels(image):
    return list(image.get_flattened_data() if hasattr(image, "get_flattened_data") else image.getdata())


def house_palette(name, metatile):
    directory = ROOT / "data/tilesets/secondary" / name
    raw = (directory / "metatiles.bin").read_bytes()
    words = struct.unpack_from("<8H", raw, (metatile - 512) * 16)[:4]
    banks = {word >> 12 for word in words}
    if len(banks) != 1:
        raise ValueError(f"{name}:{metatile:03X}: room texture needs a single palette")
    bank = banks.pop()
    directory = directory if bank >= 6 else ROOT / "data/tilesets/primary/building"
    return [tuple(map(int, line.split())) for line in (directory / f"palettes/{bank:02}.pal").read_text().splitlines()[3:19]]


def pack_palette(colors):
    return struct.pack("<16H", *(r // 8 | g // 8 << 5 | b // 8 << 10 for r, g, b in colors))


def house_texture(name, metatile):
    from PIL import Image

    directory = ROOT / "data/tilesets/secondary" / name
    raw = (directory / "metatiles.bin").read_bytes()
    words = struct.unpack_from("<8H", raw, (metatile - 512) * 16)
    sheets = [Image.open(ROOT / "data/tilesets/primary/building/tiles.png"), Image.open(directory / "tiles.png")]
    result = bytearray()
    for layer, word in enumerate(words):
        index = word & 1023
        sheet = sheets[index >= 512]
        index %= 512
        columns = sheet.width // 8
        image = sheet.crop((index % columns * 8, index // columns * 8,
                            index % columns * 8 + 8, index // columns * 8 + 8))
        if word & 0x400:
            image = image.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
        if word & 0x800:
            image = image.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
        pixels = indexed_pixels(image)
        if layer >= 4:
            if any(pixel % 16 for pixel in pixels):
                raise ValueError(f"{name}:{metatile:03X}: furniture or wall overlay is not a room texture")
            continue
        result.extend((pixels[i] % 16) | (pixels[i + 1] % 16 << 4) for i in range(0, 64, 2))
    return result


def secret_base_tiles():
    from PIL import Image

    sheet = Image.open(ROOT / "data/tilesets/secondary/secret_base/red_cave/tiles.png")
    result = bytearray()
    columns = sheet.width // 8
    for index in range(83):
        image = sheet.crop((index % columns * 8, index // columns * 8,
                            index % columns * 8 + 8, index // columns * 8 + 8))
        pixels = indexed_pixels(image)
        result.extend((pixels[i] % 16) | (pixels[i + 1] % 16 << 4) for i in range(0, 64, 2))
    result.extend(bytes(32 * (0x265 - 512 - 83)))
    return result


def copy_tiles(destination, source, indices):
    for i, index in enumerate(indices):
        destination[index * 32:(index + 1) * 32] = source[i * 32:(i + 1) * 32]


def build():
    source = ROOT / "data/tilesets/secondary/secret_base"
    destination = ROOT / "data/tilesets/secondary/local_room"
    destination.mkdir(parents=True, exist_ok=True)
    raw = (source / "metatiles.bin").read_bytes()
    words = list(struct.unpack("<" + "H" * (len(raw) // 2), raw))
    floor_tiles = {0x224, 0x225, 0x234, 0x235}
    for i, word in enumerate(words):
        if word >> 12 == 6:
            bank = 7 if word & 0x3FF in floor_tiles else 8
            words[i] = (word & 0x0FFF) | (bank << 12)

    floors = [house_texture(*source) for source in FLOOR_SOURCES]
    walls = [house_texture(name, lower) + house_texture(name, upper) for name, lower, upper in WALL_SOURCES]
    (destination / "floors.bin").write_bytes(b"".join(floors))
    (destination / "walls.bin").write_bytes(b"".join(walls))
    floor_palettes = [house_palette(*source) for source in FLOOR_SOURCES]
    wall_palettes = [house_palette(name, lower) for name, lower, _ in WALL_SOURCES]
    for palette, (name, _, upper) in zip(wall_palettes, WALL_SOURCES):
        if palette != house_palette(name, upper):
            raise ValueError(f"{name}: upper/lower wall palettes differ")
    # Reuse the house's existing blue shades for the blue wallpaper variant.
    wall_palettes[1] = wall_palettes[1].copy()
    for destination_index, source_index in ((3, 10), (4, 12), (9, 11)):
        wall_palettes[1][destination_index] = wall_palettes[0][source_index]
    (destination / "floor_palettes.bin").write_bytes(b"".join(map(pack_palette, floor_palettes)))
    (destination / "wall_palettes.bin").write_bytes(b"".join(map(pack_palette, wall_palettes)))
    graphics = secret_base_tiles()
    copy_tiles(graphics, floors[0], (0x24, 0x25, 0x34, 0x35))
    copy_tiles(graphics, walls[0][:128], (0x40, 0x41, 0x50, 0x51))
    copy_tiles(graphics, walls[0][:128], (0x04, 0x05, 0x14, 0x15))
    copy_tiles(graphics, walls[0][128:], (0x60, 0x61, 0x62, 0x63))
    graphics[0x64 * 32:0x65 * 32] = bytes([0x11] * 32)

    # Private copies: preserve all furniture behaviors, replace only the room's building shell.
    blank = [0x9264] * 4 + [0] * 4
    for index in (0, 1, 3, 9, 11, 12, 16, 17, 18, 19):
        words[index * 8:index * 8 + 8] = blank
    words[2 * 8:2 * 8 + 8] = [0x8240, 0x8241, 0x8250, 0x8251, 0, 0, 0, 0]
    words.extend((0x8260, 0x8261, 0x8262, 0x8263, 0, 0, 0, 0))

    # The existing house entrance mat, with its own palette and private tile slots.
    for index, metatile in enumerate((0x202, 0x203)):
        texture = house_texture("brendans_mays_house", metatile)
        base = len(graphics) // 32
        graphics.extend(texture)
        entry = [0xA000 | (512 + base + i) for i in range(4)] + [0] * 4
        if index == 0:
            words[4 * 8:4 * 8 + 8] = entry
        else:
            words.extend(entry)

    (destination / "tiles.bin").write_bytes(graphics)
    (destination / "metatiles.bin").write_bytes(struct.pack("<" + "H" * len(words), *words))
    attributes = (source / "metatile_attributes.bin").read_bytes() + struct.pack("<2H", 1, 0x65)
    (destination / "metatile_attributes.bin").write_bytes(attributes)


def build_map():
    layout = ROOT / "data/layouts/MiniMiniVilla_LocalRoom"
    layout.mkdir(parents=True, exist_ok=True)
    tiles = [0x0610] * (WIDTH * HEIGHT)
    for x in range(1, WIDTH - 1):
        tiles[1 * WIDTH + x] = 0x0744
        tiles[2 * WIDTH + x] = 0x0602
        tiles[12 * WIDTH + x] = 0x0612
    tiles[2 * WIDTH] = 0x0601
    tiles[2 * WIDTH + WIDTH - 1] = 0x0603
    tiles[12 * WIDTH] = 0x0611
    tiles[12 * WIDTH + WIDTH - 1] = 0x0613
    for y in range(3, 12):
        tiles[y * WIDTH] = 0x0609
        tiles[y * WIDTH + WIDTH - 1] = 0x060B
        for x in range(1, WIDTH - 1):
            tiles[y * WIDTH + x] = 0x320A
    tiles[3 * WIDTH + 1] = 0x3620
    tiles[12 * WIDTH + 7] = 0x3204
    tiles[12 * WIDTH + 8] = 0x3345
    (layout / "map.bin").write_bytes(struct.pack("<" + "H" * len(tiles), *tiles))
    (layout / "border.bin").write_bytes(struct.pack("<4H", 0x0210, 0x0210, 0x0210, 0x0210))

    events = []
    for i in range(15):
        roamer = i >= 12
        events.append({
            "graphics_id": f"OBJ_EVENT_GFX_VAR_{i:X}",
            "x": 0 if not roamer else (3, 12, 12)[i - 12],
            "y": 0 if not roamer else (6, 6, 9)[i - 12],
            "elevation": 3,
            "movement_type": "MOVEMENT_TYPE_WANDER_AROUND" if roamer else "MOVEMENT_TYPE_FACE_DOWN",
            "movement_range_x": 1 if roamer else 0,
            "movement_range_y": 1 if roamer else 0,
            "trainer_type": "TRAINER_TYPE_NONE",
            "trainer_sight_or_berry_tree_id": "0",
            "script": "LocalRoom_EventScript_Mon" if roamer else "0x0",
            "flag": f"FLAG_TEMP_{17 + i - 12:02X}" if roamer else f"FLAG_DECORATION_{i + 1}",
        })
    map_data = {
        "id": "MAP_MINI_MINI_VILLA_LOCAL_ROOM",
        "name": "MiniMiniVilla_LocalRoom",
        "layout": "LAYOUT_MINI_MINI_VILLA_LOCAL_ROOM",
        "music": "MUS_FORTREE",
        "region_map_section": "MAPSEC_LOCAL_ROOM",
        "requires_flash": False,
        "weather": "WEATHER_NONE",
        "map_type": "MAP_TYPE_INDOOR",
        "allow_cycling": False,
        "allow_escaping": False,
        "allow_running": True,
        "show_map_name": True,
        "battle_scene": "MAP_BATTLE_SCENE_NORMAL",
        "connections": None,
        "object_events": events,
        "warp_events": [
            {"x": x, "y": 12, "elevation": 0, "dest_map": "MAP_MINI_MINI_VILLA", "dest_warp_id": "1"}
            for x in (7, 8)
        ],
        "coord_events": [],
        "bg_events": [
            {"type": "sign", "x": 1, "y": 3, "elevation": 3, "player_facing_dir": "BG_EVENT_PLAYER_FACING_NORTH", "script": "LocalRoom_EventScript_PC"}
        ],
    }
    path = ROOT / "data/maps/MiniMiniVilla_LocalRoom/map.json"
    path.write_text(json.dumps(map_data, indent=2) + "\n", encoding="utf-8")


def preview(floor_style=0, wall_style=0):
    from functools import lru_cache
    from PIL import Image

    primary = ROOT / "data/tilesets/primary/secret_base"
    secondary = ROOT / "data/tilesets/secondary/secret_base/red_cave"
    directory = ROOT / "data/tilesets/secondary/local_room"
    raw_graphics = bytearray((directory / "tiles.bin").read_bytes())
    floor_tiles = (directory / "floors.bin").read_bytes()[floor_style * 128:(floor_style + 1) * 128]
    wall_tiles = (directory / "walls.bin").read_bytes()[wall_style * 256:(wall_style + 1) * 256]
    copy_tiles(raw_graphics, floor_tiles, (0x24, 0x25, 0x34, 0x35))
    copy_tiles(raw_graphics, wall_tiles[:128], (0x40, 0x41, 0x50, 0x51))
    copy_tiles(raw_graphics, wall_tiles[:128], (0x04, 0x05, 0x14, 0x15))
    copy_tiles(raw_graphics, wall_tiles[128:], (0x60, 0x61, 0x62, 0x63))
    secondary_sheet = Image.new("P", (128, ((len(raw_graphics) // 32 + 15) // 16) * 8))
    for index in range(len(raw_graphics) // 32):
        pixels = [value for byte in raw_graphics[index * 32:(index + 1) * 32] for value in (byte & 15, byte >> 4)]
        tile_image = Image.new("P", (8, 8))
        tile_image.putdata(pixels)
        secondary_sheet.paste(tile_image, (index % 16 * 8, index // 16 * 8))
    sheets = [Image.open(primary / "tiles.png"), secondary_sheet]
    meta = []
    for path in (primary, ROOT / "data/tilesets/secondary/local_room"):
        raw = (path / "metatiles.bin").read_bytes()
        meta.append(struct.unpack("<" + "H" * (len(raw) // 2), raw))
    palettes = []
    for bank in range(16):
        path = (primary if bank < 6 else secondary) / f"palettes/{bank:02}.pal"
        if bank == 10:
            path = ROOT / "data/tilesets/secondary/brendans_mays_house/palettes/07.pal"
        palettes.append([tuple(map(int, line.split())) for line in path.read_text().splitlines()[3:19]])
    palettes[9] = [(0, 0, 0)] * 16
    for bank, name, style in ((7, "floor", floor_style), (8, "wall", wall_style)):
        colors = struct.unpack_from("<16H", (directory / f"{name}_palettes.bin").read_bytes(), style * 32)
        palettes[bank] = [((c & 31) * 255 // 31, (c >> 5 & 31) * 255 // 31, (c >> 10 & 31) * 255 // 31) for c in colors]

    @lru_cache(None)
    def tile(word):
        index = word & 1023
        sheet = sheets[index >= 512]
        index %= 512
        columns = sheet.width // 8
        pixels = sheet.crop((index % columns * 8, index // columns * 8,
                             index % columns * 8 + 8, index // columns * 8 + 8))
        image = Image.new("RGBA", (8, 8))
        image.putdata([(*palettes[word >> 12][value % 16], 255 if value % 16 else 0) for value in indexed_pixels(pixels)])
        if word & 0x400:
            image = image.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
        if word & 0x800:
            image = image.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
        return image

    data = (ROOT / "data/layouts/MiniMiniVilla_LocalRoom/map.bin").read_bytes()
    words = struct.unpack("<" + "H" * (WIDTH * HEIGHT), data)
    image = Image.new("RGBA", (WIDTH * 16, HEIGHT * 16), (0, 0, 0, 255))
    for y in range(HEIGHT):
        for x in range(WIDTH):
            index = words[y * WIDTH + x] & 1023
            table = meta[index >= 512]
            index %= 512
            for layer in range(2):
                for quadrant in range(4):
                    image.alpha_composite(tile(table[index * 8 + layer * 4 + quadrant]),
                        (x * 16 + quadrant % 2 * 8, y * 16 + quadrant // 2 * 8))
    return image.convert("RGB")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--preview", action="store_true")
    parser.add_argument("--assets-only", action="store_true", help="Leave the map layout and events untouched")
    args = parser.parse_args()
    build()
    if not args.assets_only:
        build_map()
    if args.preview:
        from PIL import Image, ImageDraw
        preview().resize((WIDTH * 48, HEIGHT * 48), Image.Resampling.NEAREST).save(ROOT / "build/local_room_preview.png")
        sheet = Image.new("RGB", (WIDTH * 16 * len(WALL_SOURCES), (HEIGHT * 16 + 24) * len(FLOOR_SOURCES)), "#333333")
        draw = ImageDraw.Draw(sheet)
        for floor_style in range(len(FLOOR_SOURCES)):
            for wall_style in range(len(WALL_SOURCES)):
                x, y = wall_style * WIDTH * 16, floor_style * (HEIGHT * 16 + 24)
                sheet.paste(preview(floor_style, wall_style), (x, y + 24))
                draw.text((x + 8, y + 4), f"Floor {floor_style} / Wall {wall_style}", fill="white")
        sheet.save(ROOT / "build/local_room_styles.png")
