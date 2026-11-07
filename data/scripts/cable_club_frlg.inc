CableClub_OnTransition_Frlg::
	call CableClub_EventScript_HideOrShowMysteryGiftMan_Frlg
	end

CableClub_EventScript_HideOrShowMysteryGiftMan_Frlg::
	specialvar VAR_RESULT, ValidateSavedWonderCard
	goto_if_eq VAR_RESULT, FALSE, EventScript_HideMysteryGiftMan_Frlg
	clearflag FLAG_HIDE_MG_DELIVERYMEN
	return

EventScript_HideMysteryGiftMan_Frlg::
	setflag FLAG_HIDE_MG_DELIVERYMEN
	return

CableClub_EventScript_MysteryGiftMan_Frlg::
	trywondercardscript

@ Unused
EventScript_MysteryGiftThankYou_Frlg::
	msgbox gText_ThankYouForAccessingMysteryGift, MSGBOX_NPC
	end

CableClub_OnWarp_Frlg::
	map_script_2 VAR_CABLE_CLUB_STATE, USING_SINGLE_BATTLE, EventScript_CheckTurnAttendant_Frlg
	map_script_2 VAR_CABLE_CLUB_STATE, USING_DOUBLE_BATTLE, EventScript_CheckTurnAttendant_Frlg
	map_script_2 VAR_CABLE_CLUB_STATE, USING_MULTI_BATTLE, EventScript_CheckTurnAttendant_Frlg
	map_script_2 VAR_CABLE_CLUB_STATE, USING_TRADE_CENTER, EventScript_CheckTurnAttendant_Frlg
	map_script_2 VAR_CABLE_CLUB_STATE, USING_UNION_ROOM, EventScript_CheckTurnAttendant_Frlg
	map_script_2 VAR_CABLE_CLUB_STATE, USING_BERRY_CRUSH, EventScript_CheckTurnAttendant_Frlg
	map_script_2 VAR_CABLE_CLUB_STATE, USING_MINIGAME, EventScript_CheckTurnAttendant_Frlg
	.2byte 0

EventScript_CheckTurnAttendant_Frlg::
	goto_if_eq VAR_0x8007, 0, EventScript_CheckTurnAttendantEnd
	turnobject VAR_0x8007, DIR_WEST
EventScript_CheckTurnAttendantEnd:
	end

CableClub_OnLoad_Frlg::
	goto_if_eq VAR_CABLE_CLUB_STATE, USING_SINGLE_BATTLE, EventScript_OnLoadFromColosseum_Frlg
	goto_if_eq VAR_CABLE_CLUB_STATE, USING_DOUBLE_BATTLE, EventScript_OnLoadFromColosseum_Frlg
	goto_if_eq VAR_CABLE_CLUB_STATE, USING_MULTI_BATTLE, EventScript_OnLoadFromColosseum_Frlg
	goto_if_eq VAR_CABLE_CLUB_STATE, USING_TRADE_CENTER, EventScript_OnLoadFromTradeCenter_Frlg
	goto_if_eq VAR_CABLE_CLUB_STATE, USING_UNION_ROOM, EventScript_OnLoadFromUnionRoom_Frlg
	goto_if_eq VAR_CABLE_CLUB_STATE, USING_BERRY_CRUSH, EventScript_OnLoadFromBerryCrush_Frlg
	goto_if_eq VAR_CABLE_CLUB_STATE, USING_MINIGAME, EventScript_OnLoadFromGameCorner_Frlg
	end

EventScript_OnLoadFromColosseum_Frlg::
	call CableClub_EventScript_OpenDirectCornerBarrier_Frlg
	end

EventScript_OnLoadFromTradeCenter_Frlg::
	call CableClub_EventScript_OpenDirectCornerBarrier_Frlg
	end

EventScript_OnLoadFromUnionRoom_Frlg::
	call CableClub_EventScript_OpenUnionRoomBarrier_Frlg
	end

EventScript_OnLoadFromBerryCrush_Frlg::
	call CableClub_EventScript_OpenDirectCornerBarrier_Frlg
	end

EventScript_OnLoadFromGameCorner_Frlg::
	call CableClub_EventScript_OpenGameCornerBarrier_Frlg
	end

CableClub_OnFrame_Frlg::
	map_script_2 VAR_MAP_SCENE_POKEMON_CENTER_TEALA, 1, CableClub_EventScript_Tutorial_Frlg
	map_script_2 VAR_CABLE_CLUB_STATE, USING_SINGLE_BATTLE, CableClub_EventScript_ExitLinkRoom_Frlg
	map_script_2 VAR_CABLE_CLUB_STATE, USING_DOUBLE_BATTLE, CableClub_EventScript_ExitLinkRoom_Frlg
	map_script_2 VAR_CABLE_CLUB_STATE, USING_MULTI_BATTLE, CableClub_EventScript_ExitLinkRoom_Frlg
	map_script_2 VAR_CABLE_CLUB_STATE, USING_TRADE_CENTER, CableClub_EventScript_ExitTradeCenter_Frlg
	map_script_2 VAR_CABLE_CLUB_STATE, USING_UNION_ROOM, CableClub_EventScript_ExitUnionRoom_Frlg
	map_script_2 VAR_CABLE_CLUB_STATE, USING_BERRY_CRUSH, CableClub_EventScript_ExitLinkRoom_Frlg
	map_script_2 VAR_CABLE_CLUB_STATE, USING_MINIGAME, CableClub_EventScript_ExitMinigameRoom_Frlg
	.2byte 0

CableClub_EventScript_ExitLinkRoom_Frlg::
	lockall
	call CableClub_EventScript_CloseLinkAndExitLinkRoom_Frlg
	call CableClub_EventScript_CloseDirectCornerBarrier_Frlg
	special DrawWholeMapView
	playse SE_CLICK
	erasebox 0, 0, 29, 19
	releaseall
	end

CableClub_EventScript_ExitMinigameRoom_Frlg::
	lockall
	call CableClub_EventScript_CloseLinkAndExitLinkRoom_Frlg
	call CableClub_EventScript_CloseGameCornerBarrier_Frlg
	special DrawWholeMapView
	playse SE_CLICK
	erasebox 0, 0, 29, 19
	releaseall
	end

CableClub_EventScript_CloseLinkAndExitLinkRoom_Frlg::
	setvar VAR_CABLE_CLUB_STATE, 0
	textcolor NPC_TEXT_COLOR_FEMALE
	goto_if_eq VAR_0x8007, 0, CableClub_EventScript_PlayerExitLinkRoom_Frlg
	applymovement VAR_0x8007, Movement_AttendantFaceLeft
	waitmovement 0
	applymovement LOCALID_PLAYER, Movement_PlayerExitLinkRoom
	waitmovement 0
	applymovement VAR_0x8007, Movement_AttendantFaceDown
	waitmovement 0
	return

CableClub_EventScript_ExitTradeCenter_Frlg::
	lockall
	call CableClub_EventScript_PlayerExitTradeCenter_Frlg
	call CableClub_EventScript_CloseDirectCornerBarrier_Frlg
	special DrawWholeMapView
	playse SE_CLICK
	erasebox 0, 0, 29, 19
	releaseall
	end

CableClub_EventScript_PlayerExitTradeCenter_Frlg::
	special CloseLink
	setvar VAR_CABLE_CLUB_STATE, 0
	textcolor NPC_TEXT_COLOR_FEMALE
	goto_if_eq VAR_0x8007, 0, CableClub_EventScript_PlayerExitLinkRoom_Frlg
	applymovement LOCALID_PLAYER, Movement_PlayerFaceAttendantRight
	waitmovement 0
	applymovement VAR_0x8007, Movement_AttendantFaceLeft
	waitmovement 0
	call CableClub_EventScript_TrainerCardDataOverwritten_Frlg
	return

CableClub_EventScript_ExitUnionRoom_Frlg::
	lockall
	call CableClub_EventScript_PlayerExitUnionRoom_Frlg
	call CableClub_EventScript_CloseUnionRoomBarrier_Frlg
	special DrawWholeMapView
	playse SE_CLICK
	erasebox 0, 0, 29, 19
	releaseall
	end

CableClub_EventScript_PlayerExitUnionRoom_Frlg::
	setvar VAR_CABLE_CLUB_STATE, 0
	textcolor NPC_TEXT_COLOR_FEMALE
	goto_if_eq VAR_0x8007, 0, CableClub_EventScript_PlayerExitLinkRoom_Frlg
	applymovement LOCALID_PLAYER, Movement_PlayerFaceAttendantRight
	waitmovement 0
	applymovement VAR_0x8007, Movement_AttendantFaceLeft
	waitmovement 0
	call CableClub_EventScript_TrainerCardDataOverwritten_Frlg
	return

CableClub_EventScript_TrainerCardDataOverwritten_Frlg::
	message CableClub_Text_TrainerCardDataOverwritten_Frlg
	waitmessage
	playse SE_PIN
	message CableClub_Text_HopeToSeeYouAgain_Frlg
	waitmessage
	applymovement LOCALID_PLAYER, Movement_PlayerExitLinkRoom
	waitmovement 0
	applymovement VAR_0x8007, Movement_AttendantFaceDown
	waitmovement 0
	return

CableClub_EventScript_PlayerExitLinkRoom_Frlg::
	applymovement LOCALID_PLAYER, Movement_PlayerExitLinkRoom
	waitmovement 0
	return

CableClub_EventScript_Tutorial_Frlg::
	lockall
	textcolor NPC_TEXT_COLOR_FEMALE
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox CableClub_Text_FirstTimeRightThisWay_Frlg
	closemessage
	applymovement LOCALID_PLAYER, Movement_PlayerApproachCounter
	waitmovement 0
	delay 30
	msgbox CableClub_Text_ExplainWirelessClubFirstTime_Frlg
	setvar VAR_MAP_SCENE_POKEMON_CENTER_TEALA, 2
	releaseall
	end

Movement_PlayerApproachCounter:
	walk_up
	walk_up
	step_end

CableClub_EventScript_WelcomeToCableClub_Frlg::
	message CableClub_Text_WelcomeWhichCableClubService_Frlg
	waitmessage
	delay 15
	goto CableClub_EventScript_SelectCableClubRoom_Frlg
	end

CableClub_EventScript_UnusedWelcomeToCableClub_Frlg::
	msgbox CableClub_Text_WhichService_Frlg
	goto CableClub_EventScript_SelectCableClubRoom_Frlg
	end

CableClub_EventScript_SelectCableClubRoom_Frlg::
	setvar VAR_0x8004, 0
	multichoice 0, 0, MULTI_CABLE_CLUB_NO_RECORD_MIX, FALSE
	switch VAR_RESULT
	case 0, CableClub_EventScript_TradeCenter_Frlg
	case 1, CableClub_EventScript_Colosseum_Frlg
	case 2, CableClub_EventScript_AbortLink_Frlg
	case MULTI_B_PRESSED, CableClub_EventScript_AbortLink_Frlg
	end

CableClub_EventScript_Colosseum_Frlg::
	copyvar VAR_0x8007, VAR_LAST_TALKED
	goto CableClub_EventScript_SelectBattleMode_Frlg
	end

CableClub_EventScript_SelectBattleMode_Frlg::
	message CableClub_Text_PlayWhichBattleMode_Frlg
	waitmessage
	multichoice 0, 0, MULTI_BATTLE_MODE, FALSE
	switch VAR_RESULT
	case 0, CableClub_EventScript_SingleBattleMode_Frlg
	case 1, CableClub_EventScript_DoubleBattleMode_Frlg
	case 2, CableClub_EventScript_MultiBattleMode_Frlg
	case 3, CableClub_EventScript_BattleModeInfo_Frlg
	case 4, CableClub_EventScript_AbortLink_Frlg
	case MULTI_B_PRESSED, CableClub_EventScript_AbortLink_Frlg
	end

CableClub_EventScript_BattleModeInfo_Frlg::
	msgbox CableClub_Text_ExplainBattleModes_Frlg
	goto CableClub_EventScript_SelectBattleMode_Frlg
	end

CableClub_EventScript_SingleBattleMode_Frlg::
	setvar VAR_0x8004, USING_SINGLE_BATTLE
	goto CableClub_EventScript_TryEnterColosseum_Frlg
	end

CableClub_EventScript_DoubleBattleMode_Frlg::
	special HasEnoughMonsForDoubleBattle
	goto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, CableClub_EventScript_NeedTwoMonsForDoubleBattle_Frlg
	setvar VAR_0x8004, USING_DOUBLE_BATTLE
	goto CableClub_EventScript_TryEnterColosseum_Frlg
	end

CableClub_EventScript_NeedTwoMonsForDoubleBattle_Frlg::
	msgbox CableClub_Text_NeedTwoMonsForDoubleBattle_Frlg
	goto CableClub_EventScript_SelectBattleMode_Frlg
	end

CableClub_EventScript_MultiBattleMode_Frlg::
	setvar VAR_0x8004, USING_MULTI_BATTLE
	goto CableClub_EventScript_TryEnterColosseum_Frlg
	end

CableClub_EventScript_TryEnterColosseum_Frlg::
	call Common_EventScript_SaveGame
	goto_if_eq VAR_RESULT, 0, CableClub_EventScript_AbortLink_Frlg
	message CableClub_Text_PleaseWaitBCancel_Frlg
	waitmessage
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special TryBattleLinkup
	call EventScript_RestorePrevTextColor
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, CableClub_EventScript_EnterColosseum_Frlg
	goto_if_eq VAR_RESULT, LINKUP_SOMEONE_NOT_READY, CableClub_EventScript_AbortLinkSomeoneNotReady_Frlg
	goto_if_eq VAR_RESULT, LINKUP_DIFF_SELECTIONS, CableClub_EventScript_AbortLinkDifferentSelections_Frlg
	goto_if_eq VAR_RESULT, LINKUP_WRONG_NUM_PLAYERS, CableClub_EventScript_AbortLinkIncorrectNumberOfBattlers_Frlg
	goto_if_eq VAR_RESULT, LINKUP_FAILED, CableClub_EventScript_AbortLink_Frlg
	goto_if_eq VAR_RESULT, LINKUP_CONNECTION_ERROR, CableClub_EventScript_AbortLinkConnectionError_Frlg
	end

CableClub_EventScript_EnterColosseum_Frlg::
	special HealPlayerParty
	special SavePlayerParty
	special LoadPlayerBag
	copyvar VAR_CABLE_CLUB_STATE, VAR_0x8004
	messageautoscroll CableClub_Text_PleaseEnter_Frlg
	waitmessage
	call CableClub_EventScript_OpenDirectCornerBarrier_Frlg
	special DrawWholeMapView
	playse SE_CLICK
	delay 60
	applymovement VAR_LAST_TALKED, Movement_AttendantFaceLeft
	waitmovement 0
	closemessage
	applymovement LOCALID_PLAYER, Movement_PlayerApproachLinkRoomLeft
	waitmovement 0
	opendoor 9, 1
	waitdooranim
	applymovement LOCALID_PLAYER, Movement_PlayerEnterLinkRoom
	waitmovement 0
	hideplayer
	closedoor 9, 1
	waitdooranim
	release
	goto_if_eq VAR_0x8004, USING_MULTI_BATTLE, CableClub_EventScript_WarpTo4PColosseum_Frlg
	special SetCableClubWarp
	warp MAP_BATTLE_COLOSSEUM_2P_FRLG, 6, 8
	special DoCableClubWarp
	end

@ Unused
CableClub_EventScript_PlayerApproachLinkRoomRight_Frlg::
	applymovement LOCALID_PLAYER, Movement_PlayerApproachLinkRoomRight
	waitmovement 0
	return

CableClub_EventScript_WarpTo4PColosseum_Frlg::
	special SetCableClubWarp
	warp MAP_BATTLE_COLOSSEUM_4P_FRLG, 5, 8
	special DoCableClubWarp
	end

CableClub_EventScript_AbortLinkIncorrectNumberOfBattlers_Frlg::
	switch VAR_0x8004
	case USING_SINGLE_BATTLE, CableClub_EventScript_AbortLinkWrongNumberForSingleBattle_Frlg
	case USING_DOUBLE_BATTLE, CableClub_EventScript_AbortLinkWrongNumberForDoubleBattle_Frlg
	case USING_MULTI_BATTLE, CableClub_EventScript_AbortLinkNeedFourPlayers_Frlg
	goto CableClub_EventScript_AbortLinkIncorrectNumberOfParticipants_Frlg
	end

CableClub_EventScript_AbortLinkNeedFourPlayers_Frlg::
	special CloseLink
	msgbox CableClub_Text_NeedFourPlayers_Frlg
	goto CableClub_EventScript_ConfirmNumberAndRestart_Frlg
	end

CableClub_EventScript_AbortLinkWrongNumberForDoubleBattle_Frlg::
	special CloseLink
	msgbox CableClub_Text_CantDoubleBattleWithXPlayers_Frlg
	goto CableClub_EventScript_ConfirmNumberAndRestart_Frlg
	end

CableClub_EventScript_AbortLinkWrongNumberForSingleBattle_Frlg::
	special CloseLink
	msgbox CableClub_Text_CantSingleBattleWithXPlayers_Frlg
	goto CableClub_EventScript_ConfirmNumberAndRestart_Frlg
	end

CableClub_EventScript_ConfirmNumberAndRestart_Frlg::
	special CloseLink  @ Redundant
	msgbox CableClub_Text_PleaseConfirmNumberAndRestart_Frlg
	release
	end

CableClub_EventScript_TradeCenter_Frlg::
	copyvar VAR_0x8007, VAR_LAST_TALKED
	call CableClub_EventScript_CheckPartyTradeRequirements_Frlg
	goto_if_eq VAR_RESULT, 0, CableClub_EventScript_AbortLink_Frlg
	call Common_EventScript_SaveGame
	goto_if_eq VAR_RESULT, 0, CableClub_EventScript_AbortLink_Frlg
	message CableClub_Text_PleaseWaitBCancel_Frlg
	waitmessage
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special TryTradeLinkup
	call EventScript_RestorePrevTextColor
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, CableClub_EventScript_EnterTradeCenter_Frlg
	goto_if_eq VAR_RESULT, LINKUP_SOMEONE_NOT_READY, CableClub_EventScript_AbortLinkSomeoneNotReady_Frlg
	goto_if_eq VAR_RESULT, LINKUP_DIFF_SELECTIONS, CableClub_EventScript_AbortLinkDifferentSelections_Frlg
	goto_if_eq VAR_RESULT, LINKUP_WRONG_NUM_PLAYERS, CableClub_EventScript_AbortLinkIncorrectNumberOfParticipants_Frlg
	goto_if_eq VAR_RESULT, LINKUP_FAILED, CableClub_EventScript_AbortLink_Frlg
	goto_if_eq VAR_RESULT, LINKUP_CONNECTION_ERROR, CableClub_EventScript_AbortLinkConnectionError_Frlg
	goto_if_eq VAR_RESULT, LINKUP_PLAYER_NOT_READY, CableClub_EventScript_AbortLinkPlayerNotReady_Frlg
	goto_if_eq VAR_RESULT, LINKUP_PARTNER_NOT_READY, CableClub_EventScript_AbortLinkOtherTrainerNotReady_Frlg
	end

CableClub_EventScript_EnterTradeCenter_Frlg::
	setvar VAR_0x8004, USING_TRADE_CENTER
	copyvar VAR_CABLE_CLUB_STATE, VAR_0x8004
	messageautoscroll CableClub_Text_PleaseEnter_Frlg
	waitmessage
	call CableClub_EventScript_OpenDirectCornerBarrier_Frlg
	special DrawWholeMapView
	playse SE_CLICK
	delay 60
	applymovement VAR_LAST_TALKED, Movement_AttendantFaceLeft
	waitmovement 0
	closemessage
	applymovement LOCALID_PLAYER, Movement_PlayerApproachLinkRoomLeft
	waitmovement 0
	opendoor 9, 1
	waitdooranim
	applymovement LOCALID_PLAYER, Movement_PlayerEnterLinkRoom
	waitmovement 0
	hideplayer
	closedoor 9, 1
	waitdooranim
	release
	special SetCableClubWarp
	setwarp MAP_TRADE_CENTER_FRLG, 5, 8
	special DoCableClubWarp
	end

CableClub_EventScript_CheckPartyTradeRequirements_Frlg::
	specialvar VAR_RESULT, CalculatePlayerPartyCount
	goto_if_lt VAR_RESULT, 2, CableClub_EventScript_NeedTwoMonsToTrade_Frlg
	specialvar VAR_RESULT, DoesPartyHaveEnigmaBerry
	goto_if_eq VAR_RESULT, TRUE, CableClub_EventScript_CantTradeEnigmaBerry_Frlg
	setvar VAR_RESULT, TRUE
	return

CableClub_EventScript_NeedTwoMonsToTrade_Frlg::
	msgbox CableClub_Text_NeedTwoMonsToTrade_Frlg
	setvar VAR_RESULT, FALSE
	return

CableClub_EventScript_CantTradeEnigmaBerry_Frlg::
	msgbox CableClub_Text_CantTradeEnigmaBerry_Frlg
	setvar VAR_RESULT, FALSE
	return

@ Record Corner is nopped in FRLG
CableClub_EventScript_RecordCorner_Frlg::
	end

CableClub_EventScript_AbortLinkPlayerNotReady_Frlg::
	special CloseLink
	msgbox CableClub_Text_NotSetUpForFarAwayRegion_Frlg
	release
	end

CableClub_EventScript_AbortLinkOtherTrainerNotReady_Frlg::
	special CloseLink
	msgbox CableClub_Text_OtherTrainerNotReady_Frlg
	release
	end

CableClub_EventScript_AbortLinkConnectionError_Frlg::
	special CloseLink
	msgbox CableClub_Text_LinkErrorPleaseReset_Frlg
	release
	end

CableClub_EventScript_AbortLinkSomeoneNotReady_Frlg::
	special CloseLink
	msgbox CableClub_Text_SomeoneIsNotReadyToLink_Frlg
	release
	end

CableClub_EventScript_AbortLinkDifferentSelections_Frlg::
	special CloseLink
	msgbox CableClub_Text_PlayersMadeDifferentSelections_Frlg
	release
	end

CableClub_EventScript_AbortLink_Frlg::
	special CloseLink
	msgbox CableClub_Text_PleaseVisitAgain_Frlg
	release
	end

CableClub_EventScript_AbortMinigame_Frlg::
	special CloseLink
	msgbox CableClub_Text_ComeAgain_Frlg
	release
	end

@ Unused
CableClub_EventScript_CableClubWarp_Frlg::
	special SetCableClubWarp
	special DoCableClubWarp
	end

CableClub_EventScript_AbortLinkIncorrectNumberOfParticipants_Frlg::
	special CloseLink
	msgbox CableClub_Text_IncorrectNumberOfParticipants_Frlg
	release
	end

CableClub_EventScript_AbortLinkPlayerHasBadEgg_Frlg::
	special CloseLink
	msgbox CableClub_Text_YouHaveAMonThatCantBeTaken_Frlg
	release
	end

CableClub_EventScript_WirelessClubAdjustements_Frlg::
	msgbox Text_WirelessClubUndergoingAdjustments
	release
	end

CableClub_EventScript_NotReadyYet_Frlg::
	msgbox Text_AppearsToBeUndergoingAdjustments
	releaseall
	end

Movement_AttendantFaceDown_Frlg::
	face_down
	step_end

@ Unused
Movement_AttendantFaceRight_Frlg::
	face_right
	step_end

Movement_AttendantFaceLeft_Frlg::
	face_left
	step_end

Movement_PlayerExitLinkRoom_Frlg::
	walk_down
	walk_down
	step_end

Movement_PlayerApproachLinkRoomRight_Frlg::
	walk_right
	walk_up
	walk_up
	step_end

Movement_PlayerApproachLinkRoomLeft_Frlg::
	walk_left
	walk_up
	walk_up
	step_end

Movement_PlayerEnterLinkRoom_Frlg::
	walk_up
	step_end

@ Unused
Movement_PlayerFaceAttendantLeft_Frlg::
	face_left
	step_end

Movement_PlayerFaceAttendantRight_Frlg::
	face_right
	step_end

Movement_PlayerEnterMinigameRoom_Frlg::
	walk_left
	walk_up
	walk_up
	walk_up
	step_end

CableClub_EventScript_ShowBattleRecords_Frlg::
	lockall
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, 0
	special ShowTrainerHillRecords
	releaseall
	end

BattleColosseum_2P_EventScript_PlayerSpot0::
	setvar VAR_0x8005, 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special ColosseumPlayerSpotTriggered
	end

BattleColosseum_2P_EventScript_PlayerSpot1::
	setvar VAR_0x8005, 1
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special ColosseumPlayerSpotTriggered
	end

BattleColosseum_4P_EventScript_PlayerSpot0::
	fadescreen FADE_TO_BLACK
	special ChooseHalfPartyForBattle
	goto_if_eq VAR_RESULT, 0, BattleColosseum_4P_EventScript_CancelSpotTrigger_Frlg
	setvar VAR_0x8005, 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special ColosseumPlayerSpotTriggered
	end

BattleColosseum_4P_EventScript_PlayerSpot1::
	fadescreen FADE_TO_BLACK
	special ChooseHalfPartyForBattle
	goto_if_eq VAR_RESULT, 0, BattleColosseum_4P_EventScript_CancelSpotTrigger_Frlg
	setvar VAR_0x8005, 1
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special ColosseumPlayerSpotTriggered
	end

BattleColosseum_4P_EventScript_PlayerSpot2::
	fadescreen FADE_TO_BLACK
	special ChooseHalfPartyForBattle
	goto_if_eq VAR_RESULT, 0, BattleColosseum_4P_EventScript_CancelSpotTrigger_Frlg
	setvar VAR_0x8005, 2
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special ColosseumPlayerSpotTriggered
	end

BattleColosseum_4P_EventScript_PlayerSpot3::
	fadescreen FADE_TO_BLACK
	special ChooseHalfPartyForBattle
	goto_if_eq VAR_RESULT, 0, BattleColosseum_4P_EventScript_CancelSpotTrigger_Frlg
	setvar VAR_0x8005, 3
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special ColosseumPlayerSpotTriggered
	end

BattleColosseum_4P_EventScript_CancelSpotTrigger_Frlg::
	end

TradeCenter_EventScript_Chair0::
	setvar VAR_0x8005, 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special PlayerEnteredTradeSeat
	end

TradeCenter_EventScript_Chair1::
	setvar VAR_0x8005, 1
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special PlayerEnteredTradeSeat
	end

@ Unused
TradeCenter_EventScript_Chair2_Frlg::
	setvar VAR_0x8005, 2
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special PlayerEnteredTradeSeat
	end

@ Unused
TradeCenter_EventScript_Chair3_Frlg::
	setvar VAR_0x8005, 3
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special PlayerEnteredTradeSeat
	end

@ Nop in FRLG
@ Separate labels to match GetDirectionForEventScript
RecordCorner_EventScript_Spot0::
RecordCorner_EventScript_Spot1::
RecordCorner_EventScript_Spot2::
RecordCorner_EventScript_Spot3::
	end

CableClub_EventScript_ReadTrainerCard_Frlg::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Text_LookedAtPlayersTrainerCard_Frlg
	fadescreen FADE_TO_BLACK
	special Script_ShowLinkTrainerCard
	end

CableClub_EventScript_ReadTrainerCardColored_Frlg::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Text_LookedAtPlayersTrainerCardColored_Frlg
	fadescreen FADE_TO_BLACK
	special Script_ShowLinkTrainerCard
	end

CableClub_EventScript_TooBusyToNotice_Frlg::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Text_TrainerTooBusyToNotice_Frlg
	closemessage
	end

BattleColosseum_2P_EventScript_Attendant_Frlg::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special Script_FacePlayer
	msgbox Text_TakeSeatStartBattle_Frlg
	special Script_ClearHeldMovement
	closemessage
	end

TradeCenter_EventScript_Attendant_Frlg::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special Script_FacePlayer
	msgbox Text_TakeSeatStartTrade_Frlg
	special Script_ClearHeldMovement
	closemessage
	end

@ Nop in FRLG
RecordCorner_EventScript_Attendant_Frlg::
	end

TradeCenter_ConfirmLeaveRoom_Frlg::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Text_TerminateLinkIfYouLeaveRoom_Frlg, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, TradeCenter_TerminateLink_Frlg
	erasebox 0, 0, 29, 19
	releaseall
	end

TradeCenter_TerminateLink_Frlg::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	messageautoscroll Text_TerminateLinkConfirmation
	waitmessage
	special ExitLinkRoom
	end

CableClub_EventScript_DoLinkRoomExit_Frlg::
	special CleanupLinkRoomState
	special ReturnFromLinkRoom
	end

CableClub_EventScript_UnionRoomAttendant_Frlg::
	lock
	faceplayer
	goto_if_unset FLAG_SYS_POKEDEX_GET, CableClub_EventScript_WirelessClubAdjustements_Frlg
	specialvar VAR_RESULT, IsBadEggInParty
	goto_if_eq VAR_RESULT, TRUE, CableClub_EventScript_AbortLinkPlayerHasBadEgg_Frlg
	copyvar VAR_0x8007, VAR_LAST_TALKED
	specialvar VAR_RESULT, IsWirelessAdapterConnected
	goto_if_eq VAR_RESULT, FALSE, CableClub_EventScript_UnionRoomAdapterNotConnected_Frlg
	message CableClub_Text_WelcomeUnionRoomEnter_Frlg
	waitmessage
	goto CableClub_EventScript_AskEnterUnionRoom_Frlg
	end

CableClub_EventScript_AskEnterUnionRoom_Frlg::
	multichoice 18, 6, MULTI_YESNOINFO, FALSE
	switch VAR_RESULT
	case 0, CableClub_EventScript_EnterUnionRoom_Frlg
	case 1, CableClub_EventScript_AbortLink_Frlg
	case 2, CableClub_EventScript_UnionRoomInfo_Frlg
	case MULTI_B_PRESSED, CableClub_EventScript_AbortLink_Frlg
	end

CableClub_EventScript_UnionRoomInfo_Frlg::
	message CableClub_Text_UnionRoomInfo_Frlg
	waitmessage
	goto CableClub_EventScript_AskEnterUnionRoom_Frlg
	end

CableClub_EventScript_EnterUnionRoom_Frlg::
	call CableClub_EventScript_CheckPartyUnionRoomRequirements_Frlg
	goto_if_eq VAR_RESULT, FALSE, CableClub_EventScript_AbortLink_Frlg
	call Common_EventScript_SaveGame
	goto_if_eq VAR_RESULT, 0, CableClub_EventScript_AbortLink_Frlg
	msgbox CableClub_Text_EnjoyUnionRoom_Frlg
	closemessage
	special HealPlayerParty
	setvar VAR_0x8004, USING_UNION_ROOM
	copyvar VAR_CABLE_CLUB_STATE, VAR_0x8004
	call CableClub_EventScript_OpenUnionRoomBarrier_Frlg
	special DrawWholeMapView
	playse SE_CLICK
	delay 60
	applymovement VAR_LAST_TALKED, Movement_AttendantFaceLeft
	waitmovement 0
	applymovement LOCALID_PLAYER, Movement_PlayerApproachLinkRoomLeft
	waitmovement 0
	opendoor 5, 1
	waitdooranim
	applymovement LOCALID_PLAYER, Movement_PlayerEnterLinkRoom
	waitmovement 0
	hideplayer
	closedoor 5, 1
	waitdooranim
	special Script_ResetUnionRoomTrade
	special SetCableClubWarp
	warpspinenter MAP_UNION_ROOM_FRLG, 7, 11
	waitstate
	special RunUnionRoom
	waitstate
	end

CableClub_EventScript_CheckPartyUnionRoomRequirements_Frlg::
	specialvar VAR_RESULT, CountPartyNonEggMons
	goto_if_lt VAR_RESULT, 2, CableClub_EventScript_NeedTwoMonsForUnionRoom_Frlg
	specialvar VAR_RESULT, DoesPartyHaveEnigmaBerry
	goto_if_eq VAR_RESULT, TRUE, CableClub_EventScript_NoEnigmaBerryInUnionRoom_Frlg
	setvar VAR_RESULT, TRUE
	return

CableClub_EventScript_NeedTwoMonsForUnionRoom_Frlg::
	msgbox CableClub_Text_NeedTwoMonsForUnionRoom_Frlg
	goto EventScript_SetResultFalse
	end

CableClub_EventScript_NoEnigmaBerryInUnionRoom_Frlg::
	msgbox CableClub_Text_NoEnigmaBerryInUnionRoom_Frlg
	goto EventScript_SetResultFalse
	end

CableClub_EventScript_UnionRoomAdapterNotConnected_Frlg::
	msgbox CableClub_Text_UnionRoomAdapterNotConnected_Frlg
	release
	return

CableClub_EventScript_WirelessClubAttendant_Frlg::
	lock
	faceplayer
	goto_if_unset FLAG_SYS_POKEDEX_GET, CableClub_EventScript_WirelessClubAdjustements_Frlg
	msgbox CableClub_Text_AskAboutLinking_Frlg, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CableClub_EventScript_DontAskAboutLinking_Frlg
	msgbox CableClub_Text_ExplainWirelessClub_Frlg
	release
	return

CableClub_EventScript_DontAskAboutLinking_Frlg::
	msgbox CableClub_Text_HopeYouEnjoyWirelessSystem_Frlg
	release
	return

CableClub_EventScript_DirectCornerAttendant_Frlg::
	lock
	faceplayer
	goto_if_unset FLAG_SYS_POKEDEX_GET, CableClub_EventScript_WirelessClubAdjustements_Frlg
	specialvar VAR_RESULT, IsBadEggInParty
	goto_if_eq VAR_RESULT, TRUE, CableClub_EventScript_AbortLinkPlayerHasBadEgg_Frlg
	specialvar VAR_RESULT, IsWirelessAdapterConnected
	goto_if_eq VAR_RESULT, FALSE, CableClub_EventScript_WelcomeToCableClub_Frlg
	message CableClub_Text_WelcomeWhichDirectCornerRoom_Frlg
	waitmessage
	delay 15
	goto CableClub_EventScript_DirectCornerSelectService_Frlg
	end

CableClub_EventScript_DirectCornerSelectService_Frlg::
	goto_if_unset FLAG_GOT_POWDER_JAR, CableClub_EventScript_DirectCornerNoBerry_Frlg
	multichoice 0, 0, MULTI_WIRELESS_NO_RECORD, FALSE
	switch VAR_RESULT
	case 0, CableClub_EventScript_WirelessTrade_Frlg
	case 1, CableClub_EventScript_WirelessBattleSelect_Frlg
	case 2, CableClub_EventScript_WirelessBerryCrush_Frlg
	case 3, CableClub_EventScript_AbortLink_Frlg
	case MULTI_B_PRESSED, CableClub_EventScript_AbortLink_Frlg
	end

CableClub_EventScript_DirectCornerNoBerry_Frlg::
	multichoice 0, 0, MULTI_WIRELESS_NO_RECORD_BERRY, FALSE
	switch VAR_RESULT
	case 0, CableClub_EventScript_WirelessTrade_Frlg
	case 1, CableClub_EventScript_WirelessBattleSelect_Frlg
	case 2, CableClub_EventScript_AbortLink_Frlg
	case MULTI_B_PRESSED, CableClub_EventScript_AbortLink_Frlg
	end

CableClub_EventScript_WirelessTrade_Frlg::
	msgbox CableClub_Text_TradePokemon_Frlg, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CableClub_EventScript_AbortLink_Frlg
	call CableClub_EventScript_CheckPartyTradeRequirements_Frlg
	goto_if_eq VAR_RESULT, FALSE, CableClub_EventScript_AbortLink_Frlg
	setvar VAR_0x8004, LINK_GROUP_TRADE
	goto CableClub_EventScript_SaveAndChooseLinkLeader_Frlg
	end

CableClub_EventScript_WirelessBattleSelect_Frlg::
	message CableClub_Text_PlayWhichBattleMode_Frlg
	waitmessage
	multichoice 0, 0, MULTI_BATTLE_MODE, FALSE
	switch VAR_RESULT
	case 0, CableClub_EventScript_WirelessSingleBattle_Frlg
	case 1, CableClub_EventScript_WirelessDoubleBattle_Frlg
	case 2, CableClub_EventScript_WirelessMultiBattle_Frlg
	case 3, CableClub_EventScript_WirelessBattleInfo_Frlg
	case 4, CableClub_EventScript_AbortLink_Frlg
	case MULTI_B_PRESSED, CableClub_EventScript_AbortLink_Frlg
	end

CableClub_EventScript_WirelessSingleBattle_Frlg::
	setvar VAR_0x8004, LINK_GROUP_SINGLE_BATTLE
	goto CableClub_EventScript_SaveAndChooseLinkLeader_Frlg
	end

CableClub_EventScript_WirelessDoubleBattle_Frlg::
	special HasEnoughMonsForDoubleBattle
	goto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, CableClub_EventScript_TwoMonsNeededForWirelessDoubleBattle_Frlg
	setvar VAR_0x8004, LINK_GROUP_DOUBLE_BATTLE
	goto CableClub_EventScript_SaveAndChooseLinkLeader_Frlg
	end

CableClub_EventScript_TwoMonsNeededForWirelessDoubleBattle_Frlg::
	msgbox CableClub_Text_NeedTwoMonsForDoubleBattle_Frlg
	goto CableClub_EventScript_WirelessBattleSelect_Frlg
	end

CableClub_EventScript_WirelessMultiBattle_Frlg::
	setvar VAR_0x8004, LINK_GROUP_MULTI_BATTLE
	goto CableClub_EventScript_SaveAndChooseLinkLeader_Frlg
	end

CableClub_EventScript_WirelessBattleInfo_Frlg::
	msgbox CableClub_Text_ExplainBattleModes_Frlg
	goto CableClub_EventScript_WirelessBattleSelect_Frlg
	end

CableClub_EventScript_WirelessBerryCrush_Frlg::
	msgbox CableClub_Text_UseBerryCrush_Frlg, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CableClub_EventScript_AbortLink_Frlg
	special HasAtLeastOneBerry
	goto_if_eq VAR_RESULT, FALSE, CableClub_EventScript_NeedBerryForBerryCrush_Frlg
	setvar VAR_0x8004, LINK_GROUP_BERRY_CRUSH
	goto CableClub_EventScript_SaveAndChooseLinkLeader_Frlg
	end

CableClub_EventScript_NeedBerryForBerryCrush_Frlg::
	msgbox CableClub_Text_NeedBerryForBerryCrush_Frlg
	goto CableClub_EventScript_DirectCornerSelectService_Frlg
	end

CableClub_EventScript_SaveAndChooseLinkLeader_Frlg::
	call Common_EventScript_SaveGame
	goto_if_eq VAR_RESULT, 0, CableClub_EventScript_AbortLink_Frlg
	switch VAR_0x8004
	case LINK_GROUP_TRADE, CableClub_EventScript_ChooseLinkLeaderFrom2_Frlg
	case LINK_GROUP_SINGLE_BATTLE, CableClub_EventScript_ChooseLinkLeaderFrom2_Frlg
	case LINK_GROUP_DOUBLE_BATTLE, CableClub_EventScript_ChooseLinkLeaderFrom2_Frlg
	case LINK_GROUP_MULTI_BATTLE, CableClub_EventScript_ChooseLinkLeaderFrom4_Frlg
	case LINK_GROUP_BERRY_CRUSH, CableClub_EventScript_ChooseLinkLeader_Frlg
	end

CableClub_EventScript_ChooseLinkLeaderFrom2_Frlg::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message CableClub_Text_ChooseGroupLeaderOfTwo_Frlg
	waitmessage
	call EventScript_RestorePrevTextColor
	multichoice 13, 6, MULTI_LINK_LEADER, FALSE
	switch VAR_RESULT
	case 0, CableClub_EventScript_TryJoinGroup2Players_Frlg
	case 1, CableClub_EventScript_TryLeadGroup2Players_Frlg
	case 2, CableClub_EventScript_AbortLink_Frlg
	case MULTI_B_PRESSED, CableClub_EventScript_AbortLink_Frlg
	end

CableClub_EventScript_TryLeadGroup2Players_Frlg::
	call CableClub_EventScript_TryBecomeLinkLeader_Frlg
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, CableClub_EventScript_EnterWirelessLinkRoom_Frlg
	goto_if_eq VAR_RESULT, LINKUP_FAILED, CableClub_EventScript_ChooseLinkLeaderFrom2_Frlg
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, CableClub_EventScript_TryLeadGroup2Players_Frlg
	release
	return

CableClub_EventScript_TryJoinGroup2Players_Frlg::
	call CableClub_EventScript_TryJoinLinkGroup_Frlg
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, CableClub_EventScript_EnterWirelessLinkRoom_Frlg
	goto_if_eq VAR_RESULT, LINKUP_FAILED, CableClub_EventScript_ChooseLinkLeaderFrom2_Frlg
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, CableClub_EventScript_TryJoinGroup2Players_Frlg
	release
	return

CableClub_EventScript_ChooseLinkLeaderFrom4_Frlg::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message CableClub_Text_ChooseGroupLeaderOfFour_Frlg
	waitmessage
	call EventScript_RestorePrevTextColor
	multichoice 13, 6, MULTI_LINK_LEADER, FALSE
	switch VAR_RESULT
	case 0, CableClub_EventScript_TryJoinGroup4Players_Frlg
	case 1, CableClub_EventScript_TryLeadGroup4Players_Frlg
	case 2, CableClub_EventScript_AbortLink_Frlg
	case MULTI_B_PRESSED, CableClub_EventScript_AbortLink_Frlg
	end

CableClub_EventScript_TryLeadGroup4Players_Frlg::
	call CableClub_EventScript_TryBecomeLinkLeader_Frlg
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, CableClub_EventScript_EnterWirelessLinkRoom_Frlg
	goto_if_eq VAR_RESULT, LINKUP_FAILED, CableClub_EventScript_ChooseLinkLeaderFrom4_Frlg
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, CableClub_EventScript_TryLeadGroup4Players_Frlg
	release
	return

CableClub_EventScript_TryJoinGroup4Players_Frlg::
	call CableClub_EventScript_TryJoinLinkGroup_Frlg
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, CableClub_EventScript_EnterWirelessLinkRoom_Frlg
	goto_if_eq VAR_RESULT, LINKUP_FAILED, CableClub_EventScript_ChooseLinkLeaderFrom4_Frlg
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, CableClub_EventScript_TryJoinGroup4Players_Frlg
	release
	return

CableClub_EventScript_ChooseLinkLeader_Frlg::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message CableClub_Text_ChooseGroupLeader_Frlg
	waitmessage
	call EventScript_RestorePrevTextColor
	multichoice 13, 6, MULTI_LINK_LEADER, FALSE
	switch VAR_RESULT
	case 0, CableClub_EventScript_TryJoinGroupXPlayers_Frlg
	case 1, CableClub_EventScript_TryLeadGroupXPlayers_Frlg
	case 2, CableClub_EventScript_AbortLink_Frlg
	case MULTI_B_PRESSED, CableClub_EventScript_AbortLink_Frlg
	end

CableClub_EventScript_TryLeadGroupXPlayers_Frlg::
	call CableClub_EventScript_TryBecomeLinkLeader_Frlg
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, CableClub_EventScript_EnterWirelessLinkRoom_Frlg
	goto_if_eq VAR_RESULT, LINKUP_FAILED, CableClub_EventScript_ChooseLinkLeader_Frlg
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, CableClub_EventScript_TryLeadGroupXPlayers_Frlg
	release
	return

CableClub_EventScript_TryJoinGroupXPlayers_Frlg::
	call CableClub_EventScript_TryJoinLinkGroup_Frlg
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, CableClub_EventScript_EnterWirelessLinkRoom_Frlg
	goto_if_eq VAR_RESULT, LINKUP_FAILED, CableClub_EventScript_ChooseLinkLeader_Frlg
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, CableClub_EventScript_TryJoinGroupXPlayers_Frlg
	release
	return

CableClub_EventScript_TryBecomeLinkLeader_Frlg::
	special TryBecomeLinkLeader
	return

CableClub_EventScript_TryJoinLinkGroup_Frlg::
	special TryJoinLinkGroup
	return

CableClub_EventScript_EnterWirelessLinkRoom_Frlg::
	messageautoscroll CableClub_Text_DirectYouToYourRoom_Frlg
	waitmessage
	delay 60
	closemessage
	copyvar VAR_0x8007, VAR_LAST_TALKED
	call CableClub_EventScript_OpenDirectCornerBarrier_Frlg
	special DrawWholeMapView
	playse SE_CLICK
	delay 60
	applymovement VAR_LAST_TALKED, Movement_AttendantFaceLeft
	waitmovement 0
	closemessage
	applymovement LOCALID_PLAYER, Movement_PlayerApproachLinkRoomLeft
	waitmovement 0
	opendoor 9, 1
	waitdooranim
	applymovement LOCALID_PLAYER, Movement_PlayerEnterLinkRoom
	waitmovement 0
	hideplayer
	closedoor 9, 1
	waitdooranim
	release
	waitstate
	end

CableClub_EventScript_ShowWirelessCommunicationScreen_Frlg::
	lockall
	goto_if_unset FLAG_SYS_POKEDEX_GET, CableClub_EventScript_NotReadyYet_Frlg
	specialvar VAR_RESULT, IsWirelessAdapterConnected
	goto_if_eq VAR_RESULT, FALSE, CableClub_EventScript_AdapterNotConnected_Frlg
	fadescreen FADE_TO_BLACK
	special ShowWirelessCommunicationScreen
	msgbox CableClub_Text_ParticipantsStepUpToCounter_Frlg
	releaseall
	end

CableClub_EventScript_AdapterNotConnected_Frlg::
	msgbox CableClub_Text_AdapterNotConnected_Frlg
	releaseall
	end

CableClub_EventScript_OpenUnionRoomBarrier_Frlg::
	setmetatile 5, 3, METATILE_PokemonCenterFrlg_Floor_ShadeLeft, 0
	return

CableClub_EventScript_CloseUnionRoomBarrier_Frlg::
	setmetatile 5, 3, METATILE_PokemonCenterFrlg_CounterBarrier, 1
	return

CableClub_EventScript_OpenDirectCornerBarrier_Frlg::
	setmetatile 9, 3, METATILE_PokemonCenterFrlg_Floor_ShadeLeft, 0
	return

CableClub_EventScript_CloseDirectCornerBarrier_Frlg::
	setmetatile 9, 3, METATILE_PokemonCenterFrlg_CounterBarrier, 1
	return

CableClub_EventScript_OpenGameCornerBarrier_Frlg::
	setmetatile 5, 3, METATILE_GameCorner_CheckeredFloor_ShadeLeft, 0
	return

CableClub_EventScript_CloseGameCornerBarrier_Frlg::
	setmetatile 5, 3, METATILE_GameCorner_CounterBarrier, 1
	return

CableClub_OnResume_Frlg::
	special InitUnionRoom
	end

JoyfulGameCorner_EventScript_InfoMan2_Frlg::
	lock
	faceplayer
	message Text_DescribeWhichGame_Frlg
	waitmessage
	multichoice 0, 0, MULTI_WIRELESS_MINIGAME, FALSE
	switch VAR_RESULT
	case 0, CableClub_EventScript_PokemonJumpInfo_Frlg
	case 1, CableClub_EventScript_DodrioBerryPickingInfo_Frlg
	case 2, CableClub_EventScript_MinigameInfoExit_Frlg
	case MULTI_B_PRESSED, CableClub_EventScript_MinigameInfoExit_Frlg
	end

CableClub_EventScript_PokemonJumpInfo_Frlg::
	msgbox Text_PokemonJumpInfo_Frlg
	release
	end

CableClub_EventScript_DodrioBerryPickingInfo_Frlg::
	msgbox Text_DodrioBerryPickingInfo_Frlg
	release
	end

CableClub_EventScript_MinigameInfoExit_Frlg::
	msgbox Text_TalkToManToPlay_Frlg
	release
	end

JoyfulGameCorner_EventScript_MinigameAttendant_Frlg::
	lock
	faceplayer
	message Text_WelcomeCanYouWait_Frlg
	waitmessage
	specialvar VAR_RESULT, IsWirelessAdapterConnected
	goto_if_eq VAR_RESULT, FALSE, CableClub_EventScript_AdapterNotConnectedMinigame_Frlg
	delay 60
	message Text_PlayWhichGame_Frlg
	waitmessage
	multichoice 0, 0, MULTI_WIRELESS_MINIGAME, FALSE
	switch VAR_RESULT
	case 0, CableClub_EventScript_PlayPokemonJump_Frlg
	case 1, CableClub_EventScript_PlayDodrioBerryPicking_Frlg
	case 2, CableClub_EventScript_AbortMinigame_Frlg
	case MULTI_B_PRESSED, CableClub_EventScript_AbortMinigame_Frlg
	end

CableClub_EventScript_PlayPokemonJump_Frlg::
	setvar VAR_0x8005, 0
	special IsPokemonJumpSpeciesInParty
	goto_if_eq VAR_RESULT, FALSE, CableClub_EventScript_NoEligiblePkmn_Frlg
	msgbox Text_EnterWhichPokemon_Frlg
	setvar VAR_0x8005, 0
	special ChooseMonForWirelessMinigame
	goto_if_ge VAR_0x8004, PARTY_SIZE, CableClub_EventScript_AbortMinigame_Frlg
	call Common_EventScript_SaveGame
	goto_if_eq VAR_RESULT, 0, CableClub_EventScript_AbortMinigame_Frlg
	setvar VAR_0x8004, LINK_GROUP_POKEMON_JUMP
	goto CableClub_EventScript_ChooseLinkLeaderMinigame_Frlg
	end

CableClub_EventScript_PlayDodrioBerryPicking_Frlg::
	setvar VAR_0x8005, 1
	special IsDodrioInParty
	goto_if_eq VAR_RESULT, 0, CableClub_EventScript_NoEligiblePkmn_Frlg
	msgbox Text_EnterWhichPokemon_Frlg
	setvar VAR_0x8005, 1
	special ChooseMonForWirelessMinigame
	goto_if_ge VAR_0x8004, PARTY_SIZE, CableClub_EventScript_AbortMinigame_Frlg
	call Common_EventScript_SaveGame
	goto_if_eq VAR_RESULT, 0, CableClub_EventScript_AbortMinigame_Frlg
	setvar VAR_0x8004, LINK_GROUP_BERRY_PICKING
	goto CableClub_EventScript_ChooseLinkLeaderMinigame_Frlg
	end

CableClub_EventScript_ChooseLinkLeaderMinigame_Frlg::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message CableClub_Text_ChooseGroupLeader_Frlg
	waitmessage
	call EventScript_RestorePrevTextColor
	multichoice 13, 6, MULTI_LINK_LEADER, FALSE
	switch VAR_RESULT
	case 0, CableClub_EventScript_TryJoinMinigameLinkGroup_Frlg
	case 1, CableClub_EventScript_TryBecomeMinigameLinkLeader_Frlg
	case 2, CableClub_EventScript_AbortMinigame_Frlg
	case MULTI_B_PRESSED, CableClub_EventScript_AbortMinigame_Frlg
	end

CableClub_EventScript_TryBecomeMinigameLinkLeader_Frlg::
	call CableClub_EventScript_TryBecomeLinkLeader_Frlg
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, CableClub_EventScript_EnterMinigame_Frlg
	goto_if_eq VAR_RESULT, LINKUP_FAILED, CableClub_EventScript_ChooseLinkLeaderMinigame_Frlg
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, CableClub_EventScript_TryBecomeMinigameLinkLeader_Frlg
	release
	return

CableClub_EventScript_TryJoinMinigameLinkGroup_Frlg::
	call CableClub_EventScript_TryJoinLinkGroup_Frlg
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, CableClub_EventScript_EnterMinigame_Frlg
	goto_if_eq VAR_RESULT, LINKUP_FAILED, CableClub_EventScript_ChooseLinkLeaderMinigame_Frlg
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, CableClub_EventScript_TryJoinMinigameLinkGroup_Frlg
	release
	return

CableClub_EventScript_EnterMinigame_Frlg::
	messageautoscroll Text_AllGoodToGo_Frlg
	waitmessage
	delay 120
	closemessage
	copyvar VAR_0x8007, VAR_LAST_TALKED
	call CableClub_EventScript_OpenGameCornerBarrier_Frlg
	special DrawWholeMapView
	playse SE_CLICK
	delay 60
	applymovement VAR_LAST_TALKED, Movement_AttendantFaceLeft
	waitmovement 0
	closemessage
	applymovement LOCALID_PLAYER, Movement_PlayerEnterMinigameRoom
	waitmovement 0
	hideplayer
	release
	waitstate
	end

CableClub_EventScript_AdapterNotConnectedMinigame_Frlg::
	msgbox Text_AdapterNotConnectedMinigame_Frlg
	release
	end

CableClub_EventScript_NoEligiblePkmn_Frlg::
	msgbox EventScript_ExplainPokemonJumpRequirements_Frlg, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CableClub_EventScript_AbortMinigame_Frlg
	call_if_eq VAR_0x8005, 0, CableClub_EventScript_ExplainPokemonJumpRequirements_Frlg
	call_if_eq VAR_0x8005, 1, CableClub_EventScript_ExplainDodrioBerryPickingRequirements_Frlg
	goto CableClub_EventScript_AbortMinigame_Frlg
	end

CableClub_EventScript_ExplainPokemonJumpRequirements_Frlg::
	msgbox Text_ShortJumpingPokemonAllowed_Frlg
	return

CableClub_EventScript_ExplainDodrioBerryPickingRequirements_Frlg::
	msgbox Text_OnlyDodrioAllowed_Frlg
	return

TwoIsland_JoyfulGameCorner_EventScript_ShowPokemonJumpRecords::
	lockall
	special ShowPokemonJumpRecords
	releaseall
	end

TwoIsland_JoyfulGameCorner_EventScript_ShowDodrioBerryPickingRecords::
	lockall
	special ShowDodrioBerryPickingRecords
	releaseall
	end

CableClub_Text_WelcomeWhichCableClubService_Frlg::
	.string "{JPN}つうしん ケーブル クラブに\n"
	.string "ようこそ!\p"
	.string "どちらを ごりよう ですか?$"

CableClub_Text_WhichService_Frlg::
	.string "{JPN}どちらを ごりよう ですか?$"

CableClub_Text_TradeMonsUsingLinkCable_Frlg::
	.string "{JPN}ケーブルで つないだ おともだちと\n"
	.string "ポケモン こうかんが できます$"

CableClub_Text_BattleUsingLinkCable_Frlg::
	.string "{JPN}ケーブルで つないだ おともだちと\n"
	.string "ポケモン たいせんが できます$"

@ Unused, translated in Emerald
CableClub_Text_CloseThisMenu_Frlg::
	.string "おわります$"

CableClub_Text_NeedTwoMonsForDoubleBattle_Frlg::
	.string "{JPN}ダブルバトルでは 2ひき いじょうの\n"
	.string "ポケモンが ひつようです$"

CableClub_Text_NeedTwoMonsToTrade_Frlg::
	.string "{JPN}こうかんでは 2ひき いじょうの\n"
	.string "てもちが ひつようです$"

CableClub_Text_CantTradeEnigmaBerry_Frlg::
	.string "{JPN}{STR_VAR_1}のみを もっている\n"
	.string "ポケモンは こうかん できません!$"

CableClub_Text_PleaseWaitBCancel_Frlg::
	.string "{JPN}しょうしょう おまち ください\n"
	.string "……Bボタンで キャンセル$"

CableClub_Text_WhenAllPlayersReadyAConfirmBCancel_Frlg::
	.string "{JPN}そろったら Aボタンで すすむ\n"
	.string "……Bボタンで キャンセル$"

CableClub_Text_StartLinkWithXPlayersAConfirmBCancel_Frlg::
	.string "{JPN}{STR_VAR_1}にんで つうしんを はじめます\n"
	.string "Aボタン…けってい Bボタン…もどる$"

CableClub_Text_AwaitingLinkupBCancel_Frlg::
	.string "{JPN}つうしんかいしを まっています\n"
	.string "……Bボタンで キャンセル$"

@ Unused, translated in Emerald
CableClub_Text_OkayToSaveProgress_Frlg::
	.string "はじめる まえに レポートを\n"
	.string "かきますが よろしいですか?$"

CableClub_Text_PleaseEnter_Frlg::
	.string "{JPN}それでは へやへ どうぞ$"

CableClub_Text_DirectYouToYourRoom_Frlg::
	.string "{JPN}それでは\n"
	.string "へやに ごあんないします$"

CableClub_Text_SomeoneIsNotReadyToLink_Frlg::
	.string "{JPN}おともだちの じゅんびが\n"
	.string "できて いない ようです\p"
	.string "じゅんびが できましたら\n"
	.string "また おこしください$"

CableClub_Text_LinkErrorPleaseReset_Frlg::
	.string "{JPN}つうしんエラー です\n"
	.string "リセットして やりなおしてください$"

CableClub_Text_PlayersMadeDifferentSelections_Frlg::
	.string "{JPN}おたがいが えらんだ ものが\n"
	.string "ちがっている ようです$"

CableClub_Text_PleaseVisitAgain_Frlg::
	.string "{JPN}それでは また おこしください$"

CableClub_Text_IncorrectNumberOfParticipants_Frlg::
	.string "{JPN}さんかしている にんずうが\n"
	.string "ただしく ありません$"

CableClub_Text_CantSingleBattleWithXPlayers_Frlg::
	.string "{JPN}シングルバトルは {STR_VAR_1}にんで\n"
	.string "あそぶことは できません!$"

CableClub_Text_CantDoubleBattleWithXPlayers_Frlg::
	.string "{JPN}ダブルバトルは {STR_VAR_1}にんでは\n"
	.string "あそぶことは できません!$"

CableClub_Text_NeedFourPlayers_Frlg::
	.string "{JPN}さんか にんずうが 4にん いないと\n"
	.string "たいせん することが できません!$"

CableClub_Text_PleaseConfirmNumberAndRestart_Frlg::
	.string "{JPN}もういちど にんずうを たしかめて\n"
	.string "はじめから やりなおして ください$"

Text_TerminateLinkIfYouLeaveRoom_Frlg::
	.string "{JPN}へやからでると つうしんが\n"
	.string "おわりますが よろしいですか?$"

Text_TerminateLinkConfirmation_Frlg::
	.string "{JPN}つうしんを しゅうりょう します\n"
	.string "しばらく おまちください$"

Text_TrainerTooBusyToNotice_Frlg::
	.string "{JPN}とりこみちゅうの ようだ…$"

Text_LookedAtPlayersTrainerCard_Frlg::
	.string "{JPN}{STR_VAR_1}の トレーナーカードを\n"
	.string "みせてもらった!$"

Text_LookedAtPlayersTrainerCardColored_Frlg::
	.string "{JPN}{STR_VAR_1}の トレーナーカードを\n"
	.string "みせてもらった!\p"
	.string "…{STR_VAR_2}カードだ!$"

Text_TakeSeatStartBattle_Frlg::
	.string "{JPN}しょていの いちに ついて\n"
	.string "たいせんを はじめて ください$"

Text_TakeSeatStartTrade_Frlg::
	.string "{JPN}しょていの いちに ついて\n"
	.string "こうかんを はじめて ください$"

@ Unused, leftover from RS
RecordCorner_Text_ThanksForComing_Frlg::
	.string "ごりよう ありがとう ございました$"

CableClub_Text_TrainerCardDataOverwritten_Frlg::
	.string "{JPN}トレーナーカードの\n"
	.string "きろくを かきかえます!$"

CableClub_Text_HopeToSeeYouAgain_Frlg::
	.string "{JPN}では また どうぞ!$"

CableClub_Text_NotSetUpForFarAwayRegion_Frlg::
	.string "{JPN}おきゃくさま\n"
	.string "もうしわけ ありません\p"
	.string "とおい ちほうの かたと\n"
	.string "ポケモンを こうかん することは\l"
	.string "いまは できません…$"

CableClub_Text_OtherTrainerNotReady_Frlg::
	.string "{JPN}あいての じゅんびが\n"
	.string "できていません$"

CableClub_Text_YouHaveAMonThatCantBeTaken_Frlg::
	.string "{JPN}てもちに つれていけない\n"
	.string "ポケモンが います$"

CableClub_Text_AdapterNotConnected_Frlg::
	.string "{JPN}ワイヤレスアダプタが\n"
	.string "つながって いません$"

CableClub_Text_ParticipantsStepUpToCounter_Frlg::
	.string "{JPN}さんか したい ひとは\n"
	.string "カウンターに おこしください$"

@ Unused, translated in Emerald
CableClub_Text_Hello_Frlg::
	.string "こんにちは!$"

@ Unused, translated in Emerald
CableClub_Text_PleaseWait_Frlg::
	.string "しょうしょう おまちください$"

CableClub_Text_YouMayTradeHere_Frlg::
	.string "{JPN}ともだちと ポケモンの\n"
	.string "こうかんが たのしめます$"

CableClub_Text_YouMayBattleHere_Frlg::
	.string "{JPN}ともだちと ポケモンの\n"
	.string "たいせんが たのしめます$"

CableClub_Text_CanMakeBerryPowder_Frlg::
	.string "{JPN}2にんから 5にんの ともだちと\n"
	.string "きのみで こなを つくります$"

@ Unused, Record Mix nopped
CableClub_Text_CanMixRecords_Frlg::
	.string "ワイヤレス クラブでの\n"
	.string "あそびかたを せつめいします$"

CableClub_Text_CancelSelectedItem_Frlg::
	.string "{JPN}やめます$"

@ Unused, translated in Emerald
CableClub_Text_WhichBattleMode_Frlg::
	.string "どちらの しょうぶに しますか?$"

@ Unused, translated in Emerald
CableClub_Text_ReturnsToPreviousStep_Frlg::
	.string "ひとつ まえに もどります$"

CableClub_Text_NeedBerryForBerryCrush_Frlg::
	.string "{JPN}きのみを もって いないと\n"
	.string "きのみクラッシュは できません$"

CableClub_Text_NeedTwoMonsForUnionRoom_Frlg::
	.string "{JPN}ユニオンルームに はいるには\n"
	.string "2ひき いじょうの ポケモンを\l"
	.string "もって おこしください$"

CableClub_Text_NoEnigmaBerryInUnionRoom_Frlg::
	.string "{JPN}{STR_VAR_1}のみを もっている\n"
	.string "ポケモンを おもちの かたは\l"
	.string "ユニオンルームには はいれません…$"

CableClub_Text_UnionRoomAdapterNotConnected_Frlg::
	.string "{JPN}こちらは\n"
	.string "ポケモン ワイヤレス クラブ\l"
	.string "ユニオン ルームです\p"
	.string "ワイヤレスアダプタが\n"
	.string "つながって いません\p"
	.string "また おこしください$"

Text_PlayerIsPlayingRightNowGoForIt_Frlg::
	.string "{JPN}あら! いま {STR_VAR_1}さんも\n"
	.string "プレイちゅう みたいですね\l"
	.string "がんばって くださいね!$"

Text_DescribeWhichGame_Frlg::
	.string "{JPN}ぼくは ゲームの せつめいを\n"
	.string "しますよ!\l"
	.string "どの せつめいを ききますか?$"

Text_PokemonJumpInfo_Frlg::
	.string "{JPN}「ミニポケモンでジャンプ」は\n"
	.string "ツルの なわを Aボタンで\l"
	.string "タイミングよく とぶ ゲームです\p"
	.string "さんか できる ポケモンは\n"
	.string "たかさ 70センチくらい までの\l"
	.string "ちいさい ポケモンに なります\p"
	.string "もぐっていたり とんでいたり する\n"
	.string "ポケモンは ジャンプが\l"
	.string "じょうずに できないので\l"
	.string "ざんねんながら さんか できません\p"
	.string "みんなの ジャンプの タイミングが\n"
	.string "ピッタリ あえば\l"
	.string "よいことが ありますよ!$"

Text_DodrioBerryPickingInfo_Frlg::
	.string "{JPN}「ドードリオのきのみどり」は\n"
	.string "ドードリオの 3つの あたまに\l"
	.string "めいれいして うえから おちてくる\l"
	.string "きのみを おとさないように\l"
	.string "とりつづける ゲームです\p"
	.string "じゅうじボタンの\n"
	.string "うえ みぎ ひだりを つかいます\p"
	.string "この ゲームを あそぶには\n"
	.string "ドードリオが ひつようです$"

Text_TalkToManToPlay_Frlg::
	.string "{JPN}ゲームを するなら\n"
	.string "おとなりの おじさんに いってね!$"

Text_WelcomeCanYouWait_Frlg::
	.string "{JPN}いらっしゃい!\n"
	.string "ワイヤレスつうしんで\l"
	.string "ゲームが できるよ!\p"
	.string "ちょいと まってね$"

CableClub_Text_ComeAgain_Frlg::
	.string "{JPN}また きてくんな!$"

Text_AdapterNotConnectedMinigame_Frlg::
	.string "{JPN}ワイヤレスアダプタが\n"
	.string "つながって ないよ\p"
	.string "また きてくれ!$"

Text_PlayWhichGame_Frlg::
	.string "{JPN}さあ どの ゲームに\n"
	.string "さんか するんだい?$"

Text_EnterWhichPokemon_Frlg::
	.string "{JPN}どの ポケモンを\n"
	.string "さんか させるんだい?$"

Text_AllGoodToGo_Frlg::
	.string "{JPN}さあ じゅんびが できた!\n"
	.string "ほかの やつに まけんなよ!$"

@ Unused, translated in Emerald
Text_LeavingDoComeAgain_Frlg::
	.string "きょうは けえるのか?\n"
	.string "またこいよ!$"

EventScript_ExplainPokemonJumpRequirements_Frlg::
	.string "{JPN}ゲームに さんかできる\n"
	.string "ポケモンが いねえ みてえだな…\p"
	.string "だれが さんか できるか\n"
	.string "せつめいを きくかい?$"

Text_ShortJumpingPokemonAllowed_Frlg::
	.string "{JPN}「ミニポケモンでジャンプ」に\n"
	.string "さんか できる ポケモンは\l"
	.string "たかさ 70センチくらい までの\l"
	.string "ポケモン だよ\p"
	.string "でられねえ のは…\p"
	.string "じめんに もぐったり\n"
	.string "うかんでいる ポケモンだ!\l"
	.string "ジャンプが うまく できねえからな\p"
	.string "どうだい わかったかい$"

Text_OnlyDodrioAllowed_Frlg::
	.string "{JPN}「ドードリオのきのみどり」は\n"
	.string "なまえの とおりだ!\l"
	.string "ドードリオしか でられねえ から\l"
	.string "つれて きてくれ!$"

@ Unused, translated in Emerald
Text_RetryFromStartPlease_Frlg::
	.string "もういちど はじめから\n"
	.string "やりなおして みて くれ$"

CableClub_Text_WelcomeWhichDirectCornerRoom_Frlg::
	.string "{JPN}ポケモン ワイヤレス クラブ\n"
	.string "ダイレクト コーナーに ようこそ!\p"
	.string "こちらでは ともだちと ちょくせつ\n"
	.string "やりとり できます\p"
	.string "どの へやに はいりますか?$"

CableClub_Text_TradePokemon_Frlg::
	.string "{JPN}こうかんを しますか?$"

CableClub_Text_PlayWhichBattleMode_Frlg::
	.string "{JPN}どちらの たいせんを しますか?$"

CableClub_Text_UseBerryCrush_Frlg::
	.string "{JPN}きのみクラッシュを しますか?$"

CableClub_Text_ExplainBattleModes_Frlg::
	.string "{JPN}たいせんには 3しゅるいの\n"
	.string "モードが あります\p"
	.string "シングルバトルは たたかいの ばに\n"
	.string "ポケモンを 1ひきだして\l"
	.string "ふたりで たいせんします\p"
	.string "ダブルバトルは たたかいの ばに\n"
	.string "ポケモンを 2ひきずつ だして\l"
	.string "ふたりで たいせんします\p"
	.string "マルチバトルは たたかいの ばに\n"
	.string "4にんで ポケモンを 1ひきずつ\l"
	.string "だして 2たい2で たいせんします$"

CableClub_Text_ChooseGroupLeaderOfTwo_Frlg::
	.string "{JPN}ふたりで はなしあって ひとりが\n"
	.string "リーダーに なってください\p"
	.string "もうひとりは グループにはいる を\n"
	.string "えらんでください$"

CableClub_Text_ChooseGroupLeaderOfFour_Frlg::
	.string "{JPN}4にんで はなしあって ひとりが\n"
	.string "グループリーダーに なってください\p"
	.string "ほかの ひとは グループにはいる を\n"
	.string "えらんで ください$"

CableClub_Text_ChooseGroupLeader_Frlg::
	.string "{JPN}みんなで はなしあって ひとりが\n"
	.string "グループリーダーに なってください\p"
	.string "ほかの ひとは グループにはいる を\n"
	.string "えらんでください$"

CableClub_Text_WelcomeUnionRoomEnter_Frlg::
	.string "{JPN}ポケモン ワイヤレス クラブ\n"
	.string "ユニオン ルームに ようこそ!\p"
	.string "こちらでは みしらぬ ともだちと\n"
	.string "やりとり できます\p"
	.string "へやに はいりますか?$"

CableClub_Text_UnionRoomInfo_Frlg::
	.string "{JPN}ユニオン ルーム は あなたの\n"
	.string "ちかくで ユニオン ルームに\l"
	.string "はいっている ひとが あらわれます\p"
	.string "かんたんな あいさつをしたり\p"
	.string "レベル30までの ポケモン2ひきで\n"
	.string "1たい1の たいせん\p"
	.string "2にんから 5にんまで どうじに\n"
	.string "おしゃべりが できる チャット\p"
	.string "そして とうろくしき\n"
	.string "ポケモン こうかんが たのしめます\p"
	.string "へやに はいりますか?$"

CableClub_Text_EnjoyUnionRoom_Frlg::
	.string "{JPN}それでは ユニオン ルームで\n"
	.string "おたのしみ ください$"

CableClub_Text_FirstTimeRightThisWay_Frlg::
	.string "{JPN}こんにちは!\p"
	.string "ワイヤレス つうしんが\n"
	.string "はじめての かたですね?\p"
	.string "わたくし あんないやくの\n"
	.string "アオナと もうします\p"
	.string "ポケモンセンター 2かいの\n"
	.string "ごあんないを いたしますので\l"
	.string "こちらへ どうぞ!$"

CableClub_Text_ExplainWirelessClubFirstTime_Frlg::
	.string "{JPN}こちら 2かいには\n"
	.string "ふたつの おへやが ございます\p"
	.string "ひとつめは ひだりがわの おへや\n"
	.string "ユニオン ルーム!\p"
	.string "あなたの ちかくで\n"
	.string "ユニオン ルームに はいっている\l"
	.string "みしらぬ おともだちと\l"
	.string "いろいろな コミュニケーションが\l"
	.string "たのしめます!\p"
	.string "ふたつめは みぎがわの おへや\n"
	.string "ダイレクト コーナー!\p"
	.string "あなたの しっている おともだちと\n"
	.string "ポケモンの こうかんや\l"
	.string "たいせん などを\l"
	.string "たのしむ ことが できます!\p"
	.string "もし ワイヤレスアダプタが\n"
	.string "つながって いない ばあいでも\p"
	.string "みぎがわの おへやで\n"
	.string "ケーブル つうしんが たのしめます\p"
	.string "それでは\p"
	.string "ワイヤレス つうしんを\n"
	.string "たのしんで くださいね!$"

CableClub_Text_AskAboutLinking_Frlg::
	.string "{JPN}こんにちは! {PLAYER}さん!\p"
	.string "ポケモンセンター 2かいの\n"
	.string "あんないやく アオナです\p"
	.string "なにか つうしんで おこまりですか?$"

CableClub_Text_ExplainWirelessClub_Frlg::
	.string "{JPN}ポケモン ワイヤレス クラブでの\n"
	.string "あそびかたを せつめい します!\p"
	.string "こちら 2かいには\n"
	.string "ふたつの おへやが ございます\p"
	.string "ひとつめは ひだりがわの おへや\n"
	.string "ユニオン ルーム!\p"
	.string "あなたの ちかくで\n"
	.string "ユニオン ルームに はいっている\l"
	.string "みしらぬ おともだちと\l"
	.string "いろいろな コミュニケーションが\l"
	.string "たのしめます!\p"
	.string "ふたつめは みぎがわの おへや\n"
	.string "ダイレクト コーナー!\p"
	.string "あなたの しっている おともだちと\n"
	.string "ポケモンの こうかんや\l"
	.string "たいせん などを\l"
	.string "たのしむ ことが できます!\p"
	.string "もし ユニオン ルームや\n"
	.string "ダイレクト コーナーで\l"
	.string "おともだちが みつからない ときは\p"
	.string "もうすこし おともだちと\n"
	.string "ちかづいて みて ください\p"
	.string "また ワイヤレスアダプタが\n"
	.string "つながって いない ばあいでも\p"
	.string "みぎがわの おへやで\n"
	.string "ケーブル つうしんが たのしめます\p"
	.string "それでは\p"
	.string "ワイヤレス つうしんを\n"
	.string "たのしんで くださいね!$"

CableClub_Text_HopeYouEnjoyWirelessSystem_Frlg::
	.string "{JPN}ポケモン ワイヤレス クラブで\n"
	.string "おたのしみ ください$"

Text_WirelessClubUndergoingAdjustments::
	.string "{JPN}もうしわけ ございません\n"
	.string "ポケモン ワイヤレス クラブは\l"
	.string "ただいま ちょうせいちゅう です$"

Text_AppearsToBeUndergoingAdjustments::
	.string "{JPN}ちょうせいちゅうの ようだ$"
