SSAnne_B1F_Room2_Frlg_MapScripts::
	.byte 0

SSAnne_B1F_Room2_EventScript_Huey::
	trainerbattle_single TRAINER_SAILOR_HUEY, SSAnne_B1F_Room2_Text_HueyIntro, SSAnne_B1F_Room2_Text_HueyDefeat
	msgbox SSAnne_B1F_Room2_Text_HueyPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_B1F_Room2_Text_HueyIntro::
	.string "{JPN}ふなのり だって\n"
	.string "ポケモン ぐらい もってるぜ!$"

SSAnne_B1F_Room2_Text_HueyDefeat::
	.string "{JPN}なかなか\n"
	.string "やるじゃねーか$"

SSAnne_B1F_Room2_Text_HueyPostBattle::
	.string "{JPN}おれたちの ポケモンは\n"
	.string "うみに でた ときに\l"
	.string "じぶんで とった もの ばかり だぜ$"

