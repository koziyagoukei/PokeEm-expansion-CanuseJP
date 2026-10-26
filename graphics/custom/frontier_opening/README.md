# Frontier Opening Movie

Opening-only backgrounds and encounter artwork made with the built-in image
generation/editing tool. The movie is implemented in `src/frontier_opening.c`.
`Task_Scene1_End` in `src/intro.c` enters it after the unchanged Game Freak,
plants and silhouette scene. The existing title and main menu are retained.
Prompt records are in `PROMPTS.md`.

| Source | Use |
| --- | --- |
| `source/forest_generated.png` | Quiet forest background |
| `source/meeting_hands_generated.png` | Close-up first encounter; a small detached floating purple mitten hand, not a connected tentacle |
| `source/network_generated.png` | Dark forest background with peripheral paired vines |
| `source/ridge_generated.png` | Empty rocky ridge for the three guardian sprites |

The rejected long-tentacle encounter image is not an adopted project asset.

## Runtime

The movie is cut to its own song, `MUS_FRONTIER_OPENING` (100 BPM, so a beat is
exactly 36 frames and a bar 144). `MUS_INTRO` fades out with the first scene's
white fade, and the new song starts with the forest.

| Phase | Bars | Frames | Content |
| --- | --- | ---: | --- |
| Forest | 1-2 | 288 | White-to-forest fade, slow camera drift, fireflies |
| Hands | 3-4 | 288 | Camera push to the hands; a light on the glockenspiel triplet, snuffed on the sting, then a dark purple fade |
| Network | 5-6 | 288 | Pokevirus pulsing on the timpani heartbeat; four trainer silhouettes, each chained and recolored on a trumpet stab |
| Guardians | 7-9 | 432 | Pokevirus above; Darkrai, Zygarde 50% and Rayquaza back sprites rise along the bottom, one per downbeat, with flash, shake and cry |
| Attack | 10, beat 1 of 11 | 180 | Scenery dims while light gathers on the guardians; the rolls stop and three lights fly to the Pokevirus; the hit turns the screen white |
| White | rest of 11 | 108 | The chord fades under white, then the existing title callback |

That is 1,584 frames (26.4 seconds), the length of the song. The movie follows
the song's clock: `gMPlayInfo_BGM.clock` advances in the VBlank interrupt, so
after a slow frame the movie runs up to four timeline steps at once and no cut
falls behind its beat. Without the song (music disabled) it simply counts frames.

The four backgrounds use GBA mode 4: 240x160 palette indices, at most 64 colors,
with BG2 affine camera movement. The camera clamps its center so the bitmap's
edges never show, including under shake. Each `.pixels.bin` is 38,400 bytes and
each `.palette.bin` is a 256-entry BGR555 palette (512 bytes). The forest and the
hands share one clearing, so the hands are drawn on the second mode-4 page while
the forest plays; the page flip and the palette change happen in the same VBlank.
OBJ tiles 0..511 are reserved because bitmap modes cannot use them for sprites.

Fades and flashes use the blend unit (BLDY), not the palette fader, so a key can
skip the movie at any moment. The light sprites are semi-transparent (BLDALPHA)
and add to the scenery; the attack's dimming darkens the scenery only.

Effect sprites come from `tools/frontier_opening/sprites.py` (Pillow):

| File | Size | Use |
| --- | --- | --- |
| `firefly.png` | 16x8, two 8x8 frames | Fireflies, Pokevirus spores and gathering light (palette swapped in code) |
| `glow.png` | 32x32 | The light between the hands, the guardians' lights and the final burst |
| `bead.png` | 8x8 | Links of the chains in the network scene |

The guardians are the battle back sprites (`graphics/pokemon/*/back.png` with
`normal.pal`). The trainers keep normal affine bounds when shrunk, which keeps
the distant ones from wrapping above the screen.

Normal completion, allocation failure and key skips reach the existing title.
Sprite sheets, palettes, affine matrices, queued copies and the movie task are
released. Idle-title replay uses the same original entry path. Battle logic,
new-game story flow, communication and save layouts are not changed.

## Music

`sound/songs/midi/mus_frontier_opening.mid` uses `sound/voicegroups/frontier_opening.inc`
(SC-88 Pro harp, flute, glockenspiel and timpani, the strings, horn, trumpet and
tuba keysplits, choir, crash, a programmable-wave bass and a noise snare).
`tools/frontier_opening/music.py` writes the starting score; it documents which
bar belongs to which scene. Open the MIDI in porydaw with the voicegroup
`frontier_opening` to audition or edit it. Keep the tempo at 100 BPM and the
length at 11 bars, or update `src/frontier_opening.c` with it.

## Asset Regeneration

The `source/` PNGs are editable originals. Native-size PNGs and `_4x.png` files
are previews, not runtime dependencies. `validation.json` records dimensions,
palette counts and framebuffer SHA-256 values.

Use Node.js with `pngjs` available:

```sh
node --input-type=module -e "import {PNG} from 'pngjs'; import {prepareAssets} from './tools/frontier_opening/assets.mjs'; prepareAssets(PNG, process.cwd());"
```

The converter uses nearest-neighbor sampling and weighted palette quantization
to the GBA's 5-bit channels. Generated binaries are checked in, so normal builds
do not require Node.js or pngjs.

## Verification

```sh
python3 tools/frontier_opening/sprites.py   # only after editing the effect sprites
python3 tools/frontier_opening/music.py     # only to regenerate the starting score
python3 tools/frontier_opening/test_assets.py
make -j$(nproc) check TESTS=test/frontier_opening.c
python3 tools/check_jp_text.py
make -j$(nproc)
```

Automated coverage includes complete framebuffers, palette limits, effect sprite
formats, the movie and song sharing one length and tempo, unchanged save access
boundaries, the guardian lineup (Pokevirus above, three back sprites on the
bottom edge), bitmap-safe OBJ allocation, compact trainer bounds, six-phase
completion, key-triggered skips at any moment with cleanup, and repeated playback.

Headless mGBA captures were checked around every cut and against the song's
clock (within one frame). The score was rendered with porydaw's emulation of the
m4a mixer. Neither substitutes for real-hardware testing or for listening to the
in-game mix, where the guardians' cries share the direct-sound channels.
