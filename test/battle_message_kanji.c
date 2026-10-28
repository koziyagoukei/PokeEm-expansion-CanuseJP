#include "global.h"
#include "test/test.h"
#include "battle.h"
#include "battle_message.h"
#include "link.h"
#include "line_break.h"
#include "malloc.h"
#include "move.h"
#include "string_util.h"
#include "text.h"
#include "constants/abilities.h"
#include "constants/moves.h"

static bool32 HasKanji(const u8 *text)
{
    while (*text != EOS)
    {
        if (*text == EXT_CTRL_CODE_BEGIN)
        {
            if (IS_KANJI_GLYPH_CODE(text[1]))
                return TRUE;
            text += 1 + GetExtCtrlCodeLength(text[1]);
        }
        else
            text++;
    }
    return FALSE;
}

TEST("Battle kanji: move and ability names remain kana")
{
    for (enum Move move = MOVE_NONE; move < MOVES_COUNT; move++)
        EXPECT(!HasKanji(GetMoveName(move)));
    for (enum Ability ability = ABILITY_NONE; ability < ABILITIES_COUNT; ability++)
        EXPECT(!HasKanji(gAbilitiesInfo[ability].name));
}

TEST("Battle kanji: stat IDs expand locally without changing the wire buffer")
{
    u8 wire[BATTLE_TEXT_BUFF_ARRAY_COUNT] = {0};
    u8 copy[sizeof(wire)], display[64];
    for (u32 stat = STAT_HP; stat < NUM_BATTLE_STATS; stat++)
    {
        PREPARE_STAT_BUFFER(wire, stat);
        memcpy(copy, wire, sizeof(wire));
        ExpandBattleTextBuffPlaceholders(wire, display);
        EXPECT(HasKanji(display));
        EXPECT_EQ(StringCompare(display, gStatNamesTable[stat]), 0);
        EXPECT_EQ(memcmp(copy, wire, sizeof(wire)), 0);
    }
}

TEST("Battle kanji: control codes and kanji are never partially copied")
{
    static const u8 source[] = _("{JPN}急所に 当たった！{PLAY_SE SE_BALL_BOUNCE_1}");
    u8 guarded[96];
    for (u32 capacity = 0; capacity <= sizeof(source); capacity++)
    {
        memset(guarded, 0xA5, sizeof(guarded));
        u32 written = BattleStringExpandPlaceholders(source, guarded + 1, capacity);
        EXPECT_EQ(guarded[0], 0xA5);
        for (u32 i = capacity + 1; i < sizeof(guarded); i++)
            EXPECT_EQ(guarded[i], 0xA5);
        EXPECT_LE(written, capacity);
        if (capacity == 0)
            continue;
        u32 i = 1;
        while (guarded[i] != EOS)
        {
            if (guarded[i] == EXT_CTRL_CODE_BEGIN)
                i += 1 + GetExtCtrlCodeLength(guarded[i + 1]);
            else
                i++;
            EXPECT_LE(i, capacity);
        }
        EXPECT_EQ(written, i);
    }
}

TEST("Battle kanji: long substitutions cannot overwrite the destination")
{
    static const u8 source[] = _("{JPN}{B_BUFF1} {B_BUFF2} {B_BUFF3}");
    static const u8 bufferText[] = _("{JPN}攻撃 防御 特性");
    u8 guarded[80];
    StringCopy(gBattleTextBuff1, bufferText);
    StringCopy(gBattleTextBuff2, bufferText);
    StringCopy(gBattleTextBuff3, bufferText);
    for (u32 capacity = 1; capacity < 64; capacity++)
    {
        memset(guarded, 0xA5, sizeof(guarded));
        u32 written = BattleStringExpandPlaceholders(source, guarded + 1, capacity);
        EXPECT_LE(written, capacity);
        EXPECT_EQ(guarded[written], EOS);
        EXPECT_EQ(guarded[0], 0xA5);
        for (u32 i = capacity + 1; i < sizeof(guarded); i++)
            EXPECT_EQ(guarded[i], 0xA5);
    }
}

TEST("Battle kanji: placeholders preserve control code parameters")
{
    // Parameter zero is not a word space and must not become CHAR_NBSP.
    static const u8 source[] = _("{JPN}{B_BUFF2}");
    static const u8 buffered[] = _("{JPN}{COLOR 0}特性");
    static const u8 expected[] = _("{JPN}{JPN}{COLOR 0}特性");
    u8 display[64];
    StringCopy(gBattleTextBuff2, buffered);
    BattleStringExpandPlaceholders(source, display, sizeof(display));
    EXPECT_EQ(StringCompare(display, expected), 0);
}

TEST("Battle kanji: automatic wrapping measures glyphs without damaging controls")
{
    static const u8 source[] = _("{JPN}{COLOR 0}攻撃 防御 特性");
    u8 display[sizeof(source)];
    bool32 naive = FALSE;
    PARAMETRIZE { naive = FALSE; }
    PARAMETRIZE { naive = TRUE; }
    StringCopy(display, source);
    EXPECT(!IsWordSplittingChar(display, 4)); // COLOR parameter zero.
    if (naive)
        BreakStringNaive(display, 80, 2, FONT_NORMAL, SHOW_SCROLL_PROMPT);
    else
        BreakStringAutomatic(display, 80, 2, FONT_NORMAL, SHOW_SCROLL_PROMPT);
    EXPECT_EQ(CountLineBreaks(display), 1);
    EXPECT_LE(GetStringLineWidth(FONT_NORMAL, display, 0, 1, sizeof(display)), 80);
    EXPECT_LE(GetStringLineWidth(FONT_NORMAL, display, 0, 2, sizeof(display)), 80);
    StripLineBreaks(display);
    EXPECT_EQ(StringCompare(display, source), 0);
}

TEST("Battle kanji: received message IDs use local text without rewriting the packet")
{
    u8 packet[sizeof(struct BattleMsgData) + 4];
    u32 flags = 0;
    PARAMETRIZE { flags = BATTLE_TYPE_LINK; }
    PARAMETRIZE { flags = BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI; }
    gBattleTypeFlags = flags;
    gWirelessCommType = FALSE;
    gBattleResources = AllocZeroed(sizeof(*gBattleResources));
    gBattleStruct = AllocZeroed(sizeof(*gBattleStruct));
    struct BattleMsgData *message = (void *)&gBattleResources->bufferA[B_BATTLER_0][4];
    PREPARE_STAT_BUFFER(message->textBuffs[0], STAT_ATK);
    PREPARE_MOVE_BUFFER(message->textBuffs[1], MOVE_AERIAL_ACE);
    PREPARE_ABILITY_BUFFER(message->textBuffs[2], ABILITY_PROTEAN);
    memcpy(packet, gBattleResources->bufferA[B_BATTLER_0], sizeof(packet));
    BufferStringBattle(STRINGID_CRITICALHIT, B_BATTLER_0);
    EXPECT_EQ(StringCompare(gDisplayedStringBattle, gBattleStringsTable[STRINGID_CRITICALHIT]), 0);
    EXPECT(HasKanji(gDisplayedStringBattle));
    EXPECT_EQ(memcmp(packet, gBattleResources->bufferA[B_BATTLER_0], sizeof(packet)), 0);
    EXPECT_EQ(memcmp(gBattleTextBuff1, message->textBuffs[0], BATTLE_TEXT_BUFF_ARRAY_COUNT), 0);
    EXPECT_EQ(memcmp(gBattleTextBuff2, message->textBuffs[1], BATTLE_TEXT_BUFF_ARRAY_COUNT), 0);
    EXPECT_EQ(memcmp(gBattleTextBuff3, message->textBuffs[2], BATTLE_TEXT_BUFF_ARRAY_COUNT), 0);
    ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gStringVar1);
    EXPECT_EQ(StringCompare(gStringVar1, gStatNamesTable[STAT_ATK]), 0);
    ExpandBattleTextBuffPlaceholders(gBattleTextBuff2, gStringVar2);
    EXPECT_EQ(StringCompare(gStringVar2, GetMoveName(MOVE_AERIAL_ACE)), 0);
    ExpandBattleTextBuffPlaceholders(gBattleTextBuff3, gStringVar3);
    EXPECT_EQ(StringCompare(gStringVar3, gAbilitiesInfo[ABILITY_PROTEAN].name), 0);
    FREE_AND_SET_NULL(gBattleResources);
    FREE_AND_SET_NULL(gBattleStruct);
    gBattleMsgDataPtr = NULL;
}
