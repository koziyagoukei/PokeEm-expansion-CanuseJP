#include "global.h"
#include "battle.h"
#include "battle_ai_main.h"
#include "battle_setup.h"
#include "event_data.h"
#include "test/battle.h"
#include "constants/battle_frontier.h"

#define NORMAL_AI_FLAGS (AI_FLAG_BASIC_TRAINER | AI_FLAG_SMART_DYNAMAX)

TEST("Frontier Dynamax timing: only the normal offline opponent receives the timing flag")
{
    static const struct { u32 flags; bool32 enabled; } cases[] =
    {
        { BATTLE_TYPE_BATTLE_TOWER, TRUE },
        { BATTLE_TYPE_FACTORY, TRUE },
        { BATTLE_TYPE_PALACE, FALSE },
        { BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_LINK, FALSE },
        { 0, FALSE },
    };

    TRAINER_BATTLE_PARAM.opponentA = 1;
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES);
    gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_50;
    gSaveBlock2Ptr->frontier.factoryWinStreaks[FRONTIER_MODE_SINGLES][FRONTIER_LVL_50] = 0;
    for (u32 i = 0; i < ARRAY_COUNT(cases); i++)
    {
        gBattleTypeFlags = BATTLE_TYPE_TRAINER | cases[i].flags;
        VarSet(VAR_FRONTIER_AI_LEVEL, FRONTIER_AI_LEVEL_NORMAL);
        u64 flags = Test_GetAiFlags(1, B_BATTLER_1);
        EXPECT_EQ(!!(flags & AI_FLAG_SMART_DYNAMAX), cases[i].enabled);
        EXPECT(!(Test_GetAiFlags(1, B_BATTLER_2) & AI_FLAG_SMART_DYNAMAX));
        // The Battle Factory's AI is the other facilities' (the difficulty setting), from its first round
        if (cases[i].flags == BATTLE_TYPE_FACTORY)
        {
            gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
            EXPECT(flags == Test_GetAiFlags(1, B_BATTLER_1));
            gBattleTypeFlags = BATTLE_TYPE_TRAINER | cases[i].flags;
        }
        VarSet(VAR_FRONTIER_AI_LEVEL, FRONTIER_AI_LEVEL_EASY);
        EXPECT(!(Test_GetAiFlags(1, B_BATTLER_1) & AI_FLAG_SMART_DYNAMAX));
    }
    VarSet(VAR_FRONTIER_AI_LEVEL, FRONTIER_AI_LEVEL_NORMAL);
    gBattleTypeFlags = 0;
    TRAINER_BATTLE_PARAM.opponentA = 0;
}

AI_SINGLE_BATTLE_TEST("Frontier Dynamax timing: normal AI saves Dynamax while setting up")
{
    GIVEN {
        AI_FLAGS(NORMAL_AI_FLAGS);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SWORDS_DANCE); DynamaxLevel(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_SWORDS_DANCE, gimmick: GIMMICK_NONE); }
    }
}

AI_SINGLE_BATTLE_TEST("Frontier Dynamax timing: normal AI saves Dynamax when an attack is enough or Max Move has no effect")
{
    enum Species species = SPECIES_WOBBUFFET;
    enum Gimmick gimmick = GIMMICK_DYNAMAX;
    u32 hp = 400;
    PARAMETRIZE { species = SPECIES_WOBBUFFET; hp = 400; gimmick = GIMMICK_DYNAMAX; }
    PARAMETRIZE { species = SPECIES_WOBBUFFET; hp = 1; gimmick = GIMMICK_NONE; }
    PARAMETRIZE { species = SPECIES_GENGAR; hp = 400; gimmick = GIMMICK_NONE; }

    GIVEN {
        AI_FLAGS(NORMAL_AI_FLAGS);
        PLAYER(species) { MaxHP(400); HP(hp); Speed(10); Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(100); Moves(MOVE_TACKLE); DynamaxLevel(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_TACKLE, gimmick: gimmick); }
    }
}

AI_SINGLE_BATTLE_TEST("Frontier Dynamax timing: normal AI can Max Guard on its threatened last Pokemon")
{
    GIVEN {
        AI_FLAGS(NORMAL_AI_FLAGS);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_DRAGON_RAGE); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(60); Moves(MOVE_SWORDS_DANCE); DynamaxLevel(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_RAGE); EXPECT_MOVE(opponent, MOVE_SWORDS_DANCE, gimmick: GIMMICK_NONE); }
        TURN { MOVE(player, MOVE_DRAGON_RAGE); EXPECT_MOVE(opponent, MOVE_SWORDS_DANCE, gimmick: GIMMICK_DYNAMAX); }
    }
}
