import hashlib
import json
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[2]
ART = ROOT/'graphics/custom/frontier_opening'


class OpeningAssetsTest(unittest.TestCase):
    def test_bitmaps_are_complete_gba_palette_indices(self):
        report = json.loads((ART/'validation.json').read_text())
        for name in ('forest', 'meeting_hands', 'network', 'ridge'):
            pixels = (ART/f'{name}.pixels.bin').read_bytes()
            palette = (ART/f'{name}.palette.bin').read_bytes()
            self.assertEqual(len(pixels), 240*160)
            self.assertEqual(len(palette), 512)
            self.assertLessEqual(report[name]['colors'], 64)
            self.assertLess(max(pixels), report[name]['colors'])
            self.assertTrue(all(v < 0x8000 for v in struct.unpack('<256H', palette)))
            self.assertEqual(hashlib.sha256(pixels).hexdigest(), report[name]['sha256'])

    def test_native_and_enlarged_previews_have_integer_dimensions(self):
        for name in ('forest', 'meeting_hands', 'network', 'ridge'):
            for suffix, dimensions in (('', (240,160)), ('_4x', (960,640))):
                png = (ART/f'{name}{suffix}.png').read_bytes()
                self.assertEqual(png[:8], b'\x89PNG\r\n\x1a\n')
                self.assertEqual(struct.unpack('>II', png[16:24]), dimensions)

    def test_new_movie_does_not_access_saves_or_use_the_gameplay_intro(self):
        source = (ROOT/'src/frontier_opening.c').read_text()
        for token in ('gSaveBlock', 'SaveGame', 'TrySavingData', 'VarSet(', 'FlagSet(', 'frontier_intro.h'):
            self.assertNotIn(token, source)

    def test_guardians_are_back_sprites_under_the_pokevirus(self):
        source = (ROOT/'src/frontier_opening.c').read_text()
        for species in ('darkrai', 'zygarde', 'rayquaza'):
            self.assertIn(f'"graphics/pokemon/{species}/back.png", ".4bpp"', source)
        self.assertIn('"graphics/custom/pokevirus/front.png", ".4bpp"', source)
        self.assertIn('gReservedSpriteTileCount = 512', source)

    def test_effect_sprites_are_4bpp_tiles(self):
        for name, size in (('firefly', (16, 8)), ('glow', (32, 32)), ('bead', (8, 8))):
            png = (ART/f'{name}.png').read_bytes()
            self.assertEqual(png[:8], b'\x89PNG\r\n\x1a\n')
            self.assertEqual(struct.unpack('>II', png[16:24]), size)
            self.assertEqual(png[24:26], b'\x04\x03')  # 4-bit indexed

    def test_movie_is_cut_to_its_song(self):
        source = (ROOT/'src/frontier_opening.c').read_text()
        music = (ROOT/'tools/frontier_opening/music.py').read_text()
        # 100 BPM: a beat is 3600 / 100 = 36 frames, and the movie ends with the song.
        self.assertIn('\nBPM = 100\n', music)
        self.assertIn('\nPPQN = 24\n', music)
        self.assertIn('\nBARS = 11\n', music)
        self.assertIn('#define BEAT 36\n', source)
        self.assertIn('[FRONTIER_OPENING_DONE] = 11 * BAR,', source)
        midi = (ROOT/'sound/songs/midi/mus_frontier_opening.mid').read_bytes()
        self.assertEqual(midi[:4], b'MThd')
        self.assertEqual(struct.unpack('>HHH', midi[8:14]), (1, 10, 24))
        config = (ROOT/'sound/songs/midi/midi.cfg').read_text()
        self.assertIn('mus_frontier_opening.mid:', config)
        self.assertIn('-G_frontier_opening', config)


if __name__ == '__main__':
    unittest.main()
