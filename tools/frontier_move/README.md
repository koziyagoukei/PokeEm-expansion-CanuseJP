# Frontier Move data maintenance

`frontier_move` combines two independent tables at runtime:

- `frontier_full_learnsets.h`: regular level, TM, egg, and tutor sources
- `frontier_event_learnsets.h`: event-distribution additions

The event data is stored canonically in
`src/data/pokemon/frontier_event_moves.json`. Its metadata records the exact
Pokemon Showdown commit used for the refresh. Normal builds never access the
network.

## Refresh event moves

Run the updater manually when the external source needs to be refreshed:

```sh
python3 tools/frontier_move/update_event_moves.py
python3 tools/frontier_move/generate_event_learnsets.py
```

The updater reads Pokemon Showdown `data/learnsets.ts` entries whose source
contains `S`, the marker used for event distributions. Event moves are carried
through normal evolutions, then species/move pairs already present in the full
table are removed.

For a reproducible offline refresh, download `learnsets.ts` and `pokedex.ts`
from one commit and provide its metadata explicitly:

```sh
python3 tools/frontier_move/update_event_moves.py \
    --learnsets /path/to/learnsets.ts \
    --pokedex /path/to/pokedex.ts \
    --source-commit COMMIT_SHA \
    --source-date COMMIT_TIMESTAMP
python3 tools/frontier_move/generate_event_learnsets.py
```

Verify the checked-in generated files with:

```sh
python3 tools/frontier_move/update_event_moves.py <same source arguments> --check
python3 tools/frontier_move/generate_event_learnsets.py --check
```

Showdown-only species that have no Expansion species mapping are listed in the
JSON metadata and intentionally omitted.

## Form fallback

`GetFrontierFullLearnset` and `GetFrontierEventLearnset` first use an explicit
form table. If one is absent, they use `GET_BASE_SPECIES_ID` to resolve form 0.
This applies to all form families, including Alcremie and Ogerpon. Explicit
regional or form-specific tables continue to take priority.
