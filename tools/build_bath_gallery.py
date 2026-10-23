"""Build the bath's private tileset and map using the museum's painting frames."""

from pathlib import Path
import struct

from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / "data/tilesets/secondary/lavaridge"
MUSEUM = ROOT / "data/tilesets/secondary/lilycove_museum"
OUTPUT = ROOT / "data/tilesets/secondary/party_bath"
LAYOUT = ROOT / "data/layouts/Partybathtime"
FIRST_METATILE = 0x3C0
# Empty frame, Cool, Beauty, Cute, Smart, Tough (left and right halves).
SOURCE_METATILES = (0x258, 0x259, 0x25E, 0x25F, 0x25C, 0x25D,
                    0x260, 0x261, 0x25A, 0x25B, 0x262, 0x263)
FRAME_X = (3, 6, 9, 12, 15)
FRAME_Y = 2
PALETTE_MAP = {7: 7, 8: 12}


def words(path):
    data = path.read_bytes()
    return list(struct.unpack(f"<{len(data) // 2}H", data))


def build():
    original = Image.open(BASE / "tiles.png")
    museum = Image.open(MUSEUM / "tiles.png")
    sheet = Image.new("P", (128, 256))
    sheet.putpalette(original.getpalette())
    sheet.paste(original, (0, 0))
    tiles = words(BASE / "metatiles.bin")
    attrs = words(BASE / "metatile_attributes.bin")
    source = words(MUSEUM / "metatiles.bin")
    source_attrs = words(MUSEUM / "metatile_attributes.bin")
    entries = words(ROOT / "data/layouts/LavaridgeTown/map.bin")

    # Banks 7 and 12 are unused by the bath terrain; keep its other banks intact.
    for entry in entries:
        index = (entry & 0x3FF) - 512
        if index >= 0:
            for tile in tiles[index * 8:index * 8 + 8]:
                assert tile >> 12 not in PALETTE_MAP.values(), "Gallery palette overlaps bath terrain"
    assert len(attrs) <= FIRST_METATILE - 512
    tiles.extend([0] * ((FIRST_METATILE - 512) * 8 - len(tiles)))
    attrs.extend([0] * (FIRST_METATILE - 512 - len(attrs)))
    imported = {}
    next_tile = original.width * original.height // 64
    for index in SOURCE_METATILES:
        for value in source[(index - 512) * 8:(index - 512 + 1) * 8]:
            if value == 0:
                tiles.append(0)
                continue
            old_tile = value & 0x3FF
            assert old_tile >= 512, "Painting unexpectedly depends on the building primary tileset"
            if old_tile not in imported:
                assert next_tile < 512, "Secondary tileset exceeds VRAM capacity"
                source_x, source_y = (old_tile - 512) % 16 * 8, (old_tile - 512) // 16 * 8
                sheet.paste(museum.crop((source_x, source_y, source_x + 8, source_y + 8)),
                            (next_tile % 16 * 8, next_tile // 16 * 8))
                imported[old_tile] = next_tile + 512
                next_tile += 1
            tiles.append(PALETTE_MAP[value >> 12] << 12 | value & 0xC00 | imported[old_tile])
        attrs.append(source_attrs[index - 512])
    OUTPUT.mkdir(parents=True, exist_ok=True)
    LAYOUT.mkdir(parents=True, exist_ok=True)
    sheet.save(OUTPUT / "tiles.png")
    (OUTPUT / "metatiles.bin").write_bytes(struct.pack(f"<{len(tiles)}H", *tiles))
    (OUTPUT / "metatile_attributes.bin").write_bytes(struct.pack(f"<{len(attrs)}H", *attrs))
    for x in FRAME_X:
        for half in range(2):
            offset = FRAME_Y * 20 + x + half
            entries[offset] = (entries[offset] & 0xF000) | 0x400 | FIRST_METATILE + half
    (LAYOUT / "map.bin").write_bytes(struct.pack("<400H", *entries))
    (LAYOUT / "border.bin").write_bytes((ROOT / "data/layouts/LavaridgeTown/border.bin").read_bytes())
    print(f"Bath gallery: {next_tile} graphics tiles, {len(attrs)} secondary metatiles, five frames")


if __name__ == "__main__":
    build()
