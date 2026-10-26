// The contest's scenes about the world (ContestStory_EventScript_LobbyScene): the lines that change
// with the rescues so far, by scene, by speaker (CONTEST_SCENE_*), and by STORY_RESCUE_STAGE_COUNT
// (four or more rescued share the last). {STR_VAR_2} is the one person rescued so far.

static const u8 *const sContestSceneBranches[CONTEST_STORY_SCENE_COUNT][2][STORY_RESCUE_STAGE_COUNT] =
{
    // The applause: the venue overlaps the world of minds; the network's people are truly happy.
    {
        [CONTEST_SCENE_YORIKO] =
        {
            COMPOUND_STRING("{JPN}ヨリコ『はくしゅは ひとつも\nかけて いないわ。\pみんな ここに いるもの。』"),
            COMPOUND_STRING("{JPN}ヨリコ『……ただ ほんの すこし\nはくしゅが ちいさく なったの。\pだれか ひとり ぶん……。\nきの せい よね。』"),
            COMPOUND_STRING("{JPN}ヨリコ『……ただ はくしゅが\nすこし ちいさく なったの。\pふたり ぶん くらい……。\nきの せい かしら。』"),
            COMPOUND_STRING("{JPN}ヨリコ『……さいきん はくしゅの\nところどころに すきまが あるの。\p……きの せい じゃ\nないのかも しれないわね。』"),
            COMPOUND_STRING("{JPN}ヨリコ『はくしゅ ずいぶん\nしずかに なったわ。\pそれでも あたたかいの。\nなのに すきまの ほうに\lみみが いって しまうの。』"),
        },
        [CONTEST_SCENE_DARKRAI] =
        {
            COMPOUND_STRING("{JPN}ダークライ『わたしたちが よびもどす\nあいては みな その\lしあわせの なかに いる。\p……それを わすれないで ほしい。』"),
            COMPOUND_STRING("{JPN}ダークライ『{STR_VAR_2}も そうだった。\pつながりの しあわせを おしみながら\nそれでも じぶんで はなれる ことを\lえらんだのだ。\pヒトポケルスが しせつに すがたを\nあらわせたのも ここが こころの\lせかいと かさなる ばしょ だからだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『あなたが たすけた ふたりも\nその しあわせを おしみながら\lじぶんで えらんで はなれた。\pしあわせを うばったのでは ない。\nえらぶ ための こえを とどけたのだ。\pヒトポケルスが しせつに あらわれるのも\nこの かさなりの ためだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『たすけた 3にんの ぶんだけ\nはくしゅは ちいさく なった。\pだが それを さびしいと\nかんじられるのは\lヨリコが ヨリコで ある しるしだ。\pヒトポケルスが しせつに あらわれるのも\nこの かさなりの ためだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『おおくの ものが はなれ\nはくしゅは しずかに なった。\pそれでも まだ この かいじょうを\nみたす ほどの こころが\lつながりの なかに のこっている。\pヒトポケルスが しせつに あらわれるのも\nこの かさなりの ためだ。』"),
        },
    },
    // The handshake: how it spreads, who it cannot reach, and the mochi that binds even them.
    {
        [CONTEST_SCENE_YORIKO] =
        {
            COMPOUND_STRING("{JPN}ヨリコ『みんなとは ふれるだけで\nわかりあえるのに。』"),
            COMPOUND_STRING("{JPN}ヨリコ『{STR_VAR_2}の て……\nさいごに にぎったのは\lいつ だったかしら。\p……もう おもいだせないの。』"),
            COMPOUND_STRING("{JPN}ヨリコ『はなれた ふたりとも\nむかしは こうして\lてを つないだ きが するの。』"),
            COMPOUND_STRING("{JPN}ヨリコ『はなれた ひとたちの\nてのひらの ぬくもり……\pすこしずつ わすれて いくのね。』"),
            COMPOUND_STRING("{JPN}ヨリコ『てを つなげる ひとが\nへって いくの。\p……なのに いまの あくしゅ\nふしぎと あたたかかったわ。』"),
        },
        [CONTEST_SCENE_DARKRAI] =
        {
            COMPOUND_STRING("{JPN}ダークライ『つながりの そとに いる\nあなただから こそ\lその こえは とどく。\pてんしゅが あなたに たのんだのも\nその ためだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『そして ヒトポケルスから\nなおった ものには\lもう にどと うつらない。\p{STR_VAR_2}が リゾートで あんしんして\nくらせるのも その ためだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『そして なおった ものには\nもう にどと うつらない。\pたすけた ふたりが つながりへ\nひきもどされる ことは ないのだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『そして なおった ものには\nもう にどと うつらない。\pたすけた 3にんは もう\nじぶんの てで だれかと\lふれあえるのだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『そして なおった ものには\nもう にどと うつらない。\pたすけた ものたちは いま\nじぶんの てで ふれあっている。\p……いつか ヨリコの てが\nふれる さきも そう なるはずだ。』"),
        },
    },
    // The player's partners: brought here against their will, and safe for their old Pokérus.
    {
        [CONTEST_SCENE_YORIKO] =
        {
            COMPOUND_STRING("{JPN}ヨリコ『わたしと ハブネークは\nきもちが ぜんぶ つながってるの。\pだから えんりょ なんて しないわ。\n……その はず よね。』"),
            COMPOUND_STRING("{JPN}ヨリコ『{STR_VAR_2}の ポケモンは\nいま どうしてるのかしら。\pあいぼうの きもちが\nわからなく なって……\lさみしく ないのかしら。』"),
            COMPOUND_STRING("{JPN}ヨリコ『はなれた ふたりの ポケモンは\nいま どんな かお なのかしら。\pきもちが わからなくても\nいっしょに いられるのかしら。』"),
            COMPOUND_STRING("{JPN}ヨリコ『はなれた ひとたちと ポケモン\nわかりあえなくても\lいっしょに いるのかしら。\p……すこし きに なるの。』"),
            COMPOUND_STRING("{JPN}ヨリコ『ハブネークの きもち\nぜんぶ わかる はず なのに……\pさいきん ときどき\nわからなく なるの。\p……みんなの こえが\nへった から かしら。』"),
        },
        [CONTEST_SCENE_DARKRAI] =
        {
            COMPOUND_STRING("{JPN}ダークライ『なついて いなくても\nあの こたちは あなたの\lしじに こたえて きた。\p……いつか あなたを みとめる ひも\nくるはずだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『{STR_VAR_2}を たすけた\nたたかいを あの こたちも\lそばで みていた。\pすこしずつ あなたを みる めが\nかわって きている。』"),
            COMPOUND_STRING("{JPN}ダークライ『ふたりを たすけた\nたたかいを あの こたちも\lみてきた。\pあなたが なんの ために たたかうのか\nすこしずつ わかって きている\lはずだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『3にんを たすけるまで\nあの こたちは あなたと\lたたかい ぬいた。\p……ことばは なくても\nそれが あの こたちの\lこたえ なのだろう。』"),
            COMPOUND_STRING("{JPN}ダークライ『これほどの たたかいを\nともに してきたのだ。\pあの こたちは もう\nあなたの ことを よく しっている。\p……なつくか どうかは\nあの こたちが じぶんで きめる。\lそれで いいのだ。』"),
        },
    },
    // The same day: a world that neither moves on nor back, and why the player calls people back.
    {
        [CONTEST_SCENE_YORIKO] =
        {
            COMPOUND_STRING("{JPN}ヨリコ『みんな まいにち おなじように\nわらって はくしゅ して……\pそれで じゅうぶん しあわせなの。』"),
            COMPOUND_STRING("{JPN}ヨリコ『{STR_VAR_2}は いま\nどんな あしたを すごしてるのかしら。\p……すこし きに なって しまうの。』"),
            COMPOUND_STRING("{JPN}ヨリコ『はなれた ふたりは いま\nきのうと ちがう きょうを\lすごして いるのかしら。』"),
            COMPOUND_STRING("{JPN}ヨリコ『はなれた ひとたちは もう\nきのうと ちがう きょうに いるのね。\p……すこし うらやましい なんて\nへんよね。』"),
            COMPOUND_STRING("{JPN}ヨリコ『はなれた ひとたちの あしたは\nきっと ばらばら なのね。\p……こわい はず なのに\nどうして きに なるのかしら。』"),
        },
        [CONTEST_SCENE_DARKRAI] =
        {
            COMPOUND_STRING("{JPN}ダークライ『まだ だれも\nもどっては いない。\p……だが あなたなら できる。\nわたしは そう しんじている。』"),
            COMPOUND_STRING("{JPN}ダークライ『{STR_VAR_2}は いま リゾートで\nあしたの ことを かんがえている。\pそれが もどってくる ということだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『もどった ふたりは もう\nそれぞれの あしたを はじめている。\pヨリコにも その みちは\nひらかれている はずだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『もどった 3にんは もう\nばらばらの あしたを あるいている。\p……ばらばら だからこそ\nたがいに であえるのだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『もどった ものたちは\nそれぞれの あしたを いきている。\pつぎは ヨリコの ばんだ。』"),
        },
    },
};
