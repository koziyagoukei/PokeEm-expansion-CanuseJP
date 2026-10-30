MiniMiniVilla_Shop_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, VillaShop_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, VillaShop_OnTransition
	.byte 0

VillaShop_OnLoad:
	special Special_RestoreResortInterior
	end

VillaShop_OnTransition:
	setflag FLAG_TEMP_HIDE_FOLLOWER
	end

VillaShop_EventScript_Decorations::
	lock
	faceplayer
	msgbox VillaShop_Text_Decorations, MSGBOX_DEFAULT
	setvar VAR_0x8006, VILLA_SHOP_DECORATIONS
	setvar VAR_0x8008, TRUE
	goto VillaShop_EventScript_Catalog

VillaShop_EventScript_Balls::
	lock
	faceplayer
	msgbox VillaShop_Text_ChooseMon, MSGBOX_DEFAULT
	special ChooseNonEggPartyMon
	lock
	faceplayer
	goto_if_ge VAR_0x8004, PARTY_SIZE, VillaShop_EventScript_Goodbye
	bufferpartymonnick STR_VAR_3, VAR_0x8004
	setvar VAR_0x8006, VILLA_SHOP_BALLS
	setvar VAR_0x8008, TRUE
	goto VillaShop_EventScript_Catalog

VillaShop_EventScript_Catalog:
	closemessage
	special ShowBattlePointsWindow
	special VillaShop_OpenCatalog
	special CloseBattlePointsWindow
	goto_if_eq VAR_RESULT, FALSE, VillaShop_EventScript_Goodbye
	goto_if_eq VAR_0x8006, VILLA_SHOP_BALLS, VillaShop_EventScript_ConfirmBall
	msgbox VillaShop_Text_ConfirmDecoration, MSGBOX_YESNO
	goto VillaShop_EventScript_Confirm

VillaShop_EventScript_ConfirmBall:
	msgbox VillaShop_Text_ConfirmBall, MSGBOX_YESNO

VillaShop_EventScript_Confirm:
	goto_if_eq VAR_RESULT, NO, VillaShop_EventScript_Catalog
	special VillaShop_Purchase
	switch VAR_RESULT
	case VILLA_SHOP_NOT_ENOUGH_BP, VillaShop_EventScript_NotEnoughBP
	case VILLA_SHOP_STORAGE_FULL, VillaShop_EventScript_StorageFull
	case VILLA_SHOP_SAME_BALL, VillaShop_EventScript_SameBall
	case VILLA_SHOP_INVALID, VillaShop_EventScript_Invalid
	playse SE_SHOP
	waitse
	goto_if_eq VAR_0x8006, VILLA_SHOP_BALLS, VillaShop_EventScript_BallDone
	msgbox VillaShop_Text_DecorationDone, MSGBOX_DEFAULT
	goto VillaShop_EventScript_Catalog

VillaShop_EventScript_BallDone:
	msgbox VillaShop_Text_BallDone, MSGBOX_DEFAULT
	goto VillaShop_EventScript_Catalog

VillaShop_EventScript_NotEnoughBP:
	msgbox VillaShop_Text_NotEnoughBP, MSGBOX_DEFAULT
	goto VillaShop_EventScript_Catalog

VillaShop_EventScript_StorageFull:
	msgbox VillaShop_Text_StorageFull, MSGBOX_DEFAULT
	goto VillaShop_EventScript_Catalog

VillaShop_EventScript_SameBall:
	msgbox VillaShop_Text_SameBall, MSGBOX_DEFAULT
	goto VillaShop_EventScript_Catalog

VillaShop_EventScript_Invalid:
	msgbox VillaShop_Text_Invalid, MSGBOX_DEFAULT
	goto VillaShop_EventScript_Goodbye

VillaShop_EventScript_Goodbye:
	msgbox VillaShop_Text_Goodbye, MSGBOX_DEFAULT
	release
	end

VillaShop_Text_Decorations:
	.string "{JPN}いらっしゃいませ！\nかざりは すべて 10BPです。\pとくしゅドールは べっそうの\n部屋に 3たいまで かざれます。$"

VillaShop_Text_ChooseMon:
	.string "{JPN}ボールを かえたい ポケモンを\n選んでください。$"

VillaShop_Text_ConfirmDecoration:
	.string "{JPN}{STR_VAR_1}を\n"
	.string "{STR_VAR_2}BPで 購入しますか？$"

VillaShop_Text_ConfirmBall:
	.string "{JPN}{STR_VAR_3}の ボールを\n"
	.string "{STR_VAR_1}に {STR_VAR_2}BPで 変えますか？$"

VillaShop_Text_DecorationDone:
	.string "{JPN}{STR_VAR_1}を\n"
	.string "飾りの パソコンに 送りました！$"

VillaShop_Text_BallDone:
	.string "{JPN}{STR_VAR_3}の ボールを\n"
	.string "{STR_VAR_1}に 変えました！$"

VillaShop_Text_NotEnoughBP:
	.string "{JPN}BPが たりません。$"

VillaShop_Text_StorageFull:
	.string "{JPN}この 種類の かざりは\nこれいじょう もてません。$"

VillaShop_Text_SameBall:
	.string "{JPN}すでに 同じ ボールです。\nBPは 使いません。$"

VillaShop_Text_Invalid:
	.string "{JPN}この ポケモンの ボールは\n変更できません。$"

VillaShop_Text_Goodbye:
	.string "{JPN}また お越しください！$"
