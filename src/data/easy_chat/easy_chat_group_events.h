const u8 gEasyChatWord_Appeal[] = _("アピール");
const u8 gEasyChatWord_Events[] = _("イベント");
const u8 gEasyChatWord_StayAtHome[] = _("おるすばん");
const u8 gEasyChatWord_Berry[] = _("きのみ");
const u8 gEasyChatWord_Contest[] = _("コンテスト");
const u8 gEasyChatWord_Mc[] = _("しかいしゃ");
const u8 gEasyChatWord_Judge[] = _("しんさいん");
const u8 gEasyChatWord_Super[] = _("スーパー");
const u8 gEasyChatWord_Stage[] = _("ステージ");
const u8 gEasyChatWord_HallOfFame[] = _("でんどういり");
const u8 gEasyChatWord_Evolution[] = _("ノーマル");
const u8 gEasyChatWord_Hyper[] = _("ハイパー");
const u8 gEasyChatWord_BattleTower[] = _("バトルタワー");
const u8 gEasyChatWord_Leaders[] = _("バトルリーダー");
const u8 gEasyChatWord_BattleRoom[] = _("バトルルーム");
const u8 gEasyChatWord_Hidden[] = _("ひでん");
const u8 gEasyChatWord_SecretBase[] = _("ひみつきち");
const u8 gEasyChatWord_Blend[] = _("ブレンド");
const u8 gEasyChatWord_POKEBLOCK[] = _("ポロック");
const u8 gEasyChatWord_Master[] = _("マスター");
const u8 gEasyChatWord_Rank[] = _("ランク");
const u8 gEasyChatWord_Ribbon[] = _("リボン");
const u8 gEasyChatWord_Crush[] = _("クラッシュ");
const u8 gEasyChatWord_Direct[] = _("ダイレクト");
const u8 gEasyChatWord_Tower[] = _("タワー");
const u8 gEasyChatWord_Union[] = _("ユニオン");
const u8 gEasyChatWord_Room[] = _("ルーム");
const u8 gEasyChatWord_Wireless[] = _("ワイヤレス");
const u8 gEasyChatWord_Frontier[] = _("フロンティア");

const struct EasyChatWordInfo gEasyChatGroup_Events[] = {
    [EC_INDEX(EC_WORD_APPEAL)] =
    {
        .text = COMPOUND_STRING("アピール"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_APPEAL),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EVENTS)] =
    {
        .text = COMPOUND_STRING("イベント"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BATTLE_ROOM),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_STAY_AT_HOME)] =
    {
        .text = COMPOUND_STRING("おるすばん"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BATTLE_TOWER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BERRY)] =
    {
        .text = COMPOUND_STRING("きのみ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BERRY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CONTEST)] =
    {
        .text = COMPOUND_STRING("コンテスト"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BLEND),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MC)] =
    {
        .text = COMPOUND_STRING("しかいしゃ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_CONTEST),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_JUDGE)] =
    {
        .text = COMPOUND_STRING("しんさいん"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_CRUSH),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SUPER)] =
    {
        .text = COMPOUND_STRING("スーパー"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DIRECT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_STAGE)] =
    {
        .text = COMPOUND_STRING("ステージ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_EVENTS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HALL_OF_FAME)] =
    {
        .text = COMPOUND_STRING("でんどういり"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_EVOLUTION),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EVOLUTION)] =
    {
        .text = COMPOUND_STRING("ノーマル"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FRONTIER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HYPER)] =
    {
        .text = COMPOUND_STRING("ハイパー"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HALL_OF_FAME),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BATTLE_TOWER)] =
    {
        .text = COMPOUND_STRING("バトルタワー"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HIDDEN),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LEADERS)] =
    {
        .text = COMPOUND_STRING("バトルリーダー"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HYPER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BATTLE_ROOM)] =
    {
        .text = COMPOUND_STRING("バトルルーム"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_JUDGE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HIDDEN)] =
    {
        .text = COMPOUND_STRING("ひでん"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LEADERS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SECRET_BASE)] =
    {
        .text = COMPOUND_STRING("ひみつきち"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MASTER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BLEND)] =
    {
        .text = COMPOUND_STRING("ブレンド"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MC),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_POKEBLOCK)] =
    {
        .text = COMPOUND_STRING("ポロック"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_POKEBLOCK),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MASTER)] =
    {
        .text = COMPOUND_STRING("マスター"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_RANK),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RANK)] =
    {
        .text = COMPOUND_STRING("ランク"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_RIBBON),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RIBBON)] =
    {
        .text = COMPOUND_STRING("リボン"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ROOM),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CRUSH)] =
    {
        .text = COMPOUND_STRING("クラッシュ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SECRET_BASE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DIRECT)] =
    {
        .text = COMPOUND_STRING("ダイレクト"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_STAGE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TOWER)] =
    {
        .text = COMPOUND_STRING("タワー"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_STAY_AT_HOME),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_UNION)] =
    {
        .text = COMPOUND_STRING("ユニオン"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SUPER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ROOM)] =
    {
        .text = COMPOUND_STRING("ルーム"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TOWER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WIRELESS)] =
    {
        .text = COMPOUND_STRING("ワイヤレス"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_UNION),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FRONTIER)] =
    {
        .text = COMPOUND_STRING("フロンティア"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WIRELESS),
        .enabled = TRUE,
    },
};
