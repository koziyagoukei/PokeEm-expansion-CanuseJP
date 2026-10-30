"""BPEJ berry extraction, ownership, sprite geometry and palette bounds."""
from collections import Counter
from contextlib import redirect_stderr, redirect_stdout
from copy import deepcopy
import io
import json
import os
from pathlib import Path
import re
import struct
import sys
import tempfile
import unittest

TOOLS = Path(__file__).resolve().parents[1]
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))
import extract_bpej_berry as berry

graphics = berry.graphics
MANIFEST = TOOLS / "data/bpej_berry_manifest.json"


class ManifestTests(unittest.TestCase):
    def setUp(self):
        self.manifest = berry.load_manifest(MANIFEST)

    def test_scope_and_explicit_complete_japanese_replacements(self):
        assets = self.manifest["assets"]
        self.assertEqual(len(assets), 240)
        self.assertEqual(Counter(a["group"] for a in assets), {
            "tag": 6, "berries": 86, "blender": 15, "crush": 13, "item_icons": 88, "trees": 32})
        self.assertEqual({a["label"] for a in assets if a["verification"]["kind"] == "japanese_ui"}, berry.JAPANESE_REPLACEMENTS)
        self.assertEqual(sum(a["verification"]["byte_match"] for a in assets), 235)
        self.assertTrue(all(a["transform"] in ("copy", "gba_lz77_decompress") for a in assets))

    def test_only_explicit_include_paths_change_and_array_contracts_remain(self):
        for asset in self.manifest["assets"]:
            source = (ROOT / asset["consumer_file"]).read_text(encoding="utf-8")
            pattern = (rf'\bconst {asset["element_type"]} {asset["label"]}'
                       + re.escape(asset["dimensions"]) + r'\s*=\s*'
                       + rf'INCBIN_{asset["element_type"].upper()}\("{re.escape(asset["output_path"])}"\);')
            self.assertEqual(len(re.findall(pattern, source)), 1, asset["label"])
            if asset["group"] == "trees" or asset["element_type"] == "u16":
                self.assertEqual(asset["output_representation"], "raw", asset["label"])

    def test_only_fully_replaced_originals_are_removed_or_ignored(self):
        removed = self.manifest["removed_sources"]
        self.assertEqual(len(removed), 191)
        ignore = set((ROOT / ".gitignore").read_text(encoding="utf-8").splitlines())
        consumers = [(ROOT / f).read_text(encoding="utf-8") for f in berry.CONSUMERS]
        for record in removed:
            self.assertFalse((ROOT / record["path"]).exists(), record["path"])
            self.assertIn("/" + record["path"], ignore)
            self.assertTrue(all('"' + record["path"] + '"' not in source for source in consumers))
        for record in self.manifest["retained_sources"]:
            self.assertTrue((ROOT / record["source_file"]).is_file(), record["source_file"])
            self.assertNotIn("/" + record["source_file"], ignore)
        self.assertNotIn("/graphics/berries/", ignore)
        self.assertNotIn("/graphics/items/", ignore)
        self.assertIn("/build/extracted_berry/", ignore)

    def test_unknown_or_out_of_scope_entries_are_rejected(self):
        cases = [("status", "candidate", "verified"), ("compression", "unknown", "unsupported"),
                 ("source_file", "graphics/pokemon/test.png", "invalid source"),
                 ("consumer_file", "src/pokemon.c", "out-of-scope"),
                 ("group", "audio", "invalid berry"),
                 ("output_path", "build/extracted_berry/../../victim", "unsafe"),
                 ("label", "gItemIcon_BerryPouch", "pouch")]
        for field, value, message in cases:
            with self.subTest(field=field), tempfile.TemporaryDirectory() as directory:
                manifest = deepcopy(self.manifest)
                manifest["assets"][0][field] = value
                path = Path(directory) / "manifest.json"
                path.write_text(json.dumps(manifest), encoding="utf-8")
                with self.assertRaisesRegex(ValueError, message):
                    berry.load_manifest(path)

    def test_make_dependencies_clean_and_custom_directory(self):
        source = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertIn("AUTO_GEN_TARGETS += $(BPEJ_BERRY_EXTRACTED_OK)", source)
        self.assertIn("FORCE_BPEJ_BERRY_ASSETS:", source)
        self.assertIn("$(BPEJ_EXTRACTED_BERRY_DIR)/%: $(BPEJ_BERRY_EXTRACTED_OK) FORCE_BPEJ_BERRY_ASSETS", source)
        self.assertIn("| check-bpej-berry-outputs", source)
        self.assertIn("clean-generated: clean-bpej-berry-assets", source)
        clean = re.search(r"clean-bpej-berry-assets:\n(.*?)(?=\n\S)", source, re.S)[1]
        self.assertIn("rm -rf $(BPEJ_EXTRACTED_BERRY_DIR)", clean)
        self.assertNotIn("graphics/custom", clean)
        self.assertIn("find . -path './graphics/custom' -prune", source)


class RomTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if not (ROOT / "baserom.gba").exists():
            raise unittest.SkipTest("verified BPEJ baserom.gba required")
        cls.manifest = berry.load_manifest(MANIFEST)
        cls.rom = (ROOT / "baserom.gba").read_bytes()
        graphics.validate_baserom(cls.rom, cls.manifest, ROOT / "baserom.gba")
        cls.assets = {a["label"]: a for a in cls.manifest["assets"]}

    def decoded(self, label):
        asset = self.assets[label]
        data = graphics.rom_output(self.rom, asset)
        return graphics.decode_lz77(data, asset) if asset["output_representation"] == "gba_lz77_stream" else data

    def invoke(self, *arguments):
        stdout, stderr = io.StringIO(), io.StringIO()
        with redirect_stdout(stdout), redirect_stderr(stderr):
            code = berry.main(["--manifest", str(MANIFEST), "--baserom", str(ROOT / "baserom.gba"), *arguments])
        return code, stdout.getvalue(), stderr.getvalue()

    def test_every_asset_hash_size_and_documented_pointer(self):
        for label, asset in self.assets.items():
            with self.subTest(label=label):
                data = self.decoded(label)
                self.assertEqual(len(data), asset["decoded_size"])
                self.assertEqual(graphics.sha1(data), asset["expected_decoded_hash"])
                self.assertEqual(len(data), asset["verification"]["comparison_size"])
                if asset["verification"]["byte_match"]:
                    self.assertEqual(graphics.sha1(data), asset["verification"]["comparison_sha1"])
                for ref in asset["verification"]["references"]:
                    if "rom_pointer_offset" in ref:
                        pointer = struct.unpack_from("<I", self.rom, int(ref["rom_pointer_offset"], 16))[0]
                        self.assertEqual(pointer, 0x08000000 + int(asset["rom_offset"], 16))

    def test_43_berry_picture_and_palette_pairs_use_the_same_rom_table_owner(self):
        pictures = {label.removeprefix("gBerryPic_"): asset for label, asset in self.assets.items() if label.startswith("gBerryPic_")}
        self.assertEqual(len(pictures), 43)
        indices = set()
        for suffix, picture in pictures.items():
            palette = self.assets["gBerryPalette_" + suffix]
            a, b = picture["verification"]["references"][0], palette["verification"]["references"][0]
            self.assertEqual(a["berry_table_index"], b["berry_table_index"])
            self.assertEqual(int(b["rom_pointer_offset"], 16), int(a["rom_pointer_offset"], 16) + 4)
            self.assertEqual(len(self.decoded(picture["label"])), 48 * 48 // 2)
            self.assertEqual(len(self.decoded(palette["label"])), 32)
            indices.add(a["berry_table_index"])
        self.assertEqual(indices, set(range(43)))

    def test_background_tile_and_palette_indices_fit_complete_paired_sets(self):
        pairs = [("gBerryTag_Gfx", "gBerryCheck_Gfx", 6), ("gBerryTag_Tilemap", "gBerryCheck_Gfx", 6),
                 ("gBerryBlenderOuter_Tilemap", "gBerryBlenderOuter_Gfx", 9),
                 ("sCrusherTop_Tilemap", "gBerryCrush_Crusher_Gfx", 12),
                 ("sContainerCap_Tilemap", "gBerryCrush_Crusher_Gfx", 12),
                 ("sBg_Tilemap", "gBerryCrush_Crusher_Gfx", 12),
                 ("gBerryCrush_TextWindows_Tilemap", "gBerryCrush_Crusher_Gfx", 12)]
        for map_label, tile_label, palette_count in pairs:
            tiles = len(self.decoded(tile_label)) // 32
            for (word,) in struct.iter_unpack("<H", self.decoded(map_label)):
                self.assertLess(word & 1023, tiles, map_label)
                self.assertLess(word >> 12, palette_count, map_label)

    def test_japanese_circle_shape_frame_and_position_match_bpej(self):
        template = struct.unpack_from("<HH5I", self.rom, 0x55A940)
        a0, a1 = struct.unpack_from("<HH", self.rom, template[2] - 0x08000000)
        self.assertEqual((a0 >> 14, a1 >> 14), (0, 2))  # 32x32 square
        self.assertEqual(len(self.decoded("gBerryCheckCircle_Gfx")), 2048)
        source = (ROOT / "src/item_menu_icons.c").read_text(encoding="utf-8")
        oam = re.search(r"sBerryCheckCircleOamData\s*=\s*\{(.*?)\};", source, re.S)[1]
        self.assertIn("SPRITE_SHAPE(32x32)", oam)
        self.assertIn("SPRITE_SIZE(32x32)", oam)
        self.assertIn("CreateSprite(&sBerryCheckCircleSpriteTemplate, x, 99, 0)", source)
        instructions = struct.unpack("<40H", self.rom[0x178254:0x1782A4])
        expected = [word & 255 for word in instructions if word & 0xFF00 == 0x2000]
        tag_source = (ROOT / "src/berry_tag_screen.c").read_text(encoding="utf-8")
        actual = list(map(int, re.findall(r"= CreateBerryFlavorCircleSprite\((\d+)\);", tag_source)))
        self.assertEqual(actual, expected)
        self.assertEqual(actual, [60, 92, 124, 156, 188])
        words = struct.unpack("<14H", self.rom[0xD4A10:0xD4A2C])
        self.assertEqual([v & 255 for v in words if v & 0xFF00 == 0x2200], [99])

    def test_blender_start_sheet_and_tree_frame_allocations_match_rom(self):
        pointer, size, _ = struct.unpack_from("<IHH", self.rom, 0x30FB48)
        self.assertEqual(pointer - 0x08000000, int(self.assets["gBerryBlenderStart_Gfx"]["rom_offset"], 16))
        self.assertEqual(len(self.decoded("gBerryBlenderStart_Gfx")), size)
        template = struct.unpack_from("<HH5I", self.rom, 0x30FB50)
        a0, a1 = struct.unpack_from("<HH", self.rom, template[2] - 0x08000000)
        self.assertEqual((a0 >> 14, a1 >> 14), (1, 3))  # 64x32
        for asset in self.assets.values():
            if asset["group"] == "trees":
                ref = asset["verification"]["references"][0]
                frame_size = struct.unpack_from("<I", self.rom, int(ref["rom_pointer_offset"], 16) + 4)[0]
                self.assertIn(frame_size, (128, 256))
                self.assertEqual(asset["decoded_size"] % frame_size, 0)

    def test_palette_reads_do_not_depend_on_adjacent_linked_arrays(self):
        self.assertLessEqual(max(self.decoded("gBerryBlenderCenter_Gfx")), 15)
        self.assertEqual(len(self.decoded("sBlenderCenter_Pal")), 32)
        source = (ROOT / "src/berry_blender.c").read_text(encoding="utf-8")
        self.assertIn("LoadPalette(sBlenderCenter_Pal, BG_PLTT_ID(0), sizeof(sBlenderCenter_Pal))", source)
        labels = ("gBerryCrush_Crusher_Pal", "sCrusherBase_Pal", "sEffects_Pal", "sTimerDigits_Pal")
        combined = b"".join(self.decoded(label) for label in labels)
        self.assertEqual(combined, self.rom[0x2C5F84:0x2C6104])
        self.assertEqual(len(combined), 12 * 32)
        source = (ROOT / "src/berry_crush.c").read_text(encoding="utf-8")
        self.assertIn("LoadPalette(gBerryCrush_Crusher_Pal, BG_PLTT_ID(0), 9 * PLTT_SIZE_4BPP)", source)
        for bank, label in enumerate(labels[1:], 9):
            self.assertIn(f"LoadPalette({label}, BG_PLTT_ID({bank}), sizeof({label}))", source)

    def test_cli_dry_run_outputs_check_and_repair(self):
        with tempfile.TemporaryDirectory() as directory:
            out, stamp = Path(directory) / "out", Path(directory) / "stamp.ok"
            args = ("--out", str(out), "--stamp", str(stamp))
            code, stdout, _ = self.invoke(*args, "--print-outputs")
            self.assertEqual(code, 0)
            self.assertEqual(len(stdout.splitlines()), 240)
            self.assertEqual(self.invoke(*args, "--dry-run")[0], 0)
            self.assertFalse(out.exists())
            self.assertEqual(self.invoke(*args)[0], 0)
            self.assertEqual(self.invoke(*args, "--check")[0], 0)
            paths = [p for p in out.rglob("*") if p.is_file()]
            times = {p: p.stat().st_mtime_ns for p in paths}
            self.assertEqual(len(paths), 240)
            self.assertEqual(self.invoke(*args)[0], 0)
            self.assertEqual({p: p.stat().st_mtime_ns for p in paths}, times)
            victim = paths[0]
            victim.unlink()
            code, _, error = self.invoke(*args, "--check")
            self.assertEqual(code, 1)
            self.assertIn(str(victim), error)
            self.assertIn("baserom.gba", error)
            self.assertEqual(self.invoke(*args)[0], 0)
            victim.write_bytes(b"corrupt")
            self.assertEqual(self.invoke(*args, "--check")[0], 1)
            self.assertEqual(self.invoke(*args)[0], 0)

    def test_custom_override_bad_custom_and_bad_rom_are_nondestructive(self):
        previous = Path.cwd()
        with tempfile.TemporaryDirectory() as directory:
            try:
                os.chdir(directory)
                asset = self.assets["sFontPalette"]
                custom = Path(asset["custom_path"])
                custom.parent.mkdir(parents=True)
                custom.write_bytes(bytes(32))
                out, stamp = Path(directory) / "out", Path(directory) / "stamp.ok"
                args = ("--out", str(out), "--stamp", str(stamp))
                target = out.joinpath(*graphics.relative_path(asset["output_path"], berry.OUTPUT_ROOT).parts)
                self.assertEqual(self.invoke(*args)[0], 0)
                self.assertEqual(target.read_bytes(), bytes(32))
                self.assertEqual(self.invoke(*args, "--check")[0], 0)
                custom.write_bytes(b"bad")
                self.assertEqual(self.invoke(*args)[0], 1)
                self.assertEqual(target.read_bytes(), bytes(32))
                custom.write_bytes(bytes(32))
                invalid = Path(directory) / "bad.gba"
                invalid.write_bytes(b"not the verified ROM")
                code, _, error = self.invoke(*args, "--baserom", str(invalid))
                self.assertEqual(code, 1)
                self.assertIn("日本版エメラルドROM", error)
                self.assertEqual(target.read_bytes(), bytes(32))
            finally:
                os.chdir(previous)


if __name__ == "__main__":
    unittest.main()
