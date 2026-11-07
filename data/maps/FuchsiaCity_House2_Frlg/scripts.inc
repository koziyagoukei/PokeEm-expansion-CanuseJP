FuchsiaCity_House2_Frlg_MapScripts::
	.byte 0

FuchsiaCity_House2_EventScript_FishingGurusBrother::
	lock
	faceplayer
	goto_if_set FLAG_GOT_GOOD_ROD, FuchsiaCity_House2_EventScript_AlreadyGotGoodRod
	msgbox FuchsiaCity_House2_Text_DoYouLikeToFish, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, FuchsiaCity_House2_EventScript_GiveGoodRod
	msgbox FuchsiaCity_House2_Text_OhThatsDisappointing
	release
	end

FuchsiaCity_House2_EventScript_AlreadyGotGoodRod::
	msgbox FuchsiaCity_House2_Text_HowAreTheFishBiting
	release
	end

FuchsiaCity_House2_EventScript_GiveGoodRod::
	msgbox FuchsiaCity_House2_Text_LikeYourStyleTakeThis
	checkitemspace ITEM_GOOD_ROD
	goto_if_eq VAR_RESULT, FALSE, FuchsiaCity_House2_EventScript_NoRoomForGoodRod
	giveitem_msg FuchsiaCity_House2_Text_ReceivedGoodRod, ITEM_GOOD_ROD
	msgbox FuchsiaCity_House2_Text_GoodRodCanCatchBetterMons
	setflag FLAG_GOT_GOOD_ROD
	release
	end

FuchsiaCity_House2_EventScript_NoRoomForGoodRod::
	msgbox FuchsiaCity_House2_Text_YouHaveNoRoomForGift
	release
	end

FuchsiaCity_House2_Text_DoYouLikeToFish::
	.string "{JPN}わしは つりおやじの あに じゃ!\p"
	.string "もう しぬほど\n"
	.string "つりが すき なんだが\p"
	.string "きみは つり すき かな?$"

FuchsiaCity_House2_Text_LikeYourStyleTakeThis::
	.string "{JPN}そうか!\n"
	.string "きみとは きが あいそうだ!\p"
	.string "これ あげるから\n"
	.string "きみも つり まくりなさいよ!$"

FuchsiaCity_House2_Text_ReceivedGoodRod::
	.string "{JPN}{PLAYER}は つりおやじから\n"
	.string "いいつりざおを もらった!$"

FuchsiaCity_House2_Text_GoodRodCanCatchBetterMons::
	.string "{JPN}つり こそ\n"
	.string "さいこうの ロマン だ!\p"
	.string "へぼいつりざおは\n"
	.string "コイキングしか つれ なんだが\n"
	.string "この いいつりざおなら\n"
	.string "もっと いいもんが つれるんじゃ!$"

FuchsiaCity_House2_Text_OhThatsDisappointing::
	.string "{JPN}なんじゃ… がっかりだ$"

FuchsiaCity_House2_Text_HowAreTheFishBiting::
	.string "{JPN}やー {PLAYER}{KUN}!\p"
	.string "つり まくっとる かね!$"

FuchsiaCity_House2_Text_YouHaveNoRoomForGift::
	.string "{JPN}ありゃー!\p"
	.string "いいもの あげようと したのに\n"
	.string "にもつが いっぱいじゃ!$"

