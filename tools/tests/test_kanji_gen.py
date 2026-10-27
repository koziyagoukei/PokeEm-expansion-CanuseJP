"""Kanji detection, ledger and glyph generation (tools/kanji/kanji_gen.py)."""

from pathlib import Path
import os
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/kanji"))

import kanji_gen as gen  # noqa: E402

PROTOTYPE = "攻撃防御特性技力速命中状態異常回復対戦相手物理変化"


def glyph_bytes(data, slot):
    base = 512 * (slot >> 3) + 32 * (slot & 7)
    return b"".join(data[base + 256 * (t >> 1) + 16 * (t & 1):][:16] for t in range(4))


class Codes(unittest.TestCase):
    def test_bank_layout(self):
        self.assertEqual(gen.encode(1), bytes.fromhex("FC 1F 01"))
        self.assertEqual(gen.encode(246), bytes.fromhex("FC 1F F6"))
        self.assertEqual(gen.encode(247), bytes.fromhex("FC 20 01"))
        self.assertEqual(gen.encode(gen.MAX_KANJI), bytes.fromhex("FC 27 F6"))
        for n in (1, 246, 247, 1000, gen.MAX_KANJI):
            self.assertEqual(gen.decode(gen.encode(n)), n)
        self.assertGreaterEqual(gen.MAX_KANJI, 2000)
        for bad in (0, gen.MAX_KANJI + 1):
            with self.assertRaises(gen.KanjiError):
                gen.encode(bad)
        for bad in ("FC 1F 00", "FC 1F F7", "FC 1F FF", "FC 28 01", "FC 1E 01"):
            with self.assertRaises(gen.KanjiError):
                gen.decode(bytes.fromhex(bad))


class Generator(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        for name in ("charmap.txt", "charmap_kanji.txt", gen.ATLAS_PNG, gen.ATLAS_TXT):
            (self.root / name).parent.mkdir(parents=True, exist_ok=True)
            shutil.copy(ROOT / name, self.root / name)
        self.source("src/prototype.c", "static const u8 sText[] = _(\"{KANJI}%s\");\n" % PROTOTYPE)

    def tearDown(self):
        self.tmp.cleanup()

    def source(self, name, text):
        path = self.root / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")

    def run_update(self):
        return gen.update(self.root)

    def ledger(self):
        return [(c, n) for c, n, _ in gen.read_ledger(self.root / gen.LEDGER)]

    def output(self, name):
        return (self.root / gen.OUT_DIR / name).read_bytes()

    def slots(self):
        data = self.output(gen.SLOTS_BIN)
        return struct.unpack("<%dH" % (len(data) // 2), data)

    def test_prototype_codes_and_pixels_kept(self):
        before = (self.root / gen.LEDGER).read_bytes()
        self.run_update()
        self.assertEqual((self.root / gen.LEDGER).read_bytes(), before)
        self.assertEqual(self.ledger(), [(c, i) for i, c in enumerate(PROTOTYPE, 1)])
        # The former prototype sheet, converted the way gbagfx does it.
        _, _, rows = gen.read_png(ROOT / "graphics/fonts/kanji_koden12.png")
        cells = [[row[(i % 16) * 16:(i % 16) * 16 + 16] for row in rows[(i // 16) * 16:(i // 16) * 16 + 16]]
                 for i in range(26)]
        reference = gen.fwjpnfont(cells)
        glyphs, slots = self.output(gen.GLYPHS_BIN), self.slots()
        for n in range(1, 26):
            self.assertEqual(glyph_bytes(glyphs, slots[n]), glyph_bytes(reference, n), PROTOTYPE[n - 1])

    def test_new_kanji_appended_in_code_point_order(self):
        self.source("src/b.c", 'const u8 a[] = _("今日は良い天気ですね。");\n')
        self.source("data/text/a.inc", 'Text_A::\n\t.string "天気$"\n')
        report = self.run_update()
        added = sorted("今日良天気", key=ord)
        self.assertEqual(self.ledger()[25:], [(c, 26 + i) for i, c in enumerate(added)])
        self.assertIn("added: " + "".join(added), report)
        self.assertEqual(self.slots()[26:], tuple(range(26, 31)))
        # A second kanji text does not change existing codes.
        self.source("src/c.c", 'const u8 b[] = COMPOUND_STRING("一");\n')
        self.run_update()
        self.assertEqual(self.ledger()[25:30], [(c, 26 + i) for i, c in enumerate(added)])
        self.assertEqual(self.ledger()[30], ("一", 31))

    def test_scan_order_does_not_matter(self):
        self.source("src/z.c", 'const u8 a[] = _("天");\n')
        self.source("src/a.c", 'const u8 b[] = _("気");\n')
        self.run_update()
        first = self.ledger()
        shutil.rmtree(self.root / gen.OUT_DIR)
        shutil.copy(ROOT / gen.LEDGER, self.root / gen.LEDGER)
        os.rename(self.root / "src/z.c", self.root / "src/0.c")
        os.rename(self.root / "src/a.c", self.root / "src/zz.c")
        self.run_update()
        self.assertEqual(self.ledger(), first)

    def test_repeated_kanji_registered_once(self):
        self.source("src/a.c", 'const u8 a[] = _("天天天");\nconst u8 b[] = _("天");\n')
        self.source("data/a.inc", '\t.string "天$"\n')
        self.run_update()
        self.assertEqual([c for c, _ in self.ledger()].count("天"), 1)

    def test_removed_kanji_keeps_code_and_leaves_rom(self):
        self.source("src/a.c", 'const u8 a[] = _("天気");\n')
        self.run_update()
        size = len(self.output(gen.GLYPHS_BIN))
        self.source("src/a.c", 'const u8 a[] = _("気");\n')
        self.source("src/b.c", 'const u8 b[] = _("空");\n')
        self.run_update()
        codes = dict(self.ledger())
        self.assertEqual((codes["天"], codes["気"], codes["空"]), (26, 27, 28))
        slots = self.slots()
        self.assertEqual(slots[26], 0)
        self.assertNotEqual(slots[27], 0)
        self.assertEqual(len(self.output(gen.GLYPHS_BIN)), size)
        self.assertEqual(len(set(s for s in slots if s)), 27)

    def test_comments_identifiers_and_plain_literals_are_ignored(self):
        self.source("src/a.c", "// 天\n/* 気\n 空 */\nint 変数;\n"
                    'void f(void) { DebugPrintf("海"); }\n#include "山.h"\n')
        self.source("data/a.inc", "@ 雨\n\t.byte 0 @ 雪\n")
        self.run_update()
        self.assertEqual(len(self.ledger()), 25)

    def test_forwarding_macros_and_braille(self):
        self.source("include/m.h", "#define LIMITED(str, n) COMPOUND_STRING(str)\n"
                    "#define MY_TEXT(str) LIMITED(str, 10)\n")
        self.source("src/a.c", 'const u8 *a = MY_TEXT("海");\nconst u8 b[] = __("山");\n'
                    'const u8 *c = J_COMPOUND_STRING("川");\n')
        self.source("data/b.inc", 'Label: .braille "空"\n')
        self.run_update()
        self.assertEqual({c for c, _ in self.ledger()[25:]}, set("海山川空"))

    def test_test_only_kanji_get_a_code_but_no_glyph(self):
        self.source("test/t.c", 'static const u8 sText[] = _("試験");\n')
        self.run_update()
        codes = dict(self.ledger())
        self.assertIn("試", codes)
        self.assertEqual(self.slots()[codes["試"]], 0)

    def test_missing_glyph_is_an_error(self):
        atlas = set(gen.Atlas(self.root).index)
        absent = next(chr(c) for c in range(0x4E00, 0xA000) if chr(c) not in atlas)
        self.source("src/a.c", '\nconst u8 a[] = _("%s");\n' % absent)
        with self.assertRaises(gen.KanjiError) as error:
            self.run_update()
        self.assertIn("src/a.c:2", str(error.exception))
        self.assertEqual(len(self.ledger()), 25)

    def test_ledger_collisions_are_errors(self):
        with open(self.root / gen.LEDGER, "a", encoding="utf-8") as f:
            f.write("'海' = FC 1F 01\n")
        with self.assertRaises(gen.KanjiError) as error:
            self.run_update()
        self.assertIn("collides", str(error.exception))
        shutil.copy(ROOT / gen.LEDGER, self.root / gen.LEDGER)
        with open(self.root / gen.LEDGER, "a", encoding="utf-8") as f:
            f.write("'円' = FC 1F 1A\n")
        with self.assertRaises(gen.KanjiError) as error:
            self.run_update()
        self.assertIn("also defined in charmap.txt", str(error.exception))

    def test_unchanged_outputs_are_not_rewritten(self):
        self.run_update()
        paths = [self.root / gen.OUT_DIR / n for n in (gen.GLYPHS_BIN, gen.SLOTS_BIN)]
        paths.append(self.root / gen.LEDGER)
        stamps = [p.stat().st_mtime_ns for p in paths]
        contents = [p.read_bytes() for p in paths]
        self.run_update()
        self.assertEqual([p.stat().st_mtime_ns for p in paths], stamps)
        self.assertEqual([p.read_bytes() for p in paths], contents)


class SaveLinkChecks(unittest.TestCase):
    def setUp(self):
        import save_link_checks
        self.checks = save_link_checks
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        for name in ("charmap.txt", "charmap_kanji.txt") + save_link_checks.LIMIT_HEADERS:
            (self.root / name).parent.mkdir(parents=True, exist_ok=True)
            shutil.copy(ROOT / name, self.root / name)
        self.entries = gen.read_ledger(self.root / gen.LEDGER) + [("天", 26, None)]

    def tearDown(self):
        self.tmp.cleanup()

    def problems(self, path, text, rule):
        (self.root / path).parent.mkdir(parents=True, exist_ok=True)
        (self.root / path).write_text(text, encoding="utf-8")
        return self.checks.check_save_link_strings(self.root, self.entries, [rule])

    def species_rule(self):
        return self.checks.Rule(("src/species.h",), "POKEMON_NAME_LENGTH", "species", fields=("speciesName",))

    def test_real_tree_passes(self):
        entries = gen.read_ledger(ROOT / gen.LEDGER)
        self.assertEqual(self.checks.check_save_link_strings(ROOT, entries), [])

    def test_kanji_and_length(self):
        rule = self.species_rule()
        self.assertEqual(self.problems("src/species.h", '.speciesName = _("{JPN}ピカチュウ"),', rule), [])
        # 円 is an ordinary 1-byte glyph of the base charmap, not a kanji code.
        self.assertEqual(self.problems("src/species.h", '.speciesName = _("{JPN}円"),', rule), [])
        found = self.problems("src/species.h", '\n.speciesName = _("{JPN}天"),', rule)
        self.assertEqual(len(found), 1)
        self.assertIn("src/species.h:2", found[0])
        self.assertIn("kanji", found[0])
        found = self.problems("src/species.h", '.speciesName = _("{JPN}ピカチュウピカチュウピ"),', rule)
        self.assertIn("is 13 bytes, limit 12", found[0])

    def test_symbols_macros_and_missing_sources(self):
        rule = self.checks.Rule(("src/names.c",), "PLAYER_NAME_LENGTH", "presets", symbols=r"sNames")
        text = 'static const u8 *const sNames[] = {\n COMPOUND_STRING("{JPN}ユウキ"),\n COMPOUND_STRING("天"),\n};\n'
        found = self.problems("src/names.c", text, rule)
        self.assertEqual(len(found), 1)
        self.assertIn("src/names.c:3", found[0])
        rule = self.checks.Rule(("src/npc.inc",), "PLAYER_NAME_LENGTH", "npc", macro=("NPC", 2, "{JPN}"))
        found = self.problems("src/npc.inc", 'NPC(A, "あ", "あいうえおか", 1)\n', rule)
        self.assertIn("is 8 bytes, limit 7", found[0])
        rule = self.checks.Rule(("src/none.h",), 7, "gone", fields=("trainerName",))
        self.assertIn("found no strings", self.problems("src/other.h", "", rule)[0])


@unittest.skipUnless((ROOT / "tools/preproc/preproc").exists(), "preproc is not built")
class PreprocInclude(unittest.TestCase):
    def test_ledger_codes_reach_encoded_strings(self):
        # preproc -i reads the source from stdin, as in the Makefile.
        result = subprocess.run([str(ROOT / "tools/preproc/preproc"), "-i", "a.c", "charmap.txt"],
                                cwd=ROOT, capture_output=True, text=True, check=True,
                                input='const u8 a[] = _("{KANJI}化攻");\n', encoding="utf-8")
        self.assertIn("0xFC, 0x1E, 0xFC, 0x1F, 0x19, 0xFC, 0x1F, 0x01, 0xFF", result.stdout)


if __name__ == "__main__":
    unittest.main()
