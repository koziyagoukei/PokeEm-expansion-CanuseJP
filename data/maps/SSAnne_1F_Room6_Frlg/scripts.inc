SSAnne_1F_Room6_Frlg_MapScripts::
	.byte 0

SSAnne_1F_Room6_EventScript_Woman::
	lock
	faceplayer
	msgbox SSAnne_1F_Room6_Text_TakeAShortRest, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SSAnne_1F_Room6_EventScript_DeclineHeal
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox SSAnne_1F_Room6_Text_GladEveryoneIsRefreshed
	release
	end

SSAnne_1F_Room6_EventScript_DeclineHeal::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, SSAnne_1F_Room6_EventScript_DeclineHealMale
	msgbox SSAnne_1F_Room6_Text_SorryYouLookLikeMySister
	release
	end

SSAnne_1F_Room6_EventScript_DeclineHealMale::
	msgbox SSAnne_1F_Room6_Text_SorryYouLookLikeMyBrother
	release
	end

SSAnne_1F_Room6_Text_TakeAShortRest::
	.string "{JPN}おつかれの ようですわね\n"
	.string "ここで すこし\l"
	.string "おやすみに なりません?$"

SSAnne_1F_Room6_Text_GladEveryoneIsRefreshed::
	.string "{JPN}よかった\n"
	.string "みんな げんきに なりましたわ$"

SSAnne_1F_Room6_Text_SorryYouLookLikeMyBrother::
	.string "{JPN}おせっかいで ごめんなさいね\n"
	.string "あなたが おとうとに にていたから…$"

SSAnne_1F_Room6_Text_SorryYouLookLikeMySister::
	.string "{JPN}おせっかいで ごめんなさいね\n"
	.string "あなたが いもうとに にていたから…$"

