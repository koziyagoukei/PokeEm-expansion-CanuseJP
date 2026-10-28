"""Build-time checks for ROM strings that end up in save data or are sent over link.

Kanji codes (FC 1F..27 xx) belong to this project's ledger only, and the name
copies below truncate by byte count, which can cut a 3-byte code in half. So
every string that is copied into a saved or transmitted buffer must be free of
kanji and fit that buffer. The rules list the source of each such string; see
docs/kanji.md for the copy path behind every rule.
"""
from __future__ import annotations

import re
from dataclasses import dataclass, field
from pathlib import Path

from kanji_gen import (CHARMAP, CONSTANT_LINE, CHARMAP_LINE, DEFINE, LITERAL, call_arguments,
                       code_mask, encode, is_kanji, split_arguments, strip_c_comments, strip_charmap_comment)

WRAPPER_CALL = re.compile(r"(?<![\w])(J__|J_|__|_|J_COMPOUND_STRING|COMPOUND_STRING)\s*\(")
LIMIT_HEADERS = ("include/constants/global.h", "include/pokemon_storage_system.h")


@dataclass
class Rule:
    """Strings found by `fields` (struct members) or `symbols` (variables) in `paths`."""
    paths: tuple
    limit: str | int | None
    reason: str
    fields: tuple = ()
    symbols: str | None = None
    macro: tuple | None = None  # (name, argument index, implicit prefix)
    required: bool = True
    hits: list = field(default_factory=list)


RULES = (
    Rule(("src/data/pokemon/species_info/*.h",), "POKEMON_NAME_LENGTH",
         "default nickname of every Pokémon (saved, traded, record-mixed)", fields=("speciesName",)),
    Rule(("src/daycare.c", "src/evolution_scene.c"), "POKEMON_NAME_LENGTH",
         "Egg and Shedinja nicknames (saved)", symbols=r"sJapaneseEggNickname|sText_ShedinjaJapaneseName"),
    Rule(("src/data/trade.h",), "POKEMON_NAME_LENGTH", "in-game trade nickname (saved)", fields=("nickname",)),
    Rule(("src/data/trade.h",), "PLAYER_NAME_LENGTH",
         "in-game trade OT name and mail sender (saved, sent in union room trades)", fields=("otName",)),
    Rule(("src/data/trainers.h", "src/data/debug_trainers.h"), "TRAINER_NAME_LENGTH",
         "OT of trainer Pokémon (link multi battles, battle videos) and Frontier Brain names in TV records",
         fields=("trainerName",)),
    Rule(("src/data/battle_partners.h",), "PLAYER_NAME_LENGTH",
         "partner names copied into the Battle Tower interview (saved, record-mixed)", fields=("trainerName",)),
    Rule(("src/data/trainers.h", "src/data/battle_partners.h", "src/data/debug_trainers.h",
          "src/data/battle_frontier/battle_frontier_*.h", "src/data/battle_frontier/battle_tent.h"),
         "POKEMON_NAME_LENGTH", "trainer and Frontier Pokémon nicknames (link battles, rental party, battle videos)",
         fields=("nickname",), required=False),
    Rule(("src/data/battle_frontier/battle_frontier_trainers.h", "src/data/battle_frontier/battle_tent.h"),
         "PLAYER_NAME_LENGTH", "Frontier trainer names (Battle Tower interview, partner OT)", fields=("trainerName",)),
    Rule(("src/data/battle_frontier/trainer_hill.h",), "VANILLA_POKEMON_NAME_LENGTH",
         "Trainer Hill nicknames", fields=("nickname",)),
    Rule(("src/data/battle_frontier/apprentice.h",), "PLAYER_NAME_LENGTH",
         "apprentice names (OT of apprentice Pokémon)", fields=("name",)),
    Rule(("src/data/contest_opponents.h",), "VANILLA_POKEMON_NAME_LENGTH",
         "contest winners' Pokémon names (saved, TV shows are record-mixed)", fields=("monName", "nickname")),
    Rule(("src/data/contest_opponents.h",), "PLAYER_NAME_LENGTH",
         "contest trainer names (saved, TV shows are record-mixed)", fields=("trainerName",)),
    Rule(("src/data/contest_npc_presets.h",), "VANILLA_POKEMON_NAME_LENGTH",
         "contest NPC nicknames (saved as contest winners)", symbols=r"sContestNpcNicknameOverrides"),
    Rule(("src/data/contest_npc_extra.inc",), "VANILLA_POKEMON_NAME_LENGTH",
         "contest NPC nicknames (saved as contest winners)", macro=("EXTRA_NPC", 1, "{JPN}")),
    Rule(("src/data/contest_npc_extra.inc",), "PLAYER_NAME_LENGTH",
         "contest NPC trainer names (saved as contest winners)", macro=("EXTRA_NPC", 2, "{JPN}")),
    Rule(("src/data/lilycove_lady.h",), "VANILLA_POKEMON_NAME_LENGTH",
         "contest lady's Pokémon name (TV show, record-mixed)", fields=("monName",)),
    Rule(("src/main_menu.c",), "PLAYER_NAME_LENGTH",
         "preset player names (saved, sent as the link player name)", symbols=r"s(?:Male|Female)PresetNames"),
    Rule(("src/quickstart.c",), "PLAYER_NAME_LENGTH",
         "quick start player and rival names (saved)", symbols=r"sText_(?:PlayerMale|PlayerFemale|Rival)"),
    Rule(("src/pokemon_storage_system.c",), "BOX_NAME_LENGTH - 2",
         "default box name plus a 2-digit number (saved)", symbols=r"gText_Box"),
    Rule(("src/strings.c",), 20, "Union Room chat registered texts (saved, sent in chat)",
         symbols=r"gText_(?:Hello|Pokemon2|Trade|Battle|Lets|Ok|Sorry|YaySmileEmoji|ThankYou|ByeBye)"),
    Rule(("src/strings.c",), "PLAYER_NAME_LENGTH", "storyteller name sent to Ruby/Sapphire", symbols=r"gText_Friend"),
    Rule(("src/strings.c",), 15, "default Walda phrase (saved)", symbols=r"gText_Peekaboo"),
    Rule(("src/trader.c",), 10, "Mauville trader names (saved, record-mixed)", symbols=r"sDefaultTraderNames"),
    Rule(("src/field_specials.c",), "PLAYER_NAME_LENGTH", "fan club idol names (TV show, record-mixed)",
         symbols=r"sText_(?:Wallace|Steven|Brawly|Winona|Phoebe|Glacia)"),
    # Safety net: no kanji in any name-like field anywhere, whatever its buffer.
    Rule(("src/**/*.h", "src/**/*.c"), None, "name fields are copied into Pokémon and save data",
         fields=("speciesName", "nickname", "otName", "trainerName", "monName"), required=False),
)


def load_limits(root: Path):
    values = {}
    for header in LIMIT_HEADERS:
        for match in re.finditer(r"^#define\s+(\w+)\s+([^\n/]+)", (root / header).read_text(encoding="utf-8"), re.M):
            values[match[1]] = match[2].strip()
    return values


def evaluate(limit, values):
    if limit is None or isinstance(limit, int):
        return limit
    expression = re.sub(r"[A-Z_][A-Z0-9_]*", lambda m: "(%s)" % values[m[0]], limit)
    while re.search(r"[A-Z_]", expression):
        expression = re.sub(r"[A-Z_][A-Z0-9_]*", lambda m: "(%s)" % values[m[0]], expression)
    if not re.fullmatch(r"[0-9+\-*() ]+", expression):
        raise ValueError("cannot evaluate limit %r" % limit)
    return eval(expression)  # only digits and arithmetic remain


class Encoder:
    """Byte length of a string as preproc encodes it (charmap + kanji ledger)."""

    def __init__(self, root: Path, ledger_entries):
        self.chars, self.constants = {}, {}
        for line in (root / CHARMAP).read_text(encoding="utf-8").splitlines():
            line = strip_charmap_comment(line).strip()
            match = CHARMAP_LINE.match(line)
            if match:
                self.chars[match[1].replace("\\'", "'")] = len(bytes.fromhex(match[2]))
            elif CONSTANT_LINE.match(line):
                name, data = CONSTANT_LINE.match(line).groups()
                self.constants[name] = bytes.fromhex(data)
        # Characters of the base charmap (such as 円) are ordinary 1-byte glyphs.
        self.base = set(self.chars)
        for char, number, *_ in ledger_entries:
            self.chars[char] = len(encode(number))

    def measure(self, text: str):
        """Return (bytes without the terminator, kanji found)."""
        length, kanji, i = 0, [], 0
        while i < len(text):
            ch = text[i]
            if ch == "{":
                end = text.index("}", i)
                for token in text[i + 1:end].split():
                    sequence = self.constants.get(token)
                    if sequence is None:
                        raise ValueError(f"unresolved control argument {{{token}}}")
                    if sequence[0] in (0xFD, 0xF7):
                        raise ValueError(f"dynamic placeholder {{{token}}} is not allowed in saved/link text")
                    length += len(sequence)
                i = end + 1
            elif ch == "\\":
                escaped = text[i:i + 2]
                if escaped == r'\"':
                    escaped = '"'
                if escaped not in self.chars:
                    raise ValueError(f"unresolved escape {escaped!r}")
                length += self.chars[escaped]
                i += 2
            else:
                if is_kanji(ch) and ch not in self.base:
                    kanji.append(ch)
                if ch not in self.chars:
                    raise ValueError(f"undefined character {ch!r}")
                length += self.chars[ch]
                i += 1
        return length, kanji


def wrapped_strings(text: str):
    """(offset, contents) of every encoded string in a C fragment."""
    for match in WRAPPER_CALL.finditer(code_mask(text)):
        args = call_arguments(text, match.end() - 1)
        if args is not None:
            if LITERAL.sub("", args).strip():
                yield match.start(), None
            else:
                prefix = "{JPN}" if match[1].startswith("J_") else ""
                yield match.start(), prefix + "".join(m[1] for m in LITERAL.finditer(args))


def initializer(text: str, start: int):
    """The initializer text that starts at `start` (after '='), up to ',' / ';' at depth 0."""
    depth, i = 0, start
    while i < len(text):
        ch = text[i]
        if ch == '"':
            match = LITERAL.match(text, i)
            i = match.end() if match else i + 1
            continue
        if ch in "({[":
            depth += 1
        elif ch in ")}]":
            if depth == 0:
                return text[start:i]
            depth -= 1
        elif ch in ",;" and depth == 0:
            return text[start:i]
        i += 1
    return text[start:]


def find_strings(rule: Rule, text: str):
    if rule.fields:
        pattern = re.compile(r"\.(?:%s)\s*=" % "|".join(rule.fields))
        for match in pattern.finditer(code_mask(text)):
            body = initializer(text, match.end())
            values = list(wrapped_strings(body))
            if not values and rule.limit is not None and body.strip() not in ("0", "NULL", "{0}", "{ 0 }"):
                yield match.end(), None
            for offset, value in values:
                yield match.end() + offset, value
    if rule.symbols:
        pattern = re.compile(r"\b(?:%s)\b\s*(?:\[[^\]=;]*\]\s*)*=" % rule.symbols)
        for match in pattern.finditer(code_mask(text)):
            body = text[match.end():text.find(";", match.end())]
            for offset, value in wrapped_strings(body):
                yield match.end() + offset, value
    if rule.macro:
        name, index, prefix = rule.macro
        for match in re.finditer(r"^\s*%s\s*\(" % name, text, re.M):
            args = call_arguments(text, match.end() - 1) or ""
            parts = [p.strip() for p in split_arguments(args)]
            literal = LITERAL.fullmatch(parts[index]) if index < len(parts) else None
            if literal:
                yield match.start(), prefix + literal[1]
            else:
                yield match.start(), None


def check_save_link_strings(root: Path, ledger_entries, rules=RULES):
    encoder, values = Encoder(root, ledger_entries), load_limits(root)
    problems = []
    for rule in rules:
        limit = evaluate(rule.limit, values)
        files = sorted({p for pattern in rule.paths for p in root.glob(pattern) if p.is_file()})
        count = 0
        for path in files:
            text = strip_c_comments(path.read_bytes().decode("utf-8", errors="replace"))
            # Function-like macro definitions are templates, not initialized
            # save fields. Their instantiated EXTRA_NPC names have own rules.
            text = DEFINE.sub(lambda m: re.sub(r"[^\n]", " ", m[0]), text)
            label = path.relative_to(root).as_posix()
            for offset, value in find_strings(rule, text):
                count += 1
                where = f"{label}:{text.count(chr(10), 0, offset) + 1}"
                if value is None:
                    problems.append(f"{where}: error: unresolved saved/link text initializer: {rule.reason}")
                    continue
                try:
                    length, kanji = encoder.measure(value)
                except ValueError as error:
                    problems.append(f"{where}: error: {error}: {rule.reason}")
                    continue
                if kanji:
                    problems.append(f"{where}: error: kanji {''.join(kanji)!r} in \"{value}\": {rule.reason}; "
                                    "kanji must not reach save or link data")
                elif limit is not None and length > limit:
                    problems.append(f"{where}: error: \"{value}\" is {length} bytes, limit {limit} "
                                    f"({rule.limit}): {rule.reason}")
        if rule.required and count == 0:
            problems.append(f"save/link check found no strings for: {rule.reason} ({', '.join(rule.paths)}); "
                            "update tools/kanji/save_link_checks.py")
    return problems
