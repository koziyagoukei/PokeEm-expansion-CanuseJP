"""Regression tests for the host-only font representation audit."""
import importlib.util
import unittest
from pathlib import Path

PATH = Path(__file__).resolve().parents[1] / "font_engine_audit.py"
spec = importlib.util.spec_from_file_location("font_engine_audit", PATH)
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


class ShadowPrototypeTests(unittest.TestCase):
    def test_blank_and_matching_shadow(self):
        blank = [0] * (8 * 16)
        glyph = blank.copy()
        glyph[3 * 8 + 2] = 1
        glyph[4 * 8 + 3] = 2
        result = audit.shadow_poc([tuple(blank), tuple(glyph)], 8)
        self.assertEqual(result["mismatch_glyphs"], [])
        self.assertEqual(result["one_bpp_payload"], 32)
        self.assertEqual(result["index_bytes"], 4)

    def test_accent_and_unexpected_shadow_are_preserved_as_exceptions(self):
        glyph = [0] * (16 * 16)
        glyph[0] = 3
        glyph[10] = 2
        result = audit.shadow_poc([tuple(glyph)], 16)
        self.assertEqual(result["mismatch_glyphs"], [0])
        self.assertEqual(result["different_pixels"], 2)
        self.assertEqual(result["full_2bpp_exceptions"], 64)
        self.assertEqual(result["optimistic_total_without_decoder"], 66)


if __name__ == "__main__":
    unittest.main()
