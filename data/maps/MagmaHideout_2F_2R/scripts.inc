MagmaHideout_2F_2R_MapScripts::
	.byte 0

MagmaHideout_2F_2R_EventScript_Grunt15::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_15, MagmaHideout_2F_2R_Text_Grunt15Intro, MagmaHideout_2F_2R_Text_Grunt15Defeat
	msgbox MagmaHideout_2F_2R_Text_Grunt15PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_2R_EventScript_Grunt6::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_6, MagmaHideout_2F_2R_Text_Grunt6Intro, MagmaHideout_2F_2R_Text_Grunt6Defeat
	msgbox MagmaHideout_2F_2R_Text_Grunt6PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_2R_EventScript_Grunt7::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_7, MagmaHideout_2F_2R_Text_Grunt7Intro, MagmaHideout_2F_2R_Text_Grunt7Defeat
	msgbox MagmaHideout_2F_2R_Text_Grunt7PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_2R_EventScript_Grunt8::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_8, MagmaHideout_2F_2R_Text_Grunt8Intro, MagmaHideout_2F_2R_Text_Grunt8Defeat
	msgbox MagmaHideout_2F_2R_Text_Grunt8PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_2R_Text_Grunt15Intro:
	.string "あなたに うらみは ないけど\nこれも めいれい だからね!"
	.string "$"

MagmaHideout_2F_2R_Text_Grunt15Defeat:
	.string "わたしは まけたけど……$"

MagmaHideout_2F_2R_Text_Grunt15PostBattle:
	.string "とんでもないものを ほりだした!\nあいいろのたまも てにいれた!\pあとは リーダーが……\nふふふ…… ふはははは!"
	.string ""
	.string ""
	.string "$"

MagmaHideout_2F_2R_Text_Grunt6Intro:
	.string "オレって あつがり なんだよ\nもしかして アクアだんに はいったほうが\lよかったのかな?"
	.string "$"

MagmaHideout_2F_2R_Text_Grunt6Defeat:
	.string "やっぱり オレ\nマグマだんには むいてないかも……"
	.string "$"

MagmaHideout_2F_2R_Text_Grunt6PostBattle:
	.string "こんなところに いると\nうみが こいしく なったり しないか?"
	.string "$"

MagmaHideout_2F_2R_Text_Grunt7Intro:
	.string "ときどき じひびきが きこえるんだよ\pそれって かざんの しんどう かな?\nそれとも グラ……\pおっとっと なんでもないよ!"
	.string ""
	.string ""
	.string ""
	.string "$"

MagmaHideout_2F_2R_Text_Grunt7Defeat:
	.string "うーむ\nかざんのように あついな きみは!"
	.string "$"

MagmaHideout_2F_2R_Text_Grunt7PostBattle:
	.string "オレは たまたま おまえに まけたが\nマグマだんの もくてきは\lまもなく たっせい されるのさ!"
	.string ""
	.string ""
	.string "$"

MagmaHideout_2F_2R_Text_Grunt8Intro:
	.string "マグマのしるしを どこかに おとした\nって おおさわぎ してるやつが いたけど\p……あ! もしかして\nそれ ひろったの おまえ?"
	.string ""
	.string ""
	.string "$"

MagmaHideout_2F_2R_Text_Grunt8Defeat:
	.string "うーん なんて やつだ……$"

MagmaHideout_2F_2R_Text_Grunt8PostBattle:
	.string "なんか…… オレたちの けいかく\nしっぱい するような きが してきたよ"
	.string "$"
