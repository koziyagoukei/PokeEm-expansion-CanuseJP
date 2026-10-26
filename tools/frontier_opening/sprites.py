#!/usr/bin/env python3
"""Draw the opening movie's small effect sprites as 4bpp indexed PNGs (needs Pillow).

    firefly.png  16x8   two 8x8 frames: a lit and a dimmed spark (fireflies, light motes)
    glow.png     32x32  a soft round light (the touch, the guardians' charge)
    bead.png     8x8    one link of the network's beaded chain

Each PNG carries a base palette; src/frontier_opening.c recolors them for other moods.
"""
from pathlib import Path

from PIL import Image

OUT = Path(__file__).resolve().parents[2] / 'graphics/custom/frontier_opening'


def save(name, size, palette, pixels):
    image = Image.new('P', size, 0)
    flat = []
    for color in palette + [(0, 0, 0)] * (16 - len(palette)):
        flat += list(color)
    image.putpalette(flat)
    for (x, y), index in pixels.items():
        image.putpixel((x, y), index)
    image.save(OUT / name, bits=4)


def firefly():
    # 0 clear, 1 core, 2 glow, 3 halo
    palette = [(0, 0, 0), (248, 248, 192), (200, 240, 120), (104, 168, 72)]
    pixels = {}
    lit = ['...3....', '..323...', '.32123..', '3211123.', '.32123..', '..323...', '...3....', '........']
    dim = ['........', '........', '...3....', '..323...', '...3....', '........', '........', '........']
    for frame, rows in enumerate((lit, dim)):
        for y, row in enumerate(rows):
            for x, c in enumerate(row):
                if c != '.':
                    pixels[(frame * 8 + x, y)] = int(c)
    save('firefly.png', (16, 8), palette, pixels)


def glow():
    palette = [(0, 0, 0), (248, 248, 248), (248, 240, 200), (240, 216, 152), (216, 176, 120), (168, 128, 96)]
    pixels = {}
    for y in range(32):
        for x in range(32):
            d = ((x - 15.5) ** 2 + (y - 15.5) ** 2) ** 0.5
            # Rings dithered at the edges so the light reads soft at GBA size.
            if d < 4:
                c = 1
            elif d < 7:
                c = 2
            elif d < 10:
                c = 3 if (x + y) % 2 or d < 9 else 2
            elif d < 13:
                c = 4 if (x + y) % 2 else 3
            elif d < 15.5:
                c = 5 if (x + y) % 2 else 0
            else:
                c = 0
            if c:
                pixels[(x, y)] = c
    save('glow.png', (32, 32), palette, pixels)


def bead():
    # Colours follow the Pokevirus chain: dark red-purple links with a toxic green glint.
    palette = [(0, 0, 0), (40, 16, 40), (120, 40, 96), (184, 72, 136), (160, 216, 72)]
    rows = ['........', '..1111..', '.122221.', '.123321.', '.123421.', '.122221.', '..1111..', '........']
    pixels = {(x, y): int(c) for y, row in enumerate(rows) for x, c in enumerate(row) if c != '.'}
    save('bead.png', (8, 8), palette, pixels)


if __name__ == '__main__':
    firefly()
    glow()
    bead()
    for name in ('firefly.png', 'glow.png', 'bead.png'):
        with Image.open(OUT / name) as image:
            print(name, image.size, image.mode, image.info.get('bits', '?'))
