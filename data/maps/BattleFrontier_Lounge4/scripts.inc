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
	.string "{JPN}つよい トレーナーの インタビュー\nきょうは やって いるかなー?$"

BattleFrontier_Lounge4_Text_IfIOpenedRestaurantHere:
	.string "{JPN}ここで おいしい レストランでも\nやったら もうかりそうやな$"

BattleFrontier_Lounge4_Text_NeedBreatherAfterBattles:
	.string "{JPN}ふう\pたたかいの あいまに\nちょっと ひとやすみ……\pと いっても\nおれの あたまの なかでは\lやすみなく せんりゃくが ねられているぜ!$"

