#include "global.h"
#include "test/battle.h"
#include "frontier_gc_battle.h"

// ABL-G05 (gba-gc-extension docs/ability-mechanics): the GC refuses an ability
// whose hook list it would run only in part (a contact status with a contact
// damage, or a type reaction another of the same type always pre-empts). The
// exporter writes none: every ability, for every roster that has abilities,
// gives a block the GC's own validity check (the vendored codec) accepts.
TEST("GC V19 reference: ability hooks: every exported ability is one the GC runs in full (V11-V19)")
{
    u32 checked = 0, hooks = 0, multi = 0;
    for (u32 roster = BATTLE_APP_VERSION_ABILITIES; roster <= BATTLE_APP_VERSION_CONSUMABLE_ITEMS; roster++)
    {
        for (u32 ability = 0; ability < ABILITIES_COUNT; ability++)
        {
            BattleAbility a;
            FrontierGcBattle_TestResolveAbility(roster, ability, &a);
            a.source = ability;
            if (!battle_ability_valid(&a, NUMBER_OF_MON_TYPES, roster))
                Test_MgbaPrintf("refused: roster %d ability %d (%d hooks)", roster, ability, a.count);
            EXPECT(battle_ability_valid(&a, NUMBER_OF_MON_TYPES, roster));
            checked++;
            hooks += a.count;
            multi += a.count > 1;
        }
    }
    Test_MgbaPrintf("ABILITY_HOOKS checked=%d hooks=%d multi=%d", checked, hooks, multi);
}

// The refused lists themselves, through the same vendored check.
TEST("GC V19 reference: ability hooks: a contact status with a contact damage is refused")
{
    const BattleAbility pair = {.cls = BACLASS_HOOKS, .count = 2, .source = ABILITY_STATIC,
                                .hooks = {{BABIL_CONTACT_STATUS, {{BST_PARALYSIS, 0, 0, 30, 100, 0}}, 0},
                                          {BABIL_CONTACT_DAMAGE, {{0, 0, 8, 0, 0, 0}}, 0}}};
    BattleAbility one = pair;
    one.count = 1;
    for (u32 roster = BATTLE_APP_VERSION_ABILITY_HOOKS; roster <= BATTLE_APP_VERSION_CONSUMABLE_ITEMS; roster++)
    {
        EXPECT(!battle_ability_valid(&pair, NUMBER_OF_MON_TYPES, roster));
        EXPECT(battle_ability_valid(&one, NUMBER_OF_MON_TYPES, roster));
    }
}
