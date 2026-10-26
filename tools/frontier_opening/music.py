#!/usr/bin/env python3
"""Write the opening movie's score, sound/songs/midi/mus_frontier_opening.mid (stdlib only).

The movie (src/frontier_opening.c) is cut to this score: 100 BPM, so a beat is exactly
36 frames in the m4a driver and a 4/4 bar is 144 frames.

    bars 1-2   forest      F major, harp and a flute that wanders in
    bars 3-4   hands       the meeting; a glockenspiel on the touch, then a sting
    bars 5-6   network     D minor heartbeat, low strings, choir; a stab per trainer
    bars 7-9   guardians   Bb - C - D fanfare, one guardian per downbeat
    bar  10    release     a rise on A; the rolls stop a sixteenth early
    bar  11    white       the hit on D, a bright shimmer, and the chord fades under the white screen

Edit and audition it in porydaw (voicegroup `frontier_opening`); this script only
regenerates the starting point.
"""
import argparse
from dataclasses import dataclass, field
import hashlib
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[2]
NAME = 'mus_frontier_opening'
PPQN = 24
BEAT = PPQN
BAR = 4 * BEAT
BARS = 11
END = BARS * BAR
BPM = 100

# Voices in sound/voicegroups/frontier_opening.inc
STRINGS, HARP, FLUTE, GLOCK, HORN, TRUMPET, TUBA, TIMPANI, CHOIR, CRASH, BASS, SNARE = range(12)

NOTE = {'C': 0, 'D': 2, 'E': 4, 'F': 5, 'G': 7, 'A': 9, 'B': 11}


def n(name):
    """'F#4' -> MIDI number (C4 = 60)."""
    pitch = NOTE[name[0]]
    rest = name[1:]
    while rest and rest[0] in '#b':
        pitch += 1 if rest[0] == '#' else -1
        rest = rest[1:]
    return pitch + (int(rest) + 1) * 12


@dataclass
class Part:
    name: str
    voice: int
    pan: int
    volume: int
    notes: list = field(default_factory=list)
    programs: list = field(default_factory=list)   # (tick, voice)
    volumes: list = field(default_factory=list)    # (tick, CC7 value)

    def add(self, bar, beat, pitch, length, velocity):
        tick = bar * BAR + round(beat * BEAT)
        self.notes.append((tick, n(pitch) if isinstance(pitch, str) else pitch, round(length * BEAT), velocity))

    def seq(self, bar, beat, pitches, step, length, velocity):
        for i, pitch in enumerate(pitches):
            if pitch is not None:
                self.add(bar, beat + i * step, pitch, length, velocity if isinstance(velocity, int) else velocity[i])


def score():
    harp = Part('Harp', HARP, 44, 112)
    flute = Part('Flute', FLUTE, 84, 96)
    strings = Part('Strings', STRINGS, 64, 76)
    horn = Part('Horns', HORN, 52, 104)
    trumpet = Part('Trumpets', TRUMPET, 76, 108)
    timpani = Part('Timpani', TIMPANI, 64, 110)
    voices = Part('Glockenspiel / Choir', GLOCK, 92, 100)
    bass = Part('Bass', BASS, 64, 88)
    snare = Part('Snare', SNARE, 64, 96)
    crash = Part('Cymbal', CRASH, 64, 110)

    # Forest and hands: rolled harp chords, two notes to a beat.
    arpeggios = [
        (0, ['F3', 'A3', 'C4', 'E4'], ['E3', 'G3', 'C4', 'G4']),
        (1, ['D3', 'F3', 'A3', 'C4'], ['Bb2', 'D3', 'F3', 'A3']),
        (2, ['G2', 'Bb2', 'D3', 'F3'], ['C3', 'E3', 'G3', 'Bb3']),
    ]
    for bar, first, second in arpeggios:
        harp.seq(bar, 0, first + first[2:0:-1], 0.5, 0.5, [64, 48, 52, 48, 58, 46])
        harp.seq(bar, 2, second + second[2:0:-1], 0.5, 0.5, [60, 46, 50, 46, 56, 44])
    harp.seq(3, 0, ['A2', 'C3', 'F3', 'A3'], 0.5, 0.5, [62, 48, 52, 50])
    # The touch: a quick upward roll that stops in the air.
    harp.seq(3, 2, ['Bb2', 'F3', 'A3', 'C4', 'D4', 'F4', 'A4', 'C5'], 0.125, 0.25, 54)

    strings.add(0, 0, 'A4', 4, 48)
    strings.add(1, 0, 'F4', 2, 50)
    strings.add(1, 2, 'A4', 2, 50)
    strings.add(2, 0, 'G4', 2, 54)
    strings.add(2, 2, 'G4', 2, 54)
    strings.add(3, 0, 'A4', 3, 56)
    strings.add(3, 3, 'C#4', 1, 72)

    # The theme first appears small and unfinished on the flute.
    flute.add(1, 2, 'A4', 0.5, 70)
    flute.add(1, 2.5, 'C5', 0.5, 72)
    flute.add(1, 3, 'D5', 1, 78)
    flute.add(2, 0, 'E5', 1.5, 84)
    flute.add(2, 1.5, 'F5', 0.5, 76)
    flute.add(2, 2, 'E5', 1, 80)
    flute.add(2, 3, 'D5', 0.5, 72)
    flute.add(2, 3.5, 'C5', 0.5, 70)
    flute.add(3, 0, 'A4', 2, 76)

    voices.seq(3, 2, ['A5', 'C6', 'F6'], 1 / 3, 0.5, [70, 66, 74])

    bass.add(0, 0, 'F2', 2, 70)
    bass.add(0, 2, 'E2', 2, 66)
    bass.add(1, 0, 'D2', 2, 70)
    bass.add(1, 2, 'Bb1', 2, 66)
    bass.add(2, 0, 'G1', 2, 70)
    bass.add(2, 2, 'C2', 2, 68)
    bass.add(3, 0, 'A1', 2, 70)

    # The sting that turns the meeting into the infection.
    timpani.add(3, 3, 'A1', 0.5, 96)
    trumpet.add(3, 3, 'Bb4', 0.75, 84)

    # Network: a heartbeat on the timpani, circling low strings, a stab for each trainer.
    for bar in (4, 5):
        timpani.add(bar, 0, 'D2', 0.25, 104)
        timpani.add(bar, 0.5, 'D2', 0.25, 74)
        timpani.add(bar, 2, 'A1', 0.25, 98)
        timpani.add(bar, 2.5, 'A1', 0.25, 70)
    strings.seq(4, 0, ['D3', 'F3', 'E3', 'D3', 'C#3', 'D3', 'E3', 'F3'], 0.5, 0.45, 82)
    strings.seq(5, 0, ['D3', 'F3', 'E3', 'D3', 'C#3', 'E3', 'G3', 'Bb3'], 0.5, 0.45, [82, 82, 84, 84, 88, 92, 96, 100])
    voices.programs.append((4 * BAR, CHOIR))
    voices.add(4, 0, 'A4', 4, 70)
    voices.add(5, 0, 'Bb4', 2, 74)
    voices.add(5, 2, 'A4', 2, 78)
    trumpet.seq(4, 1, ['A4', 'Bb4', 'A4'], 1, 0.4, [86, 90, 94])
    trumpet.add(5, 0, 'Bb4', 0.75, 100)
    bass.seq(4, 0, ['D2', 'D2', None, 'D2', 'D2', None, 'D2', None], 0.5, 0.4, 84)
    bass.seq(5, 0, ['D2', 'D2', None, 'D2', 'C#2', 'C#2', 'C#2', 'C#2'], 0.5, 0.4, [84, 84, 0, 84, 88, 90, 94, 98])
    timpani.seq(5, 3, ['A1'] * 4, 0.25, 0.25, [70, 80, 92, 104])
    snare.seq(5, 3, [62] * 4, 0.25, 0.2, [60, 72, 86, 100])

    # Guardians: Bb - C - D with the theme on the horns, one arrival per downbeat.
    chords = [(6, 'Bb1', ['D4', 'F4', 'Bb4', 'F4'], 'D5'), (7, 'C2', ['E4', 'G4', 'C5', 'G4'], 'E5'),
              (8, 'D2', ['F#4', 'A4', 'D5', 'A4'], 'F#5')]
    for bar, root, arpeggio, stab in chords:
        strings.seq(bar, 0, arpeggio * 2, 0.5, 0.45, 76)
        bass.seq(bar, 0, [root] * 8, 0.5, 0.4, [100, 76, 84, 76, 92, 76, 84, 76])
        timpani.add(bar, 0, root, 0.5, 122)
        timpani.add(bar, 2, root, 0.25, 86)
        trumpet.add(bar, 0, stab, 0.75, 120)
        snare.seq(bar, 1, [62, None, 62, 62], 0.5, 0.2, [74, 0, 70, 60])
        snare.add(bar, 3.75, 62, 0.2, 66)
    crash.add(6, 0, 60, 3, 100)
    crash.add(7, 0, 60, 2, 84)
    crash.add(8, 0, 60, 3, 112)
    horn.add(6, 0, 'Bb3', 1, 104)
    horn.add(6, 1, 'F3', 0.5, 92)
    horn.add(6, 1.5, 'Bb3', 0.5, 96)
    horn.add(6, 2, 'C4', 1, 100)
    horn.add(6, 3, 'D4', 1, 104)
    horn.add(7, 0, 'E4', 2, 108)
    horn.add(7, 2, 'G4', 1, 104)
    horn.add(7, 3, 'F4', 0.5, 96)
    horn.add(7, 3.5, 'E4', 0.5, 96)
    horn.add(8, 0, 'F#4', 3, 112)
    horn.add(8, 3, 'A4', 1, 108)

    # Release: everything climbs on A, lands on D, and the shimmer takes over.
    strings.seq(9, 0, ['A3', 'B3', 'C#4', 'D4', 'E4', 'F#4', 'G4', 'G#4'], 0.5, 0.45,
                [76, 80, 84, 88, 92, 98, 104, 110])
    horn.add(9, 0, 'A4', 4, 96)
    horn.volumes += [(9 * BAR + i * 12, 70 + i * 5) for i in range(8)] + [(10 * BAR, 104)]
    trumpet.seq(9, 2, ['A4', 'A4', 'A4'], 0.5, 0.3, [84, 92, 100])
    # Rolls stop a sixteenth early: the breath before the hit.
    timpani.seq(9, 0, (['A1', 'E2'] * 8)[:15], 0.25, 0.25, [56 + i * 3 for i in range(15)])
    snare.seq(9, 0, [62] * 15, 0.25, 0.2, [48 + i * 3 for i in range(15)])
    bass.seq(9, 0, ['A1'] * 8, 0.5, 0.4, [80 + i * 3 for i in range(8)])

    strings.add(10, 0, 'D4', 4, 112)
    horn.add(10, 0, 'D5', 1, 118)
    trumpet.add(10, 0, 'A5', 1, 116)
    timpani.add(10, 0, 'D2', 0.5, 120)
    timpani.add(10, 0.5, 'A1', 0.5, 90)
    bass.add(10, 0, 'D2', 4, 110)
    snare.add(10, 0, 62, 0.3, 110)
    crash.add(10, 0, 60, 4, 120)
    harp.seq(10, 1, ['D4', 'F#4', 'A4', 'D5', 'F#5', 'A5', 'D6', 'F#6', 'A6', 'D6', 'F#6', 'A6'],
             0.25, 0.25, [50 + i * 3 for i in range(12)])
    voices.programs.append((10 * BAR, GLOCK))
    voices.seq(10, 1.5, ['D6', 'A6', 'F#6', 'D7'], 0.5, 0.5, [64, 70, 66, 74])
    # The screen is white from the second beat; the chord fades out under it before the title.
    for part, start in ((strings, 76), (bass, 88)):
        part.volumes += [(10 * BAR + 2 * BEAT + i * 3, max(0, start - i * 5)) for i in range(16)]

    return [harp, flute, strings, horn, trumpet, timpani, voices, bass, snare, crash]


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
        events = [(0, -5, meta(3, part.name.encode('ascii'))),
                  (0, -4, bytes([0xC0 | channel, part.voice])),
                  (0, -3, bytes([0xB0 | channel, 7, part.volume])),
                  (0, -2, bytes([0xB0 | channel, 10, part.pan]))]
        if channel == 0:
            events.append((0, -6, meta(0x51, round(60_000_000 / BPM).to_bytes(3, 'big'))))
        for tick, voice in part.programs:
            events.append((tick, -1, bytes([0xC0 | channel, voice])))
        for tick, value in part.volumes:
            events.append((tick, -1, bytes([0xB0 | channel, 7, value])))
        for tick, pitch, length, velocity in part.notes:
            events.append((tick, 1, bytes([0x90 | channel, pitch, velocity])))
            events.append((tick + length, 0, bytes([0x80 | channel, pitch, 0])))
        events.append((END, 3, meta(0x2F, b'')))
        track, previous = bytearray(), 0
        for tick, _, data in sorted(events, key=lambda e: (e[0], e[1])):
            track += vlq(tick - previous) + data
            previous = tick
        chunks.append(b'MTrk' + struct.pack('>I', len(track)) + track)
    return b'MThd' + struct.pack('>IHHH', 6, 1, len(parts), PPQN) + b''.join(chunks)


def validate(parts):
    assert len(parts) <= 10, 'the BGM player has 10 tracks'
    for part in parts:
        for tick, pitch, length, velocity in part.notes:
            assert 0 <= tick and tick + length <= END, (part.name, tick)
            assert 0 <= pitch <= 127 and 1 <= velocity <= 127, (part.name, pitch, velocity)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--out', type=Path, default=ROOT / 'sound/songs/midi' / (NAME + '.mid'))
    args = parser.parse_args()
    parts = score()
    validate(parts)
    data = midi(parts)
    args.out.write_bytes(data)
    print(f'{args.out}: {len(data)} bytes, {len(parts)} tracks, {END / BEAT * 60 / BPM:.2f} s, '
          f'sha256 {hashlib.sha256(data).hexdigest()[:16]}')


if __name__ == '__main__':
    main()
