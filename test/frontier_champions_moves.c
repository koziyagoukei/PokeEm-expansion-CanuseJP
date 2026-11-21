#include "global.h"
#include "config/battle.h"
#include "move.h"
#include "test/test.h"

TEST("Frontier Champions moves: keep only the favorable values")
{
    ASSUME(B_FRONTIER_CHAMPIONS_MOVE_BUFFS);
    ASSUME(B_UPDATED_MOVE_DATA == GEN_9);

    EXPECT_EQ(GetMoveAccuracy(MOVE_SCREECH), 100);
    EXPECT_EQ(GetMoveAccuracy(MOVE_METAL_SOUND), 100);
    EXPECT_EQ(GetMoveAccuracy(MOVE_STEEL_WING), 100);
    EXPECT_EQ(GetMoveAccuracy(MOVE_FROST_BREATH), 100);
    EXPECT_EQ(GetMoveAccuracy(MOVE_ROCK_SLIDE), 95);
    EXPECT_EQ(GetMoveAdditionalEffectById(MOVE_ROCK_SLIDE, 0)->chance, 30);
    EXPECT_EQ(GetMoveAccuracy(MOVE_CRABHAMMER), 95);
    EXPECT_EQ(GetMovePower(MOVE_GEAR_GRIND), 60);
    EXPECT_EQ(GetMovePP(MOVE_NIGHT_SLASH), 20);
    EXPECT_EQ(GetMovePower(MOVE_BEAK_BLAST), 120);
    EXPECT_EQ(GetMovePP(MOVE_BEAK_BLAST), 15);
    EXPECT_EQ(GetMovePower(MOVE_BLOOD_MOON), 140);
    EXPECT_EQ(GetMovePP(MOVE_WISH), 10);
    EXPECT_EQ(GetMovePP(MOVE_STRENGTH_SAP), 10);
}

TEST("Frontier historical moves: keep favorable numbers without replacing core effects")
{
    ASSUME(B_FRONTIER_CHAMPIONS_MOVE_BUFFS);
    ASSUME(B_UPDATED_MOVE_DATA == GEN_9);

    EXPECT_EQ(GetMovePower(MOVE_TACKLE), 50);
    EXPECT_EQ(GetMovePower(MOVE_DIG), 100);
    EXPECT_EQ(GetMoveAccuracy(MOVE_BLIZZARD), 90);
    EXPECT_EQ(GetMovePP(MOVE_RECOVER), 20);
    EXPECT_EQ(GetMovePP(MOVE_ROOST), 10);
    EXPECT_EQ(GetMoveAdditionalEffectById(MOVE_FIRE_BLAST, 0)->chance, 30);
    EXPECT_EQ(GetMovePriority(MOVE_FOLLOW_ME), 3);
    EXPECT_EQ(GetMovePower(MOVE_LOW_KICK), 1);
    EXPECT_EQ(GetMovePower(MOVE_BOUNCY_BUBBLE), 60);
}
