#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "bg.h"
#include "dma3.h"
#include "event_data.h"
#include "frontier_music.h"
#include "list_menu.h"
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
extern const struct SongHeader mus_vs_colosseum_2;
extern const struct SongHeader mus_vs_pmd_rayquaza;
extern const struct SongHeader mus_vs_pmd_boss;
extern const struct SongHeader mus_vs_zinnia;
extern const struct SongHeader mus_vs_pmd_dialga;
extern const struct SongHeader mus_vs_hgss_ho_oh;
extern const struct SongHeader mus_vs_hgss_lugia;
extern const struct SongHeader mus_vs_hgss_champion;
extern const struct SongHeader mus_vs_hgss_rocket;

static const u32 sFacilities[] =
{
    BATTLE_TYPE_BATTLE_TOWER, BATTLE_TYPE_DOME, BATTLE_TYPE_PALACE,
    BATTLE_TYPE_ARENA, BATTLE_TYPE_FACTORY, BATTLE_TYPE_PIKE, BATTLE_TYPE_PYRAMID,
};

static void ResetMusic(void)
{
    FrontierMusic_StopPreview();
    memset(&gSaveBlock3Ptr->frontierMusic, 0, sizeof(struct FrontierMusicSave));
    memset(&gSaveBlock3Ptr->frontierMusicExtra, 0, sizeof(struct FrontierMusicExtraSave));
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
            || song == MUS_VS_COLOSSEUM || song == MUS_VS_COLOSSEUM_ADMIN || song == MUS_VS_COLOSSEUM_2
            || song == MUS_VS_PMD_RAYQUAZA || song == MUS_VS_PMD_BOSS || song == MUS_VS_ZINNIA
            || song == MUS_VS_PMD_DIALGA || song == MUS_VS_HGSS_HO_OH || song == MUS_VS_HGSS_LUGIA
            || song == MUS_VS_HGSS_CHAMPION || song == MUS_VS_HGSS_ROCKET);
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

TEST("Frontier music: Colosseum trainer 2 preserves guitar bends and nine tracks across two loops")
{
    bool32 sawBend = FALSE;
    EXPECT_EQ(MUS_VS_COLOSSEUM_2, 616);
    EXPECT_EQ(FrontierMusic_GetSong(23), MUS_VS_COLOSSEUM);
    EXPECT_EQ(FrontierMusic_GetSong(24), MUS_VS_COLOSSEUM_ADMIN);
    EXPECT_EQ(FrontierMusic_GetSong(25), MUS_VS_COLOSSEUM_2);
    EXPECT(gSongTable[MUS_VS_COLOSSEUM_2].header == &mus_vs_colosseum_2);
    EXPECT_EQ(gSongTable[MUS_VS_COLOSSEUM_2].ms, 0);
    EXPECT_EQ(mus_vs_colosseum_2.trackCount, 9);
    EXPECT_EQ(mus_vs_colosseum_2.tone[7].type & 7, 3);
    EXPECT_EQ(mus_vs_colosseum_2.tone[8].type & 7, 2);
    EXPECT_EQ(mus_vs_colosseum_2.tone[9].type & 7, 1);
    EXPECT_EQ(mus_vs_colosseum_2.tone[10].type & 7, 4);
    m4aSongNumStart(MUS_VS_COLOSSEUM_2);
    for (u32 frame = 0; frame < 9000; frame++)
    {
        MPlayMain(&gMPlayInfo_BGM);
        if (gMPlayInfo_BGM.tracks[1].bend != 0)
            sawBend = TRUE;
    }
    EXPECT(sawBend);
    EXPECT_EQ(gMPlayInfo_BGM.tracks[1].bendRange, 2);
    EXPECT_GT(gMPlayInfo_BGM.clock, 2 * 3264);
    EXPECT_EQ(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK, 0x1FF);
    for (u32 track = 0; track < 9; track++)
        EXPECT(gMPlayInfo_BGM.tracks[track].flags & MPT_FLG_EXIST);
    m4aMPlayStop(&gMPlayInfo_BGM);
}

TEST("Frontier music: Colosseum trainer 2 supports free preview five-BP unlock fixed and random selection")
{
    struct FrontierMusicSave before;
    ResetMusic();
    ClearStoryProgress();
    before = gSaveBlock3Ptr->frontierMusic;
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    gSaveBlock2Ptr->frontier.battlePoints = 0;
    EXPECT(FrontierMusic_IsAvailable(MUS_VS_COLOSSEUM_2));
    EXPECT(FrontierMusic_Preview(MUS_VS_COLOSSEUM_2));
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_colosseum_2);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    FrontierMusic_StopPreview();
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    gSaveBlock2Ptr->frontier.battlePoints = 5;
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_COLOSSEUM_2), FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_COLOSSEUM_2), FRONTIER_MUSIC_OK);
    FrontierMusic_EnsureSaveData();
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_COLOSSEUM_2);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_COLOSSEUM_2);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: adding trainer 2 preserves all previous 25 unlock bits and the admin selection")
{
    ResetMusic();
    gSaveBlock3Ptr->frontierMusic.unlocked = (1u << 25) - 1;
    gSaveBlock3Ptr->frontierMusic.selectedSong = MUS_VS_COLOSSEUM_ADMIN;
    gSaveBlock3Ptr->frontierMusic.mode = FRONTIER_MUSIC_FIXED;
    gSaveBlock3Ptr->frontierMusic.checksum = CalcCRC16((const u8 *)&gSaveBlock3Ptr->frontierMusic,
                                                     offsetof(struct FrontierMusicSave, checksum));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, (1u << 25) - 1);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.selectedSong, MUS_VS_COLOSSEUM_ADMIN);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.mode, FRONTIER_MUSIC_FIXED);
    EXPECT(!FrontierMusic_IsUnlocked(MUS_VS_COLOSSEUM_2));
}

TEST("Frontier music: rescue Rayquaza uses real PCM percussion and keeps its intro and loop running")
{
    static const u8 drumKeys[] = {36, 38, 39, 40, 41, 43, 53, 54, 55, 57, 64};
    const struct ToneData *kit = (const void *)mus_vs_pmd_rayquaza.tone[5].wav;
    EXPECT_EQ(MUS_VS_PMD_RAYQUAZA, 617);
    EXPECT_EQ(FrontierMusic_GetSong(25), MUS_VS_COLOSSEUM_2);
    EXPECT_EQ(FrontierMusic_GetSong(26), MUS_VS_PMD_RAYQUAZA);
    EXPECT(gSongTable[MUS_VS_PMD_RAYQUAZA].header == &mus_vs_pmd_rayquaza);
    EXPECT_EQ(gSongTable[MUS_VS_PMD_RAYQUAZA].ms, 0);
    EXPECT_EQ(mus_vs_pmd_rayquaza.trackCount, 9);
    for (u32 key = 0; key < ARRAY_COUNT(drumKeys); key++)
        EXPECT_EQ(kit[drumKeys[key]].type & TONEDATA_TYPE_CGB, 0);
    EXPECT_EQ(mus_vs_pmd_rayquaza.tone[6].type & 7, 3);
    EXPECT_EQ(mus_vs_pmd_rayquaza.tone[7].type & 7, 2);
    EXPECT_EQ(mus_vs_pmd_rayquaza.tone[8].type & 7, 1);
    EXPECT_EQ(mus_vs_pmd_rayquaza.tone[9].type & 7, 4);
    m4aSongNumStart(MUS_VS_PMD_RAYQUAZA);
    for (u32 frame = 0; frame < 15000; frame++)
        MPlayMain(&gMPlayInfo_BGM);
    EXPECT_GT(gMPlayInfo_BGM.clock, 7104 + 5376);
    EXPECT_EQ(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK, 0x1FF);
    for (u32 track = 0; track < 9; track++)
        EXPECT(gMPlayInfo_BGM.tracks[track].flags & MPT_FLG_EXIST);
    m4aMPlayStop(&gMPlayInfo_BGM);
}

TEST("Frontier music: rescue Rayquaza supports free preview five-BP unlock fixed and random selection")
{
    struct FrontierMusicSave before;
    ResetMusic();
    ClearStoryProgress();
    before = gSaveBlock3Ptr->frontierMusic;
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    gSaveBlock2Ptr->frontier.battlePoints = 0;
    EXPECT(FrontierMusic_IsAvailable(MUS_VS_PMD_RAYQUAZA));
    EXPECT(FrontierMusic_Preview(MUS_VS_PMD_RAYQUAZA));
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_pmd_rayquaza);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    FrontierMusic_StopPreview();
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    gSaveBlock2Ptr->frontier.battlePoints = 5;
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_PMD_RAYQUAZA), FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_PMD_RAYQUAZA), FRONTIER_MUSIC_OK);
    FrontierMusic_EnsureSaveData();
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_PMD_RAYQUAZA);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_PMD_RAYQUAZA);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: adding rescue Rayquaza preserves previous 26 unlocks and trainer 2 selection")
{
    ResetMusic();
    gSaveBlock3Ptr->frontierMusic.unlocked = (1u << 26) - 1;
    gSaveBlock3Ptr->frontierMusic.selectedSong = MUS_VS_COLOSSEUM_2;
    gSaveBlock3Ptr->frontierMusic.mode = FRONTIER_MUSIC_FIXED;
    gSaveBlock3Ptr->frontierMusic.checksum = CalcCRC16((const u8 *)&gSaveBlock3Ptr->frontierMusic,
                                                     offsetof(struct FrontierMusicSave, checksum));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, (1u << 26) - 1);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.selectedSong, MUS_VS_COLOSSEUM_2);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.mode, FRONTIER_MUSIC_FIXED);
    EXPECT(!FrontierMusic_IsUnlocked(MUS_VS_PMD_RAYQUAZA));
}

TEST("Frontier music: rescue boss has real PCM drums and all nine tracks survive multiple loops")
{
    static const u8 drumKeys[] = {36, 41, 43, 45, 47, 48, 53};
    const struct ToneData *kit = (const void *)mus_vs_pmd_boss.tone[4].wav;
    EXPECT_EQ(MUS_VS_PMD_BOSS, 618);
    EXPECT_EQ(FrontierMusic_GetSong(26), MUS_VS_PMD_RAYQUAZA);
    EXPECT_EQ(FrontierMusic_GetSong(27), MUS_VS_PMD_BOSS);
    EXPECT(gSongTable[MUS_VS_PMD_BOSS].header == &mus_vs_pmd_boss);
    EXPECT_EQ(gSongTable[MUS_VS_PMD_BOSS].ms, 0);
    EXPECT_EQ(mus_vs_pmd_boss.trackCount, 9);
    for (u32 key = 0; key < ARRAY_COUNT(drumKeys); key++)
        EXPECT_EQ(kit[drumKeys[key]].type & TONEDATA_TYPE_CGB, 0);
    EXPECT_EQ(mus_vs_pmd_boss.tone[5].type & 7, 3);
    EXPECT_EQ(mus_vs_pmd_boss.tone[6].type & 7, 2);
    EXPECT_EQ(mus_vs_pmd_boss.tone[7].type & 7, 1);
    EXPECT_EQ(mus_vs_pmd_boss.tone[8].type & 7, 4);
    m4aSongNumStart(MUS_VS_PMD_BOSS);
    for (u32 frame = 0; frame < 10000; frame++)
        MPlayMain(&gMPlayInfo_BGM);
    EXPECT_GT(gMPlayInfo_BGM.clock, 1932 + 2 * 1920);
    EXPECT_EQ(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK, 0x1FF);
    for (u32 track = 0; track < 9; track++)
        EXPECT(gMPlayInfo_BGM.tracks[track].flags & MPT_FLG_EXIST);
    m4aMPlayStop(&gMPlayInfo_BGM);
}

TEST("Frontier music: rescue boss supports free preview five-BP unlock fixed random and Brain override")
{
    struct FrontierMusicSave before;
    ResetMusic();
    ClearStoryProgress();
    before = gSaveBlock3Ptr->frontierMusic;
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    gSaveBlock2Ptr->frontier.battlePoints = 0;
    EXPECT(FrontierMusic_IsAvailable(MUS_VS_PMD_BOSS));
    EXPECT(FrontierMusic_Preview(MUS_VS_PMD_BOSS));
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_pmd_boss);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    FrontierMusic_StopPreview();
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    gSaveBlock2Ptr->frontier.battlePoints = 5;
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_PMD_BOSS), FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_PMD_BOSS), FRONTIER_MUSIC_OK);
    FrontierMusic_EnsureSaveData();
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_PMD_BOSS);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_PMD_BOSS);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: adding rescue boss preserves previous 27 unlock bits and Rayquaza selection")
{
    ResetMusic();
    gSaveBlock3Ptr->frontierMusic.unlocked = (1u << 27) - 1;
    gSaveBlock3Ptr->frontierMusic.selectedSong = MUS_VS_PMD_RAYQUAZA;
    gSaveBlock3Ptr->frontierMusic.mode = FRONTIER_MUSIC_FIXED;
    gSaveBlock3Ptr->frontierMusic.checksum = CalcCRC16((const u8 *)&gSaveBlock3Ptr->frontierMusic,
                                                     offsetof(struct FrontierMusicSave, checksum));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, (1u << 27) - 1);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.selectedSong, MUS_VS_PMD_RAYQUAZA);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.mode, FRONTIER_MUSIC_FIXED);
    EXPECT(!FrontierMusic_IsUnlocked(MUS_VS_PMD_BOSS));
}

TEST("Frontier music: Zinnia appends to Hoenn and keeps all nine tracks and expression through its loop")
{
    static const u8 drumKeys[] = {36, 40, 55, 57, 62, 64};
    const struct ToneData *kit = (const void *)mus_vs_zinnia.tone[4].wav;
    bool32 expressionChanged = FALSE;
    EXPECT_EQ(MUS_VS_ZINNIA, 619);
    EXPECT_EQ(FrontierMusic_GetSong(27), MUS_VS_PMD_BOSS);
    EXPECT_EQ(FrontierMusic_GetSong(28), MUS_VS_ZINNIA);
    EXPECT_EQ(FrontierMusic_GetCategory(MUS_VS_ZINNIA), FRONTIER_MUSIC_CATEGORY_HOENN);
    EXPECT(gSongTable[MUS_VS_ZINNIA].header == &mus_vs_zinnia);
    EXPECT_EQ(gSongTable[MUS_VS_ZINNIA].ms, 0);
    EXPECT_EQ(mus_vs_zinnia.trackCount, 9);
    for (u32 key = 0; key < ARRAY_COUNT(drumKeys); key++)
        EXPECT_EQ(kit[drumKeys[key]].type & TONEDATA_TYPE_CGB, 0);
    EXPECT_EQ(mus_vs_zinnia.tone[6].type & 7, 3);
    EXPECT_EQ(mus_vs_zinnia.tone[7].type & 7, 2);
    EXPECT_EQ(mus_vs_zinnia.tone[8].type & 7, 1);
    EXPECT_EQ(mus_vs_zinnia.tone[9].type & 7, 4);
    m4aSongNumStart(MUS_VS_ZINNIA);
    u8 volume = gMPlayInfo_BGM.tracks[0].vol;
    for (u32 frame = 0; frame < 12000; frame++)
    {
        MPlayMain(&gMPlayInfo_BGM);
        if (gMPlayInfo_BGM.tracks[0].vol != volume)
            expressionChanged = TRUE;
    }
    EXPECT(expressionChanged);
    EXPECT_GT(gMPlayInfo_BGM.clock, 6720 + 5760);
    EXPECT_EQ(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK, 0x1FF);
    for (u32 track = 0; track < 9; track++)
        EXPECT(gMPlayInfo_BGM.tracks[track].flags & MPT_FLG_EXIST);
    m4aMPlayStop(&gMPlayInfo_BGM);
}

TEST("Frontier music: Zinnia supports free preview five-BP unlock fixed random and Brain override")
{
    struct FrontierMusicSave before;
    ResetMusic();
    ClearStoryProgress();
    before = gSaveBlock3Ptr->frontierMusic;
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    gSaveBlock2Ptr->frontier.battlePoints = 0;
    EXPECT(FrontierMusic_Preview(MUS_VS_ZINNIA));
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_zinnia);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    FrontierMusic_StopPreview();
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    gSaveBlock2Ptr->frontier.battlePoints = 5;
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_ZINNIA), FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_ZINNIA), FRONTIER_MUSIC_OK);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_ZINNIA);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_ZINNIA);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: category pages and Zinnia preserve previous 28 unlock bits and boss selection")
{
    ResetMusic();
    gSaveBlock3Ptr->frontierMusic.unlocked = (1u << 28) - 1;
    gSaveBlock3Ptr->frontierMusic.selectedSong = MUS_VS_PMD_BOSS;
    gSaveBlock3Ptr->frontierMusic.mode = FRONTIER_MUSIC_FIXED;
    gSaveBlock3Ptr->frontierMusic.checksum = CalcCRC16((const u8 *)&gSaveBlock3Ptr->frontierMusic,
                                                     offsetof(struct FrontierMusicSave, checksum));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, (1u << 28) - 1);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.selectedSong, MUS_VS_PMD_BOSS);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.mode, FRONTIER_MUSIC_FIXED);
    EXPECT(!FrontierMusic_IsUnlocked(MUS_VS_ZINNIA));
}

TEST("Frontier music: every song has exactly one work category without reindexing saved unlocks")
{
    u8 counts[FRONTIER_MUSIC_CATEGORY_COUNT] = {0};
    for (u32 i = 0; i < FRONTIER_MUSIC_TRACK_COUNT; i++)
    {
        u8 category = FrontierMusic_GetCategory(FrontierMusic_GetSong(i));
        EXPECT_LT(category, FRONTIER_MUSIC_CATEGORY_COUNT);
        counts[category]++;
    }
    EXPECT_EQ(counts[FRONTIER_MUSIC_CATEGORY_KANTO], 8);
    EXPECT_EQ(counts[FRONTIER_MUSIC_CATEGORY_JOHTO], 4);
    EXPECT_EQ(counts[FRONTIER_MUSIC_CATEGORY_HOENN], 14);
    EXPECT_EQ(counts[FRONTIER_MUSIC_CATEGORY_ORRE], 3);
    EXPECT_EQ(counts[FRONTIER_MUSIC_CATEGORY_DUNGEON], 3);
    EXPECT_EQ(counts[FRONTIER_MUSIC_CATEGORY_ORIGINAL], 2);
    EXPECT_EQ(FrontierMusic_GetCategory(MUS_NONE), FRONTIER_MUSIC_CATEGORY_COUNT);
}

TEST("Frontier music: Explorers Dialga keeps nine tracks active past its two-variation loop")
{
    static const u8 drumKeys[] = {36, 38, 39, 40, 53, 55, 57, 62, 64};
    const struct ToneData *kit = (const void *)mus_vs_pmd_dialga.tone[4].wav;
    EXPECT_EQ(MUS_VS_PMD_DIALGA, 620);
    EXPECT_EQ(FrontierMusic_GetSong(28), MUS_VS_ZINNIA);
    EXPECT_EQ(FrontierMusic_GetSong(29), MUS_VS_PMD_DIALGA);
    EXPECT_EQ(FrontierMusic_GetCategory(MUS_VS_PMD_DIALGA), FRONTIER_MUSIC_CATEGORY_DUNGEON);
    EXPECT(gSongTable[MUS_VS_PMD_DIALGA].header == &mus_vs_pmd_dialga);
    EXPECT_EQ(gSongTable[MUS_VS_PMD_DIALGA].ms, 0);
    EXPECT_EQ(mus_vs_pmd_dialga.trackCount, 9);
    for (u32 key = 0; key < ARRAY_COUNT(drumKeys); key++)
        EXPECT_EQ(kit[drumKeys[key]].type & TONEDATA_TYPE_CGB, 0);
    EXPECT_EQ(mus_vs_pmd_dialga.tone[5].type & 7, 3);
    EXPECT_EQ(mus_vs_pmd_dialga.tone[6].type & 7, 2);
    EXPECT_EQ(mus_vs_pmd_dialga.tone[7].type & 7, 1);
    EXPECT_EQ(mus_vs_pmd_dialga.tone[8].type & 7, 4);
    m4aSongNumStart(MUS_VS_PMD_DIALGA);
    for (u32 frame = 0; frame < 30000; frame++)
        MPlayMain(&gMPlayInfo_BGM);
    EXPECT_GT(gMPlayInfo_BGM.clock, 13080 + 11904);
    EXPECT_EQ(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK, 0x1FF);
    for (u32 track = 0; track < 9; track++)
        EXPECT(gMPlayInfo_BGM.tracks[track].flags & MPT_FLG_EXIST);
    m4aMPlayStop(&gMPlayInfo_BGM);
}

TEST("Frontier music: Explorers Dialga supports free preview five-BP unlock fixed random and Brain override")
{
    struct FrontierMusicSave before;
    ResetMusic();
    ClearStoryProgress();
    before = gSaveBlock3Ptr->frontierMusic;
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    gSaveBlock2Ptr->frontier.battlePoints = 0;
    EXPECT(FrontierMusic_Preview(MUS_VS_PMD_DIALGA));
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_pmd_dialga);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    FrontierMusic_StopPreview();
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    gSaveBlock2Ptr->frontier.battlePoints = 5;
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_PMD_DIALGA), FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_PMD_DIALGA), FRONTIER_MUSIC_OK);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_PMD_DIALGA);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_PMD_DIALGA);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: adding Dialga preserves previous 29 unlock bits and Zinnia selection")
{
    ResetMusic();
    gSaveBlock3Ptr->frontierMusic.unlocked = (1u << 29) - 1;
    gSaveBlock3Ptr->frontierMusic.selectedSong = MUS_VS_ZINNIA;
    gSaveBlock3Ptr->frontierMusic.mode = FRONTIER_MUSIC_FIXED;
    gSaveBlock3Ptr->frontierMusic.checksum = CalcCRC16((const u8 *)&gSaveBlock3Ptr->frontierMusic,
                                                     offsetof(struct FrontierMusicSave, checksum));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, (1u << 29) - 1);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.selectedSong, MUS_VS_ZINNIA);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.mode, FRONTIER_MUSIC_FIXED);
    EXPECT(!FrontierMusic_IsUnlocked(MUS_VS_PMD_DIALGA));
}

TEST("Frontier music: HGSS Ho-Oh preserves bends tempo changes and nine tracks across the loop")
{
    static const u8 drumKeys[] = {36, 39, 40, 41, 43, 45, 47, 48, 53, 55, 57, 62, 64};
    const struct ToneData *kit = (const void *)mus_vs_hgss_ho_oh.tone[4].wav;
    bool32 bent = FALSE;
    EXPECT_EQ(MUS_VS_HGSS_HO_OH, 621);
    EXPECT_EQ(FrontierMusic_GetSong(30), MUS_VS_HGSS_HO_OH);
    EXPECT_EQ(FrontierMusic_GetCategory(MUS_VS_HGSS_HO_OH), FRONTIER_MUSIC_CATEGORY_JOHTO);
    EXPECT(gSongTable[MUS_VS_HGSS_HO_OH].header == &mus_vs_hgss_ho_oh);
    EXPECT_EQ(gSongTable[MUS_VS_HGSS_HO_OH].ms, 0);
    EXPECT_EQ(mus_vs_hgss_ho_oh.trackCount, 9);
    for (u32 key = 0; key < ARRAY_COUNT(drumKeys); key++)
        EXPECT_EQ(kit[drumKeys[key]].type & TONEDATA_TYPE_CGB, 0);
    EXPECT_EQ(mus_vs_hgss_ho_oh.tone[12].type & 7, 3);
    EXPECT_EQ(mus_vs_hgss_ho_oh.tone[13].type & 7, 2);
    EXPECT_EQ(mus_vs_hgss_ho_oh.tone[14].type & 7, 1);
    EXPECT_EQ(mus_vs_hgss_ho_oh.tone[15].type & 7, 4);
    m4aSongNumStart(MUS_VS_HGSS_HO_OH);
    for (u32 frame = 0; frame < 15000; frame++)
    {
        MPlayMain(&gMPlayInfo_BGM);
        for (u32 track = 0; track < 9; track++)
            bent |= gMPlayInfo_BGM.tracks[track].bend != 0;
    }
    EXPECT(bent);
    EXPECT_GT(gMPlayInfo_BGM.clock, 6240 + 4992);
    EXPECT_EQ(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK, 0x1FF);
    for (u32 track = 0; track < 9; track++)
    {
        EXPECT(gMPlayInfo_BGM.tracks[track].flags & MPT_FLG_EXIST);
        EXPECT_EQ(gMPlayInfo_BGM.tracks[track].bendRange, 2);
    }
    m4aMPlayStop(&gMPlayInfo_BGM);
}

TEST("Frontier music: HGSS Ho-Oh supports free preview five-BP unlock fixed random and Brain override")
{
    struct FrontierMusicSave before;
    ResetMusic();
    ClearStoryProgress();
    before = gSaveBlock3Ptr->frontierMusic;
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    gSaveBlock2Ptr->frontier.battlePoints = 0;
    EXPECT(FrontierMusic_Preview(MUS_VS_HGSS_HO_OH));
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_hgss_ho_oh);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    FrontierMusic_StopPreview();
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    gSaveBlock2Ptr->frontier.battlePoints = 5;
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_HGSS_HO_OH), FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_HGSS_HO_OH), FRONTIER_MUSIC_OK);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_HGSS_HO_OH);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_HGSS_HO_OH);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: adding Ho-Oh preserves previous 30 unlock bits Dialga and Kanto Suicune")
{
    u16 selected = MUS_VS_PMD_DIALGA;
    PARAMETRIZE { selected = MUS_VS_PMD_DIALGA; }
    PARAMETRIZE { selected = MUS_C_VS_LEGEND_BEAST; }
    ResetMusic();
    gSaveBlock3Ptr->frontierMusic.unlocked = (1u << 30) - 1;
    gSaveBlock3Ptr->frontierMusic.selectedSong = selected;
    gSaveBlock3Ptr->frontierMusic.mode = FRONTIER_MUSIC_FIXED;
    gSaveBlock3Ptr->frontierMusic.checksum = CalcCRC16((const u8 *)&gSaveBlock3Ptr->frontierMusic,
                                                     offsetof(struct FrontierMusicSave, checksum));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, (1u << 30) - 1);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.selectedSong, selected);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.mode, FRONTIER_MUSIC_FIXED);
    EXPECT_EQ(FrontierMusic_GetSong(20), MUS_C_VS_LEGEND_BEAST);
    EXPECT_EQ(FrontierMusic_GetCategory(MUS_C_VS_LEGEND_BEAST), FRONTIER_MUSIC_CATEGORY_KANTO);
    EXPECT(!FrontierMusic_IsUnlocked(MUS_VS_HGSS_HO_OH));
}

TEST("Frontier music: HGSS Lugia uses native percussion and nine tracks across two loops")
{
    static const u8 drumKeys[] = {38, 53, 55, 57};
    const struct ToneData *kit = (const void *)mus_vs_hgss_lugia.tone[19].wav;
    EXPECT_EQ(MUS_VS_HGSS_LUGIA, 622);
    EXPECT_EQ(FrontierMusic_GetSong(31), MUS_VS_HGSS_LUGIA);
    EXPECT_EQ(FrontierMusic_GetCategory(MUS_VS_HGSS_LUGIA), FRONTIER_MUSIC_CATEGORY_JOHTO);
    EXPECT(gSongTable[MUS_VS_HGSS_LUGIA].header == &mus_vs_hgss_lugia);
    EXPECT_EQ(gSongTable[MUS_VS_HGSS_LUGIA].ms, 0);
    EXPECT_EQ(mus_vs_hgss_lugia.trackCount, 9);
    for (u32 key = 0; key < ARRAY_COUNT(drumKeys); key++)
        EXPECT_EQ(kit[drumKeys[key]].type & TONEDATA_TYPE_CGB, 0);
    EXPECT_EQ(mus_vs_hgss_lugia.tone[5].type & TONEDATA_TYPE_CGB, 0);
    EXPECT_EQ(mus_vs_hgss_lugia.tone[6].type & 7, 3);
    EXPECT_EQ(mus_vs_hgss_lugia.tone[7].type & 7, 2);
    EXPECT_EQ(mus_vs_hgss_lugia.tone[8].type & 7, 1);
    EXPECT_EQ(mus_vs_hgss_lugia.tone[9].type & 7, 4);
    m4aSongNumStart(MUS_VS_HGSS_LUGIA);
    for (u32 frame = 0; frame < 15000; frame++)
        MPlayMain(&gMPlayInfo_BGM);
    EXPECT_GT(gMPlayInfo_BGM.clock, 5352 + 2 * 3840);
    EXPECT_EQ(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK, 0x1FF);
    for (u32 track = 0; track < 9; track++)
        EXPECT(gMPlayInfo_BGM.tracks[track].flags & MPT_FLG_EXIST);
    m4aMPlayStop(&gMPlayInfo_BGM);
}

TEST("Frontier music: HGSS Lugia supports free preview five-BP unlock fixed random and Brain override")
{
    struct FrontierMusicSave before;
    ResetMusic();
    ClearStoryProgress();
    before = gSaveBlock3Ptr->frontierMusic;
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    gSaveBlock2Ptr->frontier.battlePoints = 0;
    EXPECT(FrontierMusic_Preview(MUS_VS_HGSS_LUGIA));
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_hgss_lugia);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    FrontierMusic_StopPreview();
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    gSaveBlock2Ptr->frontier.battlePoints = 5;
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_HGSS_LUGIA), FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, 0x80000000u);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_HGSS_LUGIA), FRONTIER_MUSIC_OK);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_HGSS_LUGIA);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0), FRONTIER_MUSIC_OK);
    for (u32 i = 0; i < 32; i++)
        EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_HGSS_LUGIA);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: adding Lugia preserves previous 31 unlock bits and Ho-Oh selection")
{
    ResetMusic();
    gSaveBlock3Ptr->frontierMusic.unlocked = 0x7FFFFFFFu;
    gSaveBlock3Ptr->frontierMusic.selectedSong = MUS_VS_HGSS_HO_OH;
    gSaveBlock3Ptr->frontierMusic.mode = FRONTIER_MUSIC_FIXED;
    gSaveBlock3Ptr->frontierMusic.checksum = CalcCRC16((const u8 *)&gSaveBlock3Ptr->frontierMusic,
                                                     offsetof(struct FrontierMusicSave, checksum));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, 0x7FFFFFFFu);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.selectedSong, MUS_VS_HGSS_HO_OH);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.mode, FRONTIER_MUSIC_FIXED);
    EXPECT(!FrontierMusic_IsUnlocked(MUS_VS_HGSS_LUGIA));
}

TEST("Frontier music: original 32 unlock bits survive reload in the unchanged primary record")
{
    struct FrontierMusicSave saved;
    ResetMusic();
    gSaveBlock3Ptr->frontierMusic.unlocked = 0xFFFFFFFFu;
    gSaveBlock3Ptr->frontierMusic.selectedSong = MUS_VS_HGSS_LUGIA;
    gSaveBlock3Ptr->frontierMusic.mode = FRONTIER_MUSIC_FIXED;
    gSaveBlock3Ptr->frontierMusic.checksum = CalcCRC16((const u8 *)&gSaveBlock3Ptr->frontierMusic,
                                                     offsetof(struct FrontierMusicSave, checksum));
    saved = gSaveBlock3Ptr->frontierMusic;
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(sizeof(saved), 16);
    EXPECT_EQ(offsetof(struct SaveBlock3, fusions), 168);
    EXPECT_EQ(memcmp(&saved, &gSaveBlock3Ptr->frontierMusic, sizeof(saved)), 0);
    for (u32 i = 0; i < 32; i++)
        EXPECT(FrontierMusic_IsUnlocked(FrontierMusic_GetSong(i)));
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_HGSS_LUGIA), FRONTIER_MUSIC_ALREADY_UNLOCKED);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
    EXPECT(!FrontierMusic_IsUnlocked(MUS_NONE));
    EXPECT_EQ(FrontierMusic_Unlock(MUS_NONE), FRONTIER_MUSIC_INVALID);
    EXPECT_EQ(memcmp(&saved, &gSaveBlock3Ptr->frontierMusic, sizeof(saved)), 0);
}

TEST("Frontier music: HGSS Champion preserves guitar bends and nine tracks through two loops")
{
    static const u8 drumKeys[] = {36, 38, 43, 45, 47, 48, 53, 55, 57};
    const struct ToneData *kit = (const void *)mus_vs_hgss_champion.tone[4].wav;
    bool32 bent = FALSE;
    EXPECT_EQ(MUS_VS_HGSS_CHAMPION, 623);
    EXPECT_EQ(FrontierMusic_GetSong(32), MUS_VS_HGSS_CHAMPION);
    EXPECT_EQ(FrontierMusic_GetCategory(MUS_VS_HGSS_CHAMPION), FRONTIER_MUSIC_CATEGORY_JOHTO);
    EXPECT(gSongTable[MUS_VS_HGSS_CHAMPION].header == &mus_vs_hgss_champion);
    EXPECT_EQ(mus_vs_hgss_champion.trackCount, 9);
    for (u32 key = 0; key < ARRAY_COUNT(drumKeys); key++)
        EXPECT_EQ(kit[drumKeys[key]].type & TONEDATA_TYPE_CGB, 0);
    EXPECT_EQ(mus_vs_hgss_champion.tone[6].type & 7, 3);
    EXPECT_EQ(mus_vs_hgss_champion.tone[7].type & 7, 2);
    EXPECT_EQ(mus_vs_hgss_champion.tone[8].type & 7, 1);
    EXPECT_EQ(mus_vs_hgss_champion.tone[9].type & 7, 4);
    m4aSongNumStart(MUS_VS_HGSS_CHAMPION);
    for (u32 frame = 0; frame < 15000; frame++)
    {
        MPlayMain(&gMPlayInfo_BGM);
        bent |= gMPlayInfo_BGM.tracks[2].bend != 0;
    }
    EXPECT(bent);
    EXPECT_EQ(gMPlayInfo_BGM.tracks[2].bendRange, 12);
    EXPECT_GT(gMPlayInfo_BGM.clock, 4992 + 2 * 3264);
    EXPECT_EQ(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK, 0x1FF);
    m4aMPlayStop(&gMPlayInfo_BGM);
}

TEST("Frontier music: HGSS Champion previews freely and unlocks in the extra word for five BP")
{
    struct FrontierMusicSave saved;
    struct FrontierMusicExtraSave extra;
    ResetMusic();
    ClearStoryProgress();
    saved = gSaveBlock3Ptr->frontierMusic;
    extra = gSaveBlock3Ptr->frontierMusicExtra;
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    gSaveBlock2Ptr->frontier.battlePoints = 0;
    EXPECT(FrontierMusic_Preview(MUS_VS_HGSS_CHAMPION));
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_hgss_champion);
    FrontierMusic_StopPreview();
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    EXPECT_EQ(memcmp(&saved, &gSaveBlock3Ptr->frontierMusic, sizeof(saved)), 0);
    EXPECT_EQ(memcmp(&extra, &gSaveBlock3Ptr->frontierMusicExtra, sizeof(extra)), 0);
    gSaveBlock2Ptr->frontier.battlePoints = 5;
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_HGSS_CHAMPION), FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, 0);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusicExtra.unlocked[0], 1);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_HGSS_CHAMPION), FRONTIER_MUSIC_OK);
    saved = gSaveBlock3Ptr->frontierMusic;
    extra = gSaveBlock3Ptr->frontierMusicExtra;
    memset(&gSaveBlock3Ptr->frontierMusic, 0, sizeof(saved));
    memset(&gSaveBlock3Ptr->frontierMusicExtra, 0, sizeof(extra));
    gSaveBlock3Ptr->frontierMusic = saved;
    gSaveBlock3Ptr->frontierMusicExtra = extra;
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(memcmp(&saved, &gSaveBlock3Ptr->frontierMusic, sizeof(saved)), 0);
    EXPECT_EQ(memcmp(&extra, &gSaveBlock3Ptr->frontierMusicExtra, sizeof(extra)), 0);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_HGSS_CHAMPION);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0), FRONTIER_MUSIC_OK);
    for (u32 i = 0; i < 32; i++)
        EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_HGSS_CHAMPION);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: HGSS Rocket preserves nine compiled tracks and bends through two loops")
{
    static const u8 drumKeys[] = {36, 38, 41, 43, 45, 47, 48, 55};
    const struct ToneData *kit = (const void *)mus_vs_hgss_rocket.tone[16].wav;
    bool32 bent = FALSE;
    bool32 wideBend = FALSE;
    EXPECT_EQ(MUS_VS_HGSS_ROCKET, 624);
    EXPECT_EQ(FrontierMusic_GetSong(33), MUS_VS_HGSS_ROCKET);
    EXPECT_EQ(FrontierMusic_GetCategory(MUS_VS_HGSS_ROCKET), FRONTIER_MUSIC_CATEGORY_JOHTO);
    EXPECT(gSongTable[MUS_VS_HGSS_ROCKET].header == &mus_vs_hgss_rocket);
    EXPECT_EQ(mus_vs_hgss_rocket.trackCount, 9);
    for (u32 key = 0; key < ARRAY_COUNT(drumKeys); key++)
        EXPECT_EQ(kit[drumKeys[key]].type & TONEDATA_TYPE_CGB, 0);
    EXPECT_EQ(mus_vs_hgss_rocket.tone[11].type & 7, 3);
    EXPECT_EQ(mus_vs_hgss_rocket.tone[12].type & 7, 2);
    EXPECT_EQ(mus_vs_hgss_rocket.tone[13].type & 7, 1);
    EXPECT_EQ(mus_vs_hgss_rocket.tone[14].type & 7, 4);
    m4aSongNumStart(MUS_VS_HGSS_ROCKET);
    for (u32 frame = 0; frame < 15000; frame++)
    {
        MPlayMain(&gMPlayInfo_BGM);
        bent |= gMPlayInfo_BGM.tracks[2].bend != 0;
        wideBend |= gMPlayInfo_BGM.tracks[2].bendRange == 12;
    }
    EXPECT(bent);
    EXPECT(wideBend);
    EXPECT_GT(gMPlayInfo_BGM.clock, 2400 + 2 * 1920);
    EXPECT_EQ(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK, 0x1FF);
    m4aMPlayStop(&gMPlayInfo_BGM);
}

TEST("Frontier music: HGSS Rocket preview and purchase preserve Champion unlocks and save layout")
{
    u8 prefix[588];
    struct FrontierMusicExtraSave extra;
    ResetMusic();
    ClearStoryProgress();
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_HGSS_CHAMPION), FRONTIER_MUSIC_OK);
    memcpy(prefix, gSaveBlock3Ptr, sizeof(prefix));
    extra = gSaveBlock3Ptr->frontierMusicExtra;
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(memcmp(prefix, gSaveBlock3Ptr, sizeof(prefix)), 0);
    EXPECT(!FrontierMusic_IsUnlocked(MUS_VS_HGSS_ROCKET));
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    gSaveBlock2Ptr->frontier.battlePoints = 0;
    EXPECT(FrontierMusic_Preview(MUS_VS_HGSS_ROCKET));
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_hgss_rocket);
    FrontierMusic_StopPreview();
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    EXPECT_EQ(memcmp(prefix, gSaveBlock3Ptr, sizeof(prefix)), 0);
    EXPECT_EQ(memcmp(&extra, &gSaveBlock3Ptr->frontierMusicExtra, sizeof(extra)), 0);
    gSaveBlock2Ptr->frontier.battlePoints = 5;
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_HGSS_ROCKET), FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(memcmp(prefix, gSaveBlock3Ptr, sizeof(prefix)), 0);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusicExtra.unlocked[0], 3);
    EXPECT(FrontierMusic_IsUnlocked(MUS_VS_HGSS_CHAMPION));
    EXPECT(FrontierMusic_IsUnlocked(MUS_VS_HGSS_ROCKET));
    extra = gSaveBlock3Ptr->frontierMusicExtra;
    memset(&gSaveBlock3Ptr->frontierMusicExtra, 0, sizeof(extra));
    gSaveBlock3Ptr->frontierMusicExtra = extra;
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(memcmp(&extra, &gSaveBlock3Ptr->frontierMusicExtra, sizeof(extra)), 0);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_HGSS_ROCKET), FRONTIER_MUSIC_OK);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_HGSS_ROCKET);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier music: HGSS Rocket works as the only random extra-word song without advancing primary RNG")
{
    ResetMusic();
    ClearStoryProgress();
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_HGSS_ROCKET), FRONTIER_MUSIC_OK);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, 0);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusicExtra.unlocked[0], 2);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_RANDOM, 0), FRONTIER_MUSIC_OK);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    rng_value_t rng = gRngValue;
    for (u32 i = 0; i < 32; i++)
        EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_HGSS_ROCKET);
    EXPECT_EQ(memcmp(&gRngValue, &rng, sizeof(rng)), 0);
}

TEST("Frontier music: extra flags migrate old saves without moving any legacy bytes")
{
    u8 prefix[588];
    u8 fill = 0;
    PARAMETRIZE { fill = 0; }
    PARAMETRIZE { fill = 0xFF; }
    ResetMusic();
    gSaveBlock3Ptr->frontierMusic.unlocked = 0xFFFFFFFFu;
    gSaveBlock3Ptr->frontierMusic.mode = FRONTIER_MUSIC_FIXED;
    gSaveBlock3Ptr->frontierMusic.selectedSong = MUS_VS_HGSS_LUGIA;
    gSaveBlock3Ptr->frontierMusic.checksum = CalcCRC16((const u8 *)&gSaveBlock3Ptr->frontierMusic,
                                                     offsetof(struct FrontierMusicSave, checksum));
    memcpy(prefix, gSaveBlock3Ptr, sizeof(prefix));
    memset(&gSaveBlock3Ptr->frontierMusicExtra, fill, sizeof(struct FrontierMusicExtraSave));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(offsetof(struct SaveBlock3, frontierMusicExtra), sizeof(prefix));
    EXPECT_EQ(memcmp(prefix, gSaveBlock3Ptr, sizeof(prefix)), 0);
    EXPECT_EQ(sizeof(struct FrontierMusicSave), 16);
    EXPECT_EQ(sizeof(struct FrontierMusicExtraSave), 20);
    EXPECT_EQ(sizeof(gSaveBlock3Ptr->frontierMusicExtra.unlocked) * 8 + 32, 128);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusicExtra.unlocked[0], 0);
    EXPECT(!FrontierMusic_IsUnlocked(MUS_VS_HGSS_CHAMPION));
}

TEST("Frontier music: corrupt extra flags do not destroy original unlocks or neighboring records")
{
    u8 scenario = 0;
    PARAMETRIZE { scenario = 0; }
    PARAMETRIZE { scenario = 1; }
    PARAMETRIZE { scenario = 2; }
    ResetMusic();
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_TRAINER), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_HGSS_CHAMPION), FRONTIER_MUSIC_OK);
    EXPECT_EQ(FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_VS_HGSS_CHAMPION), FRONTIER_MUSIC_OK);
    struct FrontierPokevirusSave neighbor = gSaveBlock3Ptr->pokevirus;
    if (scenario == 0)
        gSaveBlock3Ptr->frontierMusicExtra.unlocked[0] ^= 1;
    else if (scenario == 1)
        gSaveBlock3Ptr->frontierMusicExtra.magic ^= 1;
    else
    {
        gSaveBlock3Ptr->frontierMusicExtra.version++;
        gSaveBlock3Ptr->frontierMusicExtra.checksum = CalcCRC16((const u8 *)&gSaveBlock3Ptr->frontierMusicExtra,
                                                              offsetof(struct FrontierMusicExtraSave, checksum));
    }
    FrontierMusic_EnsureSaveData();
    EXPECT(FrontierMusic_IsUnlocked(MUS_VS_TRAINER));
    EXPECT(!FrontierMusic_IsUnlocked(MUS_VS_HGSS_CHAMPION));
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.mode, FRONTIER_MUSIC_DEFAULT);
    EXPECT_EQ(memcmp(&neighbor, &gSaveBlock3Ptr->pokevirus, sizeof(neighbor)), 0);
}

TEST("Frontier music: future extra bits are masked without shifting by a whole word")
{
    ResetMusic();
    struct FrontierMusicExtraSave *extra = &gSaveBlock3Ptr->frontierMusicExtra;
    for (u32 i = 0; i < ARRAY_COUNT(extra->unlocked); i++)
        extra->unlocked[i] = 0xFFFFFFFFu;
    extra->checksum = CalcCRC16((const u8 *)extra, offsetof(struct FrontierMusicExtraSave, checksum));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(extra->unlocked[0], (1u << (FRONTIER_MUSIC_TRACK_COUNT - 32)) - 1);
    EXPECT_EQ(extra->unlocked[1], 0);
    EXPECT_EQ(extra->unlocked[2], 0);
    EXPECT(FrontierMusic_IsUnlocked(MUS_VS_HGSS_CHAMPION));
    EXPECT(FrontierMusic_IsUnlocked(MUS_VS_HGSS_ROCKET));
    EXPECT(!FrontierMusic_IsUnlocked(MUS_NONE));
    EXPECT_EQ(FrontierMusic_Unlock(MUS_NONE), FRONTIER_MUSIC_INVALID);
}

TEST("Frontier music: resetting invalid primary music data also resets its extra unlocks")
{
    ResetMusic();
    EXPECT_EQ(FrontierMusic_Unlock(MUS_VS_HGSS_CHAMPION), FRONTIER_MUSIC_OK);
    memset(&gSaveBlock3Ptr->frontierMusic, 0, sizeof(struct FrontierMusicSave));
    FrontierMusic_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusic.unlocked, 0);
    EXPECT_EQ(gSaveBlock3Ptr->frontierMusicExtra.unlocked[0], 0);
    EXPECT(!FrontierMusic_IsUnlocked(MUS_VS_HGSS_CHAMPION));
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

static u8 CatalogListTask(void)
{
    for (u32 task = 0; task < NUM_TASKS; task++)
    {
        const struct ListMenu *list = (const void *)gTasks[task].data;
        if (gTasks[task].isActive && gTasks[task].priority == 0
         && list->template.fontId == FONT_NORMAL && list->template.maxShowed > 0
         && list->template.maxShowed <= 5 && list->template.moveCursorFunc != NULL)
            return task;
    }
    return TASK_NONE;
}

static u8 CatalogFixedWindow(u8 listWindow, s32 top)
{
    for (u32 window = 0; window < WINDOWS_MAX; window++)
        if (gWindows[window].tileData != NULL
         && GetWindowAttribute(window, WINDOW_BG) == GetWindowAttribute(listWindow, WINDOW_BG)
         && GetWindowAttribute(window, WINDOW_TILEMAP_LEFT) == GetWindowAttribute(listWindow, WINDOW_TILEMAP_LEFT)
         && GetWindowAttribute(window, WINDOW_TILEMAP_TOP) == top
         && GetWindowAttribute(window, WINDOW_HEIGHT) == 2)
            return window;
    return WINDOW_NONE;
}

static void (*sCatalogCursorCallback)(s32, bool8, struct ListMenu *);
static u32 sCatalogInitCallbacks;
static u32 sCatalogMoveCallbacks;

static void CountCatalogCursorCallbacks(s32 index, bool8 onInit, struct ListMenu *list)
{
    if (onInit)
        sCatalogInitCallbacks++;
    else
        sCatalogMoveCallbacks++;
    sCatalogCursorCallback(index, onInit, list);
}

static void CatalogOpenCategory(u8 category)
{
    for (u32 i = 0; i < category; i++)
        CatalogInput(DPAD_DOWN);
    CatalogInput(A_BUTTON);
}

static void InitCatalogUi(void)
{
    static const struct BgTemplate background = {0, 0, 24, 0, 0, 0, 0};
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    // Reset the queue as well as BG tracking between standalone UI fixtures.
    ClearDma3Requests();
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, &background, 1);
    SetBgTilemapBuffer(0, AllocZeroed(0x800));
    InitStandardTextBoxWindows();
    InitTextBoxGfxAndPrinters();
    gMain.newKeys = gMain.newAndRepeatedKeys = 0;
}

static void FreeCatalogUi(void)
{
    while (IsDma3ManagerBusyWithBgCopy())
        ProcessDma3Requests();
    FreeAllWindowBuffers();
    void *tilemap = GetBgTilemapBuffer(0);
    UnsetBgTilemapBuffer(0);
    Free(tilemap);
}

TEST("Frontier music: Johto follows Kanto in the catalog while Suicune remains in Kanto")
{
    static const u8 kanto[] = _("{JPN}カントー");
    static const u8 johto[] = _("{JPN}ジョウト");
    static const u8 hoenn[] = _("{JPN}ホウエン");
    ResetMusic();
    InitCatalogUi();
    FrontierMusic_OpenCatalog();
    CatalogInput(0);
    u8 task = CatalogListTask();
    EXPECT_NE(task, TASK_NONE);
    struct ListMenu *list = (void *)gTasks[task].data;
    EXPECT_EQ((u32)list->template.totalItems, 7);
    EXPECT_EQ(StringCompare(list->template.items[0].name, kanto), 0);
    EXPECT_EQ(StringCompare(list->template.items[1].name, johto), 0);
    EXPECT_EQ(StringCompare(list->template.items[2].name, hoenn), 0);
    CatalogOpenCategory(FRONTIER_MUSIC_CATEGORY_JOHTO);
    task = CatalogListTask();
    EXPECT_NE(task, TASK_NONE);
    list = (void *)gTasks[task].data;
    EXPECT_EQ((u32)list->template.totalItems, 5);
    EXPECT_EQ(list->template.items[0].id, 30);
    EXPECT_EQ(list->template.items[1].id, 31);
    EXPECT_EQ(list->template.items[2].id, 32);
    EXPECT_EQ(list->template.items[3].id, 33);
    CatalogInput(SELECT_BUTTON);
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_hgss_ho_oh);
    CatalogInput(DPAD_DOWN);
    CatalogInput(SELECT_BUTTON);
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_hgss_lugia);
    CatalogInput(DPAD_DOWN);
    CatalogInput(SELECT_BUTTON);
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_hgss_champion);
    CatalogInput(DPAD_DOWN);
    CatalogInput(SELECT_BUTTON);
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_vs_hgss_rocket);
    CatalogInput(B_BUTTON);
    CatalogInput(B_BUTTON);
    FreeCatalogUi();
}

TEST("Frontier music: SELECT previews and stops in the catalog and both A and B restore map music")
{
    u16 closeButton;
    struct FrontierMusicSave before;
    PARAMETRIZE { closeButton = A_BUTTON; }
    PARAMETRIZE { closeButton = B_BUTTON; }
    ResetMusic();
    ClearStoryProgress();
    before = gSaveBlock3Ptr->frontierMusic;
    InitCatalogUi();
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    gMain.newKeys = gMain.newAndRepeatedKeys = 0;
    FrontierMusic_OpenCatalog();
    CatalogInput(0);
    CatalogOpenCategory(FRONTIER_MUSIC_CATEGORY_HOENN);
    CatalogInput(SELECT_BUTTON);
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_VS_TRAINER].header);
    CatalogInput(SELECT_BUTTON);
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    CatalogInput(DPAD_DOWN);
    CatalogInput(SELECT_BUTTON);
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_VS_WILD].header);
    CatalogInput(closeButton);
    if (closeButton == B_BUTTON)
        CatalogInput(B_BUTTON);
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    EXPECT_EQ(GetCurrentMapMusic(), MUS_B_FRONTIER);
    EXPECT_EQ(gSpecialVar_Result, closeButton == A_BUTTON);
    if (closeButton == A_BUTTON)
        EXPECT_EQ(gSpecialVar_0x8005, MUS_VS_WILD);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
    for (u32 task = 0; task < NUM_TASKS; task++)
        EXPECT(!gTasks[task].isActive);
    FreeCatalogUi();
}

TEST("Frontier music: work pages preserve global song IDs for free preview and selection")
{
    u32 selected = 0;
    for (u32 i = 0; i < FRONTIER_MUSIC_TRACK_COUNT; i++)
        PARAMETRIZE { selected = i; }
    ResetMusic();
    ClearStoryProgress();
    FlagSet(FLAG_STORY_CURE_FOUND);
    FlagSet(FLAG_STORY_POKEVIRUS_FADED);
    struct FrontierMusicSave before = gSaveBlock3Ptr->frontierMusic;
    InitCatalogUi();
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    FrontierMusic_OpenCatalog();
    CatalogInput(0);
    u16 song = FrontierMusic_GetSong(selected);
    CatalogOpenCategory(FrontierMusic_GetCategory(song));
    u8 task = CatalogListTask();
    EXPECT_NE(task, TASK_NONE);
    struct ListMenu *list = (void *)gTasks[task].data;
    u32 position = 0;
    for (; position < list->template.totalItems; position++)
        if (list->template.items[position].id == selected)
            break;
    EXPECT_LT(position, list->template.totalItems);
    for (u32 row = 0; row < list->template.totalItems - 1; row++)
        EXPECT_EQ(FrontierMusic_GetCategory(FrontierMusic_GetSong(list->template.items[row].id)),
                  FrontierMusic_GetCategory(song));
    for (u32 row = 0; row < position; row++)
        CatalogInput(DPAD_DOWN);
    CatalogInput(SELECT_BUTTON);
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[song].header);
    CatalogInput(A_BUTTON);
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT_EQ(gSpecialVar_0x8005, song);
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    FreeCatalogUi();
}

TEST("Frontier music: empty and story-unlocked original pages return safely without task leaks")
{
    u8 available;
    PARAMETRIZE { available = 0; }
    PARAMETRIZE { available = 1; }
    PARAMETRIZE { available = 2; }
    ResetMusic();
    ClearStoryProgress();
    if (available >= 1)
        FlagSet(FLAG_STORY_CURE_FOUND);
    if (available >= 2)
        FlagSet(FLAG_STORY_POKEVIRUS_FADED);
    InitCatalogUi();
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    FrontierMusic_OpenCatalog();
    CatalogInput(0);
    for (u32 repeat = 0; repeat < 4; repeat++)
    {
        if (repeat == 0)
            CatalogOpenCategory(FRONTIER_MUSIC_CATEGORY_ORIGINAL);
        else
            CatalogInput(A_BUTTON);
        u8 task = CatalogListTask();
        EXPECT_NE(task, TASK_NONE);
        struct ListMenu *list = (void *)gTasks[task].data;
        EXPECT_EQ((u32)list->template.totalItems, available + 1);
        EXPECT_EQ((u32)list->template.maxShowed, available + 1);
        CatalogInput(SELECT_BUTTON);
        if (available == 0)
            EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
        else
            EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_POKEVIRUS_RELEASE].header);
        CatalogInput(B_BUTTON);
        EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
        task = CatalogListTask();
        EXPECT_NE(task, TASK_NONE);
        list = (void *)gTasks[task].data;
        EXPECT_EQ((u32)list->template.totalItems, FRONTIER_MUSIC_CATEGORY_COUNT + 1);
        EXPECT_EQ(list->scrollOffset + list->selectedRow, FRONTIER_MUSIC_CATEGORY_ORIGINAL);
    }
    CatalogInput(B_BUTTON);
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    for (u32 task = 0; task < NUM_TASKS; task++)
        EXPECT(!gTasks[task].isActive);
    FreeCatalogUi();
}

TEST("Frontier music: scrolling updates the list once without initializing or repainting it again")
{
    ResetMusic();
    ClearStoryProgress();
    InitCatalogUi();
    FrontierMusic_OpenCatalog();
    CatalogInput(0);
    CatalogOpenCategory(FRONTIER_MUSIC_CATEGORY_HOENN);
    u8 task = CatalogListTask();
    EXPECT_NE(task, TASK_NONE);
    struct ListMenu *list = (void *)gTasks[task].data;
    sCatalogCursorCallback = list->template.moveCursorFunc;
    sCatalogInitCallbacks = sCatalogMoveCallbacks = 0;
    list->template.moveCursorFunc = CountCatalogCursorCallbacks;
    for (u32 i = 0; i < FRONTIER_MUSIC_TRACK_COUNT; i++)
        CatalogInput(DPAD_DOWN);
    for (u32 i = 0; i < FRONTIER_MUSIC_TRACK_COUNT; i++)
        CatalogInput(DPAD_UP);
    EXPECT_GT(sCatalogMoveCallbacks, 0);
    EXPECT_EQ(sCatalogInitCallbacks, 0);
    list->template.moveCursorFunc = sCatalogCursorCallback;
    CatalogInput(B_BUTTON);
    CatalogInput(B_BUTTON);
    FreeCatalogUi();
}

TEST("Frontier music: scrolling catalog pixels match a clean redraw without leaving footer text")
{
    bool32 preview = FALSE;
    u16 down = DPAD_DOWN, up = DPAD_UP;
    u8 category = FRONTIER_MUSIC_CATEGORY_KANTO;
    struct FrontierMusicSave before;
    for (u32 i = 0; i <= FRONTIER_MUSIC_CATEGORY_COUNT; i++)
    {
        PARAMETRIZE { category = i; preview = FALSE; down = DPAD_DOWN; up = DPAD_UP; }
        PARAMETRIZE { category = i; preview = TRUE; down = DPAD_DOWN; up = DPAD_UP; }
        PARAMETRIZE { category = i; preview = FALSE; down = DPAD_RIGHT; up = DPAD_LEFT; }
        PARAMETRIZE { category = i; preview = TRUE; down = DPAD_RIGHT; up = DPAD_LEFT; }
    }
    ResetMusic();
    ClearStoryProgress();
    FlagSet(FLAG_STORY_POKEVIRUS_FADED);
    FlagSet(FLAG_STORY_CURE_FOUND);
    FrontierMusic_Unlock(MUS_RG_VS_DEOXYS);
    FrontierMusic_SetMode(FRONTIER_MUSIC_FIXED, MUS_RG_VS_DEOXYS);
    before = gSaveBlock3Ptr->frontierMusic;
    InitCatalogUi();
    PlayNewMapMusic(MUS_B_FRONTIER);
    MapMusicMain();
    MapMusicMain();
    FrontierMusic_OpenCatalog();
    CatalogInput(0);
    if (category < FRONTIER_MUSIC_CATEGORY_COUNT)
        CatalogOpenCategory(category);
    u8 listTask = CatalogListTask();
    EXPECT_NE(listTask, TASK_NONE);
    struct ListMenu *list = (void *)gTasks[listTask].data;
    u8 window = list->template.windowId;
    u8 header = CatalogFixedWindow(window, GetWindowAttribute(window, WINDOW_TILEMAP_TOP) - 2);
    u8 footer = CatalogFixedWindow(window, GetWindowAttribute(window, WINDOW_TILEMAP_TOP)
                                           + GetWindowAttribute(window, WINDOW_HEIGHT));
    EXPECT_NE(header, WINDOW_NONE);
    EXPECT_NE(footer, WINDOW_NONE);
    EXPECT_NE(header, window);
    EXPECT_NE(footer, window);
    EXPECT_EQ(GetWindowAttribute(window, WINDOW_HEIGHT), 8);
    EXPECT_EQ((u32)list->template.upText_Y, 0);
    EXPECT_LE(GetWindowAttribute(header, WINDOW_BASE_BLOCK) + 26 * 2, GetWindowAttribute(window, WINDOW_BASE_BLOCK));
    EXPECT_LE(GetWindowAttribute(window, WINDOW_BASE_BLOCK) + 26 * 8, GetWindowAttribute(footer, WINDOW_BASE_BLOCK));
    const u32 fixedSize = 26 * 2 * 32;
    u8 *headerPixels = Alloc(fixedSize);
    EXPECT(headerPixels != NULL);
    memcpy(headerPixels, (const void *)GetWindowAttribute(header, WINDOW_TILE_DATA), fixedSize);
    u32 size = GetWindowAttribute(window, WINDOW_WIDTH) * GetWindowAttribute(window, WINDOW_HEIGHT) * 32;
    u8 *actual = Alloc(size);
    EXPECT(actual != NULL);
    if (preview)
        CatalogInput(SELECT_BUTTON);
    for (u32 direction = 0; direction < 2; direction++)
    {
        for (u32 step = 0; step < FRONTIER_MUSIC_TRACK_COUNT + 2; step++)
        {
            CatalogInput(direction == 0 ? down : up);
            EXPECT_EQ(memcmp(headerPixels, (const void *)GetWindowAttribute(header, WINDOW_TILE_DATA), fixedSize), 0);
            const u8 *headerVram = (const u8 *)BG_CHAR_ADDR(0) + GetWindowAttribute(header, WINDOW_BASE_BLOCK) * 32;
            const u8 *footerVram = (const u8 *)BG_CHAR_ADDR(0) + GetWindowAttribute(footer, WINDOW_BASE_BLOCK) * 32;
            EXPECT_EQ(memcmp(headerPixels, headerVram, fixedSize), 0);
            EXPECT_EQ(memcmp((const void *)GetWindowAttribute(footer, WINDOW_TILE_DATA), footerVram, fixedSize), 0);
            memcpy(actual, (const void *)GetWindowAttribute(window, WINDOW_TILE_DATA), size);
            const u8 *vram = (const u8 *)BG_CHAR_ADDR(0) + GetWindowAttribute(window, WINDOW_BASE_BLOCK) * 32;
            if (memcmp(actual, vram, size) != 0)
            {
                for (u32 offset = 0; offset < size; offset++)
                    if (actual[offset] != vram[offset])
                    {
                        Test_MgbaPrintf("Catalog preview=%u keys=%u direction=%u step=%u scroll=%u row=%u offset=%u RAM=%u VRAM=%u",
                                        preview, down, direction, step, list->scrollOffset, list->selectedRow,
                                        offset, actual[offset], vram[offset]);
                        break;
                    }
            }
            EXPECT_EQ(memcmp(actual, vram, size), 0);
            RedrawListMenu(listTask);
            CatalogInput(0);
            EXPECT_EQ(memcmp(actual, (const void *)GetWindowAttribute(window, WINDOW_TILE_DATA), size), 0);
        }
    }
    EXPECT_EQ(list->scrollOffset, 0);
    EXPECT_EQ(list->selectedRow, 0);
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->frontierMusic, sizeof(before)), 0);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 95);
    CatalogInput(B_BUTTON);
    if (category < FRONTIER_MUSIC_CATEGORY_COUNT)
        CatalogInput(B_BUTTON);
    EXPECT(gMPlayInfo_BGM.songHeader == gSongTable[MUS_B_FRONTIER].header);
    Free(actual);
    Free(headerPixels);
    FreeCatalogUi();
}
