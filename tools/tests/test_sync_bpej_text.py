import copy
from pathlib import Path
import tempfile
import sys
import unittest

TOOLS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(TOOLS))
import sync_bpej_text as sync


class TextSyncTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.charmap = sync.Charmap(TOOLS.parent / "charmap.txt")

    def test_kana_not_music_ids_or_latin_aliases(self):
        self.assertEqual(self.charmap.decode(bytes((1, 2, 0x35, 0x51, 0xA0, 0xFF)), 0), ("あいゅアッ", 6))

    def test_placeholder_context(self):
        data = bytes((0xFD, 1, 0xFF))
        self.assertEqual(self.charmap.decode(data, 0)[0], "{PLAYER}")
        self.assertEqual(self.charmap.decode(data, 0, context="battle")[0], "{B_BUFF2}")

    def test_buttons_and_arrows_not_music_ids(self):
        data = bytes((0xF8, 1, 0xF8, 0xA, 0xF9, 0, 0xF9, 0xD0, 0xFF))
        self.assertEqual(self.charmap.decode(data, 0)[0],
                         "{B_BUTTON}{DPAD_UPDOWN}{UP_ARROW}{EMOJI_UNDERSCORE}")

    def test_english_bank_not_misread_as_kana(self):
        with self.assertRaisesRegex(ValueError, "Latin-bank"):
            self.charmap.decode(bytes((0xFC, 0x16, 1, 0xFF)), 0)
        self.assertEqual(self.charmap.decode(bytes((0xFC, 0x16, 0xFC, 0x15, 1, 0xFF)), 0)[0],
                         "{ENG}{JPN}あ")

    def test_control_argument_ff_is_not_terminator(self):
        self.assertEqual(self.charmap.decode(bytes((0xFC, 8, 0xFF, 1, 0xFF)), 0), ("{PAUSE 255}あ", 5))

    def test_boundaries_and_unknown_codes(self):
        for data in (b"\x01", b"\xFC", b"\xFD\x40\xFF", b"\xFC\xFE\xFF", b"\xF1\xFF"):
            with self.subTest(data=data), self.assertRaises(ValueError):
                self.charmap.decode(data, 0)
        with self.assertRaises(ValueError):
            self.charmap.decode(b"\x01\xFF", 0, limit=1)
        with self.assertRaises(ValueError):
            self.charmap.decode(b"\xFF", -1)

    def test_all_newline_types(self):
        self.assertEqual(self.charmap.decode(b"\x01\xFE\x02\xFA\x03\xFB\xFF", 0)[0], r"あ\nい\lう\p")

    def test_multiline_asm_and_commands(self):
        text = 'Text::\n\t.string "{JPN}あ\\n"\n\t.string "{JPN}い$"\n\nScript::\n\tlock\n\tmsgbox Text\n\trelease\n'
        start, end, value = sync.find_target(text, {"kind": "asm", "label": "Text"})
        self.assertEqual(value, r"{JPN}あ\n{JPN}い$")
        updated = text[:start] + '"{JPN}う$"' + text[end:]
        self.assertIn('Script::\n\tlock\n\tmsgbox Text\n\trelease\n', updated)
        self.assertEqual(sync.find_target(updated, {"kind": "asm", "label": "Text"})[2], "{JPN}う$")

    def test_asm_conditional_refused(self):
        text = 'Text:\n.if FLAG\n\t.string "あ$"\n.else\n\t.string "い$"\n.endif\n'
        with self.assertRaises(ValueError):
            sync.find_target(text, {"kind": "asm", "label": "Text"})

    def test_c_selectors_and_adjacent_literals(self):
        text = 'const u8 gText_Test[] = _("あ\\n" "い");\n[TABLE_ONE] = { .name = _("う"), .power = 50, },\n[STRINGID_X] = COMPOUND_STRING("え"),\n'
        self.assertEqual(sync.find_target(text, {"kind":"variable", "label":"gText_Test"})[2], r"あ\nい")
        self.assertEqual(sync.find_target(text, {"kind":"field", "label":"TABLE_ONE", "field":"name"})[2], "う")
        self.assertEqual(sync.find_target(text, {"kind":"indexed", "label":"STRINGID_X"})[2], "え")

    def test_equivalent_aliases(self):
        self.assertTrue(sync.equivalent_text(r"{JPN}あ！\n{JPN}い？", r"{JPN}あ!\nい?", self.charmap))
        self.assertFalse(sync.equivalent_text("あ", "{JPN}あ", self.charmap))
        self.assertFalse(sync.equivalent_text(r"{JPN}あ\nい", r"{JPN}あ\lい", self.charmap))

    def test_hash_staleness_atomicity_and_crlf(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            for name in ("charmap.txt", "charmap_kanji.txt"):
                (root / name).write_bytes((TOOLS.parent / name).read_bytes())
            original = b'Text::\r\n\t.string "English$"\r\n\r\nScript::\r\n\tend\r\n'
            (root / "text.inc").write_bytes(original)
            rom = b"\x01\xFF"
            entry = {"file":"text.inc", "target":{"kind":"asm", "label":"Text"},
                     "rom_offset":"0x0", "size":2, "sha256":sync.sha256(rom),
                     "source_sha256":sync.sha256(b"English$")}
            manifest = {"entries":[entry]}
            with self.assertRaises(ValueError):
                sync.sync(root, rom, manifest)
            invalid = copy.deepcopy(entry)
            invalid["target"]["label"] = "Missing"
            with self.assertRaises(ValueError):
                sync.sync(root, rom, {"entries":[entry, invalid]}, apply=True)
            self.assertEqual((root / "text.inc").read_bytes(), original)
            counts, changes = sync.sync(root, rom, manifest, apply=True)
            self.assertEqual(counts, {"updated":1})
            result = (root / "text.inc").read_bytes()
            self.assertIn('"{JPN}あ$"'.encode(), result)
            self.assertNotIn(b"\n", result.replace(b"\r\n", b""))
            self.assertEqual(sync.sync(root, rom, manifest)[0], {"verified":1})
            (root / "text.inc").write_bytes(result.replace('あ'.encode(), 'い'.encode()))
            with self.assertRaisesRegex(ValueError, "source changed"):
                sync.sync(root, rom, manifest, apply=True)

    def test_bad_rom(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "baserom.gba"
            path.write_bytes(b"not a ROM")
            with self.assertRaisesRegex(ValueError, "BPEJ SHA1"):
                sync.load_rom(path)

    def test_zero_padded_category(self):
        data = bytes((0x10, 0x18, 0, 0, 0, 0))
        entry = {"rom_offset":"0x0", "size":6, "format":"zero_padded",
                 "sha256":sync.sha256(data), "target":{"kind":"field"}}
        self.assertEqual(sync.expected_text(data, entry, self.charmap), "{JPN}たね")
        entry["format"] = "guess"
        with self.assertRaises(ValueError):
            sync.expected_text(data, entry, self.charmap)

    def test_aggregate_name_only(self):
        text = '[TRAINER_CLASS_X] = { _("name"), 50, BALL_ULTRA },'
        start, end, value = sync.find_target(text, {"kind":"aggregate", "label":"TRAINER_CLASS_X"})
        self.assertEqual(text[:start] + '"あ"' + text[end:], '[TRAINER_CLASS_X] = { _("あ"), 50, BALL_ULTRA },')

    def test_scope_duplicate_designators(self):
        text = 'int other[] = {\n[TYPE_NORMAL] = {1},\n};\nconst struct TypeInfo types[] = {\n[TYPE_NORMAL] = { .name = _("ノーマル") },\n};\n'
        start, end, value = sync.find_target(text, {"kind":"field", "table":"types", "label":"TYPE_NORMAL", "field":"name"})
        self.assertEqual(value, "ノーマル")
        self.assertEqual(text[start:end], '"ノーマル"')

    def test_ordinal_field_with_guarded_row_count(self):
        text = 'const struct Data rows[] = {\n{ .request = COMPOUND_STRING("one"), .item = 1 },\n{ .request = COMPOUND_STRING("two"), .item = 2 },\n};\n'
        target = {"kind":"ordinal_field", "table":"rows", "label":"rows", "field":"request", "index":1, "rowCount":2}
        start, end, value = sync.find_target(text, target)
        self.assertEqual(value, "two")
        self.assertIn('.item = 2', text[:start] + '"あ"' + text[end:])
        target["rowCount"] = 3
        with self.assertRaisesRegex(ValueError, "row count/index"):
            sync.find_target(text, target)

    def test_conditional_field_requires_explicit_variant_guard(self):
        text = '[EFFECT_X] = {\n#if UPDATED\n.description = COMPOUND_STRING("new"),\n#else\n.description = COMPOUND_STRING("old"),\n#endif\n},'
        target = {"kind":"field", "label":"EFFECT_X", "field":"description"}
        with self.assertRaises(ValueError):
            sync.find_target(text, target)
        target.update(variantIndex=0, variantCount=2)
        self.assertEqual(sync.find_target(text, target)[2], "new")

    def test_ordinal_literal_scoped_to_array(self):
        text = 'const u8 *names[] = {\nCOMPOUND_STRING("one"),\nCOMPOUND_STRING("two"),\n};\nconst u8 other[] = _("three");\n'
        target = {"kind":"ordinal_literal", "table":"names", "label":"names", "rowCount":2, "index":1}
        self.assertEqual(sync.find_target(text, target)[2], "two")


if __name__ == "__main__":
    unittest.main()
