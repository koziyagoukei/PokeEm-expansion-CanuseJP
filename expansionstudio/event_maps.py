from __future__ import annotations

import json
import re
import shutil
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any


LABEL_RE = re.compile(r"(?m)^([A-Za-z_][A-Za-z0-9_]*)(?:::|:)\s*(?:@.*)?$")
STRING_RE = re.compile(r"(?m)^[ \t]*\.string\b")
TEXT_COMMAND_RE = re.compile(r"(?m)^[ \t]*(?:msgbox|message|yesnobox)\s+([A-Za-z_][A-Za-z0-9_]*)\b")
SCRIPT_FIELD_RE = re.compile(r'("script"\s*:\s*")([A-Za-z_][A-Za-z0-9_]*?)(")')
SCRIPT_EVENT_KINDS = {"object_events", "coord_events", "bg_events"}
SCRIPT_TEMPLATES = {
    "msgbox": "msgbox {TEXT_LABEL}, MSGBOX_DEFAULT\nend\n",
    "talk": "lock\nfaceplayer\nmsgbox {TEXT_LABEL}, MSGBOX_DEFAULT\nrelease\nend\n",
    "yes_no": "lock\nfaceplayer\nmsgbox {TEXT_LABEL}, MSGBOX_YESNO\ncompare VAR_RESULT, YES\ngoto_if_eq {SCRIPT_LABEL}_Yes\nrelease\nend\n\n{SCRIPT_LABEL}_Yes:\nrelease\nend\n",
    "shop": "lock\nfaceplayer\nmsgbox {TEXT_LABEL}, MSGBOX_DEFAULT\npokemart {SCRIPT_LABEL}_Shop\nrelease\nend\n\n{SCRIPT_LABEL}_Shop:\n\t.2byte ITEM_POTION\n\tpokemartlistend\n",
    "bp_shop": "lock\nfaceplayer\nspecial ShowBattlePointsWindow\nmsgbox {TEXT_LABEL}, MSGBOX_DEFAULT\nspecial CloseBattlePointsWindow\nrelease\nend\n",
}


@dataclass
class MapSummary:
    name: str
    map_id: str
    file_name: str
    has_scripts: bool
    has_text: bool
    object_count: int = 0
    warp_count: int = 0
    coord_count: int = 0
    bg_count: int = 0


@dataclass
class StringPart:
    line_start: int
    line_end: int
    content: str


@dataclass
class MapScriptBlock:
    label: str
    file: str
    path: Path
    line: int
    start: int
    body_start: int
    end: int
    body: str
    text_labels: list[str] = field(default_factory=list)


@dataclass
class MapTextBlock:
    label: str
    file: str
    path: Path
    line: int
    start: int
    end: int
    string_start: int
    string_end: int
    parts: list[StringPart]
    has_terminator: bool

    @property
    def text(self) -> str:
        values: list[str] = []
        for index, part in enumerate(self.parts):
            value = part.content
            if index == len(self.parts) - 1 and self.has_terminator and value.endswith("$"):
                value = value[:-1]
            values.append(value)
        return "\n".join(values)


@dataclass
class MapEventRecord:
    kind: str
    index: int
    name: str
    script_label: str
    x: Any = ""
    y: Any = ""
    data: dict[str, Any] = field(default_factory=dict)
    script_found: bool = False
    text_labels: list[str] = field(default_factory=list)
    missing_text_labels: list[str] = field(default_factory=list)


@dataclass
class MapEventBundle:
    map_name: str
    map_id: str
    map_dir: Path
    map_json: Path
    scripts_path: Path
    text_path: Path | None
    object_events: list[MapEventRecord]
    warp_events: list[MapEventRecord]
    coord_events: list[MapEventRecord]
    bg_events: list[MapEventRecord]
    scripts: dict[str, MapScriptBlock]
    texts: dict[str, MapTextBlock]
    warnings: list[str] = field(default_factory=list)


def rel(root: Path, path: Path) -> str:
    try:
        return path.relative_to(root).as_posix()
    except ValueError:
        return path.as_posix()


def read_text(path: Path) -> str:
    with path.open("r", encoding="utf-8", newline="") as handle:
        return handle.read()


def write_text_with_backup(path: Path, source: str) -> Path:
    backup = path.with_name(path.name + ".bak")
    shutil.copy2(path, backup)
    with path.open("w", encoding="utf-8", newline="") as handle:
        handle.write(source)
    return backup


def ensure_parent(path: Path) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)


def detect_newline(source: str) -> str:
    return "\r\n" if "\r\n" in source else "\n"


def map_root(root: Path) -> Path:
    return root / "data" / "maps"


def map_dir(root: Path, map_name: str) -> Path:
    return map_root(root) / map_name


def list_event_maps(root: Path) -> list[MapSummary]:
    base = map_root(root)
    if not base.exists():
        return []
    maps: list[MapSummary] = []
    for folder in sorted((path for path in base.iterdir() if path.is_dir()), key=lambda path: path.name.casefold()):
        map_json = folder / "map.json"
        if not map_json.exists():
            continue
        map_id = ""
        counts = {"object_events": 0, "warp_events": 0, "coord_events": 0, "bg_events": 0}
        try:
            data = json.loads(read_text(map_json))
            map_id = str(data.get("id", ""))
            for key in counts:
                value = data.get(key, [])
                counts[key] = len(value) if isinstance(value, list) else 0
        except (OSError, json.JSONDecodeError):
            pass
        maps.append(MapSummary(
            name=folder.name,
            map_id=map_id,
            file_name=rel(root, map_json),
            has_scripts=(folder / "scripts.inc").exists(),
            has_text=(folder / "text.inc").exists(),
            object_count=counts["object_events"],
            warp_count=counts["warp_events"],
            coord_count=counts["coord_events"],
            bg_count=counts["bg_events"],
        ))
    return maps


def line_end(source: str, pos: int) -> int:
    end = source.find("\n", pos)
    return len(source) if end < 0 else end + 1


def parse_string_content(source: str, quote_pos: int) -> tuple[int, str] | None:
    if quote_pos >= len(source) or source[quote_pos] != '"':
        return None
    pos = quote_pos + 1
    chars: list[str] = []
    while pos < len(source):
        char = source[pos]
        if char == "\\" and pos + 1 < len(source):
            chars.append(source[pos:pos + 2])
            pos += 2
            continue
        if char == '"':
            return pos + 1, "".join(chars)
        chars.append(char)
        pos += 1
    return None


def label_body_start(source: str, label_match: re.Match[str]) -> int:
    end = source.find("\n", label_match.end())
    return len(source) if end < 0 else end + 1


def parse_labeled_blocks(root: Path, path: Path) -> tuple[dict[str, MapScriptBlock], dict[str, MapTextBlock]]:
    if not path.exists():
        return {}, {}
    source = read_text(path)
    labels = list(LABEL_RE.finditer(source))
    scripts: dict[str, MapScriptBlock] = {}
    texts: dict[str, MapTextBlock] = {}
    for index, match in enumerate(labels):
        label = match.group(1)
        start = match.start()
        body_start = label_body_start(source, match)
        end = labels[index + 1].start() if index + 1 < len(labels) else len(source)
        body = source[body_start:end]
        line = source.count("\n", 0, start) + 1
        text_labels = list(dict.fromkeys(TEXT_COMMAND_RE.findall(body)))
        parts: list[StringPart] = []
        for string_match in STRING_RE.finditer(source, body_start, end):
            line_start = source.rfind("\n", 0, string_match.start()) + 1
            line_stop = line_end(source, string_match.start())
            quote_pos = source.find('"', string_match.end(), line_stop)
            if quote_pos < 0:
                continue
            parsed = parse_string_content(source, quote_pos)
            if not parsed:
                continue
            _quote_end, content = parsed
            parts.append(StringPart(line_start, line_stop, content))
        scripts[label] = MapScriptBlock(label, rel(root, path), path, line, start, body_start, end, body, text_labels)
        if parts:
            has_terminator = parts[-1].content.endswith("$")
            texts[label] = MapTextBlock(
                label=label,
                file=rel(root, path),
                path=path,
                line=line,
                start=start,
                end=end,
                string_start=parts[0].line_start,
                string_end=parts[-1].line_end,
                parts=parts,
                has_terminator=has_terminator,
            )
    return scripts, texts


def load_map_json(path: Path) -> dict[str, Any]:
    with path.open("r", encoding="utf-8") as handle:
        return json.load(handle)


def event_name(kind: str, index: int, data: dict[str, Any]) -> str:
    if kind == "object_events":
        return str(data.get("local_id") or data.get("graphics_id") or f"object {index}")
    if kind == "warp_events":
        return f"warp {index} -> {data.get('dest_map', '')}"
    return str(data.get("type") or f"{kind} {index}")


def build_event_records(kind: str, values: Any, scripts: dict[str, MapScriptBlock], texts: dict[str, MapTextBlock]) -> list[MapEventRecord]:
    if not isinstance(values, list):
        return []
    records: list[MapEventRecord] = []
    for index, raw in enumerate(values):
        if not isinstance(raw, dict):
            continue
        script_label = str(raw.get("script", ""))
        script = scripts.get(script_label)
        text_labels = script.text_labels if script else []
        records.append(MapEventRecord(
            kind=kind,
            index=index,
            name=event_name(kind, index, raw),
            script_label=script_label,
            x=raw.get("x", ""),
            y=raw.get("y", ""),
            data=raw,
            script_found=script is not None,
            text_labels=text_labels,
            missing_text_labels=[label for label in text_labels if label not in texts],
        ))
    return records


def load_map_event_bundle(root: Path, map_name: str) -> MapEventBundle:
    folder = map_dir(root, map_name)
    map_json = folder / "map.json"
    scripts_path = folder / "scripts.inc"
    text_path = folder / "text.inc"
    if not map_json.exists():
        raise FileNotFoundError(f"map.json not found: {rel(root, map_json)}")
    data = load_map_json(map_json)
    scripts: dict[str, MapScriptBlock] = {}
    texts: dict[str, MapTextBlock] = {}
    warnings: list[str] = []
    if scripts_path.exists():
        parsed_scripts, parsed_texts = parse_labeled_blocks(root, scripts_path)
        scripts.update(parsed_scripts)
        texts.update(parsed_texts)
    else:
        warnings.append(f"scripts.inc missing: {rel(root, scripts_path)}")
    actual_text_path: Path | None = None
    if text_path.exists():
        actual_text_path = text_path
        _text_scripts, parsed_texts = parse_labeled_blocks(root, text_path)
        texts.update(parsed_texts)

    object_events = build_event_records("object_events", data.get("object_events", []), scripts, texts)
    warp_events = build_event_records("warp_events", data.get("warp_events", []), scripts, texts)
    coord_events = build_event_records("coord_events", data.get("coord_events", []), scripts, texts)
    bg_events = build_event_records("bg_events", data.get("bg_events", []), scripts, texts)
    return MapEventBundle(
        map_name=str(data.get("name") or map_name),
        map_id=str(data.get("id") or ""),
        map_dir=folder,
        map_json=map_json,
        scripts_path=scripts_path,
        text_path=actual_text_path,
        object_events=object_events,
        warp_events=warp_events,
        coord_events=coord_events,
        bg_events=bg_events,
        scripts=scripts,
        texts=texts,
        warnings=warnings,
    )


def duplicate_labels(source: str) -> list[str]:
    seen: set[str] = set()
    duplicates: list[str] = []
    for match in LABEL_RE.finditer(source):
        label = match.group(1)
        if label in seen and label not in duplicates:
            duplicates.append(label)
        seen.add(label)
    return duplicates


def validate_label_name(label: str, kind: str = "label") -> str:
    label = label.strip()
    if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", label):
        raise ValueError(f"Invalid {kind}: {label!r}")
    return label


def all_labels(bundle: MapEventBundle) -> set[str]:
    return set(bundle.scripts) | set(bundle.texts)


def assert_label_available(bundle: MapEventBundle, label: str, kind: str) -> None:
    if label in all_labels(bundle):
        raise ValueError(f"{kind} already exists: {label}")


def file_append_separator(source: str, newline: str) -> str:
    if not source:
        return ""
    if source.endswith(newline + newline):
        return ""
    if source.endswith(newline):
        return newline
    return newline + newline


def text_lines_to_source(lines: list[str], indent: str, has_terminator: bool, newline: str) -> str:
    if not lines:
        lines = [""]
    output: list[str] = []
    for index, line in enumerate(lines):
        value = line.replace('"', '\\"')
        if index == len(lines) - 1 and has_terminator and not value.endswith("$"):
            value += "$"
        output.append(f'{indent}.string "{value}"')
    return newline.join(output) + newline


def new_text_block(label: str, text: str, newline: str, indent: str = "\t") -> str:
    body = text_lines_to_source(text.splitlines(), indent, True, newline)
    return f"{label}:{newline}" + body


def new_script_block(label: str, body: str, newline: str) -> str:
    body = body.replace("\r\n", "\n").replace("\r", "\n")
    if body and not body.endswith("\n"):
        body += "\n"
    if newline != "\n":
        body = body.replace("\n", newline)
    return f"{label}:{newline}" + body


def render_script_template(template: str, script_label: str, text_label: str, shop_label: str = "MartScript") -> str:
    key = template.strip() or "talk"
    if key not in SCRIPT_TEMPLATES:
        raise ValueError(f"Unknown script template: {template}")
    return SCRIPT_TEMPLATES[key].format(SCRIPT_LABEL=script_label, TEXT_LABEL=text_label, SHOP_LABEL=shop_label)


def add_map_text_label(root: Path, map_name: str, text_label: str, text: str) -> dict[str, Any]:
    text_label = validate_label_name(text_label, "text label")
    bundle = load_map_event_bundle(root, map_name)
    assert_label_available(bundle, text_label, "Text label")
    target = bundle.text_path or bundle.scripts_path
    if not target.exists():
        raise FileNotFoundError(f"Target text file not found: {rel(root, target)}")
    ensure_parent(target)
    source = read_text(target)
    newline = detect_newline(source)
    block = new_text_block(text_label, text, newline)
    new_source = source + file_append_separator(source, newline) + block
    duplicates = duplicate_labels(new_source)
    if duplicates:
        raise ValueError("Duplicate labels after text add: " + ", ".join(duplicates[:20]))
    backup = write_text_with_backup(target, new_source)
    return {"file": rel(root, target), "backup": rel(root, backup), "label": text_label}


def add_map_script_label(root: Path, map_name: str, script_label: str, body: str | None = None, template: str = "talk", text_label: str = "", shop_label: str = "MartScript") -> dict[str, Any]:
    script_label = validate_label_name(script_label, "script label")
    if text_label:
        text_label = validate_label_name(text_label, "text label")
    bundle = load_map_event_bundle(root, map_name)
    assert_label_available(bundle, script_label, "Script label")
    target = bundle.scripts_path
    if not target.exists():
        raise FileNotFoundError(f"scripts.inc not found: {rel(root, target)}")
    ensure_parent(target)
    source = read_text(target)
    newline = detect_newline(source)
    script_body = body if body is not None else render_script_template(template, script_label, text_label or f"{script_label}_Text", shop_label)
    missing = undefined_text_labels(script_body, bundle.texts)
    block = new_script_block(script_label, script_body, newline)
    new_source = source + file_append_separator(source, newline) + block
    duplicates = duplicate_labels(new_source)
    if duplicates:
        raise ValueError("Duplicate labels after script add: " + ", ".join(duplicates[:20]))
    backup = write_text_with_backup(target, new_source)
    return {"file": rel(root, target), "backup": rel(root, backup), "label": script_label, "warnings": missing}


def add_map_script_with_text(root: Path, map_name: str, script_label: str, text_label: str, text: str, template: str = "talk", shop_label: str = "MartScript") -> dict[str, Any]:
    script_label = validate_label_name(script_label, "script label")
    text_label = validate_label_name(text_label, "text label")
    bundle = load_map_event_bundle(root, map_name)
    assert_label_available(bundle, script_label, "Script label")
    assert_label_available(bundle, text_label, "Text label")
    scripts_path = bundle.scripts_path
    text_path = bundle.text_path or bundle.scripts_path
    if not scripts_path.exists():
        raise FileNotFoundError(f"scripts.inc not found: {rel(root, scripts_path)}")
    if not text_path.exists():
        raise FileNotFoundError(f"Target text file not found: {rel(root, text_path)}")
    ensure_parent(scripts_path)
    ensure_parent(text_path)
    script_source = read_text(scripts_path)
    text_source = read_text(text_path)
    script_newline = detect_newline(script_source)
    text_newline = detect_newline(text_source)
    script_body = render_script_template(template, script_label, text_label, shop_label)
    script_block = new_script_block(script_label, script_body, script_newline)
    text_block = new_text_block(text_label, text, text_newline)

    if scripts_path == text_path:
        new_source = script_source + file_append_separator(script_source, script_newline) + script_block + script_newline + text_block
        duplicates = duplicate_labels(new_source)
        if duplicates:
            raise ValueError("Duplicate labels after script/text add: " + ", ".join(duplicates[:20]))
        backup = write_text_with_backup(scripts_path, new_source)
        return {
            "script": {"file": rel(root, scripts_path), "backup": rel(root, backup), "label": script_label},
            "text": {"file": rel(root, text_path), "backup": rel(root, backup), "label": text_label},
            "warnings": [],
        }

    new_script_source = script_source + file_append_separator(script_source, script_newline) + script_block
    new_text_source = text_source + file_append_separator(text_source, text_newline) + text_block
    script_duplicates = duplicate_labels(new_script_source)
    text_duplicates = duplicate_labels(new_text_source)
    if script_duplicates or text_duplicates:
        raise ValueError("Duplicate labels after script/text add: " + ", ".join([*script_duplicates, *text_duplicates][:20]))
    script_backup = write_text_with_backup(scripts_path, new_script_source)
    text_backup = write_text_with_backup(text_path, new_text_source)
    return {
        "script": {"file": rel(root, scripts_path), "backup": rel(root, script_backup), "label": script_label},
        "text": {"file": rel(root, text_path), "backup": rel(root, text_backup), "label": text_label},
        "warnings": [],
    }


def save_text_label(root: Path, map_name: str, text_label: str, new_text: str) -> dict[str, Any]:
    bundle = load_map_event_bundle(root, map_name)
    target = bundle.texts.get(text_label)
    if target is None:
        raise KeyError(f"Text label not found: {text_label}")
    source = read_text(target.path)
    current_scripts, current_texts = parse_labeled_blocks(root, target.path)
    current = current_texts.get(text_label)
    if current is None:
        raise KeyError(f"Text label not found on disk: {text_label}")
    newline = detect_newline(source)
    first_line = source[current.string_start:line_end(source, current.string_start)]
    indent_match = re.match(r"^[ \t]*", first_line)
    indent = indent_match.group(0) if indent_match else "\t"
    replacement = text_lines_to_source(new_text.splitlines(), indent, current.has_terminator, newline)
    new_source = source[:current.string_start] + replacement + source[current.string_end:]
    duplicates = duplicate_labels(new_source)
    if duplicates:
        raise ValueError("Duplicate labels after text save: " + ", ".join(duplicates[:20]))
    backup = write_text_with_backup(target.path, new_source)
    return {"file": rel(root, target.path), "backup": rel(root, backup), "label": text_label}


def undefined_text_labels(script_body: str, texts: dict[str, MapTextBlock]) -> list[str]:
    labels = list(dict.fromkeys(TEXT_COMMAND_RE.findall(script_body)))
    return [label for label in labels if label not in texts]


def save_script_label(root: Path, map_name: str, script_label: str, new_body: str) -> dict[str, Any]:
    bundle = load_map_event_bundle(root, map_name)
    target = bundle.scripts.get(script_label)
    if target is None:
        raise KeyError(f"Script label not found: {script_label}")
    source = read_text(target.path)
    current_scripts, _current_texts = parse_labeled_blocks(root, target.path)
    current = current_scripts.get(script_label)
    if current is None:
        raise KeyError(f"Script label not found on disk: {script_label}")
    newline = detect_newline(source)
    body = new_body.replace("\r\n", "\n").replace("\r", "\n")
    if body and not body.endswith("\n"):
        body += "\n"
    if newline != "\n":
        body = body.replace("\n", newline)
    new_source = source[:current.body_start] + body + source[current.end:]
    duplicates = duplicate_labels(new_source)
    if duplicates:
        raise ValueError("Duplicate labels after script save: " + ", ".join(duplicates[:20]))
    _new_scripts, new_texts = parse_labeled_blocks(root, target.path)
    warnings = undefined_text_labels(body, {**bundle.texts, **new_texts})
    backup = write_text_with_backup(target.path, new_source)
    return {"file": rel(root, target.path), "backup": rel(root, backup), "label": script_label, "warnings": warnings}


def script_event_ref_count(bundle: MapEventBundle, script_label: str) -> int:
    events = [*bundle.object_events, *bundle.coord_events, *bundle.bg_events]
    return sum(1 for event in events if event.script_label == script_label)


def search_map_scripts(root: Path, query: str = "", map_name: str | None = None, include_body: bool = False) -> list[dict[str, Any]]:
    query_text = (query or "").casefold()
    map_names = [map_name] if map_name else [entry.name for entry in list_event_maps(root)]
    results: list[dict[str, Any]] = []
    for name in map_names:
        if not name:
            continue
        try:
            bundle = load_map_event_bundle(root, name)
        except (OSError, json.JSONDecodeError):
            continue
        for script in bundle.scripts.values():
            if script.label in bundle.texts:
                continue
            preview = " ".join(line.strip() for line in script.body.splitlines() if line.strip())[:160]
            haystack = "\n".join((
                bundle.map_dir.name,
                bundle.map_name,
                bundle.map_id,
                script.label,
                script.file,
                " ".join(script.text_labels),
                script.body,
            )).casefold()
            if query_text and query_text not in haystack:
                continue
            entry: dict[str, Any] = {
                "map": bundle.map_dir.name,
                "map_name": bundle.map_name,
                "map_id": bundle.map_id,
                "label": script.label,
                "file": script.file,
                "line": script.line,
                "text_labels": script.text_labels,
                "event_refs": script_event_ref_count(bundle, script.label),
                "preview": preview,
            }
            if include_body:
                entry["body"] = script.body
            results.append(entry)
    return sorted(results, key=lambda item: (str(item["map"]).casefold(), str(item["file"]).casefold(), int(item["line"]), str(item["label"]).casefold()))


def replace_script_tokens_outside_strings(source: str, old_label: str, new_label: str) -> tuple[str, int]:
    token_re = re.compile(rf"\b{re.escape(old_label)}\b")
    changed = 0
    output: list[str] = []
    for line in source.splitlines(keepends=True):
        if STRING_RE.match(line):
            output.append(line)
            continue
        replaced, count = token_re.subn(new_label, line)
        changed += count
        output.append(replaced)
    return "".join(output), changed


def replace_map_json_script_refs(source: str, old_label: str, new_label: str) -> tuple[str, int]:
    def repl(match: re.Match[str]) -> str:
        if match.group(2) != old_label:
            return match.group(0)
        return f"{match.group(1)}{new_label}{match.group(3)}"

    return SCRIPT_FIELD_RE.subn(repl, source)


def rename_map_script_label(root: Path, map_name: str, old_label: str, new_label: str) -> dict[str, Any]:
    old_label = validate_label_name(old_label, "old script label")
    new_label = validate_label_name(new_label, "new script label")
    if old_label == new_label:
        raise ValueError("New script label is the same as the old label.")
    bundle = load_map_event_bundle(root, map_name)
    if old_label in bundle.texts:
        raise ValueError(f"Label is a text label, not a script label: {old_label}")
    if old_label not in bundle.scripts:
        raise KeyError(f"Script label not found: {old_label}")
    assert_label_available(bundle, new_label, "Script label")

    scripts_source = read_text(bundle.scripts_path)
    new_scripts_source, script_ref_count = replace_script_tokens_outside_strings(scripts_source, old_label, new_label)
    if new_scripts_source == scripts_source:
        raise ValueError(f"Script label was not replaced: {old_label}")
    duplicates = duplicate_labels(new_scripts_source)
    if duplicates:
        raise ValueError("Duplicate labels after script rename: " + ", ".join(duplicates[:20]))

    changed_files: list[dict[str, Any]] = []
    scripts_backup = write_text_with_backup(bundle.scripts_path, new_scripts_source)
    changed_files.append({"file": rel(root, bundle.scripts_path), "backup": rel(root, scripts_backup), "replacements": script_ref_count})

    if bundle.map_json.exists():
        map_source = read_text(bundle.map_json)
        new_map_source, map_ref_count = replace_map_json_script_refs(map_source, old_label, new_label)
        if new_map_source != map_source:
            map_backup = write_text_with_backup(bundle.map_json, new_map_source)
            changed_files.append({"file": rel(root, bundle.map_json), "backup": rel(root, map_backup), "replacements": map_ref_count})

    return {
        "map": bundle.map_dir.name,
        "old_label": old_label,
        "new_label": new_label,
        "changed_files": changed_files,
    }


def bundle_to_dict(root: Path, bundle: MapEventBundle, include_bodies: bool = False) -> dict[str, Any]:
    def event_dict(event: MapEventRecord) -> dict[str, Any]:
        return {
            "kind": event.kind,
            "index": event.index,
            "name": event.name,
            "script_label": event.script_label,
            "script_found": event.script_found,
            "text_labels": event.text_labels,
            "missing_text_labels": event.missing_text_labels,
            "x": event.x,
            "y": event.y,
        }

    payload: dict[str, Any] = {
        "map_name": bundle.map_name,
        "map_id": bundle.map_id,
        "map_dir": rel(root, bundle.map_dir),
        "map_json": rel(root, bundle.map_json),
        "scripts": rel(root, bundle.scripts_path),
        "text": rel(root, bundle.text_path) if bundle.text_path else None,
        "counts": {
            "object_events": len(bundle.object_events),
            "warp_events": len(bundle.warp_events),
            "coord_events": len(bundle.coord_events),
            "bg_events": len(bundle.bg_events),
            "script_labels": len(bundle.scripts),
            "text_labels": len(bundle.texts),
        },
        "object_events": [event_dict(event) for event in bundle.object_events],
        "warp_events": [event_dict(event) for event in bundle.warp_events],
        "coord_events": [event_dict(event) for event in bundle.coord_events],
        "bg_events": [event_dict(event) for event in bundle.bg_events],
        "warnings": bundle.warnings,
    }
    if include_bodies:
        payload["script_bodies"] = {label: block.body for label, block in bundle.scripts.items()}
        payload["text_bodies"] = {label: block.text for label, block in bundle.texts.items()}
    return payload
