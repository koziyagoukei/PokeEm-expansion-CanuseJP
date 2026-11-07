CinnabarIsland_PokemonLab_Entrance_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarIsland_PokemonLab_Entrance_OnTransition
	.byte 0

CinnabarIsland_PokemonLab_Entrance_OnTransition::
	call_if_eq VAR_MAP_SCENE_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM_REVIVE_STATE, 1, CinnabarIsland_PokemonLab_Entrance_EventScript_ReadyFossil
	end

CinnabarIsland_PokemonLab_Entrance_EventScript_ReadyFossil::
	setvar VAR_MAP_SCENE_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM_REVIVE_STATE, 2
	return

CinnabarIsland_PokemonLab_Entrance_EventScript_Scientist::
	msgbox CinnabarIsland_PokemonLab_Entrance_Text_StudyMonsExtensively, MSGBOX_NPC
	end

CinnabarIsland_PokemonLab_Entrance_EventScript_DrFujiPhoto::
	msgbox CinnabarIsland_PokemonLab_Entrance_Text_PhotoOfLabFounderDrFuji, MSGBOX_SIGN
	end

CinnabarIsland_PokemonLab_Entrance_EventScript_MeetingRoomSign::
	msgbox CinnabarIsland_PokemonLab_Entrance_Text_MeetingRoomSign, MSGBOX_SIGN
	end

CinnabarIsland_PokemonLab_Entrance_EventScript_RAndDRoomSign::
	msgbox CinnabarIsland_PokemonLab_Entrance_Text_RAndDRoomSign, MSGBOX_SIGN
	end

CinnabarIsland_PokemonLab_Entrance_EventScript_TestingRoomSign::
	msgbox CinnabarIsland_PokemonLab_Entrance_Text_TestingRoomSign, MSGBOX_SIGN
	end

CinnabarIsland_Gym_Text_PhotoOfBlaineAndFuji::
	.string "{JPN}カツラと フジろうじんの\n"
	.string "しゃしんが ある!\p"
	.string "かたを くんで\n"
	.string "たのしそうに わらってる!$"

CinnabarIsland_PokemonLab_Entrance_Text_StudyMonsExtensively::
	.string "{JPN}ここでは まいにち\n"
	.string "ポケモンの けんきゅうを してます\p"
	.string "あと めずらしい ポケモンなどを\n"
	.string "もちこんでくる おきゃくさんも\l"
	.string "いらっしゃい ますね$"

CinnabarIsland_PokemonLab_Entrance_Text_PhotoOfLabFounderDrFuji::
	.string "{JPN}グレン ラボラトリーの そうせつしゃ\n"
	.string "フジはかせの しゃしんだ!$"

CinnabarIsland_PokemonLab_Entrance_Text_MeetingRoomSign::
	.string "{JPN}ポケモン ラボ おうせつしつ$"

CinnabarIsland_PokemonLab_Entrance_Text_RAndDRoomSign::
	.string "{JPN}ポケモン ラボ けんきゅうしつ$"

CinnabarIsland_PokemonLab_Entrance_Text_TestingRoomSign::
	.string "{JPN}ポケモン ラボ じっけんしつ$"

