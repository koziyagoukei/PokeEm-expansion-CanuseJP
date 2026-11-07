Route1_Frlg_MapScripts::
	.byte 0

Route1_EventScript_MartClerk::
	lock
	faceplayer
	goto_if_set FLAG_GOT_POTION_ON_ROUTE_1, Route1_EventScript_AlreadyGotPotion
	msgbox Route1_Text_WorkAtPokeMartTakeSample
	textcolor NPC_TEXT_COLOR_NEUTRAL
	checkitemspace ITEM_POTION
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	bufferitemname STR_VAR_2, ITEM_POTION
	playfanfare MUS_LEVEL_UP
	message gText_ObtainedTheItem
	waitmessage
	waitfanfare
	additem ITEM_POTION
	msgbox Route1_Text_PutPotionAway
	call EventScript_RestorePrevTextColor
	setflag FLAG_GOT_POTION_ON_ROUTE_1
	release
	end

Route1_EventScript_AlreadyGotPotion::
	msgbox Route1_Text_ComeSeeUsIfYouNeedPokeBalls
	release
	end

Route1_EventScript_Boy::
	msgbox Route1_Text_CanJumpFromLedges, MSGBOX_NPC
	end

Route1_EventScript_RouteSign::
	msgbox Route1_Text_RouteSign, MSGBOX_SIGN
	end

Route1_Text_WorkAtPokeMartTakeSample::
	.string "{JPN}わたし フレンドリィ ショップの\n"
	.string "てんいん です\p"
	.string "べんりな どうぐや ですから\n"
	.string "トキワ シティで\l"
	.string "ぜひ よって くださいね!\p"
	.string "そうだ!\n"
	.string "みほんを さしあげましょう\l"
	.string "… どうぞ!$"

Route1_Text_ComeSeeUsIfYouNeedPokeBalls::
	.string "{JPN}ポケモンを つかまえる\n"
	.string "モンスターボール かう ときも\l"
	.string "ショップへ いらして ください!$"

Route1_Text_PutPotionAway::
	.string "{JPN}{PLAYER}は キズぐすりを バッグの\n"
	.string "どうぐポケットに しまった!$"

Route1_Text_CanJumpFromLedges::
	.string "{JPN}みちに だんさが あるだろ!\p"
	.string "とび おりるのは\n"
	.string "ちょっと こわいけど…\p"
	.string "マサラ タウンに かえる ときは\n"
	.string "はやく かえれて いいよ$"

Route1_Text_RouteSign::
	.string "{JPN}ここは 1ばん どうろ\n"
	.string "マサラ タウン … トキワ シティ$"

