from __future__ import annotations

import shutil
import struct
import zlib
from pathlib import Path
from typing import Any


PNG_SIGNATURE = b"\x89PNG\r\n\x1a\n"
VALID_JASC_SIZES = {16, 224, 256}


class PaletteError(ValueError):
    pass


def _paeth(left: int, above: int, upper_left: int) -> int:
    predictor = left + above - upper_left
    pa = abs(predictor - left)
    pb = abs(predictor - above)
    pc = abs(predictor - upper_left)
    if pa <= pb and pa <= pc:
        return left
    if pb <= pc:
        return above
    return upper_left


def _png_bpp(color_type: int, bit_depth: int) -> int:
    samples = {
        0: 1,  # grayscale
        2: 3,  # RGB
        3: 1,  # indexed
        4: 2,  # grayscale + alpha
        6: 4,  # RGBA
    }.get(color_type)
    if samples is None:
        raise PaletteError(f"Unsupported PNG color type: {color_type}")
    return samples * bit_depth


def _parse_png(path: Path) -> dict[str, Any]:
    data = path.read_bytes()
    if not data.startswith(PNG_SIGNATURE):
        raise PaletteError("Input file is not a PNG.")
    pos = len(PNG_SIGNATURE)
    info: dict[str, Any] = {"plte": [], "idat": bytearray()}
    while pos + 8 <= len(data):
        length = struct.unpack(">I", data[pos:pos + 4])[0]
        chunk_type = data[pos + 4:pos + 8]
        chunk_start = pos + 8
        chunk_end = chunk_start + length
        if chunk_end + 4 > len(data):
            raise PaletteError("PNG chunk is truncated.")
        chunk = data[chunk_start:chunk_end]
        pos = chunk_end + 4  # skip CRC
        if chunk_type == b"IHDR":
            if length != 13:
                raise PaletteError("Invalid PNG IHDR chunk.")
            width, height, bit_depth, color_type, compression, filter_method, interlace = struct.unpack(">IIBBBBB", chunk)
            if compression != 0 or filter_method != 0:
                raise PaletteError("Unsupported PNG compression/filter method.")
            if interlace != 0:
                raise PaletteError("Interlaced PNG is not supported.")
            info.update({"width": width, "height": height, "bit_depth": bit_depth, "color_type": color_type})
        elif chunk_type == b"PLTE":
            if length % 3:
                raise PaletteError("Invalid PNG PLTE chunk.")
            info["plte"] = [tuple(chunk[index:index + 3]) for index in range(0, length, 3)]
        elif chunk_type == b"IDAT":
            info["idat"].extend(chunk)
        elif chunk_type == b"IEND":
            break
    if "width" not in info or not info["idat"]:
        raise PaletteError("PNG is missing IHDR or IDAT data.")
    return info


def _unfilter_rows(raw: bytes, width: int, height: int, bpp_bits: int) -> list[bytes]:
    stride = (width * bpp_bits + 7) // 8
    filter_bpp = max(1, (bpp_bits + 7) // 8)
    rows: list[bytes] = []
    pos = 0
    previous = bytes(stride)
    for _y in range(height):
        if pos + 1 + stride > len(raw):
            raise PaletteError("PNG image data is truncated.")
        filter_type = raw[pos]
        row = bytearray(raw[pos + 1:pos + 1 + stride])
        pos += 1 + stride
        for index, value in enumerate(row):
            left = row[index - filter_bpp] if index >= filter_bpp else 0
            above = previous[index] if index < len(previous) else 0
            upper_left = previous[index - filter_bpp] if index >= filter_bpp else 0
            if filter_type == 0:
                pass
            elif filter_type == 1:
                row[index] = (value + left) & 0xFF
            elif filter_type == 2:
                row[index] = (value + above) & 0xFF
            elif filter_type == 3:
                row[index] = (value + ((left + above) // 2)) & 0xFF
            elif filter_type == 4:
                row[index] = (value + _paeth(left, above, upper_left)) & 0xFF
            else:
                raise PaletteError(f"Unsupported PNG filter type: {filter_type}")
        previous = bytes(row)
        rows.append(previous)
    return rows


def _scale_sample(value: int, bit_depth: int) -> int:
    if bit_depth == 8:
        return value
    if bit_depth == 16:
        return value >> 8
    maximum = (1 << bit_depth) - 1
    return round(value * 255 / maximum)


def _unpack_low_bit_values(row: bytes, width: int, bit_depth: int) -> list[int]:
    values: list[int] = []
    mask = (1 << bit_depth) - 1
    per_byte = 8 // bit_depth
    for byte in row:
        for shift_index in range(per_byte):
            shift = 8 - bit_depth - (shift_index * bit_depth)
            values.append((byte >> shift) & mask)
            if len(values) >= width:
                return values
    return values


def _append_color(colors: list[tuple[int, int, int]], seen: set[tuple[int, int, int]], color: tuple[int, int, int]) -> None:
    if color not in seen:
        seen.add(color)
        colors.append(color)


def extract_png_colors(path: Path) -> tuple[list[tuple[int, int, int]], dict[str, Any]]:
    info = _parse_png(path)
    width = int(info["width"])
    height = int(info["height"])
    bit_depth = int(info["bit_depth"])
    color_type = int(info["color_type"])
    bpp_bits = _png_bpp(color_type, bit_depth)
    raw = zlib.decompress(bytes(info["idat"]))
    rows = _unfilter_rows(raw, width, height, bpp_bits)
    colors: list[tuple[int, int, int]] = []
    seen: set[tuple[int, int, int]] = set()

    if color_type == 3:
        palette = info["plte"]
        if not palette:
            raise PaletteError("Indexed PNG is missing a PLTE chunk.")
        used_indices: set[int] = set()
        for row in rows:
            indices = list(row[:width]) if bit_depth == 8 else _unpack_low_bit_values(row, width, bit_depth)
            used_indices.update(indices)
        for index in sorted(used_indices):
            if index >= len(palette):
                raise PaletteError(f"Indexed PNG references missing palette index: {index}")
            color = tuple(int(component) for component in palette[index])
            _append_color(colors, seen, color)  # type: ignore[arg-type]
    elif color_type == 0:
        for row in rows:
            if bit_depth < 8:
                values = _unpack_low_bit_values(row, width, bit_depth)
                for value in values:
                    gray = _scale_sample(value, bit_depth)
                    _append_color(colors, seen, (gray, gray, gray))
            elif bit_depth == 8:
                for value in row[:width]:
                    _append_color(colors, seen, (value, value, value))
            elif bit_depth == 16:
                for x in range(width):
                    gray = row[x * 2]
                    _append_color(colors, seen, (gray, gray, gray))
            else:
                raise PaletteError(f"Unsupported grayscale bit depth: {bit_depth}")
    elif color_type == 2:
        if bit_depth not in {8, 16}:
            raise PaletteError(f"Unsupported truecolor bit depth: {bit_depth}")
        sample_bytes = bit_depth // 8
        pixel_bytes = 3 * sample_bytes
        for row in rows:
            for x in range(width):
                base = x * pixel_bytes
                _append_color(colors, seen, (row[base], row[base + sample_bytes], row[base + (2 * sample_bytes)]))
    elif color_type == 4:
        if bit_depth not in {8, 16}:
            raise PaletteError(f"Unsupported grayscale-alpha bit depth: {bit_depth}")
        sample_bytes = bit_depth // 8
        pixel_bytes = 2 * sample_bytes
        for row in rows:
            for x in range(width):
                gray = row[x * pixel_bytes]
                _append_color(colors, seen, (gray, gray, gray))
    elif color_type == 6:
        if bit_depth not in {8, 16}:
            raise PaletteError(f"Unsupported truecolor-alpha bit depth: {bit_depth}")
        sample_bytes = bit_depth // 8
        pixel_bytes = 4 * sample_bytes
        for row in rows:
            for x in range(width):
                base = x * pixel_bytes
                _append_color(colors, seen, (row[base], row[base + sample_bytes], row[base + (2 * sample_bytes)]))
    else:
        raise PaletteError(f"Unsupported PNG color type: {color_type}")

    metadata = {
        "width": width,
        "height": height,
        "bit_depth": bit_depth,
        "color_type": color_type,
        "used_colors": len(colors),
    }
    return colors, metadata


def jasc_pal_text(colors: list[tuple[int, int, int]], palette_size: int) -> str:
    if palette_size not in VALID_JASC_SIZES:
        raise PaletteError("Palette size must be 16, 224, or 256.")
    if len(colors) > palette_size:
        raise PaletteError(f"PNG uses {len(colors)} colors, which exceeds the selected palette size {palette_size}.")
    padded = [*colors, *([(0, 0, 0)] * (palette_size - len(colors)))]
    lines = ["JASC-PAL", "0100", str(palette_size)]
    lines.extend(f"{red} {green} {blue}" for red, green, blue in padded)
    return "\n".join(lines) + "\n"


def write_jasc_pal_from_png(png_path: Path, output_path: Path, palette_size: int) -> dict[str, Any]:
    if png_path.suffix.lower() != ".png":
        raise PaletteError("Input file must be a .png file.")
    if output_path.suffix.lower() != ".pal":
        raise PaletteError("Output file must use the .pal extension.")
    colors, metadata = extract_png_colors(png_path)
    text = jasc_pal_text(colors, palette_size)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    backup_path: Path | None = None
    if output_path.exists():
        backup_path = output_path.with_name(output_path.name + ".bak")
        shutil.copy2(output_path, backup_path)
    output_path.write_text(text, encoding="ascii", newline="\n")
    return {
        "ok": True,
        "png": str(png_path),
        "output": str(output_path),
        "palette_size": palette_size,
        "used_colors": len(colors),
        "padded_colors": palette_size - len(colors),
        "backup": str(backup_path) if backup_path else None,
        "image": metadata,
    }
