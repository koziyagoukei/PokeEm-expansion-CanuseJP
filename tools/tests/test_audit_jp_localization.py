from pathlib import Path
import sys
import tempfile
import unittest

TOOLS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(TOOLS))
import audit_jp_localization as audit


class LocalizationAuditTests(unittest.TestCase):
    def test_comments_do_not_erase_comment_markers_in_strings(self):
        text = '// _("ignore")\nconst u8 text[] = _("http://example"); /* _("ignore") */\n'
        masked = audit.mask_comments(text)
        self.assertIn('_("http://example")', masked)
        self.assertNotIn('ignore', masked)
        self.assertEqual(masked.count("\n"), text.count("\n"))

    def test_control_names_are_not_english(self):
        self.assertEqual(audit.classify(r"{JPN}{STR_VAR_1}の HPを\nかいふく!")[0], "japanese")
        self.assertEqual(audit.classify(r"{JPN}The きのみ")[0], "mixed")
        self.assertEqual(audit.classify("Choose a Pokemon")[0], "english")

    def test_inventory_labels_and_concatenation(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "src").mkdir()
            (root / "data").mkdir()
            (root / "src/text.c").write_text('const u8 gText_Example[] = _("Hello " "world");\n')
            (root / "data/text.inc").write_text('Text::\n\t.string "{JPN}あ$"\n', encoding="utf-8")
            rows = list(audit.inventory(root))
            self.assertEqual(len(rows), 2)
            self.assertEqual(next(row for row in rows if row["label"] == "gText_Example")["text"], "Hello world")
            self.assertEqual(next(row for row in rows if row["label"] == "Text")["status"], "japanese")

    def test_english_possessive_after_a_placeholder_is_not_missed(self):
        self.assertEqual(audit.classify("{STR_VAR_1}'s {STR_VAR_2}")[0], "english")
        self.assertEqual(audit.classify("{STR_VAR_1}'s ポケモン")[0], "mixed")
        self.assertEqual(audit.classify("{STR_VAR_1}の ポケモン")[0], "japanese")

    def test_scope_is_a_review_hint_not_a_translation(self):
        self.assertEqual(audit.review_scope({"file":"src/debug.c"}), "developer")
        self.assertEqual(audit.review_scope({"file":"src/data/battle_frontier/apprentice.h"}), "international_names")
        self.assertEqual(audit.review_scope({"file":"src/strings.c"}), "needs_review")


if __name__ == "__main__":
    unittest.main()
