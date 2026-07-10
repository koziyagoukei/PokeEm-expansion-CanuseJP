ultra_frontie_hub_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, UltraFrontieHub_OnTransition
	.byte 0

UltraFrontieHub_OnTransition:
	goto_if_set FLAG_TEMP_1, UltraFrontieHub_OnTransition_Done
	special Special_SetupFrontierHubState
	setflag FLAG_TEMP_1
UltraFrontieHub_OnTransition_Done:
	end

UltraFrontieHub_EventScript_FacilityWarpMenu::
	lock
	faceplayer
	msgbox UltraFrontieHub_Text_WhereGo, MSGBOX_DEFAULT
	goto UltraFrontieHub_EventScript_FacilityWarpMenuChoice
	end

UltraFrontieHub_EventScript_FacilityWarpMenuChoice::
	dynmultipush UltraFrontieHub_Text_BattleTower, 0
	dynmultipush UltraFrontieHub_Text_BattleDome, 1
	dynmultipush UltraFrontieHub_Text_BattleFactory, 2
	dynmultipush UltraFrontieHub_Text_BattlePalace, 3
	dynmultipush UltraFrontieHub_Text_BattleArena, 4
	dynmultipush UltraFrontieHub_Text_BattlePike, 5
	dynmultipush UltraFrontieHub_Text_BattlePyramid, 6
	dynmultipush UltraFrontieHub_Text_Cancel, 7
	dynmultistack 0, 0, FALSE, 8, 0, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	switch VAR_RESULT
	case 0, UltraFrontieHub_EventScript_WarpBattleTower
	case 1, UltraFrontieHub_EventScript_WarpBattleDome
	case 2, UltraFrontieHub_EventScript_WarpBattleFactory
	case 3, UltraFrontieHub_EventScript_WarpBattlePalace
	case 4, UltraFrontieHub_EventScript_WarpBattleArena
	case 5, UltraFrontieHub_EventScript_WarpBattlePike
	case 6, UltraFrontieHub_EventScript_WarpBattlePyramid
	case 7, UltraFrontieHub_EventScript_FacilityWarpCancel
	release
	end

UltraFrontieHub_EventScript_WarpBattleTower::
	warp MAP_BATTLE_FRONTIER_BATTLE_TOWER_LOBBY, 255, 13, 8
	waitstate
	end

UltraFrontieHub_EventScript_WarpBattleDome::
	warp MAP_BATTLE_FRONTIER_BATTLE_DOME_LOBBY, 255, 11, 15
	waitstate
	end

UltraFrontieHub_EventScript_WarpBattleFactory::
	warp MAP_BATTLE_FRONTIER_BATTLE_FACTORY_LOBBY, 255, 9, 10
	waitstate
	end

UltraFrontieHub_EventScript_WarpBattlePalace::
	warp MAP_BATTLE_FRONTIER_BATTLE_PALACE_LOBBY, 255, 11, 10
	waitstate
	end

UltraFrontieHub_EventScript_WarpBattleArena::
	warp MAP_BATTLE_FRONTIER_BATTLE_ARENA_LOBBY, 255, 7, 11
	waitstate
	end

UltraFrontieHub_EventScript_WarpBattlePike::
	warp MAP_BATTLE_FRONTIER_BATTLE_PIKE_LOBBY, 255, 5, 11
	waitstate
	end

UltraFrontieHub_EventScript_WarpBattlePyramid::
	warp MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_LOBBY, 255, 7, 15
	waitstate
	end

UltraFrontieHub_EventScript_FacilityWarpCancel::
	msgbox UltraFrontieHub_Text_ComeAgain, MSGBOX_DEFAULT
	release
	end

UltraFrontieHub_Text_WhereGo::
	.string "どこへ　いきますか？$"

UltraFrontieHub_Text_BattleTower::
	.string "バトルタワー$"

UltraFrontieHub_Text_BattleDome::
	.string "バトルドーム$"

UltraFrontieHub_Text_BattleFactory::
	.string "バトルファクトリー$"

UltraFrontieHub_Text_BattlePalace::
	.string "バトルパレス$"

UltraFrontieHub_Text_BattleArena::
	.string "バトルアリーナ$"

UltraFrontieHub_Text_BattlePike::
	.string "バトルチューブ$"

UltraFrontieHub_Text_BattlePyramid::
	.string "バトルピラミッド$"

UltraFrontieHub_Text_Cancel::
	.string "やめる$"

UltraFrontieHub_Text_ComeAgain::
	.string "またどうぞ。$"

UltraFrontieHub_EventScript_FrontierMoveTutor::
	lock
	faceplayer
	msgbox UltraFrontieHub_Text_FrontierMoveTutor, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq UltraFrontieHub_EventScript_StartFrontierMoveTutor
	msgbox UltraFrontieHub_Text_Cancel, MSGBOX_DEFAULT
	release
	end

UltraFrontieHub_EventScript_StartFrontierMoveTutor::
	closemessage
	goto MoveRelearner_EventScript_TeachFrontierFullMoves
	end

UltraFrontieHub_Text_FrontierMoveTutor::
	.string "わざを\nおもいだしますか？$"

UltraFrontieHub_EventScript_FrontierPokemonCreator::
	lock
	faceplayer
	msgbox UltraFrontieHub_Text_FrontierPokemonCreator, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq UltraFrontieHub_EventScript_OpenFrontierPokemonCreator
	msgbox UltraFrontieHub_Text_Cancel, MSGBOX_DEFAULT
	release
	end

UltraFrontieHub_EventScript_OpenFrontierPokemonCreator::
	closemessage
	special Special_OpenFrontierPokemonCreator
	waitstate
	release
	end

UltraFrontieHub_Text_FrontierPokemonCreator::
	.string "フロンティアようの\nポケモンを つくりますか？$"

UltraFrontieHub_EventScript_FrontierItemGiver::
	lock
	faceplayer
	msgbox UltraFrontieHub_Text_FrontierItemGiver, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq UltraFrontieHub_EventScript_OpenFrontierItemGiver
	msgbox UltraFrontieHub_Text_Cancel, MSGBOX_DEFAULT
	release
	end

UltraFrontieHub_EventScript_OpenFrontierItemGiver::
	closemessage
	special Special_OpenFrontierItemGiver
	waitstate
	release
	end

UltraFrontieHub_Text_FrontierItemGiver::
	.string "フロンティアようの\nどうぐを もらいますか？$"

UltraFrontieHub_EventScript_SetFrontierAi::
	lock
	faceplayer
	msgbox UltraFrontieHub_Text_SetFrontierAi, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_FRONTIER_AI_LEVEL, FALSE
	switch VAR_RESULT
	case 0, UltraFrontieHub_EventScript_SetFrontierAi_Normal
	case 1, UltraFrontieHub_EventScript_SetFrontierAi_Hard
	case 2, UltraFrontieHub_EventScript_SetFrontierAi_Strongest
	msgbox UltraFrontieHub_Text_Cancel, MSGBOX_DEFAULT
	release
	end

UltraFrontieHub_EventScript_SetFrontierAi_Normal::
	special Special_SetFrontierAiNormal
	msgbox UltraFrontieHub_Text_SetFrontierAi_NormalDone, MSGBOX_DEFAULT
	release
	end

UltraFrontieHub_EventScript_SetFrontierAi_Hard::
	special Special_SetFrontierAiHard
	msgbox UltraFrontieHub_Text_SetFrontierAi_HardDone, MSGBOX_DEFAULT
	release
	end

UltraFrontieHub_EventScript_SetFrontierAi_Strongest::
	special Special_SetFrontierAiStrongest
	msgbox UltraFrontieHub_Text_SetFrontierAi_StrongestDone, MSGBOX_DEFAULT
	release
	end

UltraFrontieHub_EventScript_SetFrontierBannedSpecies::
	lock
	faceplayer
	msgbox UltraFrontieHub_Text_SetFrontierBannedSpecies, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_FRONTIER_BANNED_SPECIES, FALSE
	switch VAR_RESULT
	case 0, UltraFrontieHub_EventScript_AllowFrontierBannedSpecies
	case 1, UltraFrontieHub_EventScript_BanFrontierBannedSpecies
	msgbox UltraFrontieHub_Text_Cancel, MSGBOX_DEFAULT
	release
	end

UltraFrontieHub_EventScript_AllowFrontierBannedSpecies::
	special Special_SetFrontierAllowBannedSpecies
	msgbox UltraFrontieHub_Text_SetFrontierBannedSpeciesAllowDone, MSGBOX_DEFAULT
	release
	end

UltraFrontieHub_EventScript_BanFrontierBannedSpecies::
	special Special_ClearFrontierAllowBannedSpecies
	msgbox UltraFrontieHub_Text_SetFrontierBannedSpeciesBanDone, MSGBOX_DEFAULT
	release
	end

UltraFrontieHub_Text_SetFrontierAi::
	.string "フロンティアの AIを\nえらんでください。$"

UltraFrontieHub_Text_SetFrontierAi_NormalDone::
	.string "AIを いつものに\nしました。$"

UltraFrontieHub_Text_SetFrontierAi_HardDone::
	.string "AIを むずかしいに\nしました。$"

UltraFrontieHub_Text_SetFrontierBannedSpecies::
	.string "BANNED POKEMON RULE$"

UltraFrontieHub_Text_SetFrontierBannedSpeciesAllowDone::
	.string "BANNED POKEMON ALLOWED.$"

UltraFrontieHub_Text_SetFrontierBannedSpeciesBanDone::
	.string "BANNED POKEMON BANNED.$"

UltraFrontieHub_Text_SetFrontierAi_StrongestDone::
	.string "AIを さいきょうに\nしました。$"
