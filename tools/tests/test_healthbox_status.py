from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

TOOLS = Path(__file__).resolve().parents[1]
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))
import build_healthbox_status as status


def pixels(data):
    return [nibble for byte in data for nibble in (byte & 15, byte >> 4)]


class StatusTilesTests(unittest.TestCase):
    def setUp(self):
        self.source = (bytes(range(256)) * 3)[:status.SHEET_SIZE]

    def test_both_nibbles_use_battler_slot_without_changing_other_colors(self):
        for battler in range(4):
            with self.subTest(battler=battler):
                result = status.remap_tiles(self.source, battler)
                self.assertEqual(len(result), status.SHEET_SIZE)
                self.assertEqual(pixels(result), [12 + battler if p >= 12 else p for p in pixels(self.source)])

    def test_already_remapped_tiles_are_idempotent(self):
        for battler in range(4):
            result = status.remap_tiles(self.source, battler)
            self.assertEqual(status.remap_tiles(result, battler), result)

    def test_invalid_battlers(self):
        for battler in (-1, 4):
            with self.assertRaisesRegex(ValueError, "battler"):
                status.remap_tiles(self.source, battler)

    def test_invalid_sheet_sizes(self):
        for data in (b"", self.source[:-1], self.source + b"\xCC"):
            with self.assertRaisesRegex(ValueError, "expected 576 bytes"):
                status.remap_tiles(data, 0)

    def test_missing_status_color_in_one_icon_is_rejected(self):
        data = self.source[:status.ICON_SIZE] + b"\x23" * status.ICON_SIZE + self.source[2 * status.ICON_SIZE:]
        with self.assertRaisesRegex(ValueError, "icon 1 has no status-color"):
            status.remap_tiles(data, 0)

    def test_palette_updates_do_not_recolor_other_battlers(self):
        data = [status.remap_tiles(self.source, battler) for battler in range(4)]
        palette = list(range(16))
        expected = [0x6198, 0x0EF7, 0x4694, 0x72D1]
        for battler, color in enumerate(expected):
            palette[12 + battler] = color
        for color in (0x4694, 0x29DC, 0x72D1, 0x0EF7):
            palette[12] = color
            for battler in range(4):
                rendered = {palette[p] for p in pixels(data[battler]) if p >= 12}
                self.assertEqual(rendered, {color if battler == 0 else expected[battler]})

    def test_nonindexed_png_does_not_touch_existing_output(self):
        with tempfile.TemporaryDirectory() as tmp:
            source = Path(tmp) / "status.png"
            output = Path(tmp) / "status.4bpp"
            header = bytearray(b"\x89PNG\r\n\x1a\n" + struct.pack(">I", 13) + b"IHDR" + struct.pack(">II", 24, 48) + bytes((8, 2, 0, 0, 0)) + b"\x00" * 4)
            source.write_bytes(header)
            output.write_bytes(b"existing")
            with self.assertRaisesRegex(ValueError, "indexed PNG"):
                status.build(source, output, Path("unused"), 0)
            self.assertEqual(output.read_bytes(), b"existing")

    def test_conversion_failure_is_atomic(self):
        with tempfile.TemporaryDirectory() as tmp:
            output = Path(tmp) / "status.4bpp"
            output.write_bytes(b"existing")
            with mock.patch.object(status.subprocess, "run", side_effect=subprocess.CalledProcessError(1, "gbagfx")):
                with self.assertRaises(subprocess.CalledProcessError):
                    status.build(ROOT / "graphics/battle_interface/status.png", output, Path("unused"), 0)
            self.assertEqual(output.read_bytes(), b"existing")
            self.assertEqual(sorted(path.name for path in Path(tmp).iterdir()), ["status.4bpp"])

    def test_source_image_cannot_be_overwritten(self):
        with tempfile.TemporaryDirectory() as tmp:
            source = Path(tmp) / "status.png"
            source.write_bytes(b"do not overwrite")
            with self.assertRaisesRegex(ValueError, "separate generated"):
                status.build(source, source, Path("unused"), 0)
            self.assertEqual(source.read_bytes(), b"do not overwrite")

    def test_build_rule_matches_runtime_palette_base(self):
        source = (ROOT / "src/battle_interface.c").read_text(encoding="utf-8")
        base = re.search(r"#define HEALTHBOX_STATUS_COLOR_START\s+(\d+)", source)
        self.assertEqual(int(base[1]), status.STATUS_COLOR_START)
        rules = (ROOT / "graphics_file_rules.mk").read_text(encoding="utf-8")
        for battler, stem in enumerate(("status", "status2", "status3", "status4")):
            self.assertIn(f"/{stem}.4bpp: private STATUS_BATTLER := {battler}", rules)


class ActualStatusAssetsTests(unittest.TestCase):
    def test_six_statuses_in_all_four_localized_sheets(self):
        gfx = TOOLS / "gbagfx" / ("gbagfx.exe" if sys.platform == "win32" else "gbagfx")
        if not gfx.is_file():
            self.skipTest("build gbagfx before the asset integration test")
        with tempfile.TemporaryDirectory() as tmp:
            for battler, stem in enumerate(("status", "status2", "status3", "status4")):
                with self.subTest(battler=battler):
                    source = ROOT / "graphics/battle_interface" / (stem + ".png")
                    original = Path(tmp) / "raw.4bpp"
                    output = Path(tmp) / (stem + ".4bpp")
                    subprocess.run([str(gfx), str(source), str(original)], check=True)
                    status.build(source, output, gfx, battler)
                    raw = original.read_bytes()
                    result = output.read_bytes()
                    self.assertEqual(pixels(result), [12 + battler if p >= 12 else p for p in pixels(raw)])
                    for icon in range(6):
                        start = icon * status.ICON_SIZE
                        colors = set(pixels(result[start:start + status.ICON_SIZE]))
                        self.assertEqual({p for p in colors if p >= 12}, {12 + battler})
                        self.assertEqual(colors - {12 + battler}, {2, 3})

    def test_built_rom_contains_the_four_isolated_sheets(self):
        rom_path = ROOT / "pokeemerald.gba"
        elf_path = ROOT / "pokeemerald.elf"
        nm = shutil.which("arm-none-eabi-nm")
        if not rom_path.is_file() or not elf_path.is_file() or nm is None:
            self.skipTest("build the ROM and install the ARM tools before the ROM integration test")
        symbols = subprocess.check_output([nm, "-n", str(elf_path)], text=True)
        match = re.search(r"^([0-9a-fA-F]+)\s+R\s+gHealthboxElementsGfxTable$", symbols, re.M)
        self.assertIsNotNone(match)
        offset = int(match[1], 16) - 0x08000000
        rom = rom_path.read_bytes()
        source = (ROOT / "src/graphics.c").read_text(encoding="utf-8")
        table = re.search(r"const u8 gHealthboxElementsGfxTable\[\]\[32\] = INCBIN_U8\((.*?)\);", source, re.S)
        self.assertIsNotNone(table)
        files = re.findall(r'"([^"\n]+)"', table[1])
        variants = {f"status{suffix}.4bpp":battler for battler,suffix in enumerate(("", "2", "3", "4"))}
        found = 0
        for file in files:
            data = (ROOT / file).read_bytes()
            if Path(file).name in variants:
                battler = variants[Path(file).name]
                actual = rom[offset:offset + len(data)]
                self.assertEqual(actual, data, f"ROM uses stale data: {file}")
                for icon in range(6):
                    start = icon * status.ICON_SIZE
                    colors = {p for p in pixels(actual[start:start + status.ICON_SIZE]) if p >= 12}
                    self.assertEqual(colors, {12 + battler}, f"{file}, icon {icon}")
                found += 1
            offset += len(data)
        self.assertEqual(found, 4)


if __name__ == "__main__":
    unittest.main()
