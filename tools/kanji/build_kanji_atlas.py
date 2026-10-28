#!/usr/bin/env python3
"""Rasterize every kanji of KH Dot Kodenmachou 12 into the committed glyph atlas.

This is a one-time maintenance tool; normal builds only read the atlas through
kanji_gen.py and need neither the font nor Pillow. It requires Pillow and
fontTools and the original TTF from the official khdotfont-20150527 archive
(http://jikasei.me/font/kh-dotfont/). The font itself is not committed.

Glyphs are drawn at 12px without rescaling, exactly as the former 25-glyph
prototype did, and only the glyph body is stored (1 bit per pixel). The shadow
is added when the build generates the ROM data.
"""
import argparse
from pathlib import Path

from fontTools.ttLib import TTFont
from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[2]
ATLAS_PNG = ROOT / "graphics/fonts/kanji/koden12_atlas.png"
ATLAS_TXT = ROOT / "graphics/fonts/kanji/koden12_atlas.txt"
FONT_NAME = "KH-Dot-Kodenmachou-12"
CELL = 12
COLUMNS = 64

# CJK ideographs, plus the iteration and closing marks that behave like kanji.
RANGES = ((0x3005, 0x3007), (0x3400, 0x4DBF), (0x4E00, 0x9FFF), (0xF900, 0xFAFF))


def is_kanji(code):
    return any(low <= code <= high for low, high in RANGES)


def rasterize(font, char):
    mask = Image.new("L", (32, 32), 0)
    ImageDraw.Draw(mask).text((0, 0), char, font=font, fill=255)
    mask = mask.point(lambda value: 255 if value >= 128 else 0)
    bounds = mask.getbbox()
    if bounds is None:
        return None, "empty"
    if bounds[0] < 0 or bounds[1] < 1 or bounds[2] > CELL or bounds[3] > CELL + 1:
        return None, "does not fit 12x12: %s" % (bounds,)
    return mask.crop((0, 1, CELL, CELL + 1)), None


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("ttf", type=Path, help="KH-Dot-Kodenmachou-12.ttf from the official archive")
    args = parser.parse_args()

    ttf = TTFont(str(args.ttf), lazy=True)
    if ttf["name"].getDebugName(6) != FONT_NAME:
        raise SystemExit("Not %s: %s" % (FONT_NAME, args.ttf))
    codes = sorted(code for code in (ttf.getBestCmap() or {}) if is_kanji(code))
    ttf.close()

    font = ImageFont.truetype(str(args.ttf), CELL)
    glyphs, skipped = [], []
    for code in codes:
        image, problem = rasterize(font, chr(code))
        if problem:
            skipped.append("U+%04X %s %s" % (code, chr(code), problem))
        else:
            glyphs.append((chr(code), image))

    rows = (len(glyphs) + COLUMNS - 1) // COLUMNS
    atlas = Image.new("1", (COLUMNS * CELL, rows * CELL), 0)
    for index, (_, image) in enumerate(glyphs):
        atlas.paste(image.convert("1"), ((index % COLUMNS) * CELL, (index // COLUMNS) * CELL))
    ATLAS_PNG.parent.mkdir(parents=True, exist_ok=True)
    atlas.save(ATLAS_PNG, optimize=True)

    chars = "".join(char for char, _ in glyphs)
    lines = [chars[i:i + COLUMNS] for i in range(0, len(chars), COLUMNS)]
    ATLAS_TXT.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")

    print("%d glyphs written to %s" % (len(glyphs), ATLAS_PNG.relative_to(ROOT)))
    for line in skipped:
        print("skipped", line)


if __name__ == "__main__":
    main()
