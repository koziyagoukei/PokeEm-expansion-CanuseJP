SSAnne_B1F_Room3_Frlg_MapScripts::
	.byte 0

SSAnne_B1F_Room3_EventScript_Dylan::
	trainerbattle_single TRAINER_SAILOR_DYLAN, SSAnne_B1F_Room3_Text_DylanIntro, SSAnne_B1F_Room3_Text_DylanDefeat
	msgbox SSAnne_B1F_Room3_Text_DylanPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_B1F_Room3_Text_DylanIntro::
	.string "{JPN}こういう げんきな こども\n"
	.string "おれは すきだぜ!\n"
	.string "$"

SSAnne_B1F_Room3_Text_DylanDefeat::
	.string "{JPN}ええい!\n"
	.string "まけたぜ!$"

SSAnne_B1F_Room3_Text_DylanPostBattle::
	.string "{JPN}うみの ポケモンは\n"
	.string "ふかい ところに いるから\l"
	.string "つりざおで つり あげるんだ!$"

