from __future__ import annotations

import argparse
import json
import re
import shutil
import subprocess
import sys
import time
import zlib
from dataclasses import dataclass
from pathlib import Path
from typing import Any

from .event_maps import bundle_to_dict, list_event_maps, load_map_event_bundle
from .event_maps import add_map_script_label, add_map_script_with_text, add_map_text_label
from .event_maps import rename_map_script_label, save_script_label, save_text_label, search_map_scripts
from .palette import PaletteError, write_jasc_pal_from_png


TEXT_EXTENSIONS = {".c", ".h", ".inc"}
FRONTIER_MON_FIELDS = [
    "nickname", "species", "moves", "heldItem", "ev", "iv", "ability", "lvl", "ball", "friendship",
    "nature", "gender", "isShiny", "teraType", "gigantamaxFactor", "shouldUseDynamax",
    "dynamaxLevel", "tags",
]


class CliError(Exception):
    def __init__(self, code: str, message: str, details: dict[str, Any] | None = None) -> None:
        super().__init__(message)
        self.code = code
        self.message = message
        self.details = details or {}


def rel(root: Path, path: Path) -> str:
    try:
        return path.relative_to(root).as_posix()
    except ValueError:
        return path.name


def read_utf8(path: Path) -> str:
    with path.open("r", encoding="utf-8", newline="") as handle:
        return handle.read()


def write_utf8_with_backup(path: Path, source: str) -> Path:
    backup = path.with_name(path.name + ".bak")
    shutil.copy2(path, backup)
    with path.open("w", encoding="utf-8", newline="") as handle:
        handle.write(source)
    return backup


def detect_newline(source: str) -> str:
    return "\r\n" if "\r\n" in source else "\n"


def skip_space(text: str, pos: int) -> int:
    while pos < len(text) and text[pos].isspace():
        pos += 1
    return pos


def skip_string(text: str, pos: int, quote: str) -> int:
    pos += 1
    while pos < len(text):
        if text[pos] == "\\":
            pos += 2
        elif text[pos] == quote:
            return pos + 1
        else:
            pos += 1
    return pos


def balanced_end(text: str, open_pos: int, opener: str = "{", closer: str = "}") -> int | None:
    depth, pos = 0, open_pos
    while pos < len(text):
        if text.startswith("//", pos):
            end = text.find("\n", pos + 2)
            pos = len(text) if end < 0 else end + 1
            continue
        if text.startswith("/*", pos):
            end = text.find("*/", pos + 2)
            pos = len(text) if end < 0 else end + 2
            continue
        if text[pos] in "\"'":
            pos = skip_string(text, pos, text[pos])
            continue
        if text[pos] == opener:
            depth += 1
        elif text[pos] == closer:
            depth -= 1
            if depth == 0:
                return pos + 1
        pos += 1
    return None


def parse_c_strings(text: str, pos: int) -> tuple[int, int, str] | None:
    pos = skip_space(text, pos)
    start = None
    parts: list[str] = []
    end = pos
    while pos < len(text) and text[pos] == '"':
        if start is None:
            start = pos
        pos += 1
        buf: list[str] = []
        while pos < len(text):
            char = text[pos]
            if char == "\\" and pos + 1 < len(text):
                buf.append(text[pos:pos + 2])
                pos += 2
            elif char == '"':
                pos += 1
                break
            else:
                buf.append(char)
                pos += 1
        parts.append("".join(buf))
        end = pos
        pos = skip_space(text, pos)
    return None if start is None else (start, end, "".join(parts))


def raw_field(block: str, name: str) -> str | None:
    match = re.search(rf"\.{re.escape(name)}\s*=\s*", block)
    if not match:
        return None
    start = match.end()
    pos = start
    depth = 0
    while pos < len(block):
        char = block[pos]
        if char in "\"'":
            pos = skip_string(block, pos, char)
            continue
        if char in "({[":
            depth += 1
        elif char in ")}]":
            if depth == 0:
                return block[start:pos].strip()
            depth -= 1
        elif char == "," and depth == 0:
            return block[start:pos].strip()
        pos += 1
    return block[start:pos].strip() if start < pos else None


def infer_symbol(text: str, macro_pos: int) -> str:
    context = text[max(0, macro_pos - 700):macro_pos]
    found = list(re.finditer(r"\b(gText_[A-Za-z0-9_]+)\b", context))
    if found:
        return found[-1].group(1)
    field = list(re.finditer(r"\.([A-Za-z_][A-Za-z0-9_]*)\s*=\s*$", context))
    if field:
        return "." + field[-1].group(1)
    return "(anonymous)"


def require_project(root: Path) -> None:
    if not root.exists():
        raise CliError("root_not_found", "Repository root does not exist.", {"root": str(root)})
    if not (root / "src").exists() or not (root / "include").exists():
        raise CliError("invalid_project", "Root does not look like pokeemerald-expansion.", {"root": str(root)})


def find_identifiers(root: Path, files: list[str], prefix: str) -> set[str]:
    identifiers: set[str] = set()
    pattern = re.compile(rf"\b{re.escape(prefix)}[A-Z0-9_]+\b")
    for file_name in files:
        path = root / file_name
        if not path.exists():
            continue
        try:
            identifiers.update(pattern.findall(read_utf8(path)))
        except UnicodeDecodeError:
            continue
    return identifiers


def parse_constant_values(root: Path, files: list[str], prefix: str) -> dict[str, int]:
    values: dict[str, int] = {}
    current = 0
    entry_re = re.compile(rf"^\s*({re.escape(prefix)}[A-Z0-9_]+)(?:\s*=\s*([^,/\n]+))?\s*,?")
    define_re = re.compile(rf"^\s*#define\s+({re.escape(prefix)}[A-Z0-9_]+)\s+([0-9]+|0x[0-9A-Fa-f]+)\b")
    for file_name in files:
        path = root / file_name
        if not path.exists():
            continue
        try:
            lines = read_utf8(path).splitlines()
        except UnicodeDecodeError:
            continue
        for line in lines:
            define_match = define_re.match(line)
            if define_match:
                values[define_match.group(1)] = int(define_match.group(2), 0)
                continue
            entry_match = entry_re.match(line)
            if not entry_match:
                continue
            name, raw_value = entry_match.groups()
            if raw_value:
                raw_value = raw_value.strip()
                try:
                    current = int(raw_value, 0)
                except ValueError:
                    current = values.get(raw_value, current)
            values[name] = current
            current += 1
    return values


@dataclass
class ProjectConstants:
    species: set[str]
    moves: set[str]
    items: set[str]
    abilities: set[str]
    types: set[str]
    ability_values: dict[str, int]


def load_project_constants(root: Path) -> ProjectConstants:
    return ProjectConstants(
        species=find_identifiers(root, ["include/constants/species.h", "src/data/pokemon/species_info.h"], "SPECIES_"),
        moves=find_identifiers(root, ["include/constants/moves.h", "src/data/moves_info.h"], "MOVE_"),
        items=find_identifiers(root, ["include/constants/items.h", "src/data/items.h"], "ITEM_"),
        abilities=find_identifiers(root, ["include/constants/abilities.h", "src/data/abilities.h"], "ABILITY_"),
        types=find_identifiers(root, ["include/constants/pokemon.h", "src/data/types_info.h"], "TYPE_"),
        ability_values=parse_constant_values(root, ["include/constants/abilities.h"], "ABILITY_"),
    )


def as_fields(mon: dict[str, Any]) -> dict[str, Any]:
    if isinstance(mon.get("fields"), dict):
        fields = dict(mon["fields"])
    else:
        fields = {}
    aliases = {
        "item": "heldItem",
        "held_item": "heldItem",
        "level": "lvl",
        "shiny": "isShiny",
        "tera_type": "teraType",
        "dynamax_level": "dynamaxLevel",
        "gigantamax": "gigantamaxFactor",
    }
    for key, value in mon.items():
        if key in {"id", "key", "file", "line", "fields", "source", "type"}:
            continue
        fields[aliases.get(key, key)] = value
    return fields


def one_identifier(value: Any, prefix: str) -> str | None:
    if value is None:
        return None
    match = re.search(rf"\b{re.escape(prefix)}[A-Z0-9_]+\b", str(value))
    return match.group(0) if match else None


def all_identifiers(value: Any, prefix: str) -> list[str]:
    if value is None:
        return []
    if isinstance(value, list):
        raw = " ".join(str(item) for item in value)
    else:
        raw = str(value)
    return re.findall(rf"\b{re.escape(prefix)}[A-Z0-9_]+\b", raw)


def parse_int_literal(value: Any) -> int | None:
    if value is None:
        return None
    if isinstance(value, bool):
        return int(value)
    if isinstance(value, int):
        return value
    raw = str(value).strip()
    if re.fullmatch(r"[+-]?(?:0x[0-9A-Fa-f]+|[0-9]+)", raw):
        return int(raw, 0)
    return None


def numeric_arguments(value: Any) -> list[int]:
    if value is None:
        return []
    if isinstance(value, list):
        numbers: list[int] = []
        for item in value:
            parsed = parse_int_literal(item)
            if parsed is not None:
                numbers.append(parsed)
        return numbers
    return [int(match, 0) for match in re.findall(r"(?<![A-Za-z0-9_])(?:0x[0-9A-Fa-f]+|[0-9]+)(?![A-Za-z0-9_])", str(value))]


def c_escape(value: str) -> str:
    return value.replace("\\", "\\\\").replace('"', '\\"')


def format_field_value(field: str, value: Any) -> str:
    if value is None:
        raise CliError("invalid_monpool_field", f"Field {field} cannot be null.", {"field": field})
    if isinstance(value, bool):
        return "TRUE" if value else "FALSE"
    if isinstance(value, int):
        return str(value)
    if isinstance(value, list):
        if field == "ev" and len(value) == 6 and all(parse_int_literal(item) is not None for item in value):
            return "TRAINER_PARTY_EVS(" + ", ".join(str(int(item)) for item in value) + ")"
        if field == "iv" and len(value) == 6 and all(parse_int_literal(item) is not None for item in value):
            return "TRAINER_PARTY_IVS(" + ", ".join(str(int(item)) for item in value) + ")"
        return "{" + ", ".join(str(item) for item in value) + "}"
    if not isinstance(value, str):
        raise CliError("invalid_monpool_field", f"Field {field} has an unsupported JSON value.", {"field": field, "value_type": type(value).__name__})
    raw = value.strip()
    if not raw:
        raise CliError("invalid_monpool_field", f"Field {field} cannot be blank.", {"field": field})
    if field == "nickname" and not re.search(r"\b(?:J_)?COMPOUND_STRING\s*\(", raw) and not raw.startswith('"') and not raw.startswith("gText_"):
        return f'J_COMPOUND_STRING("{c_escape(raw)}")'
    return raw


def field_order_from_block(block: str) -> list[str]:
    seen: set[str] = set()
    order: list[str] = []
    for match in re.finditer(r"\.([A-Za-z_][A-Za-z0-9_]*)\s*=", block):
        field = match.group(1)
        if field not in seen:
            seen.add(field)
            order.append(field)
    return order


def build_initializer_block(key: str, fields: dict[str, Any], newline: str, preferred_order: list[str] | None = None) -> str:
    ordered = [field for field in (preferred_order or []) if field in fields]
    ordered.extend(field for field in FRONTIER_MON_FIELDS if field in fields and field not in ordered)
    ordered.extend(field for field in fields if field not in FRONTIER_MON_FIELDS and field not in ordered)
    lines = [f"    [{key}] = {{"]
    for field in ordered:
        lines.append(f"        .{field} = {format_field_value(field, fields[field])},")
    lines.append("    },")
    return newline.join(lines)


def find_joined_designators(source: str) -> list[dict[str, Any]]:
    joined: list[dict[str, Any]] = []
    pattern = re.compile(r"\.[A-Za-z_][A-Za-z0-9_]*\s*=.*\.[A-Za-z_][A-Za-z0-9_]*\s*=")
    for line_no, line in enumerate(source.splitlines(), start=1):
        if pattern.search(line):
            joined.append({"line": line_no, "preview": line.strip()[:240]})
    return joined


def mon_entries_from_input(input_data: Any) -> list[dict[str, Any]]:
    if isinstance(input_data, list):
        return [entry for entry in input_data if isinstance(entry, dict)]
    if not isinstance(input_data, dict):
        raise CliError("invalid_input_json", "Input JSON must be an object or an array.", {})
    for key in ("mons", "monpool", "pokemon", "records"):
        value = input_data.get(key)
        if isinstance(value, list):
            return [entry for entry in value if isinstance(entry, dict)]
    if isinstance(input_data.get("mon"), dict):
        return [input_data["mon"]]
    if "fields" in input_data or "species" in input_data:
        return [input_data]
    return []


def validate_mon_record(mon: dict[str, Any], constants: ProjectConstants, seen_ids: set[str] | None = None) -> tuple[list[dict[str, Any]], list[dict[str, Any]]]:
    errors: list[dict[str, Any]] = []
    warnings: list[dict[str, Any]] = []
    mon_id = mon.get("id") or mon.get("key")
    fields = as_fields(mon)
    if not mon_id:
        errors.append({"code": "missing_id", "message": "Mon ID is blank."})
    elif seen_ids is not None:
        if mon_id in seen_ids:
            errors.append({"code": "duplicate_id", "id": mon_id, "message": "Mon ID is duplicated."})
        seen_ids.add(str(mon_id))

    species = one_identifier(fields.get("species"), "SPECIES_")
    if not species:
        errors.append({"code": "missing_species", "id": mon_id, "message": "Species is blank or not a SPECIES_* token."})
    elif species not in constants.species:
        errors.append({"code": "unknown_species", "id": mon_id, "value": species, "message": "Species does not exist."})

    moves = all_identifiers(fields.get("moves"), "MOVE_")
    if len(moves) != 4:
        errors.append({"code": "invalid_move_count", "id": mon_id, "count": len(moves), "message": "Exactly four moves are required."})
    for move in moves:
        if move not in constants.moves:
            errors.append({"code": "unknown_move", "id": mon_id, "value": move, "message": "Move does not exist."})

    item = one_identifier(fields.get("heldItem"), "ITEM_")
    if fields.get("heldItem") is not None and not item:
        errors.append({"code": "invalid_item", "id": mon_id, "value": str(fields.get("heldItem")), "message": "Held item is not an ITEM_* token."})
    elif item and item not in constants.items:
        errors.append({"code": "unknown_item", "id": mon_id, "value": item, "message": "Held item does not exist."})

    ability_value = fields.get("ability")
    if ability_value is not None:
        ability = one_identifier(ability_value, "ABILITY_")
        if ability:
            if ability not in constants.abilities:
                errors.append({"code": "unknown_ability", "id": mon_id, "value": ability, "message": "Ability does not exist."})
        else:
            ability_number = parse_int_literal(ability_value)
            max_ability = max(constants.ability_values.values(), default=0)
            if ability_number is None or ability_number < 0 or ability_number > max_ability:
                errors.append({"code": "invalid_ability_number", "id": mon_id, "value": str(ability_value), "message": "Ability number is invalid."})

    evs = numeric_arguments(fields.get("ev"))
    if evs and sum(evs) > 510:
        errors.append({"code": "ev_total_too_high", "id": mon_id, "total": sum(evs), "message": "EV total exceeds 510."})

    iv_value = fields.get("iv")
    ivs = numeric_arguments(iv_value)
    if iv_value is not None:
        if not ivs:
            warnings.append({"code": "unparsed_iv", "id": mon_id, "value": str(iv_value), "message": "IV expression could not be fully parsed."})
        for iv in ivs:
            if iv < 0 or iv > 31:
                errors.append({"code": "iv_out_of_range", "id": mon_id, "value": iv, "message": "IV must be 0 to 31."})

    level_value = fields.get("lvl")
    if level_value is not None:
        level = parse_int_literal(level_value)
        if level is None:
            warnings.append({"code": "unparsed_level", "id": mon_id, "value": str(level_value), "message": "Level expression could not be parsed."})
        elif level < 1 or level > 100:
            errors.append({"code": "level_out_of_range", "id": mon_id, "value": level, "message": "Level must be 1 to 100."})

    tera_value = fields.get("teraType")
    if tera_value is not None:
        tera_type = one_identifier(tera_value, "TYPE_")
        if tera_type:
            if tera_type not in constants.types:
                errors.append({"code": "unknown_tera_type", "id": mon_id, "value": tera_type, "message": "Tera type does not exist."})
        else:
            tera_number = parse_int_literal(tera_value)
            if tera_number is None or tera_number < 0 or tera_number >= max(1, len(constants.types)):
                errors.append({"code": "invalid_tera_type", "id": mon_id, "value": str(tera_value), "message": "Tera type is invalid."})

    dynamax_value = fields.get("dynamaxLevel")
    if dynamax_value is not None and str(dynamax_value).strip() != "MAX_DYNAMAX_LEVEL":
        dynamax_level = parse_int_literal(dynamax_value)
        if dynamax_level is None:
            warnings.append({"code": "unparsed_dynamax_level", "id": mon_id, "value": str(dynamax_value), "message": "Dynamax level expression could not be parsed."})
        elif dynamax_level < 0 or dynamax_level > 10:
            errors.append({"code": "dynamax_level_out_of_range", "id": mon_id, "value": dynamax_level, "message": "Dynamax level must be 0 to 10."})

    for field in ("species", "moves"):
        if field in fields and isinstance(fields[field], str) and not fields[field].strip():
            errors.append({"code": "blank_field", "id": mon_id, "field": field, "message": "Required field is blank."})
    return errors, warnings


def default_frontier_source() -> str:
    return "src/data/battle_frontier/battle_frontier_mons.h"


def command_validate_project(root: Path, _args: argparse.Namespace, _input: dict[str, Any]) -> dict[str, Any]:
    if not root.exists():
        raise CliError("root_not_found", "Repository root does not exist.", {"root": str(root)})
    required = ["src", "include", "data", "graphics", "charmap.txt", "Makefile"]
    optional = ["pokeemerald.gba", "pokeemerald.elf", "pokeemerald.map", "src/data/battle_frontier/battle_frontier_mons.h"]
    required_status = {path: (root / path).exists() for path in required}
    optional_status = {path: (root / path).exists() for path in optional}
    missing = [path for path, exists in required_status.items() if not exists]
    return {
        "ok": not missing,
        "root": str(root),
        "root_display": root.name,
        "required": required_status,
        "optional": optional_status,
        "missing": missing,
    }


def command_scan_texts(root: Path, args: argparse.Namespace, _input: dict[str, Any]) -> dict[str, Any]:
    require_project(root)
    query = (args.query or "").casefold()
    entries: list[dict[str, Any]] = []
    macro_re = re.compile(r"(?<![A-Za-z0-9_])(COMPOUND_STRING|_)\s*\(")
    for base_name in ("src", "include"):
        base = root / base_name
        if not base.exists():
            continue
        for path in sorted((p for p in base.rglob("*") if p.suffix in TEXT_EXTENSIONS and p.is_file()), key=lambda p: p.as_posix().casefold()):
            try:
                source = read_utf8(path)
            except UnicodeDecodeError:
                continue
            if "COMPOUND_STRING" not in source and "_(" not in source and "_ (" not in source:
                continue
            used: set[tuple[int, int]] = set()
            for match in macro_re.finditer(source):
                parsed = parse_c_strings(source, match.end())
                if not parsed:
                    continue
                start, end, value = parsed
                if (start, end) in used:
                    continue
                used.add((start, end))
                symbol = infer_symbol(source, match.start())
                file_name = rel(root, path)
                haystack = "\n".join((file_name, symbol, match.group(1), value)).casefold()
                if query and query not in haystack:
                    continue
                entries.append({
                    "type": "text",
                    "file": file_name,
                    "line": source.count("\n", 0, start) + 1,
                    "macro": match.group(1),
                    "symbol": symbol,
                    "text": value,
                })
    return {"ok": True, "count": len(entries), "texts": entries}


def c_text_literal(value: str) -> str:
    normalized = value.replace("\r\n", "\n").replace("\r", "\n")
    normalized = normalized.replace('"', '\\"').replace("\n", "\\n")
    return f'"{normalized}"'


def resolve_project_file(root: Path, file_name: str) -> Path:
    path = (root / file_name).resolve()
    root_resolved = root.resolve()
    try:
        path.relative_to(root_resolved)
    except ValueError as error:
        raise CliError("path_outside_root", "File path is outside the project root.", {"file": file_name}) from error
    if not path.exists() or not path.is_file():
        raise CliError("file_not_found", "Target file was not found.", {"file": file_name})
    return path


def resolve_project_output(root: Path, file_name: str) -> Path:
    path = (root / file_name).resolve()
    root_resolved = root.resolve()
    try:
        path.relative_to(root_resolved)
    except ValueError as error:
        raise CliError("path_outside_root", "Output path is outside the project root.", {"file": file_name}) from error
    return path


def find_translation_text_targets(root: Path, file_name: str, line: int | None = None, symbol: str = "", macro: str = "") -> list[dict[str, Any]]:
    path = resolve_project_file(root, file_name)
    source = read_utf8(path)
    macro_re = re.compile(r"(?<![A-Za-z0-9_])(COMPOUND_STRING|_)\s*\(")
    matches: list[dict[str, Any]] = []
    for match in macro_re.finditer(source):
        parsed = parse_c_strings(source, match.end())
        if not parsed:
            continue
        start, end, value = parsed
        line_number = source.count("\n", 0, start) + 1
        found_symbol = infer_symbol(source, match.start())
        found_macro = match.group(1)
        if line is not None and line_number != line:
            continue
        if symbol and found_symbol != symbol:
            continue
        if macro and found_macro != macro:
            continue
        matches.append({
            "path": path,
            "source": source,
            "start": start,
            "end": end,
            "line": line_number,
            "symbol": found_symbol,
            "macro": found_macro,
            "text": value,
            "file": rel(root, path),
        })
    return matches


def command_update_text(root: Path, args: argparse.Namespace, input_data: Any) -> dict[str, Any]:
    require_project(root)
    file_name = require_text_value(input_value(args, input_data, "file"), "file")
    new_text = require_text_value(input_value(args, input_data, "text"), "text")
    raw_line = input_value(args, input_data, "line")
    symbol = str(input_value(args, input_data, "symbol", "") or "")
    macro = str(input_value(args, input_data, "macro", "") or "")
    line = None
    if raw_line is not None and str(raw_line).strip():
        try:
            line = int(raw_line)
        except ValueError as error:
            raise CliError("invalid_line", "line must be an integer.", {"line": raw_line}) from error
    if line is None and not symbol:
        raise CliError("missing_argument", "Either line or symbol is required.", {"argument": "line|symbol"})
    matches = find_translation_text_targets(root, file_name, line, symbol, macro)
    if not matches:
        raise CliError("text_target_not_found", "No matching translation text target was found.", {"file": file_name, "line": line, "symbol": symbol, "macro": macro})
    if len(matches) != 1:
        raise CliError("ambiguous_text_target", "Multiple matching translation text targets were found.", {
            "file": file_name,
            "matches": [{"line": item["line"], "symbol": item["symbol"], "macro": item["macro"], "text": item["text"]} for item in matches[:20]],
        })
    target = matches[0]
    source = str(target["source"])
    new_source = source[:int(target["start"])] + c_text_literal(new_text) + source[int(target["end"]):]
    backup = write_utf8_with_backup(target["path"], new_source)
    return {
        "ok": True,
        "file": target["file"],
        "backup": rel(root, backup),
        "line": target["line"],
        "symbol": target["symbol"],
        "macro": target["macro"],
    }


def command_scan_frontier(root: Path, _args: argparse.Namespace, _input: dict[str, Any]) -> dict[str, Any]:
    require_project(root)
    paths = {
        "frontier_pool": root / "src/data/battle_frontier/battle_frontier_mons.h",
        "frontier_pool_constants": root / "include/constants/battle_frontier_mons.h",
        "frontier_trainer_mons": root / "src/data/battle_frontier/battle_frontier_trainer_mons.h",
        "battle_factory": root / "src/battle_factory.c",
        "moves": root / "src/data/moves_info.h",
        "items": root / "src/data/items.h",
        "abilities": root / "src/data/abilities.h",
        "frontier_full_learnsets": root / "src/data/pokemon/frontier_full_learnsets.h",
    }
    return {
        "ok": True,
        "paths": {
            key: {"file": rel(root, path), "exists": path.exists()}
            for key, path in paths.items()
        },
    }


def command_png_to_jasc_pal(root: Path, args: argparse.Namespace, input_data: Any) -> dict[str, Any]:
    require_project(root)
    png_name = require_text_value(input_value(args, input_data, "png"), "png")
    output_name = require_text_value(input_value(args, input_data, "output"), "output")
    raw_colors = input_value(args, input_data, "colors", 16)
    try:
        palette_size = int(raw_colors)
    except (TypeError, ValueError) as error:
        raise CliError("invalid_palette_size", "colors must be 16, 224, or 256.", {"colors": raw_colors}) from error
    png_path = resolve_project_file(root, png_name)
    output_path = resolve_project_output(root, output_name)
    try:
        result = write_jasc_pal_from_png(png_path, output_path, palette_size)
    except (OSError, PaletteError, zlib.error) as error:
        raise CliError("png_to_jasc_pal_failed", str(error), {"png": png_name, "output": output_name, "colors": palette_size}) from error
    result["png"] = rel(root, png_path)
    result["output"] = rel(root, output_path)
    if result.get("backup"):
        result["backup"] = rel(root, Path(str(result["backup"])))
    return result


def command_scan_map_events(root: Path, args: argparse.Namespace, _input: Any) -> dict[str, Any]:
    require_project(root)
    map_name = getattr(args, "map", None)
    if not map_name:
        maps = list_event_maps(root)
        return {
            "ok": True,
            "count": len(maps),
            "maps": [
                {
                    "name": entry.name,
                    "map_id": entry.map_id,
                    "file": entry.file_name,
                    "has_scripts": entry.has_scripts,
                    "has_text": entry.has_text,
                    "object_events": entry.object_count,
                    "warp_events": entry.warp_count,
                    "coord_events": entry.coord_count,
                    "bg_events": entry.bg_count,
                }
                for entry in maps
            ],
        }
    bundle = load_map_event_bundle(root, map_name)
    payload = bundle_to_dict(root, bundle, include_bodies=bool(getattr(args, "include_bodies", False)))
    payload["ok"] = True
    return payload


def input_value(args: argparse.Namespace, input_data: Any, key: str, default: Any = None) -> Any:
    if isinstance(input_data, dict) and key in input_data:
        return input_data[key]
    return getattr(args, key, default)


def require_text_value(value: Any, name: str) -> str:
    if value is None:
        raise CliError("missing_argument", f"{name} is required.", {"argument": name})
    value = str(value)
    if not value.strip():
        raise CliError("missing_argument", f"{name} is required.", {"argument": name})
    return value


def command_add_map_text(root: Path, args: argparse.Namespace, input_data: Any) -> dict[str, Any]:
    require_project(root)
    map_name = require_text_value(input_value(args, input_data, "map"), "map")
    text_label = require_text_value(input_value(args, input_data, "text_label"), "text_label")
    text = require_text_value(input_value(args, input_data, "text"), "text")
    try:
        result = add_map_text_label(root, map_name, text_label, text)
    except (OSError, KeyError, ValueError) as error:
        raise CliError("map_event_add_failed", str(error), {"map": map_name, "text_label": text_label}) from error
    return {"ok": True, "map": map_name, "text": result}


def command_add_map_script(root: Path, args: argparse.Namespace, input_data: Any) -> dict[str, Any]:
    require_project(root)
    map_name = require_text_value(input_value(args, input_data, "map"), "map")
    script_label = require_text_value(input_value(args, input_data, "script_label"), "script_label")
    template = str(input_value(args, input_data, "template", "talk") or "talk")
    body = input_value(args, input_data, "body")
    text_label = str(input_value(args, input_data, "text_label", "") or "")
    shop_label = str(input_value(args, input_data, "shop_label", "MartScript") or "MartScript")
    try:
        result = add_map_script_label(root, map_name, script_label, str(body) if body is not None else None, template, text_label, shop_label)
    except (OSError, KeyError, ValueError) as error:
        raise CliError("map_event_add_failed", str(error), {"map": map_name, "script_label": script_label}) from error
    return {"ok": True, "map": map_name, "script": result}


def command_add_map_script_with_text(root: Path, args: argparse.Namespace, input_data: Any) -> dict[str, Any]:
    require_project(root)
    map_name = require_text_value(input_value(args, input_data, "map"), "map")
    script_label = require_text_value(input_value(args, input_data, "script_label"), "script_label")
    text_label = require_text_value(input_value(args, input_data, "text_label"), "text_label")
    text = require_text_value(input_value(args, input_data, "text"), "text")
    template = str(input_value(args, input_data, "template", "talk") or "talk")
    shop_label = str(input_value(args, input_data, "shop_label", "MartScript") or "MartScript")
    try:
        result = add_map_script_with_text(root, map_name, script_label, text_label, text, template, shop_label)
    except (OSError, KeyError, ValueError) as error:
        raise CliError("map_event_add_failed", str(error), {"map": map_name, "script_label": script_label, "text_label": text_label}) from error
    result["ok"] = True
    result["map"] = map_name
    return result


def command_update_map_text(root: Path, args: argparse.Namespace, input_data: Any) -> dict[str, Any]:
    require_project(root)
    map_name = require_text_value(input_value(args, input_data, "map"), "map")
    text_label = require_text_value(input_value(args, input_data, "text_label"), "text_label")
    text_value = input_value(args, input_data, "text")
    if text_value is None:
        raise CliError("missing_argument", "text is required.", {"argument": "text"})
    try:
        result = save_text_label(root, map_name, text_label, str(text_value))
    except (OSError, KeyError, ValueError) as error:
        raise CliError("map_text_update_failed", str(error), {"map": map_name, "text_label": text_label}) from error
    return {"ok": True, "map": map_name, "text": result}


def command_update_map_script(root: Path, args: argparse.Namespace, input_data: Any) -> dict[str, Any]:
    require_project(root)
    map_name = require_text_value(input_value(args, input_data, "map"), "map")
    script_label = require_text_value(input_value(args, input_data, "script_label"), "script_label")
    body = input_value(args, input_data, "body")
    if body is None:
        raise CliError("missing_argument", "body is required.", {"argument": "body"})
    try:
        result = save_script_label(root, map_name, script_label, str(body))
    except (OSError, KeyError, ValueError) as error:
        raise CliError("map_script_update_failed", str(error), {"map": map_name, "script_label": script_label}) from error
    return {"ok": True, "map": map_name, "script": result}


def command_search_map_scripts(root: Path, args: argparse.Namespace, input_data: Any) -> dict[str, Any]:
    require_project(root)
    query = str(input_value(args, input_data, "query", "") or "")
    map_name = input_value(args, input_data, "map")
    include_body = bool(input_value(args, input_data, "include_bodies", False))
    try:
        results = search_map_scripts(root, query, str(map_name) if map_name else None, include_body)
    except (OSError, KeyError, ValueError) as error:
        raise CliError("map_script_search_failed", str(error), {"map": map_name, "query": query}) from error
    return {"ok": True, "count": len(results), "scripts": results}


def command_rename_map_script(root: Path, args: argparse.Namespace, input_data: Any) -> dict[str, Any]:
    require_project(root)
    map_name = require_text_value(input_value(args, input_data, "map"), "map")
    old_label = require_text_value(input_value(args, input_data, "old_label"), "old_label")
    new_label = require_text_value(input_value(args, input_data, "new_label"), "new_label")
    try:
        result = rename_map_script_label(root, map_name, old_label, new_label)
    except (OSError, KeyError, ValueError) as error:
        raise CliError("map_script_rename_failed", str(error), {"map": map_name, "old_label": old_label, "new_label": new_label}) from error
    return {"ok": True, "rename": result}


@dataclass
class FrontierPoolRecord:
    key: str
    file: str
    line: int
    fields: dict[str, str]
    start: int = 0
    end: int = 0
    block: str = ""


def parse_frontier_pool(root: Path) -> list[FrontierPoolRecord]:
    path = root / "src/data/battle_frontier/battle_frontier_mons.h"
    if not path.exists():
        raise CliError("frontier_pool_missing", "Battle Frontier pool file was not found.", {"file": rel(root, path)})
    source = read_utf8(path)
    records: list[FrontierPoolRecord] = []
    matcher = re.compile(r"\[(FRONTIER_MON_[A-Za-z0-9_]+)\]\s*=\s*\{")
    for match in matcher.finditer(source):
        open_pos = source.find("{", match.start(), match.end())
        end = balanced_end(source, open_pos)
        if end is None:
            continue
        block = source[open_pos:end]
        fields = {field: value for field in FRONTIER_MON_FIELDS if (value := raw_field(block, field)) is not None}
        records.append(FrontierPoolRecord(match.group(1), rel(root, path), source.count("\n", 0, match.start()) + 1, fields, match.start(), end, source[match.start():end]))
    return records


def parse_designated_monpool(root: Path, source_rel: str, id_prefix: str | None = None) -> tuple[Path, str, list[FrontierPoolRecord]]:
    path = (root / source_rel).resolve()
    try:
        path.relative_to(root.resolve())
    except ValueError:
        raise CliError("source_outside_root", "Mon pool source must be inside the project root.", {"source": source_rel})
    if not path.exists():
        raise CliError("monpool_source_missing", "Mon pool source file was not found.", {"source": source_rel})
    source = read_utf8(path)
    key_pattern = r"[A-Z][A-Z0-9_]*"
    if id_prefix:
        key_pattern = re.escape(id_prefix) + r"[A-Z0-9_]*"
    matcher = re.compile(rf"\[({key_pattern})\]\s*=\s*\{{")
    records: list[FrontierPoolRecord] = []
    for match in matcher.finditer(source):
        open_pos = source.find("{", match.start(), match.end())
        end = balanced_end(source, open_pos)
        if end is None:
            continue
        block = source[open_pos:end]
        fields = {field: value for field in FRONTIER_MON_FIELDS if (value := raw_field(block, field)) is not None}
        if "species" not in fields:
            continue
        records.append(FrontierPoolRecord(match.group(1), rel(root, path), source.count("\n", 0, match.start()) + 1, fields, match.start(), end, source[match.start():end]))
    return path, source, records


def command_export_frontier_pool(root: Path, _args: argparse.Namespace, _input: dict[str, Any]) -> dict[str, Any]:
    require_project(root)
    records = parse_frontier_pool(root)
    return {
        "ok": True,
        "count": len(records),
        "source": "src/data/battle_frontier/battle_frontier_mons.h",
        "mons": [
            {"id": record.key, "file": record.file, "line": record.line, "fields": record.fields}
            for record in records
        ],
    }


def command_validate_mon(root: Path, _args: argparse.Namespace, input_data: Any) -> dict[str, Any]:
    require_project(root)
    mons = mon_entries_from_input(input_data)
    if not mons:
        raise CliError("missing_mon_input", "No mon data was provided. Use --input-json with a mon object or mons array.", {})
    constants = load_project_constants(root)
    seen_ids: set[str] = set()
    errors: list[dict[str, Any]] = []
    warnings: list[dict[str, Any]] = []
    for mon in mons:
        mon_errors, mon_warnings = validate_mon_record(mon, constants, seen_ids)
        errors.extend(mon_errors)
        warnings.extend(mon_warnings)
    return {
        "ok": not errors,
        "valid": not errors,
        "checked": len(mons),
        "errors": errors,
        "warnings": warnings,
    }


def import_monpool_to_source(root: Path, input_data: Any, source_rel: str, id_prefix: str | None, dry_run: bool) -> dict[str, Any]:
    require_project(root)
    mons = mon_entries_from_input(input_data)
    if not mons:
        raise CliError("missing_monpool_input", "No mon pool records were provided.", {})

    constants = load_project_constants(root)
    seen_ids: set[str] = set()
    validation_errors: list[dict[str, Any]] = []
    validation_warnings: list[dict[str, Any]] = []
    for mon in mons:
        mon_errors, mon_warnings = validate_mon_record(mon, constants, seen_ids)
        validation_errors.extend(mon_errors)
        validation_warnings.extend(mon_warnings)
    if validation_errors:
        raise CliError("monpool_validation_failed", "Mon pool validation failed before writing.", {"errors": validation_errors, "warnings": validation_warnings})

    path, source, records = parse_designated_monpool(root, source_rel, id_prefix)
    by_key = {record.key: record for record in records}
    replacements: list[tuple[int, int, str, str]] = []
    newline = detect_newline(source)
    missing: list[str] = []
    for mon in mons:
        mon_id = str(mon.get("id") or mon.get("key") or "")
        record = by_key.get(mon_id)
        if record is None:
            missing.append(mon_id)
            continue
        replacements.append((record.start, record.end, mon_id, build_initializer_block(mon_id, as_fields(mon), newline, field_order_from_block(record.block))))
    if missing:
        raise CliError("monpool_record_missing", "One or more mon records were not found in the target source.", {"missing": missing[:50], "missing_count": len(missing), "source": rel(root, path)})

    new_source = source
    for start, end, _mon_id, replacement in sorted(replacements, key=lambda item: item[0], reverse=True):
        new_source = new_source[:start] + replacement + new_source[end:]
    joined = find_joined_designators(new_source)
    if joined:
        raise CliError("joined_designator_detected", "Generated source contains multiple .field assignments on one line.", {"source": rel(root, path), "matches": joined[:20]})

    changed = new_source != source
    backup_path: Path | None = None
    if changed and not dry_run:
        backup_path = write_utf8_with_backup(path, new_source)
    return {
        "ok": True,
        "source": rel(root, path),
        "checked": len(mons),
        "changed_records": sum(1 for start, end, _mon_id, replacement in replacements if source[start:end] != replacement),
        "would_write": changed,
        "dry_run": dry_run,
        "backup": rel(root, backup_path) if backup_path else None,
        "warnings": validation_warnings,
    }


def command_import_frontier_pool(root: Path, args: argparse.Namespace, input_data: Any) -> dict[str, Any]:
    source_rel = default_frontier_source()
    if isinstance(input_data, dict) and isinstance(input_data.get("source"), str):
        source_rel = input_data["source"]
    return import_monpool_to_source(root, input_data, source_rel, "FRONTIER_MON_", bool(getattr(args, "dry_run", False)))


def command_import_monpool(root: Path, args: argparse.Namespace, input_data: Any) -> dict[str, Any]:
    source_rel = default_frontier_source()
    if isinstance(input_data, dict) and isinstance(input_data.get("source"), str):
        source_rel = input_data["source"]
    if getattr(args, "source", None):
        source_rel = args.source
    id_prefix = None
    if isinstance(input_data, dict) and isinstance(input_data.get("id_prefix"), str):
        id_prefix = input_data["id_prefix"]
    if getattr(args, "id_prefix", None):
        id_prefix = args.id_prefix
    return import_monpool_to_source(root, input_data, source_rel, id_prefix, bool(getattr(args, "dry_run", False)))


def parse_map_usage(root: Path) -> dict[str, Any]:
    map_path = root / "pokeemerald.map"
    result: dict[str, Any] = {
        "map_file": rel(root, map_path),
        "map_exists": map_path.exists(),
        "ewram_used": None,
        "iwram_used": None,
        "rom_used_estimated": None,
    }
    if not map_path.exists():
        return result
    try:
        source = read_utf8(map_path)
    except UnicodeDecodeError:
        return result
    ewram_used = 0
    iwram_used = 0
    rom_end = 0
    section_re = re.compile(r"^(\.[A-Za-z0-9_.$]+)\s+0x([0-9A-Fa-f]+)\s+0x([0-9A-Fa-f]+)\b")
    for line in source.splitlines():
        match = section_re.match(line)
        if not match:
            continue
        section, address_text, size_text = match.groups()
        address = int(address_text, 16)
        size = int(size_text, 16)
        if section.startswith(".ewram"):
            ewram_used += size
        elif section.startswith(".iwram"):
            iwram_used += size
        if 0x08000000 <= address < 0x0A000000:
            rom_end = max(rom_end, address + size - 0x08000000)
    result["ewram_used"] = ewram_used
    result["iwram_used"] = iwram_used
    result["rom_used_estimated"] = rom_end or None
    return result


def command_build(root: Path, args: argparse.Namespace, _input: Any) -> dict[str, Any]:
    require_project(root)
    jobs = getattr(args, "jobs", None)
    command = ["make"]
    if jobs:
        command.append(f"-j{jobs}")
    if getattr(args, "target", None):
        command.append(args.target)
    if getattr(args, "dry_run", False):
        memory = parse_map_usage(root)
        rom_size = (root / "pokeemerald.gba").stat().st_size if (root / "pokeemerald.gba").exists() else None
        return {
            "ok": True,
            "dry_run": True,
            "command_line": " ".join(command),
            "rom_size": rom_size,
            "ewram_used": memory.get("ewram_used"),
            "rom_used": memory.get("rom_used_estimated") or rom_size,
            "memory": memory,
        }
    start = time.monotonic()
    try:
        completed = subprocess.run(command, cwd=root, capture_output=True, text=True, encoding="utf-8", errors="replace")
    except FileNotFoundError:
        raise CliError("make_not_found", "make was not found on PATH.", {"command": command[0]})
    elapsed = time.monotonic() - start
    rom_path = root / "pokeemerald.gba"
    memory = parse_map_usage(root)
    rom_size = rom_path.stat().st_size if rom_path.exists() else None
    payload = {
        "ok": completed.returncode == 0,
        "command_line": " ".join(command),
        "returncode": completed.returncode,
        "elapsed_seconds": round(elapsed, 3),
        "rom_size": rom_size,
        "ewram_used": memory.get("ewram_used"),
        "rom_used": memory.get("rom_used_estimated") or rom_size,
        "rom": {
            "file": rel(root, rom_path),
            "exists": rom_path.exists(),
            "size": rom_size,
        },
        "memory": memory,
        "stdout_tail": completed.stdout[-4000:],
        "stderr_tail": completed.stderr[-4000:],
    }
    if completed.returncode != 0:
        print(f"Expansion Studio build failed with exit code {completed.returncode}.", file=sys.stderr)
    return payload


def tool_status(name: str, extra_paths: list[Path] | None = None) -> dict[str, Any]:
    found = shutil.which(name)
    if found:
        return {"available": True, "path": found, "source": "PATH"}
    for path in extra_paths or []:
        if path.exists():
            return {"available": True, "path": str(path), "source": "project"}
    return {"available": False, "path": None, "source": None}


def command_doctor(root: Path, _args: argparse.Namespace, _input: Any) -> dict[str, Any]:
    root_exists = root.exists()
    project_status = {
        "src": (root / "src").exists(),
        "include": (root / "include").exists(),
        "data": (root / "data").exists(),
        "graphics": (root / "graphics").exists(),
        "makefile": (root / "Makefile").exists(),
    }
    japanese_fonts = sorted(rel(root, path) for path in (root / "graphics" / "fonts").glob("japanese*.png")) if (root / "graphics" / "fonts").exists() else []
    frontier_files = {
        "frontier_pool": (root / default_frontier_source()).exists(),
        "frontier_constants": (root / "include/constants/battle_frontier_mons.h").exists(),
        "frontier_trainer_mons": (root / "src/data/battle_frontier/battle_frontier_trainer_mons.h").exists(),
    }
    tools = {
        "git": tool_status("git"),
        "make": tool_status("make"),
        "python": tool_status("python"),
        "arm-none-eabi-gcc": tool_status("arm-none-eabi-gcc"),
        "agbcc": {
            "available": (root / "agbcc").exists() and ((root / "agbcc" / "build.sh").exists() or (root / "agbcc" / "install.sh").exists()),
            "path": rel(root, root / "agbcc") if (root / "agbcc").exists() else None,
            "source": "project",
        },
        "poryscript": tool_status("poryscript", [root / "poryscript.exe", root / "tools" / "poryscript" / "poryscript.exe", root / "tools" / "poryscript" / "poryscript"]),
        "porymap": tool_status("porymap"),
    }
    required_project_ok = root_exists and all(project_status.values()) and (root / "charmap.txt").exists() and bool(japanese_fonts) and all(frontier_files.values())
    missing_tools = [name for name, status in tools.items() if not status["available"]]
    healthy = required_project_ok and not missing_tools
    return {
        "ok": required_project_ok,
        "healthy": healthy,
        "root": root.name,
        "project": project_status,
        "project_valid": root_exists and project_status["src"] and project_status["include"] and project_status["makefile"],
        "tools": tools,
        "missing_tools": missing_tools,
        "assets": {
            "charmap": (root / "charmap.txt").exists(),
            "fonts_dir": (root / "graphics" / "fonts").exists(),
            "japanese_fonts": japanese_fonts,
        },
        "frontier": frontier_files,
        "porymap_project": (root / "porymap.project.cfg").exists(),
    }


def load_input_json(path_text: str | None) -> Any:
    if not path_text:
        return {}
    try:
        if path_text == "-":
            return json.load(sys.stdin)
        with Path(path_text).open("r", encoding="utf-8") as handle:
            return json.load(handle)
    except (OSError, json.JSONDecodeError) as error:
        raise CliError("invalid_input_json", "Could not read JSON input.", {"reason": str(error)})


def resolve_root(args: argparse.Namespace, input_data: Any) -> Path:
    root_from_input = input_data.get("root") if isinstance(input_data, dict) else None
    root = root_from_input or getattr(args, "root", None) or "."
    return Path(root).resolve()


def emit_json(payload: dict[str, Any], pretty: bool) -> None:
    print(json.dumps(payload, ensure_ascii=False, indent=2 if pretty else None))


def build_parser() -> argparse.ArgumentParser:
    common = argparse.ArgumentParser(add_help=False)
    common.add_argument("--root", default=argparse.SUPPRESS, help="pokeemerald-expansion repository root")
    common.add_argument("--input-json", default=argparse.SUPPRESS, help="Read command options from a JSON file, or '-' for stdin")
    common.add_argument("--pretty", action="store_true", default=argparse.SUPPRESS, help="Pretty-print JSON output")
    parser = argparse.ArgumentParser(prog="python -m expansionstudio", description="Expansion Studio CLI")
    parser.add_argument("--root", help="pokeemerald-expansion repository root")
    parser.add_argument("--input-json", help="Read command options from a JSON file, or '-' for stdin")
    parser.add_argument("--pretty", action="store_true", help="Pretty-print JSON output")
    subparsers = parser.add_subparsers(dest="command", required=True)

    subparsers.add_parser("validate-project", parents=[common], help="Validate project structure")
    scan_texts = subparsers.add_parser("scan-texts", parents=[common], help="List translation target strings")
    scan_texts.add_argument("--query", help="Optional case-insensitive text/file/symbol filter")
    update_text = subparsers.add_parser("update-text", parents=[common], help="Update one translation target string")
    update_text.add_argument("--file", help="Project-relative source file")
    update_text.add_argument("--line", type=int, help="Line number from scan-texts")
    update_text.add_argument("--symbol", help="Optional symbol from scan-texts")
    update_text.add_argument("--macro", choices=["COMPOUND_STRING", "_"], help="Optional macro filter")
    update_text.add_argument("--text", help="New text; use --input-json for multi-line text")
    png_to_pal = subparsers.add_parser("png-to-jasc-pal", parents=[common], help="Generate a JASC-PAL .pal file from a PNG")
    png_to_pal.add_argument("--png", help="Project-relative input PNG")
    png_to_pal.add_argument("--output", help="Project-relative output .pal")
    png_to_pal.add_argument("--colors", type=int, choices=[16, 224, 256], default=16, help="Palette size")
    subparsers.add_parser("scan-frontier", parents=[common], help="Detect Battle Frontier related data files")
    scan_map_events = subparsers.add_parser("scan-map-events", parents=[common], help="List maps or inspect one map's event links")
    scan_map_events.add_argument("--map", help="Map folder name under data/maps")
    scan_map_events.add_argument("--include-bodies", action="store_true", help="Include script/text bodies in JSON output")
    search_map_scripts_parser = subparsers.add_parser("search-map-scripts", parents=[common], help="Search map script labels and bodies")
    search_map_scripts_parser.add_argument("--query", help="Search text")
    search_map_scripts_parser.add_argument("--map", help="Optional map folder name under data/maps")
    search_map_scripts_parser.add_argument("--include-bodies", action="store_true", help="Include script bodies in JSON output")
    add_map_text = subparsers.add_parser("add-map-text", parents=[common], help="Add a text label to a map script/text file")
    add_map_text.add_argument("--map", help="Map folder name under data/maps")
    add_map_text.add_argument("--text-label", dest="text_label", help="New text label")
    add_map_text.add_argument("--text", help="Text body; use --input-json for multi-line text")
    add_map_script = subparsers.add_parser("add-map-script", parents=[common], help="Add a script label to scripts.inc")
    add_map_script.add_argument("--map", help="Map folder name under data/maps")
    add_map_script.add_argument("--script-label", dest="script_label", help="New script label")
    add_map_script.add_argument("--text-label", dest="text_label", help="Text label referenced by template")
    add_map_script.add_argument("--template", choices=["msgbox", "talk", "yes_no", "shop", "bp_shop"], default="talk", help="Script template")
    add_map_script.add_argument("--body", help="Raw script body; overrides template")
    add_map_script.add_argument("--shop-label", dest="shop_label", default="MartScript", help="Compatibility placeholder for custom templates")
    add_map_both = subparsers.add_parser("add-map-script-with-text", parents=[common], help="Add a script label and referenced text label")
    add_map_both.add_argument("--map", help="Map folder name under data/maps")
    add_map_both.add_argument("--script-label", dest="script_label", help="New script label")
    add_map_both.add_argument("--text-label", dest="text_label", help="New text label")
    add_map_both.add_argument("--text", help="Text body; use --input-json for multi-line text")
    add_map_both.add_argument("--template", choices=["msgbox", "talk", "yes_no", "shop", "bp_shop"], default="talk", help="Script template")
    add_map_both.add_argument("--shop-label", dest="shop_label", default="MartScript", help="Compatibility placeholder for custom templates")
    update_map_text = subparsers.add_parser("update-map-text", parents=[common], help="Update one map text label body")
    update_map_text.add_argument("--map", help="Map folder name under data/maps")
    update_map_text.add_argument("--text-label", dest="text_label", help="Existing text label")
    update_map_text.add_argument("--text", help="New text; use --input-json for multi-line text")
    update_map_script = subparsers.add_parser("update-map-script", parents=[common], help="Update one map script label body")
    update_map_script.add_argument("--map", help="Map folder name under data/maps")
    update_map_script.add_argument("--script-label", dest="script_label", help="Existing script label")
    update_map_script.add_argument("--body", help="New script body; use --input-json for multi-line body")
    rename_map_script = subparsers.add_parser("rename-map-script", parents=[common], help="Rename one map script label and local references")
    rename_map_script.add_argument("--map", help="Map folder name under data/maps")
    rename_map_script.add_argument("--old-label", dest="old_label", help="Existing script label")
    rename_map_script.add_argument("--new-label", dest="new_label", help="New script label")
    subparsers.add_parser("export-frontier-pool", parents=[common], help="Export Battle Frontier mon pool as JSON")
    import_frontier = subparsers.add_parser("import-frontier-pool", parents=[common], help="Import Battle Frontier mon pool JSON")
    import_frontier.add_argument("--dry-run", action="store_true", help="Validate and preview without writing")
    import_monpool = subparsers.add_parser("import-monpool", parents=[common], help="Import a designated-initializer mon pool JSON")
    import_monpool.add_argument("--source", help="Project-relative source file to update")
    import_monpool.add_argument("--id-prefix", help="Optional designated initializer ID prefix")
    import_monpool.add_argument("--dry-run", action="store_true", help="Validate and preview without writing")
    subparsers.add_parser("validate-mon", parents=[common], help="Validate one mon or a mon pool JSON")
    build = subparsers.add_parser("build", parents=[common], help="Run make and return build status as JSON")
    build.add_argument("--jobs", type=int, help="make parallel job count")
    build.add_argument("--target", help="Optional make target")
    build.add_argument("--dry-run", action="store_true", help="Return the command and current artifacts without running make")
    subparsers.add_parser("doctor", parents=[common], help="Diagnose project and toolchain availability")
    return parser


COMMANDS = {
    "validate-project": command_validate_project,
    "scan-texts": command_scan_texts,
    "update-text": command_update_text,
    "png-to-jasc-pal": command_png_to_jasc_pal,
    "scan-frontier": command_scan_frontier,
    "scan-map-events": command_scan_map_events,
    "search-map-scripts": command_search_map_scripts,
    "add-map-text": command_add_map_text,
    "add-map-script": command_add_map_script,
    "add-map-script-with-text": command_add_map_script_with_text,
    "update-map-text": command_update_map_text,
    "update-map-script": command_update_map_script,
    "rename-map-script": command_rename_map_script,
    "export-frontier-pool": command_export_frontier_pool,
    "import-frontier-pool": command_import_frontier_pool,
    "import-monpool": command_import_monpool,
    "validate-mon": command_validate_mon,
    "build": command_build,
    "doctor": command_doctor,
}


def main(argv: list[str] | None = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)
    try:
        input_data = load_input_json(getattr(args, "input_json", None))
        root = resolve_root(args, input_data)
        handler = COMMANDS[args.command]
        payload = handler(root, args, input_data)
        payload.setdefault("ok", True)
        payload.setdefault("command", args.command)
        if payload.get("ok") is False:
            print(f"Expansion Studio CLI command failed: {args.command}", file=sys.stderr)
        emit_json(payload, getattr(args, "pretty", False))
        return 0 if payload.get("ok", False) else 1
    except CliError as error:
        print(f"Expansion Studio CLI error [{error.code}]: {error.message}", file=sys.stderr)
        emit_json({"ok": False, "command": getattr(args, "command", None), "error": {"code": error.code, "message": error.message, "details": error.details}}, getattr(args, "pretty", False))
        return 1
    except Exception as error:  # pragma: no cover - defensive CLI boundary
        print(f"Expansion Studio CLI error [internal_error]: {error}", file=sys.stderr)
        emit_json({"ok": False, "command": getattr(args, "command", None), "error": {"code": "internal_error", "message": str(error), "details": {}}}, getattr(args, "pretty", False))
        return 1
