# Title Background Generation Record

Mode: built-in image generation/editing tool. No external API key or fallback
CLI was used. Only the final clean illustration is adopted as
`source/generated.png`; the realistic/grainier initial draft is not adopted.

## Initial Brief

A 3:2 GBA title-screen still life: four square/rectangular brown Hokkaido-style
kibidango on an off-white plate, a resting elderly hand beside it, quiet dark
negative space behind the existing logo. No lettering, UI, additional characters,
dragons or utensils. The first result had too much realistic texture.

## User's Style Correction

The user explicitly requested less grain, a look consistent with the game, and
no realism. The following redraw prompt was applied to the first image:

> Keep the concept of a plate of FOUR SQUARE Hokkaido-style brown kibidango confections and an elderly person's resting hand next to it, but REDRAW COMPLETELY as clean, simple, charming Pokemon Ruby/Sapphire/Emerald-era GBA game illustration pixel art. The user explicitly says NO REALISM and NO GRAIN. Replace the realistic hand with a stylized cartoon hand in the same visual language as GBA Pokemon human sprites: simple rounded silhouette, chunky fingers, dark 1-pixel-like outlines, just 3 or 4 concise wrinkle lines to show age. No realistic skin texture, no pores, no mottling, no hatching. The sweets must be plainly flat SQUARE brown chewy blocks, not round dango, not textured cookies; flat simple brown shapes with only two or three clean shading tones and a small highlight. A simple off-white ceramic plate with one restrained rim line. The table is a very smooth dark muted walnut/navy surface, at most TWO or THREE broad clean wood seams, absolutely NO individual wood fibers, NO noise, NO scattered tiny pixel texture, NO glitter, NO stipple, NO dithering. Think an actual clean 240x160 pixel GBA opening illustration enlarged with nearest-neighbor: large deliberate flat color clusters, bold readable shapes, approximately 16 carefully coordinated colors total. Landscape 3:2. Layout: TOP 57 PERCENT completely quiet and dark for a large game logo, no subjects there. Plate at lower left/center, spanning x18%-58%, y59%-87%; elderly hand to its right spanning x62%-97%, y61%-91%, wrist exits right edge. Bottom 8 percent quiet for copyright. Keep all food and hand below the logo area; leave a narrow clean dark band around y52%-58% for PRESS START text. Gentle nostalgic and slightly wistful mood, warm accents but cool dark background. No text, no logo, no UI, no border, no realistic rendering. Pixel art must look like a game asset, not a pixelated photograph.

## Final Layout Edit

The clean redraw was then edited with this final prompt:

> Keep this exact clean cartoon GBA pixel-art look, the four square brown sweets, ceramic plate, stylized elderly hand and the same colors. Single layout correction for use behind an existing game title logo: shrink the plate and sweets by about 15 percent and shift them DOWN. The highest visible point of the plate or sweets must be no higher than 61 percent of the entire image height, and the lowest point no lower than 89 percent. Plate center around 37%,76%. Shrink the hand slightly and shift it down so it occupies x62%-98%, y64%-90%; no part of the hand above 62% height. Leave the entire TOP 60 PERCENT as absolutely empty smooth very dark blue-brown tabletop/negative space (no horizontal edge/horizon, no objects, almost no detail). This empty area will contain an existing logo and PRESS START label. Keep bottom 8% quiet. Remove residual grain and mottled shading: flat color fills, only 2-3 simple shadow tones for each object, clear pixel-cluster outlines, NO noise, NO dithering, NO realistic skin texture. Do not add anything. No text, no logo, no border.

The final source is converted with nearest-neighbor sampling and a fixed
16-color palette. There is no error-diffusion noise or texture synthesis in the
converter. The actual logo and labels are rendered by the game, not baked in.
