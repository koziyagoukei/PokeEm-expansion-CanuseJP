"""Kanji detection, ledger and glyph generation (tools/kanji/kanji_gen.py)."""

from pathlib import Path
import os
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

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
        for name in ("charmap.txt", gen.ATLAS_PNG, gen.ATLAS_TXT):
            (self.root / name).parent.mkdir(parents=True, exist_ok=True)
            shutil.copy(ROOT / name, self.root / name)
        self.seed_ledger = gen.ledger_text([(c, i, None) for i, c in enumerate(PROTOTYPE, 1)])
        (self.root / gen.LEDGER).write_text(self.seed_ledger, encoding="utf-8")
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
        (self.root / gen.LEDGER).write_text(self.seed_ledger, encoding="utf-8")
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
        self.assertNotIn("'天' =", self.output(gen.BUILD_CHARMAP).decode("utf-8"))
        self.assertIn("'気' =", self.output(gen.BUILD_CHARMAP).decode("utf-8"))
        self.assertEqual(len(self.output(gen.GLYPHS_BIN)), size)
        self.assertEqual(len(set(s for s in slots if s)), 27)

    def test_comments_identifiers_and_plain_literals_are_ignored(self):
        self.source("src/a.c", "// 天\n/* 気\n 空 */\nint 変数;\n"
                    'void f(void) { DebugPrintf("海"); }\n#include "山.h"\n')
        self.source("data/a.inc", "@ 雨\n\t.byte 0 @ 雪\n")
        self.run_update()
        self.assertEqual(len(self.ledger()), 25)

    def test_fake_calls_inside_strings_and_asm_comments_are_ignored(self):
        self.source("src/fake.c", 'const char *a = "_(\\\"海\\\")";\n'
                    '#define FAKE(x) "COMPOUND_STRING(x)"\nconst char *b = FAKE("山");\n')
        self.source("data/fake.inc", '/*\n.string "川"\n*/\n'
                    '.string "攻$" @ .string "空"\n')
        self.run_update()
        self.assertEqual(len(self.ledger()), 25)

    def test_safety_failure_does_not_change_ledger_or_outputs(self):
        self.run_update()
        before = (self.root / gen.LEDGER).read_bytes()
        output = self.output(gen.GLYPHS_BIN)
        self.source("src/new.c", 'const u8 a[] = _("天");\n')
        with self.assertRaisesRegex(gen.KanjiError, "unsafe"):
            gen.update(self.root, checks=lambda *_: ["unsafe save/link text"])
        self.assertEqual((self.root / gen.LEDGER).read_bytes(), before)
        self.assertEqual(self.output(gen.GLYPHS_BIN), output)

    def test_missing_or_changed_legacy_ledger_is_rejected(self):
        path = self.root / gen.LEDGER
        path.unlink()
        with self.assertRaisesRegex(gen.KanjiError, "persistent ID ledger"):
            self.run_update()
        path.write_text(self.seed_ledger, encoding="utf-8")
        path.write_text(path.read_text(encoding="utf-8").replace("FC 1F 01", "FC 20 01"), encoding="utf-8")
        with self.assertRaisesRegex(gen.KanjiError, "legacy glyph"):
            self.run_update()

    def test_deleted_source_and_missing_output_are_repaired(self):
        self.source("src/new.c", 'const u8 a[] = _("天");\n')
        self.run_update()
        code = dict(self.ledger())["天"]
        (self.root / "src/new.c").unlink()
        (self.root / gen.OUT_DIR / gen.SLOTS_BIN).unlink()
        self.run_update()
        self.assertEqual(self.slots()[code], 0)
        self.assertEqual(dict(self.ledger())["天"], code)

    def test_forwarding_macros_and_braille(self):
        self.source("include/m.h", "#define LIMITED(str, n) COMPOUND_STRING(str)\n"
                    "#define MY_TEXT(str) LIMITED(str, 10)\n")
        self.source("src/a.c", 'const u8 *a = MY_TEXT("海");\nconst u8 b[] = __("山");\n'
                    'const u8 *c = J_COMPOUND_STRING("川");\n')
        self.source("data/b.inc", 'Label: .braille "空"\n')
        self.run_update()
        self.assertEqual({c for c, _ in self.ledger()[25:]}, set("海山川空"))

    def test_object_macro_with_encoded_literal_is_detected(self):
        self.source("include/texts.h", '#define MY_MESSAGE _("天気")\n')
        self.source("src/use.c", 'const u8 text[] = MY_MESSAGE;\n')
        self.run_update()
        self.assertEqual({c for c, _ in self.ledger()[25:]}, set("天気"))

    def test_only_forwarded_macro_parameters_are_display_text(self):
        self.source("include/m.h", '#define LABEL(file, text) _(text)\n'
                    '#define ANOTHER(text, file) LABEL(file, text)\n')
        self.source("src/use.c", 'const u8 a[] = ANOTHER("天", "data/山.png");\n')
        self.run_update()
        self.assertEqual({c for c, _ in self.ledger()[25:]}, {"天"})

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

    def test_supplementary_kanji_reports_missing_glyph(self):
        self.source("src/extra.c", 'const u8 a[] = _("𠮷");\n')
        with self.assertRaisesRegex(gen.KanjiError, "U\\+20BB7"):
            self.run_update()

    def test_ledger_collisions_are_errors(self):
        with open(self.root / gen.LEDGER, "a", encoding="utf-8") as f:
            f.write("'海' = FC 1F 01\n")
        with self.assertRaises(gen.KanjiError) as error:
            self.run_update()
        self.assertIn("collides", str(error.exception))
        (self.root / gen.LEDGER).write_text(self.seed_ledger, encoding="utf-8")
        with open(self.root / gen.LEDGER, "a", encoding="utf-8") as f:
            f.write("'円' = FC 1F 1A\n")
        with self.assertRaises(gen.KanjiError) as error:
            self.run_update()
        self.assertIn("also defined in charmap.txt", str(error.exception))

    def test_upstream_control_code_collision_is_rejected(self):
        self.source("include/constants/characters.h", "#define EXT_CTRL_CODE_OTHER 0x20\n")
        with self.assertRaisesRegex(gen.KanjiError, "collides with the kanji bank range"):
            self.run_update()

    def test_capacity_failure_does_not_reassign_ids(self):
        base_chars, _ = gen.load_base_charmap(self.root)
        extra = sorted(set(gen.Atlas(self.root).chars) - set(PROTOTYPE) - set(base_chars), key=ord)
        entries = [(c, i, None) for i, c in enumerate(PROTOTYPE, 1)]
        entries += [(c, i, None) for i, c in enumerate(extra[:gen.MAX_KANJI - 25], 26)]
        path = self.root / gen.LEDGER
        path.write_text(gen.ledger_text(entries), encoding="utf-8")
        before = path.read_bytes()
        self.source("src/new.c", 'const u8 a[] = _("' + extra[gen.MAX_KANJI - 25] + '");\n')
        with self.assertRaisesRegex(gen.KanjiError, "capacity exceeded"):
            self.run_update()
        self.assertEqual(path.read_bytes(), before)

    def test_unchanged_outputs_are_not_rewritten(self):
        self.run_update()
        paths = [self.root / gen.OUT_DIR / n for n in (gen.GLYPHS_BIN, gen.SLOTS_BIN, gen.BUILD_CHARMAP)]
        paths.append(self.root / gen.LEDGER)
        stamps = [p.stat().st_mtime_ns for p in paths]
        contents = [p.read_bytes() for p in paths]
        self.run_update()
        self.assertEqual([p.stat().st_mtime_ns for p in paths], stamps)
        self.assertEqual([p.read_bytes() for p in paths], contents)

    def test_parallel_updates_keep_one_ledger_assignment(self):
        self.source("src/new.c", 'const u8 a[] = _("天気");\n')
        code = ("import sys; from pathlib import Path; "
                f"sys.path.insert(0, {str(ROOT / 'tools/kanji')!r}); "
                "import kanji_gen; kanji_gen.update(Path(sys.argv[1]))")
        jobs = [subprocess.Popen([sys.executable, "-c", code, str(self.root)],
                                 stdout=subprocess.PIPE, stderr=subprocess.PIPE) for _ in range(2)]
        for job in jobs:
            _, stderr = job.communicate(timeout=30)
            self.assertEqual(job.returncode, 0, stderr.decode())
        self.assertEqual(len(self.ledger()), 27)
        self.assertEqual(len({n for _, n in self.ledger()}), 27)

    def test_failed_atomic_replace_preserves_old_file(self):
        path = self.root / "old.dat"
        path.write_bytes(b"old")
        with mock.patch.object(gen.os, "replace", side_effect=OSError("test write failure")):
            with self.assertRaises(OSError):
                gen.write_if_changed(path, b"new")
        self.assertEqual(path.read_bytes(), b"old")
        self.assertEqual(list(path.parent.glob("old.dat.*")), [])

    @unittest.skipUnless((ROOT / "tools/preproc/preproc").exists(), "preproc is not built")
    def test_registered_but_unscanned_kanji_fails_encoding(self):
        self.source("src/new.c", 'const u8 a[] = _("天");\n')
        self.run_update()
        (self.root / "src/new.c").unlink()
        self.run_update()
        result = subprocess.run([str(ROOT / "tools/preproc/preproc"), "-i", "probe.c", gen.OUT_DIR + "/" + gen.BUILD_CHARMAP],
                                cwd=self.root, input='const u8 a[] = _("天");\n', text=True, capture_output=True)
        self.assertNotEqual(result.returncode, 0)


class SaveLinkChecks(unittest.TestCase):
    def setUp(self):
        import save_link_checks
        self.checks = save_link_checks
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        for name in ("charmap.txt",) + save_link_checks.LIMIT_HEADERS:
            (self.root / name).parent.mkdir(parents=True, exist_ok=True)
            shutil.copy(ROOT / name, self.root / name)
        self.entries = [(c, i, None) for i, c in enumerate(PROTOTYPE, 1)] + [("天", 26, None)]
        (self.root / gen.LEDGER).write_text(gen.ledger_text(self.entries), encoding="utf-8")

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

    def test_dynamic_or_unresolved_saved_text_is_rejected(self):
        rule = self.species_rule()
        for value in ('_("{PLAYER}")', '_("{STR_VAR_1}")', '_("{NOT_A_TOKEN}")', 'NAME_FROM_OTHER_FILE', '_("a" UNKNOWN_NAME)'):
            with self.subTest(value=value):
                found = self.problems("src/species.h", '.speciesName = ' + value + ',', rule)
                self.assertTrue(found)
                self.assertIn("error:", found[0])
        rule = self.checks.Rule(("src/name.c",), "PLAYER_NAME_LENGTH", "name", fields=("trainerName",))
        found = self.problems("src/name.c", '.trainerName = J_("あいうえおか"),', rule)
        self.assertIn("is 8 bytes, limit 7", found[0])

    def test_name_like_text_inside_literals_is_not_an_initializer(self):
        rule = self.species_rule()
        text = 'const char *debug = ".speciesName = \\\"天\\\"";\n.speciesName = _("ピカチュウ"),'
        self.assertEqual(self.problems("src/species.h", text, rule), [])


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
