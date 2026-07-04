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
	.string "{JPN}わたしの この ポケモン\n{STR_VAR_2}って いうんですよ しってます?\lなかなか かわいくて イイかんじ ですよー\pこのコ だったら わたし\nむねをはって こうかんに だせます!\pどうです?\nわたしの この {STR_VAR_2}\l{STR_VAR_1}と こうかん しません?$"

BattleFrontier_Lounge6_Text_PromiseIllBeGoodToIt:
	.string "{JPN}わー かわいい ですねー……!\nどうも ありがとう\lたいせつに しますよ!\pあっ {STR_VAR_2}も\nよろしく おねがい します!$"

BattleFrontier_Lounge6_Text_DontTradeForAnythingButMon:
	.string "{JPN}あ ゴメンなさい!\nわたし {STR_VAR_1} いがいとは\lこうかん するつもり ないんでー$"

BattleFrontier_Lounge6_Text_WellThatsFineToo:
	.string "{JPN}あ やめますか!\nべつに いいですよ\lまた こんど きてくださいー$"

BattleFrontier_Lounge6_Text_SkittySoMuchCuterThanImagined:
	.string "{JPN}うふふ!\nエネコが おもってたより ずっと\lかわいくて ほんと うれしいですー$"
