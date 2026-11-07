CinnabarIsland_Gym_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, CinnabarIsland_Gym_OnLoad
	.byte 0

CinnabarIsland_Gym_OnLoad::
	goto_if_set FLAG_DEFEATED_BLAINE, CinnabarIsland_Gym_OnLoadOpenAllDoors
	call_if_set FLAG_CINNABAR_GYM_QUIZ_1, CinnabarIsland_Gym_OnLoadOpenDoor1
	call_if_set FLAG_CINNABAR_GYM_QUIZ_2, CinnabarIsland_Gym_OnLoadOpenDoor2
	call_if_set FLAG_CINNABAR_GYM_QUIZ_3, CinnabarIsland_Gym_OnLoadOpenDoor3
	call_if_set FLAG_CINNABAR_GYM_QUIZ_4, CinnabarIsland_Gym_OnLoadOpenDoor4
	call_if_set FLAG_CINNABAR_GYM_QUIZ_5, CinnabarIsland_Gym_OnLoadOpenDoor5
	call_if_set FLAG_CINNABAR_GYM_QUIZ_6, CinnabarIsland_Gym_OnLoadOpenDoor6
	end

CinnabarIsland_Gym_OnLoadOpenAllDoors::
	call CinnabarIsland_Gym_EventScript_OpenDoor1
	call CinnabarIsland_Gym_EventScript_OpenDoor2
	call CinnabarIsland_Gym_EventScript_OpenDoor3
	call CinnabarIsland_Gym_EventScript_OpenDoor4
	call CinnabarIsland_Gym_EventScript_OpenDoor5
	call CinnabarIsland_Gym_EventScript_OpenDoor6
	end

CinnabarIsland_Gym_OnLoadOpenDoor1::
	call CinnabarIsland_Gym_EventScript_OpenDoor1
	return

CinnabarIsland_Gym_OnLoadOpenDoor2::
	call CinnabarIsland_Gym_EventScript_OpenDoor2
	return

CinnabarIsland_Gym_OnLoadOpenDoor3::
	call CinnabarIsland_Gym_EventScript_OpenDoor3
	return

CinnabarIsland_Gym_OnLoadOpenDoor4::
	call CinnabarIsland_Gym_EventScript_OpenDoor4
	return

CinnabarIsland_Gym_OnLoadOpenDoor5::
	call CinnabarIsland_Gym_EventScript_OpenDoor5
	return

CinnabarIsland_Gym_OnLoadOpenDoor6::
	call CinnabarIsland_Gym_EventScript_OpenDoor6
	return

CinnabarIsland_Gym_EventScript_Blaine::
	famechecker FAMECHECKER_BLAINE, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_BLAINE, CinnabarIsland_Gym_Text_BlaineIntro, CinnabarIsland_Gym_Text_BlaineDefeat, CinnabarIsland_Gym_EventScript_DefeatedBlaine, NO_MUSIC
	goto_if_unset FLAG_GOT_TM38_FROM_BLAINE, CinnabarIsland_Gym_EventScript_GiveTM38
	msgbox CinnabarIsland_Gym_Text_BlainePostBattle
	release
	end

CinnabarIsland_Gym_EventScript_DefeatedBlaine::
	famechecker FAMECHECKER_BLAINE, 1
	setflag FLAG_DEFEATED_BLAINE
	setflag FLAG_BADGE07_GET
	setrespawn HEAL_LOCATION_CINNABAR_ISLAND
	setvar VAR_MAP_SCENE_CINNABAR_ISLAND, 1
	clearflag FLAG_HIDE_CINNABAR_BILL
	set_gym_trainers_frlg 7
	goto CinnabarIsland_Gym_EventScript_GiveTM38
	end

CinnabarIsland_Gym_EventScript_GiveTM38::
	msgbox CinnabarIsland_Gym_Text_ExplainVolcanoBadge
	checkitemspace ITEM_TM38
	goto_if_eq VAR_RESULT, FALSE, CinnabarIsland_Gym_EventScript_NoRoomForTM38
	giveitem_msg CinnabarIsland_Gym_Text_ReceivedTM38FromBlaine, ITEM_TM38
	setflag FLAG_GOT_TM38_FROM_BLAINE
	msgbox CinnabarIsland_Gym_Text_FireBlastIsUltimateFireMove
	release
	end

CinnabarIsland_Gym_EventScript_NoRoomForTM38::
	msgbox CinnabarIsland_Gym_Text_MakeSpaceForThis
	release
	end

CinnabarIsland_Gym_EventScript_Erik::
	trainerbattle_single TRAINER_SUPER_NERD_ERIK, CinnabarIsland_Gym_Text_ErikIntro, CinnabarIsland_Gym_Text_ErikDefeat
	msgbox CinnabarIsland_Gym_Text_ErikPostBattle, MSGBOX_AUTOCLOSE
	end

CinnabarIsland_Gym_EventScript_Avery::
	trainerbattle_single TRAINER_SUPER_NERD_AVERY, CinnabarIsland_Gym_Text_AveryIntro, CinnabarIsland_Gym_Text_AveryDefeat, CinnabarIsland_Gym_EventScript_DefeatedAvery
	msgbox CinnabarIsland_Gym_Text_AveryPostBattle, MSGBOX_AUTOCLOSE
	end

CinnabarIsland_Gym_EventScript_DefeatedAvery::
	call_if_unset FLAG_CINNABAR_GYM_QUIZ_2, CinnabarIsland_Gym_EventScript_Quiz2CompleteTrainer
	release
	end

CinnabarIsland_Gym_EventScript_Quiz2CompleteTrainer::
	call CinnabarIsland_Gym_EventScript_Quiz2Complete
	return

CinnabarIsland_Gym_EventScript_Derek::
	trainerbattle_single TRAINER_SUPER_NERD_DEREK, CinnabarIsland_Gym_Text_DerekIntro, CinnabarIsland_Gym_Text_DerekDefeat, CinnabarIsland_Gym_EventScript_DefeatedDerek
	famechecker FAMECHECKER_BLAINE, 2
	msgbox CinnabarIsland_Gym_Text_DerekPostBattle, MSGBOX_AUTOCLOSE
	end

CinnabarIsland_Gym_EventScript_DefeatedDerek::
	call_if_unset FLAG_CINNABAR_GYM_QUIZ_4, CinnabarIsland_Gym_EventScript_Quiz4CompleteTrainer
	release
	end

CinnabarIsland_Gym_EventScript_Quiz4CompleteTrainer::
	call CinnabarIsland_Gym_EventScript_Quiz4Complete
	return

CinnabarIsland_Gym_EventScript_Zac::
	trainerbattle_single TRAINER_SUPER_NERD_ZAC, CinnabarIsland_Gym_Text_ZacIntro, CinnabarIsland_Gym_Text_ZacDefeat, CinnabarIsland_Gym_EventScript_DefeatedZac
	msgbox CinnabarIsland_Gym_Text_ZacPostBattle, MSGBOX_AUTOCLOSE
	end

CinnabarIsland_Gym_EventScript_DefeatedZac::
	call_if_unset FLAG_CINNABAR_GYM_QUIZ_6, CinnabarIsland_Gym_EventScript_Quiz6CompleteTrainer
	release
	end

CinnabarIsland_Gym_EventScript_Quiz6CompleteTrainer::
	call CinnabarIsland_Gym_EventScript_Quiz6Complete
	return

CinnabarIsland_Gym_EventScript_Quinn::
	trainerbattle_single TRAINER_BURGLAR_QUINN, CinnabarIsland_Gym_Text_QuinnIntro, CinnabarIsland_Gym_Text_QuinnDefeat, CinnabarIsland_Gym_EventScript_DefeatedQuinn
	msgbox CinnabarIsland_Gym_Text_QuinnPostBattle, MSGBOX_AUTOCLOSE
	end

CinnabarIsland_Gym_EventScript_DefeatedQuinn::
	call_if_unset FLAG_CINNABAR_GYM_QUIZ_1, CinnabarIsland_Gym_EventScript_Quiz1CompleteTrainer
	release
	end

CinnabarIsland_Gym_EventScript_Quiz1CompleteTrainer::
	call CinnabarIsland_Gym_EventScript_Quiz1Complete
	return

CinnabarIsland_Gym_EventScript_Ramon::
	trainerbattle_single TRAINER_BURGLAR_RAMON, CinnabarIsland_Gym_Text_RamonIntro, CinnabarIsland_Gym_Text_RamonDefeat, CinnabarIsland_Gym_EventScript_DefeatedRamon
	msgbox CinnabarIsland_Gym_Text_RamonPostBattle, MSGBOX_AUTOCLOSE
	end

CinnabarIsland_Gym_EventScript_DefeatedRamon::
	call_if_unset FLAG_CINNABAR_GYM_QUIZ_3, CinnabarIsland_Gym_EventScript_Quiz3CompleteTrainer
	release
	end

CinnabarIsland_Gym_EventScript_Quiz3CompleteTrainer::
	call CinnabarIsland_Gym_EventScript_Quiz3Complete
	return

CinnabarIsland_Gym_EventScript_Dusty::
	trainerbattle_single TRAINER_BURGLAR_DUSTY, CinnabarIsland_Gym_Text_DustyIntro, CinnabarIsland_Gym_Text_DustyDefeat, CinnabarIsland_Gym_EventScript_DefeatedDusty
	msgbox CinnabarIsland_Gym_Text_DustyPostBattle, MSGBOX_AUTOCLOSE
	end

CinnabarIsland_Gym_EventScript_DefeatedDusty::
	call_if_unset FLAG_CINNABAR_GYM_QUIZ_5, CinnabarIsland_Gym_EventScript_Quiz5CompleteTrainer
	release
	end

CinnabarIsland_Gym_EventScript_Quiz5CompleteTrainer::
	call CinnabarIsland_Gym_EventScript_Quiz5Complete
	return

CinnabarIsland_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_BLAINE, CinnabarIsland_Gym_EventScript_GymGuyPostVictory
	msgbox CinnabarIsland_Gym_Text_GymGuyAdvice
	release
	end

CinnabarIsland_Gym_EventScript_GymGuyPostVictory::
	msgbox CinnabarIsland_Gym_Text_GymGuyPostVictory
	release
	end

CinnabarIsland_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE07_GET, CinnabarIsland_Gym_EventScript_GymStatuePostVictory
	msgbox CinnabarIsland_Gym_Text_GymStatue
	releaseall
	end

CinnabarIsland_Gym_EventScript_GymStatuePostVictory::
	msgbox CinnabarIsland_Gym_Text_GymStatuePlayerWon
	releaseall
	end

CinnabarIsland_Gym_EventScript_Quz1Left::
	lockall
	setvar VAR_TEMP_1, 0
	goto CinnabarIsland_Gym_EventScript_Quiz1
	end

CinnabarIsland_Gym_EventScript_Quz1Right::
	lockall
	setvar VAR_TEMP_1, 1
	goto CinnabarIsland_Gym_EventScript_Quiz1
	end

CinnabarIsland_Gym_EventScript_Quiz1::
	msgbox CinnabarIsland_Gym_Text_PokemonQuizRules
	msgbox CinnabarIsland_Gym_Text_QuizQuestion1, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, CinnabarIsland_Gym_EventScript_Quiz1Correct
	goto_if_eq VAR_RESULT, NO, CinnabarIsland_Gym_EventScript_Quiz1Incorrect
	end

CinnabarIsland_Gym_EventScript_CorrectAnswer::
	playfanfare MUS_LEVEL_UP
	waitfanfare
	msgbox CinnabarIsland_Gym_Text_CorrectGoOnThrough
	return

CinnabarIsland_Gym_EventScript_IncorrectAnswer::
	waitse
	playse SE_BOO
	waitse
	msgbox CinnabarIsland_Gym_Text_SorryBadCall
	closemessage
	return

CinnabarIsland_Gym_EventScript_Quiz1Correct::
	call CinnabarIsland_Gym_EventScript_CorrectAnswer
	goto_if_set FLAG_CINNABAR_GYM_QUIZ_1, CinnabarIsland_Gym_EventScript_DoorAlreadyOpen
	call CinnabarIsland_Gym_EventScript_Quiz1Complete
	releaseall
	end

CinnabarIsland_Gym_EventScript_Quiz1Complete::
	playse SE_UNLOCK
	waitse
	call CinnabarIsland_Gym_EventScript_OpenDoor1
	special DrawWholeMapView
	setflag FLAG_CINNABAR_GYM_QUIZ_1
	return

CinnabarIsland_Gym_EventScript_DoorAlreadyOpen::
	releaseall
	end

CinnabarIsland_Gym_EventScript_Quiz1Incorrect::
	call CinnabarIsland_Gym_EventScript_IncorrectAnswer
	goto_if_not_defeated TRAINER_BURGLAR_QUINN, CinnabarIsland_Gym_EventScript_BattleQuinn
	releaseall
	end

CinnabarIsland_Gym_EventScript_BattleQuinn::
	call_if_eq VAR_TEMP_1, 0, CinnabarIsland_Gym_EventScript_QuinnApproachLeft
	call_if_eq VAR_TEMP_1, 1, CinnabarIsland_Gym_EventScript_QuinnApproachRight
	textcolor NPC_TEXT_COLOR_MALE
	msgbox CinnabarIsland_Gym_Text_QuinnIntro
	trainerbattle_no_intro TRAINER_BURGLAR_QUINN, CinnabarIsland_Gym_Text_QuinnDefeat
	goto_if_set FLAG_CINNABAR_GYM_QUIZ_1, CinnabarIsland_Gym_EventScript_DoorAlreadyOpen
	call CinnabarIsland_Gym_EventScript_Quiz1Complete
	releaseall
	end

CinnabarIsland_Gym_EventScript_QuinnApproachLeft::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_QUINN, CinnabarIsland_Gym_Movement_QuinnApproachLeft
	waitmovement 0
	return

CinnabarIsland_Gym_EventScript_QuinnApproachRight::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_QUINN, CinnabarIsland_Gym_Movement_QuinnApproachRight
	waitmovement 0
	return

CinnabarIsland_Gym_Movement_QuinnApproachLeft::
	walk_left
	walk_left
	step_end

CinnabarIsland_Gym_Movement_QuinnApproachRight::
	walk_left
	step_end

CinnabarIsland_Gym_EventScript_Quiz2Left::
	lockall
	setvar VAR_TEMP_1, 0
	goto CinnabarIsland_Gym_EventScript_Quiz2
	end

CinnabarIsland_Gym_EventScript_Quiz2Right::
	lockall
	setvar VAR_TEMP_1, 1
	goto CinnabarIsland_Gym_EventScript_Quiz2
	end

CinnabarIsland_Gym_EventScript_Quiz2::
	msgbox CinnabarIsland_Gym_Text_PokemonQuizRules
	msgbox CinnabarIsland_Gym_Text_QuizQuestion2, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, CinnabarIsland_Gym_EventScript_Quiz2Incorrect
	goto_if_eq VAR_RESULT, NO, CinnabarIsland_Gym_EventScript_Quiz2Correct
	end

CinnabarIsland_Gym_EventScript_Quiz2Correct::
	call CinnabarIsland_Gym_EventScript_CorrectAnswer
	goto_if_set FLAG_CINNABAR_GYM_QUIZ_2, CinnabarIsland_Gym_EventScript_DoorAlreadyOpen
	call CinnabarIsland_Gym_EventScript_Quiz2Complete
	releaseall
	end

CinnabarIsland_Gym_EventScript_Quiz2Complete::
	playse SE_UNLOCK
	waitse
	call CinnabarIsland_Gym_EventScript_OpenDoor2
	special DrawWholeMapView
	setflag FLAG_CINNABAR_GYM_QUIZ_2
	return

CinnabarIsland_Gym_EventScript_Quiz2Incorrect::
	call CinnabarIsland_Gym_EventScript_IncorrectAnswer
	goto_if_not_defeated TRAINER_SUPER_NERD_AVERY, CinnabarIsland_Gym_EventScript_BattleAvery
	releaseall
	end

CinnabarIsland_Gym_EventScript_BattleAvery::
	call_if_eq VAR_TEMP_1, 0, CinnabarIsland_Gym_EventScript_AveryApproachLeft
	call_if_eq VAR_TEMP_1, 1, CinnabarIsland_Gym_EventScript_AveryApproachRight
	textcolor NPC_TEXT_COLOR_MALE
	msgbox CinnabarIsland_Gym_Text_AveryIntro
	trainerbattle_no_intro TRAINER_SUPER_NERD_AVERY, CinnabarIsland_Gym_Text_AveryDefeat
	goto_if_set FLAG_CINNABAR_GYM_QUIZ_2, CinnabarIsland_Gym_EventScript_DoorAlreadyOpen
	call CinnabarIsland_Gym_EventScript_Quiz2Complete
	releaseall
	end

CinnabarIsland_Gym_EventScript_AveryApproachLeft::
	applymovement LOCALID_PLAYER, CinnabarIsland_Gym_Movement_PlayerFaceAvery
	applymovement LOCALID_AVERY, CinnabarIsland_Gym_Movement_AveryApproachLeft
	waitmovement 0
	return

CinnabarIsland_Gym_EventScript_AveryApproachRight::
	applymovement LOCALID_PLAYER, CinnabarIsland_Gym_Movement_PlayerFaceAvery
	applymovement LOCALID_AVERY, CinnabarIsland_Gym_Movement_AveryApproachRight
	waitmovement 0
	return

CinnabarIsland_Gym_Movement_AveryApproachLeft::
	walk_up
	walk_up
	walk_left
	step_end

CinnabarIsland_Gym_Movement_AveryApproachRight::
	walk_up
	walk_up
	walk_in_place_faster_left
	step_end

CinnabarIsland_Gym_Movement_PlayerFaceAvery::
	delay_16
	delay_16
	walk_in_place_faster_right
	step_end

CinnabarIsland_Gym_EventScript_Quiz3Left::
	lockall
	setvar VAR_TEMP_1, 0
	goto CinnabarIsland_Gym_EventScript_Quiz3
	end

CinnabarIsland_Gym_EventScript_Quiz3Right::
	lockall
	setvar VAR_TEMP_1, 1
	goto CinnabarIsland_Gym_EventScript_Quiz3
	end

CinnabarIsland_Gym_EventScript_Quiz3::
	msgbox CinnabarIsland_Gym_Text_PokemonQuizRules
	msgbox CinnabarIsland_Gym_Text_QuizQuestion3, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, CinnabarIsland_Gym_EventScript_Quiz3Incorrect
	goto_if_eq VAR_RESULT, NO, CinnabarIsland_Gym_EventScript_Quiz3Correct
	end

CinnabarIsland_Gym_EventScript_Quiz3Correct::
	call CinnabarIsland_Gym_EventScript_CorrectAnswer
	goto_if_set FLAG_CINNABAR_GYM_QUIZ_3, CinnabarIsland_Gym_EventScript_DoorAlreadyOpen
	call CinnabarIsland_Gym_EventScript_Quiz3Complete
	releaseall
	end

CinnabarIsland_Gym_EventScript_Quiz3Complete::
	playse SE_UNLOCK
	waitse
	call CinnabarIsland_Gym_EventScript_OpenDoor3
	special DrawWholeMapView
	setflag FLAG_CINNABAR_GYM_QUIZ_3
	return

CinnabarIsland_Gym_EventScript_Quiz3Incorrect::
	call CinnabarIsland_Gym_EventScript_IncorrectAnswer
	goto_if_not_defeated TRAINER_BURGLAR_RAMON, CinnabarIsland_Gym_EventScript_BattleRamon
	releaseall
	end

CinnabarIsland_Gym_EventScript_BattleRamon::
	call_if_eq VAR_TEMP_1, 0, CinnabarIsland_Gym_EventScript_RamonApproachLeft
	call_if_eq VAR_TEMP_1, 1, CinnabarIsland_Gym_EventScript_RamonApproachRight
	textcolor NPC_TEXT_COLOR_MALE
	msgbox CinnabarIsland_Gym_Text_RamonIntro
	trainerbattle_no_intro TRAINER_BURGLAR_RAMON, CinnabarIsland_Gym_Text_RamonDefeat
	goto_if_set FLAG_CINNABAR_GYM_QUIZ_3, CinnabarIsland_Gym_EventScript_DoorAlreadyOpen
	call CinnabarIsland_Gym_EventScript_Quiz3Complete
	releaseall
	end

CinnabarIsland_Gym_EventScript_RamonApproachLeft::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_RAMON, CinnabarIsland_Gym_Movement_RamonApproachLeft
	waitmovement 0
	return

CinnabarIsland_Gym_EventScript_RamonApproachRight::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_RAMON, CinnabarIsland_Gym_Movement_RamonApproachRight
	waitmovement 0
	return

CinnabarIsland_Gym_Movement_RamonApproachLeft::
	walk_left
	walk_left
	step_end

CinnabarIsland_Gym_Movement_RamonApproachRight::
	walk_left
	step_end

CinnabarIsland_Gym_EventScript_Quiz4Left::
	lockall
	setvar VAR_TEMP_1, 0
	goto CinnabarIsland_Gym_EventScript_Quiz4
	end

CinnabarIsland_Gym_EventScript_Quiz4Right::
	lockall
	setvar VAR_TEMP_1, 1
	goto CinnabarIsland_Gym_EventScript_Quiz4
	end

CinnabarIsland_Gym_EventScript_Quiz4::
	msgbox CinnabarIsland_Gym_Text_PokemonQuizRules
	msgbox CinnabarIsland_Gym_Text_QuizQuestion4, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, CinnabarIsland_Gym_EventScript_Quiz4Incorrect
	goto_if_eq VAR_RESULT, NO, CinnabarIsland_Gym_EventScript_Quiz4Correct
	end

CinnabarIsland_Gym_EventScript_Quiz4Correct::
	call CinnabarIsland_Gym_EventScript_CorrectAnswer
	goto_if_set FLAG_CINNABAR_GYM_QUIZ_4, CinnabarIsland_Gym_EventScript_DoorAlreadyOpen
	call CinnabarIsland_Gym_EventScript_Quiz4Complete
	releaseall
	end

CinnabarIsland_Gym_EventScript_Quiz4Complete::
	playse SE_UNLOCK
	waitse
	call CinnabarIsland_Gym_EventScript_OpenDoor4
	special DrawWholeMapView
	setflag FLAG_CINNABAR_GYM_QUIZ_4
	return

CinnabarIsland_Gym_EventScript_Quiz4Incorrect::
	call CinnabarIsland_Gym_EventScript_IncorrectAnswer
	goto_if_not_defeated TRAINER_SUPER_NERD_DEREK, CinnabarIsland_Gym_EventScript_BattleDerek
	releaseall
	end

CinnabarIsland_Gym_EventScript_BattleDerek::
	call_if_eq VAR_TEMP_1, 0, CinnabarIsland_Gym_EventScript_DerekApproachLeft
	call_if_eq VAR_TEMP_1, 1, CinnabarIsland_Gym_EventScript_DerekApproachRight
	textcolor NPC_TEXT_COLOR_MALE
	msgbox CinnabarIsland_Gym_Text_DerekIntro
	trainerbattle_no_intro TRAINER_SUPER_NERD_DEREK, CinnabarIsland_Gym_Text_DerekDefeat
	goto_if_set FLAG_CINNABAR_GYM_QUIZ_4, CinnabarIsland_Gym_EventScript_DoorAlreadyOpen
	call CinnabarIsland_Gym_EventScript_Quiz4Complete
	releaseall
	end

CinnabarIsland_Gym_EventScript_DerekApproachLeft::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_DEREK, CinnabarIsland_Gym_Movement_DerekApproachLeft
	waitmovement 0
	return

CinnabarIsland_Gym_EventScript_DerekApproachRight::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_DEREK, CinnabarIsland_Gym_Movement_DerekApproachRight
	waitmovement 0
	return

CinnabarIsland_Gym_Movement_DerekApproachLeft::
	walk_left
	walk_left
	step_end

CinnabarIsland_Gym_Movement_DerekApproachRight::
	walk_left
	step_end

CinnabarIsland_Gym_EventScript_Quiz5Left::
	lockall
	setvar VAR_TEMP_1, 0
	goto CinnabarIsland_Gym_EventScript_Quiz5
	end

CinnabarIsland_Gym_EventScript_Quiz5Right::
	lockall
	setvar VAR_TEMP_1, 1
	goto CinnabarIsland_Gym_EventScript_Quiz5
	end

CinnabarIsland_Gym_EventScript_Quiz5::
	msgbox CinnabarIsland_Gym_Text_PokemonQuizRules
	msgbox CinnabarIsland_Gym_Text_QuizQuestion5, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, CinnabarIsland_Gym_EventScript_Quiz5Correct
	goto_if_eq VAR_RESULT, NO, CinnabarIsland_Gym_EventScript_Quiz5Incorrect
	end

CinnabarIsland_Gym_EventScript_Quiz5Correct::
	call CinnabarIsland_Gym_EventScript_CorrectAnswer
	goto_if_set FLAG_CINNABAR_GYM_QUIZ_5, CinnabarIsland_Gym_EventScript_DoorAlreadyOpen
	call CinnabarIsland_Gym_EventScript_Quiz5Complete
	releaseall
	end

CinnabarIsland_Gym_EventScript_Quiz5Complete::
	playse SE_UNLOCK
	waitse
	call CinnabarIsland_Gym_EventScript_OpenDoor5
	special DrawWholeMapView
	setflag FLAG_CINNABAR_GYM_QUIZ_5
	return

CinnabarIsland_Gym_EventScript_Quiz5Incorrect::
	call CinnabarIsland_Gym_EventScript_IncorrectAnswer
	goto_if_not_defeated TRAINER_BURGLAR_DUSTY, CinnabarIsland_Gym_EventScript_BattleDusty
	releaseall
	end

CinnabarIsland_Gym_EventScript_BattleDusty::
	call_if_eq VAR_TEMP_1, 0, CinnabarIsland_Gym_EventScript_DustyApproachLeft
	call_if_eq VAR_TEMP_1, 1, CinnabarIsland_Gym_EventScript_DustyApproachRight
	textcolor NPC_TEXT_COLOR_MALE
	msgbox CinnabarIsland_Gym_Text_DustyIntro
	trainerbattle_no_intro TRAINER_BURGLAR_DUSTY, CinnabarIsland_Gym_Text_DustyDefeat
	goto_if_set FLAG_CINNABAR_GYM_QUIZ_5, CinnabarIsland_Gym_EventScript_DoorAlreadyOpen
	call CinnabarIsland_Gym_EventScript_Quiz5Complete
	releaseall
	end

CinnabarIsland_Gym_EventScript_DustyApproachLeft::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_DUSTY, CinnabarIsland_Gym_Movement_DustyApproachLeft
	waitmovement 0
	return

CinnabarIsland_Gym_EventScript_DustyApproachRight::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_DUSTY, CinnabarIsland_Gym_Movement_DustyApproachRight
	waitmovement 0
	return

CinnabarIsland_Gym_Movement_DustyApproachLeft::
	walk_left
	walk_left
	step_end

CinnabarIsland_Gym_Movement_DustyApproachRight::
	walk_left
	step_end

CinnabarIsland_Gym_EventScript_Quiz6Left::
	lockall
	setvar VAR_TEMP_1, 0
	goto CinnabarIsland_Gym_EventScript_Quiz6
	end

CinnabarIsland_Gym_EventScript_Quiz6Right::
	lockall
	setvar VAR_TEMP_1, 1
	goto CinnabarIsland_Gym_EventScript_Quiz6
	end

CinnabarIsland_Gym_EventScript_Quiz6::
	msgbox CinnabarIsland_Gym_Text_PokemonQuizRules
	msgbox CinnabarIsland_Gym_Text_QuizQuestion6, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, CinnabarIsland_Gym_EventScript_Quiz6Incorrect
	goto_if_eq VAR_RESULT, NO, CinnabarIsland_Gym_EventScript_Quiz6Correct
	end

CinnabarIsland_Gym_EventScript_Quiz6Correct::
	call CinnabarIsland_Gym_EventScript_CorrectAnswer
	goto_if_set FLAG_CINNABAR_GYM_QUIZ_6, CinnabarIsland_Gym_EventScript_DoorAlreadyOpen
	call CinnabarIsland_Gym_EventScript_Quiz6Complete
	releaseall
	end

CinnabarIsland_Gym_EventScript_Quiz6Complete::
	playse SE_UNLOCK
	waitse
	call CinnabarIsland_Gym_EventScript_OpenDoor6
	special DrawWholeMapView
	setflag FLAG_CINNABAR_GYM_QUIZ_6
	return

CinnabarIsland_Gym_EventScript_Quiz6Incorrect::
	call CinnabarIsland_Gym_EventScript_IncorrectAnswer
	goto_if_not_defeated TRAINER_SUPER_NERD_ZAC, CinnabarIsland_Gym_EventScript_BattleZac
	releaseall
	end

CinnabarIsland_Gym_EventScript_BattleZac::
	call_if_eq VAR_TEMP_1, 0, CinnabarIsland_Gym_EventScript_ZacApproachLeft
	call_if_eq VAR_TEMP_1, 1, CinnabarIsland_Gym_EventScript_ZacApproachRight
	textcolor NPC_TEXT_COLOR_MALE
	msgbox CinnabarIsland_Gym_Text_ZacIntro
	trainerbattle_no_intro TRAINER_SUPER_NERD_ZAC, CinnabarIsland_Gym_Text_ZacDefeat
	goto_if_set FLAG_CINNABAR_GYM_QUIZ_6, CinnabarIsland_Gym_EventScript_DoorAlreadyOpen
	call CinnabarIsland_Gym_EventScript_Quiz6Complete
	releaseall
	end

CinnabarIsland_Gym_EventScript_ZacApproachLeft::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_ZAC, CinnabarIsland_Gym_Movement_ZacApproachLeft
	waitmovement 0
	return

CinnabarIsland_Gym_EventScript_ZacApproachRight::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_ZAC, CinnabarIsland_Gym_Movement_ZacApproachRight
	waitmovement 0
	return

CinnabarIsland_Gym_Movement_ZacApproachLeft::
	walk_left
	walk_left
	step_end

CinnabarIsland_Gym_Movement_ZacApproachRight::
	walk_left
	step_end

CinnabarIsland_Gym_EventScript_OpenDoor1::
	setmetatile 26,  8, METATILE_CinnabarGym_Floor_WallLeftCorner, 0
	setmetatile 27,  8, METATILE_CinnabarGym_Floor_WallRightCorner, 0
	setmetatile 26,  9, METATILE_CinnabarGym_Floor_WallLeftEdge, 0
	setmetatile 27,  9, METATILE_CinnabarGym_Floor_WallRightEdge, 0
	setmetatile 26, 10, METATILE_CinnabarGym_Floor_ShadeFull, 0
	setmetatile 27, 10, METATILE_CinnabarGym_Floor, 0
	setmetatile 28, 10, METATILE_CinnabarGym_Floor_ShadeDiagonal, 0
	return

CinnabarIsland_Gym_EventScript_OpenDoor2::
	setmetatile 17,  8, METATILE_CinnabarGym_Floor_WallLeftCorner, 0
	setmetatile 18,  8, METATILE_CinnabarGym_Floor_WallRightCorner, 0
	setmetatile 17,  9, METATILE_CinnabarGym_Floor_WallLeftEdge, 0
	setmetatile 18,  9, METATILE_CinnabarGym_Floor_WallRightEdge, 0
	setmetatile 17, 10, METATILE_CinnabarGym_Floor_ShadeFull, 0
	setmetatile 18, 10, METATILE_CinnabarGym_Floor, 0
	setmetatile 19, 10, METATILE_CinnabarGym_Floor_ShadeDiagonal, 0
	return

CinnabarIsland_Gym_EventScript_OpenDoor3::
	setmetatile 17, 15, METATILE_CinnabarGym_Floor_WallLeftCorner, 0
	setmetatile 18, 15, METATILE_CinnabarGym_Floor_WallRightCorner, 0
	setmetatile 17, 16, METATILE_CinnabarGym_Floor_WallLeftEdge, 0
	setmetatile 18, 16, METATILE_CinnabarGym_Floor_WallRightEdge, 0
	setmetatile 17, 17, METATILE_CinnabarGym_Floor_ShadeFull, 0
	setmetatile 18, 17, METATILE_CinnabarGym_Floor, 0
	setmetatile 19, 17, METATILE_CinnabarGym_Floor_ShadeDiagonal, 0
	return

CinnabarIsland_Gym_EventScript_OpenDoor4::
	setmetatile 11, 21, METATILE_CinnabarGym_Wall_RetractedBarrier, 1
	setmetatile 11, 22, METATILE_CinnabarGym_Floor_ShadeFull, 0
	setmetatile 11, 23, METATILE_CinnabarGym_Floor, 0
	return

CinnabarIsland_Gym_EventScript_OpenDoor5::
	setmetatile  5, 16, METATILE_CinnabarGym_Floor_WallLeftCorner, 0
	setmetatile  6, 16, METATILE_CinnabarGym_Floor_WallRightCorner, 0
	setmetatile  5, 17, METATILE_CinnabarGym_Floor_WallLeftEdge, 0
	setmetatile  6, 17, METATILE_CinnabarGym_Floor_WallRightEdge, 0
	setmetatile  5, 18, METATILE_CinnabarGym_Floor_ShadeFull, 0
	setmetatile  6, 18, METATILE_CinnabarGym_Floor, 0
	setmetatile  7, 18, METATILE_CinnabarGym_Floor_ShadeDiagonal, 0
	return

CinnabarIsland_Gym_EventScript_OpenDoor6::
	setmetatile  5,  8, METATILE_CinnabarGym_Floor_WallLeftCorner, 0
	setmetatile  6,  8, METATILE_CinnabarGym_Floor_WallRightCorner, 0
	setmetatile  5,  9, METATILE_CinnabarGym_Floor_WallLeftEdge, 0
	setmetatile  6,  9, METATILE_CinnabarGym_Floor_WallRightEdge, 0
	setmetatile  5, 10, METATILE_CinnabarGym_Floor_ShadeFull, 0
	setmetatile  6, 10, METATILE_CinnabarGym_Floor, 0
	setmetatile  7, 10, METATILE_CinnabarGym_Floor_ShadeDiagonal, 0
	return

CinnabarIsland_Gym_Text_BlaineIntro::
	.string "{JPN}うおおーす!\p"
	.string "わしは もえる おとこ!\n"
	.string "グレンじま ポケモンジムの カツラ\p"
	.string "わしの ポケモンは すべてを\n"
	.string "ほのおで やいて こがしまくる\l"
	.string "つわもの ばかり なのだー!\p"
	.string "うおおーす!\n"
	.string "やけどなおしの よういは いいか!{PLAY_BGM MUS_RG_ENCOUNTER_GYM_LEADER}$"

CinnabarIsland_Gym_Text_BlaineDefeat::
	.string "{JPN}わしは もえつきた!\p"
	.string "おまえに こそ!\n"
	.string "クリムゾン バッジは ふさわしい!$"

CinnabarIsland_Gym_Text_FireBlastIsUltimateFireMove::
	.string "{JPN}この わざを みんなは\n"
	.string "だいもんじやき と よんでるが\l"
	.string "そりゃ まちがいだ!\p"
	.string "ほんば きょうと では\n"
	.string "だいもんじ とか おくりび と\l"
	.string "よんで いるのだ!$"

CinnabarIsland_Gym_Text_ExplainVolcanoBadge::
	.string "{JPN}うおーす!\p"
	.string "この クリムゾン バッジは\n"
	.string "もってる だけで\p"
	.string "ポケモンの とくしゅ のうりょくを\n"
	.string "たかめる こうかが ある!\p"
	.string "それから!\n"
	.string "この わざマシンを うけとれい!$"

CinnabarIsland_Gym_Text_ReceivedTM38FromBlaine::
	.string "{JPN}{PLAYER}は カツラから\n"
	.string "わざマシン38を もらった!$"

CinnabarIsland_Gym_Text_BlainePostBattle::
	.string "{JPN}わざマシン38は\n"
	.string "もえる おおわざ! だいもんじ!\p"
	.string "だいもんじは\n"
	.string "ほのお タイプの ポケモン!\p"
	.string "ロコンや リザードに\n"
	.string "おぼえさせると よいぞ!$"

CinnabarIsland_Gym_Text_MakeSpaceForThis::
	.string "{JPN}にもつが いっぱいだ$"

CinnabarIsland_Gym_Text_ErikIntro::
	.string "{JPN}ポケモンが はく ほのおの おんど\n"
	.string "どの くらいか しってる?$"

CinnabarIsland_Gym_Text_ErikDefeat::
	.string "{JPN}… あち! やられた$"

CinnabarIsland_Gym_Text_ErikPostBattle::
	.string "{JPN}もえる っていう のは\n"
	.string "くうき ちゅうの さんそが…\p"
	.string "むにゃむにゃ… ブツブツ$"

CinnabarIsland_Gym_Text_QuinnIntro::
	.string "{JPN}どろぼうと なのって おりやすが\n"
	.string "あしを あらって\l"
	.string "ポケモンの しゅぎょう してまっせ$"

CinnabarIsland_Gym_Text_QuinnDefeat::
	.string "{JPN}まいりやした!$"

CinnabarIsland_Gym_Text_QuinnPostBattle::
	.string "{JPN}ひとの ポケモン みると\n"
	.string "つい ぬすみたく なっちゃうんでさ!$"

CinnabarIsland_Gym_Text_AveryIntro::
	.string "{JPN}ポケモンを けんきゅう しつくした\n"
	.string "ぼくに かてるかい!$"

CinnabarIsland_Gym_Text_AveryDefeat::
	.string "{JPN}…!\n"
	.string "もっと けんきゅう しなきゃ$"

CinnabarIsland_Gym_Text_AveryPostBattle::
	.string "{JPN}むずかしい はなしは\n"
	.string "きみに いっても わからないよ$"

CinnabarIsland_Gym_Text_RamonIntro::
	.string "{JPN}ただ なんとなく すき なんで\n"
	.string "ほのお ポケモン つかってるぜ!$"

CinnabarIsland_Gym_Text_RamonDefeat::
	.string "{JPN}… もえた!$"

CinnabarIsland_Gym_Text_RamonPostBattle::
	.string "{JPN}おいらの ゆめ…?\p"
	.string "どろぼう ポケモンが いりゃ\n"
	.string "ぜひ つかって みたいねー!$"

CinnabarIsland_Gym_Text_DerekIntro::
	.string "{JPN}おれは カツラが ポケモン\n"
	.string "はじめた きっかけを しってるぜ$"

CinnabarIsland_Gym_Text_DerekDefeat::
	.string "{JPN}ありゃまあ$"

CinnabarIsland_Gym_Text_DerekPostBattle::
	.string "{JPN}リーダーの カツラが\n"
	.string "やま のぼりで そうなん したとき\l"
	.string "くらい やまみちを てらして\l"
	.string "ひのとり ポケモンが とんでった!\p"
	.string "その おかげで カツラは ぶじ\n"
	.string "やまを おりることが できたのさ$"

CinnabarIsland_Gym_Text_DustyIntro::
	.string "{JPN}いろんな ジムに いったけど\n"
	.string "おいらには ここが あってるねえ$"

CinnabarIsland_Gym_Text_DustyDefeat::
	.string "{JPN}あー あつかった$"

CinnabarIsland_Gym_Text_DustyPostBattle::
	.string "{JPN}ギャロップ キュウコン あたりが\n"
	.string "ほのお タイプで にんきが あるよ$"

CinnabarIsland_Gym_Text_ZacIntro::
	.string "{JPN}ほのおの じゃくてんは\n"
	.string "エイチ ツー オー と いわれてる$"

CinnabarIsland_Gym_Text_ZacDefeat::
	.string "{JPN}へろへろ…$"

CinnabarIsland_Gym_Text_ZacPostBattle::
	.string "{JPN}たしかに ほのおは みずに よわい!\n"
	.string "でも とかして しまうから\l"
	.string "こおり ポケモンには つよいよ$"

CinnabarIsland_Gym_Text_GymGuyAdvice::
	.string "{JPN}おーす!\n"
	.string "みらいの チャンピオン!\p"
	.string "ねっけつ カツラは\n"
	.string "ほのおの プロフェッショナルだ!\p"
	.string "それなら こっちは\n"
	.string "クールに みずで きめろ!\p"
	.string "あと やけどなおしを\n"
	.string "もって いった ほうが いいぜ!$"

CinnabarIsland_Gym_Text_GymGuyPostVictory::
	.string "{JPN}{PLAYER}! かったか!\n"
	.string "あつ くるしい おやじ だったな!$"

CinnabarIsland_Gym_Text_GymStatue::
	.string "{JPN}グレン ポケモン ジム\n"
	.string "カツラ にんてい トレーナー!\l"
	.string "{RIVAL}$"

CinnabarIsland_Gym_Text_GymStatuePlayerWon::
	.string "{JPN}グレン ポケモン ジム\n"
	.string "カツラ にんてい トレーナー!\l"
	.string "{RIVAL} {PLAYER}$"

CinnabarIsland_Gym_Text_PokemonQuizRules::
	.string "{JPN}ポケモン クイズ!\p"
	.string "せいかい すると ドアが あいて\n"
	.string "つぎへ すすめます!\p"
	.string "まちがえたら でしの トレーナーと\n"
	.string "たたかって いただきます!\p"
	.string "ここの リーダーに あう まで\n"
	.string "ポケモンの たいりょくを\l"
	.string "とって おきたいなら!\p"
	.string "がんばって こたえて ください!\n"
	.string "では おこたえ ください!$"

CinnabarIsland_Gym_Text_QuizQuestion1::
	.string "{JPN}ポケモン キャタピーが しんかすると\n"
	.string "トランセルに なる?$"

CinnabarIsland_Gym_Text_QuizQuestion2::
	.string "{JPN}ポケモン リーグ にんてい\n"
	.string "バッジは ぜんぶで 9しゅるい?$"

CinnabarIsland_Gym_Text_QuizQuestion3::
	.string "{JPN}ニョロモは 3かい しんかする\n"
	.string "ポケモン である?$"

CinnabarIsland_Gym_Text_QuizQuestion4::
	.string "{JPN}でんき タイプの わざを\n"
	.string "くりだした とき\p"
	.string "じめん タイプの ポケモンには\n"
	.string "よく きく?$"

CinnabarIsland_Gym_Text_QuizQuestion5::
	.string "{JPN}おなじ レベルの おなじ ポケモン\n"
	.string "でも つかまえる たびに\l"
	.string "つよさは ちがう?$"

CinnabarIsland_Gym_Text_QuizQuestion6::
	.string "{JPN}「わざマシン28」とは\n"
	.string "「しねしねこうせん」である?$"

CinnabarIsland_Gym_Text_CorrectGoOnThrough::
	.string "{JPN}あたり です!\p"
	.string "さきへ すすんで いいです$"

CinnabarIsland_Gym_Text_SorryBadCall::
	.string "{JPN}ばか はずれです…$"

