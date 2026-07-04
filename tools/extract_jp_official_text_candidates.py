#!/usr/bin/env python3
from __future__ import annotations

import csv
import json
import re
import subprocess
import unicodedata
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path


WORKSPACE = Path(__file__).resolve().parents[1]
JP_REPO = Path(r"C:\Users\Ehero\Desktop\decomps\pokeemerald-jp")
JP_ROM_CANDIDATES = [
    JP_REPO / "baserom_jp.gba",
    JP_REPO / "pokeemerald-jp" / "baserom_jp.gba",
]
JP_DATA_FILES = [
    JP_REPO / "data" / "data.s",
    JP_REPO / "data" / "event_scripts.s",
]
TARGET_ASM_FILES = [
    JP_REPO / "asm" / "tv.s",
    JP_REPO / "asm" / "battle_tv.s",
    JP_REPO / "asm" / "match_call.s",
    JP_REPO / "asm" / "pokenav_match_call_data.s",
    JP_REPO / "asm" / "pokenav_match_call_ui.s",
]
EXPANSION_TARGETS = {
    "tv": Path("data/text/tv.inc"),
    "match_call": Path("data/text/match_call.inc"),
}
OUT_DIR = WORKSPACE / "workspace" / "jp_text_candidates"
JP_DECODE_CHARMAP = WORKSPACE / "charmap.txt"


ASM_ADDR_RE = re.compile(r"^(?P<label>[A-Za-z0-9_]+): @ 0x(?P<addr>[0-9A-Fa-f]{8})$")
ASM_PTR_RE = re.compile(r"^(?:(?P<label>_?[A-Za-z0-9_]+):\s+)?\.4byte 0x(?P<addr>[0-9A-Fa-f]{8})$")
DATA_LABEL_RE = re.compile(r"^(?P<label>gUnknown_[0-9A-Fa-f]+): @ 0x(?P<addr>[0-9A-Fa-f]{7,8})$")
DATA_INCBIN_RE = re.compile(r'^\s*\.incbin "baserom_jp\.gba", 0x(?P<offset>[0-9A-Fa-f]+), 0x(?P<size>[0-9A-Fa-f]+)$')
CHARMAP_RE = re.compile(r"^([^@\r\n][^=\r\n]*?)\s*=\s*([0-9A-Fa-f ]+)\s*$")
LABEL_RE = re.compile(r"^([A-Za-z0-9_]+)::\s*$")
STRING_RE = re.compile(r'"((?:[^"\\]|\\.)*)"')
TRAILING_DIGITS_RE = re.compile(r"^(.*?)(\d+)$")
TRAILING_HEX_RE = re.compile(r"^(gUnknown_)([0-9A-Fa-f]+)$")


STOPWORDS = {
    "do",
    "get",
    "put",
    "show",
    "the",
    "and",
    "text",
    "on",
    "air",
    "before",
    "after",
    "interview",
    "special",
    "sub",
    "battle",
    "call",
    "match",
    "tv",
    "pokemon",
}


@dataclass
class Blob:
    file_name: str
    label: str
    address: int
    rom_offset: int
    size: int
    line_number: int

    @property
    def end_address(self) -> int:
        return self.address + self.size


@dataclass
class Reference:
    asm_file: str
    function_name: str
    function_line: int
    asm_line: int
    source_label: str
    target_address: int


@dataclass
class ExtractedText:
    address: int
    blob: Blob
    offset_in_blob: int
    decoded_text: str
    byte_length: int
    unknown_count: int
    control_count: int
    has_japanese: bool


@dataclass
class ResolvedReference:
    source: Reference
    resolved_address: int
    table_address: int | None
    table_index: int | None
    extracted: ExtractedText


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def score_token(token: str) -> tuple[int, int, int]:
    if token == "":
        return (0, 0, 0)
    if token.startswith("{") and token.endswith("}"):
        name = token[1:-1]
        if name in {"PLAYER", "STR_VAR_1", "STR_VAR_2", "STR_VAR_3", "STR_VAR_4", "KUN"}:
            return (5, 0, -len(token))
        if name in {"PAUSE", "CLEAR_TO", "COLOR", "HIGHLIGHT", "SHADOW", "JPN", "ENG"}:
            return (4, 0, -len(token))
        return (1, 0, -len(token))
    cat_score = 0
    for ch in token:
        cat = unicodedata.category(ch)
        if ch in "\n\r\t":
            cat_score += 1
        elif "HIRAGANA" in unicodedata.name(ch, "") or "KATAKANA" in unicodedata.name(ch, ""):
            cat_score += 7
        elif cat.startswith("L") or cat.startswith("N"):
            cat_score += 6
        elif cat.startswith("P") or cat.startswith("S"):
            cat_score += 5
        elif cat == "Zs":
            cat_score += 4
        else:
            cat_score += 2
    return (10, cat_score, -len(token))


def parse_charmap(path: Path) -> list[tuple[bytes, str]]:
    sequence_to_token: dict[bytes, str] = {}
    for raw_line in read_text(path).splitlines():
        line = raw_line.split("@", 1)[0].strip()
        if not line:
            continue
        match = CHARMAP_RE.match(line)
        if not match:
            continue
        lhs = match.group(1).strip()
        rhs = bytes(int(part, 16) for part in match.group(2).split())
        if lhs.startswith("'") and lhs.endswith("'"):
            token = lhs[1:-1]
        elif lhs == r"\n":
            token = "\n"
        elif lhs == r"\l":
            token = "{L}"
        elif lhs == r"\p":
            token = "{P}"
        else:
            token = "{" + lhs + "}"

        existing = sequence_to_token.get(rhs)
        if existing is None or score_token(token) > score_token(existing):
            sequence_to_token[rhs] = token

    entries = sorted(sequence_to_token.items(), key=lambda item: (-len(item[0]), item[0]))
    return entries


def parse_data_blobs() -> list[Blob]:
    blobs: list[Blob] = []
    for data_file in JP_DATA_FILES:
        lines = read_text(data_file).splitlines()
        i = 0
        while i < len(lines):
            label_match = DATA_LABEL_RE.match(lines[i].strip())
            if label_match and i + 1 < len(lines):
                incbin_match = DATA_INCBIN_RE.match(lines[i + 1])
                if incbin_match:
                    blobs.append(
                        Blob(
                            file_name=str(data_file.relative_to(JP_REPO)).replace("\\", "/"),
                            label=label_match.group("label"),
                            address=int(label_match.group("addr"), 16),
                            rom_offset=int(incbin_match.group("offset"), 16),
                            size=int(incbin_match.group("size"), 16),
                            line_number=i + 1,
                        )
                    )
                    i += 2
                    continue
            i += 1
    blobs.sort(key=lambda blob: blob.address)
    return blobs


def parse_references() -> list[Reference]:
    refs: list[Reference] = []
    for asm_file in TARGET_ASM_FILES:
        current_function = ""
        current_function_line = 0
        for idx, line in enumerate(read_text(asm_file).splitlines(), start=1):
            function_match = ASM_ADDR_RE.match(line.strip())
            if function_match:
                current_function = function_match.group("label")
                current_function_line = idx
                continue

            ptr_match = ASM_PTR_RE.match(line.strip())
            if not ptr_match:
                continue

            target_address = int(ptr_match.group("addr"), 16)
            if target_address < 0x08000000:
                continue
            refs.append(
                Reference(
                    asm_file=str(asm_file.relative_to(JP_REPO)).replace("\\", "/"),
                    function_name=current_function,
                    function_line=current_function_line,
                    asm_line=idx,
                    source_label=ptr_match.group("label") or "",
                    target_address=target_address,
                )
            )
    return refs


def find_blob(blobs: list[Blob], address: int) -> Blob | None:
    for blob in blobs:
        if blob.address <= address < blob.end_address:
            return blob
    return None


def decode_from_address(rom: bytes, blob: Blob, address: int, charmap_entries: list[tuple[bytes, str]]) -> ExtractedText | None:
    offset_in_blob = address - blob.address
    start = blob.rom_offset + offset_in_blob
    end = blob.rom_offset + blob.size
    if start >= len(rom):
        return None

    out: list[str] = []
    unknown_count = 0
    control_count = 0
    i = start

    while i < len(rom) and i < end:
        if rom[i] == 0xFF:
            break

        matched = False
        remaining = rom[i:end]
        for byte_seq, token in charmap_entries:
            if remaining.startswith(byte_seq):
                out.append(token)
                i += len(byte_seq)
                matched = True
                if token.startswith("{") and token.endswith("}"):
                    control_count += 1
                break

        if not matched:
            out.append(f"<{rom[i]:02X}>")
            unknown_count += 1
            i += 1

    if i == start:
        return None

    decoded = "".join(out)
    visible = decoded.replace("{L}", "\n").replace("{P}", "\n\n")
    has_japanese = any(
        "HIRAGANA" in unicodedata.name(ch, "") or "KATAKANA" in unicodedata.name(ch, "")
        for ch in visible
        if ch and ch[0] != "<"
    )
    return ExtractedText(
        address=address,
        blob=blob,
        offset_in_blob=offset_in_blob,
        decoded_text=visible,
        byte_length=i - start,
        unknown_count=unknown_count,
        control_count=control_count,
        has_japanese=has_japanese,
    )


def looks_like_text(entry: ExtractedText) -> bool:
    if entry.byte_length < 2:
        return False
    visible_chars = re.sub(r"\{[^}]+\}", "", entry.decoded_text)
    visible_chars = re.sub(r"<[0-9A-F]{2}>", "", visible_chars)
    visible_chars = visible_chars.replace("\n", "")
    if len(visible_chars) < 2:
        return False
    if entry.unknown_count > max(2, entry.byte_length // 6):
        return False
    if entry.has_japanese:
        return True
    return any(ch.isalpha() for ch in visible_chars)


def count_kana(text: str) -> int:
    total = 0
    for ch in text:
        name = unicodedata.name(ch, "")
        if "HIRAGANA" in name or "KATAKANA" in name:
            total += 1
    return total


def count_named_tokens(text: str) -> int:
    return len(re.findall(r"\{[^}]+\}", text))


def readability_score(text: str, unknown_count: int) -> int:
    return count_kana(text) * 3 - count_named_tokens(text) * 2 - unknown_count * 4


def read_u32_le(data: bytes, offset: int) -> int:
    return (
        data[offset]
        | (data[offset + 1] << 8)
        | (data[offset + 2] << 16)
        | (data[offset + 3] << 24)
    )


def iter_pointer_targets(rom: bytes, blob: Blob, start_address: int, blobs: list[Blob]) -> list[tuple[int, int]]:
    offset_in_blob = start_address - blob.address
    start = blob.rom_offset + offset_in_blob
    end = blob.rom_offset + blob.size
    max_scan = min(end, start + 0x200)
    targets: list[tuple[int, int]] = []

    i = start
    while i + 4 <= max_scan:
        ptr = read_u32_le(rom, i)
        if ptr < 0x08000000 or ptr > 0x09FFFFFF:
            break
        if find_blob(blobs, ptr) is None:
            break
        targets.append((ptr, (i - start) // 4))
        i += 4

    return targets if len(targets) >= 2 else []


def resolve_reference_texts(
    ref: Reference,
    rom: bytes,
    blobs: list[Blob],
    charmap_entries: list[tuple[bytes, str]],
) -> list[ResolvedReference]:
    blob = find_blob(blobs, ref.target_address)
    if blob is None:
        return []

    direct = decode_from_address(rom, blob, ref.target_address, charmap_entries)
    if direct is not None and looks_like_text(direct):
        return [
            ResolvedReference(
                source=ref,
                resolved_address=ref.target_address,
                table_address=None,
                table_index=None,
                extracted=direct,
            )
        ]

    results: list[ResolvedReference] = []
    for child_address, table_index in iter_pointer_targets(rom, blob, ref.target_address, blobs):
        child_blob = find_blob(blobs, child_address)
        if child_blob is None:
            continue
        child = decode_from_address(rom, child_blob, child_address, charmap_entries)
        if child is None or not looks_like_text(child):
            continue
        results.append(
            ResolvedReference(
                source=ref,
                resolved_address=child_address,
                table_address=ref.target_address,
                table_index=table_index,
                extracted=child,
            )
        )
    return results


def split_camel(name: str) -> list[str]:
    parts = re.sub(r"([a-z0-9])([A-Z])", r"\1 \2", name).replace("_", " ").split()
    return [part.lower() for part in parts if part]


def normalize_name(name: str) -> list[str]:
    match = TRAILING_HEX_RE.match(name)
    if match:
        return [match.group(1).lower(), "hex"]
    stripped = TRAILING_DIGITS_RE.sub(r"\1", name)
    tokens = [token for token in split_camel(stripped) if token not in STOPWORDS]
    return tokens


def similarity_score(left: str, right: str) -> float:
    left_tokens = set(normalize_name(left))
    right_tokens = set(normalize_name(right))
    if not left_tokens or not right_tokens:
        return 0.0
    inter = len(left_tokens & right_tokens)
    union = len(left_tokens | right_tokens)
    if union == 0:
        return 0.0
    return inter / union


def read_repo_file_at_head(rel_path: Path) -> str:
    try:
        proc = subprocess.run(
            ["git", "show", f"HEAD:{rel_path.as_posix()}"],
            cwd=WORKSPACE,
            check=True,
            capture_output=True,
            text=True,
            encoding="utf-8",
        )
        return proc.stdout
    except Exception:
        return read_text(WORKSPACE / rel_path)


def unescape_asm_string(text: str) -> str:
    return text.replace(r"\\", "\\").replace(r"\"", '"')


def parse_expansion_labels(rel_path: Path) -> list[dict[str, object]]:
    content = read_repo_file_at_head(rel_path)
    lines = content.splitlines()
    entries: list[dict[str, object]] = []
    i = 0
    while i < len(lines):
        label_match = LABEL_RE.match(lines[i])
        if not label_match:
            i += 1
            continue

        label = label_match.group(1)
        start_line = i + 1
        strings: list[str] = []
        i += 1
        while i < len(lines) and not LABEL_RE.match(lines[i]):
            line = lines[i]
            if ".string" in line or "_(" in line:
                for text_match in STRING_RE.findall(line):
                    strings.append(unescape_asm_string(text_match))
            i += 1

        entries.append(
            {
                "label": label,
                "file": rel_path.as_posix(),
                "line": start_line,
                "text": "".join(strings),
                "group": TRAILING_DIGITS_RE.sub(r"\1", label),
            }
        )
    return entries


def cluster_references(records: list[dict[str, object]]) -> list[list[dict[str, object]]]:
    clusters: list[list[dict[str, object]]] = []
    if not records:
        return clusters

    current = [records[0]]
    for record in records[1:]:
        prev = current[-1]
        same_context = (
            record["asm_file"] == prev["asm_file"]
            and record["function_name"] == prev["function_name"]
            and int(record["asm_line"]) - int(prev["asm_line"]) <= 24
        )
        if record.get("table_address") and record["table_address"] == prev.get("table_address"):
            sequential_addr = int(record.get("table_index") or 0) >= int(prev.get("table_index") or 0)
        else:
            sequential_addr = int(str(record["resolved_address"]), 16) >= int(str(prev["resolved_address"]), 16)
        if same_context and sequential_addr:
            current.append(record)
        else:
            clusters.append(current)
            current = [record]
    clusters.append(current)
    return clusters


def build_matches(extracted_rows: list[dict[str, object]], expansion_entries: dict[str, list[dict[str, object]]]) -> list[dict[str, object]]:
    rows_by_domain: dict[str, list[dict[str, object]]] = defaultdict(list)
    for row in extracted_rows:
        rows_by_domain[row["domain"]].append(row)

    matches: list[dict[str, object]] = []
    for domain, rows in rows_by_domain.items():
        if domain not in expansion_entries:
            for row in rows:
                matches.append(
                    {
                        **row,
                        "match_type": "review",
                        "match_reason": "No matching Expansion target file",
                        "expansion_label": "",
                        "expansion_line": "",
                        "expansion_text": "",
                    }
                )
            continue

        domain_entries = expansion_entries[domain]
        groups: dict[str, list[dict[str, object]]] = defaultdict(list)
        for entry in domain_entries:
            groups[str(entry["group"])].append(entry)

        for cluster in cluster_references(rows):
            function_name = str(cluster[0]["function_name"])
            count = len(cluster)
            scored_groups: list[tuple[float, str, list[dict[str, object]]]] = []
            for group_name, group_entries in groups.items():
                score = similarity_score(function_name, group_name)
                if score == 0:
                    continue
                if len(group_entries) == count:
                    score += 0.35
                elif abs(len(group_entries) - count) <= 2:
                    score += 0.15
                scored_groups.append((score, group_name, group_entries))
            scored_groups.sort(key=lambda item: (-item[0], abs(len(item[2]) - count), item[1]))

            if not scored_groups:
                for row in cluster:
                    matches.append(
                        {
                            **row,
                            "match_type": "review",
                            "match_reason": "No plausible label family inferred from function context",
                            "expansion_label": "",
                            "expansion_line": "",
                            "expansion_text": "",
                        }
                    )
                continue

            best_score, best_group_name, best_group = scored_groups[0]
            second_score = scored_groups[1][0] if len(scored_groups) > 1 else -1.0

            if best_score >= 0.80 and second_score <= best_score - 0.15 and len(best_group) == count:
                match_type = "exact"
                reason = f"Function name and label family align strongly ({function_name} -> {best_group_name})"
            elif best_score >= 0.45:
                match_type = "partial"
                reason = f"Function context suggests {best_group_name}, but count or uniqueness is weaker"
            else:
                match_type = "review"
                reason = f"Only weak family inference from {function_name}"

            for idx, row in enumerate(cluster):
                entry = best_group[idx] if idx < len(best_group) else None
                matches.append(
                    {
                        **row,
                        "match_type": match_type,
                        "match_reason": reason,
                        "expansion_label": entry["label"] if entry else "",
                        "expansion_line": entry["line"] if entry else "",
                        "expansion_text": entry["text"] if entry else "",
                    }
                )
    return matches


def detect_domain(asm_file: str) -> str:
    if "match_call" in asm_file:
        return "match_call"
    if asm_file.endswith("tv.s") or "battle_tv" in asm_file:
        return "tv"
    return "unknown"


def main() -> None:
    jp_rom = next((path for path in JP_ROM_CANDIDATES if path.exists()), None)
    if jp_rom is None:
        raise SystemExit(
            "Missing JP ROM. Checked: "
            + ", ".join(str(path) for path in JP_ROM_CANDIDATES)
        )

    OUT_DIR.mkdir(parents=True, exist_ok=True)

    charmap_entries = parse_charmap(JP_DECODE_CHARMAP)
    blobs = parse_data_blobs()
    references = parse_references()
    rom = jp_rom.read_bytes()

    extracted_rows: list[dict[str, object]] = []
    seen_addresses: set[tuple[str, int, int | None, int | None]] = set()
    for ref in references:
        for resolved in resolve_reference_texts(ref, rom, blobs, charmap_entries):
            key = (
                resolved.source.asm_file,
                resolved.resolved_address,
                resolved.table_address,
                resolved.table_index,
            )
            if key in seen_addresses:
                continue
            seen_addresses.add(key)

            entry = resolved.extracted
            extracted_rows.append(
                {
                    "domain": detect_domain(ref.asm_file),
                    "asm_file": ref.asm_file,
                    "function_name": ref.function_name,
                    "function_line": ref.function_line,
                    "asm_line": ref.asm_line,
                    "source_label": ref.source_label,
                    "target_address": f"0x{ref.target_address:08X}",
                    "table_address": f"0x{resolved.table_address:08X}" if resolved.table_address is not None else "",
                    "table_index": resolved.table_index if resolved.table_index is not None else "",
                    "resolved_address": f"0x{resolved.resolved_address:08X}",
                    "data_file": entry.blob.file_name,
                    "data_label": entry.blob.label,
                    "data_line": entry.blob.line_number,
                    "rom_offset": f"0x{entry.blob.rom_offset + entry.offset_in_blob:06X}",
                    "byte_length": entry.byte_length,
                    "unknown_count": entry.unknown_count,
                    "control_count": entry.control_count,
                    "has_japanese": entry.has_japanese,
                    "kana_count": count_kana(entry.decoded_text),
                    "named_token_count": count_named_tokens(entry.decoded_text),
                    "readability_score": readability_score(entry.decoded_text, entry.unknown_count),
                    "decoded_text": entry.decoded_text,
                }
            )

    expansion_entries = {
        domain: parse_expansion_labels(path)
        for domain, path in EXPANSION_TARGETS.items()
    }
    matched_rows = build_matches(extracted_rows, expansion_entries)

    with (OUT_DIR / "jp_asm_text_candidates.json").open("w", encoding="utf-8") as fp:
        json.dump(extracted_rows, fp, ensure_ascii=False, indent=2)

    with (OUT_DIR / "jp_asm_text_candidates.csv").open("w", encoding="utf-8-sig", newline="") as fp:
        writer = csv.DictWriter(fp, fieldnames=list(extracted_rows[0].keys()) if extracted_rows else [])
        if extracted_rows:
            writer.writeheader()
            writer.writerows(extracted_rows)

    with (OUT_DIR / "jp_expansion_match_candidates.json").open("w", encoding="utf-8") as fp:
        json.dump(matched_rows, fp, ensure_ascii=False, indent=2)

    with (OUT_DIR / "jp_expansion_match_candidates.csv").open("w", encoding="utf-8-sig", newline="") as fp:
        writer = csv.DictWriter(fp, fieldnames=list(matched_rows[0].keys()) if matched_rows else [])
        if matched_rows:
            writer.writeheader()
            writer.writerows(matched_rows)

    readable_rows = [
        row
        for row in matched_rows
        if int(row.get("kana_count", 0)) >= 6 and int(row.get("readability_score", 0)) >= 6
    ]

    with (OUT_DIR / "jp_readable_candidates.json").open("w", encoding="utf-8") as fp:
        json.dump(readable_rows, fp, ensure_ascii=False, indent=2)

    with (OUT_DIR / "jp_readable_candidates.csv").open("w", encoding="utf-8-sig", newline="") as fp:
        writer = csv.DictWriter(fp, fieldnames=list(readable_rows[0].keys()) if readable_rows else [])
        if readable_rows:
            writer.writeheader()
            writer.writerows(readable_rows)

    summary = {
        "jp_rom": str(jp_rom),
        "blob_count": len(blobs),
        "reference_count": len(references),
        "extracted_count": len(extracted_rows),
        "match_counts": dict(
            sorted(
                (
                    (match_type, sum(1 for row in matched_rows if row["match_type"] == match_type))
                    for match_type in {"exact", "partial", "review"}
                ),
                key=lambda item: item[0],
            )
        ),
        "readable_candidate_count": len(readable_rows),
        "notes": [
            "JP repo source uses anonymous gUnknown_* incbin blobs, so label-to-label recovery is inferred from asm context.",
            "JP text decoding uses the current charmap.txt because the referenced JP ROM strings align better with the project's active 1-byte Japanese mapping than with old-charmap.txt.",
            "Match classification is heuristic and intended for translation reference review, not direct auto-replacement.",
        ],
    }
    with (OUT_DIR / "summary.json").open("w", encoding="utf-8") as fp:
        json.dump(summary, fp, ensure_ascii=False, indent=2)

    print(json.dumps(summary, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
