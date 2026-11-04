#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "bg.h"
#include "dma3.h"
#include "event_data.h"
#include "frontier_music.h"
#include "m4a.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "pokemon.h"
#include "random.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "util.h"
#include "window.h"
#include "test/test.h"
#include "constants/flags.h"
#include "constants/songs.h"
#include "constants/trainers.h"

extern const struct SongHeader mus_vs_colosseum;
extern const struct SongHeader mus_vs_colosseum_admin;

static const u32 sFacilities[] =
{
    BATTLE_TYPE_BATTLE_TOWER, BATTLE_TYPE_DOME, BATTLE_TYPE_PALACE,
    BATTLE_TYPE_ARENA, BATTLE_TYPE_FACTORY, BATTLE_TYPE_PIKE, BATTLE_TYPE_PYRAMID,
};

static void ResetMusic(void)
{
    FrontierMusic_StopPreview();
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

TEST("Frontier music: all catalog battle songs cost five BP each")
{
    ResetMusic();
    gSaveBlock3Ptr->frontierMusic.earned = FRONTIER_MUSIC_EARNED_RELEASE | FRONTIER_MUSIC_EARNED_FINAL;
    for (u32 i = 0; i < FRONTIER_MUSIC_TRACK_COUNT; i++)
    {
        u16 song = FrontierMusic_GetSong(i);
        for (u32 j = 0; j < i; j++)
            EXPECT_NE(song, FrontierMusic_GetSong(j));
        EXPECT((song >= START_MUS && song <= END_MUS) || song == MUS_POKEVIRUS_RELEASE || song == MUS_POKEVIRUS_FINAL
            || song == MUS_VS_COLOSSEUM || song == MUS_VS_COLOSSEUM_ADMIN);
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

static void ClearStoryProgress(void)
{
    static const u16 flags[] =
    {
        FLAG_STORY_CURE_FOUND, FLAG_STORY_POKEVIRUS_FADED,
        FLAG_STORY_RESCUED_TOWER, FLAG_STORY_RESCUED_DOME, FLAG_STORY_RESCUED_PALACE, FLAG_STORY_RESCUED_ARENA,
        FLAG_STORY_RESCUED_FACTORY, FLAG_STORY_RESCUED_PIKE, FLAG_STORY_RESCUED_PYRAMID, FLAG_STORY_RESCUED_CONTEST,
    };
    for (u32 i = 0; i < ARRAY_COUNT(flags); i++)
        FlagClear(flags[i]);
}

TEST("Frontier music: the release and Pokevirus themes are offered only after those battles are won")
{
    ResetMusic();
    ClearStoryProgress();
    EXPECT(!FrontierMusic_IsAvailable(MUS_POKEVIRUS_RELEASE));
    EXPECT(!FrontierMusic_IsAvailable(MUS_POKEVIRUS_FINAL));
    EXPECT_EQ(FrontierMusic_Unlock(MUS_POKEVIRUS_RELEASE), FRONTIER_MUSIC_INVALID);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);

    FrontierMusic_Earn(FALSE);
    EXPECT(FrontierMusic_IsAvailable(MUS_POKEVIRUS_RELEASE));
    EXPECT(!FrontierMusic_IsAvailable(MUS_POKEVIRUS_FINAL));
    EXPECT_EQ(FrontierMusic_Unlock(MUS_POKEVIRUS_RELEASE), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_POKEVIRUS_RELEASE), FRONTIER_MUSIC_OK);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_POKEVIRUS_RELEASE);
    EXPECT_EQ(FrontierMusic_Unlock(MUS_POKEVIRUS_FINAL), FRONTIER_MUSIC_INVALID);

    FrontierMusic_Earn(TRUE);
    EXPECT(FrontierMusic_IsAvailable(MUS_POKEVIRUS_FINAL));
    EXPECT_EQ(FrontierMusic_Unlock(MUS_POKEVIRUS_FINAL), FRONTIER_MUSIC_OK);
    // The earned bits survive reloading the extension.
    FrontierMusic_EnsureSaveData();
    EXPECT(FrontierMusic_IsUnlocked(MUS_POKEVIRUS_FINAL));
    EXPECT(FrontierMusic_IsAvailable(MUS_POKEVIRUS_RELEASE));
}

TEST("Frontier music: saves that already won those battles get the story themes from their flags")
{
    ResetMusic();
    ClearStoryProgress();
    FlagSet(FLAG_STORY_RESCUED_PIKE);
    EXPECT(FrontierMusic_IsAvailable(MUS_POKEVIRUS_RELEASE));
    EXPECT(!FrontierMusic_IsAvailable(MUS_POKEVIRUS_FINAL));
    FlagSet(FLAG_STORY_POKEVIRUS_FADED);
    EXPECT(FrontierMusic_IsAvailable(MUS_POKEVIRUS_FINAL));
    ClearStoryProgress();
}

TEST("Frontier music: Colosseum appends a valid eight-track BGM without changing old catalog indices")
{
    EXPECT_EQ(MUS_VS_COLOSSEUM, 614);
    EXPECT(gSongTable[MUS_VS_COLOSSEUM].header == &mus_vs_colosseum);
    EXPECT_EQ(gSongTable[MUS_VS_COLOSSEUM].ms, 0);
    EXPECT_EQ(mus_vs_colosseum.trackCount, 8);
    EXPECT_EQ(FrontierMusic_GetSong(21), MUS_POKEVIRUS_RELEASE);
    EXPECT_EQ(FrontierMusic_GetSong(22), MUS_POKEVIRUS_FINAL);
    EXPECT_EQ(FrontierMusic_GetSong(23), MUS_VS_COLOSSEUM);
    EXPECT_EQ(mus_vs_colosseum.tone[6].type & 7, 3);
    EXPECT_EQ(mus_vs_colosseum.tone[7].type & 7, 2);
    EXPECT_EQ(mus_vs_colosseum.tone[8].type & 7, 4);
}

TEST("Frontier music: Colosseum is offered without story requirements and supports fixed and random playback")
{
    ResetMusic();
    ClearStoryProgress();
    EXPECT(FrontierMusic_IsAvailable(MUS_VS_COLOSSEUM));
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_COLOSSEUM), FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 95);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_COLOSSEUM), FRONTIER_MUSIC_OK);
    FrontierMusic_EnsureSaveData();
    EXPECT(FrontierMusic_IsUnlocked(MUS_VS_COLOSSEUM));
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_COLOSSEUM);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_COLOSSEUM);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: expanding the catalog preserves existing unlocks and fixed selection")
{
    ResetMusic();
    gSaveBlock3Ptr->frontierMusic.unlocked = (1u << 23) - 1;
    gSaveBlock3Ptr->frontierMusic.selectedSong = MUS_RG_VS_DEOXYS;
    gSaveBlock3Ptr->frontierMusic.mode = FRONTIER_MUSIC_FIXED;
    gSaveBlock3Ptr->frontierMusic.checksum = CalcCRC16((const u8 *)&gSaveBlock3Ptr->frontierMusic,
                                                     offsetof(struct FrontierMusicSave, checksum));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, (1u << 23) - 1);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.selectedSong, MUS_RG_VS_DEOXYS);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.mode, FRONTIER_MUSIC_FIXED);
    EXPECT(!FrontierMusic_IsUnlocked(MUS_VS_COLOSSEUM));
}

TEST("Frontier music: Colosseum's compiled sequence keeps all eight tracks alive across two loops")
{
    m4aSongNumStart(MUS_VS_COLOSSEUM);
    for (u32 frame = 0; frame < 9000; frame++)
        MPlayMain(&gMPlayInfo_BGM);
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_colosseum);
    EXPECT_GT(gMPlayInfo_BGM.clock, 2 * 2880);
    EXPECT_EQ(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK, 0xFF);
    for (u32 track = 0; track < 8; track++)
        EXPECT(gMPlayInfo_BGM.tracks[track].flags & MPT_FLG_EXIST);
    m4aMPlayStop(&gMPlayInfo_BGM);
}

TEST("Frontier music: Colosseum admin appends a nine-track song with independent PSG voices")
{
    EXPECT_EQ(MUS_VS_COLOSSEUM_ADMIN, 615);
    EXPECT_EQ(FrontierMusic_GetSong(23), MUS_VS_COLOSSEUM);
    EXPECT_EQ(FrontierMusic_GetSong(24), MUS_VS_COLOSSEUM_ADMIN);
    EXPECT(gSongTable[MUS_VS_COLOSSEUM_ADMIN].header == &mus_vs_colosseum_admin);
    EXPECT_EQ(gSongTable[MUS_VS_COLOSSEUM_ADMIN].ms, 0);
    EXPECT_EQ(mus_vs_colosseum_admin.trackCount, 9);
    EXPECT_EQ(mus_vs_colosseum_admin.tone[3].type & 7, 3);
    EXPECT_EQ(mus_vs_colosseum_admin.tone[4].type & 7, 2);
    EXPECT_EQ(mus_vs_colosseum_admin.tone[5].type & 7, 1);
    EXPECT_EQ(mus_vs_colosseum_admin.tone[6].type & 7, 4);
    m4aSongNumStart(MUS_VS_COLOSSEUM_ADMIN);
    for (u32 frame = 0; frame < 11000; frame++)
        MPlayMain(&gMPlayInfo_BGM);
    EXPECT_GT(gMPlayInfo_BGM.clock, 2 * 4032);
    EXPECT_EQ(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK, 0x1FF);
    for (u32 track = 0; track < 9; track++)
        EXPECT(gMPlayInfo_BGM.tracks[track].flags & MPT_FLG_EXIST);
    m4aMPlayStop(&gMPlayInfo_BGM);
}

TEST("Frontier music: Colosseum admin supports free preview five-BP unlock and fixed or random battle music")
{
    struct FrontierMusicSave before;
    ResetMusic();
    ClearStoryProgress();
    before = gSaveBlock3Ptr->frontierMusic;
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    gSaveBlock2Ptr->frontier.battlePoints = 0;
    EXPECT(FrontierMusic_IsAvailable(MUS_VS_COLOSSEUM_ADMIN));
    EXPECT(FrontierMusic_Preview(MUS_VS_COLOSSEUM_ADMIN));
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_colosseum_admin);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    FrontierMusic_StopPreview();
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    gSaveBlock2Ptr->frontier.battlePoints = 5;
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_COLOSSEUM_ADMIN), FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_COLOSSEUM_ADMIN), FRONTIER_MUSIC_OK);
    FrontierMusic_EnsureSaveData();
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_COLOSSEUM_ADMIN);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_COLOSSEUM_ADMIN);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: adding Colosseum admin preserves all previous 24 unlock bits and selection")
{
    ResetMusic();
    gSaveBlock3Ptr->frontierMusic.unlocked = (1u << 24) - 1;
    gSaveBlock3Ptr->frontierMusic.selectedSong = MUS_VS_COLOSSEUM;
    gSaveBlock3Ptr->frontierMusic.mode = FRONTIER_MUSIC_FIXED;
    gSaveBlock3Ptr->frontierMusic.checksum = CalcCRC16((const u8 *)&gSaveBlock3Ptr->frontierMusic,
                                                     offsetof(struct FrontierMusicSave, checksum));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, (1u << 24) - 1);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.selectedSong, MUS_VS_COLOSSEUM);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.mode, FRONTIER_MUSIC_FIXED);
    EXPECT(!FrontierMusic_IsUnlocked(MUS_VS_COLOSSEUM_ADMIN));
}

TEST("Frontier music: previewing a locked song is free and never changes the saved preference")
{
    struct FrontierMusicSave before;
    ResetMusic();
    ClearStoryProgress();
    FrontierMusic_Unlock(MUS_RG_VS_DEOXYS);
    FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_RG_VS_DEOXYS);
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    before = gSaveBlock3Ptr->frontierMusic;
    gSaveBlock2Ptr->frontier.battlePoints = 0;
    EXPECT(FrontierMusic_Preview(MUS_VS_COLOSSEUM));
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_colosseum);
    EXPECT_EQ(GetCurrentMapMusic(), MUS_B_FRONTIER);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    FrontierMusic_StopPreview();
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    EXPECT_EQ(GetCurrentMapMusic(), MUS_B_FRONTIER);
    FrontierMusic_StopPreview();
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
}

TEST("Frontier music: catalog preview controls fit the footer")
{
    static const u8 preview[] = _("{JPN}SELECT:しちょう A:けってい B:もどる");
    static const u8 stop[] = _("{JPN}SELECT:ていし A:けってい B:もどる");
    EXPECT_LE(GetStringWidth(FONT_SMALL, preview, 0), 200);
    EXPECT_LE(GetStringWidth(FONT_SMALL, stop, 0), 200);
}

TEST("Frontier music: switching preview songs retains the original map music and respects story locks")
{
    struct FrontierMusicSave before;
    ResetMusic();
    ClearStoryProgress();
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    before = gSaveBlock3Ptr->frontierMusic;
    EXPECT(!FrontierMusic_Preview(MUS_NONE));
    EXPECT(!FrontierMusic_Preview(MUS_POKEVIRUS_FINAL));
    EXPECT(FrontierMusic_Preview(MUS_VS_COLOSSEUM));
    EXPECT(!FrontierMusic_Preview(MUS_POKEVIRUS_RELEASE));
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_colosseum);
    EXPECT(FrontierMusic_Preview(MUS_RG_VS_TRAINER));
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_RG_VS_TRAINER].header);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    FrontierMusic_StopPreview();
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
}

static void CatalogInput(u16 keys)
{
    gMain.newKeys = gMain.newAndRepeatedKeys = keys;
    RunTasks();
    while (IsDma3ManagerBusyWithBgCopy())
        ProcessDma3Requests();
    gMain.newKeys = gMain.newAndRepeatedKeys = 0;
}

TEST("Frontier music: SELECT previews and stops in the catalog and both A and B restore map music")
{
    u16 closeButton;
    struct FrontierMusicSave before;
    static const struct BgTemplate background = {0, 0, 24, 0, 0, 0, 0};
    PARAMETRIZE { closeButton = A_BUTTON; }
    PARAMETRIZE { closeButton = B_BUTTON; }
    ResetMusic();
    ClearStoryProgress();
    before = gSaveBlock3Ptr->frontierMusic;
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, &background, 1);
    SetBgTilemapBuffer(0, AllocZeroed(0x800));
    InitStandardTextBoxWindows();
    InitTextBoxGfxAndPrinters();
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    gMain.newKeys = gMain.newAndRepeatedKeys = 0;
    FrontierMusic_OpenCatalog();
    CatalogInput(0);
    CatalogInput(SELECT_BUTTON);
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_VS_TRAINER].header);
    CatalogInput(SELECT_BUTTON);
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    CatalogInput(DPAD_DOWN);
    CatalogInput(SELECT_BUTTON);
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_VS_WILD].header);
    CatalogInput(closeButton);
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    EXPECT_EQ(GetCurrentMapMusic(), MUS_B_FRONTIER);
    EXPECT_EQ(gSpecialVar_Result, closeButton == A_BUTTON);
    if (closeButton == A_BUTTON)
        EXPECT_EQ(gSpecialVar_0x8005, MUS_VS_WILD);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
    for (u32 task = 0; task < NUM_TASKS; task++)
        EXPECT(!gTasks[task].isActive);
    FreeAllWindowBuffers();
    void *tilemap = GetBgTilemapBuffer(0);
    UnsetBgTilemapBuffer(0);
    Free(tilemap);
}
