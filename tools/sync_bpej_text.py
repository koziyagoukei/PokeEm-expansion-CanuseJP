#!/usr/bin/env python3
"""Synchronize reviewed display strings with the user's verified BPEJ ROM.

The manifest contains references, not a scanned-text dump. No fuzzy match is
ever applied by this tool. Script commands, identifiers and game data outside
the selected string literals are not rewritten.
"""

from __future__ import annotations

import argparse
from collections import Counter, defaultdict
import hashlib
import json
import re
import sys
from pathlib import Path

from check_baserom_jp import EXPECTED_GAME_CODE, EXPECTED_SHA1, EXPECTED_SIZE, REQUIRED_MESSAGE


ROOT = Path(__file__).resolve().parents[1]
LITERAL = r'"(?:\\.|[^"\\])*"'
SEQUENCE = LITERAL + r'(?:\s*' + LITERAL + r')*'
CONTROLS = {
    0: ("NAME_END", 0), 1: ("COLOR", 1), 2: ("HIGHLIGHT", 1),
    3: ("SHADOW", 1), 4: ("COLOR_HIGHLIGHT_SHADOW", 3), 5: ("PALETTE", 1),
    6: ("FONT", 1), 7: ("RESET_FONT", 0), 8: ("PAUSE", 1),
    9: ("PAUSE_UNTIL_PRESS", 0), 10: ("WAIT_SE", 0), 11: ("PLAY_BGM", 2),
    12: ("ESCAPE", 1), 13: ("SHIFT_RIGHT", 1), 14: ("SHIFT_DOWN", 1),
    15: ("FILL_WINDOW", 0), 16: ("PLAY_SE", 2), 17: ("CLEAR", 1),
    18: ("SKIP", 1), 19: ("CLEAR_TO", 1), 20: ("MIN_LETTER_SPACING", 1),
    21: ("JPN", 0), 22: ("ENG", 0), 23: ("PAUSE_MUSIC", 0),
    24: ("RESUME_MUSIC", 0),
}
FIELD_PLACEHOLDERS = {
    1: "PLAYER", 2: "STR_VAR_1", 3: "STR_VAR_2", 4: "STR_VAR_3",
    5: "KUN", 6: "RIVAL", 7: "VERSION", 8: "AQUA", 9: "MAGMA",
    10: "ARCHIE", 11: "MAXIE", 12: "KYOGRE", 13: "GROUDON",
}
BUTTONS = (
    "A_BUTTON", "B_BUTTON", "L_BUTTON", "R_BUTTON", "START_BUTTON", "SELECT_BUTTON",
    "DPAD_UP", "DPAD_DOWN", "DPAD_LEFT", "DPAD_RIGHT", "DPAD_UPDOWN", "DPAD_LEFTRIGHT", "DPAD_NONE",
)
SPECIAL_GLYPHS = (
    "UP_ARROW", "DOWN_ARROW", "LEFT_ARROW", "RIGHT_ARROW", "PLUS", "LV_2", "PP", "ID", "NO",
    "UNDERSCORE", "CIRCLE_1", "CIRCLE_2", "CIRCLE_3", "CIRCLE_4", "CIRCLE_5", "CIRCLE_6",
    "CIRCLE_7", "CIRCLE_8", "CIRCLE_9", "ROUND_LEFT_PAREN", "ROUND_RIGHT_PAREN", "CIRCLE_DOT",
    "TRIANGLE", "BIG_MULT_X", "CIRCLE_HOLLOW", "STAR", "TRIANGLE_UPSIDE_DOWN",
)


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def load_rom(path: Path) -> bytes:
    data = path.read_bytes()
    if (len(data) != EXPECTED_SIZE or data[0xAC:0xB0] != EXPECTED_GAME_CODE
            or hashlib.sha1(data).hexdigest() != EXPECTED_SHA1):
        raise ValueError(f"{REQUIRED_MESSAGE} (BPEJ SHA1 mismatch)")
    return data


class Charmap:
    def __init__(self, path: Path):
        self.characters = {}
        self.tokens = {}
        for line in path.read_text(encoding="utf-8").splitlines():
            m = re.match(r"^'((?:\\.|[^'\\])*)'\s*=\s*([0-9A-F ]+)", line)
            if m:
                char = m[1].replace(r"\'", "'").replace(r"\\", "\\")
                self.characters[char] = bytes.fromhex(m[2])
            m = re.match(r"^(\w+)\s*=\s*([0-9A-F ]+)", line)
            if m:
                self.tokens[m[1]] = bytes.fromhex(m[2])
        self.glyphs = {}
        for char, data in self.characters.items():
            if len(data) == 1:
                self.glyphs[data[0]] = char
        for char, data in self.characters.items():
            if len(data) == 1 and "\u3040" <= char <= "\u30ff":
                self.glyphs[data[0]] = char
        self.glyphs.update({0: " ", 0xAB: "!", 0xAC: "?", 0xAD: "。", 0xAE: "ー",
                            0xAF: "・", 0xB0: "…", 0xB1: "『", 0xB2: "』",
                            0xB3: "「", 0xB4: "」", 0xB7: "円"})
        # Only kana is legal in the BPEJ Japanese bank. Expansion's F1 = ヴ is not.
        for code in range(1, 0xA1):
            char = self.glyphs.get(code, "")
            if not ("\u3040" <= char <= "\u30ff"):
                raise ValueError(f"charmap: missing Japanese kana at {code:02X}")
        self.battle_placeholders = {}
        self.escaped_glyphs = {}
        for name, data in self.tokens.items():
            if name.startswith("B_") and len(data) == 2 and data[0] == 0xFD:
                self.battle_placeholders[data[1]] = name
            # Music IDs share these bytes; they are not text escape tokens.
            if name in BUTTONS or name in SPECIAL_GLYPHS or name.startswith("EMOJI_"):
                if len(data) == 2 and data[0] in (0xF8, 0xF9):
                    self.escaped_glyphs[data] = name

    def decode(self, data: bytes, offset: int, *, context="field", limit=8192):
        if not 0 <= offset < len(data):
            raise ValueError(f"text offset out of bounds: 0x{offset:X}")
        out = []
        pos = offset
        end = min(len(data), offset + limit)
        placeholders = self.battle_placeholders if context == "battle" else FIELD_PLACEHOLDERS
        japanese = True
        while pos < end:
            code = data[pos]
            pos += 1
            if code == 0xFF:
                return "".join(out), pos - offset
            if code in (0xFA, 0xFB, 0xFE):
                out.append({0xFA: r"\l", 0xFB: r"\p", 0xFE: r"\n"}[code])
            elif code == 0xFD:
                if pos == end or data[pos] not in placeholders:
                    raise ValueError("unknown string placeholder")
                out.append("{" + placeholders[data[pos]] + "}")
                pos += 1
            elif code == 0xFC:
                if pos == end or data[pos] not in CONTROLS:
                    raise ValueError("unknown extended control")
                name, argc = CONTROLS[data[pos]]
                if name in ("JPN", "ENG"):
                    japanese = name == "JPN"
                pos += 1
                if pos + argc > end:
                    raise ValueError("truncated extended control")
                args = data[pos:pos + argc]
                pos += argc
                if name in ("PLAY_BGM", "PLAY_SE"):
                    # Their numeric IDs need a semantic remap, never a raw copy.
                    value = int.from_bytes(args, "little")
                    out.append("{" + name + " " + str(value) + "}")
                else:
                    out.append("{" + name + "".join(" " + str(arg) for arg in args) + "}")
            elif code in (0xF7, 0xF8, 0xF9):
                if pos == end:
                    raise ValueError("truncated escape sequence")
                arg = data[pos]
                pos += 1
                if code == 0xF7:
                    out.append("{DYNAMIC " + str(arg) + "}")
                else:
                    name = self.escaped_glyphs.get(bytes((code, arg)))
                    if name is None:
                        raise ValueError("unknown button/special glyph")
                    out.append("{" + name + "}")
            elif code == 0xF1 or code not in self.glyphs:
                raise ValueError(f"undefined BPEJ glyph 0x{code:02X}")
            elif not japanese and 1 <= code <= 0xA0:
                raise ValueError("Latin-bank glyph requires a reviewed Japanese adaptation")
            else:
                out.append(self.glyphs[code])
        raise ValueError("text has no terminator within its boundary")

    def validate(self, text: str):
        for token in re.findall(r"\{([^}]+)\}", text):
            name = token.split()[0]
            if name not in self.tokens:
                raise ValueError(f"token {{{name}}} is not in charmap.txt")
        visible = re.sub(r"\{[^}]+\}|\\[nlp]", "", text)
        visible = visible.replace(r'\"', '"').replace(r"\\", "\\")
        missing = sorted(set(visible) - self.characters.keys())
        if missing:
            raise ValueError(f"undefined characters: {missing!r}")


def literal_contents(sequence: str) -> str:
    return "".join(m[1:-1] for m in re.findall(LITERAL, sequence))


def equivalent_text(before: str, after: str, charmap: Charmap) -> bool:
    if not before.startswith("{JPN}"):
        return before == after
    def normalized(text):
        # Repeated JPN controls and full-width punctuation aliases have the same
        # display result. Do not churn already-correct translations for these.
        return [charmap.characters.get(part, part) for part in re.findall(
            r"\{[^}]+\}|\\.|.", text.replace("{JPN}", ""))]
    return normalized(before) == normalized(after)


def asm_strings(text: str):
    labels = list(re.finditer(r"^(\w+)::?[^\S\n]*(?:@[^\n]*)?$", text, re.M))
    for index, label in enumerate(labels):
        end = labels[index + 1].start() if index + 1 < len(labels) else len(text)
        body = text[label.end():end]
        directives = list(re.finditer(r"^[ \t]*\.string[ \t]+(" + LITERAL + r")[ \t]*(?:@[^\n]*)?$", body, re.M))
        if not directives:
            continue
        between = body[directives[0].start():directives[-1].end()]
        non_directives = re.sub(r"^[ \t]*\.string[^\n]*$", "", between, flags=re.M)
        if non_directives.strip():
            continue
        first = label.end() + directives[0].start(1)
        last = label.end() + directives[-1].end(1)
        value = "".join(literal_contents(m[1]) for m in directives)
        yield label[1], first, last, value


def find_target(text: str, target: dict):
    if "table" in target:
        table = re.escape(target["table"])
        blocks = list(re.finditer(rf"\b{table}\s*\[[^\]]*\]\s*=\s*\{{(?P<body>.*?)^\}};", text, re.M | re.S))
        if len(blocks) != 1:
            raise ValueError(f"expected one table definition {target['table']}")
        block = blocks[0]
        local_target = {key:value for key,value in target.items() if key != "table"}
        start, end, value = find_target(block["body"], local_target)
        return block.start("body") + start, block.start("body") + end, value
    kind = target["kind"]
    name = re.escape(target["label"])
    if kind == "asm":
        found = [entry for entry in asm_strings(text) if entry[0] == target["label"]]
        if len(found) != 1:
            raise ValueError(f"expected one unconditional string label {target['label']}")
        _, start, end, value = found[0]
        return start, end, value
    if kind == "variable":
        pattern = rf"\b{name}\s*\[[^\]]*\]\s*=\s*_\(\s*(?P<strings>{SEQUENCE})\s*\)"
    elif kind == "indexed":
        pattern = rf"\[\s*{name}\s*\]\s*=\s*COMPOUND_STRING\(\s*(?P<strings>{SEQUENCE})\s*\)"
    elif kind == "aggregate":
        pattern = rf"\[\s*{name}\s*\]\s*=\s*\{{\s*_\(\s*(?P<strings>{SEQUENCE})\s*\)"
    elif kind == "ordinal_literal":
        matches = list(re.finditer(r"\b(?:_|COMPOUND_STRING)\(\s*(?P<strings>" + SEQUENCE + r")\s*\)", text))
        if len(matches) != target["rowCount"] or not 0 <= target["index"] < len(matches):
            raise ValueError(f"literal count/index changed in {target['label']}")
        match = matches[target["index"]]
        return match.start("strings"), match.end("strings"), literal_contents(match["strings"])
    elif kind == "ordinal_field":
        field = re.escape(target["field"])
        pattern = rf"\.{field}\s*=\s*\w+\(\s*(?P<strings>{SEQUENCE})\s*\)"
        matches = list(re.finditer(pattern, text))
        if len(matches) != target["rowCount"] or not 0 <= target["index"] < len(matches):
            raise ValueError(f"row count/index changed in {target['label']}")
        match = matches[target["index"]]
        return match.start("strings"), match.end("strings"), literal_contents(match["strings"])
    elif kind == "field":
        entries = list(re.finditer(r"\[\s*(\w+)\s*\]\s*=\s*\{", text))
        blocks = []
        for idx, entry in enumerate(entries):
            if entry[1] == target["label"]:
                end = entries[idx + 1].start() if idx + 1 < len(entries) else len(text)
                blocks.append((entry.end(), text[entry.end():end]))
        if len(blocks) != 1:
            raise ValueError(f"expected one table entry {target['label']}")
        base, block = blocks[0]
        field = re.escape(target["field"])
        pattern = rf"\.{field}\s*=\s*\w+\(\s*(?P<strings>{SEQUENCE})\s*\)"
        matches = list(re.finditer(pattern, block))
        count = target.get("variantCount", 1)
        index = target.get("variantIndex", 0)
        if len(matches) != count or not 0 <= index < len(matches):
            raise ValueError(f"expected one literal field {target['label']}.{target['field']}")
        m = matches[index]
        return base + m.start("strings"), base + m.end("strings"), literal_contents(m["strings"])
    else:
        raise ValueError(f"unsupported selector kind {kind}")
    matches = list(re.finditer(pattern, text))
    if len(matches) != 1:
        raise ValueError(f"expected one literal initializer {target['label']}")
    m = matches[0]
    return m.start("strings"), m.end("strings"), literal_contents(m["strings"])


def expected_text(rom, entry, charmap):
    offset = int(entry["rom_offset"], 0)
    raw_length = entry["size"]
    raw = rom[offset:offset + raw_length]
    if len(raw) != raw_length or sha256(raw) != entry["sha256"]:
        raise ValueError("reference byte hash/boundary mismatch")
    fmt = entry.get("format", "terminated")
    if fmt == "zero_padded":
        if b"\xFF" in raw:
            raise ValueError("unexpected terminator in zero-padded text field")
        text, _ = charmap.decode(raw.rstrip(b"\x00") + b"\xFF", 0, context=entry.get("context", "field"))
    elif fmt == "terminated":
        text, consumed = charmap.decode(rom, offset, context=entry.get("context", "field"), limit=raw_length)
        if consumed != raw_length:
            raise ValueError("manifest must describe exactly one terminated string")
    else:
        raise ValueError(f"unsupported text format {fmt}")
    for before, after in entry.get("adaptations", []):
        if before not in text:
            raise ValueError(f"adaptation source not found: {before}")
        text = text.replace(before, after)
    text = re.sub(r"\{JPN\}", "", text)
    text = "{JPN}" + text
    if entry["target"]["kind"] == "asm":
        text += "$"
    charmap.validate(text)
    return text


def sync(root, rom, manifest, *, apply=False):
    charmap = Charmap(root / "charmap.txt")
    grouped = defaultdict(list)
    for entry in manifest["entries"]:
        path = root / entry["file"]
        if not path.resolve().is_relative_to(root.resolve()) or path.suffix not in (".inc", ".c", ".h"):
            raise ValueError("target path must be a display-text source inside this repository")
        grouped[path].append(entry)
    counts = Counter()
    errors = []
    changes = []
    pending = []
    for path, entries in grouped.items():
        with path.open(encoding="utf-8", newline="") as handle:
            original = handle.read()
        text = original.replace("\r\n", "\n")
        edits = []
        for entry in entries:
            try:
                start, end, before = find_target(text, entry["target"])
                after = expected_text(rom, entry, charmap)
                if equivalent_text(before, after, charmap):
                    counts["verified"] += 1
                    continue
                if sha256(before.encode()) != entry["source_sha256"]:
                    raise ValueError("source changed since review; refusing to overwrite")
                if not apply:
                    raise ValueError("display text is not synchronized with its reviewed ROM reference")
                edits.append((start, end, '"' + after + '"'))
                changes.append({"file": entry["file"], "target": entry["target"], "rom_offset": entry["rom_offset"], "before": before, "after": after})
                counts["updated"] += 1
            except (ValueError, KeyError) as error:
                errors.append(f"{entry['file']}:{entry['target']['label']}: {error}")
        ordered = sorted(edits)
        if any(a[1] > b[0] for a, b in zip(ordered, ordered[1:])):
            errors.append(f"{path}: overlapping string replacements")
        for start, end, replacement in reversed(ordered):
            text = text[:start] + replacement + text[end:]
        if edits:
            pending.append((path, text.replace("\n", "\r\n") if "\r\n" in original else text))
    # Validate every planned edit first; an error never leaves a half-imported tree.
    if errors:
        raise ValueError("\n".join(errors))
    for path, text in pending:
        with path.open("w", encoding="utf-8", newline="") as handle:
            handle.write(text)
    return dict(counts), changes


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=ROOT)
    parser.add_argument("--baserom", type=Path, default=ROOT / "baserom.gba")
    parser.add_argument("--manifest", type=Path, default=ROOT / "tools/data/bpej_text_manifest.json")
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--apply", action="store_true", help="apply only reviewed, unchanged string targets")
    mode.add_argument("--check", action="store_true", help="verify ROM hashes and all selected display strings")
    parser.add_argument("--report", type=Path, help="optional UTF-8 JSON change report (keep under build/)")
    args = parser.parse_args()
    try:
        manifest = json.loads(args.manifest.read_text(encoding="utf-8"))
        if manifest.get("schema") != "bpej-reviewed-text-v1" or manifest.get("rom_sha1") != EXPECTED_SHA1:
            raise ValueError("unsupported manifest or ROM identity")
        rom = load_rom(args.baserom)
        counts, changes = sync(args.root, rom, manifest, apply=args.apply)
        if args.report:
            args.report.parent.mkdir(parents=True, exist_ok=True)
            args.report.write_text(json.dumps({"counts": counts, "changes": changes}, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        print("BPEJ text:", json.dumps(counts, sort_keys=True))
        return 0
    except (OSError, ValueError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
