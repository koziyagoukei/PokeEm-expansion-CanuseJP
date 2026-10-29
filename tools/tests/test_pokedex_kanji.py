"""Latest-game selection, form safety, generation and common getter regressions."""
from pathlib import Path
import json
import re
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import pokedex_kanji as dex


class TextAndFormTests(unittest.TestCase):
    def test_ruby_and_width_normalization(self):
        self.assertEqual(dex.plain("{電気|でんき}　ＰＰ。\\nつぎ"), "電気 PP。 つぎ")
        self.assertEqual(dex.plain("{電気|でんき}", reading=True), "でんき")

    def test_absent_game_entries_are_not_descriptions(self):
        for text in ("", "[NULL]", "[UNUSED]", "[~ 0]", "[~ 1] [~ 0]"):
            self.assertFalse(dex.usable(text), text)
        self.assertTrue(dex.usable("電気を ためる。"))

    def test_enum_alias_does_not_confuse_species_with_national_dex(self):
        values = dex.enum_values("enum {\n SPECIES_FIRST = 1,\n SPECIES_REGIONAL = 990,\n"
                                 " SPECIES_BASE = SPECIES_FIRST,\n};", "SPECIES_")
        self.assertEqual(values, {"SPECIES_FIRST": 1, "SPECIES_REGIONAL": 990, "SPECIES_BASE": 1})

    def test_legacy_index_rows_are_limited_to_known_base_indices(self):
        self.assertEqual(dex.key_for(dict(game="UltraSunUltraMoon", qid="usum.common.124.807")), (807, 0, "A"))
        self.assertIsNone(dex.key_for(dict(game="UltraSunUltraMoon", qid="usum.common.124.808")))
        self.assertIsNone(dex.key_for(dict(game="UltraSunUltraMoon", qid="usum.common.126.25")))

    def record(self, symbol, **kwargs):
        return dict(species=symbol, name="ヒヒダルマ", dex=555, base=False,
                    kana=kwargs.get("kana", ""), fallback=kwargs.get("fallback", False))

    def test_galar_zen_never_uses_standard_regional_label(self):
        labels = {555: [(2, "ガラルのすがた")]}
        standard = self.record("SPECIES_DARMANITAN_GALAR_STANDARD")
        zen = self.record("SPECIES_DARMANITAN_GALAR_ZEN")
        self.assertEqual(dex.resolve_form(standard, [], {}, labels)[0], 2)
        self.assertIsNone(dex.resolve_form(zen, [], {}, labels)[0])

    def test_form_labels_must_be_unambiguous(self):
        r = self.record("SPECIES_DARMANITAN_GALAR")
        self.assertIsNone(dex.resolve_form(r, [], {}, {555: [(2, "ガラルのすがた"), (3, "ガラルのすがた")]})[0])
        r.update(species="SPECIES_CHARIZARD_MEGA_X", dex=6, name="リザードン")
        self.assertEqual(dex.resolve_form(r, [], {}, {6: [(1, "メガリザードンX"), (2, "メガリザードンY")]})[0], 1)

    def test_exact_text_match_is_scoped_to_national_dex(self):
        r = self.record("SPECIES_DARMANITAN_GALAR_ZEN", kana="こおりの からだ。")
        rows = {(555, 3): [dict(kana=r["kana"])], (554, 0): [dict(kana=r["kana"])]}
        self.assertEqual(dex.resolve_form(r, [], rows, {})[0], 3)
        r["fallback"] = True
        rows[(555, 3)][0].update(rank=0, version="A", kanji="氷の体。")
        self.assertIsNone(dex.resolve_form(r, [], rows, {})[0])

    def test_wrap_preserves_characters_and_enforces_both_limits(self):
        text = "電気をためる。" * 5
        lines = dex.wrap(text).split(r"\n")
        self.assertEqual("".join(lines), text)
        self.assertLessEqual(len(lines), 3)
        for line in lines:
            self.assertLessEqual(sum(map(dex.glyph_width, line)), 224)
            self.assertNotIn(line[0], "。、")
        with self.assertRaisesRegex(ValueError, "lines"):
            dex.wrap("電" * 100)

    def test_unknown_glyph_and_control_are_not_silently_emitted(self):
        atlas = dex.Atlas(ROOT)
        cmap = dex.Charmap(ROOT / "charmap.txt")
        with self.assertRaisesRegex(ValueError, "unsupported"):
            dex.font_text(dict(kanji="{STR_VAR_1}"), atlas, cmap)
        with self.assertRaisesRegex(ValueError, "unsupported"):
            dex.font_text(dict(kanji="𠮷"), atlas, cmap)
        text, substitutions = dex.font_text(dict(kanji="{𠮷|よし}は 元気。"), atlas, cmap)
        self.assertEqual(text, "よしは 元気。")
        self.assertEqual(substitutions, [dict(kanji="𠮷", kana="よし")])


class GenerationTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)

    def put(self, name, content):
        path = self.root / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding="utf-8")
        return path

    def pack(self, game, rows):
        for language, column in (("qid", "qid"), ("ja", "kanji"), ("ja-Hrkt", "kana")):
            if game == "ScarletViolet" and language == "ja-Hrkt":
                continue
            self.put(f"{dex.CACHE}/{game}/{language}_common.txt", "\n".join(r[column] for r in rows) + "\n")

    def test_latest_available_game_precedes_version_and_older_descriptions(self):
        games = ("LegendsZA", "ScarletViolet", "SwordShield")
        for game in games:
            self.pack(game, [dict(qid=f"{game}.ZKN_COMMENT_{v}_025_000", kanji="電気。", kana="でんき。") for v in ("B", "A")]
                      + [dict(qid=f"{game}.ZKN_COMMENT_A_001_000", kanji="[~ 0]" if game == "LegendsZA" else "種。", kana="たね。")])
        with mock.patch.object(dex, "PACKS", tuple((g, "common") for g in games)):
            rows, _, _ = dex.catalog(self.root)
        self.assertEqual((rows[25, 0][0]["game"], rows[25, 0][0]["version"]), ("LegendsZA", "A"))
        self.assertEqual(rows[1, 0][0]["game"], "ScarletViolet")

    def test_kana_build_needs_no_manifest_or_download_and_preserves_mtime(self):
        stamp = self.root / "build/generated/selected.ok"
        dex.generate(self.root, 0, stamp)
        header = self.root / dex.HEADER
        before = header.stat().st_mtime_ns
        self.assertNotIn("COMPOUND_STRING", header.read_text())
        self.assertTrue(stamp.exists())
        dex.generate(self.root, 0, stamp)
        self.assertEqual(header.stat().st_mtime_ns, before)
        self.assertFalse((self.root / dex.CACHE).exists())
        with self.assertRaises(ValueError):
            dex.generate(self.root, 2)

    def test_kanji_generation_verifies_inputs_before_replacing_good_header(self):
        for name in ("charmap.txt", "charmap_kanji.txt", "graphics/fonts/kanji/koden12_atlas.png", "graphics/fonts/kanji/koden12_atlas.txt"):
            path = self.root / name
            path.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy(ROOT / name, path)
        row = dict(qid="test.ZKN_COMMENT_A_025_000", kanji="{電気|でんき}。", kana="でんき。")
        self.pack("LegendsZA", [row])
        with mock.patch.object(dex, "PACKS", (("LegendsZA", "common"),)):
            _, _, hashes = dex.catalog(self.root)
            text = dex.wrap(dex.font_text(row, dex.Atlas(self.root), dex.Charmap(self.root / "charmap.txt"))[0])
            entry = dict(species="SPECIES_PIKACHU", qid=row["qid"], source_sha256=dex.digest(row["kanji"].encode()),
                         text_sha256=dex.digest(text.encode()), reading_substitutions=[])
            manifest = dict(schema=dex.SCHEMA, revision=dex.REVISION, files=hashes, entries=[entry], kana_fallback=[])
            self.put(dex.MANIFEST, json.dumps(manifest))
            dex.generate(self.root, 1)
            header = self.root / dex.HEADER
            before = header.read_bytes()
            self.assertIn('COMPOUND_STRING("{KANJI}電気。")', header.read_text())
            path = self.root / dex.CACHE / "LegendsZA/ja_common.txt"
            path.write_text("違う。\n", encoding="utf-8")
            with self.assertRaisesRegex(ValueError, "hash mismatch"):
                dex.generate(self.root, 1)
            self.assertEqual(header.read_bytes(), before)
            path.unlink()
            with self.assertRaisesRegex(ValueError, "missing.*fetch"):
                dex.generate(self.root, 1)
            self.assertEqual(header.read_bytes(), before)

    def test_fetch_rejects_wrong_download_before_overwriting_cache(self):
        from io import BytesIO
        expected = {f"LegendsZA/{lang}_common.txt": dex.digest(b"verified\n") for lang in ("qid", "ja", "ja-Hrkt")}
        self.put(dex.MANIFEST, json.dumps(dict(schema=dex.SCHEMA, revision=dex.REVISION, files=expected)))
        cache = self.put(f"{dex.CACHE}/LegendsZA/qid_common.txt", "keep this cache\n")
        with mock.patch.object(dex, "PACKS", (("LegendsZA", "common"),)), \
                mock.patch.object(dex.urllib.request, "urlopen", return_value=BytesIO(b"wrong\n")):
            with self.assertRaisesRegex(ValueError, "download hash mismatch"):
                dex.fetch(self.root)
        self.assertEqual(cache.read_text(), "keep this cache\n")
        with mock.patch.object(dex, "PACKS", (("LegendsZA", "common"),)), \
                mock.patch.object(dex.urllib.request, "urlopen", side_effect=lambda *a, **k: BytesIO(b"verified\n")):
            dex.fetch(self.root)
        for relative in expected:
            self.assertEqual((self.root / dex.CACHE / relative).read_bytes(), b"verified\n")

    @unittest.skipUnless(shutil.which("arm-none-eabi-cpp"), "ARM preprocessor required")
    def test_actual_compile_time_configuration_and_invalid_values(self):
        cpp = "arm-none-eabi-cpp -P -I include -DMODERN=1 -x c -"
        self.assertEqual(dex.configuration(ROOT, cpp), 0)
        self.assertEqual(dex.configuration(ROOT, cpp + " -DP_POKEDEX_TEXT_STYLE=1"), 1)
        with self.assertRaises(ValueError):
            dex.configuration(ROOT, cpp + " -DP_POKEDEX_TEXT_STYLE=2")

    @unittest.skipUnless(shutil.which("cc"), "host C compiler required")
    def test_actual_getter_handles_both_styles_missing_entries_and_invalid_species(self):
        source = (ROOT / "src/pokemon.c").read_text(encoding="utf-8")
        getter = re.search(r"const u8 \*GetSpeciesPokedexDescription\(enum Species species\)\n\{.*?\n\}", source, re.S)[0]
        probe = '''#include <stddef.h>
typedef unsigned char u8;
enum Species { SPECIES_NONE, HAS_KANJI, KANA_ONLY, DISABLED, NUM_SPECIES };
#define POKEDEX_TEXT_KANJI 1
static const u8 none[]="none", kana[]="kana", kanji[]="kanji";
static const struct { const u8 *description; } gSpeciesInfo[]={ {none}, {kana}, {kana}, {NULL} };
static const u8 *const sPokedexKanjiDescriptions[]={NULL,kanji,NULL,kanji};
static enum Species SanitizeSpeciesId(enum Species s) {return (unsigned)s < NUM_SPECIES ? s : SPECIES_NONE;}
''' + getter + '''
int main(void) {
 if (GetSpeciesPokedexDescription(HAS_KANJI) != (P_POKEDEX_TEXT_STYLE ? kanji : kana)) return 1;
 if (GetSpeciesPokedexDescription(KANA_ONLY) != kana) return 2;
 if (GetSpeciesPokedexDescription(DISABLED) != none) return 3;
 if (GetSpeciesPokedexDescription(9999) != none) return 4;
 return 0;
}
'''
        path = self.put("probe.c", probe)
        for style in (0, 1):
            exe = self.root / f"probe{style}"
            subprocess.run(["cc", f"-DP_POKEDEX_TEXT_STYLE={style}", str(path), "-o", str(exe)], check=True, capture_output=True)
            subprocess.run([str(exe)], check=True)

    @unittest.skipUnless(all((ROOT / dex.CACHE / game / f"{language}_{kind}.txt").exists()
                             for game, kind in dex.PACKS for language in ("qid", "ja", "ja-Hrkt")
                             if language != "ja-Hrkt" or game != "ScarletViolet"), "optional corpus download required")
    def test_entire_reference_manifest_matches_latest_available_rows_and_display_limits(self):
        manifest = json.loads((ROOT / dex.MANIFEST).read_text(encoding="utf-8"))
        rows, _, _ = dex.catalog(ROOT, verify=manifest["files"])
        atlas, cmap = dex.Atlas(ROOT), dex.Charmap(ROOT / "charmap.txt")
        species, chars = set(), set()
        for entry in manifest["entries"]:
            self.assertNotIn(entry["species"], species)
            species.add(entry["species"])
            row = rows[entry["national_dex"], entry["form"]][0]
            self.assertEqual(row["qid"], entry["qid"])
            text, substitutions = dex.font_text(row, atlas, cmap)
            chars.update(c for c in text if dex.is_kanji(c))
            wrapped = dex.wrap(text)
            self.assertEqual(dex.digest(wrapped.encode()), entry["text_sha256"])
            self.assertEqual(substitutions, entry["reading_substitutions"])
            self.assertLessEqual(len(wrapped.split(r"\n")), dex.MAX_LINES)
            for line in wrapped.split(r"\n"):
                self.assertLessEqual(sum(map(dex.glyph_width, line)), dex.MAX_WIDTH)
        self.assertLessEqual(len(chars | set("攻撃防御特性技力速命中状態異常回復対戦相手物理変化")), 2214)
        self.assertNotIn("SPECIES_NONE", species)
        self.assertTrue(any(e["game"] == "LegendsZA" for e in manifest["entries"]))


if __name__ == "__main__":
    unittest.main()
