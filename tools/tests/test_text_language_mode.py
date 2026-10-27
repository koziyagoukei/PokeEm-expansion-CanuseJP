"""Execute the engine's text parser, metrics and glyph selection on a host CPU."""

from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]


def function(source, name):
    match = re.search(
        rf"^(?:static )?[^\n;=]+\b{re.escape(name)}\([^;]*?\)\n\{{.*?^\}}",
        source, re.M | re.S)
    if match is None:
        raise AssertionError(f"missing C function: {name}")
    return match[0] + "\n"


def engine_code():
    text = (ROOT / "src/text.c").read_text(encoding="utf-8")
    strings = (ROOT / "src/string_util.c").read_text(encoding="utf-8")
    dynamic = (ROOT / "src/dynamic_placeholder_text_util.c").read_text(encoding="utf-8")
    international = (ROOT / "src/international_string_util.c").read_text(encoding="utf-8")
    fonts = (ROOT / "src/fonts.c").read_text(encoding="utf-8")
    result = []
    # Use the real metrics, attributes, decompression addressing and renderer.
    # Only hardware I/O, glyph pixels and saved-name lookup are stubbed.
    arrays = re.findall(r"ALIGNED\(4\) (const u8 \w+GlyphWidths\[\] = \{.*?^\};)",
                        fonts, re.M | re.S)
    result.extend(arrays)
    glyphs = sorted(set(re.findall(r"\bgFont\w+Glyphs\b", text)))
    result.extend(f"static const u16 {name}[32768] = {{0}};" for name in glyphs)
    result.append("#define KANJI_GLYPH_COUNT 25\n#define KANJI_NORMAL_WIDTH 12\n#define KANJI_SMALL_WIDTH 12\n"
                  "static const u16 sKanjiGlyphs[1024] = {0};")
    result.append("void DecompressGlyphTile(const void *src, void *dest) {\n"
                  "    uintptr_t address = (uintptr_t)src;\n"
                  "    sLoadedJapanese = FALSE;\n")
    for name in glyphs:
        if "Japanese" in name:
            result.append(f"    if (address >= (uintptr_t){name} && address < "
                          f"(uintptr_t)({name} + ARRAY_COUNT({name}))) sLoadedJapanese = TRUE;")
    result.append("    memset(dest, 0, 32);\n}")
    attrs = re.search(r"^static const struct FontInfo sFontInfos\[\] =\n\{.*?^\};",
                      text, re.M | re.S)[0]
    result.append(re.sub(r"\.fontFunction = \w+", ".fontFunction = NULL", attrs))
    result.append("const struct FontInfo *gFonts = sFontInfos;")
    result.append(re.search(r"^struct\n\{\n    u16 tileOffset;.*?^\};",
                            text, re.M | re.S)[0])
    for name in ("GetExtCtrlCodeLength", "StringCopy", "StringLength", "StringCopyN",
                 "StringCopyN_Multibyte", "StringLength_Multibyte", "SkipExtCtrlCode",
                 "StringCompareWithoutExtCtrlCodes", "StripExtCtrlCodes", "StringExpandPlaceholders", "ConvertInternationalString"):
        result.append(function(strings, name))
    result.append(function(international, "ConvertInternationalContestantName"))
    for name in ("DynamicPlaceholderTextUtil_Reset", "DynamicPlaceholderTextUtil_SetPlaceholderPtr",
                 "DynamicPlaceholderTextUtil_GetPlaceholderPtr", "DynamicPlaceholderTextUtil_ExpandPlaceholders"):
        result.append(function(dynamic, name))
    for name in ("IsJapaneseGlyph", "ShouldUseJapaneseGlyph", "ResolveTextMode", "IsJapaneseTextMode",
                 "TextAutoModeFromNextChar", "IsSmallKanjiFont", "DecompressKanjiGlyph", "GetJapaneseGlyphWidth", "DecompressJapaneseSpaceGlyph"):
        result.append(function(text, name))
    suffixes = ("Small", "Normal", "Short", "Narrow", "SmallNarrow", "Narrower",
                "SmallNarrower", "ShortNarrow", "ShortNarrower")
    for suffix in suffixes:
        result.append(function(text, f"GetGlyphWidth_{suffix}"))
        result.append(function(text, f"DecompressGlyph_{suffix}"))
    result.append(re.search(r"^static const struct GlyphWidthFunc sGlyphWidthFuncs\[\] =\n\{.*?^\};",
                            text, re.M | re.S)[0])
    for name in ("GetFontAttribute", "GetKeypadIconWidth", "DrawKeypadIcon", "GetFontWidthFunc",
                 "GetGlyphWidth", "GetStringWidth", "PrintGlyph", "RenderText", "AddTextPrinter"):
        result.append(function(text, name))
    return "\n".join(result)


class TextLanguageModeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        compiler = shutil.which("cc") or shutil.which("gcc")
        if compiler is None:
            raise unittest.SkipTest("a host C compiler is needed (run in WSL)")
        cls.temp = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        path = Path(cls.temp.name)
        fixture = (Path(__file__).parent / "fixtures/text_language_mode.c").read_text(encoding="utf-8")
        source = path / "text.c"
        source.write_text(fixture.replace("/* ENGINE_FUNCTIONS */", engine_code()), encoding="utf-8")
        cls.binaries = []
        for legacy in (0, 1):
            binary = path / f"text-{legacy}"
            built = subprocess.run([compiler, "-std=gnu17", "-Wall", "-Wextra", "-Werror",
                                    "-Wno-unused-parameter", "-Wno-sign-compare", "-Wno-old-style-declaration",
                                    f"-DTEST_LEGACY_MODE={legacy}", "-iquote", str(ROOT / "include"),
                                    str(source), "-o", str(binary)], capture_output=True, text=True)
            if built.returncode:
                raise AssertionError(built.stderr)
            cls.binaries.append(binary)

    def run_case(self, case):
        for binary in self.binaries:
            with self.subTest(configuration=binary.name):
                result = subprocess.run([str(binary), case], text=True, capture_output=True)
                self.assertEqual(result.returncode, 0, result.stderr)

    def test_default_and_explicit_modes(self):
        self.run_case("modes")

    def test_actual_render_and_width_agree_across_fonts(self):
        self.run_case("metrics")

    def test_inline_switches_auto_and_spacing(self):
        self.run_case("switches")

    def test_kanji_render_and_width(self):
        self.run_case("kanji")

    def test_kanji_text_utilities(self):
        self.run_case("kanji_utilities")

    def test_control_payloads_are_not_language_markers(self):
        self.run_case("controls")

    def test_placeholder_expansion_matches_rendering(self):
        self.run_case("placeholders")

    def test_names_restore_default_without_growing_buffers(self):
        self.run_case("names")

    def test_extended_symbols_and_keypad_icons(self):
        self.run_case("symbols")

    def test_minimum_advance_does_not_underflow(self):
        self.run_case("minimum")

    def test_numeric_glyphs_and_compact_ui_widths(self):
        self.run_case("numeric")


if __name__ == "__main__":
    unittest.main()
