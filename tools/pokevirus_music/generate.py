#!/usr/bin/env python3
"""Author the release-battle score and its original heartbeat sample (stdlib only)."""
import argparse
from dataclasses import dataclass
import hashlib
import json
import math
from pathlib import Path
import random
import struct
import wave

ROOT = Path(__file__).resolve().parents[2]
PPQN = 24
BAR = 96
INTRO = 2 * BAR
LOOP_BARS = 32
END = INTRO + LOOP_BARS * BAR
NAME = 'mus_pokevirus_release'
TEMPOS = [(0, 80), (96, 108), (144, 132), (INTRO, 144)]


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
    notes: list


def add(part, tick, pitch, duration, velocity=80):
    part.notes.append(Note(tick, pitch, duration, velocity))


def score():
    parts = [
        Part('01 Restarting heart', 0, 64, 102, []),
        Part('02 Pulse bass', 1, 59, 77, []),
        Part('03 Liberation lead', 7, 69, 66, []),
        Part('04 Unravelling strings', 2, 45, 52, []),
        Part('05 Fractured arpeggio', 5, 87, 58, []),
        Part('06 Trainer memories', 4, 57, 83, []),
        Part('07 Battle percussion', 6, 64, 60, []),
        Part('08 Distant echoes', 8, 91, 46, []),
    ]
    # No sound before the first returning lub-dub; each pair gets closer and stronger.
    for t, strength in [(24, 61), (78, 75), (120, 88), (162, 100)]:
        add(parts[0], t, 60, 6, strength)
        add(parts[0], t + (7 if t == 24 else 6), 60, 5, strength - 22)
    add(parts[7], 168, 79, 18, 34)
    # C minor -> borrowed major memories -> a firmer but unresolved return to C minor.
    progression = [
        (48, 51, 55), (48, 51, 55), (44, 48, 51), (43, 47, 50),
        (41, 44, 48), (44, 48, 51), (46, 50, 53), (43, 47, 50),
        (48, 52, 55), (48, 52, 55), (48, 51, 55), (46, 50, 53),
        (44, 48, 51), (41, 44, 48), (50, 53, 56), (43, 47, 50),
        (48, 51, 55), (44, 48, 51), (48, 51, 55), (43, 47, 50),
        (44, 48, 51), (46, 50, 53), (48, 51, 55), (43, 47, 50),
        (41, 44, 48), (44, 48, 51), (46, 50, 53), (43, 47, 50),
        (48, 52, 55), (48, 52, 55), (49, 52, 55), (43, 47, 50),
    ]
    memory_bars = {8, 9, 18, 19, 28, 29}
    themes = [
        [72, 74, 75, 79, 77, 75, 74, 71],
        [72, 79, 82, 79, 77, 75, 74, 72],
        [80, 79, 75, 72, 75, 77, 79, 82],
        [79, 77, 74, 71, 74, 77, 80, 83],
    ]
    for bar, chord in enumerate(progression):
        t = INTRO + bar * BAR
        root, third, fifth = chord
        memory = bar in memory_bars
        # The heart remains in the loop as a quieter two-part pulse, not a fresh intro.
        for beat in (0, 48):
            add(parts[0], t + beat, 60, 5, 71 if not memory else 53)
            add(parts[0], t + beat + 7, 60, 4, 45 if not memory else 34)
        for off, pitch in [(0, root - 12), (18, root - 12), (36, fifth - 12),
                           (48, root - 12), (66, root), (84, fifth - 12)]:
            add(parts[1], t + off, pitch, 9, 82 if off in (0, 48) else 66)
        # Single-note string lines leave the small GBA mixer headroom for cries and SEs.
        add(parts[3], t, third + 12, 44, 60 if memory else 70)
        add(parts[3], t + 48, fifth + 12, 44, 55 if memory else 64)
        arp = [root + 12, fifth + 12, third + 24, fifth + 12]
        for i in range(8):
            add(parts[4], t + i * 12, arp[i % 4], 5, 42 if memory else 57 + (i % 2) * 7)
        if not memory:
            theme = themes[(bar // 4) % len(themes)]
            # Sparse opening phrases become assertive after the second memory.
            for i, pitch in enumerate(theme):
                if bar < 4 and i in (3, 7):
                    continue
                add(parts[2], t + i * 12, pitch, 9, 75 + (9 if bar >= 20 else 0))
            if bar % 4 == 2:
                add(parts[7], t + 30, fifth + 24, 12, 44)
                add(parts[7], t + 66, third + 24, 12, 35)
        # A light snare/closed-hat groove; no extra kick competing with the heart.
        for off in range(0, BAR, 12):
            add(parts[6], t + off, 42, 2, 33 if off % 24 else 48)
        for off in (24, 72):
            add(parts[6], t + off, 38, 3, 55 if memory else 72)
        if bar % 8 == 7:
            for i in range(4):
                add(parts[6], t + 72 + i * 6, 38, 3, 45 + i * 8)
    # Short, recognizable quotations from mus_vs_trainer, not a copy of its full song.
    # Source track 1, bars 010-011: C-G-F / E-F-E-D-C-E-Bb-C, original rhythm.
    main_memory = [(0, 60, 33), (36, 67, 33), (72, 65, 21)] + [
        (96 + i * 12, pitch, 9) for i, pitch in enumerate([64, 65, 64, 62, 60, 64, 70, 72])]
    for bar, strength in [(8, 75), (28, 88)]:
        t = INTRO + bar * BAR
        for off, pitch, length in main_memory:
            add(parts[5], t + off, pitch + 12, length, strength)
            if off + 12 + length <= BAR * 2:
                add(parts[7], t + off + 12, pitch + 12, min(length, 9), strength // 2)
    # A broken recollection of the original chromatic piano opening (bar 000).
    opening = [84, 83, 82, 81, 82, 81, 80, 79]
    t = INTRO + 18 * BAR
    for i, pitch in enumerate(opening):
        add(parts[5], t + i * 6, pitch - 12, 5, 63)
        add(parts[7], t + 12 + i * 6, pitch - 12, 4, 34)
    for off, pitch in [(72, 67), (108, 70), (144, 68)]:
        add(parts[5], t + off, pitch, 12, 56)
    return parts


def vlq(value):
    assert 0 <= value <= 0x0FFFFFFF
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
                  (0, -2, bytes([0xB0 | channel, 10, part.pan])),
                  (INTRO, -1, meta(6, b'[')), (END, 2, meta(6, b']'))]
        if channel == 0:
            for tick, bpm in TEMPOS:
                events.append((tick, -6, meta(0x51, round(60_000_000 / bpm).to_bytes(3, 'big'))))
        if channel == 5:
            events.extend([
                (INTRO + 18 * BAR, -4, bytes([0xC0 | channel, 5])),
                (INTRO + 20 * BAR, -4, bytes([0xC0 | channel, 4])),
            ])
        for n in part.notes:
            events.append((n.tick, 1, bytes([0x90 | channel, n.pitch, n.velocity])))
            events.append((n.tick + n.duration, 0, bytes([0x80 | channel, n.pitch, 0])))
        events.append((END, 3, meta(0x2F, b'')))
        track, previous = bytearray(), 0
        for tick, priority, data in sorted(events):
            track.extend(vlq(tick - previous) + data)
            previous = tick
        chunks.append(b'MTrk' + struct.pack('>I', len(track)) + track)
    return b'MThd' + struct.pack('>IHHH', 6, 1, len(parts), PPQN) + b''.join(chunks)


def heartbeat():
    rate = 13379
    rng = random.Random(0x4845415254)
    pcm, filtered, phase = [], 0.0, 0.0
    for i in range(round(rate * 0.18)):
        t = i / rate
        filtered = filtered * 0.88 + rng.uniform(-1, 1) * 0.12
        phase += 2 * math.pi * (46 + 44 * math.exp(-t * 39)) / rate
        attack = min(1, t / 0.004)
        envelope = attack * math.exp(-t * 32) * min(1, (0.18 - t) / 0.015)
        sample = (math.sin(phase) + 0.19 * math.sin(phase * 2) + filtered * 0.15) * envelope
        pcm.append(round(max(-1, min(1, sample * 0.78)) * 32767))
    return rate, struct.pack('<' + 'h' * len(pcm), *pcm)


def validate(parts):
    assert len(parts) == 8
    for part in parts:
        for n in part.notes:
            assert 0 <= n.tick < n.tick + n.duration <= END
            assert 0 <= n.pitch <= 127 and 1 <= n.velocity <= 127
            assert n.tick >= 24
            assert not n.tick < INTRO < n.tick + n.duration
    assert min(n.tick for p in parts for n in p.notes) == 24
    assert not any(n.tick < INTRO for p in parts[1:7] for n in p.notes)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, default=ROOT)
    args = parser.parse_args()
    parts = score()
    validate(parts)
    target = args.out / 'sound/songs/midi' / (NAME + '.mid')
    target.parent.mkdir(parents=True, exist_ok=True)
    data = midi(parts)
    target.write_bytes(data)
    sample = args.out / 'sound/direct_sound_samples/custom/pokevirus_heartbeat.wav'
    sample.parent.mkdir(parents=True, exist_ok=True)
    rate, pcm = heartbeat()
    with wave.open(str(sample), 'wb') as wav:
        wav.setparams((1, 2, rate, 0, 'NONE', 'not compressed'))
        wav.writeframes(pcm)
    manifest = {
        'name': 'The Heart Remembers', 'symbol': NAME, 'tracks': len(parts), 'ppqn': PPQN,
        'introTicks': INTRO, 'loopTicks': END - INTRO, 'loopBpm': 144,
        'loopSeconds': (END - INTRO) / PPQN * 60 / 144,
        'tempos': TEMPOS, 'memoryBars': [8, 9, 18, 19, 28, 29],
        'reference': 'sound/songs/midi/mus_vs_trainer.mid, track 1, bars 000 and 010-011 (mid2agb numbering)',
        'midiSha256': hashlib.sha256(data).hexdigest(),
        'parts': [{'name': p.name, 'voice': p.voice, 'notes': len(p.notes)} for p in parts],
    }
    manifest_path = args.out / 'tools/pokevirus_music/score.json'
    manifest_path.parent.mkdir(parents=True, exist_ok=True)
    manifest_path.write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    print(f'{target}: {len(data)} bytes; {len(parts)} tracks; {manifest["loopSeconds"]:.2f}s loop')


if __name__ == '__main__':
    main()
