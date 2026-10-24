"""Exercise actual measurement formatters and numeric layouts in both modes."""

from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
import unittest

from test_text_language_mode import engine_code, function

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from remove_redundant_jpn import encode
from sync_bpej_text import Charmap, find_target


class NumericMeasurementTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        compiler = shutil.which("cc") or shutil.which("gcc")
        if compiler is None:
            raise unittest.SkipTest("a host C compiler is needed (run in WSL)")
        cls.temp = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        fixture = Path(__file__).parent / "fixtures"
        base = (fixture / "text_language_mode.c").read_text(encoding="utf-8")
        base = base.replace("int main(", "int fixture_main(")
        program = base.replace("/* ENGINE_FUNCTIONS */", engine_code())
        data = []
        charmap = Charmap(ROOT / "charmap.txt")
        tables = {
            "src/strings.c": ["gText_UnkHeight", "gText_UnkHeightCentimeters", "gText_XDotY", "gText_XDotY2", "gText_XDotY3"],
            "src/berry_tag_screen.c": ["sText_Var1DotVar2Cm"],
            "src/pokedex.c": ["sText_UnkHeightMetric", "sText_UnkWeight", "sText_UnkWeightMetric"],
            "src/pokemon_summary_screen.c": ["sStatsLeftColumnLayout", "sStatsLeftIVEVColumnLayout",
                "sStatsRightColumnLayout", "sMovesPPLayout"],
        }
        for path, names in tables.items():
            source = (ROOT / path).read_text(encoding="utf-8")
            for name in names:
                _, _, text = find_target(source, {"kind": "variable", "label": name})
                values = ",".join(map(str, encode(text, charmap)))
                data.append(f"static const u8 {name}[] = {{{values}}};")
        constants = (ROOT / "include/constants/pokedex.h").read_text(encoding="utf-8")
        general = (ROOT / "include/config/general.h").read_text(encoding="utf-8")
        for name in ("WEIGHT_HEIGHT_STR_LEN", "WEIGHT_HEIGHT_STR_MEM", "DECAGRAMS_IN_POUND",
                     "CM_PER_INCH", "CM_PER_INCH_FACTOR", "INCHES_IN_FOOT",
                     "INCHES_IN_ONE_AND_HALF_FOOT", "INCHES_IN_FOOT_FACTOR"):
            data.append(re.search(rf"^#define {name}\s+[^\n]+", constants, re.M)[0])
        for name in ("UNITS_IMPERIAL", "UNITS_METRIC", "UNITS_METRIC_CM"):
            data.append(re.search(rf"^#define {name}\s+\d+", general, re.M)[0])
        data.append("typedef uint64_t u64;\nstatic u8 sAllocation[WEIGHT_HEIGHT_STR_MEM + 16];\n"
                    "static void *Alloc(u32 size) { assert(size == WEIGHT_HEIGHT_STR_MEM); "
                    "memset(sAllocation, 0xA5, sizeof(sAllocation)); return sAllocation + 8; }")
        source = (ROOT / "src/pokedex.c").read_text(encoding="utf-8")
        for name in ("ConvertMeasurementToMetricString", "ConvertMonHeightToImperialString",
                     "ConvertMonHeightToMetricString", "ConvertMonHeightToCentimeterString",
                     "ConvertMonHeightToString", "ConvertMonWeightToImperialString",
                     "ConvertMonWeightToMetricString", "ConvertMonWeightToString",
                     "ReplaceDecimalSeparator", "GetUnknownMonHeightString", "GetUnknownMonWeightString"):
            body = function(source, name)
            body = body.replace('COMPOUND_STRING("{ENG}")', "((const u8[]){ENG, EOS})")
            data.append(body)
        program += "\n".join(data)
        program += (fixture / "numeric_measurements.c").read_text(encoding="utf-8")
        source = Path(cls.temp.name) / "numeric.c"
        source.write_text(program, encoding="utf-8")
        cls.binaries = []
        for unit in range(3):
            for separator in ("CHAR_PERIOD", "CHAR_COMMA"):
                binary = Path(cls.temp.name) / f"numeric-{unit}-{separator}"
                subprocess.run([compiler, "-std=gnu17", "-O1", "-DTEST_LEGACY_MODE=0",
                    f"-DUNITS={unit}", f"-DCHAR_DEC_SEPARATOR={separator}", "-iquote", str(ROOT / "include"),
                    str(source), "-o", str(binary)], check=True)
                cls.binaries.append(binary)

    def run_case(self, name):
        for binary in self.binaries:
            with self.subTest(configuration=binary.name):
                result = subprocess.run([str(binary), name], capture_output=True, text=True)
                self.assertEqual(result.returncode, 0, result.stderr)

    def test_measurements_preserve_buffer_guards_and_latin_punctuation(self):
        self.run_case("measurements")

    def test_summary_numeric_layouts_keep_latin_padding_and_separators(self):
        self.run_case("layouts")

    def test_decimal_values_keep_ascii_period_and_units(self):
        self.run_case("decimals")


if __name__ == "__main__":
    unittest.main()
