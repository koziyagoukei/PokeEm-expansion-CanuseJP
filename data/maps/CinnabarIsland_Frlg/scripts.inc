CinnabarIsland_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarIsland_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CinnabarIsland_OnFrame
	.byte 0

CinnabarIsland_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_CINNABAR_ISLAND
	call CinnabarIsland_EventScript_CheckUnlockGym
	call_if_eq VAR_MAP_SCENE_CINNABAR_ISLAND, 1, CinnabarIsland_EventScript_ReadyObjectsSailToOneIsland
	call_if_eq VAR_MAP_SCENE_CINNABAR_ISLAND, 3, CinnabarIsland_EventScript_ReadyObjectsReturnFromSeviiIslands
	call_if_eq VAR_MAP_SCENE_CINNABAR_ISLAND_2, 1, CinnabarIsland_EventScript_ReadyObjectsSailToOneIslandFromPokeCenter
	end

CinnabarIsland_EventScript_ReadyObjectsSailToOneIslandFromPokeCenter::
	savebgm MUS_FOLLOW_ME
	setflag FLAG_TEMP_2
	setobjectxyperm LOCALID_CINNABAR_BILL, 15, 12
	setobjectmovementtype LOCALID_CINNABAR_BILL, MOVEMENT_TYPE_FACE_LEFT
	setobjectxyperm LOCALID_CINNABAR_SEAGALLOP, 30, 12
	return

CinnabarIsland_EventScript_ReadyObjectsSailToOneIsland::
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_ge VAR_0x8004, 18, CinnabarIsland_EventScript_MoveSeagallopDown
	setflag FLAG_TEMP_2
	setobjectxyperm LOCALID_CINNABAR_BILL, 18, 12
	setobjectmovementtype LOCALID_CINNABAR_BILL, MOVEMENT_TYPE_FACE_UP
	setobjectxyperm LOCALID_CINNABAR_SEAGALLOP, 30, 12
	return

CinnabarIsland_EventScript_MoveSeagallopDown::
	setobjectxyperm LOCALID_CINNABAR_SEAGALLOP, 30, 8
	return

CinnabarIsland_EventScript_ReadyObjectsReturnFromSeviiIslands::
	setobjectxyperm LOCALID_CINNABAR_BILL, 21, 8
	setobjectmovementtype LOCALID_CINNABAR_BILL, MOVEMENT_TYPE_FACE_UP
	return

CinnabarIsland_EventScript_CheckUnlockGym::
	goto_if_set FLAG_HIDE_POKEMON_MANSION_B1F_SECRET_KEY, CinnabarIsland_EventScript_UnlockGym
	return

CinnabarIsland_EventScript_UnlockGym::
	setvar VAR_TEMP_1, 1
	return

CinnabarIsland_OnFrame::
	map_script_2 VAR_MAP_SCENE_CINNABAR_ISLAND_2, 1, CinnabarIsland_EventScript_ExitPokeCenterForOneIsland
	map_script_2 VAR_MAP_SCENE_CINNABAR_ISLAND, 1, CinnabarIsland_EventScript_BillScene
	map_script_2 VAR_MAP_SCENE_CINNABAR_ISLAND, 3, CinnabarIsland_EventScript_ReturnFromSeviiIslands
	.2byte 0

CinnabarIsland_EventScript_ExitPokeCenterForOneIsland::
	lockall
	clearflag FLAG_DONT_TRANSITION_MUSIC
	savebgm MUS_DUMMY
	textcolor NPC_TEXT_COLOR_MALE
	setvar VAR_MAP_SCENE_CINNABAR_ISLAND_2, 2
	delay 20
	call CinnabarIsland_EventScript_SailToOneIsland
	releaseall
	end

CinnabarIsland_EventScript_ReturnFromSeviiIslands::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	msgbox CinnabarIsland_Text_IfYouHaveTriPassYouCanGoAgain
	closemessage
	applymovement LOCALID_CINNABAR_BILL, CinnabarIsland_Movement_BillExit
	waitmovement 0
	removeobject LOCALID_CINNABAR_BILL
	setvar VAR_MAP_SCENE_CINNABAR_ISLAND, 4
	setflag FLAG_HIDE_TWO_ISLAND_GAME_CORNER_LOSTELLE
	clearflag FLAG_HIDE_LOSTELLE_IN_HER_HOME
	releaseall
	end

CinnabarIsland_Movement_BillExit::
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

CinnabarIsland_EventScript_BillScene::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	call_if_unset FLAG_TEMP_2, CinnabarIsland_EventScript_BillFacePlayer1
	call_if_set FLAG_TEMP_2, CinnabarIsland_EventScript_BillFacePlayer2
	playse SE_PIN
	applymovement LOCALID_CINNABAR_BILL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_CINNABAR_BILL, Common_Movement_Delay48
	waitmovement 0
	call_if_unset FLAG_TEMP_2, CinnabarIsland_EventScript_BillApproachPlayer1
	call_if_set FLAG_TEMP_2, CinnabarIsland_EventScript_BillApproachPlayer2
	msgbox CinnabarIsland_Text_HeyIfItIsntPlayer
	msgbox CinnabarIsland_Text_ComeWithMeToOneIsland, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, CinnabarIsland_EventScript_AgreeSailToOneIsland
	goto_if_eq VAR_RESULT, NO, CinnabarIsland_EventScript_DeclineSailToOneIsland
	end

CinnabarIsland_EventScript_BillFacePlayer1::
	applymovement LOCALID_CINNABAR_BILL, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	return

CinnabarIsland_EventScript_BillFacePlayer2::
	applymovement LOCALID_CINNABAR_BILL, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

CinnabarIsland_EventScript_BillApproachPlayer1::
	applymovement LOCALID_CINNABAR_BILL, CinnabarIsland_Movement_BillApproachPlayer1
	waitmovement 0
	return

CinnabarIsland_EventScript_BillApproachPlayer2::
	applymovement LOCALID_CINNABAR_BILL, CinnabarIsland_Movement_BillApproachPlayer2
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

CinnabarIsland_EventScript_AgreeSailToOneIsland::
	msgbox CinnabarIsland_Text_AllRightLetsGo
	closemessage
	call CinnabarIsland_EventScript_SailToOneIsland
	releaseall
	end

CinnabarIsland_EventScript_DeclineSailToOneIsland::
	msgbox CinnabarIsland_Text_IllBeWaitingInPokeCenter
	closemessage
	call_if_unset FLAG_TEMP_2, CinnabarIsland_EventScript_BillExitToPokeCenter
	call_if_set FLAG_TEMP_2, CinnabarIsland_EventScript_BillReturnToPokeCenter
	removeobject LOCALID_CINNABAR_BILL
	setvar VAR_MAP_SCENE_CINNABAR_ISLAND, 2
	clearflag FLAG_HIDE_CINNABAR_POKECENTER_BILL
	releaseall
	end

CinnabarIsland_EventScript_BillExitToPokeCenter::
	applymovement LOCALID_CINNABAR_BILL, CinnabarIsland_Movement_BillExitToPokeCenter
	waitmovement 0
	return

CinnabarIsland_Movement_BillExitToPokeCenter::
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

CinnabarIsland_EventScript_BillReturnToPokeCenter::
	applymovement LOCALID_PLAYER, CinnabarIsland_Movement_PlayerWatchBillExit
	applymovement LOCALID_CINNABAR_BILL, CinnabarIsland_Movement_BillApproachDoor
	waitmovement 0
	opendoor 14, 11
	waitdooranim
	applymovement LOCALID_CINNABAR_BILL, CinnabarIsland_Movement_BillReEnterPokeCenter
	waitmovement 0
	closedoor 14, 11
	waitdooranim
	return

CinnabarIsland_Movement_PlayerWatchBillExit::
	walk_left
	walk_in_place_faster_right
	step_end

CinnabarIsland_Movement_BillApproachDoor::
	delay_16
	walk_left
	walk_in_place_faster_up
	step_end

CinnabarIsland_Movement_BillReEnterPokeCenter::
	walk_up
	delay_4
	set_invisible
	step_end

@ Can be triggered outside, or after talking to Bill in the poke center
CinnabarIsland_EventScript_SailToOneIsland::
	call_if_set FLAG_TEMP_2, CinnabarIsland_EventScript_ApproachShore
	addobject LOCALID_CINNABAR_SEAGALLOP
	call_if_unset FLAG_TEMP_2, CinnabarIsland_EventScript_BoatArrive
	call_if_set FLAG_TEMP_2, CinnabarIsland_EventScript_BoatArriveExitedPokeCenter
	msgbox CinnabarIsland_Text_MyPalsBoatArrived
	closemessage
	call_if_unset FLAG_TEMP_2, CinnabarIsland_EventScript_BoardBoat
	call_if_set FLAG_TEMP_2, CinnabarIsland_EventScript_BoardBoatExitedPokeCenter
	setvar VAR_MAP_SCENE_CINNABAR_ISLAND, 2
	removeobject LOCALID_CINNABAR_BILL
	delay 40
	setflag FLAG_HIDE_CINNABAR_SEAGALLOP
	setvar VAR_MAP_SCENE_ONE_ISLAND_HARBOR, 1
	setvar VAR_0x8004, SEAGALLOP_CINNABAR_ISLAND
	setvar VAR_0x8006, SEAGALLOP_ONE_ISLAND
	goto EventScript_SetSail
	end

CinnabarIsland_EventScript_ApproachShore::
	applymovement LOCALID_CINNABAR_BILL, CinnabarIsland_Movement_ApproachShore
	applymovement LOCALID_PLAYER, CinnabarIsland_Movement_ApproachShore
	waitmovement 0
	return

CinnabarIsland_EventScript_BoatArrive::
	applymovement LOCALID_CINNABAR_BILL, CinnabarIsland_Movement_BillFaceBoat
	applymovement LOCALID_CINNABAR_SEAGALLOP, CinnabarIsland_Movement_BoatArrive
	waitmovement 0
	return

CinnabarIsland_EventScript_BoatArriveExitedPokeCenter::
	applymovement LOCALID_CINNABAR_SEAGALLOP, CinnabarIsland_Movement_BoatArrive
	waitmovement 0
	return

CinnabarIsland_EventScript_BoardBoat::
	applymovement LOCALID_CINNABAR_BILL, CinnabarIsland_Movement_BillBoardBoat
	applymovement LOCALID_PLAYER, CinnabarIsland_Movement_PlayerBoardBoat
	waitmovement 0
	return

CinnabarIsland_EventScript_BoardBoatExitedPokeCenter::
	applymovement LOCALID_CINNABAR_BILL, CinnabarIsland_Movement_BillBoardBoatFromShore
	applymovement LOCALID_PLAYER, CinnabarIsland_Movement_PlayerBoardBoatFromShore
	waitmovement 0
	return

CinnabarIsland_Movement_BoatArrive::
	delay_16
	delay_16
	walk_fast_left
	walk_fast_left
	walk_left
	walk_slow_left
	walk_slow_left
	step_end

CinnabarIsland_Movement_BillBoardBoat::
	walk_right
	walk_down
	walk_down
	walk_right
	delay_4
	set_invisible
	step_end

CinnabarIsland_Movement_BillApproachPlayer1::
	walk_up
	step_end

CinnabarIsland_Movement_BillApproachPlayer2::
	walk_left
	walk_left
	walk_left
	step_end

CinnabarIsland_Movement_BillFaceBoat::
	delay_16
	delay_16
	delay_8
	walk_in_place_faster_right
	step_end

CinnabarIsland_Movement_ApproachShore::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

CinnabarIsland_Movement_BillBoardBoatFromShore::
	walk_right
	delay_4
	set_invisible
	step_end

CinnabarIsland_Movement_PlayerBoardBoat::
	walk_down
	walk_right
	walk_down
	walk_down
	walk_in_place_faster_right
	delay_4
	walk_right
	delay_4
	set_invisible
	step_end

CinnabarIsland_Movement_PlayerBoardBoatFromShore::
	walk_right
	delay_4
	walk_right
	delay_4
	set_invisible
	step_end

@ Triggered when VAR_TEMP_1 is 0
CinnabarIsland_EventScript_GymDoorLocked::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 20
	msgbox CinnabarIsland_Text_DoorIsLocked
	closemessage
	applymovement LOCALID_PLAYER, CinnabarIsland_Movement_ForcePlayerFromDoor
	waitmovement 0
	releaseall
	end

CinnabarIsland_Movement_ForcePlayerFromDoor::
	walk_down
	step_end

CinnabarIsland_EventScript_Woman::
	lock
	faceplayer
	famechecker FAMECHECKER_BLAINE, 3
	msgbox CinnabarIsland_Text_BlaineLivedHereSinceBeforeLab
	release
	end

CinnabarIsland_EventScript_OldMan::
	msgbox CinnabarIsland_Text_ScientistsExperimentInMansion, MSGBOX_NPC
	end

CinnabarIsland_EventScript_IslandSign::
	msgbox CinnabarIsland_Text_IslandSign, MSGBOX_SIGN
	end

CinnabarIsland_EventScript_PokemonLabSign::
	msgbox CinnabarIsland_Text_PokemonLab, MSGBOX_SIGN
	end

CinnabarIsland_EventScript_GymSign::
	lockall
	famechecker FAMECHECKER_BLAINE, 0
	msgbox CinnabarIsland_Text_GymSign
	releaseall
	end

CinnabarIsland_Text_DoorIsLocked::
	.string "{JPN}ドアには\n"
	.string "カギが かかって いた…!$"

CinnabarIsland_Text_BlaineLivedHereSinceBeforeLab::
	.string "{JPN}グレン ジムの カツラは\n"
	.string "けんきゅうじょが できる まえから\l"
	.string "すんでて へんじん なんだって$"

CinnabarIsland_Text_ScientistsExperimentInMansion::
	.string "{JPN}ポケモン やしきは\n"
	.string "けんきゅうの ため かがくしゃが\l"
	.string "たまに でいり しとる ようだ$"

CinnabarIsland_Text_IslandSign::
	.string "{JPN}ここは グレン タウン\n"
	.string "グレンの あかは じょうねつの いろ$"

CinnabarIsland_Text_PokemonLab::
	.string "{JPN}ここは ポケモン けんきゅうじょ$"

CinnabarIsland_Text_GymSign::
	.string "{JPN}グレン じま ポケモン ジム\n"
	.string "リーダー カツラ\l"
	.string "ねっけつ クイズ おやじ$"

CinnabarIsland_Text_HeyIfItIsntPlayer::
	.string "{JPN}あれ!?\n"
	.string "{PLAYER}やないか!$"

CinnabarIsland_Text_ComeWithMeToOneIsland::
	.string "{JPN}ほれ わいや マサキや!\n"
	.string "なんや ひさしぶり!\l"
	.string "パソコン つこて くれとる?\p"
	.string "…せや ここで あったのも\n"
	.string "なんかの ごえんや!\l"
	.string "ちょっと いっしょに いかへん?\p"
	.string "とおく みなみに\n"
	.string "1のしま という\l"
	.string "ちいさな しまが あってな\p"
	.string "ともだちに よばれて\n"
	.string "いくとこ なんやけど…\p"
	.string "どや いくか?$"

CinnabarIsland_Text_AllRightLetsGo::
	.string "{JPN}ほな レッツゴーや!$"

CinnabarIsland_Text_IllBeWaitingInPokeCenter::
	.string "{JPN}なんや いそがしいんか?\p"
	.string "まー ええわ\n"
	.string "おむかえの ふねも まだやし\p"
	.string "そこの ポケモンセンターで\n"
	.string "まっとくから\l"
	.string "てが あいたら きたって!$"

CinnabarIsland_Text_MyPalsBoatArrived::
	.string "{JPN}ちょうど ともだちの ふねが\n"
	.string "ついた みたいやな!\p"
	.string "とくべつに グレンじま まで\n"
	.string "むかえに きて もろたんや$"

CinnabarIsland_Text_IfYouHaveTriPassYouCanGoAgain::
	.string "{JPN}いやー\n"
	.string "ながたび おつかれさん!\p"
	.string "ニシキも おまえさんの こと\n"
	.string "きにいってた みたいやし\l"
	.string "また いってあげて!\p"
	.string "トライパスが あれば\n"
	.string "クチバシティの みなとから\l"
	.string "いつでも ふねで いけるで!\p"
	.string "ほな きょうは ありがと!$"

