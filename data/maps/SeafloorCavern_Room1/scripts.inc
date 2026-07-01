SeafloorCavern_Room1_MapScripts::
	.byte 0

SeafloorCavern_Room1_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_1, SeafloorCavern_Room1_Text_Grunt1Intro, SeafloorCavern_Room1_Text_Grunt1Defeat
	msgbox SeafloorCavern_Room1_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room1_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_2, SeafloorCavern_Room1_Text_Grunt2Intro, SeafloorCavern_Room1_Text_Grunt2Defeat
	msgbox SeafloorCavern_Room1_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room1_Text_Grunt1Intro:
	.string "{JPN}こどもに ようは ねーんだ\nほら さっさと かえれよ!$"

SeafloorCavern_Room1_Text_Grunt1Defeat:
	.string "{JPN}おれが かえりたいよー!$"

SeafloorCavern_Room1_Text_Grunt1PostBattle:
	.string "{JPN}はやく かんぶに なって\nしたっぱを しごいて やりたい……$"

SeafloorCavern_Room1_Text_Grunt2Intro:
	.string "{JPN}せんすいてい ちっちゃいからよー\nからだの あちこちが いたい じゃねーか!$"

SeafloorCavern_Room1_Text_Grunt2Defeat:
	.string "{JPN}まけたら\nいたい じゃねーか!$"

SeafloorCavern_Room1_Text_Grunt2PostBattle:
	.string "{JPN}うばった せんすいていの のりごこち\nありゃ さいあく だな!\lとにかく せますぎ じゃねーか!$"
