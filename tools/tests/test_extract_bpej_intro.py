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
import extract_bpej_intro as intro

graphics = intro.graphics
MANIFEST = TOOLS / "data/bpej_intro_manifest.json"


class ManifestTests(unittest.TestCase):
    def setUp(self):
        self.manifest = intro.load_manifest(MANIFEST)

    def rejected(self, manifest, pattern):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "manifest.json"
            path.write_text(json.dumps(manifest), encoding="utf-8")
            with self.assertRaisesRegex(ValueError, pattern):
                intro.load_manifest(path)

    def test_only_documented_verified_opening_and_title_assets_are_selected(self):
        assets = self.manifest["assets"]
        self.assertEqual(len(assets), 98)
        self.assertEqual(sum(a["group"] == "opening" for a in assets), 59)
        self.assertEqual(sum(a["group"] == "intro_credits" for a in assets), 32)
        self.assertEqual(sum(a["group"] == "title_screen" for a in assets), 7)
        self.assertEqual(sum(a["verification"]["byte_match"] for a in assets), 96)
        self.assertEqual({a["label"] for a in assets if not a["verification"]["byte_match"]},
                         {"gIntroCopyright_Gfx", "gTitleScreenPressStartGfx"})

    def test_all_declarations_keep_their_types_dimensions_and_explicit_paths(self):
        for asset in self.manifest["assets"]:
            with self.subTest(label=asset["label"]):
                source = (ROOT / asset["consumer_file"]).read_text(encoding="utf-8")
                expression = (rf"const {asset['element_type']} {asset['label']}"
                              rf"{re.escape(asset['dimensions'])}\s*=\s*"
                              rf"INCBIN_{asset['element_type'].upper()}\(\"{re.escape(asset['output_path'])}\"\);")
                self.assertEqual(len(re.findall(expression, source)), 1)

    def test_source_assets_are_removed_and_no_longer_included(self):
        removed = self.manifest["removed_sources"]
        self.assertEqual(len(removed), 77)
        sources = {file: (ROOT / file).read_text(encoding="utf-8") for file in intro.CONSUMERS}
        for record in removed:
            self.assertFalse((ROOT / record["path"]).exists(), record["path"])
            for file, source in sources.items():
                self.assertNotIn('"' + record["path"] + '"', source, file)

    def test_existing_custom_title_and_expansion_intro_are_not_replaced(self):
        labels = {a["label"] for a in self.manifest["assets"]}
        self.assertTrue(labels.isdisjoint(self.manifest["retained_custom_title"]))
        source = (ROOT / "src/title_screen.c").read_text(encoding="utf-8")
        self.assertIn("#define VERSION_BANNER_LEFT_X 98", source)
        self.assertIn("#define VERSION_BANNER_RIGHT_X 162", source)
        self.assertIn("#define VERSION_BANNER_Y_GOAL 66", source)
        self.assertIn("yPos = gTasks[taskId].tBg2Y * 256", source)
        graphics_source = (ROOT / "src/graphics.c").read_text(encoding="utf-8")
        self.assertIn('gTitleScreenPokemonLogoGfx[]     = INCGFX_U32("graphics/title_screen/pokemon_logo.png"', graphics_source)
        self.assertIn('gTitleScreenEmeraldVersionGfx[]  = INCGFX_U32("graphics/title_screen/emerald_version.png"', graphics_source)
        self.assertTrue((ROOT / "graphics/title_screen/pokemon_logo.png").is_file())
        expansion = (ROOT / "src/expansion_intro.c").read_text(encoding="utf-8")
        self.assertNotIn("build/extracted_intro/", expansion)
        for path in re.findall(r'INCGFX_U\d+\("([^"]+)"', expansion):
            self.assertTrue((ROOT / path).is_file(), path)

    def test_title_uses_the_verified_bpej_2004_banner_without_an_override(self):
        assets = {a["label"]: a for a in self.manifest["assets"]}
        for label in ("gTitleScreenPressStartGfx", "gTitleScreenPressStartPal"):
            self.assertFalse((ROOT / assets[label]["custom_path"]).exists(), label)
        self.assertFalse((ROOT / "graphics/custom/bpej/intro/title_screen/press_start.png").exists())

    def test_unverified_or_out_of_scope_assets_are_rejected(self):
        for field, value, message in (("status", "partial", "verified"),
                                      ("source_file", "graphics/pokemon/example.png", "invalid source"),
                                      ("consumer_file", "src/expansion_intro.c", "out-of-scope"),
                                      ("group", "battle", "invalid opening/title group")):
            modified = deepcopy(self.manifest)
            modified["assets"][0][field] = value
            self.rejected(modified, message)

    def test_paths_compression_and_missing_provenance_are_rejected(self):
        for field, value, message in (("output_path", "build/extracted_intro/../../victim", "unsafe"),
                                      ("custom_path", "graphics/custom/bpej/intro/../victim", "unsafe"),
                                      ("compression", "unknown", "unsupported compression")):
            modified = deepcopy(self.manifest)
            modified["assets"][0][field] = value
            self.rejected(modified, message)
        self.manifest["assets"][0]["verification"]["references"] = []
        self.rejected(self.manifest, "reference evidence")

    def test_make_clean_excludes_custom_sources_and_removes_only_generated_intro_data(self):
        source = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertIn("find . -path './graphics/custom' -prune", source)
        self.assertIn("clean-bpej-intro-assets:\n\trm -rf $(BPEJ_EXTRACTED_INTRO_DIR)", source)
        self.assertIn("clean-generated: clean-teachables_intermediates clean-bpej-contest-assets clean-bpej-intro-assets", source)
        shell = shutil.which("sh")
        if shell is None:
            self.skipTest("a POSIX shell is needed to exercise the clean recipe")
        command = next(line.strip() for line in source.splitlines()
                       if line.startswith("\tfind . -path './graphics/custom'"))
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            custom = root / "graphics/custom/bpej/intro/test.4bpp"
            private = root / ".private/frontier_dev/archive.4bpp"
            generated = root / "graphics/example.4bpp"
            custom.parent.mkdir(parents=True)
            private.parent.mkdir(parents=True)
            custom.write_bytes(b"custom")
            private.write_bytes(b"private")
            generated.write_bytes(b"generated")
            subprocess.run([shell, "-c", command], cwd=root, check=True)
            self.assertEqual(custom.read_bytes(), b"custom")
            self.assertEqual(private.read_bytes(), b"private")
            self.assertFalse(generated.exists())


class RomTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if not (ROOT / "baserom.gba").is_file():
            raise unittest.SkipTest("place baserom.gba before ROM integration tests")
        cls.manifest = intro.load_manifest(MANIFEST)
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
            result = intro.main(["--manifest", str(MANIFEST), "--baserom", str(ROOT / "baserom.gba"), *arguments])
        return result, stdout.getvalue(), stderr.getvalue()

    def test_all_ranges_decode_and_match_the_archived_original_fingerprints(self):
        for label, asset in self.assets.items():
            with self.subTest(label=label):
                decoded = self.decoded(label)
                self.assertEqual(graphics.sha1(decoded), asset["expected_decoded_hash"])
                self.assertEqual(len(decoded), asset["decoded_size"])
                if asset["verification"]["byte_match"]:
                    self.assertEqual(graphics.sha1(decoded), asset["verification"]["comparison_sha1"])
                    self.assertEqual(len(decoded), asset["verification"]["comparison_size"])

    def test_title_banner_selection_is_byte_identical_to_the_verified_japanese_rom(self):
        for label in ("gTitleScreenPressStartGfx", "gTitleScreenPressStartPal"):
            asset = self.assets[label]
            self.assertEqual(graphics.desired_output(self.rom, asset), graphics.rom_output(self.rom, asset))

    def test_copyright_image_and_scene1_tilemaps_do_not_reference_missing_tiles(self):
        pairs = [("gIntroCopyright_Tilemap", "gIntroCopyright_Gfx")]
        pairs += [(f"sIntro1Bg{n}_Tilemap", "sIntro1Bg_Gfx") for n in range(4)]
        for map_label, gfx_label in pairs:
            count = len(self.decoded(gfx_label)) // 32
            for (word,) in struct.iter_unpack("<H", self.decoded(map_label)):
                self.assertLess(word & 1023, count, map_label)

    def test_title_banner_frame_layout_and_single_palette_bank_are_compatible(self):
        pointer, size, tag = struct.unpack_from("<IHH", self.rom, 0x51AD8C)
        self.assertEqual(pointer, 0x085176F4)
        self.assertEqual(size, 0x520)
        self.assertGreaterEqual(len(self.decoded("gTitleScreenPressStartGfx")), size)
        self.assertEqual(len(self.decoded("gTitleScreenPressStartPal")), 32)
        source = (ROOT / "src/title_screen.c").read_text(encoding="utf-8")
        self.assertIn(".size = 0x520", source)
        for frame in range(1, 38, 4):
            self.assertIn(f"ANIMCMD_FRAME({frame}, 4)", source)
            self.assertLessEqual((frame + 4) * 32, size)
        self.assertEqual(len(self.decoded("gIntro3Bg_Pal")), 16 * 16 * 2)

    def test_cli_count_dry_run_check_recovery_and_unchanged_timestamps(self):
        with tempfile.TemporaryDirectory() as directory:
            out = Path(directory) / "out"
            stamp = Path(directory) / "stamp.ok"
            args = ("--out", str(out), "--stamp", str(stamp))
            code, stdout, _ = self.invoke(*args, "--print-outputs")
            self.assertEqual(code, 0)
            self.assertEqual(len(stdout.splitlines()), 98)
            self.assertEqual(self.invoke(*args, "--dry-run")[0], 0)
            self.assertFalse(out.exists())
            self.assertFalse(stamp.exists())
            self.assertEqual(self.invoke(*args)[0], 0)
            self.assertEqual(self.invoke(*args, "--check")[0], 0)
            times = {p: p.stat().st_mtime_ns for p in out.rglob("*") if p.is_file()}
            self.assertEqual(len(times), 98)
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
            stamp.write_text("{}")
            self.assertEqual(self.invoke(*args, "--check")[0], 1)

    def test_custom_priority_removal_and_bad_rom_do_not_destroy_existing_outputs(self):
        previous = Path.cwd()
        with tempfile.TemporaryDirectory() as directory:
            try:
                os.chdir(directory)
                asset = self.assets["gIntroCopyright_Pal"]
                custom = Path(asset["custom_path"])
                custom.parent.mkdir(parents=True)
                custom.write_bytes(bytes(32))
                out = Path(directory) / "out"
                args = ("--out", str(out), "--stamp", str(Path(directory) / "stamp.ok"))
                target = out.joinpath(*graphics.relative_path(asset["output_path"], intro.OUTPUT_ROOT).parts)
                self.assertEqual(self.invoke(*args)[0], 0)
                self.assertEqual(target.read_bytes(), bytes(32))
                self.assertEqual(self.invoke(*args, "--check")[0], 0)
                bad = Path(directory) / "bad.gba"
                bad.write_bytes(b"not a ROM")
                self.assertEqual(self.invoke(*args, "--baserom", str(bad))[0], 1)
                self.assertEqual(target.read_bytes(), bytes(32))
                custom.write_bytes(b"invalid")
                self.assertEqual(self.invoke(*args)[0], 1)
                self.assertEqual(target.read_bytes(), bytes(32))
                custom.unlink()
                self.assertEqual(self.invoke(*args, "--check")[0], 1)
                self.assertEqual(self.invoke(*args)[0], 0)
                self.assertEqual(target.read_bytes(), self.decoded(asset["label"]))
            finally:
                os.chdir(previous)

    def test_linked_rom_contains_the_selected_arrays(self):
        nm = shutil.which("arm-none-eabi-nm")
        if nm is None or not (ROOT / "pokeemerald.elf").is_file():
            self.skipTest("build the game and install ARM tools before linked-ROM checks")
        symbols = subprocess.check_output([nm, "-n", str(ROOT / "pokeemerald.elf")], text=True)
        addresses = {m[2]: int(m[1], 16) for m in re.finditer(r"^([0-9a-fA-F]+)\s+[rR]\s+(\w+)$", symbols, re.M)}
        built = (ROOT / "pokeemerald.gba").read_bytes()
        checked = set()
        for label, asset in self.assets.items():
            if not label.startswith(("gIntro", "gTitleScreen", "sTitleScreen")) or label not in addresses:
                continue
            expected = graphics.desired_output(self.rom, asset)
            self.assertEqual((ROOT / asset["output_path"]).read_bytes(), expected, label)
            offset = addresses[label] - 0x08000000
            self.assertEqual(built[offset:offset + len(expected)], expected, label)
            checked.add(label)
        # The Frontier title no longer displays the original clouds/Rayquaza.
        self.assertTrue({"gIntroCopyright_Gfx", "gIntro3Bg_Pal", "gIntroMay_Gfx",
                         "gTitleScreenPressStartGfx"} <= checked)


if __name__ == "__main__":
    unittest.main()
