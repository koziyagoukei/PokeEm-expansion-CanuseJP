#!/usr/bin/env python3
"""Inventory runtime text and report remaining English without changing sources."""

from __future__ import annotations

import argparse
from collections import Counter
import csv
import json
from pathlib import Path
import re

from check_jp_text import iter_source_files
from sync_bpej_text import LITERAL, SEQUENCE, literal_contents


ROOT = Path(__file__).resolve().parents[1]
JAPANESE = re.compile(r"[\u3040-\u30ff\u3400-\u9fff]")
WRAPPER = re.compile(r"\b(?:_|COMPOUND_STRING|COMPOUND_STRING_SIZE_LIMIT|ITEM_NAME|ITEM_PLURAL_NAME)\(\s*(" + SEQUENCE + r")")
WORDS = re.compile(r"[A-Za-z]{2,}")
ABBREVIATIONS = {"HP", "PP", "BP", "EXP", "ID", "No", "OK", "Lv", "LV", "TM", "HM",
                 "DNA", "UB", "EV", "IV", "RTC", "cm", "kg", "mm", "km", "m", "g",
                 "TV", "VS", "PC", "JP", "NPC", "FPS", "MAX", "MIN"}
LEGACY_FILES = {
    "src/trainer_tower_sets.c", "src/data/trade.h", "src/battle_controller_oak_old_man.c",
    "data/text/pokedex_rating.inc", "data/scripts/flavor_text.inc", "data/scripts/mystery_event_club.inc",
    "data/scripts/route23.inc", "data/scripts/aide.inc", "data/scripts/silphco_doors.inc",
    "data/scripts/seagallop.inc", "data/maps/MossdeepCity_GameCorner_1F/scripts.inc",
}
SCOPE_NAMES = {
    "needs_review": "通常画面・追加機能などの要確認候補",
    "developer": "開発・デバッグ用の名称/表示候補",
    "international_names": "国際通信向けの別言語名",
    "legacy": "FRLG/RS由来などBPEJとの対応を別途確認する内容",
    "alphabet_or_credits": "文字入力キー・クレジットなど",
}


def review_scope(row):
    file = row["file"]
    if "debug" in file or file in ("src/pokemon_sprite_visualizer.c", "src/field_weather.c", "src/data/battle_environment.h"):
        return "developer"
    if file == "src/data/battle_frontier/apprentice.h":
        return "international_names"
    if file in LEGACY_FILES:
        return "legacy"
    if file in ("src/data/credits.h", "src/naming_screen.c", "src/text_input_strings.c", "src/dodrio_berry_picking.c"):
        return "alphabet_or_credits"
    return "needs_review"


def mask_comments(text: str) -> str:
    pattern = LITERAL + r"|//[^\n]*|/\*.*?\*/"
    def replace(match):
        value = match[0]
        if value.startswith('"'):
            return value
        return "".join("\n" if ch == "\n" else " " for ch in value)
    return re.sub(pattern, replace, text, flags=re.S)


def classify(text: str):
    visible = re.sub(r"\{[^}]+\}|\\[nlpr]|\$", " ", text)
    visible = re.sub(r"\s+", " ", visible).strip()
    words = [word for word in WORDS.findall(visible) if word not in ABBREVIATIONS]
    japanese = bool(JAPANESE.search(visible))
    if words:
        status = "mixed" if japanese else "english"
    else:
        status = "japanese" if japanese else "technical_or_symbol"
    return status, visible, words


def inventory(root: Path):
    paths = set(iter_source_files(root))
    paths.update((root / "data").rglob("*.s"))
    for path in sorted(paths):
        original = path.read_text(encoding="utf-8")
        text = mask_comments(original)
        context = []
        if path.suffix in (".inc", ".s"):
            labels = list(re.finditer(r"^(\w+)::?[^\S\n]*(?:@[^\n]*)?$", text, re.M))
            context = [(m.start(), m[1]) for m in labels]
            for m in re.finditer(r"^\s*\.string\s+(" + LITERAL + r")", text, re.M):
                label = next((name for pos, name in reversed(context) if pos <= m.start()), "(anonymous)")
                value = literal_contents(m[1])
                status, visible, words = classify(value)
                yield {"file":path.relative_to(root).as_posix(), "line":text.count("\n",0,m.start())+1,
                       "label":label, "kind":"asm_fragment", "status":status, "text":value, "visible":visible, "english_words":words}
        definitions = [(m.start(), m[1] or m[2]) for m in re.finditer(
            r"\[\s*(\w+)\s*\]\s*=|\b(\w+)\s*\[[^\]\n]*\]\s*=", text)]
        for m in WRAPPER.finditer(text):
            label = next((name for pos,name in reversed(definitions) if pos < m.start()), "(inline)")
            field = re.search(r"\.(\w+)\s*=\s*$",text[max(0,m.start()-80):m.start()])
            if field:
                label += "." + field[1]
            value = literal_contents(m[1])
            status, visible, words = classify(value)
            yield {"file":path.relative_to(root).as_posix(), "line":text.count("\n",0,m.start())+1,
                   "label":label, "kind":"c_literal", "status":status, "text":value, "visible":visible, "english_words":words}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=ROOT)
    parser.add_argument("--output", type=Path, default=ROOT / "build/reports/jp_localization")
    args = parser.parse_args()
    rows = list(inventory(args.root))
    candidates = [row for row in rows if row["status"] in ("english", "mixed")]
    for row in candidates:
        row["review_scope"] = review_scope(row)
    counts = Counter(row["status"] for row in rows)
    by_file = Counter(row["file"] for row in candidates)
    args.output.mkdir(parents=True, exist_ok=True)
    (args.output / "inventory.json").write_text(json.dumps({"counts":dict(counts), "entries":rows}, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    with (args.output / "remaining_english.csv").open("w", encoding="utf-8-sig", newline="") as handle:
        writer = csv.DictWriter(handle, fieldnames=["file","line","label","kind","status","text","visible","english_words","review_scope"])
        writer.writeheader()
        writer.writerows(candidates)
    lines = ["# 日本語化の残件候補", "", "これは画面文言の候補一覧です。英語固有名詞、国際通信向けの別言語名、開発・デバッグ画面、未使用データも含みます。公式ROMとの対応や、実際に表示されるかを確認せず一括置換しないでください。", "", "## 件数", "", "Cの連続リテラルは1件、イベントの`.string`は1行を1件として数えています。ROM参照manifestのラベル件数とは単位が異なります。", "", "| 区分 | 件数 |", "|---|---:|"]
    lines.extend(f"| {name} | {count} |" for name,count in sorted(counts.items()))
    lines += ["", "## 確認区分", "", "以下はファイルと用途に基づく確認順の目安です。使用中/未使用の判定や翻訳済みの保証ではありません。`needs_review`にも公式名の英字略称が含まれます。", "", "| 確認区分 | 候補数 |", "|---|---:|"]
    scopes = Counter(row["review_scope"] for row in candidates)
    lines.extend(f"| {SCOPE_NAMES[name]} (`{name}`) | {count} |" for name,count in scopes.most_common())
    lines += ["", "## ファイル別", "", "| ファイル | 候補数 |", "|---|---:|"]
    lines.extend(f"| `{name}` | {count} |" for name,count in by_file.most_common())
    lines += ["", "## 詳細", ""]
    for row in candidates:
        lines.append(f"- `{row['file']}:{row['line']}` `{row['label']}` [{row['review_scope']}]: {row['visible']}")
    (args.output / "remaining_english.md").write_text("\n".join(lines)+"\n", encoding="utf-8")
    print("Runtime text inventory:", json.dumps(dict(counts),sort_keys=True))
    print(f"Remaining English candidates: {len(candidates)} in {len(by_file)} files; {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
