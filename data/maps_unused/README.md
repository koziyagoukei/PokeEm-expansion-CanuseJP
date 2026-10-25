# Unused maps

These maps are outside the Battle Frontier area and are not built into the ROM.
The build only reads `data/maps/*/map.json`, `data/maps/map_groups.json` and
`data/layouts/layouts.json`, so nothing in this folder or in
`data/layouts_unused/` is compiled. The files stay here as source material, for
example for map tools that copy tiles from them.

- `map_groups_original.json`: the map groups before the removal.
- `wild_encounters_unused.json`: the wild encounter entries of these maps.
- `../layouts_unused/layouts.json`: the layout entries, pointing at
  `data/layouts_unused/`.

To bring a map back into the game, move its folder to `data/maps/`, move its
layout folder to `data/layouts/`, and restore its entries in `map_groups.json`,
`layouts.json`, the `.include` of its `scripts.inc` in `data/event_scripts.s`,
and `src/data/wild_encounters.json`. Code that still refers to removed maps uses
the placeholder constants that `mapjson` generates, so those references need to
be checked as well. See `docs/tutorials/how_to_delete_vanilla_maps.md`.
