MysteryGiftScript_BattleCard::
	setvaddress MysteryGiftScript_BattleCard
	vgoto_if_set FLAG_MYSTERY_GIFT_DONE, MysteryGiftScript_BattleCardInfo
	setorcopyvar VAR_RESULT, GET_CARD_BATTLES_WON
	specialvar VAR_0x8008, GetMysteryGiftCardStat
	vgoto_if_ne VAR_0x8008, REQUIRED_CARD_BATTLES, MysteryGiftScript_BattleCardInfo
	lock
	faceplayer
	vmessage sText_MysteryGiftBattleCountCard_WonPrize
	waitmessage
	waitbuttonpress
	giveitem ITEM_POTION
	release
	setflag FLAG_MYSTERY_GIFT_DONE
	end

MysteryGiftScript_BattleCardInfo:
	lock
	faceplayer
	vmessage sText_MysteryGiftBattleCountCard
	waitmessage
	waitbuttonpress
	release
	end

sText_MysteryGiftBattleCountCard:
	.string "{JPN}ふしぎなおくりもの を ごりよう\nいただき ありがとう ございます\pあなたの もっている\nたいせん カウント カード は\pおなじ カードを もっている\nトレーナーと たいせん することで\lせいせきが きろくされて いきます\pぜひ おなじ カードをもつ\nトレーナーと たいせん してください\pまた ニュースを よみこむことで\nランキングを みれますので\lぜひ ごりよう ください!$"

sText_MysteryGiftBattleCountCard_WonPrize:
	.string "{JPN}ふしぎなおくりもの を ごりよう\nいただき ありがとう ございます\pおめでとう ございます!\pただいま たいせんで 3しょうした\nトレーナーには プレゼントを\lさしあげて います!\pこれからも どんどん\nたいせんを たのしんで ください!$"
