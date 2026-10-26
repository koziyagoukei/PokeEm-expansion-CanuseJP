#!/usr/bin/env python3

"""Refresh the Frontier Move overlay for moves first learnable in Pokemon Champions.

Pokemon Showdown keeps the Champions learnsets in data/mods/champions. This
tool keeps only the species/move pairs that the Frontier full, event and
Legends Z-A DLC learnsets do not already have, so moves a Pokemon could learn
in an earlier game are not repeated. Moves are not carried to other members of
the evolution line; Champions teaches them only to the species it lists.

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

from update_event_moves import (
    SHOWDOWN_API_HEAD,
    SHOWDOWN_REPOSITORY,
    make_species_resolver,
    normalize_identifier,
    parse_move_constants,
    parse_pokedex,
    read_url,
)
from update_za_dlc_moves import parse_frontier_learnsets


SHOWDOWN_RAW = "https://raw.githubusercontent.com/smogon/pokemon-showdown/{commit}/{path}"
CHAMPIONS_LEARNSETS = "data/mods/champions/learnsets.ts"
POKEDEX = "data/pokedex.ts"

# Default forms Showdown names without a suffix, beyond those the event tool already knows.
CHAMPIONS_SPECIES_ALIASES = {
    "indeedee": "SPECIES_INDEEDEE_M",
    "lycanroc": "SPECIES_LYCANROC_MIDDAY",
    "meowstic": "SPECIES_MEOWSTIC_M",
}


def fetch_showdown_head() -> tuple[str, str, str, str]:
    metadata = json.loads(read_url(SHOWDOWN_API_HEAD))
    commit = metadata["sha"]
    commit_date = metadata["commit"]["committer"]["date"]
    learnsets = read_url(SHOWDOWN_RAW.format(commit=commit, path=CHAMPIONS_LEARNSETS))
    pokedex = read_url(SHOWDOWN_RAW.format(commit=commit, path=POKEDEX))
    return commit, commit_date, learnsets, pokedex


def parse_champions_moves(source: str, move_constants: dict[str, str]) -> dict[str, set[str]]:
    block_pattern = re.compile(
        r"^\t([A-Za-z0-9]+): \{\n\t\tlearnset: \{\n(.*?)^\t\t\},",
        re.M | re.S,
    )
    move_pattern = re.compile(r"^\t\t\t([A-Za-z0-9]+): \[", re.M)
    moves = {}
    for species_id, body in block_pattern.findall(source):
        species_moves = set()
        for move_id in move_pattern.findall(body):
            move = move_constants.get(normalize_identifier(move_id))
            if move is None:
                raise ValueError(f"Showdown move has no Expansion constant: {move_id}")
            species_moves.add(move)
        moves[species_id] = species_moves
    if not moves:
        raise ValueError("No Champions learnsets were found")
    return moves


def build_overlay(learnsets_source: str, pokedex_source: str, root: Path):
    data = root / "src/data/pokemon"
    full = parse_frontier_learnsets(data / "frontier_full_learnsets.h", "sFrontierFullLearnset")
    events = parse_frontier_learnsets(data / "frontier_event_learnsets.h", "sFrontierEventLearnset")
    za_dlc = parse_frontier_learnsets(data / "frontier_za_dlc_learnsets.h", "sFrontierZaDlcLearnset")
    move_constants = parse_move_constants((root / "include/constants/moves.h").read_text(encoding="utf-8"))
    base_species, _ = parse_pokedex(pokedex_source)
    champions = parse_champions_moves(learnsets_source, move_constants)
    resolve_species = make_species_resolver(full, base_species)

    overlay = defaultdict(set)
    ignored = set()
    for source_species, moves in champions.items():
        target = CHAMPIONS_SPECIES_ALIASES.get(source_species) or resolve_species(source_species)
        if target not in full:
            ignored.add(source_species)
            continue
        known = full[target] | events.get(target, set()) | za_dlc.get(target, set())
        overlay[target].update(moves - known)

    overlay = {
        species: sorted(moves)
        for species, moves in sorted(overlay.items())
        if moves
    }
    return overlay, sorted(ignored), len(champions)


def make_document(overlay, ignored, champions_species_count, commit, commit_date):
    return {
        "metadata": {
            "description": "Pokemon Champions move additions for Frontier Move",
            "source": f"{SHOWDOWN_REPOSITORY}/blob/{commit}/{CHAMPIONS_LEARNSETS}",
            "source_commit": commit,
            "source_commit_date": commit_date,
            "champions_species": champions_species_count,
            "propagate_through_evolution": False,
            "included_species": len(overlay),
            "included_pairs": sum(map(len, overlay.values())),
            "ignored_unmapped_source_species": ignored,
            "verification_sources": [
                "https://game8.co/games/Pokemon-Champions/archives/593893",
                "https://bulbapedia.bulbagarden.net/wiki/Swampert_(Pok%C3%A9mon)/Champions_learnset",
            ],
        },
        "species": overlay,
    }


def parse_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument("--learnsets", type=Path, help="Downloaded Showdown data/mods/champions/learnsets.ts")
    parser.add_argument("--pokedex", type=Path, help="Downloaded Showdown data/pokedex.ts")
    parser.add_argument("--source-commit", help="Commit SHA for local input files")
    parser.add_argument("--source-date", help="Commit timestamp for local input files")
    parser.add_argument("--output", type=Path)
    parser.add_argument("--check", action="store_true", help="Fail if the tracked JSON differs")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    root = args.root.resolve()
    output = args.output or root / "src/data/pokemon/frontier_champions_moves.json"

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
        print(f"Ignored {len(ignored)} unmapped species: {', '.join(ignored)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
