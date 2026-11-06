static const u16 sInfoViewerFramePalette_Blue[] =
{
    RGB_WHITE, RGB(6, 6, 10), RGB(28, 28, 31), RGB(14, 17, 25),
    RGB(4, 5, 9), RGB(20, 23, 30), RGB(31, 31, 31), RGB(10, 12, 18),
    RGB(0, 0, 0), RGB(8, 10, 15), RGB(16, 18, 24), RGB(24, 26, 30),
    RGB(31, 31, 31), RGB(5, 7, 12), RGB(12, 15, 22), RGB_BLACK
};

static const struct InfoViewerStyle sInfoViewerStyle_Default =
{
    .windowFill = PIXEL_FILL(TEXT_COLOR_WHITE),
    .framePaletteNum = STD_WINDOW_PALETTE_NUM,
    .framePalette = NULL,
    .textColors = { TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY },
};

static const struct InfoViewerStyle sInfoViewerStyle_BlueFrame =
{
    .windowFill = PIXEL_FILL(TEXT_COLOR_WHITE),
    .framePaletteNum = INFO_VIEWER_ALT_FRAME_PALETTE_NUM,
    .framePalette = sInfoViewerFramePalette_Blue,
    .textColors = { TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY },
};

static const u8 sInfoText_TitleFrontierRules[] = _("{JPN}バトルフロンティア");
static const u8 sInfoText_TitleBannedMons[] = _("{JPN}禁止ポケモン 一覧");
static const u8 sInfoText_TitleGimmickHelp[] = _("{JPN}ギミック");
static const u8 sInfoText_TitleDiaryTest[] = _("{JPN}日記");
static const u8 sInfoText_TitleKanjiTest[] = _("{KANJI}対戦 フォント");
static const u8 sInfoText_TitleMonologueTest[] = _("{JPN}独り言");
static const u8 sInfoText_TitleChoiceTest[] = _("{JPN}分岐テスト");

static const u8 sInfoText_FrontierRulesPage1[] = _(
    "{JPN}タマゴは 参加できません。\n"
    "同じ 種類の ポケモンや\n"
    "同じ 持ち物は 選べません。\n"
    "禁止ポケモンの 扱いは\n"
    "ハブで 設定した ルールに\n"
    "したがいます。");

static const u8 sInfoText_FrontierRulesPage2[] = _(
    "{JPN}レベル50では レベル51以上も\n"
    "参加でき 挑戦中だけ\n"
    "レベル50に なります。\n"
    "オープンでは 相手の レベルが\n"
    "こちらに 合わせて 決まります。\n"
    "相手は 最低 レベル60です。");

static const u8 sInfoText_BannedMonsPage1[] = _(
    "{JPN}説明の あとに 対象の\n"
    "ポケモンを すべて 表示します。\n"
    "フォルムちがいは まとめています。\n"
    "禁止ルールでは 参加できません。\n"
    "解禁ルールでは おたがいに\n"
    "対象を 1匹まで 使えます。");

static const u8 sInfoText_FrontierRulesPage3[] = _(
    "{JPN}途中で まけるか リタイアすると\n"
    "連勝は おわります。\n"
    "挑戦中は 技の 変更や\n"
    "リゾートへの ワープは できません。\n"
    "中断は 施設の 案内か\n"
    "メニューの やすむを 使います。");

static const u8 sInfoText_GimmickHelpPage1[] = _(
    "{JPN}「戦闘中の 操作」\n"
    "たたかうで 技を 選ぶ画面を 開きます。\n"
    "R:メガシンカ / Zワザ / バースト\n"
    "START:ダイマックス\n"
    "SELECT:テラスタル\n"
    "使いたいものを 選んでから 技を 決定。");

static const u8 sInfoText_GimmickHelpPage2[] = _(
    "{JPN}「回数と 組み合わせ」\n"
    "各種ギミックは 1人につき 1戦で 1回。\n"
    "別の仲間に 別の種類を 使わせられます。\n"
    "ただし 同じ個体には 重ねられません。\n"
    "バースト後の Zワザは 例外です。\n"
    "ダイマックスは マルチバトルでも\n"
    "味方全体で 1回を 分け合います。");

static const u8 sInfoText_GimmickHelpPage3[] = _(
    "{JPN}「メガシンカ」\n"
    "対応する メガストーンを 持たせます。\n"
    "Rで 選ぶと 技を 出す前に 変身。\n"
    "能力や 特性などが 変わります。\n"
    "交代しても メガシンカは 続きます。\n"
    "レックウザは ガリョウテンセイを\n"
    "覚えていると 石なしでも 使えます。");

static const u8 sInfoText_GimmickHelpPage4[] = _(
    "{JPN}「Zワザ / 攻撃技」\n"
    "元の技と タイプが あう\n"
    "Zクリスタルを 持たせます。\n"
    "対象の技に カーソルを あわせて R。\n"
    "強力な Zワザに 変えて 攻撃します。\n"
    "専用Zワザは ポケモンの 種類と\n"
    "元の技にも 条件が あります。");

static const u8 sInfoText_GimmickHelpPage5[] = _(
    "{JPN}「Zワザ / 使いどころ」\n"
    "変化技にも 能力上昇や 回復などの\n"
    "追加効果が つくものが あります。\n"
    "攻撃Zワザは まもるを 使われても\n"
    "一部の ダメージを 与えられます。\n"
    "ここぞという 場面で 使いましょう。\n"
    "使うと 自分の 他の仲間も 使えません。");

static const u8 sInfoText_GimmickHelpPage6[] = _(
    "{JPN}「ダイマックス」\n"
    "STARTで 選ぶと 巨大化します。\n"
    "HPが 増え 技が ダイマックスわざに。\n"
    "天候や 能力を 変える技も あります。\n"
    "変化技は ダイウォールに なります。\n"
    "3ターンで 終わり 交代でも 解除。\n"
    "解除後は もう一度 使えません。");

static const u8 sInfoText_GimmickHelpPage7[] = _(
    "{JPN}「キョダイマックス」\n"
    "対応する 種類で キョダイマックスの\n"
    "素質が あると 特別な 姿に なります。\n"
    "一部の 攻撃技が 専用技に 変わり\n"
    "通常の ダイマックスと 効果が 違います。\n"
    "操作と 3ターンの 制限は 同じです。\n"
    "素質は 定食屋で 切り替えられます。");

static const u8 sInfoText_GimmickHelpPage8[] = _(
    "{JPN}「テラスタル」\n"
    "SELECTで 選ぶと テラタイプに 変化。\n"
    "攻撃を 受けるときの 相性が 変わります。\n"
    "元のタイプの 技の 強化は 残り\n"
    "テラタイプと 同じ技も 強化されます。\n"
    "元と 同じタイプなら さらに 強化。\n"
    "交代しても 効果は 続きます。");

static const u8 sInfoText_GimmickHelpPage9[] = _(
    "{JPN}「ステラ / 相性を 保つ」\n"
    "ステラは 特別な テラタイプです。\n"
    "受ける技の 相性は 元のままで\n"
    "攻撃技を タイプごとに 1回 強化。\n"
    "一部の ポケモンには 例外も あります。\n"
    "テラタイプの 変更は 定食屋から。\n"
    "弱点を 変えるか 火力を 上げるかで 選択。");

static const u8 sInfoText_GimmickHelpPage10[] = _(
    "{JPN}「ウルトラバースト」\n"
    "にっしょくか げっしょくの ネクロズマに\n"
    "ウルトラネクロZを 持たせて Rで 変身。\n"
    "能力と タイプが 変わります。\n"
    "変身後は フォトンゲイザーを 選び\n"
    "次のターン以降 Rで 専用Zワザも 使用可。\n"
    "Zワザが 未使用であることが 条件です。");

static const u8 sInfoText_GimmickHelpPage11[] = _(
    "{JPN}「施設ごとの 制限」\n"
    "パレスは メガシンカと テラスタルのみ。\n"
    "使うかどうかは 自動で 決まります。\n"
    "アリーナは Zワザ / ダイマックス /\n"
    "ウルトラバーストのみ。\n"
    "ピラミッドは Zワザと テラスタルのみ。\n"
    "制限は おたがいに 同じです。");

static const u8 sInfoText_GimmickHelpPage12[] = _(
    "{JPN}「選べないときは」\n"
    "種類と 持ち物と 施設の ルールを 確認。\n"
    "メガストーンや Zクリスタルを 持つと\n"
    "ダイマックスや テラスタルが できません。\n"
    "必要な 大切なものは ハブで そろいます。\n"
    "同じボタンで 予約を 取り消せます。\n"
    "別のボタンなら 予約する 種類を 変更。");

static const u8 sInfoText_DiaryTestPage1[] = _(
    "{JPN}今日は フロンティアで\n"
    "新しい ルールを 読んだ\n\n"
    "難しいけれど\n"
    "おもしろい しくみだった");

static const u8 sInfoText_DiaryTestPage2[] = _(
    "{JPN}次に 来るときは\n"
    "素早い ポケモンを\n"
    "つれてこよう\n\n"
    "メモとして 残しておく");

static const u8 sInfoText_DiaryTestKanji[] = _(
    "{KANJI}攻撃 防御 特性\n"
    "技の力と 速さ\n"
    "命中 状態異常\n"
    "回復 対戦 相手\n"
    "物理と 変化\n"
    "{ENG}ABC 123{JPN} かな");

static const u8 sInfoText_MonologueTestPage1[] = _(
    "{JPN}ここまで 来たんだ\n"
    "もうすこし だけ\n"
    "前に 進んでみよう");

static const u8 sInfoText_MonologueTestPage2[] = _(
    "{JPN}迷っても いい\n"
    "でも 立ち止まったままでは\n"
    "なにも かわらない");

static const u8 sInfoText_ChoiceTestPage1[] = _(
    "{JPN}これは 分岐テストです\n"
    "最後まで 読むと\n"
    "選べる 項目が でます\n\n"
    "選んだ 内容で\n"
    "別の ページへ 進みます");

static const u8 sInfoText_ChoiceTestPage2[] = _(
    "{JPN}Aで 選ぶ\n"
    "Bで 前の 画面へ 戻る\n\n"
    "閉じるを 選ぶと\n"
    "スクリプトへ 戻ります");

static const u8 sInfoText_ChoiceReadDiary[] = _("{JPN}日記を 読む");
static const u8 sInfoText_ChoiceReadMonologue[] = _("{JPN}独り言を 読む");
static const u8 sInfoText_ChoiceBack[] = _("{JPN}戻る");
static const u8 sInfoText_ChoiceClose[] = _("{JPN}閉じる");

static const u8 *const sInfoViewerPages_FrontierRules[] =
{
    sInfoText_FrontierRulesPage1,
    sInfoText_FrontierRulesPage2,
    sInfoText_FrontierRulesPage3,
};

static const u8 *const sInfoViewerPages_BannedMons[] =
{
    sInfoText_BannedMonsPage1,
};

static const u8 *const sInfoViewerPages_GimmickHelp[] =
{
    sInfoText_GimmickHelpPage1,
    sInfoText_GimmickHelpPage2,
    sInfoText_GimmickHelpPage3,
    sInfoText_GimmickHelpPage4,
    sInfoText_GimmickHelpPage5,
    sInfoText_GimmickHelpPage6,
    sInfoText_GimmickHelpPage7,
    sInfoText_GimmickHelpPage8,
    sInfoText_GimmickHelpPage9,
    sInfoText_GimmickHelpPage10,
    sInfoText_GimmickHelpPage11,
    sInfoText_GimmickHelpPage12,
};

static const u8 *const sInfoViewerPages_DiaryTest[] =
{
    sInfoText_DiaryTestPage1,
    sInfoText_DiaryTestPage2,
};

static const u8 *const sInfoViewerPages_KanjiTest[] =
{
    sInfoText_DiaryTestKanji,
};

static const u8 *const sInfoViewerPages_MonologueTest[] =
{
    sInfoText_MonologueTestPage1,
    sInfoText_MonologueTestPage2,
};

static const u8 *const sInfoViewerPages_ChoiceTest[] =
{
    sInfoText_ChoiceTestPage1,
    sInfoText_ChoiceTestPage2,
};

static const struct InfoViewerChoice sInfoViewerChoices_DiaryTest[] =
{
    {
        .text = sInfoText_ChoiceBack,
        .nextInfoId = INFO_VIEWER_BACK,
    },
    {
        .text = sInfoText_ChoiceClose,
        .nextInfoId = INFO_VIEWER_CLOSE,
    },
};

static const struct InfoViewerChoice sInfoViewerChoices_ChoiceTest[] =
{
    {
        .text = sInfoText_ChoiceReadDiary,
        .nextInfoId = INFO_DIARY_TEST,
    },
    {
        .text = sInfoText_ChoiceReadMonologue,
        .nextInfoId = INFO_MONOLOGUE_TEST,
    },
    {
        .text = sInfoText_ChoiceClose,
        .nextInfoId = INFO_VIEWER_CLOSE,
    },
};

#include "resort_tips.h"
#include "contest_tips.h"
#include "frontier_tips.h"

static const struct InfoViewerEntry sInfoViewerEntries[INFO_COUNT] =
{
    [INFO_TIPS] = {
        .title = sInfoText_TitleTips,
        .choices = sInfoViewerChoices_Tips,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_Tips),
    },
    [INFO_TIPS_FACILITIES] = {
        .title = sInfoText_TitleFacilities,
        .choices = sInfoViewerChoices_Facilities,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_Facilities),
    },
    [INFO_TIPS_TOWER] = TIPS_PAGE_ENTRY(Tower),
    [INFO_TIPS_DOME] = TIPS_PAGE_ENTRY(Dome),
    [INFO_TIPS_PALACE] = TIPS_PAGE_ENTRY(Palace),
    [INFO_TIPS_ARENA] = TIPS_PAGE_ENTRY(Arena),
    [INFO_TIPS_FACTORY] = TIPS_PAGE_ENTRY(Factory),
    [INFO_TIPS_PIKE] = TIPS_PAGE_ENTRY(Pike),
    [INFO_TIPS_PYRAMID] = TIPS_PAGE_ENTRY(Pyramid),
    [INFO_TIPS_HUB] = {
        .title = sInfoText_TitleHub,
        .choices = sInfoViewerChoices_Hub,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_Hub),
    },
    [INFO_TIPS_HUB_POKEMON] = TIPS_PAGE_ENTRY(HubPokemon),
    [INFO_TIPS_HUB_ITEMS] = TIPS_PAGE_ENTRY(HubItems),
    [INFO_TIPS_HUB_MUSIC] = TIPS_PAGE_ENTRY(HubMusic),
    [INFO_TIPS_HUB_DIFFICULTY] = TIPS_PAGE_ENTRY(HubDifficulty),
    [INFO_TIPS_HUB_BANNED] = TIPS_PAGE_ENTRY(HubBanned),
    [INFO_TIPS_HUB_SUPPORT] = TIPS_PAGE_ENTRY(HubSupport),
    [INFO_TIPS_RESORT] = {
        .title = sInfoText_TitleResort,
        .choices = sInfoViewerChoices_Resort,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_Resort),
    },
    [INFO_TIPS_RESORT_OVERVIEW] = TIPS_PAGE_ENTRY(ResortOverview),
    [INFO_TIPS_RESORT_CAMP] = TIPS_PAGE_ENTRY(ResortCamp),
    [INFO_TIPS_RESORT_DINER] = TIPS_PAGE_ENTRY(ResortDiner),
    [INFO_TIPS_RESORT_BATH] = TIPS_PAGE_ENTRY(ResortBath),
    [INFO_TIPS_RESORT_VILLA] = TIPS_PAGE_ENTRY(ResortVilla),
    [INFO_TIPS_RESORT_SHOP] = TIPS_PAGE_ENTRY(ResortShop),
    [INFO_TIPS_RESORT_ROOM] = TIPS_PAGE_ENTRY(ResortRoom),
    [INFO_TIPS_RESORT_DOLLS] = TIPS_PAGE_ENTRY(ResortDolls),
    [INFO_TIPS_CONTEST] = {
        .title = sInfoText_TitleContest,
        .choices = sInfoViewerChoices_Contest,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_Contest),
    },
    [INFO_TIPS_CONTEST_BASICS] = TIPS_PAGE_ENTRY(ContestBasics),
    [INFO_TIPS_CONTEST_PREPARATION] = TIPS_PAGE_ENTRY(ContestPreparation),
    [INFO_TIPS_CONTEST_SELECTION] = TIPS_PAGE_ENTRY(ContestSelection),
    [INFO_TIPS_CONTEST_MEGA] = TIPS_PAGE_ENTRY(ContestMega),
    [INFO_TIPS_CONTEST_Z] = TIPS_PAGE_ENTRY(ContestZ),
    [INFO_TIPS_CONTEST_TERA] = TIPS_PAGE_ENTRY(ContestTera),
    [INFO_TIPS_CONTEST_DYNAMAX] = TIPS_PAGE_ENTRY(ContestDynamax),
    [INFO_TIPS_CONTEST_REWARDS] = TIPS_PAGE_ENTRY(ContestRewards),
    [INFO_TIPS_SHADOW] = TIPS_PAGE_ENTRY(Shadow),
    [INFO_KANJI_FONT_TEST] =
    {
        .title = sInfoText_TitleKanjiTest,
        .pages = sInfoViewerPages_KanjiTest,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_KanjiTest),
        .style = &sInfoViewerStyle_Default,
    },
    [INFO_FRONTIER_RULES] =
    {
        .title = sInfoText_TitleFrontierRules,
        .pages = sInfoViewerPages_FrontierRules,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_FrontierRules),
        .choices = NULL,
        .choiceCount = 0,
        .style = &sInfoViewerStyle_Default,
    },
    [INFO_BANNED_MONS] =
    {
        .title = sInfoText_TitleBannedMons,
        .pages = sInfoViewerPages_BannedMons,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_BannedMons),
        .choices = NULL,
        .choiceCount = 0,
        .style = &sInfoViewerStyle_Default,
    },
    [INFO_GIMMICK_HELP] =
    {
        .title = sInfoText_TitleGimmickHelp,
        .pages = sInfoViewerPages_GimmickHelp,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_GimmickHelp),
        .choices = NULL,
        .choiceCount = 0,
        .style = &sInfoViewerStyle_BlueFrame,
    },
    [INFO_DIARY_TEST] =
    {
        .title = sInfoText_TitleDiaryTest,
        .pages = sInfoViewerPages_DiaryTest,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_DiaryTest),
        .choices = sInfoViewerChoices_DiaryTest,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_DiaryTest),
        .style = &sInfoViewerStyle_Default,
    },
    [INFO_MONOLOGUE_TEST] =
    {
        .title = sInfoText_TitleMonologueTest,
        .pages = sInfoViewerPages_MonologueTest,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_MonologueTest),
        .choices = NULL,
        .choiceCount = 0,
        .style = &sInfoViewerStyle_BlueFrame,
    },
    [INFO_CHOICE_TEST] =
    {
        .title = sInfoText_TitleChoiceTest,
        .pages = sInfoViewerPages_ChoiceTest,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_ChoiceTest),
        .choices = sInfoViewerChoices_ChoiceTest,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_ChoiceTest),
        .style = &sInfoViewerStyle_BlueFrame,
    },
};
