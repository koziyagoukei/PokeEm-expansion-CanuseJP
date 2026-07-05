#!/usr/bin/env python3
from __future__ import annotations

import argparse
import hashlib
import sys
import zlib
from pathlib import Path


EXPECTED_FILE_NAME = "baserom.gba"
EXPECTED_SIZE = 16 * 1024 * 1024
EXPECTED_GAME_CODE = b"BPEJ"
EXPECTED_SHA1 = "d7cf8f156ba9c455d164e1ea780a6bf1945465c2"
EXPECTED_CRC32 = "4881f3f8"
REQUIRED_MESSAGE = (
    "日本版エメラルドROMをプロジェクトルートに baserom.gba として配置してください"
)


def fail(reason: str) -> int:
    print(f"error: {REQUIRED_MESSAGE}", file=sys.stderr)
    print(f"detail: {reason}", file=sys.stderr)
    return 1


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Verify that baserom.gba is an unmodified Japanese Pokemon Emerald ROM."
    )
    parser.add_argument("--baserom", required=True, help="Path to baserom.gba")
    parser.add_argument("--output", required=True, help="Path to the verification token")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    baserom_path = Path(args.baserom)
    output_path = Path(args.output)

    if baserom_path.name != EXPECTED_FILE_NAME:
        return fail(f"ROM filename must be {EXPECTED_FILE_NAME}.")

    if not baserom_path.is_file():
        return fail(f"{EXPECTED_FILE_NAME} was not found.")

    data = baserom_path.read_bytes()
    file_size = len(data)
    if file_size != EXPECTED_SIZE:
        return fail(
            f"{EXPECTED_FILE_NAME} has size 0x{file_size:08X}, expected 0x{EXPECTED_SIZE:08X}."
        )

    game_code = data[0xAC:0xB0]
    if game_code != EXPECTED_GAME_CODE:
        decoded_game_code = game_code.decode("ascii", errors="replace")
        return fail(
            f"{EXPECTED_FILE_NAME} has game code {decoded_game_code!r}, expected 'BPEJ'."
        )

    sha1 = hashlib.sha1(data).hexdigest()
    crc32 = f"{zlib.crc32(data) & 0xFFFFFFFF:08x}"
    if sha1 != EXPECTED_SHA1 or crc32 != EXPECTED_CRC32:
        return fail(
            f"{EXPECTED_FILE_NAME} does not match the expected BPEJ hashes "
            f"(sha1={sha1}, crc32={crc32})."
        )

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(
        "\n".join(
            [
                "verified=1",
                f"file={baserom_path.name}",
                f"game_code={game_code.decode('ascii')}",
                f"size={file_size}",
                f"sha1={sha1}",
                f"crc32={crc32}",
                "",
            ]
        ),
        encoding="utf-8",
    )
    print(f"Verified {baserom_path.name} as Japanese Emerald (BPEJ).")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
