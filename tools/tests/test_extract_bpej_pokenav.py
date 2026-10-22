from contextlib import redirect_stderr, redirect_stdout
from copy import deepcopy
import io
import json
import os
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
import extract_bpej_pokenav as pokenav

graphics = pokenav.graphics
MANIFEST = TOOLS / "data/bpej_pokenav_manifest.json"


class ManifestTests(unittest.TestCase):
    def setUp(self):
        self.manifest = pokenav.load_manifest(MANIFEST)

    def test_verified_scope_and_existing_condition_assets_are_separate(self):
        assets = self.manifest["assets"]
        self.assertEqual(len(assets), 91)
        self.assertEqual(sum(a["verification"]["byte_match"] for a in assets), 84)
        self.assertEqual({a["label"] for a in assets if not a["verification"]["byte_match"]}, {
            "gPokenavHeader_Gfx", "gPokenavLeftHeaderMatchCall_Gfx", "gPokenavLeftHeaderHoennMap_Gfx",
            "gPokenavLeftHeaderRibbons_Gfx", "gPokenavMessageBox_Tilemap", "gRegionMapCityZoomText_Gfx",
            "gPokenavRibbonsSummaryBg_Tilemap",
        })
        contest = graphics.load_manifest(TOOLS / "data/bpej_contest_manifest.json")
        self.assertTrue({a["label"] for a in assets}.isdisjoint(a["label"] for a in contest["assets"]))

    def test_declarations_preserve_types_dimensions_and_use_only_explicit_paths(self):
        for asset in self.manifest["assets"]:
            source = (ROOT / asset["consumer_file"]).read_text(encoding="utf-8")
            pattern = (rf"const {asset['element_type']}(?: ALIGNED\(4\))? {asset['label']}"
                       rf"{re.escape(asset['dimensions'])}\s*=\s*"
                       rf"INCBIN_{asset['element_type'].upper()}\(\"{re.escape(asset['output_path'])}\"\);")
            self.assertEqual(len(re.findall(pattern, source)), 1, asset["label"])

    def test_replaced_originals_are_absent_and_retained_additions_remain(self):
        self.assertEqual(len(self.manifest["removed_sources"]), 72)
        sources = {file: (ROOT / file).read_text(encoding="utf-8") for file in pokenav.CONSUMERS}
        ignores = (ROOT / ".gitignore").read_text(encoding="utf-8").splitlines()
        for record in self.manifest["removed_sources"]:
            self.assertFalse((ROOT / record["path"]).exists(), record["path"])
            self.assertIn("/" + record["path"], ignores)
            for file, source in sources.items():
                self.assertNotIn('"' + record["path"] + '"', source, file)
        self.assertEqual(len(self.manifest["retained_sources"]), 17)
        for record in self.manifest["retained_sources"]:
            self.assertTrue((ROOT / record["source_file"]).is_file(), record["source_file"])
            self.assertNotIn("/" + record["source_file"], ignores)

    def test_unverified_foreign_unsafe_or_unsupported_entries_are_rejected(self):
        cases = [("status", "candidate", "verified"),
                 ("consumer_file", "src/battle_interface.c", "out-of-scope"),
                 ("source_file", "graphics/pokemon/test.png", "invalid source"),
                 ("group", "opening", "invalid Pokenav group"),
                 ("label", "sRegionMapKanto_Pal", "FRLG"),
                 ("output_path", "build/extracted_pokenav/../../victim", "unsafe"),
                 ("compression", "unknown", "unsupported compression")]
        for field, value, message in cases:
            changed = deepcopy(self.manifest)
            changed["assets"][0][field] = value
            with tempfile.TemporaryDirectory() as directory:
                path = Path(directory) / "manifest.json"
                path.write_text(json.dumps(changed), encoding="utf-8")
                with self.assertRaisesRegex(ValueError, message):
                    pokenav.load_manifest(path)

    def test_make_clean_preserves_custom_directory_and_removes_extraction(self):
        source = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertIn("find . -path './graphics/custom' -prune", source)
        self.assertIn("clean-bpej-pokenav-assets:\n\trm -rf $(BPEJ_EXTRACTED_POKENAV_DIR)", source)
        self.assertIn("clean-generated: clean-bpej-pokenav-assets", source)
        self.assertIn("FORCE_BPEJ_POKENAV_ASSETS:", source)
        self.assertIn("$(BPEJ_POKENAV_EXTRACTED_OK) FORCE_BPEJ_POKENAV_ASSETS", source)


class RomTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if not (ROOT / "baserom.gba").is_file():
            raise unittest.SkipTest("place baserom.gba before ROM integration tests")
        cls.manifest = pokenav.load_manifest(MANIFEST)
        cls.rom = (ROOT / "baserom.gba").read_bytes()
        graphics.validate_baserom(cls.rom, cls.manifest, ROOT / "baserom.gba")
        cls.assets = {a["label"]: a for a in cls.manifest["assets"]}

    def decoded(self, label):
        asset = self.assets[label]
        data = graphics.rom_output(self.rom, asset)
        return graphics.decode_lz77(data, asset) if asset["compression"] == "gba_lz77" else data

    def invoke(self, *arguments):
        stdout, stderr = io.StringIO(), io.StringIO()
        with redirect_stdout(stdout), redirect_stderr(stderr):
            result = pokenav.main(["--manifest", str(MANIFEST), "--baserom", str(ROOT / "baserom.gba"), *arguments])
        return result, stdout.getvalue(), stderr.getvalue()

    def test_all_ranges_match_original_or_documented_japanese_replacement(self):
        for label, asset in self.assets.items():
            decoded = self.decoded(label)
            self.assertEqual(graphics.sha1(decoded), asset["expected_decoded_hash"], label)
            self.assertEqual(len(decoded), asset["verification"]["comparison_size"], label)
            if asset["verification"]["byte_match"]:
                self.assertEqual(graphics.sha1(decoded), asset["verification"]["comparison_sha1"], label)
            for ref in asset["verification"]["references"]:
                if "rom_pointer_offset" in ref:
                    pointer = struct.unpack_from("<I", self.rom, int(ref["rom_pointer_offset"], 16))[0]
                    self.assertEqual(pointer, int(asset["rom_offset"], 16) + 0x08000000, label)

    def test_paired_background_tilemaps_and_palette_banks_stay_in_bounds(self):
        pairs = [
            ("gPokenavHeader_Tilemap", "gPokenavHeader_Gfx", 0, {0}),
            ("gPokenavMessageBox_Tilemap", "gPokenavMessageBox_Gfx", 0, {0, 1}),
            ("gPokenavRibbonsSummaryBg_Tilemap", "gPokenavRibbonsSummaryBg_Gfx", 0, {1}),
            ("sPokenavBgDotsTilemap", "sPokenavBgDotsTiles", 0, {3}),
            ("sPokenavDeviceBgTilemap", "sPokenavDeviceBgTiles", 0, {2}),
            ("sMatchCallUI_Tilemap", "sMatchCallUI_Gfx", 128, {0, 2}),
            ("sMonRibbonListFrameTilemap", "sMonRibbonListFrameTiles", 0, {0, 1}),
            ("sRegionMapFrameTilemapLZ", "sRegionMapFrameGfxLZ", 0, {1}),
        ]
        pairs += [(label, "sRegionMapCityZoomTiles_Gfx", 0, {3})
                  for label in self.assets if label.startswith("gPokenavCityMap_")]
        self.assertEqual(len(pairs), 8 + 22)
        for map_label, gfx_label, base, banks in pairs:
            count = len(self.decoded(gfx_label)) // 32
            for (word,) in struct.iter_unpack("<H", self.decoded(map_label)):
                index = word & 1023
                if base and index == 0:
                    continue
                self.assertTrue(base <= index < base + count, (map_label, index, count))
                self.assertIn(word >> 12, banks, map_label)

    def test_affine_hoenn_map_and_palette_copy_size_are_compatible(self):
        gfx, tilemap = self.decoded("sRegionMapBg_GfxLZ"), self.decoded("sRegionMapBg_TilemapLZ")
        palette = self.decoded("sRegionMapBg_Pal")
        self.assertEqual(len(tilemap), 64 * 64)
        self.assertLess(max(tilemap), len(gfx) // 64)
        self.assertEqual(len(palette), 64)
        self.assertGreaterEqual(min(gfx), 7 * 16)
        self.assertLess(max(gfx), 7 * 16 + len(palette) // 2)
        source = (ROOT / "src/region_map.c").read_text(encoding="utf-8")
        for label in ("sRegionMapBg_Pal", "sRegionMapKanto_Pal", "sRegionMapSevii123_Pal",
                      "sRegionMapSevii45_Pal", "sRegionMapSevii67_Pal"):
            self.assertIn(f".regionMapPaletteSize = sizeof({label})", source)
        self.assertIn("BG_PLTT_ID(7), gRegionMapInfos[regionMapType].regionMapPaletteSize)", source)

    def test_text_windows_match_bpej_templates_and_do_not_replace_frame_edges(self):
        cases = [
            ("src/pokenav_menu_handler_gfx.c", "sOptionDescWindowTemplate", 0x5F2834,
             "gPokenavMessageBox_Tilemap", 0x1005, 0x1004, 0x1006),
            ("src/pokenav_ribbons_summary.c", "sRibbonSummaryMonNameWindowTemplate", 0x5F7240,
             "gPokenavRibbonsSummaryBg_Tilemap", 0x100A, 0x1009, 0x100B),
        ]
        for file, name, rom_offset, label, interior, left_edge, right_edge in cases:
            source = (ROOT / file).read_text(encoding="utf-8")
            template = re.search(rf"static const struct WindowTemplate {name}\s*=\s*\{{(.*?)\}};", source, re.S)
            self.assertIsNotNone(template)
            fields = ("bg", "tilemapLeft", "tilemapTop", "width", "height", "paletteNum", "baseBlock")
            values = tuple(int(re.search(rf"\.{field}\s*=\s*(0x[0-9A-Fa-f]+|\d+)", template[1])[1], 0)
                           for field in fields)
            self.assertEqual(values, struct.unpack_from("<6BH", self.rom, rom_offset), name)
            _, x, y, width, height, _, _ = values
            tilemap = struct.unpack("<640H", self.decoded(label))
            for row in range(y, y + height):
                self.assertEqual(tilemap[row * 32 + x - 1], left_edge, name)
                self.assertEqual(tilemap[row * 32 + x + width], right_edge, name)
                self.assertTrue(all(tilemap[row * 32 + col] == interior for col in range(x, x + width)), name)
            self.assertIn("GetWindowAttribute(", source)
        source = (ROOT / "src/pokenav_ribbons_summary.c").read_text(encoding="utf-8")
        self.assertIn("levelX = GetStringRightAlignXOffset(", source)
        self.assertIn("GetFontIdToFit(gStringVar3, FONT_NORMAL, 0, levelX - 4)", source)

    def test_left_header_sprite_tiles_and_rom_sheet_sizes_match(self):
        for label, size, second in (("gPokenavLeftHeaderMatchCall_Gfx", 2048, 32),
                                    ("gPokenavLeftHeaderRibbons_Gfx", 2048, 32),
                                    ("gPokenavLeftHeaderHoennMap_Gfx", 3072, 64)):
            self.assertEqual(len(self.decoded(label)), size)
            self.assertLessEqual((second + 32) * 32, size)
        pointer, size, tag = struct.unpack_from("<IHH", self.rom, 0x5F4D1C)
        self.assertEqual((pointer, size, tag), (0x085F4A98, 0x800, 6))

    def test_actual_city_zoom_sprite_callback_never_reads_past_64_tiles(self):
        compiler = shutil.which("cc") or shutil.which("gcc")
        if compiler is None:
            self.skipTest("a host C compiler is needed for the sprite callback execution test")
        source = (ROOT / "src/pokenav_region_map.c").read_text(encoding="utf-8")
        callback = re.search(r"^static void SpriteCB_CityZoomText\([^;]*?\)\n\{.*?^\}", source, re.M | re.S)
        self.assertIsNotNone(callback)
        self.assertIn(".shape = SPRITE_SHAPE(32x8)", source)
        self.assertIn("{gRegionMapCityZoomText_Gfx, 0x800, GFXTAG_CITY_ZOOM}", source)
        self.assertEqual(len(self.decoded("gRegionMapCityZoomText_Gfx")), 64 * 32)
        program = r"""
#include <assert.h>
#include <stdint.h>
struct Sprite { int16_t data[8]; struct { uint16_t tileNum; } oam; };
""" + callback[0] + r"""
int main(void)
{
    unsigned seen[61] = {0};
    for (int i = 0; i < 3; i++)
    {
        struct Sprite sprite = { .data = {0, i * 4, 100, 150, i * 4} };
        sprite.oam.tileNum = 100 + i * 4;
        for (int frame = 0; frame < 20000; frame++)
        {
            SpriteCB_CityZoomText(&sprite);
            int tile = sprite.oam.tileNum - 100;
            assert(tile >= 0 && tile + 4 <= 64);
            seen[tile]++;
        }
    }
    for (int tile = 0; tile <= 60; tile++)
        assert(seen[tile] > 0);
    return 0;
}
"""
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)
            cfile, binary = path / "callback.c", path / ("callback.exe" if sys.platform == "win32" else "callback")
            cfile.write_text(program, encoding="utf-8")
            subprocess.run([compiler, "-std=gnu17", "-Wall", "-Werror", str(cfile), "-o", str(binary)], check=True)
            subprocess.run([str(binary)], check=True)

    def test_sprite_frames_fit_original_allocations(self):
        frames = [("sSpinningPokenav_Gfx", 16, range(0, 128, 16)),
                  ("sRegionMapCursorSmallGfxLZ", 4, (0, 4)),
                  ("sRegionMapCursorLargeGfxLZ", 16, (0, 16, 32)),
                  ("sRibbonIconsBig_Gfx", 16, range(0, 192, 16))]
        for label, frame_tiles, starts in frames:
            count = len(self.decoded(label)) // 32
            self.assertTrue(all(start + frame_tiles <= count for start in starts), label)
        self.assertEqual(len(self.decoded("sRibbonIconsSmall_Gfx")), 12 * 2 * 32)
        self.assertEqual(len(self.decoded("sFlyTargetIcons_Gfx")), 14 * 32)

    def test_cli_count_dry_run_check_and_missing_or_corrupt_output_recovery(self):
        with tempfile.TemporaryDirectory() as directory:
            out, stamp = Path(directory) / "out", Path(directory) / "stamp.ok"
            args = ("--out", str(out), "--stamp", str(stamp))
            code, stdout, _ = self.invoke(*args, "--print-outputs")
            self.assertEqual(code, 0)
            self.assertEqual(len(stdout.splitlines()), 91)
            self.assertEqual(self.invoke(*args, "--dry-run")[0], 0)
            self.assertFalse(out.exists())
            self.assertEqual(self.invoke(*args)[0], 0)
            self.assertEqual(self.invoke(*args, "--check")[0], 0)
            times = {p: p.stat().st_mtime_ns for p in out.rglob("*") if p.is_file()}
            self.assertEqual(len(times), 91)
            self.assertEqual(self.invoke(*args)[0], 0)
            self.assertTrue(all(p.stat().st_mtime_ns == before for p, before in times.items()))
            victim = next(iter(times))
            victim.unlink()
            code, _, error = self.invoke(*args, "--check")
            self.assertEqual(code, 1)
            self.assertIn(str(victim), error)
            self.assertIn("baserom.gba", error)
            self.assertEqual(self.invoke(*args)[0], 0)
            victim.write_bytes(b"bad")
            self.assertEqual(self.invoke(*args, "--check")[0], 1)
            self.assertEqual(self.invoke(*args)[0], 0)

    def test_custom_override_and_invalid_rom_or_custom_are_nondestructive(self):
        previous = Path.cwd()
        with tempfile.TemporaryDirectory() as directory:
            try:
                os.chdir(directory)
                asset = self.assets["sListArrow_Pal"]
                custom = Path(asset["custom_path"])
                custom.parent.mkdir(parents=True)
                custom.write_bytes(bytes(32))
                out = Path(directory) / "out"
                args = ("--out", str(out), "--stamp", str(Path(directory) / "stamp.ok"))
                target = out.joinpath(*graphics.relative_path(asset["output_path"], pokenav.OUTPUT_ROOT).parts)
                self.assertEqual(self.invoke(*args)[0], 0)
                self.assertEqual(target.read_bytes(), bytes(32))
                self.assertEqual(self.invoke(*args, "--check")[0], 0)
                bad = Path(directory) / "bad.gba"
                bad.write_bytes(b"not a ROM")
                code, _, error = self.invoke(*args, "--baserom", str(bad))
                self.assertEqual(code, 1)
                self.assertIn("baserom.gba", error)
                self.assertEqual(target.read_bytes(), bytes(32))
                custom.write_bytes(b"invalid")
                self.assertEqual(self.invoke(*args)[0], 1)
                self.assertEqual(target.read_bytes(), bytes(32))
                custom.unlink()
                self.assertEqual(self.invoke(*args)[0], 0)
                self.assertEqual(target.read_bytes(), self.decoded(asset["label"]))
            finally:
                os.chdir(previous)

    def test_linked_rom_uses_the_selected_arrays(self):
        nm = shutil.which("arm-none-eabi-nm")
        if nm is None or not (ROOT / "pokeemerald.elf").is_file():
            self.skipTest("build the game and install ARM tools before linked-ROM checks")
        symbols = subprocess.check_output([nm, "-n", str(ROOT / "pokeemerald.elf")], text=True)
        addresses = {m[2]: int(m[1], 16) for m in re.finditer(r"^([0-9a-fA-F]+)\s+[rR]\s+(\w+)$", symbols, re.M)}
        built = (ROOT / "pokeemerald.gba").read_bytes()
        checked = set()
        for label, asset in self.assets.items():
            if not label.startswith(("gPokenav", "gRegionMap")) or label not in addresses:
                continue
            expected = graphics.desired_output(self.rom, asset)
            self.assertEqual((ROOT / asset["output_path"]).read_bytes(), expected, label)
            offset = addresses[label] - 0x08000000
            self.assertEqual(built[offset:offset + len(expected)], expected, label)
            checked.add(label)
        self.assertTrue({"gPokenavHeader_Gfx", "gPokenavLeftHeaderHoennMap_Gfx",
                         "gPokenavLeftHeaderMatchCall_Gfx", "gRegionMapCityZoomText_Gfx",
                         "gPokenavRibbonsSummaryBg_Tilemap"} <= checked)


if __name__ == "__main__":
    unittest.main()
