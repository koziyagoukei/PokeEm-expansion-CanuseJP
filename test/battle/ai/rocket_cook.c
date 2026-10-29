#include "global.h"
#include "test/battle.h"
#include "battle_partner.h"
#include "data.h"
#include "frontier_story.h"
#include "trainer_util.h"
#include "constants/trainers.h"

// The battle harness replaces gBattlePartners. Check the real generated profile.
static const struct Trainer sCookTestPartners[DIFFICULTY_COUNT][PARTNER_COUNT] =
{
#include "../../../src/data/battle_partners.h"
};

#define COOK (sCookTestPartners[DIFFICULTY_NORMAL][PARTNER_ROCKET_COOK])

TEST("Rocket cook: survival AI is confined to the cook's partner profile")
{
    u64 required = AI_FLAG_BASIC_TRAINER | AI_FLAG_TRY_TO_2HKO | AI_FLAG_HP_AWARE
        | AI_FLAG_SMART_SWITCHING | AI_FLAG_SMART_MON_CHOICES
        | AI_FLAG_PP_STALL_PREVENTION | AI_FLAG_SMART_TERA;
    EXPECT_EQ(COOK.aiFlags & required, required);
    EXPECT_EQ(COOK.aiFlags & (AI_FLAG_RISKY | AI_FLAG_WILL_SUICIDE | AI_FLAG_FORCE_SETUP_FIRST_TURN | AI_FLAG_OMNISCIENT), 0);
    EXPECT_EQ(sCookTestPartners[DIFFICULTY_NORMAL][PARTNER_STEVEN].aiFlags, AI_FLAG_BASIC_TRAINER);
}

TEST("Rocket cook: party keeps its three species and has legal defensive options")
{
    static const enum Species species[] = {SPECIES_ZYGARDE_50_POWER_CONSTRUCT, SPECIES_DARKRAI, SPECIES_TOGEKISS};
    struct TrainerGenerator generator = {0};
    struct Pokemon mon;
    FrontierStory_EndStoryBattle();
    MakePartnerGenerator(&generator, &COOK);
    EXPECT_EQ((u32)COOK.partySize, ARRAY_COUNT(species));
    for (u32 i = 0; i < ARRAY_COUNT(species); i++)
    {
        bool32 hasProtect = FALSE;
        GenerateMonFromTrainerMon(&mon, &COOK.party[i], &generator);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), species[i]);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), 50);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_IVS), TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31));
        for (u32 j = 0; j < MAX_MON_MOVES; j++)
        {
            if (GetMonData(&mon, MON_DATA_MOVE1 + j) == MOVE_PROTECT)
                hasProtect = TRUE;
        }
        EXPECT(hasProtect);
        if (species[i] == SPECIES_DARKRAI)
            EXPECT_EQ(GetMonData(&mon, MON_DATA_HELD_ITEM), ITEM_FOCUS_SASH);
    }
}

AI_MULTI_BATTLE_TEST("Rocket cook: Zygarde finishes a weakened faster foe instead of setting up")
{
    GIVEN {
        AI_FLAGS(0);
        BATTLER_AI_FLAGS(playerRight, COOK.aiFlags);
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        PARTNER(SPECIES_ZYGARDE_50_POWER_CONSTRUCT) { Speed(1); Moves(MOVE_THOUSAND_ARROWS, MOVE_DRAGON_DANCE, MOVE_EXTREME_SPEED, MOVE_PROTECT); }
        OPPONENT_A(SPECIES_WEAVILE) { Speed(3); HP(1); Moves(MOVE_ICE_PUNCH); }
        OPPONENT_B(SPECIES_WOBBUFFET) { Speed(4); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { EXPECT_MOVE(playerRight, MOVE_EXTREME_SPEED, target: opponentLeft); }
    }
}

AI_MULTI_BATTLE_TEST("Rocket cook: Darkrai takes a knockout rather than wasting a turn")
{
    GIVEN {
        AI_FLAGS(0);
        BATTLER_AI_FLAGS(playerRight, COOK.aiFlags);
        PLAYER(SPECIES_WOBBUFFET);
        PARTNER(SPECIES_DARKRAI) { Item(ITEM_FOCUS_SASH); Moves(MOVE_DARK_VOID, MOVE_DARK_PULSE, MOVE_SLUDGE_BOMB, MOVE_PROTECT); }
        OPPONENT_A(SPECIES_ALAKAZAM) { HP(1); Moves(MOVE_CELEBRATE); }
        OPPONENT_B(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { EXPECT_MOVES(playerRight, MOVE_DARK_PULSE, MOVE_SLUDGE_BOMB); }
    } THEN {
        EXPECT_EQ(opponentLeft->hp, 0);
    }
}

MULTI_BATTLE_TEST("Rocket cook: Darkrai's sash prevents an immediate one-hit knockout")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PARTNER(SPECIES_DARKRAI) { Speed(2); Item(COOK.party[1].heldItem); }
        OPPONENT_A(SPECIES_HERACROSS) { Speed(3); Attack(999); }
        OPPONENT_B(SPECIES_WOBBUFFET) { Speed(4); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_DARK_PULSE, target: opponentLeft);
            MOVE(opponentLeft, MOVE_CLOSE_COMBAT, target: playerRight);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } THEN {
        EXPECT_EQ(playerRight->hp, 1);
        EXPECT_EQ(playerRight->item, ITEM_NONE);
    }
}
