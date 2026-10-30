#!/usr/bin/env python3
"""Extract verified BPEJ berry tag, fruit, icon, tree, Blender and Crush assets."""
from pathlib import Path, PurePosixPath

import extract_bpej_contest as graphics


OUTPUT_ROOT = PurePosixPath("build/extracted_berry")
CUSTOM_ROOT = PurePosixPath("graphics/custom/bpej/berry")
CONSUMERS = {
    "src/data/graphics/berries.h", "src/data/graphics/items.h", "src/graphics.c",
    "src/berry_tag_screen.c", "src/berry_blender.c", "src/berry_crush.c",
    "src/data/object_events/object_event_graphics.h",
}
SOURCE_ROOTS = ("graphics/berries/", "graphics/berry_blender/", "graphics/berry_crush/",
                "graphics/bag/check_berry", "graphics/bag/berry_tag",
                "graphics/items/icons/", "graphics/items/icon_palettes/",
                "graphics/object_events/pics/berry_trees/")
JAPANESE_REPLACEMENTS = {
    "gBerryCheck_Gfx", "gBerryTag_Gfx", "gBerryCheckCircle_Gfx",
    "gBerryBlenderStart_Gfx", "gBerryCrush_Crusher_Gfx",
}
GROUP_ROOTS = {
    "tag": "graphics/bag/", "berries": "graphics/berries/", "blender": "graphics/berry_blender/",
    "crush": "graphics/berry_crush/", "item_icons": "graphics/items/",
    "trees": "graphics/object_events/pics/berry_trees/",
}


def load_manifest(path: Path) -> dict:
    manifest = graphics.load_manifest(
        path, output_root=OUTPUT_ROOT, custom_root=CUSTOM_ROOT,
        source_roots=SOURCE_ROOTS, consumers=CONSUMERS, category="berry",
    )
    for asset in manifest["assets"]:
        label = asset["label"]
        root = GROUP_ROOTS.get(asset["group"])
        if root is None or not asset["source_file"].startswith(root):
            raise ValueError(f"{label}: invalid berry group/source")
        if asset["transform"] not in ("copy", "gba_lz77_decompress"):
            raise ValueError(f"{label}: berry assets must be complete ROM payloads")
        evidence = asset["verification"]
        if not evidence.get("references"):
            raise ValueError(f"{label}: BPEJ ownership references are missing")
        if "BerryPouch" in label or "berry_pouch" in asset["source_file"]:
            raise ValueError(f"{label}: FRLG berry pouch is not a verified Emerald berry asset")
        if evidence["kind"] == "japanese_ui" and label not in JAPANESE_REPLACEMENTS:
            raise ValueError(f"{label}: unreviewed Japanese UI replacement")
        if asset["dimensions"] != "[]":
            raise ValueError(f"{label}: unexpected array dimensions")
        if asset["element_type"] == "u16" and asset["output_representation"] != "raw":
            raise ValueError(f"{label}: palettes are loaded as uncompressed data")
        if asset["group"] == "trees" and asset["output_representation"] != "raw":
            raise ValueError(f"{label}: object-event frame images must remain uncompressed")
    return manifest


def main(argv: list[str] | None = None) -> int:
    return graphics.run_extraction(
        argv, loader=load_manifest, output_root=OUTPUT_ROOT,
        default_manifest="tools/data/bpej_berry_manifest.json",
        category="berry", description=__doc__,
    )


if __name__ == "__main__":
    raise SystemExit(main())
