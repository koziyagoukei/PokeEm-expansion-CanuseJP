"""Compose furniture over matching floors without changing shared house graphics."""

from pathlib import Path
from io import BytesIO
import struct

from PIL import Image

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "data/tilesets/secondary/brendans_mays_house"
OUTPUT = ROOT / "data/tilesets/secondary/resort_house"
FURNITURE = (0x219, 0x21A, 0x21B, 0x21C, 0x221, 0x222, 0x223, 0x224,
             0x240, 0x241, 0x248, 0x249, 0x24A)
FIRST = 0x2C4
PLANT_TOP = FIRST + len(FURNITURE) * 2
PLANT_BOTTOM = PLANT_TOP + 1


def furniture(source, carpet):
    return FIRST + FURNITURE.index(source) + (len(FURNITURE) if carpet else 0)


def generated_assets():
    raw = (SOURCE / "metatiles.bin").read_bytes()
    attrs = (SOURCE / "metatile_attributes.bin").read_bytes()
    assert len(raw) == (FIRST - 512) * 16
    assert len(attrs) == (FIRST - 512) * 2
    output, attributes = bytearray(raw), bytearray(attrs)
    for floor in (0x201, 0x2A1):
        for source in FURNITURE:
            # The original top layer contains the complete chair/table/cabinet.
            output.extend(raw[(floor - 512) * 16:(floor - 512) * 16 + 8])
            output.extend(raw[(source - 512) * 16 + 8:(source - 511) * 16])
            attributes.extend(attrs[(source - 512) * 2:(source - 511) * 2])
    # Import one complete plant using unused graphics slots and palette 12.
    # House floor/furniture colors and the source tilesets stay unchanged.
    house_sheet = Image.open(SOURCE / "tiles.png")
    sheet = Image.new("P", (128, 248))
    sheet.putpalette(house_sheet.getpalette())
    sheet.paste(house_sheet, (0, 0))
    shop = ROOT / "data/tilesets/secondary/shop"
    shop_sheet = Image.open(shop / "tiles.png")
    shop_tiles = (shop / "metatiles.bin").read_bytes()
    shop_attrs = (shop / "metatile_attributes.bin").read_bytes()
    destination = 992
    for source in (0x225, 0x22D):
        output.extend(raw[16:24])
        for word in struct.unpack_from("<4H", shop_tiles, (source - 512) * 16 + 8):
            assert word >> 12 == 8
            offset = (word & 1023) - 512
            sheet.paste(shop_sheet.crop((offset % 16 * 8, offset // 16 * 8,
                                        offset % 16 * 8 + 8, offset // 16 * 8 + 8)),
                        ((destination - 512) % 16 * 8, (destination - 512) // 16 * 8))
            output.extend(struct.pack("<H", 0xC000 | word & 0xC00 | destination))
            destination += 1
        attributes.extend(shop_attrs[(source - 512) * 2:(source - 511) * 2])
    buffer = BytesIO()
    sheet.save(buffer, format="PNG")
    assert len(output) // 16 + 512 <= 1024
    return {OUTPUT / "metatiles.bin": output, OUTPUT / "metatile_attributes.bin": attributes,
            OUTPUT / "tiles.png": buffer.getvalue()}


def build(check=False):
    for path, content in generated_assets().items():
        if check:
            assert path.read_bytes() == content, f"outdated tileset: {path}"
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(content)


if __name__ == "__main__":
    build()
