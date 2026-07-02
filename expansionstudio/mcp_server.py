from __future__ import annotations

import argparse
import json
import os
import subprocess
import sys
import traceback
from dataclasses import dataclass
from pathlib import Path, PureWindowsPath
from typing import Any


SERVER_NAME = "expansionstudio"
SERVER_VERSION = "0.1.0"
PROTOCOL_VERSION = "2024-11-05"
PACKAGE_PARENT = Path(__file__).resolve().parent.parent


class McpToolError(Exception):
    def __init__(self, code: str, message: str, details: dict[str, Any] | None = None) -> None:
        super().__init__(message)
        self.code = code
        self.message = message
        self.details = details or {}

    def to_json(self) -> dict[str, Any]:
        return {"ok": False, "error": {"code": self.code, "message": self.message, "details": self.details}}


@dataclass(frozen=True)
class ToolSpec:
    name: str
    cli_command: str
    description: str
    schema: dict[str, Any]
    write: bool = False
    dry_run_supported: bool = False
    default_dry_run: bool = False


def object_schema(properties: dict[str, Any], required: list[str] | None = None) -> dict[str, Any]:
    return {
        "type": "object",
        "properties": properties,
        "required": required or [],
        "additionalProperties": True,
    }


COMMON_CONFIRM = {
    "confirm": {
        "type": "boolean",
        "description": "Must be true for write operations. The CLI still creates backups before source writes.",
    }
}


TOOLS: dict[str, ToolSpec] = {
    "validate_project": ToolSpec(
        "validate_project", "validate-project", "Validate the configured pokeemerald-expansion root.",
        object_schema({}),
    ),
    "doctor": ToolSpec(
        "doctor", "doctor", "Check local tools and important project files.",
        object_schema({}),
    ),
    "scan_texts": ToolSpec(
        "scan_texts", "scan-texts", "List translation target strings from src/include.",
        object_schema({"query": {"type": "string"}}),
    ),
    "png_to_jasc_pal": ToolSpec(
        "png_to_jasc_pal", "png-to-jasc-pal", "Generate a JASC-PAL .pal file from a PNG.",
        object_schema({
            "png": {"type": "string", "description": "Project-relative input PNG."},
            "output": {"type": "string", "description": "Project-relative output .pal."},
            "colors": {"type": "integer", "enum": [16, 224, 256]},
            **COMMON_CONFIRM,
        }, ["png", "output", "colors", "confirm"]),
        write=True,
    ),
    "update_text": ToolSpec(
        "update_text", "update-text", "Update one translation target string selected by file and line or symbol.",
        object_schema({
            "file": {"type": "string", "description": "Project-relative source path from scan_texts."},
            "line": {"type": "integer"},
            "symbol": {"type": "string"},
            "macro": {"type": "string", "enum": ["COMPOUND_STRING", "_"]},
            "text": {"type": "string"},
            **COMMON_CONFIRM,
        }, ["file", "text", "confirm"]),
        write=True,
    ),
    "scan_map_events": ToolSpec(
        "scan_map_events", "scan-map-events", "List maps or inspect one map's events/scripts/text links.",
        object_schema({"map": {"type": "string"}, "include_bodies": {"type": "boolean"}}),
    ),
    "search_map_scripts": ToolSpec(
        "search_map_scripts", "search-map-scripts", "Search event script labels and script bodies.",
        object_schema({"query": {"type": "string"}, "map": {"type": "string"}, "include_bodies": {"type": "boolean"}}),
    ),
    "update_map_text": ToolSpec(
        "update_map_text", "update-map-text", "Update one map text label body.",
        object_schema({"map": {"type": "string"}, "text_label": {"type": "string"}, "text": {"type": "string"}, **COMMON_CONFIRM}, ["map", "text_label", "text", "confirm"]),
        write=True,
    ),
    "update_map_script": ToolSpec(
        "update_map_script", "update-map-script", "Update one map script label body.",
        object_schema({"map": {"type": "string"}, "script_label": {"type": "string"}, "body": {"type": "string"}, **COMMON_CONFIRM}, ["map", "script_label", "body", "confirm"]),
        write=True,
    ),
    "add_map_text": ToolSpec(
        "add_map_text", "add-map-text", "Add a new text label to a map.",
        object_schema({"map": {"type": "string"}, "text_label": {"type": "string"}, "text": {"type": "string"}, **COMMON_CONFIRM}, ["map", "text_label", "text", "confirm"]),
        write=True,
    ),
    "add_map_script": ToolSpec(
        "add_map_script", "add-map-script", "Add a new script label to scripts.inc.",
        object_schema({
            "map": {"type": "string"},
            "script_label": {"type": "string"},
            "text_label": {"type": "string"},
            "template": {"type": "string", "enum": ["msgbox", "talk", "yes_no", "shop", "bp_shop"]},
            "body": {"type": "string"},
            "shop_label": {"type": "string"},
            **COMMON_CONFIRM,
        }, ["map", "script_label", "confirm"]),
        write=True,
    ),
    "add_map_script_with_text": ToolSpec(
        "add_map_script_with_text", "add-map-script-with-text", "Add a script label and referenced text label to a map.",
        object_schema({
            "map": {"type": "string"},
            "script_label": {"type": "string"},
            "text_label": {"type": "string"},
            "text": {"type": "string"},
            "template": {"type": "string", "enum": ["msgbox", "talk", "yes_no", "shop", "bp_shop"]},
            "shop_label": {"type": "string"},
            **COMMON_CONFIRM,
        }, ["map", "script_label", "text_label", "text", "confirm"]),
        write=True,
    ),
    "rename_map_script": ToolSpec(
        "rename_map_script", "rename-map-script", "Rename a map script label and local map/script references.",
        object_schema({"map": {"type": "string"}, "old_label": {"type": "string"}, "new_label": {"type": "string"}, **COMMON_CONFIRM}, ["map", "old_label", "new_label", "confirm"]),
        write=True,
    ),
    "scan_frontier": ToolSpec(
        "scan_frontier", "scan-frontier", "Detect Battle Frontier related source files.",
        object_schema({}),
    ),
    "export_frontier_pool": ToolSpec(
        "export_frontier_pool", "export-frontier-pool", "Export the Battle Frontier Pokemon pool as JSON.",
        object_schema({}),
    ),
    "import_frontier_pool": ToolSpec(
        "import_frontier_pool", "import-frontier-pool", "Import Battle Frontier Pokemon pool JSON. Defaults to dry-run.",
        object_schema({
            "mons": {"type": "array"},
            "source": {"type": "string"},
            "dry_run": {"type": "boolean", "description": "Defaults to true in MCP. Set false with confirm=true to write."},
            **COMMON_CONFIRM,
        }, ["mons"]),
        write=True,
        dry_run_supported=True,
        default_dry_run=True,
    ),
    "import_monpool": ToolSpec(
        "import_monpool", "import-monpool", "Import a designated-initializer mon pool JSON. Defaults to dry-run.",
        object_schema({
            "mons": {"type": "array"},
            "source": {"type": "string"},
            "id_prefix": {"type": "string"},
            "dry_run": {"type": "boolean", "description": "Defaults to true in MCP. Set false with confirm=true to write."},
            **COMMON_CONFIRM,
        }, ["mons", "source"]),
        write=True,
        dry_run_supported=True,
        default_dry_run=True,
    ),
    "validate_mon": ToolSpec(
        "validate_mon", "validate-mon", "Validate one Pokemon record or a mons array.",
        object_schema({"mon": {"type": "object"}, "mons": {"type": "array"}}),
    ),
    "build": ToolSpec(
        "build", "build", "Run or preview make. Defaults to dry-run in MCP.",
        object_schema({
            "jobs": {"type": "integer"},
            "target": {"type": "string"},
            "dry_run": {"type": "boolean", "description": "Defaults to true in MCP. Set false with confirm=true to run make."},
            **COMMON_CONFIRM,
        }),
        write=True,
        dry_run_supported=True,
        default_dry_run=True,
    ),
}


def resolve_project_root(raw_root: str | None) -> Path:
    root = Path(raw_root or os.environ.get("EXPANSIONSTUDIO_ROOT") or ".").resolve()
    if not root.exists():
        raise McpToolError("root_not_found", "ExpansionStudio project root does not exist.", {"root": str(root)})
    return root


def is_relative_project_path(value: str) -> bool:
    if not value or "\x00" in value:
        return False
    normalized = value.replace("\\", "/")
    if normalized.startswith("/") or normalized.startswith("//"):
        return False
    win_path = PureWindowsPath(value)
    if win_path.is_absolute() or win_path.drive:
        return False
    return all(part not in {"", ".", ".."} for part in normalized.split("/"))


def ensure_inside_root(root: Path, value: str, field: str) -> None:
    if not is_relative_project_path(value):
        raise McpToolError("invalid_path", f"{field} must be a project-relative path without traversal.", {field: value})
    target = (root / value).resolve()
    try:
        target.relative_to(root.resolve())
    except ValueError as error:
        raise McpToolError("path_outside_root", f"{field} resolves outside the allowed root.", {field: value}) from error


def ensure_map_name(value: Any) -> None:
    if value is None:
        return
    text = str(value)
    if "/" in text or "\\" in text or text in {"", ".", ".."} or "\x00" in text:
        raise McpToolError("invalid_map_name", "map must be a data/maps folder name, not a path.", {"map": text})


def sanitize_arguments(root: Path, spec: ToolSpec, arguments: dict[str, Any]) -> dict[str, Any]:
    args = dict(arguments)
    forbidden = sorted(set(args) & {"root", "input_json", "pretty"})
    if forbidden:
        raise McpToolError("forbidden_argument", "MCP tools cannot override server-level CLI options.", {"arguments": forbidden})
    if spec.default_dry_run and "dry_run" not in args:
        args["dry_run"] = True
    if spec.write:
        dry_run = bool(args.get("dry_run", False))
        if spec.dry_run_supported and dry_run:
            pass
        elif args.get("confirm") is not True:
            raise McpToolError(
                "confirmation_required",
                "This tool writes files or runs a write-capable command. Set confirm=true, or keep dry_run=true when supported.",
                {"tool": spec.name, "dry_run_supported": spec.dry_run_supported},
            )
    for field in ("file", "source", "png", "output"):
        value = args.get(field)
        if isinstance(value, str):
            ensure_inside_root(root, value, field)
    if "map" in args:
        ensure_map_name(args.get("map"))
    args.pop("confirm", None)
    return args


def append_arg(command: list[str], flag: str, value: Any) -> None:
    if value is None:
        return
    if isinstance(value, bool):
        if value:
            command.append(flag)
        return
    command.extend([flag, str(value)])


def build_cli_command(python: str, root: Path, spec: ToolSpec, args: dict[str, Any]) -> list[str]:
    command = [python, "-m", "expansionstudio", spec.cli_command, "--root", str(root), "--input-json", "-"]
    if spec.cli_command == "scan-texts":
        append_arg(command, "--query", args.get("query"))
    elif spec.cli_command == "scan-map-events":
        append_arg(command, "--map", args.get("map"))
        append_arg(command, "--include-bodies", args.get("include_bodies"))
    elif spec.cli_command == "search-map-scripts":
        append_arg(command, "--query", args.get("query"))
        append_arg(command, "--map", args.get("map"))
        append_arg(command, "--include-bodies", args.get("include_bodies"))
    elif spec.cli_command == "png-to-jasc-pal":
        append_arg(command, "--png", args.get("png"))
        append_arg(command, "--output", args.get("output"))
        append_arg(command, "--colors", args.get("colors"))
    elif spec.cli_command in {"import-frontier-pool", "import-monpool", "build"}:
        append_arg(command, "--dry-run", args.get("dry_run"))
    if spec.cli_command == "import-monpool":
        append_arg(command, "--source", args.get("source"))
        append_arg(command, "--id-prefix", args.get("id_prefix"))
    elif spec.cli_command == "build":
        append_arg(command, "--jobs", args.get("jobs"))
        append_arg(command, "--target", args.get("target"))
    return command


def run_cli(root: Path, python: str, spec: ToolSpec, arguments: dict[str, Any], timeout: int) -> dict[str, Any]:
    args = sanitize_arguments(root, spec, arguments)
    command = build_cli_command(python, root, spec, args)
    env = os.environ.copy()
    env["PYTHONPATH"] = str(PACKAGE_PARENT) + os.pathsep + env.get("PYTHONPATH", "")
    try:
        completed = subprocess.run(
            command,
            input=json.dumps(args, ensure_ascii=False),
            cwd=PACKAGE_PARENT,
            env=env,
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
            timeout=timeout,
        )
    except subprocess.TimeoutExpired as error:
        raise McpToolError("cli_timeout", "ExpansionStudio CLI timed out.", {"tool": spec.name, "timeout_seconds": timeout}) from error
    except OSError as error:
        raise McpToolError("cli_spawn_failed", "Could not start ExpansionStudio CLI.", {"tool": spec.name, "reason": str(error)}) from error

    stdout = completed.stdout.strip()
    try:
        payload = json.loads(stdout) if stdout else {}
    except json.JSONDecodeError as error:
        raise McpToolError("invalid_cli_json", "ExpansionStudio CLI did not return valid JSON.", {
            "tool": spec.name,
            "returncode": completed.returncode,
            "stdout_tail": completed.stdout[-2000:],
            "stderr_tail": completed.stderr[-2000:],
        }) from error
    if completed.stderr.strip():
        payload.setdefault("stderr", completed.stderr[-4000:])
    payload.setdefault("returncode", completed.returncode)
    if completed.returncode != 0:
        payload.setdefault("ok", False)
    return payload


def mcp_tool_list() -> list[dict[str, Any]]:
    return [
        {
            "name": spec.name,
            "description": spec.description,
            "inputSchema": spec.schema,
        }
        for spec in TOOLS.values()
    ]


def read_message() -> dict[str, Any] | None:
    stream = sys.stdin.buffer
    first = stream.readline()
    if not first:
        return None
    if first.lstrip().startswith(b"{"):
        return json.loads(first.decode("utf-8"))

    headers: dict[str, str] = {}
    line = first
    while line and line not in {b"\r\n", b"\n"}:
        name, _, value = line.decode("ascii", errors="replace").partition(":")
        headers[name.strip().lower()] = value.strip()
        line = stream.readline()
    length_text = headers.get("content-length")
    if not length_text:
        raise McpToolError("bad_message", "Missing Content-Length header.", {})
    body = stream.read(int(length_text))
    return json.loads(body.decode("utf-8"))


def write_message(message: dict[str, Any]) -> None:
    body = json.dumps(message, ensure_ascii=False, separators=(",", ":")).encode("utf-8")
    sys.stdout.buffer.write(f"Content-Length: {len(body)}\r\n\r\n".encode("ascii"))
    sys.stdout.buffer.write(body)
    sys.stdout.buffer.flush()


def success_response(request_id: Any, result: dict[str, Any]) -> dict[str, Any]:
    return {"jsonrpc": "2.0", "id": request_id, "result": result}


def error_response(request_id: Any, code: int, message: str, data: Any = None) -> dict[str, Any]:
    error: dict[str, Any] = {"code": code, "message": message}
    if data is not None:
        error["data"] = data
    return {"jsonrpc": "2.0", "id": request_id, "error": error}


def tool_result(payload: dict[str, Any]) -> dict[str, Any]:
    text = json.dumps(payload, ensure_ascii=False, indent=2)
    return {
        "content": [{"type": "text", "text": text}],
        "structuredContent": payload,
        "isError": not bool(payload.get("ok", True)),
    }


def handle_request(message: dict[str, Any], root: Path, python: str, timeout: int) -> dict[str, Any] | None:
    request_id = message.get("id")
    method = message.get("method")
    if method == "initialize":
        return success_response(request_id, {
            "protocolVersion": PROTOCOL_VERSION,
            "capabilities": {"tools": {}},
            "serverInfo": {"name": SERVER_NAME, "version": SERVER_VERSION},
        })
    if method == "notifications/initialized":
        return None
    if method == "ping":
        return success_response(request_id, {})
    if method == "tools/list":
        return success_response(request_id, {"tools": mcp_tool_list()})
    if method == "tools/call":
        params = message.get("params") or {}
        tool_name = params.get("name")
        arguments = params.get("arguments") or {}
        if tool_name not in TOOLS:
            return success_response(request_id, tool_result(McpToolError("unknown_tool", "Unknown ExpansionStudio MCP tool.", {"tool": tool_name}).to_json()))
        if not isinstance(arguments, dict):
            return success_response(request_id, tool_result(McpToolError("invalid_arguments", "Tool arguments must be an object.", {"tool": tool_name}).to_json()))
        try:
            payload = run_cli(root, python, TOOLS[tool_name], arguments, timeout)
        except McpToolError as error:
            payload = error.to_json()
        return success_response(request_id, tool_result(payload))
    return error_response(request_id, -32601, f"Method not found: {method}")


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="ExpansionStudio stdio MCP server")
    parser.add_argument("--root", help="pokeemerald-expansion project root. Defaults to EXPANSIONSTUDIO_ROOT or current directory.")
    parser.add_argument("--python", default=sys.executable, help="Python executable used to run python -m expansionstudio.")
    parser.add_argument("--timeout", type=int, default=120, help="CLI subprocess timeout in seconds.")
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        root = resolve_project_root(args.root)
        while True:
            message = read_message()
            if message is None:
                break
            try:
                response = handle_request(message, root, args.python, args.timeout)
            except Exception as error:  # Defensive: MCP server should return a JSON-RPC error, not crash.
                print(traceback.format_exc(), file=sys.stderr)
                response = error_response(message.get("id"), -32603, "Internal ExpansionStudio MCP server error.", {"reason": str(error)})
            if response is not None:
                write_message(response)
        return 0
    except McpToolError as error:
        print(json.dumps(error.to_json(), ensure_ascii=False), file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
