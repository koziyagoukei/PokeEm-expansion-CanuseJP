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
from unittest import mock

TOOLS = Path(__file__).resolve().parents[1]
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))
import extract_bpej_contest as contest

MANIFEST = ROOT / "tools/data/bpej_contest_manifest.json"


class ManifestTests(unittest.TestCase):
    def setUp(self):
        self.manifest = contest.load_manifest(MANIFEST)

    def rejected(self, manifest, pattern):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "manifest.json"
            path.write_text(json.dumps(manifest), encoding="utf-8")
            with self.assertRaisesRegex(ValueError, pattern):
                contest.load_manifest(path)

    def test_verified_sets_distinguish_translation_from_byte_matches(self):
        assets = self.manifest["assets"]
        self.assertEqual(len(assets), 85)
        self.assertEqual(sum(a["verification"]["byte_match"] for a in assets), 46)
        self.assertEqual(sum(a["verification"]["kind"] == "japanese_ui" for a in assets), 39)

    def test_every_asset_is_explicitly_included_with_the_original_c_type(self):
        for asset in self.manifest["assets"]:
            with self.subTest(label=asset["label"]):
                source = (ROOT / asset["consumer_file"]).read_text(encoding="utf-8")
                if asset.get("consumer_kind") == "make_input":
                    self.assertIn("$(types:%=$(TYPESGFXDIR)/%.4bpp)", source)
                    self.assertIn("$(contest_types:%=$(BPEJ_EXTRACTED_CONTEST_DIR)/types/contest_%.4bpp)", source)
                    continue
                declaration = (rf"const {asset['element_type']} {asset['label']}\[\]\s*=\s*"
                               rf"INCBIN_{asset['element_type'].upper()}\(\"{re.escape(asset['output_path'])}\"\);")
                self.assertEqual(len(re.findall(declaration, source)), 1)

    def test_unverified_assets_are_rejected(self):
        self.manifest["assets"][0]["status"] = "candidate"
        self.rejected(self.manifest, "verified")

    def test_japanese_replacements_require_evidence(self):
        asset = next(a for a in self.manifest["assets"] if a["verification"]["kind"] == "japanese_ui")
        asset["verification"]["references"] = []
        self.rejected(self.manifest, "provenance")

    def test_byte_match_evidence_must_match_decoded_hash(self):
        self.manifest["assets"][0]["verification"]["comparison_sha1"] = "0" * 40
        self.rejected(self.manifest, "evidence")

    def test_unknown_compression_is_rejected(self):
        self.manifest["assets"][0]["compression"] = "unknown"
        self.rejected(self.manifest, "unsupported compression")

    def test_unsafe_output_and_custom_paths_are_rejected(self):
        for field, value in (("output_path", "build/extracted_contest/../../victim"),
                             ("custom_path", "C:\\victim"), ("output_path", "/tmp/victim")):
            modified = deepcopy(self.manifest)
            modified["assets"][0][field] = value
            self.rejected(modified, "unsafe")

    def test_duplicate_outputs_and_labels_are_rejected(self):
        for field in ("output_path", "label"):
            modified = deepcopy(self.manifest)
            modified["assets"][1][field] = modified["assets"][0][field]
            if field == "output_path":
                modified["assets"][1]["custom_path"] = modified["assets"][0]["custom_path"]
            self.rejected(modified, "duplicate")

    def test_manifest_cannot_change_the_trusted_rom(self):
        self.manifest["baserom"]["sha1"] = "0" * 40
        self.rejected(self.manifest, "verified BPEJ")

    def test_unaligned_compressed_c_arrays_are_rejected(self):
        asset = next(a for a in self.manifest["assets"] if a["label"] == "sConditionGraphData_Gfx")
        asset["size"] = asset["compressed_payload_size"]
        self.rejected(self.manifest, "C element type")

    def test_bad_decoded_slice_is_rejected(self):
        asset = next(a for a in self.manifest["assets"] if a["transform"] == "gba_lz77_slice")
        asset["decoded_slice_offset"] = asset["source_decoded_size"]
        self.rejected(self.manifest, "slice range")


class DataTests(unittest.TestCase):
    def setUp(self):
        self.raw = b"\x12\x34\x56\x78"
        self.lz = b"\x10\x04\x00\x00\x00" + self.raw + b"\x00" * 3
        self.asset = {
            "label": "test", "rom_offset": 0, "size": len(self.lz), "compression": "gba_lz77",
            "compressed_payload_size": 9, "decoded_size": 4, "element_type": "u32", "transform": "copy",
            "output_representation": "gba_lz77_stream", "expected_source_hash": contest.sha1(self.lz),
            "expected_hash": contest.sha1(self.lz), "expected_decoded_hash": contest.sha1(self.raw),
        }

    def test_lz_copy_and_palette_decompression(self):
        self.assertEqual(contest.rom_output(self.lz, self.asset), self.lz)
        self.asset.update(transform="gba_lz77_decompress", output_representation="raw",
                          expected_hash=contest.sha1(self.raw))
        self.assertEqual(contest.rom_output(self.lz, self.asset), self.raw)

    def test_lz_slice_is_taken_after_full_source_validation(self):
        selected = self.raw[1:3]
        self.asset.update(transform="gba_lz77_slice", output_representation="raw", element_type="u8",
                          source_decoded_size=4, decoded_size=2, decoded_slice_offset=1,
                          expected_source_decoded_hash=contest.sha1(self.raw),
                          expected_hash=contest.sha1(selected), expected_decoded_hash=contest.sha1(selected))
        self.assertEqual(contest.rom_output(self.lz, self.asset), selected)
        self.asset["expected_source_decoded_hash"] = "0" * 40
        with self.assertRaisesRegex(ValueError, "full decoded source"):
            contest.rom_output(self.lz, self.asset)

    def test_wrong_range_or_hash_is_rejected(self):
        for update in ({"rom_offset": -1}, {"rom_offset": 99}, {"size": 100},
                       {"expected_source_hash": "0" * 40}, {"expected_decoded_hash": "0" * 40}):
            with self.subTest(update=update):
                with self.assertRaises(ValueError):
                    contest.rom_output(self.lz, dict(self.asset, **update))

    def test_malformed_lz_and_padding_are_rejected(self):
        for data in (b"", self.lz[:8], b"\x10\x04\x00\x00\x80\x00\x00\x00",
                     self.lz[:-1] + b"\x01", self.lz + b"\x00" * 4):
            with self.subTest(data=data):
                with self.assertRaises(ValueError):
                    contest.validate_output(data, self.asset)

    def test_failed_atomic_write_preserves_the_old_file(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "asset.bin"
            path.write_bytes(b"old")
            with mock.patch.object(Path, "replace", side_effect=OSError("test failure")):
                with self.assertRaises(OSError):
                    contest.write_if_changed(path, b"new")
            self.assertEqual(path.read_bytes(), b"old")
            self.assertEqual(list(Path(directory).iterdir()), [path])

    def test_unchanged_output_keeps_its_timestamp(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "asset.bin"
            contest.write_if_changed(path, self.raw)
            before = path.stat().st_mtime_ns
            contest.write_if_changed(path, self.raw)
            self.assertEqual(before, path.stat().st_mtime_ns)


class RomTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if not (ROOT / "baserom.gba").is_file():
            raise unittest.SkipTest("place baserom.gba before the ROM integration tests")
        cls.manifest = contest.load_manifest(MANIFEST)
        cls.rom = (ROOT / "baserom.gba").read_bytes()
        contest.validate_baserom(cls.rom, cls.manifest, ROOT / "baserom.gba")
        cls.assets = {a["label"]: a for a in cls.manifest["assets"]}

    def decoded(self, label):
        asset = self.assets[label]
        data = contest.rom_output(self.rom, asset)
        return contest.decode_lz77(data, asset) if asset["output_representation"] == "gba_lz77_stream" else data

    def invoke(self, *arguments):
        stdout, stderr = io.StringIO(), io.StringIO()
        with redirect_stdout(stdout), redirect_stderr(stderr):
            result = contest.main(["--manifest", str(MANIFEST), "--baserom", str(ROOT / "baserom.gba"), *arguments])
        return result, stdout.getvalue(), stderr.getvalue()

    def test_all_rom_assets_decode_to_the_manifest_hashes(self):
        for label, asset in self.assets.items():
            with self.subTest(label=label):
                self.assertEqual(contest.sha1(self.decoded(label)), asset["expected_decoded_hash"])

    def test_identical_assets_match_archived_original_source_fingerprints(self):
        # Fingerprints were captured and byte-compared before the originals were
        # removed. A fresh checkout needs only the user's ROM, not old PNGs.
        for label, asset in self.assets.items():
            if asset["verification"]["kind"] != "identical":
                continue
            with self.subTest(label=label):
                data = self.decoded(label)
                self.assertEqual(len(data), asset["verification"]["comparison_size"])
                self.assertEqual(contest.sha1(data), asset["verification"]["comparison_sha1"])

    def test_replaced_original_files_are_not_required_or_retained(self):
        removed = self.manifest["removed_sources"]
        self.assertEqual(len(removed), 86)
        for record in removed:
            self.assertFalse((ROOT / record["path"]).exists(), record["path"])
        source = (ROOT / "src/graphics.c").read_text(encoding="utf-8")
        self.assertIn('gPokenavOptions_Tilemap[] = INCBIN_U16("build/extracted_contest/pokenav/condition/search_options.bin")', source)
        rules = (ROOT / "graphics_file_rules.mk").read_text(encoding="utf-8")
        self.assertNotIn("PKNAVOPTIONSGFXDIR", rules)

    def assert_tilemap_fits(self, map_label, gfx_label, width, height, base=0):
        data = self.decoded(map_label)
        self.assertEqual(len(data), width * height * 2)
        count = len(self.decoded(gfx_label)) // 32
        for (word,) in struct.iter_unpack("<H", data):
            tile = word & 1023
            self.assertTrue(tile == 0 or base <= tile < base + count, (map_label, tile, base, count))

    def test_japanese_graph_and_overlay_tilemaps_fit_the_loaded_tiles(self):
        self.assert_tilemap_fits("gPokenavCondition_Tilemap", "gPokenavCondition_Gfx", 32, 32)
        self.assert_tilemap_fits("sConditionSearchOptions_Tilemap", "gPokenavCondition_Gfx", 9, 4)
        self.assert_tilemap_fits("gUsePokeblockGraph_Tilemap", "gUsePokeblockGraph_Gfx", 32, 20, 640)
        self.assert_tilemap_fits("gUsePokeblockNatureWin_Pal", "gUsePokeblockGraph_Gfx", 11, 4, 640)

    def test_every_rank_category_and_link_results_title_fits(self):
        for rank in ("Normal", "Super", "Hyper", "Master", "Link"):
            self.assert_tilemap_fits("gContestResultsTitle_" + rank + "_Tilemap", "gContestResults_Gfx", 8, 2)
        for category in ("Cool", "Beauty", "Cute", "Smart", "Tough", ""):
            name = "gContestResultsTitle_" + (category + "_" if category else "") + "Tilemap"
            self.assert_tilemap_fits(name, "gContestResults_Gfx", 5, 2)
        source = (ROOT / "src/contest_util.c").read_text(encoding="utf-8")
        for rank in ("Normal", "Super", "Hyper", "Master", "Link"):
            self.assertIn(f"gContestResultsTitle_{rank}_Tilemap, 5, 1, 8, 2)", source)
        self.assertIn("gContestResultsTitle_Tilemap, x, y, 5, 2)", source)

    def test_japanese_next_turn_read_sizes_and_number_slot(self):
        self.assertEqual(len(self.decoded("gContestNextTurnGfx")), 5 * 32)
        source = (ROOT / "src/contest.c").read_text(encoding="utf-8")
        self.assertEqual(len(re.findall(r"\.data = gContestNextTurnGfx,\s+\.size = 0xA0,", source)), 4)
        self.assertIn("nextTurnSpriteId].oam.tileNum + 2) * 32", source)
        self.assertEqual(len(self.decoded("gContestNextTurnNumbersGfx")), 4 * 32)
        self.assertEqual(len(self.decoded("gContestNextTurnRandomGfx")), 32)
        self.assertEqual(struct.unpack_from("<HH", self.rom, 0x560A7C), (0x4E22, 0x4E22))

    def test_japanese_options_fit_all_thirteen_existing_sprite_slots(self):
        self.assertEqual(len(self.decoded("gPokenavOptions_Gfx")), 13 * 32 * 32)
        self.assertEqual(len(self.decoded("gPokenavOptions_Pal")), 5 * 32)
        for asset in self.assets.values():
            for reference in asset["verification"]["references"]:
                if "table_index" in reference:
                    offset = int(reference["table_rom_offset"], 16) + reference["table_index"] * 8
                    self.assertEqual(struct.unpack_from("<I", self.rom, offset)[0],
                                     int(asset["rom_offset"], 16) + 0x08000000, asset["label"])

    def test_pokeblock_graph_uses_its_own_japanese_palette_and_copy_sizes(self):
        self.assertEqual(int(self.assets["gUsePokeblockGraph_Pal"]["rom_offset"], 16), 0x5BFA4C)
        self.assertNotEqual(self.decoded("gUsePokeblockGraph_Pal"), self.decoded("gPokenavCondition_Pal"))
        self.assertEqual(len(self.decoded("gUsePokeblockGraph_Gfx")), 4000)
        source = (ROOT / "src/use_pokeblock.c").read_text(encoding="utf-8")
        self.assertIn("Alloc(GetDecompressedDataSize(gUsePokeblockGraph_Gfx))", source)
        self.assertIn("LoadBgTiles(1, sGraph_Gfx, GetDecompressedDataSize(gUsePokeblockGraph_Gfx)", source)
        self.assertIn("gUsePokeblockNatureWin_Pal, 0, 13, 11, 4)", source)

    def test_summary_badges_keep_all_expansion_types_and_their_palette(self):
        palette = self.rom[0xD97C54:0xD97D54]
        size = contest.gba_lz77_stream_length(palette, "type palette")
        decoded = contest.decode_lz77(palette[:size], {"label": "type palette", "decoded_size": 96})
        self.assertEqual(decoded, (ROOT / "graphics/types/move_types.gbapal").read_bytes())
        source = (ROOT / "graphics_file_rules.mk").read_text(encoding="utf-8")
        names = re.search(r"^types := (.*)$", source, re.M)[1].split()
        self.assertEqual(len(names), 21)
        original = b"".join((ROOT / "graphics/types" / (name + ".4bpp")).read_bytes() for name in names)
        badges = []
        for category, name in enumerate(("cool", "beauty", "cute", "smart", "tough")):
            asset = self.assets["bpej_contest_badge_" + name]
            reference = asset["verification"]["references"][0]
            index = 18 + category
            command = struct.unpack_from("<I", self.rom, 0x5ED3B0 + index * 4)[0] - 0x08000000
            tile = struct.unpack_from("<H", self.rom, command)[0]
            self.assertEqual(reference["animation_index"], index)
            self.assertEqual(tile, index * 8)
            self.assertEqual(asset["decoded_slice_offset"], tile * 32)
            self.assertEqual(reference["palette_index"], (13, 14, 14, 15, 13)[category])
            badge = self.decoded(asset["label"])
            self.assertEqual(len(badge), 256)
            badges.append(badge)
        composite = (ROOT / "graphics/types/move_types.4bpp").read_bytes()
        self.assertEqual(composite, original + b"".join(badges))

    def test_cli_outputs_dry_run_check_corruption_and_timestamps(self):
        with tempfile.TemporaryDirectory() as directory:
            out = Path(directory) / "out"
            stamp = Path(directory) / "stamp.ok"
            args = ("--out", str(out), "--stamp", str(stamp))
            code, stdout, _ = self.invoke(*args, "--print-outputs")
            self.assertEqual(code, 0)
            self.assertEqual(len(stdout.splitlines()), 85)
            self.assertEqual(self.invoke(*args, "--dry-run")[0], 0)
            self.assertFalse(out.exists())
            self.assertFalse(stamp.exists())
            self.assertEqual(self.invoke(*args)[0], 0)
            self.assertEqual(self.invoke(*args, "--check")[0], 0)
            times = {p: p.stat().st_mtime_ns for p in out.rglob("*") if p.is_file()}
            self.assertEqual(self.invoke(*args)[0], 0)
            self.assertTrue(all(p.stat().st_mtime_ns == before for p, before in times.items()))
            victim = next(iter(times))
            victim.unlink()
            code, _, stderr = self.invoke(*args, "--check")
            self.assertEqual(code, 1)
            self.assertIn(str(victim), stderr)
            self.assertEqual(self.invoke(*args)[0], 0)
            victim.write_bytes(b"bad")
            self.assertEqual(self.invoke(*args, "--check")[0], 1)
            self.assertEqual(self.invoke(*args)[0], 0)
            stamp.write_text("{}")
            self.assertEqual(self.invoke(*args, "--check")[0], 1)

    def test_custom_override_is_validated_and_removal_restores_rom_data(self):
        previous = Path.cwd()
        with tempfile.TemporaryDirectory() as directory:
            try:
                os.chdir(directory)
                asset = self.assets["gContestPal"]
                custom = Path(asset["custom_path"])
                custom.parent.mkdir(parents=True)
                custom.write_bytes(bytes(32))
                out = Path(directory) / "out"
                args = ("--out", str(out), "--stamp", str(Path(directory) / "stamp.ok"))
                self.assertEqual(self.invoke(*args)[0], 0)
                target = out.joinpath(*contest.relative_path(asset["output_path"], contest.OUTPUT_ROOT).parts)
                self.assertEqual(target.read_bytes(), bytes(32))
                self.assertEqual(self.invoke(*args, "--check")[0], 0)
                custom.write_bytes(b"invalid")
                self.assertEqual(self.invoke(*args)[0], 1)
                self.assertEqual(target.read_bytes(), bytes(32))
                custom.unlink()
                self.assertEqual(self.invoke(*args, "--check")[0], 1)
                self.assertEqual(self.invoke(*args)[0], 0)
                self.assertEqual(target.read_bytes(), self.decoded("gContestPal"))
            finally:
                os.chdir(previous)

    def test_bad_rom_cannot_create_output_files(self):
        with tempfile.TemporaryDirectory() as directory:
            bad = Path(directory) / "bad.gba"
            bad.write_bytes(b"bad ROM")
            out = Path(directory) / "out"
            code, _, stderr = self.invoke("--baserom", str(bad), "--out", str(out))
            self.assertEqual(code, 1)
            self.assertIn("baserom.gba", stderr)
            self.assertFalse(out.exists())

    def test_linked_rom_uses_the_extracted_global_arrays(self):
        nm = shutil.which("arm-none-eabi-nm")
        if nm is None or not (ROOT / "pokeemerald.elf").is_file():
            self.skipTest("build the game and install ARM tools before linked-ROM checks")
        symbols = subprocess.check_output([nm, "-n", str(ROOT / "pokeemerald.elf")], text=True)
        addresses = {m[2]: int(m[1], 16) for m in re.finditer(r"^([0-9a-fA-F]+)\s+[rR]\s+(\w+)$", symbols, re.M)}
        built = (ROOT / "pokeemerald.gba").read_bytes()
        checked = set()
        for label, asset in self.assets.items():
            if not label.startswith("g") or label not in addresses:
                continue
            data = (ROOT / asset["output_path"]).read_bytes()
            custom = ROOT / asset["custom_path"]
            expected = custom.read_bytes() if custom.is_file() else contest.rom_output(self.rom, asset)
            self.assertEqual(data, expected, f"stale extraction: {label}")
            offset = addresses[label] - 0x08000000
            self.assertEqual(built[offset:offset + len(data)], data, label)
            checked.add(label)
        self.assertTrue({"gContestInterfaceGfx", "gContestResults_Gfx", "gPokenavCondition_Gfx",
                         "gUsePokeblockGraph_Gfx", "gPokenavOptions_Gfx"} <= checked)


if __name__ == "__main__":
    unittest.main()
