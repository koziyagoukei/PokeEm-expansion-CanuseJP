LittlerootTown_ProfessorBirchsLab_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LittlerootTown_ProfessorBirchsLab_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, LittlerootTown_ProfessorBirchsLab_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, LittlerootTown_ProfessorBirchsLab_OnFrame
	.byte 0

	@ State descriptions for VAR_DEX_UPGRADE_JOHTO_STARTER_STATE
	@ 1: Beat Elite Four, Dex upgrade ready
	@ 2: Received National Dex
	@ 3: Left lab after receiving National Dex
	@ 4: Entered lab after catching all Hoenn mons
	@ 5: Birch told player to choose Johto starter
	@ 6: Chose Johto starter

	@ State descriptions for VAR_BIRCH_LAB_STATE
	@ 1: Never occurs
	@ 2: Chose starter
	@ 3: Received starter in lab
	@ 4: Defeated rival on Route 103
	@ 5: Received pokedex

LittlerootTown_ProfessorBirchsLab_OnTransition:
	call Common_EventScript_SetupRivalGfxId
	call ProfBirch_EventScript_UpdateLocation
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, LittlerootTown_ProfessorBirchsLab_EventScript_SetAfterJohtoStarterLayout
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 4, LittlerootTown_ProfessorBirchsLab_EventScript_SetJohtoStarterLayout
	goto_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 3, LittlerootTown_ProfessorBirchsLab_EventScript_CheckReadyForJohtoStarter
	end

LittlerootTown_ProfessorBirchsLab_EventScript_CheckReadyForJohtoStarter::
	specialvar VAR_RESULT, HasAllHoennMons
	goto_if_eq VAR_RESULT, TRUE, LittlerootTown_ProfessorBirchsLab_EventScript_SetReadyForJohtoStarter
	setobjectmovementtype LOCALID_BIRCHS_LAB_RIVAL, MOVEMENT_TYPE_WANDER_UP_AND_DOWN
	setobjectxyperm LOCALID_BIRCHS_LAB_RIVAL, 5, 10
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SetReadyForJohtoStarter::
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 4
	goto LittlerootTown_ProfessorBirchsLab_EventScript_SetJohtoStarterLayout

LittlerootTown_ProfessorBirchsLab_EventScript_SetJohtoStarterLayout::
	setmaplayoutindex LAYOUT_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB_WITH_TABLE
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SetAfterJohtoStarterLayout::
	setmaplayoutindex LAYOUT_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB_WITH_TABLE
	setobjectmovementtype LOCALID_BIRCHS_LAB_RIVAL, MOVEMENT_TYPE_WANDER_UP_AND_DOWN
	setobjectxyperm LOCALID_BIRCHS_LAB_RIVAL, 5, 10
	end

LittlerootTown_ProfessorBirchsLab_OnWarp:
	map_script_2 VAR_BIRCH_LAB_STATE, 2, LittlerootTown_ProfessorBirchsLab_EventScript_SetPlayerPosForReceiveStarter
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1, LittlerootTown_ProfessorBirchsLab_EventScript_SetObjectPosForDexUpgrade
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_ProfessorBirchsLab_EventScript_SetObjectPosForDexUpgrade
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 3, LittlerootTown_ProfessorBirchsLab_EventScript_AddRivalObject
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, LittlerootTown_ProfessorBirchsLab_EventScript_AddRivalObject
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 4, LittlerootTown_ProfessorBirchsLab_EventScript_SetObjectPosForJohtoStarters
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 5, LittlerootTown_ProfessorBirchsLab_EventScript_SetObjectPosForJohtoStarters
	.2byte 0

LittlerootTown_ProfessorBirchsLab_EventScript_SetPlayerPosForReceiveStarter::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SetObjectPosForDexUpgrade::
	turnobject LOCALID_PLAYER, DIR_NORTH
	addobject LOCALID_BIRCHS_LAB_BIRCH
	addobject LOCALID_BIRCHS_LAB_RIVAL
	turnobject LOCALID_BIRCHS_LAB_RIVAL, DIR_NORTH
	setobjectxy LOCALID_BIRCHS_LAB_RIVAL, 5, 5
	turnobject LOCALID_BIRCHS_LAB_BIRCH, DIR_SOUTH
	setobjectxy LOCALID_BIRCHS_LAB_BIRCH, 6, 4
	turnobject LOCALID_BIRCHS_LAB_AIDE, DIR_EAST
	setobjectxy LOCALID_BIRCHS_LAB_AIDE, 10, 10
	end

LittlerootTown_ProfessorBirchsLab_EventScript_AddRivalObject::
	addobject LOCALID_BIRCHS_LAB_RIVAL
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SetObjectPosForJohtoStarters::
	addobject LOCALID_BIRCHS_LAB_CYNDAQUIL_BALL
	addobject LOCALID_BIRCHS_LAB_TOTODILE_BALL
	addobject LOCALID_BIRCHS_LAB_CHIKORITA_BALL
	addobject LOCALID_BIRCHS_LAB_RIVAL
	turnobject LOCALID_BIRCHS_LAB_CYNDAQUIL_BALL, DIR_SOUTH
	setobjectxy LOCALID_BIRCHS_LAB_CYNDAQUIL_BALL, 8, 4
	turnobject LOCALID_BIRCHS_LAB_TOTODILE_BALL, DIR_SOUTH
	setobjectxy LOCALID_BIRCHS_LAB_TOTODILE_BALL, 9, 4
	turnobject LOCALID_BIRCHS_LAB_CHIKORITA_BALL, DIR_SOUTH
	setobjectxy LOCALID_BIRCHS_LAB_CHIKORITA_BALL, 10, 4
	turnobject LOCALID_BIRCHS_LAB_BIRCH, DIR_SOUTH
	setobjectxy LOCALID_BIRCHS_LAB_BIRCH, 6, 4
	turnobject LOCALID_BIRCHS_LAB_RIVAL, DIR_EAST
	setobjectxy LOCALID_BIRCHS_LAB_RIVAL, 5, 5
	end

LittlerootTown_ProfessorBirchsLab_OnFrame:
	map_script_2 VAR_BIRCH_LAB_STATE, 2, LittlerootTown_ProfessorBirchsLab_EventScript_GiveStarterEvent
	map_script_2 VAR_BIRCH_LAB_STATE, 4, LittlerootTown_ProfessorBirchsLab_EventScript_GivePokedexEvent
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1, LittlerootTown_ProfessorBirchsLab_EventScript_UpgradeToNationalDex
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 4, LittlerootTown_ProfessorBirchsLab_EventScript_ChooseJohtoStarter
	.2byte 0

@ The starter is technically given prior to this on Route 101 by special ChooseStarter
@ This is just where the game tells you its yours and lets you nickname it
LittlerootTown_ProfessorBirchsLab_EventScript_GiveStarterEvent::
	lockall
	bufferleadmonspeciesname STR_VAR_1
	message LittlerootTown_ProfessorBirchsLab_Text_LikeYouToHavePokemon
	waitmessage
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	msgbox LittlerootTown_ProfessorBirchsLab_Text_WhyNotGiveNicknameToMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LittlerootTown_ProfessorBirchsLab_EventScript_NicknameStarter
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_GoSeeRival
	end

LittlerootTown_ProfessorBirchsLab_EventScript_NicknameStarter::
	setvar VAR_0x8004, 0
	call Common_EventScript_NameReceivedPartyMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_GoSeeRival
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GoSeeRival::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MightBeGoodIdeaToGoSeeRival, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LittlerootTown_ProfessorBirchsLab_EventScript_AgreeToSeeRival
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_DeclineSeeingRival
	end

LittlerootTown_ProfessorBirchsLab_EventScript_AgreeToSeeRival::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_GetRivalToTeachYou, MSGBOX_DEFAULT
	clearflag FLAG_HIDE_ROUTE_101_BOY
	setvar VAR_BIRCH_LAB_STATE, 3
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_DeclineSeeingRival::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_DontBeThatWay, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LittlerootTown_ProfessorBirchsLab_EventScript_AgreeToSeeRival
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_DeclineSeeingRival
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GivePokedexEvent::
	lockall
	applymovement LOCALID_PLAYER, LittlerootTown_ProfessorBirchsLab_Movement_PlayerEnterLabForPokedex
	waitmovement 0
	goto LittlerootTown_ProfessorBirchsLab_EventScript_GivePokedex
	end

LittlerootTown_ProfessorBirchsLab_Movement_PlayerEnterLabForPokedex:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_ProfessorBirchsLab_EventScript_UpgradeToNationalDex::
	lockall
	delay 30
	msgbox LittlerootTown_ProfessorBirchsLab_Text_OtherRegionsUpgradeToNational, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BIRCHS_LAB_BIRCH, LittlerootTown_ProfessorBirchsLab_Movement_BirchRetrievePokedexes
	waitmovement 0
	delay 20
	playse SE_CLICK
	delay 10
	playse SE_CLICK
	delay 10
	playse SE_CLICK
	delay 10
	playse SE_CLICK
	delay 20
	applymovement LOCALID_BIRCHS_LAB_RIVAL, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_ProfessorBirchsLab_EventScript_MayUpgradeComment
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanUpgradeComment
	playse SE_PC_ON
	waitse
	delay 20
	applymovement LOCALID_BIRCHS_LAB_BIRCH, LittlerootTown_ProfessorBirchsLab_Movement_BirchReturnPokedex
	waitmovement 0
	applymovement LOCALID_BIRCHS_LAB_RIVAL, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox LittlerootTown_ProfessorBirchsLab_Text_OkayAllDone, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message LittlerootTown_ProfessorBirchsLab_Text_PokedexUpgradedToNational
	waitmessage
	waitfanfare
	setflag FLAG_SYS_NATIONAL_DEX
	special EnableNationalPokedex
	msgbox LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting2, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BIRCH
	setflag FLAG_HIDE_LITTLEROOT_TOWN_RIVAL
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2
	setvar VAR_SCOTT_BF_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_BATTLE_FRONTIER
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayUpgradeComment::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MayUpgradeSoCool, MSGBOX_DEFAULT
	closemessage
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanUpgradeComment::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BrendanYouCanThankMe, MSGBOX_DEFAULT
	closemessage
	return

LittlerootTown_ProfessorBirchsLab_Movement_BirchRetrievePokedexes:
	walk_left
	walk_in_place_faster_down
	delay_16
	delay_16
	walk_right
	walk_in_place_faster_down
	delay_16
	delay_16
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

LittlerootTown_ProfessorBirchsLab_Movement_BirchReturnPokedex:
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_in_place_faster_down
	step_end

LittlerootTown_ProfessorBirchsLab_EventScript_ChooseJohtoStarter::
	lockall
	applymovement LOCALID_PLAYER, LittlerootTown_ProfessorBirchsLab_Movement_PlayerEnterLabForJohtoStarter
	waitmovement 0
	msgbox LittlerootTown_ProfessorBirchsLab_Text_CompletedDexChoosePokemon, MSGBOX_DEFAULT
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 5
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_Movement_PlayerEnterLabForJohtoStarter:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_ProfessorBirchsLab_EventScript_Aide::
	lock
	faceplayer
	goto_if_ge VAR_BIRCH_LAB_STATE, 3, LittlerootTown_ProfessorBirchsLab_EventScript_AideReceivedStarter
	goto_if_set FLAG_BIRCH_AIDE_MET, LittlerootTown_ProfessorBirchsLab_EventScript_AideAlreadyMet
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BirchAwayOnFieldwork, MSGBOX_DEFAULT
	setflag FLAG_BIRCH_AIDE_MET
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_AideAlreadyMet::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BirchIsntOneForDeskWork, MSGBOX_DEFAULT
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_AideReceivedStarter::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BirchEnjoysRivalsHelpToo, MSGBOX_DEFAULT
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Cyndaquil::
	release
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, LittlerootTown_ProfessorBirchsLab_EventScript_AlreadyChoseJohtoStarter
	applymovement LOCALID_BIRCHS_LAB_BIRCH, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	showmonpic SPECIES_CYNDAQUIL, 10, 3
	msgbox LittlerootTown_ProfessorBirchsLab_Text_YoullTakeCyndaquil, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_TakeYourTime
	goto LittlerootTown_ProfessorBirchsLab_EventScript_GiveCyndaquil
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Totodile::
	release
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, LittlerootTown_ProfessorBirchsLab_EventScript_AlreadyChoseJohtoStarter
	applymovement LOCALID_BIRCHS_LAB_BIRCH, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	showmonpic SPECIES_TOTODILE, 10, 3
	msgbox LittlerootTown_ProfessorBirchsLab_Text_YoullTakeTotodile, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_TakeYourTime
	goto LittlerootTown_ProfessorBirchsLab_EventScript_GiveTotodile
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Chikorita::
	release
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, LittlerootTown_ProfessorBirchsLab_EventScript_AlreadyChoseJohtoStarter
	applymovement LOCALID_BIRCHS_LAB_BIRCH, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	showmonpic SPECIES_CHIKORITA, 10, 3
	msgbox LittlerootTown_ProfessorBirchsLab_Text_YoullTakeChikorita, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_TakeYourTime
	goto LittlerootTown_ProfessorBirchsLab_EventScript_GiveChikorita
	end

LittlerootTown_ProfessorBirchsLab_EventScript_AlreadyChoseJohtoStarter::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BetterLeaveOthersAlone, MSGBOX_DEFAULT
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_TakeYourTime::
	hidemonpic
	msgbox LittlerootTown_ProfessorBirchsLab_Text_TakeYourTimeAllInvaluable, MSGBOX_DEFAULT
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GiveCyndaquil::
	bufferspeciesname STR_VAR_1, SPECIES_CYNDAQUIL
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_CYNDAQUIL
	givemon SPECIES_CYNDAQUIL, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, LittlerootTown_ProfessorBirchsLab_EventScript_SendCyndaquilToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, LittlerootTown_ProfessorBirchsLab_EventScript_SendCyndaquilToPC
	hidemonpic
	goto Common_EventScript_NoMoreRoomForPokemon
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendCyndaquilToParty::
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter
	removeobject LOCALID_BIRCHS_LAB_CYNDAQUIL_BALL
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedCyndaquil
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedCyndaquil
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendCyndaquilToPC::
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter
	removeobject LOCALID_BIRCHS_LAB_CYNDAQUIL_BALL
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_CyndaquilTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_CyndaquilTransferredToPC
	end

LittlerootTown_ProfessorBirchsLab_EventScript_CyndaquilTransferredToPC::
	call Common_EventScript_TransferredToPC
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedCyndaquil
	end

LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedCyndaquil::
	hidemonpic
	msgbox LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting, MSGBOX_DEFAULT
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GiveTotodile::
	bufferspeciesname STR_VAR_1, SPECIES_TOTODILE
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_TOTODILE
	givemon SPECIES_TOTODILE, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, LittlerootTown_ProfessorBirchsLab_EventScript_SendTotodileToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, LittlerootTown_ProfessorBirchsLab_EventScript_SendTotodileToPC
	hidemonpic
	goto Common_EventScript_NoMoreRoomForPokemon
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendTotodileToParty::
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter
	removeobject LOCALID_BIRCHS_LAB_TOTODILE_BALL
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedTotodile
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedTotodile
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendTotodileToPC::
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter
	removeobject LOCALID_BIRCHS_LAB_TOTODILE_BALL
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_TotodileTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_TotodileTransferredToPC
	end

LittlerootTown_ProfessorBirchsLab_EventScript_TotodileTransferredToPC::
	call Common_EventScript_TransferredToPC
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedTotodile
	end

LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedTotodile::
	hidemonpic
	msgbox LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting, MSGBOX_DEFAULT
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GiveChikorita::
	bufferspeciesname STR_VAR_1, SPECIES_CHIKORITA
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_CHIKORITA
	givemon SPECIES_CHIKORITA, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, LittlerootTown_ProfessorBirchsLab_EventScript_SendChikoritaToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, LittlerootTown_ProfessorBirchsLab_EventScript_SendChikoritaToPC
	hidemonpic
	goto Common_EventScript_NoMoreRoomForPokemon
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendChikoritaToParty::
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter
	removeobject LOCALID_BIRCHS_LAB_CHIKORITA_BALL
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedChikorita
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedChikorita
	end

LittlerootTown_ProfessorBirchsLab_EventScript_SendChikoritaToPC::
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter
	removeobject LOCALID_BIRCHS_LAB_CHIKORITA_BALL
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LittlerootTown_ProfessorBirchsLab_EventScript_ChikoritaTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ChikoritaTransferredToPC
	end

LittlerootTown_ProfessorBirchsLab_EventScript_ChikoritaTransferredToPC::
	call Common_EventScript_TransferredToPC
	goto LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedChikorita
	end

LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedChikorita::
	hidemonpic
	msgbox LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting, MSGBOX_DEFAULT
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_ReceivedJohtoStarter::
	playfanfare MUS_OBTAIN_ITEM
	message LittlerootTown_ProfessorBirchsLab_Text_ReceivedJohtoStarter
	waitmessage
	waitfanfare
	return

LittlerootTown_ProfessorBirchsLab_EventScript_Birch::
	lock
	faceplayer
	goto_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 5, LittlerootTown_ProfessorBirchsLab_EventScript_CanHaveAnyOneOfRarePokemon
	goto_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_ProfessorBirchsLab_EventScript_GrassyPatchWaiting
	goto_if_unset FLAG_HAS_MATCH_CALL, LittlerootTown_ProfessorBirchsLab_EventScript_TryRatePokedexOrRegister
	goto_if_unset FLAG_ENABLE_PROF_BIRCH_MATCH_CALL, EventScript_RegisterProfBirch
	goto LittlerootTown_ProfessorBirchsLab_EventScript_TryRatePokedexOrRegister
	end

LittlerootTown_ProfessorBirchsLab_EventScript_CanHaveAnyOneOfRarePokemon::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_CanHaveAnyOneOfRarePokemon, MSGBOX_DEFAULT
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GrassyPatchWaiting::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting2, MSGBOX_DEFAULT
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_TryRatePokedexOrRegister::
	goto_if_unset FLAG_HIDE_LITTLEROOT_TOWN_BIRCHS_LAB_UNKNOWN_0x380, ProfBirch_EventScript_RatePokedexOrRegister
	goto_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 3, ProfBirch_EventScript_RatePokedexOrRegister
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, ProfBirch_EventScript_RatePokedexOrRegister
	goto_if_eq VAR_BIRCH_LAB_STATE, 5, LittlerootTown_ProfessorBirchsLab_EventScript_PokemonAwait
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BirchRivalGoneHome, MSGBOX_DEFAULT
	release
	end

EventScript_RegisterProfBirch::
	msgbox MatchCall_Text_BirchRegisterCall, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox MatchCall_Text_RegisteredBirch, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_PROF_BIRCH_MATCH_CALL
	setvar VAR_REGISTER_BIRCH_STATE, 2
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_GivePokedex::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_HeardYouBeatRivalTakePokedex, MSGBOX_DEFAULT
	call LittlerootTown_ProfessorBirchsLab_EventScript_ReceivePokedex
	msgbox LittlerootTown_ProfessorBirchsLab_Text_ExplainPokedex, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BIRCHS_LAB_RIVAL, LittlerootTown_ProfessorBirchsLab_Movement_RivalApproachPlayer
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_ProfessorBirchsLab_EventScript_MayGivePokeBalls
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanGivePokeBalls
	setvar VAR_BIRCH_LAB_STATE, 5
	setflag FLAG_ADVENTURE_STARTED
	setvar VAR_OLDALE_TOWN_STATE, 1
	setvar VAR_LITTLEROOT_RIVAL_STATE, 4
	setvar VAR_LITTLEROOT_TOWN_STATE, 3
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayGivePokeBalls::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MayGotPokedexTooTakeThese, MSGBOX_DEFAULT
	giveitem ITEM_POKE_BALL, 5
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_MayNoRoomForPokeBalls
	msgbox LittlerootTown_ProfessorBirchsLab_Text_CatchCutePokemonWithPokeBalls, MSGBOX_DEFAULT
	setvar VAR_RESULT, 0
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanGivePokeBalls::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BrendanGotPokedexTooTakeThese, MSGBOX_DEFAULT
	giveitem ITEM_POKE_BALL, 5
	call_if_eq VAR_RESULT, FALSE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanNoRoomForPokeBalls
	msgbox LittlerootTown_ProfessorBirchsLab_Text_CatchCoolPokemonWithPokeBalls, MSGBOX_DEFAULT
	setvar VAR_RESULT, 1
	return

LittlerootTown_ProfessorBirchsLab_EventScript_ReceivePokedex::
	playfanfare MUS_OBTAIN_ITEM
	message LittlerootTown_ProfessorBirchsLab_Text_ReceivedPokedex
	waitfanfare
	setflag FLAG_SYS_POKEDEX_GET
	special SetUnlockedPokedexFlags
	setflag FLAG_RECEIVED_POKEDEX_FROM_BIRCH
	setvar VAR_CABLE_CLUB_TUTORIAL_STATE, 1
	return

LittlerootTown_ProfessorBirchsLab_EventScript_PokemonAwait::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_CountlessPokemonAwait, MSGBOX_DEFAULT
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayNoRoomForPokeBalls::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_OhYourBagsFull, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanNoRoomForPokeBalls::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_HeyYourBagsFull, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_Movement_RivalApproachPlayer:
	walk_down
	walk_in_place_faster_left
	step_end

LittlerootTown_ProfessorBirchsLab_EventScript_Machine::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_SeriousLookingMachine, MSGBOX_SIGN
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Rival::
	lock
	faceplayer
	goto_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 5, LittlerootTown_ProfessorBirchsLab_EventScript_RivalFuturePlans
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6, LittlerootTown_ProfessorBirchsLab_EventScript_RivalHaveYouGoneToBattleFrontier
	goto_if_ge VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_ProfessorBirchsLab_EventScript_RivalTakeBreakFromFieldwork
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_ProfessorBirchsLab_EventScript_MayWhereShouldIGoNext
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanWhereShouldIGoNext
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayWhereShouldIGoNext::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MayWhereShouldIGoNext, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanWhereShouldIGoNext::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BrendanWhereShouldIGoNext, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_RivalFuturePlans::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_ProfessorBirchsLab_EventScript_MayWhatNextImStayingHere
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanPreferCollectingSlowly
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayWhatNextImStayingHere::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MayWhatNextImStayingHere, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanPreferCollectingSlowly:
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BrendanPreferCollectingSlowly, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_RivalHaveYouGoneToBattleFrontier::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_ProfessorBirchsLab_EventScript_MayHaveYouGoneToBattleFrontier
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanHaveYouGoneToBattleFrontier
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayHaveYouGoneToBattleFrontier::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MayHaveYouGoneToBattleFrontier, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanHaveYouGoneToBattleFrontier::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BrendanHaveYouGoneToBattleFrontier, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_RivalTakeBreakFromFieldwork::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_ProfessorBirchsLab_EventScript_MayTakeBreakFromFieldwork
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_ProfessorBirchsLab_EventScript_BrendanTakeBreakFromFieldwork
	release
	end

LittlerootTown_ProfessorBirchsLab_EventScript_MayTakeBreakFromFieldwork::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_MayTakeBreakFromFieldwork, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_BrendanTakeBreakFromFieldwork::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BrendanTakeBreakFromFieldwork, MSGBOX_DEFAULT
	return

LittlerootTown_ProfessorBirchsLab_EventScript_PC::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_PCUsedForResearch, MSGBOX_SIGN
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Bookshelf::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_CrammedWithBooksOnPokemon, MSGBOX_SIGN
	end

LittlerootTown_ProfessorBirchsLab_EventScript_Book::
	msgbox LittlerootTown_ProfessorBirchsLab_Text_BookTooHardToRead, MSGBOX_SIGN
	end

LittlerootTown_ProfessorBirchsLab_EventScript_ScottAboardSSTidalCall::
	lockall
	pokenavcall LittlerootTown_ProfessorBirchsLab_Text_ScottAboardSSTidalCall
	waitmessage
	clearflag FLAG_SCOTT_CALL_BATTLE_FRONTIER
	releaseall
	end

LittlerootTown_ProfessorBirchsLab_Text_BirchAwayOnFieldwork:
	.string "{JPN}? オダマキはかせ?\p"
	.string "{JPN}はかせ なら フィールドワークに\n"
	.string "{JPN}でかけていて いませんよ\p"
	.string "{JPN}……あ フィールドワークって いうのは\p"
	.string "{JPN}じっさいに のやまへ でかけていって\n"
	.string "{JPN}けんきゅうを すること ですね!\p"
	.string "{JPN}はかせは つくえのうえで けんきゅうを\n"
	.string "{JPN}するよりも じっさいに そとへ とびだして\l"
	.string "{JPN}たいけんする タイプのひと なんです!$"

LittlerootTown_ProfessorBirchsLab_Text_BirchIsntOneForDeskWork:
	.string "{JPN}はかせは つくえのうえで けんきゅうを\n"
	.string "{JPN}するよりも じっさいに そとへ とびだして\l"
	.string "{JPN}たいけんする タイプのひと なんです!$"

LittlerootTown_ProfessorBirchsLab_Text_BirchEnjoysRivalsHelpToo:
	.string "{JPN}オダマキはかせは\n"
	.string "{JPN}ポケモンが どこに すんでいるのか\l"
	.string "{JPN}ぶんぷの ちょうさを なさっています\p
	.string "{JPN}{RIVAL}さんは すきな おとうさんの\n"
	.string "{JPN}ポケモンが どこに すんでいるのか\l"
	.string "{JPN}おてつだいを なさって いるのですよ$"

LittlerootTown_ProfessorBirchsLab_Text_LikeYouToHavePokemon:
	.string "{JPN}オダマキ『……さて {PLAYER}{KUN}!\p"
	.string "{JPN}きみのことは おとうさん から\n"
	.string "{JPN}いつも きかされていたよ!\p"
	.string "{JPN}きみは まだ じぶんの ポケモンを\n"
	.string "{JPN}もったことが ないんだって?\p"
	.string "{JPN}それにしては さっきの たたかいぶり\n"
	.string "{JPN}なかなか みごと だったよ!\p"
	.string "{JPN}やっぱり きみには おとうさんの\n"
	.string "{JPN}ちが ながれて いるんだな!\p"
	.string "{JPN}そうそう! たすけてくれた おれいに\n"
	.string "{JPN}さっきの ポケモンは わたしから\l"
	.string "{JPN}きみへの プレゼントに しよう!\p"
	.string "{JPN}{PLAYER}は\n"
	.string "{JPN}{STR_VAR_1}を もらった!$"

LittlerootTown_ProfessorBirchsLab_Text_WhyNotGiveNicknameToMon:
	.string "{JPN}オダマキ『せっかく だから {STR_VAR_1}に\n"
	.string "{JPN}ニックネームを つけてみては どうだい?$"

LittlerootTown_ProfessorBirchsLab_Text_MightBeGoodIdeaToGoSeeRival:
	.string "{JPN}オダマキ『これから さらに\n"
	.string "{JPN}けいけんを つんで いけば\l"
	.string "{JPN}いい トレーナーに なれそうだ!\p"
	.string "{JPN}ちょうど うちの {RIVAL}も\n"
	.string "{JPN}わたしを てつだい ながら\l"
	.string "{JPN}ポケモンの ちょうさを しているんだ\p"
	.string "{JPN}{PLAYER}{KUN} いちど\n"
	.string "{JPN}あいに いったら どうだろう?$"

LittlerootTown_ProfessorBirchsLab_Text_GetRivalToTeachYou:
	.string "{JPN}オダマキ『そうか!\n"
	.string "{JPN}それは {RIVAL}も よろこぶぞ!\p"
	.string "{JPN}トレーナーが どんな ものか\n"
	.string "{JPN}おしえてもらうと いいぞ!$"

LittlerootTown_ProfessorBirchsLab_Text_DontBeThatWay:
	.string "{JPN}オダマキ『そんなこと いわずに\n"
	.string "{JPN}あいに いったら どうだろう?$"

LittlerootTown_ProfessorBirchsLab_Text_BirchRivalGoneHome:
	.string "{JPN}オダマキ『{RIVAL} だったら\n"
	.string "{JPN}いえに かえってるんじゃ ないのか?\p"
	.string "{JPN}それとも また どこかの くさむらに\n"
	.string "{JPN}もぐりこんで いるのかも……\p"
	.string "{JPN}もしも きみの ポケモンが キズついたら\n"
	.string "{JPN}じぶんの いえで やすむと いいぞ!$"

LittlerootTown_ProfessorBirchsLab_Text_HeardYouBeatRivalTakePokedex:
	.string "{JPN}オダマキ『おっ {PLAYER}{KUN}!\p"
	.string "{JPN}はじめてで {RIVAL}に\n"
	.string "{JPN}かつ だなんて すごいじゃないか!\p"
	.string "{JPN}{RIVAL}は かなり まえから\n"
	.string "{JPN}わたしの けんきゅうを てつだっていて\l"
	.string "{JPN}トレーナーれきは けっこう ながいんだよ!\p"
	.string "{JPN}うむ! {PLAYER}{KUN} にも\n"
	.string "{JPN}けんきゅうの ために とりよせた\l"
	.string "{JPN}この ポケモンずかんを あげよう!$"

LittlerootTown_ProfessorBirchsLab_Text_ReceivedPokedex:
	.string "{JPN}{PLAYER}は\n"
	.string "{JPN}ポケモンずかんを もらった!$"

LittlerootTown_ProfessorBirchsLab_Text_ExplainPokedex:
	.string "{JPN}オダマキ『その ポケモンずかんは\n"
	.string "{JPN}きみが であった ポケモンを\p"
	.string "{JPN}じどうてきに きろく していってくれる\n"
	.string "{JPN}ハイテクな どうぐ なんだ!\p"
	.string "{JPN}{RIVAL}も その ずかんを もっていて\n"
	.string "{JPN}あちこちに あそびに いくんだな\p"
	.string "{JPN}で めずらしい ポケモンを つかまえて\n"
	.string "{JPN}ずかんに きろく できるとだね\p"
	.string "{JPN}フィールドワークを している わたしを\n"
	.string "{JPN}さがしだしては みせて くれるんだよ!$"

LittlerootTown_ProfessorBirchsLab_Text_CountlessPokemonAwait:
	.string "{JPN}オダマキ『きみの ゆくてには\n"
	.string "{JPN}たくさんの ポケモンが まっているよ!\p"
	.string "{JPN}あー はやく フィールドワークに\n"
	.string "{JPN}いきたく なってきたぞ!$"

LittlerootTown_ProfessorBirchsLab_Text_MayGotPokedexTooTakeThese:
	.string "{JPN}ハルカ『{PLAYER}{KUN}も\n"
	.string "{JPN}ポケモンずかん もらったんだね!\p"
	.string "{JPN}よかったね! あたしと いっしょだよ!\n"
	.string "{JPN}じゃあ あたしは これを あげる!$"

LittlerootTown_ProfessorBirchsLab_Text_CatchCutePokemonWithPokeBalls:
	.string "{JPN}ハルカ『ポケモンが\n"
	.string "{JPN}たくさん いると たのしいよ!\p"
	.string "{JPN}あたし いろんな ポケモン さがしたいから\n"
	.string "{JPN}いろんな ところを さがすわ!\p"
	.string "{JPN}そして かわいい ポケモンが いたら\n"
	.string "{JPN}モンスターボールで つかまえちゃう!$"

LittlerootTown_ProfessorBirchsLab_Text_OhYourBagsFull:
	.string "{JPN}あら? バッグ いっぱいだよ$"

LittlerootTown_ProfessorBirchsLab_Text_MayWhereShouldIGoNext:
	.string "{JPN}ハルカ『こんどは どこまで\n"
	.string "{JPN}ポケモンを さがしに いこうかな?$"

LittlerootTown_ProfessorBirchsLab_Text_BrendanGotPokedexTooTakeThese:
	.string "{JPN}ユウキ『ふうん {PLAYER}も\n"
	.string "{JPN}ポケモンずかん もらったのか\p"
	.string "{JPN}じゃあ オレは これ やるよ$"

LittlerootTown_ProfessorBirchsLab_Text_CatchCoolPokemonWithPokeBalls:
	.string "{JPN}ユウキ『ポケモンが\n"
	.string "{JPN}たくさん いると たのしいからな\p"
	.string "{JPN}オレ いろんな ポケモン さがすため\n"
	.string "{JPN}いろんな ところに いくんだ\p"
	.string "{JPN}そして かっこいい ポケモンが いたら\n"
	.string "{JPN}モンスターボールで つかまえるんだ$"

LittlerootTown_ProfessorBirchsLab_Text_HeyYourBagsFull:
	.string "{JPN}おや? バッグ いっぱいだよ$"

LittlerootTown_ProfessorBirchsLab_Text_BrendanWhereShouldIGoNext:
	.string "{JPN}ユウキ『こんどは どこに\n"
	.string "{JPN}ポケモン さがしに いこうかな……$"

LittlerootTown_ProfessorBirchsLab_Text_SeriousLookingMachine:
	.string "{JPN}なんか すごそうな マシンだ!\n"
	.string "{JPN}はかせが けんきゅうに つかうのかな?$"

LittlerootTown_ProfessorBirchsLab_Text_PCUsedForResearch:
	.string "{JPN}けんきゅうに つかわれている パソコンだ!\n"
	.string "{JPN}…… さわるのは やめておこう$"

LittlerootTown_ProfessorBirchsLab_Text_CrammedWithBooksOnPokemon:
	.string "{JPN}ポケモンに ついて かかれた\n"
	.string "{JPN}ほんが いっぱい ある!$"

LittlerootTown_ProfessorBirchsLab_Text_BookTooHardToRead:
	.string "{JPN}むずかしそうな ほんが おいてある!$"

LittlerootTown_ProfessorBirchsLab_Text_OtherRegionsUpgradeToNational:
	.string "{JPN}オダマキ『……さて\p"
	.string "{JPN}{PLAYER}{KUN}と {RIVAL}に ポケモンの\n"
	.string "{JPN}ちょうさを おねがい していた わけだが\p"
	.string "{JPN}きみたちの どりょくの おかげで\n"
	.string "{JPN}いろんな ことが わかってきた!\p"
	.string "{JPN}どうやら ホウエンちほう には\n"
	.string "{JPN}それいがいの ちほうの ポケモンも\l"
	.string "{JPN}せいそく している らしいんだ!\p"
	.string "{JPN}それだけ ホウエンちほうは\n"
	.string "{JPN}ゆたかな しぜんに めぐまれている\l"
	.string "{JPN}という ことなんだろうな!\p"
	.string "{JPN}というわけで きみたちの ずかんを\n"
	.string "{JPN}ぜんこくばんに バージョンアップ\l"
	.string "{JPN}してあげようと おもう!\p"
	.string "{JPN}さあ ずかんを かしておくれ!$"

LittlerootTown_ProfessorBirchsLab_Text_MayUpgradeSoCool:
	.string "{JPN}ハルカ『えへへー!\p"
	.string "{JPN}あたしの ずかんまで\n"
	.string "{JPN}ぜんこくばんに してもらえる なんて\p"
	.string "{JPN}{PLAYER}{KUN}が たくさん ポケモンを\n"
	.string "{JPN}しらべてくれた おかげだね!$"

LittlerootTown_ProfessorBirchsLab_Text_BrendanYouCanThankMe:
	.string "{JPN}ユウキ『オレ はりきって ホウエンじゅうの\n"
	.string "{JPN}ポケモンのこと しらべたからな\p"
	.string "{JPN}{PLAYER}も ずかんを ぜんこくばんに\n"
	.string "{JPN}してもらえて ラッキーだったな!$"

LittlerootTown_ProfessorBirchsLab_Text_OkayAllDone:
	.string "{JPN}オダマキ『ホイ できたぞ!$"

LittlerootTown_ProfessorBirchsLab_Text_PokedexUpgradedToNational:
	.string "{JPN}{PLAYER}の ポケモンずかんが\n"
	.string "{JPN}ぜんこくばんに バージョンアップ した!$"

LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting2:
	.string "{JPN}オダマキ『いいかい?\n"
	.string "{JPN}チャンピオンに なったから といって\l"
	.string "{JPN}ポケモンの みちに おわりは ないんだ\p"
	.string "{JPN}まだ あしを ふみいれていない くさむらは\n"
	.string "{JPN}いつまでも きみを まっているよ!$"

LittlerootTown_ProfessorBirchsLab_Text_MayTakeBreakFromFieldwork:
	.string "{JPN}ハルカ『さーて フィールドワークは\n"
	.string "{JPN}ちょっとだけ おやすみして\p"
	.string "{JPN}しばらくは ここで\n"
	.string "{JPN}はかせの おてつだいでも しよっかなー$"

LittlerootTown_ProfessorBirchsLab_Text_BrendanTakeBreakFromFieldwork:
	.string "{JPN}ユウキ『オレは とりあえず\n"
	.string "{JPN}フィールドワークは おやすみだな!\p"
	.string "{JPN}しばらくは ここで\n"
	.string "{JPN}はかせの てつだいでも してるよ!$"

LittlerootTown_ProfessorBirchsLab_Text_CompletedDexChoosePokemon:
	.string "{JPN}オダマキ『おお {PLAYER}{KUN}!\n"
	.string "{JPN}ちょっと ずかんを みせて もらうよ\p"
	.string "{JPN}…… …… ……\n"
	.string "{JPN}…… …… ……\p"
	.string "{JPN}……うむ やはり まちがいない!\n"
	.string "{JPN}キミは ほんとうに ホウエンちほうの\l"
	.string "{JPN}ずかんを かんせい させたんだな!\p"
	.string "{JPN}いやー たいしたもんだ!\p"
	.string "{JPN}きみの その すばらしい はたらきに\n"
	.string "{JPN}かんしゃの きもちを こめて\l"
	.string "{JPN}わたしから プレゼントを おくろう!\p"
	.string "{JPN}それは……\p"
	.string "{JPN}ほかの ちほうでしか みつからない\n"
	.string "{JPN}めずらしい ポケモンだよ!\p"
	.string "{JPN}3ひきの なかから\n"
	.string "{JPN}すきなものを 1ひきだけ あげよう!$"

LittlerootTown_ProfessorBirchsLab_Text_CanHaveAnyOneOfRarePokemon:
	.string "{JPN}オダマキ『ほかの ちほうでしか\n"
	.string "{JPN}みつからない めずらしい ポケモンだよ!\p"
	.string "{JPN}3ひきの なかから\n"
	.string "{JPN}すきなものを 1ひきだけ あげよう!$"

LittlerootTown_ProfessorBirchsLab_Text_YoullTakeCyndaquil:
	.string "{JPN}オダマキ『ほのおの ポケモン ヒノアラシに\n"
	.string "{JPN}めを つけるとは やはり するどいな!\p"
	.string "{JPN}じゃあ ヒノアラシで いいんだね?$"

LittlerootTown_ProfessorBirchsLab_Text_YoullTakeTotodile:
	.string "{JPN}オダマキ『みずの ポケモン ワニノコか\n"
	.string "{JPN}うーむ いいものを えらぶじゃないか!\p"
	.string "{JPN}よし ワニノコで いいんだね?$"

LittlerootTown_ProfessorBirchsLab_Text_YoullTakeChikorita:
	.string "{JPN}オダマキ『くさの ポケモン チコリータか\n"
	.string "{JPN}さすがだ わかってるなあ!\p"
	.string "{JPN}ならば チコリータで けっていだね?$"

LittlerootTown_ProfessorBirchsLab_Text_TakeYourTimeAllInvaluable:
	.string "{JPN}オダマキ『じっくり かんがえて\n"
	.string "{JPN}きめると いいよ\l"
	.string "{JPN}どれも きちょうな ポケモン だからね$"

@ Unused
LittlerootTown_ProfessorBirchsLab_Text_PickedFinePokemon:
	.string "{JPN}オダマキ『なるほど!\n"
	.string "{JPN}いい ポケモンを えらんだね!$"

LittlerootTown_ProfessorBirchsLab_Text_ReceivedJohtoStarter:
	.string "{JPN}{PLAYER}は オダマキはかせ から\n"
	.string "{JPN}{STR_VAR_1}を もらった!$"

@ Unused
LittlerootTown_ProfessorBirchsLab_Text_NicknameJohtoStarter:
	.string "{JPN}もらった {STR_VAR_1}に\n"
	.string "{JPN}ニックネームを つけますか?$"

LittlerootTown_ProfessorBirchsLab_Text_GrassyPatchWaiting:
	.string "{JPN}オダマキ『{PLAYER}{KUN} いいかい?\n"
	.string "{JPN}ホウエンの ずかんが かんせい しても\l"
	.string "{JPN}ポケモンの みちに おわりは ないんだ\p"
	.string "{JPN}まだ あしを ふみいれていない くさむらは\n"
	.string "{JPN}いつまでも きみを まっているよ!$"

LittlerootTown_ProfessorBirchsLab_Text_BetterLeaveOthersAlone:
	.string "{JPN}やくそくの 1ひきは もう もらったから\n"
	.string "{JPN}のこりには さわらないで おこう!$"

@ Unused
LittlerootTown_ProfessorBirchsLab_Text_DontHaveAnyRoomForPokemon:
	.string "{JPN}おや? それいじょう\n"
	.string "{JPN}ポケモンを もてない みたいだな$"

LittlerootTown_ProfessorBirchsLab_Text_MayWhatNextImStayingHere:
	.string "{JPN}ハルカ『{PLAYER}{KUN}は\n"
	.string "{JPN}このあと…… どうする つもり?\p"
	.string "{JPN}ポケモン しょうぶの みちを きわめる?\p"
	.string "{JPN}それとも ぜんこく ずかんの\n"
	.string "{JPN}かんせいを めざすの?\p"
	.string "{JPN}あたしは ここで\n"
	.string "{JPN}はかせの おてつだいを つづけるわ$"

LittlerootTown_ProfessorBirchsLab_Text_BrendanPreferCollectingSlowly:
	.string "{JPN}ユウキ『オレは ポケモン あつめるより\n"
	.string "{JPN}これは! と おもった 1ひきを\l"
	.string "{JPN}じっくり そだてるのが すきなのさ!$"

LittlerootTown_ProfessorBirchsLab_Text_MayHaveYouGoneToBattleFrontier:
	.string "{JPN}ハルカ『あっ {PLAYER}{KUN}!\n"
	.string "{JPN}バトルフロンティア っていう ところには\l"
	.string "{JPN}もう いってみた?$"

LittlerootTown_ProfessorBirchsLab_Text_BrendanHaveYouGoneToBattleFrontier:
	.string "{JPN}ユウキ『やあ {PLAYER}か!\n"
	.string "{JPN}バトルフロンティア って ところには\l"
	.string "{JPN}もう いってみたか?$"

LittlerootTown_ProfessorBirchsLab_Text_ScottAboardSSTidalCall:
	.string "{JPN}…… …… ……\n"
	.string "{JPN}…… …… …… ピッ!\p"
	.string "{JPN}エニシダ『やあやあ {PLAYER}{KUN}!\n"
	.string "{JPN}わたしだよ エニシダだよ!\p"
	.string "{JPN}いま タイドリップごうに のってるんだけど\n"
	.string "{JPN}やっぱり うみは きもちが いいねえー\p"
	.string "{JPN}ところで……\p"
	.string "{JPN}きみを しょうたい したい ばしょが\n"
	.string "{JPN}あるんだけど どうだい?\p"
	.string "{JPN}もし きょうみが あるなら\n"
	.string "{JPN}カイナか ミナモの ふねのりばから\l"
	.string "{JPN}ふねに のっておくれ!\p"
	.string "{JPN}くわしいことは あったときに おしえよう\n"
	.string "{JPN}じゃあな まってるぞ!\p"
	.string "{JPN}…… …… ……\n"
	.string "{JPN}…… …… …… ツー$"
