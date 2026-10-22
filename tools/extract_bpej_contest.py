#!/usr/bin/env python3
"""Extract verified Japanese contest and condition UI assets from a BPEJ ROM."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import sys
import tempfile

from check_baserom_jp import (
    EXPECTED_CRC32, EXPECTED_GAME_CODE, EXPECTED_SHA1, EXPECTED_SIZE, REQUIRED_MESSAGE,
)
from extract_bpej_battle_backgrounds import decompress_lz77
from extract_bpej_pokedex import gba_lz77_stream_length, validate_baserom


OUTPUT_ROOT = PurePosixPath("build/extracted_contest")
CUSTOM_ROOT = PurePosixPath("graphics/custom/bpej/contest")
SOURCE_ROOTS = ("graphics/contest/", "graphics/pokenav/condition/", "graphics/pokenav/left_headers/",
                "graphics/pokenav/options/", "graphics/pokeblock/use_screen/", "graphics/types/contest_")
CONSUMERS = {
    "src/graphics.c", "src/contest.c", "src/contest_util.c", "src/menu_specialized.c",
    "src/pokenav_conditions_gfx.c", "src/pokenav_conditions_search_results.c", "src/use_pokeblock.c",
    "graphics_file_rules.mk",
}
HASH_RE = re.compile(r"[0-9a-f]{40}\Z")


def sha1(data: bytes) -> str:
    return hashlib.sha1(data).hexdigest()


def integer(value: int | str) -> int:
    return int(value, 0) if isinstance(value, str) else int(value)


def relative_path(value: str, root: PurePosixPath) -> PurePosixPath:
    path = PurePosixPath(value)
    if path.is_absolute() or ".." in path.parts or "\\" in value or ":" in value:
        raise ValueError(f"unsafe path: {value}")
    relative = path.relative_to(root)
    if not relative.parts:
        raise ValueError(f"output must be a file below {root}: {value}")
    return relative


def load_manifest(path: Path, *, output_root=OUTPUT_ROOT, custom_root=CUSTOM_ROOT,
                  source_roots=SOURCE_ROOTS, consumers=CONSUMERS, category="contest") -> dict:
    manifest = json.loads(path.read_text(encoding="utf-8"))
    assets = manifest.get("assets", [])
    if manifest.get("format_version") != 1 or not assets or manifest.get("asset_count") != len(assets):
        raise ValueError(f"invalid {category} manifest version or asset count")
    expected_rom = {
        "file": "baserom.gba", "gameCode": EXPECTED_GAME_CODE.decode("ascii"),
        "size": EXPECTED_SIZE, "sha1": EXPECTED_SHA1, "crc32": EXPECTED_CRC32,
    }
    if manifest.get("baserom") != expected_rom:
        raise ValueError("manifest must identify the project's verified BPEJ ROM")

    seen = {field: set() for field in ("symbol_id", "label", "output_path", "custom_path")}
    ranges = []
    for asset in assets:
        label = asset["label"]
        verification = asset.get("verification", {})
        if asset.get("status") != "verified" or not verification.get("method") or not verification.get("detail"):
            raise ValueError(f"{label}: only documented, verified assets may be extracted")
        if (not HASH_RE.fullmatch(verification["comparison_sha1"])
                or integer(verification["comparison_size"]) <= 0):
            raise ValueError(f"{label}: existing-asset comparison evidence is missing")
        kind = verification.get("kind")
        if kind == "identical":
            if (verification.get("byte_match") is not True
                    or verification.get("comparison_sha1") != asset["expected_decoded_hash"]
                    or verification.get("comparison_size") != asset["decoded_size"]):
                raise ValueError(f"{label}: byte-match evidence disagrees with decoded output")
        elif kind == "japanese_ui":
            if (verification.get("byte_match") is not False or not verification.get("references")
                    or not verification.get("screen_set") or not verification.get("compatibility")):
                raise ValueError(f"{label}: Japanese replacement needs screen provenance and compatibility evidence")
        else:
            raise ValueError(f"{label}: unrecognized verification kind")
        if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", label) or asset["consumer_file"] not in consumers:
            raise ValueError(f"{label}: invalid or out-of-scope consumer")
        if asset["consumer_file"] == "graphics_file_rules.mk":
            if (asset.get("consumer_kind") != "make_input"
                    or asset.get("consumer_target") != "graphics/types/move_types.4bpp"
                    or not re.fullmatch(r"graphics/types/contest_(cool|beauty|cute|smart|tough)\.png", asset["source_file"])):
                raise ValueError(f"{label}: only the five contest badges may replace make inputs")
        if not asset["source_file"].startswith(source_roots) or asset["element_type"] not in ("u8", "u16", "u32"):
            raise ValueError(f"{label}: invalid source asset or element type")
        output = relative_path(asset["output_path"], output_root)
        if relative_path(asset["custom_path"], custom_root) != output:
            raise ValueError(f"{label}: custom path must mirror the output path")
        for field, values in seen.items():
            if asset[field] in values:
                raise ValueError(f"duplicate {field}: {asset[field]}")
            values.add(asset[field])
        for field in ("expected_source_hash", "expected_hash", "expected_decoded_hash"):
            if not HASH_RE.fullmatch(asset[field]):
                raise ValueError(f"{label}: invalid {field}")
        combination = (asset["compression"], asset["transform"], asset["output_representation"])
        if combination not in {
            ("none", "copy", "raw"), ("gba_lz77", "copy", "gba_lz77_stream"),
            ("gba_lz77", "gba_lz77_decompress", "raw"),
            ("gba_lz77", "gba_lz77_slice", "raw"),
        }:
            raise ValueError(f"{label}: unsupported compression/transform/representation {combination}")
        if asset["transform"] == "copy" and asset["expected_source_hash"] != asset["expected_hash"]:
            raise ValueError(f"{label}: copied output hash differs from the ROM source")
        if asset["output_representation"] == "raw" and asset["expected_hash"] != asset["expected_decoded_hash"]:
            raise ValueError(f"{label}: raw output hash differs from decoded data")
        offset, size, decoded_size = (integer(asset[field]) for field in ("rom_offset", "size", "decoded_size"))
        if offset < 0 or size <= 0 or offset + size > EXPECTED_SIZE or not 0 < decoded_size <= 0x10000:
            raise ValueError(f"{label}: invalid ROM range or decoded size")
        if asset["compression"] == "none" and size != decoded_size:
            raise ValueError(f"{label}: raw ROM size differs from output size")
        if asset["compression"] == "gba_lz77":
            payload = integer(asset["compressed_payload_size"])
            if not 4 < payload <= size or size - payload > 3:
                raise ValueError(f"{label}: invalid compressed payload/alignment size")
        if asset["transform"] == "gba_lz77_slice":
            source_size = integer(asset["source_decoded_size"])
            start = integer(asset["decoded_slice_offset"])
            if (not 0 < source_size <= 0x10000 or start < 0 or start + decoded_size > source_size
                    or not HASH_RE.fullmatch(asset["expected_source_decoded_hash"])):
                raise ValueError(f"{label}: invalid decoded slice range or source hash")
        output_size = decoded_size if asset["output_representation"] == "raw" else size
        if output_size % (int(asset["element_type"][1:]) // 8):
            raise ValueError(f"{label}: output size does not fit its C element type")
        for other_offset, other_size, other_hash in ranges:
            if offset < other_offset + other_size and other_offset < offset + size:
                if (offset, size, asset["expected_source_hash"]) != (other_offset, other_size, other_hash):
                    raise ValueError(f"{label}: overlapping, non-identical ROM ranges")
        ranges.append((offset, size, asset["expected_source_hash"]))
    return manifest


def decode_lz77(data: bytes, asset: dict) -> bytes:
    label = asset["label"]
    if len(data) < 4 or data[0] != 0x10 or int.from_bytes(data[1:4], "little") != asset["decoded_size"]:
        raise ValueError(f"{label}: wrong LZ77 header or decoded size")
    gba_lz77_stream_length(data, label)
    decoded, _ = decompress_lz77(data, label)
    return decoded


def validate_output(data: bytes, asset: dict) -> None:
    if len(data) % (int(asset["element_type"][1:]) // 8):
        raise ValueError(f"{asset['label']}: output is not aligned to its C element type")
    if asset["output_representation"] == "gba_lz77_stream":
        decode_lz77(data, asset)
    elif len(data) != asset["decoded_size"]:
        raise ValueError(f"{asset['label']}: expected {asset['decoded_size']} raw bytes, got {len(data)}")


def rom_output(rom: bytes, asset: dict) -> bytes:
    offset, size = integer(asset["rom_offset"]), integer(asset["size"])
    if offset < 0 or size <= 0 or offset + size > len(rom):
        raise ValueError(f"{asset['label']}: ROM range is out of bounds")
    source = rom[offset:offset + size]
    if sha1(source) != asset["expected_source_hash"]:
        raise ValueError(f"{asset['label']}: ROM range hash mismatch at 0x{offset:X}")
    source_asset = dict(asset, decoded_size=asset.get("source_decoded_size", asset["decoded_size"]))
    decoded = decode_lz77(source, source_asset) if asset["compression"] == "gba_lz77" else source
    if (asset["compression"] == "gba_lz77"
            and gba_lz77_stream_length(source, asset["label"]) != asset["compressed_payload_size"]):
        raise ValueError(f"{asset['label']}: compressed payload size differs from manifest")
    if asset["transform"] == "gba_lz77_slice":
        if sha1(decoded) != asset["expected_source_decoded_hash"]:
            raise ValueError(f"{asset['label']}: full decoded source hash mismatch")
        start = integer(asset["decoded_slice_offset"])
        decoded = decoded[start:start + asset["decoded_size"]]
    if len(decoded) != asset["decoded_size"] or sha1(decoded) != asset["expected_decoded_hash"]:
        raise ValueError(f"{asset['label']}: decoded data does not match the verified ROM asset")
    output = decoded if asset["transform"] in ("gba_lz77_decompress", "gba_lz77_slice") else source
    if sha1(output) != asset["expected_hash"]:
        raise ValueError(f"{asset['label']}: output hash mismatch")
    validate_output(output, asset)
    return output


def desired_output(rom: bytes, asset: dict) -> bytes:
    # Verify the underlying ROM range even when a custom override is present.
    original = rom_output(rom, asset)
    custom = Path(asset["custom_path"])
    if custom.is_file():
        data = custom.read_bytes()
        validate_output(data, asset)
        return data
    return original


def output_state(assets: list[dict], outputs: list[Path]) -> tuple[list[str], str, int]:
    failures, state = [], []
    custom_count = 0
    for asset, path in zip(assets, outputs):
        custom = Path(asset["custom_path"])
        origin = "custom" if custom.is_file() else "baserom"
        expected_hash = asset["expected_hash"]
        try:
            if origin == "custom":
                custom_count += 1
                data = custom.read_bytes()
                validate_output(data, asset)
                expected_hash = sha1(data)
            if not path.is_file():
                raise ValueError("missing extracted file")
            data = path.read_bytes()
            validate_output(data, asset)
            if sha1(data) != expected_hash:
                raise ValueError(f"hash mismatch for {origin} source")
            state.append(f"{asset['output_path']}={origin}:{expected_hash}")
        except (OSError, ValueError) as error:
            failures.append(f"{path} ({asset['label']}): {error}")
    return failures, sha1("\n".join(state).encode("utf-8")), custom_count


def write_if_changed(path: Path, data: bytes) -> None:
    if path.is_file() and path.read_bytes() == data:
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = None
    try:
        with tempfile.NamedTemporaryFile(prefix=path.name + ".", suffix=".tmp", dir=path.parent, delete=False) as target:
            temporary = Path(target.name)
            target.write(data)
        temporary.replace(path)
    finally:
        if temporary is not None:
            temporary.unlink(missing_ok=True)


def stamp_content(manifest_path: Path, state_hash: str, count: int, custom_count: int) -> dict:
    return {"verified": 1, "baserom_sha1": EXPECTED_SHA1, "manifest_sha1": sha1(manifest_path.read_bytes()),
            "file_count": count, "custom_count": custom_count, "output_state_sha1": state_hash}


def run_extraction(argv, *, loader, output_root, default_manifest, category, description) -> int:
    """Share the validated binary pipeline without broadening either manifest's scope."""
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument("--baserom", type=Path, default=Path("baserom.gba"))
    parser.add_argument("--manifest", type=Path, default=Path(default_manifest))
    parser.add_argument("--out", type=Path, default=Path(str(output_root)))
    parser.add_argument("--stamp", type=Path)
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--check", action="store_true")
    mode.add_argument("--print-outputs", action="store_true")
    mode.add_argument("--dry-run", action="store_true")
    args = parser.parse_args(argv)
    try:
        manifest = loader(args.manifest)
        assets = manifest["assets"]
        outputs = [args.out.joinpath(*relative_path(asset["output_path"], output_root).parts) for asset in assets]
        if args.print_outputs:
            print("\n".join(path.as_posix() for path in outputs))
            return 0
        if not args.check:
            rom = args.baserom.read_bytes()
            validate_baserom(rom, manifest, args.baserom)
            resolved = [desired_output(rom, asset) for asset in assets]
            if args.dry_run:
                for asset, path in zip(assets, outputs):
                    print(f"{asset['label']}: {asset['rom_offset']} + {asset['size']} -> {path}")
                return 0
            for path, data in zip(outputs, resolved):
                write_if_changed(path, data)
        failures, state_hash, custom_count = output_state(assets, outputs)
        expected_stamp = stamp_content(args.manifest, state_hash, len(outputs), custom_count)
        if args.check and args.stamp:
            if not args.stamp.is_file() or json.loads(args.stamp.read_text(encoding="utf-8")) != expected_stamp:
                failures.append(f"{args.stamp}: missing or stale extraction stamp")
        if failures:
            raise ValueError("output validation failed:\n  " + "\n  ".join(failures))
        if not args.check and args.stamp:
            write_if_changed(args.stamp, (json.dumps(expected_stamp, indent=2) + "\n").encode("utf-8"))
        print(f"{'Validated' if args.check else 'Extracted'} {len(outputs)} {category} assets ({custom_count} custom overrides).")
    except (OSError, ValueError, KeyError, TypeError) as error:
        print(f"error: {REQUIRED_MESSAGE}\ndetail: {category} extraction: {error}", file=sys.stderr)
        return 1
    return 0


def main(argv: list[str] | None = None) -> int:
    return run_extraction(argv, loader=load_manifest, output_root=OUTPUT_ROOT,
                          default_manifest="tools/data/bpej_contest_manifest.json",
                          category="contest/condition", description=__doc__)


if __name__ == "__main__":
    raise SystemExit(main())
