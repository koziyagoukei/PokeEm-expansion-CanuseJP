const struct EasyChatWordInfo gEasyChatGroup_Conditions[] = {
    [EC_INDEX(EC_WORD_HOT)] =
    {
        .text = COMPOUND_STRING("あつい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ABSENT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EXISTS)] =
    {
        .text = COMPOUND_STRING("あった"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ALONE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EXCESS)] =
    {
        .text = COMPOUND_STRING("あまり"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_AMUSING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_APPROVED)] =
    {
        .text = COMPOUND_STRING("あり"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_APPROVED),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HAS)] =
    {
        .text = COMPOUND_STRING("ある"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_AWFUL),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GOOD)] =
    {
        .text = COMPOUND_STRING("いい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BADLY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LESS)] =
    {
        .text = COMPOUND_STRING("いか"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BECOMES),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MOMENTUM)] =
    {
        .text = COMPOUND_STRING("いきおい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BEING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GOING)] =
    {
        .text = COMPOUND_STRING("いける"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BEST),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WEIRD)] =
    {
        .text = COMPOUND_STRING("いじょう"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BIG),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BUSY)] =
    {
        .text = COMPOUND_STRING("いそがしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BORED),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TOGETHER)] =
    {
        .text = COMPOUND_STRING("いっしょに"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_BUSY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FULL)] =
    {
        .text = COMPOUND_STRING("いっぱい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_CLOSE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ABSENT)] =
    {
        .text = COMPOUND_STRING("いない"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_COLD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BEING)] =
    {
        .text = COMPOUND_STRING("いや"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_CORRECT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NEED)] =
    {
        .text = COMPOUND_STRING("いる"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DIFFERENT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TASTY)] =
    {
        .text = COMPOUND_STRING("うまい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_DOCILE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SKILLED)] =
    {
        .text = COMPOUND_STRING("うまく"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ELSE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NOISY)] =
    {
        .text = COMPOUND_STRING("うるさい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_ENTERTAINING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BIG)] =
    {
        .text = COMPOUND_STRING("おおきい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_EXCELLENT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LATE)] =
    {
        .text = COMPOUND_STRING("おくれ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_EXCESS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CLOSE)] =
    {
        .text = COMPOUND_STRING("おしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_EXISTS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DOCILE)] =
    {
        .text = COMPOUND_STRING("おとなしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_EXPENSIVE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AMUSING)] =
    {
        .text = COMPOUND_STRING("おもしろい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FABULOUS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ENTERTAINING)] =
    {
        .text = COMPOUND_STRING("おもしろく"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FAST),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PERFECTION)] =
    {
        .text = COMPOUND_STRING("かんぺき"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_FULL),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PRETTY)] =
    {
        .text = COMPOUND_STRING("けっこう"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GOING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HEALTHY)] =
    {
        .text = COMPOUND_STRING("げんき"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_GOOD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EXCELLENT)] =
    {
        .text = COMPOUND_STRING("さいこう"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HAS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_UPSIDE_DOWN)] =
    {
        .text = COMPOUND_STRING("さかさま"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HEALTHY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_COLD)] =
    {
        .text = COMPOUND_STRING("さむい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_HOT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_REFRESHING)] =
    {
        .text = COMPOUND_STRING("さわやか"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_IMPOSSIBLE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_UNAVOIDABLE)] =
    {
        .text = COMPOUND_STRING("しかたない"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_KIND),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MUCH)] =
    {
        .text = COMPOUND_STRING("すごい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LACKS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_OVERWHELMING)] =
    {
        .text = COMPOUND_STRING("すごすぎ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LATE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FABULOUS)] =
    {
        .text = COMPOUND_STRING("すてき"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LESS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ELSE)] =
    {
        .text = COMPOUND_STRING("たいした"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LOUSY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EXPENSIVE)] =
    {
        .text = COMPOUND_STRING("たかい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LOW),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CORRECT)] =
    {
        .text = COMPOUND_STRING("ただしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_LUKEWARM),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_IMPOSSIBLE)] =
    {
        .text = COMPOUND_STRING("だめ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MISTAKE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SMALL)] =
    {
        .text = COMPOUND_STRING("ちいさい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MOMENTUM),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DIFFERENT)] =
    {
        .text = COMPOUND_STRING("ちがう"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MUCH),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TIRED)] =
    {
        .text = COMPOUND_STRING("つかれ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_MYSTERY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SKILL)] =
    {
        .text = COMPOUND_STRING("とくい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_NATURAL),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TOP)] =
    {
        .text = COMPOUND_STRING("トップ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_NEED),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NON_STOP)] =
    {
        .text = COMPOUND_STRING("とまらない"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_NOISY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PREPOSTEROUS)] =
    {
        .text = COMPOUND_STRING("とんでもない"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_NON_STOP),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NONE)] =
    {
        .text = COMPOUND_STRING("ない"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_NONE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NOTHING)] =
    {
        .text = COMPOUND_STRING("なかった"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_NOTHING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_NATURAL)] =
    {
        .text = COMPOUND_STRING("なし"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_OVERWHELMING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BECOMES)] =
    {
        .text = COMPOUND_STRING("なって"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_PERFECTION),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LUKEWARM)] =
    {
        .text = COMPOUND_STRING("ぬるい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_PREPOSTEROUS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FAST)] =
    {
        .text = COMPOUND_STRING("はやい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_PRETTY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LOW)] =
    {
        .text = COMPOUND_STRING("ひくい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_REFRESHING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_AWFUL)] =
    {
        .text = COMPOUND_STRING("ひどい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SECRET),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ALONE)] =
    {
        .text = COMPOUND_STRING("ひとりで"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SEEMS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BORED)] =
    {
        .text = COMPOUND_STRING("ひま"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SIMPLE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SECRET)] =
    {
        .text = COMPOUND_STRING("ひみつ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SKILL),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MYSTERY)] =
    {
        .text = COMPOUND_STRING("ふしぎ"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SKILLED),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LACKS)] =
    {
        .text = COMPOUND_STRING("ふそく"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_SMALL),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BEST)] =
    {
        .text = COMPOUND_STRING("ベスト"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TASTY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LOUSY)] =
    {
        .text = COMPOUND_STRING("へた"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TIRED),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MISTAKE)] =
    {
        .text = COMPOUND_STRING("まちがって"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TOGETHER),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_KIND)] =
    {
        .text = COMPOUND_STRING("やさしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_TOP),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WELL)] =
    {
        .text = COMPOUND_STRING("よく"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_UNAVOIDABLE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WEAKENED)] =
    {
        .text = COMPOUND_STRING("よわって"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_UPSIDE_DOWN),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SIMPLE)] =
    {
        .text = COMPOUND_STRING("らく"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WEAKENED),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SEEMS)] =
    {
        .text = COMPOUND_STRING("らしい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WEIRD),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BADLY)] =
    {
        .text = COMPOUND_STRING("わるい"),
        .alphabeticalOrder = EC_INDEX(EC_WORD_WELL),
        .enabled = TRUE,
    },
};
