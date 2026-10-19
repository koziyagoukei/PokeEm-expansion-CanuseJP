"""Verify local-room boundaries, isolated metatiles, persistent layout and link exclusion."""

import argparse
import json
from pathlib import Path
import re
import struct
import subprocess

from build_assets import FLOOR_SOURCES, WALL_SOURCES, house_palette, house_texture, pack_palette

ROOT = Path(__file__).resolve().parents[2]


def read(path):
    return (ROOT / path).read_text(encoding="utf-8")


def original(path):
    return subprocess.check_output(["git", "show", f"HEAD:{path}"], cwd=ROOT).decode("utf-8")


def structure(text, name):
    match = re.search(r"struct " + re.escape(name) + r"\s*\{", text)
    if not match:
        raise ValueError(name)
    start, depth = match.end(), 1
    for index in range(start, len(text)):
        if text[index] == "{":
            depth += 1
        elif text[index] == "}":
            depth -= 1
            if depth == 0:
                return text[start:index]
    raise ValueError(name)


def main(check_textures=False):
    header = read("include/global.h")
    before = original("include/global.h")
    for name in ("SaveBlock1", "SaveBlock2", "SecretBase", "SecretBaseParty"):
        assert structure(header, name) == structure(before, name), name
    sb3 = structure(header, "SaveBlock3")
    previous = structure(before, "SaveBlock3")
    if "struct LocalRoomSave localRoom;" in previous:
        assert sb3 == previous
        assert structure(header, "LocalRoomSave") == structure(before, "LocalRoomSave")
    else:
        assert sb3.startswith(previous.rstrip())
        assert "struct LocalRoomSave localRoom;" in sb3[len(previous.rstrip()):]
    for path in ("src/record_mixing.c", "src/link.c", "src/link_rfu_2.c", "src/link_rfu_3.c", "src/link_compat.c", "include/config/link_compat.h"):
        assert read(path) == original(path), path
        assert not re.search(r"localRoom|LocalRoom|LocalRoomSave|gSaveBlock3Ptr", read(path)), path
    assert "secretBases" not in read("src/local_room.c")

    group_path = "data/maps/map_groups.json"
    groups, old_groups = json.loads(read(group_path)), json.loads(original(group_path))
    assert groups["group_order"][:len(old_groups["group_order"])] == old_groups["group_order"]
    for group in old_groups["group_order"]:
        assert groups[group] == old_groups[group], group
    layout_path = "data/layouts/layouts.json"
    layouts, old_layouts = json.loads(read(layout_path)), json.loads(original(layout_path))
    assert layouts["layouts"][:len(old_layouts["layouts"])] == old_layouts["layouts"]

    room = json.loads(read("data/maps/MiniMiniVilla_LocalRoom/map.json"))
    villa_path = "data/maps/MiniMiniVilla/map.json"
    villa, old_villa = json.loads(read(villa_path)), json.loads(original(villa_path))
    assert len(villa["warp_events"]) == len(old_villa["warp_events"])
    assert villa["warp_events"][0] == old_villa["warp_events"][0]
    assert (villa["warp_events"][1]["x"], villa["warp_events"][1]["y"]) == (3, 6)
    assert villa["warp_events"][1]["dest_map"] == room["id"]
    assert all(warp["dest_map"] == villa["id"] and warp["dest_warp_id"] == "1" for warp in room["warp_events"])
    assert room["map_type"] == "MAP_TYPE_INDOOR"
    assert "shared_scripts_map" not in room
    assert len(room["object_events"]) == 15
    assert [event["flag"] for event in room["object_events"][:12]] == [f"FLAG_DECORATION_{i}" for i in range(1, 13)]
    assert [event["flag"] for event in room["object_events"][12:]] == ["FLAG_TEMP_11", "FLAG_TEMP_12", "FLAG_TEMP_13"]
    assert all(event["trainer_type"] == "TRAINER_TYPE_NONE" for event in room["object_events"])

    attrs_path = ROOT / "data/tilesets/secondary/local_room/metatile_attributes.bin"
    attrs = attrs_path.read_bytes()
    old_attrs = (ROOT / "data/tilesets/secondary/secret_base/metatile_attributes.bin").read_bytes()
    assert attrs[:len(old_attrs)] == old_attrs
    metatiles = (ROOT / "data/tilesets/secondary/local_room/metatiles.bin").read_bytes()
    graphics = (ROOT / "data/tilesets/secondary/local_room/tiles.bin").read_bytes()
    assert len(metatiles) == len(attrs) * 8
    assert len(graphics) % 32 == 0
    words = struct.unpack("<" + "H" * (len(metatiles) // 2), metatiles)
    assert all(word & 1023 < 512 + len(graphics) // 32 for word in words)
    style_count = int(re.search(r"#define LOCAL_ROOM_STYLE_COUNT (\d+)", read("include/constants/local_room.h"))[1])
    assert len(FLOOR_SOURCES) == len(WALL_SOURCES) == style_count
    assert (style_count + 1) % 2 == 0  # Grid menus need complete rows, including Exit.
    assert len((ROOT / "data/tilesets/secondary/local_room/floors.bin").read_bytes()) == style_count * 128
    assert len((ROOT / "data/tilesets/secondary/local_room/walls.bin").read_bytes()) == style_count * 256
    directory = ROOT / "data/tilesets/secondary/local_room"
    if check_textures:
        assert (directory / "floors.bin").read_bytes() == b"".join(house_texture(*source) for source in FLOOR_SOURCES)
        assert (directory / "walls.bin").read_bytes() == b"".join(house_texture(name, lower) + house_texture(name, upper) for name, lower, upper in WALL_SOURCES)
    floor_palettes = b"".join(pack_palette(house_palette(*source)) for source in FLOOR_SOURCES)
    assert (directory / "floor_palettes.bin").read_bytes() == floor_palettes
    wall_palettes = (directory / "wall_palettes.bin").read_bytes()
    assert len(wall_palettes) == style_count * 32
    for i, (name, lower, upper) in enumerate(WALL_SOURCES):
        palette = house_palette(name, lower)
        assert palette == house_palette(name, upper)
        if i == 1:
            for destination, source in ((3, 10), (4, 12), (9, 11)):
                palette[destination] = house_palette(*WALL_SOURCES[0][:2])[source]
        assert wall_palettes[i * 32:(i + 1) * 32] == pack_palette(palette)
    assert wall_palettes[:32] != wall_palettes[32:64]
    assert len({(directory / "floors.bin").read_bytes()[i * 128:(i + 1) * 128] for i in range(style_count)}) == style_count
    for name in ("floors.bin", "walls.bin", "floor_palettes.bin", "wall_palettes.bin"):
        path = directory / name
        old = subprocess.check_output(["git", "show", f"HEAD:{path.relative_to(ROOT).as_posix()}"], cwd=ROOT)
        assert path.read_bytes().startswith(old), f"Existing style IDs changed: {name}"
    menus = read("src/data/script_menu.h")
    for name in ("Floor", "Wall"):
        menu = re.search(r"sMultichoiceList_LocalRoom" + name + r"Style\[\]\s*=\s*\{(.*?)\};", menus, re.S)[1]
        assert menu.count("COMPOUND_STRING") == style_count
        assert menu.count("gText_Exit") == 1

    raw = (ROOT / "data/layouts/MiniMiniVilla_LocalRoom/map.bin").read_bytes()
    assert len(raw) == 16 * 14 * 2
    tiles = struct.unpack("<224H", raw)
    def behavior(x, y):
        return struct.unpack_from("<H", attrs, ((tiles[y * 16 + x] & 1023) - 512) * 2)[0] & 255
    assert all(behavior(x, 2) == 0xB7 for x in range(1, 15))  # north-wall posters
    assert all(behavior(x, y) == 0 for y in range(3, 12) for x in range(1, 15) if (x, y) != (1, 3))
    assert behavior(1, 3) == 0xB0  # PC
    for x in (7, 8):
        assert behavior(x, 12) == 0x65
        assert all(tiles[y * 16 + x] & 0x0C00 == 0 for y in range(4, 13))
    assert all(tiles[4 * 16 + x] & 0x0C00 == 0 for x in range(1, 9))
    script = read("data/maps/MiniMiniVilla_LocalRoom/scripts.inc")
    assert "RecordMix" not in script and "Registry" not in script and "VAR_CURRENT_SECRET_BASE" not in script
    assert "setflag VAR_0x8005" not in script
    for name in ("FLOOR", "WALL"):
        assert f"multichoicegrid 0, 0, MULTI_LOCAL_ROOM_{name}_STYLE, 2, FALSE" in script
    print("PASS: SaveBlock1/2, SecretBase and existing SaveBlock3 prefix unchanged")
    print("PASS: record-mixing/link sources and compatibility versions unchanged; no local data references")
    print("PASS: existing map/layout IDs preserved, reciprocal house warps, no secret-base owner/registry scripts")
    print("PASS: 12 furniture object slots + 3 roamers + player fit; graphics and coordinates bounded")
    print("PASS: floor/wall/PC/exit behaviors, protected access path, private house-style tiles")
    print("PASS: room styles have distinct floor tiles and corresponding palette banks")
    if check_textures:
        print("PASS: textures match original building tiles; no furniture/curtain overlays")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--textures", action="store_true", help="Compare original PNG textures (requires Pillow)")
    main(parser.parse_args().textures)
