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
    const source = PNG.sync.read(fs.readFileSync(path.join(directory, 'source/chain_generated.png')));
    let left = source.width, right = 0, top = source.height, bottom = 0;
    for (let y = 0; y < source.height; y++)
        for (let x = 0; x < source.width; x++)
            if (source.data[(y * source.width + x) * 4 + 3] >= 192) {
                left = Math.min(left, x); right = Math.max(right, x);
                top = Math.min(top, y); bottom = Math.max(bottom, y);
            }
    if (left > right) throw new Error('Empty chain source');
    const width = 32, height = 64;
    const scale = Math.min(14 / (right - left + 1), height / (bottom - top + 1));
    const w = Math.max(1, Math.round((right - left + 1) * scale));
    const h = Math.max(1, Math.round((bottom - top + 1) * scale));
    const pixels = new Uint8Array(width * height);
    for (let y = 0; y < h; y++)
        for (let x = 0; x < w; x++) {
            const sx = left + Math.min(right - left, Math.floor((x + 0.5) * (right - left + 1) / w));
            const sy = top + Math.min(bottom - top, Math.floor((y + 0.5) * (bottom - top + 1) / h));
            const at = (sy * source.width + sx) * 4;
            if (source.data[at + 3] < 192) continue;
            let best = 1, distance = Infinity;
            for (let i = 1; i < palette.length; i++) {
                const d = palette[i].reduce((sum, c, k) => sum + (c - source.data[at + k]) ** 2, 0);
                if (d < distance) { best = i; distance = d; }
            }
            pixels[(y + Math.floor((height - h) / 2)) * width + x + Math.floor((width - w) / 2)] = best;
        }
    const crcTable = Array.from({length: 256}, (_, n) => {
        for (let i = 0; i < 8; i++) n = (n >>> 1) ^ ((n & 1) ? 0xEDB88320 : 0);
        return n >>> 0;
    });
    function chunk(type, data) {
        const body = Buffer.concat([Buffer.from(type), data]);
        let crc = 0xFFFFFFFF;
        for (const b of body) crc = crcTable[(crc ^ b) & 255] ^ (crc >>> 8);
        const output = Buffer.alloc(data.length + 12);
        output.writeUInt32BE(data.length); body.copy(output, 4);
        output.writeUInt32BE((crc ^ 0xFFFFFFFF) >>> 0, output.length - 4);
        return output;
    }
    function write(name, w, h, data) {
        const header = Buffer.alloc(13);
        header.writeUInt32BE(w, 0); header.writeUInt32BE(h, 4);
        header[8] = 4; header[9] = 3;
        const rows = Buffer.alloc((w / 2 + 1) * h);
        for (let y = 0; y < h; y++)
            for (let x = 0; x < w; x++)
                rows[y * (w / 2 + 1) + 1 + (x >> 1)] |= data[y * w + x] << ((x & 1) ? 0 : 4);
        const png = Buffer.concat([
            Buffer.from([137, 80, 78, 71, 13, 10, 26, 10]), chunk('IHDR', header),
            chunk('PLTE', Buffer.from(palette.flat())), chunk('tRNS', Buffer.from([0])),
            chunk('IDAT', zlib.deflateSync(rows)), chunk('IEND', Buffer.alloc(0)),
        ]);
        fs.writeFileSync(path.join(directory, name), png);
        return png;
    }
    const png = write('chain.png', width, height, pixels);
    const preview = new Uint8Array(width * height * 64);
    for (let y = 0; y < height * 8; y++)
        for (let x = 0; x < width * 8; x++)
            preview[y * width * 8 + x] = pixels[Math.floor(y / 8) * width + Math.floor(x / 8)];
    write('chain_preview_8x.png', width * 8, height * 8, preview);
    const decoded = PNG.sync.read(png);
    const alpha = new Set(), colors = new Set();
    for (let i = 0; i < decoded.data.length; i += 4) {
        alpha.add(decoded.data[i + 3]); colors.add(decoded.data.subarray(i, i + 4).toString('hex'));
    }
    if ([...alpha].some(a => a !== 0 && a !== 255) || colors.size > 16) throw new Error('Invalid indexed asset');
    const result = {width, height, bitDepth: png[24], colorType: png[25], colors: colors.size,
        alpha: [...alpha].sort(), gbaBytes: width * height / 2,
        sha256: crypto.createHash('sha256').update(png).digest('hex'), generatedWith: 'built-in image_gen'};
    fs.writeFileSync(path.join(directory, 'validation.json'), JSON.stringify(result, null, 2) + '\n');
    return result;
}
