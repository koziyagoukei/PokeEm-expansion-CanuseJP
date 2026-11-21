#include "global.h"
#include "test/battle.h"
#include "battle_frontier.h"
#include "frontier_gc_battle.h"
#include "malloc.h"
#include "constants/battle_frontier_mons.h"

// GameCube battle request V18, M4-B1a+ (gba-gc-extension docs/items.md,
// section 11): audited Z-Crystals and Mega Stones held in a GC battle without
// gimmicks are held items with no effect (BICLASS_NONE, the id kept). Three
// parts:
//   1. this build's battles: without the gimmick these items do nothing, but
//      the moves that read or move held items see them (why the GC must keep
//      the id and never run those moves);
//   2. the exporter's own judgement (FrontierGcBattle_Test*): the audit, the
//      battle-wide conditions, and the moves it must not send as plain hits;
//   3. the Battle Frontier sets through the real exporter (coverage tally).
// No MESSAGE checks: the text of this build is Japanese.

ASSUMPTIONS
{
    ASSUME(GetItemHoldEffect(ITEM_NORMALIUM_Z) == HOLD_EFFECT_Z_CRYSTAL);
    ASSUME(GetItemHoldEffect(ITEM_VENUSAURITE) == HOLD_EFFECT_MEGA_STONE);
    ASSUME(GetItemHoldEffect(ITEM_RED_ORB) == HOLD_EFFECT_PRIMAL_ORB);
}

// ---------------------------------------------------------------- 1. battles

SINGLE_BATTLE_TEST("GC V18 gimmick items: a Z-Crystal does nothing without the Z-Move", s16 damage)
{
    enum Item item;
    PARAMETRIZE { item = ITEM_NONE; }
    PARAMETRIZE { item = ITEM_NORMALIUM_Z; }
    PARAMETRIZE { item = ITEM_SNORLIUM_Z; }
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Item(item); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_SNORLAX);
        EXPECT_EQ(player->item, item);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
        EXPECT_EQ(results[0].damage, results[2].damage);
    }
}

SINGLE_BATTLE_TEST("GC V18 gimmick items: a Mega Stone does nothing without Mega Evolution", s16 damage)
{
    enum Item item;
    PARAMETRIZE { item = ITEM_NONE; }
    PARAMETRIZE { item = ITEM_VENUSAURITE; }
    GIVEN {
        PLAYER(SPECIES_VENUSAUR) { Item(item); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_VENUSAUR);
        EXPECT_EQ(player->item, item);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("GC V18 gimmick items: Mega Evolution and Ultra Burst need the gimmick, Primal Reversion does not")
{
    enum Species species, after;
    enum Item item;
    PARAMETRIZE { species = SPECIES_VENUSAUR; item = ITEM_VENUSAURITE; after = SPECIES_VENUSAUR; }
    PARAMETRIZE { species = SPECIES_NECROZMA_DUSK_MANE; item = ITEM_ULTRANECROZIUM_Z; after = SPECIES_NECROZMA_DUSK_MANE; }
    PARAMETRIZE { species = SPECIES_GROUDON; item = ITEM_RED_ORB; after = SPECIES_GROUDON_PRIMAL; }
    GIVEN {
        PLAYER(species) { Item(item); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(player->species, after);
    }
}

SINGLE_BATTLE_TEST("GC V18 gimmick items: Acrobatics counts a held Z-Crystal or Mega Stone", s16 damage)
{
    enum Item item;
    PARAMETRIZE { item = ITEM_NONE; }
    PARAMETRIZE { item = ITEM_NORMALIUM_Z; }
    PARAMETRIZE { item = ITEM_VENUSAURITE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ACROBATICS) == EFFECT_ACROBATICS);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Item(item); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_ACROBATICS); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[1].damage, Q_4_12(2.0), results[0].damage);
        EXPECT_EQ(results[1].damage, results[2].damage);
    }
}

SINGLE_BATTLE_TEST("GC V18 gimmick items: Knock Off cannot remove a Z-Crystal or a holder's own Mega Stone, nor gets its boost", s16 damage)
{
    enum Species species;
    enum Item item;
    bool32 removed;
    PARAMETRIZE { species = SPECIES_WOBBUFFET; item = ITEM_NONE; removed = FALSE; }
    PARAMETRIZE { species = SPECIES_WOBBUFFET; item = ITEM_NORMALIUM_Z; removed = FALSE; }
    PARAMETRIZE { species = SPECIES_VENUSAUR; item = ITEM_VENUSAURITE; removed = FALSE; }
    PARAMETRIZE { species = SPECIES_WOBBUFFET; item = ITEM_POTION; removed = TRUE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_KNOCK_OFF) == EFFECT_KNOCK_OFF);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(species) { Item(item); }
    } WHEN {
        TURN { MOVE(player, MOVE_KNOCK_OFF); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } THEN {
        EXPECT_EQ(opponent->item, removed ? ITEM_NONE : item);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.5), results[3].damage);
    }
}

SINGLE_BATTLE_TEST("GC V18 gimmick items: Trick fails with a Z-Crystal or a Mega Stone on either side")
{
    enum Species atkSpecies, defSpecies;
    enum Item atkItem, defItem;
    PARAMETRIZE { atkSpecies = SPECIES_WOBBUFFET; atkItem = ITEM_NORMALIUM_Z; defSpecies = SPECIES_WOBBUFFET; defItem = ITEM_POTION; }
    PARAMETRIZE { atkSpecies = SPECIES_WOBBUFFET; atkItem = ITEM_POTION; defSpecies = SPECIES_WOBBUFFET; defItem = ITEM_NORMALIUM_Z; }
    PARAMETRIZE { atkSpecies = SPECIES_VENUSAUR; atkItem = ITEM_VENUSAURITE; defSpecies = SPECIES_WOBBUFFET; defItem = ITEM_POTION; }
    PARAMETRIZE { atkSpecies = SPECIES_WOBBUFFET; atkItem = ITEM_POTION; defSpecies = SPECIES_VENUSAUR; defItem = ITEM_VENUSAURITE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_TRICK) == EFFECT_TRICK);
        PLAYER(atkSpecies) { Item(atkItem); }
        OPPONENT(defSpecies) { Item(defItem); }
    } WHEN {
        TURN { MOVE(player, MOVE_TRICK); }
    } THEN {
        EXPECT_EQ(player->item, atkItem);
        EXPECT_EQ(opponent->item, defItem);
    }
}

SINGLE_BATTLE_TEST("GC V18 gimmick items: Poltergeist hits a Z-Crystal or Mega Stone holder, not an empty hand")
{
    enum Species species;
    enum Item item;
    PARAMETRIZE { species = SPECIES_WOBBUFFET; item = ITEM_NONE; }
    PARAMETRIZE { species = SPECIES_WOBBUFFET; item = ITEM_NORMALIUM_Z; }
    PARAMETRIZE { species = SPECIES_VENUSAUR; item = ITEM_VENUSAURITE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_POLTERGEIST) == EFFECT_POLTERGEIST);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(species) { Item(item); }
    } WHEN {
        TURN { MOVE(player, MOVE_POLTERGEIST); }
    } SCENE {
        if (item == ITEM_NONE)
            NOT HP_BAR(opponent);
        else
            HP_BAR(opponent);
    }
}

SINGLE_BATTLE_TEST("GC V18 gimmick items: Sunsteel Strike ignores the target's Sturdy")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_IRON_HEAD; }
    PARAMETRIZE { move = MOVE_SUNSTEEL_STRIKE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SUNSTEEL_STRIKE) == EFFECT_HIT);
        ASSUME(MoveIgnoresTargetAbility(MOVE_SUNSTEEL_STRIKE));
        ASSUME(!MoveIgnoresTargetAbility(MOVE_IRON_HEAD));
        PLAYER(SPECIES_SOLGALEO) { Level(100); }
        OPPONENT(SPECIES_GEODUDE) { Ability(ABILITY_STURDY); Level(5); }
    } WHEN {
        TURN { MOVE(player, move); }
    } THEN {
        if (move == MOVE_SUNSTEEL_STRIKE)
            EXPECT_EQ(opponent->hp, 0);
        else
            EXPECT_EQ(opponent->hp, 1);
    }
}

SINGLE_BATTLE_TEST("GC V18 gimmick items: Sacred Sword ignores the target's Defense stages", s16 damage)
{
    enum Move move;
    PARAMETRIZE { move = MOVE_CLOSE_COMBAT; }
    PARAMETRIZE { move = MOVE_CLOSE_COMBAT; }
    PARAMETRIZE { move = MOVE_SACRED_SWORD; }
    PARAMETRIZE { move = MOVE_SACRED_SWORD; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SACRED_SWORD) == EFFECT_HIT);
        ASSUME(MoveIgnoresDefenseEvasionStages(MOVE_SACRED_SWORD));
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, i % 2 ? MOVE_IRON_DEFENSE : MOVE_CELEBRATE); }
        TURN { MOVE(player, move); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_LT(results[1].damage, results[0].damage);
        EXPECT_EQ(results[2].damage, results[3].damage);
    }
}

// ------------------------------------------------- 2. the exporter's judgement

// The audited items (gba-gc-extension docs/items.md 11.2). Ultranecrozium Z
// is left out (Ultra Burst); a Z-Crystal or Mega Stone not on these lists
// must be refused (the exporter's lists are the same; this one is the test's).
static const u16 sAuditedZ[] = {
    ITEM_NORMALIUM_Z, ITEM_FIRIUM_Z, ITEM_WATERIUM_Z, ITEM_ELECTRIUM_Z, ITEM_GRASSIUM_Z, ITEM_ICIUM_Z,
    ITEM_FIGHTINIUM_Z, ITEM_POISONIUM_Z, ITEM_GROUNDIUM_Z, ITEM_FLYINIUM_Z, ITEM_PSYCHIUM_Z, ITEM_BUGINIUM_Z,
    ITEM_ROCKIUM_Z, ITEM_GHOSTIUM_Z, ITEM_DRAGONIUM_Z, ITEM_DARKINIUM_Z, ITEM_STEELIUM_Z, ITEM_FAIRIUM_Z,
    ITEM_PIKANIUM_Z, ITEM_EEVIUM_Z, ITEM_SNORLIUM_Z, ITEM_MEWNIUM_Z, ITEM_DECIDIUM_Z, ITEM_INCINIUM_Z,
    ITEM_PRIMARIUM_Z, ITEM_LYCANIUM_Z, ITEM_MIMIKIUM_Z, ITEM_KOMMONIUM_Z, ITEM_TAPUNIUM_Z, ITEM_SOLGANIUM_Z,
    ITEM_LUNALIUM_Z, ITEM_MARSHADIUM_Z, ITEM_ALORAICHIUM_Z, ITEM_PIKASHUNIUM_Z,
};

static bool32 InList(const u16 *list, u32 count, u16 item)
{
    for (u32 i = 0; i < count; i++)
        if (list[i] == item)
            return TRUE;
    return FALSE;
}

TEST("GC V18 export: every Z-Crystal and Mega Stone of this build is audited or refused")
{
    u32 z = 0, mega = 0;
    for (u16 item = 1; item < ITEMS_COUNT; item++)
    {
        const enum HoldEffect hold = GetItemHoldEffect(item);
        const u8 kind = FrontierGcBattle_TestGimmickItem(item, SPECIES_WOBBUFFET);
        if (hold == HOLD_EFFECT_Z_CRYSTAL)
        {
            if (item == ITEM_ULTRANECROZIUM_Z || !InList(sAuditedZ, ARRAY_COUNT(sAuditedZ), item))
                EXPECT_EQ(kind, 0);
            else
                EXPECT_NE(kind, 0);
            z += kind != 0;
        }
        else if (hold == HOLD_EFFECT_MEGA_STONE)
        {
            EXPECT_NE(kind, 0);  // an unaudited stone fails here: audit it (docs/items.md 11.2) or leave it out
            mega += kind != 0;
        }
        else
        {
            EXPECT_EQ(kind, 0);
        }
    }
    EXPECT_EQ(z, 34);
    EXPECT_EQ(mega, 92);
}

// Every form change of every species that names a Z-Crystal or a Mega Stone:
// the item is a candidate for that holder only if the change is Mega
// Evolution with its own stone (a gimmick, never run).
TEST("GC V18 export: a gimmick item is refused for a holder whose form change uses it")
{
    for (u32 species = 1; species < NUM_SPECIES; species++)
    {
        const struct FormChange *f;
        const struct SpeciesInfo *info = &gSpeciesInfo[species];
        if (!IsSpeciesEnabled(species))
            continue;
        f = GetSpeciesFormChanges(species);
        for (; f != NULL && f->method != FORM_CHANGE_TERMINATOR; f++)
        {
            enum HoldEffect hold;
            u8 kind;
            if (f->param1 == ITEM_NONE || f->param1 >= ITEMS_COUNT)
                continue;  // no item (or another parameter: a weather, an ability ...)
            hold = GetItemHoldEffect(f->param1);
            kind = FrontierGcBattle_TestGimmickItem(f->param1, species);
            if (hold != HOLD_EFFECT_Z_CRYSTAL && hold != HOLD_EFFECT_MEGA_STONE && hold != HOLD_EFFECT_PRIMAL_ORB)
                continue;
            if (f->method == FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM && hold == HOLD_EFFECT_MEGA_STONE
             && !info->isMegaEvolution && !info->isPrimalReversion && !info->isUltraBurst && !info->isGigantamax
             && !info->isTeraForm)
                EXPECT_NE(kind, 0);
            else
                EXPECT_EQ(kind, 0);
        }
        // A Mega, Primal, Ultra Burst, Gigantamax or Tera form never holds one.
        if (info->isMegaEvolution || info->isPrimalReversion || info->isUltraBurst || info->isGigantamax || info->isTeraForm)
        {
            EXPECT_EQ(FrontierGcBattle_TestGimmickItem(ITEM_NORMALIUM_Z, species), 0);
            EXPECT_EQ(FrontierGcBattle_TestGimmickItem(ITEM_VENUSAURITE, species), 0);
        }
    }
}

TEST("GC V18 export: Arceus, Necrozma, Primal orbs and other items")
{
    for (u32 i = 0; i < ARRAY_COUNT(sAuditedZ); i++)
        EXPECT_EQ(FrontierGcBattle_TestGimmickItem(sAuditedZ[i], SPECIES_ARCEUS), 0);  // Multitype forms
    EXPECT_EQ(FrontierGcBattle_TestGimmickItem(ITEM_FIGHTINIUM_Z, SPECIES_ARCEUS_FIGHTING), 0);
    EXPECT_EQ(FrontierGcBattle_TestGimmickItem(ITEM_ULTRANECROZIUM_Z, SPECIES_NECROZMA_DUSK_MANE), 0);
    EXPECT_EQ(FrontierGcBattle_TestGimmickItem(ITEM_ULTRANECROZIUM_Z, SPECIES_WOBBUFFET), 0);
    EXPECT_NE(FrontierGcBattle_TestGimmickItem(ITEM_SOLGANIUM_Z, SPECIES_NECROZMA_DUSK_MANE), 0);
    EXPECT_EQ(FrontierGcBattle_TestGimmickItem(ITEM_RED_ORB, SPECIES_GROUDON), 0);
    EXPECT_EQ(FrontierGcBattle_TestGimmickItem(ITEM_BLUE_ORB, SPECIES_WOBBUFFET), 0);
    EXPECT_EQ(FrontierGcBattle_TestGimmickItem(ITEM_LEFTOVERS, SPECIES_WOBBUFFET), 0);
    EXPECT_EQ(FrontierGcBattle_TestGimmickItem(ITEM_RUSTED_SWORD, SPECIES_ZACIAN), 0);
    EXPECT_NE(FrontierGcBattle_TestGimmickItem(ITEM_VENUSAURITE, SPECIES_VENUSAUR), 0);
    EXPECT_NE(FrontierGcBattle_TestGimmickItem(ITEM_VENUSAURITE, SPECIES_WOBBUFFET), 0);
    EXPECT_EQ(FrontierGcBattle_TestGimmickItem(ITEM_VENUSAURITE, SPECIES_VENUSAUR_MEGA), 0);
    EXPECT_NE(FrontierGcBattle_TestGimmickItem(ITEM_CHARIZARDITE_X, SPECIES_CHARIZARD), 0);
}

// Moves with a behaviour the GC does not model must never be sent as plain hits.
TEST("GC V18 export: moves that ignore abilities or stages, and Shadow moves, are unsupported")
{
    static const u16 moves[] = {
        MOVE_SUNSTEEL_STRIKE, MOVE_MOONGEIST_BEAM, MOVE_PHOTON_GEYSER,     // the target's ability
        MOVE_SACRED_SWORD, MOVE_CHIP_AWAY, MOVE_DARKEST_LARIAT,             // its Defense and evasion stages
        MOVE_THOUSAND_ARROWS,                                               // Flying types and Levitate
        MOVE_SHADOW_BLITZ, MOVE_SHADOW_RUSH, MOVE_SHADOW_BREAK, MOVE_SHADOW_WAVE, MOVE_SHADOW_RAVE,
        MOVE_SHADOW_STORM, MOVE_SHADOW_FIRE, MOVE_SHADOW_BOLT, MOVE_SHADOW_CHILL, MOVE_SHADOW_BLAST,
        MOVE_SHADOW_MIST, MOVE_SHADOW_DOWN, MOVE_SHADOW_PANIC, MOVE_SHADOW_HOLD, MOVE_SHADOW_SKY,
    };
    for (u32 i = 0; i < ARRAY_COUNT(moves); i++)
    {
        EXPECT_NE(FrontierGcBattle_TestMoveSupport(BATTLE_APP_VERSION_END_TURN_ITEMS, BATTLE_FORMAT_SINGLE, moves[i]), 0);
        EXPECT_NE(FrontierGcBattle_TestMoveSupport(BATTLE_APP_VERSION_END_TURN_ITEMS, BATTLE_FORMAT_DOUBLE, moves[i]), 0);
    }
    // The legacy V1 path (no roster) refuses them too.
    EXPECT_NE(FrontierGcBattle_TestMoveSupport(0, BATTLE_FORMAT_SINGLE, MOVE_SHADOW_BLITZ), 0);
    EXPECT_NE(FrontierGcBattle_TestMoveSupport(0, BATTLE_FORMAT_SINGLE, MOVE_SACRED_SWORD), 0);
    EXPECT_EQ(FrontierGcBattle_TestMoveSupport(0, BATTLE_FORMAT_SINGLE, MOVE_TACKLE), 0);
    // Ignoring the target's ability matters from V11, where abilities take part.
    EXPECT_NE(FrontierGcBattle_TestMoveSupport(BATTLE_APP_VERSION_ABILITIES, BATTLE_FORMAT_SINGLE, MOVE_SUNSTEEL_STRIKE), 0);
    EXPECT_EQ(FrontierGcBattle_TestMoveSupport(BATTLE_APP_VERSION_TIMING, BATTLE_FORMAT_SINGLE, MOVE_SUNSTEEL_STRIKE), 0);
    // Controls: plain hits stay supported.
    EXPECT_EQ(FrontierGcBattle_TestMoveSupport(BATTLE_APP_VERSION_END_TURN_ITEMS, BATTLE_FORMAT_SINGLE, MOVE_TACKLE), 0);
    EXPECT_EQ(FrontierGcBattle_TestMoveSupport(BATTLE_APP_VERSION_END_TURN_ITEMS, BATTLE_FORMAT_SINGLE, MOVE_IRON_HEAD), 0);
    EXPECT_EQ(FrontierGcBattle_TestMoveSupport(BATTLE_APP_VERSION_END_TURN_ITEMS, BATTLE_FORMAT_SINGLE, MOVE_CLOSE_COMBAT), 0);
}

// Moves that read, take, swap, remove or use held items (and Z-Moves): the GC
// runs none of them, so a held Z-Crystal or Mega Stone is never read in a GC battle.
TEST("GC V18 export: every move that reads or moves held items is unsupported")
{
    u32 found = 0;
    for (u16 move = 1; move < FIRST_SHADOW_MOVE; move++)
    {
        bool32 items = FALSE;
        switch (GetMoveEffect(move))
        {
        case EFFECT_KNOCK_OFF: case EFFECT_STEAL_ITEM: case EFFECT_TRICK: case EFFECT_BESTOW:
        case EFFECT_FLING: case EFFECT_ACROBATICS: case EFFECT_POLTERGEIST: case EFFECT_CORROSIVE_GAS:
        case EFFECT_EMBARGO: case EFFECT_MAGIC_ROOM: case EFFECT_RECYCLE: case EFFECT_NATURAL_GIFT:
        case EFFECT_CHANGE_TYPE_ON_ITEM: case EFFECT_BELCH: case EFFECT_STUFF_CHEEKS: case EFFECT_TEATIME:
            items = TRUE;
            break;
        default:
            break;
        }
        for (u32 e = 0; e < GetMoveAdditionalEffectCount(move); e++)
        {
            const enum MoveEffect m = GetMoveAdditionalEffectById(move, e)->moveEffect;
            if (m == MOVE_EFFECT_BUG_BITE || m == MOVE_EFFECT_INCINERATE)
                items = TRUE;
        }
        if (move >= FIRST_Z_MOVE)
            items = TRUE;  // Z-Moves and Max Moves: gimmicks
        if (!items)
            continue;
        found++;
        if (FrontierGcBattle_TestMoveSupport(BATTLE_APP_VERSION_END_TURN_ITEMS, BATTLE_FORMAT_SINGLE, move) == 0)
            Test_MgbaPrintf("supported item move: %d", move);
        EXPECT_NE(FrontierGcBattle_TestMoveSupport(BATTLE_APP_VERSION_END_TURN_ITEMS, BATTLE_FORMAT_SINGLE, move), 0);
        EXPECT_NE(FrontierGcBattle_TestMoveSupport(BATTLE_APP_VERSION_END_TURN_ITEMS, BATTLE_FORMAT_TRIPLE, move), 0);
    }
    EXPECT_GT(found, 30);
}

// A test Pokemon: level 50, the given moves (at least Tackle), ability slot 0.
static void MakeMon(struct Pokemon *mon, u16 species, u16 item, u16 move1, u16 move2)
{
    CreateMonWithIVs(mon, species, 50, 0x4D0000 + species, OTID_STRUCT_PLAYER_ID, 31);
    SetMonMoveSlot(mon, move1, 0);
    SetMonMoveSlot(mon, move2, 1);
    SetMonMoveSlot(mon, MOVE_NONE, 2);
    SetMonMoveSlot(mon, MOVE_NONE, 3);
    SetMonData(mon, MON_DATA_HELD_ITEM, &item);
}

static void SetAbilityNum(struct Pokemon *mon, u8 num)
{
    SetMonData(mon, MON_DATA_ABILITY_NUM, &num);
}

static void Export(struct FrontierGcExportResult *r, struct Pokemon *mons, u8 count, u8 format,
                   u8 roster, bool8 strictItems, bool8 strictAll, bool8 allowGimmicks)
{
    const struct FrontierGcExportTest t = {
        .roster = roster, .format = format, .count = count, .strictItems = strictItems,
        .strictAll = strictAll, .allowGimmicks = allowGimmicks, .mons = mons,
    };
    FrontierGcBattle_TestExport(&t, r);
}

static void Release(struct FrontierGcExportResult *r)
{
    TRY_FREE_AND_SET_NULL(r->request);
}

#define V18 BATTLE_APP_VERSION_END_TURN_ITEMS

TEST("GC V18 export: an audited Z-Crystal is held with no effect, its id kept, beside Leftovers")
{
    struct FrontierGcExportResult r;
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    MakeMon(&mons[0], SPECIES_SNORLAX, ITEM_SNORLIUM_Z, MOVE_TACKLE, MOVE_BODY_SLAM);
    SetAbilityNum(&mons[0], 0);  // Immunity
    MakeMon(&mons[1], SPECIES_WOBBUFFET, ITEM_LEFTOVERS, MOVE_TACKLE, MOVE_NONE);
    Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, FALSE, FALSE, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_ACCEPTED);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    EXPECT(r.request != NULL);
    if (r.request != NULL)
    {
        const BattleItem *z = &r.request->player[0].mon.item, *lo = &r.request->opponent[0].mon.item;
        EXPECT_EQ(r.request->version, BATTLE_REQUEST_VERSION_END_TURN_ITEMS);
        EXPECT(r.request->rules & RULE_NO_GIMMICKS);
        EXPECT(!(r.request->rules & RULE_IGNORE_ITEMS));
        EXPECT_EQ(r.request->player[0].record.held_item, ITEM_SNORLIUM_Z);
        EXPECT_EQ(z->cls, BICLASS_NONE);
        EXPECT_EQ(z->source, ITEM_SNORLIUM_Z);
        EXPECT_EQ(z->count, 0);
        EXPECT_EQ(lo->cls, BICLASS_RECORDS);
        EXPECT_EQ(lo->source, ITEM_LEFTOVERS);
        EXPECT_EQ(lo->count, 1);
        EXPECT_EQ(lo->records[0].kind, BITEM_HP);
    }
    Release(&r);
    // Strict items: nothing is normalised, so the battle starts.
    Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, TRUE, FALSE, FALSE);
    EXPECT(r.built);
    EXPECT(r.request != NULL && (r.request->rules & RULE_STRICT_ITEMS));
    Release(&r);
    Free(mons);
}

TEST("GC V18 export: a gimmick item stays unsupported unless the request forbids gimmicks and is V18")
{
    struct FrontierGcExportResult r;
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    MakeMon(&mons[0], SPECIES_VENUSAUR, ITEM_VENUSAURITE, MOVE_TACKLE, MOVE_NONE);
    MakeMon(&mons[1], SPECIES_WOBBUFFET, ITEM_LEFTOVERS, MOVE_TACKLE, MOVE_NONE);
    // Gimmicks allowed: every item is off, the stone named.
    Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, FALSE, FALSE, TRUE);
    EXPECT(r.built);
    EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_OFF_RULES);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
    EXPECT_EQ(r.itemReason, ITEM_VENUSAURITE);
    EXPECT(r.request != NULL && (r.request->rules & RULE_IGNORE_ITEMS));
    EXPECT(r.request != NULL && r.request->player[0].mon.item.cls == BICLASS_UNSUPPORTED);
    Release(&r);
    // ... and strict items refuse the battle.
    Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, TRUE, FALSE, TRUE);
    EXPECT(!r.built);
    Release(&r);
    // V17: unsupported as before (V17 has no Leftovers either).
    Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, BATTLE_APP_VERSION_ITEMS, FALSE, FALSE, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_OFF_VERSION);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
    EXPECT(r.request != NULL && r.request->player[0].mon.item.cls == BICLASS_UNSUPPORTED);
    Release(&r);
    // V18, gimmicks forbidden: accepted.
    Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, TRUE, FALSE, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_ACCEPTED);
    EXPECT(r.request != NULL && r.request->player[0].mon.item.cls == BICLASS_NONE
           && r.request->player[0].mon.item.source == ITEM_VENUSAURITE);
    Release(&r);
    Free(mons);
}

TEST("GC V18 export: an accepted gimmick item never lets another unsupported item in")
{
    struct FrontierGcExportResult r;
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    MakeMon(&mons[0], SPECIES_SNORLAX, ITEM_NORMALIUM_Z, MOVE_TACKLE, MOVE_NONE);
    SetAbilityNum(&mons[0], 0);
    MakeMon(&mons[1], SPECIES_WOBBUFFET, ITEM_LIFE_ORB, MOVE_TACKLE, MOVE_NONE);
    Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, FALSE, FALSE, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_ACCEPTED);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
    EXPECT_EQ(r.itemReason, ITEM_LIFE_ORB);
    EXPECT(r.request != NULL && (r.request->rules & RULE_IGNORE_ITEMS));
    Release(&r);
    Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, TRUE, FALSE, FALSE);
    EXPECT(!r.built);
    Release(&r);
    // An item ability (Frisk) turns every item off as before.
    MakeMon(&mons[1], SPECIES_BANETTE, ITEM_NONE, MOVE_TACKLE, MOVE_NONE);
    SetAbilityNum(&mons[1], 1);
    EXPECT_EQ(GetMonAbility(&mons[1]), ABILITY_FRISK);
    Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, FALSE, FALSE, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ABILITY);
    EXPECT_EQ(r.itemReason, ABILITY_FRISK);
    Release(&r);
    Free(mons);
}

TEST("GC V18 export: a Pokemon that changes form by itself, on either side or the bench, keeps gimmick items unsupported")
{
    struct FrontierGcExportResult r;
    struct Pokemon *mons = AllocZeroed(6 * sizeof(struct Pokemon));
    MakeMon(&mons[0], SPECIES_CHARIZARD, ITEM_CHARIZARDITE_X, MOVE_TACKLE, MOVE_NONE);
    MakeMon(&mons[1], SPECIES_SNORLAX, ITEM_LEFTOVERS, MOVE_TACKLE, MOVE_NONE);
    MakeMon(&mons[2], SPECIES_VENUSAUR, ITEM_BLACK_SLUDGE, MOVE_TACKLE, MOVE_NONE);
    MakeMon(&mons[3], SPECIES_WOBBUFFET, ITEM_NONE, MOVE_TACKLE, MOVE_NONE);
    MakeMon(&mons[4], SPECIES_WOBBUFFET, ITEM_NONE, MOVE_TACKLE, MOVE_NONE);
    MakeMon(&mons[5], SPECIES_WOBBUFFET, ITEM_NONE, MOVE_TACKLE, MOVE_NONE);
    Export(&r, mons, 3, BATTLE_FORMAT_DOUBLE, V18, FALSE, FALSE, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_ACCEPTED);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    Release(&r);
    // The opponent's bench: Castform (FORM_CHANGE_BEGIN_BATTLE).
    MakeMon(&mons[5], SPECIES_CASTFORM, ITEM_NONE, MOVE_TACKLE, MOVE_NONE);
    Export(&r, mons, 3, BATTLE_FORMAT_DOUBLE, V18, FALSE, FALSE, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_OFF_FORM);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
    EXPECT_EQ(r.itemReason, ITEM_CHARIZARDITE_X);
    Release(&r);
    // Xerneas: its only automatic change is at the battle start (FORM_CHANGE_BEGIN_BATTLE).
    MakeMon(&mons[5], SPECIES_XERNEAS, ITEM_NONE, MOVE_TACKLE, MOVE_NONE);
    Export(&r, mons, 3, BATTLE_FORMAT_DOUBLE, V18, FALSE, FALSE, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_OFF_FORM);
    Release(&r);
    // Without a gimmick item the same battle is as in M4-B1a.
    MakeMon(&mons[0], SPECIES_CHARIZARD, ITEM_NONE, MOVE_TACKLE, MOVE_NONE);
    Export(&r, mons, 3, BATTLE_FORMAT_DOUBLE, V18, FALSE, FALSE, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_NONE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    Release(&r);
    Free(mons);
}

// Condition 8: a Pokemon of the battle (either side, the bench too) that
// knows a move reading or moving held items keeps gimmick items unsupported,
// even though the GC never runs that move: in this build it would see the
// Z-Crystal or Mega Stone (Knock Off, Poltergeist above). Normal: every item
// off; strict items: refused.
TEST("GC V18 export: a move that reads held items anywhere in the battle keeps gimmick items unsupported")
{
    struct FrontierGcExportResult r;
    struct Pokemon *mons = AllocZeroed(6 * sizeof(struct Pokemon));
    EXPECT_NE(FrontierGcBattle_TestMoveSupport(BATTLE_APP_VERSION_END_TURN_ITEMS, BATTLE_FORMAT_SINGLE, MOVE_KNOCK_OFF), 0);
    MakeMon(&mons[0], SPECIES_SNORLAX, ITEM_SNORLIUM_Z, MOVE_TACKLE, MOVE_NONE);
    SetAbilityNum(&mons[0], 0);
    MakeMon(&mons[1], SPECIES_WOBBUFFET, ITEM_LEFTOVERS, MOVE_TACKLE, MOVE_KNOCK_OFF);
    Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, FALSE, FALSE, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_OFF_MOVE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
    EXPECT_EQ(r.itemReason, ITEM_SNORLIUM_Z);
    EXPECT(r.request != NULL && r.request->player[0].mon.item.cls == BICLASS_UNSUPPORTED);
    Release(&r);
    Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, TRUE, FALSE, FALSE);
    EXPECT(!r.built);
    Release(&r);
    // Without the Z-Crystal, Knock Off changes nothing for the items (M4-B1a).
    MakeMon(&mons[0], SPECIES_SNORLAX, ITEM_NONE, MOVE_TACKLE, MOVE_NONE);
    SetAbilityNum(&mons[0], 0);
    Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, FALSE, FALSE, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_NONE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    Release(&r);
    // The holder's own bench partner with Poltergeist, in a Double.
    MakeMon(&mons[0], SPECIES_CHARIZARD, ITEM_CHARIZARDITE_X, MOVE_TACKLE, MOVE_NONE);
    MakeMon(&mons[1], SPECIES_SNORLAX, ITEM_LEFTOVERS, MOVE_TACKLE, MOVE_NONE);
    MakeMon(&mons[2], SPECIES_GENGAR, ITEM_NONE, MOVE_TACKLE, MOVE_POLTERGEIST);
    MakeMon(&mons[3], SPECIES_WOBBUFFET, ITEM_NONE, MOVE_TACKLE, MOVE_NONE);
    MakeMon(&mons[4], SPECIES_WOBBUFFET, ITEM_NONE, MOVE_TACKLE, MOVE_NONE);
    MakeMon(&mons[5], SPECIES_WOBBUFFET, ITEM_NONE, MOVE_TACKLE, MOVE_NONE);
    Export(&r, mons, 3, BATTLE_FORMAT_DOUBLE, V18, FALSE, FALSE, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_OFF_MOVE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
    EXPECT_EQ(r.itemReason, ITEM_CHARIZARDITE_X);
    Release(&r);
    Free(mons);
}

TEST("GC V18 export: holders refused one by one (Arceus, Ultranecrozium Z, Primal orb, a Mega form)")
{
    struct FrontierGcExportResult r;
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    static const u16 holders[][2] = {
        {SPECIES_ARCEUS_FIRE, ITEM_FIRIUM_Z},
        {SPECIES_NECROZMA_DAWN_WINGS, ITEM_ULTRANECROZIUM_Z},
        {SPECIES_KYOGRE, ITEM_BLUE_ORB},
        {SPECIES_VENUSAUR_MEGA, ITEM_VENUSAURITE},
    };
    for (u32 i = 0; i < ARRAY_COUNT(holders); i++)
    {
        MakeMon(&mons[0], holders[i][0], holders[i][1], MOVE_TACKLE, MOVE_NONE);
        MakeMon(&mons[1], SPECIES_WOBBUFFET, ITEM_NONE, MOVE_TACKLE, MOVE_NONE);
        Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, FALSE, FALSE, FALSE);
        EXPECT(r.built);
        EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
        EXPECT_EQ(r.itemReason, holders[i][1]);
        EXPECT(r.request != NULL && r.request->player[0].mon.item.cls == BICLASS_UNSUPPORTED);
        Release(&r);
    }
    Free(mons);
}

// ---------------------------------------- 3. the Battle Frontier sets (tally)

// Every gBattleFrontierMons set as the player's lead in a V18 Single against
// a plain Snorlax (Immunity, Tackle, no item), through the real exporter. One
// line per chunk; tools add them up (gba-gc-extension docs/items.md 11.8).
// Counted: requests built; items simulated (no normalisation); items, ability
// and all four moves simulated; a strict battle (items, abilities and moves)
// that starts on the GC; and why items were normalised.
#define TALLY_CHUNK 120
TEST("GC V18 export tally: Battle Frontier sets")
{
    u32 chunk;
    u32 built = 0, simulated = 0, full = 0, strict = 0, offItem = 0, offAbility = 0, offZ = 0, offMega = 0;
    u32 zAccepted = 0, megaAccepted = 0, gimmickOff[8] = {0}, zHeld = 0, megaHeld = 0;
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    for (chunk = 0; chunk < (NUM_FRONTIER_MONS + TALLY_CHUNK - 1) / TALLY_CHUNK; chunk++)
        PARAMETRIZE_LABEL("%d", chunk) {}
    chunk = gFunctionTestRunnerState->runParameter;
    for (u32 set = chunk * TALLY_CHUNK; set < (chunk + 1) * TALLY_CHUNK && set < NUM_FRONTIER_MONS; set++)
    {
        struct FrontierGcExportResult r;
        const enum HoldEffect hold = GetItemHoldEffect(gBattleFrontierMons[set].heldItem);
        CreateFacilityMon(&gBattleFrontierMons[set], 50, 31, 0x12345678, 0, &mons[0]);
        MakeMon(&mons[1], SPECIES_SNORLAX, ITEM_NONE, MOVE_TACKLE, MOVE_NONE);
        SetAbilityNum(&mons[1], 0);
        zHeld += hold == HOLD_EFFECT_Z_CRYSTAL;
        megaHeld += hold == HOLD_EFFECT_MEGA_STONE;
        Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, FALSE, FALSE, FALSE);
        if (r.built)
        {
            const BattleEntry *e = &r.request->player[0];
            bool32 movesOk = TRUE;
            built++;
            for (u32 m = 0; m < MAX_MON_MOVES; m++)
                if (e->record.moves[m] != MOVE_NONE && e->mon.moves[m].unsupported != 0)
                    movesOk = FALSE;
            if (r.itemState == FGC_ITEMS_SIMULATED)
            {
                simulated++;
                if (movesOk && e->mon.ability.cls != BACLASS_UNSUPPORTED)
                    full++;
            }
            else if (r.itemState == FGC_ITEMS_OFF_ABILITY)
                offAbility++;
            else
            {
                offItem++;
                offZ += GetItemHoldEffect(r.itemReason) == HOLD_EFFECT_Z_CRYSTAL;
                offMega += GetItemHoldEffect(r.itemReason) == HOLD_EFFECT_MEGA_STONE;
            }
            if (r.gimmickState == FGC_GIMMICK_ITEMS_ACCEPTED && e->mon.item.cls == BICLASS_NONE && e->record.held_item)
            {
                zAccepted += hold == HOLD_EFFECT_Z_CRYSTAL;
                megaAccepted += hold == HOLD_EFFECT_MEGA_STONE;
            }
            gimmickOff[r.gimmickState]++;
        }
        Release(&r);
        // Strict: items, abilities and moves must all be simulated. The
        // exporter refuses items; the GC refuses abilities and moves
        // (RULE_STRICT_ABILITIES, RULE_STRICT_MOVES), here read from the request.
        Export(&r, mons, 1, BATTLE_FORMAT_SINGLE, V18, TRUE, TRUE, FALSE);
        if (r.built && r.request != NULL)
        {
            bool32 ok = TRUE;
            for (u32 side = 0; side < 2; side++)
            {
                const BattleEntry *e = side ? &r.request->opponent[0] : &r.request->player[0];
                if (e->mon.ability.cls == BACLASS_UNSUPPORTED)
                    ok = FALSE;
                for (u32 m = 0; m < MAX_MON_MOVES; m++)
                    if (e->record.moves[m] != MOVE_NONE && e->mon.moves[m].unsupported != 0)
                        ok = FALSE;
            }
            strict += ok;
        }
        Release(&r);
    }
    Free(mons);
    Test_MgbaPrintf("TALLY chunk=%d built=%d simulated=%d full=%d strict=%d offItem=%d offAbility=%d offZ=%d offMega=%d "
                    "zHeld=%d megaHeld=%d zAccepted=%d megaAccepted=%d g0=%d g1=%d g2=%d g3=%d g4=%d g5=%d",
                    chunk, built, simulated, full, strict, offItem, offAbility, offZ, offMega,
                    zHeld, megaHeld, zAccepted, megaAccepted,
                    gimmickOff[0], gimmickOff[1], gimmickOff[2], gimmickOff[3], gimmickOff[4], gimmickOff[5]);
}

// ------------------------------- 4. the debug fixtures, byte for byte (regression)

static u32 Crc32(const u8 *data, u32 length)
{
    u32 crc = 0xFFFFFFFF;
    for (u32 i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (u32 b = 0; b < 8; b++)
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
    }
    return ~crc;
}

// The request as the GC receives it, with the battle id (random outside the
// Lab) set to 0; 0 if none was built.
static u32 FixtureHash(u8 lab, u8 roster, u8 format, bool8 endurance, bool8 legacy, struct FrontierGcExportResult *out)
{
    enum { CAPACITY = BATTLE_REQUEST_MAX_BYTES(PARTY_SIZE, NUMBER_OF_MON_TYPES) };
    u8 *bytes = AllocZeroed(CAPACITY);
    u32 crc = 0, length;
    FrontierGcBattle_TestSetLegacyExport(legacy);
    FrontierGcBattle_TestExportFixture(lab, roster, format, endurance, TRAINER_CALVIN_1, out);
    FrontierGcBattle_TestSetLegacyExport(FALSE);
    if (out->request != NULL)
    {
        out->request->battle_id = 0;
        length = battle_request_encode(out->request, bytes, CAPACITY);
        crc = length ? Crc32(bytes, length) : 1;
    }
    Free(bytes);
    return crc;
}

TEST("GC V18 export: Lab fixtures L01-L57 are byte for byte as in M4-B1a; L58-L64 as designed")
{
    struct FrontierGcExportResult r, old;
    u32 lab;
    for (lab = 1; lab <= FrontierGcBattle_TestLabCount(); lab++)
        PARAMETRIZE_LABEL("L%d", lab) {}
    lab = gFunctionTestRunnerState->runParameter + 1;
    {
        const u32 now = FixtureHash(lab, 0, 0, FALSE, FALSE, &r);
        const u32 before = FixtureHash(lab, 0, 0, FALSE, TRUE, &old);
        Test_MgbaPrintf("FIXTURE L%d built=%d items=%d reason=%d gimmick=%d crc=%d m4b1a=%d",
                        lab, r.built, r.itemState, r.itemReason, r.gimmickState, (s32)now, (s32)before);
        if (lab <= 57)
        {
            EXPECT_EQ(now, before);
            EXPECT_EQ(r.built, old.built);
            EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_NONE);
        }
        else if (lab <= 61)
        {
            // Accepted beside Leftovers / Black Sludge, every ability and move simulated (strict fixtures).
            EXPECT(r.built);
            EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_ACCEPTED);
            EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
            EXPECT_NE(now, before);
            EXPECT_EQ(old.itemState, FGC_ITEMS_OFF_ITEM);
            if (r.request != NULL)
            {
                EXPECT(r.request->rules & RULE_STRICT_ABILITIES);
                EXPECT(r.request->rules & RULE_STRICT_MOVES);
                for (u32 side = 0; side < 2; side++)
                {
                    const BattleEntry *e = side ? r.request->opponent : r.request->player;
                    for (u32 i = 0; i < (side ? r.request->opponent_count : r.request->player_count); i++)
                    {
                        const enum HoldEffect hold = GetItemHoldEffect(e[i].record.held_item);
                        EXPECT_NE(e[i].mon.ability.cls, BACLASS_UNSUPPORTED);
                        for (u32 m = 0; m < MAX_MON_MOVES; m++)
                            if (e[i].record.moves[m] != MOVE_NONE)
                                EXPECT_EQ(e[i].mon.moves[m].unsupported, 0);
                        EXPECT_EQ(e[i].mon.item.source, e[i].record.held_item);
                        if (hold == HOLD_EFFECT_Z_CRYSTAL || hold == HOLD_EFFECT_MEGA_STONE)
                            EXPECT_EQ(e[i].mon.item.cls, BICLASS_NONE);
                        else
                            EXPECT_EQ(e[i].mon.item.cls, BICLASS_RECORDS);
                    }
                }
            }
        }
        else if (lab == 62)
        {
            EXPECT(r.built);
            EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_ACCEPTED);
            EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
            EXPECT_EQ(r.itemReason, ITEM_LIFE_ORB);
        }
        else if (lab == 63)
        {
            EXPECT(!r.built);
            EXPECT(!old.built);
        }
        else if (lab == 64)
        {
            EXPECT(r.built);
            EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_OFF_FORM);
            EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
            EXPECT_EQ(r.itemReason, ITEM_RED_ORB);
            EXPECT_EQ(now, before);
        }
        TRY_FREE_AND_SET_NULL(r.request);
        TRY_FREE_AND_SET_NULL(old.request);
    }
}

TEST("GC V18 export: debug fixtures V2-V12 are byte for byte as in M4-B1a")
{
    static const u8 rosters[] = {
        BATTLE_APP_VERSION_ROSTER, BATTLE_APP_VERSION_SWITCH, BATTLE_APP_VERSION_STRUGGLE, BATTLE_APP_VERSION_DRAG,
        BATTLE_APP_VERSION_PIVOT, BATTLE_APP_VERSION_CONDITIONS, BATTLE_APP_VERSION_EFFECTS,
        BATTLE_APP_VERSION_VOLATILES, BATTLE_APP_VERSION_TIMING, BATTLE_APP_VERSION_ABILITIES,
        BATTLE_APP_VERSION_ABILITY_HOOKS,
    };
    struct FrontierGcExportResult r, old;
    u32 built = 0;
    for (u32 i = 0; i < ARRAY_COUNT(rosters); i++)
    {
        for (u8 format = BATTLE_FORMAT_SINGLE; format <= BATTLE_FORMAT_ROTATION; format++)
        {
            for (u32 endurance = 0; endurance < 2; endurance++)
            {
                const u32 now = FixtureHash(0, rosters[i], format, endurance, FALSE, &r);
                const u32 before = FixtureHash(0, rosters[i], format, endurance, TRUE, &old);
                EXPECT_EQ(now, before);
                EXPECT_EQ(r.built, old.built);
                built += r.built;
                TRY_FREE_AND_SET_NULL(r.request);
                TRY_FREE_AND_SET_NULL(old.request);
            }
        }
    }
    Test_MgbaPrintf("FIXTURE debug configurations built=%d", built);
    EXPECT_GT(built, 0);
}
