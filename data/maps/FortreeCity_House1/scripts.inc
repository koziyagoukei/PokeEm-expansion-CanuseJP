FortreeCity_House1_MapScripts::
	.byte 0

FortreeCity_House1_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_FORTREE_NPC_TRADE_COMPLETED, FortreeCity_House1_EventScript_TradeCompleted
	ingame_trade INGAME_TRADE_PLUSLE, FortreeCity_House1_Text_YouWillTradeWontYou, FortreeCity_House1_EventScript_DeclineTrade, FortreeCity_House1_EventScript_NotRequestedMon, FortreeCity_House1_Text_MonYouTakeCare
	setflag FLAG_FORTREE_NPC_TRADE_COMPLETED
	release
	end

FortreeCity_House1_EventScript_DeclineTrade::
	msgbox FortreeCity_House1_Text_YouWontTradeMe, MSGBOX_DEFAULT
	release
	end

FortreeCity_House1_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox FortreeCity_House1_Text_ThisIsntAMon, MSGBOX_DEFAULT
	release
	end

FortreeCity_House1_EventScript_TradeCompleted::
	msgbox FortreeCity_House1_Text_GoingToMakeVolbeatStrong, MSGBOX_DEFAULT
	release
	end

FortreeCity_House1_EventScript_ExpertF::
	msgbox FortreeCity_House1_Text_TradingMemoriesWithOthers, MSGBOX_NPC
	end

FortreeCity_House1_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox FortreeCity_House1_Text_Zigzagoon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

FortreeCity_House1_Text_YouWillTradeWontYou:
	.string "{JPN}うぉー!\nどうしても {STR_VAR_1}が ほしーっ!\l……と\lこころの さけびを きかれて しまった!\pきいた からには\nきみの {STR_VAR_1}と ボクの {STR_VAR_2}を\lもちろん こうかん するでしょ!$"

FortreeCity_House1_Text_MonYouTakeCare:
	.string "{JPN}おー やったー!\p{STR_VAR_1} よくきたなー!\n{STR_VAR_2} げんきで やれよー!$"

FortreeCity_House1_Text_ThisIsntAMon:
	.string "{JPN}いやソレ\n{STR_VAR_1}じゃないでしょ$"

FortreeCity_House1_Text_YouWontTradeMe:
	.string "{JPN}え こうかん しないの?\nこころの さけびを きいたくせに……$"

FortreeCity_House1_Text_GoingToMakeVolbeatStrong:
	.string "{JPN}よし いまから\nバルビートを つよく するぞー!\lプラスルも つよく してやってよ!$"

FortreeCity_House1_Text_TradingMemoriesWithOthers:
	.string "{JPN}ひとと ポケモンを こうかん する……\pつまりは それぞれの\nおもいでを こうかん しているのじゃな$"

FortreeCity_House1_Text_Zigzagoon:
	.string "{JPN}ジグザグマ『グモモォ?$"

