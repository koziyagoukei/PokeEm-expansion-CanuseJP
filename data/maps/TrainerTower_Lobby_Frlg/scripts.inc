TrainerTower_Lobby_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, TrainerTower_Lobby_OnResume
	map_script MAP_SCRIPT_ON_RETURN_TO_FIELD, TrainerTower_Lobby_OnReturnToField
	map_script MAP_SCRIPT_ON_TRANSITION, TrainerTower_Lobby_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, TrainerTower_Lobby_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, TrainerTower_Lobby_OnFrame
	.byte 0

TrainerTower_Lobby_OnResume::
	setvar VAR_TEMP_2, 0
	ttower_resumetimer
	ttower_shouldexit
	goto_if_eq VAR_RESULT, FALSE, TrainerTower_Lobby_OnResumeEnd
	@ Never reached, above always FALSE
	setvar VAR_TEMP_0, 0
	setobjectxy LOCALID_PLAYER, 9, 7
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_FaceReceptionist
TrainerTower_Lobby_OnResumeEnd::
	end

TrainerTower_Lobby_OnReturnToField::
	addobject LOCALID_TOWER_NURSE
	addobject LOCALID_TOWER_MART_CLERK
	addobject LOCALID_TOWER_RECEPTIONIST
	addobject LOCALID_TOWER_COOLTRAINER_F
	addobject LOCALID_TOWER_BALDING_MAN
	end

TrainerTower_Lobby_OnLoad::
	call_if_eq VAR_TEMP_D, 17, TrainerTower_Lobby_OpenCounterBarrier
	end

TrainerTower_Lobby_OpenCounterBarrier::
	setmetatile 17, 10, METATILE_TrainerTower_Floor_ShadeBottomLeft, 0
	return

TrainerTower_Lobby_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_TRAINER_TOWER_LOBBY
	getplayerxy VAR_TEMP_D, VAR_RESULT
	end

TrainerTower_Lobby_OnFrame::
	map_script_2 VAR_TEMP_0, 0, TrainerTower_Lobby_EventScript_Enter
	map_script_2 VAR_TEMP_D, 17, TrainerTower_Lobby_EventScript_ExitElevator
	.2byte 0

TrainerTower_Lobby_EventScript_ExitElevator::
	lockall
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_ExitElevator
	waitmovement 0
	setmetatile 17, 10, METATILE_TrainerTower_CounterBarrier, 1
	special DrawWholeMapView
	playse SE_CLICK
	waitse
	setvar VAR_TEMP_D, 0
	releaseall
	end

TrainerTower_Lobby_Movement_ExitElevator::
	walk_down
	walk_down
	step_end

TrainerTower_Lobby_EventScript_Enter::
	setvar VAR_TEMP_0, 1
	ttower_getchallengestatus
	switch VAR_RESULT
	case TT_CHALLENGE_STATUS_LOST, TrainerTower_Lobby_EventScript_LostChallenge
	case TT_CHALLENGE_STATUS_UNK, TrainerTower_Lobby_EventScript_ExitChallengeSpeakToReceptionist
	case TT_CHALLENGE_STATUS_NORMAL, TrainerTower_Lobby_EventScript_EnterEnd

TrainerTower_Lobby_EventScript_LostChallenge::
	lock
	faceplayer
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_FaceReceptionist
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox TrainerTower_Lobby_Text_TooBadComeBackTryAgain
	goto TrainerTower_Lobby_EventScript_ExitChallenge

@ Presumably E-Reader related
TrainerTower_Lobby_EventScript_ExitChallengeSpeakToReceptionist::
	lock
	faceplayer
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_FaceReceptionist
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox TrainerTower_Lobby_Text_MoveCounterHereWhenTrainersSwitch
TrainerTower_Lobby_EventScript_ExitChallenge::
	closemessage
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_WalkDown
	waitmovement 0
	setvar VAR_MAP_SCENE_TRAINER_TOWER, 0
	release
TrainerTower_Lobby_EventScript_EnterEnd::
	end

TrainerTower_Lobby_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

TrainerTower_Lobby_EventScript_Receptionist::
	lock
	faceplayer
	ttower_getbeatchallenge
	goto_if_eq VAR_RESULT, TRUE, TrainerTower_Lobby_EventScript_ThanksForCompeting
	msgbox TrainerTower_Lobby_Text_GiveItYourBest
	goto TrainerTower_Lobby_EventScript_ReceptionistEnd

TrainerTower_Lobby_EventScript_ThanksForCompeting::
	msgbox TrainerTower_Lobby_Text_ThanksForCompeting
TrainerTower_Lobby_EventScript_ReceptionistEnd::
	release
	end

TrainerTower_Lobby_EventScript_MartClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart TrainerTower_Lobby_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
TrainerTower_Lobby_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_MAX_POTION
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_NONE
	release
	end

TrainerTower_Lobby_EventScript_EntryTrigger::
	lockall
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_FaceReceptionist
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox TrainerTower_Lobby_Text_WelcomeToTrainerTower
	ttower_getnumfloors
	goto_if_eq VAR_RESULT, FALSE, TrainerTower_Lobby_EventScript_AllFloorsUsed
	msgbox TrainerTower_Lobby_Text_TrainersUpToFloorNum
	goto TrainerTower_Lobby_EventScript_AskEnterChallenge

TrainerTower_Lobby_EventScript_AllFloorsUsed::
	msgbox TrainerTower_Lobby_Text_TrainersUpEighthFloor
TrainerTower_Lobby_EventScript_AskEnterChallenge::
	message TrainerTower_Lobby_Text_LikeToChallengeTrainers
	waitmessage
	multichoice 18, 6, MULTI_YESNOINFO, FALSE
	switch VAR_RESULT
	case 0, TrainerTower_Lobby_EventScript_ChooseChallenge
	case 1, TrainerTower_Lobby_EventScript_DeclineChallenge
	case 2, TrainerTower_Lobby_EventScript_ChallengeInfo
	case MULTI_B_PRESSED, TrainerTower_Lobby_EventScript_DeclineChallenge
	end

TrainerTower_Lobby_EventScript_ChallengeInfo::
	msgbox TrainerTower_Lobby_Text_ExplainTrainerTower
	goto TrainerTower_Lobby_EventScript_AskEnterChallenge
	end

TrainerTower_Lobby_EventScript_ChooseChallenge::
	multichoice 13, 3, MULTI_TRAINER_TOWER_MODE, FALSE
	switch VAR_RESULT
	case 0, TrainerTower_Lobby_EventScript_BeginChallenge
	case 1, TrainerTower_Lobby_EventScript_BeginChallenge
	case 2, TrainerTower_Lobby_EventScript_BeginChallenge
	case 3, TrainerTower_Lobby_EventScript_BeginChallenge
	case 4, TrainerTower_Lobby_EventScript_DeclineChallenge
	case MULTI_B_PRESSED, TrainerTower_Lobby_EventScript_DeclineChallenge
	end

TrainerTower_Lobby_EventScript_BeginChallenge::
	copyvar VAR_0x8005, VAR_RESULT
	setvar VAR_MAP_SCENE_TRAINER_TOWER, 1
	special HealPlayerParty
	msgbox TrainerTower_Lobby_Text_StartClockGetSetGo
	ttower_startchallenge
	releaseall
	end

TrainerTower_Lobby_EventScript_DeclineChallenge::
	msgbox TrainerTower_Lobby_Text_PleaseVisitUsAgain
	closemessage
	applymovement LOCALID_PLAYER, TrainerTower_Lobby_Movement_WalkDown
	waitmovement 0
	releaseall
	end

TrainerTower_Lobby_Movement_FaceReceptionist::
	face_right
	step_end

TrainerTower_Lobby_Movement_WalkDown::
	walk_down
	step_end

TrainerTower_Lobby_EventScript_ShowRecords::
	lockall
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, 1
	special ShowTrainerHillRecords
	releaseall
	end

TrainerTower_Lobby_EventScript_CooltrainerF::
	msgbox TrainerTower_Lobby_Text_WonderWhatKindsOfTrainers, MSGBOX_NPC
	end

TrainerTower_Lobby_EventScript_BaldingMan::
	msgbox TrainerTower_Lobby_Text_StairsTougherThanAnyBattle, MSGBOX_NPC
	end

@ All Trainer Tower text (Lobby and Roof) is interleaved here

TrainerTower_Lobby_Text_WelcomeToTrainerTower::
	.string "{JPN}こんにちは!\p"
	.string "トレーナーさんたちが あつまる\n"
	.string "トレーナータワーです!$"

TrainerTower_Lobby_Text_TrainersUpToFloorNum::
	.string "{JPN}こちらには いろんな ところから\n"
	.string "トレーナーさんたちが あつまります\p"
	.string "いまは……\p"
	.string "{STR_VAR_1}かい までしか\n"
	.string "いない みたいです$"

TrainerTower_Lobby_Text_TrainersUpEighthFloor::
	.string "{JPN}こちらには いろんな ところから\n"
	.string "トレーナーさんたちが やってきます\p"
	.string "いまは……\p"
	.string "ぜんぶ へやが うまってる\n"
	.string "みたいです!$"

TrainerTower_Lobby_Text_LikeToChallengeTrainers::
	.string "{JPN}トレーナーさんたちと\n"
	.string "しょうぶ しますか?$"

TrainerTower_Lobby_Text_StartClockGetSetGo::
	.string "{JPN}それでは じかんを はかりますので\n"
	.string "がんばってくださいね!\n"
	.string "いちについて…………………\p"
	.string "ようい……\p"
	.string "ドン!!$"

TrainerTower_Lobby_Text_PleaseVisitUsAgain::
	.string "{JPN}また おこしくださいね!$"

TrainerTower_Lobby_Text_TooBadComeBackTryAgain::
	.string "{JPN}ざんねん でしたね!\p"
	.string "とっても いい しょうぶ\n"
	.string "だったと おもいます\p"
	.string "また きてくださいね!$"

TrainerTower_Lobby_Text_GiveItYourBest::
	.string "{JPN}がんばってくださいね!$"

TrainerTower_Lobby_Text_MoveCounterHereWhenTrainersSwitch::
	.string "{JPN}トレーナーさんたちが いれかわって\n"
	.string "あぶないので\p"
	.string "こちらに いどう させて\n"
	.string "いただきました!\p"
	.string "ごめんなさい!$"

TrainerTower_Roof_Text_ImOwnerBattledPerfectly::
	.string "{JPN}こんにちは……\p"
	.string "わたしは ここの オーナー\p"
	.string "おくじょうから みた\n"
	.string "そら……\p"
	.string "おくじょうで ふく\n"
	.string "かぜ……\p"
	.string "さいこうだな……\p"
	.string "キミの たたかいも\n"
	.string "さいこうだった……$"

TrainerTower_Roof_Text_ThisIsForYou::
	.string "{JPN}これを キミに……$"

TrainerTower_Roof_Text_DoneItInRecordTime::
	.string "{JPN}おお!\n"
	.string "すばらしい!\p"
	.string "キミは ずいぶんと はやい じかんで\n"
	.string "ここまで きたようだ\p"
	.string "いままでで いちばん はやい……\p"
	.string "うけつけの タイムボードに\n"
	.string "とうろく しておこう$"

TrainerTower_Roof_Text_TookSweetTimeGettingHere::
	.string "{JPN}すこし ここに くるのが\n"
	.string "おそいようだが……$"

TrainerTower_Roof_Text_IdLikeToSeeBetterTime::
	.string "{JPN}もっと\n"
	.string "すばやい うごきを\l"
	.string "わたしに みせておくれ……\p"
	.string "よろしくな\p"
	.string "では また……$"

TrainerTower_Text_XMinYZSec::
	.string "{JPN}{STR_VAR_1}ふん {STR_VAR_2}びょう {STR_VAR_3}$"

TrainerTower_Lobby_Text_HereAreTheResults::
	.string "せいせきひょうです$"

TrainerTower_Lobby_Text_NeedTwoMonsForDouble::
	.string "{JPN}2たい2の しょうぶです\p"
	.string "たたかえる ポケモンが\n"
	.string "2たい いじょう いないと\l"
	.string "しょうぶ できません!$"

TrainerTower_Lobby_Text_ExplainTrainerTower::
	.string "{JPN}トレーナータワーでは\n"
	.string "タイムアタックという きょうぎが\l"
	.string "おこなわれます\p"
	.string "はやい じかんは タイムボードに\n"
	.string "のりますので ともだちと じかんを\l"
	.string "くらべて みてください\p"
	.string "うけつけから おくじょうにいる\n"
	.string "オーナーに はなしかけるまでの\l"
	.string "じかんが はかられます\p"
	.string "また トレーナーとの しょうぶで\n"
	.string "かっても けいけんちや おかねの\l"
	.string "やりとりは おこなわれません$"

TrainerTower_Lobby_Text_ThanksForCompeting::
	.string "{JPN}おつかれさまでした!$"

TrainerTower_Lobby_Text_WonderWhatKindsOfTrainers::
	.string "{JPN}さあ うでだめしよ!\p"
	.string "どんな トレーナーが\n"
	.string "わたしを まちうけて いるのかしら\l"
	.string "わくわく するわ$"

TrainerTower_Lobby_Text_StairsTougherThanAnyBattle::
	.string "{JPN}はあ はあ はあ……\n"
	.string "…… はあ はあ\p"
	.string "し しょうぶ より\n"
	.string "かいだんを かけあがる ほうが\l"
	.string "はあ はあ…… キツイ……$"

