#!/usr/bin/env python3
"""Pre-import checks for Japanese text updates.

This script catches common merge/import mistakes before the full ROM build:
unbalanced .string quotes, conflict markers, stray quotes after "$",
possibly unterminated .string directives, and Japanese glyphs missing from
charmap.txt.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


SCAN_DIRS = ("data", "src", "include")
TEXT_SUFFIXES = {".c", ".h", ".inc"}
CONFLICT_MARKERS = ("<<<<<<<", "=======", ">>>>>>>")

STRING_DIRECTIVE_RE = re.compile(r"^[ \t]*\.string\b")
STRING_LITERAL_RE = re.compile(r'"(?:\\.|[^"\\])*"', re.DOTALL)
CHARMAP_CHAR_RE = re.compile(r"^'((?:\\.|[^'\\])*)'\s*=")
JAPANESE_CHAR_RE = re.compile(
    r"[\u2190-\u21ff\u25a0-\u27bf\u3000-\u303f\u3040-\u30ff\uff00-\uffef\u3400-\u9fff]"
)


def iter_source_files(root: Path):
    for dirname in SCAN_DIRS:
        base = root / dirname
        if not base.exists():
            continue
        for path in base.rglob("*"):
            if path.is_file() and path.suffix in TEXT_SUFFIXES and not path.name.endswith(".bak"):
                yield path


def display_path(root: Path, path: Path) -> str:
    return path.relative_to(root).as_posix()


def count_unescaped_quotes(line: str) -> int:
    count = 0
    escaped = False
    for ch in line:
        if escaped:
            escaped = False
        elif ch == "\\":
            escaped = True
        elif ch == '"':
            count += 1
    return count


def decode_charmap_char(raw: str) -> str:
    if raw == r"\'":
        return "'"
    if raw == r"\\":
        return "\\"
    return raw


def load_charmap_chars(root: Path) -> set[str]:
    chars: set[str] = set()
    charmap = root / "charmap.txt"
    if not charmap.exists():
        return chars

    for line in charmap.read_text(encoding="utf-8", errors="replace").splitlines():
        match = CHARMAP_CHAR_RE.match(line.strip())
        if not match:
            continue
        decoded = decode_charmap_char(match.group(1))
        if len(decoded) == 1:
            chars.add(decoded)
    return chars


def line_for_offset(text: str, offset: int) -> int:
    return text.count("\n", 0, offset) + 1


def get_string_literals_after_directive(text: str, start: int) -> tuple[list[re.Match[str]], bool]:
    """Return literal matches for one .string directive and whether it closed.

    A .string is treated as closed once a quoted literal contains "$", or once
    parsing reaches a line whose last quoted literal closes normally.
    """
    matches: list[re.Match[str]] = []
    pos = start
    while pos < len(text):
        newline = text.find("\n", pos)
        line_end = len(text) if newline == -1 else newline
        line = text[pos:line_end]

        if not matches and not STRING_DIRECTIVE_RE.match(line):
            return matches, False

        for match in STRING_LITERAL_RE.finditer(line):
            # Convert match offsets from line-relative to file-relative.
            file_match = _OffsetMatch(match, pos)
            matches.append(file_match)  # type: ignore[arg-type]
            if "$" in match.group(0):
                return matches, True

        if count_unescaped_quotes(line) % 2 == 1:
            return matches, False

        if matches:
            # Most script imports use one literal per .string line. If the line
            # closed cleanly, do not accidentally consume the next directive.
            return matches, True

        if newline == -1:
            break
        pos = newline + 1
    return matches, False


class _OffsetMatch:
    def __init__(self, match: re.Match[str], base: int):
        self._match = match
        self._base = base

    def start(self) -> int:
        return self._base + self._match.start()

    def group(self, index: int = 0) -> str:
        return self._match.group(index)


def literal_text(literal: str) -> str:
    return literal[1:-1]


def check_file(root: Path, path: Path, charmap_chars: set[str]) -> list[str]:
    rel = display_path(root, path)
    text = path.read_text(encoding="utf-8", errors="replace")
    errors: list[str] = []

    lines = text.splitlines()
    for line_no, line in enumerate(lines, start=1):
        stripped = line.lstrip()

        if any(stripped.startswith(marker) for marker in CONFLICT_MARKERS):
            errors.append(f"{rel}:{line_no}: error: conflict marker remains")

        if STRING_DIRECTIVE_RE.match(line) and count_unescaped_quotes(line) % 2 == 1:
            errors.append(f"{rel}:{line_no}: error: .string has an odd number of quotes")

        if re.search(r'\$\s*""', line) or re.search(r'\$"\s*"', line):
            errors.append(f'{rel}:{line_no}: error: suspicious extra quote after "$"')

    for match in re.finditer(r"^[ \t]*\.string\b", text, re.MULTILINE):
        literals, closed = get_string_literals_after_directive(text, match.start())
        if not literals:
            errors.append(f"{rel}:{line_for_offset(text, match.start())}: error: .string has no closed string literal")
        elif not closed:
            errors.append(f"{rel}:{line_for_offset(text, match.start())}: error: .string may be unterminated")

    for match in STRING_LITERAL_RE.finditer(text):
        raw = literal_text(match.group(0))
        for ch in JAPANESE_CHAR_RE.findall(raw):
            if ch not in charmap_chars:
                errors.append(
                    f"{rel}:{line_for_offset(text, match.start())}: error: Japanese character {ch!r} is not defined in charmap.txt"
                )
                break

    return errors


def main() -> int:
    parser = argparse.ArgumentParser(description="Check Japanese text import safety.")
    parser.add_argument(
        "--root",
        default=Path(__file__).resolve().parents[1],
        type=Path,
        help="repository root (default: parent of tools/)",
    )
    args = parser.parse_args()

    root = args.root.resolve()
    charmap_chars = load_charmap_chars(root)
    errors: list[str] = []

    for path in iter_source_files(root):
        errors.extend(check_file(root, path, charmap_chars))

    if errors:
        print("Japanese text check failed:", file=sys.stderr)
        for error in errors:
            print(error, file=sys.stderr)
        return 1

    print("Japanese text check passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
