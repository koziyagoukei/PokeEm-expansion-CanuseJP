// Battle Frontier trainers' own lines, in place of the Easy Chat words in
// battle_frontier_trainers.h (those stay for record mixing and links).
// Order: before the battle, when the trainer wins, when the trainer loses.
static const struct FrontierTrainerTexts sFrontierTrainerTexts[FRONTIER_TRAINERS_COUNT] =
{
    [FRONTIER_TRAINER_BRADY] = { // たんぱんこぞう トキヒト
        COMPOUND_STRING("{JPN}ぼく 負ける気が しないんだ！\n今日は 絶好調だからね！"),
        COMPOUND_STRING("{JPN}へへん！ 言ったとおり\n絶好調だったでしょ？"),
        COMPOUND_STRING("{JPN}あれ……？ 絶好調の はずが……\nなんで 負けちゃったんだろう？"),
    },
    [FRONTIER_TRAINER_CONNER] = { // たんぱんこぞう ヒロミチ
        COMPOUND_STRING("{JPN}勝っても 負けても 楽しもう！\nそれが ぼくの モットーさ！"),
        COMPOUND_STRING("{JPN}やった！ 勝つと もっと\n楽しいね！"),
        COMPOUND_STRING("{JPN}ちぇっ……！ でも 楽しかったから\n怒ったりは しないよ！"),
    },
    [FRONTIER_TRAINER_BRADLEY] = { // たんぱんこぞう モトヨシ
        COMPOUND_STRING("{JPN}おいおい！ きみが 次の 相手？\nかかってきなよ！"),
        COMPOUND_STRING("{JPN}えへへ！ 勝利の においが\nしてたんだよね！"),
        COMPOUND_STRING("{JPN}くそー！ 明日こそ\n本気の 勝負で 勝ってやる！"),
    },
    [FRONTIER_TRAINER_CYBIL] = { // ミニスカート ユカコ
        COMPOUND_STRING("{JPN}わたしと 勝負？\n冗談でしょ？ 手加減しないわよ！"),
        COMPOUND_STRING("{JPN}ほらね！ わたしに\nかなう 子なんて いないの！"),
        COMPOUND_STRING("{JPN}…………うそ！\nわたしが 負けるなんて！"),
    },
    [FRONTIER_TRAINER_RODETTE] = { // ミニスカート マヤ
        COMPOUND_STRING("{JPN}ドキドキするー！\nがんばるから 見ててね！"),
        COMPOUND_STRING("{JPN}きゃー！ 勝っちゃった！\nわたしって 強いでしょ？"),
        COMPOUND_STRING("{JPN}あーあ……\nがんばったのに 負けちゃった。"),
    },
    [FRONTIER_TRAINER_PEGGY] = { // ミニスカート ニナ
        COMPOUND_STRING("{JPN}かわいい ポケモンだって\n強いのよ！ 見せてあげる！"),
        COMPOUND_STRING("{JPN}かわいくて 強い！\nこれが わたしの チームよ！"),
        COMPOUND_STRING("{JPN}かわいさだけじゃ\n勝てないのね……。"),
    },
    [FRONTIER_TRAINER_KEITH] = { // じゅくがえり キミオ
        COMPOUND_STRING("{JPN}タイプ相性は ぜんぶ\n頭に 入っているよ！"),
        COMPOUND_STRING("{JPN}予習 したとおりの\n結果だね！"),
        COMPOUND_STRING("{JPN}教科書には 書いてない\n戦い方だった……！"),
    },
    [FRONTIER_TRAINER_GRAYSON] = { // じゅくがえり キミツグ
        COMPOUND_STRING("{JPN}塾の 帰りに 寄ったんだ。\n息抜きの 勝負を しよう！"),
        COMPOUND_STRING("{JPN}いい 息抜きに なった！\nこれで 勉強も はかどるよ。"),
        COMPOUND_STRING("{JPN}負けた……。 これじゃ\n勉強にも 身が 入らないや。"),
    },
    [FRONTIER_TRAINER_GLENN] = { // じゅくがえり スギオ
        COMPOUND_STRING("{JPN}計算では ぼくの 勝率は\n八割を こえている！"),
        COMPOUND_STRING("{JPN}ほらね！ 計算は\nうそを つかないのさ！"),
        COMPOUND_STRING("{JPN}計算 ちがいだ……！\n残りの 二割を 引いたか……。"),
    },
    [FRONTIER_TRAINER_LILIANA] = { // じゅくがえり アケミ
        COMPOUND_STRING("{JPN}ノートに まとめた 作戦を\nためさせて もらうわね！"),
        COMPOUND_STRING("{JPN}作戦どおり！\nノートに 花丸を つけなきゃ！"),
        COMPOUND_STRING("{JPN}作戦を 一から\n考え直さなくちゃ……。"),
    },
    [FRONTIER_TRAINER_ELISE] = { // じゅくがえり モエカ
        COMPOUND_STRING("{JPN}テストより 勝負の ほうが\nずっと 好きなの！"),
        COMPOUND_STRING("{JPN}勝負なら いつでも\n満点が とれるのよね！"),
        COMPOUND_STRING("{JPN}勝負でも 赤点だなんて……\nショックだわ。"),
    },
    [FRONTIER_TRAINER_ZOEY] = { // じゅくがえり レイミ
        COMPOUND_STRING("{JPN}先生に 教わった ことを\n全部 出しきるわ！"),
        COMPOUND_STRING("{JPN}先生に 報告しなくちゃ！\nちゃんと 勝てましたって！"),
        COMPOUND_STRING("{JPN}先生の 教えが まだ\n身に ついてなかったみたい。"),
    },
    [FRONTIER_TRAINER_MANUEL] = { // おぼっちゃま ケイタロウ
        COMPOUND_STRING("{JPN}ぼくの ポケモンは 一流だよ。\n負けるはずが ないさ。"),
        COMPOUND_STRING("{JPN}当然の 結果だね。\n一流は ちがうだろう？"),
        COMPOUND_STRING("{JPN}そんな……！ パパに\nなんて 言えば いいんだ！"),
    },
    [FRONTIER_TRAINER_RUSS] = { // おぼっちゃま ミキマサ
        COMPOUND_STRING("{JPN}お金で 買えない ものを\n探しに 来たんだ。"),
        COMPOUND_STRING("{JPN}勝利は お金じゃ\n買えないからね。 いい 気分だ。"),
        COMPOUND_STRING("{JPN}負ける くやしさも\nお金では 買えないんだね……。"),
    },
    [FRONTIER_TRAINER_DUSTIN] = { // おぼっちゃま チハル
        COMPOUND_STRING("{JPN}じいやが 言うんだ。\n勝負は 品よく 戦えってね。"),
        COMPOUND_STRING("{JPN}品のある 勝利だったろう？\nじいやも 喜ぶよ。"),
        COMPOUND_STRING("{JPN}品よく 負けるのも\nたしなみ……だよね？"),
    },
    [FRONTIER_TRAINER_TINA] = { // おじょうさま アサエ
        COMPOUND_STRING("{JPN}ごきげんよう。\nお手柔らかに お願いしますわ。"),
        COMPOUND_STRING("{JPN}あら 勝ってしまいましたわ。\nごめんあそばせ。"),
        COMPOUND_STRING("{JPN}まあ……！ わたくしが\n負けるなんて 初めてですわ。"),
    },
    [FRONTIER_TRAINER_GILLIAN1] = { // おじょうさま ユキノ
        COMPOUND_STRING("{JPN}お茶の 時間までに\n勝負を すませましょう。"),
        COMPOUND_STRING("{JPN}これで ゆっくり\nお茶が いただけますわ。"),
        COMPOUND_STRING("{JPN}今日の お茶は\n少し にがく なりそうですわ。"),
    },
    [FRONTIER_TRAINER_ZOE] = { // おじょうさま コウコ
        COMPOUND_STRING("{JPN}わたくしの ポケモンたち\n毎日 手入れを していますのよ。"),
        COMPOUND_STRING("{JPN}手入れの 成果ですわね。\nみんな きれいでしょう？"),
        COMPOUND_STRING("{JPN}きれいなだけでは\n勝てないのですね……。"),
    },
    [FRONTIER_TRAINER_CHEN] = { // キャンプボーイ マヒロ
        COMPOUND_STRING("{JPN}山で きたえた 体力なら\nだれにも 負けないぞ！"),
        COMPOUND_STRING("{JPN}よーし！ 今夜は\nたき火を 囲んで お祝いだ！"),
        COMPOUND_STRING("{JPN}まいった！ 山へ 戻って\n一から きたえ直すよ！"),
    },
    [FRONTIER_TRAINER_AL] = { // キャンプボーイ ヒビキ
        COMPOUND_STRING("{JPN}テントを たたんで\nここまで 来たんだ！"),
        COMPOUND_STRING("{JPN}来た かいが あった！\n最高の 一日だ！"),
        COMPOUND_STRING("{JPN}今夜は 星を 見ながら\n反省会だな……。"),
    },
    [FRONTIER_TRAINER_MITCH] = { // キャンプボーイ ツカサ
        COMPOUND_STRING("{JPN}キャンプで 覚えた\nサバイバル戦法を 見せるぜ！"),
        COMPOUND_STRING("{JPN}サバイバルは\n生き残った ほうの 勝ちさ！"),
        COMPOUND_STRING("{JPN}ここで 生き残るのは\n大変 だなあ……。"),
    },
    [FRONTIER_TRAINER_ANNE] = { // ピクニックガール ナオカ
        COMPOUND_STRING("{JPN}お弁当を 食べたら\n元気 いっぱい！ 勝負よ！"),
        COMPOUND_STRING("{JPN}ね？ お弁当パワーは\nすごいでしょ？"),
        COMPOUND_STRING("{JPN}おなかが すいて\n力が 出なかったかも……。"),
    },
    [FRONTIER_TRAINER_ALIZE] = { // ピクニックガール トシエ
        COMPOUND_STRING("{JPN}お花畑で 育てた\nポケモンたちよ！"),
        COMPOUND_STRING("{JPN}お花みたいに\nきれいに 勝てたわ！"),
        COMPOUND_STRING("{JPN}お花が しおれちゃった\nみたいな 気分……。"),
    },
    [FRONTIER_TRAINER_LAUREN] = { // ピクニックガール コトエ
        COMPOUND_STRING("{JPN}晴れた 日の 勝負って\n気持ちいいわよね！"),
        COMPOUND_STRING("{JPN}いい 天気に\nいい 勝負！ 最高だわ！"),
        COMPOUND_STRING("{JPN}負けちゃったけど\n空は 青いし まあ いいか！"),
    },
    [FRONTIER_TRAINER_KIPP] = { // ぼうやたち カイ
        COMPOUND_STRING("{JPN}浮き輪が あれば\nどこでも 泳げるんだ！"),
        COMPOUND_STRING("{JPN}やったー！ 浮き輪の\nおかげかな？"),
        COMPOUND_STRING("{JPN}うわーん！ 浮き輪から\n空気が 抜けちゃった！"),
    },
    [FRONTIER_TRAINER_JASON] = { // ぼうやたち ヒロ
        COMPOUND_STRING("{JPN}ぼく まだ 小さいけど\n勝負は 強いよ！"),
        COMPOUND_STRING("{JPN}ね！ 小さくても\n強いでしょ！"),
        COMPOUND_STRING("{JPN}大きく なったら\nもっと 強く なるもん！"),
    },
    [FRONTIER_TRAINER_JOHN] = { // ぼうやたち ゲン
        COMPOUND_STRING("{JPN}海で 遊ぶのも 好きだけど\n勝負は もっと 好き！"),
        COMPOUND_STRING("{JPN}勝った！ 次は\n海で 遊ぶぞー！"),
        COMPOUND_STRING("{JPN}負けちゃった……。\n海で 泣いてくる……。"),
    },
    [FRONTIER_TRAINER_ANN] = { // おじょうちゃんたち カヤ
        COMPOUND_STRING("{JPN}ママが 見てるの！\nかっこいい ところ 見せるね！"),
        COMPOUND_STRING("{JPN}ママー！ 見てた？\nわたし 勝ったよー！"),
        COMPOUND_STRING("{JPN}ママ……。\n負けちゃった……。"),
    },
    [FRONTIER_TRAINER_EILEEN] = { // おじょうちゃんたち ノン
        COMPOUND_STRING("{JPN}えへへ！ わたしの ポケモン\nとっても 強いんだよ！"),
        COMPOUND_STRING("{JPN}ほらね！\nいちばん 強いでしょ！"),
        COMPOUND_STRING("{JPN}むー！ つぎは\nぜったい 負けないもん！"),
    },
    [FRONTIER_TRAINER_CARLIE] = { // おじょうちゃんたち ルイ
        COMPOUND_STRING("{JPN}お兄ちゃんに 教わった\n技を 見せてあげる！"),
        COMPOUND_STRING("{JPN}お兄ちゃんに\n自慢 しちゃおっと！"),
        COMPOUND_STRING("{JPN}お兄ちゃんに\nないしょに してね……。"),
    },
    [FRONTIER_TRAINER_GORDON] = { // かいパンやろう ミナト
        COMPOUND_STRING("{JPN}毎朝 海で 泳いでるんだ。\n体力には 自信が あるぜ！"),
        COMPOUND_STRING("{JPN}体力勝負なら\nおれの 勝ちだ！"),
        COMPOUND_STRING("{JPN}ぷはっ！ 息が\n続かなかったぜ……。"),
    },
    [FRONTIER_TRAINER_AYDEN] = { // かいパンやろう ナミノリ
        COMPOUND_STRING("{JPN}波に 乗るのも 勝負の\n流れに 乗るのも 得意さ！"),
        COMPOUND_STRING("{JPN}いい 波 来てたぜ！\n最高の 乗り心地だ！"),
        COMPOUND_STRING("{JPN}波に のまれちまった……。\nまだまだ 修行が 足りないな。"),
    },
    [FRONTIER_TRAINER_MARCO] = { // かいパンやろう ホウサク
        COMPOUND_STRING("{JPN}遠泳で きたえた ねばりを\n見せてやるよ！"),
        COMPOUND_STRING("{JPN}最後まで ねばった\nほうが 勝つのさ！"),
        COMPOUND_STRING("{JPN}ねばり負けか……。\nきみ なかなか やるな。"),
    },
    [FRONTIER_TRAINER_CIERRA] = { // ビキニのおねえさん リリー
        COMPOUND_STRING("{JPN}日焼け止めは ばっちり！\nさあ 勝負しましょ！"),
        COMPOUND_STRING("{JPN}うふふ！ 太陽の 下だと\n調子が いいのよね！"),
        COMPOUND_STRING("{JPN}負けちゃった……。\n日かげで 休もうかしら。"),
    },
    [FRONTIER_TRAINER_MARCY] = { // ビキニのおねえさん トウコ
        COMPOUND_STRING("{JPN}水の 中みたいに\nすいすい 戦うわよ！"),
        COMPOUND_STRING("{JPN}すいすい 勝てちゃった！\n気持ちいいー！"),
        COMPOUND_STRING("{JPN}あら？ 足が\nつっちゃった みたい……。"),
    },
    [FRONTIER_TRAINER_KATHY] = { // ビキニのおねえさん クレア
        COMPOUND_STRING("{JPN}海の 女を\nなめないでね！"),
        COMPOUND_STRING("{JPN}どう？ 海の 女は\n強いでしょ？"),
        COMPOUND_STRING("{JPN}海より 広い 心で\n負けを みとめるわ。"),
    },
    [FRONTIER_TRAINER_PEYTON] = { // ポケモンだいすきクラブ スギハル
        COMPOUND_STRING("{JPN}わしの ポケモンは\n世界一 かわいいんじゃ！"),
        COMPOUND_STRING("{JPN}かわいい うえに 強い！\nさすが わしの ポケモンじゃ！"),
        COMPOUND_STRING("{JPN}負けても かわいいのう……。\nよしよし よく がんばった！"),
    },
    [FRONTIER_TRAINER_JULIAN] = { // ポケモンだいすきクラブ ナオジ
        COMPOUND_STRING("{JPN}ポケモンへの 愛なら\nだれにも 負けないぞ！"),
        COMPOUND_STRING("{JPN}愛の 勝利だ！\nみんな ありがとう！"),
        COMPOUND_STRING("{JPN}きみも ポケモンを\n愛しているんだね……。"),
    },
    [FRONTIER_TRAINER_QUINN] = { // ポケモンだいすきクラブ ミノブ
        COMPOUND_STRING("{JPN}毎日 ブラッシングしてるから\n毛並みは ばっちりだ！"),
        COMPOUND_STRING("{JPN}毛並みも 勝負も\n申し分 なしだな！"),
        COMPOUND_STRING("{JPN}負けたけど 毛並みは\n負けてないぞ！"),
    },
    [FRONTIER_TRAINER_HAYLEE] = { // ポケモンだいすきクラブ ヌマエ
        COMPOUND_STRING("{JPN}うちの子たちを\n見てちょうだい！"),
        COMPOUND_STRING("{JPN}よく がんばったわね！\nごほうびを あげなくちゃ！"),
        COMPOUND_STRING("{JPN}負けちゃっても\nうちの子が いちばんよ！"),
    },
    [FRONTIER_TRAINER_AMANDA] = { // ポケモンだいすきクラブ フミカ
        COMPOUND_STRING("{JPN}おそろいの リボンで\n気合い 十分よ！"),
        COMPOUND_STRING("{JPN}リボンの おかげで\n勝てたわ！"),
        COMPOUND_STRING("{JPN}リボンが ほどけちゃった……。\n結び直さなきゃ。"),
    },
    [FRONTIER_TRAINER_STACY] = { // ポケモンだいすきクラブ キリエ
        COMPOUND_STRING("{JPN}ポケモンと いっしょなら\n毎日が 楽しいわ！"),
        COMPOUND_STRING("{JPN}勝てると もっと\n楽しいわね！"),
        COMPOUND_STRING("{JPN}負けても いっしょに\nいられれば 幸せよ。"),
    },
    [FRONTIER_TRAINER_RAFAEL] = { // ポケモンブリーダー ヨリノブ
        COMPOUND_STRING("{JPN}手塩に かけて 育てた\nポケモンの 力を 見てくれ！"),
        COMPOUND_STRING("{JPN}育て方に まちがいは\nなかったようだ！"),
        COMPOUND_STRING("{JPN}育て直しだな……。\nきみの 育て方を 教えてくれ。"),
    },
    [FRONTIER_TRAINER_OLIVER] = { // ポケモンブリーダー コダマ
        COMPOUND_STRING("{JPN}能力の 配分は\n完璧に 計算してあるよ。"),
        COMPOUND_STRING("{JPN}計算どおりの\n仕上がりだ！"),
        COMPOUND_STRING("{JPN}数字だけでは\n測れない 強さか……。"),
    },
    [FRONTIER_TRAINER_PAYTON] = { // ポケモンブリーダー ハンペイ
        COMPOUND_STRING("{JPN}タマゴから 育てた\n自慢の ポケモンだ！"),
        COMPOUND_STRING("{JPN}立派に 育ったなあ！\n親として うれしいよ！"),
        COMPOUND_STRING("{JPN}負けたけど 立派に\n戦ってくれた！"),
    },
    [FRONTIER_TRAINER_PAMELA] = { // ポケモンブリーダー ミイナ
        COMPOUND_STRING("{JPN}育てる 喜びと\n戦う 喜び 両方 味わいたいの！"),
        COMPOUND_STRING("{JPN}育てた 子が 勝つのは\n最高の 喜びね！"),
        COMPOUND_STRING("{JPN}次は もっと\nじっくり 育てるわ。"),
    },
    [FRONTIER_TRAINER_ELIZA] = { // ポケモンブリーダー マカコ
        COMPOUND_STRING("{JPN}きのみで 体調管理も\nばっちりよ！"),
        COMPOUND_STRING("{JPN}体調が いいと\n動きも ちがうわね！"),
        COMPOUND_STRING("{JPN}体調は よかったのに……\nあなたが 上手だったのね。"),
    },
    [FRONTIER_TRAINER_MARISA] = { // ポケモンブリーダー ジュンカ
        COMPOUND_STRING("{JPN}この子たちの いいところ\nたくさん 見つけてきたの！"),
        COMPOUND_STRING("{JPN}いいところが\nぜんぶ 出せたわ！"),
        COMPOUND_STRING("{JPN}あなたの ポケモンの\nいいところも 見えたわ。"),
    },
    [FRONTIER_TRAINER_LEWIS] = { // むしとりしょうねん エイタ
        COMPOUND_STRING("{JPN}森で つかまえた\n最強の 虫ポケモンだぞ！"),
        COMPOUND_STRING("{JPN}虫ポケモンは\n最強なんだ！"),
        COMPOUND_STRING("{JPN}虫ポケモンを\n見直して くれたかな……？"),
    },
    [FRONTIER_TRAINER_YOSHI] = { // むしとりしょうねん カスケ
        COMPOUND_STRING("{JPN}あみを 振り回して\n朝から 修行してたんだ！"),
        COMPOUND_STRING("{JPN}修行の 成果が\nばっちり 出たぞ！"),
        COMPOUND_STRING("{JPN}きみは あみでも\nつかまえられないや！"),
    },
    [FRONTIER_TRAINER_DESTIN] = { // むしとりしょうねん タケフミ
        COMPOUND_STRING("{JPN}虫かごに 入りきらないほど\n強い 仲間が いるんだ！"),
        COMPOUND_STRING("{JPN}へへっ！ ぼくの\n仲間は すごいでしょ！"),
        COMPOUND_STRING("{JPN}負けた……。 でも 仲間は\nみんな 宝物だよ！"),
    },
    [FRONTIER_TRAINER_KEON] = { // ちびっこにんじゃ テツ
        COMPOUND_STRING("{JPN}にんにん！ 忍びの 技\nとくと 見よ！"),
        COMPOUND_STRING("{JPN}にんにん！\n忍びの 勝利で ござる！"),
        COMPOUND_STRING("{JPN}む むねん……！\nどろん！"),
    },
    [FRONTIER_TRAINER_STUART] = { // ちびっこにんじゃ イサフミ
        COMPOUND_STRING("{JPN}拙者の 姿 見えるかな？\n見えた ときには もう 遅い！"),
        COMPOUND_STRING("{JPN}影のように 動けば\n負けは ないので ござる！"),
        COMPOUND_STRING("{JPN}見破られて しまった……！\n修行の やり直しで ござる。"),
    },
    [FRONTIER_TRAINER_NESTOR] = { // ちびっこにんじゃ ダイゴロウ
        COMPOUND_STRING("{JPN}忍法 かくれみの！\n……あれ？ 見えてる？"),
        COMPOUND_STRING("{JPN}かくれなくても\n勝てたで ござる！"),
        COMPOUND_STRING("{JPN}忍法が きかなかった……。\n師匠に しかられる！"),
    },
    [FRONTIER_TRAINER_DERRICK] = { // むしマニア タダスケ
        COMPOUND_STRING("{JPN}虫ポケモンの 美しさ\nきみに わかるかな？"),
        COMPOUND_STRING("{JPN}見たまえ この 羽の 輝き！\n虫こそ 至高！"),
        COMPOUND_STRING("{JPN}きみの ポケモンも\nなかなか 美しい……。"),
    },
    [FRONTIER_TRAINER_BRYSON] = { // むしマニア ホウジ
        COMPOUND_STRING("{JPN}虫ポケモンの 観察日記は\nもう 五十冊目だよ。"),
        COMPOUND_STRING("{JPN}今日の 日記は\n勝利の 記録だね！"),
        COMPOUND_STRING("{JPN}負けた ことも\nしっかり 記録しておこう……。"),
    },
    [FRONTIER_TRAINER_CLAYTON] = { // むしマニア サンペイ
        COMPOUND_STRING("{JPN}ふふふ……\n虫の 底力を 見せてあげよう。"),
        COMPOUND_STRING("{JPN}小さな 体に\n大きな 力！ それが 虫だ！"),
        COMPOUND_STRING("{JPN}底力では\n足りなかったか……。"),
    },
    [FRONTIER_TRAINER_TRENTON] = { // つりびと カズト
        COMPOUND_STRING("{JPN}大物を ねらうなら\nあせりは 禁物だぞ。"),
        COMPOUND_STRING("{JPN}よーし 大物が\n釣れたぞ！"),
        COMPOUND_STRING("{JPN}逃がした 魚は\n大きかったなあ……。"),
    },
    [FRONTIER_TRAINER_JENSON] = { // つりびと シズカ
        COMPOUND_STRING("{JPN}釣りも 勝負も\n待つことが 大事なんだ。"),
        COMPOUND_STRING("{JPN}じっと 待った\nかいが あったよ。"),
        COMPOUND_STRING("{JPN}待ちすぎて\nチャンスを 逃したか……。"),
    },
    [FRONTIER_TRAINER_WESLEY] = { // つりびと モズク
        COMPOUND_STRING("{JPN}今日は 釣れない 日でな。\n勝負で うさを 晴らすぞ！"),
        COMPOUND_STRING("{JPN}はっはっは！\nうさが 晴れたわい！"),
        COMPOUND_STRING("{JPN}今日は 何を やっても\nだめな 日だな……。"),
    },
    [FRONTIER_TRAINER_ANTON] = { // やまおとこ（ルインマニア） ザック
        COMPOUND_STRING("{JPN}遺跡で 見つけた 石板に\n勝負の 極意が 書いてあった！"),
        COMPOUND_STRING("{JPN}石板の 教えは\n本物だったぞ！"),
        COMPOUND_STRING("{JPN}石板を 読みまちがえて\nいたのかも しれん……。"),
    },
    [FRONTIER_TRAINER_LAWSON] = { // ルインマニア ボガール
        COMPOUND_STRING("{JPN}古代の ロマンを\nポケモンに 感じるのだ！"),
        COMPOUND_STRING("{JPN}これぞ 古代の\n力の 目覚めだ！"),
        COMPOUND_STRING("{JPN}古代の 力も\n現代の きみには かなわんか。"),
    },
    [FRONTIER_TRAINER_SAMMY] = { // ルインマニア モリアキ
        COMPOUND_STRING("{JPN}掘っても 掘っても\n強い 相手が 出てくるな！"),
        COMPOUND_STRING("{JPN}今日も いい\n掘り出し物だった！"),
        COMPOUND_STRING("{JPN}掘り当てたのは\nきみの ほうだったか……。"),
    },
    [FRONTIER_TRAINER_ARNIE] = { // コレクター トヨジ
        COMPOUND_STRING("{JPN}めずらしい ポケモンを\n集めるのが 生きがいでね。"),
        COMPOUND_STRING("{JPN}わたしの コレクションは\n飾りじゃ ないのだよ。"),
        COMPOUND_STRING("{JPN}きみの ポケモンも\nコレクションに 加えたいなあ。"),
    },
    [FRONTIER_TRAINER_ADRIAN] = { // コレクター シゲタ
        COMPOUND_STRING("{JPN}この バッジも あの 道具も\nぜんぶ 自慢の 品なんだ。"),
        COMPOUND_STRING("{JPN}勝利の 記念品が\nまた ひとつ 増えたよ！"),
        COMPOUND_STRING("{JPN}負けの 記念品は\nいらないんだけどなあ……。"),
    },
    [FRONTIER_TRAINER_TRISTAN] = { // コレクター ティム
        COMPOUND_STRING("{JPN}色ちがいを 探して\n世界中を 回ったんだ。"),
        COMPOUND_STRING("{JPN}どうだい？ 苦労して\n集めた かいが あるだろう？"),
        COMPOUND_STRING("{JPN}きみは 色ちがいより\nめずらしい トレーナーだね。"),
    },
    [FRONTIER_TRAINER_JULIANA] = { // パラソルおねえさん ナオエ
        COMPOUND_STRING("{JPN}日差しが 強いわね。\n日がさを さして 戦うわ。"),
        COMPOUND_STRING("{JPN}日がさの かげで\nゆうゆうと 勝てたわ。"),
        COMPOUND_STRING("{JPN}日がさが 風に\n飛ばされちゃった……。"),
    },
    [FRONTIER_TRAINER_RYLEE] = { // パラソルおねえさん ロビン
        COMPOUND_STRING("{JPN}雨の 日も 晴れの 日も\nこの かさが 相棒なの。"),
        COMPOUND_STRING("{JPN}かさを くるりと 回せば\n勝利も くるりと 来るの！"),
        COMPOUND_STRING("{JPN}かさを 閉じて\n休むことに するわ……。"),
    },
    [FRONTIER_TRAINER_CHELSEA] = { // パラソルおねえさん ヤスミン
        COMPOUND_STRING("{JPN}優雅な 勝負を\nお見せ するわね。"),
        COMPOUND_STRING("{JPN}優雅に 決まったわ。\nうふふ。"),
        COMPOUND_STRING("{JPN}優雅さを 失って\nしまったわ……。"),
    },
    [FRONTIER_TRAINER_DANELA] = { // おとなのおねえさん ナグモ
        COMPOUND_STRING("{JPN}わたしの 美しさに\n見とれて いないで ね？"),
        COMPOUND_STRING("{JPN}見とれている うちに\n勝負は ついちゃったわ。"),
        COMPOUND_STRING("{JPN}わたしに 見とれないなんて\nやるじゃない。"),
    },
    [FRONTIER_TRAINER_LIZBETH] = { // おとなのおねえさん アリア
        COMPOUND_STRING("{JPN}きれいな 花には\nトゲが あるのよ。"),
        COMPOUND_STRING("{JPN}トゲの 痛さ\nわかったかしら？"),
        COMPOUND_STRING("{JPN}わたしの トゲも\nきみには 届かなかったのね。"),
    },
    [FRONTIER_TRAINER_AMELIA] = { // おとなのおねえさん ヨウコ
        COMPOUND_STRING("{JPN}強くて きれい。\nそれが わたしの 理想なの。"),
        COMPOUND_STRING("{JPN}理想に また\n一歩 近づいたわ。"),
        COMPOUND_STRING("{JPN}理想は まだまだ\n遠いみたいね……。"),
    },
    [FRONTIER_TRAINER_JILLIAN] = { // アロマなおねえさん ミホコ
        COMPOUND_STRING("{JPN}花の 香りで\n心を 落ち着かせて……。"),
        COMPOUND_STRING("{JPN}落ち着いて 戦えば\n負けないのよ。"),
        COMPOUND_STRING("{JPN}香りでも 落ち着けない\nくらい 強かったわ……。"),
    },
    [FRONTIER_TRAINER_ABBIE] = { // アロマなおねえさん ソヨギ
        COMPOUND_STRING("{JPN}ハーブの 香りは\nポケモンも 元気に するの。"),
        COMPOUND_STRING("{JPN}ハーブの 力って\nすごいでしょ？"),
        COMPOUND_STRING("{JPN}ハーブティーでも 飲んで\nひと休み するわ。"),
    },
    [FRONTIER_TRAINER_BRIANA] = { // アロマなおねえさん メリー
        COMPOUND_STRING("{JPN}いい 香りに 包まれて\n勝負しましょう。"),
        COMPOUND_STRING("{JPN}勝利の 香りが\nするわ！"),
        COMPOUND_STRING("{JPN}香りより 強い\n風が 吹いたみたいね。"),
    },
    [FRONTIER_TRAINER_ANTONIO] = { // ギターリスト ダレン
        COMPOUND_STRING("{JPN}イエーイ！ 勝負の\nリズムに 乗っていくぜ！"),
        COMPOUND_STRING("{JPN}最高の ライブだったぜ！\nアンコール するかい？"),
        COMPOUND_STRING("{JPN}弦が 切れちまった……。\n今日の ライブは ここまでだ。"),
    },
    [FRONTIER_TRAINER_JADEN] = { // ギターリスト ヨシノブ
        COMPOUND_STRING("{JPN}おれの ギターと ポケモンの\nセッションを 聞いてくれ！"),
        COMPOUND_STRING("{JPN}息の 合った\nいい セッションだった！"),
        COMPOUND_STRING("{JPN}音が ずれちまった……。\nチューニングし直しだ。"),
    },
    [FRONTIER_TRAINER_DAKOTA] = { // ギターリスト ティンタン
        COMPOUND_STRING("{JPN}ジャーン！ 勝負の 前に\n一曲 聞かせてやるぜ！"),
        COMPOUND_STRING("{JPN}勝利の メロディーが\n鳴り響くぜ！"),
        COMPOUND_STRING("{JPN}負けの ブルースでも\n歌うと するか……。"),
    },
    [FRONTIER_TRAINER_BRAYDEN] = { // とりつかい バッシー
        COMPOUND_STRING("{JPN}大空を 舞う\n鳥ポケモンの 強さを 見よ！"),
        COMPOUND_STRING("{JPN}空を 制する 者が\n勝負を 制するのさ！"),
        COMPOUND_STRING("{JPN}翼を 休めて\n出直すと しよう。"),
    },
    [FRONTIER_TRAINER_CORSON] = { // とりつかい シンノスケ
        COMPOUND_STRING("{JPN}風を 読むのは\n得意なんだ！"),
        COMPOUND_STRING("{JPN}追い風が\n吹いていたね！"),
        COMPOUND_STRING("{JPN}向かい風だったか……。\n風を 読みちがえたよ。"),
    },
    [FRONTIER_TRAINER_TREVIN] = { // とりつかい マーロン
        COMPOUND_STRING("{JPN}羽ばたきの 音で\nポケモンの 調子が わかるんだ。"),
        COMPOUND_STRING("{JPN}今日は 羽の 音が\nいつもより 軽かった！"),
        COMPOUND_STRING("{JPN}羽が 重く\n聞こえたよ……。"),
    },
    [FRONTIER_TRAINER_PATRICK] = { // ふなのり ジョニー
        COMPOUND_STRING("{JPN}荒波で きたえた 腕っぷし\n見せてやるぜ！"),
        COMPOUND_STRING("{JPN}がっはっは！\n海の 男は 強いだろう！"),
        COMPOUND_STRING("{JPN}嵐に のまれた\n気分だぜ……。"),
    },
    [FRONTIER_TRAINER_KADEN] = { // ふなのり ブイチ
        COMPOUND_STRING("{JPN}ようそろー！\n勝負の 海へ 出航だ！"),
        COMPOUND_STRING("{JPN}順風満帆！\nいい 航海だったぜ！"),
        COMPOUND_STRING("{JPN}座礁しちまった……。\n港へ 戻るとするか。"),
    },
    [FRONTIER_TRAINER_MAXWELL] = { // ふなのり サブロウタ
        COMPOUND_STRING("{JPN}船の 上でも 陸の 上でも\nおれは 負けねえ！"),
        COMPOUND_STRING("{JPN}陸でも\nおれの 勝ちだな！"),
        COMPOUND_STRING("{JPN}陸の 上は\n揺れなくて 調子が 狂うぜ……。"),
    },
    [FRONTIER_TRAINER_DARYL] = { // やまおとこ ヒダカ
        COMPOUND_STRING("{JPN}山を 越え 谷を 越え\nここまで 来たんだ！"),
        COMPOUND_STRING("{JPN}頂上に たどり着いた\n気分だ！"),
        COMPOUND_STRING("{JPN}まだ 山の\n中腹だったか……。"),
    },
    [FRONTIER_TRAINER_KENNETH] = { // やまおとこ エミール
        COMPOUND_STRING("{JPN}重い 荷物を 背負って\n足腰を きたえているんだ！"),
        COMPOUND_STRING("{JPN}足腰の 強さが\n勝負を 決めるのさ！"),
        COMPOUND_STRING("{JPN}荷物を 下ろして\n休むとするか……。"),
    },
    [FRONTIER_TRAINER_RICH] = { // やまおとこ ソウキ
        COMPOUND_STRING("{JPN}岩のように どっしり\n構えて 戦うぞ！"),
        COMPOUND_STRING("{JPN}岩は\n動じないのさ！"),
        COMPOUND_STRING("{JPN}岩も 削られれば\n崩れるか……。"),
    },
    [FRONTIER_TRAINER_CADEN] = { // ほのおのぼうや モンジロウ
        COMPOUND_STRING("{JPN}燃えろ 燃えろ！\nおれの 心は 燃えているぞ！"),
        COMPOUND_STRING("{JPN}燃え尽きる ほどの\n勝負だったぜ！"),
        COMPOUND_STRING("{JPN}火が 消えちまった……。"),
    },
    [FRONTIER_TRAINER_MARLON] = { // ほのおのぼうや アサト
        COMPOUND_STRING("{JPN}炎の 熱さを\nその 身で 感じろ！"),
        COMPOUND_STRING("{JPN}熱かった だろう？\nこれが おれの 炎だ！"),
        COMPOUND_STRING("{JPN}きみの 情熱の ほうが\n熱かったか……。"),
    },
    [FRONTIER_TRAINER_NASH] = { // ほのおのぼうや モウキ
        COMPOUND_STRING("{JPN}火の 輪くぐりで\n度胸を きたえたんだ！"),
        COMPOUND_STRING("{JPN}度胸の 勝利だ！"),
        COMPOUND_STRING("{JPN}度胸だけじゃ\n勝てないんだな……。"),
    },
    [FRONTIER_TRAINER_ROBBY] = { // ランナー アユム
        COMPOUND_STRING("{JPN}朝の ランニングの\nついでに 勝負だ！"),
        COMPOUND_STRING("{JPN}いい 汗を\nかいたぜ！"),
        COMPOUND_STRING("{JPN}息が 上がっちまった……。\nもう ひと走り してくるよ。"),
    },
    [FRONTIER_TRAINER_REECE] = { // ランナー デリック
        COMPOUND_STRING("{JPN}スタートダッシュには\n自信が あるんだ！"),
        COMPOUND_STRING("{JPN}最初から 最後まで\nトップを 守れたな！"),
        COMPOUND_STRING("{JPN}ゴール前で\nぬかれちまった……。"),
    },
    [FRONTIER_TRAINER_KATHRYN] = { // ランナー メロディ
        COMPOUND_STRING("{JPN}自分の ペースを 守れば\nきっと 勝てるわ！"),
        COMPOUND_STRING("{JPN}ペース配分は\nばっちり だったわね！"),
        COMPOUND_STRING("{JPN}ペースを\n乱されちゃった……。"),
    },
    [FRONTIER_TRAINER_ELLEN] = { // ランナー ナツル
        COMPOUND_STRING("{JPN}走るのと 同じくらい\n勝負も 大好きなの！"),
        COMPOUND_STRING("{JPN}自己ベスト 更新よ！"),
        COMPOUND_STRING("{JPN}あーあ……\n記録は のびなかったわ。"),
    },
    [FRONTIER_TRAINER_RAMON] = { // スイマー カルロス
        COMPOUND_STRING("{JPN}トライアスロンの\n途中 なんだ。 手短に 頼むよ！"),
        COMPOUND_STRING("{JPN}よし！ この 勢いで\n次の 種目へ 行くぞ！"),
        COMPOUND_STRING("{JPN}ここで タイムを\nロス するなんて……！"),
    },
    [FRONTIER_TRAINER_ARTHUR] = { // スイマー ケント
        COMPOUND_STRING("{JPN}水を かくように\n相手を かきわけて 進むぞ！"),
        COMPOUND_STRING("{JPN}ひとかき リードだったな！"),
        COMPOUND_STRING("{JPN}きみの ほうが\nひとかき 速かったか……。"),
    },
    [FRONTIER_TRAINER_ALONDRA] = { // スイマー リル
        COMPOUND_STRING("{JPN}泳いだ あとの 勝負は\n体が 温まってて いいの！"),
        COMPOUND_STRING("{JPN}ほら 体が\nよく 動いたでしょ！"),
        COMPOUND_STRING("{JPN}体が 冷えて\nきちゃったみたい……。"),
    },
    [FRONTIER_TRAINER_ADRIANA] = { // スイマー タマエ
        COMPOUND_STRING("{JPN}どんな 流れにも\n負けない 泳ぎを 見せるわ！"),
        COMPOUND_STRING("{JPN}流れに 逆らわず\n乗りこなしたわ！"),
        COMPOUND_STRING("{JPN}流されちゃった……。"),
    },
    [FRONTIER_TRAINER_MALIK] = { // サイクリング チョウ
        COMPOUND_STRING("{JPN}ペダルを こぐ 足は\n止まらないぜ！"),
        COMPOUND_STRING("{JPN}このまま\nゴールまで 一直線だ！"),
        COMPOUND_STRING("{JPN}チェーンが 外れちまった\n気分だ……。"),
    },
    [FRONTIER_TRAINER_JILL] = { // サイクリング ヒカル
        COMPOUND_STRING("{JPN}風を 切って 走るのが\nたまらなく 好きなの！"),
        COMPOUND_STRING("{JPN}勝利の 風を\n感じるわ！"),
        COMPOUND_STRING("{JPN}向かい風が\n強すぎたわ……。"),
    },
    [FRONTIER_TRAINER_ERIK] = { // ランナー ジュウベイ (100)
        COMPOUND_STRING("{JPN}長い 距離を 走りぬく\nスタミナを 見せてやる！"),
        COMPOUND_STRING("{JPN}最後に 物を 言うのは\nスタミナさ！"),
        COMPOUND_STRING("{JPN}スタミナ切れか……。\n走りこみが 足りないな。"),
    },
    [FRONTIER_TRAINER_YAZMIN] = { // ランナー ソナ
        COMPOUND_STRING("{JPN}走るたびに 景色が 変わるの。\n勝負も きっと そうよね！"),
        COMPOUND_STRING("{JPN}勝ったときの 景色は\nやっぱり 格別ね！"),
        COMPOUND_STRING("{JPN}負けたときの 景色も\n悪くは ないわね……。"),
    },
    [FRONTIER_TRAINER_JAMAL] = { // スイマー ビル
        COMPOUND_STRING("{JPN}海でも プールでも\nおれの 泳ぎは 一流だ！"),
        COMPOUND_STRING("{JPN}一流の 泳ぎに\n一流の 勝負だったろう？"),
        COMPOUND_STRING("{JPN}今日は 水が\n合わなかった みたいだ……。"),
    },
    [FRONTIER_TRAINER_LESLIE] = { // スイマー カーリー
        COMPOUND_STRING("{JPN}息つぎの タイミングが\n勝負の 決め手なの！"),
        COMPOUND_STRING("{JPN}タイミングばっちり！\nいい リズムだったわ！"),
        COMPOUND_STRING("{JPN}息つぎを\n失敗しちゃった……。"),
    },
    [FRONTIER_TRAINER_DAVE] = { // サイクリング シアムン
        COMPOUND_STRING("{JPN}坂道で きたえた 脚力\nあなどるなよ！"),
        COMPOUND_STRING("{JPN}どんな 坂も\nこの 脚で 登りきるぜ！"),
        COMPOUND_STRING("{JPN}きみと いう 坂は\n急すぎたぜ……。"),
    },
    [FRONTIER_TRAINER_CARLO] = { // サイクリング ジンベイ
        COMPOUND_STRING("{JPN}自転車の 整備は\n完璧だ！ 勝負も 完璧に いくぞ！"),
        COMPOUND_STRING("{JPN}完璧な 走りだったな！"),
        COMPOUND_STRING("{JPN}どこかの ネジが\nゆるんでいたかな……。"),
    },
    [FRONTIER_TRAINER_EMILIA] = { // サイクリング キャリー
        COMPOUND_STRING("{JPN}こぐのを 止めたら\n倒れちゃうの！ 全力で いくわ！"),
        COMPOUND_STRING("{JPN}止まらずに\n走りきれたわ！"),
        COMPOUND_STRING("{JPN}止まっちゃった……。\nころんじゃった 気分よ。"),
    },
    [FRONTIER_TRAINER_DALIA] = { // サイクリング カナエ
        COMPOUND_STRING("{JPN}ヘルメットの ひもを\nきゅっと しめて 勝負よ！"),
        COMPOUND_STRING("{JPN}気合いが 入ったら\n勝てちゃった！"),
        COMPOUND_STRING("{JPN}ひもを しめ直して\n出直すわ……。"),
    },
    [FRONTIER_TRAINER_HITOMI] = { // からておう カズ
        COMPOUND_STRING("{JPN}押忍！ 鍛え抜いた\n拳を 受けてみよ！"),
        COMPOUND_STRING("{JPN}押忍！\n鍛錬の 成果だ！"),
        COMPOUND_STRING("{JPN}押忍……！\n参りました！"),
    },
    [FRONTIER_TRAINER_RICARDO] = { // からておう ヤスキ
        COMPOUND_STRING("{JPN}滝に 打たれて\n心を 磨いてきた！"),
        COMPOUND_STRING("{JPN}心身 ともに\n充実しておるわ！"),
        COMPOUND_STRING("{JPN}まだ 心の 修行が\n足りなかったか……。"),
    },
    [FRONTIER_TRAINER_SHIZUKA] = { // からておう バラトシン
        COMPOUND_STRING("{JPN}正拳突き 一万回！\n今日の 稽古は もう 済ませた！"),
        COMPOUND_STRING("{JPN}一万回の 稽古は\nうそを つかん！"),
        COMPOUND_STRING("{JPN}明日は 二万回だ……！"),
    },
    [FRONTIER_TRAINER_JOANA] = { // バトルガール チナツ
        COMPOUND_STRING("{JPN}えいっ！ やあっ！\n気合いじゃ 負けないわよ！"),
        COMPOUND_STRING("{JPN}気合いの 勝利ね！\nたあっ！"),
        COMPOUND_STRING("{JPN}きゃあ！\n気合い負け しちゃった！"),
    },
    [FRONTIER_TRAINER_KELLY] = { // バトルガール ミワコ
        COMPOUND_STRING("{JPN}わたしと ポケモン\n一心同体で 戦うの！"),
        COMPOUND_STRING("{JPN}息も ぴったり\nだったでしょ！"),
        COMPOUND_STRING("{JPN}息が\n合わなかったのかな……。"),
    },
    [FRONTIER_TRAINER_RAYNA] = { // バトルガール クォン
        COMPOUND_STRING("{JPN}遠い 国から\n修行に 来たの！ 勝負よ！"),
        COMPOUND_STRING("{JPN}修行の 旅は\nまだまだ 続くわ！"),
        COMPOUND_STRING("{JPN}来た かいが あったわ。\nあなた 強いのね！"),
    },
    [FRONTIER_TRAINER_EVAN] = { // ベテラントレーナー ノブアキ
        COMPOUND_STRING("{JPN}長年の 経験から\n言わせてもらおう。 若いな！"),
        COMPOUND_STRING("{JPN}経験の 差が\n出たようじゃな。"),
        COMPOUND_STRING("{JPN}若さの 勢いには\nかなわんか……。"),
    },
    [FRONTIER_TRAINER_JORDAN] = { // ベテラントレーナー タイケン
        COMPOUND_STRING("{JPN}多くの 勝負を 見てきたが\nきみは どうかな？"),
        COMPOUND_STRING("{JPN}ふむ。 まだまだ\n伸びしろが あるな。"),
        COMPOUND_STRING("{JPN}見事じゃ。 久しぶりに\n血が 騒いだわい。"),
    },
    [FRONTIER_TRAINER_JOEL] = { // ベテラントレーナー タモン
        COMPOUND_STRING("{JPN}この 年に なっても\n勝負の 楽しさは 変わらん！"),
        COMPOUND_STRING("{JPN}はっはっは！\nまだまだ 若い者には 負けん！"),
        COMPOUND_STRING("{JPN}負けても 楽しいわい。\nいい 勝負じゃった！"),
    },
    [FRONTIER_TRAINER_KRISTEN] = { // ベテラントレーナー シズ
        COMPOUND_STRING("{JPN}あらあら かわいい\n挑戦者さんね。"),
        COMPOUND_STRING("{JPN}年の 功と いう\nやつかしら。"),
        COMPOUND_STRING("{JPN}まあ 見事だこと。\n感心したわ。"),
    },
    [FRONTIER_TRAINER_SELPHY] = { // ベテラントレーナー オーレン
        COMPOUND_STRING("{JPN}若い 頃は\nわたしも 無茶を したものよ。"),
        COMPOUND_STRING("{JPN}無茶は\nほどほどに ね。"),
        COMPOUND_STRING("{JPN}その 無茶が\n勝利を 呼んだのね。"),
    },
    [FRONTIER_TRAINER_CHLOE] = { // ベテラントレーナー キンショウ
        COMPOUND_STRING("{JPN}勝負は 準備で\n九割 決まるのよ。"),
        COMPOUND_STRING("{JPN}準備の 差よ。\nよく 覚えておきなさい。"),
        COMPOUND_STRING("{JPN}あなたの 準備が\n一枚 上手だったわね。"),
    },
    [FRONTIER_TRAINER_NORTON] = { // サイキッカー ユーリ
        COMPOUND_STRING("{JPN}きみの 次の 一手……\nもう 見えているよ。"),
        COMPOUND_STRING("{JPN}言ったとおりだろう？\n未来は 見えていたのさ。"),
        COMPOUND_STRING("{JPN}こんな 未来は\n見えていなかった……！"),
    },
    [FRONTIER_TRAINER_LUKAS] = { // サイキッカー フユオ
        COMPOUND_STRING("{JPN}念力で スプーンを\n曲げるのは 朝飯前さ。"),
        COMPOUND_STRING("{JPN}勝負の 流れも\n念力で 曲げてみせたよ。"),
        COMPOUND_STRING("{JPN}ぼくの 心が\n曲がって しまった……。"),
    },
    [FRONTIER_TRAINER_ZACH] = { // サイキッカー キンゾウ
        COMPOUND_STRING("{JPN}むむむ……！\n勝利の 気配を 感じる！"),
        COMPOUND_STRING("{JPN}ほら！\n気配は 本物だった！"),
        COMPOUND_STRING("{JPN}気配は きみの\nほうから していたのか……。"),
    },
    [FRONTIER_TRAINER_KAITLYN] = { // サイキッカー ビビアン
        COMPOUND_STRING("{JPN}あなたの 心の 声が\n聞こえるわ……。"),
        COMPOUND_STRING("{JPN}心の 声が\nあせっていたわよ。"),
        COMPOUND_STRING("{JPN}あなたの 心は\n最後まで 迷わなかったのね。"),
    },
    [FRONTIER_TRAINER_BREANNA] = { // サイキッカー ローザ
        COMPOUND_STRING("{JPN}カードが 告げているわ。\n今日は わたしの 日だって。"),
        COMPOUND_STRING("{JPN}カードの お告げは\n正しかったわ。"),
        COMPOUND_STRING("{JPN}カードを\nめくりまちがえたかしら……。"),
    },
    [FRONTIER_TRAINER_KENDRA] = { // サイキッカー ナリス
        COMPOUND_STRING("{JPN}目を 閉じても\nあなたの 動きが わかるの。"),
        COMPOUND_STRING("{JPN}目を 閉じたままでも\n勝てたわね。"),
        COMPOUND_STRING("{JPN}目を 開けて\n戦うべきだったわ……。"),
    },
    [FRONTIER_TRAINER_MOLLY] = { // オカルトマニア アマネ
        COMPOUND_STRING("{JPN}うふふ…… あなたに\n呪いを かけてあげる……。"),
        COMPOUND_STRING("{JPN}呪いが\nきいたみたいね……。"),
        COMPOUND_STRING("{JPN}呪いが 返って\nきちゃった……。"),
    },
    [FRONTIER_TRAINER_JAZMIN] = { // オカルトマニア ウノ
        COMPOUND_STRING("{JPN}夜に なると わたしの\nポケモンは 強くなるの……。"),
        COMPOUND_STRING("{JPN}あら 昼間でも\n十分 だったわね……。"),
        COMPOUND_STRING("{JPN}やっぱり 夜まで\n待つべきだったわ……。"),
    },
    [FRONTIER_TRAINER_KELSEY] = { // オカルトマニア ミサ
        COMPOUND_STRING("{JPN}あなたの 後ろに……\nいえ なんでも ないわ。"),
        COMPOUND_STRING("{JPN}後ろが 気に なって\n集中 できなかったでしょ？"),
        COMPOUND_STRING("{JPN}あなたの 後ろの 何かが\n味方 したのね……。"),
    },
    [FRONTIER_TRAINER_JALEN] = { // ポケモンマニア ゴウキ
        COMPOUND_STRING("{JPN}ポケモン図鑑の 説明なら\nぜんぶ 言えるぞ！"),
        COMPOUND_STRING("{JPN}知識は\n力なり！"),
        COMPOUND_STRING("{JPN}知識だけじゃ\n勝てない ものだな……。"),
    },
    [FRONTIER_TRAINER_GRIFFEN] = { // ポケモンマニア タテオ
        COMPOUND_STRING("{JPN}めずらしい ポケモンを\n見せてあげよう！"),
        COMPOUND_STRING("{JPN}めずらしくて 強い！\n最高だろう？"),
        COMPOUND_STRING("{JPN}きみの ポケモンも\nなかなか めずらしい……。"),
    },
    [FRONTIER_TRAINER_XANDER] = { // ポケモンマニア コウガ
        COMPOUND_STRING("{JPN}好きな ポケモンの\n話なら 一晩中 できるよ！"),
        COMPOUND_STRING("{JPN}勝ったら 話したく\nなっちゃうなあ！"),
        COMPOUND_STRING("{JPN}負けた 話は\nしたくないなあ……。"),
    },
    [FRONTIER_TRAINER_MARVIN] = { // ジェントルマン ハシ
        COMPOUND_STRING("{JPN}紳士たる もの\n勝負も 品格を 持って 臨むのだ。"),
        COMPOUND_STRING("{JPN}失礼。 少々\n本気を 出しすぎたかな。"),
        COMPOUND_STRING("{JPN}見事だ。\n紳士として 敬意を 表そう。"),
    },
    [FRONTIER_TRAINER_BRENNAN] = { // ジェントルマン キク
        COMPOUND_STRING("{JPN}わたしの 執事も\nこの 勝負を 見ている。"),
        COMPOUND_STRING("{JPN}執事に よい 姿を\n見せられたよ。"),
        COMPOUND_STRING("{JPN}執事に 合わせる\n顔が ないな……。"),
    },
    [FRONTIER_TRAINER_BALEY] = { // むしマニア ヒコキチ
        COMPOUND_STRING("{JPN}虫ポケモンの 鳴き声が\nわたしを 呼んでいる……。"),
        COMPOUND_STRING("{JPN}虫たちの 勝利の\n合唱が 聞こえるぞ！"),
        COMPOUND_STRING("{JPN}虫たちが\n静かに なってしまった……。"),
    },
    [FRONTIER_TRAINER_ZACKARY] = { // ルインマニア ヒサヤ
        COMPOUND_STRING("{JPN}古い 地図を たどって\nここへ 来たんだ。"),
        COMPOUND_STRING("{JPN}地図の 先には\n勝利が 待っていた！"),
        COMPOUND_STRING("{JPN}地図に のっていない\n強さだった……。"),
    },
    [FRONTIER_TRAINER_GABRIEL] = { // コレクター ホズミ
        COMPOUND_STRING("{JPN}勝負の 記録を\n集めるのが 趣味なんだ。"),
        COMPOUND_STRING("{JPN}いい 記録が\nひとつ 増えたよ！"),
        COMPOUND_STRING("{JPN}負けの 記録も\n大事に しまっておこう。"),
    },
    [FRONTIER_TRAINER_EMILY] = { // パラソルおねえさん セリカ
        COMPOUND_STRING("{JPN}かさの 下で\n優雅に お相手 いたしますわ。"),
        COMPOUND_STRING("{JPN}かさを 閉じるまでも\nなかったわね。"),
        COMPOUND_STRING("{JPN}かさの 下まで\n雨が 降り込んだみたい……。"),
    },
    [FRONTIER_TRAINER_JORDYN] = { // おとなのおねえさん パトリック
        COMPOUND_STRING("{JPN}美しさと 強さ\n両方 そろえて こそ 一流よ。"),
        COMPOUND_STRING("{JPN}一流の 勝負を\nお見せしたわ。"),
        COMPOUND_STRING("{JPN}あなたも 一流の\nトレーナーなのね。"),
    },
    [FRONTIER_TRAINER_SOFIA] = { // アロマなおねえさん ルリカ
        COMPOUND_STRING("{JPN}ラベンダーの 香りで\nリラックスして 戦いましょ。"),
        COMPOUND_STRING("{JPN}リラックス してたら\n勝っちゃったわ。"),
        COMPOUND_STRING("{JPN}リラックス しすぎて\nしまったかしら……。"),
    },
    [FRONTIER_TRAINER_BRADEN] = { // エリートトレーナー タツ
        COMPOUND_STRING("{JPN}ここまで 勝ち進んだ\n実力を 見せてもらおう！"),
        COMPOUND_STRING("{JPN}ここで 終わりだ。\n出直して きな。"),
        COMPOUND_STRING("{JPN}本物の 実力だな。\n認めるよ。"),
    },
    [FRONTIER_TRAINER_KAYDEN] = { // エリートトレーナー リュウガ
        COMPOUND_STRING("{JPN}データは そろっている。\nあとは 実行するだけだ。"),
        COMPOUND_STRING("{JPN}データどおりの\n結果だな。"),
        COMPOUND_STRING("{JPN}データを 超える\n動きだった……！"),
    },
    [FRONTIER_TRAINER_COOPER] = { // エリートトレーナー トミー
        COMPOUND_STRING("{JPN}エリートの 誇りに かけて\n負けるわけには いかない！"),
        COMPOUND_STRING("{JPN}誇りは\n守りぬいたぞ！"),
        COMPOUND_STRING("{JPN}誇りが\n傷ついたが……悔いは ない。"),
    },
    [FRONTIER_TRAINER_JULIA] = { // エリートトレーナー マユ
        COMPOUND_STRING("{JPN}わたしの 戦略に\nついてこられるかしら？"),
        COMPOUND_STRING("{JPN}戦略の 勝利よ。\n出直しなさい。"),
        COMPOUND_STRING("{JPN}戦略を 読まれて\nいたのね……。"),
    },
    [FRONTIER_TRAINER_AMARA] = { // エリートトレーナー フミコ
        COMPOUND_STRING("{JPN}この 施設の 頂点を\n目指しているの。 道を あけて！"),
        COMPOUND_STRING("{JPN}頂点まで\nあと 少しよ！"),
        COMPOUND_STRING("{JPN}頂点は まだ\n遠いのね……。"),
    },
    [FRONTIER_TRAINER_LYNN] = { // エリートトレーナー フユミ
        COMPOUND_STRING("{JPN}冷静に いきましょう。\n熱くなった ほうが 負けよ。"),
        COMPOUND_STRING("{JPN}ほらね。\n冷静さが 勝負を 決めたわ。"),
        COMPOUND_STRING("{JPN}わたしが\n熱くなって いたのね……。"),
    },
    [FRONTIER_TRAINER_JOVAN] = { // ポケモンレンジャー ヨシテル
        COMPOUND_STRING("{JPN}自然を 守る ために\nきたえた 力を 見せよう！"),
        COMPOUND_STRING("{JPN}守る 力は\n強い 力でも あるのさ！"),
        COMPOUND_STRING("{JPN}きみも 何かを 守るために\n戦っているんだね。"),
    },
    [FRONTIER_TRAINER_DOMINIC] = { // ポケモンレンジャー マサハル
        COMPOUND_STRING("{JPN}パトロールの 途中だが\n少しだけ 相手を しよう！"),
        COMPOUND_STRING("{JPN}よし！ パトロールに\n戻るとしよう！"),
        COMPOUND_STRING("{JPN}パトロールを サボったと\n思われちまうな……。"),
    },
    [FRONTIER_TRAINER_NIKOLAS] = { // ポケモンレンジャー ショウジ
        COMPOUND_STRING("{JPN}森の 奥で 鍛えた\n野生の 勘を 見せてやる！"),
        COMPOUND_STRING("{JPN}勘は\nさえていたな！"),
        COMPOUND_STRING("{JPN}勘が\n鈍って いたか……。"),
    },
    [FRONTIER_TRAINER_VALERIA] = { // ポケモンレンジャー イベ
        COMPOUND_STRING("{JPN}ポケモンと 心を 通わせれば\nどんな 相手にも 勝てるわ！"),
        COMPOUND_STRING("{JPN}心が\n通じ合っていたわね！"),
        COMPOUND_STRING("{JPN}あなたの ほうが\n心が 通じていたのね……。"),
    },
    [FRONTIER_TRAINER_DELANEY] = { // ポケモンレンジャー マナカ (150)
        COMPOUND_STRING("{JPN}どんな 険しい 道でも\nわたしたちは 進むわ！"),
        COMPOUND_STRING("{JPN}険しい 道の 先に\n勝利が あったわ！"),
        COMPOUND_STRING("{JPN}この 道は\n険しすぎたわね……。"),
    },
    [FRONTIER_TRAINER_MEGHAN] = { // ポケモンレンジャー ヨシコ
        COMPOUND_STRING("{JPN}ポケモンを 助けるのが\nわたしの 仕事！ 勝負も 全力よ！"),
        COMPOUND_STRING("{JPN}ポケモンたちに\n助けられちゃった！"),
        COMPOUND_STRING("{JPN}助けを 呼びたい\n気分だわ……。"),
    },
    [FRONTIER_TRAINER_ROBERTO] = { // ドラゴンつかい ギュウゾウ
        COMPOUND_STRING("{JPN}ドラゴンの 怒りを\nその 身に 受けよ！"),
        COMPOUND_STRING("{JPN}ドラゴンこそ\n最強の 証よ！"),
        COMPOUND_STRING("{JPN}ドラゴンを\n従えるには まだ 早かったか……。"),
    },
    [FRONTIER_TRAINER_DAMIAN] = { // ドラゴンつかい ラモン
        COMPOUND_STRING("{JPN}竜の 里で 育った\nおれの 力を 見ろ！"),
        COMPOUND_STRING("{JPN}竜の 里の 名に\nかけて 負けられんのだ！"),
        COMPOUND_STRING("{JPN}里の 長老に\nしかられそうだ……。"),
    },
    [FRONTIER_TRAINER_BRODY] = { // ドラゴンつかい ヒロカツ
        COMPOUND_STRING("{JPN}ドラゴンと 共に\n天を 目指すのだ！"),
        COMPOUND_STRING("{JPN}天に また\n一歩 近づいた！"),
        COMPOUND_STRING("{JPN}天は まだ\n高いな……。"),
    },
    [FRONTIER_TRAINER_GRAHAM] = { // ドラゴンつかい コテツ
        COMPOUND_STRING("{JPN}鋼のような うろこを 持つ\nドラゴンの 力を 見せよう！"),
        COMPOUND_STRING("{JPN}このうろこは\n伊達では ないぞ！"),
        COMPOUND_STRING("{JPN}うろこを\n貫かれるとは……！"),
    },
    [FRONTIER_TRAINER_TYLOR] = { // ポケモンだいすきクラブ ノブハル
        COMPOUND_STRING("{JPN}ポケモンの 写真を\n毎日 千枚 撮っているんだ！"),
        COMPOUND_STRING("{JPN}勝利の 瞬間も\n写真に 収めたぞ！"),
        COMPOUND_STRING("{JPN}負けた 顔は\n撮らないでくれよ……。"),
    },
    [FRONTIER_TRAINER_JAREN] = { // ポケモンだいすきクラブ リョウ
        COMPOUND_STRING("{JPN}ポケモンの 洋服を\n手作りしてるの！ かわいいでしょ？"),
        COMPOUND_STRING("{JPN}かわいい 服で\n勝てちゃった！"),
        COMPOUND_STRING("{JPN}服が\n汚れちゃったわ……。"),
    },
    [FRONTIER_TRAINER_CORDELL] = { // ポケモンブリーダー クニハル
        COMPOUND_STRING("{JPN}育て屋さんの 秘伝を\n受け継いでいるんだ。"),
        COMPOUND_STRING("{JPN}秘伝の 力は\nだてじゃない！"),
        COMPOUND_STRING("{JPN}秘伝にも\n弱点が あったか……。"),
    },
    [FRONTIER_TRAINER_JAZLYN] = { // ポケモンブリーダー ハマエ
        COMPOUND_STRING("{JPN}愛情たっぷりに\n育てた 子たちよ！"),
        COMPOUND_STRING("{JPN}愛情が\n力に なったのね！"),
        COMPOUND_STRING("{JPN}愛情は 負けてないわ。\n作戦が 負けたのね。"),
    },
    [FRONTIER_TRAINER_ZACHERY] = { // たんぱんこぞう ジュビリー
        COMPOUND_STRING("{JPN}半ズボンは 一年中\nはいてるんだ！ 寒くないよ！"),
        COMPOUND_STRING("{JPN}半ズボンの ほうが\n動きやすいんだ！"),
        COMPOUND_STRING("{JPN}ちょっと 寒く\nなってきた……。"),
    },
    [FRONTIER_TRAINER_JOHAN] = { // たんぱんこぞう カズヤ
        COMPOUND_STRING("{JPN}友だちと 特訓したんだ！\nその 成果を 見せるよ！"),
        COMPOUND_STRING("{JPN}特訓の 成果\nばっちりだね！"),
        COMPOUND_STRING("{JPN}友だちに\nなんて 言おう……。"),
    },
    [FRONTIER_TRAINER_SHEA] = { // ミニスカート カツミ
        COMPOUND_STRING("{JPN}負けず嫌い なのよ。\n本気で いくからね！"),
        COMPOUND_STRING("{JPN}ふふん！\n当然の 結果よ！"),
        COMPOUND_STRING("{JPN}くやしい！\nもう一回 やりたい！"),
    },
    [FRONTIER_TRAINER_KAILA] = { // ミニスカート ジリアン
        COMPOUND_STRING("{JPN}おしゃれも 勝負も\n手を 抜かないの！"),
        COMPOUND_STRING("{JPN}勝った わたしは\nもっと おしゃれね！"),
        COMPOUND_STRING("{JPN}髪が\n乱れちゃった……。"),
    },
    [FRONTIER_TRAINER_ISIAH] = { // じゅくがえり ショウスケ
        COMPOUND_STRING("{JPN}宿題は 終わらせてきた！\nあとは 勝つだけだ！"),
        COMPOUND_STRING("{JPN}今日は\n完璧な 一日だね！"),
        COMPOUND_STRING("{JPN}宿題より\nむずかしかった……。"),
    },
    [FRONTIER_TRAINER_GARRETT] = { // じゅくがえり トモユキ
        COMPOUND_STRING("{JPN}理科の 実験みたいに\n勝負を 分析するよ！"),
        COMPOUND_STRING("{JPN}仮説どおりの\n結果だ！"),
        COMPOUND_STRING("{JPN}仮説が\nまちがっていた……。"),
    },
    [FRONTIER_TRAINER_HAYLIE] = { // じゅくがえり シズコ
        COMPOUND_STRING("{JPN}委員長として\nみっともない 勝負は できないわ！"),
        COMPOUND_STRING("{JPN}委員長の\n面目は 保てたわ！"),
        COMPOUND_STRING("{JPN}クラスの みんなには\nないしょに してね……。"),
    },
    [FRONTIER_TRAINER_MEGAN] = { // じゅくがえり イリス
        COMPOUND_STRING("{JPN}図書館で 調べた\n知識を 試させて！"),
        COMPOUND_STRING("{JPN}本で 読んだ とおりね！"),
        COMPOUND_STRING("{JPN}本には\n書いてなかったわ……。"),
    },
    [FRONTIER_TRAINER_ISSAC] = { // おぼっちゃま ケンシ
        COMPOUND_STRING("{JPN}ぼくの 家には プールが\n三つも あるんだ！ すごいだろ？"),
        COMPOUND_STRING("{JPN}家も すごいし\n勝負も すごいのさ！"),
        COMPOUND_STRING("{JPN}プールで 泳いで\n頭を 冷やしてくるよ……。"),
    },
    [FRONTIER_TRAINER_QUINTON] = { // おぼっちゃま ズイシ
        COMPOUND_STRING("{JPN}最高級の きのみで\n育てた ポケモンだよ。"),
        COMPOUND_STRING("{JPN}やっぱり\n最高級は ちがうね！"),
        COMPOUND_STRING("{JPN}きのみの 値段じゃ\n決まらないのか……。"),
    },
    [FRONTIER_TRAINER_SALMA] = { // おじょうさま ジキ
        COMPOUND_STRING("{JPN}本日は どうぞ\nよろしく お願いいたします。"),
        COMPOUND_STRING("{JPN}よい 勝負を\nありがとうございました。"),
        COMPOUND_STRING("{JPN}お見事ですわ。\n勉強に なりました。"),
    },
    [FRONTIER_TRAINER_ANSLEY] = { // おじょうさま マリー
        COMPOUND_STRING("{JPN}ばあやに 内緒で\n来てしまいましたの。"),
        COMPOUND_STRING("{JPN}勝ったのだから\nばあやも 許してくれますわ。"),
        COMPOUND_STRING("{JPN}ばあやに\n見つかる前に 帰らなくては……。"),
    },
    [FRONTIER_TRAINER_HOLDEN] = { // むしとりしょうねん カンタ
        COMPOUND_STRING("{JPN}夏休みの 自由研究は\n虫ポケモンの 強さに したんだ！"),
        COMPOUND_STRING("{JPN}研究の 成果が\n出たぞ！"),
        COMPOUND_STRING("{JPN}研究を\n書き直さなきゃ……。"),
    },
    [FRONTIER_TRAINER_LUCA] = { // むしとりしょうねん アヤキ
        COMPOUND_STRING("{JPN}麦わら帽子は\nむしとりの 必需品さ！"),
        COMPOUND_STRING("{JPN}今日の 収穫は\n勝利だ！"),
        COMPOUND_STRING("{JPN}帽子が 飛ばされた\n気分だよ……。"),
    },
    [FRONTIER_TRAINER_JAMISON] = { // ちびっこにんじゃ サノスケ
        COMPOUND_STRING("{JPN}忍法 分身の術！\nどれが 本物か わかるかな？"),
        COMPOUND_STRING("{JPN}本物を 見破れぬとは\nまだまだで ござるな！"),
        COMPOUND_STRING("{JPN}全部 見破られたで\nござる……。"),
    },
    [FRONTIER_TRAINER_GUNNAR] = { // ちびっこにんじゃ ハンゾウ
        COMPOUND_STRING("{JPN}名前だけは\n伝説の 忍者と 同じで ござる！"),
        COMPOUND_STRING("{JPN}伝説に\n一歩 近づいたで ござる！"),
        COMPOUND_STRING("{JPN}名前負け して\nしまったで ござる……。"),
    },
    [FRONTIER_TRAINER_CRAIG] = { // ぼうやたち カンクン
        COMPOUND_STRING("{JPN}ぼく 泳げるように\nなったんだ！ 勝負も できるよ！"),
        COMPOUND_STRING("{JPN}やったー！\nぼく すごいでしょ！"),
        COMPOUND_STRING("{JPN}ぶくぶく……。\n負けちゃった……。"),
    },
    [FRONTIER_TRAINER_PIERCE] = { // ぼうやたち オタワ
        COMPOUND_STRING("{JPN}お父さんと いっしょに\nここまで 来たんだ！"),
        COMPOUND_STRING("{JPN}お父さんに\n見せたかったなあ！"),
        COMPOUND_STRING("{JPN}お父さーん！\n負けちゃったよー！"),
    },
    [FRONTIER_TRAINER_REGINA] = { // おじょうちゃんたち ヤヤ
        COMPOUND_STRING("{JPN}ヤヤね ポケモン\nだいすき なの！"),
        COMPOUND_STRING("{JPN}ポケモンも ヤヤのこと\nだいすき なんだよ！"),
        COMPOUND_STRING("{JPN}ポケモン\nごめんね……。"),
    },
    [FRONTIER_TRAINER_ALISON] = { // おじょうちゃんたち キリカ
        COMPOUND_STRING("{JPN}お姉ちゃんの まねして\nがんばるの！"),
        COMPOUND_STRING("{JPN}お姉ちゃんより\n強いかも！"),
        COMPOUND_STRING("{JPN}お姉ちゃんなら\n勝てたのかな……。"),
    },
    [FRONTIER_TRAINER_HANK] = { // むしマニア カズホ
        COMPOUND_STRING("{JPN}どんな 虫ポケモンも\n愛して やまないのさ！"),
        COMPOUND_STRING("{JPN}愛は\n勝つのだ！"),
        COMPOUND_STRING("{JPN}愛が 足りなかった\nのだろうか……。"),
    },
    [FRONTIER_TRAINER_EARL] = { // むしマニア ヨウジロウ
        COMPOUND_STRING("{JPN}脱皮するたびに\n強くなるのが 虫ポケモンさ！"),
        COMPOUND_STRING("{JPN}また ひとつ\n殻を 破ったな！"),
        COMPOUND_STRING("{JPN}脱皮の 途中で\nやられて しまった……。"),
    },
    [FRONTIER_TRAINER_RAMIRO] = { // つりびと カンネン
        COMPOUND_STRING("{JPN}釣り糸が\nピンと 張っておる。 来るぞ！"),
        COMPOUND_STRING("{JPN}大当たりじゃ！"),
        COMPOUND_STRING("{JPN}糸が 切れて\nしもうた……。"),
    },
    [FRONTIER_TRAINER_HUNTER] = { // つりびと ヒロノリ
        COMPOUND_STRING("{JPN}朝まずめの 一番いい\n時間を 使って 来たんだぞ！"),
        COMPOUND_STRING("{JPN}早起きは\n三文の 得だな！"),
        COMPOUND_STRING("{JPN}早起き しても\nだめな 日は だめか……。"),
    },
    [FRONTIER_TRAINER_AIDEN] = { // ルインマニア キジロウ
        COMPOUND_STRING("{JPN}遺跡の 謎を 解く\n鍵を 探しているんだ！"),
        COMPOUND_STRING("{JPN}謎が ひとつ\n解けた 気分だ！"),
        COMPOUND_STRING("{JPN}謎は\n深まるばかりだ……。"),
    },
    [FRONTIER_TRAINER_XAVIER] = { // ルインマニア シャロン
        COMPOUND_STRING("{JPN}古い 文字に\n勝負の 秘けつが 隠れているの。"),
        COMPOUND_STRING("{JPN}秘けつは\n本物だったわ！"),
        COMPOUND_STRING("{JPN}読みまちがえて\nいたのかしら……。"),
    },
    [FRONTIER_TRAINER_CLINTON] = { // コレクター リマー
        COMPOUND_STRING("{JPN}世界に ひとつだけの\nコレクションを 見せよう！"),
        COMPOUND_STRING("{JPN}どうだい？\n価値が わかるかい？"),
        COMPOUND_STRING("{JPN}きみの 強さこそ\n世界に ひとつだけだ……。"),
    },
    [FRONTIER_TRAINER_JESSE] = { // コレクター トーヤ
        COMPOUND_STRING("{JPN}道具も ポケモンも\nいい物だけを 集めているんだ。"),
        COMPOUND_STRING("{JPN}いい物は\n裏切らないのさ！"),
        COMPOUND_STRING("{JPN}使い方が\nまずかったか……。"),
    },
    [FRONTIER_TRAINER_EDUARDO] = { // ギターリスト ロイド
        COMPOUND_STRING("{JPN}ロックな 勝負を\nしようぜ！ イエー！"),
        COMPOUND_STRING("{JPN}最高に\nロックだったぜ！"),
        COMPOUND_STRING("{JPN}アンプが\n壊れちまったぜ……。"),
    },
    [FRONTIER_TRAINER_HAL] = { // ギターリスト デイブ
        COMPOUND_STRING("{JPN}静かな 曲も\n激しい 勝負も 好きなんだ。"),
        COMPOUND_STRING("{JPN}いい 演奏だった\nだろう？"),
        COMPOUND_STRING("{JPN}きみの 演奏の\nほうが 上だったな……。"),
    },
    [FRONTIER_TRAINER_GAGE] = { // とりつかい ナゴム
        COMPOUND_STRING("{JPN}鳥ポケモンの 背に 乗って\n空を 飛ぶのが 夢なんだ！"),
        COMPOUND_STRING("{JPN}夢に 一歩\n近づいたぞ！"),
        COMPOUND_STRING("{JPN}夢は\nまだ 遠いなあ……。"),
    },
    [FRONTIER_TRAINER_ARNOLD] = { // とりつかい ケイマ
        COMPOUND_STRING("{JPN}空から 見れば\n勝負の 流れも 一目で わかる！"),
        COMPOUND_STRING("{JPN}空からの\n眺めは 最高だ！"),
        COMPOUND_STRING("{JPN}地上の ほうが\nよく 見えていたか……。"),
    },
    [FRONTIER_TRAINER_JARRETT] = { // ふなのり ラザ
        COMPOUND_STRING("{JPN}七つの 海を 渡ってきた\nおれに 勝てるかな？"),
        COMPOUND_STRING("{JPN}八つめの 海も\n制覇したぜ！"),
        COMPOUND_STRING("{JPN}七つの 海より\nきみの ほうが 手ごわかった。"),
    },
    [FRONTIER_TRAINER_GARETT] = { // ふなのり シュウタ
        COMPOUND_STRING("{JPN}錨を 上げろー！\n全速前進だ！"),
        COMPOUND_STRING("{JPN}全速前進で\n大勝利だ！"),
        COMPOUND_STRING("{JPN}錨を 下ろして\n休むとするか……。"),
    },
    [FRONTIER_TRAINER_EMANUEL] = { // やまおとこ ブンチョウ
        COMPOUND_STRING("{JPN}山の 天気と 勝負は\n変わりやすいものだ。"),
        COMPOUND_STRING("{JPN}今日は\n晴天だったな！"),
        COMPOUND_STRING("{JPN}急に\n雨が 降ってきたな……。"),
    },
    [FRONTIER_TRAINER_GUSTAVO] = { // やまおとこ ブンケン
        COMPOUND_STRING("{JPN}どんな 岩壁でも\nよじ登ってきた！"),
        COMPOUND_STRING("{JPN}この 壁も\n登りきったぞ！"),
        COMPOUND_STRING("{JPN}足を\n踏み外しちまった……。"),
    },
    [FRONTIER_TRAINER_KAMERON] = { // ほのおのぼうや チカゾウ
        COMPOUND_STRING("{JPN}炎の 技は\nおれの 十八番だ！"),
        COMPOUND_STRING("{JPN}熱い 勝負を\nありがとうよ！"),
        COMPOUND_STRING("{JPN}炎が\n燃え移っちまったか……。"),
    },
    [FRONTIER_TRAINER_ALFREDO] = { // ほのおのぼうや トミタカ
        COMPOUND_STRING("{JPN}汗が 止まらないぜ！\n熱い 勝負を しよう！"),
        COMPOUND_STRING("{JPN}汗だくの\n勝利だ！"),
        COMPOUND_STRING("{JPN}冷や汗が\n出てきたぜ……。"),
    },
    [FRONTIER_TRAINER_RUBEN] = { // ジェントルマン ジュン
        COMPOUND_STRING("{JPN}どうぞ お手柔らかに。\nいや 本気で 来たまえ。"),
        COMPOUND_STRING("{JPN}本気で 来たきみに\n本気で 応えたまでだ。"),
        COMPOUND_STRING("{JPN}いや 参った。\nすばらしい 腕前だ。"),
    },
    [FRONTIER_TRAINER_LAMAR] = { // ジェントルマン パーカー
        COMPOUND_STRING("{JPN}わが 家に 伝わる\n紳士の 戦い方を お見せしよう。"),
        COMPOUND_STRING("{JPN}家訓に 従った\nまでだよ。"),
        COMPOUND_STRING("{JPN}家訓を 見直す\n時期かも しれんな。"),
    },
    [FRONTIER_TRAINER_JAXON] = { // たんぱんこぞう トモアキ (200)
        COMPOUND_STRING("{JPN}ここまで 勝ってきたんだ！\nきみにも 勝つぞ！"),
        COMPOUND_STRING("{JPN}連勝記録\n更新中だよ！"),
        COMPOUND_STRING("{JPN}連勝が\n止まっちゃった……。"),
    },
    [FRONTIER_TRAINER_LOGAN] = { // たんぱんこぞう シズオ
        COMPOUND_STRING("{JPN}ぼく 名前は 静かだけど\n勝負は にぎやかだよ！"),
        COMPOUND_STRING("{JPN}にぎやかな\n勝負だったね！"),
        COMPOUND_STRING("{JPN}しーん……。"),
    },
    [FRONTIER_TRAINER_EMILEE] = { // ミニスカート クニエ
        COMPOUND_STRING("{JPN}かわいいからって\n油断したら 痛い目 見るわよ！"),
        COMPOUND_STRING("{JPN}ほら\n油断したでしょ？"),
        COMPOUND_STRING("{JPN}油断してたのは\nわたしの ほうだったわ……。"),
    },
    [FRONTIER_TRAINER_JOSIE] = { // ミニスカート ニコル
        COMPOUND_STRING("{JPN}今日の 占いは 大吉！\n負ける気が しないわ！"),
        COMPOUND_STRING("{JPN}占いは\n当たるのよ！"),
        COMPOUND_STRING("{JPN}占いなんて\nもう 信じない！"),
    },
    [FRONTIER_TRAINER_ARMANDO] = { // キャンプボーイ マスジ
        COMPOUND_STRING("{JPN}飯ごうで 炊いた ごはんで\nパワー全開だ！"),
        COMPOUND_STRING("{JPN}ごはんの 力は\nすごいだろう！"),
        COMPOUND_STRING("{JPN}ごはんが\nこげていたかな……。"),
    },
    [FRONTIER_TRAINER_SKYLER] = { // キャンプボーイ シンキチ
        COMPOUND_STRING("{JPN}寝袋で 寝ると\n朝から 元気 いっぱいさ！"),
        COMPOUND_STRING("{JPN}よく 寝た\nおかげだね！"),
        COMPOUND_STRING("{JPN}寝不足 だったかも……。"),
    },
    [FRONTIER_TRAINER_RUTH] = { // ピクニックガール ヒトエ
        COMPOUND_STRING("{JPN}サンドイッチを\nたくさん 作ってきたの！"),
        COMPOUND_STRING("{JPN}勝ったから\nおいしく 食べられるわ！"),
        COMPOUND_STRING("{JPN}やけ食い しちゃおう\nかしら……。"),
    },
    [FRONTIER_TRAINER_MELODY] = { // ピクニックガール リサ
        COMPOUND_STRING("{JPN}原っぱで のびのび\n育った ポケモンたちよ！"),
        COMPOUND_STRING("{JPN}のびのび\n戦えたわ！"),
        COMPOUND_STRING("{JPN}ちょっと\n窮屈 だったかしら……。"),
    },
    [FRONTIER_TRAINER_PEDRO] = { // かいパンやろう ウミヘイ
        COMPOUND_STRING("{JPN}海の 男の 名に かけて\n負けられないぜ！"),
        COMPOUND_STRING("{JPN}海の 男は\n強いんだぜ！"),
        COMPOUND_STRING("{JPN}海の 男が\n陸で 負けるとは……。"),
    },
    [FRONTIER_TRAINER_ERICK] = { // かいパンやろう ゴウジン
        COMPOUND_STRING("{JPN}この 筋肉を 見ろ！\n泳いで 鍛えたんだ！"),
        COMPOUND_STRING("{JPN}筋肉は\nうそを つかない！"),
        COMPOUND_STRING("{JPN}筋肉だけじゃ\nだめなのか……。"),
    },
    [FRONTIER_TRAINER_ELAINE] = { // ビキニのおねえさん アンナ
        COMPOUND_STRING("{JPN}夏の 日差しみたいに\n熱い 勝負を しましょ！"),
        COMPOUND_STRING("{JPN}熱く なれたわ！\nありがとう！"),
        COMPOUND_STRING("{JPN}日焼け しちゃった\nだけかしら……。"),
    },
    [FRONTIER_TRAINER_JOYCE] = { // ビキニのおねえさん ハイネ
        COMPOUND_STRING("{JPN}イルカみたいに\n軽やかに 戦うわよ！"),
        COMPOUND_STRING("{JPN}軽やかに\n決まったわね！"),
        COMPOUND_STRING("{JPN}体が\n重かったみたい……。"),
    },
    [FRONTIER_TRAINER_TODD] = { // ポケモンだいすきクラブ バンリ
        COMPOUND_STRING("{JPN}わしの ポケモンの\n話を 聞いてくれんか？"),
        COMPOUND_STRING("{JPN}勝ったから\n話も 長くなるぞ！"),
        COMPOUND_STRING("{JPN}負けても\n話は 聞いてもらうぞ……。"),
    },
    [FRONTIER_TRAINER_GAVIN] = { // ポケモンだいすきクラブ チカオ
        COMPOUND_STRING("{JPN}ポケモンの ために\n毎日 歌を 歌っているんだ！"),
        COMPOUND_STRING("{JPN}歌の 力が\n届いたね！"),
        COMPOUND_STRING("{JPN}今日は\n音痴 だったかな……。"),
    },
    [FRONTIER_TRAINER_MALORY] = { // ポケモンだいすきクラブ ハルエ
        COMPOUND_STRING("{JPN}かわいい かわいい\nうちの子を 見てちょうだいな！"),
        COMPOUND_STRING("{JPN}ほら かわいいうえに\n強いのよ！"),
        COMPOUND_STRING("{JPN}負けても\nかわいいわね……。"),
    },
    [FRONTIER_TRAINER_ESTHER] = { // ポケモンだいすきクラブ スガコ
        COMPOUND_STRING("{JPN}ポケモンの ために\nお菓子を 焼いてきたのよ。"),
        COMPOUND_STRING("{JPN}お菓子で\nパワー全開ね！"),
        COMPOUND_STRING("{JPN}お菓子が\n足りなかったかしら。"),
    },
    [FRONTIER_TRAINER_OSCAR] = { // ポケモンブリーダー ナツノスケ
        COMPOUND_STRING("{JPN}夏に 生まれた 子は\n元気が いいんだ！"),
        COMPOUND_STRING("{JPN}夏の 元気さが\n勝因だな！"),
        COMPOUND_STRING("{JPN}夏バテ\nしていたかな……。"),
    },
    [FRONTIER_TRAINER_WILSON] = { // ポケモンブリーダー シガノスケ
        COMPOUND_STRING("{JPN}育成には\n根気が 必要なのさ。"),
        COMPOUND_STRING("{JPN}根気の\n勝利だよ。"),
        COMPOUND_STRING("{JPN}根気よく\nまた 育て直すよ。"),
    },
    [FRONTIER_TRAINER_CLARE] = { // ポケモンブリーダー テツコ
        COMPOUND_STRING("{JPN}厳しく 育てたから\nどんな 相手にも 負けないわ！"),
        COMPOUND_STRING("{JPN}厳しさが\n実を 結んだわね！"),
        COMPOUND_STRING("{JPN}厳しすぎたのかしら……。"),
    },
    [FRONTIER_TRAINER_TESS] = { // ポケモンブリーダー トキ
        COMPOUND_STRING("{JPN}時間を かけて\n育てた 子たちよ。"),
        COMPOUND_STRING("{JPN}時間を かけた\nかいが あったわ。"),
        COMPOUND_STRING("{JPN}まだ 時間が\n足りなかったのね。"),
    },
    [FRONTIER_TRAINER_LEON] = { // エリートトレーナー ヒョウ
        COMPOUND_STRING("{JPN}獲物を ねらう\n豹のように 戦うぞ！"),
        COMPOUND_STRING("{JPN}獲物は\n逃がさない！"),
        COMPOUND_STRING("{JPN}逃げられたか……。"),
    },
    [FRONTIER_TRAINER_ALONZO] = { // エリートトレーナー リョウガ
        COMPOUND_STRING("{JPN}全国を 巡って\n腕を 磨いてきた！"),
        COMPOUND_STRING("{JPN}旅の 成果だな！"),
        COMPOUND_STRING("{JPN}まだ 旅は\n終われないようだ……。"),
    },
    [FRONTIER_TRAINER_VINCE] = { // エリートトレーナー ホクト
        COMPOUND_STRING("{JPN}北の 地で 鍛えた\nぶれない 心を 見せよう！"),
        COMPOUND_STRING("{JPN}心が\nぶれなかったな！"),
        COMPOUND_STRING("{JPN}心が\n揺らいで しまった……。"),
    },
    [FRONTIER_TRAINER_BRYON] = { // エリートトレーナー カイザ
        COMPOUND_STRING("{JPN}頂点に 立つ 者は\nただ 一人！ それは おれだ！"),
        COMPOUND_STRING("{JPN}頂点は\nおれの ものだ！"),
        COMPOUND_STRING("{JPN}頂点は\nきみに 譲るとしよう……。"),
    },
    [FRONTIER_TRAINER_AVA] = { // エリートトレーナー エレナ
        COMPOUND_STRING("{JPN}華麗に 決めて\nあげるわ！"),
        COMPOUND_STRING("{JPN}華麗な\n勝利でしょ？"),
        COMPOUND_STRING("{JPN}華麗さが\n足りなかったわ……。"),
    },
    [FRONTIER_TRAINER_MIRIAM] = { // エリートトレーナー ルリ
        COMPOUND_STRING("{JPN}どんな 相手も\n分析すれば 怖くないわ。"),
        COMPOUND_STRING("{JPN}分析どおりよ。"),
        COMPOUND_STRING("{JPN}分析が\n甘かったのね……。"),
    },
    [FRONTIER_TRAINER_CARRIE] = { // エリートトレーナー チヅコ
        COMPOUND_STRING("{JPN}千の 勝負を\nこなしてきた わたしに 挑むの？"),
        COMPOUND_STRING("{JPN}千と 一回目の\n勝利ね。"),
        COMPOUND_STRING("{JPN}千と 一回目は\n負けね……。"),
    },
    [FRONTIER_TRAINER_GILLIAN2] = { // エリートトレーナー ユキノ
        COMPOUND_STRING("{JPN}雪のように 静かに\nそして 確実に 決めるわ。"),
        COMPOUND_STRING("{JPN}静かに\n決まったわね。"),
        COMPOUND_STRING("{JPN}雪が\nとけちゃったわ……。"),
    },
    [FRONTIER_TRAINER_TYLER] = { // ポケモンレンジャー マサノブ
        COMPOUND_STRING("{JPN}どんな 困難にも\n立ち向かうのが レンジャーだ！"),
        COMPOUND_STRING("{JPN}困難を\n乗り越えたぞ！"),
        COMPOUND_STRING("{JPN}乗り越えられない\n壁だった……。"),
    },
    [FRONTIER_TRAINER_CHAZ] = { // ポケモンレンジャー ハルアキ
        COMPOUND_STRING("{JPN}春も 秋も\n野山を 駆け回ってきたんだ！"),
        COMPOUND_STRING("{JPN}野山で 鍛えた\n足腰の 勝利だ！"),
        COMPOUND_STRING("{JPN}もう 一度\n野山で 修行だな……。"),
    },
    [FRONTIER_TRAINER_NELSON] = { // ポケモンレンジャー ナミオ
        COMPOUND_STRING("{JPN}海辺の パトロールで\n鍛えた 力を 見せるよ！"),
        COMPOUND_STRING("{JPN}海辺は\nおれの 庭さ！"),
        COMPOUND_STRING("{JPN}大波に\nさらわれた 気分だ……。"),
    },
    [FRONTIER_TRAINER_SHANIA] = { // ポケモンレンジャー アユ
        COMPOUND_STRING("{JPN}川の 流れのように\nしなやかに 戦うわ！"),
        COMPOUND_STRING("{JPN}しなやかに\n決まったわね！"),
        COMPOUND_STRING("{JPN}流れを\nせき止められちゃった……。"),
    },
    [FRONTIER_TRAINER_STELLA] = { // ポケモンレンジャー ルリミ
        COMPOUND_STRING("{JPN}ポケモンの 声に\n耳を 傾けるのが 大事なのよ。"),
        COMPOUND_STRING("{JPN}ポケモンの 声が\nよく 聞こえたわ！"),
        COMPOUND_STRING("{JPN}あなたの ポケモンの 声も\n力強かったわ。"),
    },
    [FRONTIER_TRAINER_DORINE] = { // ポケモンレンジャー カホコ
        COMPOUND_STRING("{JPN}森の 仲間たちの\n力を 借りるわね！"),
        COMPOUND_STRING("{JPN}森の 仲間に\n感謝ね！"),
        COMPOUND_STRING("{JPN}森へ 帰って\n出直すわ……。"),
    },
    [FRONTIER_TRAINER_MADDOX] = { // ドラゴンつかい リュウマ
        COMPOUND_STRING("{JPN}竜の 血が 騒ぐ……！\nいざ 勝負！"),
        COMPOUND_STRING("{JPN}竜の 誇りは\n守られた！"),
        COMPOUND_STRING("{JPN}竜の 血が\n静まって しまった……。"),
    },
    [FRONTIER_TRAINER_DAVIN] = { // ドラゴンつかい コリュウ
        COMPOUND_STRING("{JPN}小さな 竜も\nいずれは 大きく なるのだ！"),
        COMPOUND_STRING("{JPN}竜は 確実に\n大きく なっている！"),
        COMPOUND_STRING("{JPN}まだ\n小さな 竜だったか……。"),
    },
    [FRONTIER_TRAINER_TREVON] = { // ドラゴンつかい サイガ
        COMPOUND_STRING("{JPN}竜の 牙を\nかわしきれるかな？"),
        COMPOUND_STRING("{JPN}竜の 牙は\n鋭いだろう！"),
        COMPOUND_STRING("{JPN}牙が\n折れて しまった……。"),
    },
    [FRONTIER_TRAINER_MATEO] = { // からておう ノブ
        COMPOUND_STRING("{JPN}押忍！\n全身全霊で 参る！"),
        COMPOUND_STRING("{JPN}押忍！\n修行の 成果だ！"),
        COMPOUND_STRING("{JPN}押忍……。\n完敗だ！"),
    },
    [FRONTIER_TRAINER_BRET] = { // からておう ソウマ
        COMPOUND_STRING("{JPN}一撃必殺！\nそれが おれの 流儀だ！"),
        COMPOUND_STRING("{JPN}一撃で\n決まったな！"),
        COMPOUND_STRING("{JPN}一撃が\n届かなかった……。"),
    },
    [FRONTIER_TRAINER_RAUL] = { // からておう ジョウ
        COMPOUND_STRING("{JPN}瓦 百枚 割りの\n力を 見せてやる！"),
        COMPOUND_STRING("{JPN}瓦も 勝負も\n真っ二つだ！"),
        COMPOUND_STRING("{JPN}割れたのは\nおれの 自信だった……。"),
    },
    [FRONTIER_TRAINER_KAY] = { // バトルガール アコ
        COMPOUND_STRING("{JPN}回し蹴りの 練習は\nばっちりよ！"),
        COMPOUND_STRING("{JPN}きれいに\n決まったわ！"),
        COMPOUND_STRING("{JPN}空振り\nしちゃった……。"),
    },
    [FRONTIER_TRAINER_ELENA] = { // バトルガール カナ
        COMPOUND_STRING("{JPN}汗を かくのって\n気持ちいいわよね！"),
        COMPOUND_STRING("{JPN}いい 汗\nかいたわ！"),
        COMPOUND_STRING("{JPN}汗だけじゃ\n勝てないのね……。"),
    },
    [FRONTIER_TRAINER_ALANA] = { // バトルガール タエ
        COMPOUND_STRING("{JPN}どんな 攻撃も\n受け止めてみせるわ！"),
        COMPOUND_STRING("{JPN}全部\n受け止めたわよ！"),
        COMPOUND_STRING("{JPN}受け止め\nきれなかった……。"),
    },
    [FRONTIER_TRAINER_ALEXAS] = { // ベテラントレーナー オトゾウ
        COMPOUND_STRING("{JPN}わしの 若い ころは\nもっと 強い 相手が おったぞ。"),
        COMPOUND_STRING("{JPN}まだまだ\n腕は 衰えておらん！"),
        COMPOUND_STRING("{JPN}きみは 昔の\n強者に 劣らんのう。"),
    },
    [FRONTIER_TRAINER_WESTON] = { // ベテラントレーナー グンペイ
        COMPOUND_STRING("{JPN}作戦とは\n相手の 一歩 先を 読むことだ。"),
        COMPOUND_STRING("{JPN}一歩 先を\n読めていたな。"),
        COMPOUND_STRING("{JPN}きみは 二歩 先を\n読んでいたか……。"),
    },
    [FRONTIER_TRAINER_JASPER] = { // ベテラントレーナー モキチ
        COMPOUND_STRING("{JPN}畑仕事の 合間に\n鍛えてきたんじゃ。"),
        COMPOUND_STRING("{JPN}畑で 鍛えた\n体は 丈夫じゃろう！"),
        COMPOUND_STRING("{JPN}畑に 戻って\n出直すかのう。"),
    },
    [FRONTIER_TRAINER_NADIA] = { // ベテラントレーナー マツ
        COMPOUND_STRING("{JPN}長生きの 秘けつは\n毎日の 勝負なのよ。"),
        COMPOUND_STRING("{JPN}これで また\n長生き できるわ。"),
        COMPOUND_STRING("{JPN}負けても\n楽しかったわ。"),
    },
    [FRONTIER_TRAINER_MIRANDA] = { // ベテラントレーナー キヨ
        COMPOUND_STRING("{JPN}清く 正しく\n美しく 戦いましょう。"),
        COMPOUND_STRING("{JPN}清らかな\n勝負でしたね。"),
        COMPOUND_STRING("{JPN}あなたの 戦い方は\n清らかでしたわ。"),
    },
    [FRONTIER_TRAINER_EMMA] = { // ベテラントレーナー イカホ
        COMPOUND_STRING("{JPN}温泉で 鍛えた\nのんびり戦法を お見せするわ。"),
        COMPOUND_STRING("{JPN}のんびりでも\n勝てるのよ。"),
        COMPOUND_STRING("{JPN}のんびり\nしすぎたわね……。"),
    },
    [FRONTIER_TRAINER_ROLANDO] = { // サイキッカー アサオ
        COMPOUND_STRING("{JPN}朝の 瞑想で\n勝利の 光景を 見たのだ。"),
        COMPOUND_STRING("{JPN}瞑想の\nとおりだ！"),
        COMPOUND_STRING("{JPN}瞑想が\n足りなかったか……。"),
    },
    [FRONTIER_TRAINER_STANLY] = { // サイキッカー トキオ (250)
        COMPOUND_STRING("{JPN}時間を 止める 力が\nあったら……なんてね。"),
        COMPOUND_STRING("{JPN}時間を 止めなくても\n勝てたよ。"),
        COMPOUND_STRING("{JPN}時間を 戻したい\n気分だ……。"),
    },
    [FRONTIER_TRAINER_DARIO] = { // サイキッカー レイ
        COMPOUND_STRING("{JPN}霊感が 強くてね。\nきみの オーラが 見えるよ。"),
        COMPOUND_STRING("{JPN}オーラが\n弱っていたね。"),
        COMPOUND_STRING("{JPN}こんなに 強い オーラは\n初めて 見た……。"),
    },
    [FRONTIER_TRAINER_KARLEE] = { // サイキッカー エナ
        COMPOUND_STRING("{JPN}ふふ……\nこの 勝負の 結末 知ってるの。"),
        COMPOUND_STRING("{JPN}ね？\n知っていた とおりでしょ。"),
        COMPOUND_STRING("{JPN}結末が\n書き換わっちゃった……。"),
    },
    [FRONTIER_TRAINER_JAYLIN] = { // サイキッカー エリサ
        COMPOUND_STRING("{JPN}わたしの 超能力と\nあなたの 実力 比べてみる？"),
        COMPOUND_STRING("{JPN}超能力の\n勝ちね。"),
        COMPOUND_STRING("{JPN}実力って\nすごいのね……。"),
    },
    [FRONTIER_TRAINER_INGRID] = { // サイキッカー ムツコ
        COMPOUND_STRING("{JPN}見えない 力で\nあなたを 包みこんであげる。"),
        COMPOUND_STRING("{JPN}包みこまれて\n動けなかった でしょ？"),
        COMPOUND_STRING("{JPN}見えない 力が\n跳ね返されたわ……。"),
    },
    [FRONTIER_TRAINER_DELILAH] = { // オカルトマニア ミナヨ
        COMPOUND_STRING("{JPN}ろうそくの 火が\n揺れているわ……。 来るのね。"),
        COMPOUND_STRING("{JPN}火は\n消えなかったわ……。"),
        COMPOUND_STRING("{JPN}火が\n消えてしまった……。"),
    },
    [FRONTIER_TRAINER_CARLY] = { // オカルトマニア トミコ
        COMPOUND_STRING("{JPN}ゴーストポケモンと\nお話し するのが 好きなの……。"),
        COMPOUND_STRING("{JPN}ゴーストたちも\n喜んでいるわ……。"),
        COMPOUND_STRING("{JPN}ゴーストたちが\nすねちゃった……。"),
    },
    [FRONTIER_TRAINER_LEXIE] = { // オカルトマニア ミサヨ
        COMPOUND_STRING("{JPN}夜中の 三時に\n修行 しているの……。"),
        COMPOUND_STRING("{JPN}夜中の 修行は\nききめが あるのよ……。"),
        COMPOUND_STRING("{JPN}今夜は\n眠れそうに ないわ……。"),
    },
    [FRONTIER_TRAINER_MILLER] = { // ポケモンマニア モリヤ
        COMPOUND_STRING("{JPN}ぼくの 部屋は\nポケモングッズで いっぱいさ！"),
        COMPOUND_STRING("{JPN}グッズより\n本物が いちばんだね！"),
        COMPOUND_STRING("{JPN}グッズでも 眺めて\n元気を 出そう……。"),
    },
    [FRONTIER_TRAINER_MARV] = { // ポケモンマニア ヒフミ
        COMPOUND_STRING("{JPN}ぼくの 好きな ポケモンを\n当ててみて！"),
        COMPOUND_STRING("{JPN}正解は\nぜんぶ さ！"),
        COMPOUND_STRING("{JPN}きみの ポケモンも\n好きに なっちゃった……。"),
    },
    [FRONTIER_TRAINER_LAYTON] = { // ポケモンマニア フミアキ
        COMPOUND_STRING("{JPN}ポケモンの 鳴き声\nぜんぶ まねできるよ！"),
        COMPOUND_STRING("{JPN}勝利の 雄たけびも\nまねしてみようか？"),
        COMPOUND_STRING("{JPN}負けた ときの 鳴き声は\nまねしたくないなあ……。"),
    },
    [FRONTIER_TRAINER_BROOKS] = { // ジェントルマン スミス
        COMPOUND_STRING("{JPN}遠い 国から\n紳士の たしなみとして 参った。"),
        COMPOUND_STRING("{JPN}たしなみも\n大切だろう？"),
        COMPOUND_STRING("{JPN}すばらしい。\n帰国の 土産話が できたよ。"),
    },
    [FRONTIER_TRAINER_GREGORY] = { // ジェントルマン コリン
        COMPOUND_STRING("{JPN}午後の 紅茶の 前に\n一勝負 いかがかな？"),
        COMPOUND_STRING("{JPN}これで 紅茶も\nおいしく いただける。"),
        COMPOUND_STRING("{JPN}紅茶が\n冷めてしまったな……。"),
    },
    [FRONTIER_TRAINER_REESE] = { // ジェントルマン ブラウン
        COMPOUND_STRING("{JPN}勝負は 礼に 始まり\n礼に 終わるのだよ。"),
        COMPOUND_STRING("{JPN}よい 勝負を\nありがとう。"),
        COMPOUND_STRING("{JPN}お見事。\n礼を 言わせて もらうよ。"),
    },
    [FRONTIER_TRAINER_MASON] = { // ランナー トーマ
        COMPOUND_STRING("{JPN}マラソン大会の 前に\n調子を 確かめたいんだ！"),
        COMPOUND_STRING("{JPN}調子は\n上々だな！"),
        COMPOUND_STRING("{JPN}調整し直さないと……。"),
    },
    [FRONTIER_TRAINER_TOBY] = { // ランナー ナツオ
        COMPOUND_STRING("{JPN}真夏の 炎天下でも\n走りぬいてきたぜ！"),
        COMPOUND_STRING("{JPN}暑さにも\n勝負にも 勝ったぜ！"),
        COMPOUND_STRING("{JPN}暑さに\nやられたかな……。"),
    },
    [FRONTIER_TRAINER_DOROTHY] = { // ランナー チサ
        COMPOUND_STRING("{JPN}一歩 一歩が\n勝利に つながるの！"),
        COMPOUND_STRING("{JPN}一歩ずつ\n積み上げた 勝利よ！"),
        COMPOUND_STRING("{JPN}一歩\n足りなかったわ……。"),
    },
    [FRONTIER_TRAINER_PIPER] = { // ランナー ミチ
        COMPOUND_STRING("{JPN}知らない 道を\n走るのが 好きなの！"),
        COMPOUND_STRING("{JPN}新しい 道で\n勝てちゃった！"),
        COMPOUND_STRING("{JPN}道に\n迷っちゃった……。"),
    },
    [FRONTIER_TRAINER_FINN] = { // スイマー ソウタ
        COMPOUND_STRING("{JPN}波を 切り裂く\nクロールを 見せてやるぜ！"),
        COMPOUND_STRING("{JPN}波を 切り裂いて\n一着だ！"),
        COMPOUND_STRING("{JPN}波に\n押し戻されたか……。"),
    },
    [FRONTIER_TRAINER_SAMIR] = { // スイマー テルアキ
        COMPOUND_STRING("{JPN}晴れた 日の 海は\n最高だね！ 勝負も 最高に しよう！"),
        COMPOUND_STRING("{JPN}最高の\n一日だ！"),
        COMPOUND_STRING("{JPN}雲が\n出てきたな……。"),
    },
    [FRONTIER_TRAINER_FIONA] = { // スイマー サキコ
        COMPOUND_STRING("{JPN}先頭を 泳ぐのは\nいつも わたしよ！"),
        COMPOUND_STRING("{JPN}今日も\n先頭ね！"),
        COMPOUND_STRING("{JPN}追い抜かれちゃった……。"),
    },
    [FRONTIER_TRAINER_GLORIA] = { // スイマー シオ
        COMPOUND_STRING("{JPN}潮の 満ち引きを\n読むのは 得意なの！"),
        COMPOUND_STRING("{JPN}潮目を\n読みきったわ！"),
        COMPOUND_STRING("{JPN}潮が\n引いちゃった……。"),
    },
    [FRONTIER_TRAINER_NICO] = { // サイクリング トウア
        COMPOUND_STRING("{JPN}どんな 道でも\nこの 自転車なら 走りきれる！"),
        COMPOUND_STRING("{JPN}ゴールまで\n走りきったぜ！"),
        COMPOUND_STRING("{JPN}タイヤが\nパンクしちまった……。"),
    },
    [FRONTIER_TRAINER_JEREMY] = { // サイクリング ナルオ
        COMPOUND_STRING("{JPN}ベルを 鳴らして\n勝負開始だ！ チリンチリン！"),
        COMPOUND_STRING("{JPN}勝利の ベルを\n鳴らそう！"),
        COMPOUND_STRING("{JPN}ベルの 音が\nむなしく 響くぜ……。"),
    },
    [FRONTIER_TRAINER_CAITLIN] = { // サイクリング サチエ
        COMPOUND_STRING("{JPN}下り坂は\n一気に 攻めるわよ！"),
        COMPOUND_STRING("{JPN}一気に\n攻めきったわ！"),
        COMPOUND_STRING("{JPN}ブレーキを\nかけすぎたかしら……。"),
    },
    [FRONTIER_TRAINER_REENA] = { // サイクリング コスズ
        COMPOUND_STRING("{JPN}小さな 体でも\nスピードは 負けないわ！"),
        COMPOUND_STRING("{JPN}スピードの\n勝ちね！"),
        COMPOUND_STRING("{JPN}もっと 速く\nこがなくちゃ……。"),
    },
    [FRONTIER_TRAINER_AVERY] = { // むしマニア サダハル
        COMPOUND_STRING("{JPN}虫ポケモンの 標本……\nいや 図鑑を 作っているんだ。"),
        COMPOUND_STRING("{JPN}図鑑の ページが\nまた 増えるな！"),
        COMPOUND_STRING("{JPN}きみの ポケモンも\n図鑑に 載せたいくらいだ。"),
    },
    [FRONTIER_TRAINER_LIAM] = { // むしマニア トシカズ
        COMPOUND_STRING("{JPN}夜の 森で\n光る 虫を 探していたんだ。"),
        COMPOUND_STRING("{JPN}勝利の 光が\n見えたよ！"),
        COMPOUND_STRING("{JPN}光が\n消えてしまった……。"),
    },
    [FRONTIER_TRAINER_THEO] = { // つりびと キハチ
        COMPOUND_STRING("{JPN}釣り竿 一本で\n世界を 旅しておる。"),
        COMPOUND_STRING("{JPN}旅の 土産が\nまた 増えたわい！"),
        COMPOUND_STRING("{JPN}次の 釣り場へ\n向かうと するか……。"),
    },
    [FRONTIER_TRAINER_BAILEY] = { // つりびと ノンキ
        COMPOUND_STRING("{JPN}まあ そう あせらず\nのんびり いこうや。"),
        COMPOUND_STRING("{JPN}のんびりでも\n勝てるもんだ。"),
        COMPOUND_STRING("{JPN}のんびり しすぎたか。\nまあ いいさ。"),
    },
    [FRONTIER_TRAINER_HUGO] = { // ルインマニア カンジ
        COMPOUND_STRING("{JPN}遺跡の 壁画に\nこの 勝負が 描かれていた！"),
        COMPOUND_STRING("{JPN}壁画の とおりの\n結末だ！"),
        COMPOUND_STRING("{JPN}壁画を 逆さまに\n見ていたかも……。"),
    },
    [FRONTIER_TRAINER_BRYCE] = { // ルインマニア ノリユキ
        COMPOUND_STRING("{JPN}つるはしの 代わりに\nポケモンで 道を 切り開く！"),
        COMPOUND_STRING("{JPN}道は\n開けたぞ！"),
        COMPOUND_STRING("{JPN}岩盤が\n固すぎた……。"),
    },
    [FRONTIER_TRAINER_GIDEON] = { // コレクター スミオ
        COMPOUND_STRING("{JPN}古い コインを 集めてるんだ。\n勝負は 表か 裏か！"),
        COMPOUND_STRING("{JPN}表が 出たな！"),
        COMPOUND_STRING("{JPN}裏が\n出ちまった……。"),
    },
    [FRONTIER_TRAINER_TRISTON] = { // コレクター ヒサオ
        COMPOUND_STRING("{JPN}長年 集めた\nとっておきの ポケモンだ！"),
        COMPOUND_STRING("{JPN}とっておきは\n伊達じゃない！"),
        COMPOUND_STRING("{JPN}とっておきでも\nかなわなかった……。"),
    },
    [FRONTIER_TRAINER_CHARLES] = { // ギターリスト カズサ
        COMPOUND_STRING("{JPN}即興演奏みたいに\n自由に 戦うぜ！"),
        COMPOUND_STRING("{JPN}いい アドリブ\nだったろう？"),
        COMPOUND_STRING("{JPN}アドリブが\n空回り しちまった……。"),
    },
    [FRONTIER_TRAINER_RAYMOND] = { // ギターリスト トシヤ
        COMPOUND_STRING("{JPN}ステージは ここだ！\n観客は きみだけだぜ！"),
        COMPOUND_STRING("{JPN}拍手は\nいらないぜ！"),
        COMPOUND_STRING("{JPN}ブーイングは\nやめてくれよ……。"),
    },
    [FRONTIER_TRAINER_DIRK] = { // とりつかい ヨリト
        COMPOUND_STRING("{JPN}渡り鳥のように\n各地の 施設を 巡っているんだ。"),
        COMPOUND_STRING("{JPN}この 施設でも\n勝てたぞ！"),
        COMPOUND_STRING("{JPN}次の 施設へ\n渡るとするか……。"),
    },
    [FRONTIER_TRAINER_HAROLD] = { // とりつかい スミト
        COMPOUND_STRING("{JPN}朝日と 共に\n鳥たちと 飛び立つんだ！"),
        COMPOUND_STRING("{JPN}いい 朝に\nなったな！"),
        COMPOUND_STRING("{JPN}夕日が\nしみるぜ……。"),
    },
    [FRONTIER_TRAINER_OMAR] = { // ふなのり タカ
        COMPOUND_STRING("{JPN}嵐の 夜も\n乗り越えてきたぜ！"),
        COMPOUND_STRING("{JPN}嵐より\n楽な もんだ！"),
        COMPOUND_STRING("{JPN}きみは\n嵐より 手ごわいな……。"),
    },
    [FRONTIER_TRAINER_PETER] = { // ふなのり ナンリ
        COMPOUND_STRING("{JPN}南の 海で\n鍛えた 腕を 見せてやる！"),
        COMPOUND_STRING("{JPN}南の 海の\n男は 強いぜ！"),
        COMPOUND_STRING("{JPN}南の 海へ\n帰りたくなったぜ……。"),
    },
    [FRONTIER_TRAINER_DEV] = { // やまおとこ タイガ
        COMPOUND_STRING("{JPN}大河を 渡り\n山を 越えてきた！"),
        COMPOUND_STRING("{JPN}大河のように\n悠々と 勝ったぞ！"),
        COMPOUND_STRING("{JPN}流れに\nのまれちまった……。"),
    },
    [FRONTIER_TRAINER_COREY] = { // やまおとこ センゴ
        COMPOUND_STRING("{JPN}千の 山を 越えるまで\n歩みは 止めないぞ！"),
        COMPOUND_STRING("{JPN}また ひとつ\n山を 越えた！"),
        COMPOUND_STRING("{JPN}この 山は\n高かった……。"),
    },
    [FRONTIER_TRAINER_ANDRE] = { // ほのおのぼうや ショウキチ
        COMPOUND_STRING("{JPN}炎の ように\n真っ赤に 燃えてるぜ！"),
        COMPOUND_STRING("{JPN}真っ赤な\n勝利だ！"),
        COMPOUND_STRING("{JPN}真っ白に\n燃え尽きた……。"),
    },
    [FRONTIER_TRAINER_FERRIS] = { // ほのおのぼうや チカシ
        COMPOUND_STRING("{JPN}火の 粉を 浴びて\n強くなった おれを 見ろ！"),
        COMPOUND_STRING("{JPN}熱い 勝負\nだったぜ！"),
        COMPOUND_STRING("{JPN}水を\nかけられた 気分だ……。"),
    },
    [FRONTIER_TRAINER_ALIVIA] = { // パラソルおねえさん ヨシミ
        COMPOUND_STRING("{JPN}かさを 開いたら\n勝負の 始まりよ。"),
        COMPOUND_STRING("{JPN}かさを 閉じて\nお祝いね。"),
        COMPOUND_STRING("{JPN}かさの 骨が\n折れちゃった……。"),
    },
    [FRONTIER_TRAINER_PAIGE] = { // パラソルおねえさん マイ
        COMPOUND_STRING("{JPN}かさを くるくる\n回しながら 舞うように 戦うわ。"),
        COMPOUND_STRING("{JPN}華麗に\n舞えたわね。"),
        COMPOUND_STRING("{JPN}目が\n回っちゃった……。"),
    },
    [FRONTIER_TRAINER_ANYA] = { // おとなのおねえさん ナオミ
        COMPOUND_STRING("{JPN}大人の 余裕って\nものを 見せてあげる。"),
        COMPOUND_STRING("{JPN}余裕の\n勝利ね。"),
        COMPOUND_STRING("{JPN}余裕が\nなくなっちゃった……。"),
    },
    [FRONTIER_TRAINER_DAWN] = { // おとなのおねえさん ユカ
        COMPOUND_STRING("{JPN}きれいな 勝ち方に\nこだわりが あるの。"),
        COMPOUND_STRING("{JPN}きれいに\n決まったでしょ？"),
        COMPOUND_STRING("{JPN}きれいに\n負けちゃったわ……。"),
    },
    [FRONTIER_TRAINER_ABBY] = { // アロマなおねえさん マリカ
        COMPOUND_STRING("{JPN}ばらの 香りは\n勝利の 香りよ。"),
        COMPOUND_STRING("{JPN}ばらが\n咲き誇ったわ！"),
        COMPOUND_STRING("{JPN}ばらの トゲが\n刺さっちゃった……。"),
    },
    [FRONTIER_TRAINER_GRETEL] = { // アロマなおねえさん ヤスコ
        COMPOUND_STRING("{JPN}森の 香りで\n心を 落ち着けて……。"),
        COMPOUND_STRING("{JPN}落ち着いて\n戦えたわ。"),
        COMPOUND_STRING("{JPN}心が\nざわついちゃった……。"),
    },
};
