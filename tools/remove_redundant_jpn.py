#!/usr/bin/env python3
"""Audit/remove redundant leading JPN in reviewed standalone display contexts.

Defaults to a dry run. Internal mode switches, saved names and reusable text
fragments are not rewritten. The report lists every decision and source hash.
"""

from __future__ import annotations

import argparse
from collections import Counter, defaultdict
import hashlib
import json
from pathlib import Path
import re
import sys

from audit_jp_localization import WRAPPER, mask_comments
from check_jp_text import iter_source_files
from sync_bpej_text import Charmap, asm_strings, literal_contents

ROOT = Path(__file__).resolve().parents[1]
# These tables contain complete messages/pages, not names or appended fragments.
MESSAGE_FILES = {
    "data/text/birch_speech.inc", "data/text/tv.inc", "data/text/match_call.inc",
    "data/text/apprentice.inc", "data/text/trainers.inc", "data/text/battle_tent.inc",
    "src/data/text/match_call_messages.h", "src/data/info_viewer.h",
}
DESCRIPTION_FIELDS = {"description", "pokedexText", "pokedexDescription"}
DIRECT_DISPLAY = re.compile(
    r"\b(?:AddTextPrinter\w*|AddSpriteTextPrinter\w*|PrintTextOnWindow\w*|"
    r"PrintMainMenu\w*|ShowFieldMessage|ShowFieldAutoScrollMessage|"
    r"DisplayMessageAndContinueTask|DisplayItemMessage\w*|"
    r"GetStringWidth|GetStringCenterAlignXOffset\w*|GetStringRightAlignXOffset\w*)\s*\(")


def encode(text, charmap):
    """Encode reviewed literals for the renderer regression harness."""
    out = bytearray()
    pos = 0
    while pos < len(text):
        if text[pos] == "{":
            end = text.index("}", pos)
            parts = text[pos + 1:end].split()
            out.extend(charmap.tokens[parts[0]])
            for arg in parts[1:]:
                if arg in charmap.tokens:
                    out.extend(charmap.tokens[arg])
                else:
                    value = int(arg, 0)
                    size = 2 if parts[0] in {"PLAY_SE", "PLAY_BGM"} else 1
                    out.extend(value.to_bytes(size, "little"))
            pos = end + 1
        elif text[pos] == "\\":
            value = text[pos:pos + 2]
            if value in charmap.characters:
                out.extend(charmap.characters[value])
            elif value in {r'\"', r"\\", r"\'"}:
                out.extend(charmap.characters[value[1]])
            else:
                raise ValueError(f"unsupported escape {value}")
            pos += 2
        else:
            out.extend(charmap.characters[text[pos]])
            pos += 1
    if not out or out[-1] != 0xFF:
        out.append(0xFF)
    return bytes(out)


def candidates(path, source):
    text = mask_comments(source).replace("\r", " ")
    if path.suffix in {".inc", ".s"}:
        for label, first, last, value in asm_strings(text):
            if value.startswith("{JPN}"):
                yield {"label": label, "offset": first + 1, "text": value, "kind": "asm"}
    for match in WRAPPER.finditer(text):
        value = literal_contents(match[1])
        if not value.startswith("{JPN}"):
            continue
        before = text[max(0, match.start() - 180):match.start()]
        field = re.search(r"\.(\w+)\s*=\s*$", before)
        named = re.search(r"\b(?:const\s+)?u8\s+(\w+)\s*\[[^\]\n]*\]\s*=\s*$", before)
        yield {"label": named[1] if named else "." + field[1] if field else "(inline)",
               "offset": match.start(1) + 1, "text": value, "kind": "c",
               "field": field[1] if field else None,
               "name": named[1] if named else None}


def policy(row, relative, references, script_messages, embedded):
    label = row["label"]
    if label in embedded:
        return "retain", "embedded_fragment"
    if relative in MESSAGE_FILES:
        return "remove", "complete_message_table"
    if row["kind"] == "asm":
        if label in script_messages:
            return "remove", "script_message"
        return "retain", "review_callers"
    if row.get("field") in DESCRIPTION_FIELDS:
        return "remove", "description"
    if name := row.get("name"):
        uses = references.get(name, [])
        if uses and all(DIRECT_DISPLAY.search(line) for line in uses):
            return "remove", "direct_display"
        # The new-game dialogue is passed through placeholder expansion first.
        if name == "gText_ThisIsAPokemon" or name.startswith(("gText_MainMenu", "gText_ContinueMenu")):
            return "remove", "main_menu"
    return "retain", "name_or_reusable_fragment"


def audit(root):
    paths = set(iter_source_files(root)) | set((root / "data").rglob("*.s"))
    sources = {p: p.read_bytes().decode("utf-8") for p in sorted(paths)}
    print(f"Read {len(sources)} source files", flush=True)
    rows = []
    for path, source in sources.items():
        for row in candidates(path, source):
            row.update(file=path.relative_to(root).as_posix(), line=source.count("\n", 0, row["offset"]) + 1)
            rows.append(row)
    print(f"Found {len(rows)} leading-prefix candidates", flush=True)
    names = {row["label"] for row in rows}
    references = defaultdict(list)
    script_messages, embedded = set(), set()
    for path, source in sources.items():
        for line in mask_comments(source).splitlines():
            # Definitions, declarations and comments do not identify a caller.
            if re.search(r"\bextern\b|\bu8\s+\w+\s*\[|^\w+::?\s*$|\.string\b", line):
                continue
            used = names.intersection(re.findall(r"\b\w+\b", line))
            for name in used:
                references[name].append(line)
            if re.match(r"\s*(?:msgbox|message|trainerbattle\w*)\b", line):
                script_messages.update(used)
            if re.search(r"\b(?:bufferstring|StringAppend|StringAppendN|DynamicPlaceholderTextUtil_SetPlaceholderPtr)\b", line):
                embedded.update(used)
    charmap = Charmap(root / "charmap.txt")
    print("Classifying callers and encoding candidates", flush=True)
    for row in rows:
        row["action"], row["reason"] = policy(row, row["file"], references, script_messages, embedded)
        if row["action"] == "remove":
            try:
                encoded = encode(row["text"], charmap)
                if not encoded.startswith(b"\xfc\x15"):
                    raise ValueError("not a leading JPN")
                # A later leading ENG/AUTO can change the width of earlier
                # spaces when ResolveTextMode looks ahead. Keep that boundary.
                rest = row["text"][5:]
                if re.match(r"\s|\\[nlp]", rest):
                    raise ValueError("leading whitespace needs an explicit mode")
                row["encoded_hex"] = encoded.hex()
            except (KeyError, ValueError, OverflowError) as error:
                row.update(action="retain", reason="encoding_or_initial_mode_review", detail=str(error))
    return sources, rows


def apply_rows(root, sources, rows):
    by_file = defaultdict(list)
    for row in rows:
        if row["action"] == "remove":
            by_file[root / row["file"]].append(row)
    for path in by_file:
        if path.read_bytes().decode("utf-8") != sources[path]:
            raise ValueError(f"source changed during audit: {path}")
    for path, selected in by_file.items():
        source = sources[path]
        after = source
        for row in sorted(selected, key=lambda r: r["offset"], reverse=True):
            start = row["offset"]
            if after[start:start + 5] != "{JPN}":
                raise ValueError(f"prefix moved: {path}:{row['line']}")
            after = after[:start] + after[start + 5:]
        # Preserve the file's original newline representation and all other bytes.
        original = path.read_bytes()
        if original.decode("utf-8") != source:
            raise ValueError(f"source changed during audit: {path}")
        path.write_bytes(after.encode("utf-8"))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=ROOT)
    parser.add_argument("--report", type=Path, default=ROOT / "build/reports/jpn_prefixes/migration.json")
    parser.add_argument("--apply", action="store_true")
    args = parser.parse_args()
    root = args.root.resolve()
    if not re.search(r"^#define\s+TEXT_JAPANESE_BY_DEFAULT\s+TRUE\b", (root / "include/config/text.h").read_text(encoding="utf-8"), re.M):
        raise SystemExit("TEXT_JAPANESE_BY_DEFAULT must be TRUE")
    sources, rows = audit(root)
    if args.apply:
        # Refuse to edit if actual rendering/metrics differ. Run in WSL with cc.
        sys.path.insert(0, str(root / "tools/tests"))
        from test_remove_redundant_jpn import verify_rows
        verify_rows(root, rows)
        apply_rows(root, sources, rows)
    counts = Counter(row["action"] for row in rows)
    changed_files = {row["file"] for row in rows if row["action"] == "remove"}
    report = {"applied": args.apply, "renderer_verified": args.apply, "counts": dict(counts),
              "reasons": dict(Counter(row["reason"] for row in rows)),
              "source_sha256": {path.relative_to(root).as_posix(): hashlib.sha256(text.encode("utf-8")).hexdigest()
                                for path, text in sources.items() if path.relative_to(root).as_posix() in changed_files},
              "entries": rows}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"counts": report["counts"], "reasons": report["reasons"],
                      "files": len(report["source_sha256"]), "applied": args.apply}, ensure_ascii=False, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
