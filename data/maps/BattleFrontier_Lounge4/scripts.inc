BattleFrontier_Lounge4_MapScripts::
	.byte 0

BattleFrontier_Lounge4_EventScript_Woman::
	msgbox BattleFrontier_Lounge4_Text_WonderIfInterviewsAiring, MSGBOX_NPC
	end

BattleFrontier_Lounge4_EventScript_Cook::
	msgbox BattleFrontier_Lounge4_Text_IfIOpenedRestaurantHere, MSGBOX_NPC
	end

BattleFrontier_Lounge4_EventScript_Man::
	msgbox BattleFrontier_Lounge4_Text_NeedBreatherAfterBattles, MSGBOX_NPC
	end

BattleFrontier_Lounge4_Text_WonderIfInterviewsAiring:
	.string "{JPN}テレビの インタビューが すきなの。\nあのときの ドキドキが よみがえるから。\pなんど みても あきないわ。$"

BattleFrontier_Lounge4_Text_IfIOpenedRestaurantHere:
	.string "{JPN}ひがしの リゾートに\nめしやが できたんやて。\pどんな あじやろなあ。\nこんど おしえてもらお！$"

BattleFrontier_Lounge4_Text_NeedBreatherAfterBattles:
	.string "{JPN}ふう。 ここで ひとやすみ。\nポケモンも のんびり してるだろ。\pつぎの さくせんは\nおちゃを のんでからだ！$"

