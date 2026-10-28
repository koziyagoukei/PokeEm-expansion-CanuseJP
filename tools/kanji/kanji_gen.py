#!/usr/bin/env python3
"""Detect the kanji used by display strings and generate their font data.

Run by `make` before any string is encoded:

1. Scan the display strings that preproc encodes: arguments of _(), __(),
   J_(), COMPOUND_STRING() and of every macro that forwards to them in C
   sources and headers, and .string/.braille directives in assembly.
2. Give every kanji that is not in the ledger (charmap_kanji.txt) the next
   free code and append it. Ledger entries are never removed, reordered or
   reused, so codes do not depend on scan order or on later text changes.
3. Write the glyphs of the kanji that are currently used, taken from the
   committed KH Dot Kodenmachou 12 atlas, and the code -> glyph slot table.

Only the Python standard library is needed. The ledger is source; everything
under build/generated/kanji is a build product.
"""
from __future__ import annotations

import argparse
from contextlib import contextmanager
import os
import re
import struct
import sys
import tempfile
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CHARMAP = "charmap.txt"
LEDGER = "charmap_kanji.txt"
ATLAS_PNG = "graphics/fonts/kanji/koden12_atlas.png"
ATLAS_TXT = "graphics/fonts/kanji/koden12_atlas.txt"
OUT_DIR = "build/generated/kanji"
GLYPHS_BIN = "kanji_glyphs.fwjpnfont"
SLOTS_BIN = "kanji_slots.bin"
REPORT = "kanji_report.txt"
BUILD_CHARMAP = "charmap.txt"

# Must match include/constants/characters.h.
EXT_CTRL_CODE_BEGIN = 0xFC
FIRST_BANK_CODE = 0x1F  # EXT_CTRL_CODE_KANJI_GLYPH
BANK_COUNT = 9          # FC 1F .. FC 27
GLYPHS_PER_BANK = 0xF6  # payload 01..F6; F7..FF are text control bytes
MAX_KANJI = BANK_COUNT * GLYPHS_PER_BANK
LEGACY_GLYPHS = "攻撃防御特性技力速命中状態異常回復対戦相手物理変化"

CELL = 12
SCAN_DIRS = ("src", "include", "data")
TEST_SCAN_DIRS = ("test",)
C_SUFFIXES = {".c", ".h"}
ASM_SUFFIXES = {".inc", ".s"}
BASE_WRAPPERS = {"_", "__", "J_", "J__", "COMPOUND_STRING", "J_COMPOUND_STRING"}

KANJI_RANGES = ((0x3005, 0x3007), (0x3400, 0x4DBF), (0x4E00, 0x9FFF), (0xF900, 0xFAFF),
                (0x20000, 0x2EBEF), (0x2F800, 0x2FA1F), (0x30000, 0x323AF))


class KanjiError(Exception):
    pass


def is_kanji(char: str) -> bool:
    code = ord(char)
    return any(low <= code <= high for low, high in KANJI_RANGES)


# --- codes -----------------------------------------------------------------

def encode(number: int) -> bytes:
    """Ledger number (1-based) -> FC <bank code> <payload>."""
    if not 1 <= number <= MAX_KANJI:
        raise KanjiError(f"kanji number {number} is outside 1..{MAX_KANJI}")
    bank, payload = divmod(number - 1, GLYPHS_PER_BANK)
    return bytes((EXT_CTRL_CODE_BEGIN, FIRST_BANK_CODE + bank, payload + 1))


def decode(sequence: bytes) -> int:
    if (len(sequence) != 3 or sequence[0] != EXT_CTRL_CODE_BEGIN
            or not FIRST_BANK_CODE <= sequence[1] < FIRST_BANK_CODE + BANK_COUNT
            or not 1 <= sequence[2] <= GLYPHS_PER_BANK):
        raise KanjiError("not a kanji code: " + sequence.hex(" ").upper())
    return (sequence[1] - FIRST_BANK_CODE) * GLYPHS_PER_BANK + sequence[2]


# --- PNG ---------------------------------------------------------------------

def read_png(path: Path):
    """Decode a non-interlaced grayscale or palette PNG with 1/2/4/8-bit samples."""
    data = path.read_bytes()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise KanjiError(f"{path}: not a PNG")
    pos, idat, header = 8, bytearray(), None
    while pos < len(data):
        if pos + 12 > len(data):
            raise KanjiError(f"{path}: truncated PNG chunk")
        length, kind = struct.unpack(">I4s", data[pos:pos + 8])
        body = data[pos + 8:pos + 8 + length]
        if pos + 12 + length > len(data) or zlib.crc32(kind + body) != struct.unpack(">I", data[pos + 8 + length:pos + 12 + length])[0]:
            raise KanjiError(f"{path}: invalid PNG chunk/CRC")
        pos += 12 + length
        if kind == b"IHDR":
            header = struct.unpack(">IIBBBBB", body)
        elif kind == b"IDAT":
            idat += body
        elif kind == b"IEND":
            break
    if header is None:
        raise KanjiError(f"{path}: missing PNG header")
    width, height, depth, color, compression, filtering, interlace = header
    if not width or not height or color not in (0, 3) or compression or filtering or interlace or depth not in (1, 2, 4, 8):
        raise KanjiError(f"{path}: unsupported PNG format")
    raw = zlib.decompress(bytes(idat))
    stride = (width * depth + 7) // 8
    if len(raw) != height * (stride + 1):
        raise KanjiError(f"{path}: invalid PNG pixel data length")
    rows, prev, pos = [], bytearray(stride), 0
    for _ in range(height):
        kind, line = raw[pos], bytearray(raw[pos + 1:pos + 1 + stride])
        if kind > 4:
            raise KanjiError(f"{path}: invalid PNG filter {kind}")
        pos += 1 + stride
        for i in range(stride):
            left = line[i - 1] if i else 0
            up, upleft = prev[i], prev[i - 1] if i else 0
            if kind == 1:
                line[i] = (line[i] + left) & 0xFF
            elif kind == 2:
                line[i] = (line[i] + up) & 0xFF
            elif kind == 3:
                line[i] = (line[i] + (left + up) // 2) & 0xFF
            elif kind == 4:
                p = left + up - upleft
                pa, pb, pc = abs(p - left), abs(p - up), abs(p - upleft)
                pred = left if pa <= pb and pa <= pc else up if pb <= pc else upleft
                line[i] = (line[i] + pred) & 0xFF
        per_byte, mask = 8 // depth, (1 << depth) - 1
        rows.append([(line[x // per_byte] >> (8 - depth * (x % per_byte + 1))) & mask for x in range(width)])
        prev = line
    return width, height, rows


class Atlas:
    """All kanji of the committed font atlas, 12x12 glyph bodies, 64 per row."""

    def __init__(self, root: Path):
        self.chars = "".join((root / ATLAS_TXT).read_text(encoding="utf-8").split())
        self.index = {char: i for i, char in enumerate(self.chars)}
        width, height, self.rows = read_png(root / ATLAS_PNG)
        if (not self.chars or len(self.index) != len(self.chars) or width % CELL or height % CELL
                or len(self.chars) > (width // CELL) * (height // CELL)):
            raise KanjiError("kanji atlas dimensions or character index are invalid")
        self.columns = width // CELL

    def body(self, char: str):
        index = self.index[char]
        x0, y0 = (index % self.columns) * CELL, (index // self.columns) * CELL
        body = [[self.rows[y0 + y][x0 + x] != 0 for x in range(CELL)] for y in range(CELL)]
        if not any(any(row) for row in body):
            raise KanjiError(f"kanji atlas has an empty glyph for {char!r}")
        return body


def glyph_cell(body):
    """16x16 cell: 1 = body, 2 = shadow one pixel right and down, clipped to 12x12."""
    cell = [[0] * 16 for _ in range(16)]
    for y in range(1, CELL):
        for x in range(1, CELL):
            if body[y - 1][x - 1]:
                cell[y][x] = 2
    for y in range(CELL):
        for x in range(CELL):
            if body[y][x]:
                cell[y][x] = 1
    return cell


def fwjpnfont(cells) -> bytes:
    """Same layout gbagfx writes for .fwjpnfont: 8 glyphs share a 512-byte block."""
    count = (len(cells) + 7) // 8 * 8
    out = bytearray(count * 64)
    for slot, cell in enumerate(cells):
        base = 512 * (slot >> 3) + 32 * (slot & 7)
        for tile in range(4):
            offset = base + 256 * (tile >> 1) + 16 * (tile & 1)
            for i in range(8):
                row = cell[(tile >> 1) * 8 + i][(tile & 1) * 8:(tile & 1) * 8 + 8]
                packed = 0
                for k, value in enumerate(row):
                    packed |= value << (14 - 2 * k)
                out[offset + 2 * i] = packed & 0xFF
                out[offset + 2 * i + 1] = packed >> 8
    return bytes(out)


# --- charmap and ledger --------------------------------------------------------

CHARMAP_LINE = re.compile(r"^'((?:\\.|[^'\\])+)'\s*=\s*((?:[0-9A-Fa-f]{2}\s*)+)$")
CONSTANT_LINE = re.compile(r"^(\w+)\s*=\s*((?:[0-9A-Fa-f]{2}\s*)+)$")


def strip_charmap_comment(line: str) -> str:
    in_char = False
    for i, ch in enumerate(line):
        if ch == "'" and (i == 0 or line[i - 1] != "\\"):
            in_char = not in_char
        elif ch == "@" and not in_char:
            return line[:i]
    return line


def load_base_charmap(root: Path):
    """Characters and constants of charmap.txt itself (not its includes)."""
    chars, constants = {}, {}
    for number, line in enumerate((root / CHARMAP).read_text(encoding="utf-8").splitlines(), 1):
        line = strip_charmap_comment(line).strip()
        match = CHARMAP_LINE.match(line)
        if match:
            chars[match[1].replace("\\'", "'").replace("\\\\", "\\")] = (bytes.fromhex(match[2]), number)
            continue
        match = CONSTANT_LINE.match(line)
        if match:
            constants[match[1]] = (bytes.fromhex(match[2]), number)
    return chars, constants


LEDGER_HEADER = """\
@ Kanji ID ledger (漢字ID台帳), included by charmap.txt.
@
@ `make` appends kanji found in display strings via tools/kanji/kanji_gen.py.
@ Codes are permanent: never edit, reorder or delete these lines, and never
@ reuse a code, even if a kanji is no longer used. Unused glyphs are omitted;
@ their 2-byte slot entries remain to preserve IDs.
@ Code = FC <bank 1F..27> <01..F6>. See docs/kanji.md.
"""


def read_ledger(path: Path):
    entries = []
    if not path.exists():
        raise KanjiError(f"{path}: missing persistent ID ledger; restore it from version control")
    for number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        stripped = strip_charmap_comment(line).strip()
        if not stripped:
            continue
        match = CHARMAP_LINE.match(stripped)
        if not match or len(match[1]) != 1:
            raise KanjiError(f"{LEDGER}:{number}: expected 'X' = FC xx xx")
        entries.append((match[1], decode(bytes.fromhex(match[2])), number))
    return entries


def ledger_text(entries) -> str:
    lines = [f"'{char}' = {encode(n).hex(' ').upper()}" for char, n, *_ in entries]
    return LEDGER_HEADER + "\n" + "\n".join(lines) + "\n"


def validate_ledger(entries, base_chars, base_constants):
    errors, seen_chars, seen_numbers = [], {}, {}
    for char, n, line in entries:
        if not is_kanji(char):
            errors.append(f"{LEDGER}:{line}: '{char}' is not a kanji")
        if char in seen_chars:
            errors.append(f"{LEDGER}:{line}: '{char}' already has a code (line {seen_chars[char]})")
        if n in seen_numbers:
            errors.append(f"{LEDGER}:{line}: code collides with line {seen_numbers[n]}")
        if char in base_chars:
            errors.append(f"{LEDGER}:{line}: '{char}' is also defined in {CHARMAP}:{base_chars[char][1]}")
        seen_chars.setdefault(char, line)
        seen_numbers.setdefault(n, line)
    kanji_codes = range(FIRST_BANK_CODE, FIRST_BANK_CODE + BANK_COUNT)
    by_char = {char: n for char, n, _ in entries}
    for n, char in enumerate(LEGACY_GLYPHS, 1):
        if by_char.get(char) != n:
            errors.append(f"{LEDGER}: legacy glyph {char!r} must retain ID {n}")
    for name, (sequence, line) in list(base_chars.items()) + list(base_constants.items()):
        if len(sequence) >= 2 and sequence[0] == EXT_CTRL_CODE_BEGIN and sequence[1] in kanji_codes:
            errors.append(f"{CHARMAP}:{line}: {name} uses a kanji code (FC {sequence[1]:02X}); kanji belong in {LEDGER}")
    return errors


# --- scanning ------------------------------------------------------------------

C_TOKENS = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|//[^\n]*|/\*.*?(?:\*/|\Z)', re.S)
KANJI_CHAR = re.compile("[" + "".join(chr(low) + "-" + chr(high) for low, high in KANJI_RANGES) + "]")


def strip_c_comments(text: str) -> str:
    """Blank out comments, keeping string/char literals and line numbers."""
    def blank(match):
        token = match[0]
        return token if token[0] in "\"'" else re.sub(r"[^\n]", " ", token)
    return C_TOKENS.sub(blank, text)


def code_mask(text: str) -> str:
    """Hide literals as well as comments when looking for executable tokens."""
    return C_TOKENS.sub(lambda m: re.sub(r"[^\n]", " ", m[0]), text)


def strip_asm_comments(text: str) -> str:
    tokens = re.compile(C_TOKENS.pattern + r"|@[^\n]*", re.S)
    return tokens.sub(lambda m: m[0] if m[0][0] in "\"'" else re.sub(r"[^\n]", " ", m[0]), text)


LITERAL = re.compile(r'"((?:\\.|[^"\\\n])*)"')
DEFINE = re.compile(r"^[ \t]*#[ \t]*define[ \t]+(\w+)\(([^)]*)\)((?:[^\n]*\\\n)*[^\n]*)", re.M)


def literal_kanji(literal_body: str):
    visible = re.sub(r"\{[^}]*\}", "", re.sub(r"\\.", "", literal_body))
    return [char for char in visible if is_kanji(char)]


def find_wrappers(c_texts) -> dict[str, set[int]]:
    """Macros that pass one of their parameters on to a string-encoding wrapper."""
    defines = []
    for text in c_texts:
        for match in DEFINE.finditer(text):
            params = [p.strip() for p in match[2].split(",") if p.strip()]
            defines.append((match[1], params, match[3]))
    wrappers = {name: {0} for name in BASE_WRAPPERS}
    while True:
        names = "|".join(sorted(map(re.escape, wrappers), key=len, reverse=True))
        call = re.compile(r"(?<![\w])(" + names + r")\s*\(")
        changed = False
        for name, params, body in defines:
            forwarded = set()
            for match in call.finditer(code_mask(body)):
                args = call_arguments(body, match.end() - 1)
                if args is None:
                    continue
                parts = split_arguments(args)
                for argument in wrappers[match[1]]:
                    if argument < len(parts):
                        expression = code_mask(parts[argument])
                        forwarded.update(i for i, p in enumerate(params)
                                         if re.search(r"\b" + re.escape(p) + r"\b", expression))
            if forwarded - wrappers.get(name, set()):
                wrappers.setdefault(name, set()).update(forwarded)
                changed = True
        if not changed:
            return wrappers


def call_arguments(text: str, open_paren: int):
    """Text between a call's parentheses, or None if unbalanced."""
    depth, i = 0, open_paren
    while i < len(text):
        ch = text[i]
        if ch in "\"'":
            match = C_TOKENS.match(text, i)
            i = match.end() if match else i + 1
            continue
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                return text[open_paren + 1:i]
        i += 1
    return None


def split_arguments(text: str):
    parts, start, depth, i = [], 0, 0, 0
    while i < len(text):
        if text[i] in "\"'":
            match = C_TOKENS.match(text, i)
            if match:
                i = match.end()
                continue
        if text[i] in "({[":
            depth += 1
        elif text[i] in ")}]":
            depth -= 1
        elif text[i] == "," and depth == 0:
            parts.append(text[start:i])
            start = i + 1
        i += 1
    parts.append(text[start:])
    return parts


def scan_c(text: str, wrappers, found, label):
    pattern = re.compile(r"(?<![\w])(" + "|".join(sorted(map(re.escape, wrappers), key=len, reverse=True)) + r")\s*\(")
    for match in pattern.finditer(code_mask(text)):
        args = call_arguments(text, match.end() - 1)
        if args is None:
            continue
        parts = split_arguments(args)
        for argument in wrappers[match[1]]:
            if argument < len(parts):
                for literal in LITERAL.finditer(parts[argument]):
                    for char in literal_kanji(literal[1]):
                        found.setdefault(char, f"{label}:{text.count(chr(10), 0, match.start()) + 1}")


ASM_STRING = re.compile(r'^[ \t]*(?:\w+:+[ \t]*)?\.(?:string|braille)\b(.*)$', re.M)


def scan_asm(text: str, found, label):
    text = strip_asm_comments(text)
    for match in ASM_STRING.finditer(text):
        for literal in LITERAL.finditer(match[1]):
            for char in literal_kanji(literal[1]):
                found.setdefault(char, f"{label}:{text.count(chr(10), 0, match.start()) + 1}")


def source_files(root: Path, dirs):
    for name in dirs:
        base = root / name
        if base.exists():
            yield from sorted(p for p in base.rglob("*") if p.is_file() and p.suffix in C_SUFFIXES | ASM_SUFFIXES)


def read_source(path: Path) -> str:
    return path.read_bytes().decode("utf-8", errors="replace")


def is_c_source(root: Path, path: Path) -> bool:
    # .inc files under src/ are #included by C files (e.g. contest_npc_extra.inc).
    top = path.relative_to(root).parts[0]
    return path.suffix in C_SUFFIXES or (path.suffix == ".inc" and top in ("src", "include", "test"))


def scan(root: Path):
    """Return ({kanji: first location} for the game, same for test-only sources)."""
    sources, defines = {}, []
    for group, dirs in (("game", SCAN_DIRS), ("test", TEST_SCAN_DIRS)):
        for path in source_files(root, dirs):
            text = read_source(path)
            if is_c_source(root, path) and "define" in text:
                defines.append(text)
            # Only files containing kanji anywhere need a careful look.
            if KANJI_CHAR.search(text):
                sources[path] = (group, text)
    wrappers = find_wrappers(strip_c_comments(t) for t in defines)
    found = {"game": {}, "test": {}}
    for path, (group, text) in sources.items():
        label = path.relative_to(root).as_posix()
        if is_c_source(root, path):
            scan_c(strip_c_comments(text), wrappers, found[group], label)
        else:
            scan_asm(text, found[group], label)
    return found["game"], found["test"]


# --- outputs -------------------------------------------------------------------

def write_if_changed(path: Path, data: bytes) -> bool:
    if path.exists() and path.read_bytes() == data:
        return False
    path.parent.mkdir(parents=True, exist_ok=True)
    temp = None
    try:
        with tempfile.NamedTemporaryFile(dir=path.parent, prefix=path.name + ".", delete=False) as handle:
            temp = Path(handle.name)
            handle.write(data)
            handle.flush()
            os.fsync(handle.fileno())
        os.replace(temp, path)
    finally:
        if temp is not None and temp.exists():
            temp.unlink()
    return True


@contextmanager
def ledger_lock(root: Path):
    path = root / OUT_DIR / ".lock"
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("a+b") as handle:
        if os.name == "nt":
            import msvcrt
            if handle.tell() == 0:
                handle.write(b"\0")
                handle.flush()
            handle.seek(0)
            msvcrt.locking(handle.fileno(), msvcrt.LK_LOCK, 1)
        else:
            import fcntl
            fcntl.flock(handle.fileno(), fcntl.LOCK_EX)
        try:
            yield
        finally:
            if os.name == "nt":
                handle.seek(0)
                msvcrt.locking(handle.fileno(), msvcrt.LK_UNLCK, 1)
            else:
                fcntl.flock(handle.fileno(), fcntl.LOCK_UN)


def update(root: Path, stamp: Path | None = None, checks=None) -> list[str]:
    """Run the whole build step; returns human-readable report lines."""
    with ledger_lock(root):
        return _update(root, stamp, checks)


def _update(root: Path, stamp: Path | None, checks) -> list[str]:
    base_chars, base_constants = load_base_charmap(root)
    entries = read_ledger(root / LEDGER)
    errors = validate_ledger(entries, base_chars, base_constants)
    header = root / "include/constants/characters.h"
    if header.exists():
        for match in re.finditer(r"^#define\s+(EXT_CTRL_CODE_\w+)\s+(0x[0-9A-Fa-f]+)\b", header.read_text(encoding="utf-8"), re.M):
            if (FIRST_BANK_CODE <= int(match[2], 16) < FIRST_BANK_CODE + BANK_COUNT
                    and match[1] not in ("EXT_CTRL_CODE_KANJI_GLYPH", "EXT_CTRL_CODE_KANJI_GLYPH_LAST")):
                errors.append(f"{header}: {match[1]} collides with the kanji bank range")
    if errors:
        raise KanjiError("\n".join(errors))

    atlas = Atlas(root)
    game, test = scan(root)
    used = {c: loc for c, loc in {**test, **game}.items() if c not in base_chars}
    missing = [f"{loc}: error: no glyph for '{c}' (U+{ord(c):04X}) in {ATLAS_PNG}"
               for c, loc in sorted(used.items(), key=lambda item: item[1]) if c not in atlas.index]
    if missing:
        raise KanjiError("\n".join(missing))

    known = {char for char, *_ in entries}
    next_number = max((n for _, n, _ in entries), default=0) + 1
    added = sorted((c for c in used if c not in known), key=ord)
    if next_number - 1 + len(added) > MAX_KANJI:
        raise KanjiError(f"kanji capacity exceeded: {MAX_KANJI} codes available")
    for char in added:
        entries.append((char, next_number, None))
        next_number += 1
    if checks:
        problems = checks(root, entries)
        if problems:
            raise KanjiError("\n".join(problems))

    # Glyph slot 0 stays blank: unknown and retired codes draw nothing.
    by_number = sorted(entries, key=lambda e: e[1])
    max_number = by_number[-1][1] if by_number else 0
    slots = [0] * (max_number + 1)
    cells = [[[0] * 16 for _ in range(16)]]
    for char, n, _ in by_number:
        if char in game:
            slots[n] = len(cells)
            cells.append(glyph_cell(atlas.body(char)))
    glyph_data = fwjpnfont(cells)
    slot_data = struct.pack("<%dH" % len(slots), *slots)

    # An assigned but no longer scanned character must fail at preproc rather
    # than silently reference the blank glyph. Build with only active entries.
    base_text = (root / CHARMAP).read_text(encoding="utf-8")
    include_pattern = re.compile(r'^\s*\.include\s+"([^"]+)"[^\n]*$', re.M)
    includes = list(include_pattern.finditer(base_text))
    if len(includes) != 1 or includes[0][1] != LEDGER:
        raise KanjiError(f"{CHARMAP}: expected exactly one include of {LEDGER} for the build charmap")
    active_lines = "\n".join(f"'{c}' = {encode(n).hex(' ').upper()}" for c, n, _ in by_number if c in used)
    build_charmap = (base_text[:includes[0].start()] + active_lines + base_text[includes[0].end():]).encode("utf-8")

    # Validation and rendering must succeed before any persistent ID is added.
    if added or not (root / LEDGER).exists():
        write_if_changed(root / LEDGER, ledger_text(entries).encode("utf-8"))

    out = root / OUT_DIR
    report = [
        f"ledger entries: {len(entries)} (added this run: {len(added)})",
        f"kanji with glyphs in ROM: {len(cells) - 1}",
        f"test-only kanji (no glyph): {len(set(test) - set(game) - set(base_chars))}",
        f"glyph data: {len(glyph_data)} bytes, slot table: {len(slot_data)} bytes",
        f"capacity: {MAX_KANJI} codes, {MAX_KANJI - max_number} unassigned",
    ]
    if added:
        report.append("added: " + "".join(added))
    write_if_changed(out / GLYPHS_BIN, glyph_data)
    write_if_changed(out / SLOTS_BIN, slot_data)
    write_if_changed(out / BUILD_CHARMAP, build_charmap)
    write_if_changed(out / REPORT, ("\n".join(report) + "\n").encode("utf-8"))
    if stamp:
        stamp.parent.mkdir(parents=True, exist_ok=True)
        stamp.write_text("ok\n")
    return report


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("command", choices=("update",))
    parser.add_argument("--root", type=Path, default=ROOT)
    parser.add_argument("--stamp", type=Path)
    args = parser.parse_args(argv)
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    from save_link_checks import check_save_link_strings
    try:
        report = update(args.root.resolve(), args.stamp, check_save_link_strings)
    except KanjiError as error:
        print(error, file=sys.stderr)
        return 1
    for line in report:
        print("kanji: " + line)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
