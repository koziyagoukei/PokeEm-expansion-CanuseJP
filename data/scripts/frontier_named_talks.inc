@ Cured Brains and guests meeting on the Frontier's squares once all of a group are cured
@ (NAMED_TALK_*, see src/data/frontier_named_talks.h). Talked to, they talk among themselves once
@ and go on their way.

NamedTalk_EventScript_MagmaAquaLeaders::
	lockall
	msgbox NamedTalk_Text_MagmaAquaLeaders, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_TALK_LEADERS_MAXIE
	removeobject LOCALID_TALK_LEADERS_ARCHIE
	setvar VAR_0x8004, NAMED_TALK_MAGMA_AQUA_LEADERS
	goto NamedTalk_EventScript_GoTheirWay

NamedTalk_EventScript_MagmaAdmins::
	lockall
	msgbox NamedTalk_Text_MagmaAdmins, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_TALK_MAGMA_TABITHA
	removeobject LOCALID_TALK_MAGMA_COURTNEY
	setvar VAR_0x8004, NAMED_TALK_MAGMA_ADMINS
	goto NamedTalk_EventScript_GoTheirWay

NamedTalk_EventScript_AquaAdmins::
	lockall
	msgbox NamedTalk_Text_AquaAdmins, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_TALK_AQUA_MATT
	removeobject LOCALID_TALK_AQUA_SHELLY
	setvar VAR_0x8004, NAMED_TALK_AQUA_ADMINS
	goto NamedTalk_EventScript_GoTheirWay

NamedTalk_EventScript_StevenWallace::
	lockall
	msgbox NamedTalk_Text_StevenWallace, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_TALK_FRIENDS_STEVEN
	removeobject LOCALID_TALK_FRIENDS_WALLACE
	setvar VAR_0x8004, NAMED_TALK_STEVEN_WALLACE
	goto NamedTalk_EventScript_GoTheirWay

NamedTalk_EventScript_JuanWallace::
	lockall
	msgbox NamedTalk_Text_JuanWallace, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_TALK_MENTOR_JUAN
	removeobject LOCALID_TALK_MENTOR_WALLACE
	setvar VAR_0x8004, NAMED_TALK_JUAN_WALLACE
	goto NamedTalk_EventScript_GoTheirWay

NamedTalk_EventScript_NormanWally::
	lockall
	msgbox NamedTalk_Text_NormanWally, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_TALK_NORMAN
	removeobject LOCALID_TALK_WALLY
	setvar VAR_0x8004, NAMED_TALK_NORMAN_WALLY
	goto NamedTalk_EventScript_GoTheirWay

NamedTalk_EventScript_Winstrates::
	lockall
	msgbox NamedTalk_Text_Winstrates, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_TALK_WINSTRATE_VICTOR
	removeobject LOCALID_TALK_WINSTRATE_VICTORIA
	removeobject LOCALID_TALK_WINSTRATE_VIVI
	removeobject LOCALID_TALK_WINSTRATE_VICKY
	removeobject LOCALID_TALK_WINSTRATE_VITO
	setvar VAR_0x8004, NAMED_TALK_WINSTRATES
	goto NamedTalk_EventScript_GoTheirWay

NamedTalk_EventScript_EliteFour::
	lockall
	msgbox NamedTalk_Text_EliteFour, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_TALK_E4_SIDNEY
	removeobject LOCALID_TALK_E4_PHOEBE
	removeobject LOCALID_TALK_E4_GLACIA
	removeobject LOCALID_TALK_E4_DRAKE
	setvar VAR_0x8004, NAMED_TALK_ELITE_FOUR
	goto NamedTalk_EventScript_GoTheirWay

NamedTalk_EventScript_BrandonSteven::
	lockall
	msgbox NamedTalk_Text_BrandonSteven, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_TALK_EXPLORERS_BRANDON
	removeobject LOCALID_TALK_EXPLORERS_STEVEN
	setvar VAR_0x8004, NAMED_TALK_BRANDON_STEVEN
	goto NamedTalk_EventScript_GoTheirWay

NamedTalk_EventScript_AnabelTucker::
	lockall
	msgbox NamedTalk_Text_AnabelTucker, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_TALK_STARS_ANABEL
	removeobject LOCALID_TALK_STARS_TUCKER
	setvar VAR_0x8004, NAMED_TALK_ANABEL_TUCKER
	goto NamedTalk_EventScript_GoTheirWay

NamedTalk_EventScript_SpenserGreta::
	lockall
	msgbox NamedTalk_Text_SpenserGreta, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_TALK_ELDERS_SPENSER
	removeobject LOCALID_TALK_ELDERS_GRETA
	setvar VAR_0x8004, NAMED_TALK_SPENSER_GRETA
	goto NamedTalk_EventScript_GoTheirWay

NamedTalk_EventScript_LucyNoland::
	lockall
	msgbox NamedTalk_Text_LucyNoland, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_TALK_QUIET_LUCY
	removeobject LOCALID_TALK_QUIET_NOLAND
	setvar VAR_0x8004, NAMED_TALK_LUCY_NOLAND
	goto NamedTalk_EventScript_GoTheirWay

@ The screen is dark and the group gone: the talk is not shown again.
NamedTalk_EventScript_GoTheirWay:
	special FrontierStory_FinishNamedTalk
	fadescreen FADE_FROM_BLACK
	releaseall
	end

NamedTalk_Text_MagmaAquaLeaders:
	.string "{JPN}アオギリ『ガハハ！ マツブサ！\n"
	.string "生きてたか！』\p"
	.string "マツブサ『……騒がしい 男だ。\n"
	.string "旅館の 静けさが 恋しくなる。』\p"
	.string "アオギリ『つながりの 中じゃ\n"
	.string "海も 大地も 同じに 見えてたよな。\p"
	.string "ケンカの 理由も なかった。』\p"
	.string "マツブサ『……ああ。 君の 考えが\n"
	.string "私の 考えに なっていた。』\p"
	.string "アオギリ『で 今は どうだ？』\p"
	.string "マツブサ『……やはり 大地だ。\n"
	.string "君とは 相容れない。』\p"
	.string "アオギリ『ガハハ！ そうこなくっちゃな！\n"
	.string "張り合う 相手が いねえと\l"
	.string "つまらねえ！』\p"
	.string "マツブサ『……フッ。\n"
	.string "その点だけは 同意しよう。』$"

NamedTalk_Text_MagmaAdmins:
	.string "{JPN}ホムラ『おう カガリ！\n"
	.string "元気そうだな！』\p"
	.string "カガリ『……ホムラ。 ……うるさい。』\p"
	.string "ホムラ『つながりの 中じゃ\n"
	.string "おまえの 考えが 全部 わかったのに\l"
	.string "今は さっぱりだぜ。』\p"
	.string "カガリ『……アハ。 ……それで いい。』\p"
	.string "ホムラ『そういう もんか？』\p"
	.string "カガリ『……わからないから……\n"
	.string "調べがいが ある。\p"
	.string "……アンタも。』\p"
	.string "ホムラ『お オレを 調べるのかよ！』$"

NamedTalk_Text_AquaAdmins:
	.string "{JPN}ウシオ『ウハハ！ イズミ！\n"
	.string "元気そうだなーっ！』\p"
	.string "イズミ『……あんたは 相変わらず\n"
	.string "うるさいわね。』\p"
	.string "ウシオ『つながりの 中じゃ\n"
	.string "アオギリさまの 夢も おれの 夢も\l"
	.string "同じだった！\p"
	.string "……でも 今は ちょっと\n"
	.string "違う 気が するんだ。』\p"
	.string "イズミ『ふうん。 あんたにも\n"
	.string "自分の 夢が あったのね。』\p"
	.string "ウシオ『ウハハ！ まだ わからん！\n"
	.string "でも 探すのは 楽しいぞーっ！』\p"
	.string "イズミ『……そ。 見つかったら\n"
	.string "聞いて あげるわ。』$"

NamedTalk_Text_StevenWallace:
	.string "{JPN}ミクリ『ダイゴ。 その 石……\n"
	.string "また ずいぶん 地味だね。』\p"
	.string "ダイゴ『わかってないなあ。\n"
	.string "この 縞模様が いいんだよ。』\p"
	.string "ミクリ『つながりの 中では\n"
	.string "私も その 石が 美しく 見えた。\p"
	.string "……今は さっぱり わからない。』\p"
	.string "ダイゴ『ははっ。 ぼくも きみの 衣装の\n"
	.string "良さが 急に わからなく なったよ。』\p"
	.string "ミクリ『それは 困るな。\n"
	.string "……でも 不思議と 悪くない 気分だ。』\p"
	.string "ダイゴ『わからないから\n"
	.string "話したく なるんだろうね。\p"
	.string "この 石の 話 もう少し いいかい？』\p"
	.string "ミクリ『……短めに 頼むよ。』$"

NamedTalk_Text_JuanWallace:
	.string "{JPN}アダン『ミクリ。 ……久しぶりですね。』\p"
	.string "ミクリ『師匠。 つながりの 中では\n"
	.string "私の 演技は 師匠の 演技と\l"
	.string "同じでした。』\p"
	.string "アダン『ええ。 どこまでが 私で\n"
	.string "どこからが あなたか 分からなかった。\p"
	.string "心地よい ものでしたね。』\p"
	.string "ミクリ『……それでも 私は\n"
	.string "教わった その先で\l"
	.string "私だけの 美しさを 見つけたい。』\p"
	.string "アダン『ふふ。 ならば 見せてください。\n"
	.string "私の 知らない あなたの 演技を。』$"

NamedTalk_Text_NormanWally:
	.string "{JPN}ミツル『センリさん！ ぼく 旅館で\n"
	.string "ずっと 考えて いたんです。』\p"
	.string "センリ『ミツルくんか。 何を だい？』\p"
	.string "ミツル『つながりの 中では\n"
	.string "みんなの 強さが ぼくの ものみたいで\l"
	.string "安心でした。\p"
	.string "目が 覚めたら ぼくは\n"
	.string "ぼく ひとりで…… 少し 怖いんです。』\p"
	.string "センリ『……初めて ポケモンを\n"
	.string "捕まえた 日の きみも 震えて いたね。\p"
	.string "それでも きみは 自分の 足で\n"
	.string "草むらに 入った。\p"
	.string "あの 勇気は 誰かの ものじゃない。\n"
	.string "きみの ものだ。』\p"
	.string "ミツル『……はい！ ぼく もう一度\n"
	.string "自分の 力で 挑戦します！』$"

NamedTalk_Text_Winstrates:
	.string "{JPN}ハルヒコ『カチヌキファミリー\n"
	.string "全員 集合だ！』\p"
	.string "ヤスエ『つながりの 中では\n"
	.string "家族の 気持ちが 全部 わかって\l"
	.string "楽だったわね。』\p"
	.string "アキ『でも 今は パパの 考えてる こと\n"
	.string "ぜーんぜん わかんない！』\p"
	.string "ミツヨ『そりゃ そうさ。\n"
	.string "家族だって 別々の 人間だからねえ。』\p"
	.string "リョウヘイ『わからないから\n"
	.string "作戦会議を するんだろ。 昔みたいに。』\p"
	.string "ハルヒコ『そうだ！ よし 家族会議だ！\n"
	.string "議題は 次の 勝負の 先鋒！』\p"
	.string "アキ『アキが いい！』\p"
	.string "リョウヘイ『いや おれだ！』\p"
	.string "ヤスエ『……ふふ。\n"
	.string "けんかが できるって いいわね。』$"

NamedTalk_Text_EliteFour:
	.string "{JPN}カゲツ『へっ そろったな 四天王。』\p"
	.string "プリム『つながりの 中では\n"
	.string "わたくしたち 四人とも\l"
	.string "同じ ことを 考えて いましたわね。』\p"
	.string "フヨウ『あたい それ ちょっと\n"
	.string "つまんなかった！\l"
	.string "カゲツの 悪口も 言えないし！』\p"
	.string "カゲツ『言うなよ。』\p"
	.string "ゲンジ『四人 そろって 同じ 戦い方では\n"
	.string "四天王の 意味が ないのう。』\p"
	.string "プリム『ええ。 わたくしは 氷。\n"
	.string "皆さまは それぞれの 道を。』\p"
	.string "フヨウ『また バラバラに\n"
	.string "強く なろうね！\p"
	.string "チャンピオンを 驚かせて やろう！』$"

NamedTalk_Text_BrandonSteven:
	.string "{JPN}ジンダイ『ガハハ！ ダイゴくん！\n"
	.string "ピラミッドの 奥で\l"
	.string "珍しい 石を 見つけたぞ！』\p"
	.string "ダイゴ『本当ですか！\n"
	.string "ぜひ 見せて ください！』\p"
	.string "ジンダイ『つながりの 中では\n"
	.string "見つけた 喜びも みんなの ものだった。\p"
	.string "わしが 見つけても 誰が 見つけても\n"
	.string "同じ じゃった。』\p"
	.string "ダイゴ『今は 違いますか？』\p"
	.string "ジンダイ『うむ！ わしが 見つけた\n"
	.string "わしの 宝じゃ！\p"
	.string "……だから こうして\n"
	.string "誰かに 見せたく なる！』\p"
	.string "ダイゴ『ははっ。 その 気持ち\n"
	.string "よく わかります。』$"

NamedTalk_Text_AnabelTucker:
	.string "{JPN}ヒース『アッハッハッ!! リラくん！\n"
	.string "ボクの ステージに 来たまえ!!』\p"
	.string "リラ『……ヒース。 声が 大きい。』\p"
	.string "ヒース『つながりの 中では みんなが\n"
	.string "同じ タイミングで 拍手を くれた！\p"
	.string "最高だった!! ……はず なんだけど。』\p"
	.string "リラ『……今は？』\p"
	.string "ヒース『バラバラの 拍手の 方が\n"
	.string "なぜか ゾクゾク するんだ!!\l"
	.string "アッハッハッ!!』\p"
	.string "リラ『……ふふ。 私も\n"
	.string "自分の 声が 聞こえる 方が 好き。』$"

NamedTalk_Text_SpenserGreta:
	.string "{JPN}コゴミ『ウコンじいちゃん！\n"
	.string "久しぶり！』\p"
	.string "ウコン『……コゴミか。\n"
	.string "相変わらず 騒がしいのう。』\p"
	.string "コゴミ『つながりの 中では\n"
	.string "じいちゃんの 考え方も\l"
	.string "わかった 気が してたのに！\p"
	.string "今は やっぱり わかんない！』\p"
	.string "ウコン『わしも おぬしの 気持ちは\n"
	.string "わからん。\p"
	.string "ポケモンの 気持ちと 同じじゃ。\n"
	.string "わからんから 聞く。\p"
	.string "聞けば 少し わかる。\n"
	.string "それで よい。』\p"
	.string "コゴミ『じゃあ 聞く！ じいちゃん\n"
	.string "今度 アリーナで 勝負して！』\p"
	.string "ウコン『……やれやれ。』$"

NamedTalk_Text_LucyNoland:
	.string "{JPN}ダツラ『……お アザミ。\n"
	.string "外に 出てるの 珍しいな。』\p"
	.string "アザミ『……ダツラ。 ……うん。』\p"
	.string "ダツラ『つながりの 中じゃ\n"
	.string "黙ってても 全部 伝わったよな。』\p"
	.string "アザミ『……でも ……今は\n"
	.string "言わないと 伝わらない。』\p"
	.string "ダツラ『そうだな。\n"
	.string "……で 何か 言いたい ことは？』\p"
	.string "アザミ『…… ……また チューブで。』\p"
	.string "ダツラ『はは。 それで\n"
	.string "十分 伝わったよ。』$"
