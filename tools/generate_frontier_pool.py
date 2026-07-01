#!/usr/bin/env python3
from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]

SPECIES_CONSTANTS = ROOT / "include" / "constants" / "species.h"
FRONTIER_CONSTANTS = ROOT / "include" / "constants" / "battle_frontier_mons.h"
SPECIES_INFO_DIR = ROOT / "src" / "data" / "pokemon" / "species_info"
LEVEL_UP_DIR = ROOT / "src" / "data" / "pokemon" / "level_up_learnsets"
FRONTIER_FULL_LEARNSETS = ROOT / "src" / "data" / "pokemon" / "frontier_full_learnsets.h"
FORM_CHANGE_TABLES = ROOT / "src" / "data" / "pokemon" / "form_change_tables.h"
SIGNATURE_Z_MOVES = ROOT / "src" / "battle_z_move.c"
BATTLE_FRONTIER_MONS = ROOT / "src" / "data" / "battle_frontier" / "battle_frontier_mons.h"
BATTLE_FRONTIER_TRAINER_MONS = ROOT / "src" / "data" / "battle_frontier" / "battle_frontier_trainer_mons.h"

EXTRA_SPECIES = {
    "SPECIES_PIKACHU",
    "SPECIES_EEVEE",
    "SPECIES_CHANSEY",
    "SPECIES_PORYGON2",
    "SPECIES_DOUBLADE",
    "SPECIES_DURALUDON",
    "SPECIES_CLEFAIRY",
    "SPECIES_DUSCLOPS",
    "SPECIES_CORSOLA_GALAR",
    "SPECIES_DIPPLIN",
    "SPECIES_URSARING",
}

EXCLUDED_SPECIES = {
    "SPECIES_SMEARGLE",
}

BATTLE_ONLY_FLAGS = {
    "isMegaEvolution",
    "isPrimalReversion",
    "isUltraBurst",
    "isGigantamax",
    "isTeraForm",
    "isTotem",
    "isFrontierBanned",
}

BATTLE_ONLY_NAME_PARTS = (
    "_MEGA",
    "_PRIMAL",
    "_GMAX",
    "_TOTEM",
    "_STARTER",
    "_BUSTED",
    "_BLADE",
    "_ZEN",
    "_SCHOOL",
    "_ACTIVE",
    "_TERA",
    "_TERASTAL",
    "_STELLAR",
)

PIKACHU_COSTUME_NAMES = {
    "SPECIES_PIKACHU_COSPLAY",
    "SPECIES_PIKACHU_ROCK_STAR",
    "SPECIES_PIKACHU_BELLE",
    "SPECIES_PIKACHU_POP_STAR",
    "SPECIES_PIKACHU_PHD",
    "SPECIES_PIKACHU_LIBRE",
    "SPECIES_PIKACHU_ORIGINAL",
    "SPECIES_PIKACHU_HOENN",
    "SPECIES_PIKACHU_SINNOH",
    "SPECIES_PIKACHU_UNOVA",
    "SPECIES_PIKACHU_KALOS",
    "SPECIES_PIKACHU_ALOLA",
    "SPECIES_PIKACHU_PARTNER",
    "SPECIES_PIKACHU_WORLD",
}

REPRESENTATIVE_FORM_NAMES = {
    "SPECIES_VIVILLON_ICY_SNOW",
    "SPECIES_FLABEBE_RED",
    "SPECIES_FLOETTE_RED",
    "SPECIES_FLORGES_RED",
    "SPECIES_FURFROU_NATURAL",
    "SPECIES_PUMPKABOO_AVERAGE",
    "SPECIES_GOURGEIST_AVERAGE",
    "SPECIES_MINIOR_METEOR_RED",
    "SPECIES_ALCREMIE_STRAWBERRY_VANILLA_CREAM",
    "SPECIES_SQUAWKABILLY_GREEN",
    "SPECIES_TATSUGIRI_CURLY",
}

COSMETIC_FORM_PREFIXES = (
    "SPECIES_VIVILLON_",
    "SPECIES_FLABEBE_",
    "SPECIES_FLOETTE_",
    "SPECIES_FLORGES_",
    "SPECIES_FURFROU_",
    "SPECIES_PUMPKABOO_",
    "SPECIES_GOURGEIST_",
    "SPECIES_MINIOR_",
    "SPECIES_ALCREMIE_",
    "SPECIES_SQUAWKABILLY_",
    "SPECIES_TATSUGIRI_",
)

STAT_FIELDS = {
    "hp": "baseHP",
    "atk": "baseAttack",
    "def": "baseDefense",
    "speed": "baseSpeed",
    "spatk": "baseSpAttack",
    "spdef": "baseSpDefense",
}

NATURES = {
    ("atk", "def"): "NATURE_LONELY",
    ("atk", "speed"): "NATURE_BRAVE",
    ("atk", "spatk"): "NATURE_ADAMANT",
    ("atk", "spdef"): "NATURE_NAUGHTY",
    ("def", "atk"): "NATURE_BOLD",
    ("def", "speed"): "NATURE_RELAXED",
    ("def", "spatk"): "NATURE_IMPISH",
    ("def", "spdef"): "NATURE_LAX",
    ("speed", "atk"): "NATURE_TIMID",
    ("speed", "def"): "NATURE_HASTY",
    ("speed", "spatk"): "NATURE_JOLLY",
    ("speed", "spdef"): "NATURE_NAIVE",
    ("spatk", "atk"): "NATURE_MODEST",
    ("spatk", "def"): "NATURE_MILD",
    ("spatk", "speed"): "NATURE_QUIET",
    ("spatk", "spdef"): "NATURE_RASH",
    ("spdef", "atk"): "NATURE_CALM",
    ("spdef", "def"): "NATURE_GENTLE",
    ("spdef", "speed"): "NATURE_SASSY",
    ("spdef", "spatk"): "NATURE_CAREFUL",
}


@dataclass
class SpeciesData:
    species: str
    value: int
    order: int
    stats: dict[str, int]
    type1: str
    abilities: list[str]
    level_up_table: str | None
    form_change_table: str | None
    evolutions: list[str]
    flags: set[str]


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def strip_line_comment(line: str) -> str:
    return line.split("//", 1)[0]


def eval_enum_expr(expr: str, values: dict[str, int], current: int) -> int:
    expr = expr.strip().strip(",").strip()
    if expr.isdigit():
        return int(expr)
    if expr in values:
        return values[expr]
    m = re.fullmatch(r"([A-Z0-9_]+)\s*([+-])\s*(\d+)", expr)
    if m and m.group(1) in values:
        base = values[m.group(1)]
        delta = int(m.group(3))
        return base + delta if m.group(2) == "+" else base - delta
    return current


def parse_species_constants() -> tuple[dict[str, int], dict[str, int]]:
    values: dict[str, int] = {}
    order: dict[str, int] = {}
    current = 0
    in_enum = False

    for line in read_text(SPECIES_CONSTANTS).splitlines():
        if line.startswith("enum __attribute__((packed)) Species"):
            in_enum = True
            continue
        if not in_enum:
            continue
        if line.strip() == "};":
            break

        line = strip_line_comment(line).strip()
        if not line or line.startswith("#"):
            continue
        line = line.rstrip(",").strip()
        m = re.fullmatch(r"(SPECIES_[A-Z0-9_]+|NUM_SPECIES)(?:\s*=\s*(.+))?", line)
        if not m:
            continue

        name = m.group(1)
        expr = m.group(2)
        value = eval_enum_expr(expr, values, current) if expr else current
        values[name] = value
        order[name] = len(order)
        current = value + 1

    return values, order


def find_matching_brace(text: str, start: int) -> int:
    depth = 0
    in_string = False
    in_char = False
    in_line_comment = False
    in_block_comment = False
    escape = False
    i = start

    while i < len(text):
        c = text[i]
        n = text[i + 1] if i + 1 < len(text) else ""

        if in_line_comment:
            if c == "\n":
                in_line_comment = False
            i += 1
            continue
        if in_block_comment:
            if c == "*" and n == "/":
                in_block_comment = False
                i += 2
            else:
                i += 1
            continue
        if in_string:
            if escape:
                escape = False
            elif c == "\\":
                escape = True
            elif c == '"':
                in_string = False
            i += 1
            continue
        if in_char:
            if escape:
                escape = False
            elif c == "\\":
                escape = True
            elif c == "'":
                in_char = False
            i += 1
            continue

        if c == "/" and n == "/":
            in_line_comment = True
            i += 2
            continue
        if c == "/" and n == "*":
            in_block_comment = True
            i += 2
            continue
        if c == '"':
            in_string = True
            i += 1
            continue
        if c == "'":
            in_char = True
            i += 1
            continue
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return i
        i += 1

    raise ValueError("No matching brace found")


def parse_stat_macros() -> dict[str, str]:
    macros: dict[str, str] = {}
    for path in sorted(SPECIES_INFO_DIR.glob("gen_*_families.h")):
        for line in read_text(path).splitlines():
            m = re.match(r"\s*#\s*define\s+([A-Z0-9_]+)\s+(.+)", line)
            if m and m.group(1) not in macros:
                macros[m.group(1)] = strip_line_comment(m.group(2)).strip()
    return macros


def eval_stat_expr(expr: str, macros: dict[str, str], seen: set[str] | None = None) -> int:
    expr = strip_line_comment(expr).strip().strip(",").strip()
    while expr.startswith("(") and expr.endswith(")"):
        expr = expr[1:-1].strip()
    seen = seen or set()

    if expr in macros and expr not in seen:
        seen.add(expr)
        return eval_stat_expr(macros[expr], macros, seen)

    if "?" in expr and ":" in expr:
        true_branch = expr.split("?", 1)[1].rsplit(":", 1)[0]
        return eval_stat_expr(true_branch, macros, seen)

    m = re.search(r"\b\d+\b", expr)
    if m:
        return int(m.group(0))

    return 0


def resolve_type_token(token: str, macros: dict[str, str], seen: set[str] | None = None) -> str:
    token = token.strip()
    if token.startswith("TYPE_"):
        return token

    seen = seen or set()
    if token in macros and token not in seen:
        seen.add(token)
        return resolve_type_token(macros[token], macros, seen)

    if "?" in token and ":" in token:
        true_branch = token.split("?", 1)[1].rsplit(":", 1)[0]
        return resolve_type_token(true_branch, macros, seen)

    m = re.search(r"TYPE_[A-Z0-9_]+", token)
    return m.group(0) if m else token


def get_scalar_field(block: str, field: str, macros: dict[str, str]) -> int:
    m = re.search(rf"\.{field}\s*=\s*([^,\n]+)", block)
    if not m:
        return 0
    return eval_stat_expr(m.group(1), macros)


def parse_species_info(values: dict[str, int], order: dict[str, int]) -> dict[str, SpeciesData]:
    macros = parse_stat_macros()
    species_data: dict[str, SpeciesData] = {}

    for path in sorted(SPECIES_INFO_DIR.glob("gen_*_families.h")):
        text = read_text(path)
        for match in re.finditer(r"\[(SPECIES_[A-Z0-9_]+)\]\s*=\s*\{", text):
            species = match.group(1)
            start = text.find("{", match.start())
            end = find_matching_brace(text, start)
            block = text[start : end + 1]

            if species not in values:
                continue

            stats = {key: get_scalar_field(block, field, macros) for key, field in STAT_FIELDS.items()}

            type_match = re.search(r"\.types\s*=\s*MON_TYPES\(([^)]*)\)", block, re.S)
            type1 = "TYPE_NORMAL"
            if type_match:
                type_tokens = [token.strip() for token in type_match.group(1).split(",") if token.strip()]
                if type_tokens:
                    type1 = resolve_type_token(type_tokens[0], macros)

            ability_match = re.search(r"\.abilities\s*=\s*\{([^}]*)\}", block, re.S)
            abilities = re.findall(r"ABILITY_[A-Z0-9_]+", ability_match.group(1)) if ability_match else []
            while len(abilities) < 3:
                abilities.append("ABILITY_NONE")
            abilities = abilities[:3]

            level_match = re.search(r"\.levelUpLearnset\s*=\s*(s[A-Za-z0-9_]+LevelUpLearnset)", block)
            form_match = re.search(r"\.formChangeTable\s*=\s*(s[A-Za-z0-9_]+FormChangeTable)", block)

            flags = {
                flag
                for flag in (
                    "isRestrictedLegendary",
                    "isSubLegendary",
                    "isMythical",
                    "isUltraBeast",
                    "isParadox",
                    "isTotem",
                    "isMegaEvolution",
                    "isPrimalReversion",
                    "isUltraBurst",
                    "isGigantamax",
                    "isTeraForm",
                    "isFrontierBanned",
                )
                if re.search(rf"\.{flag}\s*=\s*TRUE", block)
            }

            evolutions: list[str] = []
            evo_start = block.find(".evolutions")
            if evo_start >= 0:
                evo_text = block[evo_start:]
                for target in re.findall(r"SPECIES_[A-Z0-9_]+", evo_text):
                    if target != species and target != "SPECIES_NONE":
                        evolutions.append(target)

            species_data[species] = SpeciesData(
                species=species,
                value=values[species],
                order=order.get(species, len(order)),
                stats=stats,
                type1=type1,
                abilities=abilities,
                level_up_table=level_match.group(1) if level_match else None,
                form_change_table=form_match.group(1) if form_match else None,
                evolutions=evolutions,
                flags=flags,
            )

    return species_data


def parse_level_up_learnsets() -> dict[str, list[str]]:
    learnsets: dict[str, list[str]] = {}
    for path in sorted(LEVEL_UP_DIR.glob("gen_*.h")):
        text = read_text(path)
        for match in re.finditer(
            r"static const struct LevelUpMove\s+(s[A-Za-z0-9_]+LevelUpLearnset)\[\]\s*=\s*\{(.*?)\};",
            text,
            re.S,
        ):
            table = match.group(1)
            body = match.group(2)
            moves: list[str] = []
            for level_text, move in re.findall(r"LEVEL_UP_MOVE\(\s*(\d+)\s*,\s*(MOVE_[A-Z0-9_]+)\s*\)", body):
                if int(level_text) > 100:
                    continue
                if move in ("MOVE_NONE", "MOVE_UNAVAILABLE"):
                    continue
                if move in moves:
                    moves.remove(move)
                moves.append(move)
            learnsets[table] = moves
    return learnsets


def parse_full_learnsets() -> dict[str, list[str]]:
    values, _order = parse_species_constants()
    text = read_text(FRONTIER_FULL_LEARNSETS)
    arrays: dict[str, list[str]] = {}
    for match in re.finditer(r"static const u16\s+(sFrontierFullLearnset_SPECIES_[A-Z0-9_]+)\[\]\s*=\s*\{(.*?)\};", text, re.S):
        arrays[match.group(1)] = [
            move
            for move in re.findall(r"MOVE_[A-Z0-9_]+", match.group(2))
            if move not in ("MOVE_NONE", "MOVE_UNAVAILABLE")
        ]

    mapped: dict[str, list[str]] = {}
    for species, table in re.findall(r"\[(SPECIES_[A-Z0-9_]+)\]\s*=\s*(sFrontierFullLearnset_SPECIES_[A-Z0-9_]+)", text):
        mapped[species] = arrays.get(table, [])

    by_value: dict[int, list[str]] = {}
    for species, moves in mapped.items():
        if species in values and moves:
            by_value.setdefault(values[species], moves)
    for species, value in values.items():
        if species.startswith("SPECIES_") and species not in mapped and value in by_value:
            mapped[species] = by_value[value]
    return mapped


def parse_mega_stones_by_form_table() -> dict[str, list[str]]:
    form_table_items: dict[str, list[str]] = {}
    text = read_text(FORM_CHANGE_TABLES)
    for match in re.finditer(r"static const struct FormChange\s+(s[A-Za-z0-9_]+FormChangeTable)\[\]\s*=\s*\{(.*?)\};", text, re.S):
        items = []
        for item in re.findall(r"\{FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,\s*SPECIES_[A-Z0-9_]+,\s*(ITEM_[A-Z0-9_]+)\}", match.group(2)):
            if item not in items:
                items.append(item)
        if items:
            form_table_items[match.group(1)] = items
    return form_table_items


def parse_signature_z_moves() -> dict[str, tuple[str, str]]:
    text = read_text(SIGNATURE_Z_MOVES)
    z_moves: dict[str, tuple[str, str]] = {}
    for species, item, source_move, _z_move in re.findall(
        r"\{(SPECIES_[A-Z0-9_]+),\s*(ITEM_[A-Z0-9_]+),\s*(MOVE_[A-Z0-9_]+),\s*(MOVE_[A-Z0-9_]+)\}",
        text,
    ):
        z_moves.setdefault(species, (item, source_move))
    return z_moves


def is_excluded_base(species: str, data: SpeciesData) -> bool:
    if species in EXTRA_SPECIES:
        return False
    if data.flags & BATTLE_ONLY_FLAGS:
        return True
    if species in PIKACHU_COSTUME_NAMES:
        return True
    if species.startswith("SPECIES_UNOWN_") and species != "SPECIES_UNOWN":
        return True
    if species not in REPRESENTATIVE_FORM_NAMES and any(species.startswith(prefix) for prefix in COSMETIC_FORM_PREFIXES):
        return True
    return any(part in species for part in BATTLE_ONLY_NAME_PARTS)


def select_species(species_data: dict[str, SpeciesData]) -> list[SpeciesData]:
    outgoing = {species for species, data in species_data.items() if data.evolutions}
    selected: list[SpeciesData] = []

    for species, data in species_data.items():
        if species == "SPECIES_NONE":
            continue
        if species in EXCLUDED_SPECIES:
            continue
        if is_excluded_base(species, data):
            continue
        if species in EXTRA_SPECIES or species not in outgoing or "isSubLegendary" in data.flags:
            selected.append(data)

    selected.sort(key=lambda data: (data.value, data.species))
    return selected


def mon_label(species: str, slot: int) -> str:
    return f"FRONTIER_MON_{species.removeprefix('SPECIES_')}_{slot}"


def choose_ability(species: str, abilities: list[str], slot: int) -> str:
    if species == "SPECIES_DITTO":
        return "ABILITY_IMPOSTER"
    fallback = next((ability for ability in abilities if ability != "ABILITY_NONE"), "ABILITY_NONE")
    ability = abilities[slot] if slot < len(abilities) else "ABILITY_NONE"
    return ability if ability != "ABILITY_NONE" else fallback


def choose_ev(stats: dict[str, int]) -> str:
    if stats["atk"] > stats["spatk"]:
        return "TRAINER_PARTY_EVS(0, 252, 0, 252, 0, 0)"
    if stats["spatk"] > stats["atk"]:
        return "TRAINER_PARTY_EVS(0, 0, 0, 252, 252, 0)"
    return "TRAINER_PARTY_EVS(0, 252, 0, 0, 252, 0)"


def choose_nature(stats: dict[str, int]) -> str:
    relevant = {key: stats[key] for key in ("atk", "def", "speed", "spatk", "spdef")}
    max_value = max(relevant.values())
    min_value = min(relevant.values())
    max_stats = [key for key, value in relevant.items() if value == max_value]
    min_stats = [key for key, value in relevant.items() if value == min_value]
    if len(max_stats) != 1 or len(min_stats) != 1 or max_stats[0] == min_stats[0]:
        return "NATURE_QUIRKY"
    return NATURES.get((max_stats[0], min_stats[0]), "NATURE_QUIRKY")


def choose_moves(species: str, data: SpeciesData, level_up: dict[str, list[str]], full: dict[str, list[str]], z_source: str | None) -> list[str]:
    level_moves = list(level_up.get(data.level_up_table or "", []))
    full_moves = list(full.get(species, []))

    moves: list[str] = []
    if z_source:
        moves.append(z_source)
        for move in reversed(level_moves):
            if move not in moves:
                moves.append(move)
            if len(moves) == 4:
                return moves
        for move in full_moves:
            if move not in moves:
                moves.append(move)
            if len(moves) == 4:
                return moves
    else:
        for move in level_moves[-4:]:
            if move not in moves:
                moves.append(move)
        for move in full_moves:
            if move not in moves:
                moves.append(move)
            if len(moves) == 4:
                return moves

    while len(moves) < 4:
        moves.append("MOVE_NONE")
    return moves[:4]


def write_frontier_constants(selected: list[SpeciesData]) -> None:
    lines = [
        "#ifndef GUARD_CONSTANTS_BATTLE_FRONTIER_MONS_H",
        "#define GUARD_CONSTANTS_BATTLE_FRONTIER_MONS_H",
        "",
        "// Auto-generated by tools/generate_frontier_pool.py.",
        "#define FRONTIER_MON_POOL_FIRST 0",
    ]
    index = 0
    for data in selected:
        for slot in range(1, 4):
            lines.append(f"#define {mon_label(data.species, slot):<45} {index}")
            index += 1
    lines.extend(
        [
            "",
            f"#define NUM_FRONTIER_MONS {index}",
            "#define FRONTIER_MONS_HIGH_TIER (NUM_FRONTIER_MONS - 1)",
            "",
            "#endif // GUARD_CONSTANTS_BATTLE_FRONTIER_MONS_H",
            "",
        ]
    )
    FRONTIER_CONSTANTS.write_text("\n".join(lines), encoding="utf-8", newline="\n")


def write_battle_frontier_mons(selected: list[SpeciesData]) -> None:
    level_up = parse_level_up_learnsets()
    full = parse_full_learnsets()
    mega_stones_by_form_table = parse_mega_stones_by_form_table()
    z_moves = parse_signature_z_moves()

    lines = [
        "// Auto-generated by tools/generate_frontier_pool.py.",
        "const struct TrainerMon gBattleFrontierMons[NUM_FRONTIER_MONS] =",
        "{",
    ]
    for data in selected:
        z_item, z_source = z_moves.get(data.species, ("ITEM_NONE", None))
        mega_items = mega_stones_by_form_table.get(data.form_change_table or "", [])

        for slot in range(1, 4):
            held_item = "ITEM_NONE"
            if z_source:
                held_item = z_item
            elif mega_items:
                held_item = mega_items[(slot - 1) % len(mega_items)]
            elif data.species in EXTRA_SPECIES:
                held_item = "ITEM_EVIOLITE"

            moves = choose_moves(data.species, data, level_up, full, z_source)
            lines.extend(
                [
                    f"    [{mon_label(data.species, slot)}] = {{",
                    f"        .species = {data.species},",
                    f"        .moves = {{{', '.join(moves)}}},",
                    f"        .heldItem = {held_item},",
                    f"        .ev = {choose_ev(data.stats)},",
                    f"        .nature = {choose_nature(data.stats)},",
                    "        .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),",
                    f"        .ability = {choose_ability(data.species, data.abilities, slot - 1)},",
                    f"        .teraType = {data.type1},",
                    "        .shouldUseDynamax = TRUE,",
                    "        .dynamaxLevel = 10,",
                    "        .ball = BALL_POKE,",
                    "    },",
                ]
            )
    lines.extend(["};", ""])
    BATTLE_FRONTIER_MONS.write_text("\n".join(lines), encoding="utf-8", newline="\n")


def parse_existing_trainer_mons_macros() -> list[tuple[str, str]]:
    text = read_text(BATTLE_FRONTIER_TRAINER_MONS)
    macros: list[tuple[str, str]] = []
    seen: set[str] = set()
    for line in text.splitlines():
        m = re.match(r"#define\s+(FRONTIER_MONS_[A-Z0-9_]+)(\([^)]*\))?", line)
        if not m:
            continue
        name = m.group(1)
        signature = m.group(2) or ""
        if name == "FRONTIER_MONS_DEFAULT" or name in seen:
            continue
        seen.add(name)
        macros.append((name, signature))
    return macros


def write_battle_frontier_trainer_mons(selected: list[SpeciesData]) -> None:
    macros = parse_existing_trainer_mons_macros()
    default_labels = [mon_label(data.species, 1) for data in selected[:12]]
    if not default_labels:
        raise RuntimeError("No default frontier mons were generated")

    lines = [
        "// Auto-generated by tools/generate_frontier_pool.py.",
        "// Normal frontier trainers are filled from the full pool at runtime.",
        "",
        "#define FRONTIER_MONS_DEFAULT \\",
    ]
    for label in default_labels:
        lines.append(f"    {label}, \\")
    lines.append("    0xFFFF")
    lines.append("")

    for name, signature in macros:
        lines.append(f"#define {name}{signature} FRONTIER_MONS_DEFAULT")

    lines.append("")
    BATTLE_FRONTIER_TRAINER_MONS.write_text("\n".join(lines), encoding="utf-8", newline="\n")


def main() -> None:
    values, order = parse_species_constants()
    species_data = parse_species_info(values, order)
    selected = select_species(species_data)
    missing_extras = sorted(EXTRA_SPECIES - {data.species for data in selected})
    if missing_extras:
        raise RuntimeError(f"Extra species were not generated: {', '.join(missing_extras)}")

    write_frontier_constants(selected)
    write_battle_frontier_mons(selected)
    write_battle_frontier_trainer_mons(selected)
    print(f"Generated {len(selected)} species and {len(selected) * 3} frontier mon sets.")


if __name__ == "__main__":
    main()
