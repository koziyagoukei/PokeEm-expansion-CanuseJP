FiveIsland_Meadow_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, FiveIsland_Meadow_OnLoad
	.byte 0

FiveIsland_Meadow_OnLoad::
	call_if_set FLAG_UNLOCKED_ROCKET_WAREHOUSE, FiveIsland_Meadow_EventScript_SetWarehouseDoorUnlocked
	end

FiveIsland_Meadow_EventScript_WarehouseDoor::
	lockall
	goto_if_set FLAG_UNLOCKED_ROCKET_WAREHOUSE, FiveIsland_Meadow_EventScript_WarehouseDoorAlreadyOpen
	goto_if_set FLAG_LEARNED_YES_NAH_CHANSEY, FiveIsland_Meadow_EventScript_OpenWarehouseDoor
	msgbox FiveIsland_Meadow_Text_EnteredPasswordAnotherNeeded
	releaseall
	end

FiveIsland_Meadow_EventScript_OpenWarehouseDoor::
	setflag FLAG_UNLOCKED_ROCKET_WAREHOUSE
	message FiveIsland_Meadow_Text_EnteredPasswordDoorOpened
	waitmessage
	delay 60
	call FiveIsland_Meadow_EventScript_SetWarehouseDoorUnlocked
	special DrawWholeMapView
	playse SE_PIN
	waitse
	releaseall
	end

FiveIsland_Meadow_EventScript_WarehouseDoorAlreadyOpen::
	msgbox FiveIsland_Meadow_Text_WarehouseDoorAlreadyOpen
	releaseall
	end

FiveIsland_Meadow_EventScript_SetWarehouseDoorUnlocked::
	setmetatile 12, 21, METATILE_SeviiIslands45_RocketWarehouseDoor_Unlocked, 1
	return

FiveIsland_Meadow_EventScript_Rocket1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_49, FiveIsland_Meadow_Text_Rocket1Intro, FiveIsland_Meadow_Text_Rocket1Defeat
	msgbox FiveIsland_Meadow_Text_Rocket1PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_Meadow_EventScript_Rocket2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_50, FiveIsland_Meadow_Text_Rocket2Intro, FiveIsland_Meadow_Text_Rocket2Defeat
	msgbox FiveIsland_Meadow_Text_Rocket2PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_Meadow_EventScript_Rocket3::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_51, FiveIsland_Meadow_Text_Rocket3Intro, FiveIsland_Meadow_Text_Rocket3Defeat
	msgbox FiveIsland_Meadow_Text_Rocket3PostBattle, MSGBOX_AUTOCLOSE
	end

@ Unused
FiveIsland_Meadow_Text_DoorNeedsTwoPasswords::
	.string "とびらを あけるには\n"
	.string "パスワードが 2つ いるようだ$"

FiveIsland_Meadow_Text_EnteredPasswordAnotherNeeded::
	.string "{JPN}{PLAYER}は パスワードを\n"
	.string "にゅうりょく した\p"
	.string "『またまた タマタマ』\p"
	.string "…… …… ……\p"
	.string "とびらを あけるには もうひとつ\n"
	.string "パスワードが ひつようだ$"

FiveIsland_Meadow_Text_EnteredPasswordDoorOpened::
	.string "{JPN}{PLAYER}は パスワードを\n"
	.string "にゅうりょく した\p"
	.string "『またまた タマタマ』\n"
	.string "『カブトは とぶか』\p"
	.string "…… …… ……\p"
	.string "そうこの とびらが ひらいた!$"

FiveIsland_Meadow_Text_WarehouseDoorAlreadyOpen::
	.string "{JPN}そうこの とびらは\n"
	.string "すでに ひらいている$"

FiveIsland_Meadow_Text_Rocket1Intro::
	.string "{JPN}ここは たちいり きんし\n"
	.string "はいって くんな!$"

FiveIsland_Meadow_Text_Rocket1Defeat::
	.string "{JPN}ほんとかよ$"

FiveIsland_Meadow_Text_Rocket1PostBattle::
	.string "{JPN}おまえ かんけい ないだろ\n"
	.string "かえれよ!$"

FiveIsland_Meadow_Text_Rocket2Intro::
	.string "{JPN}この さきは\n"
	.string "おれたちの そうこ だぜ\l"
	.string "なんの ようだ?$"

FiveIsland_Meadow_Text_Rocket2Defeat::
	.string "{JPN}こんな はずじゃ…$"

FiveIsland_Meadow_Text_Rocket2PostBattle::
	.string "{JPN}その そうこには\n"
	.string "なにが はいって いるかって?\p"
	.string "くっくっく…\n"
	.string "しらない ほうが みのためだぜ$"

FiveIsland_Meadow_Text_Rocket3Intro::
	.string "{JPN}この さきに\n"
	.string "いっても ムダよ!$"

FiveIsland_Meadow_Text_Rocket3Defeat::
	.string "{JPN}あたま きたわ$"

FiveIsland_Meadow_Text_Rocket3PostBattle::
	.string "{JPN}どうせ パスワードを\n"
	.string "しらなきゃ はいれないわよ$"

