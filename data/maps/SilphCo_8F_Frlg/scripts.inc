SilphCo_8F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_8F_OnLoad
	.byte 0

SilphCo_8F_OnLoad::
	call_if_unset FLAG_SILPH_8F_DOOR, EventScript_Close8FDoor
	end

SilphCo_8F_EventScript_WorkerM::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_8F_EventScript_WorkerMRocketsGone
	msgbox SilphCo_8F_Text_WonderIfSilphIsFinished
	release
	end

SilphCo_8F_EventScript_WorkerMRocketsGone::
	msgbox SilphCo_8F_Text_ThanksForSavingUs
	release
	end

SilphCo_8F_EventScript_FloorSign::
	msgbox SilphCo_8F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_8F_EventScript_Parker::
	trainerbattle_single TRAINER_SCIENTIST_PARKER, SilphCo_8F_Text_ParkerIntro, SilphCo_8F_Text_ParkerDefeat
	msgbox SilphCo_8F_Text_ParkerPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_8F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_32, SilphCo_8F_Text_Grunt1Intro, SilphCo_8F_Text_Grunt1Defeat
	msgbox SilphCo_8F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_8F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_36, SilphCo_8F_Text_Grunt2Intro, SilphCo_8F_Text_Grunt2Defeat
	msgbox SilphCo_8F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_8F_Text_WonderIfSilphIsFinished::
	.string "{JPN}シルフは のっとられて\n"
	.string "… おわりかな$"

SilphCo_8F_Text_ThanksForSavingUs::
	.string "{JPN}ありがとう たすかった!$"

SilphCo_8F_Text_Grunt1Intro::
	.string "{JPN}ここから さきは いかせ ないぜ!$"

SilphCo_8F_Text_Grunt1Defeat::
	.string "{JPN}きあいが たりんか$"

SilphCo_8F_Text_Grunt1PostBattle::
	.string "{JPN}はやく ひきかえさないと\n"
	.string "… なかまを よぶぜ!$"

SilphCo_8F_Text_ParkerIntro::
	.string "{JPN}すき かって やられちゃ\n"
	.string "こまるんだぜ!$"

SilphCo_8F_Text_ParkerDefeat::
	.string "{JPN}…? まけたのか$"

SilphCo_8F_Text_ParkerPostBattle::
	.string "{JPN}どうだね?\n"
	.string "めいろの ような\l"
	.string "シルフ ビルの かんそうは?$"

SilphCo_8F_Text_Grunt2Intro::
	.string "{JPN}おれこそは\n"
	.string "ロケット 4きょうだいの ひとり!$"

SilphCo_8F_Text_Grunt2Defeat::
	.string "{JPN}にいさん まけたよ$"

SilphCo_8F_Text_Grunt2PostBattle::
	.string "{JPN}まあ いい\n"
	.string "あにきが かたきを とって くれる$"

SilphCo_8F_Text_FloorSign::
	.string "{JPN}シルフ カンパニー ほんしゃ\n"
	.string "8かい$"

SilphCo_8F_Text_ToRocketBossMonsAreTools::
	.string "{JPN}ロケットだんの ボスは\n"
	.string "ひどい やつだぜ…!\p"
	.string "ポケモンを どうぐの ようにしか\n"
	.string "かんがえちゃ いないんだ!\p"
	.string "あんなやつに この かいしゃを\n"
	.string "のっとられたら…$"
