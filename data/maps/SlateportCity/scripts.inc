SlateportCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SlateportCity_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SlateportCity_OnFrame
	.byte 0

SlateportCity_OnTransition:
	setvar VAR_SLATEPORT_MUSEUM_1F_STATE, 0
	call SlateportCity_EventScript_EnterSlateport
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_MovePeopleForSternInterview
	call_if_eq VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 1, SlateportCity_EventScript_SetReadyForScottScene
	end

SlateportCity_EventScript_EnterSlateport::
	setflag FLAG_VISITED_SLATEPORT_CITY
	setvar VAR_CONTEST_HALL_STATE, 0
	setflag FLAG_HIDE_SLATEPORT_CITY_CONTEST_REPORTER
	return

SlateportCity_EventScript_MovePeopleForSternInterview::
	setobjectxyperm LOCALID_SLATEPORT_CAPT_STERN, 28, 13
	setobjectxyperm LOCALID_SLATEPORT_OLD_WOMAN, 25, 13
	setobjectxyperm LOCALID_SLATEPORT_RICH_BOY, 25, 14
	setobjectxyperm LOCALID_SLATEPORT_COOK, 27, 16
	setobjectxyperm LOCALID_SLATEPORT_GIRL, 28, 16
	setobjectxyperm LOCALID_SLATEPORT_FAT_MAN, 29, 16
	setobjectxyperm LOCALID_SLATEPORT_MAN_1, 31, 14
	setobjectmovementtype LOCALID_SLATEPORT_CAPT_STERN, MOVEMENT_TYPE_FACE_DOWN
	setobjectmovementtype LOCALID_SLATEPORT_OLD_WOMAN, MOVEMENT_TYPE_FACE_DOWN_AND_RIGHT
	setobjectmovementtype LOCALID_SLATEPORT_RICH_BOY, MOVEMENT_TYPE_FACE_UP_AND_RIGHT
	setobjectmovementtype LOCALID_SLATEPORT_COOK, MOVEMENT_TYPE_FACE_UP
	setobjectmovementtype LOCALID_SLATEPORT_GIRL, MOVEMENT_TYPE_FACE_UP
	setobjectmovementtype LOCALID_SLATEPORT_FAT_MAN, MOVEMENT_TYPE_FACE_UP
	setobjectmovementtype LOCALID_SLATEPORT_MAN_1, MOVEMENT_TYPE_FACE_LEFT
	return

SlateportCity_EventScript_SetReadyForScottScene::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_eq VAR_0x8004, 30, SlateportCity_EventScript_MoveScottLeft
	setobjectxyperm LOCALID_SLATEPORT_SCOTT, 23, 27
	setobjectmovementtype LOCALID_SLATEPORT_SCOTT, MOVEMENT_TYPE_FACE_RIGHT
	return

SlateportCity_EventScript_MoveScottLeft::
	setobjectxyperm LOCALID_SLATEPORT_SCOTT, 22, 27
	setobjectmovementtype LOCALID_SLATEPORT_SCOTT, MOVEMENT_TYPE_FACE_RIGHT
	return

SlateportCity_OnFrame:
	map_script_2 VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 1, SlateportCity_EventScript_ScottScene
	.2byte 0

SlateportCity_EventScript_ScottScene::
	lockall
	addobject LOCALID_SLATEPORT_SCOTT
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerFaceScott
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottApproachPlayer
	waitmovement 0
	msgbox SlateportCity_Text_YouDroveTeamAquaAway, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_SCOTT, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	delay 60
	msgbox SlateportCity_Text_MaybeThisTrainer, MSGBOX_DEFAULT
	applymovement LOCALID_SLATEPORT_SCOTT, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox SlateportCity_Text_LetsRegisterEachOther, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox SlateportCity_Text_RegisteredScott, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_SCOTT_MATCH_CALL
	msgbox SlateportCity_Text_KeepEyeOnTrainersBeSeeingYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottExit
	waitmovement 0
	removeobject LOCALID_SLATEPORT_SCOTT
	setobjectxyperm LOCALID_SLATEPORT_SCOTT, 10, 12
	setobjectmovementtype LOCALID_SLATEPORT_SCOTT, MOVEMENT_TYPE_FACE_DOWN
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 2
	addvar VAR_SCOTT_STATE, 1
	releaseall
	end

SlateportCity_Movement_PlayerFaceScott:
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

SlateportCity_Movement_ScottApproachPlayer:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_Movement_ScottExit:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

SlateportCity_EventScript_EnergyGuru::
	lock
	faceplayer
	message SlateportCity_Text_EnergyGuruSellWhatYouNeed
	waitmessage
	pokemart SlateportCity_Pokemart_EnergyGuru
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_Pokemart_EnergyGuru:
	.2byte ITEM_PROTEIN
	.2byte ITEM_IRON
	.2byte ITEM_CARBOS
	.2byte ITEM_ZINC
	.2byte ITEM_CALCIUM
	.2byte ITEM_HP_UP
	pokemartlistend

SlateportCity_EventScript_EffortRibbonWoman::
	lock
	faceplayer
	bufferleadmonspeciesname STR_VAR_1
	msgbox SlateportCity_Text_OhYourPokemon, MSGBOX_DEFAULT
	specialvar VAR_RESULT, LeadMonHasEffortRibbon
	call_if_eq VAR_RESULT, TRUE, SlateportCity_EventScript_MonHasEffortRibbon
	specialvar VAR_RESULT, Special_AreLeadMonEVsMaxedOut
	call_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_MonEVsNotMaxed
	msgbox SlateportCity_Text_PleaseGiveItThisEffortRibbon, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message SlateportCity_Text_ReceivedEffortRibbon
	waitfanfare
	msgbox SlateportCity_Text_PutEffortRibbonOnMon, MSGBOX_DEFAULT
	special GiveLeadMonEffortRibbon
	release
	end

SlateportCity_EventScript_MonEVsNotMaxed::
	msgbox SlateportCity_Text_GoForItLittleHarder, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_MonHasEffortRibbon::
	msgbox SlateportCity_Text_EffortRibbonLooksGoodOnIt, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Cook::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_CookSternInterview
	msgbox SlateportCity_Text_SeaweedFullOfLife, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_CookSternInterview::
	msgbox SlateportCity_Text_CaptainComeBackWithBigFish, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_OldWoman::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_OldWomanSternInterview
	msgbox SlateportCity_Text_HowTownIsBornAndGrows, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_OldWomanSternInterview::
	msgbox SlateportCity_Text_CaptSternBeingInterviewed, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Girl::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_GirlSternInterview
	goto_if_set FLAG_RECEIVED_SECRET_POWER, SlateportCity_EventScript_GirlSecretBase
	msgbox SlateportCity_Text_SlateportWonderfulPlace, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_GirlSternInterview::
	msgbox SlateportCity_Text_InterviewerSoCool, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_GirlSecretBase::
	msgbox SlateportCity_Text_BuyBricksSoDecorWontGetDirty, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_RichBoy::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_RichBoySternInterview
	msgbox SlateportCity_Text_GoingToCompeteInBattleTent, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_RichBoySternInterview::
	msgbox SlateportCity_Text_SternSaysDiscoveredSomething, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_FatMan::
	goto_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_FatManSternInterview
	msgbox SlateportCity_Text_BushedHikingFromMauville, MSGBOX_NPC
	end

SlateportCity_EventScript_FatManSternInterview::
	msgbox SlateportCity_Text_AmIOnTV, MSGBOX_SIGN
	end

SlateportCity_EventScript_Man1::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_Man1SternInterview
	msgbox SlateportCity_Text_EveryoneCallsHimCaptStern, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Man1SternInterview::
	msgbox SlateportCity_Text_CaptainsACelebrity, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Woman1::
	lock
	faceplayer
	goto_if_set FLAG_DOCK_REJECTED_DEVON_GOODS, SlateportCity_EventScript_Woman1AquaGone
	msgbox SlateportCity_Text_WhatsLongLineOverThere, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Woman1AquaGone::
	msgbox SlateportCity_Text_VisitedMuseumOften, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_BattleTentSign::
	msgbox SlateportCity_Text_BattleTentSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_SternsShipyardSign::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_EventScript_SternsShipyardFerryComplete
	goto_if_set FLAG_BADGE07_GET, SlateportCity_EventScript_SternsShipyardNearsCompletion
	msgbox SlateportCity_Text_SternsShipyardWantedSign, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_SternsShipyardNearsCompletion::
	msgbox SlateportCity_Text_SternsShipyardNearsCompletion, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_SternsShipyardFerryComplete::
	msgbox SlateportCity_Text_SternsShipyardFerryComplete, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_PokemonFanClubSign::
	msgbox SlateportCity_Text_PokemonFanClubSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_OceanicMuseumSign::
	msgbox SlateportCity_Text_OceanicMuseumSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_CitySign::
	msgbox SlateportCity_Text_CitySign, MSGBOX_SIGN
	end

SlateportCity_EventScript_MarketSign::
	msgbox SlateportCity_Text_MarketSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_HarborSign::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_EventScript_HarborSignFerryComplete
	msgbox SlateportCity_Text_HarborFerryUnderConstruction, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_HarborSignFerryComplete::
	msgbox SlateportCity_Text_HarborSign, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_NameRatersHouseSign::
	msgbox SlateportCity_Text_NameRatersHouseSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_Maniac::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_ManiacSternInterview
	msgbox SlateportCity_Text_GetNameRaterToHelpYou, MSGBOX_DEFAULT
	release
	end

@ Unclear if the text here was meant to be different, but its not
SlateportCity_EventScript_ManiacSternInterview::
	msgbox SlateportCity_Text_GetNameRaterToHelpYou, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Woman2::
	msgbox SlateportCity_Text_CantChangeTradeMonName, MSGBOX_NPC
	end

SlateportCity_EventScript_Sailor1::
	msgbox SlateportCity_Text_SeaIsSoWet, MSGBOX_NPC
	end

SlateportCity_EventScript_Sailor2::
	msgbox SlateportCity_Text_SinkOldBoats, MSGBOX_NPC
	end

SlateportCity_EventScript_PokefanF::
	msgbox SlateportCity_Text_BuyTooMuch, MSGBOX_NPC
	end

SlateportCity_EventScript_Man2::
	msgbox SlateportCity_Text_BattleTentBuiltRecently, MSGBOX_NPC
	end

SlateportCity_EventScript_AquaGrunt1::
	lock
	faceplayer
	msgbox SlateportCity_Text_QuitPushing, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt2::
	lock
	faceplayer
	msgbox SlateportCity_Text_AquaHasPolicy, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt3::
	lock
	faceplayer
	msgbox SlateportCity_Text_BossIsBrilliant, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt4::
	lock
	faceplayer
	msgbox SlateportCity_Text_WhatsNewSchemeIWonder, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_4, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt5::
	lock
	faceplayer
	msgbox SlateportCity_Text_ShouldTakeItAll, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_5, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt6::
	lock
	faceplayer
	msgbox SlateportCity_Text_DontButtIn, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_6, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt7::
	lock
	faceplayer
	msgbox SlateportCity_Text_RemindsMeOfLongLineForGames, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_7, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt8::
	lock
	faceplayer
	msgbox SlateportCity_Text_WhyAreWeLiningUp, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_8, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt9::
	lock
	faceplayer
	playse SE_PIN
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_Delay48
	waitmovement 0
	msgbox SlateportCity_Text_WhatDoYouWant, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FaceOriginalDirection
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, SlateportCity_Movement_DelayAquaGrunt
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_Text_IllReadSignForYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FaceOriginalDirection
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, SlateportCity_Movement_DelayAquaGrunt
	waitmovement 0
	msgbox SlateportCity_Text_SaysSomethingLikeSeaIsEndless, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FacePlayer
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, SlateportCity_Movement_DelayAquaGrunt
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_Movement_DelayAquaGrunt:
	delay_16
	delay_16
	step_end

SlateportCity_EventScript_AquaGrunt10::
	msgbox SlateportCity_Text_ShouldveBroughtMyGameBoy, MSGBOX_SIGN
	end

SlateportCity_EventScript_AquaGrunt11::
	lock
	faceplayer
	msgbox SlateportCity_Text_HotSpringsAfterOperation, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_11, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_DollClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemartdecoration SlateportCity_PokemartDecor_Dolls
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_PokemartDecor_Dolls:
	.2byte DECOR_AZURILL_DOLL
	.2byte DECOR_MARILL_DOLL
	.2byte DECOR_SKITTY_DOLL
	pokemartlistend

SlateportCity_EventScript_ComeBackWithSecretPower::
	msgbox gText_ComeBackWithSecretPower, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_DecorClerk::
	lock
	faceplayer
	goto_if_unset FLAG_RECEIVED_SECRET_POWER, SlateportCity_EventScript_ComeBackWithSecretPower
	message gText_HowMayIServeYou
	waitmessage
	pokemartdecoration SlateportCity_PokemartDecor
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_PokemartDecor:
	.2byte DECOR_RED_BRICK
	.2byte DECOR_BLUE_BRICK
	.2byte DECOR_YELLOW_BRICK
	.2byte DECOR_RED_BALLOON
	.2byte DECOR_BLUE_BALLOON
	.2byte DECOR_YELLOW_BALLOON
	.2byte DECOR_C_LOW_NOTE_MAT
	.2byte DECOR_D_NOTE_MAT
	.2byte DECOR_E_NOTE_MAT
	.2byte DECOR_F_NOTE_MAT
	.2byte DECOR_G_NOTE_MAT
	.2byte DECOR_A_NOTE_MAT
	.2byte DECOR_B_NOTE_MAT
	.2byte DECOR_C_HIGH_NOTE_MAT
	pokemartlistend

SlateportCity_EventScript_PowerTMClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart SlateportCity_Pokemart_PowerTMs
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_Pokemart_PowerTMs:
	.2byte ITEM_TM_HIDDEN_POWER
	.2byte ITEM_TM_SECRET_POWER
	pokemartlistend

@ Scene with Capt Sterns interview and Team Aqua announcing plans to steal Submarine
SlateportCity_EventScript_CaptStern::
	lockall
	msgbox SlateportCity_Text_SternMoveAheadWithExploration, MSGBOX_DEFAULT
	msgbox SlateportCity_Text_GabbyWonderfulThanksForInterview, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GABBY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 10
	applymovement LOCALID_SLATEPORT_TY, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 25
	applymovement LOCALID_SLATEPORT_CAPT_STERN, SlateportCity_Movement_SternWatchGabbyAndTyExit
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerFaceStern
	applymovement LOCALID_SLATEPORT_GABBY, SlateportCity_Movement_GabbyExit
	applymovement LOCALID_SLATEPORT_TY, SlateportCity_Movement_TyExit
	waitmovement 0
	removeobject LOCALID_SLATEPORT_GABBY
	removeobject LOCALID_SLATEPORT_TY
	msgbox SlateportCity_Text_SternWhewFirstInterview, MSGBOX_DEFAULT
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox SlateportCity_Text_OhPlayerWeMadeDiscovery, MSGBOX_DEFAULT
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	msgbox SlateportCity_Text_AquaWillAssumeControlOfSubmarine, MSGBOX_DEFAULT
	applymovement LOCALID_SLATEPORT_COOK, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_SLATEPORT_FAT_MAN, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_SLATEPORT_OLD_WOMAN, SlateportCity_Movement_OldWomanConcern
	applymovement LOCALID_SLATEPORT_RICH_BOY, Common_Movement_QuestionMark
	applymovement LOCALID_SLATEPORT_MAN_1, SlateportCity_Movement_ManConcern
	waitmovement 0
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox SlateportCity_Text_SternWhatWasAllThat, MSGBOX_DEFAULT
	playse SE_PIN
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox SlateportCity_Text_FromHarborTryingToTakeSub, MSGBOX_DEFAULT
	msgbox SlateportCity_Text_PleaseComeWithMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_CAPT_STERN, SlateportCity_Movement_SternEnterHarbor
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerEnterHarbor
	waitmovement 0
	removeobject LOCALID_SLATEPORT_CAPT_STERN
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_CAPTAIN_STERN
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_SUBMARINE_SHADOW
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_AQUA_GRUNT
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_ARCHIE
	setvar VAR_SLATEPORT_CITY_STATE, 2
	warp MAP_SLATEPORT_CITY_HARBOR, 11, 14
	waitstate
	releaseall
	end

SlateportCity_Movement_OldWomanConcern:
	delay_16
	delay_16
	emote_question_mark
	walk_in_place_faster_right
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

SlateportCity_Movement_ManConcern:
	emote_question_mark
	walk_in_place_faster_up
	delay_16
	walk_in_place_faster_down
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

SlateportCity_Movement_GabbyExit:
	delay_16
	walk_left
	walk_left
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

SlateportCity_Movement_TyExit:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

SlateportCity_Movement_Unused:
	walk_down
	walk_in_place_faster_up
	step_end

SlateportCity_Movement_SternEnterHarbor:
	walk_right
	walk_up
	walk_up
	set_invisible
	step_end

SlateportCity_Movement_SternWatchGabbyAndTyExit:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_down
	walk_left
	step_end

SlateportCity_Movement_PlayerEnterHarbor:
	walk_in_place_faster_right
	delay_16
	delay_16
	walk_right
	walk_up
	step_end

SlateportCity_Movement_PlayerFaceStern:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_down
	step_end

SlateportCity_EventScript_Ty::
	msgbox SlateportCity_Text_BigSmileForCamera, MSGBOX_SIGN
	end

SlateportCity_EventScript_Gabby::
	msgbox SlateportCity_Text_MostInvaluableExperience, MSGBOX_SIGN
	end

SlateportCity_EventScript_Man3::
	msgbox SlateportCity_Text_WonderIfLighthouseStartlesPokemon, MSGBOX_NPC
	end

SlateportCity_EventScript_BerryPowderClerk::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_POWDER_JAR, SlateportCity_EventScript_ReceivedPowderJar
	msgbox SlateportCity_Text_ExplainBerries, MSGBOX_DEFAULT
	giveitem ITEM_POWDER_JAR
	setflag FLAG_RECEIVED_POWDER_JAR
	msgbox SlateportCity_Text_ExplainBerryPowder, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_ReceivedPowderJar::
	setvar VAR_0x8004, 1
	specialvar VAR_RESULT, HasEnoughBerryPowder
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_ExplainBerryPowder
	msgbox SlateportCity_Text_BroughtMeSomeBerryPowder, MSGBOX_DEFAULT
	special DisplayBerryPowderVendorMenu
	goto SlateportCity_EventScript_ChooseBerryPowderItem
	end

SlateportCity_EventScript_ExplainBerryPowder::
	msgbox SlateportCity_Text_ExplainBerryPowder, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_ChooseBerryPowderItem::
	message SlateportCity_Text_ExchangeWhatWithIt
	waitmessage
	setvar VAR_0x8004, SCROLL_MULTI_BERRY_POWDER_VENDOR
	special ShowScrollableMultichoice
	switch VAR_RESULT
	case 0, SlateportCity_EventScript_EnergyPowder
	case 1, SlateportCity_EventScript_EnergyRoot
	case 2, SlateportCity_EventScript_HealPowder
	case 3, SlateportCity_EventScript_RevivalHerb
	case 4, SlateportCity_EventScript_Protein
	case 5, SlateportCity_EventScript_Iron
	case 6, SlateportCity_EventScript_Carbos
	case 7, SlateportCity_EventScript_Calcium
	case 8, SlateportCity_EventScript_Zinc
	case 9, SlateportCity_EventScript_HPUp
	case 10, SlateportCity_EventScript_PPUp
	case 11, SlateportCity_EventScript_CancelPowderItemSelect
	case MULTI_B_PRESSED, SlateportCity_EventScript_CancelPowderItemSelect
	end

SlateportCity_EventScript_EnergyPowder::
	bufferitemname STR_VAR_1, ITEM_ENERGY_POWDER
	setvar VAR_0x8008, ITEM_ENERGY_POWDER
	setvar VAR_0x8009, 50
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_EnergyRoot::
	bufferitemname STR_VAR_1, ITEM_ENERGY_ROOT
	setvar VAR_0x8008, ITEM_ENERGY_ROOT
	setvar VAR_0x8009, 80
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_HealPowder::
	bufferitemname STR_VAR_1, ITEM_HEAL_POWDER
	setvar VAR_0x8008, ITEM_HEAL_POWDER
	setvar VAR_0x8009, 50
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_RevivalHerb::
	bufferitemname STR_VAR_1, ITEM_REVIVAL_HERB
	setvar VAR_0x8008, ITEM_REVIVAL_HERB
	setvar VAR_0x8009, 300
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Protein::
	bufferitemname STR_VAR_1, ITEM_PROTEIN
	setvar VAR_0x8008, ITEM_PROTEIN
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Iron::
	bufferitemname STR_VAR_1, ITEM_IRON
	setvar VAR_0x8008, ITEM_IRON
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Carbos::
	bufferitemname STR_VAR_1, ITEM_CARBOS
	setvar VAR_0x8008, ITEM_CARBOS
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Calcium::
	bufferitemname STR_VAR_1, ITEM_CALCIUM
	setvar VAR_0x8008, ITEM_CALCIUM
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Zinc::
	bufferitemname STR_VAR_1, ITEM_ZINC
	setvar VAR_0x8008, ITEM_ZINC
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_HPUp::
	bufferitemname STR_VAR_1, ITEM_HP_UP
	setvar VAR_0x8008, ITEM_HP_UP
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_PPUp::
	bufferitemname STR_VAR_1, ITEM_PP_UP
	setvar VAR_0x8008, ITEM_PP_UP
	setvar VAR_0x8009, 3000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_CancelPowderItemSelect::
	msgbox SlateportCity_Text_ComeBackToTradeBerryPowder, MSGBOX_DEFAULT
	special RemoveBerryPowderVendorMenu
	release
	end

SlateportCity_EventScript_TryBuyBerryPowderItem::
	msgbox SlateportCity_Text_ExchangeBerryPowderForItem, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SlateportCity_EventScript_ChooseBerryPowderItem
	copyvar VAR_0x8004, VAR_0x8009
	specialvar VAR_RESULT, HasEnoughBerryPowder
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_NotEnoughBerryPowder
	giveitem VAR_0x8008
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_NoRoomForBerryPowderItem
	copyvar VAR_0x8004, VAR_0x8009
	special TakeBerryPowder
	special PrintPlayerBerryPowderAmount
	msgbox SlateportCity_Text_FineBerryPowderTradeSomethingElse, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SlateportCity_EventScript_ChooseBerryPowderItem
	msgbox SlateportCity_Text_WhenYouGetMoreBringItToMe, MSGBOX_DEFAULT
	special RemoveBerryPowderVendorMenu
	release
	end

SlateportCity_EventScript_NoRoomForBerryPowderItem::
	msgbox gText_TheBagIsFull, MSGBOX_DEFAULT
	special RemoveBerryPowderVendorMenu
	release
	end

SlateportCity_EventScript_NotEnoughBerryPowder::
	msgbox SlateportCity_Text_DontHaveEnoughBerryPowder, MSGBOX_DEFAULT
	goto SlateportCity_EventScript_ChooseBerryPowderItem
	end

SlateportCity_EventScript_ScottBattleTentScene::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	opendoor 10, 12
	waitdooranim
	addobject LOCALID_SLATEPORT_SCOTT
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PushPlayerDown
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottExitBattleTent
	waitmovement 0
	closedoor 10, 12
	waitdooranim
	msgbox SlateportCity_Text_TakingBattleTentChallenge, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerWatchScottExit
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottExitAfterBattleTent
	waitmovement 0
	removeobject LOCALID_SLATEPORT_SCOTT
	addvar VAR_SCOTT_STATE, 1
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 3
	releaseall
	end

SlateportCity_Movement_PushPlayerDown:
	lock_facing_direction
	walk_down
	unlock_facing_direction
	step_end

SlateportCity_Movement_PlayerWatchScottExit:
	delay_16
	walk_in_place_faster_right
	step_end

SlateportCity_Movement_ScottExitBattleTent:
	delay_8
	walk_down
	step_end

SlateportCity_Movement_ScottExitAfterBattleTent:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_EventScript_BerryCrushRankingsSign::
	lockall
	special ShowBerryCrushRankings
	releaseall
	end

SlateportCity_Text_EnergyGuruSellWhatYouNeed:
	.string "{JPN}わし がんばりや!\nがんばって いろいろ うりますよ!"
	.string "$"

SlateportCity_Text_OhYourPokemon:
	.string "{JPN}あら?\nきみの {STR_VAR_1}……"
	.string "$"

SlateportCity_Text_PleaseGiveItThisEffortRibbon:
	.string "{JPN}ものすごく がんばったのね!\pごほうびと して\nこの がんばリボンを つけてあげて!"
	.string ""
	.string "$"

SlateportCity_Text_ReceivedEffortRibbon:
	.string "{JPN}{PLAYER}は がんばリボンを もらった!$"

SlateportCity_Text_PutEffortRibbonOnMon:
	.string "{JPN}{PLAYER}は がんばリボンを\n{STR_VAR_1}に つけて あげた!"
	.string "$"

SlateportCity_Text_GoForItLittleHarder:
	.string "{JPN}もっと がんばってみて!\pがんばれば わたしが ポケモンに\nいいもの あげるから!"
	.string ""
	.string "$"

SlateportCity_Text_EffortRibbonLooksGoodOnIt:
	.string "{JPN}あっ! きみの {STR_VAR_1}\nがんばリボン にあってるよ!"
	.string "$"

SlateportCity_Text_WonderIfLighthouseStartlesPokemon:
	.string "{JPN}とうだいの あかりは\nすう10キロさき にも とどくんだ\pうみの なかの ポケモン\nまぶしくて おどろいて いないかな?"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_SeaweedFullOfLife:
	.string "{JPN}ほれ みてみろ!\pこのあたりで とれる かいそうは\nしんせん ピチピチ!\pいまにも おそい かかって きそうだ!"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_HowTownIsBornAndGrows:
	.string "{JPN}みずの きれいな ところに\nたくさんの みのりが あつまり\pひとびとが やってくる ところに\nいちばが できる……\pこうして まちは おおきくなるのさ"
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_SlateportWonderfulPlace:
	.string "{JPN}うみの かおりが する ばしょで\nかいものが できるなんて……\pカイナって すてきな ところ!"
	.string ""
	.string "$"

SlateportCity_Text_BuyBricksSoDecorWontGetDirty:
	.string "{JPN}ぬいぐるみや クッションって\nゆかに おくと よごれちゃうでしょ?\pだから きれいなまま かざれるように\nぬいぐるみや クッションを おくための\lレンガを かいに きたの!"
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_GoingToCompeteInBattleTent:
	.string "{JPN}うぉー! バトルテントに\nぼくも しゅつじょう するぞおー!\pと そのまえに!\nポケモン つかまえて こなくちゃ!"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_BushedHikingFromMauville:
	.string "{JPN}ふぅ ふぅ……\pおれは キンセツシティ から\nあるいて ここまで きたんだけど\lこの まちって ひろいなぁ……\pこんな ことなら じてんしゃに\nのって くれば よかったよ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_EveryoneCallsHimCaptStern:
	.string "{JPN}この はくぶつかんを つくったのは\nクスノキさん といって\pかいていを たんさくする せんすいていの\nかんちょう でも あるんだ!\pせんすいていの かんちょうで\nはくぶつかんの かんちょう……\pまあ どっちにせよ\nクスノキかんちょう と よんでおけば\lまちがい ないよな!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_WhatsLongLineOverThere:
	.string "{JPN}なんなのかしら\nあの ぎょうれつ?"
	.string "$"

SlateportCity_Text_VisitedMuseumOften:
	.string "{JPN}この はくぶつかん には\nこどもの ころ わたしも よく きました\pかんないの てんじひんを みて\nうみの しんぴに あこがれた ものです"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_QuitPushing:
	.string "{JPN}ちょっと あんた! おさないでよ!\nじゅんばん なんだから!"
	.string "$"

SlateportCity_Text_AquaHasPolicy:
	.string "{JPN}おれたち アクアだんの ポリシーは\n「げんち しゅうごう」 そして\l「げんち かいさん」 なんだぜ!"
	.string ""
	.string "$"

SlateportCity_Text_BossIsBrilliant:
	.string "{JPN}リーダーほど あたまの いい ひとが\nいまさら はくぶつかんに\lなんの ようじが あるんだ……?"
	.string ""
	.string "$"

SlateportCity_Text_WhatsNewSchemeIWonder:
	.string "{JPN}こんどの さくせんは なんだろう?\pリーダーを おこらせると こわいから\nしっぱい したくないぜ……"
	.string ""
	.string "$"

SlateportCity_Text_ShouldTakeItAll:
	.string "{JPN}はくぶつかんに ほしいもの あるなら\nまるごと うばっちゃえば いいのに……"
	.string "$"

SlateportCity_Text_DontButtIn:
	.string "{JPN}おい こら!\nじゅんばん ぬかすなよ!"
	.string "$"

SlateportCity_Text_RemindsMeOfLongLineForGames:
	.string "{JPN}ぎょうれつ というと\nだいにんきの ゲームを かったときを\lおもいだすなあ……"
	.string ""
	.string "$"

SlateportCity_Text_WhyAreWeLiningUp:
	.string "{JPN}まじめに おかね はらわずに\nむりやり はいれば いいのによ!"
	.string "$"

SlateportCity_Text_WhatDoYouWant:
	.string "{JPN}なんだ なんだ!?$"

SlateportCity_Text_IllReadSignForYou:
	.string "{JPN}おまえ このカンバンが よみたいのか?\nなら オレが かわりに よんでやらぁ!"
	.string "$"

SlateportCity_Text_SaysSomethingLikeSeaIsEndless:
	.string "{JPN}えーと……\p『てしない それは の の』\p……って かいて あるぜ!"
	.string ""
	.string "$"

SlateportCity_Text_ShouldveBroughtMyGameBoy:
	.string "{JPN}ぶつぶつぶつ……\pゲームボーイが あれば\nぎょうれつも たいくつ しないぜ\pぶつぶつぶつ……"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_HotSpringsAfterOperation:
	.string "{JPN}このさくせんが おわったら\nおんせんに つれていって やる!\pって リーダーが いってたんだ\nたのしみ だなぁ……"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_SeaIsSoWet:
	.string "{JPN}うみって おおきいなあ……\pひょっとして うみって\nポケモンが ながした なみだで\lできてるのかなあ……"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_SinkOldBoats:
	.string "{JPN}ふるく なって のれない ふねは\nどうするか しって いるかね?\pうみの そこに しずめて\nポケモンの すみかに してやるのだよ!"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_BuyTooMuch:
	.string "{JPN}ここに くると うれしくて\nつい たくさん かいもの しちゃう!"
	.string "$"

SlateportCity_Text_GetNameRaterToHelpYou:
	.string "{JPN}ポケモンの ニックネームを かえたいなら\nせいめい はんだんしに たのむんだね!"
	.string "$"

SlateportCity_Text_CantChangeTradeMonName:
	.string "{JPN}ひとと こうかんした ポケモンは\nニックネームを かえられないわよ!\pだって つけた ひとの きもちが\nこめられて いるんですもの!"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_BattleTentBuiltRecently:
	.string "{JPN}さいきん カイナには\nバトルテントが できたんだ\lジムも いいけど バトルテントも いいぜ!\pおまえも つよい ポケモン さがしに\nいって みなよ!"
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_CaptSternBeingInterviewed:
	.string "{JPN}ゆうめいな スターが きていたら\nサインを もらうつもり だったのに\pインタビュー されてるのは\nあれ…… クスノキさん でないかい?"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_InterviewerSoCool:
	.string "{JPN}あの インタビュアーの おねえさん\nかっこよくて びじん!\pあたしも おおきく なったら\nこくさい ジャーナリストに なるわ!"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_SternSaysDiscoveredSomething:
	.string "{JPN}クスノキかんちょうは うみの そこで\nなにかを はっけん したんだって!\pなんだろなー? なんだろなー?"
	.string ""
	.string "$"

SlateportCity_Text_CaptainComeBackWithBigFish:
	.string "{JPN}なんだ なんだ!?\pかんちょうは うみの そこから\nおおもの でも つりあげたのか?"
	.string ""
	.string "$"

SlateportCity_Text_AmIOnTV:
	.string "{JPN}みんなー! みてるかー!\nおれも テレビに うつってるかー?"
	.string "$"

SlateportCity_Text_CaptainsACelebrity:
	.string "{JPN}テレビが しゅざいに くる なんて\nかんちょうも ゆうめいじん ですなあ!"
	.string "$"

SlateportCity_Text_BigSmileForCamera:
	.string "{JPN}ダイ『はい クスノキさん\nそこで えがお!"
	.string "$"

SlateportCity_Text_MostInvaluableExperience:
	.string "{JPN}マリ『なるほど なるほど それは\nきちょうな たいけんを なさいましたね……"
	.string "$"

SlateportCity_Text_SternMoveAheadWithExploration:
	.string "{JPN}クスノキ『……というわけで こんごも\nちょうさは つづけて いきますよ!"
	.string "$"

SlateportCity_Text_GabbyWonderfulThanksForInterview:
	.string "{JPN}マリ『そうですか いやあ クスノキさん!\nきょうは おいそがしい ところ\lどうも ありがとう ございました!\pまた なにか はっけんが ありましたら\nぜひ しゅざい させて くださいね!"
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_SternWhewFirstInterview:
	.string "{JPN}クスノキ『ふう……\pテレビカメラに うつされる なんて\nはじめて だったから きんちょう したよ!"
	.string ""
	.string "$"

SlateportCity_Text_OhPlayerWeMadeDiscovery:
	.string "{JPN}おっ! {PLAYER}{KUN}\nげんきそう だね!\pこんかいの かいてい ちょうさで\nすごい はっけんを したんだよ!\p128ばん すいどうで\nおおむかしに ぜつめつ したと いわれる\lポケモンの すみかと おもわれる\lかいてい どうくつを みつけた……"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_AquaWillAssumeControlOfSubmarine:
	.string "{JPN}『フフフ……\nクスノキかんちょう とやら\pきみの せんすいていは\nわれわれ アクアだんが いただく!\pきみたちは われわれに だまって\nきょうりょく すれば いいのだよ!\pフフフ……\nこれから アクアだんが なにを するか\lたのしみに していたまえ!』"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_SternWhatWasAllThat:
	.string "{JPN}クスノキ『いまの こえは……?\pだれかが かくせいきを つかって……\pどこから はなしてるんだ……?"
	.string ""
	.string "$"

SlateportCity_Text_FromHarborTryingToTakeSub:
	.string "{JPN}ふねのりばの なか!\pせんすいていが…… うばわれる……?"
	.string "$"

SlateportCity_Text_PleaseComeWithMe:
	.string "{JPN}{PLAYER}{KUN}!\nいっしょに きてくれ!"
	.string "$"

SlateportCity_Text_BattleTentSign:
	.string "{JPN}バトルテント カイナシティ かいじょう\nさがせ! さいきょう ポケモン"
	.string "$"

SlateportCity_Text_SternsShipyardWantedSign:
	.string "{JPN}「クスノキ ぞうせんじょ」\pなみを じざいに あやつる ふなのり\nだいぼしゅう してます!"
	.string ""
	.string "$"

SlateportCity_Text_SternsShipyardNearsCompletion:
	.string "{JPN}「クスノキ ぞうせんじょ」\pカイナと ミナモを むすぶ\nれんらくせん タイドリップごうは\lかんせい まで あと わずか!"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_SternsShipyardFerryComplete:
	.string "{JPN}「クスノキ ぞうせんじょ」\pれんらくせん タイドリップごうに\nじょうせんを きぼう される かたは\lふねのりば うけつけ まで どうぞ!"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_PokemonFanClubSign:
	.string "{JPN}「ポケモン だいすきクラブ」\nポケモンが すきなひと みんな あつまれ!"
	.string "$"

SlateportCity_Text_OceanicMuseumSign:
	.string "{JPN}はてしない うみ……\nそれは いのちの みなもと……\p「うみの かがく はくぶつかん」"
	.string ""
	.string "$"

SlateportCity_Text_CitySign:
	.string "{JPN}ここは カイナ シティ\nヒトと ポケモンが いきかう みなと"
	.string "$"

SlateportCity_Text_MarketSign:
	.string "{JPN}「カイナ いちば」\nここでしか かえない もの たくさん!"
	.string "$"

SlateportCity_Text_HarborFerryUnderConstruction:
	.string "{JPN}「カイナ シティ ふねのりば」\pれんらくせん タイドリップごう は\nぞうせんじょ にて けんぞう ちゅう!\pもう しばらく おまちください!"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_HarborSign:
	.string "{JPN}「カイナ シティ ふねのりば」\pれんらくせん タイドリップごうで\nふねの たびを おたのしみ ください!"
	.string ""
	.string "$"

SlateportCity_Text_NameRatersHouseSign:
	.string "{JPN}「せいめい はんだんし の いえ」\nポケモンの なまえ うらないます"
	.string "$"

SlateportCity_Text_ExplainBerries:
	.string "{JPN}きずついた やせいの ポケモンは\nきのみを かじって けがを なおすの\pしってた?\pそれを もくげきした だれかが\nきのみから くすりを つくることを\lおもいついたの\pしってた?\pきのみを くすりに するためには\nこなごなに くだいて こなにするの\pしってた?\pあなた……\nきのみに きょうみが ありそうね!\pいいものを あげるわ"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_ExplainBerryPowder:
	.string "{JPN}さいきん ポケモンセンターの 2かいの\nダイレクト コーナー というところに\lきのみを つぶす きかいを\lおいて もらったの\pしってた?\pそこで きのみの こなを つくってきて!\pきのみの こなを\nたくさん つくって きてくれたら\lいろいろな くすりを あげるわ"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_BroughtMeSomeBerryPowder:
	.string "{JPN}きのみの こなを もってきたの?$"

SlateportCity_Text_ExchangeWhatWithIt:
	.string "{JPN}なにと こうかん しましょ?$"

SlateportCity_Text_ExchangeBerryPowderForItem:
	.string "{JPN}{STR_VAR_1}と\nこうかん するのね?"
	.string "$"

SlateportCity_Text_DontHaveEnoughBerryPowder:
	.string "{JPN}あれま!?\nこなが たりない じゃないの!"
	.string "$"

SlateportCity_Text_FineBerryPowderTradeSomethingElse:
	.string "{JPN}まあ いいこな だこと!\pあなたが くれた こなで\nまた いいくすりを つくれるわ!\pほかにも なにか こうかんする?"
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_WhenYouGetMoreBringItToMe:
	.string "{JPN}そう\nこなが できたら また よろしくね!"
	.string "$"

SlateportCity_Text_ComeBackToTradeBerryPowder:
	.string "{JPN}くすりと こうかん したくなったら\nまた ここに くるのよ\pわたしが いつも\nおみせ だしてるから ね\pしってた?"
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_YouDroveTeamAquaAway:
	.string "{JPN}エニシダ『……あれ?\nきみとは まえに どこかで あったよね\pじこしょうかい したっけ?\nわたしは エニシダ って いうんだ\pいま ここから アクアだんが\nにげだして いったけど\lもしかして きみが やっつけたの かい?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_MaybeThisTrainer:
	.string "{JPN}エニシダ『うーむ これは\nひょっとして ひょっとすると……"
	.string "$"

SlateportCity_Text_LetsRegisterEachOther:
	.string "{JPN}エニシダ『……よっし!\nきみとは なかよく なれそう だから\lおたがい ポケナビに\lなまえを とうろく しておこう!\p…… …… ……"
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_RegisteredScott:
	.string "{JPN}エニシダ という ひとを\nポケナビに とうろく した!"
	.string "$"

SlateportCity_Text_KeepEyeOnTrainersBeSeeingYou:
	.string "{JPN}エニシダ『ホントは きみに\nくっついて いきたい ところ だけど\lほかのひとの さいのうも きになるし\pもうすこし いろんな まちを\nたずね あるいて みるよ!\pじゃあね {PLAYER}{KUN}!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_Text_TakingBattleTentChallenge:
	.string "{JPN}エニシダ『お おおっと\n{PLAYER}{KUN}か!\p……もしかして {PLAYER}{KUN}\nバトルテントに ちょうせん するのかい?\pうん! それは いいことだな!\nものすごく いいことだ!\lバトルテントには つよい トレーナーが\lとっても ふさわしい からね\pじゃ がんばってね!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"
