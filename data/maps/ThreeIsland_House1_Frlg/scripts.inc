ThreeIsland_House1_Frlg_MapScripts::
	.byte 0

ThreeIsland_House1_EventScript_Lostelle::
	lock
	faceplayer
	special GetPlayerBigGuyGirlString
	msgbox ThreeIsland_House1_Text_YoureAlwaysSoCool
	release
	end

ThreeIsland_House1_EventScript_DisplayCase::
	msgbox ThreeIsland_House1_Text_RareRocksGemsDisplayed, MSGBOX_SIGN
	end

ThreeIsland_House1_Text_YoureAlwaysSoCool::
	.string "{JPN}マヨ『わー {PLAYER}だ\n"
	.string "いっつも かっこいいねえ\p"
	.string "いつか {PLAYER} にも\n"
	.string "おべんとう つくって あげる!$"

ThreeIsland_House1_Text_RareRocksGemsDisplayed::
	.string "{JPN}めずらしい いしが かざってある!$"

