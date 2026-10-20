#!/usr/bin/env python3
"""Restore only the three NPC speech fields from the verified Japanese Emerald ROM."""

import argparse
import hashlib
from pathlib import Path
import re
import struct

ROOT = Path(__file__).resolve().parents[1]
TRAINERS = ROOT / "src/data/battle_frontier/battle_frontier_trainers.h"
ROM_SHA1 = "d7cf8f156ba9c455d164e1ea780a6bf1945465c2"
TRAINER_TABLE = 0x5B4A10
TRAINER_SIZE = 52
TRAINER_COUNT = 300
SPEECH_FIELDS = ("speechBefore", "speechWin", "speechLose")


def load_charmap():
    result = {}
    for line in (ROOT / "charmap.txt").read_text(encoding="utf-8").splitlines():
        match = re.match(r"^'([^']+)'\s*=\s*([0-9A-F ]+)", line)
        if match and len(match[1]) == 1:
            result[match[1]] = bytes.fromhex(match[2].strip())
    return result


def encode(text, charmap):
    text = text.removeprefix("{JPN}")
    return b"".join(charmap[char] for char in text)


def rom_string(rom, offset):
    end = rom.index(b"\xff", offset, offset + 64)
    return rom[offset:end]


def species_names(charmap):
    result = {}
    for generation in (1, 2, 3):
        path = ROOT / f"src/data/pokemon/species_info/gen_{generation}_families.h"
        text = path.read_text(encoding="utf-8")
        entries = re.split(r"(?m)^    \[(SPECIES_\w+)\]\s*=", text)
        for symbol, body in zip(entries[1::2], entries[2::2]):
            name = re.search(r'\.speciesName\s*=\s*_\("([^"]+)"\)', body)
            if name:
                result.setdefault(encode(name[1], charmap), symbol.removeprefix("SPECIES_"))
    return result


def word_constants():
    text = (ROOT / "include/constants/easy_chat.h").read_text(encoding="utf-8")
    groups = {name: int(value) for name, value in re.findall(
        r"^#define (EC_GROUP_\w+)\s+(\d+)\s*$", text, re.M)}
    return {(groups[group] << 9) | int(index): name for name, group, index in re.findall(
        r"^#define (EC_WORD_\w+)\s+\(\((EC_GROUP_\w+) << EC_MASK_BITS\) \| (\d+)\)", text, re.M)}


def move_constants():
    text = (ROOT / "include/constants/moves.h").read_text(encoding="utf-8")
    return {int(value): name for name, value in re.findall(r"^    MOVE_(\w+) = (\d+),", text, re.M)}


def restore(rom, source):
    charmap = load_charmap()
    species = species_names(charmap)
    words = word_constants()
    moves = move_constants()

    def convert(value):
        if value == 0xFFFF:
            return "EC_EMPTY_WORD"
        group, index = value >> 9, value & 511
        if group in (0, 21):
            # Gen III's internal Hoenn species IDs differ from Expansion's IDs.
            name = rom_string(rom, 0x2EA31C + index * 6)
            macro = "EC_POKEMON" if group == 0 else "EC_POKEMON_NATIONAL"
            return f"{macro}({species[name]})"
        if group in (18, 19):
            return f"{'EC_MOVE' if group == 18 else 'EC_MOVE2'}({moves[index]})"
        return words[value]

    entries = re.split(r"(?m)^    \[(FRONTIER_TRAINER_\w+)\]\s*=", source)
    if (len(entries) - 1) // 2 != TRAINER_COUNT:
        raise ValueError("Unexpected Frontier trainer count")
    changed = 0
    original_names = [rom_string(rom, TRAINER_TABLE + i * TRAINER_SIZE + 4)
                      for i in range(TRAINER_COUNT)]
    restored_ids = set()
    for trainer in range(TRAINER_COUNT):
        body = entries[trainer * 2 + 2]
        name = re.search(r'\.trainerName\s*=\s*_\("([^"]+)"\)', body)
        if name is None or encode(name[1], charmap) not in original_names:
            raise ValueError(f"Trainer identity mismatch: {entries[trainer * 2 + 1]}")
        # Four localized names occupy swapped English-table IDs. Match identity
        # by the Japanese name, without changing names, teams, or trainer IDs.
        encoded_name = encode(name[1], charmap)
        original_id = trainer
        if original_names[trainer] != encoded_name:
            candidates = [i for i, original in enumerate(original_names) if original == encoded_name]
            if len(candidates) != 1:
                raise ValueError(f"Ambiguous trainer identity: {entries[trainer * 2 + 1]}")
            original_id = candidates[0]
        restored_ids.add(original_id)
        for which, field in enumerate(SPEECH_FIELDS):
            values = struct.unpack_from("<6H", rom, TRAINER_TABLE + original_id * TRAINER_SIZE + 12 + which * 12)
            pattern = rf"(?m)^(        \.{field} = )\{{[^}}]*\}}"
            replacement = "{" + ", ".join(convert(value) for value in values) + "}"
            original = re.search(pattern, body)
            if original is None:
                raise ValueError(f"Missing {field} in trainer {trainer}")
            changed += original[0] != original[1] + replacement
            body = re.sub(pattern, lambda match: match[1] + replacement, body, count=1)
        entries[trainer * 2 + 2] = body
    if len(restored_ids) != TRAINER_COUNT:
        raise ValueError("Restored trainer identities must be unique")
    # re.split captured trainer symbols; put their initializer prefixes back.
    output = entries[0]
    for symbol, body in zip(entries[1::2], entries[2::2]):
        output += f"    [{symbol}] =" + body
    speech_pattern = r"\.(?:speechBefore|speechWin|speechLose) = \{[^}]*\}"
    if re.sub(speech_pattern, "", output) != re.sub(speech_pattern, "", source):
        raise ValueError("Refusing to change non-speech trainer data")
    return output, changed


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--baserom", type=Path, default=ROOT / "baserom.gba")
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--write", action="store_true")
    mode.add_argument("--check", action="store_true")
    args = parser.parse_args()
    rom = args.baserom.read_bytes()
    if rom[0xAC:0xB0] != b"BPEJ" or hashlib.sha1(rom).hexdigest() != ROM_SHA1:
        raise ValueError("Expected an unmodified Japanese Emerald BPEJ ROM")
    source = TRAINERS.read_text(encoding="utf-8")
    output, changed = restore(rom, source)
    if args.write:
        TRAINERS.write_text(output, encoding="utf-8", newline="\n")
    print(f"Verified {TRAINER_COUNT} trainer identities, {TRAINER_COUNT * 3} speeches; {changed} fields {'updated' if args.write else 'differ'}.")
    return int(args.check and source != output)


if __name__ == "__main__":
    raise SystemExit(main())
