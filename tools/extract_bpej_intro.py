#!/usr/bin/env python3
"""Extract verified Emerald opening and title assets without replacing custom logos."""

from pathlib import Path, PurePosixPath

import extract_bpej_contest as graphics


OUTPUT_ROOT = PurePosixPath("build/extracted_intro")
CUSTOM_ROOT = PurePosixPath("graphics/custom/bpej/intro")
CONSUMERS = {
    "src/graphics.c", "src/title_screen.c", "src/intro.c",
    "src/intro_credits_graphics.c", "src/data/graphics/intro_scene.h",
}


def load_manifest(path: Path) -> dict:
    manifest = graphics.load_manifest(
        path, output_root=OUTPUT_ROOT, custom_root=CUSTOM_ROOT,
        source_roots=("graphics/intro/", "graphics/title_screen/"),
        consumers=CONSUMERS, category="opening/title",
    )
    for asset in manifest["assets"]:
        if asset["group"] not in ("opening", "intro_credits", "title_screen"):
            raise ValueError(f"{asset['label']}: invalid opening/title group")
        if asset["transform"] == "gba_lz77_slice":
            raise ValueError(f"{asset['label']}: opening/title assets must be complete assets")
        if not asset["verification"].get("references"):
            raise ValueError(f"{asset['label']}: BPEJ reference evidence is missing")
    return manifest


def main(argv: list[str] | None = None) -> int:
    return graphics.run_extraction(
        argv, loader=load_manifest, output_root=OUTPUT_ROOT,
        default_manifest="tools/data/bpej_intro_manifest.json",
        category="opening/title", description=__doc__,
    )


if __name__ == "__main__":
    raise SystemExit(main())
