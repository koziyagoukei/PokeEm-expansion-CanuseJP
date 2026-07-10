Party_eat_time_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Party_eat_time_OnTransition
	.byte 0

Party_eat_time_OnTransition:
	setflag FLAG_TEMP_HIDE_FOLLOWER
	special Special_SpawnPartyRoamerMons
	end

#define TEISHOKUYA_MENU_GIGANTAMAX 21
#define TEISHOKUYA_MENU_CANCEL 22

PartyEatTime_EventScript_RocketCook::
	lock
	faceplayer
	msgbox PartyEatTime_Text_RocketCookGreeting, MSGBOX_DEFAULT
	dynmultipush PartyEatTime_Text_Order, 0
	dynmultipush PartyEatTime_Text_Cancel, 1
	dynmultistack 0, 0, FALSE, 2, 0, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	switch VAR_RESULT
	case 0, PartyEatTime_EventScript_Order
	case 1, PartyEatTime_EventScript_Cancel
	case MULTI_B_PRESSED, PartyEatTime_EventScript_Cancel
	release
	end

PartyEatTime_EventScript_Order::
	msgbox PartyEatTime_Text_ChooseMealMon, MSGBOX_DEFAULT
	special ChooseNonEggPartyMon
	lock
	faceplayer
	goto_if_ge VAR_0x8004, PARTY_SIZE, PartyEatTime_EventScript_Cancel
	special IsSelectedMonEgg
	goto_if_eq VAR_RESULT, TRUE, PartyEatTime_EventScript_CantChooseEgg
	special BufferMonNickname
	msgbox PartyEatTime_Text_ChooseMenu, MSGBOX_DEFAULT
	special Special_TeishokuyaCanToggleGigantamax
	goto_if_eq VAR_RESULT, TRUE, PartyEatTime_EventScript_ShowMenuWithGigantamax
	goto PartyEatTime_EventScript_ShowMenu

PartyEatTime_EventScript_ShowMenuWithGigantamax::
	call PartyEatTime_EventScript_PushTeraTypeMenu
	dynmultipush PartyEatTime_Text_Gigantamax, TEISHOKUYA_MENU_GIGANTAMAX
	dynmultipush PartyEatTime_Text_Cancel, TEISHOKUYA_MENU_CANCEL
	dynmultistack 0, 0, FALSE, 8, 0, 0, DYN_MULTICHOICE_CB_NONE
	goto PartyEatTime_EventScript_HandleMenuResult

PartyEatTime_EventScript_ShowMenu::
	call PartyEatTime_EventScript_PushTeraTypeMenu
	dynmultipush PartyEatTime_Text_Cancel, TEISHOKUYA_MENU_CANCEL
	dynmultistack 0, 0, FALSE, 8, 0, 0, DYN_MULTICHOICE_CB_NONE
	goto PartyEatTime_EventScript_HandleMenuResult

PartyEatTime_EventScript_PushTeraTypeMenu::
	dynmultipush PartyEatTime_Text_TypeNormal, TYPE_NORMAL
	dynmultipush PartyEatTime_Text_TypeFire, TYPE_FIRE
	dynmultipush PartyEatTime_Text_TypeGrass, TYPE_GRASS
	dynmultipush PartyEatTime_Text_TypeWater, TYPE_WATER
	dynmultipush PartyEatTime_Text_TypeElectric, TYPE_ELECTRIC
	dynmultipush PartyEatTime_Text_TypeIce, TYPE_ICE
	dynmultipush PartyEatTime_Text_TypeFighting, TYPE_FIGHTING
	dynmultipush PartyEatTime_Text_TypePoison, TYPE_POISON
	dynmultipush PartyEatTime_Text_TypeGround, TYPE_GROUND
	dynmultipush PartyEatTime_Text_TypeFlying, TYPE_FLYING
	dynmultipush PartyEatTime_Text_TypePsychic, TYPE_PSYCHIC
	dynmultipush PartyEatTime_Text_TypeBug, TYPE_BUG
	dynmultipush PartyEatTime_Text_TypeRock, TYPE_ROCK
	dynmultipush PartyEatTime_Text_TypeGhost, TYPE_GHOST
	dynmultipush PartyEatTime_Text_TypeDragon, TYPE_DRAGON
	dynmultipush PartyEatTime_Text_TypeDark, TYPE_DARK
	dynmultipush PartyEatTime_Text_TypeSteel, TYPE_STEEL
	dynmultipush PartyEatTime_Text_TypeFairy, TYPE_FAIRY
	dynmultipush PartyEatTime_Text_TypeStellar, TYPE_STELLAR
	return

PartyEatTime_EventScript_HandleMenuResult::
	closemessage
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, PartyEatTime_EventScript_Cancel
	goto_if_eq VAR_RESULT, TEISHOKUYA_MENU_CANCEL, PartyEatTime_EventScript_Cancel
	goto_if_eq VAR_RESULT, TEISHOKUYA_MENU_GIGANTAMAX, PartyEatTime_EventScript_ToggleGigantamax
	special Special_TeishokuyaChangeTeraType
	goto PartyEatTime_EventScript_AfterOrder

PartyEatTime_EventScript_ToggleGigantamax::
	special Special_TeishokuyaToggleGigantamax
	goto PartyEatTime_EventScript_AfterOrder

PartyEatTime_EventScript_AfterOrder::
	fadescreen FADE_TO_BLACK
	playse SE_BIKE_BELL
	waitse
	special Special_TeishokuyaPlayMealScene
	special Special_TeishokuyaIncreasePartyFriendship
	msgbox PartyEatTime_Text_Satisfied, MSGBOX_DEFAULT
	release
	end

PartyEatTime_EventScript_CantChooseEgg::
	msgbox PartyEatTime_Text_CantChooseEgg, MSGBOX_DEFAULT
	release
	end

PartyEatTime_EventScript_Cancel::
	msgbox PartyEatTime_Text_ComeAgain, MSGBOX_DEFAULT
	release
	end

PartyEatTime_Text_RocketCookGreeting:
	.string "{JPN}サカキさまー おれは いつになったら\nめしやの しごと やめれるんですか\pなんで もうかっちまうんだよー$"

PartyEatTime_Text_Order:
	.string "{JPN}ちゅうもんする$"

PartyEatTime_Text_Cancel:
	.string "{JPN}やめる$"

PartyEatTime_Text_ChooseMealMon:
	.string "{JPN}あ? めし?$"

PartyEatTime_Text_ChooseMenu:
	.string "{JPN}オラ メニューだ なにをくう?$"

PartyEatTime_Text_Satisfied:
	.string "{JPN}{STR_VAR_1}たちは まんぞくしたようだ!$"

PartyEatTime_Text_CantChooseEgg:
	.string "{JPN}タマゴに くわせる メシは ねえぞ$"

PartyEatTime_Text_ComeAgain:
	.string "{JPN}……また くいに こいや$"

PartyEatTime_Text_TypeNormal:
	.string "{JPN}いつものすうどん$"

PartyEatTime_Text_TypeFire:
	.string "{JPN}ろうそくプリン$"

PartyEatTime_Text_TypeGrass:
	.string "{JPN}くさごとかしわもち$"

PartyEatTime_Text_TypeWater:
	.string "{JPN}みずみずしいギョーザ$"

PartyEatTime_Text_TypeElectric:
	.string "{JPN}しおみビリビリがゆ$"

PartyEatTime_Text_TypeIce:
	.string "{JPN}こなゆきかきごおり$"

PartyEatTime_Text_TypeFighting:
	.string "{JPN}ボンボンプロテイン$"

PartyEatTime_Text_TypePoison:
	.string "{JPN}どくみされたサイダー$"

PartyEatTime_Text_TypeGround:
	.string "{JPN}つちいろトリュフ$"

PartyEatTime_Text_TypeFlying:
	.string "{JPN}ひこうきグミ$"

PartyEatTime_Text_TypePsychic:
	.string "{JPN}エスパーおまかせドン$"

PartyEatTime_Text_TypeBug:
	.string "{JPN}ミツミツむしパン$"

PartyEatTime_Text_TypeRock:
	.string "{JPN}いわノリノリビーフン$"

PartyEatTime_Text_TypeGhost:
	.string "{JPN}ばけのかおチップス$"

PartyEatTime_Text_TypeDragon:
	.string "{JPN}たつのこていしょく$"

PartyEatTime_Text_TypeDark:
	.string "{JPN}あくぬきさんさいそば$"

PartyEatTime_Text_TypeSteel:
	.string "{JPN}ハガネのビスケット$"

PartyEatTime_Text_TypeFairy:
	.string "{JPN}フェアリーばえケーキ$"

PartyEatTime_Text_TypeStellar:
	.string "{JPN}にじいろまかない$"

PartyEatTime_Text_Gigantamax:
	.string "{JPN}マックスやくぜんなべ$"
