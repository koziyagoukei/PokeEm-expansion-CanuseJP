// The contest's scenes about the world (ContestStory_EventScript_LobbyScene): the lines that change
// with the rescues so far, by scene, by speaker (CONTEST_SCENE_*), and by STORY_RESCUE_STAGE_COUNT
// (four or more rescued share the last). {STR_VAR_2} is the one person rescued so far.

static const u8 *const sContestSceneBranches[CONTEST_STORY_SCENE_COUNT][2][STORY_RESCUE_STAGE_COUNT] =
{
    // The applause: the venue overlaps the world of minds; the network's people are truly happy.
    {
        [CONTEST_SCENE_YORIKO] =
        {
            COMPOUND_STRING("{JPN}ヨリコ『拍手は ひとつも\n欠けていないの。\pみんな ここにいるから。』"),
            COMPOUND_STRING("{JPN}ヨリコ『……ただ ほんの少し\n拍手が 小さくなったの。\p誰か一人分 くらい……。\n気のせいよね。』"),
            COMPOUND_STRING("{JPN}ヨリコ『……ただ 拍手が\n少し 小さくなったの。\p二人分くらい……。\n気のせいかしら。』"),
            COMPOUND_STRING("{JPN}ヨリコ『……最近 拍手の\n所々に 隙間があるの。\p……気のせいじゃ\nないのかもしれないわね。』"),
            COMPOUND_STRING("{JPN}ヨリコ『拍手は ずいぶん\n静かになったわ。\pそれでも あたたかいの。\nなのに 隙間のほうに\l耳がいってしまうの。』"),
        },
        [CONTEST_SCENE_DARKRAI] =
        {
            COMPOUND_STRING("{JPN}ダークライ『私達が 呼び戻す相手は\n皆 その幸福の中にいる。\p……それを 忘れないでくれ。』"),
            COMPOUND_STRING("{JPN}ダークライ『{STR_VAR_2}も そうだった。\p繋がりの幸福を 惜しみながら \nそれでも 自ら離別を選択したのだ。\pヒトポケルスが ここに姿を\n見せたのも この場所が\l心象世界と 重なる空間だからだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『あなたが 助けた二人も\nその幸福を 理解しながら \l自ら離別を 選んだ。\p幸福を 奪ったのではない。\n自分で選びたいと 願ったのだ。\pヒトポケルスが ここに現れるのも\nこの重なりの ためだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『離れた三人の分だけ\n拍手は 小さくなった。\pだが それを 寂しいと感じるのは\nヨリコが ヨリコである証明だ。\pヒトポケルスが ここに出てくるのも\n離れてほしくないと 願うためだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『多くの者が 離れ \n賞賛の声は 少なくなった。\pそれでも まだ この会場を\n満たすほどの心が 繋がりの中に残る。\pヒトポケルス……というより \n人の情念 故なのだろう。』"),
        },
    },
    // The handshake: how it spreads, who it cannot reach, and the mochi that binds even them.
    {
        [CONTEST_SCENE_YORIKO] =
        {
            COMPOUND_STRING("{JPN}ヨリコ『皆とは 触れるだけで\n分かり合えるのに。』"),
            COMPOUND_STRING("{JPN}ヨリコ『{STR_VAR_2}の手……。\n最後に 握ったのは\lいつだったかしら。\p……もう 思い出せないの。』"),
            COMPOUND_STRING("{JPN}ヨリコ『離れた誰かとも\n昔は こうして\l手を繋いだ気がするの。』"),
            COMPOUND_STRING("{JPN}ヨリコ『離れた皆の\nぬくもりの残滓……。\p少しずつ 忘れていくのね。』"),
            COMPOUND_STRING("{JPN}ヨリコ『手を繋げる人が\n減っていくの。\p……ああ 今の握手。\nずいぶんと あたたかかったわ。』"),
        },
        [CONTEST_SCENE_DARKRAI] =
        {
            COMPOUND_STRING("{JPN}ダークライ『繋がりの外にいる\nあなただからこそ その声は届く。\p彼が あなたに頼んだのも\nそのためだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『そして ヒトポケルスから\n完治した者には 二度と感染しない。\p{STR_VAR_2}が リゾートで安心して\n生活できるのも そのためだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『そして ヒトポケルスから\n完治した者には 二度と感染しない。\p彼ら二人が 鎖の中へ\n引き戻されることは ないのだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『そして ヒトポケルスから\n完治した者には 二度と感染しない。\p助けた三人は もう 自らの手で\n他人と 触れ合えるのだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『そして ヒトポケルスから\n完治した者には 二度と感染しない。\p助けた者は 今 誰かと\n互いに 手を繋いでいる。\p……いつか ヨリコの手も \n自分以外の温もりを 知るだろう。』"),
        },
    },
    // The player's partners: brought here against their will, and safe for their old Pokérus.
    {
        [CONTEST_SCENE_YORIKO] =
        {
            COMPOUND_STRING("{JPN}ヨリコ『私と ハブネークは\n気持ちが 全部繋がっているの。\pだから 遠慮なんてしないわ。\n……そのはずよね。』"),
            COMPOUND_STRING("{JPN}ヨリコ『{STR_VAR_2}のポケモンは\n今 どうしているのかしら。\p相棒の気持ちが 分からなくなって……\n寂しくないのかしら。』"),
            COMPOUND_STRING("{JPN}ヨリコ『離れた二人の ポケモンは\n今 どんな顔なのかしら。\p気持ちが 分からなくても\n一緒にいられるのかしら。』"),
            COMPOUND_STRING("{JPN}ヨリコ『離れた人達と ポケモンは\n分かり合えなくても\l一緒にいるのかしら。\p……少し 気になるの。』"),
            COMPOUND_STRING("{JPN}ヨリコ『ハブネークの気持ち \n全部 分かるはずなのに……。\p最近 時々\n分からなくなるの。\p……皆の声が\n減ったからかしら。』"),
        },
        [CONTEST_SCENE_DARKRAI] =
        {
            COMPOUND_STRING("{JPN}ダークライ『懐いていなくとも \nあの子達は あなたの指示に応えてきた。\p……いつか あなたを認める日も\n来るはずだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『{STR_VAR_2}を 助けた戦いを\nあの子達も そばで見ていた。\p少しずつ あなたを見る目が\n変わってきている。』"),
            COMPOUND_STRING("{JPN}ダークライ『二人を 助けた戦いを\nあの子達も 見てきた。\pあなたが 何のために戦うのか。\n少しずつ 分かってきているはずだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『三人を 助けるまで \nあの子達は あなたと戦い抜いた。\p……言葉はなくとも それが\nあの子達の 答えなのだろう。』"),
            COMPOUND_STRING("{JPN}ダークライ『これほどの戦いを\n共にしてきたのだ。\pあの子達は もう \nあなたのことを よく知っている。\p……懐くかどうかは\nあの子達が 自分で決める。\lそれでいいのだ。』"),
        },
    },
    // The same day: a world that neither moves on nor back, and why the player calls people back.
    {
        [CONTEST_SCENE_YORIKO] =
        {
            COMPOUND_STRING("{JPN}ヨリコ『皆 毎日 同じように\n笑って 拍手して……。\pそれで 十分幸せなの。』"),
            COMPOUND_STRING("{JPN}ヨリコ『{STR_VAR_2}は 今 \nどんな明日を 過ごしているのかしら。\p……少し 気になってしまうの。』"),
            COMPOUND_STRING("{JPN}ヨリコ『離れた二人は 今 \n昨日と違う今日を\l過ごしているのかしら。』"),
            COMPOUND_STRING("{JPN}ヨリコ『離れた人達は もう \n昨日と違う今日に いるのね。\p……少し 羨ましいなんて。\n変よね。』"),
            COMPOUND_STRING("{JPN}ヨリコ『離れた人達の明日は\nきっと ばらばらなのね。\p……怖いはずなのに。\nどうして 気になるのかしら。』"),
        },
        [CONTEST_SCENE_DARKRAI] =
        {
            COMPOUND_STRING("{JPN}ダークライ『まだ 誰も\n戻ってはいない。\p……だが あなたならできる。\n私は そう信じている。』"),
            COMPOUND_STRING("{JPN}ダークライ『{STR_VAR_2}は 今 リゾートで\n明日のことを 考えている。\pそれが 戻ってくるということだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『戻った二人は もう \nそれぞれの明日を 始めている。\pヨリコにも その道は\n開かれているはずだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『戻った三人は もう \nばらばらの明日を 歩いている。\p……ばらばらだからこそ \n互いに 出会えるのだ。』"),
            COMPOUND_STRING("{JPN}ダークライ『戻った者達は\nそれぞれの明日を 生きている。\p次は ヨリコの番だ。』"),
        },
    },
};
