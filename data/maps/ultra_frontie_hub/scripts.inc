ultra_frontie_hub_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, UltraFrontieHub_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, UltraFrontieHub_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, UltraFrontieHub_OnFrame
	.byte 0

UltraFrontieHub_OnLoad:
	special Special_RestoreResortInterior
	setflag FLAG_TEMP_10
	call FrontierStory_EventScript_SetupHub
	call FinalStory_EventScript_SetupEndingHub
	special FrontierPokevirus_CheckEpilogue
	copyvar VAR_TEMP_6, VAR_RESULT
	goto_if_ne VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_ARRIVAL, UltraFrontieHub_OnLoad_Done
	clearflag FLAG_TEMP_10
UltraFrontieHub_OnLoad_Done:
	end

UltraFrontieHub_OnFrame:
	map_script_2 VAR_TEMP_6, 1, FinalStory_EventScript_PokevirusEpilogue
	map_script_2 VAR_STORY_ENDING_STATE, ENDING_STATE_HUB, FinalStory_EventScript_EndingHub
	map_script_2 VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_ARRIVAL, FrontierIntro_EventScript_Arrival
	map_script_2 VAR_TOWER_STORY_STATE, TOWER_STORY_RETURNED, TowerStory_EventScript_HubCare
	map_script_2 VAR_TEMP_5, 1, FrontierStory_EventScript_HubRescue
	map_script_2 VAR_TEMP_2, 0, FrontierStory_EventScript_HubArrival
	.2byte 0

UltraFrontieHub_OnTransition:
	setrespawn HEAL_LOCATION_BATTLE_FRONTIER_OUTSIDE_EAST
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
	.string "{JPN}こちらは 施設への 案内です。\nちょくせつ むかうなら どちらへ？\p外を 歩いて むかうことも できます。\nおすきな 方法で どうぞ！$"

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
	goto Common_EventScript_MoveRelearnerDynMultiChoice
	end

UltraFrontieHub_Text_FrontierMoveTutor::
	.string "{JPN}技の そうだんは まかせて！\nきみの ポケモンに あう 技を さがそう。\p技を 思いだしますか？$"

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
	.string "{JPN}新しい 仲間を 探すなら\nぼくに こえを かけてね！\pフロンティアようの\nポケモンを 作りますか？$"

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
	.string "{JPN}キルリアは 道具を 並べている。\nほしいものを 選んでみよう！$"

UltraFrontieHub_EventScript_SetFrontierAi::
	lock
	faceplayer
	msgbox UltraFrontieHub_Text_SetFrontierAi, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_FRONTIER_AI_LEVEL, FALSE
	switch VAR_RESULT
	case 0, UltraFrontieHub_EventScript_SetFrontierAi_Easy
	case 1, UltraFrontieHub_EventScript_SetFrontierAi_Normal
	case 2, UltraFrontieHub_EventScript_SetFrontierAi_Hard
	case 3, UltraFrontieHub_EventScript_SetFrontierAi_Strongest
	msgbox UltraFrontieHub_Text_Cancel, MSGBOX_DEFAULT
	release
	end

UltraFrontieHub_EventScript_SetFrontierAi_Easy::
	special Special_SetFrontierAiEasy
	msgbox UltraFrontieHub_Text_SetFrontierAi_EasyDone, MSGBOX_DEFAULT
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
	.string "{JPN}ピカチュウの 前に 設定きが ある。\nフロンティアの 強さを えらぼう。$"

UltraFrontieHub_Text_SetFrontierAi_EasyDone::
	.string "AIを かんたんに\nしました。\p相手の ポケモンの どりょくちが\n0に なります。\pコンテストの 相手も\nてかげん してくれます。$"

UltraFrontieHub_Text_SetFrontierAi_NormalDone::
	.string "AIを いつものに\nしました。$"

UltraFrontieHub_Text_SetFrontierAi_HardDone::
	.string "AIを 難しいに\nしました。$"

UltraFrontieHub_Text_SetFrontierBannedSpecies::
	.string "{JPN}……。 強い ポケモンとの\n勝負も のぞんでいるのか？\pきんしポケモンの 設定を\n選んでくれ。$"

UltraFrontieHub_Text_SetFrontierBannedSpeciesAllowDone::
	.string "{JPN}きんしポケモンの 参加を\n認める 設定に した。$"

UltraFrontieHub_Text_SetFrontierBannedSpeciesBanDone::
	.string "{JPN}きんしポケモンの 参加を\n認めない 設定に した。$"

UltraFrontieHub_Text_SetFrontierAi_StrongestDone::
	.string "AIを 最強に\nしました。$"

ultra_frontie_hub_VisualEvent1::
	lockall
	faceplayer
	msgbox UltraFrontieHub_Text_MusicIntro, MSGBOX_DEFAULT
	goto UltraFrontieHub_EventScript_MusicMenu

UltraFrontieHub_EventScript_MusicMenu:
	special FrontierMusic_BufferSelection
	msgbox UltraFrontieHub_Text_MusicCurrent, MSGBOX_DEFAULT
	dynmultipush UltraFrontieHub_Text_MusicChoose, 0
	dynmultipush UltraFrontieHub_Text_MusicRandom, 1
	dynmultipush UltraFrontieHub_Text_MusicDefault, 2
	dynmultipush UltraFrontieHub_Text_Cancel, 3
	dynmultistack 0, 0, FALSE, 4, 0, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case 0, UltraFrontieHub_EventScript_MusicCatalog
	case 1, UltraFrontieHub_EventScript_MusicRandom
	case 2, UltraFrontieHub_EventScript_MusicDefault
	goto UltraFrontieHub_EventScript_MusicGoodbye

UltraFrontieHub_EventScript_MusicCatalog:
	closemessage
	special ShowBattlePointsWindow
	special FrontierMusic_OpenCatalog
	special CloseBattlePointsWindow
	goto_if_eq VAR_RESULT, FALSE, UltraFrontieHub_EventScript_MusicMenu
	setvar VAR_0x8006, FRONTIER_MUSIC_FIXED
	special FrontierMusic_ApplySelection
	goto_if_eq VAR_RESULT, FRONTIER_MUSIC_NOT_UNLOCKED, UltraFrontieHub_EventScript_MusicUnlock
	goto_if_ne VAR_RESULT, FRONTIER_MUSIC_OK, UltraFrontieHub_EventScript_MusicMenu
	msgbox UltraFrontieHub_Text_MusicFixedDone, MSGBOX_DEFAULT
	goto UltraFrontieHub_EventScript_MusicMenu

UltraFrontieHub_EventScript_MusicUnlock:
	msgbox UltraFrontieHub_Text_MusicConfirm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, UltraFrontieHub_EventScript_MusicCatalog
	special FrontierMusic_UnlockSelection
	goto_if_eq VAR_RESULT, FRONTIER_MUSIC_NOT_ENOUGH_BP, UltraFrontieHub_EventScript_MusicNotEnough
	goto_if_eq VAR_RESULT, FRONTIER_MUSIC_ALREADY_UNLOCKED, UltraFrontieHub_EventScript_MusicUnlocked
	goto_if_ne VAR_RESULT, FRONTIER_MUSIC_OK, UltraFrontieHub_EventScript_MusicMenu
	playse SE_SHOP
	waitse
UltraFrontieHub_EventScript_MusicUnlocked:
	special FrontierMusic_ApplySelection
	msgbox UltraFrontieHub_Text_MusicUnlockDone, MSGBOX_DEFAULT
	goto UltraFrontieHub_EventScript_MusicMenu

UltraFrontieHub_EventScript_MusicNotEnough:
	msgbox UltraFrontieHub_Text_MusicNotEnough, MSGBOX_DEFAULT
	goto UltraFrontieHub_EventScript_MusicCatalog

UltraFrontieHub_EventScript_MusicRandom:
	setvar VAR_0x8006, FRONTIER_MUSIC_RANDOM
	special FrontierMusic_ApplySelection
	goto_if_eq VAR_RESULT, FRONTIER_MUSIC_EMPTY, UltraFrontieHub_EventScript_MusicEmpty
	msgbox UltraFrontieHub_Text_MusicRandomDone, MSGBOX_DEFAULT
	goto UltraFrontieHub_EventScript_MusicMenu

UltraFrontieHub_EventScript_MusicEmpty:
	msgbox UltraFrontieHub_Text_MusicEmpty, MSGBOX_DEFAULT
	goto UltraFrontieHub_EventScript_MusicMenu

UltraFrontieHub_EventScript_MusicDefault:
	setvar VAR_0x8006, FRONTIER_MUSIC_DEFAULT
	special FrontierMusic_ApplySelection
	msgbox UltraFrontieHub_Text_MusicDefaultDone, MSGBOX_DEFAULT
	goto UltraFrontieHub_EventScript_MusicMenu

UltraFrontieHub_EventScript_MusicGoodbye:
	msgbox UltraFrontieHub_Text_MusicGoodbye, MSGBOX_DEFAULT
	releaseall
	end

UltraFrontieHub_Text_MusicIntro:
	.string "{JPN}ぼくは とおりすがりの\nおんがく オタク。\p1きょく 5BPで かいほうすると\n施設の バトルで ながせるよ！\pブレーンせんは いつもどおり\n特別な BGMに なるよ。$"

UltraFrontieHub_Text_MusicCurrent:
	.string "{JPN}今の 設定は\n{STR_VAR_1} だよ。$"

UltraFrontieHub_Text_MusicChoose:
	.string "{JPN}きょくを 選ぶ$"

UltraFrontieHub_Text_MusicRandom:
	.string "{JPN}まいせん ランダム$"

UltraFrontieHub_Text_MusicDefault:
	.string "{JPN}もとの BGMに もどす$"

UltraFrontieHub_Text_MusicConfirm:
	.string "{JPN}{STR_VAR_1}を\n5BPで かいほうして 設定する？$"

UltraFrontieHub_Text_MusicUnlockDone:
	.string "{JPN}かいほうしたよ！\n{STR_VAR_1}に 設定したよ。$"

UltraFrontieHub_Text_MusicFixedDone:
	.string "{JPN}{STR_VAR_1}に\n設定したよ。BPは 使わないよ。$"

UltraFrontieHub_Text_MusicRandomDone:
	.string "{JPN}解放済みの 曲から\n"
	.string "1戦ごとに ランダムで 流すよ！$"

UltraFrontieHub_Text_MusicDefaultDone:
	.string "{JPN}もとの BGMに 戻したよ。\nかいほうした きょくは 残るよ！$"

UltraFrontieHub_Text_MusicNotEnough:
	.string "{JPN}BPが 足りないよ。\n"
	.string "1曲の 解放には 5BPだよ。$"

UltraFrontieHub_Text_MusicEmpty:
	.string "{JPN}まだ きょくを かいほうしてないよ。\nまずは 好きな きょくを 選んでね！$"

UltraFrontieHub_Text_MusicGoodbye:
	.string "{JPN}おんがくと 一緒に\nバトルを 楽しんでね！$"
