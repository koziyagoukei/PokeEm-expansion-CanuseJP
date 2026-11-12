// Frontier content: Single and multi-format GC Battle tests (GC_MODE_BATTLE).
//
// Started from the debug menu. Builds a GcBattleRequestV1 from the player's
// lead Pokemon and the lead of a trainer's party, resolved from this game's
// own move/species/type data, and runs it through the generic GC Extension
// connector. The GameCube owns the temporary battle state; the GBA is the
// command input and second screen. The result is shown; with the "apply"
// variant, HP and PP of the lead are written back once, after the link is
// closed and the result was validated. Nothing else is changed.
#include "global.h"
#include "battle_main.h"
#include "battle_setup.h"
#include "data.h"
#include "event_data.h"
#include "main.h"
#include "malloc.h"
#include "move.h"
#include "pokemon.h"
#include "random.h"
#include "sound.h"
#include "string_util.h"
#include "constants/battle.h"
#include "constants/characters.h"
#include "constants/songs.h"
#include "gc_extension.h"
#include "gc_ext/battle_protocol.h"
#include "frontier_gc_battle.h"

#define REQUEST_CAPACITY (BATTLE_REQUEST_HEADER + PARTY_SIZE * BATTLE_ENTRY_BYTES + NUMBER_OF_MON_TYPES * NUMBER_OF_MON_TYPES)

enum GbaScreen { SCREEN_PREPARE, SCREEN_COMMAND, SCREEN_MOVES, SCREEN_TARGET, SCREEN_REPLACE, SCREEN_SWITCH, SCREEN_WAIT, SCREEN_ENDED };

struct FrontierGcBattle
{
    struct Pokemon opponent[PARTY_SIZE];
    u8 request[REQUEST_CAPACITY];
    u32 requestLength;
    u32 battleId;
    u32 leadPersonality;
    u16 trainerId;
    u8 environment;
    u8 leadSlot;
    u8 policy;
    u8 format, count, fieldCount;
    u8 roster; // zero for the legacy app, otherwise the roster app version
    u8 rotation, target;
    BattleMultiState multiState;
    u8 reserveMask;
    BattleMultiAction multiAction;
    u8 screen;
    u8 cursor;
    u8 supportedMask;
    BattleState state;
    bool8 haveState;
    BattleAction pending;
    bool8 resultAccepted;
    bool8 applied;
    BattleResult result;
    u8 message[48];
};

static EWRAM_DATA struct FrontierGcBattle *sBattle = NULL;
static EWRAM_DATA u8 sPendingPolicy = 0;
static EWRAM_DATA u16 sPendingTrainer = 0;
static EWRAM_DATA u8 sPendingFormat = 0;
static EWRAM_DATA u8 sPendingRoster = 0;

static const u8 sText_NoMemory[] = _("{ENG}Not enough memory.");
static const u8 sText_NoLead[] = _("{ENG}No able Pokemon with a supported move.");
static const u8 sText_NoOpponent[] = _("{ENG}The trainer has no Pokemon.");
static const u8 sText_NoMoves[] = _("{ENG}Lead has no supported move.");
static const u8 sText_NoOpponentMoves[] = _("{ENG}Opponent has no supported move.");
static const u8 sText_Prepare[] = _("{ENG}GC Battle: waiting for the GameCube");
static const u8 sText_Fight[] = _("たたかう");
static const u8 sText_Cursor[] = _("{ENG}>");
static const u8 sText_Unsupported[] = _("{ENG}(unsupported)");
static const u8 sText_Wait[] = _("{ENG}GameCube is resolving the turn...");
static const u8 sText_Win[] = _("{ENG}WIN!");
static const u8 sText_Draw[] = _("{ENG}Draw.");
static const u8 sText_Lose[] = _("{ENG}LOSE...");
static const u8 sText_Cancelled[] = _("{ENG}Battle cancelled.");
static const u8 sText_Refused[] = _("{ENG}Battle not supported.");
static const u8 sText_HelpCommand[] = _("{ENG}A: Fight   B: Forfeit");
static const u8 sText_HelpMoves[] = _("{ENG}A: Use move   B: Back");
static const u8 sText_HelpEnded[] = _("{ENG}A: Return");
static const u8 sText_Vs[] = _("{ENG}VS ");
static const u8 sText_Lv[] = _("{ENG} Lv.");
static const u8 sText_Hp[] = _("{ENG}HP ");
static const u8 sText_Pp[] = _("{ENG}PP ");
static const u8 sText_Slash[] = _("{ENG}/");
static const u8 sText_Jpn[] = _("{JPN}");
static const u8 sText_Eng[] = _("{ENG}");
static const u8 sText_NotApplied[] = _("{ENG}Result shown only; party unchanged.");
static const u8 sText_NoResult[] = _("{ENG}No battle result; party unchanged.");
static const u8 sText_AppliedHp[] = _("{ENG}Applied to lead: HP ");
static const u8 sText_ApplyRefused[] = _("{ENG}Result not applied (lead changed).");

// ------------------------------------------------------------- request

static u8 TypeQuarters(enum Type attack, enum Type defend)
{
    return (gTypeEffectivenessTable[attack][defend] + 512) >> 10;
}

static void ResolveMove(enum Move move, BattleMove *out)
{
    memset(out, 0, sizeof(*out));
    if (move == MOVE_NONE || move >= MOVES_COUNT_ALL)
    {
        out->unsupported = UNSUP_EMPTY;
        return;
    }
    out->power = GetMovePower(move);
    out->type = GetMoveType(move);
    switch (GetMoveCategory(move))
    {
    case DAMAGE_CATEGORY_PHYSICAL: out->category = BCAT_PHYSICAL; break;
    case DAMAGE_CATEGORY_SPECIAL:  out->category = BCAT_SPECIAL; break;
    default:                       out->category = BCAT_STATUS; break;
    }
    out->accuracy = GetMoveAccuracy(move);
    out->priority = GetMovePriority(move);
    switch (GetMoveTarget(move))
    {
    case TARGET_SELECTED:      out->target = BTARGET_SINGLE; break;
    case TARGET_BOTH:          out->target = BTARGET_ALL_FOES; break;
    case TARGET_FOES_AND_ALLY:
        out->target = sBattle->format == BATTLE_FORMAT_SINGLE ? BTARGET_ALL_FOES : BTARGET_ALL_ADJACENT;
        break;
    case TARGET_USER:          out->target = BTARGET_SELF; break;
    default:                   out->target = BTARGET_OTHER; break;
    }
    // Move flags are resolved here so the host can remain game-independent.
    if (sBattle->format == BATTLE_FORMAT_TRIPLE && move == MOVE_WING_ATTACK)
        out->target |= BTARGET_IGNORE_DISTANCE;
    if (GetMoveEffect(move) != EFFECT_HIT)
        out->unsupported |= UNSUP_EFFECT;
    if (GetMoveAdditionalEffectCount(move) != 0)
        out->unsupported |= UNSUP_SECONDARY;
    if (out->priority != 0)
        out->unsupported |= UNSUP_PRIORITY;
    if ((out->target & BTARGET_CLASS_MASK) == BTARGET_OTHER ||
        (sBattle->format == BATTLE_FORMAT_SINGLE && out->target == BTARGET_SELF))
        out->unsupported |= UNSUP_TARGET;
    if (out->category == BCAT_STATUS || out->power == 0)
        out->unsupported |= UNSUP_STATUS_MOVE;
    if (GetMoveStrikeCount(move) > 1 || gMovesInfo[move].multiHit)
        out->unsupported |= UNSUP_MULTI_HIT;
    if (IsExplosionMove(move) || gMovesInfo[move].alwaysCriticalHit || gMovesInfo[move].cantUseTwice
     || gMovesInfo[move].noAffectOnSameTypeTarget || gMovesInfo[move].accDecreaseIfUserNotSameType
     || out->accuracy > 100 || out->type >= NUMBER_OF_MON_TYPES)
        out->unsupported |= UNSUP_OTHER;
}

static u8 FillEntry(u32 slot, BattleEntry *entry)
{
    struct Pokemon *mon = GcExtension_GetExportMon(slot);
    enum Species species;
    u32 i;
    u8 mask = 0;

    GcExtension_BuildRecord(slot, &entry->record);
    species = SanitizeSpeciesId(GetMonData(mon, MON_DATA_SPECIES));
    entry->mon.types[0] = gSpeciesInfo[species].types[0];
    entry->mon.types[1] = gSpeciesInfo[species].types[1];
    entry->mon.type_count = entry->mon.types[0] == entry->mon.types[1] ? 1 : 2;
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        ResolveMove(entry->record.moves[i], &entry->mon.moves[i]);
        if (entry->mon.moves[i].unsupported == 0 && entry->record.pp[i] > 0)
            mask |= 1u << i;
    }
    return mask;
}

// Test rule: the player's battler is the first party member that can battle
// and knows at least one move the GC battle core supports.
static s32 FindLead(BattleEntry *scratch)
{
    u32 i;
    for (i = 0; i < gPartiesCount[B_TRAINER_PLAYER]; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(mon, MON_DATA_IS_EGG)
         && GetMonData(mon, MON_DATA_HP) != 0 && FillEntry(i, scratch) != 0)
            return i;
    }
    return -1;
}

// Mode-owned fixtures use the same export path as a normal party. They
// never enter gPlayerParty or the save. Slot IDs remain stable as they move.
static bool8 BuildFixtureEntries(BattleRequest *request, const u8 **reason)
{
    static const struct BattleFixture
    {
        enum Species species;
        enum Move move1, move2;
    } player[] = {
        {SPECIES_TAILLOW, MOVE_WING_ATTACK, MOVE_TACKLE},
        {SPECIES_MARSHTOMP, MOVE_WATER_GUN, MOVE_SURF},
        {SPECIES_GROVYLE, MOVE_STRENGTH, MOVE_ABSORB},
    }, foe[] = {
        {SPECIES_MAKUHITA, MOVE_TACKLE, MOVE_POUND},
        {SPECIES_GEODUDE, MOVE_ROCK_THROW, MOVE_TACKLE},
        {SPECIES_NUMEL, MOVE_TACKLE, MOVE_EMBER},
    };
    u32 side, i;
    const u8 foeLevel = sBattle->trainerId == TRAINER_STEVEN ? 45 : 13;
    GcExtension_SetExtraMons(sBattle->opponent, 2 * sBattle->count);
    for (side = 0; side < 2; side++)
    {
        BattleEntry *entries = side ? request->opponent : request->player;
        for (i = 0; i < sBattle->count; i++)
        {
            u32 index = side * sBattle->count + i;
            const struct BattleFixture *fixture = side ? &foe[i] : &player[i];
            struct Pokemon *mon = &sBattle->opponent[index];
            CreateMonWithIVs(mon, fixture->species, side ? foeLevel : 23, 0x48210000 + index, OTID_STRUCT_PLAYER_ID, 15);
            SetMonMoveSlot(mon, fixture->move1, 0);
            SetMonMoveSlot(mon, fixture->move2, 1);
            SetMonMoveSlot(mon, MOVE_NONE, 2);
            SetMonMoveSlot(mon, MOVE_NONE, 3);
            if (sBattle->roster && side == 0 && i == 0)
            {
                u16 hp = 1;
                SetMonData(mon, MON_DATA_HP, &hp);
            }
            if (FillEntry(GC_EXTRA_SLOT_BASE + index, &entries[i]) == 0)
            {
                *reason = side ? sText_NoOpponentMoves : sText_NoMoves;
                return FALSE;
            }
        }
    }
    return TRUE;
}

static bool8 BuildSingleEntries(BattleRequest *request, const struct Trainer *trainer, const u8 **reason)
{
    s32 lead = FindLead(&request->player[0]);
    if (lead < 0)
    {
        *reason = sText_NoLead;
        return FALSE;
    }
    CreateNPCTrainerPartyFromTrainer(sBattle->opponent, trainer);
    if (GetMonData(&sBattle->opponent[0], MON_DATA_SPECIES) == SPECIES_NONE)
    {
        *reason = sText_NoOpponent;
        return FALSE;
    }
    GcExtension_SetExtraMons(sBattle->opponent, 1);
    sBattle->leadSlot = lead;
    sBattle->leadPersonality = GetMonData(&gParties[B_TRAINER_PLAYER][lead], MON_DATA_PERSONALITY);
    sBattle->supportedMask = FillEntry(lead, &request->player[0]);
    if (FillEntry(GC_EXTRA_SLOT_BASE, &request->opponent[0]) == 0)
    {
        *reason = sText_NoOpponentMoves;
        return FALSE;
    }
    return TRUE;
}

static bool8 BuildRequest(const u8 **reason)
{
    BattleRequest *request = AllocZeroedUnchecked(sizeof(*request));
    const struct Trainer *trainer = GetTrainerStructFromId(sBattle->trainerId);
    u32 a, d;

    if (request == NULL)
    {
        *reason = sText_NoMemory;
        return FALSE;
    }
    if (!(sBattle->format == BATTLE_FORMAT_SINGLE ? BuildSingleEntries(request, trainer, reason)
                                                  : BuildFixtureEntries(request, reason)))
    {
        Free(request);
        return FALSE;
    }
    sBattle->battleId = Random32() | 1;
    request->format = sBattle->format;
    request->battle_type = BTYPE_TRAINER;
    request->player_count = sBattle->count;
    request->opponent_count = sBattle->count;
    request->active_per_side = sBattle->fieldCount;
    request->version = sBattle->roster ? sBattle->roster : BATTLE_REQUEST_VERSION;
    request->rules = RULE_IGNORE_ABILITIES | RULE_IGNORE_ITEMS | RULE_IGNORE_STATUS | RULE_NO_GIMMICKS
                   | RULE_NO_CRITICAL_HITS | RULE_SUPPORTED_MOVES_ONLY;
    request->trainer_id = sBattle->trainerId;
    request->trainer_class = trainer->trainerClass;
    request->reward_mode = 0;
    request->result_policy = sBattle->policy;
    request->type_count = NUMBER_OF_MON_TYPES;
    request->battle_id = sBattle->battleId;
    request->environment = sBattle->environment;
    for (a = 0; a < NUMBER_OF_MON_TYPES; a++)
        for (d = 0; d < NUMBER_OF_MON_TYPES; d++)
            request->type_chart[a * NUMBER_OF_MON_TYPES + d] = TypeQuarters(a, d);
    sBattle->requestLength = battle_request_encode(request, sBattle->request, sizeof(sBattle->request));
    Free(request);
    if (sBattle->requestLength == 0)
    {
        *reason = sText_NoMemory;
        return FALSE;
    }
    return TRUE;
}

// ------------------------------------------------------------- handler

static void FreeBattle(void)
{
    FrontierGcBattle_FreeGfx();
    GcExtension_SetExtraMons(NULL, 0);
    TRY_FREE_AND_SET_NULL(sBattle);
}

static bool8 Init(const u8 **reason)
{
    sBattle = AllocZeroedUnchecked(sizeof(*sBattle));
    if (sBattle == NULL)
    {
        *reason = sText_NoMemory;
        return FALSE;
    }
    sBattle->format = sPendingFormat;
    sBattle->fieldCount = sPendingFormat == BATTLE_FORMAT_SINGLE ? 1 : sPendingFormat == BATTLE_FORMAT_DOUBLE ? 2 : 3;
    sBattle->roster = sPendingRoster;
    sBattle->count = sBattle->roster ? 3 : sBattle->fieldCount;
    sBattle->policy = sPendingFormat == BATTLE_FORMAT_SINGLE ? sPendingPolicy : RESULT_POLICY_DISPLAY_ONLY;
    sBattle->trainerId = sPendingTrainer;
    sBattle->environment = BattleSetup_GetEnvironmentId();
    sBattle->screen = SCREEN_PREPARE;
    if (BuildRequest(reason))
        return TRUE;
    FreeBattle();
    return FALSE;
}

static const u8 *GetRequest(u32 *length)
{
    *length = sBattle->requestLength;
    return sBattle->request;
}

static u16 HandleMultiApp(u16 opcode, const u8 *args, u16 argLength, u8 *out, u16 *outLength)
{
    BattleMultiState state;
    BattleMultiAction action = sBattle->multiAction;
    u32 side, pos;
    u8 mask = 0;
    const bool8 roster = sBattle->roster != 0;

    switch (opcode)
    {
    case BATTLE_OP_STATE_ROSTER:
    case BATTLE_OP_STATE_MULTI:
        if (roster)
        {
            BattleRosterState received;
            if (opcode != BATTLE_OP_STATE_ROSTER || !battle_roster_state_decode(args, argLength, &received))
                return APP_BAD_ARGS;
            state = received.field_state;
            mask = received.reserves;
            if (mask >= (1u << sBattle->count)
             || (state.phase == BPHASE_REPLACE && (state.actor >= sBattle->fieldCount
                 || state.field[0][state.actor].party_slot != BATTLE_SLOT_NONE)))
                return APP_BAD_ARGS;
            for (pos = 0; pos < BATTLE_MAX_PER_SIDE; pos++)
                if (received.player[pos].party_slot != (pos < sBattle->count ? GC_EXTRA_SLOT_BASE + pos : BATTLE_SLOT_NONE))
                    return APP_BAD_ARGS;
        }
        else if (opcode != BATTLE_OP_STATE_MULTI || !battle_multi_state_decode(args, argLength, &state))
            return APP_BAD_ARGS;
        for (side = 0; side < 2; side++)
            for (pos = 0; pos < BATTLE_FIELD_SLOTS; pos++)
            {
                u8 slot = state.field[side][pos].party_slot;
                if (slot == BATTLE_SLOT_NONE && (roster || pos >= sBattle->fieldCount || state.outcome == OUTCOME_UNSUPPORTED))
                    continue;
                if (slot < GC_EXTRA_SLOT_BASE + side * sBattle->count
                 || slot >= GC_EXTRA_SLOT_BASE + (side + 1) * sBattle->count)
                    return APP_BAD_ARGS;
            }
        sBattle->multiState = state;
        sBattle->reserveMask = mask;
        sBattle->haveState = TRUE;
        sBattle->multiAction = (BattleMultiAction){.token = state.token, .actor = state.actor};
        sBattle->screen = state.phase == BPHASE_CHOOSE ? SCREEN_COMMAND
                        : state.phase == BPHASE_ENDED ? SCREEN_ENDED
                        : state.phase == BPHASE_REPLACE ? SCREEN_REPLACE : SCREEN_WAIT;
        sBattle->cursor = 0;
        if (state.phase == BPHASE_REPLACE)
            while (!(mask & (1u << sBattle->cursor)))
                sBattle->cursor++;
        sBattle->rotation = BROT_KEEP;
        if (sBattle->format == BATTLE_FORMAT_ROTATION && state.phase == BPHASE_CHOOSE && !state.field[0][BPOS_CENTER].hp)
            sBattle->rotation = state.field[0][BPOS_LEFT].hp ? BROT_LEFT : BROT_RIGHT;
        GcExtension_Redraw();
        return APP_OK;
    case BATTLE_OP_ACTION_ROSTER:
    case BATTLE_OP_ACTION_MULTI:
        if (argLength != 0 || (opcode == BATTLE_OP_ACTION_ROSTER) != roster)
            return APP_BAD_ARGS;
        if (GcExtension_IsExitRequested())
            action.flags |= BACTION_FLAG_EXIT;
        *outLength = battle_multi_action_encode(&action, out);
        return APP_OK;
    }
    return APP_UNKNOWN_OP;
}

static u16 HandleApp(u16 opcode, const u8 *args, u16 argLength, u8 *out, u16 *outLength)
{
    BattleState state;
    BattleAction action = {0};

    if (sBattle->format != BATTLE_FORMAT_SINGLE)
        return HandleMultiApp(opcode, args, argLength, out, outLength);
    switch (opcode)
    {
    case BATTLE_OP_STATE:
        if (!battle_state_decode(args, argLength, &state))
            return APP_BAD_ARGS;
        sBattle->state = state;
        sBattle->haveState = TRUE;
        memset(&sBattle->pending, 0, sizeof(sBattle->pending));
        if (state.phase == BPHASE_CHOOSE)
        {
            sBattle->screen = SCREEN_COMMAND;
            sBattle->cursor = 0;
        }
        else
        {
            sBattle->screen = state.phase == BPHASE_ENDED ? SCREEN_ENDED : SCREEN_WAIT;
        }
        GcExtension_Redraw();
        return APP_OK;
    case BATTLE_OP_ACTION:
        if (sBattle->pending.action != BACTION_NONE)
            action = sBattle->pending;
        else
            action.turn = sBattle->state.turn;
        if (GcExtension_IsExitRequested())
            action.flags |= BACTION_FLAG_EXIT;
        *outLength = battle_action_encode(&action, out);
        return APP_OK;
    }
    return APP_UNKNOWN_OP;
}

static u16 OnResult(u16 version, const u8 *data, u16 length)
{
    BattleResult result;
    u32 i, m;

    if (version != BATTLE_RESULT_VERSION || !battle_result_decode(data, length, &result))
        return RESULT_REJECTED;
    if (result.battle_id != sBattle->battleId || result.player_count != sBattle->count
     || result.opponent_count != sBattle->count)
        return RESULT_REJECTED;
    for (i = 0; i < result.player_count; i++)
    {
        u8 slot = sBattle->format == BATTLE_FORMAT_SINGLE ? sBattle->leadSlot : GC_EXTRA_SLOT_BASE + i;
        struct Pokemon *mon = GcExtension_GetExportMon(slot);
        if (result.player[i].slot != slot || result.player[i].hp > GetMonData(mon, MON_DATA_MAX_HP))
            return RESULT_REJECTED;
        for (m = 0; m < MAX_MON_MOVES; m++)
        {
            u16 move = GetMonData(mon, MON_DATA_MOVE1 + m);
            u8 maxPp = move != MOVE_NONE ? CalculatePPWithBonus(move, GetMonData(mon, MON_DATA_PP_BONUSES), m) : 0;
            if (result.player[i].pp[m] > maxPp)
                return RESULT_REJECTED;
        }
    }
    sBattle->result = result;
    sBattle->resultAccepted = TRUE;
    return RESULT_ACCEPTED;
}

static void AppendNumber(u8 *dest, s32 value)
{
    u8 digits[8];
    ConvertIntToDecimalStringN(digits, value, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(dest, sText_Eng);
    StringAppend(dest, digits);
}

// Runs once, after the link is closed. Only the "apply" policy writes HP/PP,
// only for a validated result, only to the same lead Pokemon, only once.
static const u8 *Finish(bool8 *changedParty)
{
    struct Pokemon *lead = &gParties[B_TRAINER_PLAYER][sBattle->leadSlot];
    u32 i, before;

    if (!sBattle->resultAccepted)
        return sText_NoResult;
    if (sBattle->policy != RESULT_POLICY_APPLY_HP_PP || sBattle->applied
     || sBattle->result.outcome == OUTCOME_CANCELLED || sBattle->result.outcome == OUTCOME_UNSUPPORTED)
        return sText_NotApplied;
    if (GetMonData(lead, MON_DATA_PERSONALITY) != sBattle->leadPersonality)
        return sText_ApplyRefused;
    before = GetMonData(lead, MON_DATA_HP);
    SetMonData(lead, MON_DATA_HP, &sBattle->result.player[0].hp);
    for (i = 0; i < MAX_MON_MOVES; i++)
        SetMonData(lead, MON_DATA_PP1 + i, &sBattle->result.player[0].pp[i]);
    sBattle->applied = TRUE;
    *changedParty = TRUE;
    StringCopy(sBattle->message, sText_AppliedHp);
    AppendNumber(sBattle->message, before);
    StringAppend(sBattle->message, sText_Slash);
    AppendNumber(sBattle->message, sBattle->result.player[0].hp);
    return sBattle->message;
}

void FrontierGcBattle_GetScene(struct FrontierGcBattleScene *scene)
{
    scene->player = sBattle != NULL ? &gParties[B_TRAINER_PLAYER][sBattle->leadSlot] : NULL;
    scene->opponent = sBattle != NULL ? &sBattle->opponent[0] : NULL;
    scene->trainerId = sBattle != NULL ? sBattle->trainerId : 0;
    scene->environment = sBattle != NULL ? sBattle->environment : 0;
}

// ------------------------------------------------------------- GBA screen

static void AppendMonLine(u8 *text, struct Pokemon *mon)
{
    u8 name[POKEMON_NAME_BUFFER_SIZE];
    GetMonData(mon, MON_DATA_NICKNAME, name);
    StringAppend(text, sText_Jpn);
    StringAppend(text, name);
    StringAppend(text, sText_Lv);
    AppendNumber(text, GetMonData(mon, MON_DATA_LEVEL));
}

static void DrawHp(u8 body, u8 line, u16 hp, u16 max)
{
    u8 text[32];
    StringCopy(text, sText_Hp);
    AppendNumber(text, hp);
    StringAppend(text, sText_Slash);
    AppendNumber(text, max);
    GcExtension_PrintLine(body, text, 136, line, GC_COLOR_NORMAL);
}

static void DrawSingle(u8 body, u8 help)
{
    struct Pokemon *lead = &gParties[B_TRAINER_PLAYER][sBattle->leadSlot];
    u8 text[64];
    u32 i;

    if (!sBattle->haveState)
    {
        GcExtension_PrintLine(body, sText_Prepare, 0, 0, GC_COLOR_NORMAL);
        return;
    }
    StringCopy(text, sText_Vs);
    AppendMonLine(text, &sBattle->opponent[0]);
    GcExtension_PrintLine(body, text, 0, 0, GC_COLOR_DIM);
    DrawHp(body, 0, sBattle->state.opponent_hp, sBattle->state.opponent_max_hp);
    text[0] = EOS;
    AppendMonLine(text, lead);
    GcExtension_PrintLine(body, text, 0, 1, GC_COLOR_NORMAL);
    DrawHp(body, 1, sBattle->state.player_hp, sBattle->state.player_max_hp);

    switch (sBattle->screen)
    {
    case SCREEN_COMMAND:
        GcExtension_PrintLine(body, sText_Cursor, 0, 3, GC_COLOR_HIGHLIGHT);
        GcExtension_PrintLine(body, sText_Fight, 12, 3, GC_COLOR_HIGHLIGHT);
        GcExtension_PrintLine(help, sText_HelpCommand, 0, 0, GC_COLOR_NORMAL);
        break;
    case SCREEN_MOVES:
        for (i = 0; i < MAX_MON_MOVES; i++)
        {
            u16 move = GetMonData(lead, MON_DATA_MOVE1 + i);
            bool8 usable = (sBattle->supportedMask & (1u << i)) && sBattle->state.pp[i] > 0;
            u8 color = !usable ? GC_COLOR_DIM : i == sBattle->cursor ? GC_COLOR_HIGHLIGHT : GC_COLOR_NORMAL;
            if (move == MOVE_NONE)
                continue;
            if (i == sBattle->cursor)
                GcExtension_PrintLine(body, sText_Cursor, 0, 2 + i, color);
            StringCopy(text, sText_Jpn);
            StringAppend(text, GetMoveName(move));
            GcExtension_PrintLine(body, text, 12, 2 + i, color);
            if (!(sBattle->supportedMask & (1u << i)))
            {
                GcExtension_PrintLine(body, sText_Unsupported, 120, 2 + i, GC_COLOR_DIM);
            }
            else
            {
                StringCopy(text, sText_Pp);
                AppendNumber(text, sBattle->state.pp[i]);
                StringAppend(text, sText_Slash);
                AppendNumber(text, CalculatePPWithBonus(move, GetMonData(lead, MON_DATA_PP_BONUSES), i));
                GcExtension_PrintLine(body, text, 150, 2 + i, color);
            }
        }
        GcExtension_PrintLine(help, sText_HelpMoves, 0, 0, GC_COLOR_NORMAL);
        break;
    case SCREEN_TARGET:
    case SCREEN_REPLACE:
    case SCREEN_SWITCH:
    case SCREEN_WAIT:
        GcExtension_PrintLine(body, sText_Wait, 0, 3, GC_COLOR_DIM);
        break;
    case SCREEN_ENDED:
        switch (sBattle->state.outcome)
        {
        case OUTCOME_WIN:  GcExtension_PrintLine(body, sText_Win, 0, 3, GC_COLOR_HIGHLIGHT); break;
        case OUTCOME_LOSE: GcExtension_PrintLine(body, sText_Lose, 0, 3, GC_COLOR_ERROR); break;
        case OUTCOME_CANCELLED: GcExtension_PrintLine(body, sText_Cancelled, 0, 3, GC_COLOR_NORMAL); break;
        default: GcExtension_PrintLine(body, sText_Refused, 0, 3, GC_COLOR_ERROR); break;
        }
        GcExtension_PrintLine(help, sText_HelpEnded, 0, 0, GC_COLOR_NORMAL);
        break;
    }
}

static bool8 MoveUsable(u8 slot)
{
    return (sBattle->supportedMask & (1u << slot)) && sBattle->state.pp[slot] > 0;
}

static void InputSingle(void)
{
    u32 i;
    switch (sBattle->screen)
    {
    case SCREEN_COMMAND:
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            sBattle->screen = SCREEN_MOVES;
            for (sBattle->cursor = 0; sBattle->cursor < MAX_MON_MOVES && !MoveUsable(sBattle->cursor); sBattle->cursor++)
                ;
            if (sBattle->cursor == MAX_MON_MOVES)
                sBattle->cursor = 0;
            GcExtension_Redraw();
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            sBattle->pending.turn = sBattle->state.turn;
            sBattle->pending.action = BACTION_FORFEIT;
            sBattle->screen = SCREEN_WAIT;
            GcExtension_Redraw();
        }
        break;
    case SCREEN_MOVES:
        if (JOY_REPEAT(DPAD_UP) || JOY_REPEAT(DPAD_DOWN))
        {
            s32 step = JOY_REPEAT(DPAD_UP) ? MAX_MON_MOVES - 1 : 1;
            for (i = 0; i < MAX_MON_MOVES; i++)
            {
                sBattle->cursor = (sBattle->cursor + step) % MAX_MON_MOVES;
                if (GetMonData(&gParties[B_TRAINER_PLAYER][sBattle->leadSlot], MON_DATA_MOVE1 + sBattle->cursor) != MOVE_NONE)
                    break;
            }
            PlaySE(SE_SELECT);
            GcExtension_Redraw();
        }
        else if (JOY_NEW(A_BUTTON) && MoveUsable(sBattle->cursor))
        {
            PlaySE(SE_SELECT);
            sBattle->pending.turn = sBattle->state.turn;
            sBattle->pending.action = BACTION_MOVE;
            sBattle->pending.move_slot = sBattle->cursor;
            sBattle->screen = SCREEN_WAIT;
            GcExtension_Redraw();
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_FAILURE);
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            sBattle->screen = SCREEN_COMMAND;
            GcExtension_Redraw();
        }
        break;
    case SCREEN_ENDED:
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            GcExtension_RequestExit();
        }
        break;
    }
}

static const u8 sText_PositionHp[] = _("{ENG} HP ");
static const u8 sText_Left[] = _("{ENG}L");
static const u8 sText_Center[] = _("{ENG}C");
static const u8 sText_Right[] = _("{ENG}R");
static const u8 sText_Front[] = _("{ENG}FRONT");
static const u8 sText_Target[] = _("{ENG}Choose enemy position");
static const u8 sText_TargetHelp[] = _("{ENG}D-Pad: Target  A: OK  B: Back");
static const u8 sText_ShiftHelp[] = _("{ENG}A: Fight  SELECT: Shift  B: Quit");
static const u8 sText_RotateHelp[] = _("{ENG}L/R: Rotate  A: Fight  B: Quit");
static const u8 sText_Empty[] = _("{ENG}--");

static u8 SelectedPosition(void)
{
    if (sBattle->format != BATTLE_FORMAT_ROTATION)
        return sBattle->multiState.actor;
    if (sBattle->rotation == BROT_LEFT)
        return BPOS_LEFT;
    if (sBattle->rotation == BROT_RIGHT)
        return BPOS_RIGHT;
    return BPOS_CENTER;
}

static const u8 *PositionName(u8 pos)
{
    if (pos == BPOS_LEFT)
        return sText_Left;
    if (pos == BPOS_RIGHT || sBattle->format == BATTLE_FORMAT_DOUBLE)
        return sText_Right;
    return sBattle->format == BATTLE_FORMAT_ROTATION ? sText_Front : sText_Center;
}

static u8 FirstChoice(u8 mask)
{
    u8 i;
    for (i = 0; i < MAX_MON_MOVES; i++)
        if (mask & (1 << i))
            return i;
    return 0;
}

static u8 ChoiceAfter(u8 mask, u8 current, u8 count, bool8 backward)
{
    u8 i;
    for (i = 0; i < count; i++)
    {
        current = (current + (backward ? count - 1 : 1)) % count;
        if (mask & (1u << current))
            return current;
    }
    return current;
}

static u8 AvailableMoves(u8 pos)
{
    u8 mask = 0, i;
    for (i = 0; i < MAX_MON_MOVES; i++)
        if (sBattle->multiState.targets[pos][i])
            mask |= 1u << i;
    return mask;
}

static void DrawPositions(u8 body, u8 side, u8 row, u8 selected, u8 legal)
{
    const BattleMultiState *state = &sBattle->multiState;
    u8 slot, text[32];
    for (slot = 0; slot < sBattle->fieldCount; slot++)
    {
        const BattleFieldMon *mon = &state->field[side][slot];
        u8 color = !(legal & (1 << slot)) || !mon->hp ? GC_COLOR_DIM
                 : slot == selected ? GC_COLOR_HIGHLIGHT : GC_COLOR_NORMAL;
        StringCopy(text, PositionName(slot));
        StringAppend(text, sText_PositionHp);
        if (mon->hp)
            AppendNumber(text, mon->hp);
        else
            StringAppend(text, sText_Empty);
        if (slot == selected)
            GcExtension_PrintLine(body, sText_Cursor, slot * 74, row, color);
        GcExtension_PrintLine(body, text, slot * 74 + 9, row, color);
    }
}

static void DrawMulti(u8 body, u8 help)
{
    const BattleMultiState *state = &sBattle->multiState;
    const bool8 rotation = sBattle->format == BATTLE_FORMAT_ROTATION;
    const u8 pos = SelectedPosition();
    const BattleFieldMon *mon = &state->field[0][pos];
    u8 text[64], i;

    if (!sBattle->haveState)
    {
        GcExtension_PrintLine(body, sText_Prepare, 0, 0, GC_COLOR_NORMAL);
        return;
    }
    DrawPositions(body, 1, 0, sBattle->screen == SCREEN_TARGET ? sBattle->target : BATTLE_SLOT_NONE,
                  sBattle->screen == SCREEN_TARGET ? state->targets[pos][sBattle->cursor] : 7);
    if (sBattle->screen == SCREEN_REPLACE || sBattle->screen == SCREEN_SWITCH)
    {
        static const u8 choose[] = _("{ENG}Choose a reserve for ");
        static const u8 switchOut[] = _("{ENG}Switch out position ");
        static const u8 replaceHelp[] = _("{ENG}A: Send out   B: Forfeit");
        static const u8 switchHelp[] = _("{ENG}A: Switch   B: Back");
        const bool8 switching = sBattle->screen == SCREEN_SWITCH;
        StringCopy(text, switching ? switchOut : choose);
        StringAppend(text, PositionName(state->actor));
        GcExtension_PrintLine(body, text, 0, 1, GC_COLOR_HIGHLIGHT);
        u8 row = 2;
        for (i = 0; i < sBattle->count; i++)
        {
            if (!(sBattle->reserveMask & (1u << i)))
                continue;
            u8 color = i == sBattle->cursor ? GC_COLOR_HIGHLIGHT : GC_COLOR_NORMAL;
            if (i == sBattle->cursor)
                GcExtension_PrintLine(body, sText_Cursor, 0, row, color);
            text[0] = EOS;
            AppendMonLine(text, GcExtension_GetExportMon(GC_EXTRA_SLOT_BASE + i));
            GcExtension_PrintLine(body, text, 12, row++, color);
        }
        GcExtension_PrintLine(help, switching ? switchHelp : replaceHelp, 0, 0, GC_COLOR_NORMAL);
        return;
    }
    if (sBattle->screen == SCREEN_ENDED)
    {
        const u8 *result = state->outcome == OUTCOME_WIN ? sText_Win : state->outcome == OUTCOME_LOSE ? sText_Lose
                         : state->outcome == OUTCOME_DRAW ? sText_Draw
                         : state->outcome == OUTCOME_CANCELLED ? sText_Cancelled : sText_Refused;
        GcExtension_PrintLine(body, result, 0, 3, GC_COLOR_HIGHLIGHT);
        GcExtension_PrintLine(help, sText_HelpEnded, 0, 0, GC_COLOR_NORMAL);
        return;
    }
    if (sBattle->screen == SCREEN_WAIT)
    {
        GcExtension_PrintLine(body, sText_Wait, 0, 3, GC_COLOR_DIM);
        return;
    }

    StringCopy(text, PositionName(pos));
    StringAppend(text, sText_Cursor);
    AppendMonLine(text, GcExtension_GetExportMon(mon->party_slot));
    GcExtension_PrintLine(body, text, 0, 1, GC_COLOR_HIGHLIGHT);
    DrawHp(body, 1, mon->hp, mon->max_hp);

    if (sBattle->screen == SCREEN_COMMAND)
    {
        DrawPositions(body, 0, 3, pos, 7);
        GcExtension_PrintLine(body, sText_Fight, 0, 4, GC_COLOR_HIGHLIGHT);
        static const u8 switchHint[] = _("{ENG}START: Party");
        GcExtension_PrintLine(help, rotation ? sText_RotateHelp
                            : state->flags & BATTLE_MULTI_SHIFT ? sText_ShiftHelp : sText_HelpCommand,
                              0, 0, GC_COLOR_NORMAL);
        if (sBattle->reserveMask)
            GcExtension_PrintLine(body, switchHint, 88, 4, GC_COLOR_NORMAL);
        return;
    }
    if (sBattle->screen == SCREEN_TARGET)
    {
        GcExtension_PrintLine(body, sText_Target, 0, 3, GC_COLOR_HIGHLIGHT);
        GcExtension_PrintLine(help, sText_TargetHelp, 0, 0, GC_COLOR_NORMAL);
        return;
    }

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        u16 move = GetMonData(GcExtension_GetExportMon(mon->party_slot), MON_DATA_MOVE1 + i);
        u8 color = !state->targets[pos][i] ? GC_COLOR_DIM : i == sBattle->cursor ? GC_COLOR_HIGHLIGHT : GC_COLOR_NORMAL;
        if (move == MOVE_NONE)
            continue;
        if (i == sBattle->cursor)
            GcExtension_PrintLine(body, sText_Cursor, 0, 2 + i, color);
        StringCopy(text, sText_Jpn);
        StringAppend(text, GetMoveName(move));
        GcExtension_PrintLine(body, text, 12, 2 + i, color);
        StringCopy(text, sText_Pp);
        AppendNumber(text, state->pp[pos][i]);
        GcExtension_PrintLine(body, text, 150, 2 + i, color);
    }
    GcExtension_PrintLine(help, sText_HelpMoves, 0, 0, GC_COLOR_NORMAL);
}

static void QueueMultiAction(u8 kind, u8 target)
{
    sBattle->multiAction.action = kind;
    sBattle->multiAction.move_slot = kind == BACTION_MOVE ? sBattle->cursor : 0;
    sBattle->multiAction.target = target;
    sBattle->multiAction.rotation = kind == BACTION_MOVE ? sBattle->rotation : BROT_KEEP;
    sBattle->screen = SCREEN_WAIT;
}

static void InputMulti(void)
{
    const BattleMultiState *state = &sBattle->multiState;
    const u8 pos = SelectedPosition();
    const u8 mask = AvailableMoves(pos);
    switch (sBattle->screen)
    {
    case SCREEN_COMMAND:
        if (sBattle->format == BATTLE_FORMAT_ROTATION && (JOY_NEW(L_BUTTON) || JOY_NEW(R_BUTTON)))
        {
            const bool8 left = JOY_NEW(L_BUTTON) != 0;
            const u8 next = left ? BROT_LEFT : BROT_RIGHT;
            const u8 rotation = sBattle->rotation == next ? BROT_KEEP : next;
            const u8 slot = rotation == BROT_KEEP ? BPOS_CENTER : left ? BPOS_LEFT : BPOS_RIGHT;
            if (!AvailableMoves(slot))
            {
                PlaySE(SE_FAILURE);
                return;
            }
            sBattle->rotation = rotation;
        }
        else if (JOY_NEW(START_BUTTON) && sBattle->reserveMask)
        {
            sBattle->cursor = ChoiceAfter(sBattle->reserveMask, sBattle->count - 1, sBattle->count, FALSE);
            sBattle->screen = SCREEN_SWITCH;
        }
        else if (JOY_NEW(SELECT_BUTTON) && (state->flags & BATTLE_MULTI_SHIFT))
            QueueMultiAction(BACTION_SHIFT, 0);
        else if (JOY_NEW(B_BUTTON))
            QueueMultiAction(BACTION_FORFEIT, 0);
        else if (JOY_NEW(A_BUTTON) && mask)
        {
            sBattle->cursor = FirstChoice(mask);
            sBattle->screen = SCREEN_MOVES;
        }
        else
            return;
        break;
    case SCREEN_MOVES:
        if (JOY_REPEAT(DPAD_UP) || JOY_REPEAT(DPAD_DOWN))
            sBattle->cursor = ChoiceAfter(mask, sBattle->cursor, MAX_MON_MOVES, JOY_REPEAT(DPAD_UP));
        else if (JOY_NEW(B_BUTTON))
            sBattle->screen = SCREEN_COMMAND;
        else if (JOY_NEW(A_BUTTON) && (mask & (1 << sBattle->cursor)))
        {
            u8 targets = state->targets[pos][sBattle->cursor];
            sBattle->target = FirstChoice(targets & 7);
            if ((targets & BATTLE_TARGET_AUTO) || sBattle->format == BATTLE_FORMAT_ROTATION)
                QueueMultiAction(BACTION_MOVE, sBattle->target);
            else
                sBattle->screen = SCREEN_TARGET;
        }
        else
            return;
        break;
    case SCREEN_TARGET:
        if (JOY_REPEAT(DPAD_LEFT) || JOY_REPEAT(DPAD_RIGHT))
            sBattle->target = ChoiceAfter(state->targets[pos][sBattle->cursor] & 7,
                                         sBattle->target, sBattle->fieldCount, JOY_REPEAT(DPAD_LEFT));
        else if (JOY_NEW(B_BUTTON))
            sBattle->screen = SCREEN_MOVES;
        else if (JOY_NEW(A_BUTTON))
            QueueMultiAction(BACTION_MOVE, sBattle->target);
        else
            return;
        break;
    case SCREEN_REPLACE:
    case SCREEN_SWITCH:
        if (JOY_REPEAT(DPAD_UP) || JOY_REPEAT(DPAD_DOWN))
            sBattle->cursor = ChoiceAfter(sBattle->reserveMask, sBattle->cursor, sBattle->count, JOY_REPEAT(DPAD_UP));
        else if (JOY_NEW(A_BUTTON))
            QueueMultiAction(sBattle->screen == SCREEN_SWITCH ? BACTION_SWITCH : BACTION_REPLACE, sBattle->cursor);
        else if (JOY_NEW(B_BUTTON))
        {
            if (sBattle->screen == SCREEN_SWITCH)
                sBattle->screen = SCREEN_COMMAND;
            else
                QueueMultiAction(BACTION_FORFEIT, 0);
        }
        else
            return;
        break;
    case SCREEN_ENDED:
        if (!JOY_NEW(A_BUTTON))
            return;
        GcExtension_RequestExit();
        break;
    default:
        return;
    }
    PlaySE(SE_SELECT);
    GcExtension_Redraw();
}

static void Draw(u8 body, u8 help)
{
    if (sBattle->format == BATTLE_FORMAT_SINGLE)
        DrawSingle(body, help);
    else
        DrawMulti(body, help);
}

static void Input(void)
{
    if (sBattle->format == BATTLE_FORMAT_SINGLE)
        InputSingle();
    else
        InputMulti();
}

static const struct GcModeHandler sFrontierGcBattleMode =
{
    .modeId = GC_MODE_BATTLE,
    .modeVersion = BATTLE_MODE_VERSION,
    .appId = APP_GC_BATTLE,
    .appVersion = BATTLE_APP_VERSION,
    .init = Init,
    .getRequest = GetRequest,
    .handleApp = HandleApp,
    .draw = Draw,
    .input = Input,
    .onResult = OnResult,
    .finish = Finish,
    .free = FreeBattle,
    .resolveAsset = FrontierGcBattle_ResolveAsset,
};

// Script special (waitstate): VAR_0x8004 = result policy (0 show, 1 apply
// HP/PP), VAR_0x8005 = trainer id. VAR_0x8006 is the format (0 for
// Single), with bit 8 selecting the replacement fixture or bit 9 switching.
static EWRAM_DATA struct GcModeHandler sFrontierGcRosterMode = {0};

void FrontierGcBattle_Start(void)
{
    u8 format = gSpecialVar_0x8006 & 0xff;
    sPendingPolicy = gSpecialVar_0x8004 == RESULT_POLICY_APPLY_HP_PP ? RESULT_POLICY_APPLY_HP_PP : RESULT_POLICY_DISPLAY_ONLY;
    sPendingTrainer = gSpecialVar_0x8005;
    sPendingFormat = format >= BATTLE_FORMAT_DOUBLE && format <= BATTLE_FORMAT_ROTATION ? format : BATTLE_FORMAT_SINGLE;
    sPendingRoster = 0;
    if (format == BATTLE_FORMAT_DOUBLE)
    {
        if (gSpecialVar_0x8006 & 0x200)
            sPendingRoster = BATTLE_APP_VERSION_SWITCH;
        else if (gSpecialVar_0x8006 & 0x100)
            sPendingRoster = BATTLE_APP_VERSION_ROSTER;
    }
    if (sPendingRoster)
    {
        sFrontierGcRosterMode = sFrontierGcBattleMode;
        sFrontierGcRosterMode.appVersion = sPendingRoster;
        GcStartMode(&sFrontierGcRosterMode);
    }
    else
    {
        GcStartMode(&sFrontierGcBattleMode);
    }
}
