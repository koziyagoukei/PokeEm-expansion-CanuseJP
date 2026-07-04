RustboroCity_House1_MapScripts::
	.byte 0

RustboroCity_House1_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_RUSTBORO_NPC_TRADE_COMPLETED, RustboroCity_House1_EventScript_TradeCompleted
	ingame_trade INGAME_TRADE_SEEDOT, RustboroCity_House1_Text_IllTradeIfYouWant, RustboroCity_House1_EventScript_DeclineTrade, RustboroCity_House1_EventScript_NotRequestedMon, RustboroCity_House1_Text_PleaseBeGoodToMyPokemon
	setflag FLAG_RUSTBORO_NPC_TRADE_COMPLETED
	release
	end

RustboroCity_House1_EventScript_DeclineTrade::
	msgbox RustboroCity_House1_Text_YouDontWantToThatsOkay, MSGBOX_DEFAULT
	release
	end

RustboroCity_House1_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox RustboroCity_House1_Text_DoesntLookLikeMonToMe, MSGBOX_DEFAULT
	release
	end

RustboroCity_House1_EventScript_TradeCompleted::
	msgbox RustboroCity_House1_Text_AnyPokemonCanBeCute, MSGBOX_DEFAULT
	release
	end

RustboroCity_House1_EventScript_Hiker::
	msgbox RustboroCity_House1_Text_AllSortsOfPlaces, MSGBOX_NPC
	end

RustboroCity_House1_Text_IllTradeIfYouWant:
	.string "{JPN}え?\nぼくの ポケモンが かわいい?\lウン しってるよー\pあっ どうしても って いうなら\nこうかん して あげても いいよ\pじゃあ ぼくの {STR_VAR_2}\n{STR_VAR_1} となら こうかん するよ$"

RustboroCity_House1_Text_PleaseBeGoodToMyPokemon:
	.string "{JPN}えへへ……\nちゃんと かわいがってね$"

RustboroCity_House1_Text_DoesntLookLikeMonToMe:
	.string "{JPN}えー どうみても {STR_VAR_1}に\nみえないんだけど……$"

RustboroCity_House1_Text_YouDontWantToThatsOkay:
	.string "{JPN}あ いやなら いいけど……\nせっかく かわいい ポケモンなのに$"

RustboroCity_House1_Text_AnyPokemonCanBeCute:
	.string "{JPN}だいじに そだてると\nどんな ポケモンも すごく かわいいよねー$"

RustboroCity_House1_Text_AllSortsOfPlaces:
	.string "{JPN}いろんな ばしょに\nいろんな ポケモンが いる\lいろんな ひとも いる\pそれが たのしみで わしも\nいろんな ところに でかけるのさ!$"

