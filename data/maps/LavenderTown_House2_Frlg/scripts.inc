LavenderTown_House2_Frlg_MapScripts::
	.byte 0

LavenderTown_House2_EventScript_NameRater::
	lock
	faceplayer
	msgbox LavenderTown_House2_Text_WantMeToRateNicknames, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LavenderTown_House2_EventScript_ChooseMon
	goto_if_eq VAR_RESULT, NO, LavenderTown_House2_EventScript_DontRateNickname
	end

LavenderTown_House2_EventScript_ChooseMon::
	msgbox LavenderTown_House2_Text_CritiqueWhichMonsNickname
	special ChoosePartyMon
	goto_if_lt VAR_0x8004, PARTY_SIZE, LavenderTown_House2_EventScript_CheckCanRateMon
	goto_if_ge VAR_0x8004, PARTY_SIZE, LavenderTown_House2_EventScript_DontRateNickname
	end

LavenderTown_House2_EventScript_DontRateNickname::
	msgbox LavenderTown_House2_Text_ISeeComeVisitAgain
	release
	end

LavenderTown_House2_EventScript_CheckCanRateMon::
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, LavenderTown_House2_EventScript_CantNicknameEgg
	special BufferMonNickname
	special IsMonOTIDNotPlayers
	goto_if_eq VAR_RESULT, TRUE, LavenderTown_House2_EventScript_CantNicknameTradeMon
	specialvar VAR_RESULT, MonOTNameNotPlayer
	special BufferMonNickname
	goto_if_eq VAR_RESULT, TRUE, LavenderTown_House2_EventScript_CantNicknameTradeMon
	msgbox LavenderTown_House2_Text_GiveItANicerName, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LavenderTown_House2_EventScript_ChooseNewNickname
	goto_if_eq VAR_RESULT, NO, LavenderTown_House2_EventScript_DontRateNickname
	end

LavenderTown_House2_EventScript_CantNicknameEgg::
	msgbox LavenderTown_House2_Text_ThatIsMerelyAnEgg
	release
	end

LavenderTown_House2_EventScript_CantNicknameTradeMon::
	msgbox LavenderTown_House2_Text_TrulyImpeccableName
	release
	end

LavenderTown_House2_EventScript_ChooseNewNickname::
	msgbox LavenderTown_House2_Text_WhatShallNewNicknameBe
	call Common_EventScript_NameReceivedPartyMon
	specialvar VAR_RESULT, TryPutNameRaterShowOnTheAir
	special BufferMonNickname
	goto_if_eq VAR_RESULT, TRUE, LavenderTown_House2_EventScript_ChoseNewNickname
	msgbox LavenderTown_House2_Text_FromNowOnShallBeKnownAsSameName
	release
	end

LavenderTown_House2_EventScript_ChoseNewNickname::
	msgbox LavenderTown_House2_Text_FromNowOnShallBeKnownAsName
	release
	end

LavenderTown_House2_Text_WantMeToRateNicknames::
	.string "{JPN}はい はい!\n"
	.string "わたしは せいめい はんだんし\l"
	.string "いうなれば なまえの うらないです\p"
	.string "はい あなたの ポケモンの\n"
	.string "ニックネーム うらなって あげるよ$"

LavenderTown_House2_Text_CritiqueWhichMonsNickname::
	.string "{JPN}どの ポケモンの\n"
	.string "ニックネームを うらなうかね?$"

LavenderTown_House2_Text_GiveItANicerName::
	.string "{JPN}うーむ {STR_VAR_1}か……\n"
	.string "なかなか\l"
	.string "いい ニックネームを つけたね\p"
	.string "でもねー\n"
	.string "もう すこしだけ いい なまえ\p"
	.string "わたしが つけて あげようか?\n"
	.string "どうかな?$"

LavenderTown_House2_Text_WhatShallNewNicknameBe::
	.string "{JPN}そうか では\n"
	.string "どんな ニックネームに しようかな$"

LavenderTown_House2_Text_FromNowOnShallBeKnownAsName::
	.string "{JPN}よし これから\n"
	.string "この ポケモンは {STR_VAR_1}だ!\p"
	.string "まえより いい なまえ じゃないか\n"
	.string "よかったな!$"

LavenderTown_House2_Text_ISeeComeVisitAgain::
	.string "{JPN}そうか\n"
	.string "わかった また きなさいよ$"

LavenderTown_House2_Text_FromNowOnShallBeKnownAsSameName::
	.string "{JPN}よし これから\n"
	.string "この ポケモンは {STR_VAR_1}だ!\p"
	.string "まえと おなじように みえるが\n"
	.string "こっちの ほうが だんぜん いいぞ!\l"
	.string "よかったな!$"

LavenderTown_House2_Text_TrulyImpeccableName::
	.string "{JPN}うむ {STR_VAR_1}か!\n"
	.string "これは すばらしい ニックネームだ\l"
	.string "けちの つけようもない!\p"
	.string "これからも {STR_VAR_1}を\n"
	.string "かわいがって あげなさいよ!$"

LavenderTown_House2_Text_ThatIsMerelyAnEgg::
	.string "{JPN}おいおい……\n"
	.string "それは タマゴ じゃないか!$"

