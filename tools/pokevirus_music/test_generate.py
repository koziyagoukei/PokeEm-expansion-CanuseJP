import struct
import unittest

import generate


class ScoreTest(unittest.TestCase):
    def test_score_and_silent_opening(self):
        parts = generate.score()
        generate.validate(parts)
        self.assertEqual(len(parts), 8)
        self.assertEqual(min(n.tick for p in parts for n in p.notes), 24)
        self.assertEqual([n.tick for n in parts[0].notes if n.tick < generate.INTRO],
                         [24, 31, 78, 84, 120, 126, 162, 168])

    def test_all_track_loop_markers_match(self):
        data = generate.midi(generate.score())
        self.assertEqual(data[:4], b'MThd')
        self.assertEqual(struct.unpack('>IHHH', data[4:14]), (6, 1, 8, 24))
        self.assertEqual(data.count(b'\xff\x06\x01['), 8)
        self.assertEqual(data.count(b'\xff\x06\x01]'), 8)
        self.assertEqual(data.count(b'\xff\x2f\x00'), 8)
        self.assertEqual(data, generate.midi(generate.score()))

    def test_no_note_crosses_intro_or_loop_boundary(self):
        for part in generate.score():
            for n in part.notes:
                self.assertLessEqual(n.tick + n.duration, generate.END)
                self.assertFalse(n.tick < generate.INTRO < n.tick + n.duration)

    def test_original_heartbeat_is_bounded_and_tapers(self):
        rate, pcm = generate.heartbeat()
        values = struct.unpack('<' + 'h' * (len(pcm) // 2), pcm)
        self.assertEqual(rate, 13379)
        self.assertGreater(len(values), 2000)
        self.assertLess(max(abs(v) for v in values), 32767)
        self.assertGreater(sum(v * v for v in values[100:500]),
                           sum(v * v for v in values[-400:]) * 100)
        self.assertEqual(generate.heartbeat(), (rate, pcm))

    def test_memories_are_short_and_separated(self):
        notes = generate.score()[5].notes
        for n in notes:
            bar = (n.tick - generate.INTRO) // generate.BAR
            self.assertIn(bar, [8, 9, 18, 19, 28, 29])
        phrase = [(n.tick - generate.INTRO - 8 * generate.BAR, n.pitch - 12) for n in notes
                  if generate.INTRO + 8 * generate.BAR <= n.tick < generate.INTRO + 10 * generate.BAR]
        self.assertEqual(phrase[:3], [(0, 60), (36, 67), (72, 65)])


if __name__ == '__main__':
    unittest.main()
