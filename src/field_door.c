#include "global.h"
#include "event_data.h"
#include "field_door.h"
#include "field_camera.h"
#include "fieldmap.h"
#include "metatile_behavior.h"
#include "task.h"
#include "tilesets.h"
#include "constants/songs.h"
#include "constants/metatile_labels.h"

#define DOOR_SOUND_NORMAL  0
#define DOOR_SOUND_SLIDING 1
#define DOOR_SOUND_ARENA   2

enum DoorSize
{
    DOOR_SIZE_1x1,
    DOOR_SIZE_1x2,
    DOOR_SIZE_2x2_LEFT,
    DOOR_SIZE_2x2_RIGHT,
    DOOR_SIZE_COUNT,
};

struct DoorGraphics
{
    u16 metatileNum;
    const struct Tileset *tileset;
    u8 sound;
    enum DoorSize size:8;
    const void *tiles;
    const void *palettes;
};

struct DoorAnimFrame
{
    u8 time;
    u16 offset;
};

struct DoorSizeInfo
{
    const struct DoorAnimFrame *openAnimFrames;
    const struct DoorAnimFrame *closeAnimFrames;
};

static bool8 ShouldUseMultiCorridorDoor(void);

static const u16 sDoorNullPalette1[16] = {};
static const u16 sDoorNullPalette2[16] = {};
static const u16 sDoorNullPalette3[16] = {};
static const u8 sDoorAnimTiles_Lilycove[] = INCGFX_U8("graphics/door_anims/lilycove.png", ".4bpp");
static const u16 sDoorNullPalette4[16] = {};
static const u8 sDoorAnimTiles_LilycoveWooden[] = INCGFX_U8("graphics/door_anims/lilycove_wooden.png", ".4bpp");
static const u16 sDoorNullPalette5[16] = {};
static const u8 sDoorAnimTiles_General[] = INCGFX_U8("graphics/door_anims/general.png", ".4bpp");
static const u16 sDoorNullPalette6[16] = {};
static const u8 sDoorAnimTiles_PokeCenter[] = INCGFX_U8("graphics/door_anims/poke_center.png", ".4bpp");
static const u8 sDoorAnimTiles_Gym[] = INCGFX_U8("graphics/door_anims/gym.png", ".4bpp");
static const u8 sDoorAnimTiles_PokeMart[] = INCGFX_U8("graphics/door_anims/poke_mart.png", ".4bpp");
static const u16 sDoorNullPalette7[16] = {};
static const u16 sDoorNullPalette8[16] = {};
static const u8 sFiller1[0x5900] = {};
static const u8 sDoorAnimTiles_UnusedTops[] = INCGFX_U8("graphics/door_anims/unused_top.png", ".4bpp");
static const u8 sFiller2[0x140] = {};
static const u8 sDoorAnimTiles_UnusedBottoms[] = INCGFX_U8("graphics/door_anims/unused_bottom.png", ".4bpp");
static const u16 sDoorNullPalette11[16] = {};
static const u16 sDoorNullPalette12[16] = {};
static const u16 sDoorNullPalette13[16] = {};
static const u16 sDoorNullPalette14[16] = {};
static const u16 sDoorNullPalette15[16] = {};
static const u8 sDoorAnimTiles_Contest[] = INCGFX_U8("graphics/door_anims/contest.png", ".4bpp");
static const u16 sDoorNullPalette16[16] = {};
static const u16 sDoorNullPalette17[16] = {};
static const u16 sDoorNullPalette18[16] = {};
static const u16 sDoorNullPalette19[16] = {};
static const u16 sDoorNullPalette20[16] = {};
static const u16 sDoorNullPalette21[16] = {};
static const u16 sDoorNullPalette22[16] = {};
static const u16 sDoorNullPalette23[16] = {};
static const u8 sDoorAnimTiles_LilycoveDeptStore[] = INCGFX_U8("graphics/door_anims/lilycove_dept_store.png", ".4bpp");
static const u16 sDoorNullPalette24[16] = {};
static const u8 sDoorAnimTiles_SafariZone[] = INCGFX_U8("graphics/door_anims/safari_zone.png", ".4bpp");
static const u16 sDoorNullPalette25[16] = {};
static const u16 sDoorNullPalette26[16] = {};
static const u8 sDoorAnimTiles_CableClub[] = INCGFX_U8("graphics/door_anims/cable_club.png", ".4bpp");
static const u16 sDoorNullPalette27[16] = {};
static const u8 sDoorAnimTiles_AbandonedShip[] = INCGFX_U8("graphics/door_anims/abandoned_ship.png", ".4bpp");
static const u16 sDoorNullPalette28[16] = {};
static const u16 sDoorNullPalette29[16] = {};
static const u16 sDoorNullPalette30[16] = {};
static const u8 sDoorAnimTiles_LilycoveDeptStoreElevator[] = INCGFX_U8("graphics/door_anims/lilycove_dept_store_elevator.png", ".4bpp");
static const u16 sDoorNullPalette31[16] = {};
static const u16 sDoorNullPalette32[16] = {};
static const u8 sDoorAnimTiles_BattleTowerElevator[] = INCGFX_U8("graphics/door_anims/battle_tower_elevator.png", ".4bpp");
static const u16 sDoorNullPalette33[16] = {};
static const u8 sDoorAnimTiles_UnusedBattleFrontier[] = INCGFX_U8("graphics/door_anims/unused_battle_frontier.png", ".4bpp");
static const u16 sDoorNullPalette34[16] = {};
static const u8 sDoorAnimTiles_BattleDome[] = INCGFX_U8("graphics/door_anims/battle_dome.png", ".4bpp");
static const u16 sDoorNullPalette35[16] = {};
static const u8 sDoorAnimTiles_BattleFactory[] = INCGFX_U8("graphics/door_anims/battle_factory.png", ".4bpp");
static const u16 sDoorNullPalette36[16] = {};
static const u8 sDoorAnimTiles_BattleTower[] = INCGFX_U8("graphics/door_anims/battle_tower.png", ".4bpp");
static const u16 sDoorNullPalette37[16] = {};
static const u8 sDoorAnimTiles_BattleArena[] = INCGFX_U8("graphics/door_anims/battle_arena.png", ".4bpp");
static const u16 sDoorNullPalette38[16] = {};
static const u8 sDoorAnimTiles_BattleArenaLobby[] = INCGFX_U8("graphics/door_anims/battle_arena_lobby.png", ".4bpp");
static const u16 sDoorNullPalette39[16] = {};
static const u8 sDoorAnimTiles_BattleDomeLobby[] = INCGFX_U8("graphics/door_anims/battle_dome_lobby.png", ".4bpp");
static const u16 sDoorNullPalette40[16] = {};
static const u8 sDoorAnimTiles_BattlePalaceLobby[] = INCGFX_U8("graphics/door_anims/battle_palace_lobby.png", ".4bpp");
static const u16 sDoorNullPalette41[16] = {};
static const u8 sDoorAnimTiles_BattleDomeCorridor[] = INCGFX_U8("graphics/door_anims/battle_dome_corridor.png", ".4bpp");
static const u16 sDoorNullPalette42[16] = {};
static const u8 sDoorAnimTiles_BattleTowerMultiCorridor[] = INCGFX_U8("graphics/door_anims/battle_tower_multi_corridor.png", ".4bpp", "-mwidth 2 -mheight 4");
static const u16 sDoorNullPalette43[16] = {};
static const u8 sDoorAnimTiles_BattleFrontier[] = INCGFX_U8("graphics/door_anims/battle_frontier.png", ".4bpp");
static const u16 sDoorNullPalette44[16] = {};
static const u8 sDoorAnimTiles_BattleFrontierSliding[] = INCGFX_U8("graphics/door_anims/battle_frontier_sliding.png", ".4bpp");
static const u16 sDoorNullPalette45[16] = {};
static const u8 sDoorAnimTiles_BattleDomePreBattleRoom[] = INCGFX_U8("graphics/door_anims/battle_dome_pre_battle_room.png", ".4bpp");
static const u16 sDoorNullPalette46[16] = {};
static const u16 sDoorNullPalette47[16] = {};
static const u16 sDoorNullPalette48[16] = {};
static const u16 sDoorNullPalette49[16] = {};

#if IS_FRLG

static const u8 sDoorAnimTiles_PokemonFanClub[] = INCGFX_U8("graphics/door_anims/pokemon_fan_club.png", ".4bpp");

#endif // IS_FRLG

static const struct DoorAnimFrame sDoorAnimFrames_Open1x1[] = {
    {4, -1},
    {4, 0 * TILE_SIZE_4BPP},
    {4, 4 * TILE_SIZE_4BPP},
    {4, 8 * TILE_SIZE_4BPP},
    {}
};

static const struct DoorAnimFrame sDoorAnimFrames_Close1x1[] = {
    {4, 8 * TILE_SIZE_4BPP},
    {4, 4 * TILE_SIZE_4BPP},
    {4, 0 * TILE_SIZE_4BPP},
    {4, -1},
    {}
};

static const struct DoorAnimFrame sDoorAnimFrames_Open1x2[] = {
    {4, -1},
    {4, 0 * TILE_SIZE_4BPP},
    {4, 8 * TILE_SIZE_4BPP},
    {4, 16 * TILE_SIZE_4BPP},
    {}
};

static const struct DoorAnimFrame sDoorAnimFrames_Close1x2[] = {
    {4, 16 * TILE_SIZE_4BPP},
    {4, 8 * TILE_SIZE_4BPP},
    {4, 0 * TILE_SIZE_4BPP},
    {4, -1},
    {}
};

static const struct DoorAnimFrame sDoorAnimFrames_Open2x2[] =
{
    {4, -1},
    {4, 0 * TILE_SIZE_4BPP},
    {4, 16 * TILE_SIZE_4BPP},
    {4, 32 * TILE_SIZE_4BPP},
    {0, 0},
};

static const struct DoorAnimFrame sDoorAnimFrames_Close2x2[] =
{
    {4, 32 * TILE_SIZE_4BPP},
    {4, 16 * TILE_SIZE_4BPP},
    {4, 0 * TILE_SIZE_4BPP},
    {4, -1},
    {}
};

static const struct DoorSizeInfo sDoorSizeInfo[DOOR_SIZE_COUNT] = 
{
    [DOOR_SIZE_1x1] =
    {
        .openAnimFrames = sDoorAnimFrames_Open1x1,
        .closeAnimFrames = sDoorAnimFrames_Close1x1,
    },
    [DOOR_SIZE_1x2] =
    {
        .openAnimFrames = sDoorAnimFrames_Open1x2,
        .closeAnimFrames = sDoorAnimFrames_Close1x2,
    },
    [DOOR_SIZE_2x2_LEFT] =
    {
        .openAnimFrames = sDoorAnimFrames_Open2x2,
        .closeAnimFrames = sDoorAnimFrames_Close2x2,
    },
    [DOOR_SIZE_2x2_RIGHT] =
    {
        .openAnimFrames = sDoorAnimFrames_Open2x2,
        .closeAnimFrames = sDoorAnimFrames_Close2x2,
    },
};

static const u8 sDoorAnimPalettes_General[] = {1, 1, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_PokeCenter[] = {1, 1, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_Gym[] = {5, 5, 5, 5, 5, 5, 5, 5};
static const u8 sDoorAnimPalettes_PokeMart[] = {0, 0, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_Lilycove[] = {8, 8, 8, 8, 8, 8, 8, 8};
static const u8 sDoorAnimPalettes_LilycoveWooden[] = {5, 5, 5, 5, 5, 5, 5, 5};
static const u8 sDoorAnimPalettes_Contest[] = {1, 1, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_LilycoveDeptStore[] = {5, 5, 5, 5, 5, 5, 5, 5};
static const u8 sDoorAnimPalettes_SafariZone[] = {9, 9, 9, 9, 9, 9, 9, 9};
static const u8 sDoorAnimPalettes_CableClub[] = {6, 6, 6, 6, 6, 6, 6, 6};
static const u8 sDoorAnimPalettes_LilycoveDeptStoreElevator[] = {6, 6, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_BattleTowerElevator[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_UnusedBattleFrontier[] = {9, 9, 9, 9, 9, 9, 9, 9};
static const u8 sDoorAnimPalettes_BattleDome[] = {1, 1, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_BattleFactory[] = {9, 9, 9, 9, 9, 9, 9, 9};
static const u8 sDoorAnimPalettes_BattleTower[] = {0, 0, 0, 0, 0, 0, 0, 0};
static const u8 sDoorAnimPalettes_BattleArena[] = {5, 5, 5, 5, 5, 5, 5, 5};
static const u8 sDoorAnimPalettes_BattleArenaLobby[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_BattleDomeLobby[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_BattlePalaceLobby[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_BattleDomeCorridor[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_BattleTowerMultiCorridor[] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_Unused[] = {1, 1, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_BattleFrontier[] = {1, 1, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_BattleDomePreBattleRoom[] = {9, 9, 7, 7, 7, 7, 7, 7};

#if IS_FRLG

static const u8 sDoorAnimPalettes_PokemonFanClub[] = {9, 9, 9, 9, 9, 9, 9, 9};

#endif // IS_FRLG

static const struct DoorGraphics sDoorAnimGraphicsTable[] =
{
    // Doors of tilesets no kept map uses (and every FRLG door) were removed with
    // the Hoenn mainland.
#if !IS_FRLG
    {
        .metatileNum = METATILE_General_Door,
        .tileset = &gTileset_General,
        .sound = DOOR_SOUND_NORMAL,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_General,
        .palettes = sDoorAnimPalettes_General
    },
    {
        .metatileNum = METATILE_General_Door_PokeCenter,
        .tileset = &gTileset_General,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_PokeCenter,
        .palettes = sDoorAnimPalettes_PokeCenter
    },
    {
        .metatileNum = METATILE_General_Door_Gym,
        .tileset = &gTileset_General,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_Gym,
        .palettes = sDoorAnimPalettes_Gym
    },
    {
        .metatileNum = METATILE_General_Door_PokeMart,
        .tileset = &gTileset_General,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_PokeMart,
        .palettes = sDoorAnimPalettes_PokeMart
    },
    {
        .metatileNum = METATILE_General_Door_Contest,
        .tileset = &gTileset_General,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_Contest,
        .palettes = sDoorAnimPalettes_Contest
    },
    {
        .metatileNum = METATILE_Lilycove_Door,
        .tileset = &gTileset_Lilycove,
        .sound = DOOR_SOUND_NORMAL,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_Lilycove,
        .palettes = sDoorAnimPalettes_Lilycove
    },
    {
        .metatileNum = METATILE_Lilycove_Door_Wooden,
        .tileset = &gTileset_Lilycove,
        .sound = DOOR_SOUND_NORMAL,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_LilycoveWooden,
        .palettes = sDoorAnimPalettes_LilycoveWooden
    },
    {
        .metatileNum = METATILE_Lilycove_Door_DeptStore,
        .tileset = &gTileset_Lilycove,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_LilycoveDeptStore,
        .palettes = sDoorAnimPalettes_LilycoveDeptStore
    },
    {
        .metatileNum = METATILE_Lilycove_Door_SafariZone,
        .tileset = &gTileset_Lilycove,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_SafariZone,
        .palettes = sDoorAnimPalettes_SafariZone
    },
    {
        .metatileNum = METATILE_PokemonCenter_Door_CableClub,
        .tileset = &gTileset_PokemonCenter,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_CableClub,
        .palettes = sDoorAnimPalettes_CableClub
    },
    {
        .metatileNum = METATILE_Shop_Door_Elevator,
        .tileset = &gTileset_Shop,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_LilycoveDeptStoreElevator,
        .palettes = sDoorAnimPalettes_LilycoveDeptStoreElevator
    },
    {
        .metatileNum = METATILE_BattleFrontier_Door_Elevator,
        .tileset = &gTileset_BattleFrontier,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattleTowerElevator,
        .palettes = sDoorAnimPalettes_BattleTowerElevator
    },
    {
        .metatileNum = METATILE_BattleFrontierOutsideWest_Door_BattleDome,
        .tileset = &gTileset_BattleFrontierOutsideWest,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattleDome,
        .palettes = sDoorAnimPalettes_BattleDome
    },
    {
        .metatileNum = METATILE_BattleFrontierOutsideWest_Door_BattleFactory,
        .tileset = &gTileset_BattleFrontierOutsideWest,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattleFactory,
        .palettes = sDoorAnimPalettes_BattleFactory
    },
    {
        .metatileNum = METATILE_BattleFrontierOutsideEast_Door_BattleTower,
        .tileset = &gTileset_BattleFrontierOutsideEast,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattleTower,
        .palettes = sDoorAnimPalettes_BattleTower
    },
    {
        .metatileNum = METATILE_BattleFrontierOutsideEast_Door_BattleArena,
        .tileset = &gTileset_BattleFrontierOutsideEast,
        .sound = DOOR_SOUND_NORMAL,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattleArena,
        .palettes = sDoorAnimPalettes_BattleArena
    },
    {
        .metatileNum = METATILE_BattleArena_Door,
        .tileset = &gTileset_BattleArena,
        .sound = DOOR_SOUND_ARENA,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattleArenaLobby,
        .palettes = sDoorAnimPalettes_BattleArenaLobby
    },
    {
        .metatileNum = METATILE_BattleDome_Door_Lobby,
        .tileset = &gTileset_BattleDome,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattleDomeLobby,
        .palettes = sDoorAnimPalettes_BattleDomeLobby
    },
    {
        .metatileNum = METATILE_BattlePalace_Door,
        .tileset = &gTileset_BattlePalace,
        .sound = DOOR_SOUND_NORMAL,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattlePalaceLobby,
        .palettes = sDoorAnimPalettes_BattlePalaceLobby
    },
    {
        .metatileNum = METATILE_BattleDome_Door_Corridor,
        .tileset = &gTileset_BattleDome,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattleDomeCorridor,
        .palettes = sDoorAnimPalettes_BattleDomeCorridor
    },
    {
        .metatileNum = METATILE_BattleFrontier_Door_MultiCorridor_Left,
        .tileset = &gTileset_BattleFrontier,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_2x2_LEFT,
        .tiles = sDoorAnimTiles_BattleTowerMultiCorridor,
        .palettes = sDoorAnimPalettes_BattleTowerMultiCorridor
    },
    {
        .metatileNum = METATILE_BattleFrontier_Door_MultiCorridor_Right,
        .tileset = &gTileset_BattleFrontier,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_2x2_RIGHT,
        .tiles = sDoorAnimTiles_BattleTowerMultiCorridor,
        .palettes = sDoorAnimPalettes_BattleTowerMultiCorridor
    },
    {
        .metatileNum = METATILE_BattleFrontierOutsideWest_Door,
        .tileset = &gTileset_BattleFrontierOutsideWest,
        .sound = DOOR_SOUND_NORMAL,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattleFrontier,
        .palettes = sDoorAnimPalettes_BattleFrontier
    },
    {
        .metatileNum = METATILE_BattleFrontierOutsideWest_Door_Sliding,
        .tileset = &gTileset_BattleFrontierOutsideWest,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattleFrontierSliding,
        .palettes = sDoorAnimPalettes_BattleFrontier
    },
    {
        .metatileNum = METATILE_BattleFrontierOutsideWest_Door,
        .tileset = &gTileset_BattleFrontierOutsideEast,
        .sound = DOOR_SOUND_NORMAL,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattleFrontier,
        .palettes = sDoorAnimPalettes_BattleFrontier
    },
    {
        .metatileNum = METATILE_BattleFrontierOutsideWest_Door_Sliding,
        .tileset = &gTileset_BattleFrontierOutsideEast,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattleFrontierSliding,
        .palettes = sDoorAnimPalettes_BattleFrontier
    },
    {
        .metatileNum = METATILE_BattleDome_Door_PreBattleRoom,
        .tileset = &gTileset_BattleDome,
        .sound = DOOR_SOUND_SLIDING,
        .size = DOOR_SIZE_1x2,
        .tiles = sDoorAnimTiles_BattleDomePreBattleRoom,
        .palettes = sDoorAnimPalettes_BattleDomePreBattleRoom
    },
#endif // !IS_FRLG
    {},
};

// NOTE: The tiles of a door's animation must be copied to VRAM because they are not already part of any given tileset.
//       This means that if there are any pre-existing tiles in this copied region that are visible when the door
//       animation is played they will be overwritten.
#define DOOR_TILE_START_SIZE1 (NUM_TILES_TOTAL - 8)
#define DOOR_TILE_START_SIZE2 (NUM_TILES_TOTAL - 16)

static void CopyDoorTilesToVram(const struct DoorGraphics *gfx, const struct DoorAnimFrame *frame)
{
    if (gfx->size == DOOR_SIZE_2x2_LEFT || gfx->size == DOOR_SIZE_2x2_RIGHT)
        CpuFastCopy(gfx->tiles + frame->offset, (void *)(VRAM + TILE_OFFSET_4BPP(DOOR_TILE_START_SIZE2)), 16 * TILE_SIZE_4BPP);
    else
        CpuFastCopy(gfx->tiles + frame->offset, (void *)(VRAM + TILE_OFFSET_4BPP(DOOR_TILE_START_SIZE1)), 8 * TILE_SIZE_4BPP);
}

static void BuildDoorTiles(u16 *tiles, u16 tileNum, const u8 *paletteNums)
{
    int i;
    u16 tile;

    // Only the first 4 tiles of each metatile (bottom layer) actually use the door tiles
    for (i = 0; i < 4; i++)
    {
        tile = *(paletteNums++) << 12;
        tiles[i] = tile | (tileNum + i);
    }

    // The remaining layers are left as tile 0 (with the same palette)
    for (; i < 8; i++)
    {
        tile = *(paletteNums++) << 12;
        tiles[i] = tile;
    }
}

static void DrawCurrentDoorAnimFrame(const struct DoorGraphics *gfx, u32 x, u32 y, const u8 *paletteNums)
{
    u16 tiles[24];

    switch (gfx->size)
    {
    case DOOR_SIZE_2x2_LEFT:
        // Top left metatile
        BuildDoorTiles(&tiles[8], DOOR_TILE_START_SIZE2 + 0, &paletteNums[0]);
        DrawDoorMetatileAt(x, y - 1, &tiles[8]);

        // Bottom left metatile
        BuildDoorTiles(&tiles[8], DOOR_TILE_START_SIZE2 + 4, &paletteNums[4]);
        DrawDoorMetatileAt(x, y, &tiles[8]);

        // Top right metatile
        BuildDoorTiles(&tiles[8], DOOR_TILE_START_SIZE2 + 8, &paletteNums[8]);
        DrawDoorMetatileAt(x + 1, y - 1, &tiles[8]);

        // Bottom right metatile
        BuildDoorTiles(&tiles[8], DOOR_TILE_START_SIZE2 + 12, &paletteNums[12]);
        DrawDoorMetatileAt(x + 1, y, &tiles[8]);
        break;
    case DOOR_SIZE_2x2_RIGHT:
        // Top left metatile
        BuildDoorTiles(&tiles[8], DOOR_TILE_START_SIZE2 + 0, &paletteNums[0]);
        DrawDoorMetatileAt(x - 1, y - 1, &tiles[8]);

        // Bottom left metatile
        BuildDoorTiles(&tiles[8], DOOR_TILE_START_SIZE2 + 4, &paletteNums[4]);
        DrawDoorMetatileAt(x - 1, y, &tiles[8]);

        // Top right metatile
        BuildDoorTiles(&tiles[8], DOOR_TILE_START_SIZE2 + 8, &paletteNums[8]);
        DrawDoorMetatileAt(x, y - 1, &tiles[8]);

        // Bottom right metatile
        BuildDoorTiles(&tiles[8], DOOR_TILE_START_SIZE2 + 12, &paletteNums[12]);
        DrawDoorMetatileAt(x, y, &tiles[8]);
        break;
    case DOOR_SIZE_1x2:
        // Top metatile
        BuildDoorTiles(&tiles[0], DOOR_TILE_START_SIZE1 + 0, &paletteNums[0]);
        DrawDoorMetatileAt(x, y - 1, &tiles[0]);

        // Bottom metatile
        BuildDoorTiles(&tiles[0], DOOR_TILE_START_SIZE1 + 4, &paletteNums[4]);
        DrawDoorMetatileAt(x, y, &tiles[0]);
        break;
    default:
    case DOOR_SIZE_1x1:
        BuildDoorTiles(&tiles[0], DOOR_TILE_START_SIZE1 + 0, &paletteNums[0]);
        DrawDoorMetatileAt(x, y, &tiles[0]);
        break;
    }
}

static void DrawClosedDoorTiles(const struct DoorGraphics *gfx, u32 x, u32 y)
{
    switch (gfx->size)
    {
    case DOOR_SIZE_2x2_LEFT:
        CurrentMapDrawMetatileAt(x + 1, y - 1);
        CurrentMapDrawMetatileAt(x + 1, y);
        CurrentMapDrawMetatileAt(x, y - 1);
        CurrentMapDrawMetatileAt(x, y);
        break;
    case DOOR_SIZE_2x2_RIGHT:
        CurrentMapDrawMetatileAt(x - 1, y - 1);
        CurrentMapDrawMetatileAt(x - 1, y);
        CurrentMapDrawMetatileAt(x, y - 1);
        CurrentMapDrawMetatileAt(x, y);
        break;
    case DOOR_SIZE_1x2:
        CurrentMapDrawMetatileAt(x, y - 1);
        CurrentMapDrawMetatileAt(x, y);
        break;
    default:
    case DOOR_SIZE_1x1:
        CurrentMapDrawMetatileAt(x, y);
        break;
    }
}

static void DrawDoor(const struct DoorGraphics *gfx, const struct DoorAnimFrame *frame, u32 x, u32 y)
{
    if (frame->offset == 0xFFFF)
    {
        DrawClosedDoorTiles(gfx, x, y);
        if (ShouldUseMultiCorridorDoor())
            DrawClosedDoorTiles(gfx, gSpecialVar_0x8004 + MAP_OFFSET, gSpecialVar_0x8005 + MAP_OFFSET);
    }
    else
    {
        CopyDoorTilesToVram(gfx, frame);
        DrawCurrentDoorAnimFrame(gfx, x, y, gfx->palettes);
        if (ShouldUseMultiCorridorDoor())
            DrawCurrentDoorAnimFrame(gfx, gSpecialVar_0x8004 + MAP_OFFSET, gSpecialVar_0x8005 + MAP_OFFSET, gfx->palettes);
    }
}

#define tFramesHi data[0]
#define tFramesLo data[1]
#define tGfxHi data[2]
#define tGfxLo data[3]
#define tFrameId data[4]
#define tCounter data[5]
#define tX data[6]
#define tY data[7]

// Draws a single frame of the door animation, or skips drawing to wait between frames.
// Returns FALSE when the final frame has been reached
static bool32 AnimateDoorFrame(struct DoorGraphics *gfx, struct DoorAnimFrame *frames, s16 *data)
{
    if (tCounter == 0)
        DrawDoor(gfx, &frames[tFrameId], tX, tY);

    if (tCounter == frames[tFrameId].time)
    {
        tCounter = 0;
        tFrameId++;
        if (frames[tFrameId].time == 0)
            return FALSE;
        else
            return TRUE;
    }
    tCounter++;
    return TRUE;
}

static void Task_AnimateDoor(u8 taskId)
{
    u16 *data = (u16*) gTasks[taskId].data;
    struct DoorAnimFrame *frames = (struct DoorAnimFrame *)(tFramesHi << 16 | tFramesLo);
    struct DoorGraphics *gfx = (struct DoorGraphics *)(tGfxHi << 16 | tGfxLo);

    if (AnimateDoorFrame(gfx, frames, gTasks[taskId].data) == FALSE)
        DestroyTask(taskId);
}

static const struct DoorAnimFrame *GetLastDoorFrame(const struct DoorAnimFrame *frame, const void *unused)
{
    while (frame->time != 0)
        frame++;
    return frame - 1;
}

static const struct DoorGraphics *GetDoorGraphics(const struct DoorGraphics *gfx, u16 metatileNum)
{
    while (gfx->tiles != NULL)
    {
        if (gfx->metatileNum == metatileNum && (gfx->tileset == gMapHeader.mapLayout->primaryTileset || gfx->tileset == gMapHeader.mapLayout->secondaryTileset))
            return gfx;
        gfx++;
    }
    return NULL;
}

static s8 StartDoorAnimationTask(const struct DoorGraphics *gfx, const struct DoorAnimFrame *frames, u32 x, u32 y)
{
    if (FuncIsActiveTask(Task_AnimateDoor) == TRUE)
    {
        return -1;
    }
    else
    {
        u8 taskId = CreateTask(Task_AnimateDoor, 0x50);
        s16 *data = gTasks[taskId].data;

        tX = x;
        tY = y;

        tFramesLo = (u32)frames;
        tFramesHi = (u32)frames >> 16;

        tGfxLo = (u32)gfx;
        tGfxHi = (u32)gfx >> 16;

        return taskId;
    }
}

static void DrawClosedDoor(const struct DoorGraphics *gfx, u32 x, u32 y)
{
    DrawClosedDoorTiles(gfx, x, y);
}

static void DrawOpenedDoor(const struct DoorGraphics *gfx, u32 x, u32 y)
{
    gfx = GetDoorGraphics(gfx, MapGridGetMetatileIdAt(x, y));
    if (gfx != NULL)
    {
        const struct DoorAnimFrame *doorAnimFrames = sDoorSizeInfo[gfx->size].openAnimFrames;
        DrawDoor(gfx, GetLastDoorFrame(doorAnimFrames, doorAnimFrames), x, y);
    }
}

static s8 StartDoorOpenAnimation(const struct DoorGraphics *gfx, u32 x, u32 y)
{
    gfx = GetDoorGraphics(gfx, MapGridGetMetatileIdAt(x, y));
    if (gfx == NULL)
    {
        return -1;
    }
    else
    {
        const struct DoorAnimFrame *doorAnimFrames = sDoorSizeInfo[gfx->size].openAnimFrames;
        return StartDoorAnimationTask(gfx, doorAnimFrames, x, y);
    }
}

static s8 StartDoorCloseAnimation(const struct DoorGraphics *gfx, u32 x, u32 y)
{
    gfx = GetDoorGraphics(gfx, MapGridGetMetatileIdAt(x, y));
    if (gfx == NULL)
    {
        return -1;
    }
    else
    {
        const struct DoorAnimFrame *doorAnimFrames = sDoorSizeInfo[gfx->size].closeAnimFrames;
        return StartDoorAnimationTask(gfx, doorAnimFrames, x, y);
    }
}

static s8 GetDoorSoundType(const struct DoorGraphics *gfx, u32 x, u32 y)
{
    gfx = GetDoorGraphics(gfx, MapGridGetMetatileIdAt(x, y));
    if (gfx == NULL)
        return -1;
    else
        return gfx->sound;
}

void FieldSetDoorOpened(u32 x, u32 y)
{
    if (MetatileBehavior_IsDoor(MapGridGetMetatileBehaviorAt(x, y)))
        DrawOpenedDoor(sDoorAnimGraphicsTable, x, y);
}

void FieldSetDoorClosed(u32 x, u32 y)
{
    if (MetatileBehavior_IsDoor(MapGridGetMetatileBehaviorAt(x, y)))
        DrawClosedDoor(sDoorAnimGraphicsTable, x, y);
}

s8 FieldAnimateDoorClose(u32 x, u32 y)
{
    if (!MetatileBehavior_IsDoor(MapGridGetMetatileBehaviorAt(x, y)))
        return -1;
    else
        return StartDoorCloseAnimation(sDoorAnimGraphicsTable, x, y);
}

s8 FieldAnimateDoorOpen(u32 x, u32 y)
{
    if (!MetatileBehavior_IsDoor(MapGridGetMetatileBehaviorAt(x, y)))
        return -1;
    else
        return StartDoorOpenAnimation(sDoorAnimGraphicsTable, x, y);
}

bool8 FieldIsDoorAnimationRunning(void)
{
    return FuncIsActiveTask(Task_AnimateDoor);
}

u32 GetDoorSoundEffect(u32 x, u32 y)
{
    int sound = GetDoorSoundType(sDoorAnimGraphicsTable, x, y);

    if (sound == DOOR_SOUND_NORMAL)
        return SE_DOOR;
    else if (sound == DOOR_SOUND_SLIDING)
        return SE_SLIDING_DOOR;
    else if (sound == DOOR_SOUND_ARENA)
        return SE_REPEL;
    else
        return SE_DOOR;
}

// Opens the Battle Tower multi partner's door in sync with the player's door
static bool8 ShouldUseMultiCorridorDoor(void)
{
    if (FlagGet(FLAG_ENABLE_MULTI_CORRIDOR_DOOR))
    {
        if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_TOWER_MULTI_CORRIDOR)
            && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_TOWER_MULTI_CORRIDOR))
        {
            return TRUE;
        }
    }
    return FALSE;
}
