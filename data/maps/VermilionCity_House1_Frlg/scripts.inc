VermilionCity_House1_Frlg_MapScripts::
	.byte 0

VermilionCity_House1_EventScript_FishingGuru::
	lock
	faceplayer
	goto_if_set FLAG_GOT_OLD_ROD, VermilionCity_House1_EventScript_AlreadyGotOldRod
	msgbox VermilionCity_House1_Text_ImFishingGuruDoYouLikeToFish, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, VermilionCity_House1_EventScript_GiveOldRod
	msgbox VermilionCity_House1_Text_OhThatsSoDisappointing
	release
	end

VermilionCity_House1_EventScript_AlreadyGotOldRod::
	msgbox VermilionCity_House1_Text_HowAreTheFishBiting
	release
	end

VermilionCity_House1_EventScript_GiveOldRod::
	checkitemspace ITEM_OLD_ROD
	goto_if_eq VAR_RESULT, FALSE, VermilionCity_House1_EventScript_NoRoomForOldRod
	additem ITEM_OLD_ROD
	msgbox VermilionCity_House1_Text_TakeThisAndFish
	msgreceiveditem VermilionCity_House1_Text_ReceivedOldRodFromFishingGuru, ITEM_OLD_ROD
	setflag FLAG_GOT_OLD_ROD
	msgbox VermilionCity_House1_Text_FishingIsAWayOfLife
	release
	end

VermilionCity_House1_EventScript_NoRoomForOldRod::
	msgbox VermilionCity_House1_Text_NoRoomForNiceGift
	release
	end

VermilionCity_House1_Text_ImFishingGuruDoYouLikeToFish::
	.string "{JPN}わしは つりおやじ じゃ!\p"
	.string "もう しぬほど\n"
	.string "つりが すき なんだが\p"
	.string "きみは つり すき かな?$"

VermilionCity_House1_Text_TakeThisAndFish::
	.string "{JPN}そうか!\n"
	.string "きみとは きが あいそうだ!\p"
	.string "これ あげるから\n"
	.string "きみも つり まくりなさいよ!$"

VermilionCity_House1_Text_ReceivedOldRodFromFishingGuru::
	.string "{JPN}{PLAYER}は つりおやじから\n"
	.string "ボロのつりざおを もらった!$"

VermilionCity_House1_Text_FishingIsAWayOfLife::
	.string "{JPN}つり こそ\n"
	.string "さいこうの ロマン だ!\p"
	.string "うみでも かわでも!\p"
	.string "えんりょ なく\n"
	.string "つりざおを つかって くれい!$"

VermilionCity_House1_Text_OhThatsSoDisappointing::
	.string "{JPN}なんじゃ… がっかりだ$"

VermilionCity_House1_Text_HowAreTheFishBiting::
	.string "{JPN}やー {PLAYER}{KUN}!\p"
	.string "つり まくっとる かね!$"

VermilionCity_House1_Text_NoRoomForNiceGift::
	.string "{JPN}ありゃー!\p"
	.string "いいもの あげようと したのに\n"
	.string "にもつが いっぱいじゃ!$"

