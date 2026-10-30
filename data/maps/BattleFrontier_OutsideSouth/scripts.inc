BattleFrontier_OutsideSouth_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_OutsideSouth_OnTransition
	.byte 0

BattleFrontier_OutsideSouth_OnTransition:
	setrespawn HEAL_LOCATION_BATTLE_FRONTIER_OUTSIDE_EAST
	end

BattleFrontier_OutsideSouth_EventScript_HubSign::
	msgbox BattleFrontier_OutsideSouth_Text_Hub, MSGBOX_SIGN
	end

BattleFrontier_OutsideSouth_EventScript_ContestSign::
	msgbox BattleFrontier_OutsideSouth_Text_Contest, MSGBOX_SIGN
	end

BattleFrontier_OutsideSouth_EventScript_MuseumSign::
	msgbox BattleFrontier_OutsideSouth_Text_Museum, MSGBOX_SIGN
	end

BattleFrontier_OutsideSouth_EventScript_FacilitiesSign::
	msgbox BattleFrontier_OutsideSouth_Text_Facilities, MSGBOX_SIGN
	end

BattleFrontier_OutsideSouth_EventScript_ResortSign::
	msgbox BattleFrontier_OutsideSouth_Text_Resort, MSGBOX_SIGN
	end

BattleFrontier_OutsideSouth_Text_Hub:
	.string "フロンティア ハブ\n挑戦の 準備は こちらで！$"

BattleFrontier_OutsideSouth_Text_Contest:
	.string "ポケモン コンテストかいじょう$"

BattleFrontier_OutsideSouth_Text_Museum:
	.string "フロンティア びじゅつかん$"

BattleFrontier_OutsideSouth_Text_Facilities:
	.string "この先 バトルフロンティア\n"
	.string "7つの バトル施設$"

BattleFrontier_OutsideSouth_Text_Resort:
	.string "ミニミニリゾート ゲート\p"
	.string "この先 ミニミニキャンプ\n"
	.string "さらに 南に ミニミニ別荘$"
