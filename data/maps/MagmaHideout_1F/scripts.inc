MagmaHideout_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MagmaHideout_1F_OnTransition
	.byte 0

MagmaHideout_1F_OnTransition:
	setvar VAR_JAGGED_PASS_ASH_WEATHER, 0
	end

MagmaHideout_1F_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_1, MagmaHideout_1F_Text_Grunt1Intro, MagmaHideout_1F_Text_Grunt1Defeat
	msgbox MagmaHideout_1F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_1F_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_2, MagmaHideout_1F_Text_Grunt2Intro, MagmaHideout_1F_Text_Grunt2Defeat
	msgbox MagmaHideout_1F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_1F_Text_Grunt1Intro:
	.string "マグマだんは ごうれいが かかったとき\nはやく せいれつした じゅんばんで\lじゅうような ばしょの\lけいびを まかせて もらえるんだぜ!\pオレが こんな すみっこに いるのは\nちこく ばっかり してるからさ!"
	.string ""
	.string ""
	.string ""
	.string "$"

MagmaHideout_1F_Text_Grunt1Defeat:
	.string "トレーニングにも ちこくする オレだ!\nわれながら よわいぜ……"
	.string ""
	.string "$"

MagmaHideout_1F_Text_Grunt1PostBattle:
	.string "わかったよ こんどから もうすこし\nがんばって みるよ……"
	.string "$"

MagmaHideout_1F_Text_Grunt2Intro:
	.string "リーダーに いわれた とおり\nえんとつやまを どんどん ほっていたら\lとんでもねえモンが でちまった!\pなにが でたかって?\pふははは!\nオレを たおせたら おしえて やるよ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MagmaHideout_1F_Text_Grunt2Defeat:
	.string "むぐぅ…… たおされちまった!"
	.string "$"

MagmaHideout_1F_Text_Grunt2PostBattle:
	.string "やっぱり おしえるの やーめた!\nどうせ おくまで いけば わかることさ!\pおまえも あとに おたのしみを\nのこして おいたほうが いいだろ?"
	.string ""
	.string ""
	.string "$"
