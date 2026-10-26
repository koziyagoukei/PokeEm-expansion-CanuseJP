import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';

export function prepareAssets(PNG, root) {
    const directory=path.join(root,'graphics/custom/frontier_opening');
    const report={};
    for(const name of ['forest','meeting_hands','network','ridge']) {
        const source=PNG.sync.read(fs.readFileSync(path.join(directory,'source',name+'_generated.png')));
        const pixels=[], histogram=new Map();
        for(let y=0;y<160;y++) for(let x=0;x<240;x++) {
            const at=(Math.floor((y+0.5)*source.height/160)*source.width+Math.floor((x+0.5)*source.width/240))*4;
            const rgb=[0,1,2].map(c=>Math.round(source.data[at+c]/8)*8).map(c=>Math.min(248,c));
            const key=rgb.join(',');
            pixels.push(rgb);
            const entry=histogram.get(key);
            if(entry) entry.count++;
            else histogram.set(key,{rgb,count:1});
        }
        const boxes=[[...histogram.values()]];
        function extent(box) {
            return [0,1,2].map(c=>Math.max(...box.map(p=>p.rgb[c]))-Math.min(...box.map(p=>p.rgb[c])));
        }
        while(boxes.length<63) {
            let selected=-1, score=-1;
            for(let i=0;i<boxes.length;i++) {
                const e=extent(boxes[i]), s=Math.max(...e)*Math.sqrt(boxes[i].reduce((n,p)=>n+p.count,0));
                if(boxes[i].length>1&&s>score) { selected=i;score=s; }
            }
            if(selected<0) break;
            const box=boxes.splice(selected,1)[0], ranges=extent(box), axis=ranges.indexOf(Math.max(...ranges));
            box.sort((a,b)=>a.rgb[axis]-b.rgb[axis]);
            const half=box.reduce((n,p)=>n+p.count,0)/2;
            let count=0, split=1;
            for(;split<box.length;split++) { count+=box[split-1].count; if(count>=half) break; }
            split=Math.min(split,box.length-1);
            boxes.push(box.slice(0,split),box.slice(split));
        }
        const palette=[[0,0,0],...boxes.map(box=>{
            const total=box.reduce((n,p)=>n+p.count,0);
            return [0,1,2].map(c=>Math.round(box.reduce((n,p)=>n+p.rgb[c]*p.count,0)/total/8)*8);
        })];
        const indices=Buffer.alloc(240*160), lookup=new Map(), image=new PNG({width:240,height:160});
        for(let i=0;i<pixels.length;i++) {
            const rgb=pixels[i], key=rgb.join(',');
            if(!lookup.has(key)) {
                let best=0, distance=Infinity;
                for(let c=0;c<palette.length;c++) {
                    const d=palette[c].reduce((sum,v,k)=>sum+(v-rgb[k])**2,0);
                    if(d<distance) {best=c;distance=d;}
                }
                lookup.set(key,best);
            }
            const index=lookup.get(key);indices[i]=index;
            image.data.set([...palette[index],255],i*4);
        }
        const pal=Buffer.alloc(512);
        palette.forEach((rgb,i)=>pal.writeUInt16LE((rgb[0]>>3)|((rgb[1]>>3)<<5)|((rgb[2]>>3)<<10),i*2));
        fs.writeFileSync(path.join(directory,name+'.pixels.bin'),indices);
        fs.writeFileSync(path.join(directory,name+'.palette.bin'),pal);
        fs.writeFileSync(path.join(directory,name+'.png'),PNG.sync.write(image));
        const zoom=new PNG({width:960,height:640});
        for(let y=0;y<640;y++) for(let x=0;x<960;x++)
            image.data.copy(zoom.data,(y*960+x)*4,(Math.floor(y/4)*240+Math.floor(x/4))*4,(Math.floor(y/4)*240+Math.floor(x/4))*4+4);
        fs.writeFileSync(path.join(directory,name+'_4x.png'),PNG.sync.write(zoom));
        report[name]={width:240,height:160,colors:palette.length,bytes:indices.length,
            sha256:crypto.createHash('sha256').update(indices).digest('hex')};
    }
    fs.writeFileSync(path.join(directory,'validation.json'),JSON.stringify(report,null,2)+'\n');
    return report;
}
