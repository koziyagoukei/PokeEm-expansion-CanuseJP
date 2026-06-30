MagmaHideout_3F_1R_MapScripts::
	.byte 0

MagmaHideout_3F_1R_EventScript_Grunt9::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_9, MagmaHideout_3F_1R_Text_Grunt9Intro, MagmaHideout_3F_1R_Text_Grunt9Defeat
	msgbox MagmaHideout_3F_1R_Text_Grunt9PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_3F_1R_EventScript_Grunt16::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_16, MagmaHideout_3F_1R_Text_Grunt16Intro, MagmaHideout_3F_1R_Text_Grunt16Defeat
	msgbox MagmaHideout_3F_1R_Text_Grunt16PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_3F_1R_Text_Grunt9Intro:
	.string "{JPN}なんで オレは こんなところの\nたんとう なんだよ!\pひだりのみみ が あつい じゃねーか!"
	.string ""
	.string "$"

MagmaHideout_3F_1R_Text_Grunt9Defeat:
	.string "{JPN}あつくて ヘロヘロ……$"

MagmaHideout_3F_1R_Text_Grunt9PostBattle:
	.string "{JPN}こんな マグマだらけの かざんの なかで\nフードを かぶってる オレたち へん?"
	.string "$"

MagmaHideout_3F_1R_Text_Grunt16Intro:
	.string "{JPN}アタシたちは リーダーが めざしている\nすばらしい もくてきを\lじつげん するために あつまったの!\pアクアだん だろうが\nとおりすがりの おこさま だろうが\lじゃまを するなら ゆるさないわ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MagmaHideout_3F_1R_Text_Grunt16Defeat:
	.string "{JPN}ひえー あなどれない ひと!"
	.string "$"

MagmaHideout_3F_1R_Text_Grunt16PostBattle:
	.string "{JPN}いい? ただしいのは マグマだんよ!\nアクアだんの いうことなんか\lうそ ばっかりだから しんじちゃ ダメよ!"
	.string ""
	.string ""
	.string "$"
