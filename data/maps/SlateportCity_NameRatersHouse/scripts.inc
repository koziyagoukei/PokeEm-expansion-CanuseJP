SlateportCity_NameRatersHouse_MapScripts::
	.byte 0

SlateportCity_NameRatersHouse_EventScript_NameRater::
	lock
	faceplayer
	msgbox SlateportCity_NameRatersHouse_Text_PleasedToRateMonNickname, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SlateportCity_NameRatersHouse_EventScript_ChooseMonToRate
	goto_if_eq VAR_RESULT, NO, SlateportCity_NameRatersHouse_EventScript_DeclineNameRate
	end

SlateportCity_NameRatersHouse_EventScript_ChooseMonToRate::
	msgbox SlateportCity_NameRatersHouse_Text_CritiqueWhichMonNickname, MSGBOX_DEFAULT
	chooseboxmon
	waitstate
	goto_if_ne VAR_0x8004, PARTY_NOTHING_CHOSEN, SlateportCity_NameRatersHouse_EventScript_RateMonNickname
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, SlateportCity_NameRatersHouse_EventScript_DeclineNameRate
	end

SlateportCity_NameRatersHouse_EventScript_DeclineNameRate::
	msgbox SlateportCity_NameRatersHouse_Text_DoVisitAgain, MSGBOX_DEFAULT
	release
	end

SlateportCity_NameRatersHouse_EventScript_RateMonNickname::
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, SlateportCity_NameRatersHouse_EventScript_CantRateEgg
	special BufferMonNickname
	special IsMonOTIDNotPlayers
	goto_if_eq VAR_RESULT, TRUE, SlateportCity_NameRatersHouse_EventScript_PlayerNotMonsOT
	specialvar VAR_RESULT, MonOTNameNotPlayer
	special BufferMonNickname
	goto_if_eq VAR_RESULT, TRUE, SlateportCity_NameRatersHouse_EventScript_PlayerNotMonsOT
	msgbox SlateportCity_NameRatersHouse_Text_FineNameSuggestBetterOne, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SlateportCity_NameRatersHouse_EventScript_ChangeNickname
	goto_if_eq VAR_RESULT, NO, SlateportCity_NameRatersHouse_EventScript_DeclineNameRate
	end

SlateportCity_NameRatersHouse_EventScript_CantRateEgg::
	msgbox SlateportCity_NameRatersHouse_Text_ThatIsMerelyAnEgg, MSGBOX_DEFAULT
	release
	end

SlateportCity_NameRatersHouse_EventScript_PlayerNotMonsOT::
	msgbox SlateportCity_NameRatersHouse_Text_MagnificentName, MSGBOX_DEFAULT
	release
	end

SlateportCity_NameRatersHouse_EventScript_ChangeNickname::
	msgbox SlateportCity_NameRatersHouse_Text_WhatShallNewNameBe, MSGBOX_DEFAULT
	call Common_EventScript_NameReceivedPartyMon
	specialvar VAR_RESULT, TryPutNameRaterShowOnTheAir
	special BufferMonNickname
	goto_if_eq VAR_RESULT, TRUE, SlateportCity_NameRatersHouse_EventScript_NewNameDifferent
	msgbox SlateportCity_NameRatersHouse_Text_NameNoDifferentYetSuperior, MSGBOX_DEFAULT
	release
	end

SlateportCity_NameRatersHouse_EventScript_NewNameDifferent::
	msgbox SlateportCity_NameRatersHouse_Text_MonShallBeKnownAsName, MSGBOX_DEFAULT
	release
	end

SlateportCity_NameRatersHouse_Text_PleasedToRateMonNickname:
	.string "{JPN}はい はい！\nわたしは せいめい はんだんし\lいうなれば なまえの うらないです\pはい あなたの ポケモンの\nニックネームを うらなって あげるよ$"

SlateportCity_NameRatersHouse_Text_CritiqueWhichMonNickname:
	.string "{JPN}どの ポケモンの\nニックネームを うらなうかね？$"

SlateportCity_NameRatersHouse_Text_FineNameSuggestBetterOne:
	.string "{JPN}うーむ {STR_VAR_1}か……\nなかなか いい ニックネームを つけたね\pでもねー\nもう すこしだけ いい なまえを\lわたしが つけて あげようか？ どうかな？$"

SlateportCity_NameRatersHouse_Text_WhatShallNewNameBe:
	.string "{JPN}そうか では\nどんな ニックネームに しようかな$"

SlateportCity_NameRatersHouse_Text_MonShallBeKnownAsName:
	.string "{JPN}よし これから\nこの ポケモンは {STR_VAR_1}だ！\pまえより いい なまえ じゃないか\nよかったな！$"

SlateportCity_NameRatersHouse_Text_DoVisitAgain:
	.string "{JPN}そうか\nわかった また きなさいよ$"

SlateportCity_NameRatersHouse_Text_NameNoDifferentYetSuperior:
	.string "{JPN}よし これから\nこの ポケモンは {STR_VAR_1}だ！\pまえと おなじように みえるが\nこっちの ほうが だんぜん いいぞ！\lよかったな！$"

SlateportCity_NameRatersHouse_Text_MagnificentName:
	.string "{JPN}うーむ {STR_VAR_1}か！\nこれは すばらしい ニックネームだ！\lけちの つけようもない！\pこれからも {STR_VAR_1}を\nかわいがって あげなさいよ！$"

SlateportCity_NameRatersHouse_Text_ThatIsMerelyAnEgg:
	.string "{JPN}おいおい……\nそれは タマゴ じゃないか！$"

