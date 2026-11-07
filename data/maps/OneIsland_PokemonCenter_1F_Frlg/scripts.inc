OneIsland_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OneIsland_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	map_script MAP_SCRIPT_ON_LOAD, OneIsland_PokemonCenter_1F_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, OneIsland_PokemonCenter_1F_OnFrame
	.byte 0

OneIsland_PokemonCenter_1F_OnLoad::
	call_if_ge VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 5, OneIsland_PokemonCenter_1F_EventScript_SetRubyMetatile
	call_if_set FLAG_IS_CHAMPION, OneIsland_PokemonCenter_1F_EventScript_SetNetworkMachineOn
	end

OneIsland_PokemonCenter_1F_EventScript_SetRubyMetatile::
	setmetatile 13, 4, METATILE_PokemonCenterFrlg_NetworkMachine_Ruby, 1
	return

OneIsland_PokemonCenter_1F_EventScript_SetSapphireMetatile::
	setmetatile 16, 4, METATILE_PokemonCenterFrlg_NetworkMachine_Sapphire, 1
	return

OneIsland_PokemonCenter_1F_EventScript_SetNetworkMachineOn::
	call OneIsland_PokemonCenter_1F_EventScript_SetSapphireMetatile
	setmetatile 14, 4, METATILE_PokemonCenterFrlg_NetworkMachine_ScreenLeft_On, 1
	setmetatile 15, 4, METATILE_PokemonCenterFrlg_NetworkMachine_ScreenRight_On, 1
	return

OneIsland_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_ONE_ISLAND
	call_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 6, OneIsland_PokemonCenter_1F_EventScript_SetCelioQuestDone
	call_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 0, OneIsland_PokemonCenter_1F_EventScript_SetBillCelioFirstMeetingPos
	call_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 2, OneIsland_PokemonCenter_1F_EventScript_SetBillCelioReadyToLeavePos
	end

OneIsland_PokemonCenter_1F_EventScript_SetCelioQuestDone::
	setvar VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 7
	return

OneIsland_PokemonCenter_1F_EventScript_SetBillCelioFirstMeetingPos::
	setobjectxyperm LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, 9, 8
	setobjectmovementtype LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, MOVEMENT_TYPE_FACE_UP
	setobjectxyperm LOCALID_CELIO, 15, 6
	setobjectmovementtype LOCALID_CELIO, MOVEMENT_TYPE_FACE_UP
	return

OneIsland_PokemonCenter_1F_EventScript_SetBillCelioReadyToLeavePos::
	setobjectxyperm LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, 14, 7
	setobjectmovementtype LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, MOVEMENT_TYPE_FACE_LEFT
	setobjectxyperm LOCALID_CELIO, 14, 8
	setobjectmovementtype LOCALID_CELIO, MOVEMENT_TYPE_FACE_LEFT
	return

OneIsland_PokemonCenter_1F_OnFrame::
	map_script_2 VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 0, OneIsland_PokemonCenter_1F_EventScript_MeetCelioScene
	.2byte 0

OneIsland_PokemonCenter_1F_EventScript_MeetCelioScene::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_PLAYER, OneIsland_PokemonCenter_1F_Movement_PlayerWalkToCelio
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, OneIsland_PokemonCenter_1F_Movement_BillWalkToCelio
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_BillHeyThereCelio
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_CELIO, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_CELIO, Common_Movement_Delay48
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CelioCantBelieveYouCameOut
	msgbox OneIsland_PokemonCenter_1F_Text_BillHowsYourResearchComing
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_ThisIsMyBuddyCelio
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	call_if_set FLAG_SYS_GAME_CLEAR, OneIsland_PokemonCenter_1F_EventScript_IntroducePlayerChamp
	call_if_unset FLAG_SYS_GAME_CLEAR, OneIsland_PokemonCenter_1F_EventScript_IntroducePlayerNotChamp
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CelioThatsReallyImpressive
	msgbox OneIsland_PokemonCenter_1F_Text_BillBringMeUpToSpeed
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CelioPCsCantLinkWithYours
	closemessage
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_BillLetMeHelpYou
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CanYouDeliverThisMeteoritePlayer
	msgreceiveditem OneIsland_PokemonCenter_1F_Text_AcceptedMeteoriteFromBill, ITEM_METEORITE, 1, MUS_RG_OBTAIN_KEY_ITEM
	additem ITEM_METEORITE
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CelioPleaseTakeThis
	giveitem_msg OneIsland_PokemonCenter_1F_Text_ObtainedTriPass, ITEM_TRI_PASS, 1, MUS_RG_OBTAIN_KEY_ITEM
	msgbox OneIsland_PokemonCenter_1F_Text_PassLetsYouTravelBetweenIslands
	call_if_ge VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, OneIsland_PokemonCenter_1F_EventScript_ReceiveTownMapPage
	call_if_lt VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, OneIsland_PokemonCenter_1F_EventScript_ReceiveTownMap
	setflag FLAG_SYS_SEVII_MAP_123
	msgbox OneIsland_PokemonCenter_1F_Text_BillCatchYouLater
	closemessage
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	setflag FLAG_SYS_PC_STORAGE_DISABLED
	setvar VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 1
	releaseall
	end

OneIsland_PokemonCenter_1F_EventScript_IntroducePlayerChamp::
	msgbox OneIsland_PokemonCenter_1F_Text_PlayerIsReigningPokemonChamp
	return

OneIsland_PokemonCenter_1F_EventScript_IntroducePlayerNotChamp::
	msgbox OneIsland_PokemonCenter_1F_Text_PlayerIsRisingPokemonChamp
	return

OneIsland_PokemonCenter_1F_EventScript_ReceiveTownMapPage::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message OneIsland_PokemonCenter_1F_Text_ReceivedExtraPageForTownMap
	waitmessage
	waitfanfare
	call EventScript_RestorePrevTextColor
	return

OneIsland_PokemonCenter_1F_EventScript_ReceiveTownMap::
	setflag FLAG_HIDE_TOWN_MAP
	setvar VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2
	giveitem_msg OneIsland_PokemonCenter_1F_Text_ReceivedTownMap, ITEM_TOWN_MAP, 1, MUS_RG_OBTAIN_KEY_ITEM
	return

OneIsland_PokemonCenter_1F_Movement_PlayerWalkToCelio::
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

OneIsland_PokemonCenter_1F_Movement_BillWalkToCelio::
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_in_place_faster_right
	step_end

@ Unused
OneIsland_PokemonCenter_1F_Movement_WalkUp:
	walk_up
	step_end

@ Unused
OneIsland_PokemonCenter_1F_Movement_StepAsideLeft::
	walk_left
	walk_in_place_faster_right
	step_end

OneIsland_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

OneIsland_PokemonCenter_1F_EventScript_Bill::
	lock
	goto_if_set FLAG_SEVII_DETOUR_FINISHED, OneIsland_PokemonCenter_1F_EventScript_BillGoTakeStroll
	msgbox OneIsland_PokemonCenter_1F_Text_HmmHowAboutLikeThis
	release
	end

OneIsland_PokemonCenter_1F_EventScript_BillGoTakeStroll::
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_FacePlayer
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_GotPCWorkingStrollAWhileMore
	release
	end

OneIsland_PokemonCenter_1F_EventScript_Celio::
	lock
	faceplayer
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 7, OneIsland_PokemonCenter_1F_EventScript_CelioGiveBillFact
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 6, OneIsland_PokemonCenter_1F_EventScript_CelioJustGivenSapphire
	goto_if_set FLAG_RECOVERED_SAPPHIRE, OneIsland_PokemonCenter_1F_EventScript_GiveCelioSapphire
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 5, OneIsland_PokemonCenter_1F_EventScript_ExplainRainbowPass
	goto_if_set FLAG_GOT_RUBY, OneIsland_PokemonCenter_1F_EventScript_GiveCelioRuby
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 4, OneIsland_PokemonCenter_1F_EventScript_CelioWaitingForRuby
	specialvar VAR_RESULT, IsNationalPokedexEnabled
	goto_if_eq VAR_RESULT, TRUE, OneIsland_PokemonCenter_1F_EventScript_CelioRequestRuby
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 3, OneIsland_PokemonCenter_1F_EventScript_CelioPlayerMissingNationalDex
	msgbox OneIsland_PokemonCenter_1F_Text_SorryForBeingPoorHost
	closemessage
	applymovement LOCALID_CELIO, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

OneIsland_PokemonCenter_1F_EventScript_CelioGiveBillFact::
	msgbox OneIsland_PokemonCenter_1F_Text_CelioHearingRumorsAboutYou
	random 3
	goto_if_eq VAR_RESULT, 0, OneIsland_PokemonCenter_1F_EventScript_BillFact1
	goto_if_eq VAR_RESULT, 1, OneIsland_PokemonCenter_1F_EventScript_BillFact2
	famechecker FAMECHECKER_BILL, 5
	msgbox OneIsland_PokemonCenter_1F_Text_BillCantStomachMilk
	release
	end

OneIsland_PokemonCenter_1F_EventScript_BillFact1::
	famechecker FAMECHECKER_BILL, 3
	msgbox OneIsland_PokemonCenter_1F_Text_BillsFirstMonWasAbra
	release
	end

OneIsland_PokemonCenter_1F_EventScript_BillFact2::
	famechecker FAMECHECKER_BILL, 4
	msgbox OneIsland_PokemonCenter_1F_Text_BillsHometownInGoldenrod
	release
	end

OneIsland_PokemonCenter_1F_EventScript_CelioJustGivenSapphire::
	msgbox OneIsland_PokemonCenter_1F_Text_WishYouBestOfLuck
	release
	end

OneIsland_PokemonCenter_1F_EventScript_GiveCelioSapphire::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_TMHM
	message OneIsland_PokemonCenter_1F_Text_HandedSapphireToCelio
	waitmessage
	waitfanfare
	call EventScript_RestorePrevTextColor
	removeitem ITEM_SAPPHIRE
	msgbox OneIsland_PokemonCenter_1F_Text_ThankYouGiveMeTime
	closemessage
	applymovement LOCALID_CELIO, OneIsland_PokemonCenter_1F_Movement_CelioPutGemInMachine
	waitmovement 0
	playse SE_CLICK
	call OneIsland_PokemonCenter_1F_EventScript_SetSapphireMetatile
	special DrawWholeMapView
	msgbox OneIsland_PokemonCenter_1F_Text_OkayThisIsGood
	closemessage
	delay 60
	applymovement LOCALID_CELIO, OneIsland_PokemonCenter_1F_Movement_CelioCheckMachine
	waitmovement 0
	delay 30
	applymovement LOCALID_CELIO, OneIsland_PokemonCenter_1F_Movement_CelioCheckMachine
	waitmovement 0
	delay 40
	playse SE_PC_ON
	call OneIsland_PokemonCenter_1F_EventScript_SetNetworkMachineOn
	special DrawWholeMapView
	msgbox OneIsland_PokemonCenter_1F_Text_LinkedUpWithLanette
	applymovement LOCALID_CELIO, Common_Movement_FacePlayer
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_ManagedToLinkWithHoennThankYou
	special SetPostgameFlags
	setflag FLAG_IS_CHAMPION
	setflag FLAG_HIDE_CERULEAN_CAVE_GUARD
	setflag FLAG_HIDE_LORELEI_IN_HER_HOUSE
	setvar VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 6
	special InitRoamer
	release
	end

OneIsland_PokemonCenter_1F_EventScript_ExplainRainbowPass::
	msgbox OneIsland_PokemonCenter_1F_Text_PassLetYouGetToAllIslands
	release
	end

OneIsland_PokemonCenter_1F_EventScript_GiveCelioRuby::
	msgbox OneIsland_PokemonCenter_1F_Text_OhThats
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_TMHM
	message OneIsland_PokemonCenter_1F_Text_HandedRubyToCelio
	waitmessage
	waitfanfare
	call EventScript_RestorePrevTextColor
	closemessage
	applymovement LOCALID_CELIO, OneIsland_PokemonCenter_1F_Movement_CelioPutGemInMachine
	waitmovement 0
	playse SE_CLICK
	call OneIsland_PokemonCenter_1F_EventScript_SetRubyMetatile
	special DrawWholeMapView
	delay 30
	applymovement LOCALID_CELIO, Common_Movement_FacePlayer
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_MayIAskOneMoreFavor, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, OneIsland_PokemonCenter_1F_EventScript_DeclineHelpCelio
	goto OneIsland_PokemonCenter_1F_EventScript_GiveRainbowPass
	end

OneIsland_PokemonCenter_1F_EventScript_DeclineHelpCelio::
	msgbox OneIsland_PokemonCenter_1F_Text_PleaseINeedYourHelp, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, OneIsland_PokemonCenter_1F_EventScript_DeclineHelpCelio
	goto OneIsland_PokemonCenter_1F_EventScript_GiveRainbowPass
	end

OneIsland_PokemonCenter_1F_EventScript_GiveRainbowPass::
	msgbox OneIsland_PokemonCenter_1F_Text_AnotherGemstoneInSeviiIslands
	removeitem ITEM_RUBY
	removeitem ITEM_TRI_PASS
	setvar VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 5
	additem ITEM_RAINBOW_PASS
	setflag FLAG_SYS_SEVII_MAP_4567
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message OneIsland_PokemonCenter_1F_Text_ReturnedTriPassForRainbowPass
	waitmessage
	waitfanfare
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message OneIsland_PokemonCenter_1F_Text_ObtainedExtraMapPage
	waitmessage
	waitfanfare
	call EventScript_RestorePrevTextColor
	goto OneIsland_PokemonCenter_1F_EventScript_ExplainRainbowPass
	end

OneIsland_PokemonCenter_1F_EventScript_CelioWaitingForRuby::
	msgbox OneIsland_PokemonCenter_1F_Text_TryingToFindGem
	release
	end

OneIsland_PokemonCenter_1F_EventScript_CelioRequestRuby::
	msgbox OneIsland_PokemonCenter_1F_Text_CelioCaughtMoreMonMaybeICanBeUseful
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_YoullBeTradingFromTrainersFarAway
	applymovement LOCALID_CELIO, Common_Movement_FacePlayer
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_NeedsSpecialGemstone
	setvar VAR_MAP_SCENE_MT_EMBER_EXTERIOR, 1
	setvar VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 4
	release
	end

OneIsland_PokemonCenter_1F_EventScript_CelioPlayerMissingNationalDex::
	msgbox OneIsland_PokemonCenter_1F_Text_CelioImModifyingMyNetworkMachine
	release
	end

OneIsland_PokemonCenter_1F_Movement_CelioCheckMachine::
	walk_in_place_up
	delay_8
	walk_in_place_up
	step_end

OneIsland_PokemonCenter_1F_Movement_CelioPutGemInMachine::
	walk_in_place_faster_up
	walk_in_place_up
	delay_16
	walk_in_place_up
	walk_in_place_up
	step_end

OneIsland_PokemonCenter_1F_EventScript_LittleBoy::
	msgbox OneIsland_PokemonCenter_1F_Text_CameFromPalletDontKnowIt, MSGBOX_NPC
	end

OneIsland_PokemonCenter_1F_EventScript_Hiker::
	msgbox OneIsland_PokemonCenter_1F_Text_EnormousVolcanoOnIsland, MSGBOX_NPC
	end

OneIsland_PokemonCenter_1F_EventScript_BattleGirl::
	lock
	faceplayer
	goto_if_set FLAG_IS_CHAMPION, OneIsland_PokemonCenter_1F_EventScript_BattleGirlHoennLinked
	msgbox OneIsland_PokemonCenter_1F_Text_WishICouldTradeWithBoyfriend
	release
	end

OneIsland_PokemonCenter_1F_EventScript_BattleGirlHoennLinked::
	msgbox OneIsland_PokemonCenter_1F_Text_TradedWithFarAwayBoyfriend
	release
	end

OneIsland_PokemonCenter_1F_EventScript_NetworkMachine::
	lockall
	goto_if_set FLAG_IS_CHAMPION, OneIsland_PokemonCenter_1F_EventScript_NetworkMachineLinkedWithHoenn
	goto_if_set FLAG_SEVII_DETOUR_FINISHED, OneIsland_PokemonCenter_1F_EventScript_NetworkMachineLinkedWithKanto
	msgbox OneIsland_PokemonCenter_1F_Text_MachineUnderAdjustment
	releaseall
	end

OneIsland_PokemonCenter_1F_EventScript_NetworkMachineLinkedWithKanto::
	msgbox OneIsland_PokemonCenter_1F_Text_MachineLinkedWithKanto
	releaseall
	end

OneIsland_PokemonCenter_1F_EventScript_NetworkMachineLinkedWithHoenn::
	msgbox OneIsland_PokemonCenter_1F_Text_MachineLinkedWithKantoAndHoenn
	releaseall
	end

OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandTriggerTop::
	lockall
	setvar VAR_TEMP_1, 1
	goto OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandScene
	end

OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandTriggerMidTop::
	lockall
	setvar VAR_TEMP_1, 2
	goto OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandScene
	end

OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandTriggerMidBottom::
	lockall
	setvar VAR_TEMP_1, 3
	goto OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandScene
	end

OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandTriggerBottom::
	lockall
	setvar VAR_TEMP_1, 4
	goto OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandScene
	end

OneIsland_PokemonCenter_1F_EventScript_LeaveOneIslandScene::
	textcolor NPC_TEXT_COLOR_MALE
	playse SE_PIN
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_BillOhHeyPlayer
	closemessage
	call_if_eq VAR_TEMP_1, 1, OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillTop
	call_if_eq VAR_TEMP_1, 2, OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillMidTop
	call_if_eq VAR_TEMP_1, 3, OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillMidBottom
	call_if_eq VAR_TEMP_1, 4, OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillBottom
	delay 10
	msgbox OneIsland_PokemonCenter_1F_Text_BillWeGotItDone
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CelioJobWentQuick
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_BillYouveLearnedALot
	msgbox OneIsland_PokemonCenter_1F_Text_CelioOhReallyEhehe
	closemessage
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	delay 10
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 15
	msgbox OneIsland_PokemonCenter_1F_Text_BillWeShouldHeadBackToKanto
	closemessage
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 15
	applymovement LOCALID_CELIO, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox OneIsland_PokemonCenter_1F_Text_CelioPromiseIllShowYouAroundSometime
	closemessage
	delay 25
	applymovement LOCALID_PLAYER, OneIsland_PokemonCenter_1F_Movement_PlayerExit
	applymovement LOCALID_ONE_ISLAND_POKEMON_CENTER_BILL, OneIsland_PokemonCenter_1F_Movement_BillExit
	waitmovement 0
	playse SE_EXIT
	setflag FLAG_HIDE_ONE_ISLAND_POKECENTER_BILL
	setvar VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 3
	clearflag FLAG_HIDE_CINNABAR_BILL
	setvar VAR_MAP_SCENE_CINNABAR_ISLAND, 3
	setvar VAR_0x8004, SEAGALLOP_ONE_ISLAND
	setvar VAR_0x8006, SEAGALLOP_CINNABAR_ISLAND
	goto EventScript_SetSail
	end

OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillTop::
	applymovement LOCALID_PLAYER, OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillTop
	waitmovement 0
	return

OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillMidTop::
	applymovement LOCALID_PLAYER, OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillMidTop
	waitmovement 0
	return

OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillMidBottom::
	applymovement LOCALID_PLAYER, OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillMidBottom
	waitmovement 0
	return

OneIsland_PokemonCenter_1F_EventScript_PlayerWalkToBillBottom::
	applymovement LOCALID_PLAYER, OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillBottom
	waitmovement 0
	return

OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillTop::
	walk_right
	walk_down
	walk_in_place_faster_right
	step_end

OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillMidTop::
	walk_right
	step_end

OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillMidBottom::
	walk_right
	walk_up
	walk_in_place_faster_right
	step_end

OneIsland_PokemonCenter_1F_Movement_PlayerWalkToBillBottom::
	walk_right
	walk_up
	walk_up
	walk_in_place_faster_right
	step_end

@ Unused
OneIsland_PokemonCenter_1F_Movement_DelayWalkInPlaceLeft::
	delay_16
	delay_16
	delay_4
	walk_in_place_faster_left
	step_end

@ Unused
OneIsland_PokemonCenter_1F_Movement_WalkInPlaceDown::
	walk_in_place_fast_down
	step_end

OneIsland_PokemonCenter_1F_Movement_PlayerExit::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	step_end

OneIsland_PokemonCenter_1F_Movement_BillExit::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

OneIsland_PokemonCenter_1F_Text_BillHeyThereCelio::
	.string "{JPN}マサキ『よッ ニシキ!$"

OneIsland_PokemonCenter_1F_Text_CelioCantBelieveYouCameOut::
	.string "{JPN}ニシキ『マサキさん\n"
	.string "きて くれたんですね!$"

OneIsland_PokemonCenter_1F_Text_BillHowsYourResearchComing::
	.string "{JPN}マサキ『とうぜんや!\n"
	.string "どや けんきゅうの ほうは\p"
	.string "……っと そのまえに$"

OneIsland_PokemonCenter_1F_Text_ThisIsMyBuddyCelio::
	.string "{JPN}{PLAYER}! こいつが ニシキ!\n"
	.string "がんばる パソコンマニアや!$"

OneIsland_PokemonCenter_1F_Text_PlayerIsRisingPokemonChamp::
	.string "{JPN}ニシキ! こいつが {PLAYER}!\n"
	.string "ポケモンチャンピオン こうほや!$"

OneIsland_PokemonCenter_1F_Text_PlayerIsReigningPokemonChamp::
	.string "{JPN}ニシキ! こいつが {PLAYER}!\n"
	.string "ポケモンチャンピオンや!$"

OneIsland_PokemonCenter_1F_Text_CelioThatsReallyImpressive::
	.string "{JPN}ニシキ『すごいんですね!\n"
	.string "オレ しょうぶは さっぱり\l"
	.string "かてない から…\p"
	.string "よろしく おねがいします!$"

OneIsland_PokemonCenter_1F_Text_BillBringMeUpToSpeed::
	.string "{JPN}マサキ『で どないなん?\n"
	.string "マシンの ほうは$"

OneIsland_PokemonCenter_1F_Text_CelioPCsCantLinkWithYours::
	.string "{JPN}ニシキ『やはり とおすぎます…\p"
	.string "このしまの パソコン からは\n"
	.string "マサキさんの パソコンには\l"
	.string "なかなか つながらない です…$"

OneIsland_PokemonCenter_1F_Text_BillLetMeHelpYou::
	.string "{JPN}マサキ『どれ みせてみい\p"
	.string "………なんや これなら\n"
	.string "なんとか なりそうやないか!\l"
	.string "ちっと てつだったるわ$"

OneIsland_PokemonCenter_1F_Text_CanYouDeliverThisMeteoritePlayer::
	.string "{JPN}{PLAYER} ちょっとだけ\n"
	.string "まっといて くれる?\p"
	.string "……せや ひとつ たのまれて!\p"
	.string "おとなりに 2のしま ちゅう\n"
	.string "しまが あって\l"
	.string "ゲームコーナーを やってる\l"
	.string "おっさんが いてるんや\p"
	.string "めずらしい いしが すきでな…\n"
	.string "マニアどうし つきあいが あるんや\p"
	.string "そのおっさんに\n"
	.string "この いんせき あげたって$"

OneIsland_PokemonCenter_1F_Text_AcceptedMeteoriteFromBill::
	.string "{JPN}{PLAYER}は いんせきを\n"
	.string "マサキから あずかった!$"

OneIsland_PokemonCenter_1F_Text_CelioPleaseTakeThis::
	.string "{JPN}ニシキ『{PLAYER}さん\n"
	.string "2のしまに いかれる なら\l"
	.string "これを さしあげます$"

OneIsland_PokemonCenter_1F_Text_PassLetsYouTravelBetweenIslands::
	.string "{JPN}2のしまと 3のしまに\n"
	.string "ふねで いける パスポートです\p"
	.string "それと これもどうぞ!$"

OneIsland_PokemonCenter_1F_Text_ReceivedExtraPageForTownMap::
	.string "{JPN}{PLAYER}は タウンマップの\n"
	.string "あたらしいページを もらった!$"

OneIsland_PokemonCenter_1F_Text_ReceivedTownMap::
	.string "{JPN}{PLAYER}は\n"
	.string "タウンマップを もらった!$"

OneIsland_PokemonCenter_1F_Text_BillCatchYouLater::
	.string "{JPN}マサキ『ほな また あとでな!\n"
	.string "おっさんに よろしゅう!$"

OneIsland_PokemonCenter_1F_Text_HmmHowAboutLikeThis::
	.string "{JPN}うーん\p"
	.string "ここは こうすれば\n"
	.string "ええんちゃう かな……?$"

OneIsland_PokemonCenter_1F_Text_GotPCWorkingStrollAWhileMore::
	.string "{JPN}おッ {PLAYER}!\n"
	.string "パソコン つかえる ように\l"
	.string "なっとった やろ!\p"
	.string "あとは… ニシキに\n"
	.string "おしえとく ことが あるから\l"
	.string "もすこし さんぽ しといて!$"

OneIsland_PokemonCenter_1F_Text_SorryForBeingPoorHost::
	.string "{JPN}マサキさんを おかり しますね\p"
	.string "せっかく いらしたのに\n"
	.string "なにも できなくて ごめんなさい$"

OneIsland_PokemonCenter_1F_Text_UsualPCServicesUnavailable::
	.string "{JPN}いつも つかってる パソコンは\n"
	.string "まだ よびだせない…$"

OneIsland_PokemonCenter_1F_Text_BillOhHeyPlayer::
	.string "{JPN}マサキ『あっ {PLAYER}!$"

OneIsland_PokemonCenter_1F_Text_BillWeGotItDone::
	.string "{JPN}マサキ『おそいやないか\n"
	.string "たのしんで きたん?\p"
	.string "こっちは もー バッチリ\n"
	.string "パソコンが つながったで!$"

OneIsland_PokemonCenter_1F_Text_CelioJobWentQuick::
	.string "{JPN}ニシキ『あっというま\n"
	.string "だったん ですよ\p"
	.string "マサキさんは やっぱり すごいな…$"

OneIsland_PokemonCenter_1F_Text_BillYouveLearnedALot::
	.string "{JPN}マサキ『いやいや! ほとんど\n"
	.string "てつだうとこ なかったわ\p"
	.string "ニシキー! おまえさん\n"
	.string "ずいぶん べんきょう したんやなあ$"

OneIsland_PokemonCenter_1F_Text_CelioOhReallyEhehe::
	.string "{JPN}ニシキ『えっ!\n"
	.string "えへへ…$"

OneIsland_PokemonCenter_1F_Text_BillWeShouldHeadBackToKanto::
	.string "{JPN}マサキ『まっ というわけで\n"
	.string "しごとは おしまいや!\l"
	.string "カントーに かえろか\p"
	.string "ニシキ ほな またな!$"

OneIsland_PokemonCenter_1F_Text_CelioPromiseIllShowYouAroundSometime::
	.string "{JPN}ニシキ『{PLAYER}さん\n"
	.string "きょうは おひとりで\l"
	.string "いかせちゃって ごめんなさい\p"
	.string "オレ そのうち かならず\n"
	.string "ごあんない しますから!$"

OneIsland_PokemonCenter_1F_Text_CelioImModifyingMyNetworkMachine::
	.string "{JPN}ニシキ『こんにちは!\n"
	.string "いつも おいそがしそう ですね\p"
	.string "オレですか?\n"
	.string "この ネットワークマシンを\l"
	.string "かいりょう してます\p"
	.string "マシンが かんせいしたら\n"
	.string "まっさきに {PLAYER}さんに\l"
	.string "つかって ほしいなあ$"

OneIsland_PokemonCenter_1F_Text_CelioCaughtMoreMonMaybeICanBeUseful::
	.string "{JPN}ニシキ『{PLAYER}さん\n"
	.string "さいきんは どうですか?\p"
	.string "へえー もっと たくさんの\n"
	.string "しゅるいの ポケモンを…?\p"
	.string "それなら オレ\n"
	.string "おてつだい できるかも しれません$"

OneIsland_PokemonCenter_1F_Text_YoullBeTradingFromTrainersFarAway::
	.string "{JPN}いま とおいちほうの ひとと\n"
	.string "ポケモンを こうかん できるように\l"
	.string "ネットワークマシンを\l"
	.string "かいりょう して いるんです\p"
	.string "マシンが かんせい したら\n"
	.string "めずらしい ポケモンを\l"
	.string "こうかん して もらえますよ!$"

OneIsland_PokemonCenter_1F_Text_NeedsSpecialGemstone::
	.string "{JPN}ただ… つうしんが うまくいくには\n"
	.string "ある とくべつな いしが\l"
	.string "ひつよう なんです\p"
	.string "1のしまの どこかに\n"
	.string "あると いうけど みつからなくて…\p"
	.string "どこに あるんでしょうね?$"

OneIsland_PokemonCenter_1F_Text_TryingToFindGem::
	.string "{JPN}けんきゅう しながら\n"
	.string "いしを さがして いるから\l"
	.string "どちらも はかどらないんです…\p"
	.string "マサキさんに たよれば\n"
	.string "けんきゅうは すすみます けど\l"
	.string "こんどは それは なしで\l"
	.string "がんばろうと おもって いるんです$"

OneIsland_PokemonCenter_1F_Text_OhThats::
	.string "{JPN}え! あ それは!$"

OneIsland_PokemonCenter_1F_Text_HandedRubyToCelio::
	.string "{JPN}{PLAYER}は ニシキに\n"
	.string "ルビーを わたした!$"

OneIsland_PokemonCenter_1F_Text_MayIAskOneMoreFavor::
	.string "{JPN}ありがとう…!\n"
	.string "{PLAYER}さんって すごいです!\p"
	.string "……… ………\p"
	.string "あの おねがいが あるんですが\n"
	.string "きいて もらえますか?$"

OneIsland_PokemonCenter_1F_Text_PleaseINeedYourHelp::
	.string "{JPN}へ へんな ことじゃ ないです\n"
	.string "どうか おねがいします!$"

OneIsland_PokemonCenter_1F_Text_AnotherGemstoneInSeviiIslands::
	.string "{JPN}いしの ことを しらべていて\n"
	.string "わかったんですが…\p"
	.string "この ルビーと\n"
	.string "ついに なっている いしが\l"
	.string "ナナシマの どこかに\l"
	.string "ある はずなんです\p"
	.string "{PLAYER}さん おねがいです\n"
	.string "それを さがして ください!\p"
	.string "{PLAYER}さん ふねのパスと\n"
	.string "タウンマップを かしてください$"

OneIsland_PokemonCenter_1F_Text_ReturnedTriPassForRainbowPass::
	.string "{JPN}{PLAYER}は トライパスを かえして\n"
	.string "レインボーパスを もらった!$"

OneIsland_PokemonCenter_1F_Text_ObtainedExtraMapPage::
	.string "{JPN}タウンマップの\n"
	.string "あたらしいページを もらった!$"

OneIsland_PokemonCenter_1F_Text_PassLetYouGetToAllIslands::
	.string "{JPN}これ オレの パスなんです\n"
	.string "このパス だったら\l"
	.string "ナナシマの すべての しまへ\l"
	.string "ゆけるんです\p"
	.string "{PLAYER}さん\n"
	.string "どうか おねがいします!$"

OneIsland_PokemonCenter_1F_Text_HandedSapphireToCelio::
	.string "{JPN}{PLAYER}は ニシキに\n"
	.string "サファイアを わたした!$"

OneIsland_PokemonCenter_1F_Text_ThankYouGiveMeTime::
	.string "{JPN}ニシキ『これが\n"
	.string "ルビーと ついの いし…\p"
	.string "{PLAYER}さん……\n"
	.string "これを てに いれるのに\l"
	.string "いろいろと くろう されましたね\l"
	.string "いわなくても わかります\p"
	.string "ほんとうに ありがとう!\p"
	.string "よし オレも がんばるぞ!\n"
	.string "すこし まってて ください!$"

OneIsland_PokemonCenter_1F_Text_OkayThisIsGood::
	.string "{JPN}よし いいぞ…$"

OneIsland_PokemonCenter_1F_Text_LinkedUpWithLanette::
	.string "{JPN}やった マユミさんと\n"
	.string "つながったぞ…!$"

OneIsland_PokemonCenter_1F_Text_ManagedToLinkWithHoennThankYou::
	.string "{JPN}{PLAYER}さん… {PLAYER}さん\n"
	.string "できました!\p"
	.string "ホウエンちほうの トレーナーと\n"
	.string "つうしんが できるんです!\p"
	.string "これで ネットワークマシンは\n"
	.string "かんせい です……!\p"
	.string "すべて\n"
	.string "{PLAYER}さんの おかげです!\l"
	.string "オレの ゆめが かなった のは…$"

OneIsland_PokemonCenter_1F_Text_WishYouBestOfLuck::
	.string "{JPN}な ないてなんか いませんよ\p"
	.string "それより {PLAYER}さん\n"
	.string "たくさんの めずらしい ポケモン\l"
	.string "さがすの でしょ?\l"
	.string "がんばって ください!$"

OneIsland_PokemonCenter_1F_Text_CelioHearingRumorsAboutYou::
	.string "{JPN}ニシキ『こんにちは!\p"
	.string "{PLAYER}さんの おうわさ\n"
	.string "きいて いますよ!$"

OneIsland_PokemonCenter_1F_Text_BillsFirstMonWasAbra::
	.string "{JPN}{PLAYER}さんは どんな\n"
	.string "ポケモンが おすき ですか?\p"
	.string "マサキさんは マニアだから\n"
	.string "どんな ポケモンも おすきですけど\l"
	.string "はじめて つかまえた のは\l"
	.string "ケーシィだそう ですよ$"

OneIsland_PokemonCenter_1F_Text_BillsHometownInGoldenrod::
	.string "{JPN}そういえば {PLAYER}さんって\n"
	.string "マサラタウンの かた でしたね\l"
	.string "しずかで よいところと ききます!\p"
	.string "マサキさんは コガネシティに\n"
	.string "ごじっかが あるんですよ!\l"
	.string "それは それは\l"
	.string "にぎやかな まちだそうです\p"
	.string "オレも いって みたいなあ…$"

OneIsland_PokemonCenter_1F_Text_BillCantStomachMilk::
	.string "{JPN}{PLAYER}さんは\n"
	.string "にがてな ものって ありますか?\p"
	.string "マサキさんは どうしても\n"
	.string "ミルクが のめないん ですよ$"

OneIsland_PokemonCenter_1F_Text_CameFromPalletDontKnowIt::
	.string "{JPN}しらない ひとだー!\n"
	.string "ねえ どっから きたの?\p"
	.string "…マサラタウンー?\n"
	.string "きいた こと ないや$"

OneIsland_PokemonCenter_1F_Text_EnormousVolcanoOnIsland::
	.string "{JPN}この しまには\n"
	.string "おおきな かざんが あるのだ!\p"
	.string "さいきんは ふんか\n"
	.string "してない みたいだし\l"
	.string "やまのぼりを たのしんで きたら?$"

OneIsland_PokemonCenter_1F_Text_WishICouldTradeWithBoyfriend::
	.string "{JPN}とおくに すむ カレと\n"
	.string "ポケモンの こうかんが したいわ$"

OneIsland_PokemonCenter_1F_Text_TradedWithFarAwayBoyfriend::
	.string "{JPN}とおくの カレと\n"
	.string "ポケモンを こうかん できたの!\p"
	.string "あなたと ニシキくんの\n"
	.string "おかげですって みんなが…\l"
	.string "ありがとう!$"

OneIsland_PokemonCenter_1F_Text_MachineUnderAdjustment::
	.string "{JPN}ネットワークマシン\n"
	.string "つうしん レベル 0\p"
	.string "……ポケモン あずかり システム\n"
	.string "ちょうせいちゅう$"

OneIsland_PokemonCenter_1F_Text_MachineLinkedWithKanto::
	.string "{JPN}ネットワークマシン\n"
	.string "つうしん レベル 1\p"
	.string "カントーちほうとの\n"
	.string "つうしん じゅんび かんりょう$"

OneIsland_PokemonCenter_1F_Text_MachineLinkedWithKantoAndHoenn::
	.string "{JPN}ネットワークマシン\n"
	.string "つうしん レベル 2\p"
	.string "カントーちほう ホウエンちほうとの\n"
	.string "つうしん じゅんび かんりょう$"

OneIsland_PokemonCenter_1F_Text_ObtainedTriPass::
	.string "{JPN}トライパスを てにいれた!$"

