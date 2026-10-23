"""Run the actual evolution dispatcher with small, independent method tables."""

from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

from test_text_language_mode import function

ROOT = Path(__file__).resolve().parents[2]

HARNESS = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
typedef uint32_t u32;
typedef uint32_t bool32;
#define TRUE 1
#define FALSE 0
#define PARTY_SIZE 6
#define EVOLUTIONS_END 0xFFFF
#define GEN_4 4
#define P_KADABRA_EVERSTONE GEN_4
enum Species { SPECIES_NONE, SPECIES_SOURCE, SPECIES_SCRIPT, SPECIES_SPIN, SPECIES_KADABRA };
enum Item { ITEM_NONE };
enum HoldEffect { HOLD_EFFECT_NONE, HOLD_EFFECT_PREVENT_EVOLVE };
enum EvoState { CHECK_EVO, DO_EVO };
enum MonData { MON_DATA_SPECIES, MON_DATA_HELD_ITEM, MON_DATA_LEVEL, MON_DATA_GIGANTAMAX_FACTOR };
/* ENUMS */
struct Pokemon { u32 unused; };
struct Evolution { u32 method, param; enum Species targetSpecies; const void *params; };
static const struct Evolution *sEvolutions;
static u32 gSpecialVar_0x8000;
static u32 GetMonData(struct Pokemon *mon, enum MonData field, ...) {
    if (field == MON_DATA_SPECIES) return SPECIES_SOURCE;
    if (field == MON_DATA_LEVEL) return 20;
    return 0;
}
static const struct Evolution *GetSpeciesEvolutions(enum Species species) { return sEvolutions; }
static enum HoldEffect GetItemHoldEffect(enum Item item) { return HOLD_EFFECT_NONE; }
static enum Species SanitizeSpeciesId(enum Species species) { return species; }
static enum Species GetGMaxTargetSpecies(enum Species species) { return species; }
static bool32 DoesMonMeetAdditionalConditions(struct Pokemon *mon, const void *params,
    struct Pokemon *partner, u32 slot, bool32 *canStopEvo, enum EvoState state) {
    return params == NULL;
}
/* ENGINE */
int main(int argc, char **argv)
{
    assert(argc == 2);
    struct Pokemon mon = {0};
    bool32 canStop = FALSE;
    const struct Evolution spin[] = {
        {EVO_SPIN, SPIN_CW_SHORT, SPECIES_SPIN, NULL}, {EVOLUTIONS_END}};
    const struct Evolution both[] = {
        {EVO_SCRIPT_TRIGGER, 7, SPECIES_SCRIPT, NULL},
        {EVO_SPIN, SPIN_CW_SHORT, SPECIES_SPIN, NULL}, {EVOLUTIONS_END}};
    switch (atoi(argv[1]))
    {
    case 0:
        sEvolutions = spin;
        assert(GetEvolutionTargetSpecies(&mon, EVO_MODE_SCRIPT_TRIGGER, 7, NULL,
            &canStop, CHECK_EVO) == SPECIES_NONE);
        break;
    case 1:
        sEvolutions = both;
        assert(GetEvolutionTargetSpecies(&mon, EVO_MODE_SCRIPT_TRIGGER, 7, NULL,
            &canStop, CHECK_EVO) == SPECIES_SCRIPT);
        assert(GetEvolutionTargetSpecies(&mon, EVO_MODE_SCRIPT_TRIGGER, 8, NULL,
            &canStop, CHECK_EVO) == SPECIES_NONE);
        break;
    case 2:
        sEvolutions = spin;
        gSpecialVar_0x8000 = SPIN_CW_SHORT;
        assert(GetEvolutionTargetSpecies(&mon, EVO_MODE_OVERWORLD_SPECIAL, 0, NULL,
            &canStop, CHECK_EVO) == SPECIES_SPIN);
        gSpecialVar_0x8000 = SPIN_CCW_SHORT;
        assert(GetEvolutionTargetSpecies(&mon, EVO_MODE_OVERWORLD_SPECIAL, 0, NULL,
            &canStop, CHECK_EVO) == SPECIES_NONE);
        break;
    case 3:
        sEvolutions = both;
        assert(GetEvolutionTargetSpecies(&mon, EVO_MODE_OVERWORLD_SPIN_EVO_CHECK, 0, NULL,
            &canStop, CHECK_EVO) == SPECIES_SPIN);
        break;
    default: abort();
    }
    return 0;
}
'''


class EvolutionModeDispatchTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        compiler = shutil.which("cc") or shutil.which("gcc")
        if compiler is None:
            raise unittest.SkipTest("a host C compiler is needed (run in WSL)")
        cls.temp = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        header = (ROOT / "include/constants/pokemon.h").read_text(encoding="utf-8")
        enums = "\n".join(re.search(rf"enum {name} \{{.*?\n\}};", header, re.S)[0]
                          for name in ("EvolutionMethods", "EvolutionMode", "EvoSpinDirections"))
        engine = function((ROOT / "src/pokemon.c").read_text(encoding="utf-8"), "GetEvolutionTargetSpecies")
        source = Path(cls.temp.name) / "test.c"
        source.write_text(HARNESS.replace("/* ENUMS */", enums).replace("/* ENGINE */", engine), encoding="utf-8")
        cls.binary = Path(cls.temp.name) / "test"
        subprocess.run([compiler, "-std=gnu17", "-O1", str(source), "-o", str(cls.binary)], check=True)

    def test_script_trigger_does_not_enable_spin_evolution(self):
        self.run_case(0)

    def test_script_target_and_parameter_are_preserved(self):
        self.run_case(1)

    def test_spin_direction_still_controls_overworld_evolution(self):
        self.run_case(2)

    def test_explicit_spin_check_remains_available(self):
        self.run_case(3)

    def run_case(self, case):
        result = subprocess.run([str(self.binary), str(case)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()
