import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';

// PNG decoding is injected so this asset utility needs no project dependency.
export function prepareAssets(PNG, directory) {
    const palette = [
        [144, 200, 176], [16, 16, 24], [32, 32, 48], [56, 56, 72],
        [64, 72, 88], [104, 112, 136], [128, 136, 160], [248, 248, 248],
        [192, 192, 208], [136, 136, 152], [248, 200, 144], [216, 152, 104],
        [152, 96, 72], [216, 40, 40], [136, 24, 24], [232, 176, 120],
    ];
    const crcTable = Array.from({length: 256}, (_, n) => {
        for (let i = 0; i < 8; i++) n = (n >>> 1) ^ ((n & 1) ? 0xEDB88320 : 0);
        return n >>> 0;
    });
    function chunk(type, data) {
        const body = Buffer.concat([Buffer.from(type), data]);
        let crc = 0xFFFFFFFF;
        for (const b of body) crc = crcTable[(crc ^ b) & 255] ^ (crc >>> 8);
        const result = Buffer.alloc(data.length + 12);
        result.writeUInt32BE(data.length);
        body.copy(result, 4);
        result.writeUInt32BE((crc ^ 0xFFFFFFFF) >>> 0, result.length - 4);
        return result;
    }
    function writeIndexed(name, width, height, pixels) {
        const header = Buffer.alloc(13);
        header.writeUInt32BE(width, 0);
        header.writeUInt32BE(height, 4);
        header[8] = 4;
        header[9] = 3;
        const rowBytes = Math.ceil(width / 2);
        const rows = Buffer.alloc((rowBytes + 1) * height);
        for (let y = 0; y < height; y++)
            for (let x = 0; x < width; x++)
                rows[y * (rowBytes + 1) + 1 + (x >> 1)] |= pixels[y * width + x] << ((x & 1) ? 0 : 4);
        fs.writeFileSync(path.join(directory, name), Buffer.concat([
            Buffer.from([137, 80, 78, 71, 13, 10, 26, 10]),
            chunk('IHDR', header), chunk('PLTE', Buffer.from(palette.flat())),
            chunk('tRNS', Buffer.from([0])), chunk('IDAT', zlib.deflateSync(rows)),
            chunk('IEND', Buffer.alloc(0)),
        ]));
    }
    function nearestColor(r, g, b) {
        let index = 1, distance = Infinity;
        for (let i = 1; i < palette.length; i++) {
            const p = palette[i];
            const d = (r - p[0]) ** 2 + (g - p[1]) ** 2 + (b - p[2]) ** 2;
            if (d < distance) { distance = d; index = i; }
        }
        return index;
    }
    function makeFrame(source, rect, width, height, bodyHeight) {
        let left = rect.x + rect.width, top = rect.y + rect.height, right = -1, bottom = -1;
        for (let y = rect.y; y < rect.y + rect.height; y++)
            for (let x = rect.x; x < rect.x + rect.width; x++)
                if (source.data[(y * source.width + x) * 4 + 3] >= 192) {
                    left = Math.min(left, x); right = Math.max(right, x);
                    top = Math.min(top, y); bottom = Math.max(bottom, y);
                }
        if (right < left) throw new Error('Empty sprite cell');
        const sourceWidth = right - left + 1, sourceHeight = bottom - top + 1;
        const scale = Math.min((width - 2) / sourceWidth, bodyHeight / sourceHeight);
        const w = Math.max(1, Math.round(sourceWidth * scale));
        const h = Math.max(1, Math.round(sourceHeight * scale));
        const xOffset = Math.floor((width - w) / 2), yOffset = height - h - 1;
        const output = new Uint8Array(width * height);
        for (let y = 0; y < h; y++)
            for (let x = 0; x < w; x++) {
                const sx = left + Math.min(sourceWidth - 1, Math.floor((x + 0.5) * sourceWidth / w));
                const sy = top + Math.min(sourceHeight - 1, Math.floor((y + 0.5) * sourceHeight / h));
                const i = (sy * source.width + sx) * 4;
                if (source.data[i + 3] >= 192)
                    output[(y + yOffset) * width + x + xOffset] = nearestColor(...source.data.subarray(i, i + 3));
            }
        return output;
    }
    const frontSource = PNG.sync.read(fs.readFileSync(path.join(directory, 'source/front_generated.png')));
    const sdSource = PNG.sync.read(fs.readFileSync(path.join(directory, 'source/overworld_generated.png')));
    const front = makeFrame(frontSource, {x: 0, y: 0, width: frontSource.width, height: frontSource.height}, 64, 64, 60);
    writeIndexed('front.png', 64, 64, front);
    const sheet = new Uint8Array(144 * 32);
    const frames = [];
    for (let i = 0; i < 9; i++) {
        const x = Math.floor(i % 3 * sdSource.width / 3);
        const y = Math.floor(Math.floor(i / 3) * sdSource.height / 3);
        const rect = {x, y, width: Math.floor((i % 3 + 1) * sdSource.width / 3) - x,
            height: Math.floor((Math.floor(i / 3) + 1) * sdSource.height / 3) - y};
        const frame = makeFrame(sdSource, rect, 16, 32, 26);
        // Keep the chest lettering legible after reduction to the engine's grid.
        if ([0, 3, 4].includes(i)) {
            const mark = ['RR.', 'R.R', 'RR.', 'R.R'];
            for (let my = 0; my < mark.length; my++)
                for (let mx = 0; mx < 3; mx++)
                    frame[(21 + my) * 16 + 7 + mx] = mark[my][mx] === 'R' ? 13 : 7;
        }
        frames.push(frame);
        for (let row = 0; row < 32; row++) sheet.set(frame.subarray(row * 16, row * 16 + 16), row * 144 + i * 16);
    }
    writeIndexed('overworld.png', 144, 32, sheet);
    writeIndexed('overworld_front.png', 16, 32, frames[0]);
    function preview(name, width, height, pixels, scale) {
        const output = new Uint8Array(width * height * scale * scale);
        for (let y = 0; y < height * scale; y++)
            for (let x = 0; x < width * scale; x++)
                output[y * width * scale + x] = pixels[Math.floor(y / scale) * width + Math.floor(x / scale)];
        writeIndexed(name, width * scale, height * scale, output);
    }
    preview('front_preview_8x.png', 64, 64, front, 8);
    const grid = new Uint8Array(48 * 96);
    for (let i = 0; i < 9; i++)
        for (let y = 0; y < 32; y++)
            grid.set(frames[i].subarray(y * 16, y * 16 + 16), (Math.floor(i / 3) * 32 + y) * 48 + i % 3 * 16);
    preview('overworld_preview_8x.png', 48, 96, grid, 8);
    const backPath = path.join(directory, 'source/back_generated.png');
    if (fs.existsSync(backPath)) {
        const source = PNG.sync.read(fs.readFileSync(backPath));
        const splitX = Math.floor(source.width / 2), splitY = Math.round(source.height * 0.525);
        const cells = [];
        // Discard isolated generation artifacts before finding a common scale.
        for (let frame = 0; frame < 4; frame++) {
            const x = frame % 2 ? splitX : 0, y = frame < 2 ? 0 : splitY;
            const width = frame % 2 ? source.width - splitX : splitX;
            const height = frame < 2 ? splitY : source.height - splitY;
            const visited = new Uint8Array(width * height);
            let largest = [];
            const opaque = index => source.data[((y + Math.floor(index / width)) * source.width + x + index % width) * 4 + 3] >= 192;
            for (let start = 0; start < visited.length; start++) {
                if (visited[start] || !opaque(start)) continue;
                const queue = [start];
                visited[start] = 1;
                for (let q = 0; q < queue.length; q++) {
                    const at = queue[q], px = at % width, py = Math.floor(at / width);
                    const neighbors = [];
                    if (px) neighbors.push(at - 1);
                    if (px + 1 < width) neighbors.push(at + 1);
                    if (py) neighbors.push(at - width);
                    if (py + 1 < height) neighbors.push(at + width);
                    for (const next of neighbors)
                        if (!visited[next] && opaque(next)) { visited[next] = 1; queue.push(next); }
                }
                if (queue.length > largest.length) largest = queue;
            }
            if (!largest.length) throw new Error('Empty back sprite frame');
            const mask = new Uint8Array(width * height);
            let left = width, right = 0, top = height, bottom = 0;
            for (const at of largest) {
                mask[at] = 1;
                left = Math.min(left, at % width); right = Math.max(right, at % width);
                top = Math.min(top, Math.floor(at / width)); bottom = Math.max(bottom, Math.floor(at / width));
            }
            let headLeft = width, headRight = 0;
            for (const at of largest)
                if (Math.floor(at / width) <= top + (bottom - top) * 0.15) {
                    headLeft = Math.min(headLeft, at % width); headRight = Math.max(headRight, at % width);
                }
            const anchorX = (headLeft + headRight) / 2;
            cells.push({x, y, width, height, mask, left, right, top, bottom, anchorX});
        }
        const minX = Math.min(...cells.map(c => c.left - c.anchorX));
        const maxX = Math.max(...cells.map(c => c.right - c.anchorX));
        const maxHeight = Math.max(...cells.map(c => c.bottom - c.top + 1));
        // Match the engine's waist-up framing without shrinking the head for the extended arm.
        const scaleX = 62 / (maxX - minX + 1), scaleY = 60 / maxHeight;
        const anchorX = 1 - minX * scaleX;
        const back = new Uint8Array(64 * 256), backGrid = new Uint8Array(128 * 128);
        for (let f = 0; f < 4; f++) {
            const cell = cells[f];
            for (let y = 0; y < 64; y++)
                for (let x = 0; x < 64; x++) {
                    const sx = Math.floor(cell.anchorX + (x + 0.5 - anchorX) / scaleX);
                    const sy = Math.floor(cell.bottom + 1 - (64 - y - 0.5) / scaleY);
                    if (sx < 0 || sx >= cell.width || sy < 0 || sy >= cell.height || !cell.mask[sy * cell.width + sx]) continue;
                    const at = ((cell.y + sy) * source.width + cell.x + sx) * 4;
                    const color = nearestColor(...source.data.subarray(at, at + 3));
                    back[(f * 64 + y) * 64 + x] = color;
                    backGrid[(Math.floor(f / 2) * 64 + y) * 128 + f % 2 * 64 + x] = color;
                }
        }
        writeIndexed('back.png', 64, 256, back);
        preview('back_preview_4x.png', 128, 128, backGrid, 4);
    }
    fs.writeFileSync(path.join(directory, 'palette.pal'),
        'JASC-PAL\n0100\n16\n' + palette.map(c => c.join(' ')).join('\n') + '\n');
    return {front: '64x64', overworld: '144x32 (9 frames, 16x32 each)', back: fs.existsSync(backPath) ? '64x256 (4 frames)' : null,
        paletteEntries: 16, transparentIndex: 0, bitDepth: 4, colorType: 3};
}
