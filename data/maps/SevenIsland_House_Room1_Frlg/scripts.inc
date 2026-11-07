.equ TRAINER_VISITING, VAR_TEMP_1

SevenIsland_House_Room1_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SevenIsland_House_Room1_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SevenIsland_House_Room1_OnFrame
	.byte 0

SevenIsland_House_Room1_OnTransition::
	special ValidateEReaderTrainer
	call_if_eq VAR_RESULT, 0, SevenIsland_House_Room1_EventScript_SetTrainerVisitingLayout
	call_if_ne VAR_MAP_SCENE_SEVEN_ISLAND_HOUSE_ROOM1, 0, SevenIsland_House_Room1_EventScript_MoveOldWomanToDoor
	end

SevenIsland_House_Room1_EventScript_SetTrainerVisitingLayout::
	setvar TRAINER_VISITING, TRUE
	setobjectxyperm LOCALID_SEVEN_ISLAND_HOUSE_OLD_WOMAN, 4, 2
	setobjectmovementtype LOCALID_SEVEN_ISLAND_HOUSE_OLD_WOMAN, MOVEMENT_TYPE_FACE_DOWN
	setmaplayoutindex LAYOUT_SEVEN_ISLAND_HOUSE_ROOM1_DOOR_OPEN
	return

SevenIsland_House_Room1_EventScript_MoveOldWomanToDoor::
	setobjectxyperm LOCALID_SEVEN_ISLAND_HOUSE_OLD_WOMAN, 3, 2
	setobjectmovementtype LOCALID_SEVEN_ISLAND_HOUSE_OLD_WOMAN, MOVEMENT_TYPE_FACE_RIGHT
	return

SevenIsland_House_Room1_OnFrame::
	map_script_2 VAR_MAP_SCENE_SEVEN_ISLAND_HOUSE_ROOM1, 1, SevenIsland_House_Room1_EventScript_OldWomanCommentOnBattle
	map_script_2 VAR_MAP_SCENE_SEVEN_ISLAND_HOUSE_ROOM1, 2, SevenIsland_House_Room1_EventScript_OldWomanCommentOnBattle
	map_script_2 VAR_MAP_SCENE_SEVEN_ISLAND_HOUSE_ROOM1, 3, SevenIsland_House_Room1_EventScript_OldWomanCommentOnBattle
	.2byte 0

SevenIsland_House_Room1_EventScript_OldWomanCommentOnBattle::
	lockall
	textcolor NPC_TEXT_COLOR_FEMALE
	applymovement LOCALID_PLAYER, SevenIsland_House_Room1_Movement_PlayerReEnterRoom
	waitmovement 0
	applymovement LOCALID_SEVEN_ISLAND_HOUSE_OLD_WOMAN, SevenIsland_House_Room1_Movement_OldWomanWalkBehindPlayer
	waitmovement 0
	copyobjectxytoperm LOCALID_SEVEN_ISLAND_HOUSE_OLD_WOMAN
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	call_if_eq VAR_MAP_SCENE_SEVEN_ISLAND_HOUSE_ROOM1, 1, SevenIsland_House_Room1_EventScript_BattleWonComment
	call_if_eq VAR_MAP_SCENE_SEVEN_ISLAND_HOUSE_ROOM1, 2, SevenIsland_House_Room1_EventScript_BattleLostComment
	call_if_eq VAR_MAP_SCENE_SEVEN_ISLAND_HOUSE_ROOM1, 3, SevenIsland_House_Room1_EventScript_BattleTiedComment
	special LoadPlayerParty
	setvar VAR_MAP_SCENE_SEVEN_ISLAND_HOUSE_ROOM1, 0
	releaseall
	end

SevenIsland_House_Room1_EventScript_BattleWonComment::
	msgbox SevenIsland_House_Room1_Text_AstoundingBattle
	return

SevenIsland_House_Room1_EventScript_BattleLostComment::
	msgbox SevenIsland_House_Room1_Text_FeelYourDisappointment
	return

SevenIsland_House_Room1_EventScript_BattleTiedComment::
	msgbox SevenIsland_House_Room1_Text_DrawWasIt
	return

SevenIsland_House_Room1_Movement_PlayerReEnterRoom::
	walk_down
	step_end

SevenIsland_House_Room1_Movement_OldWomanWalkBehindPlayer::
	walk_right
	walk_in_place_faster_down
	step_end

SevenIsland_House_Room1_EventScript_OldWoman::
	lock
	faceplayer
	special ValidateEReaderTrainer
	call_if_eq VAR_RESULT, 1, SevenIsland_House_Room1_EventScript_InvalidVisitingTrainer
	goto_if_eq TRAINER_VISITING, TRUE, SevenIsland_House_Room1_EventScript_TrainerVisiting
	msgbox SevenIsland_House_Room1_Text_OnlyEnjoymentWatchingBattles
	release
	end

SevenIsland_House_Room1_EventScript_InvalidVisitingTrainer::
	msgbox SevenIsland_House_Room1_Text_OnlyEnjoymentWatchingBattles
	release
	end

SevenIsland_House_Room1_EventScript_TrainerVisiting::
	special SavePlayerParty
	special BufferEReaderTrainerName
	msgbox SevenIsland_House_Room1_Text_ChallengeVisitingTrainer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SevenIsland_House_Room1_EventScript_DeclineBattle
	call SevenIsland_House_Room1_EventScript_ChooseParty
	goto_if_eq VAR_RESULT, 0, SevenIsland_House_Room1_EventScript_DeclineBattle
	msgbox SevenIsland_House_Room1_Text_SaveProgressBeforeBattle, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SevenIsland_House_Room1_EventScript_DeclineBattle
	special LoadPlayerParty
	call Common_EventScript_SaveGame
	goto_if_eq VAR_RESULT, 0, SevenIsland_House_Room1_EventScript_DeclineBattle
	special SavePlayerParty
	special ReducePlayerPartyToSelectedMons
	msgbox SevenIsland_House_Room1_Text_DontHoldAnythingBack
	closemessage
	call_if_eq VAR_FACING, DIR_NORTH, SevenIsland_House_Room1_EventScript_EnterBattleRoomNorth
	call_if_eq VAR_FACING, DIR_EAST, SevenIsland_House_Room1_EventScript_EnterBattleRoomEast
	call_if_eq VAR_FACING, DIR_WEST, SevenIsland_House_Room1_EventScript_EnterBattleRoomWest
	warp MAP_SEVEN_ISLAND_HOUSE_ROOM2, 3, 1
	waitstate
	release
	end

SevenIsland_House_Room1_EventScript_DeclineBattle::
	special LoadPlayerParty
	msgbox SevenIsland_House_Room1_Text_OhYouWontBattle
	release
	end

SevenIsland_House_Room1_EventScript_ChooseParty::
	msgbox SevenIsland_House_Room1_Text_LimitThreeMonsPerSide
	fadescreen FADE_TO_BLACK
	special ChooseHalfPartyForBattle
	return

SevenIsland_House_Room1_EventScript_EnterBattleRoomNorth::
	applymovement VAR_LAST_TALKED, SevenIsland_House_Room1_Movement_OldWomanMoveAsideLeft
	applymovement LOCALID_PLAYER, SevenIsland_House_Room1_Movement_PlayerEnterBattleRoomNorth
	waitmovement 0
	return

SevenIsland_House_Room1_EventScript_EnterBattleRoomEast::
	applymovement VAR_LAST_TALKED, SevenIsland_House_Room1_Movement_OldWomanMoveAsideRight
	applymovement LOCALID_PLAYER, SevenIsland_House_Room1_Movement_PlayerEnterBattleRoomEast
	waitmovement 0
	return

SevenIsland_House_Room1_EventScript_EnterBattleRoomWest::
	applymovement VAR_LAST_TALKED, SevenIsland_House_Room1_Movement_OldWomanMoveAsideLeft
	applymovement LOCALID_PLAYER, SevenIsland_House_Room1_Movement_PlayerEnterBattleRoomWest
	waitmovement 0
	return

SevenIsland_House_Room1_Movement_PlayerEnterBattleRoomNorth::
	delay_16
	walk_up
	walk_up
	step_end

SevenIsland_House_Room1_Movement_PlayerEnterBattleRoomEast::
	delay_16
	walk_right
	walk_up
	step_end

SevenIsland_House_Room1_Movement_PlayerEnterBattleRoomWest::
	delay_16
	walk_left
	walk_up
	step_end

SevenIsland_House_Room1_Movement_OldWomanMoveAsideLeft::
	walk_left
	walk_in_place_faster_right
	step_end

SevenIsland_House_Room1_Movement_OldWomanMoveAsideRight::
	walk_right
	walk_in_place_faster_left
	step_end

SevenIsland_House_Room1_EventScript_Box::
	msgbox SevenIsland_House_Room1_Text_SlightBreezeAroundBox, MSGBOX_SIGN
	end

SevenIsland_House_Room1_Text_OnlyEnjoymentWatchingBattles::
	.string "{JPN}わしは こうみえても\n"
	.string "むかしは いろんな ばしょを\l"
	.string "とびまわって\l"
	.string "とびまわって!\p"
	.string "いろんな トレーナーに\n"
	.string "しょうぶを いどんだ ものじゃ\p"
	.string "いまでは しょうぶを みながら\n"
	.string "むかしを おもいだすの だけが\l"
	.string "たのしみ なのじゃ$"

SevenIsland_House_Room1_Text_SlightBreezeAroundBox::
	.string "{JPN}はこの すきまから すこしだけ\n"
	.string "かぜが はいりこんで きてる!?$"

SevenIsland_House_Room1_Text_ChallengeVisitingTrainer::
	.string "{JPN}……うむ!\n"
	.string "よく きたのう!\l"
	.string "じつはな いま\l"
	.string "ちかの かくしべやに\p"
	.string "{STR_VAR_1} という トレーナーが\n"
	.string "きておるのじゃ!\p"
	.string "もちろん {STR_VAR_1}と しょうぶ\n"
	.string "するために ここに きたのじゃな!$"

SevenIsland_House_Room1_Text_OhYouWontBattle::
	.string "{JPN}そうか……\n"
	.string "しょうぶ せんのか……\l"
	.string "そうか そうか\l"
	.string "……\l"
	.string "まあ きが むいたら\l"
	.string "また きておくれ$"

SevenIsland_House_Room1_Text_LimitThreeMonsPerSide::
	.string "{JPN}おお! ほんとに\n"
	.string "しょうぶを してくれるのか!\p"
	.string "がんばって はこを どけた\n"
	.string "かいが あったのじゃ!\p"
	.string "……\n"
	.string "じゃがな\l"
	.string "この かくしべやは\l"
	.string "あんまり がんじょう でないから\p"
	.string "せめて 3たい3の\n"
	.string "しょうぶに して くれんかのう?$"

SevenIsland_House_Room1_Text_SaveProgressBeforeBattle::
	.string "{JPN}じゃあ しょうぶの まえに\n"
	.string "レポートを かくかの!?$"

SevenIsland_House_Room1_Text_DontHoldAnythingBack::
	.string "{JPN}うむ ほんきで\n"
	.string "ぶつかって いくのじゃぞ!$"

@ Unused
SevenIsland_House_Room1_Text_StrVar1_1::
	.string "{STR_VAR_1}$"

SevenIsland_House_Room2_Text_MatchEndedUpDraw::
	.string "{JPN}しょうぶは ひきわけに おわった!$"

SevenIsland_House_Room1_Text_DrawWasIt::
	.string "{JPN}ひきわけ じゃったか!\p"
	.string "しかし!\p"
	.string "みどころの ある\n"
	.string "じつに ないようの ふかい\p"
	.string "すばらしい\n"
	.string "たたかい じゃったな!$"

@ Unused
SevenIsland_House_Room1_Text_StrVar1_2::
	.string "{STR_VAR_1}$"

SevenIsland_House_Room1_Text_AstoundingBattle::
	.string "{JPN}すばらしい\p"
	.string "すばらしい\n"
	.string "しょうぶ じゃった!!\l"
	.string "すごすぎて\l"
	.string "なみだが とまらんのじゃ!\p"
	.string "わざ と わざ!\p"
	.string "ちから と ちから の\n"
	.string "ぶつかりあい!\p"
	.string "うむ!\n"
	.string "みごと じゃったー!$"

@ Unused
SevenIsland_House_Room1_Text_StrVar1_3::
	.string "{STR_VAR_1}$"

SevenIsland_House_Room1_Text_FeelYourDisappointment::
	.string "{JPN}うむ\n"
	.string "ざんねん じゃったのう\p"
	.string "じゃが しかし!\n"
	.string "その くやしさこそが!\l"
	.string "おのれを つよくする\l"
	.string "げんどうりょく に なるのじゃ!\p"
	.string "この くやしさを わすれず\n"
	.string "しゅぎょうに はげむのじゃー!$"

