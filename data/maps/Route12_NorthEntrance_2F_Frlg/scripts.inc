Route12_NorthEntrance_2F_Frlg_MapScripts::
	.byte 0

Route12_NorthEntrance_2F_EventScript_LeftBinoculars::
	msgbox Route12_NorthEntrance_2F_Text_TheresManFishing, MSGBOX_SIGN
	end

Route12_NorthEntrance_2F_EventScript_RightBinoculars::
	msgbox Route12_NorthEntrance_2F_Text_ItsPokemonTower, MSGBOX_SIGN
	end

Route12_NorthEntrance_2F_EventScript_Lass::
	lock
	faceplayer
	goto_if_set FLAG_GOT_TM27, Route12_NorthEntrance_2F_EventScript_ExplainTM27
	checkplayergender
	call_if_eq VAR_RESULT, MALE, Route12_NorthEntrance_2F_EventScript_TakeTMMale
	call_if_eq VAR_RESULT, FEMALE, Route12_NorthEntrance_2F_EventScript_TakeTMFemale
	checkitemspace ITEM_TM27
	goto_if_eq VAR_RESULT, FALSE, Route12_NorthEntrance_2F_EventScript_NoRoomForTM27
	giveitem_msg Route12_NorthEntrance_2F_Text_ReceivedTM27FromLittleGirl, ITEM_TM27
	msgbox Route12_NorthEntrance_2F_Text_ExplainTM27
	setflag FLAG_GOT_TM27
	release
	end

Route12_NorthEntrance_2F_EventScript_TakeTMMale::
	msgbox Route12_NorthEntrance_2F_Text_TakeTMDontNeedAnymoreMale
	return

Route12_NorthEntrance_2F_EventScript_TakeTMFemale::
	msgbox Route12_NorthEntrance_2F_Text_TakeTMDontNeedAnymoreFemale
	return

Route12_NorthEntrance_2F_EventScript_NoRoomForTM27::
	msgbox Route12_NorthEntrance_2F_Text_DontHaveRoomForThis
	release
	end

Route12_NorthEntrance_2F_EventScript_ExplainTM27::
	msgbox Route12_NorthEntrance_2F_Text_ExplainTM27
	release
	end

@ Male and female text identical, differ (presumably) in JP
Route12_NorthEntrance_2F_Text_TakeTMDontNeedAnymoreMale::
	.string "{JPN}わたしが かわいがってた ポケモン\n"
	.string "いまは ポケモン タワーで\l"
	.string "ねむってるの……\p"
	.string "…… そうだ わたしの わざマシン\n"
	.string "おにいちゃんに あげるわ\l"
	.string "わたし…… もう いらない から$"

Route12_NorthEntrance_2F_Text_TakeTMDontNeedAnymoreFemale::
	.string "{JPN}わたしが かわいがってた ポケモン\n"
	.string "いまは ポケモン タワーで\l"
	.string "ねむってるの……\p"
	.string "…… そうだ わたしの わざマシン\n"
	.string "おねえちゃんに あげるわ\l"
	.string "わたし…… もう いらない から$"

Route12_NorthEntrance_2F_Text_ReceivedTM27FromLittleGirl::
	.string "{JPN}{PLAYER}は おんなのこから\n"
	.string "わざマシン27を もらった!$"

Route12_NorthEntrance_2F_Text_ExplainTM27::
	.string "{JPN}わざマシン27は\n"
	.string "おんがえし という わざなの……\p"
	.string "ポケモンを たいせつに していれば\n"
	.string "しょうぶの ときに\l"
	.string "きっと こたえてくれるよ!$"

Route12_NorthEntrance_2F_Text_DontHaveRoomForThis::
	.string "{JPN}にもつ いっぱいで もてないよ$"

Route12_NorthEntrance_2F_Text_TheresManFishing::
	.string "{JPN}ぼうえんきょうを のぞいた!\p"
	.string "つりを してる おじさんだ!$"

Route12_NorthEntrance_2F_Text_ItsPokemonTower::
	.string "{JPN}ぼうえんきょうを のぞいた!\p"
	.string "ポケモン タワーが みえる!$"

