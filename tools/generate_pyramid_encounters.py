#!/usr/bin/env python3
"""Generate only Pyramid wild tables; never rewrite the shared Frontier pool."""
from __future__ import annotations

import argparse
import re

import generate_frontier_pool as data

# Eight encounter slots retain the existing per-floor encounter weights.
# Earlier slots favor smaller species; every round includes at least four
# post-Hoenn species or regional forms. Themes follow the Pyramid hints.
ROUNDS = [
    ("paralysis", "SHINX YAMPER PAWMI PIKACHU FLAAFFY LUXIO BELLIBOLT MANECTRIC", "THUNDER_WAVE NUZZLE SPARK", "STATIC"),
    ("poison", "GULPIN TRUBBISH SKORUPI SALANDIT ROSELIA TOXEL SEVIPER GARBODOR", "POISON_GAS POISON_FANG TOXIC", "POISON_POINT"),
    ("burn", "VULPIX LITWICK FLETCHINDER SIZZLIPEDE GROWLITHE SALANDIT MAGMAR CENTISKORCH", "WILL_O_WISP LAVA_PLUME", "FLAME_BODY"),
    ("PP pressure", "SHUPPET DUSKULL YAMASK FRILLISH DUSCLOPS SPIRITOMB CURSOLA CORVIKNIGHT", "SPITE GRUDGE", "PRESSURE"),
    ("levitation", "KOFFING BALTOY BRONZOR ROTOM MISDREAVUS TYNAMO CRYOGONAL VIKAVOLT", "", "LEVITATE"),
    ("trapping", "DIGLETT TRAPINCH GOTHITA LITWICK DUGTRIO PALOSSAND GOTHORITA SPIDOPS", "MEAN_LOOK SAND_TOMB INFESTATION", "ARENA_TRAP"),
    ("ice", "SNORUNT SNOM BERGMITE VANILLITE SWINUB CETODDLE GLALIE FROSMOTH", "ICY_WIND ICE_BEAM", ""),
    ("explosion", "VOLTORB KOFFING DRIFLOON TRUBBISH GEODUDE FERROSEED TURTONATOR DRIFBLIM", "SELF_DESTRUCT EXPLOSION MISTY_EXPLOSION", ""),
    ("psychic", "SPOINK ELGYEM SOLOSIS HATENNA KADABRA ESPURR GIRAFARIG ORBEETLE", "PSYBEAM PSYCHIC", ""),
    ("rock", "GEODUDE ROGGENROLA ROCKRUFF NACLI NOSEPASS BINACLE SUDOWOODO LYCANROC_MIDDAY", "ROCK_SLIDE ROCK_TOMB", ""),
    ("fighting", "MACHOP TIMBURR RIOLU CRABRAWLER MAKUHITA PANCHAM MEDITITE FALINKS", "BRICK_BREAK LOW_SWEEP", ""),
    ("weather", "CASTFORM_NORMAL HIPPOPOTAS VANILLITE SANDACONDA PELIPPER AMAURA TORKOAL ABOMASNOW", "RAIN_DANCE SANDSTORM SUNNY_DAY SNOWSCAPE", ""),
    ("bug", "PARAS DWEBBLE GRUBBIN BLIPBUG ARIADOS CHARJABUG SCYTHER LOKIX", "BUG_BITE STRUGGLE_BUG", ""),
    ("dark", "POOCHYENA PURRLOIN ZORUA MASCHIFF CARVANHA NICKIT SABLEYE MABOSSTIFF", "BITE SNARL", ""),
    ("water", "MARILL BUIZEL DEWPIDER ARROKUDA LOMBRE FINIZEN SEALEO GOLISOPOD", "WATER_PULSE BRINE", ""),
    ("ghost", "GASTLY SANDYGAST SINISTEA GREAVARD SHUPPET PHANTUMP SABLEYE POLTEAGEIST", "HEX SHADOW_BALL", ""),
    ("steel", "MAGNEMITE BRONZOR KLINK CUFANT ARON VAROOM MAWILE COPPERAJAH", "METAL_CLAW IRON_HEAD", ""),
    ("flying and dragon", "SWABLU NOIBAT AXEW ROOKIDEE ALTARIA FRIGIBAX SALAMENCE NOIVERN", "DRAGON_BREATH AIR_SLASH", ""),
    ("evolution stones", "VULPIX GROWLITHE PANSAGE PANSEAR PANPOUR EELEKTRIK EEVEE VULPIX_ALOLA", "", ""),
    ("powerful normal", "TEDDIURSA BUNNELBY STUFFUL SKWOVET LINOONE MAUSHOLD_FOUR TAUROS BEWEAR", "BODY_SLAM DOUBLE_EDGE THRASH", ""),
]
FORBIDDEN = data.BATTLE_ONLY_FLAGS | {
    "isRestrictedLegendary", "isSubLegendary", "isMythical", "isUltraBeast", "isParadox",
}


def learnsets_with_levels():
    result = {}
    for path in sorted(data.LEVEL_UP_DIR.glob("gen_*.h")):
        for name, body in re.findall(
            r"static const struct LevelUpMove\s+(\w+)\[\]\s*=\s*\{(.*?)\};",
            data.read_text(path), re.S,
        ):
            result[name] = [(int(level), move) for level, move in re.findall(
                r"LEVEL_UP_MOVE\(\s*(\d+)\s*,\s*(MOVE_\w+)\s*\)", body)]
    return result


def choose_moves(mon, level, theme, learnsets, moves):
    legal = list(dict.fromkeys(move for learned, move in learnsets[mon.level_up_table]
                              if learned <= level and move in moves))
    attacks = [move for move in legal if moves[move].category != "DAMAGE_CATEGORY_STATUS"
               and move not in {"MOVE_SELF_DESTRUCT", "MOVE_EXPLOSION", "MOVE_FINAL_GAMBIT"}]
    if not attacks:
        raise ValueError(f"No safe damaging move: {mon.species} at {level}")
    attacks.sort(key=lambda m: (moves[m].type in {mon.type1, mon.type2},
                               min(moves[m].power, 80)), reverse=True)
    chosen = [attacks[0]]
    # One thematic move, not four consecutive sources of status/PP attrition.
    for name in theme.split():
        move = "MOVE_" + name
        if move in legal and move not in chosen:
            chosen.append(move)
            break
    for move in attacks + list(reversed(legal)):
        if move not in chosen:
            chosen.append(move)
        if len(chosen) == 4:
            break
    return (chosen + ["MOVE_NONE"] * 4)[:4]


def generate(check=False):
    values, order = data.parse_species_constants()
    species = data.parse_species_info(values, order)
    learnsets = learnsets_with_levels()
    moves = data.parse_move_info()
    generation = {}
    for path in sorted(data.SPECIES_INFO_DIR.glob("gen_*_families.h")):
        gen = int(re.search(r"gen_(\d+)", path.name)[1])
        for name in re.findall(r"\[(SPECIES_\w+)\]", data.read_text(path)):
            generation[name] = gen
    outputs = {"Level50": [], "OpenLevel": []}
    unique = set()
    for round_index, (theme, names, theme_moves, ability) in enumerate(ROUNDS):
        names = ["SPECIES_" + name for name in names.split()]
        names = [name if name in species else next(n for n in species if values[n] == values[name]) for name in names]
        assert len(names) == len(set(names)) == 8
        assert sum(generation[n] >= 4 or n.endswith(("_ALOLA", "_GALAR", "_HISUI", "_PALDEA")) for n in names) >= 4
        for label in outputs:
            outputs[label].extend([f"// Round {round_index + 1}: {theme}.",
                f"static const struct PyramidWildMon s{label}WildMons_Round{round_index + 1}[] =", "{"])
        for slot, name in enumerate(names):
            mon = species[name]
            assert not mon.flags & FORBIDDEN, (name, mon.flags & FORBIDDEN)
            assert mon.level_up_table in learnsets, name
            level = min(50, 35 + round_index // 2 + (slot // 2) * 2)
            selected = choose_moves(mon, level - 5, theme_moves, learnsets, moves)
            ability_slot = mon.abilities[:2].index("ABILITY_" + ability) if "ABILITY_" + ability in mon.abilities[:2] else "ABILITY_RANDOM"
            unique.add(name)
            for label in outputs:
                output_level = level if label == "Level50" else 50 - level
                outputs[label].append(f"    {{ {name}, {output_level}, {ability_slot}, {{ {', '.join(selected)} }} }},")
        for output in outputs.values():
            output.extend(["};", ""])
    for label, lines in outputs.items():
        lines.extend([f"static const struct PyramidWildMon *const s{label}WildMonPointers[TOTAL_PYRAMID_ROUNDS] =", "{"])
        lines.extend(f"    s{label}WildMons_Round{i + 1}," for i in range(len(ROUNDS)))
        lines.extend(["};", ""])
        content = "// Generated by tools/generate_pyramid_encounters.py. Do not edit.\n\n" + "\n".join(lines)
        suffix = "level_50" if label == "Level50" else "open_level"
        path = data.ROOT / f"src/data/battle_frontier/battle_pyramid_{suffix}_wild_mons.h"
        if check:
            if path.read_text(encoding="utf-8") != content:
                raise SystemExit(f"Stale generated table: {path}")
        else:
            path.write_text(content, encoding="utf-8", newline="\n")
    print(f"Pyramid: 20 rounds x 8 slots x 2 level modes; {len(unique)} species; validation passed")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true")
    generate(parser.parse_args().check)
