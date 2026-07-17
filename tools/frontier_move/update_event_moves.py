#!/usr/bin/env python3

"""Refresh the Frontier Move event-distribution move overlay.

Pokemon Showdown encodes event-distribution learn sources with ``S`` (for
example, ``6S3``). This tool extracts those species/move pairs, carries moves
forward through normal evolutions, and writes only pairs missing from the
existing Frontier full learnsets.

The network path is intentionally manual and is not part of normal builds.
Use --learnsets and --pokedex to reproduce a refresh from downloaded files.
"""

from __future__ import annotations

import argparse
from collections import defaultdict
from datetime import datetime, timezone
import json
from pathlib import Path
import re
import sys
import unicodedata
from urllib.request import Request, urlopen


SHOWDOWN_REPOSITORY = "https://github.com/smogon/pokemon-showdown"
SHOWDOWN_API_HEAD = "https://api.github.com/repos/smogon/pokemon-showdown/commits/master"
SHOWDOWN_RAW = "https://raw.githubusercontent.com/smogon/pokemon-showdown/{commit}/data/{name}"

# Showdown uses the unqualified name for these default forms. Expansion uses
# an explicit form suffix in the Frontier table.
SHOWDOWN_SPECIES_ALIASES = {
    "deoxys": "SPECIES_DEOXYS_NORMAL",
    "hoopa": "SPECIES_HOOPA_CONFINED",
    "shaymin": "SPECIES_SHAYMIN_LAND",
    "toxtricity": "SPECIES_TOXTRICITY_AMPED",
}


def normalize_identifier(value: str) -> str:
    value = unicodedata.normalize("NFKD", value)
    value = "".join(char for char in value if not unicodedata.combining(char))
    return re.sub(r"[^a-z0-9]", "", value.lower())


def read_url(url: str) -> str:
    request = Request(url, headers={"User-Agent": "pokeemerald-frontier-move-updater"})
    with urlopen(request, timeout=60) as response:
        return response.read().decode("utf-8")


def fetch_showdown_head() -> tuple[str, str, str, str]:
    metadata = json.loads(read_url(SHOWDOWN_API_HEAD))
    commit = metadata["sha"]
    commit_date = metadata["commit"]["committer"]["date"]
    learnsets_url = SHOWDOWN_RAW.format(commit=commit, name="learnsets.ts")
    pokedex_url = SHOWDOWN_RAW.format(commit=commit, name="pokedex.ts")
    return commit, commit_date, read_url(learnsets_url), read_url(pokedex_url)


def parse_frontier_learnsets(header: str) -> dict[str, set[str]]:
    arrays = {}
    pattern = re.compile(
        r"static const u16 sFrontierFullLearnset_(SPECIES_[A-Z0-9_]+)\[\]\s*=\s*\{(.*?)\};",
        re.S,
    )
    for species, body in pattern.findall(header):
        moves = set(re.findall(r"\bMOVE_[A-Z0-9_]+\b", body))
        moves.discard("MOVE_UNAVAILABLE")
        arrays[species] = moves
    if not arrays:
        raise ValueError("No Frontier full learnsets were found")
    return arrays


def parse_move_constants(header: str) -> dict[str, str]:
    constants = {}
    for move in re.findall(r"^\s*(MOVE_[A-Z0-9_]+)\s*(?:=|,)", header, re.M):
        key = normalize_identifier(move.removeprefix("MOVE_"))
        constants.setdefault(key, move)
    return constants


def parse_pokedex(source: str):
    blocks = dict(re.findall(r"^\t([A-Za-z0-9]+): \{\n(.*?)^\t\},", source, re.M | re.S))
    name_to_id = {}
    for species_id, body in blocks.items():
        match = re.search(r'^\t\tname: "([^"]+)",', body, re.M)
        if match:
            name_to_id[normalize_identifier(match.group(1))] = species_id

    base_species = {}
    children = defaultdict(set)
    for species_id, body in blocks.items():
        match = re.search(r'^\t\tbaseSpecies: "([^"]+)",', body, re.M)
        if match:
            base_name = normalize_identifier(match.group(1))
            base_species[species_id] = name_to_id.get(base_name, base_name)

        match = re.search(r'^\t\tprevo: "([^"]+)",', body, re.M)
        if match:
            prevo = name_to_id.get(normalize_identifier(match.group(1)))
            if prevo is not None:
                children[prevo].add(species_id)

    return base_species, children


def parse_event_moves(source: str, move_constants: dict[str, str]) -> dict[str, set[str]]:
    event_moves = defaultdict(set)
    block_pattern = re.compile(
        r"^\t([A-Za-z0-9]+): \{\n\t\tlearnset: \{\n(.*?)^\t\t\},",
        re.M | re.S,
    )
    move_pattern = re.compile(r"^\t\t\t([A-Za-z0-9]+): \[(.*?)\],", re.M)

    for species_id, body in block_pattern.findall(source):
        for move_id, sources in move_pattern.findall(body):
            if re.search(r'"\d+S\d+', sources) is None:
                continue
            move = move_constants.get(normalize_identifier(move_id))
            if move is None:
                raise ValueError(f"Showdown move has no Expansion constant: {move_id}")
            event_moves[species_id].add(move)

    return event_moves


def make_species_resolver(frontier_learnsets, base_species):
    by_identifier = {
        normalize_identifier(species.removeprefix("SPECIES_")): species
        for species in frontier_learnsets
    }

    def resolve(species_id: str) -> str | None:
        current = species_id
        visited = set()
        while current not in visited:
            visited.add(current)
            direct = by_identifier.get(normalize_identifier(current))
            if direct is not None:
                return direct
            if current in SHOWDOWN_SPECIES_ALIASES:
                alias = SHOWDOWN_SPECIES_ALIASES[current]
                if alias in frontier_learnsets:
                    return alias
            if current not in base_species:
                break
            current = base_species[current]
        return None

    return resolve


def build_overlay(learnsets_source: str, pokedex_source: str, root: Path):
    frontier_header = (root / "src/data/pokemon/frontier_full_learnsets.h").read_text(encoding="utf-8")
    move_header = (root / "include/constants/moves.h").read_text(encoding="utf-8")
    frontier_learnsets = parse_frontier_learnsets(frontier_header)
    move_constants = parse_move_constants(move_header)
    base_species, children = parse_pokedex(pokedex_source)
    event_moves = parse_event_moves(learnsets_source, move_constants)
    resolve_species = make_species_resolver(frontier_learnsets, base_species)

    overlay = defaultdict(set)
    ignored = set()
    for source_species, moves in event_moves.items():
        pending = [source_species]
        visited = set()
        while pending:
            species_id = pending.pop()
            if species_id in visited:
                continue
            visited.add(species_id)

            target = resolve_species(species_id)
            if target is None:
                if species_id == source_species:
                    ignored.add(source_species)
            else:
                overlay[target].update(moves - frontier_learnsets[target])

            pending.extend(children[species_id])

    overlay = {
        species: sorted(moves)
        for species, moves in sorted(overlay.items())
        if moves
    }
    return overlay, sorted(ignored), len(event_moves)


def make_document(overlay, ignored, event_species_count, commit, commit_date):
    source_url = f"{SHOWDOWN_REPOSITORY}/blob/{commit}/data/learnsets.ts"
    return {
        "metadata": {
            "description": "Event-distribution move additions for Frontier Move",
            "source": source_url,
            "source_commit": commit,
            "source_commit_date": commit_date,
            "source_marker": "S (Pokemon Showdown event source)",
            "event_source_species": event_species_count,
            "propagate_through_evolution": True,
            "ignored_unmapped_source_species": ignored,
            "verification_sources": [
                "https://bulbapedia.bulbagarden.net/wiki/Event_Pokemon",
                "https://www.serebii.net/games/eventmoves.shtml",
            ],
        },
        "species": overlay,
    }


def parse_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument("--learnsets", type=Path, help="Downloaded Pokemon Showdown learnsets.ts")
    parser.add_argument("--pokedex", type=Path, help="Downloaded Pokemon Showdown pokedex.ts")
    parser.add_argument("--source-commit", help="Commit SHA for local input files")
    parser.add_argument("--source-date", help="Commit timestamp for local input files")
    parser.add_argument("--output", type=Path)
    parser.add_argument("--check", action="store_true", help="Fail if the tracked JSON differs")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    root = args.root.resolve()
    output = args.output or root / "src/data/pokemon/frontier_event_moves.json"

    if bool(args.learnsets) != bool(args.pokedex):
        raise SystemExit("--learnsets and --pokedex must be specified together")

    if args.learnsets:
        learnsets_source = args.learnsets.read_text(encoding="utf-8")
        pokedex_source = args.pokedex.read_text(encoding="utf-8")
        commit = args.source_commit or "local-input"
        commit_date = args.source_date or datetime.now(timezone.utc).isoformat()
    else:
        commit, commit_date, learnsets_source, pokedex_source = fetch_showdown_head()

    overlay, ignored, source_count = build_overlay(learnsets_source, pokedex_source, root)
    document = make_document(overlay, ignored, source_count, commit, commit_date)
    content = json.dumps(document, ensure_ascii=False, indent=2) + "\n"

    if args.check:
        if not output.exists() or output.read_text(encoding="utf-8") != content:
            print(f"{output} is out of date", file=sys.stderr)
            return 1
        print(f"{output} is up to date")
        return 0

    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(content, encoding="utf-8")
    print(f"Wrote {len(overlay)} species and {sum(map(len, overlay.values()))} moves to {output}")
    if ignored:
        print(f"Ignored {len(ignored)} source-only species: {', '.join(ignored)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

