#!/usr/bin/env python3
"""Export the current contest combinations and tested species examples."""

import argparse
from collections import defaultdict
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]


def strip_comments(text):
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    return re.sub(r"//[^\r\n]*", "", text)


def evaluate_enum(text, marker):
    # Combo IDs are a literal, sequential enum. Reject new expression forms
    # instead of importing Frontierhack's unrelated link fingerprint tooling.
    clean = strip_comments(text)
    for body in re.findall(r"\benum\s*\{([^}]+)\}", clean, re.DOTALL):
        if not re.search(r"\b" + re.escape(marker) + r"\b", body):
            continue
        values = {}
        current = -1
        for entry in body.split(","):
            entry = entry.strip()
            if not entry:
                continue
            match = re.fullmatch(r"(COMBO_STARTER_\w+)(?:\s*=\s*(0x[0-9A-Fa-f]+|[0-9]+))?", entry)
            if match is None:
                raise ValueError(f"Unsupported combo enum entry: {entry}")
            current = current + 1 if match[2] is None else int(match[2], 0)
            values[match[1]] = current
        return values
    raise ValueError(f"Combo enum not found: {marker}")


def read_moves(root):
    source = (root / "src/data/moves_info.h").read_text(encoding="utf-8")
    # Move entries are top-level designated initializers; nested effects are
    # deliberately not parsed as entries. Reject nonliteral combo fields.
    entries = list(re.finditer(r"^    \[(MOVE_\w+)\] =", source, re.M))
    moves = {}
    for index, entry in enumerate(entries):
        end = entries[index + 1].start() if index + 1 < len(entries) else len(source)
        block = strip_comments(source[entry.end():end])
        name = re.search(r'\.name = COMPOUND_STRING\("([^"]*)"\)', block)
        if name is None:
            raise ValueError(f"Missing move name: {entry[1]}")
        starter = re.search(r"\.contestComboStarterId = ([^,]+),", block)
        followers = re.search(r"\.contestComboMoves = \{([^}]+)\}", block)
        starter = starter[1].strip() if starter else "0"
        followers = [s.strip() for s in followers[1].split(",")] if followers else []
        for value in [starter, *followers]:
            if value != "0" and not re.fullmatch(r"COMBO_STARTER_\w+", value):
                raise ValueError(f"Unsupported combo field: {entry[1]}: {value}")
        moves[entry[1]] = {
            "name": re.sub(r"\{(?:JPN|ENG|AUTO)\}", "", name[1]),
            "starter": starter,
            "followers": followers,
        }
    return moves


def combinations(root, moves):
    constants = (root / "include/constants/contest.h").read_text(encoding="utf-8")
    ids = evaluate_enum(constants, "COMBO_STARTER_POUND")
    ids["0"] = 0
    if max(ids.values()) > 255:
        raise ValueError("Combo starter no longer fits the u8 field")
    limit_text = (root / "include/contest_effect.h").read_text(encoding="utf-8")
    limit = int(re.search(r"#define MAX_COMBO_MOVES (\d+)", limit_text)[1])
    starters = defaultdict(list)
    for move, data in moves.items():
        if ids[data["starter"]] != 0:
            starters[ids[data["starter"]]].append(move)
    pairs = set()
    for move, data in moves.items():
        if len(data["followers"]) > limit:
            raise ValueError(f"Too many combo starters for {move}")
        followers = [ids[value] for value in data["followers"] if ids[value] != 0]
        if len(followers) != len(set(followers)):
            raise ValueError(f"Duplicate starter in {move}")
        for starter in followers:
            if not starters[starter]:
                raise ValueError(f"Unreachable combo starter: {move}: {starter}")
            for first in starters[starter]:
                pairs.add((first, move))
    return pairs


def species_names(root):
    names = {}
    for path in (root / "src/data/pokemon/species_info").glob("*.h"):
        source = path.read_text(encoding="utf-8")
        entries = list(re.finditer(r"^    \[(SPECIES_\w+)\]\s*=", source, re.M))
        for i, entry in enumerate(entries):
            end = entries[i + 1].start() if i + 1 < len(entries) else len(source)
            name = re.search(r'\.speciesName\s*=\s*_\("(?:\{JPN\})?([^"{]*)"\)', source[entry.end():end])
            if name:
                names[entry[1]] = name[1]
    aliases = (root / "include/constants/species.h").read_text(encoding="utf-8")
    for alias, target in re.findall(r"(SPECIES_\w+) = (SPECIES_\w+)", aliases):
        if target in names:
            names[alias] = names[target]
    # These names are provided by shared initializer macros.
    names["SPECIES_ARCEUS_NORMAL"] = "アルセウス"
    names["SPECIES_GENESECT"] = "ゲノセクト"
    forms = {
        "RAICHU_ALOLA": "アローラ", "WEEZING_GALAR": "ガラル",
        "NINETALES_ALOLA": "アローラ", "MOLTRES_GALAR": "ガラル",
        "INDEEDEE_M": "オス", "GIRATINA_ALTERED": "アナザー",
        "KYUREM_WHITE": "ホワイト", "KYUREM_BLACK": "ブラック",
        "ZYGARDE_50": "50%", "NECROZMA_DUSK_MANE": "たそがれのたてがみ",
        "NECROZMA_DAWN_WINGS": "あかつきのつばさ",
        "ZACIAN_CROWNED": "けんのおう", "ZAMAZENTA_CROWNED": "たてのおう",
        "CALYREX_ICE": "はくばじょう", "CALYREX_SHADOW": "こくばじょう",
        "HOOPA_CONFINED": "いましめられし", "HOOPA_UNBOUND": "ときはなたれし",
    }
    for species, form in forms.items():
        key = "SPECIES_" + species
        if key in names:
            names[key] += f"（{form}）"
    return names


def render(root):
    moves = read_moves(root)
    pairs = combinations(root, moves)
    names = species_names(root)
    examples = (root / "test/contest_combo_examples.inc").read_text(encoding="utf-8")
    lines = [
        "# コンテスト・コンビネーション技一覧", "",
        "`python3 tools/export_contest_combos.py` で技データから生成した一覧です。", "",
        f"全 **{len(pairs)}組**。順番は「前のターン → 次のターン」です。",
        "同じポケモンが順に使い、審査員の注目が維持されている場合に、既存ルールのコンボ加点を受けます。",
        "逆順は、逆順の組み合わせも一覧にある場合のみ成立します。",
        "Z・ダイマックスの表示技名ではなく、選択した元技で判定します。", "",
        "コンボ定義の追加だけでは、通常戦闘の威力・効果や習得技、セーブ・通信データの配列サイズは変更しません。",
        "通常・リンクコンテストとも同じコンボ定義を使います。通信相手も同じ版のROMを使用してください。", "",
        "## 種族別の使用例", "",
        "以下は、現行のゲーム内データと移植元の過去作・配布技データに基づく例です（王形態の専用技2件は戦闘時の技置換を確認）。",
        "ギラティナのおそろしいかぜ、ゼルネアスのマジカルシャイン、ゲノセクトのギアチェンジは移植元の追加習得技に依存します。",
        "今回、そのプレイヤー向け習得手段は移植していません。コンボ定義は有効ですが、現行の思い出しだけではこの3組を用意できません。",
        "一部は既存コンボです。技自体の組み合わせなので、両方を覚える他種族でも使用できます。",
        "伝説・幻は固有技や代表的な技を優先しています。専用の通常技がないミュウ・セレビィ・フィオネには代表技を採用しました。",
        "コスモッグ・コスモウム・通常バドレックス・メルタン等に、専用技を新設したり他形態の専用技を与えたりはしていません。",
        "同じ種族のフォームでは習得できる技に従います。合体固有技は対応する合体形態用です。", "",
        "※きょじゅうざん・きょじゅうだんはコンボ定義を追加しましたが、現在の王形態・技置換は戦闘開始時専用です。",
        "コンテスト入場時の王形態変化や技置換は今回追加していないため、通常の入場手順ではこの2件は利用できません。", "",
        "| 使用例 | 1ターン目 | 2ターン目 |", "| --- | --- | --- |",
    ]
    for species, first, second in re.findall(r"COMBO_CASE\((\w+), (\w+), (\w+)\)", examples):
        first, second = "MOVE_" + first, "MOVE_" + second
        if (first, second) not in pairs:
            raise ValueError(f"Example is not a combo: {first} -> {second}")
        key = "SPECIES_" + species
        if key not in names:
            raise ValueError(f"Missing species display name: {key}")
        lines.append(f"| {names[key]} | {moves[first]['name']} | {moves[second]['name']} |")
    lines.extend(["", "## 全組み合わせ（開始技順）", "",
                  "同じ行の終了技はいずれか1つを選びます。", "",
                  "| 開始技 | 続ける技 |", "| --- | --- |"])
    by_first = defaultdict(list)
    for first, second in pairs:
        by_first[first].append(second)
    for first in sorted(by_first, key=lambda move: (moves[move]["name"], move)):
        next_names = [moves[m]["name"] for m in sorted(by_first[first], key=lambda m: (moves[m]["name"], m))]
        lines.append(f"| {moves[first]['name']} | {' / '.join(next_names)} |")
    return "\n".join(lines) + "\n"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    output = ROOT / "docs/contest_combinations.md"
    content = render(ROOT)
    if args.check:
        if not output.exists() or output.read_text(encoding="utf-8") != content:
            raise SystemExit("Contest combination catalog is out of date")
        print("Contest combination catalog is up to date")
    else:
        output.write_text(content, encoding="utf-8", newline="\n")
        print(f"Wrote {output}")


if __name__ == "__main__":
    main()
