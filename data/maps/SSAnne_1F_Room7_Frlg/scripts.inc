SSAnne_1F_Room7_Frlg_MapScripts::
	.byte 0

SSAnne_1F_Room7_EventScript_Thomas::
	trainerbattle_single TRAINER_GENTLEMAN_THOMAS, SSAnne_1F_Room7_Text_ThomasIntro, SSAnne_1F_Room7_Text_ThomasDefeat
	msgbox SSAnne_1F_Room7_Text_ThomasPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_1F_Room7_Text_ThomasIntro::
	.string "{JPN}わたしは たびびと…!\p"
	.string "たびさきで とった ポケモン だけが\n"
	.string "わたしの ともだち$"

SSAnne_1F_Room7_Text_ThomasDefeat::
	.string "{JPN}と……ともだちが……$"

SSAnne_1F_Room7_Text_ThomasPostBattle::
	.string "{JPN}あのね!\n"
	.string "ともだちは だいじに しなさい!$"

