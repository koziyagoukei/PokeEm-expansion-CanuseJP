PartyRoamerPark_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PartyRoamerPark_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PartyRoamerPark_OnFrame
	.byte 0

PartyRoamerPark_OnTransition:
	setflag FLAG_TEMP_HIDE_FOLLOWER
	setvar VAR_TEMP_0, 0
	special Special_SpawnPartyRoamerMons
	end

PartyRoamerPark_OnFrame:
	map_script_2 VAR_TEMP_0, 0, PartyRoamerPark_EventScript_RefreshMons
	.2byte 0

PartyRoamerPark_EventScript_RefreshMons::
	setvar VAR_TEMP_0, 1
	special Special_RefreshPartyRoamerMons
	end

PartyRoamerPark_EventScript_Talk::
	lock
	faceplayer
	special Special_BufferPartyRoamerMessage
	msgbox gStringVar4, MSGBOX_DEFAULT
	release
	end

PartyRoamerPark_EventScript_Norman::
	lock
	faceplayer
	dotimebasedevents
	goto_if_set FLAG_DAILY_PARTY_ROAMER_PARK_NORMAN_GIFT, PartyRoamerPark_EventScript_NormanGeneric
	msgbox PartyRoamerPark_Text_NormanDailyCheckIn, MSGBOX_DEFAULT
	msgbox PartyRoamerPark_Text_NormanGift, MSGBOX_DEFAULT
	random 10
	switch VAR_RESULT
	case 0, PartyRoamerPark_EventScript_NormanGiveRageCandyBar
	case 1, PartyRoamerPark_EventScript_NormanGiveBigMalasada
	case 2, PartyRoamerPark_EventScript_NormanGiveShalourSable
	case 3, PartyRoamerPark_EventScript_NormanGiveMaxHoney
	case 4, PartyRoamerPark_EventScript_NormanGivePewterCrunchies
	case 5, PartyRoamerPark_EventScript_NormanGiveSweetHeart
	case 6, PartyRoamerPark_EventScript_NormanGiveCasteliacone
	case 7, PartyRoamerPark_EventScript_NormanGiveLavaCookie
	case 8, PartyRoamerPark_EventScript_NormanGiveLumioseGalette
	case 9, PartyRoamerPark_EventScript_NormanGiveOldGateau
	end

PartyRoamerPark_EventScript_NormanGiveRageCandyBar:
	setvar VAR_0x8004, ITEM_RAGE_CANDY_BAR
	goto PartyRoamerPark_EventScript_NormanGiveItem

PartyRoamerPark_EventScript_NormanGiveBigMalasada:
	setvar VAR_0x8004, ITEM_BIG_MALASADA
	goto PartyRoamerPark_EventScript_NormanGiveItem

PartyRoamerPark_EventScript_NormanGiveShalourSable:
	setvar VAR_0x8004, ITEM_SHALOUR_SABLE
	goto PartyRoamerPark_EventScript_NormanGiveItem

PartyRoamerPark_EventScript_NormanGiveMaxHoney:
	setvar VAR_0x8004, ITEM_MAX_HONEY
	goto PartyRoamerPark_EventScript_NormanGiveItem

PartyRoamerPark_EventScript_NormanGivePewterCrunchies:
	setvar VAR_0x8004, ITEM_PEWTER_CRUNCHIES
	goto PartyRoamerPark_EventScript_NormanGiveItem

PartyRoamerPark_EventScript_NormanGiveSweetHeart:
	setvar VAR_0x8004, ITEM_SWEET_HEART
	goto PartyRoamerPark_EventScript_NormanGiveItem

PartyRoamerPark_EventScript_NormanGiveCasteliacone:
	setvar VAR_0x8004, ITEM_CASTELIACONE
	goto PartyRoamerPark_EventScript_NormanGiveItem

PartyRoamerPark_EventScript_NormanGiveLavaCookie:
	setvar VAR_0x8004, ITEM_LAVA_COOKIE
	goto PartyRoamerPark_EventScript_NormanGiveItem

PartyRoamerPark_EventScript_NormanGiveLumioseGalette:
	setvar VAR_0x8004, ITEM_LUMIOSE_GALETTE
	goto PartyRoamerPark_EventScript_NormanGiveItem

PartyRoamerPark_EventScript_NormanGiveOldGateau:
	setvar VAR_0x8004, ITEM_OLD_GATEAU
	goto PartyRoamerPark_EventScript_NormanGiveItem

PartyRoamerPark_EventScript_NormanGiveItem:
	giveitem VAR_0x8004
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_DAILY_PARTY_ROAMER_PARK_NORMAN_GIFT
	goto PartyRoamerPark_EventScript_NormanGeneric

PartyRoamerPark_EventScript_NormanGeneric:
	msgbox PartyRoamerPark_Text_NormanGeneric, MSGBOX_DEFAULT
	release
	end

PartyRoamerPark_EventScript_Mom::
	lockall
	faceplayer
	goto_if_set FLAG_RECEIVED_POKEBLOCK_CASE, PartyRoamerPark_EventScript_MomHasPokeblockCase
	msgbox PartyRoamerPark_Text_AlolaFestivalCircle, MSGBOX_DEFAULT
	msgbox PartyRoamerPark_Text_FestivalCircleSoundsFun, MSGBOX_DEFAULT
	releaseall
	end

PartyRoamerPark_EventScript_MomHasPokeblockCase:
	msgbox PartyRoamerPark_Text_AskMakePokeblocks, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, PartyRoamerPark_EventScript_AskPokeblockFlavor
	goto PartyRoamerPark_EventScript_DeclinePokeblock

PartyRoamerPark_EventScript_AskPokeblockFlavor:
	msgbox PartyRoamerPark_Text_AskPokeblockFlavor, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_PARTY_ROAMER_POKEBLOCK_FLAVOR, FALSE
	switch VAR_RESULT
	case 0, PartyRoamerPark_EventScript_SetSpicyPokeblock
	case 1, PartyRoamerPark_EventScript_SetSweetPokeblock
	case 2, PartyRoamerPark_EventScript_SetBitterPokeblock
	case 3, PartyRoamerPark_EventScript_SetDryPokeblock
	case 4, PartyRoamerPark_EventScript_SetSourPokeblock
	case MULTI_B_PRESSED, PartyRoamerPark_EventScript_DeclinePokeblock
	end

PartyRoamerPark_EventScript_SetSpicyPokeblock:
	setvar VAR_0x8005, 0
	goto PartyRoamerPark_EventScript_StartPokeblockBlending

PartyRoamerPark_EventScript_SetSweetPokeblock:
	setvar VAR_0x8005, 1
	goto PartyRoamerPark_EventScript_StartPokeblockBlending

PartyRoamerPark_EventScript_SetBitterPokeblock:
	setvar VAR_0x8005, 2
	goto PartyRoamerPark_EventScript_StartPokeblockBlending

PartyRoamerPark_EventScript_SetDryPokeblock:
	setvar VAR_0x8005, 3
	goto PartyRoamerPark_EventScript_StartPokeblockBlending

PartyRoamerPark_EventScript_SetSourPokeblock:
	setvar VAR_0x8005, 4
	goto PartyRoamerPark_EventScript_StartPokeblockBlending

PartyRoamerPark_EventScript_StartPokeblockBlending:
	msgbox PartyRoamerPark_Text_Okay, MSGBOX_DEFAULT
	checkitem ITEM_POKEBLOCK_CASE
	goto_if_eq VAR_RESULT, FALSE, PartyRoamerPark_EventScript_NoPokeblockCase
	specialvar VAR_RESULT, GetFirstFreePokeblockSlot
	goto_if_eq VAR_RESULT, 65535, PartyRoamerPark_EventScript_PokeblockCaseFull
	specialvar VAR_RESULT, PlayerHasBerries
	goto_if_eq VAR_RESULT, FALSE, PartyRoamerPark_EventScript_NoBerries
	setvar VAR_0x8004, 2
	special SetPartyRoamerParkBerryBlenderFlavor
	fadescreen FADE_TO_BLACK
	special DoBerryBlending
	specialvar VAR_RESULT, DidPartyRoamerParkBerryBlenderComplete
	goto_if_eq VAR_RESULT, TRUE, PartyRoamerPark_EventScript_FinishedPokeblockBlending
	releaseall
	end

PartyRoamerPark_EventScript_FinishedPokeblockBlending:
	msgbox PartyRoamerPark_Text_FinishedPokeblockBlending, MSGBOX_DEFAULT
	releaseall
	end

PartyRoamerPark_EventScript_DeclinePokeblock:
	msgbox PartyRoamerPark_Text_DeclinePokeblock, MSGBOX_DEFAULT
	releaseall
	end

PartyRoamerPark_EventScript_NoPokeblockCase:
	msgbox PartyRoamerPark_Text_NoPokeblockCase, MSGBOX_DEFAULT
	releaseall
	end

PartyRoamerPark_EventScript_PokeblockCaseFull:
	msgbox PartyRoamerPark_Text_PokeblockCaseFull, MSGBOX_DEFAULT
	releaseall
	end

PartyRoamerPark_EventScript_NoBerries:
	msgbox PartyRoamerPark_Text_NoBerries, MSGBOX_DEFAULT
	releaseall
	end

PartyRoamerPark_Text_AlolaFestivalCircle:
	.string "{JPN}アローラちほう では フェスサークル\n"
	.string "っていうサーバーが あるんですって$"

PartyRoamerPark_Text_FestivalCircleSoundsFun:
	.string "{JPN}きっと いろいろ\n"
	.string "できるのでしょうね！$"

PartyRoamerPark_Text_AskMakePokeblocks:
	.string "{JPN}ねぇ {PLAYER}\n"
	.string "ママと パパで ポロックつくらない?$"

PartyRoamerPark_Text_AskPokeblockFlavor:
	.string "{JPN}どういう ポロックが\n"
	.string "いいかしら？$"

PartyRoamerPark_Text_Okay:
	.string "{JPN}わかったわ！$"

PartyRoamerPark_Text_DeclinePokeblock:
	.string "{JPN}ちぇっ ざんねん……$"

PartyRoamerPark_Text_FinishedPokeblockBlending:
	.string "{JPN}みんなでつくると\n"
	.string "たのしいわね！$"

PartyRoamerPark_Text_NoPokeblockCase:
	.string "{JPN}ポロックケースが\n"
	.string "ないみたいね$"

PartyRoamerPark_Text_PokeblockCaseFull:
	.string "{JPN}ポロックケースが\n"
	.string "いっぱいみたいね$"

PartyRoamerPark_Text_NoBerries:
	.string "{JPN}きのみが ないと\n"
	.string "つくれないわ$"

PartyRoamerPark_Text_NormanDailyCheckIn:
	.string "{JPN}{PLAYER} ちゃんとたべてるか?\n"
	.string "げんきに しているか?$"

PartyRoamerPark_Text_NormanGift:
	.string "{JPN}ジムで おみやげもらったから\n"
	.string "たべなさい$"

PartyRoamerPark_Text_NormanGeneric:
	.string "{JPN}なにかあったら\n"
	.string "れんらくするんだぞ$"
