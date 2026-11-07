SilphCo_9F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_9F_OnLoad
	.byte 0

SilphCo_9F_OnLoad::
	call_if_unset FLAG_SILPH_9F_DOOR_1, EventScript_Close9FDoor1
	call_if_unset FLAG_SILPH_9F_DOOR_2, EventScript_Close9FDoor2
	call_if_unset FLAG_SILPH_9F_DOOR_3, EventScript_Close9FDoor3
	call_if_unset FLAG_SILPH_9F_DOOR_4, EventScript_Close9FDoor4
	end

SilphCo_9F_EventScript_HealWoman::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_9F_EventScript_HealWomanRocketsGone
	msgbox SilphCo_9F_Text_YouShouldTakeQuickNap
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox SilphCo_9F_Text_DontGiveUp
	release
	end

SilphCo_9F_EventScript_HealWomanRocketsGone::
	msgbox SilphCo_9F_Text_ThankYouSoMuch
	release
	end

SilphCo_9F_EventScript_FloorSign::
	msgbox SilphCo_9F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_9F_EventScript_Ed::
	trainerbattle_single TRAINER_SCIENTIST_ED, SilphCo_9F_Text_EdIntro, SilphCo_9F_Text_EdDefeat
	msgbox SilphCo_9F_Text_EdPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_9F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_37, SilphCo_9F_Text_Grunt1Intro, SilphCo_9F_Text_Grunt1Defeat
	msgbox SilphCo_9F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_9F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_38, SilphCo_9F_Text_Grunt2Intro, SilphCo_9F_Text_Grunt2Defeat
	msgbox SilphCo_9F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_9F_Text_YouShouldTakeQuickNap::
	.string "{JPN}つかれてる みたいよ!\n"
	.string "かみんしつで やすんで いったら?$"

SilphCo_9F_Text_DontGiveUp::
	.string "{JPN}がんばって!$"

SilphCo_9F_Text_ThankYouSoMuch::
	.string "{JPN}たすかったわ ありがと$"

SilphCo_9F_Text_Grunt1Intro::
	.string "{JPN}こどもの くせに おまえの\n"
	.string "ポケモン ずいぶん なついているな$"

SilphCo_9F_Text_Grunt1Defeat::
	.string "{JPN}ぐうーッ!$"

SilphCo_9F_Text_Grunt1PostBattle::
	.string "{JPN}おまえ くらいの としから\n"
	.string "ポケモン やってれば おれも…$"

SilphCo_9F_Text_EdIntro::
	.string "{JPN}きみの ポケモン には\n"
	.string "… じゃくてんが ある!\l"
	.string "そこを せめれば おれ かてるよ!$"

SilphCo_9F_Text_EdDefeat::
	.string "{JPN}じゃくてんを…\n"
	.string "つくまえに やられたあ$"

SilphCo_9F_Text_EdPostBattle::
	.string "{JPN}じゃくてんを\n"
	.string "せめると いいのは じじつだ!\l"
	.string "タイプの あいしょう とか…$"

SilphCo_9F_Text_Grunt2Intro::
	.string "{JPN}おれこそは\n"
	.string "ロケット 4きょうだいの ひとりだ!$"

SilphCo_9F_Text_Grunt2Defeat::
	.string "{JPN}うー!\n"
	.string "まけた おとうとよ!$"

SilphCo_9F_Text_Grunt2PostBattle::
	.string "{JPN}まあ いい\n"
	.string "おとうとが かたきを とってくれる$"

SilphCo_9F_Text_FloorSign::
	.string "{JPN}シルフ カンパニー ほんしゃ\n"
	.string "9かい$"

