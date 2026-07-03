import sys, re

src, dst = sys.argv[1], sys.argv[2]
limit = int(sys.argv[3]) if len(sys.argv) >= 4 else None

text = open(src, "r", encoding="utf-8", errors="replace").read()
vals = re.findall(r"0x([0-9a-fA-F]{8})", text)

colors = []
for v in vals:
    n = int(v, 16)
    colors.append(((n >> 16) & 255, (n >> 8) & 255, n & 255))

if limit:
    colors = colors[:limit]

with open(dst, "w", encoding="ascii", newline="\n") as f:
    f.write("JASC-PAL\n0100\n")
    f.write(f"{len(colors)}\n")
    for r, g, b in colors:
        f.write(f"{r} {g} {b}\n")
