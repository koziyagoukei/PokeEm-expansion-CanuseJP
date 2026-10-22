"""Runtime layout checks in addition to the extracted-file hash checks."""

from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest

TOOLS = Path(__file__).resolve().parents[1]
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))
import extract_bpej_contest as contest


def function(source, name):
    match = re.search(rf"^(?:static )?\w+ {name}\([^;]*?\)\n\{{.*?^\}}", source, re.M | re.S)
    if match is None:
        raise AssertionError(f"missing C function: {name}")
    return match[0]


class ContestUILayoutTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if not (ROOT / "baserom.gba").is_file():
            raise unittest.SkipTest("place baserom.gba before ROM integration tests")
        manifest = contest.load_manifest(TOOLS / "data/bpej_contest_manifest.json")
        rom = (ROOT / "baserom.gba").read_bytes()
        contest.validate_baserom(rom, manifest, ROOT / "baserom.gba")
        cls.data = {}
        for asset in manifest["assets"]:
            raw = contest.rom_output(rom, asset)
            cls.data[asset["label"]] = (contest.decode_lz77(raw, asset)
                if asset["output_representation"] == "gba_lz77_stream" else raw)
        cls.source = (ROOT / "src/contest.c").read_text(encoding="utf-8")

    def assert_tiles(self, label, indexes, expected):
        data = self.data[label]
        self.assertTrue(all(0 <= i < len(data) // 32 for i in indexes))
        selected = b"".join(data[i * 32:(i + 1) * 32] for i in indexes)
        self.assertEqual(contest.sha1(selected), expected, label)

    def test_move_cursor_draw_and_erase_restore_all_four_rows(self):
        compiler = shutil.which("cc") or shutil.which("gcc")
        if compiler is None:
            self.skipTest("a host C compiler is needed for the cursor execution test")
        # Compile the actual cursor functions with a RAM tilemap instead of GBA
        # VRAM. Use the engine's actual tile-entry palette handling as well.
        bg_source = (ROOT / "src/bg.c").read_text(encoding="utf-8")
        program = r"""
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
static u16 maps[4][32 * 64], before[4][32 * 64], expected[4][32 * 64];
static u32 copyFlags;
static void Contest_SetBgCopyFlags(u32 bg) { copyFlags |= 1u << bg; }
"""
        program += function(bg_source, "CopyTileMapEntry")
        program += r"""
static void WriteSequenceToBgTilemapBuffer(u32 bg, u16 tile, u8 x, u8 y,
    u8 width, u8 height, u8 palette, s16 delta)
{
    assert(bg < 4 && x + width <= 32 && y + height <= 64);
    for (u32 row = y; row < y + height; row++)
        for (u32 col = x; col < x + width; col++)
        {
            CopyTileMapEntry(&tile, &maps[bg][row * 32 + col], palette, 0, 0);
            tile = (tile & 0xFC00) + ((tile + delta) & 0x3FF);
        }
}
"""
        for name in ("ContestBG_FillBoxWithIncrementingTile", "ContestBG_FillBoxWithTile",
                     "DrawMoveSelectArrow", "EraseMoveSelectArrow"):
            program += function(self.source, name)
        enum = re.search(r"^enum \{\n    STAT_SYMBOL_CIRCLE,.*?^\};", self.source, re.M | re.S)
        self.assertIsNotNone(enum)
        program += enum[0]
        for name in ("GetStatusSymbolTileOffset", "GetStarTileOffset", "GetAppealHeartTileOffset"):
            program += function(self.source, name)
        program += r"""
int main(int argc, char **argv)
{
    const int selections[] = {0, 1, 2, 3, 0, 3, 2, 1, 0};
    const u16 statuses[] = {0x9080, 0x9084, 0x9086, 0x9088, 0x9082};
    assert(argc == 2);
    FILE *input = fopen(argv[1], "rb");
    assert(input != NULL);
    assert(fread(maps[2], 1, sizeof(maps[2]), input) == sizeof(maps[2]));
    fclose(input);
    memcpy(before, maps, sizeof(maps));
    for (unsigned n = 0; n < sizeof(selections) / sizeof(selections[0]); n++)
    {
        int move = selections[n];
        memcpy(expected, before, sizeof(expected));
        expected[2][(31 + move * 2) * 32 + 1] = 1;
        expected[2][(32 + move * 2) * 32 + 1] = 2;
        copyFlags = 0;
        DrawMoveSelectArrow(move);
        assert(memcmp(maps, expected, sizeof(maps)) == 0);
        assert(copyFlags == (1u << 2));
        DrawMoveSelectArrow(move);
        assert(memcmp(maps, expected, sizeof(maps)) == 0);
        copyFlags = 0;
        EraseMoveSelectArrow(move);
        assert(memcmp(maps, before, sizeof(maps)) == 0);
        assert(copyFlags == (1u << 2));
    }
    assert(GetStarTileOffset() == 0x2034);
    for (int i = 0; i < 4; i++)
        assert(GetAppealHeartTileOffset(i) == 0x5012 + (i << 12));
    for (int i = 0; i < 5; i++)
        assert(GetStatusSymbolTileOffset(i) == statuses[i]);
    return 0;
}
"""
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)
            source = path / "cursor.c"
            binary = path / ("cursor.exe" if sys.platform == "win32" else "cursor")
            tilemap = path / "interface.bin"
            source.write_text(program, encoding="utf-8")
            tilemap.write_bytes(self.data["gContestInterfaceTilemap"])
            built = subprocess.run([compiler, "-std=gnu17", "-Wall", "-Wextra", "-Werror",
                                    str(source), "-o", str(binary)], text=True, capture_output=True)
            self.assertEqual(built.returncode, 0, built.stderr)
            result = subprocess.run([str(binary), str(tilemap)], text=True, capture_output=True)
            self.assertEqual(result.returncode, 0, result.stderr)

        # The empty cell is palette index 10 throughout. Column 0 is the border,
        # and column 2 onward belongs to the move names, not to the cursor.
        self.assertEqual(self.data["gContestInterfaceGfx"][12 * 32:13 * 32], b"\xAA" * 32)
        for y in range(31, 39):
            self.assertEqual(struct.unpack_from("<HH", self.data["gContestInterfaceTilemap"], y * 64), (11, 12))

    def test_interface_hearts_stars_and_status_tiles_match_the_pre_extraction_art(self):
        # These hashes were independently compared with the deleted original
        # PNG conversions, not just with the ROM-derived manifest hashes.
        groups = [
            ([0x11, 0x12, 0x13, 0x14, 0x35, 0x36], "7ee67e1cc331792a677d25cd45a91919d246e336"),
            ([0x34], "64d6837e0e4edcd0aae960e4767ab3671f06362e"),
            ([i for start in (0x80, 0x82, 0x84, 0x86, 0x88)
                for i in (start, start + 1, start + 16, start + 17)],
             "c831d31154b5a2097d0120d082743de024c67017"),
        ]
        for indexes, expected in groups:
            self.assert_tiles("gContestInterfaceGfx", indexes, expected)
        for name, tile in (("FILLED_APPEAL", 0x5012), ("FILLED_JAM", 0x5014),
                           ("EMPTY_APPEAL", 0x5035), ("EMPTY_JAM", 0x5036)):
            self.assertRegex(self.source, rf"#define TILE_{name}_HEART\s+{hex(tile)}\b")

    def test_results_and_applause_cells_match_the_pre_extraction_art(self):
        groups = [
            ("gContestResults_Gfx", [0x4C, 0x4D, 0x4E, 0x4F, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F],
             "201e35e5d01bb6ebe729f372ce921b911f04c24a"),
            ("gContestResults_Gfx", [i for start in (0x43, 0x45, 0x47, 0x49)
                                    for i in (start, start + 1, start + 16, start + 17)],
             "6494cd204498f960013651d1f5b91022262e87b5"),
            ("gContestResults_Gfx", list(range(0xA2, 0xA7)) + list(range(0xB2, 0xB5)),
             "b5575243e3ed47f40f4ed3bd008aa281229b95f8"),
            ("gContestApplauseGfx", list(range(17, 22)) + list(range(25, 30)),
             "b17b51fb19e7ae7b760c7a0f8dcce7794b3cc80f"),
        ]
        for label, indexes, expected in groups:
            self.assert_tiles(label, indexes, expected)
        update = function(self.source, "UpdateApplauseMeter")
        self.assertIn("tileNum + 17 + i", update)
        self.assertIn("tileNum + 25 + i", update)
        self.assertEqual(len(self.data["gContestApplauseMeterGfx"]), 128)

    def test_active_small_sprite_frames_stay_within_the_extracted_sheets(self):
        # Starts and footprints are in 32-byte OBJ tile units. Only the two
        # condition-title animations actually selected by CreateConditionSprite
        # are included; its legacy third animation is not used.
        layouts = [
            ("gContestSliderHeart_Gfx", [0], 1),
            ("gContestJudgeGfx", [0], 64),
            ("gContestJudgeSymbolsGfx", list(range(0, 28, 4)), 4),
            ("gContestApplauseGfx", [0], 32),
            ("gContestNextTurnGfx", [0], 4),
            ("gContestNextTurnGfx", [4], 1),
            ("gContestNextTurnNumbersGfx", list(range(4)), 1),
            ("gContestNextTurnRandomGfx", [0], 1),
            ("sConditionPokeball_Gfx", [0, 4], 4),
            ("sConditionPokeballPlaceholder_Gfx", [0], 1),
            ("gPokenavConditionCancel_Gfx", [0], 8),
            ("sConditionSparkle_Pal", list(range(0, 28, 4)), 4),
            ("gUsePokeblockUpDown_Gfx", [0, 8], 8),
            ("gUsePokeblockCondition_Gfx", [0, 32], 32),
        ]
        for label, starts, count in layouts:
            with self.subTest(label=label):
                self.assertTrue(all((start + count) * 32 <= len(self.data[label]) for start in starts))
        for label in ("gContestSliderHeart_Gfx", "gContestJudgeGfx", "gContestJudgeSymbolsGfx",
                      "gContestApplauseGfx", "gContestNextTurnGfx"):
            sizes = re.findall(rf"\.data = {label},\s+\.size = (0x[0-9A-Fa-f]+),", self.source)
            self.assertTrue(sizes, label)
            self.assertTrue(all(int(size, 16) == len(self.data[label]) for size in sizes), label)
        menu_source = (ROOT / "src/menu_specialized.c").read_text(encoding="utf-8")
        use_source = (ROOT / "src/use_pokeblock.c").read_text(encoding="utf-8")
        for label in ("sConditionPokeball_Gfx", "sConditionPokeballPlaceholder_Gfx",
                      "gPokenavConditionCancel_Gfx", "sConditionSparkle_Pal", "gUsePokeblockUpDown_Gfx"):
            size = re.search(rf"\{{\s*{label},\s*(0x[0-9A-Fa-f]+),", menu_source + use_source)
            self.assertIsNotNone(size, label)
            self.assertEqual(int(size[1], 16), len(self.data[label]), label)
        sparkle = re.search(r"sAnim_ConditionSparkle\[\] =\s*\{(.*?)\};", menu_source, re.S)[1]
        self.assertEqual([int(n) for n in re.findall(r"ANIMCMD_FRAME\((\d+),", sparkle)], list(range(0, 28, 4)))
        for suffix in ("MainMenu", "Condition"):
            self.assertEqual(len(self.data[f"gPokenavLeftHeader{suffix}_Gfx"]), 64 * 32)
        for suffix in ("Party", "Search", "Cool", "Beauty", "Cute", "Smart", "Tough"):
            self.assertGreaterEqual(len(self.data[f"gPokenavLeftHeader{suffix}_Gfx"]), 2 * 8 * 32)


if __name__ == "__main__":
    unittest.main()
