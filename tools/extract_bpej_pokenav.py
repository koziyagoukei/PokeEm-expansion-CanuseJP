#!/usr/bin/env python3
"""Extract verified BPEJ Pokenav, Match Call and ribbon UI assets."""

from pathlib import Path, PurePosixPath

import extract_bpej_contest as graphics


OUTPUT_ROOT = PurePosixPath("build/extracted_pokenav")
CUSTOM_ROOT = PurePosixPath("graphics/custom/bpej/pokenav")
CONSUMERS = {
    "src/graphics.c", "src/pokenav_list.c", "src/pokenav_main_menu.c",
    "src/pokenav_match_call_gfx.c", "src/pokenav_menu_handler_gfx.c",
    "src/pokenav_ribbons_list.c", "src/pokenav_ribbons_summary.c", "src/match_call.c",
}


def load_manifest(path: Path) -> dict:
    manifest = graphics.load_manifest(
        path, output_root=OUTPUT_ROOT, custom_root=CUSTOM_ROOT,
        source_roots=("graphics/pokenav/",), consumers=CONSUMERS, category="Pokenav",
    )
    for asset in manifest["assets"]:
        label = asset["label"]
        if asset["group"] not in ("common", "region_map", "match_call", "ribbons"):
            raise ValueError(f"{label}: invalid Pokenav group")
        if any(word in label for word in ("Kanto", "Sevii", "_Red", "_Leaf")):
            raise ValueError(f"{label}: FRLG map/player additions are not BPEJ assets")
        if asset["transform"] != "copy":
            raise ValueError(f"{label}: Pokenav assets must be complete, unmodified ROM assets")
        if not asset["verification"].get("references"):
            raise ValueError(f"{label}: BPEJ reference evidence is missing")
    return manifest


def main(argv: list[str] | None = None) -> int:
    return graphics.run_extraction(
        argv, loader=load_manifest, output_root=OUTPUT_ROOT,
        default_manifest="tools/data/bpej_pokenav_manifest.json",
        category="Pokenav", description=__doc__,
    )


if __name__ == "__main__":
    raise SystemExit(main())
