RocketHideout_B2F_Frlg_MapScripts::
	.byte 0

RocketHideout_B2F_EventScript_Grunt::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_13, RocketHideout_B2F_Text_GruntIntro, RocketHideout_B2F_Text_GruntDefeat
	msgbox RocketHideout_B2F_Text_GruntPostBattle, MSGBOX_AUTOCLOSE
	end

RocketHideout_B2F_Text_GruntIntro::
	.string "{JPN}シルフスコープを つかうと\n"
	.string "みえない ゆうれいも みえるって\l"
	.string "ボスが いってたぞ$"

RocketHideout_B2F_Text_GruntDefeat::
	.string "{JPN}まいった!$"

RocketHideout_B2F_Text_GruntPostBattle::
	.string "{JPN}ロケットだんの ほんぶは\n"
	.string "ちか 4かい まで ある\l"
	.string "ボスまで たどり つけるかな?$"

