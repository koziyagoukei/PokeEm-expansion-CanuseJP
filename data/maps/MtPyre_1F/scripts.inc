MtPyre_1F_MapScripts::
	.byte 0

MtPyre_1F_EventScript_CleanseTagWoman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_CLEANSE_TAG, MtPyre_1F_EventScript_ReceivedCleanseTag
	msgbox MtPyre_1F_Text_TakeThisForYourOwnGood, MSGBOX_DEFAULT
	giveitem ITEM_CLEANSE_TAG
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_CLEANSE_TAG
	release
	end

MtPyre_1F_EventScript_ReceivedCleanseTag::
	msgbox MtPyre_1F_Text_ExplainCleanseTag, MSGBOX_DEFAULT
	release
	end

MtPyre_1F_EventScript_PokefanF::
	msgbox MtPyre_1F_Text_ComeToPayRespects, MSGBOX_NPC
	end

MtPyre_1F_EventScript_Man::
	msgbox MtPyre_1F_Text_RestingPlaceOfZigzagoon, MSGBOX_NPC
	end

MtPyre_1F_Text_TakeThisForYourOwnGood:
	.string "この おくりびやま には\nいろんな ものが さまよっておる……\pなにが おきるか わからん\nねんのため これを もっていきなされ"
	.string ""
	.string ""
	.string "$"

MtPyre_1F_Text_ExplainCleanseTag:
	.string "きよめのおふだ をな\nポケモンに もたせて おく\pすると やせいの ポケモンが\nあんまり とびだして こなくなるぞよ"
	.string ""
	.string "$"

MtPyre_1F_Text_ComeToPayRespects:
	.string "あなたも おはか まいりに きたの?\nずいぶんと ポケモン おもい なのね"
	.string ""
	.string "$"

MtPyre_1F_Text_RestingPlaceOfZigzagoon:
	.string "ここには ぼくが たいせつに していた\nジグザグマが やすらかに ねむってる……"
	.string "$"
