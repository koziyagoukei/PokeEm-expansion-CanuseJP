SeafloorCavern_Room4_MapScripts::
	.byte 0

SeafloorCavern_Room4_EventScript_Grunt3::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_3, SeafloorCavern_Room4_Text_Grunt3Intro, SeafloorCavern_Room4_Text_Grunt3Defeat
	msgbox SeafloorCavern_Room4_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room4_EventScript_Grunt4::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_4, SeafloorCavern_Room4_Text_Grunt4Intro, SeafloorCavern_Room4_Text_Grunt4Defeat
	msgbox SeafloorCavern_Room4_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room4_Text_Grunt3Intro:
	.string "{JPN}だれだ おまえ?\nどこから はいってきた?$"

SeafloorCavern_Room4_Text_Grunt3Defeat:
	.string "{JPN}まけた……!$"

SeafloorCavern_Room4_Text_Grunt3PostBattle:
	.string "{JPN}でぐちが わかんねーんだよ\pこわがってる わけじゃ ねーぞ\nかんちがい すんなよ!$"

SeafloorCavern_Room4_Text_Grunt4Intro:
	.string "{JPN}あなた だれ?\nどこへ いこうってのよ!?$"

SeafloorCavern_Room4_Text_Grunt4Defeat:
	.string "{JPN}かてなかった……!$"

SeafloorCavern_Room4_Text_Grunt4PostBattle:
	.string "{JPN}このひとったら せんすいていに\nちずを わすれて きちゃったの!\pまったく たよりに なんないんだから!$"
