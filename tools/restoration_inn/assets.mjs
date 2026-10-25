import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';
import crypto from 'node:crypto';

const palettes = {
    exterior: [[240,0,240],[24,24,32],[48,56,64],[72,88,120],[120,144,168],[64,48,40],
        [112,80,56],[168,128,80],[200,192,160],[232,224,192],[40,80,56],[80,128,64],
        [144,176,96],[136,48,64],[208,96,72],[48,120,152]],
    interior: [[240,0,240],[24,24,32],[64,48,40],[112,80,56],[168,128,80],[88,112,64],
        [152,168,104],[200,208,152],[232,224,192],[184,176,152],[104,104,104],[112,48,64],
        [184,80,72],[56,80,128],[128,152,176],[232,184,104]],
};

function indexedFile(width, height, pixels, palette) {
    const table = Array.from({length:256}, (_,n) => {
        for (let k=0;k<8;k++) n=(n>>>1)^((n&1)?0xEDB88320:0);
        return n>>>0;
    });
    function chunk(type,data) {
        const body=Buffer.concat([Buffer.from(type),data]), out=Buffer.alloc(data.length+12);
        let crc=0xFFFFFFFF;
        for(const b of body) crc=table[(crc^b)&255]^(crc>>>8);
        out.writeUInt32BE(data.length); body.copy(out,4);
        out.writeUInt32BE((crc^0xFFFFFFFF)>>>0,out.length-4); return out;
    }
    const ihdr=Buffer.alloc(13); ihdr.writeUInt32BE(width); ihdr.writeUInt32BE(height,4);
    ihdr[8]=4; ihdr[9]=3;
    const rows=Buffer.alloc((width/2+1)*height);
    for(let y=0;y<height;y++) for(let x=0;x<width;x++)
        rows[y*(width/2+1)+1+(x>>1)] |= pixels[y*width+x]<<((x&1)?0:4);
    return Buffer.concat([Buffer.from([137,80,78,71,13,10,26,10]),chunk('IHDR',ihdr),
        chunk('PLTE',Buffer.from(palette.flat())),chunk('tRNS',Buffer.from([0])),
        chunk('IDAT',zlib.deflateSync(rows)),chunk('IEND',Buffer.alloc(0))]);
}

export function prepareAssets(PNG, root) {
    const reports={};
    for(const kind of ['exterior','interior']) {
        const palette=palettes[kind];
        const source=PNG.sync.read(fs.readFileSync(path.join(root,'graphics/custom/restoration_inn/source',kind+'_generated.png')));
        function sample(rect,w=16,h=16,trim=false) {
            let [x,y,rw,rh]=rect;
            if(trim) {
                const occupied=[];
                for(let sy=Math.ceil(y);sy<Math.floor(y+rh);sy++) for(let sx=Math.ceil(x);sx<Math.floor(x+rw);sx++)
                    if(source.data[(sy*source.width+sx)*4+3]>=224) occupied.push([sx,sy]);
                if(occupied.length) {
                    const xs=occupied.map(p=>p[0]),ys=occupied.map(p=>p[1]);
                    x=Math.min(...xs); y=Math.min(...ys); rw=Math.max(...xs)-x+1; rh=Math.max(...ys)-y+1;
                }
            }
            const out=new Uint8Array(w*h);
            for(let dy=0;dy<h;dy++) for(let dx=0;dx<w;dx++) {
                const sx=Math.min(source.width-1,Math.floor(x+(dx+0.5)*rw/w));
                const sy=Math.min(source.height-1,Math.floor(y+(dy+0.5)*rh/h));
                const at=(sy*source.width+sx)*4;
                if(source.data[at+3]<192) continue;
                let best=1, distance=Infinity;
                for(let i=1;i<16;i++) {
                    const c=palette[i], d=(c[0]-source.data[at])**2+(c[1]-source.data[at+1])**2+(c[2]-source.data[at+2])**2;
                    if(d<distance) { best=i; distance=d; }
                }
                out[dy*w+dx]=best;
            }
            return out;
        }
        const cells=Array.from({length:64},(_,i)=>sample([(i%8)*source.width/8,Math.floor(i/8)*source.height/8,source.width/8,source.height/8],16,16,kind==='interior'));
        function object(rect,w,h,ids) {
            const image=sample(rect,w,h,true);
            for(let row=0;row<h/16;row++) for(let col=0;col<w/16;col++) {
                const cell=new Uint8Array(256);
                for(let y=0;y<16;y++) cell.set(image.subarray((row*16+y)*w+col*16,(row*16+y)*w+col*16+16),y*16);
                cells[ids[row*(w/16)+col]]=cell;
            }
        }
        if(kind==='interior') {
            cells[0]=sample([36,36,112,112]);
            cells[1]=sample([195,36,112,112]);
            cells[2]=sample([347,42,110,110]);
            cells[3]=sample([500,42,110,110]);
            // Furniture is extracted as complete objects before slicing into 16-pixel metatiles.
            object([150,625,170,180],16,32,[32,33]);
            object([730,630,180,170],16,16,[34]);
            object([24,478,515,150],48,16,[24,25,26]);
            object([20,938,300,148],32,32,[48,49,50,51]);
            object([385,930,580,160],48,16,[52,53,54]);
            object([473,1090,310,150],32,16,[59,60]);
            object([778,1090,308,150],32,16,[61,62]);
            // Keep fully opaque repeats for floors; the source's atlas gutters are not game tiles.
            for(const i of [0,1,2,3,4,5,6,7,8,9,11]) {
                const cell=cells[i];
                for(let p=0;p<cell.length;p++) if(!cell[p]) cell[p]=(i>=7?8:i>=2&&i<=5?6:3);
            }
        }
        const atlas=new Uint8Array(128*128);
        for(let i=0;i<64;i++) for(let y=0;y<16;y++)
            atlas.set(cells[i].subarray(y*16,y*16+16),(Math.floor(i/8)*16+y)*128+(i%8)*16);
        const tiles=[new Uint8Array(64)], tileMap=new Map([[Buffer.from(tiles[0]).toString('hex'),0]]), refs=[];
        for(const cell of cells) {
            const indices=[];
            for(let by=0;by<2;by++) for(let bx=0;bx<2;bx++) {
                const t=new Uint8Array(64);
                for(let y=0;y<8;y++) t.set(cell.subarray((by*8+y)*16+bx*8,(by*8+y)*16+bx*8+8),y*8);
                const key=Buffer.from(t).toString('hex');
                if(!tileMap.has(key)) { tileMap.set(key,tiles.length); tiles.push(t); }
                indices.push(tileMap.get(key));
            }
            refs.push(indices);
        }
        const solidTile=tiles.length; tiles.push(new Uint8Array(64).fill(1));
        const tileRows=Math.ceil(tiles.length/16), sheet=new Uint8Array(128*tileRows*8);
        tiles.forEach((t,i)=>{for(let y=0;y<8;y++) sheet.set(t.subarray(y*8,y*8+8),(Math.floor(i/16)*8+y)*128+i%16*8);});
        if(tiles.length>512) throw Error('Secondary tile limit exceeded');
        const dir=path.join(root,'data/tilesets/secondary/restoration_inn_'+kind);
        fs.mkdirSync(path.join(dir,'palettes'),{recursive:true});
        fs.writeFileSync(path.join(dir,'tiles.png'),indexedFile(128,tileRows*8,sheet,palette));
        const pal='JASC-PAL\n0100\n16\n'+palette.map(c=>c.join(' ')).join('\n')+'\n';
        for(let i=0;i<16;i++) fs.writeFileSync(path.join(dir,'palettes',String(i).padStart(2,'0')+'.pal'),pal);
        const definitions=refs.map((own,i)=> {
            let bottom=own, top=[0,0,0,0];
            if(kind==='interior'&&i>=7&&i<24&&i!==8) { bottom=refs[i<18?8:0]; top=own; }
            if(kind==='interior'&&i>=24) { bottom=refs[(i===32||i===33||i===34||i===40||i===41)?2:0]; top=own; }
            if(kind==='exterior'&&i>=40) { bottom=refs[i>=56&&i<=59?6:3]; top=own; }
            return [...bottom,...top].map(t=>(512+t)|(6<<12));
        });
        definitions.push([...definitions[kind==='exterior'?33:14]]); // 64: north/entrance warp
        definitions.push([...definitions[kind==='exterior'?4:6]]); // 65: south exit mat
        definitions.push([...Array(4).fill((512+solidTile)|(6<<12)),...Array(4).fill(512|(6<<12))]); // 66: solid dark surround
        const raw=Buffer.alloc(definitions.length*16),attrs=Buffer.alloc(definitions.length*2);
        definitions.forEach((d,i)=>d.forEach((t,j)=>raw.writeUInt16LE(t,i*16+j*2)));
        attrs.writeUInt16LE(0x60,64*2); attrs.writeUInt16LE(0x65,65*2);
        fs.writeFileSync(path.join(dir,'metatiles.bin'),raw);
        fs.writeFileSync(path.join(dir,'metatile_attributes.bin'),attrs);
        const artDir=path.join(root,'graphics/custom/restoration_inn');
        fs.writeFileSync(path.join(artDir,kind+'_metatiles.png'),indexedFile(128,128,atlas,palette));
        const zoom=new Uint8Array(512*512);
        for(let y=0;y<512;y++) for(let x=0;x<512;x++) zoom[y*512+x]=atlas[Math.floor(y/4)*128+Math.floor(x/4)];
        fs.writeFileSync(path.join(artDir,kind+'_preview_4x.png'),indexedFile(512,512,zoom,palette));
        reports[kind]={metatiles:definitions.length,uniqueTiles:tiles.length,tileCapacity:512,paletteBank:6,
            bitDepth:4,colors:16,sha256:crypto.createHash('sha256').update(fs.readFileSync(path.join(dir,'tiles.png'))).digest('hex')};
    }
    fs.writeFileSync(path.join(root,'graphics/custom/restoration_inn/validation.json'),JSON.stringify(reports,null,2)+'\n');
    return reports;
}

function paletteFromPng(buffer) {
    let pos=8;
    while(pos<buffer.length) {
        const size=buffer.readUInt32BE(pos), name=buffer.toString('ascii',pos+4,pos+8);
        if(name==='PLTE') return Array.from({length:size/3},(_,i)=>buffer.subarray(pos+8+i*3,pos+11+i*3).toString('hex'));
        pos+=size+12;
    }
    throw Error('Expected indexed PNG');
}

export function renderMap(PNG,root,name,output) {
    const dirs={gTileset_General:'primary/general',gTileset_Building:'primary/building',gTileset_Lilycove:'secondary/lilycove',
        gTileset_RestorationInnExterior:'secondary/restoration_inn_exterior',gTileset_RestorationInnInterior:'secondary/restoration_inn_interior'};
    const layouts=JSON.parse(fs.readFileSync(path.join(root,'data/layouts/layouts.json'))).layouts;
    const layout=layouts.find(l=>l&&l.name===name+'_Layout');
    if(!layout) throw Error('Unknown layout '+name);
    const data=['primary_tileset','secondary_tileset'].map(k=>{
        const dir=path.join(root,'data/tilesets',dirs[layout[k]]), raw=fs.readFileSync(path.join(dir,'tiles.png'));
        const image=PNG.sync.read(raw), pal=paletteFromPng(raw), lookup=new Map(pal.map((c,i)=>[c,i]));
        const indices=new Uint8Array(image.width*image.height);
        for(let i=0;i<indices.length;i++) indices[i]=lookup.get(image.data.subarray(i*4,i*4+3).toString('hex'));
        return {dir,image,indices,meta:fs.readFileSync(path.join(dir,'metatiles.bin'))};
    });
    const palettes=Array.from({length:16},(_,i)=>{
        const p=path.join(data[i>=6?1:0].dir,'palettes',String(i).padStart(2,'0')+'.pal');
        return fs.readFileSync(p,'utf8').trim().split(/\r?\n/).slice(3,19).map(l=>l.trim().split(/\s+/).map(Number));
    });
    const grid=fs.readFileSync(path.join(root,layout.blockdata_filepath));
    const result=new PNG({width:layout.width*16,height:layout.height*16});
    const cache=new Map();
    function getMeta(id) {
        if(cache.has(id)) return cache.get(id);
        const d=data[id>=512?1:0], start=(id%512)*16, pixels=Buffer.alloc(16*16*4);
        for(let layer=0;layer<2;layer++) for(let part=0;part<4;part++) {
            const word=d.meta.readUInt16LE(start+(layer*4+part)*2), tile=word&1023, set=data[tile>=512?1:0];
            for(let y=0;y<8;y++) for(let x=0;x<8;x++) {
                const sx=(tile%512)%(set.image.width/8)*8+((word&0x400)?7-x:x);
                const sy=Math.floor((tile%512)/(set.image.width/8))*8+((word&0x800)?7-y:y);
                const color=set.indices[sy*set.image.width+sx];
                if(layer&&color===0) continue;
                const p=((Math.floor(part/2)*8+y)*16+(part%2)*8+x)*4;
                const rgb=palettes[word>>12][color];
                pixels[p]=rgb[0];pixels[p+1]=rgb[1];pixels[p+2]=rgb[2];pixels[p+3]=255;
            }
        }
        cache.set(id,pixels);return pixels;
    }
    for(let y=0;y<layout.height;y++) for(let x=0;x<layout.width;x++) {
        const p=getMeta(grid.readUInt16LE((y*layout.width+x)*2)&1023);
        for(let row=0;row<16;row++) p.copy(result.data,((y*16+row)*result.width+x*16)*4,row*64,(row+1)*64);
    }
    fs.mkdirSync(path.dirname(output),{recursive:true});fs.writeFileSync(output,PNG.sync.write(result));
    return {name,width:result.width,height:result.height,output};
}
