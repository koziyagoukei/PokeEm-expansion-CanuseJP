Route4_Frlg_MapScripts::
	.byte 0

Route4_EventScript_Unused::
	end

Route4_EventScript_Woman::
	msgbox Route4_Text_TrippedOverGeodude, MSGBOX_NPC
	end

Route4_EventScript_MtMoonSign::
	msgbox Route4_Text_MtMoonEntrance, MSGBOX_SIGN
	end

Route4_EventScript_RouteSign::
	msgbox Route4_Text_RouteSign, MSGBOX_SIGN
	end

Route4_Text_TrippedOverGeodude::
	.string "{JPN}いたッ!\n"
	.string "つまずいて ころんじゃった\l"
	.string "ポケモンの イシツブテだわ!$"

Route4_Text_CrissyIntro::
	.string "{JPN}わたし オツキミやまに\n"
	.string "キノコの ポケモン とりに きたの!$"

Route4_Text_CrissyDefeat::
	.string "{JPN}せっかく とったのに!$"

Route4_Text_CrissyPostBattle::
	.string "{JPN}このあたりは\n"
	.string "もう キノコ みつかんないかも\p"
	.string "だって あたしが\n"
	.string "ぜんぶ とっちゃったもん$"

Route4_Text_MtMoonEntrance::
	.string "{JPN}ここは オツキミやま\n"
	.string "… どうくつ いりぐち$"

Route4_Text_RouteSign::
	.string "{JPN}ここは 4ばん どうろ\n"
	.string "オツキミやま … ハナダ シティ$"

Text_MegaPunchTeach::
	.string "{JPN}うなる こぶし!\p"
	.string "その はかいりょく!\p"
	.string "やはり メガトンパンチ こそ\n"
	.string "さいきょうの わざだ!\l"
	.string "そう おもうだろう!\p"
	.string "さあ!\n"
	.string "おまえの ポケモンに おしえよう!$"

Text_MegaPunchDeclined::
	.string "{JPN}メガトンパンチの ありがたみが\n"
	.string "わかったら また くるがよい$"

Text_MegaPunchWhichMon::
	.string "{JPN}よし!\n"
	.string "どの ポケモンに するのか?$"

Text_MegaPunchTaught::
	.string "{JPN}これで おまえは パンチなかま!\p"
	.string "あっちで つまらん キックを\n"
	.string "しゅぎょうしてる やつに\l"
	.string "つかまらん うちに いけ!$"

Text_MegaKickTeach::
	.string "{JPN}はげしい けり!\p"
	.string "その はかいりょく!\p"
	.string "やはり メガトンキック こそ\n"
	.string "さいきょうの わざだ!\l"
	.string "そう おもうだろう!\p"
	.string "さあ!\n"
	.string "おまえの ポケモンに おしえよう!$"

Text_MegaKickDeclined::
	.string "{JPN}メガトンキックの ありがたみが\n"
	.string "わかったら また くるがよい$"

Text_MegaKickWhichMon::
	.string "{JPN}よし!\n"
	.string "どの ポケモンに するのか?$"

Text_MegaKickTaught::
	.string "{JPN}これで おまえは キックなかま!\p"
	.string "あっちで つまらん パンチを\n"
	.string "しゅぎょうしてる やつに\l"
	.string "つかまらん うちに いけ!$"

Route4_Text_PeopleLikeAndRespectBrock::
	.string "{JPN}お それは グレーバッジ!\n"
	.string "タケシに もらったんだね\p"
	.string "おれも タケシを みならって\n"
	.string "つよいだけ じゃなくて\l"
	.string "みんなに したわれる\l"
	.string "ジムリーダーに なりたいなあ$"
