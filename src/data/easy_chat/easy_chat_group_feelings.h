const u8 gEasyChatWord_Meet[] = _("あいたい");
const u8 gEasyChatWord_Play[] = _("あそびたい");
const u8 gEasyChatWord_Hurried[] = _("あわてて");
const u8 gEasyChatWord_Goes[] = _("いきたい");
const u8 gEasyChatWord_Giddy[] = _("うかれて");
const u8 gEasyChatWord_Happy[] = _("うれしい");
const u8 gEasyChatWord_Happiness[] = _("うれしさ");
const u8 gEasyChatWord_Excite[] = _("エキサイト");
const u8 gEasyChatWord_Important[] = _("えらい");
const u8 gEasyChatWord_Funny[] = _("おかしい");
const u8 gEasyChatWord_Got[] = _("オッケー");
const u8 gEasyChatWord_GoHome[] = _("かえりたい");
const u8 gEasyChatWord_Disappointed[] = _("がっかり");
const u8 gEasyChatWord_Disappoints[] = _("がっくし");
const u8 gEasyChatWord_Sad[] = _("かなしい");
const u8 gEasyChatWord_Try[] = _("がんばって");
const u8 gEasyChatWord_Tries[] = _("がんばる");
const u8 gEasyChatWord_Hears[] = _("きがしない");
const u8 gEasyChatWord_Think[] = _("きがする");
const u8 gEasyChatWord_Hear[] = _("ききたい");
const u8 gEasyChatWord_Wants[] = _("きになる");
const u8 gEasyChatWord_Misheard[] = _("きのせい");
const u8 gEasyChatWord_Dislike[] = _("きらい");
const u8 gEasyChatWord_Angry[] = _("くやしい");
const u8 gEasyChatWord_Anger[] = _("くやしさ");
const u8 gEasyChatWord_Scary[] = _("こわい");
const u8 gEasyChatWord_Lonesome[] = _("さみしい");
const u8 gEasyChatWord_Disappoint[] = _("ざんねん");
const u8 gEasyChatWord_Joy[] = _("しあわせ");
const u8 gEasyChatWord_Gets[] = _("したい");
const u8 gEasyChatWord_Never[] = _("したくない");
const u8 gEasyChatWord_Darn[] = _("しまった");
const u8 gEasyChatWord_Downcast[] = _("しょんぼり");
const u8 gEasyChatWord_Incredible[] = _("しんじられない");
const u8 gEasyChatWord_Likes[] = _("すき");
const u8 gEasyChatWord_Dislikes[] = _("だいきらい");
const u8 gEasyChatWord_Boring[] = _("たいくつ");
const u8 gEasyChatWord_Care[] = _("だいじ");
const u8 gEasyChatWord_Cares[] = _("だいじに");
const u8 gEasyChatWord_AllRight[] = _("だいじょうぶ");
const u8 gEasyChatWord_Adore[] = _("だいすき");
const u8 gEasyChatWord_Disaster[] = _("たいへん");
const u8 gEasyChatWord_Enjoy[] = _("たのしい");
const u8 gEasyChatWord_Enjoys[] = _("たのしすぎ");
const u8 gEasyChatWord_Eat[] = _("たべたい");
const u8 gEasyChatWord_Lacking[] = _("たりない");
const u8 gEasyChatWord_Bad[] = _("ちくしょー");
const u8 gEasyChatWord_Hard[] = _("つらい");
const u8 gEasyChatWord_Terrible[] = _("つらかった");
const u8 gEasyChatWord_Should[] = _("どうしよう");
const u8 gEasyChatWord_Nice[] = _("ナイス");
const u8 gEasyChatWord_Drink[] = _("のみたい");
const u8 gEasyChatWord_Surprise[] = _("びっくり");
const u8 gEasyChatWord_Fear[] = _("ふあん");
const u8 gEasyChatWord_Want[] = _("ほしい");
const u8 gEasyChatWord_Wait[] = _("まてない");
const u8 gEasyChatWord_Satisfied[] = _("まんぞく");
const u8 gEasyChatWord_See[] = _("みたい");
const u8 gEasyChatWord_Rare[] = _("めずらしい");
const u8 gEasyChatWord_Negative[] = _("やだ");
const u8 gEasyChatWord_Done[] = _("やったー");
const u8 gEasyChatWord_Danger[] = _("やばい");
const u8 gEasyChatWord_Defeated[] = _("やられた");
const u8 gEasyChatWord_Beat[] = _("やられて");
const u8 gEasyChatWord_Great[] = _("よかった");
const u8 gEasyChatWord_Romantic[] = _("ロマン");
const u8 gEasyChatWord_Question[] = _("ろんがい");
const u8 gEasyChatWord_Understand[] = _("わから");
const u8 gEasyChatWord_Understands[] = _("わかり");

const struct EasyChatWordInfo gEasyChatGroup_Feelings[] = {
    [EC_INDEX(EC_WORD_MEET)] =
    {
        .text = COMPOUND_STRING("あいたい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ADORE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PLAY)] =
    {
        .text = COMPOUND_STRING("あそびたい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ALL_RIGHT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HURRIED)] =
    {
        .text = COMPOUND_STRING("あわてて"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ANGER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GOES)] =
    {
        .text = COMPOUND_STRING("いきたい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ANGRY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GIDDY)] =
    {
        .text = COMPOUND_STRING("うかれて"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BAD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HAPPY)] =
    {
        .text = COMPOUND_STRING("うれしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BEAT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HAPPINESS)] =
    {
        .text = COMPOUND_STRING("うれしさ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BORING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EXCITE)] =
    {
        .text = COMPOUND_STRING("エキサイト"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_CARE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_IMPORTANT)] =
    {
        .text = COMPOUND_STRING("えらい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_CARES),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FUNNY)] =
    {
        .text = COMPOUND_STRING("おかしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DANGER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GOT)] =
    {
        .text = COMPOUND_STRING("オッケー"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DARN),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GO_HOME)] =
    {
        .text = COMPOUND_STRING("かえりたい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DEFEATED),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DISAPPOINTED)] =
    {
        .text = COMPOUND_STRING("がっかり"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DISAPPOINT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DISAPPOINTS)] =
    {
        .text = COMPOUND_STRING("がっくし"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DISAPPOINTED),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SAD)] =
    {
        .text = COMPOUND_STRING("かなしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DISAPPOINTS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TRY)] =
    {
        .text = COMPOUND_STRING("がんばって"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DISASTER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TRIES)] =
    {
        .text = COMPOUND_STRING("がんばる"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DISLIKE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HEARS)] =
    {
        .text = COMPOUND_STRING("きがしない"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DISLIKES),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THINK)] =
    {
        .text = COMPOUND_STRING("きがする"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DONE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HEAR)] =
    {
        .text = COMPOUND_STRING("ききたい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DOWNCAST),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WANTS)] =
    {
        .text = COMPOUND_STRING("きになる"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DRINK),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MISHEARD)] =
    {
        .text = COMPOUND_STRING("きのせい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_EAT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DISLIKE)] =
    {
        .text = COMPOUND_STRING("きらい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ENJOY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ANGRY)] =
    {
        .text = COMPOUND_STRING("くやしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ENJOYS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ANGER)] =
    {
        .text = COMPOUND_STRING("くやしさ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_EXCITE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SCARY)] =
    {
        .text = COMPOUND_STRING("こわい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FEAR),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LONESOME)] =
    {
        .text = COMPOUND_STRING("さみしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FUNNY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DISAPPOINT)] =
    {
        .text = COMPOUND_STRING("ざんねん"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GETS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_JOY)] =
    {
        .text = COMPOUND_STRING("しあわせ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GIDDY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GETS)] =
    {
        .text = COMPOUND_STRING("したい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GO_HOME),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NEVER)] =
    {
        .text = COMPOUND_STRING("したくない"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GOES),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DARN)] =
    {
        .text = COMPOUND_STRING("しまった"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GOT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DOWNCAST)] =
    {
        .text = COMPOUND_STRING("しょんぼり"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GREAT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_INCREDIBLE)] =
    {
        .text = COMPOUND_STRING("しんじられない"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HAPPINESS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LIKES)] =
    {
        .text = COMPOUND_STRING("すき"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HAPPY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DISLIKES)] =
    {
        .text = COMPOUND_STRING("だいきらい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HARD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BORING)] =
    {
        .text = COMPOUND_STRING("たいくつ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HEAR),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CARE)] =
    {
        .text = COMPOUND_STRING("だいじ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HEARS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CARES)] =
    {
        .text = COMPOUND_STRING("だいじに"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HURRIED),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ALL_RIGHT)] =
    {
        .text = COMPOUND_STRING("だいじょうぶ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_IMPORTANT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ADORE)] =
    {
        .text = COMPOUND_STRING("だいすき"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_INCREDIBLE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DISASTER)] =
    {
        .text = COMPOUND_STRING("たいへん"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_JOY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ENJOY)] =
    {
        .text = COMPOUND_STRING("たのしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LACKING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ENJOYS)] =
    {
        .text = COMPOUND_STRING("たのしすぎ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LIKES),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EAT)] =
    {
        .text = COMPOUND_STRING("たべたい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LONESOME),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LACKING)] =
    {
        .text = COMPOUND_STRING("たりない"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MEET),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BAD)] =
    {
        .text = COMPOUND_STRING("ちくしょー"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MISHEARD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HARD)] =
    {
        .text = COMPOUND_STRING("つらい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_NEGATIVE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TERRIBLE)] =
    {
        .text = COMPOUND_STRING("つらかった"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_NEVER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SHOULD)] =
    {
        .text = COMPOUND_STRING("どうしよう"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_NICE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NICE)] =
    {
        .text = COMPOUND_STRING("ナイス"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_PLAY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DRINK)] =
    {
        .text = COMPOUND_STRING("のみたい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_QUESTION),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SURPRISE)] =
    {
        .text = COMPOUND_STRING("びっくり"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_RARE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FEAR)] =
    {
        .text = COMPOUND_STRING("ふあん"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ROMANTIC),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WANT)] =
    {
        .text = COMPOUND_STRING("ほしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SAD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WAIT)] =
    {
        .text = COMPOUND_STRING("まてない"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SATISFIED),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SATISFIED)] =
    {
        .text = COMPOUND_STRING("まんぞく"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SCARY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SEE)] =
    {
        .text = COMPOUND_STRING("みたい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SEE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RARE)] =
    {
        .text = COMPOUND_STRING("めずらしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SHOULD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NEGATIVE)] =
    {
        .text = COMPOUND_STRING("やだ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SURPRISE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DONE)] =
    {
        .text = COMPOUND_STRING("やったー"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TERRIBLE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DANGER)] =
    {
        .text = COMPOUND_STRING("やばい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_THINK),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DEFEATED)] =
    {
        .text = COMPOUND_STRING("やられた"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TRIES),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BEAT)] =
    {
        .text = COMPOUND_STRING("やられて"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TRY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GREAT)] =
    {
        .text = COMPOUND_STRING("よかった"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_UNDERSTAND),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ROMANTIC)] =
    {
        .text = COMPOUND_STRING("ロマン"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_UNDERSTANDS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_QUESTION)] =
    {
        .text = COMPOUND_STRING("ろんがい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WAIT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_UNDERSTAND)] =
    {
        .text = COMPOUND_STRING("わから"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WANT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_UNDERSTANDS)] =
    {
        .text = COMPOUND_STRING("わかり"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WANTS),
        .enabled = TRUE,
    },
};
