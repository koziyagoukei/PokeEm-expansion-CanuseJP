#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "frontier_music.h"
#include "m4a.h"
#include "test/test.h"
#include "constants/opponents.h"
#include "constants/songs.h"

extern const struct SongHeader mus_pokevirus_final;

TEST("Final music: appended id preserves the release battle and title songs")
{
    EXPECT_EQ(MUS_POKEVIRUS_RELEASE, 610);
    EXPECT_EQ(MUS_FRONTIER_OPENING, 611);
    EXPECT_EQ(MUS_FRONTIER_TITLE, 612);
    EXPECT_EQ(MUS_POKEVIRUS_FINAL, 613);
    EXPECT(gSongTable[MUS_POKEVIRUS_FINAL].header == &mus_pokevirus_final);
    EXPECT_EQ(gSongTable[MUS_POKEVIRUS_FINAL].ms, 0);
    EXPECT_EQ(mus_pokevirus_final.trackCount, 8);
}

TEST("Final music: wordless choir and three PSG voices fit the existing mixer")
{
    const struct ToneData *voices = mus_pokevirus_final.tone;
    EXPECT_EQ(voices[0].type & TONEDATA_TYPE_CGB, 0);
    EXPECT(voices[0].wav != NULL);
    EXPECT_EQ(voices[5].type & 7, 3);
    EXPECT_EQ(voices[6].type & 7, 2);
    EXPECT_EQ(voices[7].type & 7, 4);
}

TEST("Final music: explicit final score is not replaced by facility preferences")
{
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    for (u32 mode = FRONTIER_MUSIC_DEFAULT; mode <= FRONTIER_MUSIC_RANDOM; mode++)
    {
        gSaveBlock3Ptr->frontierMusic.mode = mode;
        gSaveBlock3Ptr->frontierMusic.selectedSong = MUS_VS_TRAINER;
        EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_POKEVIRUS_FINAL), MUS_POKEVIRUS_FINAL);
        EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_POKEVIRUS_RELEASE), MUS_POKEVIRUS_RELEASE);
    }
}
