#!/usr/bin/env python3
"""Rasterize KH Dot Kodenmachou 12 test glyphs at 12px, without rescaling.

Requires Pillow and fontTools. Pass the original TTF, or converted TTF subsets
that together cover GLYPHS. The font itself is not included in this repository.
"""
import argparse
from pathlib import Path

from fontTools.ttLib import TTFont
from PIL import Image, ImageChops, ImageDraw, ImageFont

GLYPHS = "攻撃防御特性技力速命中状態異常回復対戦相手物理変化"
ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("fonts", nargs="+", type=Path, help="original TTF or matching TTF subsets")
    args = parser.parse_args()
    coverage = {}
    for path in args.fonts:
        font = TTFont(str(path), lazy=True)
        name = font["name"].getDebugName(6)
        if name != "KH-Dot-Kodenmachou-12":
            font.close()
            raise ValueError("Not KH Dot Kodenmachou 12: %s (%s)" % (path, name))
        cmap = font.getBestCmap() or {}
        for char in GLYPHS:
            if ord(char) in cmap:
                coverage.setdefault(char, path)
        font.close()
    missing = [char for char in GLYPHS if char not in coverage]
    if missing:
        raise ValueError("Font is missing glyphs: " + " ".join(missing))

    sheet = Image.new("P", (256, 32), 0)
    sheet.putpalette([144, 200, 255, 56, 56, 56, 216, 216, 216, 255, 255, 255] + [0] * 756)
    fonts = {}
    for index, char in enumerate(GLYPHS, 1):
        path = coverage[char]
        if path not in fonts:
            fonts[path] = ImageFont.truetype(str(path), 12)
        mask = Image.new("L", (32, 32), 0)
        ImageDraw.Draw(mask).text((0, 0), char, font=fonts[path], fill=255)
        mask = mask.point(lambda value: 255 if value >= 128 else 0)
        bounds = mask.getbbox()
        if bounds is None or bounds[0] < 0 or bounds[1] < 1 or bounds[2] > 12 or bounds[3] > 13:
            raise ValueError("Glyph does not fit 12x12 without clipping: %s %s" % (char, bounds))
        mask = mask.crop((0, 1, 12, 13))
        # Keep the shadow inside the existing 12px advance and small-font height.
        shadow = ImageChops.offset(mask, 1, 1)
        ImageDraw.Draw(shadow).line((0, 0, 11, 0), fill=0)
        ImageDraw.Draw(shadow).line((0, 0, 0, 11), fill=0)
        position = ((index % 16) * 16, (index // 16) * 16)
        sheet.paste(2, position, shadow)
        sheet.paste(1, position, mask)
    sheet.save(ROOT / "graphics/fonts/kanji_koden12.png", bits=2)


if __name__ == "__main__":
    main()
