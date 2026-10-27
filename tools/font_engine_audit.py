#!/usr/bin/env python3
"""Measure linked font data and a lossless-by-exception 1bpp prototype.

The prototype runs on the host only; it never changes game assets or code.
Run after make: python3 tools/font_engine_audit.py --elf pokeemerald.elf
"""
import argparse
import hashlib
import json
import subprocess
import tempfile
import time
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
FONTS = [
    ("LatinSmallNarrow", "gFontSmallNarrowLatinGlyphs", "latin_small_narrow", "latfont", 16),
    ("LatinSmall", "gFontSmallLatinGlyphs", "latin_small", "latfont", 16),
    ("LatinNarrow", "gFontNarrowLatinGlyphs", "latin_narrow", "latfont", 16),
    ("LatinShort", "gFontShortLatinGlyphs", "latin_short", "latfont", 16),
    ("LatinNormal", "gFontNormalLatinGlyphs", "latin_normal", "latfont", 16),
    ("LatinNarrower", "gFontNarrowerLatinGlyphs", "latin_narrower", "latfont", 16),
    ("LatinSmallNarrower", "gFontSmallNarrowerLatinGlyphs", "latin_small_narrower", "latfont", 16),
    ("LatinShortNarrow", "gFontShortNarrowLatinGlyphs", "latin_short_narrow", "latfont", 16),
    ("LatinShortNarrower", "gFontShortNarrowerLatinGlyphs", "latin_short_narrower", "latfont", 16),
    ("JapaneseSmall", "gFontSmallJapaneseGlyphs", "japanese_small", "hwjpnfont", 8),
    ("JapaneseNormal", "gFontNormalJapaneseGlyphs", "japanese_normal", "hwjpnfont", 8),
    ("JapaneseShort", "gFontShortJapaneseGlyphs", "japanese_short", "fwjpnfont", 16),
    ("JapaneseBold", "sFontBoldJapaneseGlyphs", "japanese_bold", "hwjpnfont", 8),
    ("KanjiKoden12", "sKanjiGlyphs", "kanji_koden12", "fwjpnfont", 16),
    ("FRLGMaleJapanese", "gFontFRLGMaleJapaneseGlyphs", "japanese_normal", "hwjpnfont", 8),
    ("FRLGFemaleJapanese", "gFontFRLGFemaleJapaneseGlyphs", "japanese_normal", "hwjpnfont", 8),
]


def symbols(elf):
    out = subprocess.check_output(["arm-none-eabi-nm", "-S", "--defined-only", str(elf)], text=True)
    result = {}
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 4:
            addr, size, kind, name = parts
            result[name] = (int(addr, 16), int(size, 16), kind)
        elif len(parts) == 3:
            addr, kind, name = parts
            result[name] = (int(addr, 16), 0, kind)
    return result


def glyph_cells(path, width):
    image = Image.open(path)
    if image.mode != "P" or image.width != 16 * width or image.height % 16:
        raise ValueError("Unexpected font image layout: %s" % path)
    cells = []
    for i in range((image.height // 16) * 16):
        x, y = (i % 16) * width, (i // 16) * 16
        cells.append(tuple(image.crop((x, y, x + width, y + 16)).tobytes()))
    return image, cells


def shadow_poc(cells, width):
    mismatches = []
    pixel_diffs = 0
    nonempty = 0
    for index, cell in enumerate(cells):
        if any(cell):
            nonempty += 1
        foreground = {i for i, value in enumerate(cell) if value == 1}
        generated = [0] * len(cell)
        for i in foreground:
            x, y = i % width, i // width
            if x + 1 < width and y + 1 < 16:
                generated[i + width + 1] = 2
        for i in foreground:
            generated[i] = 1
        diffs = sum(a != b for a, b in zip(cell, generated))
        if diffs:
            mismatches.append(index)
            pixel_diffs += diffs
    # Full original 2bpp for every nonmatching glyph; every glyph has a u16
    # locator. No decoder, headers, alignment or width-dependent cropping.
    matches = len(cells) - len(mismatches)
    one_bpp_bytes = matches * width * 16 // 8
    exception_bytes = len(mismatches) * width * 16 // 4
    index_bytes = len(cells) * 2
    return {"glyphs": len(cells), "nonempty": nonempty,
            "exact_glyphs": matches, "mismatch_glyphs": mismatches,
            "different_pixels": pixel_diffs,
            "pixels": len(cells) * width * 16,
            "one_bpp_payload": one_bpp_bytes,
            "full_2bpp_exceptions": exception_bytes,
            "index_bytes": index_bytes,
            "optimistic_total_without_decoder": one_bpp_bytes + exception_bytes + index_bytes}


def smol_size(input_path, temp_dir):
    compressor = ROOT / "tools/compresSmol/compresSmol"
    if not compressor.is_file():
        return None
    output, decoded = temp_dir / "font.smol", temp_dir / "font.decoded"
    started = time.perf_counter()
    subprocess.run([str(compressor), "-w", str(input_path), str(output)], check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    elapsed = time.perf_counter() - started
    subprocess.run([str(compressor), "-d", str(output), str(decoded)], check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    if decoded.read_bytes() != input_path.read_bytes():
        raise ValueError("smol roundtrip failed: %s" % input_path)
    return {"bytes": output.stat().st_size, "host_seconds": round(elapsed, 3),
            "decompressed_bytes": input_path.stat().st_size}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--elf", type=Path, default=ROOT / "pokeemerald.elf")
    parser.add_argument("--rom", type=Path, default=ROOT / "pokeemerald.gba")
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    syms, rom = symbols(args.elf), args.rom.read_bytes()
    files = []
    with tempfile.TemporaryDirectory(prefix="font-audit-") as tmp:
        for name, symbol, filename, extension, width in FONTS:
            if symbol not in syms:
                files.append({"name": name, "symbol": symbol, "linked": False})
                continue
            addr, size, kind = syms[symbol]
            path = ROOT / "graphics/fonts" / (filename + ".png")
            asset = ROOT / "build/assets/graphics/fonts" / (filename + ".png." + extension)
            raw = asset.read_bytes()
            if size != len(raw) or rom[addr - ROM_BASE:addr - ROM_BASE + size] != raw:
                raise ValueError("ROM / linked symbol / asset mismatch: " + symbol)
            image, cells = glyph_cells(path, width)
            row = {"name": name, "symbol": symbol, "linked": True,
                   "rom_address": addr, "rom_bytes": size, "symbol_type": kind,
                   "png": str(path.relative_to(ROOT)), "png_bytes": path.stat().st_size,
                   "png_size": list(image.size), "asset": str(asset.relative_to(ROOT)),
                   "asset_sha256": hashlib.sha256(raw).hexdigest(),
                   "width_table_bytes": syms.get(symbol.replace("Glyphs", "GlyphWidths"), (0, 0, ""))[1],
                   "duplicate_cells": len(cells) - len(set(cells)),
                   "one_bpp_shadow_poc": shadow_poc(cells, width),
                   "whole_font_smol": smol_size(asset, Path(tmp))}
            files.append(row)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps({"rom_sha256": hashlib.sha256(rom).hexdigest(),
                                      "rom_end": syms.get("__rom_end", (0, 0, ""))[0],
                                      "fonts": files}, ensure_ascii=False, indent=2) + "\n", encoding="utf8")


if __name__ == "__main__":
    main()
