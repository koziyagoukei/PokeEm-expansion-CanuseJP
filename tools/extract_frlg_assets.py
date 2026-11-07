#!/usr/bin/env python3
"""Make the assets the Emerald build extracts from baserom.gba out of a Japanese FireRed/LeafGreen ROM.

A FireRed/LeafGreen build needs no Emerald ROM: every output the BPEJ manifests describe (the same files at
the same build/extracted_* paths, so nothing that uses them changes) is searched for in baserom_fr.gba /
baserom_lg.gba by content and checked by SHA1:

1. the same ROM bytes (found through tools/data/frlg_asset_probes.json, then the manifest's SHA1);
2. for LZ77 data, a stream that decodes to the same bytes (it may be compressed differently);
3. otherwise, for the Pokedex and battle background assets, the repository file the BPEJ manifest names as
   their fallback (converted with gbagfx), and for the rest a blank asset of the same size and format: these
   are Hoenn-only screens and sounds (contests, PokeNav, the Emerald opening...), which FireRed and LeafGreen
   do not have.

A custom override (graphics/custom/bpej/...) is used as in the Emerald build. The stamp records where each
output came from; --check verifies the outputs against it.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import struct
import subprocess
import sys
import tempfile

import check_baserom_frlg as frlg_rom
import extract_bpej_contest as graphics
from extract_bpej_battle_backgrounds import decompress_lz77
from extract_bpej_pokedex import gba_lz77_stream_length
import extract_bpej_sound as sound


GRAPHICS_KINDS = {
    "contest": ("extract_bpej_contest", "build/extracted_contest"),
    "intro": ("extract_bpej_intro", "build/extracted_intro"),
    "pokenav": ("extract_bpej_pokenav", "build/extracted_pokenav"),
    "berry": ("extract_bpej_berry", "build/extracted_berry"),
}
KINDS = sorted([*GRAPHICS_KINDS, "sound", "pokedex", "battle_backgrounds"])
GBAGFX = "tools/gbagfx/gbagfx"
SILENT_TRACK = b"\xb1"  # FINE: a track that ends at once
SILENT_SAMPLE = struct.pack("<HHIII", 0, 0, 13379 << 10, 0, 16) + bytes(16)


def sha1(data: bytes) -> str:
    return hashlib.sha1(data).hexdigest()


def number(value) -> int:
    return int(value, 0) if isinstance(value, str) else int(value)


class Rom:
    """The FireRed/LeafGreen ROM, searched by content."""

    def __init__(self, data: bytes, probes: dict):
        self.data = data
        self.probes = probes
        self.length = len(data)
        self.lz77_streams = {}  # decoded size -> {decoded SHA1: stream}

    def find(self, digest: str, size: int) -> int | None:
        """Where the bytes with this SHA1 and size are, if anywhere."""
        probe = self.probes.get(digest)
        if probe is None or probe["size"] != size:
            return None
        key, offset = bytes.fromhex(probe["bytes"]), probe["offset"]
        position = self.data.find(key, offset)
        while position >= 0:
            start = position - offset
            if start + size <= self.length and sha1(self.data[start:start + size]) == digest:
                return start
            position = self.data.find(key, position + 1)
        return None

    def find_lz77(self, decoded_size: int, decoded_digest: str) -> bytes | None:
        """An LZ77 stream (any compression) that decodes to the bytes with this SHA1, as stored in the ROM."""
        if decoded_size not in self.lz77_streams:
            streams = {}
            header = b"\x10" + decoded_size.to_bytes(3, "little")
            bound = 4 + decoded_size * 9 // 8 + 8
            position = self.data.find(header)
            while position >= 0:
                if position % 4 == 0:
                    try:
                        window = self.data[position:position + bound]
                        length = gba_lz77_stream_length(window, "candidate")
                        decoded, _ = decompress_lz77(window[:length], "candidate")
                        streams.setdefault(sha1(decoded), window[:length])
                    except ValueError:
                        pass
                position = self.data.find(header, position + 1)
            self.lz77_streams[decoded_size] = streams
        return self.lz77_streams[decoded_size].get(decoded_digest)


def lz77_literal_stream(data: bytes) -> bytes:
    """A valid GBA LZ77 stream of data with no back-references."""
    out = bytearray(b"\x10" + len(data).to_bytes(3, "little"))
    for start in range(0, len(data), 8):
        out.append(0)
        out += data[start:start + 8]
    return pad4(bytes(out))


def pad4(data: bytes) -> bytes:
    return data + bytes(-len(data) % 4)


def gbagfx(source: Path, suffix: str) -> bytes:
    """Convert a repository asset with the project's gbagfx (png -> 4bpp, pal -> gbapal, X -> X.lz)."""
    with tempfile.TemporaryDirectory() as directory:
        current = Path(directory) / source.name
        current.write_bytes(source.read_bytes())
        steps = {".4bpp": [".4bpp"], ".gbapal": [".gbapal"], ".4bpp.lz": [".4bpp", ".4bpp.lz"],
                 ".gbapal.lz": [".gbapal", ".gbapal.lz"], ".bin.lz": [".bin.lz"]}[suffix]
        for step in steps:
            target = Path(directory) / (source.stem + step)
            if step.endswith(".lz"):
                target = Path(str(current) + ".lz")
            subprocess.run([GBAGFX, str(current), str(target)], check=True, capture_output=True)
            current = target
        return current.read_bytes()


# ---- the BPEJ graphics manifests (contest, opening/title, PokeNav, berries) ----------------------------------------

def graphics_output(rom: Rom, asset: dict) -> tuple[bytes, str]:
    custom = Path(asset["custom_path"])
    if custom.is_file():
        data = custom.read_bytes()
        graphics.validate_output(data, asset)
        return data, "custom"
    position = rom.find(asset["expected_source_hash"], number(asset["size"]))
    if position is not None:
        return graphics.rom_output(rom.data, dict(asset, rom_offset=position)), "rom"
    if asset["compression"] == "gba_lz77":
        slicing = asset["transform"] == "gba_lz77_slice"
        source_size = number(asset["source_decoded_size"]) if slicing else asset["decoded_size"]
        digest = asset["expected_source_decoded_hash"] if slicing else asset["expected_decoded_hash"]
        stream = rom.find_lz77(source_size, digest)
        if stream is not None:
            if asset["output_representation"] == "gba_lz77_stream":
                output = pad4(stream)
            else:
                output, _ = decompress_lz77(stream, asset["label"])
                if slicing:
                    start = number(asset["decoded_slice_offset"])
                    output = output[start:start + asset["decoded_size"]]
            graphics.validate_output(output, asset)
            return output, "rom (recompressed)"
    blank = bytes(asset["decoded_size"])
    output = lz77_literal_stream(blank) if asset["output_representation"] == "gba_lz77_stream" else blank
    graphics.validate_output(output, asset)
    return output, "blank"


def graphics_assets(kind: str, manifest_path: Path) -> list[tuple[str, callable]]:
    module = __import__(GRAPHICS_KINDS[kind][0])
    manifest = module.load_manifest(manifest_path)
    return [(asset["output_path"], lambda rom, asset=asset: graphics_output(rom, asset)) for asset in manifest["assets"]]


# ---- sound ------------------------------------------------------------------------------------------------------------

def sound_assets(manifest_path: Path) -> list[tuple[str, callable]]:
    manifest = sound.load_manifest(manifest_path)
    items = []
    for song in manifest["songs"]:
        def tracks(rom, song=song):
            found = [rom.find(track["sha1"], number(track["size"])) for track in song["tracks"]]
            if all(position is not None for position in found):
                return [rom.data[p:p + number(t["size"])] for p, t in zip(found, song["tracks"])], "rom"
            return [SILENT_TRACK for _ in song["tracks"]], "silent"
        cache = {}

        def track_output(rom, index, tracks=tracks, cache=cache):
            if "result" not in cache:
                cache["result"] = tracks(rom)
            data, origin = cache["result"]
            return data[index], origin

        for index, track in enumerate(song["tracks"]):
            items.append((track["output_path"], lambda rom, index=index, f=track_output: f(rom, index)))
        items.append((song["output_path"], lambda rom, song=song, f=track_output:
                      (sound.song_wrapper(song).encode("utf-8"), f(rom, 0)[1] if song["tracks"] else "rom")))
    for blob in manifest["blobs"]:
        def blob_output(rom, blob=blob):
            position = rom.find(blob["sha1"], number(blob["size"]))
            if position is None:
                return SILENT_SAMPLE, "silent"
            return rom.data[position:position + number(blob["size"])], "rom"
        items.append((blob["output_path"], blob_output))
    return items


# ---- the generated Pokedex and battle background manifests (with repository fallbacks) -------------------------------

def fallback_output(asset: dict, size: int | None) -> bytes:
    """The repository asset the BPEJ manifest names as this output's fallback, in the output's format."""
    suffix = next(s for s in (".4bpp.lz", ".bin.lz", ".gbapal.lz", ".4bpp", ".gbapal", ".bin")
                  if asset["output_path"].endswith(s))
    source = Path(asset["fallback_file"])
    data = source.read_bytes() if suffix == ".bin" else gbagfx(source, suffix)
    if size is not None:
        data = data[:size] + bytes(max(0, size - len(data)))
    return data


def pokedex_assets(manifest_path: Path) -> list[tuple[str, callable]]:
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))

    def output(rom, asset):
        size = number(asset["size"])
        position = rom.find(asset["expected_hash"], size)
        if position is not None:
            return rom.data[position:position + size], "rom"
        return fallback_output(asset, size), "fallback"
    return [(asset["output_path"], lambda rom, asset=asset: output(rom, asset)) for asset in manifest["assets"]]


def battle_background_assets(manifest_path: Path) -> list[tuple[str, callable]]:
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))

    def output(rom, asset):
        custom = Path(asset["custom_path"])
        if custom.is_file():
            return custom.read_bytes(), "custom"
        size = number(asset["size"])
        position = rom.find(asset["expected_source_hash"], size)
        if position is not None:
            source = rom.data[position:position + size]
            if asset["output_representation"] == "raw":
                return decompress_lz77(source, asset["symbol_id"])[0], "rom"
            return source, "rom"
        if asset["output_representation"] == "raw":
            return fallback_output(asset, number(asset["decompressed_size"])), "fallback"
        return pad4(fallback_output(asset, None)), "fallback"
    return [(asset["output_path"], lambda rom, asset=asset: output(rom, asset)) for asset in manifest["assets"]]


# ---- driver -----------------------------------------------------------------------------------------------------------

def load_items(kind: str, manifest: Path) -> list[tuple[str, callable]]:
    if kind in GRAPHICS_KINDS:
        return graphics_assets(kind, manifest)
    if kind == "sound":
        return sound_assets(manifest)
    if kind == "pokedex":
        return pokedex_assets(manifest)
    return battle_background_assets(manifest)


def output_file(out_root: Path | None, path: str) -> Path:
    if out_root is None:
        return Path(path)
    relative = PurePosixPath(path)
    return out_root.joinpath(*relative.parts[2:]) if relative.parts[:1] == ("build",) else out_root / relative


def state_of(outputs: list[Path]) -> str:
    return sha1("\n".join(f"{path.as_posix()}={sha1(path.read_bytes())}" for path in outputs).encode("utf-8"))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--kind", required=True, choices=KINDS)
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--baserom", default="", help="baserom_fr.gba or baserom_lg.gba")
    parser.add_argument("--probes", type=Path, default=Path("tools/data/frlg_asset_probes.json"))
    parser.add_argument("--out", type=Path, help="Output directory (default: the manifest's build/extracted_* paths)")
    parser.add_argument("--stamp", type=Path)
    parser.add_argument("--check", action="store_true", help="Only verify the outputs against the stamp")
    args = parser.parse_args()

    try:
        items = load_items(args.kind, args.manifest)
        outputs = [output_file(args.out, path) for path, _ in items]
        if args.check:
            missing = [path for path in outputs if not path.is_file()]
            if missing:
                raise ValueError("missing outputs: " + ", ".join(path.as_posix() for path in missing[:5]))
            if args.stamp:
                stamp = json.loads(args.stamp.read_text(encoding="utf-8")) if args.stamp.is_file() else {}
                if stamp.get("output_state_sha1") != state_of(outputs):
                    raise ValueError(f"{args.stamp}: missing or stale extraction stamp")
            return 0

        if not args.baserom or not Path(args.baserom).is_file():
            raise ValueError("neither baserom_fr.gba nor baserom_lg.gba was found")
        data = Path(args.baserom).read_bytes()
        _, _, rom_name = frlg_rom.identify(data)
        probes = json.loads(args.probes.read_text(encoding="utf-8"))["probes"]
        rom = Rom(data, probes)
        origins = {}
        for (path, make), target in zip(items, outputs):
            output, origin = make(rom)
            origins[origin] = origins.get(origin, 0) + 1
            graphics.write_if_changed(target, output)
        if args.stamp:
            stamp = {"verified": 1, "rom": rom_name, "baserom_sha1": sha1(data), "kind": args.kind,
                     "manifest_sha1": sha1(args.manifest.read_bytes()), "file_count": len(outputs),
                     "origins": dict(sorted(origins.items())), "output_state_sha1": state_of(outputs)}
            graphics.write_if_changed(args.stamp, (json.dumps(stamp, indent=2) + "\n").encode("utf-8"))
            args.stamp.touch()  # newer than its inputs even when nothing changed, so make does not redo it
        summary = ", ".join(f"{count} {origin}" for origin, count in sorted(origins.items()))
        print(f"Made {len(outputs)} {args.kind} assets from {Path(args.baserom).name} ({summary}).")
    except (OSError, ValueError, KeyError, subprocess.CalledProcessError) as error:
        print(f"error: {frlg_rom.REQUIRED_MESSAGE}\ndetail: {args.kind}: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
