RocketHideout_B1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, RocketHideout_B1F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, RocketHideout_B1F_OnTransition
	.byte 0

RocketHideout_B1F_OnLoad::
	call_if_not_defeated TRAINER_TEAM_ROCKET_GRUNT_12 RocketHideout_B1F_EventScript_SetBarrier
	end

RocketHideout_B1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_ROCKET_HIDEOUT_B1F
	end

RocketHideout_B1F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_8, RocketHideout_B1F_Text_Grunt1Intro, RocketHideout_B1F_Text_Grunt1Defeat
	msgbox RocketHideout_B1F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

RocketHideout_B1F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_9, RocketHideout_B1F_Text_Grunt2Intro, RocketHideout_B1F_Text_Grunt2Defeat
	msgbox RocketHideout_B1F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

RocketHideout_B1F_EventScript_Grunt3::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_10, RocketHideout_B1F_Text_Grunt3Intro, RocketHideout_B1F_Text_Grunt3Defeat
	msgbox RocketHideout_B1F_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

RocketHideout_B1F_EventScript_Grunt4::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_11, RocketHideout_B1F_Text_Grunt4Intro, RocketHideout_B1F_Text_Grunt4Defeat
	msgbox RocketHideout_B1F_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

RocketHideout_B1F_EventScript_Grunt5::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_12, RocketHideout_B1F_Text_Grunt5Intro, RocketHideout_B1F_Text_Grunt5Defeat, RocketHideout_B1F_EventScript_DefeatedGrunt5
	msgbox RocketHideout_B1F_Text_Grunt5PostBattle, MSGBOX_AUTOCLOSE
	end

RocketHideout_B1F_EventScript_DefeatedGrunt5::
	call RocketHideout_B1F_EventScript_RemoveBarrier
	playse SE_UNLOCK
	special DrawWholeMapView
	waitse
	release
	end

RocketHideout_B1F_EventScript_SetBarrier::
	setmetatile 20, 19, METATILE_SilphCo_HideoutBarrier_TopLeft_Floor, 1
	setmetatile 21, 19, METATILE_SilphCo_HideoutBarrier_TopRight_Floor, 1
	setmetatile 20, 20, METATILE_SilphCo_HideoutBarrier_BottomLeft, 1
	setmetatile 21, 20, METATILE_SilphCo_HideoutBarrier_BottomRight, 1
	setmetatile 20, 21, METATILE_SilphCo_HideoutFloor_ShadeFull, 0
	setmetatile 21, 21, METATILE_SilphCo_HideoutFloor_ShadeFull, 0
	return

RocketHideout_B1F_EventScript_RemoveBarrier::
	setmetatile 20, 19, METATILE_SilphCo_HideoutFloor_ShadeFull, 0
	setmetatile 21, 19, METATILE_SilphCo_HideoutFloor, 0
	setmetatile 20, 20, METATILE_SilphCo_HideoutFloor_ShadeFull, 0
	setmetatile 21, 20, METATILE_SilphCo_HideoutFloor, 0
	setmetatile 20, 21, METATILE_SilphCo_HideoutFloor_ShadeFull, 0
	setmetatile 21, 21, METATILE_SilphCo_HideoutFloor, 0
	return

RocketHideout_B1F_Text_Grunt1Intro::
	.string "{JPN}… おまえは だれだ?\n"
	.string "どうやって ここに はいった$"

RocketHideout_B1F_Text_Grunt1Defeat::
	.string "{JPN}あう! やられた$"

RocketHideout_B1F_Text_Grunt1PostBattle::
	.string "{JPN}くそう… おまえ\n"
	.string "ロケットだんを なめてるな!$"

RocketHideout_B1F_Text_Grunt2Intro::
	.string "{JPN}ロケットだんに\n"
	.string "しのび こむ とは なんて やつ$"

RocketHideout_B1F_Text_Grunt2Defeat::
	.string "{JPN}ばったり…!$"

RocketHideout_B1F_Text_Grunt2PostBattle::
	.string "{JPN}くそう! このまま らくに\n"
	.string "すすめると おもうな!$"

RocketHideout_B1F_Text_Grunt3Intro::
	.string "{JPN}しんにゅうしゃを はっけん!$"

RocketHideout_B1F_Text_Grunt3Defeat::
	.string "{JPN}ああ! だめか$"

RocketHideout_B1F_Text_Grunt3PostBattle::
	.string "{JPN}ふん…!\n"
	.string "シルフスコープを さがしてる?\l"
	.string "… おれは しらねえよ$"

RocketHideout_B1F_Text_Grunt4Intro::
	.string "{JPN}ここに きた ねらいは なんだ?$"

RocketHideout_B1F_Text_Grunt4Defeat::
	.string "{JPN}こりゃ イカン$"

RocketHideout_B1F_Text_Grunt4PostBattle::
	.string "{JPN}わかった いう…\n"
	.string "ボスに あいたい なら\l"
	.string "エレベータに のるんだな$"

RocketHideout_B1F_Text_Grunt5Intro::
	.string "{JPN}まいごの まいごの…\n"
	.string "おこさま かな?$"

RocketHideout_B1F_Text_Grunt5Defeat::
	.string "{JPN}なんで じゃ$"

RocketHideout_B1F_Text_Grunt5PostBattle::
	.string "{JPN}あれ? いまの さわぎで\n"
	.string "とびらが あいた しまった$"

