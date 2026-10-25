#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "event_data.h"
#include "frontier_music.h"
#include "pokemon.h"
#include "random.h"
#include "sound.h"
#include "string_util.h"
#include "task.h"
#include "util.h"
#include "test/test.h"
#include "constants/songs.h"
#include "constants/trainers.h"

static const u32 sFacilities[] =
{
    BATTLE_TYPE_BATTLE_TOWER, BATTLE_TYPE_DOME, BATTLE_TYPE_PALACE,
    BATTLE_TYPE_ARENA, BATTLE_TYPE_FACTORY, BATTLE_TYPE_PIKE, BATTLE_TYPE_PYRAMID,
};

static void ResetMusic(void)
{
    memset(&gSaveBlock3Ptr->frontierMusic, 0, sizeof(struct FrontierMusicSave));
    FrontierMusic_EnsureSaveData();
    gSaveBlock2Ptr->frontier.battlePoints = 100;
    TRAINER_BATTLE_PARAM.opponentA = 0;
    TRAINER_BATTLE_PARAM.opponentB = 0;
}

TEST("Frontier music: extension is append-only and old-save garbage initializes locally")
{
    u8 prefix[152];
    EXPECT_EQ(offsetof(struct SaveBlock3, frontierMusic), sizeof(prefix));
    EXPECT_EQ(sizeof(struct FrontierMusicSave), 16);
    EXPECT_EQ(offsetof(struct SaveBlock3, fusions), 168);
    memcpy(prefix, gSaveBlock3Ptr, sizeof(prefix));
    memset(&gSaveBlock3Ptr->frontierMusic, 0xFF, sizeof(struct FrontierMusicSave));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(memcmp(prefix, gSaveBlock3Ptr, sizeof(prefix)), 0);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, 0);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.mode, FRONTIER_MUSIC_DEFAULT);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.selectedSong, MUS_DUMMY);
}

TEST("Frontier music: all 21 distinct battle songs cost five BP each")
{
    ResetMusic();
    for (u32 i = 0; i < FRONTIER_MUSIC_TRACK_COUNT; i++)
    {
        u16 song = FrontierMusic_GetSong(i);
        for (u32 j = 0; j < i; j++)
            EXPECT_NE(song, FrontierMusic_GetSong(j));
        EXPECT(song >= START_MUS && song <= END_MUS);
        EXPECT(!FrontierMusic_IsUnlocked(song));
        gSaveBlock2Ptr->frontier.battlePoints = 5;
        EXPECT_EQ(FrontierMusic_Unlock(song), FRONTIER_MUSIC_OK);
        EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
        EXPECT(FrontierMusic_IsUnlocked(song));
        EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, song), FRONTIER_MUSIC_OK);
        EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.selectedSong, song);
    }
    EXPECT_EQ(FrontierMusic_GetSong(FRONTIER_MUSIC_TRACK_COUNT), MUS_DUMMY);
    EXPECT_EQ(FrontierMusic_GetSong(0xFFFFFFFF), MUS_DUMMY);
}

TEST("Frontier music: insufficient BP invalid ids and repeat unlocks never charge")
{
    ResetMusic();
    gSaveBlock2Ptr->frontier.battlePoints = 4;
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_TRAINER), FRONTIER_MUSIC_NOT_ENOUGH_BP);
    EXPECT_EQ(FrontierMusic_Unlock(MUS_RG_MYSTERY_GIFT), FRONTIER_MUSIC_INVALID);
    EXPECT_EQ(FrontierMusic_Unlock(0xFFFF), FRONTIER_MUSIC_INVALID);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 4);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, 0);
    gSaveBlock2Ptr->frontier.battlePoints = 5;
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_TRAINER), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_TRAINER), FRONTIER_MUSIC_ALREADY_UNLOCKED);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
}

TEST("Frontier music: unowned fixed songs and empty random selection preserve the setting")
{
    struct FrontierMusicSave before;
    ResetMusic();
    before = gSaveBlock3Ptr->frontierMusic;
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_TRAINER), FRONTIER_MUSIC_NOT_UNLOCKED);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0), FRONTIER_MUSIC_EMPTY);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_DUMMY), FRONTIER_MUSIC_INVALID);
    EXPECT_EQ(FrontierMusic_SetMode(255, MUS_VS_TRAINER), FRONTIER_MUSIC_INVALID);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
}

TEST("Frontier music: switching modes is free and returning to default retains all unlocks")
{
    ResetMusic();
    FrontierMusic_Unlock(MUS_VS_TRAINER);
    FrontierMusic_Unlock(MUS_RG_VS_CHAMPION);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_RG_VS_CHAMPION), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_DEFAULT, 0), FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 90);
    EXPECT(FrontierMusic_IsUnlocked(MUS_VS_TRAINER));
    EXPECT(FrontierMusic_IsUnlocked(MUS_RG_VS_CHAMPION));
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.selectedSong, MUS_DUMMY);
}

TEST("Frontier music: saved unlocks and selection survive reloading the extension")
{
    struct FrontierMusicSave saved;
    ResetMusic();
    FrontierMusic_Unlock(MUS_RG_VS_DEOXYS);
    FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_RG_VS_DEOXYS);
    saved = gSaveBlock3Ptr->frontierMusic;
    memset(&gSaveBlock3Ptr->frontierMusic, 0, sizeof(saved));
    gSaveBlock3Ptr->frontierMusic = saved;
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(memcmp(&saved, &gSaveBlock3Ptr->frontierMusic, sizeof(saved)), 0);
    FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0);
    saved = gSaveBlock3Ptr->frontierMusic;
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(memcmp(&saved, &gSaveBlock3Ptr->frontierMusic, sizeof(saved)), 0);
}

TEST("Frontier music: damaged checksum cannot turn old bytes into an arbitrary song")
{
    ResetMusic();
    FrontierMusic_Unlock(MUS_VS_TRAINER);
    FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_TRAINER);
    gSaveBlock3Ptr->frontierMusic.selectedSong = 0xFFFF;
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.mode, FRONTIER_MUSIC_DEFAULT);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, 0);
}

TEST("Frontier music: malformed mode or fixed song is repaired even with a valid checksum")
{
    struct FrontierMusicSave *save = &gSaveBlock3Ptr->frontierMusic;
    u32 mode = 0;
    PARAMETRIZE { mode = 255; }
    PARAMETRIZE { mode = FRONTIER_MUSIC_FIXED; }
    PARAMETRIZE { mode = FRONTIER_MUSIC_RANDOM; }
    ResetMusic();
    save->mode = mode;
    save->selectedSong = 0xFFFF;
    save->checksum = CalcCRC16((const u8 *)save, offsetof(struct FrontierMusicSave, checksum));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(save->mode, FRONTIER_MUSIC_DEFAULT);
    EXPECT_EQ(save->selectedSong, MUS_DUMMY);
}

TEST("Frontier music: fixed music applies in all seven facilities including doubles and NPC multi")
{
    u32 facility = 0, format = 0;
    static const u32 formats[] = {0, BATTLE_TYPE_DOUBLE,
        BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TWO_OPPONENTS};
    for (u32 f = 0; f < ARRAY_COUNT(sFacilities); f++)
        for (u32 d = 0; d < ARRAY_COUNT(formats); d++)
            PARAMETRIZE { facility = f; format = d; }
    ResetMusic();
    FrontierMusic_Unlock(MUS_RG_VS_DEOXYS);
    FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_RG_VS_DEOXYS);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | sFacilities[facility] | formats[format];
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_RG_VS_DEOXYS);
}

TEST("Frontier music: Brain battles always override fixed random default and explicit map music")
{
    u32 facility = 0, mode = 0;
    for (u32 f = 0; f < ARRAY_COUNT(sFacilities); f++)
        for (u32 m = FRONTIER_MUSIC_DEFAULT; m <= FRONTIER_MUSIC_RANDOM; m++)
            PARAMETRIZE { facility = f; mode = m; }
    ResetMusic();
    FrontierMusic_Unlock(MUS_VS_WILD);
    FrontierMusic_SetMode(mode, MUS_VS_WILD);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | sFacilities[facility];
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_RG_VS_CHAMPION), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: a second opponent Brain is also protected")
{
    ResetMusic();
    FrontierMusic_Unlock(MUS_VS_WILD);
    FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_WILD);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_PIKE | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS;
    TRAINER_BATTLE_PARAM.opponentB = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: wild nonfacility link and recorded battles keep their original music")
{
    u32 flags = 0;
    static const u32 cases[] = {
        0, BATTLE_TYPE_TRAINER, BATTLE_TYPE_TRAINER | BATTLE_TYPE_TRAINER_HILL,
        BATTLE_TYPE_PYRAMID, BATTLE_TYPE_PIKE,
        BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_LINK,
        BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_LINK | BATTLE_TYPE_TOWER_LINK_MULTI,
        BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_RECORDED_LINK,
        BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_RECORDED,
    };
    for (u32 i = 0; i < ARRAY_COUNT(cases); i++)
        PARAMETRIZE { flags = cases[i]; }
    ResetMusic();
    FrontierMusic_Unlock(MUS_RG_VS_DEOXYS);
    FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_RG_VS_DEOXYS);
    gBattleTypeFlags = flags;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_TRAINER);
}

TEST("Frontier music: random draws use only unlocked tracks and leave gameplay RNG unchanged")
{
    rng_value_t before;
    u32 seen = 0;
    ResetMusic();
    FrontierMusic_Unlock(MUS_VS_WILD);
    FrontierMusic_Unlock(MUS_RG_VS_DEOXYS);
    FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_FACTORY;
    SeedRng(1234);
    SeedRng2(5678);
    before = gRngValue;
    for (u32 i = 0; i < 100; i++)
    {
        u16 song = FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER);
        EXPECT(song == MUS_VS_WILD || song == MUS_RG_VS_DEOXYS);
        seen |= song == MUS_VS_WILD ? 1 : 2;
    }
    EXPECT_EQ(seen, 3);
    EXPECT_EQ(memcmp(&before, &gRngValue, sizeof(before)), 0);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 90);
}

TEST("Frontier music: random mode with one unlocked track always uses that track")
{
    ResetMusic();
    FrontierMusic_Unlock(MUS_VS_FRONTIER_BRAIN);
    FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_ARENA;
    for (u32 i = 0; i < 20; i++)
        EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: default selection retains the existing trainer or map-selected theme")
{
    ResetMusic();
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_TRAINER);
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_RG_VS_GYM_LEADER), MUS_RG_VS_GYM_LEADER);
}

TEST("Frontier music: immediate battle entry uses the chosen song but never overrides a Brain")
{
    ResetMusic();
    FrontierMusic_Unlock(MUS_RG_VS_DEOXYS);
    FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_RG_VS_DEOXYS);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    PlayMapChosenOrBattleBGM(MUS_RG_VS_WILD);
    EXPECT_EQ(GetCurrentMapMusic(), MUS_RG_VS_DEOXYS);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    PlayMapChosenOrBattleBGM(MUS_RG_VS_WILD);
    EXPECT_EQ(GetCurrentMapMusic(), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: script purchase and selection use the selected song and mode")
{
    static const u8 expectedName[] = _("{JPN}ミュウツー");
    ResetMusic();
    gSpecialVar_0x8005 = MUS_RG_VS_MEWTWO;
    gSpecialVar_0x8006 = FRONTIER_MUSIC_FIXED;
    FrontierMusic_ApplySelection();
    EXPECT_EQ(gSpecialVar_Result, FRONTIER_MUSIC_NOT_UNLOCKED);
    FrontierMusic_UnlockSelection();
    EXPECT_EQ(gSpecialVar_Result, FRONTIER_MUSIC_OK);
    FrontierMusic_ApplySelection();
    EXPECT_EQ(gSpecialVar_Result, FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.selectedSong, MUS_RG_VS_MEWTWO);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 95);
    FrontierMusic_BufferSelection();
    EXPECT_EQ(StringCompare(gStringVar1, expectedName), 0);
    gSpecialVar_0x8006 = 0x100;
    FrontierMusic_ApplySelection();
    EXPECT_EQ(gSpecialVar_Result, FRONTIER_MUSIC_INVALID);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.mode, FRONTIER_MUSIC_FIXED);
}

TEST("Frontier music: Dome task chooses once at entry and does not reroll during playback")
{
    u32 taskId;
    u16 song;
    rng_value_t before;
    ResetMusic();
    FrontierMusic_Unlock(MUS_VS_WILD);
    FrontierMusic_Unlock(MUS_RG_VS_DEOXYS);
    FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOME;
    for (taskId = 0; taskId < NUM_TASKS; taskId++)
        if (!gTasks[taskId].isActive)
            break;
    EXPECT(taskId < NUM_TASKS);
    CreateTask_PlayMapChosenOrBattleBGM(0);
    song = gTasks[taskId].data[0];
    EXPECT(song == MUS_VS_WILD || song == MUS_RG_VS_DEOXYS);
    before = gRng2Value;
    gTasks[taskId].func(taskId);
    EXPECT_EQ(GetCurrentMapMusic(), song);
    EXPECT_EQ(memcmp(&before, &gRng2Value, sizeof(before)), 0);
    EXPECT(!gTasks[taskId].isActive);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    CreateTask_PlayMapChosenOrBattleBGM(MUS_RG_VS_WILD);
    gTasks[taskId].func(taskId);
    EXPECT_EQ(GetCurrentMapMusic(), MUS_VS_FRONTIER_BRAIN);
}
