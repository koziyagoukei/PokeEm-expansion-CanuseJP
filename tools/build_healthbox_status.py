#!/usr/bin/env python3
"""Build status-icon tiles with the healthbox palette slot for one battler."""

from __future__ import annotations

import argparse
from pathlib import Path
import struct
import subprocess
import sys
import tempfile


STATUS_COLOR_START = 12
BATTLER_COUNT = 4
ICON_COUNT = 6
ICON_SIZE = 3 * 32
SHEET_SIZE = ICON_COUNT * ICON_SIZE


def remap_tiles(data: bytes, battler: int) -> bytes:
    if not 0 <= battler < BATTLER_COUNT:
        raise ValueError("battler must be 0, 1, 2 or 3")
    if len(data) != SHEET_SIZE:
        raise ValueError(f"expected {SHEET_SIZE} bytes for six 24x8 status icons; got {len(data)}")
    color = STATUS_COLOR_START + battler
    result = bytes((color if byte & 15 >= STATUS_COLOR_START else byte & 15)
                   | ((color if byte >> 4 >= STATUS_COLOR_START else byte >> 4) << 4)
                   for byte in data)
    for icon in range(ICON_COUNT):
        pixels = result[icon * ICON_SIZE:(icon + 1) * ICON_SIZE]
        if not any(byte & 15 == color or byte >> 4 == color for byte in pixels):
            raise ValueError(f"status icon {icon} has no status-color pixels (indices 12..15)")
    return result


def build(source: Path, output: Path, gfx: Path, battler: int):
    if output.suffix != ".4bpp" or source.resolve() == output.resolve():
        raise ValueError("output must be a separate generated .4bpp file")
    header = source.read_bytes()[:33]
    if (len(header) != 33 or header[:8] != b"\x89PNG\r\n\x1a\n"
            or header[12:16] != b"IHDR" or header[25] != 3
            or struct.unpack_from(">II", header, 16) != (24, 48)):
        raise ValueError(f"{source}: expected a 24x48 indexed PNG")
    if not 0 <= battler < BATTLER_COUNT:
        raise ValueError("battler must be 0, 1, 2 or 3")
    output.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="status-", dir=output.parent.resolve()) as tmp:
        converted = Path(tmp) / "status.4bpp"
        subprocess.run([str(gfx.resolve()), str(source.resolve()), str(converted)], check=True)
        # gbagfx keeps the low nibble of each indexed pixel. Japanese sheets
        # can use 12, 28, 44, ...; all alias to 12 and must be separated here.
        converted.write_bytes(remap_tiles(converted.read_bytes(), battler))
        converted.replace(output)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--gfx", type=Path, required=True, help="existing gbagfx executable")
    parser.add_argument("--battler", type=int, required=True, choices=range(BATTLER_COUNT))
    parser.add_argument("source", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    try:
        build(args.source, args.output, args.gfx, args.battler)
    except (OSError, ValueError, subprocess.CalledProcessError) as error:
        print(f"error: status icon conversion: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
