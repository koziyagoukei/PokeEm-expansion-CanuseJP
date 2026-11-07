TwoIsland_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, TwoIsland_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

TwoIsland_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_TWO_ISLAND
	end

TwoIsland_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

TwoIsland_PokemonCenter_1F_EventScript_GBAKid::
	msgbox TwoIsland_PokemonCenter_1F_Text_HaveYouVisitedGameCorner, MSGBOX_NPC
	end

TwoIsland_PokemonCenter_1F_EventScript_BugCatcher::
	msgbox TwoIsland_PokemonCenter_1F_Text_OldLadyLivesOutOnCape, MSGBOX_NPC
	end

TwoIsland_PokemonCenter_1F_Text_HaveYouVisitedGameCorner::
	.string "{JPN}ゲームコーナー いって みた?\p"
	.string "ひとりで いっても ダメだぞ\n"
	.string "だれか ともだちを つれてけよ$"

TwoIsland_PokemonCenter_1F_Text_OldLadyLivesOutOnCape::
	.string "{JPN}2の しまに\n"
	.string "すんでいる ひとは すくないよ\p"
	.string "ただ みさきの いえに\n"
	.string "ずーっと むかしから すんでいる\l"
	.string "おばあさんが いるんだ$"

