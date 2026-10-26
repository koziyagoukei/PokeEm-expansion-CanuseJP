import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import tempfile
import unittest

import music

ROOT = Path(__file__).resolve().parents[2]
ART = ROOT / 'graphics/custom/frontier_title'


class TitleScoreTest(unittest.TestCase):
    def test_three_minute_finite_score_and_monophonic_tracks(self):
        music.validate(music.score())
        self.assertEqual(music.END / music.PPQN * 60 / music.BPM, 180)
        self.assertEqual(len(music.score()), 7)
        self.assertEqual(music.BEATS, 3)

    def test_midi_is_reproducible_and_every_track_has_a_finite_end(self):
        data = music.midi(music.score())
        self.assertEqual(struct.unpack('>IHHH', data[4:14]), (6, 1, 7, 24))
        self.assertEqual(data.count(b'\xff\x2f\x00'), 7)
        self.assertNotIn(b'\xff\x06\x01[', data)
        self.assertNotIn(b'\xff\x06\x01]', data)
        self.assertEqual(data, music.midi(music.score()))
        self.assertEqual(data, (ROOT / 'sound/songs/midi/mus_frontier_title.mid').read_bytes())
        report = json.loads((Path(__file__).parent / 'score.json').read_text())
        self.assertEqual(report['midiSha256'], hashlib.sha256(data).hexdigest())
        self.assertFalse(report['loop'])

    def test_score_sections_have_contrasting_dynamics_and_a_quiet_coda(self):
        lead = music.score()[0]
        self.assertEqual(lead.programs, [(24 * music.BAR, music.FLUTE), (40 * music.BAR, music.PIANO)])
        for part in music.score():
            self.assertEqual(part.volumes[-1][1], 0)
            self.assertGreater(len(part.notes), 10)
        self.assertTrue(any(n.tick >= 71 * music.BAR for n in lead.notes))


class TitleArtworkTest(unittest.TestCase):
    def test_bg_tiles_fit_before_the_map_and_only_use_the_reserved_bank(self):
        tiles = (ART / 'still.tiles.bin').read_bytes()
        tilemap = (ART / 'still.map.bin').read_bytes()
        palette = (ART / 'still.palette.bin').read_bytes()
        self.assertEqual(len(tiles) % 32, 0)
        self.assertLessEqual(len(tiles), 0x5000)
        self.assertEqual(len(tilemap), 0x800)
        self.assertEqual(len(palette), 32)
        self.assertEqual(tiles[:32], bytes(32))
        for word, in struct.iter_unpack('<H', tilemap):
            self.assertEqual(word >> 12, 15)
            self.assertLess(word & 1023, len(tiles) // 32)
        self.assertTrue(all(c < 0x8000 for c, in struct.iter_unpack('<H', palette)))
        report = json.loads((ART / 'validation.json').read_text())
        self.assertEqual(report['colors'], 16)
        self.assertEqual(report['sha256'], hashlib.sha256(tiles + tilemap + palette).hexdigest())

    def test_native_and_integer_zoom_previews(self):
        for name, dimensions in [('still.png', (240, 160)), ('still_4x.png', (960, 640))]:
            data = (ART / name).read_bytes()
            self.assertEqual(data[:8], b'\x89PNG\r\n\x1a\n')
            self.assertEqual(struct.unpack('>II', data[16:24]), dimensions)

    def test_existing_logo_does_not_use_the_background_palette_bank(self):
        converter = ROOT / 'tools/gbagfx/gbagfx'
        if not converter.is_file() or not shutil.which('sh'):
            self.skipTest('build gbagfx and run in the build environment')
        with tempfile.TemporaryDirectory() as directory:
            target = Path(directory) / 'logo.8bpp'
            subprocess.run([str(converter), str(ROOT / 'graphics/title_screen/pokemon_logo.png'),
                            str(target)], check=True)
            self.assertLess(max(target.read_bytes()), 240)

    def test_title_keeps_navigation_but_removes_moving_clouds_and_marking_color(self):
        source = (ROOT / 'src/title_screen.c').read_text()
        self.assertIn('m4aSongNumStart(MUS_FRONTIER_TITLE)', source)
        self.assertIn('CreatePressStartBanner(START_BANNER_X, START_BANNER_Y)', source)
        self.assertNotIn('ScanlineEffect_InitWave(', source)
        self.assertNotIn('UpdateLegendaryMarkingColor(', source)
        self.assertIn('JOY_HELD(CLEAR_SAVE_BUTTON_COMBO)', source)
        self.assertIn('SetMainCallback2(CB2_InitMainMenu)', source)


if __name__ == '__main__':
    unittest.main()
