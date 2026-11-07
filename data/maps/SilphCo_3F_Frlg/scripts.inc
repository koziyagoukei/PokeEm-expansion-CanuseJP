SilphCo_3F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_3F_OnLoad
	.byte 0

SilphCo_3F_OnLoad::
	call_if_unset FLAG_SILPH_3F_DOOR_1, EventScript_Close3FDoor1
	call_if_unset FLAG_SILPH_3F_DOOR_2, EventScript_Close3FDoor2
	end

SilphCo_3F_EventScript_WorkerM::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_3F_EventScript_WorkerMRocketsGone
	msgbox SilphCo_3F_Text_WhatAmIToDo
	release
	end

SilphCo_3F_EventScript_WorkerMRocketsGone::
	msgbox SilphCo_3F_Text_YouAndYourMonsSavedUs
	release
	end

SilphCo_3F_EventScript_FloorSign::
	msgbox SilphCo_3F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_3F_EventScript_Jose::
	trainerbattle_single TRAINER_SCIENTIST_JOSE, SilphCo_3F_Text_JoseIntro, SilphCo_3F_Text_JoseDefeat
	msgbox SilphCo_3F_Text_JosePostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_3F_EventScript_Grunt::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_25, SilphCo_3F_Text_GruntIntro, SilphCo_3F_Text_GruntDefeat
	msgbox SilphCo_3F_Text_GruntPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_3F_Text_WhatAmIToDo::
	.string "{JPN}ぼく シルフ カンパニーの\n"
	.string "しゃいんです… おろおろ$"

SilphCo_3F_Text_YouAndYourMonsSavedUs::
	.string "{JPN}{PLAYER}{KUN}と\n"
	.string "ポケモンの おかげで\l"
	.string "ほんと たすかったよ$"

SilphCo_3F_Text_GruntIntro::
	.string "{JPN}おれたちの じゃまを する\n"
	.string "やつは じゃま なのだ!$"

SilphCo_3F_Text_GruntDefeat::
	.string "{JPN}まいった!$"

SilphCo_3F_Text_GruntPostBattle::
	.string "{JPN}… では ヒントだ!\n"
	.string "しまってる ドアの カギは\l"
	.string "カード キーで あけられる$"

SilphCo_3F_Text_JoseIntro::
	.string "{JPN}わたしは\n"
	.string "シルフ カンパニーより\l"
	.string "ロケットだんに みかた する!$"

SilphCo_3F_Text_JoseDefeat::
	.string "{JPN}やるねえ!$"

SilphCo_3F_Text_JosePostBattle::
	.string "{JPN}… なに\p"
	.string "ロケットだんの れんちゅうが\n"
	.string "おもう ぞんぶん\l"
	.string "ポケモンの けんきゅうを\l"
	.string "させて くれるって いったんでね$"

SilphCo_3F_Text_FloorSign::
	.string "{JPN}シルフ カンパニー ほんしゃ\n"
	.string "3かい$"

