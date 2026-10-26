#!/usr/bin/env python3
"""Author a three-minute winter-inn title score; no external Python dependencies.

Original melody, with the gentle piano/bells/strings and snowy-town mood requested
by the user. This is not a transcription or imported recording of Snowpoint City.
Seventy-two 3/4 bars at 72 BPM give 180 seconds, then FINE (not an infinite loop).
"""
import argparse
from dataclasses import dataclass, field
import hashlib
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[2]
NAME = 'mus_frontier_title'
PPQN, BEATS, BARS, BPM = 24, 3, 72, 72
BAR = PPQN * BEATS
END = BAR * BARS
PIANO, HARP, STRINGS, FLUTE, BELL, BASS = range(6)


@dataclass(frozen=True)
class Note:
    tick: int
    pitch: int
    duration: int
    velocity: int


@dataclass
class Part:
    name: str
    voice: int
    pan: int
    volume: int
    notes: list = field(default_factory=list)
    programs: list = field(default_factory=list)
    volumes: list = field(default_factory=list)

    def add(self, bar, beat, pitch, length, velocity):
        if pitch is not None:
            self.notes.append(Note(bar * BAR + round(beat * PPQN), pitch,
                                   round(length * PPQN), velocity))


A = [
    [(74, 1), (78, .5), (76, .5), (69, 1)],
    [(71, .5), (74, .5), (76, 1), (78, 1)],
    [(76, 1), (73, 1), (69, 1)],
    [(66, 1), (69, .5), (71, .5), (74, 1)],
    [(67, 1), (71, .5), (74, .5), (76, 1)],
    [(78, 1.5), (76, .5), (74, 1)],
    [(73, .5), (71, .5), (69, 1), (76, 1)],
    [(74, 2.5), (None, .5)],
]
B = [
    [(76, 1), (79, .5), (78, .5), (76, 1)],
    [(74, .5), (71, .5), (74, 1), (78, 1)],
    [(81, 1.5), (79, .5), (78, 1)],
    [(76, 1), (74, 1), (73, 1)],
    [(71, 1), (74, 1), (78, .5), (79, .5)],
    [(78, 1), (76, .5), (74, .5), (71, 1)],
    [(73, 1), (76, 1), (69, 1)],
    [(73, 2), (None, 1)],
]
CHORDS_A = [(50, 54, 57, 61), (47, 50, 54, 57), (43, 47, 50, 54), (45, 49, 52, 57),
            (43, 47, 50, 54), (42, 45, 50, 54), (40, 43, 47, 50), (45, 49, 52, 55)]
CHORDS_B = [(40, 43, 47, 50), (43, 47, 50, 54), (42, 45, 49, 52), (47, 50, 54, 57),
            (43, 47, 50, 54), (42, 45, 50, 54), (40, 43, 47, 50), (45, 49, 52, 55)]


def phrase(part, start, melody, velocity, octave=0):
    for bar, notes in enumerate(melody):
        beat = 0
        for pitch, length in notes:
            part.add(start + bar, beat, None if pitch is None else pitch + octave,
                     length * .90, velocity - (3 if beat % 1 else 0))
            beat += length


def score():
    lead = Part('01 Winter window - piano', PIANO, 68, 96)
    harp = Part('02 Hearth - harp', HARP, 35, 78)
    strings = Part('03 Remembered warmth - strings', STRINGS, 57, 54)
    flute = Part('04 Answer across the snow - flute', FLUTE, 90, 62)
    bell = Part('05 Snowlight - glockenspiel', BELL, 99, 64)
    bass = Part('06 Quiet footsteps - pizzicato', BASS, 58, 70)
    inner = Part('07 Inner voice - piano', PIANO, 43, 58)
    parts = [lead, harp, strings, flute, bell, bass, inner]

    # Eight-bar introduction, A/A, B/B, quiet bridge, A/A', eight-bar farewell.
    for bar in range(BARS):
        progression = CHORDS_B if 24 <= bar < 48 else CHORDS_A
        root, third, fifth, seventh = progression[bar % 8]
        quiet = bar < 8 or 40 <= bar < 48 or bar >= 64
        strength = 49 if quiet else (61 if bar >= 48 else 55)
        for step, pitch in enumerate([root, fifth, third + 12, seventh + 12, fifth, third + 12]):
            harp.add(bar, step * .5, pitch, .43, strength - (step % 3) * 4)
        bass.add(bar, 0, root - 12, 1.8, 57 if quiet else 64)
        if not quiet:
            bass.add(bar, 2, fifth - 12, .7, 45)
        if 4 <= bar < 70:
            strings.add(bar, 0, third + 12, 2.8, 44 if quiet else 51)
        if 16 <= bar < 64 and not 40 <= bar < 48:
            inner.add(bar, 1, fifth + 12, .75, 48)
            inner.add(bar, 2, seventh + 12, .7, 44)
        if bar >= 8 and bar % 4 == 0:
            bell.add(bar, .5, fifth + 24, 1.2, 51)
        if bar >= 8 and bar % 4 == 2:
            bell.add(bar, 2, third + 24, .75, 43)

    for bar, pitch in [(0, 81), (2, 78), (4, 79), (6, 76)]:
        bell.add(bar, 0, pitch, 1.6, 53)
    phrase(lead, 4, A[:4], 54, -12)
    phrase(lead, 8, A, 76)
    phrase(lead, 16, A, 80)
    lead.programs += [(24 * BAR, FLUTE), (40 * BAR, PIANO)]
    phrase(lead, 24, B, 73)
    phrase(lead, 32, B, 77)
    phrase(lead, 40, [[(71, 2), (None, 1)], [(69, 1), (66, 2)], [(67, 2.5), (None, .5)],
                      [(64, 1), (66, 1), (69, 1)], [(71, 2), (74, 1)], [(73, 2), (None, 1)],
                      [(76, 1), (73, 1), (69, 1)], [(None, 1), (69, .5), (73, .5), (76, 1)]], 62)
    phrase(lead, 48, A, 82)
    phrase(lead, 56, A, 74)
    phrase(lead, 64, [[(78, 1.5), (76, .5), (74, 1)], [(71, 2), (69, 1)],
                      [(67, 1), (71, 1), (74, 1)], [(73, 2), (None, 1)],
                      [(74, 1), (69, 1), (66, 1)], [(64, 1), (66, 1), (69, 1)],
                      [(74, 2.5), (None, .5)], [(62, 2.75), (None, .25)]], 59)

    # Responses leave rests; each track stays monophonic for the small GBA mixer.
    for start in (16, 48, 56):
        for offset, pitch in [(1, 66), (3, 69), (5, 71), (7, 73)]:
            flute.add(start + offset, .25, pitch, 1.25, 48)
            flute.add(start + offset, 2, pitch + 2, .7, 43)
    for bar in (51, 55, 59, 63):
        bell.add(bar, .5, 86 if bar != 63 else 81, 1.5, 45)
    strings.add(70, 0, 66, 5.8, 43)
    inner.add(71, 0, 69, 2.75, 40)
    for part in parts:
        for step in range(12):
            part.volumes.append((68 * BAR + step * PPQN, round(part.volume * (11 - step) / 11)))
    return parts


def vlq(value):
    data = [value & 127]
    value >>= 7
    while value:
        data.insert(0, (value & 127) | 128)
        value >>= 7
    return bytes(data)


def meta(kind, data):
    return bytes([255, kind]) + vlq(len(data)) + data


def midi(parts):
    chunks = []
    for channel, part in enumerate(parts):
        events = [(0, -4, meta(3, part.name.encode('ascii'))),
                  (0, -3, bytes([0xC0 | channel, part.voice])),
                  (0, -2, bytes([0xB0 | channel, 7, part.volume])),
                  (0, -1, bytes([0xB0 | channel, 10, part.pan]))]
        if channel == 0:
            events += [(0, -6, meta(0x51, round(60_000_000 / BPM).to_bytes(3, 'big'))),
                       (0, -5, meta(0x58, bytes([3, 2, 24, 8])))]
        for tick, voice in part.programs:
            events.append((tick, -1, bytes([0xC0 | channel, voice])))
        for tick, volume in part.volumes:
            events.append((tick, -1, bytes([0xB0 | channel, 7, volume])))
        for note in part.notes:
            events.append((note.tick, 1, bytes([0x90 | channel, note.pitch, note.velocity])))
            events.append((note.tick + note.duration, 0, bytes([0x80 | channel, note.pitch, 0])))
        events.append((END, 3, meta(0x2F, b'')))
        track, previous = bytearray(), 0
        for tick, _, data in sorted(events, key=lambda e: (e[0], e[1])):
            track += vlq(tick - previous) + data
            previous = tick
        chunks.append(b'MTrk' + struct.pack('>I', len(track)) + track)
    return b'MThd' + struct.pack('>IHHH', 6, 1, len(parts), PPQN) + b''.join(chunks)


def validate(parts):
    assert len(parts) == 7
    for part in parts:
        previous_end = 0
        for note in sorted(part.notes, key=lambda n: n.tick):
            assert 0 <= note.tick < note.tick + note.duration <= END
            assert 0 <= note.pitch <= 127 and 1 <= note.velocity <= 127
            assert note.tick >= previous_end, ('overlapping notes', part.name, note.tick)
            previous_end = note.tick + note.duration


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, default=ROOT)
    args = parser.parse_args()
    parts = score()
    validate(parts)
    data = midi(parts)
    target = args.out / 'sound/songs/midi' / (NAME + '.mid')
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_bytes(data)
    report = {'title': 'An Old Hand, a Quiet Table', 'symbol': NAME, 'bpm': BPM,
              'timeSignature': '3/4', 'bars': BARS, 'ppqn': PPQN, 'ticks': END,
              'seconds': END / PPQN * 60 / BPM, 'loop': False,
              'midiSha256': hashlib.sha256(data).hexdigest(),
              'sections': [{'bar': b, 'name': n} for b, n in
                           [(0, 'Introduction'), (8, 'A'), (24, 'B'), (40, 'Bridge'),
                            (48, 'A reprise'), (64, 'Coda')]],
              'parts': [{'name': p.name, 'voice': p.voice, 'notes': len(p.notes)} for p in parts],
              'source': 'Original melody; winter-town piano/bell/strings mood, not a game-song transcription'}
    manifest = args.out / 'tools/frontier_title/score.json'
    manifest.parent.mkdir(parents=True, exist_ok=True)
    manifest.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{target}: {len(data)} bytes, {len(parts)} tracks, {report["seconds"]:.2f} seconds')


if __name__ == '__main__':
    main()
