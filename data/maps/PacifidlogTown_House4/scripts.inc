PacifidlogTown_House3_MapScripts::
	.byte 0

PacifidlogTown_House3_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_PACIFIDLOG_NPC_TRADE_COMPLETED, PacifidlogTown_House3_EventScript_TradeCompleted
	ingame_trade INGAME_TRADE_HORSEA, PacifidlogTown_House3_Text_WillingToTradeIt, PacifidlogTown_House3_EventScript_DeclineTrade, PacifidlogTown_House3_EventScript_NotRequestedMon, PacifidlogTown_House3_Text_ItsSubtlyDifferentThankYou
	setflag FLAG_PACIFIDLOG_NPC_TRADE_COMPLETED
	release
	end

PacifidlogTown_House3_EventScript_DeclineTrade::
	msgbox PacifidlogTown_House3_Text_NotDesperateOrAnything, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House3_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox PacifidlogTown_House3_Text_WontAcceptAnyLessThanRealMon, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House3_EventScript_TradeCompleted::
	msgbox PacifidlogTown_House3_Text_ReallyWantedToGetBagon, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House3_EventScript_Girl::
	msgbox PacifidlogTown_House3_Text_IsThatAPokedex, MSGBOX_NPC
	end

PacifidlogTown_House3_Text_WillingToTradeIt:
	.string "{JPN}みてくれよ この {STR_VAR_2}!\nおれが きのう じぶんの たんじょうびに\lきねんに つかまえた {STR_VAR_2}だ!\pお! ほしい って かおを したな?\nまっ きちょうな {STR_VAR_2}だからな!\pよし! {STR_VAR_1}と だったら\nこうかん して やっても いいぞ!$"

PacifidlogTown_House3_Text_ItsSubtlyDifferentThankYou:
	.string "{JPN}おっ これが {STR_VAR_1} かあー!\nやっぱ {STR_VAR_2} とは\lいろいろ にている ようで\lびみょうに ちがうなあ!\pサンキュ!$"

PacifidlogTown_House3_Text_WontAcceptAnyLessThanRealMon:
	.string "{JPN}だめだめ! {STR_VAR_1} じゃないと \nひきうけられないね!$"

PacifidlogTown_House3_Text_NotDesperateOrAnything:
	.string "{JPN}ふーん やめるのか……\pまっ おれのほうも\nどうしても って わけじゃ ないしね$"

PacifidlogTown_House3_Text_ReallyWantedToGetBagon:
	.string "{JPN}じぶんで さがしても いいんだけど……\pほかの トレーナーが つかまえた\nタツベイ って いうのが\lどうしても ほしかったんだよね$"

PacifidlogTown_House3_Text_IsThatAPokedex:
	.string "{JPN}それって ポケモンずかん?\pそれって あなた たくさんの\nポケモンと であった ってこと?\pそれって すっごい うらやましい!$"
