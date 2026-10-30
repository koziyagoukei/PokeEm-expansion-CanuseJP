"""Run the actual berry tag text functions against the Japanese window geometry."""
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest

from test_text_language_mode import engine_code, function

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from kanji.kanji_gen import strip_c_comments
from remove_redundant_jpn import encode
from sync_bpej_text import Charmap, SEQUENCE, literal_contents


def table(source, name):
    return re.search(rf"^static const [^\n]+\b{name}\[.*?^\}};", source, re.M | re.S)[0]


def berry_cases():
    source = strip_c_comments((ROOT / "src/berry.c").read_text(encoding="utf-8"))
    starts = list(re.finditer(r"^\s*\[(BERRY_ID_\w+)\]\s*=", source, re.M))
    result = []
    for i, match in enumerate(starts):
        body = source[match.end():starts[i + 1].start() if i + 1 < len(starts) else len(source)]
        fields = {}
        for field in ("name", "description1", "description2"):
            value = re.search(r"\." + field + r"\s*=\s*(?:_|COMPOUND_STRING)\(\s*(" + SEQUENCE + r")\s*\)", body)
            if value:
                fields[field] = literal_contents(value[1])
        if len(fields) == 3:
            size = re.search(r"\.size\s*=\s*(\d+)", body)
            firmness = re.search(r"\.firmness\s*=\s*(\w+)", body)
            fields.update(id=match[1], size=size[1] if size else "0",
                          firmness=firmness[1] if firmness else "BERRY_FIRMNESS_UNKNOWN")
            result.append(fields)
    return result


class BerryTagLayoutTests(unittest.TestCase):
    def test_window_rectangles_and_tile_storage(self):
        source = (ROOT / "src/berry_tag_screen.c").read_text(encoding="utf-8")
        windows = table(source, "sWindowTemplates")
        expected = [(11, 4, 15, 2, 69), (11, 7, 15, 4, 99),
                    (4, 14, 22, 4, 159), (3, 0, 6, 2, 247)]
        used = set(range(69))  # Both Japanese static tilemaps reference at most tile 68.
        blocks = re.findall(r"\[WIN_\w+\]\s*=\s*\{(.*?)\}", windows, re.S)
        self.assertEqual(len(blocks), 4)
        for body, geometry in zip(blocks, expected):
            fields = dict(re.findall(r"\.(\w+)\s*=\s*(\d+)", body))
            actual = tuple(int(fields[f]) for f in ("tilemapLeft", "tilemapTop", "width", "height", "baseBlock"))
            self.assertEqual(actual, geometry)
            x, y, w, h, start = actual
            self.assertLessEqual(x + w, 30)
            self.assertLessEqual(y + h, 20)
            tiles = set(range(start, start + w * h))
            self.assertFalse(used & tiles)
            used |= tiles
        self.assertLess(max(used), 28 * 0x800 // 32)  # Lowest screen block starts at 0xE000.

    def test_both_scroll_directions_clear_all_data_windows(self):
        source = (ROOT / "src/berry_tag_screen.c").read_text(encoding="utf-8")
        scroll = function(source, "Task_DisplayAnotherBerry")
        for window in ("WIN_BERRY_NAME", "WIN_SIZE_FIRM", "WIN_DESC"):
            self.assertEqual(scroll.count(f"FillWindowPixelBuffer({window}, PIXEL_FILL(0))"), 2)

    def test_all_berry_text_pixels_fit_and_labels_do_not_overlap_values(self):
        compiler = shutil.which("cc") or shutil.which("gcc")
        if compiler is None:
            self.skipTest("a host C compiler is needed (run in WSL)")
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory)
            binary = build_renderer(output, compiler)
            result = subprocess.run([str(binary), str(output)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertEqual(len(list(output.glob("*.bin"))), len(berry_cases()))
            for path in output.glob("*.bin"):
                self.assertEqual(path.stat().st_size, 240 * 160)
                self.assertGreater(sum(v in (2, 3) for v in path.read_bytes()), 200)


def build_renderer(output, compiler):
    """Also usable by a local preview script; no game renderer is replaced."""
    tag = (ROOT / "src/berry_tag_screen.c").read_text(encoding="utf-8")
    text = (ROOT / "src/text.c").read_text(encoding="utf-8")
    strings = (ROOT / "src/string_util.c").read_text(encoding="utf-8")
    international = (ROOT / "src/international_string_util.c").read_text(encoding="utf-8")
    charmap = Charmap(ROOT / "charmap.txt")
    code = engine_code()
    for language, extension in (("Latin", "latfont"), ("Japanese", "hwjpnfont")):
        filename = f"{'latin' if language == 'Latin' else 'japanese'}_normal.png.{extension}"
        path = ROOT / "build/assets/graphics/fonts" / filename
        if not path.is_file():
            raise unittest.SkipTest(f"build the normal fonts first: {path}")
        data = path.read_bytes()
        values = ",".join(map(str, struct.unpack(f"<{len(data) // 2}H", data)))
        name = f"gFontNormal{language}Glyphs"
        code = code.replace(f"static const u16 {name}[32768] = {{0}};",
                            f"static const u16 {name}[] = {{{values}}};")
    code = re.sub(r"void DecompressGlyphTile\(.*?\n\}", function(text, "DecompressGlyphTile"), code, count=1, flags=re.S)
    fixture = (Path(__file__).parent / "fixtures/text_language_mode.c").read_text(encoding="utf-8")
    fixture = fixture[:fixture.index("static void reset(void)")] + function(fixture, "reset")
    fixture = re.sub(r"u32 CopyGlyphToVRAM\(.*?\n\}", """u32 CopyGlyphToVRAM(struct TextPrinter *p)
{
    for (u32 y = 0; y < gCurGlyph.height; y++)
        for (u32 x = 0; x < gCurGlyph.width; x++)
        {
            const u32 *rows = y < 8 ? gCurGlyph.gfxBufferTop : gCurGlyph.gfxBufferBottom;
            u8 pixel = rows[x / 8 * 8 + y % 8] >> (x % 8 * 4) & 15;
            host_pixel(p->printerTemplate.windowId, p->printerTemplate.currentX + x,
                p->printerTemplate.currentY + y, pixel, p->printerTemplate.color.background);
        }
    return 0;
}""", fixture, count=1, flags=re.S)
    fixture = fixture.replace("void GenerateFontHalfRowLookupTable(union TextColor color) {}",
                              function(text, "GenerateFontHalfRowLookupTable"))
    fixture = fixture.replace("static void FillWindowPixelBuffer(u8 id, u8 color) {}",
                              "static void FillWindowPixelBuffer(u8 id, u8 color) { host_fill(id, color); }")
    fixture = fixture.replace("/* ENGINE_FUNCTIONS */", code)
    fixture = fixture.replace("static u16 RenderText(struct TextPrinter *p);", """static u16 RenderText(struct TextPrinter *p);
static void host_pixel(u8 id, u32 x, u32 y, u8 color, u8 background);
static void host_fill(u8 id, u8 color);
static u16 sFontHalfRowLookupTable[256];
static union TextColor sLastTextColor;""")
    data = [table(tag, "sTextColors"), table(tag, "sWindowTemplates")]
    encode_c = lambda s: "((const u8[]){" + ",".join(map(str, encode(s, charmap))) + "})"
    firmness = table(tag, "sBerryFirmnessStrings")
    firmness = re.sub(r"COMPOUND_STRING\((" + SEQUENCE + r")\)",
                      lambda m: encode_c(literal_contents(m[1])), firmness)
    data.append(firmness)
    for match in re.finditer(r"static const u8 (sText_\w+)\[\] = _\((" + SEQUENCE + r")\);", tag):
        values = ",".join(map(str, encode(literal_contents(match[2]), charmap)))
        data.append(f"static const u8 {match[1]}[] = {{{values}}};")
    data.extend(re.findall(r"^#define BERRY_TAG_\w+.*", tag, re.M))
    data.append("static const struct BerryInfo sBerryCases[] = {")
    for entry in berry_cases():
        data.append("{" + ",".join([encode_c(entry[f]) for f in ("name", "description1", "description2")]
                                     + [entry["size"], entry["firmness"]]) + "},")
    data.append("};")
    data.append(function(strings, "ConvertIntToDecimalStringN"))
    for name in ("GetStringWidthDifference", "GetStringCenterAlignXOffsetWithLetterSpacing", "GetStringCenterAlignXOffset"):
        data.append(function(international, name))
    for name in ("PrintTextInBerryTagScreen", "AddBerryTagTextToBg0", "PrintBerryNumberAndName", "PrintBerrySize",
                 "PrintBerryFirmness", "PrintBerryDescription1", "PrintBerryDescription2", "PrintAllBerryData"):
        data.append(function(tag, name))
    screen = (Path(__file__).parent / "fixtures/berry_tag_layout.c").read_text(encoding="utf-8")
    screen = screen.replace("/* SCREEN_FUNCTIONS */", "\n".join(data))
    path = output / "berry.c"
    path.write_text(fixture + '\n#include "constants/berry.h"\n' + screen, encoding="utf-8")
    binary = output / "berry"
    result = subprocess.run([compiler, "-std=gnu17", "-O1", "-DTEST_LEGACY_MODE=0",
                             "-iquote", str(ROOT / "include"), str(path), "-o", str(binary)],
                            capture_output=True, text=True)
    if result.returncode:
        raise AssertionError(result.stderr)
    return binary


if __name__ == "__main__":
    unittest.main()
