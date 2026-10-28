BattleFrontier_BattlePyramidLobby_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_BattlePyramidLobby_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_BattlePyramidLobby_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, BattleFrontier_BattleDomeLobby_OnWarp
	.byte 0

BattleFrontier_BattlePyramidLobby_OnTransition:
	@ The cook and Pokevirus only appear for Nobuo's release battle.
	setflag FLAG_TEMP_1D
	setflag FLAG_TEMP_1E
	setvar VAR_0x8004, STORY_TARGET_NOBUO
	call FrontierStory_EventScript_SetupTarget
	end

@ Unused. Pyramid uses Dome's OnWarp (presumably by mistake). Their effects are identical
BattleFrontier_BattlePyramidLobby_OnWarp:
	map_script_2 VAR_TEMP_1, 0, BattleFrontier_BattlePyramidLobby_EventScript_TurnPlayerNorth
	.2byte 0

BattleFrontier_BattlePyramidLobby_EventScript_TurnPlayerNorth:
	setvar VAR_TEMP_1, 1
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

BattleFrontier_BattlePyramidLobby_OnFrame:
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, 0, BattleFrontier_BattlePyramidLobby_EventScript_GetChallengeStatus
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_SAVING, BattleFrontier_BattlePyramidLobby_EventScript_QuitWithoutSaving
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_PAUSED, BattleFrontier_BattlePyramidLobby_EventScript_ResumeChallenge
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_WON, BattleFrontier_BattlePyramidLobby_EventScript_WonChallenge
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_LOST, BattleFrontier_BattlePyramidLobby_EventScript_LostChallenge
	map_script_2 VAR_TEMP_E, 0, PyramidStory_EventScript_LobbyArrival
	map_script_2 VAR_TEMP_E, 1, FrontierStory_EventScript_LobbyDarkraiCall
	.2byte 0

BattleFrontier_BattlePyramidLobby_EventScript_GetChallengeStatus::
	frontier_getstatus
	end

BattleFrontier_BattlePyramidLobby_EventScript_QuitWithoutSaving::
	lockall
	message BattleFrontier_BattlePyramidLobby_Text_DidntSaveBeforeQuittingTakeBag
	waitmessage
	playse SE_EXP_MAX
	waitse
	closemessage
	pyramid_set PYRAMID_DATA_WIN_STREAK, 0
	pyramid_set PYRAMID_DATA_WIN_STREAK_ACTIVE, FALSE
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	releaseall
	end

BattleFrontier_BattlePyramidLobby_EventScript_WonChallenge::
	lockall
	frontier_isbrain
	goto_if_eq VAR_RESULT, TRUE, BattleFrontier_BattlePyramidLobby_EventScript_DefeatedKing
	msgbox BattleFrontier_BattlePyramidLobby_Text_YouveConqueredPyramid, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_GiveBattlePoints

BattleFrontier_BattlePyramidLobby_EventScript_DefeatedKing::
	msgbox BattleFrontier_BattlePyramidLobby_Text_YouveDefeatedPyramidKing, MSGBOX_DEFAULT
BattleFrontier_BattlePyramidLobby_EventScript_GiveBattlePoints::
	special DoBattlePyramidMonsHaveHeldItem
	call_if_eq VAR_RESULT, TRUE, BattleFrontier_BattlePyramidLobby_EventScript_StoreHeldItemsInPyramidBag
	clearflag FLAG_STORING_ITEMS_IN_PYRAMID_BAG
	frontier_checkairshow
	special LoadPlayerParty
	pyramid_clearhelditems
	special HealPlayerParty
	callnative UpdateFollowingPokemon
	message BattleFrontier_BattlePyramidLobby_Text_UsedBattleBagWillBeKept
	waitmessage
	playse SE_EXP_MAX
	waitse
	msgbox BattleFrontier_BattlePyramidLobby_Text_GiveYouTheseBattlePoints, MSGBOX_DEFAULT
	frontier_givepoints
	msgbox BattleFrontier_Text_ObtainedXBattlePoints, MSGBOX_GETPOINTS
	message BattleFrontier_BattlePyramidLobby_Text_RecordResultsWait
	waitmessage
	pyramid_setprize
	pyramid_save 0
	playse SE_SAVE
	waitse
	msgbox BattleFrontier_BattlePyramidLobby_Text_LookForwardToNextChallenge, MSGBOX_DEFAULT
	closemessage
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	releaseall
	end

BattleFrontier_BattlePyramidLobby_EventScript_LostChallenge::
	frontier_checkairshow
	special LoadPlayerParty
	pyramid_clearhelditems
	special HealPlayerParty
	callnative UpdateFollowingPokemon
	lockall
	message BattleFrontier_BattlePyramidLobby_Text_DisappointingHereIsBag
	waitmessage
	playse SE_EXP_MAX
	waitse
	message BattleFrontier_BattlePyramidLobby_Text_RecordResultsWait
	waitmessage
	pyramid_set PYRAMID_DATA_WIN_STREAK_ACTIVE, FALSE
	pyramid_save 0
	playse SE_SAVE
	waitse
	msgbox BattleFrontier_BattlePyramidLobby_Text_LookForwardToNextChallenge, MSGBOX_DEFAULT
	closemessage
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	releaseall
	end

BattleFrontier_BattlePyramidLobby_EventScript_ResumeChallenge::
	goto BattleFrontier_BattlePyramidLobby_EventScript_EnterChallenge

BattleFrontier_BattlePyramidLobby_EventScript_Attendant::
	lock
	faceplayer
	setvar VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_PYRAMID
	setvar VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES
	special SavePlayerParty
	msgbox BattleFrontier_BattlePyramidLobby_Text_WelcomeToBattlePyramid, MSGBOX_DEFAULT
BattleFrontier_BattlePyramidLobby_EventScript_AskTakeChallenge::
	message BattleFrontier_BattlePyramidLobby_Text_EmbarkOnChallenge
	waitmessage
	multichoice 17, 6, MULTI_CHALLENGEINFO, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_BattlePyramidLobby_EventScript_TryEnterChallenge
	case 1, BattleFrontier_BattlePyramidLobby_EventScript_ExplainChallenge
	case 2, BattleFrontier_BattlePyramidLobby_EventScript_CancelChallenge
	case MULTI_B_PRESSED, BattleFrontier_BattlePyramidLobby_EventScript_CancelChallenge

BattleFrontier_BattlePyramidLobby_EventScript_TryEnterChallenge::
	message BattleFrontier_BattlePyramidLobby_Text_WhichLevelMode
	waitmessage
	multichoice 17, 6, MULTI_LEVEL_MODE, FALSE
	switch VAR_RESULT
	case FRONTIER_LVL_TENT, BattleFrontier_BattlePyramidLobby_EventScript_CancelChallenge
	case MULTI_B_PRESSED, BattleFrontier_BattlePyramidLobby_EventScript_CancelChallenge
	frontier_checkineligible
	goto_if_eq VAR_0x8004, TRUE, BattleFrontier_BattlePyramidLobby_EventScript_NotEnoughValidMons
	frontier_set FRONTIER_DATA_LVL_MODE, VAR_RESULT
	msgbox BattleFrontier_BattlePyramidLobby_Text_SelectThreeMons, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	callnative RemoveFollowingPokemon
	call BattleFrontier_EventScript_GetLvlMode
	copyvar VAR_0x8004, VAR_RESULT
	setvar VAR_0x8005, FRONTIER_PARTY_SIZE
	special ChoosePartyForBattleFrontier
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattlePyramidLobby_EventScript_LoadPartyAndCancelChallenge
	msgbox BattleFrontier_BattlePyramidLobby_Text_OkayToSaveBeforeChallenge, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, BattleFrontier_BattlePyramidLobby_EventScript_LoadPartyAndCancelChallenge
	case YES, BattleFrontier_BattlePyramidLobby_EventScript_SaveBeforeChallenge
	case MULTI_B_PRESSED, BattleFrontier_BattlePyramidLobby_EventScript_LoadPartyAndCancelChallenge

BattleFrontier_BattlePyramidLobby_EventScript_SaveBeforeChallenge::
	setvar VAR_TEMP_CHALLENGE_STATUS, 0
	frontier_set FRONTIER_DATA_SELECTED_MON_ORDER
	pyramid_init
	pyramid_set PYRAMID_DATA_WIN_STREAK_ACTIVE, TRUE
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, CHALLENGE_STATUS_SAVING
	frontier_set FRONTIER_DATA_PAUSED, FALSE
	pyramid_seedfloor
	pyramid_settrainers
	special LoadPlayerParty
	closemessage
	delay 2
	call Common_EventScript_SaveGame
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattlePyramidLobby_EventScript_CancelChallengeSaveFailed
BattleFrontier_BattlePyramidLobby_EventScript_EnterChallenge::
	special SavePlayerParty
	frontier_setpartyorder FRONTIER_PARTY_SIZE
	msgbox BattleFrontier_BattlePyramidLobby_Text_ShowYouIntoPyramid, MSGBOX_DEFAULT
	closemessage
	call BattleFrontier_BattlePyramidLobby_EventScript_WalkToPanelAndReceiveBag
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	setvar VAR_RESULT, 0
	special HealPlayerParty
	warp MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR, 1, 1
	setvar VAR_TEMP_CHALLENGE_STATUS, 0
	waitstate
	end

BattleFrontier_BattlePyramidLobby_EventScript_ExplainChallenge::
	msgbox BattleFrontier_BattlePyramidLobby_Text_ExplainBattlePyramid, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_AskTakeChallenge

BattleFrontier_BattlePyramidLobby_EventScript_NotEnoughValidMons::
	switch VAR_RESULT
	case FRONTIER_LVL_50, BattleFrontier_BattlePyramidLobby_EventScript_NotEnoughValidMonsLv50
	case FRONTIER_LVL_OPEN, BattleFrontier_BattlePyramidLobby_EventScript_NotEnoughValidMonsLvOpen

BattleFrontier_BattlePyramidLobby_EventScript_NotEnoughValidMonsLv50::
	msgbox BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsLv50, MSGBOX_DEFAULT
	call BattleFrontier_ShowCaughtBannedSpecies
	msgbox BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsEnd, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_EndCancelChallenge

BattleFrontier_BattlePyramidLobby_EventScript_NotEnoughValidMonsLvOpen::
	msgbox BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsLvOpen, MSGBOX_DEFAULT
	call BattleFrontier_ShowCaughtBannedSpecies
	msgbox BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsEnd, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_EndCancelChallenge

BattleFrontier_BattlePyramidLobby_EventScript_CancelChallengeSaveFailed::
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	goto BattleFrontier_BattlePyramidLobby_EventScript_CancelChallenge

BattleFrontier_BattlePyramidLobby_EventScript_LoadPartyAndCancelChallenge::
	special LoadPlayerParty
	callnative UpdateFollowingPokemon
BattleFrontier_BattlePyramidLobby_EventScript_CancelChallenge::
	msgbox BattleFrontier_BattlePyramidLobby_Text_AwaitFutureChallenge, MSGBOX_DEFAULT
BattleFrontier_BattlePyramidLobby_EventScript_EndCancelChallenge::
	release
	end

BattleFrontier_BattlePyramidLobby_EventScript_HintGiver::
	lockall
	applymovement LOCALID_PYRAMID_LOBBY_HINT_GIVER, Common_Movement_FacePlayer
	waitmovement 0
	msgbox BattleFrontier_BattlePyramidLobby_Text_TellYouWhatMisfortunesAwait, MSGBOX_DEFAULT
	call BattleFrontier_BattlePyramidLobby_EventScript_GiveHint
	msgbox BattleFrontier_BattlePyramidLobby_Text_BelieveMyFortunesOrNot, MSGBOX_DEFAULT
	releaseall
	end

BattleFrontier_BattlePyramidLobby_EventScript_GiveHint::
	multichoice 17, 6, MULTI_LEVEL_MODE, FALSE
	switch VAR_RESULT
	case FRONTIER_LVL_50, BattleFrontier_BattlePyramidLobby_EventScript_GiveHintLv50
	case FRONTIER_LVL_OPEN, BattleFrontier_BattlePyramidLobby_EventScript_GiveHintLvOpen
	case FRONTIER_LVL_TENT, BattleFrontier_BattlePyramidLobby_EventScript_NoHint
	case MULTI_B_PRESSED, BattleFrontier_BattlePyramidLobby_EventScript_NoHint
	return

@ Shouldnt occur
BattleFrontier_BattlePyramidLobby_EventScript_NoHint::
	return

BattleFrontier_BattlePyramidLobby_EventScript_GiveHintLv50::
	msgbox BattleFrontier_BattlePyramidLobby_Text_Aah, MSGBOX_DEFAULT
	pyramid_get PYRAMID_DATA_WIN_STREAK_ACTIVE_50
	goto_if_ne VAR_RESULT, FALSE, BattleFrontier_BattlePyramidLobby_EventScript_GiveHintGetLv50Streak
	setvar VAR_RESULT, 0
	goto BattleFrontier_BattlePyramidLobby_EventScript_DoHintComment
	return

BattleFrontier_BattlePyramidLobby_EventScript_GiveHintGetLv50Streak::
	pyramid_get PYRAMID_DATA_WIN_STREAK_50
	goto BattleFrontier_BattlePyramidLobby_EventScript_DoHintComment
	return

BattleFrontier_BattlePyramidLobby_EventScript_GiveHintLvOpen::
	msgbox BattleFrontier_BattlePyramidLobby_Text_Aah, MSGBOX_DEFAULT
	pyramid_get PYRAMID_DATA_WIN_STREAK_ACTIVE_OPEN
	goto_if_ne VAR_RESULT, FALSE, BattleFrontier_BattlePyramidLobby_EventScript_GiveHintGetLvOpenStreak
	setvar VAR_RESULT, 0
	goto BattleFrontier_BattlePyramidLobby_EventScript_DoHintComment
	return

BattleFrontier_BattlePyramidLobby_EventScript_GiveHintGetLvOpenStreak::
	pyramid_get PYRAMID_DATA_WIN_STREAK_OPEN
	goto BattleFrontier_BattlePyramidLobby_EventScript_DoHintComment
	return

BattleFrontier_BattlePyramidLobby_EventScript_DoHintComment::
	copyvar VAR_0x8004, VAR_RESULT
	special GetBattlePyramidHint
	switch VAR_RESULT
	case 0, BattleFrontier_BattlePyramidLobby_EventScript_HintParalysis
	case 1, BattleFrontier_BattlePyramidLobby_EventScript_HintPoison
	case 2, BattleFrontier_BattlePyramidLobby_EventScript_HintBurn
	case 3, BattleFrontier_BattlePyramidLobby_EventScript_HintPPWaste
	case 4, BattleFrontier_BattlePyramidLobby_EventScript_HintLevitate
	case 5, BattleFrontier_BattlePyramidLobby_EventScript_HintTrapAbility
	case 6, BattleFrontier_BattlePyramidLobby_EventScript_HintIce
	case 7, BattleFrontier_BattlePyramidLobby_EventScript_HintExplosion
	case 8, BattleFrontier_BattlePyramidLobby_EventScript_HintPsychic
	case 9, BattleFrontier_BattlePyramidLobby_EventScript_HintRock
	case 10, BattleFrontier_BattlePyramidLobby_EventScript_HintFighting
	case 11, BattleFrontier_BattlePyramidLobby_EventScript_HintWeather
	case 12, BattleFrontier_BattlePyramidLobby_EventScript_HintBug
	case 13, BattleFrontier_BattlePyramidLobby_EventScript_HintDark
	case 14, BattleFrontier_BattlePyramidLobby_EventScript_HintWater
	case 15, BattleFrontier_BattlePyramidLobby_EventScript_HintGhost
	case 16, BattleFrontier_BattlePyramidLobby_EventScript_HintSteel
	case 17, BattleFrontier_BattlePyramidLobby_EventScript_HintFlyingDragon
	case 18, BattleFrontier_BattlePyramidLobby_EventScript_HintStoneEvolve
	case 19, BattleFrontier_BattlePyramidLobby_EventScript_HintNormal
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintParalysis::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintParalysis, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintPoison::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintPoison, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintBurn::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintBurn, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintPPWaste::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintPPWaste, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintLevitate::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintLevitate, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintTrapAbility::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintTrapAbility, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintIce::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintIce, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintExplosion::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintExplosion, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintPsychic::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintPsychic, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintRock::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintRock, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintFighting::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintFighting, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintWeather::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintWeather, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintBug::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintBug, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintDark::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintDark, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintWater::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintWater, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintGhost::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintGhost, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintSteel::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintSteel, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintFlyingDragon::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintFlyingDragon, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintStoneEvolve::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintStoneEvolve, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintNormal::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintNormal, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_ShowResults::
	lockall
	frontier_results FRONTIER_FACILITY_PYRAMID
	waitbuttonpress
	special RemoveRecordsWindow
	releaseall
	end

BattleFrontier_BattlePyramidLobby_EventScript_WalkToPanelAndReceiveBag::
	applymovement LOCALID_PYRAMID_LOBBY_ATTENDANT, BattleFrontier_BattlePyramidLobby_Movement_AttendantWalkToPanel
	applymovement LOCALID_PLAYER, BattleFrontier_BattlePyramidLobby_Movement_PlayerWalkToPanel
	waitmovement 0
	msgbox BattleFrontier_BattlePyramidLobby_Text_WeWillHoldBagForSafekeeping, MSGBOX_DEFAULT
	pyramid_get PYRAMID_DATA_WIN_STREAK
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattlePyramidLobby_EventScript_ReceiveNewBattleBag
	msgbox BattleFrontier_BattlePyramidLobby_Text_PleaseTakePreviousBattleBag, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_ReceiveBattleBag

BattleFrontier_BattlePyramidLobby_EventScript_ReceiveNewBattleBag::
	msgbox BattleFrontier_BattlePyramidLobby_Text_PleaseTakeThisBattleBag, MSGBOX_DEFAULT
BattleFrontier_BattlePyramidLobby_EventScript_ReceiveBattleBag::
	message BattleFrontier_BattlePyramidLobby_Text_ExchangedBagForBattleBag
	waitmessage
	playse SE_EXP_MAX
	waitse
	msgbox BattleFrontier_BattlePyramidLobby_Text_StepOnFloorPanel, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PYRAMID_LOBBY_ATTENDANT, BattleFrontier_BattlePyramidLobby_Movement_AttendantMoveAside
	waitmovement 0
	applymovement LOCALID_PLAYER, BattleFrontier_BattlePyramidLobby_Movement_PlayerStepOnPanel
	waitmovement 0
	return

BattleFrontier_BattlePyramidLobby_Movement_AttendantWalkToPanel:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	face_down
	step_end

BattleFrontier_BattlePyramidLobby_Movement_PlayerWalkToPanel:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

BattleFrontier_BattlePyramidLobby_Movement_AttendantMoveAside:
	walk_right
	face_left
	step_end

BattleFrontier_BattlePyramidLobby_Movement_PlayerStepOnPanel:
	walk_up
	walk_up
	step_end

BattleFrontier_BattlePyramidLobby_EventScript_StoreHeldItemsInPyramidBag::
	msgbox BattleFrontier_BattlePyramidLobby_Text_MonHoldingItemCannotTake, MSGBOX_DEFAULT
	setflag FLAG_STORING_ITEMS_IN_PYRAMID_BAG
	special TryStoreHeldItemsInPyramidBag
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattlePyramidLobby_EventScript_HeldItemsStoredInPyramidBag
	message BattleFrontier_BattlePyramidLobby_Text_BagCannotHoldPickItemsToKeep
	waitmessage
	goto BattleFrontier_BattlePyramidLobby_EventScript_PickItemsToKeep
	end

BattleFrontier_BattlePyramidLobby_EventScript_HeldItemsStoredInPyramidBag::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HeldItemsMovedToBag, MSGBOX_DEFAULT
	return

@ When exiting Battle Pyramid with a full pyramid bag and held items the player must select to keep/toss party held items and make room for any kept items by tossing from the pyramid bag
BattleFrontier_BattlePyramidLobby_EventScript_PickItemsToKeep::
	multichoice 17, 6, MULTI_FRONTIER_ITEM_CHOOSE, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_BattlePyramidLobby_EventScript_PickItemsFromBag
	case 1, BattleFrontier_BattlePyramidLobby_EventScript_PickItemsFromParty
	case 2, BattleFrontier_BattlePyramidLobby_EventScript_ExitPickItems
	case MULTI_B_PRESSED, BattleFrontier_BattlePyramidLobby_EventScript_ExitPickItems
	end

BattleFrontier_BattlePyramidLobby_EventScript_PickItemsFromBag::
	special ChooseItemsToTossFromPyramidBag
	message BattleFrontier_BattlePyramidLobby_Text_PickItemsToKeep
	waitmessage
	goto BattleFrontier_BattlePyramidLobby_EventScript_PickItemsToKeep
	end

BattleFrontier_BattlePyramidLobby_EventScript_PickItemsFromParty::
	special BattlePyramidChooseMonHeldItems
	message BattleFrontier_BattlePyramidLobby_Text_PickItemsToKeep
	waitmessage
	goto BattleFrontier_BattlePyramidLobby_EventScript_PickItemsToKeep
	end

BattleFrontier_BattlePyramidLobby_EventScript_ExitPickItems::
	special DoBattlePyramidMonsHaveHeldItem
	goto_if_eq VAR_RESULT, TRUE, BattleFrontier_BattlePyramidLobby_EventScript_PartyStillHasHeldItems
	return

BattleFrontier_BattlePyramidLobby_EventScript_PartyStillHasHeldItems::
	msgbox BattleFrontier_BattlePyramidLobby_Text_LeastOneMonHoldingItem, MSGBOX_DEFAULT
	message BattleFrontier_BattlePyramidLobby_Text_PickItemsToKeep
	waitmessage
	goto BattleFrontier_BattlePyramidLobby_EventScript_PickItemsToKeep
	end

BattleFrontier_BattlePyramidLobby_EventScript_RulesBoard::
	lockall
	msgbox BattleFrontier_BattlePyramidLobby_Text_RulesAreListed, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattlePyramidLobby_EventScript_ReadRulesBoard::
	message BattleFrontier_BattlePyramidLobby_Text_ReadWhichHeading
	waitmessage
	multichoice 15, 2, MULTI_BATTLE_PYRAMID_RULES, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_BattlePyramidLobby_EventScript_RulesPokemon
	case 1, BattleFrontier_BattlePyramidLobby_EventScript_RulesTrainers
	case 2, BattleFrontier_BattlePyramidLobby_EventScript_RulesMaze
	case 3, BattleFrontier_BattlePyramidLobby_EventScript_RulesBag
	case 4, BattleFrontier_BattlePyramidLobby_EventScript_ExitRules
	case MULTI_B_PRESSED, BattleFrontier_BattlePyramidLobby_EventScript_ExitRules
	end

BattleFrontier_BattlePyramidLobby_EventScript_RulesPokemon::
	msgbox BattleFrontier_BattlePyramidLobby_Text_ExplainMonRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattlePyramidLobby_EventScript_RulesTrainers::
	msgbox BattleFrontier_BattlePyramidLobby_Text_ExplainTrainerRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattlePyramidLobby_EventScript_RulesMaze::
	msgbox BattleFrontier_BattlePyramidLobby_Text_ExplainMazeRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattlePyramidLobby_EventScript_RulesBag::
	msgbox BattleFrontier_BattlePyramidLobby_Text_ExplainBagRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattlePyramidLobby_EventScript_ExitRules::
	releaseall
	end

BattleFrontier_BattlePyramidLobby_Text_WelcomeToBattlePyramid:
	.string "トレーナーの ゆうきを ためす ばしょ!\nバトルピラミッドへ ようこそ!\pこちらでは バトルアドベンチャーの\nごあんないを しておりますぞ!$"

BattleFrontier_BattlePyramidLobby_Text_EmbarkOnChallenge:
	.string "バトルアドベンチャーに\nちょうせん されますかな?$"

BattleFrontier_BattlePyramidLobby_Text_AwaitFutureChallenge:
	.string "またの ちょうせんを\nおまちして おりますぞ!$"

BattleFrontier_BattlePyramidLobby_Text_ExplainBattlePyramid:
	.string "バトルアドベンチャーは\nピラミッドの なかへ はいり\lちょうじょう せいはを めざす\lぼうけん イベント ですぞ!\pピラミッドの なかは\n7だんの めいろで わかれていて\lうでに おぼえの ある トレーナーと\lやせい ポケモンが まって いますぞ!\pどうぐを もたせて いない\n3ひきの ポケモンと\lおわたしする バトルバッグを もって\lちょうじょうを めざして いただきますぞ!\pこの バトルバッグに いれた どうぐは\nぼうけんに しっぱいすると\lなくなって しまいますので\lおきをつけ くだされ!\pまた ぼうけんを ちゅうだん するときは\n「やすむ」を えらんで おわらせないと\lぼうけんは ちゅうしと なってしまいます!\lじゅうぶん おきをつけ くだされ!$"

BattleFrontier_BattlePyramidLobby_Text_WhichLevelMode:
	.string "ピラミッドには\nレベル50と オープンレベルと ありますが\lどちらに はいられますかな?$"

BattleFrontier_BattlePyramidLobby_Text_SelectThreeMons:
	.string "では つれていく ポケモンを\n3ひき おえらび くだされ!$"

BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsLvOpen:
	.string "ぼうけんしゃよ!!\pつれていける ポケモンが\n3ひき いないようですぞ!\pまた ポケモンに もたせている どうぐは\nすべて はずしてくだされ!\pなお タマゴ{STR_VAR_1}さんか させる ことは できませんぞ!\pじゅんびが できましたら\nまた おこし くだされ!$"

BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsLv50:
	.string "ぼうけんしゃよ!!\pつれていける ポケモンが\n3ひき いないようですぞ!\pつれていく ためには レベルが 50いかで\nすべて ちがう しゅるいの ポケモンを\l3ひき よういして くだされ!\pまた ポケモンに もたせている どうぐは\nすべて はずしてくだされ!\pなお タマゴ{STR_VAR_1}さんか させる ことは できませんぞ!\pじゅんびが できましたら\nまた おこし くだされ!$"

BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsEnd:
	.string "じゅんびが できましたら\nまた おこし くだされ!$"

BattleFrontier_BattlePyramidLobby_Text_OkayToSaveBeforeChallenge:
	.string "バトルピラミッドへ はいる まえに\nレポートを かきますが よろしいですかな?$"

BattleFrontier_BattlePyramidLobby_Text_ShowYouIntoPyramid:
	.string "では バトルピラミッドの なかへ\nおつれいたしますぞ!$"

BattleFrontier_BattlePyramidLobby_Text_WeWillHoldBagForSafekeeping:
	.string "{PLAYER}さんの もっている バッグは\nこちらで あずからせて いただきますぞ!$"

BattleFrontier_BattlePyramidLobby_Text_PleaseTakePreviousBattleBag:
	.string "かわりに ぜんかい おつかいになった\nこの バトルバッグを おもちくだされ!$"

BattleFrontier_BattlePyramidLobby_Text_PleaseTakeThisBattleBag:
	.string "かわりに こちらの\nバトルバッグを おもちくだされ!$"

BattleFrontier_BattlePyramidLobby_Text_ExchangedBagForBattleBag:
	.string "{PLAYER}は バッグを あずけて\nバトルバッグを うけとった!$"

BattleFrontier_BattlePyramidLobby_Text_StepOnFloorPanel:
	.string "こちらにある ゆかを ふんで いただくと\nピラミッドの うえの だんへと\lすすむことが できますぞ!\pぶじ ぼうけんが すすむことを\nいのって おりますぞ!$"

BattleFrontier_BattlePyramidLobby_Text_DidntSaveBeforeQuittingTakeBag:
	.string "ぼうけんしゃよ!\pぜんかい レポートを かいて\nしゅうりょう しませんでしたな?\pざんねんですが この ばあい\nちょうせんは むこうと なりますぞ!\pおあずかり していた\nバッグは おかえし いたしますぞ!\p{PLAYER}は\nバッグを うけとった!$"

BattleFrontier_BattlePyramidLobby_Text_YouveConqueredPyramid:
	.string "おかえりなさいませ!\pみごとに バトルピラミッドを\nせいは されましたな!$"

BattleFrontier_BattlePyramidLobby_Text_MonHoldingItemCannotTake:
	.string "おおっ!\pポケモンに\nどうぐを もたせていますな……\pもうしわけ ありませんが\nピラミッドで てにいれた どうぐは\lもっていくことが できませんぞ!$"

BattleFrontier_BattlePyramidLobby_Text_HeldItemsMovedToBag:
	.string "もたせている どうぐは\nすべて {PLAYER}さんの バトルバッグに\lいれておきますぞ!$"

BattleFrontier_BattlePyramidLobby_Text_BagCannotHoldPickItemsToKeep:
	.string "バトルバッグには\nすべて はいらない ようなので\lバトルバッグと もたせたどうぐを\lせいりして くだされ!$"

BattleFrontier_BattlePyramidLobby_Text_LeastOneMonHoldingItem:
	.string "ポケモンに\nまだ どうぐを もたせていますぞ!$"

BattleFrontier_BattlePyramidLobby_Text_PickItemsToKeep:
	.string "バトルバッグと もたせたどうぐを\nせいりして くだされ!$"

@ Unused
BattleFrontier_BattlePyramidLobby_Text_ReturnedEverythingMonsHeld:
	.string "{JPN}{PLAYER}は ポケモンに もたせている\nどうぐを かえした!$"

BattleFrontier_BattlePyramidLobby_Text_UsedBattleBagWillBeKept:
	.string "おつかいになられた バトルバッグは\nじかいの ちょうせんまで\lおあずかり いたしますぞ!\p{PLAYER}は バトルバッグを あずけて\nバッグを うけとった!$"

BattleFrontier_BattlePyramidLobby_Text_RecordResultsWait:
	.string "せいせきを レポートに かきますので\nおまちくだされ!$"

@ Unused
BattleFrontier_BattlePyramidLobby_Text_ForConqueringPyramidTakeThis:
	.string "{JPN}バトルピラミッド せいはの きねんとして\nこの しょうひんを おうけとり くだされ!$"

@ Unused
BattleFrontier_BattlePyramidLobby_Text_ReceivedPrizeItem:
	.string "{JPN}{PLAYER}は\n{STR_VAR_1}を もらった!$"

@ Unused
BattleFrontier_BattlePyramidLobby_Text_BagIsFull:
	.string "{JPN}……ほう!\nどうぐが いっぱいの ようですな\pどうぐを せいりしてから\nまた おこしくだされ!$"

BattleFrontier_BattlePyramidLobby_Text_DisappointingHereIsBag:
	.string "ざんねんでしたな……\pおあずかりしていた\nバッグは おかえし いたしますぞ!\p{PLAYER}は\nバッグを うけとった!$"

BattleFrontier_BattlePyramidLobby_Text_LookForwardToNextChallenge:
	.string "またの ちょうせんを\nおまちして おりますぞ!$"

@ Unused
BattleFrontier_BattlePyramidLobby_Text_HereIsPrize:
	.string "{JPN}おまちしておりましたぞ!\pバトルピラミッド せいはの きねんとして\nこの しょうひんを おうけとり くだされ!$"

BattleFrontier_BattlePyramidLobby_Text_TellYouWhatMisfortunesAwait:
	.string "ようこそ……\pピラミッドで おこる わざわいを\nうらなって さしあげましょう……$"

BattleFrontier_BattlePyramidLobby_Text_Aah:
	.string "………………………………………………………\n………………………………………………………\p……っああっ!$"

BattleFrontier_BattlePyramidLobby_Text_HintParalysis:
	.string "でんきが とびちっています……\p……そして\nまひにより おもうように うごけない\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintPoison:
	.string "どくが みえます……\p……そして\nどくに おかされ くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintBurn:
	.string "まっかな ほのおが みえます……\p……そして\nやけどを おって くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintPPWaste:
	.string "すさまじい プレッシャーと おんねん……\nうらみの ちからが みえます……\pそして……\nパワーポイントが なくなり\lわるあがきを する\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintLevitate:
	.string "ゆるやかに ふゆうする\nポケモンたちが みえます……\p……そして\nじめんの わざを つかえずに くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintTrapAbility:
	.string "あしもとから わきおこる\nつよい ちからが みえます……\p……そして\nにげようとしても にげられない\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintIce:
	.string "こおりタイプの\nポケモンたちが みえます……\p……そして\nこおりの わざに くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintExplosion:
	.string "いのちを かけて\nくりだされる わざが みえます……\p……そして\nわざを うけて たおれていく\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintPsychic:
	.string "エスパータイプの\nポケモンたちが みえます……\p……そして\nエスパーの わざに くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintRock:
	.string "いわタイプの\nポケモンたちが みえます……\p……そして\nいわの わざに くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintFighting:
	.string "かくとうタイプの\nポケモンたちが みえます……\p……そして\nかくとうの わざに くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintWeather:
	.string "あまごい…… にほんばれ……\nすなあらし…… あられ……\pてんこうの へんかで つよくなる\nポケモンたちが みえます\p……そして\nさまざまな タイプに くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintBug:
	.string "むしタイプの\nポケモンたちが みえます……\p……そして\nさまざまな こうげきで くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintDark:
	.string "あく タイプの\nポケモンたちが みえます……\p……そして\nあくの わざに くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintWater:
	.string "みずタイプの\nポケモンたちが みえます……\p……そして\nみずの わざに くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintGhost:
	.string "ゴーストタイプの\nポケモンたちが みえます……\p……そして\nゴーストの わざに くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintSteel:
	.string "はがねタイプの\nポケモンたちが みえます……\p……そして\nきょうだいな わざに くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintFlyingDragon:
	.string "そらをとぶ\nポケモンたちが みえます……\p……そして\nきょうだいな わざに くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintStoneEvolve:
	.string "いしの ちからにより\nしんかした ものたちが みえます……\p……そして\nほのお…… みず…… でんき……\p3つの ちからに くるしむ\nあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_HintNormal:
	.string "ノーマルタイプの\nポケモンたちが みえます……\p……そして\nきょうだいな わざに くるしむ\lあなたの ポケモンの すがたも……$"

BattleFrontier_BattlePyramidLobby_Text_BelieveMyFortunesOrNot:
	.string "うらないを しんじるも しんじないも\nあなた しだい……\pみらいは いつでも かえられます……\nどうぞ ごぶじで……$"

BattleFrontier_BattlePyramidLobby_Text_YouveDefeatedPyramidKing:
	.string "おかえりなさいませ!\pピラミッドキングを たおし\nみごと バトルピラミッドを\lせいは されましたな!$"

BattleFrontier_BattlePyramidLobby_Text_GiveYouTheseBattlePoints:
	.string "ぼうけんしゃよ!\nあなたの あつい ゆうきを たたえ\lバトルポイントを おおくり しましょう!$"

BattleFrontier_BattlePyramidLobby_Text_RulesAreListed:
	.string "バトルアドベンチャーの\nルールが かかれている!$"

BattleFrontier_BattlePyramidLobby_Text_ReadWhichHeading:
	.string "どの せつめいを よみますか?$"

BattleFrontier_BattlePyramidLobby_Text_ExplainMonRules:
	.string "ピラミッドの なかで でてくる\nやせい ポケモンは\lピラミッドを せいはすると かわります\pどんな ポケモンが でてくるのかは\nじっさいに ぼうけんして おぼえましょう$"

BattleFrontier_BattlePyramidLobby_Text_ExplainTrainerRules:
	.string "ピラミッドの なかには\nトレーナーが まちかまえて います\p1つの だんには さいこうで\n8にんの トレーナーが います\pまた トレーナーに かつと\nちょっと ぼうけんに やくだつ\lヒントが もらえます$"

BattleFrontier_BattlePyramidLobby_Text_ExplainMazeRules:
	.string "ピラミッドの めいろは\nはいる たびに かわります\pなかは とても くらく なっているので\nしんちょうに すすみましょう\pまた やせいポケモンや トレーナーとの\nたたかいに かつと\lちょっとだけ まわりが あかるくなります$"

BattleFrontier_BattlePyramidLobby_Text_ExplainBagRules:
	.string "バトルバッグは ピラミッドに いる あいだ\nつかうことが できます\pレベル50と オープンレベルの\nバッグが それぞれ ようい されていて\l10しゅるいの どうぐを\l99こ まで いれることが できます\pまた バッグの なかみは ぼうけんに\nしっぱい すると なくなって しまいます$"
