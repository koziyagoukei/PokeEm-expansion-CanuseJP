#!/usr/bin/env python3
"""Regenerate tools/data/frlg_asset_probes.json from the Japanese Emerald ROM (a maintainer tool).

A FireRed/LeafGreen build takes the assets the Emerald build extracts from baserom.gba out of the Japanese
FireRed/LeafGreen ROM instead (tools/extract_frlg_assets.py). It finds each one by content: the ROM bytes'
SHA1 is already in the BPEJ manifests, and this file adds PROBE_LENGTH bytes of each (its most varied window,
with their offset in the asset), so the FireRed/LeafGreen ROM can be searched quickly. Run it again whenever a BPEJ manifest gains or changes assets:

    python3 tools/generate_frlg_asset_probes.py --baserom baserom.gba \\
        --pokedex-manifest build/generated/bpej_pokedex_manifest.json \\
        --battle-background-manifest build/generated/bpej_battle_backgrounds_manifest.json

(the two generated manifests come from `make` on the Emerald build).
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import sys

from check_baserom_jp import EXPECTED_CRC32, EXPECTED_GAME_CODE, EXPECTED_SHA1


PROBE_LENGTH = 8
GRAPHICS_MANIFESTS = {
    "contest": "tools/data/bpej_contest_manifest.json",
    "intro": "tools/data/bpej_intro_manifest.json",
    "pokenav": "tools/data/bpej_pokenav_manifest.json",
    "berry": "tools/data/bpej_berry_manifest.json",
}
SOUND_MANIFEST = "tools/data/bpej_sound_manifest.json"


def sha1(data: bytes) -> str:
    return hashlib.sha1(data).hexdigest()


def number(value) -> int:
    return int(value, 0) if isinstance(value, str) else int(value)


def sources(args) -> list[tuple[int, int, str]]:
    """(offset, size, sha1) of every ROM range an extraction reads."""
    ranges = []
    for path in GRAPHICS_MANIFESTS.values():
        for asset in json.loads(Path(path).read_text(encoding="utf-8"))["assets"]:
            ranges.append((number(asset["rom_offset"]), number(asset["size"]), asset["expected_source_hash"]))
    sound = json.loads(Path(SOUND_MANIFEST).read_text(encoding="utf-8"))
    for song in sound["songs"]:
        for track in song["tracks"]:
            ranges.append((number(track["rom_offset"]), number(track["size"]), track["sha1"]))
    for blob in sound["blobs"]:
        ranges.append((number(blob["rom_offset"]), number(blob["size"]), blob["sha1"]))
    for asset in json.loads(Path(args.pokedex_manifest).read_text(encoding="utf-8"))["assets"]:
        ranges.append((number(asset["rom_offset"]), number(asset["size"]), asset["expected_hash"]))
    for asset in json.loads(Path(args.battle_background_manifest).read_text(encoding="utf-8"))["assets"]:
        ranges.append((number(asset["rom_offset"]), number(asset["size"]), asset["expected_source_hash"]))
    return ranges


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--baserom", type=Path, default=Path("baserom.gba"))
    parser.add_argument("--pokedex-manifest", type=Path, default=Path("build/generated/bpej_pokedex_manifest.json"))
    parser.add_argument("--battle-background-manifest", type=Path,
                        default=Path("build/generated/bpej_battle_backgrounds_manifest.json"))
    parser.add_argument("--output", type=Path, default=Path("tools/data/frlg_asset_probes.json"))
    args = parser.parse_args()

    rom = args.baserom.read_bytes()
    if rom[0xAC:0xB0] != EXPECTED_GAME_CODE or sha1(rom) != EXPECTED_SHA1:
        print("error: --baserom must be the verified Japanese Emerald ROM (BPEJ)", file=sys.stderr)
        return 1
    probes = {}
    for offset, size, digest in sources(args):
        data = rom[offset:offset + size]
        if sha1(data) != digest:
            print(f"error: BPEJ range 0x{offset:X}+{size} does not match its manifest hash", file=sys.stderr)
            return 1
        # The most varied window, so runs of zeros or repeated bytes do not match all over the ROM
        offset_in = max(range(max(1, size - PROBE_LENGTH + 1)),
                        key=lambda k: (len(set(data[k:k + PROBE_LENGTH])), -k))
        probes[digest] = {"size": size, "offset": offset_in, "bytes": data[offset_in:offset_in + PROBE_LENGTH].hex()}
    document = {
        "format_version": 1,
        "description": "Search keys for the BPEJ manifest assets in a Japanese FireRed/LeafGreen ROM "
                       "(tools/extract_frlg_assets.py); regenerate with tools/generate_frlg_asset_probes.py.",
        "source_rom": {"gameCode": EXPECTED_GAME_CODE.decode("ascii"), "sha1": EXPECTED_SHA1, "crc32": EXPECTED_CRC32},
        "probe_length": PROBE_LENGTH,
        "probes": dict(sorted(probes.items())),
    }
    args.output.write_text(json.dumps(document, indent=1) + "\n", encoding="utf-8", newline="\n")
    print(f"Wrote {len(probes)} probes to {args.output}.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
