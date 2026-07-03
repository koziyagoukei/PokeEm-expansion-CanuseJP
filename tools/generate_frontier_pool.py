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
MOVE_INFO = ROOT / "src" / "data" / "moves_info.h"
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

Z_CRYSTALS_BY_TYPE = {
    "TYPE_NORMAL": "ITEM_NORMALIUM_Z",
    "TYPE_FIRE": "ITEM_FIRIUM_Z",
    "TYPE_WATER": "ITEM_WATERIUM_Z",
    "TYPE_ELECTRIC": "ITEM_ELECTRIUM_Z",
    "TYPE_GRASS": "ITEM_GRASSIUM_Z",
    "TYPE_ICE": "ITEM_ICIUM_Z",
    "TYPE_FIGHTING": "ITEM_FIGHTINIUM_Z",
    "TYPE_POISON": "ITEM_POISONIUM_Z",
    "TYPE_GROUND": "ITEM_GROUNDIUM_Z",
    "TYPE_FLYING": "ITEM_FLYINIUM_Z",
    "TYPE_PSYCHIC": "ITEM_PSYCHIUM_Z",
    "TYPE_BUG": "ITEM_BUGINIUM_Z",
    "TYPE_ROCK": "ITEM_ROCKIUM_Z",
    "TYPE_GHOST": "ITEM_GHOSTIUM_Z",
    "TYPE_DRAGON": "ITEM_DRAGONIUM_Z",
    "TYPE_DARK": "ITEM_DARKINIUM_Z",
    "TYPE_STEEL": "ITEM_STEELIUM_Z",
    "TYPE_FAIRY": "ITEM_FAIRIUM_Z",
}

DEFENSIVE_TERA_TYPES = (
    "TYPE_WATER",
    "TYPE_FAIRY",
    "TYPE_STEEL",
    "TYPE_POISON",
    "TYPE_FIRE",
    "TYPE_GRASS",
)

MOVE_BLACKLIST = {
    "MOVE_NONE",
    "MOVE_UNAVAILABLE",
    "MOVE_SKETCH",
    "MOVE_CELEBRATE",
    "MOVE_HAPPY_HOUR",
    "MOVE_HOLD_HANDS",
    "MOVE_HYPER_BEAM",
    "MOVE_GIGA_IMPACT",
    "MOVE_BLAST_BURN",
    "MOVE_FRENZY_PLANT",
    "MOVE_HYDRO_CANNON",
    "MOVE_ROCK_WRECKER",
    "MOVE_ROAR_OF_TIME",
    "MOVE_FOCUS_PUNCH",
}

PHYSICAL_SETUP_MOVES = (
    "MOVE_SWORDS_DANCE",
    "MOVE_DRAGON_DANCE",
    "MOVE_BULK_UP",
    "MOVE_COIL",
    "MOVE_SHELL_SMASH",
    "MOVE_SHIFT_GEAR",
    "MOVE_BELLY_DRUM",
    "MOVE_HONE_CLAWS",
    "MOVE_VICTORY_DANCE",
    "MOVE_CURSE",
)

SPECIAL_SETUP_MOVES = (
    "MOVE_NASTY_PLOT",
    "MOVE_QUIVER_DANCE",
    "MOVE_CALM_MIND",
    "MOVE_TAIL_GLOW",
    "MOVE_GEOMANCY",
    "MOVE_TAKE_HEART",
    "MOVE_AGILITY",
)

RECOVERY_MOVES = (
    "MOVE_RECOVER",
    "MOVE_ROOST",
    "MOVE_SLACK_OFF",
    "MOVE_SOFT_BOILED",
    "MOVE_MILK_DRINK",
    "MOVE_MOONLIGHT",
    "MOVE_SYNTHESIS",
    "MOVE_MORNING_SUN",
    "MOVE_SHORE_UP",
    "MOVE_STRENGTH_SAP",
    "MOVE_WISH",
    "MOVE_REST",
)

UTILITY_MOVES = (
    "MOVE_STEALTH_ROCK",
    "MOVE_SPIKES",
    "MOVE_TOXIC_SPIKES",
    "MOVE_STICKY_WEB",
    "MOVE_DEFOG",
    "MOVE_RAPID_SPIN",
    "MOVE_TAUNT",
    "MOVE_ENCORE",
    "MOVE_THUNDER_WAVE",
    "MOVE_WILL_O_WISP",
    "MOVE_TOXIC",
    "MOVE_YAWN",
    "MOVE_LEECH_SEED",
    "MOVE_SUBSTITUTE",
    "MOVE_PROTECT",
    "MOVE_TRICK_ROOM",
    "MOVE_LIGHT_SCREEN",
    "MOVE_REFLECT",
    "MOVE_AURORA_VEIL",
    "MOVE_HAZE",
    "MOVE_ROAR",
    "MOVE_WHIRLWIND",
)

PIVOT_MOVES = (
    "MOVE_U_TURN",
    "MOVE_VOLT_SWITCH",
    "MOVE_FLIP_TURN",
    "MOVE_PARTING_SHOT",
    "MOVE_BATON_PASS",
    "MOVE_SHED_TAIL",
    "MOVE_CHILLY_RECEPTION",
    "MOVE_TELEPORT",
)

PRIORITY_MOVES = (
    "MOVE_EXTREME_SPEED",
    "MOVE_SUCKER_PUNCH",
    "MOVE_FAKE_OUT",
    "MOVE_FIRST_IMPRESSION",
    "MOVE_JET_PUNCH",
    "MOVE_AQUA_JET",
    "MOVE_BULLET_PUNCH",
    "MOVE_MACH_PUNCH",
    "MOVE_ICE_SHARD",
    "MOVE_SHADOW_SNEAK",
    "MOVE_VACUUM_WAVE",
    "MOVE_GRASSY_GLIDE",
    "MOVE_ACCELEROCK",
    "MOVE_QUICK_ATTACK",
)

HIGH_VALUE_ATTACKS = (
    "MOVE_KNOCK_OFF",
    "MOVE_EARTHQUAKE",
    "MOVE_STOMPING_TANTRUM",
    "MOVE_HIGH_HORSEPOWER",
    "MOVE_CLOSE_COMBAT",
    "MOVE_DRAIN_PUNCH",
    "MOVE_BODY_PRESS",
    "MOVE_SUPERPOWER",
    "MOVE_FLARE_BLITZ",
    "MOVE_FIRE_BLAST",
    "MOVE_HEAT_WAVE",
    "MOVE_WAVE_CRASH",
    "MOVE_SURF",
    "MOVE_SCALD",
    "MOVE_LIQUIDATION",
    "MOVE_HEADLONG_RUSH",
    "MOVE_ICICLE_CRASH",
    "MOVE_ICE_SPINNER",
    "MOVE_TRIPLE_AXEL",
    "MOVE_MOONBLAST",
    "MOVE_PLAY_ROUGH",
    "MOVE_SHADOW_BALL",
    "MOVE_POLTERGEIST",
    "MOVE_SHADOW_CLAW",
    "MOVE_EARTH_POWER",
    "MOVE_THUNDERBOLT",
    "MOVE_WILD_CHARGE",
    "MOVE_THUNDER",
    "MOVE_ICE_BEAM",
    "MOVE_FLAMETHROWER",
    "MOVE_HYDRO_PUMP",
    "MOVE_WATERFALL",
    "MOVE_DRACO_METEOR",
    "MOVE_DRAGON_CLAW",
    "MOVE_OUTRAGE",
    "MOVE_LEAF_STORM",
    "MOVE_GIGA_DRAIN",
    "MOVE_POWER_WHIP",
    "MOVE_ENERGY_BALL",
    "MOVE_MAKE_IT_RAIN",
    "MOVE_FLASH_CANNON",
    "MOVE_IRON_HEAD",
    "MOVE_POISON_JAB",
    "MOVE_SLUDGE_BOMB",
    "MOVE_SLUDGE_WAVE",
    "MOVE_BRAVE_BIRD",
    "MOVE_AIR_SLASH",
    "MOVE_HURRICANE",
    "MOVE_STONE_EDGE",
    "MOVE_ROCK_SLIDE",
    "MOVE_BUG_BUZZ",
    "MOVE_X_SCISSOR",
    "MOVE_DARK_PULSE",
    "MOVE_PSYCHIC",
    "MOVE_PSYSHOCK",
)

MULTI_HIT_MOVES = {
    "MOVE_ICICLE_SPEAR",
    "MOVE_BULLET_SEED",
    "MOVE_ROCK_BLAST",
    "MOVE_SCALE_SHOT",
    "MOVE_TAIL_SLAP",
    "MOVE_ARM_THRUST",
    "MOVE_BONEMERANG",
}


@dataclass
class SpeciesData:
    species: str
    value: int
    order: int
    stats: dict[str, int]
    type1: str
    type2: str
    abilities: list[str]
    level_up_table: str | None
    form_change_table: str | None
    evolutions: list[str]
    flags: set[str]


@dataclass
class MoveData:
    move: str
    power: int
    type: str
    category: str
    accuracy: int
    priority: int
    effect: str


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
            type2 = "TYPE_NORMAL"
            if type_match:
                type_tokens = [token.strip() for token in type_match.group(1).split(",") if token.strip()]
                if type_tokens:
                    type1 = resolve_type_token(type_tokens[0], macros)
                    type2 = resolve_type_token(type_tokens[1], macros) if len(type_tokens) > 1 else type1

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
                type2=type2,
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


def first_int(expr: str, default: int = 0) -> int:
    if "?" in expr and ":" in expr:
        expr = expr.split("?", 1)[1].split(":", 1)[0]
    m = re.search(r"-?\d+", expr)
    return int(m.group(0)) if m else default


def first_token(expr: str, prefix: str, default: str) -> str:
    m = re.search(rf"{prefix}[A-Z0-9_]+", expr)
    return m.group(0) if m else default


def get_move_field(block: str, field: str) -> str:
    m = re.search(rf"\.{field}\s*=\s*([^,\n]+)", block)
    return m.group(1).strip() if m else ""


def parse_move_info() -> dict[str, MoveData]:
    text = read_text(MOVE_INFO)
    moves: dict[str, MoveData] = {}
    for match in re.finditer(r"\[(MOVE_[A-Z0-9_]+)\]\s*=\s*\{", text):
        move = match.group(1)
        start = text.find("{", match.start())
        end = find_matching_brace(text, start)
        block = text[start : end + 1]

        moves[move] = MoveData(
            move=move,
            power=first_int(get_move_field(block, "power")),
            type=first_token(get_move_field(block, "type"), "TYPE_", "TYPE_NORMAL"),
            category=first_token(get_move_field(block, "category"), "DAMAGE_CATEGORY_", "DAMAGE_CATEGORY_STATUS"),
            accuracy=first_int(get_move_field(block, "accuracy"), 100),
            priority=first_int(get_move_field(block, "priority")),
            effect=first_token(get_move_field(block, "effect"), "EFFECT_", "EFFECT_HIT"),
        )
    return moves


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


def choose_role(data: SpeciesData, slot: int) -> str:
    physical = data.stats["atk"] >= data.stats["spatk"]
    if slot == 3:
        return "bulky"
    if slot == 1:
        return "physical" if physical else "special"
    if abs(data.stats["atk"] - data.stats["spatk"]) <= 30:
        return "special" if physical else "physical"
    return "physical" if physical else "special"


def choose_ev(stats: dict[str, int], role: str) -> str:
    if role == "physical":
        return "TRAINER_PARTY_EVS(0, 252, 0, 252, 0, 0)"
    if role == "special":
        return "TRAINER_PARTY_EVS(0, 0, 0, 252, 252, 0)"
    if stats["def"] <= stats["spdef"]:
        return "TRAINER_PARTY_EVS(252, 0, 252, 0, 0, 4)"
    return "TRAINER_PARTY_EVS(252, 0, 4, 0, 0, 252)"


def choose_nature(stats: dict[str, int], role: str) -> str:
    if role == "physical":
        return "NATURE_JOLLY" if stats["speed"] >= 80 else "NATURE_ADAMANT"
    if role == "special":
        return "NATURE_TIMID" if stats["speed"] >= 80 else "NATURE_MODEST"
    return "NATURE_BOLD" if stats["def"] <= stats["spdef"] else "NATURE_CALM"


def species_types(data: SpeciesData) -> set[str]:
    return {data.type1, data.type2}


def legal_move_pool(species: str, data: SpeciesData, level_up: dict[str, list[str]], full: dict[str, list[str]], move_info: dict[str, MoveData], z_source: str | None) -> list[str]:
    level_moves = list(level_up.get(data.level_up_table or "", []))
    full_moves = list(full.get(species, []))

    moves: list[str] = []
    if z_source:
        moves.append(z_source)
    for move in full_moves + list(reversed(level_moves)):
        if move in MOVE_BLACKLIST or move not in move_info:
            continue
        if move not in moves:
            moves.append(move)
    return moves


def add_move(moves: list[str], move: str, available: set[str]) -> bool:
    if move in available and move not in moves:
        moves.append(move)
        return True
    return False


def add_from_priority(moves: list[str], priority: tuple[str, ...], available: set[str], limit: int = 4) -> bool:
    for move in priority:
        if len(moves) >= limit:
            return False
        if add_move(moves, move, available):
            return True
    return False


def move_damage_score(move: str, data: SpeciesData, move_info: dict[str, MoveData], wanted_category: str | None, used_types: set[str]) -> int:
    info = move_info[move]
    if info.category == "DAMAGE_CATEGORY_STATUS":
        return -10000
    if wanted_category is not None and info.category != wanted_category:
        return -10000

    power = info.power
    if power <= 1:
        power = 45
    score = power
    if info.type in species_types(data):
        score += 35
    if info.type not in used_types:
        score += 15
    if move in HIGH_VALUE_ATTACKS:
        score += 25
    if move in PRIORITY_MOVES or info.priority > 0:
        score += 20
    if info.accuracy and info.accuracy < 90:
        score -= 90 - info.accuracy
    return score


def add_best_damage(moves: list[str], available: list[str], data: SpeciesData, move_info: dict[str, MoveData], wanted_category: str | None = None) -> bool:
    used_types = {move_info[move].type for move in moves if move in move_info}
    candidates = [move for move in available if move not in moves]
    candidates.sort(
        key=lambda move: move_damage_score(move, data, move_info, wanted_category, used_types),
        reverse=True,
    )
    for move in candidates:
        if move_damage_score(move, data, move_info, wanted_category, used_types) > -10000:
            moves.append(move)
            return True
    return False


def fill_moves(moves: list[str], available: list[str], data: SpeciesData, move_info: dict[str, MoveData]) -> list[str]:
    while len(moves) < 4 and add_best_damage(moves, available, data, move_info):
        pass
    for move in UTILITY_MOVES + PIVOT_MOVES + RECOVERY_MOVES:
        if len(moves) >= 4:
            break
        add_move(moves, move, set(available))
    for move in available:
        if len(moves) >= 4:
            break
        add_move(moves, move, set(available))
    while len(moves) < 4:
        moves.append("MOVE_NONE")
    return moves[:4]


def choose_moves(species: str, data: SpeciesData, level_up: dict[str, list[str]], full: dict[str, list[str]], move_info: dict[str, MoveData], z_source: str | None, slot: int) -> list[str]:
    if species == "SPECIES_DITTO":
        return ["MOVE_TRANSFORM", "MOVE_TRANSFORM", "MOVE_TRANSFORM", "MOVE_TRANSFORM"]

    required_z_move = z_source if slot == 1 else None
    available = legal_move_pool(species, data, level_up, full, move_info, required_z_move)
    available_set = set(available)
    role = choose_role(data, slot)
    moves: list[str] = []

    if required_z_move:
        add_move(moves, required_z_move, available_set)

    if role == "physical":
        if slot != 2:
            add_from_priority(moves, PHYSICAL_SETUP_MOVES, available_set)
        add_best_damage(moves, available, data, move_info, "DAMAGE_CATEGORY_PHYSICAL")
        add_from_priority(moves, PRIORITY_MOVES + PIVOT_MOVES, available_set)
        add_best_damage(moves, available, data, move_info, "DAMAGE_CATEGORY_PHYSICAL")
        if slot == 2:
            add_from_priority(moves, UTILITY_MOVES, available_set)
    elif role == "special":
        if slot != 2:
            add_from_priority(moves, SPECIAL_SETUP_MOVES, available_set)
        add_best_damage(moves, available, data, move_info, "DAMAGE_CATEGORY_SPECIAL")
        add_from_priority(moves, PIVOT_MOVES, available_set)
        if slot == 2:
            add_move(moves, "MOVE_HIDDEN_POWER", available_set)
            add_move(moves, "MOVE_TERA_BLAST", available_set)
        add_best_damage(moves, available, data, move_info, "DAMAGE_CATEGORY_SPECIAL")
    else:
        add_from_priority(moves, RECOVERY_MOVES, available_set)
        if "MOVE_REST" in moves:
            add_move(moves, "MOVE_SLEEP_TALK", available_set)
        add_from_priority(moves, UTILITY_MOVES, available_set, limit=3)
        preferred_category = "DAMAGE_CATEGORY_PHYSICAL" if data.stats["atk"] >= data.stats["spatk"] else "DAMAGE_CATEGORY_SPECIAL"
        add_best_damage(moves, available, data, move_info, preferred_category)
        add_from_priority(moves, PIVOT_MOVES, available_set)

    if role == "physical":
        while len(moves) < 4 and add_best_damage(moves, available, data, move_info, "DAMAGE_CATEGORY_PHYSICAL"):
            pass
    elif role == "special":
        while len(moves) < 4 and add_best_damage(moves, available, data, move_info, "DAMAGE_CATEGORY_SPECIAL"):
            pass

    return fill_moves(moves, available, data, move_info)


def choose_tera_type(data: SpeciesData, moves: list[str], move_info: dict[str, MoveData], slot: int) -> str:
    if slot == 3:
        return DEFENSIVE_TERA_TYPES[data.order % len(DEFENSIVE_TERA_TYPES)]
    for move in moves:
        info = move_info.get(move)
        if info and info.category != "DAMAGE_CATEGORY_STATUS" and info.type not in species_types(data):
            return info.type
    return data.type1


def z_item_for_moves(moves: list[str], move_info: dict[str, MoveData]) -> str:
    for move in moves:
        info = move_info.get(move)
        if info and info.category != "DAMAGE_CATEGORY_STATUS":
            return Z_CRYSTALS_BY_TYPE.get(info.type, "ITEM_NORMALIUM_Z")
    return "ITEM_NORMALIUM_Z"


def has_status_move(moves: list[str], move_info: dict[str, MoveData]) -> bool:
    return any(move_info.get(move, MoveData(move, 0, "TYPE_NORMAL", "DAMAGE_CATEGORY_STATUS", 0, 0, "EFFECT_HIT")).category == "DAMAGE_CATEGORY_STATUS" for move in moves)


def choose_held_item(data: SpeciesData, slot: int, role: str, moves: list[str], move_info: dict[str, MoveData], mega_items: list[str], z_item: str, z_source: str | None) -> str:
    if data.species == "SPECIES_DITTO":
        return ("ITEM_CHOICE_SCARF", "ITEM_QUICK_POWDER", "ITEM_FOCUS_SASH")[slot - 1]
    if z_source and slot == 1:
        return z_item
    if mega_items and slot <= len(mega_items):
        return mega_items[slot - 1]
    if data.species in EXTRA_SPECIES and slot == 3:
        return "ITEM_EVIOLITE"
    if "isParadox" in data.flags and slot != 3:
        return "ITEM_BOOSTER_ENERGY"
    if "MOVE_SHELL_SMASH" in moves or "MOVE_GEOMANCY" in moves:
        return "ITEM_WHITE_HERB"
    if any(move in MULTI_HIT_MOVES for move in moves):
        return "ITEM_LOADED_DICE"
    if slot == 2:
        return z_item_for_moves(moves, move_info)
    if role == "physical":
        if has_status_move(moves, move_info):
            return "ITEM_LIFE_ORB" if data.stats["speed"] >= 75 else "ITEM_CLEAR_AMULET"
        return "ITEM_LIFE_ORB" if data.stats["speed"] >= 75 else "ITEM_CHOICE_BAND"
    if role == "special":
        if has_status_move(moves, move_info):
            return "ITEM_LIFE_ORB"
        return "ITEM_LIFE_ORB" if data.stats["speed"] >= 75 else "ITEM_CHOICE_SPECS"
    if all(move_info.get(move, MoveData(move, 0, "TYPE_NORMAL", "DAMAGE_CATEGORY_STATUS", 0, 0, "EFFECT_HIT")).category != "DAMAGE_CATEGORY_STATUS" for move in moves):
        return "ITEM_ASSAULT_VEST"
    return "ITEM_LEFTOVERS" if data.stats["hp"] >= 80 else "ITEM_SITRUS_BERRY"


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
    move_info = parse_move_info()
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
            role = choose_role(data, slot)
            if data.species == "SPECIES_DITTO":
                role = "physical"
            moves = choose_moves(data.species, data, level_up, full, move_info, z_source, slot)
            held_item = choose_held_item(data, slot, role, moves, move_info, mega_items, z_item, z_source)
            lines.extend(
                [
                    f"    [{mon_label(data.species, slot)}] = {{",
                    f"        .species = {data.species},",
                    f"        .moves = {{{', '.join(moves)}}},",
                    f"        .heldItem = {held_item},",
                    f"        .ev = {choose_ev(data.stats, role)},",
                    f"        .nature = {choose_nature(data.stats, role)},",
                    "        .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),",
                    f"        .ability = {choose_ability(data.species, data.abilities, slot - 1)},",
                    f"        .teraType = {choose_tera_type(data, moves, move_info, slot)},",
                    "        .shouldUseDynamax = TRUE,",
                    "        .dynamaxLevel = 10,",
                    "        .ball = BALL_POKE,",
                    "        .gender = TRAINER_MON_RANDOM_GENDER,",
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
