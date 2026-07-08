#!/usr/bin/env python3
from __future__ import annotations

import argparse
import hashlib
import json
import sys
import zlib
from pathlib import Path


EXPECTED_MESSAGE = "日本版エメラルドROMをプロジェクトルートに baserom.gba として配置してください"


def parse_int(value: int | str) -> int:
    if isinstance(value, int):
        return value
    return int(value, 0)


def fail(message: str, missing: list[Path] | None = None) -> int:
    print(f"error: {message}", file=sys.stderr)
    if missing:
        print("missing extracted sound files:", file=sys.stderr)
        for path in missing:
            print(f"  {path}", file=sys.stderr)
    return 1


def sha1(data: bytes) -> str:
    return hashlib.sha1(data).hexdigest()


def load_manifest(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as f:
        return json.load(f)


def manifest_value(entry: dict, key: str, legacy_key: str | None = None):
    if key in entry:
        return entry[key]
    if legacy_key and legacy_key in entry:
        return entry[legacy_key]
    raise KeyError(key)


def validate_baserom(data: bytes, manifest: dict, baserom_path: Path) -> None:
    expected = manifest["baserom"]
    game_code = data[0xAC:0xB0].decode("ascii", errors="replace")
    actual_sha1 = sha1(data)
    actual_crc32 = f"{zlib.crc32(data) & 0xFFFFFFFF:08x}"
    if game_code != expected["gameCode"]:
        raise ValueError(f"{baserom_path} has game code {game_code!r}, expected {expected['gameCode']!r}.")
    if actual_sha1 != expected["sha1"] or actual_crc32 != expected["crc32"]:
        raise ValueError(
            f"{baserom_path} is not the expected BPEJ ROM "
            f"(sha1={actual_sha1}, crc32={actual_crc32})."
        )


def output_path(out_dir: Path, manifest_path: str) -> Path:
    path = Path(manifest_path)
    parts = path.parts
    if len(parts) >= 2 and parts[0] == "build" and parts[1] == "extracted_sound":
        path = Path(*parts[2:])
    return out_dir / path


def slice_rom(data: bytes, offset_value: int | str, size_value: int | str, label: str) -> bytes:
    offset = parse_int(offset_value)
    size = parse_int(size_value)
    end = offset + size
    if offset < 0 or end > len(data):
        raise ValueError(f"{label} range 0x{offset:X}+0x{size:X} is outside the ROM.")
    return data[offset:end]


def write_blob(rom: bytes, entry: dict, out_dir: Path) -> Path:
    rom_offset = manifest_value(entry, "rom_offset", "romOffset")
    data = slice_rom(rom, rom_offset, entry["size"], entry["label"])
    expected_sha1 = entry.get("sha1")
    if expected_sha1 and sha1(data) != expected_sha1:
        raise ValueError(f"{entry['label']} hash mismatch at {rom_offset}.")
    path = output_path(out_dir, manifest_value(entry, "output_path", "output"))
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)
    return path


def song_wrapper(song: dict) -> str:
    lines: list[str] = [
        '\t.include "MPlayDef.s"',
        "",
        "\t.section .rodata",
        f"\t.global\t{song['label']}",
        "\t.align\t2",
        "",
    ]

    for track in song["tracks"]:
        lines.extend(
            [
                f"{track['label']}:",
                f"\t.incbin \"{manifest_value(track, 'output_path', 'output')}\"",
                "",
            ]
        )

    lines.extend(
        [
            "\t.align\t2",
            f"{song['label']}:",
            f"\t.byte\t{len(song['tracks'])}\t@ NumTrks",
            "\t.byte\t0\t@ NumBlks",
            f"\t.byte\t{song['priority']}\t@ Priority",
            f"\t.byte\t{song['reverb']}\t@ Reverb.",
            "",
            f"\t.word\t{song['voicegroup']}",
            "",
        ]
    )
    for track in song["tracks"]:
        lines.append(f"\t.word\t{track['label']}")
    lines.extend(["", "\t.end", ""])
    return "\n".join(lines)


def write_song(rom: bytes, song: dict, out_dir: Path) -> list[Path]:
    written: list[Path] = []
    header = slice_rom(
        rom,
        manifest_value(song, "song_offset", "songOffset"),
        manifest_value(song, "header_size", "headerSize"),
        song["label"],
    )
    if header[0] != len(song["tracks"]):
        raise ValueError(f"{song['label']} track count mismatch in manifest.")
    if header[2] != song["priority"] or header[3] != song["reverb"]:
        raise ValueError(f"{song['label']} priority/reverb mismatch in manifest.")

    for track in song["tracks"]:
        written.append(write_blob(rom, track, out_dir))

    path = output_path(out_dir, manifest_value(song, "output_path", "output"))
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(song_wrapper(song), encoding="utf-8", newline="\n")
    written.append(path)
    return written


def expected_outputs(manifest: dict, out_dir: Path) -> list[Path]:
    paths: list[Path] = []
    for song in manifest.get("songs", []):
        paths.append(output_path(out_dir, manifest_value(song, "output_path", "output")))
        for track in song.get("tracks", []):
            paths.append(output_path(out_dir, manifest_value(track, "output_path", "output")))
    for blob in manifest.get("blobs", []):
        paths.append(output_path(out_dir, manifest_value(blob, "output_path", "output")))
    return paths


def write_stamp(path: Path, manifest_path: Path, outputs: list[Path]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    lines = [
        "verified=1",
        f"manifest={manifest_path.as_posix()}",
        f"file_count={len(outputs)}",
        "",
    ]
    path.write_text("\n".join(lines), encoding="utf-8")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Extract a small verified set of BPEJ sound data.")
    parser.add_argument("--baserom", help="Path to baserom.gba")
    parser.add_argument("--manifest", required=True, help="Path to tools/data/bpej_sound_manifest.json")
    parser.add_argument("--out", required=True, help="Output directory, usually build/extracted_sound")
    parser.add_argument("--stamp", help="Stamp file to write after successful extraction")
    parser.add_argument("--check", action="store_true", help="Only check that manifest outputs exist")
    parser.add_argument("--print-outputs", action="store_true", help="Print manifest output paths and exit")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    manifest_path = Path(args.manifest)
    out_dir = Path(args.out)
    manifest = load_manifest(manifest_path)
    outputs = expected_outputs(manifest, out_dir)

    if args.print_outputs:
        for path in outputs:
            print(path.as_posix())
        return 0

    if args.check:
        missing = [path for path in outputs if not path.is_file()]
        if missing:
            return fail("音データ抽出物が不足しています。", missing)
        return 0

    if not args.baserom:
        return fail(EXPECTED_MESSAGE)

    baserom_path = Path(args.baserom)
    if not baserom_path.is_file():
        return fail(EXPECTED_MESSAGE, outputs)

    try:
        rom = baserom_path.read_bytes()
        validate_baserom(rom, manifest, baserom_path)
        written: list[Path] = []
        for song in manifest.get("songs", []):
            written.extend(write_song(rom, song, out_dir))
        for blob in manifest.get("blobs", []):
            written.append(write_blob(rom, blob, out_dir))
        missing = [path for path in outputs if not path.is_file()]
        if missing:
            return fail("音データ抽出に失敗しました。", missing)
        if args.stamp:
            write_stamp(Path(args.stamp), manifest_path, outputs)
    except Exception as e:
        return fail(f"音データ抽出に失敗しました: {e}")

    print(f"Extracted {len(written)} BPEJ sound files to {out_dir}.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
