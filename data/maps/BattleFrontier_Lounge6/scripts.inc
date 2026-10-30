BattleFrontier_Lounge6_MapScripts::
	.byte 0

BattleFrontier_Lounge6_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_BATTLE_FRONTIER_TRADE_DONE, BattleFrontier_Lounge6_EventScript_TradeCompleted
	ingame_trade INGAME_TRADE_MEOWTH, BattleFrontier_Lounge6_Text_WouldYouLikeToTrade, BattleFrontier_Lounge6_EventScript_DeclineTrade, BattleFrontier_Lounge6_EventScript_NotRequestedMon, BattleFrontier_Lounge6_Text_PromiseIllBeGoodToIt
	setflag FLAG_BATTLE_FRONTIER_TRADE_DONE
	release
	end

BattleFrontier_Lounge6_EventScript_DeclineTrade::
	msgbox BattleFrontier_Lounge6_Text_WellThatsFineToo, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox BattleFrontier_Lounge6_Text_DontTradeForAnythingButMon, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_TradeCompleted::
	msgbox BattleFrontier_Lounge6_Text_SkittySoMuchCuterThanImagined, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_Text_WouldYouLikeToTrade:
	.string "{JPN}わたしの {STR_VAR_2}\nかわいいでしょう？\p{STR_VAR_1}と 交換して\n新しい であいを 作りませんか？$"

BattleFrontier_Lounge6_Text_PromiseIllBeGoodToIt:
	.string "{JPN}わー かわいい ですねー……!\nどうも ありがとう\l大切に しますよ!\pあっ {STR_VAR_2}も\nよろしく おねがい します!$"

BattleFrontier_Lounge6_Text_DontTradeForAnythingButMon:
	.string "あ ゴメンなさい!\nわたし {STR_VAR_1} 以外とは\l交換 するつもり ないんでー$"

BattleFrontier_Lounge6_Text_WellThatsFineToo:
	.string "あ やめますか!\n別に いいですよ\lまた 今度 来てくださいー$"

BattleFrontier_Lounge6_Text_SkittySoMuchCuterThanImagined:
	.string "{JPN}この エネコと いると\n前の トレーナーの たのしさまで\lつたわってくる きがします！$"
