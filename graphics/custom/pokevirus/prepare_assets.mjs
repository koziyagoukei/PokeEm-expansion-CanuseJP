import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';
import crypto from 'node:crypto';

export function prepareAssets(PNG, directory) {
    const palette = [
        [128, 192, 176], [24, 16, 32], [40, 24, 48], [64, 40, 72],
        [88, 56, 104], [120, 80, 136], [160, 120, 168], [200, 168, 208],
        [232, 208, 232], [72, 24, 48], [112, 40, 72], [160, 64, 104],
        [192, 96, 128], [72, 96, 32], [136, 168, 48], [200, 224, 104],
    ];
    const crcTable = Array.from({length: 256}, (_, n) => {
        for (let i = 0; i < 8; i++) n = (n >>> 1) ^ ((n & 1) ? 0xEDB88320 : 0);
        return n >>> 0;
    });
    function chunk(type, data) {
        const body = Buffer.concat([Buffer.from(type), data]);
        let crc = 0xFFFFFFFF;
        for (const b of body) crc = crcTable[(crc ^ b) & 255] ^ (crc >>> 8);
        const output = Buffer.alloc(data.length + 12);
        output.writeUInt32BE(data.length);
        body.copy(output, 4);
        output.writeUInt32BE((crc ^ 0xFFFFFFFF) >>> 0, output.length - 4);
        return output;
    }
    function writeIndexed(name, width, height, pixels) {
        const header = Buffer.alloc(13);
        header.writeUInt32BE(width, 0); header.writeUInt32BE(height, 4);
        header[8] = 4; header[9] = 3;
        const rowBytes = Math.ceil(width / 2);
        const rows = Buffer.alloc((rowBytes + 1) * height);
        for (let y = 0; y < height; y++)
            for (let x = 0; x < width; x++)
                rows[y * (rowBytes + 1) + 1 + (x >> 1)] |= pixels[y * width + x] << ((x & 1) ? 0 : 4);
        const output = Buffer.concat([
            Buffer.from([137, 80, 78, 71, 13, 10, 26, 10]), chunk('IHDR', header),
            chunk('PLTE', Buffer.from(palette.flat())), chunk('tRNS', Buffer.from([0])),
            chunk('IDAT', zlib.deflateSync(rows)), chunk('IEND', Buffer.alloc(0)),
        ]);
        fs.writeFileSync(path.join(directory, name), output);
    }
    function nearestColor(r, g, b) {
        let best = 1, distance = Infinity;
        for (let i = 1; i < palette.length; i++) {
            const c = palette[i], d = (r - c[0]) ** 2 + (g - c[1]) ** 2 + (b - c[2]) ** 2;
            if (d < distance) { distance = d; best = i; }
        }
        return best;
    }
    function bounds(source, rect) {
        let left = rect.x + rect.width, right = -1, top = rect.y + rect.height, bottom = -1;
        for (let y = rect.y; y < rect.y + rect.height; y++)
            for (let x = rect.x; x < rect.x + rect.width; x++)
                if (source.data[(y * source.width + x) * 4 + 3] >= 192) {
                    left = Math.min(left, x); right = Math.max(right, x);
                    top = Math.min(top, y); bottom = Math.max(bottom, y);
                }
        if (right < left) throw new Error('Empty sprite cell');
        return {left, right, top, bottom, width: right - left + 1, height: bottom - top + 1};
    }
    function frame(source, area, scale) {
        const width = Math.round(area.width * scale), height = Math.round(area.height * scale);
        const xOffset = Math.floor((64 - width) / 2), yOffset = Math.floor((64 - height) / 2);
        const pixels = new Uint8Array(64 * 64);
        for (let y = 0; y < height; y++)
            for (let x = 0; x < width; x++) {
                const sx = area.left + Math.min(area.width - 1, Math.floor((x + 0.5) * area.width / width));
                const sy = area.top + Math.min(area.height - 1, Math.floor((y + 0.5) * area.height / height));
                const at = (sy * source.width + sx) * 4;
                if (source.data[at + 3] >= 192)
                    pixels[(yOffset + y) * 64 + xOffset + x] = nearestColor(...source.data.subarray(at, at + 3));
            }
        return pixels;
    }
    function preview(name, width, height, pixels, scale) {
        const output = new Uint8Array(width * height * scale * scale);
        for (let y = 0; y < height * scale; y++)
            for (let x = 0; x < width * scale; x++)
                output[y * width * scale + x] = pixels[Math.floor(y / scale) * width + Math.floor(x / scale)];
        writeIndexed(name, width * scale, height * scale, output);
    }
    const frontSource = PNG.sync.read(fs.readFileSync(path.join(directory, 'source/front_generated.png')));
    const sdSource = PNG.sync.read(fs.readFileSync(path.join(directory, 'source/overworld_generated.png')));
    const frontArea = bounds(frontSource, {x: 0, y: 0, width: frontSource.width, height: frontSource.height});
    const front = frame(frontSource, frontArea, Math.min(58 / frontArea.width, 58 / frontArea.height));
    writeIndexed('front.png', 64, 64, front);
    preview('front_preview_8x.png', 64, 64, front, 8);
    const breaks = [0, Math.round(sdSource.height * 17 / 48), Math.round(sdSource.height * 2 / 3), sdSource.height];
    const areas = Array.from({length: 6}, (_, i) => {
        const column = i % 2, row = Math.floor(i / 2);
        const x = Math.floor(column * sdSource.width / 2);
        return bounds(sdSource, {x, y: breaks[row],
            width: Math.floor((column + 1) * sdSource.width / 2) - x,
            height: breaks[row + 1] - breaks[row]});
    });
    const scale = Math.min(56 / Math.max(...areas.map(a => a.width)), 56 / Math.max(...areas.map(a => a.height)));
    const frames = areas.map(a => frame(sdSource, a, scale));
    const sheet = new Uint8Array(384 * 64), grid = new Uint8Array(128 * 192);
    for (let i = 0; i < 6; i++)
        for (let y = 0; y < 64; y++) {
            sheet.set(frames[i].subarray(y * 64, y * 64 + 64), y * 384 + i * 64);
            grid.set(frames[i].subarray(y * 64, y * 64 + 64), (Math.floor(i / 2) * 64 + y) * 128 + i % 2 * 64);
        }
    writeIndexed('overworld.png', 384, 64, sheet);
    writeIndexed('overworld_idle.png', 64, 64, frames[0]);
    preview('overworld_preview_4x.png', 128, 192, grid, 4);
    const comparison = new Uint8Array(128 * 64);
    for (let y = 0; y < 64; y++) {
        comparison.set(front.subarray(y * 64, y * 64 + 64), y * 128);
        comparison.set(frames[0].subarray(y * 64, y * 64 + 64), y * 128 + 64);
    }
    preview('comparison_preview_4x.png', 128, 64, comparison, 4);
    fs.writeFileSync(path.join(directory, 'palette.pal'),
        'JASC-PAL\n0100\n16\n' + palette.map(c => c.join(' ')).join('\n') + '\n');
    const assets = [];
    for (const name of ['front.png', 'overworld.png', 'overworld_idle.png']) {
        const data = fs.readFileSync(path.join(directory, name)), image = PNG.sync.read(data);
        const colors = new Set(), alpha = new Set();
        for (let i = 0; i < image.data.length; i += 4) {
            colors.add(image.data.subarray(i, i + 4).toString('hex')); alpha.add(image.data[i + 3]);
        }
        if (colors.size > 16 || [...alpha].some(a => a !== 0 && a !== 255)) throw new Error('Invalid palette/alpha');
        assets.push({name, width: image.width, height: image.height, bitDepth: data[24], colorType: data[25],
            colors: colors.size, alpha: [...alpha].sort(), sha256: crypto.createHash('sha256').update(data).digest('hex')});
    }
    const frameHashes = frames.map(f => crypto.createHash('sha256').update(f).digest('hex'));
    if (new Set(frameHashes).size !== 6) throw new Error('Duplicated overworld frames');
    const result = {assets, frameOrder: ['south_idle', 'south_pulse', 'north_idle', 'north_pulse', 'west_idle', 'west_pulse'],
        frameHashes, generatedWith: 'built-in image_gen',
        integration: {mapGraphicsId: 'OBJ_EVENT_GFX_POKEVIRUS', trainerPicId: 'TRAINER_PIC_POKEVIRUS', placedOnMap: false}};
    fs.writeFileSync(path.join(directory, 'validation.json'), JSON.stringify(result, null, 2) + '\n');
    return result;
}
