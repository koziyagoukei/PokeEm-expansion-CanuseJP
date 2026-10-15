"""Back up and grant every decoration in compatible save slots, preserving unrelated data."""

import argparse
from collections import Counter
from datetime import datetime
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[2]
FIELDS = (
    "magic", "version", "sb1_size", "sb2_size", "sb3_size", "storage_size",
    "sector_size", "data_size", "signature", "id_offset", "checksum_offset",
    "signature_offset", "counter_offset", "bases_offset", "base_size",
    "base_decor_offset", "base_decor_count", "player_room_offset", "player_room_count",
    "local_room_offset", "local_room_size", "local_decor_offset", "local_checksum_offset",
    "desk_offset", "desk_size", "chair_offset", "chair_size", "plant_offset", "plant_size",
    "ornament_offset", "ornament_size", "mat_offset", "mat_size", "poster_offset", "poster_size",
    "doll_offset", "doll_size", "cushion_offset", "cushion_size", "decor_count",
)
CATEGORIES = (
    ("desk", list(range(1, 10))), ("chair", list(range(10, 19))),
    ("plant", list(range(19, 25))), ("ornament", list(range(25, 48))),
    ("mat", list(range(48, 66))), ("poster", list(range(66, 76))),
    ("doll", list(range(76, 101)) + list(range(111, 121))), ("cushion", list(range(101, 111))),
)


def checksum(data):
    total = sum(struct.unpack("<" + "I" * (len(data) // 4), data[:len(data) // 4 * 4]))
    return ((total >> 16) + total) & 0xFFFF


def crc16(data):
    crc = 0x1121
    for value in data:
        crc ^= value
        for _ in range(8):
            crc = (crc >> 1) ^ (0x8408 if crc & 1 else 0)
    return (~crc) & 0xFFFF


def valid_slots(raw, layout):
    sizes = [layout["sb2_size"]]
    sizes += [max(0, min(layout["data_size"], layout["sb1_size"] - i * layout["data_size"])) for i in range(4)]
    sizes += [max(0, min(layout["data_size"], layout["storage_size"] - i * layout["data_size"])) for i in range(9)]
    slots = []
    for slot in range(2):
        sectors, counters = {}, set()
        for physical in range(slot * 14, (slot + 1) * 14):
            start = physical * layout["sector_size"]
            sector = raw[start:start + layout["sector_size"]]
            if len(sector) != layout["sector_size"]:
                continue
            id_ = struct.unpack_from("<H", sector, layout["id_offset"])[0]
            signature = struct.unpack_from("<I", sector, layout["signature_offset"])[0]
            if id_ >= 14 or signature != layout["signature"]:
                continue
            stored = struct.unpack_from("<H", sector, layout["checksum_offset"])[0]
            if stored != checksum(sector[:sizes[id_]]):
                continue
            if id_ in sectors:
                raise ValueError(f"Duplicate sector id {id_} in slot {slot}")
            sectors[id_] = (start, sizes[id_])
            counters.add(struct.unpack_from("<I", sector, layout["counter_offset"])[0])
        if len(sectors) == 14 and len(counters) == 1:
            slots.append((slot, counters.pop(), sectors))
    return slots


def placed_copies(sb1, raw, sectors, layout):
    placed = Counter()
    start = layout["bases_offset"] + layout["base_decor_offset"]
    placed.update(value for value in sb1[start:start + layout["base_decor_count"]] if value)
    start = layout["player_room_offset"]
    placed.update(value for value in sb1[start:start + layout["player_room_count"]] if value)
    sector0 = sectors[0][0]
    start = sector0 + layout["data_size"] + layout["local_room_offset"]
    local = raw[start:start + layout["local_room_size"]]
    if len(local) == 44 and struct.unpack_from("<I", local)[0] == 0x4D524C31 and local[4] == 1:
        if struct.unpack_from("<H", local, 40)[0] == crc16(local[:40]):
            placed.update(value for value in local[8:24] if value)
    return placed


def grant(raw, layout):
    slots = valid_slots(raw, layout)
    if not slots:
        raise ValueError("No complete save slot matches the current ROM's save layout; no data written")
    result = bytearray(raw)
    reports = []
    for slot, counter, sectors in slots:
        sb1 = bytearray().join(raw[sectors[id_][0]:sectors[id_][0] + sectors[id_][1]] for id_ in range(1, 5))
        before = bytes(sb1)
        placed = placed_copies(sb1, raw, sectors, layout)
        allowed = set()
        for category, decorations in CATEGORIES:
            start, count = layout[f"{category}_offset"], layout[f"{category}_size"]
            existing = list(sb1[start:start + count])
            if any(value and value not in decorations for value in existing):
                raise ValueError(f"Invalid {category} inventory in slot {slot}; no data written")
            missing = [decor for decor in decorations if decor not in existing]
            for decor in missing:
                if 0 not in existing:
                    # Only redundant, unplaced copies may be collapsed when a category is full.
                    duplicate = next((i for i, value in enumerate(existing)
                        if existing.count(value) > max(1, placed[value])), None)
                    if duplicate is None:
                        raise ValueError(f"No room for all {category} furniture without altering placed furniture in slot {slot}")
                    existing[duplicate] = 0
                existing[existing.index(0)] = decor
            assert set(decorations).issubset(existing)
            assert all(existing.count(decor) >= quantity for decor, quantity in placed.items() if decor in decorations)
            sb1[start:start + count] = bytes(existing)
            allowed.update(range(start, start + count))
        assert all(index in allowed for index, (old, new) in enumerate(zip(before, sb1)) if old != new)
        offset = 0
        for id_ in range(1, 5):
            start, size = sectors[id_]
            result[start:start + size] = sb1[offset:offset + size]
            struct.pack_into("<H", result, start + layout["checksum_offset"], checksum(sb1[offset:offset + size]))
            offset += size
        reports.append(f"slot {slot}, counter {counter}: all {layout['decor_count']} decorations present")
    assert [(slot, counter) for slot, counter, _ in valid_slots(result, layout)] == [(slot, counter) for slot, counter, _ in slots]
    return result, reports


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("save", type=Path)
    parser.add_argument("--layout", type=Path, default=ROOT / "build/local_room_save_layout.bin")
    parser.add_argument("--apply", action="store_true", help="Back up then update the save; otherwise validate without writing")
    args = parser.parse_args()
    binary = args.layout.read_bytes()
    if len(binary) != len(FIELDS) * 4:
        raise ValueError("Unexpected layout metadata size")
    layout = dict(zip(FIELDS, struct.unpack("<" + "I" * len(FIELDS), binary)))
    if layout["magic"] != 0x4C52534C or layout["version"] != 1 or layout["decor_count"] != 120:
        raise ValueError("Unsupported layout metadata")
    path = args.save.resolve()
    raw = path.read_bytes()
    updated, reports = grant(raw, layout)
    print("\n".join(reports))
    if args.apply:
        backup = path.with_name(f"{path.stem}.before-all-decorations-{datetime.now():%Y%m%d-%H%M%S}{path.suffix}")
        with backup.open("xb") as file:
            file.write(raw)
        if path.read_bytes() != raw:
            raise ValueError("Save changed during validation; backup retained, original not overwritten")
        path.write_bytes(updated)
        assert path.read_bytes() == updated
        print(f"Updated: {path}\nBackup: {backup}")
    else:
        print("Dry run: no save data written")


if __name__ == "__main__":
    main()
