#include "global.h"
#include "main_menu.h"
#include "quickstart.h"
#include "string_util.h"
#include "test/test.h"
#include "constants/characters.h"

TEST("Frontier startup: default names match the selected gender")
{
    u32 gender = MALE;
    const u8 *expected = COMPOUND_STRING("{JPN}ルビー");
    PARAMETRIZE { gender = MALE; expected = COMPOUND_STRING("{JPN}ルビー"); }
    PARAMETRIZE { gender = FEMALE; expected = COMPOUND_STRING("{JPN}サファイア"); }

    gSaveBlock2Ptr->playerGender = gender;
    memset(gSaveBlock2Ptr->playerName, 0, sizeof(gSaveBlock2Ptr->playerName));
    NewGameBirchSpeech_SetDefaultPlayerName(0);
    EXPECT_LE(StringLength(expected), PLAYER_NAME_LENGTH);
    EXPECT_EQ(StringCompare(gSaveBlock2Ptr->playerName, expected), 0);
    EXPECT_EQ(gSaveBlock2Ptr->playerName[PLAYER_NAME_LENGTH], EOS);
}

TEST("Frontier startup: other preset names remain terminated and quickstart is disabled")
{
    gSaveBlock2Ptr->playerGender = MALE;
    NewGameBirchSpeech_SetDefaultPlayerName(10);
    EXPECT_EQ(StringCompare(gSaveBlock2Ptr->playerName, COMPOUND_STRING("{JPN}トム")), 0);
    EXPECT_EQ(gSaveBlock2Ptr->playerName[PLAYER_NAME_LENGTH], EOS);
    EXPECT_EQ(QUICKSTART, FALSE);
}
