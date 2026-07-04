FortreeCity_House2_MapScripts::
	.byte 0

FortreeCity_House2_EventScript_HiddenPowerGiver::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM_HIDDEN_POWER, FortreeCity_House2_EventScript_ExplainHiddenPower
	call_if_unset FLAG_MET_HIDDEN_POWER_GIVER, FortreeCity_House2_EventScript_Greeting
	msgbox FortreeCity_House2_Text_CoinInWhichHand, MSGBOX_DEFAULT
	multichoice 21, 8, MULTI_RIGHTLEFT, TRUE
	switch VAR_RESULT
	case 1, FortreeCity_House2_EventScript_WrongGuess
	msgbox FortreeCity_House2_Text_CorrectTryAgainWhichHand, MSGBOX_DEFAULT
	multichoice 21, 8, MULTI_RIGHTLEFT, TRUE
	switch VAR_RESULT
	case 1, FortreeCity_House2_EventScript_WrongGuess
	msgbox FortreeCity_House2_Text_CorrectTryAgainWhichHand2, MSGBOX_DEFAULT
	multichoice 21, 8, MULTI_RIGHTLEFT, TRUE
	switch VAR_RESULT
	case 0, FortreeCity_House2_EventScript_WrongGuess
	msgbox FortreeCity_House2_Text_YourHiddenPowerHasAwoken, MSGBOX_DEFAULT
	giveitem ITEM_TM_HIDDEN_POWER
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_HIDDEN_POWER
	msgbox FortreeCity_House2_Text_ExplainHiddenPower, MSGBOX_DEFAULT
	release
	end

FortreeCity_House2_EventScript_Greeting::
	msgbox FortreeCity_House2_Text_HiddenPowersArousedByNature, MSGBOX_DEFAULT
	setflag FLAG_MET_HIDDEN_POWER_GIVER
	return

FortreeCity_House2_EventScript_ExplainHiddenPower::
	msgbox FortreeCity_House2_Text_ExplainHiddenPower, MSGBOX_DEFAULT
	release
	end

FortreeCity_House2_EventScript_WrongGuess::
	msgbox FortreeCity_House2_Text_YouGuessedWrong, MSGBOX_DEFAULT
	release
	end

FortreeCity_House2_Text_HiddenPowersArousedByNature:
	.string "{JPN}ひとも ポケモンも\nしぜんの なかで くらすことで\lもともとの パワーが めざめる……$"

FortreeCity_House2_Text_CoinInWhichHand:
	.string "{JPN}おぬしの パワーが めざめているか\nたしかめると するかの……\pどれ…… わしの みぎてと ひだりて\nどちらに コインが はいっておるか\lちょっかんで えらんで みなさい$"

FortreeCity_House2_Text_CorrectTryAgainWhichHand:
	.string "{JPN}おお! せいかい じゃ!\pつぎは みぎてと ひだりて\nどちらに コインが はいっておるか\lちょっかんで えらんで みなさい$"

FortreeCity_House2_Text_CorrectTryAgainWhichHand2:
	.string "{JPN}おお! せいかい じゃ!\pつぎは みぎてと ひだりて\nどちらに コインが はいっておるか\lちょっかんで えらんで みなさい$"

FortreeCity_House2_Text_YourHiddenPowerHasAwoken:
	.string "{JPN}おお! みごと じゃ!\nおぬしの パワーは めざめているの!\pこれを あげるから\nおぬしの ポケモンも パワーを\lめざめさせて やると ええぞ!$"

FortreeCity_House2_Text_ExplainHiddenPower:
	.string "{JPN}めざめるパワーは ポケモンによって\nいりょくが かわる わざ なんじゃよ!$"

FortreeCity_House2_Text_YouGuessedWrong:
	.string "{JPN}ざんねん……\nはずれ じゃ……$"

