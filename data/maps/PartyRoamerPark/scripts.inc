PartyRoamerPark_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, PartyRoamerPark_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, PartyRoamerPark_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PartyRoamerPark_OnFrame
	.byte 0

PartyRoamerPark_OnLoad:
	@ The opening adds the cook itself once he steps out of the gate.
	setflag FLAG_TEMP_10
	special Special_PreparePartyRoamerPark
	@ Also replace soil restored from an old save's cached map view.
	setmetatile 1, 7, METATILE_General_Grass, FALSE
	setmetatile 2, 7, METATILE_General_Grass, FALSE
	setmetatile 3, 7, METATILE_General_Grass, FALSE
	setmetatile 4, 7, METATILE_General_Grass, FALSE
	setmetatile 1, 8, METATILE_General_Grass, FALSE
	setmetatile 2, 8, METATILE_General_Grass, FALSE
	setmetatile 3, 8, METATILE_General_Grass, FALSE
	setmetatile 4, 8, METATILE_General_Grass, FALSE
	setmetatile 1, 9, METATILE_General_Grass, FALSE
	setmetatile 2, 9, METATILE_General_Grass, FALSE
	setmetatile 3, 9, METATILE_General_Grass, FALSE
	setmetatile 4, 9, METATILE_General_Grass, FALSE
	end

PartyRoamerPark_OnTransition:
	setflag FLAG_TEMP_HIDE_FOLLOWER
	call_if_eq VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_FAMILY, PartyRoamerPark_EventScript_PlaceIntroParents
	call FrontierStory_EventScript_SetupCamp
	setvar VAR_TEMP_0, 0
	special Special_SpawnPartyRoamerMons
	end

@ Only for this visit: the parents chat south of the gate until the player arrives.
PartyRoamerPark_EventScript_PlaceIntroParents:
	setobjectxyperm LOCALID_PARTY_ROAMER_MOM, 12, 9
	setobjectmovementtype LOCALID_PARTY_ROAMER_MOM, MOVEMENT_TYPE_FACE_RIGHT
	setobjectxyperm LOCALID_PARTY_ROAMER_NORMAN, 13, 9
	setobjectmovementtype LOCALID_PARTY_ROAMER_NORMAN, MOVEMENT_TYPE_FACE_LEFT
	return

PartyRoamerPark_OnFrame:
	map_script_2 VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_FAMILY, FrontierIntro_EventScript_Family
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

PartyRoamerPark_EventScript_BathAttendant::
	lock
	faceplayer
	special Special_BufferPartyBathAdditive
	msgbox PartyRoamerPark_Text_BathWelcome, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_PARTY_BATH_ADDITIVE, FALSE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, PartyRoamerPark_EventScript_BathCancel
	copyvar VAR_0x8004, VAR_RESULT
	specialvar VAR_RESULT, Special_SetPartyBathAdditive
	goto_if_eq VAR_RESULT, FALSE, PartyRoamerPark_EventScript_BathCancel
	msgbox PartyRoamerPark_Text_BathChanged, MSGBOX_DEFAULT
	release
	end

PartyRoamerPark_EventScript_BathCancel:
	msgbox PartyRoamerPark_Text_BathCancel, MSGBOX_DEFAULT
	release
	end

PartyRoamerPark_Text_BathWelcome:
	.string "{JPN}おんせんの にゅうよくざいを\nかえてみるかの？\pいまは {STR_VAR_1}じゃ。\nすきな かおりを えらぶと よいぞ。$"

PartyRoamerPark_Text_BathChanged:
	.string "{JPN}{STR_VAR_1}に\nかえておいたぞい。\pタマゴいがいの てもち みんなの\n{STR_VAR_2}が\l5びょうごとに 1ずつ あがるぞい！$"

PartyRoamerPark_Text_BathCancel:
	.string "{JPN}ゆっくり あたたまって いくんじゃぞ。$"

PartyRoamerPark_EventScript_Norman::
	lock
	faceplayer
	dotimebasedevents
	goto_if_set FLAG_DAILY_PARTY_ROAMER_PARK_NORMAN_GIFT, PartyRoamerPark_EventScript_NormanInvite
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

PartyRoamerPark_EventScript_NormanInvite:
	msgbox PartyRoamerPark_Text_NormanInvite, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, PartyRoamerPark_EventScript_NormanDecline
	msgbox PartyRoamerPark_Text_NormanChooseMon, MSGBOX_DEFAULT
	special ChooseNonEggPartyMon
	@ Finish Norman's idle step before starting the scripted warm-up.
	lock
	faceplayer
	goto_if_ge VAR_0x8004, PARTY_SIZE, PartyRoamerPark_EventScript_NormanTrainingCancel
	special BufferMonNickname
	msgbox PartyRoamerPark_Text_NormanWarmUp, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PARTY_ROAMER_NORMAN, PartyRoamerPark_Movement_NormanWarmUp
	waitmovement 0
	playse SE_M_JUMP_KICK
	applymovement LOCALID_PARTY_ROAMER_NORMAN, PartyRoamerPark_Movement_NormanJump
	waitmovement 0
	faceplayer
	msgbox PartyRoamerPark_Text_NormanTooSerious, MSGBOX_DEFAULT
	msgbox PartyRoamerPark_Text_NormanTrainingHelp, MSGBOX_DEFAULT
	closemessage
	special Special_OpenPartyTraining
	goto_if_eq VAR_RESULT, FALSE, PartyRoamerPark_EventScript_NormanTrainingCancel
	msgbox PartyRoamerPark_Text_NormanTrainingDone, MSGBOX_DEFAULT
	releaseall
	end

PartyRoamerPark_EventScript_NormanTrainingCancel:
	msgbox PartyRoamerPark_Text_NormanTrainingCancel, MSGBOX_DEFAULT
	releaseall
	end

PartyRoamerPark_EventScript_NormanDecline:
	msgbox PartyRoamerPark_Text_NormanDecline, MSGBOX_DEFAULT
	release
	end

PartyRoamerPark_Movement_NormanWarmUp:
	emote_exclamation_mark
	step_end

PartyRoamerPark_Movement_NormanJump:
	jump_in_place_left_right
	delay_8
	jump_in_place_right_left
	step_end

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
	.string "{JPN}ママ『ポケモンも ここなら\n"
	.string "のびのび あそべそうね！$"

PartyRoamerPark_Text_FestivalCircleSoundsFun:
	.string "{JPN}ポロックケースを てにいれたら\n"
	.string "いっしょに ポロックを つくりましょ！$"

PartyRoamerPark_Text_AskMakePokeblocks:
	.string "ねぇ {PLAYER}\n"
	.string "ママと パパで ポロックつくらない?$"

PartyRoamerPark_Text_AskPokeblockFlavor:
	.string "どういう ポロックが\n"
	.string "いいかしら？$"

PartyRoamerPark_Text_Okay:
	.string "わかったわ！$"

PartyRoamerPark_Text_DeclinePokeblock:
	.string "ちぇっ ざんねん……$"

PartyRoamerPark_Text_FinishedPokeblockBlending:
	.string "みんなでつくると\n"
	.string "たのしいわね！$"

PartyRoamerPark_Text_NoPokeblockCase:
	.string "ポロックケースが\n"
	.string "ないみたいね$"

PartyRoamerPark_Text_PokeblockCaseFull:
	.string "ポロックケースが\n"
	.string "いっぱいみたいね$"

PartyRoamerPark_Text_NoBerries:
	.string "きのみが ないと\n"
	.string "つくれないわ$"

PartyRoamerPark_Text_NormanDailyCheckIn:
	.string "{PLAYER} ちゃんとたべてるか?\n"
	.string "げんきに しているか?$"

PartyRoamerPark_Text_NormanGift:
	.string "{JPN}きょうの おやつを よういしたぞ。\n"
	.string "ポケモンと いっしょに たべなさい。$"

PartyRoamerPark_Text_NormanGeneric:
	.string "なにかあったら\n"
	.string "れんらくするんだぞ$"

PartyRoamerPark_Text_NormanInvite:
	.string "{JPN}{PLAYER} すこし パパと\n"
	.string "あそぼうじゃないか！$"

PartyRoamerPark_Text_NormanChooseMon:
	.string "{JPN}いっしょに あそぶ ポケモンを\n"
	.string "えらんで ごらん。$"

PartyRoamerPark_Text_NormanWarmUp:
	.string "{JPN}よーし {STR_VAR_1}！\n"
	.string "まずは パパが おてほんだ！$"

PartyRoamerPark_Text_NormanTooSerious:
	.string "{JPN}……はっ！ つい ジムの\n"
	.string "とっくんみたいに なってしまったな。\p"
	.string "きょうは あそびだった！\n"
	.string "ははは！$"

PartyRoamerPark_Text_NormanTrainingHelp:
	.string "{JPN}どんな あそびで きたえるか\n"
	.string "きそポイントを きめて ごらん。\p"
	.string "のうりょくを えらんで Aで へんしゅう。\n"
	.string "さゆうで けた じょうげで すうじだ。\p"
	.string "ひとつ 252まで ごうけい 510まで。\n"
	.string "けっていで はんえいするぞ。$"

PartyRoamerPark_Text_NormanTrainingDone:
	.string "{JPN}よし！ いい あせを かいたな！\n"
	.string "また パパと あそぼうな。$"

PartyRoamerPark_Text_NormanTrainingCancel:
	.string "{JPN}むりは しなくて いいんだぞ。\n"
	.string "きそポイントは かえていないからな。$"

PartyRoamerPark_Text_NormanDecline:
	.string "{JPN}そうか。 あそびたく なったら\n"
	.string "いつでも パパを よんでくれ！$"
