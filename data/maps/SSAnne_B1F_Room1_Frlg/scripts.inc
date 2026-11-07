SSAnne_B1F_Room1_Frlg_MapScripts::
	.byte 0

SSAnne_B1F_Room1_EventScript_Barny::
	trainerbattle_single TRAINER_FISHERMAN_BARNY, SSAnne_B1F_Room1_Text_BarnyIntro, SSAnne_B1F_Room1_Text_BarnyDefeat
	msgbox SSAnne_B1F_Room1_Text_BarnyPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_B1F_Room1_EventScript_Phillip::
	trainerbattle_single TRAINER_SAILOR_PHILLIP, SSAnne_B1F_Room1_Text_PhillipIntro, SSAnne_B1F_Room1_Text_PhillipDefeat
	msgbox SSAnne_B1F_Room1_Text_PhillipPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_B1F_Room1_Text_PhillipIntro::
	.string "{JPN}うっしゃー!\n"
	.string "まけたら うみ に おとすぞー!$"

SSAnne_B1F_Room1_Text_PhillipDefeat::
	.string "{JPN}くー やられた!$"

SSAnne_B1F_Room1_Text_PhillipPostBattle::
	.string "{JPN}うみに でてると\n"
	.string "たまーに クラゲ ポケモンが\l"
	.string "ながれて くるぜ$"

SSAnne_B1F_Room1_Text_BarnyIntro::
	.string "{JPN}うみのもの とも やまのもの とも\n"
	.string "わからない しんいり さん!\p"
	.string "わたしが もってるのは\n"
	.string "うみのもの ばかり です!$"

SSAnne_B1F_Room1_Text_BarnyDefeat::
	.string "{JPN}ちっ!\n"
	.string "つり のがしたよ$"

SSAnne_B1F_Room1_Text_BarnyPostBattle::
	.string "{JPN}あなた まかしたら つりびとの\n"
	.string "つきびとに しようと おもってた$"

