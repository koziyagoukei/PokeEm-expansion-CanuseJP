FiveIsland_LostCave_Room1_Frlg_MapScripts::
	.byte 0

FiveIsland_LostCave_Room1_EventScript_Lawson::
	trainerbattle_single TRAINER_RUIN_MANIAC_LAWSON, FiveIsland_LostCave_Room1_Text_LawsonIntro, FiveIsland_LostCave_Room1_Text_LawsonDefeat
	msgbox FiveIsland_LostCave_Room1_Text_LawsonPostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_LostCave_Room1_Text_LawsonIntro::
	.string "{JPN}ひいいい!\p"
	.string "いせき かと おもって はいったら\n"
	.string "ゴーストだらけ じゃないか!$"

FiveIsland_LostCave_Room1_Text_LawsonDefeat::
	.string "{JPN}ひいいい\n"
	.string "おたすけを!$"

FiveIsland_LostCave_Room1_Text_LawsonPostBattle::
	.string "{JPN}ここから にげだそうにも\n"
	.string "おなじ ところに でてしまう\p"
	.string "いわの かずに なにか\n"
	.string "ヒントが あるのかな…?$"

