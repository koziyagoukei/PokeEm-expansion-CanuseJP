"""Corpus alignment is a review aid, not permission to overwrite game text."""
from pathlib import Path
import json
import re
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import audit_poke_corpus as corpus
from sync_bpej_text import Charmap, find_target, load_rom, sync


class CorpusReaderTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.charmap = Charmap(ROOT / "charmap.txt")

    def test_width_aliases_and_dialogue_controls(self):
        self.assertEqual(corpus.from_corpus(r"[PLAYER]は　１２！\nはい\rいいえ\c‥‥"),
                         r"{PLAYER}は 12!\nはい\lいいえ\p……")

    def test_missing_and_unsupported_reference_is_rejected(self):
        for text in ("[NULL]", "[UNUSED]", r"\[abc", "{漢|かん}"):
            with self.subTest(text=text), self.assertRaises(ValueError):
                corpus.from_corpus(text)

    def test_numeric_and_symbolic_arguments_encode_identically(self):
        self.assertEqual(corpus.encode_text("{COLOR RED}あ", self.charmap),
                         corpus.encode_text("{COLOR 4}あ", self.charmap))
        self.assertEqual(corpus.encode_text("{JPN}あ……$", self.charmap),
                         corpus.encode_text("あ{0xB0}{0xB0}", self.charmap))

    def test_unknown_arguments_and_embedded_terminators_are_rejected(self):
        for text in ("{COLOR}", "{COLOR 256}", "{COLOR UNKNOWN}", "{STR_VAR_1 4}", "{0xFF}あ"):
            with self.subTest(text=text), self.assertRaises(ValueError):
                corpus.encode_text(text, self.charmap)

    def test_different_placeholder_multiplicities_are_not_compatible(self):
        self.assertNotEqual(corpus.placeholders("{STR_VAR_1}と{STR_VAR_1}"),
                            corpus.placeholders("{STR_VAR_1}と{STR_VAR_2}"))
        self.assertEqual(corpus.placeholders("{JPN}{PLAYER}が{STR_VAR_2}をえらぶ"),
                         corpus.placeholders("{STR_VAR_2}を{PLAYER}がえらぶ"))

    def test_aligned_rows_and_duplicate_qids_remain_ambiguous(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)
            for name, body in {"qid_msg.txt": "e.script.x.Label\ne.script.x.Label\n",
                               "en_msg.txt": "one\ntwo\n", "ja-Hrkt_msg.txt": "あ\nい\n",
                               "revision.txt": "a" * 40}.items():
                (path / name).write_text(body, encoding="utf-8")
            index, metadata = corpus.load_corpus(path)
            self.assertEqual(len(index["Label"]), 2)
            self.assertEqual(metadata["duplicate_qids_skipped"], 1)
            (path / "en_msg.txt").write_text("one\n", encoding="utf-8")
            with self.assertRaisesRegex(ValueError, "line counts differ"):
                corpus.load_corpus(path)

    def test_comments_and_fake_declarations_in_literals_are_not_sources(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "data").mkdir()
            (root / "src").mkdir()
            (root / "src/test.c").write_text(
                '// const u8 hidden[] = _("天");\n'
                'const char *path = "const u8 fake[] = _(\\\"海\\\");";\n'
                'const u8 real[] = _("あ");\n', encoding="utf-8")
            self.assertEqual([row[3]["label"] for row in corpus.source_strings(root)], ["real"])


class AppliedTextTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.manifest = json.loads((ROOT / "tools/data/poke_corpus_text_manifest.json").read_text(encoding="utf-8"))

    @unittest.skipUnless((ROOT / "baserom.gba").exists(), "provide verified BPEJ for ROM-reference checks")
    def test_reviewed_replacements_match_verified_rom(self):
        counts, changes = sync(ROOT, load_rom(ROOT / "baserom.gba"), self.manifest)
        self.assertEqual(counts, {"verified": 41})
        self.assertEqual(changes, [])

    def test_critical_semantic_corrections(self):
        cases = (
            ("data/maps/BattleFrontier_BattlePyramidFloor/scripts.inc", "asm", "BattlePyramid_Text_OneTrainersRemaining1", "まだ 1にん"),
            ("data/text/cable_club.inc", "asm", "CableClub_Text_WhichBattleMode", "どちらの しょうぶ"),
            ("data/text/cable_club.inc", "asm", "CableClub_Text_ReturnsToPreviousStep", "ひとつ まえ"),
            ("src/data/text/gift_ribbon_descriptions.h", "variable", "gGiftRibbonDescriptionPart1_SpringHolidays", "はるやすみ"),
            ("src/contest_painting.c", "variable", "gContestHallPaintingCaption", "{STR_VAR_2} の {STR_VAR_3}"),
            ("src/strings.c", "variable", "gText_ExpandedPlaceholder_Aqua", "{JPN}アクア"),
        )
        for file, kind, label, expected in cases:
            with self.subTest(label=label):
                value = find_target((ROOT / file).read_text(encoding="utf-8"), dict(kind=kind, label=label))[2]
                self.assertIn(expected, value)
                if label == "gText_ExpandedPlaceholder_Aqua":
                    self.assertNotIn("だん", value)

    def test_manual_complements_preserve_placeholder_contract(self):
        mail = find_target((ROOT / "src/mail.c").read_text(encoding="utf-8"),
                           dict(kind="variable", label="sText_FromSpace"))[2]
        self.assertEqual(mail, "なまえ ")
        # 4 kana cells plus the longest 7-character player name fit 88px;
        # the signature remains a display-only string, never a saved name.
        self.assertLessEqual(len(mail) * 7 + 7 * 8, 88)
        timer = find_target((ROOT / "src/trainer_tower.c").read_text(encoding="utf-8"),
                            dict(kind="variable", label="gText_XMinYZSec"))[2]
        self.assertEqual(corpus.placeholders(timer), corpus.placeholders("{STR_VAR_1}{STR_VAR_2}{STR_VAR_3}"))
        self.assertNotIn("MIN", timer)
        self.assertNotIn("SEC", timer)

    def test_long_relearner_names_keep_a_conservative_width_bound(self):
        source = (ROOT / "src/strings.c").read_text(encoding="utf-8")
        for label in ("gText_MoveRelearnerPkmnTryingToLearnMove", "gText_MoveRelearnerPkmnForgotMoveAndLearnedNew",
                      "gText_MoveRelearnedPkmnDidNotLearnMove"):
            value = find_target(source, dict(kind="variable", label=label))[2]
            value = value.replace("{STR_VAR_1}", "あ" * 12)
            value = value.replace("{STR_VAR_2}", "あ" * 16).replace("{STR_VAR_3}", "あ" * 16)
            value = corpus.MODES.sub("", value)
            for line in re.split(r"\\[nlp]", value):
                with self.subTest(label=label, line=line):
                    # Overestimate every visible glyph, including spaces, at 7px.
                    self.assertLessEqual(len(line) * 7, 200)


if __name__ == "__main__":
    unittest.main()
