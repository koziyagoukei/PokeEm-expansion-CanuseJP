#!/usr/bin/env python3
"""Compare display strings with a local, revision-pinned Emerald Poké Corpus.

This independently written reader does not download data or apply translations.
The corpus uses aligned qid/en/ja-Hrkt lines; see its NOTES.md for notation.
Reference downloads and full-text reports belong under ignored build/ paths.
"""
from __future__ import annotations

import argparse
from collections import Counter, defaultdict
import hashlib
import json
from pathlib import Path
import re
import subprocess

from audit_jp_localization import classify, mask_comments
from kanji.kanji_gen import code_mask
from sync_bpej_text import Charmap, CONTROLS, LITERAL, SEQUENCE, asm_strings, literal_contents, load_rom

ROOT = Path(__file__).resolve().parents[1]
MODES = re.compile(r"\{(?:JPN|AUTO|KANJI)\}")
TOKENS = re.compile(r"\{([^}]+)\}")
VARIABLE = re.compile(r"\b(\w+)\s*\[\s*\]\s*=\s*_\(\s*(" + SEQUENCE + r")\s*\)")
INDEXED = re.compile(r"\[\s*(\w+)\s*\]\s*=\s*COMPOUND_STRING\(\s*(" + SEQUENCE + r")\s*\)")
WIDTH_ALIASES = str.maketrans({chr(c): chr(c - 0xFEE0) for c in range(0xFF01, 0xFF5F)})


def from_corpus(value):
    if "[NULL]" in value or "[UNUSED]" in value:
        raise ValueError("no Japanese counterpart")
    value = value.translate(WIDTH_ALIASES).replace("\u3000", " ").replace("‥", "…")
    value = value.replace(r"\c", r"\p").replace(r"\r", r"\l")
    # Literal escaped brackets and ruby need explicit handling before importing.
    if r"\[" in value or r"\{" in value or "{" in value or "}" in value:
        raise ValueError("unsupported literal brackets/ruby")
    return re.sub(r"\[([^\[\]]+)\]", lambda m: "{" + m[1] + "}", value)


def encode_text(value, charmap):
    """Encode only literal glyphs and already-known token bytes, without guesses."""
    value = MODES.sub("", value)
    if value.endswith("$"):
        value = value[:-1]
    result, i = bytearray(), 0
    while i < len(value):
        if value[i] == "{":
            end = value.index("}", i)
            tokens = value[i + 1:end].split()
            if len(tokens) == 1 and re.fullmatch(r"0x[0-9A-Fa-f]{2}", tokens[0]):
                result.append(int(tokens[0], 16))
                i = end + 1
                continue
            if not tokens or tokens[0] not in charmap.tokens:
                raise ValueError("unresolved token: " + value[i + 1:end])
            result.extend(charmap.tokens[tokens[0]])
            arguments = []
            for token in tokens[1:]:
                if token in charmap.tokens:
                    arguments.extend(charmap.tokens[token])
                elif re.fullmatch(r"\d+|0[xX][0-9A-Fa-f]+", token):
                    number = int(token, 16 if token.lower().startswith("0x") else 10)
                    if not 0 <= number <= 255:
                        raise ValueError("control byte out of range")
                    arguments.append(number)
                else:
                    raise ValueError("unresolved token argument: " + token)
            control = next((argc for name, argc in CONTROLS.values() if name == tokens[0]), 0)
            if len(arguments) != control:
                raise ValueError("control argument count differs: " + tokens[0])
            result.extend(arguments)
            i = end + 1
        else:
            key = value[i:i + 2] if value[i] == "\\" else value[i]
            if key == r'\"':
                key = '"'
            if key not in charmap.characters:
                raise ValueError("unmapped glyph: " + repr(key))
            result.extend(charmap.characters[key])
            i += 2 if value[i] == "\\" else 1
    if 0xFF in result:
        raise ValueError("embedded terminator requires review")
    return bytes(result) + b"\xFF"


def placeholders(value):
    return Counter(token for token in TOKENS.findall(value)
                   if token.startswith(("STR_VAR_", "B_", "DYNAMIC"))
                   or token in {"PLAYER", "RIVAL", "KUN", "VERSION", "AQUA", "MAGMA",
                                "ARCHIE", "MAXIE", "KYOGRE", "GROUDON"})


def load_corpus(directory):
    names = ("qid_msg.txt", "en_msg.txt", "ja-Hrkt_msg.txt")
    contents = [(directory / name).read_bytes() for name in names]
    lines = [data.decode("utf-8-sig").splitlines() for data in contents]
    if len({len(part) for part in lines}) != 1:
        raise ValueError("corpus qid/en/ja line counts differ")
    index = defaultdict(list)
    for number, (qid, english, japanese) in enumerate(zip(*lines), 1):
        parts = qid.split(".")
        if len(parts) == 4 and parts[0] == "e":
            index[parts[-1]].append(dict(qid=qid, group=parts[2], corpus_line=number,
                                        english=english, japanese=japanese))
    metadata = {"revision": (directory / "revision.txt").read_text(encoding="ascii").strip(),
                "duplicate_qids_skipped": sum(n > 1 for n in Counter(lines[0]).values()),
                "files": {name: hashlib.sha256(data).hexdigest() for name, data in zip(names, contents)}}
    return index, metadata


def source_strings(root):
    for path in sorted((root / "data").rglob("*.inc")):
        text = mask_comments(path.read_text(encoding="utf-8"))
        group = path.parent.name if path.name == "scripts.inc" else path.stem
        for label, start, end, value in asm_strings(text):
            yield path, text, group, {"kind": "asm", "label": label}, start, end, value
    for path in sorted((root / "src").rglob("*")):
        if path.suffix not in (".c", ".h") or not path.is_file():
            continue
        text = mask_comments(path.read_text(encoding="utf-8"))
        masked = code_mask(text)
        for match in VARIABLE.finditer(text):
            if masked[match.start(1):match.end(1)] != match[1]:
                continue
            yield path, text, path.stem, {"kind": "variable", "label": match[1]}, \
                match.start(2), match.end(2), literal_contents(match[2])


def english_key(text):
    return re.sub(r"\s+", " ", MODES.sub("", text)).strip()


def indexed_battle_strings(root, index):
    """Match modern STRINGIDs through this checkout's upstream English literals."""
    result = subprocess.run(["git", "show", "upstream/master:src/battle_message.c"], cwd=root,
                            capture_output=True, text=True, encoding="utf-8")
    if result.returncode:
        return
    english = {m[1]: literal_contents(m[2]) for m in INDEXED.finditer(mask_comments(result.stdout))}
    by_english = defaultdict(list)
    for entries in index.values():
        for entry in entries:
            if entry["group"] == "battle_message":
                try:
                    by_english[english_key(from_corpus(entry["english"]))].append(entry)
                except ValueError:
                    pass
    path = root / "src/battle_message.c"
    text = mask_comments(path.read_text(encoding="utf-8"))
    for match in INDEXED.finditer(text):
        if match[1] not in english:
            continue
        matches = by_english.get(english_key(english.get(match[1], "")), [])
        if len(matches) == 1:
            yield path, text, {"kind": "indexed", "label": match[1]}, match.start(2), \
                literal_contents(match[2]), matches[0]


def audit(root, corpus_dir, rom):
    charmap = Charmap(root / "charmap.txt")
    index, metadata = load_corpus(corpus_dir)
    revision = subprocess.run(["git", "rev-parse", "upstream/master"], cwd=root,
                              capture_output=True, text=True)
    metadata["upstream_revision"] = revision.stdout.strip() if revision.returncode == 0 else None
    rows, counts = [], Counter()
    aligned = []
    for path, text, group, target, start, end, before in source_strings(root):
        matches = [entry for entry in index.get(target["label"], []) if entry["group"] == group]
        if len(matches) == 1:
            aligned.append((path, text, target, start, before, matches[0]))
    aligned.extend(indexed_battle_strings(root, index))
    for path, text, target, start, before, entry in aligned:
        row = dict(file=path.relative_to(root).as_posix(), target=target,
                   line=text.count("\n", 0, start) + 1, before=before,
                   source_sha256=hashlib.sha256(before.encode()).hexdigest(), **entry)
        try:
            reference = from_corpus(entry["japanese"])
            raw = encode_text(reference, charmap)
            offset = rom.find(raw)
            if offset < 0:
                raise ValueError("Japanese reference bytes not found in verified BPEJ")
            row.update(reference=reference, rom_offset=hex(offset), size=len(raw),
                       sha256=hashlib.sha256(raw).hexdigest())
            try:
                same = encode_text(before, charmap) == raw
            except ValueError:
                same = False
            if same:
                row["status"] = "matching"
            elif placeholders(before) != placeholders(reference):
                row["status"] = "placeholder_review"
            else:
                row["status"] = "english" if classify(before)[0] == "english" else "japanese_difference"
        except ValueError as error:
            row.update(status="unsupported", reason=str(error))
        counts[row["status"]] += 1
        rows.append(row)
    return {"corpus": metadata, "counts": dict(counts), "entries": rows}


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--root", type=Path, default=ROOT)
    parser.add_argument("--corpus", type=Path, default=ROOT / "build/reference/poke-corpus")
    parser.add_argument("--output", type=Path, default=ROOT / "build/reports/poke_corpus/comparison.json")
    args = parser.parse_args()
    try:
        report = audit(args.root, args.corpus, load_rom(args.root / "baserom.gba"))
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(report, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    except (OSError, ValueError) as error:
        parser.exit(1, f"error: {error}\n")
    print(json.dumps(report["counts"], ensure_ascii=False))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
