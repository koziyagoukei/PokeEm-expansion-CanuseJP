#!/usr/bin/env python3

import argparse
import ast
import hashlib
import json
import re
import sys
from pathlib import Path


SCHEMA_VERSION = 1

ID_FILES = {
    "species_ids": "include/constants/species.h",
    "move_ids": "include/constants/moves.h",
    "ability_ids": "include/constants/abilities.h",
    "item_ids": "include/constants/items.h",
}

DATA_STRUCTS = {
    "pokemon_substruct_0": ("include/pokemon.h", "PokemonSubstruct0"),
    "pokemon_substruct_1": ("include/pokemon.h", "PokemonSubstruct1"),
    "pokemon_substruct_2": ("include/pokemon.h", "PokemonSubstruct2"),
    "pokemon_substruct_3": ("include/pokemon.h", "PokemonSubstruct3"),
    "box_pokemon": ("include/pokemon.h", "BoxPokemon"),
    "pokemon": ("include/pokemon.h", "Pokemon"),
    "battle_pokemon": ("include/pokemon.h", "BattlePokemon"),
    "battle_volatiles": ("include/pokemon.h", "Volatiles"),
    "contest_pokemon": ("include/contest.h", "ContestPokemon"),
    "contestant_status": ("include/contest.h", "ContestantStatus"),
    "contest_appeal_results": ("include/contest.h", "ContestAppealMoveResults"),
    "contest_excitement": ("include/contest.h", "ContestExcitement"),
    "link_player": ("include/link.h", "LinkPlayer"),
    "link_player_block": ("include/link.h", "LinkPlayerBlock"),
    "link_battler_header": ("include/battle.h", "LinkBattlerHeader"),
    "choose_move_packet": ("include/battle_controllers.h", "ChooseMoveStruct"),
    "multi_partner_menu_pokemon": ("include/battle_main.h", "MultiPartnerMenuPokemon"),
    "trainer_card": ("include/trainer_card.h", "TrainerCard"),
    "contest_move_selection_packet": ("src/contest_link.c", "LinkContestMoveSelection"),
    "rfu_sio_info": ("src/link_rfu_2.c", "SioInfo"),
}

VERSION_NAMES = ("LINK_COMPAT_VERSION", "LINK_DATA_LAYOUT_VERSION")
SIZE_NAMES = (
    "LINK_SIZEOF_BOX_POKEMON",
    "LINK_SIZEOF_POKEMON",
    "LINK_SIZEOF_BATTLE_POKEMON",
    "LINK_SIZEOF_CONTEST_POKEMON",
    "LINK_SIZEOF_LINK_PLAYER",
    "LINK_SIZEOF_LINK_PLAYER_BLOCK",
)


def read_text(root, relative_path):
    return (root / relative_path).read_text(encoding="utf-8")


def strip_comments(text):
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    return re.sub(r"//[^\r\n]*", "", text)


def normalize(text):
    return re.sub(r"\s+", "", strip_comments(text))


def find_matching_brace(text, open_pos):
    depth = 0
    for pos in range(open_pos, len(text)):
        if text[pos] == "{":
            depth += 1
        elif text[pos] == "}":
            depth -= 1
            if depth == 0:
                return pos
    raise ValueError("unterminated brace-delimited block")


def extract_struct(text, name):
    clean = strip_comments(text)
    match = re.search(r"\bstruct\s+" + re.escape(name) + r"\s*\{", clean)
    if match is None:
        raise ValueError("struct {} not found".format(name))
    open_pos = clean.find("{", match.start())
    close_pos = find_matching_brace(clean, open_pos)
    semicolon = clean.find(";", close_pos)
    if semicolon == -1:
        raise ValueError("struct {} has no trailing semicolon".format(name))
    return clean[match.start():semicolon + 1]


def extract_enum_containing(text, marker):
    clean = strip_comments(text)
    if marker not in clean:
        raise ValueError("enum marker {} not found".format(marker))
    for match in re.finditer(r"\benum\b[^;{]*\{", clean):
        open_pos = match.end() - 1
        close_pos = find_matching_brace(clean, open_pos)
        if clean.find(marker, open_pos, close_pos) != -1:
            return clean[match.start():close_pos + 1]
    raise ValueError("enum containing {} not found".format(marker))


def evaluate_integer_expression(expression, values):
    binary_ops = {
        ast.Add: lambda a, b: a + b,
        ast.Sub: lambda a, b: a - b,
        ast.Mult: lambda a, b: a * b,
        ast.FloorDiv: lambda a, b: a // b,
        ast.LShift: lambda a, b: a << b,
        ast.RShift: lambda a, b: a >> b,
        ast.BitOr: lambda a, b: a | b,
        ast.BitAnd: lambda a, b: a & b,
        ast.BitXor: lambda a, b: a ^ b,
    }
    unary_ops = {
        ast.UAdd: lambda value: value,
        ast.USub: lambda value: -value,
        ast.Invert: lambda value: ~value,
    }

    def evaluate(node):
        if isinstance(node, ast.Expression):
            return evaluate(node.body)
        if isinstance(node, ast.Constant) and isinstance(node.value, int):
            return node.value
        if isinstance(node, ast.Name) and node.id in values:
            return values[node.id]
        if isinstance(node, ast.BinOp) and type(node.op) in binary_ops:
            return binary_ops[type(node.op)](evaluate(node.left), evaluate(node.right))
        if isinstance(node, ast.UnaryOp) and type(node.op) in unary_ops:
            return unary_ops[type(node.op)](evaluate(node.operand))
        raise ValueError("unsupported enum expression: {}".format(expression))

    return evaluate(ast.parse(expression.strip(), mode="eval"))


def evaluate_enum(text, marker):
    enum_block = extract_enum_containing(text, marker)
    body = enum_block[enum_block.find("{") + 1:enum_block.rfind("}")]
    values = {}
    current = -1
    for raw_entry in body.split(","):
        entry = re.sub(r"^\s*#.*$", "", raw_entry, flags=re.MULTILINE).strip()
        if not entry:
            continue
        match = re.match(r"^([A-Za-z_][A-Za-z0-9_]*)(?:\s*=\s*(.+))?$", entry, flags=re.DOTALL)
        if match is None:
            raise ValueError("unsupported enum entry: {}".format(entry))
        name, expression = match.groups()
        current = current + 1 if expression is None else evaluate_integer_expression(expression, values)
        values[name] = current
    return values


def evaluate_enum_value(text, marker):
    try:
        return evaluate_enum(text, marker)[marker]
    except ValueError:
        enum_block = extract_enum_containing(text, marker)
        marker_match = re.search(r"\b{}\b".format(re.escape(marker)), enum_block)
        prefix = enum_block[:marker_match.start()]
        assignments = list(
            re.finditer(r"\b[A-Za-z_][A-Za-z0-9_]*\s*=\s*(0[xX][0-9A-Fa-f]+|[0-9]+)\s*,", prefix)
        )
        if not assignments:
            raise
        last = assignments[-1]
        value = int(last.group(1), 0)
        tail = prefix[last.end():]
        implicit_entries = re.findall(r"(?:^|,)\s*([A-Za-z_][A-Za-z0-9_]*)\s*(?=,|$)", tail)
        return value + len(implicit_entries) + 1


def extract_defines(text, predicate):
    clean = strip_comments(text)
    lines = clean.splitlines()
    records = []
    index = 0
    while index < len(lines):
        line = lines[index]
        match = re.match(r"\s*#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)\b", line)
        if match is None:
            index += 1
            continue
        record = line.strip()
        while record.rstrip().endswith("\\") and index + 1 < len(lines):
            index += 1
            record += "\n" + lines[index].strip()
        if predicate(match.group(1)):
            records.append(record)
        index += 1
    return "\n".join(records)


def parse_integer_defines(text, names):
    clean = strip_comments(text)
    values = {}
    for name in names:
        match = re.search(r"^\s*#\s*define\s+{}\s+(0[xX][0-9A-Fa-f]+|[0-9]+)\b".format(re.escape(name)), clean, re.MULTILINE)
        if match is None:
            raise ValueError("integer define {} not found".format(name))
        values[name] = int(match.group(1), 0)
    return values


def digest(text):
    return hashlib.sha256(text.encode("utf-8")).hexdigest()


def digest_component_map(components):
    aggregate = "".join("{}\0{}\n".format(name, components[name]) for name in sorted(components))
    return digest(aggregate)


def collect_fingerprint(root):
    config_text = read_text(root, "include/config/link_compat.h")
    link_text = read_text(root, "include/link.h")
    battle_text = read_text(root, "include/battle.h")
    versions = parse_integer_defines(config_text, VERSION_NAMES)
    sizes = parse_integer_defines(config_text, SIZE_NAMES)
    buffer_sizes = parse_integer_defines(link_text + "\n" + battle_text, ("BLOCK_BUFFER_SIZE", "BATTLE_BUFFER_LINK_SIZE"))

    data_components = {}
    identifier_counts = {}
    token_prefixes = {
        "species_ids": "SPECIES_",
        "move_ids": "MOVE_",
        "ability_ids": "ABILITY_",
        "item_ids": "ITEM_",
    }
    for name, relative_path in ID_FILES.items():
        text = read_text(root, relative_path)
        data_components[name] = digest(normalize(text))
        tokens = set(re.findall(r"\b{}[A-Z0-9_]+\b".format(token_prefixes[name]), strip_comments(text)))
        identifier_counts[name] = len(tokens)

    for name, (relative_path, struct_name) in DATA_STRUCTS.items():
        data_components[name] = digest(normalize(extract_struct(read_text(root, relative_path), struct_name)))

    layout_defines = extract_defines(
        config_text
        + "\n"
        + link_text
        + "\n"
        + battle_text
        + "\n"
        + read_text(root, "include/constants/global.h")
        + "\n"
        + read_text(root, "include/constants/pokemon.h"),
        lambda name: name.startswith("LINK_SIZEOF_")
        or name in {
            "MAX_LINK_PLAYERS",
            "MAX_RFU_PLAYERS",
            "BLOCK_BUFFER_SIZE",
            "BATTLE_BUFFER_LINK_SIZE",
            "MAX_MON_MOVES",
            "POKEMON_NAME_LENGTH",
            "PLAYER_NAME_LENGTH",
            "NUM_BATTLE_STATS",
        },
    )
    data_components["layout_constants"] = digest(normalize(layout_defines))
    data_components["battle_volatile_layout"] = digest(
        normalize(extract_defines(read_text(root, "include/constants/battle.h"), lambda name: name == "VOLATILE_DEFINITIONS"))
    )

    protocol_components = {}
    protocol_components["link_commands_and_types"] = digest(
        normalize(
            extract_defines(
                link_text,
                lambda name: name.startswith("LINKCMD_")
                or name.startswith("LINKTYPE_")
                or name in {
                    "MAX_LINK_PLAYERS",
                    "MAX_RFU_PLAYERS",
                    "CMD_LENGTH",
                    "QUEUE_CAPACITY",
                    "OVERWORLD_RECV_QUEUE_MAX",
                    "BLOCK_BUFFER_SIZE",
                },
            )
        )
    )
    union_room_text = read_text(root, "include/constants/union_room.h")
    protocol_components["union_room_activities"] = digest(
        normalize(extract_defines(union_room_text, lambda name: name.startswith("ACTIVITY_") or name.startswith("LINK_GROUP_")))
    )
    battle_constants = read_text(root, "include/constants/battle.h")
    protocol_components["battle_type_flags"] = digest(
        normalize(extract_defines(battle_constants, lambda name: name.startswith("BATTLE_TYPE_")))
    )
    controller_text = read_text(root, "include/battle_controllers.h")
    protocol_components["battle_controller_commands"] = digest(
        normalize(extract_enum_containing(controller_text, "CONTROLLER_GETMONDATA"))
    )
    protocol_components["battle_controller_requests"] = digest(
        normalize(extract_enum_containing(controller_text, "REQUEST_ALL_BATTLE"))
    )
    protocol_components["battle_gimmicks"] = digest(normalize(read_text(root, "include/battle_gimmick.h")))
    protocol_components["contest_gimmicks"] = digest(
        normalize(extract_enum_containing(read_text(root, "include/contest.h"), "CONTEST_GIMMICK_MEGA"))
    )
    protocol_components["battle_sync_config"] = digest(
        normalize(extract_defines(read_text(root, "include/config/battle.h"), lambda name: name.startswith("B_")))
    )
    protocol_components["gimmick_feature_flags"] = digest(
        normalize(
            extract_defines(
                read_text(root, "include/config/species_enabled.h"),
                lambda name: name in {
                    "P_MEGA_EVOLUTIONS",
                    "P_GEN_9_MEGA_EVOLUTIONS",
                    "P_GIGANTAMAX_FORMS",
                    "P_TERA_FORMS",
                },
            )
        )
    )

    data_hash = digest_component_map(data_components)
    protocol_hash = digest_component_map(protocol_components)
    combined_hash = digest("{}\0{}".format(data_hash, protocol_hash))
    species_values = evaluate_enum(read_text(root, "include/constants/species.h"), "NUM_SPECIES")
    move_values = evaluate_enum(read_text(root, "include/constants/moves.h"), "MOVES_COUNT_ALL")
    ability_values = evaluate_enum(read_text(root, "include/constants/abilities.h"), "ABILITIES_COUNT")
    item_count = evaluate_enum_value(read_text(root, "include/constants/items.h"), "ITEMS_COUNT")
    return {
        "schema_version": SCHEMA_VERSION,
        "link_compat_version": versions["LINK_COMPAT_VERSION"],
        "data_layout_version": versions["LINK_DATA_LAYOUT_VERSION"],
        "fingerprints": {
            "data_layout": data_hash,
            "protocol": protocol_hash,
            "combined": combined_hash,
        },
        "summary": {
            "identifier_counts": identifier_counts,
            "struct_sizes": sizes,
            "buffer_sizes": {
                "link_block": buffer_sizes["BLOCK_BUFFER_SIZE"],
                "battle_link": buffer_sizes["BATTLE_BUFFER_LINK_SIZE"],
            },
            "controller_command_end": "CONTROLLER_CMDS_COUNT",
            "enum_values": {
                "NUM_SPECIES": species_values["NUM_SPECIES"],
                "MOVES_COUNT": move_values["MOVES_COUNT"],
                "MOVES_COUNT_ALL": move_values["MOVES_COUNT_ALL"],
                "ABILITIES_COUNT": ability_values["ABILITIES_COUNT"],
                "ITEMS_COUNT": item_count,
            },
        },
        "components": {
            "data_layout": data_components,
            "protocol": protocol_components,
        },
    }


def changed_components(baseline, current, category):
    old = baseline.get("components", {}).get(category, {})
    new = current.get("components", {}).get(category, {})
    return sorted(name for name in set(old) | set(new) if old.get(name) != new.get(name))


def compare_fingerprints(baseline, current):
    errors = []
    warnings = []
    checks = (
        ("data_layout", "data_layout_version", "LINK_DATA_LAYOUT_VERSION"),
        ("protocol", "link_compat_version", "LINK_COMPAT_VERSION"),
    )
    for category, version_key, define_name in checks:
        if baseline["fingerprints"][category] == current["fingerprints"][category]:
            continue
        changed = ", ".join(changed_components(baseline, current, category))
        old_version = baseline[version_key]
        new_version = current[version_key]
        if new_version <= old_version:
            errors.append(
                "{} changed ({}) but {} was not increased (baseline {}, current {}).".format(
                    category, changed, define_name, old_version, new_version
                )
            )
        else:
            warnings.append(
                "{} changed ({}) and {} increased to {}; update the baseline with --update.".format(
                    category, changed, define_name, new_version
                )
            )
    return errors, warnings


def compare_versions(baseline, current):
    errors = []
    updates = []
    for version_key, define_name in (
        ("data_layout_version", "LINK_DATA_LAYOUT_VERSION"),
        ("link_compat_version", "LINK_COMPAT_VERSION"),
    ):
        old_version = baseline[version_key]
        new_version = current[version_key]
        if new_version < old_version:
            errors.append("{} decreased from {} to {}.".format(define_name, old_version, new_version))
        elif new_version > old_version:
            updates.append("{} increased from {} to {}.".format(define_name, old_version, new_version))
    return errors, updates


def run_self_test():
    base = {
        "link_compat_version": 1,
        "data_layout_version": 1,
        "fingerprints": {"data_layout": "data-a", "protocol": "protocol-a"},
        "components": {"data_layout": {"pokemon": "a"}, "protocol": {"commands": "a"}},
    }
    same = json.loads(json.dumps(base))
    assert compare_fingerprints(base, same) == ([], [])

    data_changed = json.loads(json.dumps(base))
    data_changed["fingerprints"]["data_layout"] = "data-b"
    data_changed["components"]["data_layout"]["pokemon"] = "b"
    errors, warnings = compare_fingerprints(base, data_changed)
    assert len(errors) == 1 and not warnings and "LINK_DATA_LAYOUT_VERSION" in errors[0]

    data_changed["data_layout_version"] = 2
    errors, warnings = compare_fingerprints(base, data_changed)
    assert not errors and len(warnings) == 1

    protocol_changed = json.loads(json.dumps(base))
    protocol_changed["fingerprints"]["protocol"] = "protocol-b"
    protocol_changed["components"]["protocol"]["commands"] = "b"
    errors, warnings = compare_fingerprints(base, protocol_changed)
    assert len(errors) == 1 and not warnings and "LINK_COMPAT_VERSION" in errors[0]
    assert evaluate_enum("enum __attribute__((packed)) Example { A = 3, B, C = B + 2, COUNT };", "COUNT")["COUNT"] == 7
    print("link compatibility fingerprint self-test passed")


def main():
    default_root = Path(__file__).resolve().parents[2]
    parser = argparse.ArgumentParser(description="Check communication-sensitive source fingerprints.")
    parser.add_argument("--root", type=Path, default=default_root)
    parser.add_argument("--baseline", type=Path)
    parser.add_argument("--stamp", type=Path)
    parser.add_argument("--update", action="store_true")
    parser.add_argument("--self-test", action="store_true")
    args = parser.parse_args()

    if args.self_test:
        run_self_test()
        return 0

    root = args.root.resolve()
    baseline_path = args.baseline or root / "tools/link_compat/fingerprint.json"
    if not baseline_path.is_absolute():
        baseline_path = root / baseline_path
    current = collect_fingerprint(root)

    if not baseline_path.exists() and not args.update:
        print("error: link compatibility fingerprint baseline is missing: {}".format(baseline_path), file=sys.stderr)
        return 1

    if args.update:
        if baseline_path.exists():
            baseline = json.loads(baseline_path.read_text(encoding="utf-8"))
            if baseline.get("schema_version") != SCHEMA_VERSION:
                print("error: unsupported link compatibility fingerprint schema", file=sys.stderr)
                return 1
            errors, warnings = compare_fingerprints(baseline, current)
            version_errors, version_updates = compare_versions(baseline, current)
            errors.extend(version_errors)
            if errors:
                for error in errors:
                    print("error: {}".format(error), file=sys.stderr)
                print("error: refusing to update the baseline without the required version increase.", file=sys.stderr)
                return 1
            for warning in warnings:
                print("accepted: {}".format(warning), file=sys.stderr)
            for update in version_updates:
                print("accepted: {}".format(update), file=sys.stderr)
        baseline_path.parent.mkdir(parents=True, exist_ok=True)
        baseline_path.write_text(json.dumps(current, indent=2, sort_keys=True) + "\n", encoding="utf-8")
        print("updated link compatibility fingerprint: {}".format(baseline_path))
        return 0

    baseline = json.loads(baseline_path.read_text(encoding="utf-8"))
    if baseline.get("schema_version") != SCHEMA_VERSION:
        print("error: unsupported link compatibility fingerprint schema", file=sys.stderr)
        return 1

    errors, warnings = compare_fingerprints(baseline, current)
    version_errors, version_updates = compare_versions(baseline, current)
    errors.extend(version_errors)
    for warning in warnings:
        print("warning: {}".format(warning), file=sys.stderr)
    for update in version_updates:
        print("warning: {}".format(update), file=sys.stderr)
    for error in errors:
        print("error: {}".format(error), file=sys.stderr)
    if errors:
        print("error: bump the required version in include/config/link_compat.h before updating the baseline.", file=sys.stderr)
        return 1
    if warnings or version_updates:
        print("error: the reviewed version change must be recorded with --update.", file=sys.stderr)
        return 1

    if args.stamp is not None:
        stamp_path = args.stamp if args.stamp.is_absolute() else root / args.stamp
        stamp_path.parent.mkdir(parents=True, exist_ok=True)
        stamp_path.write_text(current["fingerprints"]["combined"] + "\n", encoding="ascii")
    print(
        "link compatibility fingerprint OK (compat {}, data {})".format(
            current["link_compat_version"], current["data_layout_version"]
        )
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
