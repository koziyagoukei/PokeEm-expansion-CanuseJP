#!/usr/bin/env python3
"""Measure a hypothetical kanji rewrite using the real charmap preprocessor.

This reports a sample, not a translation or a whole-ROM prediction.
"""
import json
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "src/data/abilities.h"
REPLACEMENTS = (
    ("じょうたいいじょう", "状態異常"),
    ("こうげき", "攻撃"), ("ぼうぎょ", "防御"), ("とくせい", "特性"),
    ("めいちゅう", "命中"), ("かいふく", "回復"), ("あいて", "相手"),
    ("すばやさ", "速さ"), ("ぶつり", "物理"), ("へんか", "変化"),
    ("わざ", "技"), ("ちから", "力"), ("たいせん", "対戦"),
)


def main():
    source = SOURCE.read_text(encoding="utf8")
    lines = re.findall(r'\.description\s*=\s*COMPOUND_STRING\("([^"\\{}]*)"\)', source)
    pairs = []
    for text in lines:
        converted = text
        for old, new in REPLACEMENTS:
            converted = converted.replace(old, new)
        if converted != text:
            pairs.append((text, converted))
    if not pairs:
        raise RuntimeError("No sample descriptions found")
    source_c = "\n".join('const unsigned char s%d[] = _("%s");' % (i, text)
                         for i, pair in enumerate(pairs) for text in pair)
    result = subprocess.run([str(ROOT / "tools/preproc/preproc"), "-i", "sample.c", "charmap.txt"],
                            cwd=ROOT, input=source_c, text=True, capture_output=True, check=True)
    arrays = re.findall(r'\{\s*((?:0x[0-9a-fA-F]{2}\s*,?\s*)+)\}', result.stdout)
    if len(arrays) != len(pairs) * 2:
        raise RuntimeError("Unexpected preprocessor output")
    rows = []
    for i, (old, new) in enumerate(pairs):
        original, changed = ([int(x, 16) for x in re.findall(r'0x[0-9a-fA-F]{2}', arrays[j])]
                             for j in (i * 2, i * 2 + 1))
        if original[-1] != 0xFF or changed[-1] != 0xFF:
            raise ValueError("Missing EOS")
        kanji = sum(changed[pos:pos + 2] == [0xFC, 0x1F] for pos in range(len(changed) - 1))
        rows.append({"original_bytes_including_eos": len(original),
                     "fc3_bytes_including_eos": len(changed), "kanji_count": kanji,
                     "hypothetical_2byte": len(changed) - kanji,
                     "hypothetical_4byte": len(changed) + kanji})
    keys = ("original_bytes_including_eos", "fc3_bytes_including_eos", "kanji_count",
            "hypothetical_2byte", "hypothetical_4byte")
    print(json.dumps({"source": str(SOURCE.relative_to(ROOT)), "matched_descriptions": len(rows),
                      "totals": {key: sum(row[key] for row in rows) for key in keys},
                      "scope": "hypothetical substring substitutions; not an applied translation"},
                     ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
