#!/usr/bin/env python3
"""Verify the Japanese FireRed / LeafGreen ROM a FireRed or LeafGreen build is made from.

The Emerald build needs the Japanese Emerald ROM at baserom.gba (tools/check_baserom_jp.py). A FireRed or
LeafGreen build needs one unmodified Japanese FireRed or LeafGreen ROM instead, at baserom_fr.gba or
baserom_lg.gba; the shared assets the Emerald build takes from baserom.gba are then taken from it
(tools/extract_frlg_assets.py), so the Emerald ROM is not needed.
"""
from __future__ import annotations

import argparse
import hashlib
from pathlib import Path
import sys
import zlib


EXPECTED_SIZE = 16 * 1024 * 1024
FILE_NAMES = {"firered": "baserom_fr.gba", "leafgreen": "baserom_lg.gba"}
# No-Intro dumps of the Japanese releases (both revisions of each)
KNOWN_ROMS = {
    "04139887b6cd8f53269aca098295b006ddba6cfe": ("BPRJ", 0, "3b2056e9", "Pocket Monsters - FireRed (Japan)"),
    "7c7107b87c3ccf6e3dbceb9cf80ceeffb25a1857": ("BPRJ", 1, "bb640df7", "Pocket Monsters - FireRed (Japan) (Rev 1)"),
    "5946f1b59e8d71cc61249661464d864185c92a5f": ("BPGJ", 0, "0a48556b", "Pocket Monsters - LeafGreen (Japan)"),
    "de9d5a844f9bfb63a4448cccd4a2d186ecf455c3": ("BPGJ", 1, None, "Pocket Monsters - LeafGreen (Japan) (Rev 1)"),
}
REQUIRED_MESSAGE = (
    "日本版ファイアレッドROMを baserom_fr.gba、または日本版リーフグリーンROMを baserom_lg.gba として"
    "プロジェクトルートに配置してください"
)


def find_baserom(root: Path, prefer: str) -> Path | None:
    """The ROM to use: the edition being built first, then the other one."""
    order = ["firered", "leafgreen"] if prefer != "leafgreen" else ["leafgreen", "firered"]
    for edition in order:
        path = root / FILE_NAMES[edition]
        if path.is_file():
            return path
    return None


def identify(data: bytes) -> tuple[str, int, str]:
    """Raises ValueError unless data is one of the known Japanese FireRed/LeafGreen ROMs."""
    if len(data) != EXPECTED_SIZE:
        raise ValueError(f"size is 0x{len(data):08X}, expected 0x{EXPECTED_SIZE:08X} (an unmodified 16 MiB dump)")
    game_code = data[0xAC:0xB0].decode("ascii", errors="replace")
    if game_code not in ("BPRJ", "BPGJ"):
        raise ValueError(f"game code is {game_code!r}, expected 'BPRJ' (FireRed) or 'BPGJ' (LeafGreen)")
    sha1 = hashlib.sha1(data).hexdigest()
    known = KNOWN_ROMS.get(sha1)
    if known is None:
        raise ValueError(f"sha1 {sha1} is not an unmodified Japanese FireRed/LeafGreen ROM")
    code, revision, crc32, name = known
    if code != game_code or data[0xBC] != revision:
        raise ValueError(f"header ({game_code} rev {data[0xBC]}) does not match {name}")
    if crc32 is not None and f"{zlib.crc32(data) & 0xFFFFFFFF:08x}" != crc32:
        raise ValueError(f"crc32 does not match {name}")
    return game_code, revision, name


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=Path("."), help="Project root holding baserom_fr.gba / baserom_lg.gba")
    parser.add_argument("--prefer", choices=sorted(FILE_NAMES), default="firered", help="Edition being built")
    parser.add_argument("--output", type=Path, help="Verification stamp to write")
    parser.add_argument("--print-path", action="store_true", help="Print the ROM that would be used and exit")
    args = parser.parse_args()

    path = find_baserom(args.root, args.prefer)
    if args.print_path:
        print(path.as_posix() if path else "")
        return 0
    if path is None:
        print(f"error: {REQUIRED_MESSAGE}", file=sys.stderr)
        print("detail: neither baserom_fr.gba nor baserom_lg.gba was found.", file=sys.stderr)
        return 1
    data = path.read_bytes()
    try:
        game_code, revision, name = identify(data)
    except ValueError as error:
        print(f"error: {REQUIRED_MESSAGE}", file=sys.stderr)
        print(f"detail: {path.name}: {error}.", file=sys.stderr)
        return 1
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        content = "\n".join([
            "verified=1", f"file={path.name}", f"rom={name}", f"game_code={game_code}", f"revision={revision}",
            f"sha1={hashlib.sha1(data).hexdigest()}", "",
        ])
        # Always rewritten: make compares its time with the ROM's and .map_version's
        args.output.write_text(content, encoding="utf-8")
    print(f"Verified {path.name}: {name}.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
