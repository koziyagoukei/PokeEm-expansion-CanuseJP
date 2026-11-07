.equ GAVE_PROFILE,  VAR_TEMP_1

MysteryEventClub_EventScript_Woman::
	lock
	faceplayer
	goto_if_eq GAVE_PROFILE, TRUE, EventScript_AlreadyGaveProfile
	goto_if_set FLAG_SYS_SET_TRAINER_CARD_PROFILE, EventScript_GivenProfileBefore
	msgbox Text_IdLoveToHearYourProfile
	goto EventScript_AskForProfile
	end

EventScript_AlreadyGaveProfile:
	msgbox Text_TellMoreNextTime
	release
	end

EventScript_AskForProfile:
	message Text_TellMeWhatsYourProfile
	waitmessage
	multichoice 18, 6, MULTI_YESNOINFO_2, FALSE
	switch VAR_RESULT
	case 0, EventScript_AcceptGiveProfile
	case 1, EventScript_DeclineGiveProfile
	case 2, EventScript_ProfileInfo
	case MULTI_B_PRESSED, EventScript_DeclineGiveProfile
	end

EventScript_ProfileInfo:
	msgbox Text_ExplainProfile
	goto EventScript_AskForProfile
	end

EventScript_AcceptGiveProfile:
	msgbox Text_WhatsYourProfile
	closemessage
	goto EventScript_GiveProfile
	end

EventScript_CancelGiveProfile:
	msgbox Text_DecidedNotTo
	release
	end

EventScript_GaveProfile:
	setvar GAVE_PROFILE, TRUE
	setvar VAR_0x8004, 0
	special ShowEasyChatProfile
	waitmessage
	delay 80
	msgbox Text_ProfileSaysSomethingAboutYou
	release
	end

EventScript_DeclineGiveProfile:
	msgbox Text_DontBeLikeThat
	release
	end

EventScript_GivenProfileBefore:
	msgbox Text_HiWeMeetAgain
	goto EventScript_AskForNewProfile
	end

EventScript_AskForNewProfile:
	message Text_ChangeYourProfile
	waitmessage
	multichoice 18, 6, MULTI_YESNOINFO_2, FALSE
	switch VAR_RESULT
	case 0, EventScript_AcceptGiveNewProfile
	case 1, EventScript_DeclineGiveNewProfile
	case 2, EventScript_ProfileInfo2
	@case MULTI_B_PRESSED, EventScript_DeclineGiveNewProfile
	end

EventScript_ProfileInfo2:
	msgbox Text_ExplainProfile
	goto EventScript_AskForNewProfile
	end

EventScript_AcceptGiveNewProfile:
	msgbox Text_WhatAreYouLikeNow
	closemessage
	goto EventScript_GiveProfile
	end

EventScript_DeclineGiveNewProfile:
	msgbox Text_LikeProfileWayItIs
	release
	end

EventScript_GaveSpecialProfile:
	setvar GAVE_PROFILE, TRUE
	setvar VAR_0x8004, 0
	special ShowEasyChatProfile
	waitmessage
	delay 80
	msgbox Text_HeardThatBefore
	release
	end

EventScript_GiveProfile:
	setvar VAR_0x8004, EASY_CHAT_TYPE_PROFILE
	call Common_ShowEasyChatScreen
	lock
	faceplayer
	goto_if_eq VAR_0x8004, 0, EventScript_GaveSpecialProfile
	goto_if_eq VAR_RESULT, FALSE, EventScript_CancelGiveProfile
	goto_if_eq VAR_RESULT, TRUE, EventScript_GaveProfile
	end

Text_IdLoveToHearYourProfile:
	.string "{JPN}ハーイ はじめまして!\p"
	.string "……って\n"
	.string "こっちが あいさつ したんだから\l"
	.string "あなただって あいさつ かえしてよ!\p"
	.string "そうよ せっかく だから\n"
	.string "じこしょうかい してほしいな\l"
	.string "なにか プロフィールを きかせて!$"

Text_TellMeWhatsYourProfile:
	.string "{JPN}あなた らしい\n"
	.string "すてきな プロフィール きかせて!$"

Text_ExplainProfile:
	.string "{JPN}プロフィールは すきな ことばを\n"
	.string "4つ くみあわせて つくるのよ\p"
	.string "くみあわせる ことばは\n"
	.string "たくさん あるの!\p"
	.string "あなたが わかりやすいように\n"
	.string "いくつかの グループに\l"
	.string "わけて あるから まず あなたは\l"
	.string "グループを えらんでから\l"
	.string "その なかの ことばを えらんでね!\p"
	.string "……むずかしいって かお してるわね\n"
	.string "とにかく やって みましょ!$"

Text_WhatsYourProfile:
	.string "{JPN}どんな プロフィール なの?$"

Text_DontBeLikeThat:
	.string "{JPN}えー\n"
	.string "おしえて くれても いいじゃないー$"

Text_DecidedNotTo:
	.string "{JPN}あ やめちゃうの?\n"
	.string "けっこう きまぐれねー$"

Text_HiWeMeetAgain:
	.string "{JPN}ハーイ また あったわね!$"

Text_ChangeYourProfile:
	.string "{JPN}あたらしい プロフィールが\n"
	.string "できたのかな?$"

Text_WhatAreYouLikeNow:
	.string "{JPN}いまの あなたは どんな かんじ?\n"
	.string "さっそく おしえて!$"

Text_LikeProfileWayItIs:
	.string "{JPN}いまの ままが いいのね\p"
	.string "わたしも それがいいと おもうわ$"

Text_ProfileSaysSomethingAboutYou:
	.string "{JPN}ふむふむ\p"
	.string "あなたって ひとが よく わかる\n"
	.string "すてきな じこしょうかいね!\l"
	.string "わたしたち これで\l"
	.string "もっと なかよしに なったわね!\l"
	.string "うふっ$"

Text_HeardThatBefore:
	.string "{JPN}なんか きいた ことあるわ それ……\n"
	.string "とおくに すんでる いとこが\l"
	.string "いつも いっていた ような……?$"

Text_TellMoreNextTime:
	.string "{JPN}またこんど あなたのこと\n"
	.string "もっと おしえてねっ!$"
