# Chain source

Generated with the built-in `image_gen` tool, transparent background.
Reference: `graphics/custom/pokevirus/comparison_preview_4x.png` (colors and poison-chain motif only).

Prompt:

> Use case: stylized-concept. Asset type: one reusable pixel-art chain segment for an actual GBA battle-screen transition. Use the attached reference ONLY for the dark burgundy crimson-magenta double-helix poison chains on the faceless virus creature; do not include the creature. Create a SINGLE narrow VERTICAL double-helix chain, 2 twisted strands of articulated ring-like links with tiny pale lime toxic seams. Dark near-black plum outline, burgundy red shadow, muted magenta red highlight, very sparse poison yellow-green accents. True clean Generation III 16-color pixel-art technique, bold clusters, hard edges, NO anti-alias, NO gradients, NO glow, NO text, NO face, NO sphere, NO UI, NO hands. Intended final tile is 16 pixels wide by 32 pixels tall: show exactly one full vertical repeating double-helix period as enlarged logical pixels, simple enough to remain readable after nearest-neighbor reduction. The chain occupies the center of a transparent canvas, has a narrow roughly 2:1 tall shape, runs vertically straight, touches the top and bottom at the same horizontal positions for repeating vertically; generous transparent margins at left and right. Both strands must be physically recognizable as interlocking chain links rather than merely smooth DNA ribbon. Exactly one chain sample, not a grid or sprite sheet. Transparent background.

The generated source is converted to a GBA-valid 32x64 OBJ canvas, with a narrower centered chain, by `prepare_assets.mjs`. Source dimensions are not game dimensions.
