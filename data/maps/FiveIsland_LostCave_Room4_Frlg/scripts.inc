FiveIsland_LostCave_Room4_Frlg_MapScripts::
	.byte 0

FiveIsland_LostCave_Room4_EventScript_Laura::
	trainerbattle_single TRAINER_PSYCHIC_LAURA, FiveIsland_LostCave_Room4_Text_LauraIntro, FiveIsland_LostCave_Room4_Text_LauraDefeat
	msgbox FiveIsland_LostCave_Room4_Text_LauraPostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_LostCave_Room4_Text_LauraIntro::
	.string "{JPN}この あな には\n"
	.string "ふしぎな ちからが\l"
	.string "はたらいて いるの…!$"

FiveIsland_LostCave_Room4_Text_LauraDefeat::
	.string "{JPN}ただしい あなに はいらないと\n"
	.string "さきへ すすめない わよ$"

FiveIsland_LostCave_Room4_Text_LauraPostBattle::
	.string "{JPN}そういえば さっき\n"
	.string "おんなの ひとが\l"
	.string "このさきへ はいって いったわ$"

