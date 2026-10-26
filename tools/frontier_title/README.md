# Title Score: An Old Hand, a Quiet Table

Song ID: `MUS_FRONTIER_TITLE` (612). Symbol: `mus_frontier_title`.

The user requested roughly three minutes and the feeling of a Diamond/Pearl
Snowpoint City arrangement. This score uses an original melody and harmony,
not a transcription, sampled recording or imported game MIDI. The reference
is interpreted as a quiet snowy-town mood: piano, glockenspiel, harp, soft
strings, flute responses and restrained pizzicato bass. There is no drum beat.

- 3/4, 72 BPM, 72 bars, 5,184 MIDI ticks at 24 PPQN: nominally 180 seconds.
- Eight-bar introduction, two A statements, contrasting flute-led B section,
  quiet bridge, piano reprise, and an eight-bar farewell.
- Seven monophonic tracks fit within the existing ten-track BGM player.
- The last four bars reduce volume gently. Every track ends with FINE; there
  are no infinite loop markers or network/save-dependent playback decisions.
- Once the finite song finishes, the existing title callback returns to the
  existing opening replay. A/START can leave for the menu before then.

`music.py` (Python standard library only) regenerates the editable
`sound/songs/midi/mus_frontier_title.mid` and `score.json`:

```sh
python3 tools/frontier_title/music.py
python3 -m unittest discover -s tools/frontier_title -p test_title.py -v
```

`sound/voicegroups/frontier_title.inc` reuses the project's existing sample
instruments. The build uses the ordinary `mid2agb` route and the configuration
in `sound/songs/midi/midi.cfg`. Generated assembly is not edited by hand. MIDI
program numbers refer to this dedicated voicegroup, not General MIDI, so a
desktop GM player's sound is not an accurate preview of the game version.

The song is appended; existing song/phoneme IDs, the original `MUS_TITLE`, the
opening's `MUS_FRONTIER_OPENING`, and the battle-music shop remain unchanged.
Only the Emerald title screen selects the new song. No save or communication
layout/version changes are needed.

Automated checks cover score duration, reproducibility, finite track endings,
monophony, section changes, fade-out, background memory/palette boundaries,
and real title-to-menu key handling. The original opening extraction tests
continue to verify retained original assets, without requiring unused title
clouds to remain linked after their display path has been replaced.

Live title playback uses the specified `mGBA-celio-rom128-4` build and an
isolated ROM/save copy. Audio preview hooks exist only in ignored QA files:
they pause the existing sound player for a calibration gap, then restart this
same title song. They do not patch production ROM or save data. Recording the
actual GBA sound driver distinguishes the preview from a generic MIDI synth.

## Verified on 2026-10-26

- Normal build passed; seven Python checks, three title C tests, fourteen
  existing opening-extraction tests and the Japanese-text check passed.
- The specified mGBA displayed the still life without hiding the food/hand
  behind the title labels; the original main menu opened with A.
- The complete score ended after 10,802 virtual frames (about 180.85 seconds
  at GBA refresh rate), then the original idle-title replay callback ran.
- `build/qa_frontier_title_20261026/frontier_title_preview.wav` is a 22,050 Hz
  stereo recording of that actual driver output. Calibration/extra recording
  time is removed; no additional gain or fade is applied. Measured peak is
  5,088/32,767 with zero clipped samples.
- The tested ROM copy and final `pokeemerald.gba` have identical SHA-256:
  `c2377509686f63d9c3d84338b1f623bef9cb01d70cb808b069ddcd40bac66751`.
- The original save and the user's edited opening C/intro/score-generator
  files retained their task-start hashes. No commit or push was made.
- Real-hardware display/speaker balance and subjective listening feedback
  remain separate checks, not implied by automated playback verification.
