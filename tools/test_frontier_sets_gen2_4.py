"""Host-side checks for curated sets, protected edits and historical move handling."""
from dataclasses import replace
import re
import tempfile
from pathlib import Path
import unittest

import frontier_sets_gen2_4 as earlier
import frontier_sets_gen5_9 as common
from frontier_move import update_event_moves


class FrontierSetsTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.generator = common.get_generator()
        cls.entries = common.read_entries(cls.generator.read_text(cls.generator.BATTLE_FRONTIER_MONS))

    def test_exact_scope_protects_prefix_and_shedinja(self):
        targets = earlier.target_species(self.entries)
        self.assertEqual(targets, set(earlier.SETS))
        boundary = next(end for label, _, _, end, _ in self.entries if label == "FRONTIER_MON_SUDOWOODO_3")
        for _, species, start, _, _ in self.entries:
            if start < boundary or species == "SHEDINJA":
                self.assertNotIn(species, earlier.SETS)
        self.assertNotIn("MEGANIUM", earlier.SETS)
        self.assertNotIn("TYPHLOSION_HISUI", earlier.SETS)

    def test_castform_has_one_definition_and_smeargle_has_three(self):
        for form in ("NORMAL", "SUNNY", "RAINY", "SNOWY"):
            self.assertEqual(len(earlier.SETS[f"CASTFORM_{form}"]), 1)
        self.assertEqual(len(earlier.SETS["SMEARGLE"]), 3)
        self.assertEqual(sum(map(len, earlier.SETS.values())), 577)

    def test_constants_have_unique_dense_ids_and_smeargle_is_appended(self):
        text = self.generator.read_text(self.generator.FRONTIER_CONSTANTS)
        ids = dict((label, int(value)) for label, value in re.findall(r"#define (FRONTIER_MON_[A-Z0-9_]+_[123])\s+(\d+)", text))
        self.assertEqual(len(ids), len(self.entries))
        self.assertEqual(sorted(ids.values()), list(range(len(self.entries))))
        self.assertEqual([ids[f"FRONTIER_MON_SMEARGLE_{i}"] for i in (1, 2, 3)], [2037, 2038, 2039])

    def test_incremental_writer_preserves_protected_bodies_and_metadata(self):
        labels = {"FRONTIER_MON_SUDOWOODO_1", "FRONTIER_MON_SHEDINJA_1", "FRONTIER_MON_POLITOED_1"}
        bodies = {label: body for label, _, _, _, body in self.entries if label in labels}
        original = "const struct TrainerMon mons[] = {\n" + "\n".join(f"[{label}] = {body}," for label, body in bodies.items()) + "\n};\n"
        # Deliberately stale curated fields; also preserve a user-supplied nickname.
        original = original.replace(".heldItem = ITEM_CHOICE_SPECS,", ".heldItem = ITEM_ORAN_BERRY,")
        original = original.replace(".species = SPECIES_POLITOED,", '.species = SPECIES_POLITOED,\n.nickname = COMPOUND_STRING("UserEdit"),')
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "mons.h"
            path.write_text(original, encoding="utf-8")
            changed = common.apply_curated_sets(path, require_all=False, sets=earlier.SETS)
            self.assertEqual(changed, 1)
            updated = {entry[0]: entry[4] for entry in common.read_entries(path.read_text(encoding="utf-8"))}
            for label in ("FRONTIER_MON_SUDOWOODO_1", "FRONTIER_MON_SHEDINJA_1"):
                self.assertEqual(updated[label], bodies[label])
            self.assertIn('.nickname = COMPOUND_STRING("UserEdit"),', updated["FRONTIER_MON_POLITOED_1"])

    def test_validator_rejects_item_and_move_mismatches(self):
        sets = dict(earlier.SETS)
        variants = list(sets["POLITOED"])
        variants[0] = replace(variants[0], item="ASSAULT_VEST", moves=("SURF", "ICE_BEAM", "PSYCHIC", "PROTECT"))
        variants[1] = replace(variants[1], item="SCIZORITE")
        variants[2] = replace(variants[2], moves=("DRAGON_ASCENT", "ICE_BEAM", "PSYCHIC", "PROTECT"))
        sets["POLITOED"] = tuple(variants)
        with self.assertRaises(ValueError) as caught:
            common.validate_sets({"POLITOED"}, require_all=False, sets=sets)
        self.assertIn("Assault Vest", str(caught.exception))
        self.assertIn("Mega stone", str(caught.exception))
        self.assertIn("cannot learn MOVE_DRAGON_ASCENT", str(caught.exception))

    def test_catalog_and_sketch_and_dream_world_sources(self):
        learnsets = common.read_learnsets()
        for form, move in (("HEAT", "OVERHEAT"), ("WASH", "HYDRO_PUMP"), ("FROST", "BLIZZARD"), ("FAN", "AIR_SLASH"), ("MOW", "LEAF_STORM")):
            self.assertIn(f"MOVE_{move}", learnsets[f"SPECIES_ROTOM_{form}"])
        for variant in earlier.SETS["SMEARGLE"]:
            for move in variant.moves:
                self.assertIn(f"MOVE_{move}", learnsets["SPECIES_SMEARGLE"])
        self.assertNotIn("MOVE_STRUGGLE", learnsets["SPECIES_SMEARGLE"])
        self.assertNotIn("MOVE_NINE_EVO_BOOST", learnsets["SPECIES_SMEARGLE"])
        overlay, _, _ = update_event_moves.build_overlay("", "", self.generator.ROOT)
        self.assertIn("MOVE_SUPERPOWER", overlay["SPECIES_SPINDA"])

    def test_full_generator_includes_smeargle_and_uses_curated_moves(self):
        values, order = self.generator.parse_species_constants()
        selected = self.generator.select_species(self.generator.parse_species_info(values, order))
        smeargle = next(mon for mon in selected if mon.species == "SPECIES_SMEARGLE")
        destination = self.generator.BATTLE_FRONTIER_MONS
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "mons.h"
            try:
                self.generator.BATTLE_FRONTIER_MONS = path
                self.generator.write_battle_frontier_mons([smeargle])
            finally:
                self.generator.BATTLE_FRONTIER_MONS = destination
            entries = common.read_entries(path.read_text(encoding="utf-8"))
            self.assertEqual(len(entries), 3)
            for (_, species, _, _, body), variant in zip(entries, earlier.SETS["SMEARGLE"]):
                self.assertEqual(species, "SMEARGLE")
                for move in variant.moves:
                    self.assertIn(f"MOVE_{move}", body)
                self.assertIn(f"ABILITY_{variant.ability}", body)


if __name__ == "__main__":
    unittest.main()
