import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';

// A deliberately small palette removes painted texture instead of dithering it.
const COLORS = [
    [0, 0, 0], [16, 24, 48], [8, 16, 32], [48, 24, 32],
    [72, 32, 32], [104, 48, 32], [88, 40, 16], [144, 64, 24],
    [184, 96, 40], [152, 120, 88], [248, 224, 168], [216, 184, 136],
    [184, 104, 64], [232, 144, 88], [248, 176, 112], [48, 56, 64],
];

export function prepareAssets(PNG, root) {
    const directory = path.join(root, 'graphics/custom/frontier_title');
    const source = PNG.sync.read(fs.readFileSync(path.join(directory, 'source/generated.png')));
    const image = new PNG({width: 240, height: 160});
    const pixels = Buffer.alloc(240 * 160);
    for (let y = 0; y < 160; y++) for (let x = 0; x < 240; x++) {
        const at = (Math.floor((y + 0.5) * source.height / 160) * source.width
                  + Math.floor((x + 0.5) * source.width / 240)) * 4;
        let best = 1, distance = Infinity;
        for (let color = 1; color < COLORS.length; color++) {
            const d = COLORS[color].reduce((sum, c, i) => sum + (c - source.data[at + i]) ** 2, 0);
            if (d < distance) { best = color; distance = d; }
        }
        pixels[y * 240 + x] = best;
        image.data.set([...COLORS[best], 255], (y * 240 + x) * 4);
    }

    const tiles = [Buffer.alloc(32)], index = new Map([[tiles[0].toString('hex'), 0]]);
    const map = Buffer.alloc(32 * 32 * 2);
    for (let y = 0; y < 32; y++) for (let x = 0; x < 32; x++) {
        const tile = Buffer.alloc(32);
        if (x < 30 && y < 20) {
            for (let py = 0; py < 8; py++) for (let px = 0; px < 8; px += 2)
                tile[py * 4 + px / 2] = pixels[(y * 8 + py) * 240 + x * 8 + px]
                    | (pixels[(y * 8 + py) * 240 + x * 8 + px + 1] << 4);
        }
        const key = tile.toString('hex');
        if (!index.has(key)) { index.set(key, tiles.length); tiles.push(tile); }
        map.writeUInt16LE(0xF000 | index.get(key), (y * 32 + x) * 2);
    }
    const gfx = Buffer.concat(tiles), palette = Buffer.alloc(32);
    COLORS.forEach((rgb, i) => palette.writeUInt16LE((rgb[0] >> 3)
        | ((rgb[1] >> 3) << 5) | ((rgb[2] >> 3) << 10), i * 2));
    if (gfx.length > 0x5000) throw new Error('Title BG tiles overlap screen block 26');
    fs.writeFileSync(path.join(directory, 'still.tiles.bin'), gfx);
    fs.writeFileSync(path.join(directory, 'still.map.bin'), map);
    fs.writeFileSync(path.join(directory, 'still.palette.bin'), palette);
    fs.writeFileSync(path.join(directory, 'still.png'), PNG.sync.write(image));
    const zoom = new PNG({width: 960, height: 640});
    for (let y = 0; y < zoom.height; y++) for (let x = 0; x < zoom.width; x++) {
        const at = (Math.floor(y / 4) * 240 + Math.floor(x / 4)) * 4;
        image.data.copy(zoom.data, (y * zoom.width + x) * 4, at, at + 4);
    }
    fs.writeFileSync(path.join(directory, 'still_4x.png'), PNG.sync.write(zoom));
    const report = {width: 240, height: 160, colors: COLORS.length, tileCount: tiles.length,
        paletteBank: 15, gfxBytes: gfx.length, mapBytes: map.length,
        sha256: crypto.createHash('sha256').update(gfx).update(map).update(palette).digest('hex')};
    fs.writeFileSync(path.join(directory, 'validation.json'), JSON.stringify(report, null, 2) + '\n');
    return report;
}
