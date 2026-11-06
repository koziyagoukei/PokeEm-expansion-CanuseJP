#include "global.h"
#include "event_data.h"
#include "frontier_util.h"
#include "line_break.h"
#include "pokemon.h"
#include "string_util.h"
#include "text.h"
#include "ultra_help.h"
#include "test/test.h"

TEST("Frontier TIPS: all pages and choices fit the explanation window")
{
    u32 info = 0;
    for (u32 i = 0; i < INFO_COUNT; i++)
        PARAMETRIZE { info = i; }
    const struct InfoViewerEntry *entry = GetInfoViewerEntry(info);
    EXPECT(GetStringWidth(FONT_NORMAL, entry->title, 0) <= 208);
    EXPECT(entry->pageCount != 0 || entry->choiceCount != 0);
    for (u32 page = 0; page < entry->pageCount; page++)
    {
        EXPECT(GetStringWidth(FONT_SMALL, entry->pages[page], 0) <= 208);
        EXPECT((CountLineBreaks((u8 *)entry->pages[page]) + 1) * 14 <= 98);
    }
    for (u32 choice = 0; choice < entry->choiceCount; choice++)
    {
        u16 target = entry->choices[choice].nextInfoId;
        EXPECT(GetStringWidth(FONT_SMALL, entry->choices[choice].text, 0) <= 188);
        EXPECT(target < INFO_COUNT || target == INFO_VIEWER_BACK || target == INFO_VIEWER_CLOSE);
    }
}

static void CheckExplicitMode(const u8 *text)
{
    EXPECT_EQ(text[0], EXT_CTRL_CODE_BEGIN);
    EXPECT(text[1] == EXT_CTRL_CODE_JPN || text[1] == EXT_CTRL_CODE_ENG || text[1] == EXT_CTRL_CODE_KANJI);
}

TEST("Frontier TIPS: every topic page and choice explicitly selects its display language")
{
    for (u32 info = 0; info < INFO_COUNT; info++)
    {
        const struct InfoViewerEntry *entry = GetInfoViewerEntry(info);
        CheckExplicitMode(entry->title);
        for (u32 page = 0; page < entry->pageCount; page++)
            CheckExplicitMode(entry->pages[page]);
        for (u32 choice = 0; choice < entry->choiceCount; choice++)
            CheckExplicitMode(entry->choices[choice].text);
    }
}

TEST("Frontier TIPS: facilities, hub, resort, contest and rule topics are reachable from TIPS")
{
    bool8 seen[INFO_COUNT] = {0};
    seen[INFO_TIPS] = TRUE;
    for (u32 pass = 0; pass < INFO_COUNT; pass++)
        for (u32 id = 0; id < INFO_COUNT; id++)
            if (seen[id])
            {
                const struct InfoViewerEntry *entry = GetInfoViewerEntry(id);
                for (u32 i = 0; i < entry->choiceCount; i++)
                    if (entry->choices[i].nextInfoId < INFO_COUNT)
                        seen[entry->choices[i].nextInfoId] = TRUE;
            }
    EXPECT(seen[INFO_FRONTIER_RULES]);
    EXPECT(seen[INFO_BANNED_MONS]);
    EXPECT(seen[INFO_GIMMICK_HELP]);
    EXPECT(seen[INFO_TIPS_SHADOW]);
    EXPECT_EQ(GetInfoViewerPageCount(INFO_TIPS_SHADOW), 5);
    for (u32 id = INFO_TIPS_TOWER; id <= INFO_TIPS_PYRAMID; id++)
        EXPECT(seen[id]);
    for (u32 id = INFO_TIPS_HUB; id <= INFO_TIPS_HUB_SUPPORT; id++)
        EXPECT(seen[id]);
    for (u32 id = INFO_TIPS_RESORT; id <= INFO_TIPS_RESORT_DOLLS; id++)
        EXPECT(seen[id]);
    for (u32 id = INFO_TIPS_CONTEST; id <= INFO_TIPS_CONTEST_REWARDS; id++)
        EXPECT(seen[id]);
    EXPECT(!seen[INFO_DIARY_TEST]);
    EXPECT(!seen[INFO_CHOICE_TEST]);
}

TEST("Frontier TIPS: contest has eight topics and twenty-one pages without changing older topic IDs")
{
    static const u8 pages[] = {3, 3, 2, 2, 2, 4, 2, 3};
    const struct InfoViewerEntry *contest = GetInfoViewerEntry(INFO_TIPS_CONTEST);
    EXPECT_EQ(INFO_TIPS_RESORT_DOLLS, 30);
    EXPECT_EQ(INFO_TIPS_CONTEST, 31);
    EXPECT_EQ(contest->choiceCount, ARRAY_COUNT(pages) + 1);
    u32 total = 0;
    for (u32 i = 0; i < ARRAY_COUNT(pages); i++)
    {
        u32 topic = INFO_TIPS_CONTEST_BASICS + i;
        EXPECT_EQ(contest->choices[i].nextInfoId, topic);
        EXPECT_EQ(GetInfoViewerPageCount(topic), pages[i]);
        total += GetInfoViewerPageCount(topic);
    }
    EXPECT_EQ(total, 21);
    EXPECT_EQ(contest->choices[ARRAY_COUNT(pages)].nextInfoId, INFO_VIEWER_BACK);
}

TEST("Frontier TIPS: beginner guides cover battle controls, every gimmick and all four AI levels")
{
    static const u8 *const gimmickHeadings[] = {
        COMPOUND_STRING("{JPN}「戦闘中の 操作」"),
        COMPOUND_STRING("{JPN}「回数と 組み合わせ」"),
        COMPOUND_STRING("{JPN}「メガシンカ」"),
        COMPOUND_STRING("{JPN}「Zワザ / 攻撃技」"),
        COMPOUND_STRING("{JPN}「Zワザ / 使いどころ」"),
        COMPOUND_STRING("{JPN}「ダイマックス」"),
        COMPOUND_STRING("{JPN}「キョダイマックス」"),
        COMPOUND_STRING("{JPN}「テラスタル」"),
        COMPOUND_STRING("{JPN}「ステラ / 相性を 保つ」"),
        COMPOUND_STRING("{JPN}「ウルトラバースト」"),
        COMPOUND_STRING("{JPN}「施設ごとの 制限」"),
        COMPOUND_STRING("{JPN}「選べないときは」"),
    };
    static const u8 *const aiHeadings[] = {
        COMPOUND_STRING("{JPN}「難しさの 選び方」"),
        COMPOUND_STRING("{JPN}「簡単 / まずは 練習」"),
        COMPOUND_STRING("{JPN}「いつもの / 基本の 駆け引き」"),
        COMPOUND_STRING("{JPN}「難しい / 先の 展開を 考える」"),
        COMPOUND_STRING("{JPN}「最強 / 読み合いに 挑戦」"),
        COMPOUND_STRING("{JPN}「施設による 違い」"),
        COMPOUND_STRING("{JPN}「連勝による 強さの 変化」"),
        COMPOUND_STRING("{JPN}「困ったときの 考え方」"),
        COMPOUND_STRING("{JPN}「相手の 降参」"),
    };
    const struct InfoViewerEntry *gimmicks = GetInfoViewerEntry(INFO_GIMMICK_HELP);
    const struct InfoViewerEntry *ai = GetInfoViewerEntry(INFO_TIPS_HUB_DIFFICULTY);
    EXPECT_EQ(gimmicks->pageCount, ARRAY_COUNT(gimmickHeadings));
    EXPECT_EQ(ai->pageCount, ARRAY_COUNT(aiHeadings));
    for (u32 i = 0; i < ARRAY_COUNT(gimmickHeadings); i++)
        EXPECT_EQ(StringCompareN(gimmicks->pages[i], gimmickHeadings[i], StringLength(gimmickHeadings[i])), 0);
    for (u32 i = 0; i < ARRAY_COUNT(aiHeadings); i++)
        EXPECT_EQ(StringCompareN(ai->pages[i], aiHeadings[i], StringLength(aiHeadings[i])), 0);
}

TEST("Frontier TIPS: AI guide is directly accessible from both TIPS and hub services")
{
    static const u16 menus[] = {INFO_TIPS, INFO_TIPS_HUB};
    for (u32 i = 0; i < ARRAY_COUNT(menus); i++)
    {
        const struct InfoViewerEntry *menu = GetInfoViewerEntry(menus[i]);
        u32 links = 0;
        for (u32 j = 0; j < menu->choiceCount; j++)
            if (menu->choices[j].nextInfoId == INFO_TIPS_HUB_DIFFICULTY)
                links++;
        EXPECT_EQ(links, 1);
    }
    EXPECT_EQ(INFO_GIMMICK_HELP, 2);
    EXPECT_EQ(INFO_TIPS_HUB_DIFFICULTY, 19);
    EXPECT_EQ(INFO_KANJI_FONT_TEST, 40);
}

TEST("Frontier TIPS: banned list covers actual admission rules including all forms")
{
    bool8 listed[NUM_SPECIES] = {0};
    u32 count = 0;
    enum Species species;
    FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    while ((species = GetInfoViewerBannedSpecies(count)) != SPECIES_NONE)
    {
        EXPECT(!listed[species]);
        listed[species] = TRUE;
        EXPECT(IsFrontierSpeciesBannedByCurrentRules(species));
        EXPECT(GetStringWidth(FONT_SMALL, GetSpeciesName(species), 0) <= 100);
        count++;
        EXPECT(count < NUM_SPECIES);
    }
    EXPECT(count > 30);
    EXPECT(listed[SPECIES_MEW]);
    EXPECT(listed[SPECIES_COSMOG]);
    EXPECT(listed[SPECIES_NECROZMA]);
    EXPECT(!listed[SPECIES_PIKACHU]);
    EXPECT_EQ(GetInfoViewerPageCount(INFO_BANNED_MONS), GetInfoViewerEntry(INFO_BANNED_MONS)->pageCount + (count + 9) / 10);
    for (u32 i = 1; i < NUM_SPECIES; i++)
    {
        if (!IsSpeciesEnabled(i))
            continue;
        enum Species base = GET_BASE_SPECIES_ID(i);
        // Mega forms are battle transformations, not separately entered mons.
        if (gSpeciesInfo[i].isMegaEvolution)
            continue;
        EXPECT_EQ(listed[i] || listed[base], IsFrontierSpeciesBannedByCurrentRules(i));
    }
    FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    for (u32 i = 0; i < count; i++)
    {
        species = GetInfoViewerBannedSpecies(i);
        EXPECT(listed[species]);
        EXPECT(!IsFrontierSpeciesBannedByCurrentRules(species));
        EXPECT(IsFrontierSpeciesSubjectToBannedLimit(species));
    }
    EXPECT_EQ(GetInfoViewerBannedSpecies(count), SPECIES_NONE);
    FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
}
