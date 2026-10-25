#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "event_data.h"
#include "frontier_music.h"
#include "m4a.h"
#include "overworld.h"
#include "pokemon.h"
#include "sound.h"
#include "task.h"
#include "trainer_see.h"
#include "test/test.h"
#include "constants/event_objects.h"
#include "constants/maps.h"
#include "constants/map_groups.h"
#include "constants/opponents.h"
#include "constants/songs.h"

extern const struct SongHeader mus_pokevirus_release;

TEST("Release music: appended song id keeps old music and phonemes unchanged")
{
    EXPECT_EQ(MUS_VS_TRAINER, 476);
    EXPECT_EQ(PH_NURSE_SOLO, 609);
    EXPECT_EQ(MUS_POKEVIRUS_RELEASE, 610);
    const struct SongHeader *song = gSongTable[MUS_POKEVIRUS_RELEASE].header;
    EXPECT_EQ(song, &mus_pokevirus_release);
    EXPECT_EQ(gSongTable[MUS_POKEVIRUS_RELEASE].ms, 0);
    EXPECT_EQ(mus_pokevirus_release.trackCount, 8);
    const u8 *const *tracks = (const u8 *const *)((const u8 *)song + offsetof(struct SongHeader, part));
    for (u32 i = 0; i < 8; i++) EXPECT(tracks[i] != NULL);
}

TEST("Release music: heartbeat uses a small original non-looping PCM sample")
{
    const struct ToneData *tone = &mus_pokevirus_release.tone[0];
    const struct WaveData *wave = tone->wav;
    EXPECT_EQ(tone->type, TONEDATA_TYPE_FIX);
    EXPECT(wave != NULL);
    EXPECT_GT(wave->size, 2000);
    EXPECT_LT(wave->size, 3000);
    EXPECT_EQ(wave->loopStart, 0);
    EXPECT_EQ(wave->status & 0xC000, 0);
}

TEST("Release music: facility fixed and random preferences cannot replace the story score")
{
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    for (u32 mode = FRONTIER_MUSIC_DEFAULT; mode <= FRONTIER_MUSIC_RANDOM; mode++)
    {
        gSaveBlock3Ptr->frontierMusic.mode = mode;
        gSaveBlock3Ptr->frontierMusic.selectedSong = MUS_VS_TRAINER;
        EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_POKEVIRUS_RELEASE), MUS_POKEVIRUS_RELEASE);
    }
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_TRAINER);
}

TEST("Release music: only the dedicated trainer command selects the release score")
{
    bool32 release = FALSE;
    PARAMETRIZE { release = FALSE; }
    PARAMETRIZE { release = TRUE; }
    TrainerBattleParameter args = {0};
    struct ScriptContext ctx;
    ResetTasks();
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_ULTRA_FRONTIE_HUB), MAP_NUM(MAP_ULTRA_FRONTIE_HUB));
    gNoOfApproachingTrainers = 0;
    args.params.opponentA = TRAINER_SAWYER_1;
    args.params.objEventLocalIdA = LOCALID_NONE;
    args.params.skipFlagCheck = TRUE;
    InitScriptContext(&ctx, NULL, NULL);
    ctx.scriptPtr = args.data;
    if (release) Script_TrainerBattlePokevirus(&ctx);
    else ConfigureTrainerBattle(&ctx);
    BattleSetup_StartTrainerBattle();
    if (release) EXPECT_EQ(GetCurrentMapMusic(), MUS_POKEVIRUS_RELEASE);
    else EXPECT_NE(GetCurrentMapMusic(), MUS_POKEVIRUS_RELEASE);
    ResetTasks();
    StopMapMusic();
    InitTrainerBattleParameter();
}
