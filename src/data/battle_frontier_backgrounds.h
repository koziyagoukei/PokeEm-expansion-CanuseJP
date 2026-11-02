// Display-only assets. Battle environment mechanics remain unchanged.
static const u32 sFrontierBackgroundTiles_Tower[] = INCGFX_U32("graphics/battle_environment/frontier/tower/tiles.png", ".4bpp.smol");
static const u32 sFrontierBackgroundMap_Tower[] = INCGFX_U32("graphics/battle_environment/frontier/tower/map.bin", ".smolTM");
static const u16 sFrontierBackgroundPalette_Tower[] = INCGFX_U16("graphics/battle_environment/frontier/tower/palette.pal", ".gbapal");

static const u32 sFrontierBackgroundTiles_Dome[] = INCGFX_U32("graphics/battle_environment/frontier/dome/tiles.png", ".4bpp.smol");
static const u32 sFrontierBackgroundMap_Dome[] = INCGFX_U32("graphics/battle_environment/frontier/dome/map.bin", ".smolTM");
static const u16 sFrontierBackgroundPalette_Dome[] = INCGFX_U16("graphics/battle_environment/frontier/dome/palette.pal", ".gbapal");

static const u32 sFrontierBackgroundTiles_Palace[] = INCGFX_U32("graphics/battle_environment/frontier/palace/tiles.png", ".4bpp.smol");
static const u32 sFrontierBackgroundMap_Palace[] = INCGFX_U32("graphics/battle_environment/frontier/palace/map.bin", ".smolTM");
static const u16 sFrontierBackgroundPalette_Palace[] = INCGFX_U16("graphics/battle_environment/frontier/palace/palette.pal", ".gbapal");

static const u32 sFrontierBackgroundTiles_Arena[] = INCGFX_U32("graphics/battle_environment/frontier/arena/tiles.png", ".4bpp.smol");
static const u32 sFrontierBackgroundMap_Arena[] = INCGFX_U32("graphics/battle_environment/frontier/arena/map.bin", ".smolTM");
static const u16 sFrontierBackgroundPalette_Arena[] = INCGFX_U16("graphics/battle_environment/frontier/arena/palette.pal", ".gbapal");

static const u32 sFrontierBackgroundTiles_Factory[] = INCGFX_U32("graphics/battle_environment/frontier/factory/tiles.png", ".4bpp.smol");
static const u32 sFrontierBackgroundMap_Factory[] = INCGFX_U32("graphics/battle_environment/frontier/factory/map.bin", ".smolTM");
static const u16 sFrontierBackgroundPalette_Factory[] = INCGFX_U16("graphics/battle_environment/frontier/factory/palette.pal", ".gbapal");

static const u32 sFrontierBackgroundTiles_Pike[] = INCGFX_U32("graphics/battle_environment/frontier/pike/tiles.png", ".4bpp.smol");
static const u32 sFrontierBackgroundMap_Pike[] = INCGFX_U32("graphics/battle_environment/frontier/pike/map.bin", ".smolTM");
static const u16 sFrontierBackgroundPalette_Pike[] = INCGFX_U16("graphics/battle_environment/frontier/pike/palette.pal", ".gbapal");

static const u32 sFrontierBackgroundTiles_Pyramid[] = INCGFX_U32("graphics/battle_environment/frontier/pyramid/tiles.png", ".4bpp.smol");
static const u32 sFrontierBackgroundMap_Pyramid[] = INCGFX_U32("graphics/battle_environment/frontier/pyramid/map.bin", ".smolTM");
static const u16 sFrontierBackgroundPalette_Pyramid[] = INCGFX_U16("graphics/battle_environment/frontier/pyramid/palette.pal", ".gbapal");

static const struct
{
    u32 battleType;
    struct BattleBackgroundGraphics graphics;
} sFrontierBattleBackgrounds[] =
{
    {BATTLE_TYPE_BATTLE_TOWER, {sFrontierBackgroundTiles_Tower, sFrontierBackgroundMap_Tower, sFrontierBackgroundPalette_Tower}},
    {BATTLE_TYPE_DOME, {sFrontierBackgroundTiles_Dome, sFrontierBackgroundMap_Dome, sFrontierBackgroundPalette_Dome}},
    {BATTLE_TYPE_PALACE, {sFrontierBackgroundTiles_Palace, sFrontierBackgroundMap_Palace, sFrontierBackgroundPalette_Palace}},
    {BATTLE_TYPE_ARENA, {sFrontierBackgroundTiles_Arena, sFrontierBackgroundMap_Arena, sFrontierBackgroundPalette_Arena}},
    {BATTLE_TYPE_FACTORY, {sFrontierBackgroundTiles_Factory, sFrontierBackgroundMap_Factory, sFrontierBackgroundPalette_Factory}},
    {BATTLE_TYPE_PIKE, {sFrontierBackgroundTiles_Pike, sFrontierBackgroundMap_Pike, sFrontierBackgroundPalette_Pike}},
    {BATTLE_TYPE_PYRAMID, {sFrontierBackgroundTiles_Pyramid, sFrontierBackgroundMap_Pyramid, sFrontierBackgroundPalette_Pyramid}},
};
