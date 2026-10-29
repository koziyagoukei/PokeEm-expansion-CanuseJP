#!/usr/bin/env python3
"""Prepare and build optional, latest-generation Japanese Pokédex descriptions.

Only reference IDs/hashes are committed. Corpus downloads and selected text
are generated locally; ordinary kana builds need no corpus download.
"""
from __future__ import annotations

import argparse
from collections import Counter, defaultdict
import hashlib
import json
from pathlib import Path
import re
import shlex
import subprocess
import sys
import urllib.request

from kanji.kanji_gen import Atlas, is_kanji, strip_c_comments, write_if_changed
from sync_bpej_text import Charmap, SEQUENCE, literal_contents

ROOT = Path(__file__).resolve().parents[1]
REVISION = "a022d41cc0e62858bc875a62eb1ba51b1d465826"
CACHE = "build/reference/poke-corpus"
MANIFEST = "tools/data/pokedex_kanji_manifest.json"
HEADER = "src/data/pokemon/pokedex_text_selected.h"
REPORT = "build/generated/pokedex_text/report.json"
# Newest generation first, then newest release within that generation.
PACKS = (("LegendsZA", "common"), ("ScarletViolet", "common"),
         ("LegendsArceus", "common"), ("BrilliantDiamondShiningPearl", "message"),
         ("SwordShield", "common"), ("UltraSunUltraMoon", "common"),
         ("OmegaRubyAlphaSapphire", "common"))
RUBY = re.compile(r"\{([^{}|]+)\|([^{}]+)\}")
ALIASES = str.maketrans({chr(c): chr(c - 0xFEE0) for c in range(0xFF01, 0xFF5F)})
SCHEMA = "poke-corpus-pokedex-kanji-v1"
MAX_WIDTH = 224
MAX_LINES = 3


def digest(data):
    return hashlib.sha256(data).hexdigest()


def plain(text, *, reading=False):
    text = RUBY.sub(lambda m: m[2 if reading else 1], text)
    text = text.translate(ALIASES).replace("\u3000", " ").replace("‥", "…").replace("、", " ")
    text = text.replace(r"\n", " ").replace(r"\r", " ").replace(r"\c", " ")
    return re.sub(r"\s+", " ", text).strip()


def usable(text):
    return (bool(text.strip()) and "[NULL]" not in text and "[UNUSED]" not in text
            and not re.fullmatch(r"(?:\[[^\]]+\]\s*)+", text.strip()))


def fold(text):
    return re.sub(r"\s+|\{(?:JPN|AUTO|KANJI)\}|\\[nlp]", "", plain(text, reading=True))


def read_pack(root, game, kind, *, verify=None):
    values, fingerprints = {}, {}
    for language in ("qid", "ja", "ja-Hrkt"):
        if language == "ja-Hrkt" and game == "ScarletViolet":
            continue
        relative = f"{game}/{language}_{kind}.txt"
        path = root / CACHE / relative
        if not path.exists():
            raise ValueError(f"missing {path}; run python3 tools/pokedex_kanji.py fetch")
        data = path.read_bytes()
        fingerprints[relative] = digest(data)
        if verify is not None and verify.get(relative) != fingerprints[relative]:
            raise ValueError(f"corpus hash mismatch: {relative}; fetch the pinned revision again")
        values[language] = data.decode("utf-8-sig").splitlines()
    if len({len(rows) for rows in values.values()}) != 1:
        raise ValueError(f"unaligned corpus languages: {game}")
    rows = []
    for i, qid in enumerate(values["qid"]):
        rows.append(dict(game=game, qid=qid, line=i + 1, kanji=values["ja"][i],
                         kana=values.get("ja-Hrkt", values["ja"])[i]))
    return rows, fingerprints


def key_for(row):
    qid, game = row["qid"], row["game"]
    match = re.search(r"\.ZKN_COMMENT_([AB])_(\d+)_(\d+)$", qid)
    if match:
        return int(match[2]), int(match[3]), match[1]
    match = re.search(r"\.DP_pokedex_(diamond|pearl)_(\d+)$", qid)
    if game == "BrilliantDiamondShiningPearl" and match:
        return int(match[2]), 0, "A" if match[1] == "diamond" else "B"
    match = re.fullmatch(r"usum\.common\.(124|125)\.(\d+)", qid)
    if game == "UltraSunUltraMoon" and match and 1 <= int(match[2]) <= 807:
        return int(match[2]), 0, "A" if match[1] == "124" else "B"
    match = re.fullmatch(r"oras\.common\.(6|7)\.(\d+)", qid)
    if game == "OmegaRubyAlphaSapphire" and match and 1 <= int(match[2]) <= 721:
        return int(match[2]), 0, "A" if match[1] == "6" else "B"
    return None


def catalog(root, verify=None):
    candidates, form_labels, hashes = defaultdict(list), defaultdict(list), {}
    for rank, (game, kind) in enumerate(PACKS):
        rows, fingerprints = read_pack(root, game, kind, verify=verify)
        hashes.update(fingerprints)
        for row in rows:
            if not usable(row["kanji"]):
                continue
            key = key_for(row)
            if key and key[0] > 0:
                row.update(dex=key[0], form=key[1], version=key[2], rank=rank)
                candidates[key[:2]].append(row)
            match = re.search(r"\.ZKN_FORM_(\d+)_(\d+)$", row["qid"])
            if match and int(match[2]) != 0:
                form_labels[int(match[1])].append((int(match[2]), plain(row["kanji"])))
    for rows in candidates.values():
        rows.sort(key=lambda row: (row["rank"], row["version"], row["qid"]))
    return candidates, form_labels, hashes


def enum_values(text, prefix):
    values, last = {}, -1
    for match in re.finditer(r"^\s*(" + prefix + r"\w+)\s*(?:=\s*(\w+))?\s*,", strip_c_comments(text), re.M):
        name, value = match.groups()
        if value is None:
            last += 1
        elif value.isdigit():
            last = int(value)
        elif value in values:
            last = values[value]
        else:
            continue
        values[name] = last
    return values


def species_records(root, cpp=None):
    dex = enum_values((root / "include/constants/pokedex.h").read_text(encoding="utf-8"), "NATIONAL_DEX_")
    ids = enum_values((root / "include/constants/species.h").read_text(encoding="utf-8"), "SPECIES_")
    shared = (root / "src/data/pokemon/species_info/shared_dex_text.h").read_text(encoding="utf-8")
    if cpp:
        result = subprocess.run(shlex.split(cpp) + ["-I", "src"], cwd=root,
                                input='#include "global.h"\n#include "data/pokemon/species_info.h"\n',
                                text=True, capture_output=True, encoding="utf-8")
        if result.returncode:
            raise ValueError(result.stderr)
        inputs = [(Path("src/data/pokemon/species_info.h"), strip_c_comments(result.stdout))]
        shared = result.stdout
    else:
        inputs = [(path.relative_to(root), strip_c_comments(path.read_text(encoding="utf-8")))
                  for path in sorted((root / "src/data/pokemon/species_info").glob("gen_*_families.h"))]
    strings = {m[1]: literal_contents(m[2]) for m in re.finditer(
        r"\b(\w+)\s*\[\s*\]\s*=\s*_\(\s*(" + SEQUENCE + r")\s*\)", shared)}
    records = []
    for path, text in inputs:
        blocks = list(re.finditer(r"^\s*\[(SPECIES_\w+)\]\s*=\s*\{", text, re.M))
        for i, match in enumerate(blocks):
            body = text[match.end():blocks[i + 1].start() if i + 1 < len(blocks) else len(text)]
            national = re.search(r"\.natDexNum\s*=\s*(NATIONAL_DEX_\w+)", body)
            name = re.search(r"\.speciesName\s*=\s*_\(\s*(" + SEQUENCE + r")\s*\)", body)
            desc = re.search(r"\.description\s*=\s*(?:COMPOUND_STRING\(\s*(" + SEQUENCE + r")\s*\)|(\w+))", body)
            if not national or not name or match[1] not in ids or ".description" not in body:
                continue
            number = dex[national[1]]
            if number == 0:
                continue
            canonical = "SPECIES_" + national[1].removeprefix("NATIONAL_DEX_")
            records.append(dict(species=match[1], dex=number,
                                name=plain(re.sub(r"\{[^}]+\}", "", literal_contents(name[1]))),
                                kana=(literal_contents(desc[1]) if desc[1] else strings.get(desc[2], "")) if desc else "",
                                fallback=bool(desc and desc[2] == "gFallbackPokedexText"),
                                base=ids.get(canonical) == ids[match[1]],
                                source=path.as_posix()))
    return records


def resolve_form(record, records, candidates, labels):
    if record["base"]:
        return 0, "national-dex canonical species alias"
    symbol = record["species"]
    names = set()
    if "_MEGA" in symbol:
        suffix = re.search(r"_MEGA(?:_([XYZ]))?$", symbol)
        if suffix:
            names.add("メガ" + record["name"] + (suffix[1] or ""))
    if symbol.endswith("_ALOLA"):
        names.add("アローラのすがた")
    if symbol.endswith(("_GALAR", "_GALAR_STANDARD")):
        names.add("ガラルのすがた")
    if symbol.endswith("_HISUI"):
        names.add("ヒスイのすがた")
    if symbol.endswith("_GMAX"):
        names.add("キョダイマックス")
    forms = {form for form, label in labels.get(record["dex"], []) if label in names}
    if len(forms) == 1:
        return forms.pop(), "matching corpus national-dex/form display label"
    exact = {form for (dex, form), rows in candidates.items() if dex == record["dex"]
             for row in rows if record["kana"] and fold(record["kana"]) == fold(row["kana"])}
    if len(exact) == 1 and not record["fallback"]:
        return exact.pop(), "same-national-dex existing kana description matches one corpus form"
    # Only inherit generic text when there is no distinct description for any
    # other form of that species. Never assign a regional form base-form prose.
    relevant = [row for (dex, _), rows in candidates.items() if dex == record["dex"] for row in rows]
    rank = min((row["rank"] for row in relevant), default=None)
    latest = [row for row in relevant if row["rank"] == rank and row["version"] == "A"]
    generic = len({plain(row["kanji"]).replace(" ", "") for row in latest}) == 1
    if generic and not record["fallback"]:
        base = next((r for r in records if r["base"] and r["dex"] == record["dex"]), None)
        if base and record["kana"] and fold(base["kana"]) == fold(record["kana"]):
            return 0, "shared existing description; latest corpus form prose is identical"
        if symbol.startswith("SPECIES_SILVALLY_"):
            return 0, "Silvally types share identical latest corpus prose"
    return None, "unresolved form; preserve existing kana"


def font_text(row, atlas, charmap):
    substitutions = []
    def ruby(match):
        body, reading = match.groups()
        if any(is_kanji(c) and c not in atlas.index for c in body):
            substitutions.append(dict(kanji=body, kana=reading))
            return reading
        return body
    text = plain(RUBY.sub(ruby, row["kanji"]))
    unknown = sorted({c for c in text if c not in charmap.characters and not (is_kanji(c) and c in atlas.index)})
    if unknown:
        raise ValueError("unsupported glyphs: " + "".join(unknown))
    if any(c in text for c in "[]{}\\\"$"):
        raise ValueError("unsupported text syntax/control")
    return text, substitutions


def glyph_width(char):
    return 12 if is_kanji(char) else (8 if char.isascii() and char.isalpha() else 7)


def wrap(text):
    lines, line, width = [], "", 0
    no_start = set("。、!?！？)]」』ぁぃぅぇぉっゃゅょァィゥェォッャュョー")
    for char in text:
        amount = glyph_width(char)
        if line and width + amount > MAX_WIDTH:
            if char in no_start:
                # Carry one preceding glyph too, rather than start with punctuation.
                last, line = line[-1], line[:-1]
                lines.append(line.rstrip())
                line, width = last, glyph_width(last)
            else:
                lines.append(line.rstrip())
                line, width = "", 0
        if char != " " or line:
            line += char
            width += amount
    if line:
        lines.append(line.rstrip())
    if len(lines) > MAX_LINES:
        raise ValueError(f"description needs {len(lines)} lines, limit {MAX_LINES}")
    return r"\n".join(lines)


def prepare(root, cpp):
    candidates, labels, hashes = catalog(root)
    records, atlas, charmap = species_records(root, cpp), Atlas(root), Charmap(root / "charmap.txt")
    entries, skipped = [], []
    for record in records:
        form, evidence = resolve_form(record, records, candidates, labels)
        choices = candidates.get((record["dex"], form), []) if form is not None else []
        if not choices:
            skipped.append(dict(species=record["species"], reason=evidence if form is None else "no corpus description"))
            continue
        row = choices[0]
        try:
            text, substitutions = font_text(row, atlas, charmap)
            text = wrap(text)
        except ValueError as error:
            skipped.append(dict(species=record["species"], game=row["game"], qid=row["qid"], reason=str(error)))
            continue
        entries.append(dict(species=record["species"], national_dex=record["dex"], form=form,
                            game=row["game"], qid=row["qid"], corpus_line=row["line"],
                            source_sha256=digest(row["kanji"].encode()), text_sha256=digest(text.encode()),
                            mapping=evidence, reading_substitutions=substitutions))
    result = dict(schema=SCHEMA, revision=REVISION, priority=[game for game, _ in PACKS],
                  files=hashes, width=MAX_WIDTH, lines=MAX_LINES, entries=entries, kana_fallback=skipped)
    write_if_changed(root / MANIFEST, (json.dumps(result, ensure_ascii=False, indent=2) + "\n").encode())
    print(json.dumps(dict(selected=len(entries), kana_fallback=len(skipped),
                          sources=dict(Counter(e["game"] for e in entries))), ensure_ascii=False))


def configuration(root, cpp):
    result = subprocess.run(shlex.split(cpp), cwd=root, input='#include "global.h"\nPOKEDEX_STYLE_VALUE P_POKEDEX_TEXT_STYLE\n',
                            text=True, capture_output=True, encoding="utf-8")
    if result.returncode:
        raise ValueError(result.stderr)
    match = re.search(r"^POKEDEX_STYLE_VALUE\s+([01])\s*$", result.stdout, re.M)
    if not match:
        raise ValueError("P_POKEDEX_TEXT_STYLE must expand to POKEDEX_TEXT_KANA or POKEDEX_TEXT_KANJI")
    return int(match[1])


def generate(root, style, stamp=None):
    if style not in (0, 1):
        raise ValueError("P_POKEDEX_TEXT_STYLE must be 0 or 1")
    output = ["// Generated by tools/pokedex_kanji.py. Do not edit.",
              f"#if P_POKEDEX_TEXT_STYLE != {style}", '#error "Stale Pokedex text selection; run make"', "#endif"]
    report = dict(style="kanji" if style else "kana", selected=0)
    if style:
        manifest = json.loads((root / MANIFEST).read_text(encoding="utf-8"))
        if manifest["schema"] != SCHEMA or manifest["revision"] != REVISION:
            raise ValueError("unsupported Pokedex corpus manifest")
        rows, _, _ = catalog(root, verify=manifest["files"])
        by_qid = {row["qid"]: row for values in rows.values() for row in values}
        atlas, charmap = Atlas(root), Charmap(root / "charmap.txt")
        output.append("static const u8 *const sPokedexKanjiDescriptions[NUM_SPECIES] =\n{")
        for entry in manifest["entries"]:
            row = by_qid[entry["qid"]]
            text, substitutions = font_text(row, atlas, charmap)
            text = wrap(text)
            if (digest(row["kanji"].encode()) != entry["source_sha256"] or digest(text.encode()) != entry["text_sha256"]
                    or substitutions != entry["reading_substitutions"]):
                raise ValueError(f"Pokedex corpus content changed: {entry['species']}; review the manifest")
            output.append(f'    [{entry["species"]}] = COMPOUND_STRING("{{KANJI}}{text}"),')
        output.append("};")
        report.update(selected=len(manifest["entries"]), kana_fallback=len(manifest["kana_fallback"]))
    write_if_changed(root / HEADER, ("\n".join(output) + "\n").encode())
    write_if_changed(root / REPORT, (json.dumps(report, indent=2) + "\n").encode())
    if stamp:
        stamp.parent.mkdir(parents=True, exist_ok=True)
        stamp.write_text("ok\n", encoding="ascii")
    print("Pokedex text:", json.dumps(report))


def fetch(root):
    manifest = json.loads((root / MANIFEST).read_text(encoding="utf-8"))
    if manifest["revision"] != REVISION or manifest["schema"] != SCHEMA:
        raise ValueError("unsupported Pokedex corpus manifest")
    for game, kind in PACKS:
        for language in ("qid", "ja", "ja-Hrkt"):
            if language == "ja-Hrkt" and game == "ScarletViolet":
                continue
            path = root / CACHE / game / f"{language}_{kind}.txt"
            url = f"https://raw.githubusercontent.com/abcboy101/poke-corpus/{REVISION}/corpus/{game}/{path.name}"
            with urllib.request.urlopen(url, timeout=60) as response:
                data = response.read()
            relative = f"{game}/{path.name}"
            if digest(data) != manifest["files"].get(relative):
                raise ValueError(f"download hash mismatch: {relative}; existing cache was not replaced")
            write_if_changed(path, data)
        print("Fetched", game)


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("command", choices=("fetch", "prepare", "generate"))
    parser.add_argument("--root", type=Path, default=ROOT)
    parser.add_argument("--cpp", default="arm-none-eabi-cpp -P -I include -DMODERN=1 -x c -")
    parser.add_argument("--stamp", type=Path)
    args = parser.parse_args()
    try:
        if args.command == "fetch":
            fetch(args.root)
        elif args.command == "prepare":
            prepare(args.root, args.cpp)
        else:
            generate(args.root, configuration(args.root, args.cpp), args.stamp)
    except (OSError, ValueError, KeyError) as error:
        parser.exit(1, f"error: {error}\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
