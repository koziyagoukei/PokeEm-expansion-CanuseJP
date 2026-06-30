MagmaHideout_3F_2R_MapScripts::
	.byte 0

MagmaHideout_3F_2R_EventScript_Grunt10::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_10, MagmaHideout_3F_2R_Text_Grunt10Intro, MagmaHideout_3F_2R_Text_Grunt10Defeat
	msgbox MagmaHideout_3F_2R_Text_Grunt10PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_3F_2R_Text_Grunt10Intro:
	.string "{JPN}あのな リーダーの いうことは\nオレにも よく わかるんだけど な\pちょうこだいポケモンを ほりだしたり\nひとの いんせきを よこどりしたり\lなんか それって ちょっと やりすぎ\lのような きもするんだよ な\pおまえ どう おもう?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MagmaHideout_3F_2R_Text_Grunt10Defeat:
	.string "{JPN}やっぱり なんか まちがってる よなあ?"
	.string "$"

MagmaHideout_3F_2R_Text_Grunt10PostBattle:
	.string "{JPN}しょうぶに まけて スッキリしたよ!\pオレも こんど リーダーに あったら\nじっさい どうなのか きいて みるよ!"
	.string ""
	.string "$"
