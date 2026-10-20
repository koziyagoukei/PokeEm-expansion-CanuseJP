#include "global.h"
#include "test/battle.h"
#include "battle_gimmick.h"
#include "battle_gimmick_extra.h"
#include "battle_terastal.h"
#include "battle_util.h"

#define USE_ARENA_RULES() (gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_ARENA)

SINGLE_BATTLE_TEST("Arena gimmicks: Mega and Tera are unavailable to both sides only in Arena battles")
{
    enum Gimmick gimmick = GIMMICK_MEGA;
    bool32 arena = TRUE, rayquaza = FALSE;
    for (u32 a = 0; a < 2; a++)
    {
        PARAMETRIZE { arena = a; gimmick = GIMMICK_MEGA; rayquaza = FALSE; }
        PARAMETRIZE { arena = a; gimmick = GIMMICK_MEGA; rayquaza = TRUE; }
        PARAMETRIZE { arena = a; gimmick = GIMMICK_TERA; rayquaza = FALSE; }
    }
    GIVEN {
        if (arena)
            USE_ARENA_RULES();
        PLAYER(rayquaza ? SPECIES_RAYQUAZA : SPECIES_VENUSAUR) {
            if (gimmick == GIMMICK_TERA)
                TeraType(TYPE_WATER);
            else if (!rayquaza)
                Item(ITEM_VENUSAURITE);
            Moves(MOVE_CELEBRATE, MOVE_DRAGON_ASCENT);
        }
        OPPONENT(rayquaza ? SPECIES_RAYQUAZA : SPECIES_VENUSAUR) {
            if (gimmick == GIMMICK_TERA)
                TeraType(TYPE_WATER);
            else if (!rayquaza)
                Item(ITEM_VENUSAURITE);
            Moves(MOVE_CELEBRATE, MOVE_DRAGON_ASCENT);
        }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        AssignUsableGimmicks();
        for (u32 battler = 0; battler < gBattlersCount; battler++)
        {
            EXPECT_EQ(CanActivateGimmick(battler, gimmick), !arena);
            EXPECT_EQ((GetUsableGimmickMask(battler) & (1u << gimmick)) != 0, !arena);
            if (gimmick == GIMMICK_MEGA)
                EXPECT_EQ(CanMegaEvolve(battler), !arena);
            else
                EXPECT_EQ(CanTerastallize(battler), !arena);
            if (arena)
            {
                EXPECT_EQ(gBattleStruct->gimmick.usableGimmick[battler], GIMMICK_NONE);
                gBattleStruct->gimmick.usableGimmick[battler] = gimmick;
                gBattleStruct->gimmick.playerSelect[battler] = TRUE;
                EXPECT(!CanUseSelectedGimmickWithMove(battler, MOVE_CELEBRATE));
            }
        }
    }
}

SINGLE_BATTLE_TEST("Arena gimmicks: both sides can activate Z-Moves")
{
    GIVEN {
        USE_ARENA_RULES();
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); Item(ITEM_NORMALIUM_Z); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); Item(ITEM_NORMALIUM_Z); }
    } WHEN {
        TURN {
            MOVE(player, MOVE_POUND, gimmick: GIMMICK_Z_MOVE);
            MOVE(opponent, MOVE_POUND, gimmick: GIMMICK_Z_MOVE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BREAKNECK_BLITZ, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BREAKNECK_BLITZ, opponent);
    } THEN {
        EXPECT(HasTrainerUsedGimmick(B_BATTLER_0, GIMMICK_Z_MOVE));
        EXPECT(HasTrainerUsedGimmick(B_BATTLER_1, GIMMICK_Z_MOVE));
    }
}

SINGLE_BATTLE_TEST("Arena gimmicks: both sides can activate Dynamax")
{
    GIVEN {
        USE_ARENA_RULES();
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); DynamaxLevel(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); DynamaxLevel(10); }
    } WHEN {
        TURN {
            MOVE(player, MOVE_POUND, gimmick: GIMMICK_DYNAMAX);
            MOVE(opponent, MOVE_POUND, gimmick: GIMMICK_DYNAMAX);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_STRIKE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_STRIKE, opponent);
    } THEN {
        EXPECT_EQ(GetActiveGimmick(B_BATTLER_0), GIMMICK_DYNAMAX);
        EXPECT_EQ(GetActiveGimmick(B_BATTLER_1), GIMMICK_DYNAMAX);
    }
}

SINGLE_BATTLE_TEST("Arena gimmicks: Dusk Mane and Dawn Wings can still Ultra Burst on both sides")
{
    GIVEN {
        USE_ARENA_RULES();
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        PLAYER(SPECIES_NECROZMA_DUSK_MANE) { Speed(2); Item(ITEM_ULTRANECROZIUM_Z); }
        OPPONENT(SPECIES_NECROZMA_DAWN_WINGS) { Speed(1); Item(ITEM_ULTRANECROZIUM_Z); }
    } WHEN {
        TURN {
            MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_ULTRA_BURST);
            MOVE(opponent, MOVE_CELEBRATE, gimmick: GIMMICK_ULTRA_BURST);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ULTRA_BURST, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ULTRA_BURST, opponent);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_NECROZMA_ULTRA);
        EXPECT_EQ(opponent->species, SPECIES_NECROZMA_ULTRA);
        EXPECT(HasTrainerUsedGimmick(B_BATTLER_0, GIMMICK_ULTRA_BURST));
        EXPECT(HasTrainerUsedGimmick(B_BATTLER_1, GIMMICK_ULTRA_BURST));
    }
}

AI_SINGLE_BATTLE_TEST("Arena gimmicks: AI cannot choose Mega or Tera")
{
    enum Gimmick gimmick = GIMMICK_MEGA;
    PARAMETRIZE { gimmick = GIMMICK_MEGA; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    GIVEN {
        USE_ARENA_RULES();
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_VENUSAUR) {
            if (gimmick == GIMMICK_MEGA)
                Item(ITEM_VENUSAURITE);
            else
                TeraType(TYPE_WATER);
            Moves(MOVE_SLUDGE_BOMB);
        }
    } WHEN {
        TURN { EXPECT_MOVE(opponent, MOVE_SLUDGE_BOMB, gimmick: GIMMICK_NONE); }
    } SCENE {
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MEGA_EVOLUTION, opponent);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_ACTIVATE, opponent);
        }
    } THEN {
        EXPECT_EQ(opponent->species, SPECIES_VENUSAUR);
        EXPECT_EQ(GetActiveGimmick(B_BATTLER_1), GIMMICK_NONE);
    }
}

AI_SINGLE_BATTLE_TEST("Arena gimmicks: AI can choose Z-Moves Dynamax and Ultra Burst")
{
    enum Gimmick gimmick = GIMMICK_Z_MOVE;
    PARAMETRIZE { gimmick = GIMMICK_Z_MOVE; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; }
    PARAMETRIZE { gimmick = GIMMICK_ULTRA_BURST; }
    GIVEN {
        USE_ARENA_RULES();
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(1000); HP(1000); }
        OPPONENT(gimmick == GIMMICK_ULTRA_BURST ? SPECIES_NECROZMA_DUSK_MANE : SPECIES_WOBBUFFET) {
            if (gimmick == GIMMICK_Z_MOVE)
                Item(ITEM_NORMALIUM_Z);
            else if (gimmick == GIMMICK_DYNAMAX)
                DynamaxLevel(10);
            else
                Item(ITEM_ULTRANECROZIUM_Z);
            Moves(MOVE_POUND);
        }
    } WHEN {
        TURN { EXPECT_MOVE(opponent, MOVE_POUND, gimmick: gimmick); }
    } THEN {
        EXPECT(HasTrainerUsedGimmick(B_BATTLER_1, gimmick));
    }
}

#undef USE_ARENA_RULES
