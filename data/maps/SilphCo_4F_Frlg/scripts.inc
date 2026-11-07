SilphCo_4F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_4F_OnLoad
	.byte 0

SilphCo_4F_OnLoad::
	call_if_unset FLAG_SILPH_4F_DOOR_1, EventScript_Close4FDoor1
	call_if_unset FLAG_SILPH_4F_DOOR_2, EventScript_Close4FDoor2
	end

SilphCo_4F_EventScript_WorkerM::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_4F_EventScript_WorkerMRocketsGone
	msgbox SilphCo_4F_Text_CantYouSeeImHiding
	release
	end

SilphCo_4F_EventScript_WorkerMRocketsGone::
	msgbox SilphCo_4F_Text_TeamRocketIsGone
	release
	end

SilphCo_4F_EventScript_FloorSign::
	msgbox SilphCo_4F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_4F_EventScript_Rodney::
	trainerbattle_single TRAINER_SCIENTIST_RODNEY, SilphCo_4F_Text_RodneyIntro, SilphCo_4F_Text_RodneyDefeat
	msgbox SilphCo_4F_Text_RodneyPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_4F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_26, SilphCo_4F_Text_Grunt1Intro, SilphCo_4F_Text_Grunt1Defeat
	msgbox SilphCo_4F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_4F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_27, SilphCo_4F_Text_Grunt2Intro, SilphCo_4F_Text_Grunt2Defeat
	msgbox SilphCo_4F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_4F_Text_CantYouSeeImHiding::
	.string "{JPN}びくびく… こわくて\n"
	.string "ここに かくれてるんだ…$"

SilphCo_4F_Text_TeamRocketIsGone::
	.string "{JPN}え…\n"
	.string "ロケットだん もう いないの?$"

SilphCo_4F_Text_Grunt1Intro::
	.string "{JPN}シルフ カンパニーは\n"
	.string "ロケットだんが せんきょした!$"

SilphCo_4F_Text_Grunt1Defeat::
	.string "{JPN}ぐぐぐ…$"

SilphCo_4F_Text_Grunt1PostBattle::
	.string "{JPN}がはは!\n"
	.string "ボスは まえから\l"
	.string "ここを ねらってたのだ$"

SilphCo_4F_Text_RodneyIntro::
	.string "{JPN}ポケモンは\n"
	.string "おれの ちゅうじつな こぶんだぜ!$"

SilphCo_4F_Text_RodneyDefeat::
	.string "{JPN}くそ!\n"
	.string "なさけない ポケモンめ$"

SilphCo_4F_Text_RodneyPostBattle::
	.string "{JPN}この ビルの ドアは ぜんぶ\n"
	.string "でんし ロック されてるぜ!\l"
	.string "カードキーが ないと あかないぜ!$"

SilphCo_4F_Text_Grunt2Intro::
	.string "{JPN}ふしんな やつを はっけん!$"

SilphCo_4F_Text_Grunt2Defeat::
	.string "{JPN}おまえ なにものだ$"

SilphCo_4F_Text_Grunt2PostBattle::
	.string "{JPN}11かいに いる\n"
	.string "ボスに れんらく しなくては!$"

SilphCo_4F_Text_FloorSign::
	.string "{JPN}シルフ カンパニー ほんしゃ\n"
	.string "4かい$"

