#include "global.h"
#include "event_data.h"
#include "frontier_story.h"
#include "party_bath_expansion.h"
#include "random.h"
#include "script.h"
#include "string_util.h"
#include "text.h"
#include "test/test.h"
#include "constants/frontier_guest.h"
#include "constants/maps.h"
#include "constants/characters.h"

static const u16 sTrainers[] = {TRAINER_FRONTIER_STEVEN, TRAINER_FRONTIER_WALLACE, TRAINER_FRONTIER_MAXIE, TRAINER_FRONTIER_ARCHIE};

static void SetRescued(u32 trainer, u32 state)
{
    u32 flag = FLAG_STORY_NAMED_STATES_START + 2 * (trainer - TRAINER_FRONTIER_GUEST_FIRST);
    if (state & 1) FlagSet(flag); else FlagClear(flag);
    if (state & 2) FlagSet(flag + 1); else FlagClear(flag + 1);
}

static void Prepare(void)
{
    VarSet(VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_COMPLETE);
    FlagSet(FLAG_SYS_POKENAV_GET);
    for (u32 i = 0; i < ARRAY_COUNT(sTrainers); i++)
    {
        SetRescued(sTrainers[i], NAMED_STORY_INFECTED);
        FlagClear(FLAG_BATH_ROCK_BUILT + i);
    }
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_PARTYBATHTIME);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_PARTYBATHTIME);
    EXPECT(!PartyBath_TryStartExpansionCall());
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_ULTRA_FRONTIE_HUB);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_ULTRA_FRONTIE_HUB);
}

TEST("Bath expansion: only a fully rescued guest can call, once per expansion")
{
    Prepare();
    EXPECT_EQ(PartyBath_GetPendingExpansionCall(), PARTY_BATH_EXPANSION_COUNT);
    for (u32 i = 0; i < ARRAY_COUNT(sTrainers); i++)
    {
        SetRescued(sTrainers[i], NAMED_STORY_RECOVERED);
        EXPECT_EQ(PartyBath_GetPendingExpansionCall(), PARTY_BATH_EXPANSION_COUNT);
        gSpecialVar_0x8004 = i;
        PartyBath_CompleteCall();
        EXPECT_EQ(gSpecialVar_Result, FALSE);
        SetRescued(sTrainers[i], NAMED_STORY_RESCUED);
        EXPECT_EQ(PartyBath_GetPendingExpansionCall(), i);
        PartyBath_CompleteCall();
        EXPECT_EQ(gSpecialVar_Result, TRUE);
        EXPECT_EQ(PartyBath_GetGain(), i + 2);
        PartyBath_CompleteCall();
        EXPECT_EQ(gSpecialVar_Result, FALSE);
        EXPECT_EQ(PartyBath_GetGain(), i + 2);
    }
    EXPECT_EQ(PartyBath_GetPendingExpansionCall(), PARTY_BATH_EXPANSION_COUNT);
    gSpecialVar_0x8004 = 0xFFFF;
    PartyBath_CompleteCall();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
}

TEST("Bath expansion: walking checks probability, never forces a call on the tenth step")
{
    Prepare();
    SetRescued(TRAINER_FRONTIER_STEVEN, NAMED_STORY_RESCUED);
    SET_RNG(RNG_NONE, PARTY_BATH_CALL_CHANCE - 1);
    for (u32 step = 0; step < PARTY_BATH_CALL_STEPS; step++)
        EXPECT(!PartyBath_TryStartExpansionCall());
    SET_RNG(RNG_NONE, 0);
    for (u32 step = 1; step < PARTY_BATH_CALL_STEPS; step++)
        EXPECT(!PartyBath_TryStartExpansionCall());
    EXPECT(PartyBath_TryStartExpansionCall());
    ScriptContext_Stop();
}

TEST("Bath expansion: old stories and other maps cannot receive expansion calls")
{
    Prepare();
    SetRescued(TRAINER_FRONTIER_STEVEN, NAMED_STORY_RESCUED);
    VarSet(VAR_FRONTIER_INTRO_STATE, 0);
    EXPECT_EQ(PartyBath_GetPendingExpansionCall(), PARTY_BATH_EXPANSION_COUNT);
    VarSet(VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_COMPLETE);
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_PARTY_BATH_DEEP);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_PARTY_BATH_DEEP);
    for (u32 step = 0; step < 30; step++)
        EXPECT(!PartyBath_TryStartExpansionCall());
    EXPECT(!FrontierStory_IsGuestRescued(TRAINER_CALVIN_1));
}

extern const u8 PartyBathExpansion_Text_CallSteven[], PartyBathExpansion_Text_CallWallace[];
extern const u8 PartyBathExpansion_Text_CallMaxie[], PartyBathExpansion_Text_CallArchie[];
extern const u8 PartyBathExpansion_Text_Completed[], PartyBathExpansion_Text_Guide[], PartyBathExpansion_Text_Locked[];

static void CheckMessageWidth(const u8 *text, u32 limit)
{
    u8 normalized[1000];
    EXPECT_LT(StringLength(text), sizeof(normalized));
    StringCopy(normalized, text);
    for (u8 *cursor = normalized; *cursor != EOS;)
    {
        if (*cursor == EXT_CTRL_CODE_BEGIN)
            cursor += 1 + GetExtCtrlCodeLength(cursor[1]);
        else
        {
            if (*cursor == CHAR_PROMPT_CLEAR || *cursor == CHAR_PROMPT_SCROLL)
                *cursor = CHAR_NEWLINE;
            cursor++;
        }
    }
    EXPECT_LE(GetStringWidth(FONT_NORMAL, normalized, 0), limit);
}

TEST("Bath expansion: call texts fit the Pokénav window and guide text fits the field window")
{
    const u8 *calls[] = {PartyBathExpansion_Text_CallSteven, PartyBathExpansion_Text_CallWallace,
                        PartyBathExpansion_Text_CallMaxie, PartyBathExpansion_Text_CallArchie};
    for (u32 i = 0; i < ARRAY_COUNT(calls); i++)
        CheckMessageWidth(calls[i], 192);
    CheckMessageWidth(PartyBathExpansion_Text_Guide, 208);
    CheckMessageWidth(PartyBathExpansion_Text_Locked, 208);
    StringCopy(gStringVar1, COMPOUND_STRING("{JPN}深いお風呂"));
    StringCopy(gStringVar2, COMPOUND_STRING("5"));
    StringExpandPlaceholders(gStringVar4, PartyBathExpansion_Text_Completed);
    CheckMessageWidth(gStringVar4, 208);
}
