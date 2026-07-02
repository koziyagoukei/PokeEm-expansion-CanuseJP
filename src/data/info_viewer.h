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
static const u8 sInfoText_TitleBannedMons[] = _("{JPN}さんかできない ポケモン");
static const u8 sInfoText_TitleBannedItems[] = _("{JPN}つかえない もちもの");
static const u8 sInfoText_TitleGimmickHelp[] = _("{JPN}ギミック");
static const u8 sInfoText_TitleDiaryTest[] = _("{JPN}にっき");
static const u8 sInfoText_TitleMonologueTest[] = _("{JPN}ひとりごと");
static const u8 sInfoText_TitleChoiceTest[] = _("{JPN}ぶんきテスト");

static const u8 sInfoText_FrontierRulesPage1[] = _(
    "{JPN}フロンティアでは\n"
    "レベルと ルールを そろえて\n"
    "バトルを おこないます\n\n"
    "つかえる ポケモンや\n"
    "もちものには せいげんがあります");

static const u8 sInfoText_FrontierRulesPage2[] = _(
    "{JPN}いちど ちょうせんを はじめると\n"
    "きめられた かいすうまで\n"
    "れんぞくで たたかいます\n\n"
    "とちゅうで まけると\n"
    "その ちょうせんは おわります");

static const u8 sInfoText_BannedMonsPage1[] = _(
    "{JPN}ミュウツー ホウオウ ルギア\n"
    "グラードン カイオーガ\n"
    "レックウザ ディアルガ パルキア\n"
    "ギラティナ レシラム ゼクロム\n"
    "キュレム ゼルネアス イベルタル");

static const u8 sInfoText_BannedMonsPage2[] = _(
    "{JPN}ジガルデ ソルガレオ ルナアーラ\n"
    "ネクロズマ ザシアン ザマゼンタ\n"
    "ムゲンダイナ コライドン\n"
    "ミライドン テラパゴス\n"
    "そのほか とくべつな ポケモン");

static const u8 sInfoText_BannedItemsPage1[] = _(
    "{JPN}こころのしずく など\n"
    "フロンティアの ルールで\n"
    "もちこめない もちものが\n"
    "あります\n\n"
    "くわしくは しせつごとの\n"
    "せつめいを かくにんしてください");

static const u8 sInfoText_GimmickHelpPage1[] = _(
    "{JPN}しせつごとに\n"
    "とくべつな ルールや\n"
    "ギミックが あります\n\n"
    "せつめいを よんでから\n"
    "ちょうせんしてください");

static const u8 sInfoText_GimmickHelpPage2[] = _(
    "{JPN}ギミックは\n"
    "バトルの じゅんびや\n"
    "ポケモンの えらびかたに\n"
    "かかわることが あります");

static const u8 sInfoText_DiaryTestPage1[] = _(
    "{JPN}きょうは フロンティアで\n"
    "あたらしい ルールを よんだ\n\n"
    "むずかしいけれど\n"
    "おもしろい しくみだった");

static const u8 sInfoText_DiaryTestPage2[] = _(
    "{JPN}つぎに くるときは\n"
    "すばやい ポケモンを\n"
    "つれてこよう\n\n"
    "メモとして のこしておく");

static const u8 sInfoText_MonologueTestPage1[] = _(
    "{JPN}ここまで きたんだ\n"
    "もうすこし だけ\n"
    "まえに すすんでみよう");

static const u8 sInfoText_MonologueTestPage2[] = _(
    "{JPN}まよっても いい\n"
    "でも たちどまったままでは\n"
    "なにも かわらない");

static const u8 sInfoText_ChoiceTestPage1[] = _(
    "{JPN}これは ぶんきテストです\n"
    "さいごまで よむと\n"
    "えらべる こうもくが でます\n\n"
    "えらんだ ないようで\n"
    "べつの ページへ すすみます");

static const u8 sInfoText_ChoiceTestPage2[] = _(
    "{JPN}Aで えらぶ\n"
    "Bで まえの がめんへ もどる\n\n"
    "とじるを えらぶと\n"
    "スクリプトへ もどります");

static const u8 sInfoText_ChoiceReadDiary[] = _("{JPN}にっきを よむ");
static const u8 sInfoText_ChoiceReadMonologue[] = _("{JPN}ひとりごとを よむ");
static const u8 sInfoText_ChoiceBack[] = _("{JPN}もどる");
static const u8 sInfoText_ChoiceClose[] = _("{JPN}とじる");

static const u8 *const sInfoViewerPages_FrontierRules[] =
{
    sInfoText_FrontierRulesPage1,
    sInfoText_FrontierRulesPage2,
    sInfoText_BannedMonsPage1,
    sInfoText_BannedMonsPage2,
    sInfoText_BannedItemsPage1,
};

static const u8 *const sInfoViewerPages_BannedMons[] =
{
    sInfoText_BannedMonsPage1,
    sInfoText_BannedMonsPage2,
};

static const u8 *const sInfoViewerPages_GimmickHelp[] =
{
    sInfoText_GimmickHelpPage1,
    sInfoText_GimmickHelpPage2,
};

static const u8 *const sInfoViewerPages_DiaryTest[] =
{
    sInfoText_DiaryTestPage1,
    sInfoText_DiaryTestPage2,
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

static const struct InfoViewerEntry sInfoViewerEntries[INFO_COUNT] =
{
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
