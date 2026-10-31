#include "global.h"
#include "battle.h"
#include "battle_factory.h"
#include "battle_frontier.h"
#include "battle_setup.h"
#include "battle_tower.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "frontier_guest.h"
#include "frontier_music.h"
#include "frontier_util.h"
#include "item.h"
#include "overworld.h"
#include "palette.h"
#include "pokemon.h"
#include "random.h"
#include "string_util.h"
#include "sprite.h"
#include "text.h"
#include "test/test.h"
#include "constants/battle_factory.h"
#include "constants/battle_frontier.h"
#include "constants/battle_frontier_mons.h"
#include "constants/battle_palace.h"
#include "constants/battle_tower.h"
#include "constants/event_objects.h"
#include "constants/frontier_util.h"
#include "constants/maps.h"
#include "constants/map_groups.h"
#include "constants/songs.h"

void CallBattlePalaceFunction(void);

static void SetUpGuests(u32 facility, u32 mode)
{
    VarSet(VAR_FRONTIER_FACILITY, facility);
    VarSet(VAR_FRONTIER_BATTLE_MODE, mode);
    gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_50;
    gSaveBlock2Ptr->frontier.curChallengeBattleNum = 6;
    memset(gSaveBlock2Ptr->frontier.trainerIds, 0xFF, sizeof(gSaveBlock2Ptr->frontier.trainerIds));
    memset(gSaveBlock2Ptr->frontier.towerWinStreaks, 0, sizeof(gSaveBlock2Ptr->frontier.towerWinStreaks));
    memset(gSaveBlock2Ptr->frontier.palaceWinStreaks, 0, sizeof(gSaveBlock2Ptr->frontier.palaceWinStreaks));
    memset(gSaveBlock2Ptr->frontier.arenaWinStreaks, 0, sizeof(gSaveBlock2Ptr->frontier.arenaWinStreaks));
    memset(gSaveBlock2Ptr->frontier.factoryWinStreaks, 0, sizeof(gSaveBlock2Ptr->frontier.factoryWinStreaks));
    for (u32 i = 0; i < NUM_FRONTIER_FACILITIES; i++)
    {
        FlagClear(gFrontierBrainInfo[i].silverSymbolFlag);
        FlagClear(gFrontierBrainInfo[i].goldSymbolFlag);
    }
    FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    gFacilityTrainers = gBattleFrontierTrainers;
    gFacilityTrainerMons = gBattleFrontierMons;
    TRAINER_BATTLE_PARAM.opponentB = 0;
    SeedRng(12345);
}

static bool32 HasType(enum Species species, enum Type type)
{
    return gSpeciesInfo[species].types[0] == type || gSpeciesInfo[species].types[1] == type;
}

static void CheckParty(u32 trainer, u32 count, enum Type type)
{
    u32 mega = 0, z = 0, banned = 0;
    for (u32 i = 0; i < count; i++)
    {
        struct Pokemon *mon = &gParties[trainer][i];
        enum Species species = GetMonData(mon, MON_DATA_SPECIES);
        enum Item item = GetMonData(mon, MON_DATA_HELD_ITEM);
        EXPECT_NE(species, SPECIES_NONE);
        if (i < 2)
            EXPECT(HasType(species, type));
        EXPECT(!IsFrontierSpeciesBannedByCurrentRules(species));
        mega += gItemsInfo[item].sortType == ITEM_TYPE_MEGA_STONE;
        z += gItemsInfo[item].sortType == ITEM_TYPE_Z_CRYSTAL;
        banned += IsFrontierSpeciesSubjectToBannedLimit(species);
        for (u32 j = 0; j < i; j++)
        {
            EXPECT_NE(species, GetMonData(&gParties[trainer][j], MON_DATA_SPECIES));
            if (item != ITEM_NONE)
                EXPECT_NE(item, GetMonData(&gParties[trainer][j], MON_DATA_HELD_ITEM));
        }
    }
    EXPECT(mega <= 1 && z <= 1 && banned <= 1);
}

TEST("Frontier guest: roster is separate from the original saved trainer IDs")
{
    EXPECT_EQ(FRONTIER_TRAINERS_COUNT, 300);
    EXPECT_EQ(TRAINER_FRONTIER_GUEST_FIRST, TRAINER_PLAYER + 1);
    EXPECT_EQ(FRONTIER_GUEST_RANDOM_COUNT, 26);
    EXPECT_EQ(FRONTIER_GUEST_COUNT, 28);
    EXPECT_EQ(GetFrontierGuestTrainer(TRAINER_PLAYER), NULL);
    EXPECT_EQ(GetFrontierGuestTrainer(TRAINER_FRONTIER_GUEST_END), NULL);
    EXPECT_EQ(GetFrontierGuestTrainer(TRAINER_FRONTIER_BRAIN), NULL);
    EXPECT_EQ(GetFrontierGuestTrainer(TRAINER_FRONTIER_VITO)->objectGfx, OBJ_EVENT_GFX_MAN_3);
    EXPECT_EQ(GetFrontierGuestTrainer(TRAINER_FRONTIER_VITO)->portrait, TRAINER_PIC_COOLTRAINER_M);
    EXPECT_EQ(GetFrontierGuestTrainer(TRAINER_FRONTIER_COURTNEY)->portrait, TRAINER_PIC_MAGMA_GRUNT_F);
    EXPECT_EQ(GetFrontierGuestTrainer(TRAINER_FRONTIER_TATE_AND_LIZA)->portrait, TRAINER_PIC_LEADER_TATE_AND_LIZA);
    u32 types = 0;
    for (u32 id = TRAINER_FRONTIER_GUEST_FIRST; id < TRAINER_FRONTIER_TATE; id++)
        types |= 1 << GetFrontierGuestTrainer(id)->type;
    EXPECT_EQ(__builtin_popcount(types), 18);
    for (u32 id = TRAINER_FRONTIER_VICTOR; id <= TRAINER_FRONTIER_VITO; id++)
        EXPECT_EQ(GetFrontierGuestTrainer(id)->type, TYPE_GRASS);
}

TEST("Frontier guest: all identity and dialogue paths accept guest IDs including replays")
{
    u32 id = TRAINER_FRONTIER_GUEST_FIRST;
    for (u32 i = TRAINER_FRONTIER_GUEST_FIRST; i < TRAINER_FRONTIER_GUEST_END; i++)
        PARAMETRIZE { id = i; }
    SetUpGuests(FRONTIER_FACILITY_TOWER, FRONTIER_MODE_SINGLES);
    const struct FrontierGuestTrainer *guest = GetFrontierGuestTrainer(id);
    u8 name[PLAYER_NAME_LENGTH + 1], language;
    for (u32 replay = 0; replay < 2; replay++)
    {
        if (replay)
            gBattleTypeFlags |= BATTLE_TYPE_RECORDED;
        GetFrontierTrainerName(name, id);
        EXPECT_EQ(StringCompare(name, guest->name), 0);
        EXPECT(StringLength(name) <= PLAYER_NAME_LENGTH);
        EXPECT_EQ(IsFrontierTrainerFemale(id), guest->female);
        EXPECT_EQ(GetFrontierOpponentClass(id), guest->trainerClass);
        EXPECT_EQ(GetFrontierTrainerFrontSpriteId(id), guest->portrait);
        EXPECT_EQ(GetBattleFacilityTrainerGfxId(id), guest->objectGfx);
        EXPECT_EQ(GetFrontierTrainerFacilityClass(id), guest->female ? FACILITY_CLASS_COOLTRAINER_F : FACILITY_CLASS_COOLTRAINER_M);
        GetBattleTowerTrainerLanguage(&language, id);
        EXPECT_EQ(language, gGameLanguage);
        for (u32 text = 0; text < 3; text++)
        {
            CopyFrontierTrainerText(text, id);
            EXPECT(StringLength(gStringVar4) > 0);
            EXPECT(GetStringWidth(FONT_NORMAL, gStringVar4, -1) <= 204);
        }
    }
    SetBattleFacilityTrainerGfxId(id, 0);
    SetBattleFacilityTrainerGfxId(id, 1);
    SetBattleFacilityTrainerGfxId(id, 15);
    EXPECT_EQ(VarGet(VAR_OBJ_GFX_ID_0), guest->objectGfx);
    EXPECT_EQ(VarGet(VAR_OBJ_GFX_ID_1), guest->objectGfx);
    EXPECT_EQ(VarGet(VAR_OBJ_GFX_ID_E), guest->objectGfx);
}

TEST("Frontier guest: every Arena opponent has visible overworld frames and a loaded palette")
{
    u32 id = TRAINER_FRONTIER_GUEST_FIRST;
    for (u32 i = TRAINER_FRONTIER_GUEST_FIRST; i < TRAINER_FRONTIER_GUEST_END; i++)
        PARAMETRIZE { id = i; }
    SetUpGuests(FRONTIER_FACILITY_ARENA, FRONTIER_MODE_SINGLES);
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_ARENA_BATTLE_ROOM);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_ARENA_BATTLE_ROOM);
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum);
    InitMap();
    memset(gObjectEvents, 0, sizeof(gObjectEvents));
    ResetSpriteData();
    FreeAndReserveObjectSpritePalettes();
    SetBattleFacilityTrainerGfxId(id, 0);
    const struct ObjectEventGraphicsInfo *info = GetObjectEventGraphicsInfo(GetFrontierGuestTrainer(id)->objectGfx);
    EXPECT(info != NULL);
    EXPECT_EQ(GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_VAR_0), info);
    const struct ObjectEventTemplate *template = &gMapHeader.events->objectEvents[LOCALID_ARENA_BATTLE_OPPONENT - 1];
    EXPECT_EQ(template->graphicsId, OBJ_EVENT_GFX_VAR_0);
    u8 objectId = TrySpawnObjectEventTemplate(template, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, 0, 0);
    EXPECT(objectId < OBJECT_EVENTS_COUNT);
    struct ObjectEvent *object = &gObjectEvents[objectId];
    EXPECT_EQ(object->graphicsId, GetFrontierGuestTrainer(id)->objectGfx);
    EXPECT(object->spriteId < MAX_SPRITES);
    struct Sprite *sprite = &gSprites[object->spriteId];
    EXPECT(sprite->inUse);
    EXPECT_EQ(sprite->images, info->images);
    EXPECT_EQ((u32)sprite->oam.paletteNum, IndexOfSpritePaletteTag(info->paletteTag));
    for (u32 dir = DIR_SOUTH; dir <= DIR_EAST; dir++)
    {
        for (u32 walking = 0; walking < 2; walking++)
        {
            u32 anim = walking ? GetMoveDirectionAnimNum(dir) : GetFaceDirectionAnimNum(dir);
            StartSpriteAnim(sprite, anim);
            for (u32 tick = 0; tick < 24; tick++)
            {
                AnimateSprite(sprite);
                u32 frame = info->anims[anim][sprite->animCmdIndex].frame.imageValue;
                const u8 *pixels = info->images[0].relativeFrames ? info->images[0].data + frame * info->images[0].size : info->images[frame].data;
                u32 visible = 0;
                for (u32 i = 0; i < info->size; i++)
                    visible += pixels[i] != 0;
                EXPECT(visible > 0);
                BuildOamBuffer();
                ProcessSpriteCopyRequests();
                EXPECT_EQ(memcmp((const void *)(OBJ_VRAM0 + TILE_SIZE_4BPP * sprite->oam.tileNum), pixels, info->size), 0);
            }
        }
    }
    RemoveObjectEvent(object);
}

TEST("Frontier guest: only the final stage in seven-battle facilities is eligible")
{
    for (u32 facility = 0; facility < NUM_FRONTIER_FACILITIES; facility++)
    {
        SetUpGuests(facility, FRONTIER_MODE_SINGLES);
        bool32 eligible = facility == FRONTIER_FACILITY_TOWER || facility == FRONTIER_FACILITY_PALACE
            || facility == FRONTIER_FACILITY_ARENA || facility == FRONTIER_FACILITY_FACTORY;
        for (u32 stage = 0; stage < FRONTIER_STAGES_PER_CHALLENGE; stage++)
            EXPECT_EQ(IsFrontierGuestBattle(stage), eligible && stage == 6);
        gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_TENT;
        EXPECT(!IsFrontierGuestBattle(6));
    }
}

TEST("Frontier guest: Brain milestones take priority")
{
    SetUpGuests(FRONTIER_FACILITY_TOWER, FRONTIER_MODE_SINGLES);
    gSaveBlock2Ptr->frontier.towerWinStreaks[FRONTIER_MODE_SINGLES][FRONTIER_LVL_50] = 34;
    EXPECT_EQ(GetFrontierBrainStatus(), FRONTIER_BRAIN_SILVER);
    EXPECT(!IsFrontierGuestBattle(6));
    gSaveBlock2Ptr->frontier.towerWinStreaks[FRONTIER_MODE_SINGLES][FRONTIER_LVL_50] = 69;
    FlagSet(gFrontierBrainInfo[FRONTIER_FACILITY_TOWER].silverSymbolFlag);
    EXPECT_EQ(GetFrontierBrainStatus(), FRONTIER_BRAIN_GOLD);
    EXPECT(!IsFrontierGuestBattle(6));
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    EXPECT_EQ(FrontierMusic_ChooseBattleBGM(MUS_VS_TRAINER), MUS_VS_FRONTIER_BRAIN);
}

TEST("Frontier guest: Tower Palace and Arena use the guest selector at stage seven")
{
    u32 facility = FRONTIER_FACILITY_TOWER, mode = FRONTIER_MODE_SINGLES;
    PARAMETRIZE { facility = FRONTIER_FACILITY_TOWER; mode = FRONTIER_MODE_SINGLES; }
    PARAMETRIZE { facility = FRONTIER_FACILITY_TOWER; mode = FRONTIER_MODE_DOUBLES; }
    PARAMETRIZE { facility = FRONTIER_FACILITY_PALACE; mode = FRONTIER_MODE_SINGLES; }
    PARAMETRIZE { facility = FRONTIER_FACILITY_PALACE; mode = FRONTIER_MODE_DOUBLES; }
    PARAMETRIZE { facility = FRONTIER_FACILITY_ARENA; mode = FRONTIER_MODE_SINGLES; }
    SetUpGuests(facility, mode);
    gSpecialVar_0x8004 = BATTLE_TOWER_FUNC_SET_OPPONENT;
    CallBattleTowerFunc();
    u16 id = TRAINER_BATTLE_PARAM.opponentA;
    EXPECT(GetFrontierGuestTrainer(id) != NULL);
    EXPECT_EQ(VarGet(VAR_OBJ_GFX_ID_0), GetFrontierGuestTrainer(id)->objectGfx);
    gSpecialVar_0x8005 = 0;
    gSpecialVar_0x8004 = BATTLE_TOWER_FUNC_GET_OPPONENT_INTRO;
    CallBattleTowerFunc();
    EXPECT(StringLength(gStringVar4) > 0);
}

TEST("Frontier guest: random selection never selects an unpaired twin")
{
    bool8 seen[FRONTIER_GUEST_RANDOM_COUNT] = {0};
    SeedRng(12345);
    for (u32 i = 0; i < 4096; i++)
    {
        u16 id = GetRandomFrontierGuestTrainer();
        EXPECT(id >= TRAINER_FRONTIER_GUEST_FIRST && id < TRAINER_FRONTIER_TATE);
        seen[id - TRAINER_FRONTIER_GUEST_FIRST] = TRUE;
    }
    for (u32 i = 0; i < ARRAY_COUNT(seen); i++)
        EXPECT(seen[i]);
}

TEST("Frontier guest: NPC and link multi pairs stay in the final two slots and keep twins together")
{
    u32 mode = FRONTIER_MODE_MULTIS;
    PARAMETRIZE { mode = FRONTIER_MODE_MULTIS; }
    PARAMETRIZE { mode = FRONTIER_MODE_LINK_MULTIS; }
    SetUpGuests(FRONTIER_FACILITY_TOWER, mode);
    u16 *ids = gSaveBlock2Ptr->frontier.trainerIds;
    for (u32 i = 0; i < ARRAY_COUNT(gSaveBlock2Ptr->frontier.trainerIds); i++)
        ids[i] = i;
    bool32 sawTwins = FALSE;
    for (u32 i = 0; i < 128; i++)
    {
        SetFrontierGuestMultiPair(ids);
        EXPECT(GetFrontierGuestTrainer(ids[12]) != NULL);
        EXPECT(GetFrontierGuestTrainer(ids[13]) != NULL);
        EXPECT_NE(ids[12], ids[13]);
        EXPECT_NE(ids[12], TRAINER_FRONTIER_TATE_AND_LIZA);
        EXPECT_NE(ids[13], TRAINER_FRONTIER_TATE_AND_LIZA);
        if (ids[12] == TRAINER_FRONTIER_TATE || ids[13] == TRAINER_FRONTIER_LIZA)
        {
            EXPECT_EQ(ids[12], TRAINER_FRONTIER_TATE);
            EXPECT_EQ(ids[13], TRAINER_FRONTIER_LIZA);
            sawTwins = TRUE;
        }
        for (u32 slot = 0; slot < ARRAY_COUNT(gSaveBlock2Ptr->frontier.trainerIds); slot++)
            if (slot != 12 && slot != 13)
                EXPECT_EQ(ids[slot], slot);
    }
    EXPECT(sawTwins);
    TRAINER_BATTLE_PARAM.opponentA = 0;
    gSpecialVar_0x8004 = BATTLE_TOWER_FUNC_SET_OPPONENT;
    CallBattleTowerFunc();
    EXPECT_EQ(TRAINER_BATTLE_PARAM.opponentA, ids[12]);
    EXPECT_EQ(TRAINER_BATTLE_PARAM.opponentB, ids[13]);
}

TEST("Frontier guest: every guest has two typed mons while retaining party clauses in singles doubles and multi")
{
    u32 id = TRAINER_FRONTIER_GUEST_FIRST, count = 3, banned = FALSE;
    for (u32 i = TRAINER_FRONTIER_GUEST_FIRST; i < TRAINER_FRONTIER_TATE; i++)
        for (u32 c = 2; c <= 4; c++)
            for (u32 b = 0; b < 2; b++)
                PARAMETRIZE { id = i; count = c; banned = b; }
    SetUpGuests(FRONTIER_FACILITY_TOWER, count == 4 ? FRONTIER_MODE_DOUBLES : FRONTIER_MODE_SINGLES);
    SeedRng(id * 7919 + count * 311 + banned);
    if (banned)
        FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    TRAINER_BATTLE_PARAM.opponentA = id;
    FillFrontierTrainerParty(count);
    CheckParty(B_TRAINER_OPPONENT_A, count, GetFrontierGuestTrainer(id)->type);
    EXPECT_EQ(GetFrontierGuestMonFilter(id, 2), FRONTIER_MON_FILTER_NONE);
}

TEST("Frontier guest: both sides of a multi guest pair receive their own typed party")
{
    SetUpGuests(FRONTIER_FACILITY_TOWER, FRONTIER_MODE_MULTIS);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_TATE;
    TRAINER_BATTLE_PARAM.opponentB = TRAINER_FRONTIER_LIZA;
    FillFrontierTrainersParties(FRONTIER_MULTI_PARTY_SIZE);
    CheckParty(B_TRAINER_OPPONENT_A, 2, TYPE_PSYCHIC);
    CheckParty(B_TRAINER_OPPONENT_B, 2, TYPE_PSYCHIC);
}

TEST("Frontier guest: Factory generation applies types before hints and excludes all rental species")
{
    u32 seed = 0;
    for (u32 i = 0; i < 32; i++)
        PARAMETRIZE { seed = i; }
    SetUpGuests(FRONTIER_FACILITY_FACTORY, FRONTIER_MODE_SINGLES);
    SeedRng(seed * 7919);
    for (u32 i = 0; i < ARRAY_COUNT(gSaveBlock2Ptr->frontier.rentalMons); i++)
        gSaveBlock2Ptr->frontier.rentalMons[i].monId = i * 3;
    gSpecialVar_0x8004 = BATTLE_FACTORY_FUNC_GENERATE_OPPONENT_MONS;
    CallBattleFactoryFunction();
    const struct FrontierGuestTrainer *guest = GetFrontierGuestTrainer(TRAINER_BATTLE_PARAM.opponentA);
    EXPECT(guest != NULL);
    for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
    {
        enum Species species = gBattleFrontierMons[gFrontierTempParty[i]].species;
        if (i < 2)
            EXPECT(HasType(species, guest->type));
        for (u32 r = 0; r < ARRAY_COUNT(gSaveBlock2Ptr->frontier.rentalMons); r++)
            EXPECT_NE(species, gBattleFrontierMons[gSaveBlock2Ptr->frontier.rentalMons[r].monId].species);
    }
    gSpecialVar_0x8004 = BATTLE_PALACE_FUNC_GET_OPPONENT_INTRO;
    CallBattlePalaceFunction();
    EXPECT(StringLength(gStringVar4) > 0);
}

TEST("Frontier guests: Tate and Liza as two trainers each show one twin, not the pair picture")
{
    EXPECT_EQ(GetFrontierGuestTrainer(TRAINER_FRONTIER_TATE)->portrait, TRAINER_PIC_LEADER_TATE);
    EXPECT_EQ(GetFrontierGuestTrainer(TRAINER_FRONTIER_LIZA)->portrait, TRAINER_PIC_LEADER_LIZA);
    EXPECT_EQ(GetFrontierGuestTrainer(TRAINER_FRONTIER_TATE_AND_LIZA)->portrait, TRAINER_PIC_LEADER_TATE_AND_LIZA);
}
