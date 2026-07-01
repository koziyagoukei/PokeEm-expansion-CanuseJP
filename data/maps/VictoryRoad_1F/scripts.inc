VictoryRoad_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VictoryRoad_1F_OnTransition
	.byte 0

VictoryRoad_1F_OnTransition:
	call_if_eq VAR_VICTORY_ROAD_1F_STATE, 1, VictoryRoad_1F_EventScript_SetEntranceWallyPos1
	call_if_eq VAR_VICTORY_ROAD_1F_STATE, 2, VictoryRoad_1F_EventScript_SetEntranceWallyPos2
	end

VictoryRoad_1F_EventScript_SetEntranceWallyPos1::
	setobjectxyperm LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, 2, 24
	setobjectmovementtype LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, MOVEMENT_TYPE_FACE_DOWN
	return

VictoryRoad_1F_EventScript_SetEntranceWallyPos2::
	setobjectxyperm LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, 3, 24
	setobjectmovementtype LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, MOVEMENT_TYPE_FACE_DOWN
	return

VictoryRoad_1F_EventScript_WallyBattleTrigger1::
	lockall
	setvar VAR_0x8008, 1
	addobject LOCALID_VICTORY_ROAD_ENTRANCE_WALLY
	applymovement LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, VictoryRoad_1F_Movement_WallyApproachPlayer1
	waitmovement 0
	goto VictoryRoad_1F_EventScript_WallyEntranceBattle
	end

VictoryRoad_1F_EventScript_WallyBattleTrigger2::
	lockall
	setvar VAR_0x8008, 2
	addobject LOCALID_VICTORY_ROAD_ENTRANCE_WALLY
	applymovement LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, VictoryRoad_1F_Movement_WallyApproachPlayer2
	waitmovement 0
	goto VictoryRoad_1F_EventScript_WallyEntranceBattle
	end

VictoryRoad_1F_EventScript_WallyEntranceBattle::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox VictoryRoad_1F_Text_WallyNotGoingToLoseAnymore, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLY_VR_1, VictoryRoad_1F_Text_WallyEntranceDefeat
	msgbox VictoryRoad_1F_Text_WallyPostEntranceBattle, MSGBOX_DEFAULT
	clearflag FLAG_HIDE_VICTORY_ROAD_ENTRANCE_WALLY
	copyobjectxytoperm LOCALID_VICTORY_ROAD_ENTRANCE_WALLY
	setflag FLAG_DEFEATED_WALLY_VICTORY_ROAD
	copyvar VAR_VICTORY_ROAD_1F_STATE, VAR_0x8008
	releaseall
	end

VictoryRoad_1F_Movement_WallyApproachPlayer1:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

VictoryRoad_1F_Movement_WallyApproachPlayer2:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

@ This Wally appears near the entrance once his battle is triggered and remains there until the Hall of Fame is entered
VictoryRoad_1F_EventScript_EntranceWally::
	msgbox VictoryRoad_1F_Text_WallyPostEntranceBattle, MSGBOX_NPC
	end

@ This Wally appears and remains at the exit after the Hall of Fame is entered
VictoryRoad_1F_EventScript_ExitWally::
	trainerbattle_single TRAINER_WALLY_VR_2, VictoryRoad_1F_Text_WallyIntro, VictoryRoad_1F_Text_WallyDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, VictoryRoad_1F_EventScript_RematchWally
	msgbox VictoryRoad_1F_Text_WallyPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_RematchWally::
	trainerbattle_rematch TRAINER_WALLY_VR_2, VictoryRoad_1F_Text_WallyIntro, VictoryRoad_1F_Text_WallyDefeat
	msgbox VictoryRoad_1F_Text_WallyPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Edgar::
	trainerbattle_single TRAINER_EDGAR, VictoryRoad_1F_Text_EdgarIntro, VictoryRoad_1F_Text_EdgarDefeat
	msgbox VictoryRoad_1F_Text_EdgarPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Albert::
	trainerbattle_single TRAINER_ALBERT, VictoryRoad_1F_Text_AlbertIntro, VictoryRoad_1F_Text_AlbertDefeat
	msgbox VictoryRoad_1F_Text_AlbertPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Hope::
	trainerbattle_single TRAINER_HOPE, VictoryRoad_1F_Text_HopeIntro, VictoryRoad_1F_Text_HopeDefeat
	msgbox VictoryRoad_1F_Text_HopePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Quincy::
	trainerbattle_single TRAINER_QUINCY, VictoryRoad_1F_Text_QuincyIntro, VictoryRoad_1F_Text_QuincyDefeat
	msgbox VictoryRoad_1F_Text_QuincyPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Katelynn::
	trainerbattle_single TRAINER_KATELYNN, VictoryRoad_1F_Text_KatelynnIntro, VictoryRoad_1F_Text_KatelynnDefeat
	msgbox VictoryRoad_1F_Text_KatelynnPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_Text_WallyNotGoingToLoseAnymore:
	.string "{JPN}ミツル『あっ! {PLAYER}さん!\pぼくが こんな ところに いるなんて\nおどろいた でしょ!\pぼくが ここまで これたのは\n{PLAYER}さんの おかげだよ!\pあのとき {PLAYER}さんに まけたから\nぼくは つよく なれたんだ!\p……だけど!\nぼくは もう まけない\pぼくに ゆうきと げんきを くれた\nポケモンの ためにも かつんだ!\pそれでは…… いきますよ!$"

VictoryRoad_1F_Text_WallyEntranceDefeat:
	.string "{JPN}うわぁ!\nやっぱり {PLAYER}さんは つよいよ!$"

VictoryRoad_1F_Text_WallyPostEntranceBattle:
	.string "{JPN}ミツル『…… こんかいは\n{PLAYER}さんに かてなかった けど\lいつか きっと おいついて みせるからね!$"

VictoryRoad_1F_Text_WallyIntro:
	.string "{JPN}ミツル『あっ! {PLAYER}さん!\pぼく あれから つよくなった はずだよ!\nそれを {PLAYER}さんに\lみてもらいたいんだ!\pそれでは…… いきますよ!$"

VictoryRoad_1F_Text_WallyDefeat:
	.string "{JPN}うわぁ!\nやっぱり {PLAYER}さんは つよいよ!$"

VictoryRoad_1F_Text_WallyPostBattle:
	.string "{JPN}ミツル『…… こんかいも\n{PLAYER}さんに かてなかった けど\lいつか きっと おいついて……\pポケモンリーグに ちょうせん するんだ!$"

VictoryRoad_1F_Text_EdgarIntro:
	.string "{JPN}ここまでは なんとか これたが\nあと すこしが とても とおいよ……$"

VictoryRoad_1F_Text_EdgarDefeat:
	.string "{JPN}ここで おわったか……$"

VictoryRoad_1F_Text_EdgarPostBattle:
	.string "{JPN}ここまで きたなら\nいきおいで チャンピオンに なれよ!\lきみなら できるさ!$"

VictoryRoad_1F_Text_AlbertIntro:
	.string "{JPN}せっかく ここまで きたんだ\nいまさら まけるなんて ありえないよ!$"

VictoryRoad_1F_Text_AlbertDefeat:
	.string "{JPN}まさか……\nまけたのか……?$"

VictoryRoad_1F_Text_AlbertPostBattle:
	.string "{JPN}ここで まけた……\pということは まだ チャンピオンに なる\nしかくが なかったと いうことか……$"

VictoryRoad_1F_Text_HopeIntro:
	.string "{JPN}この はてしなく けわしい みち こそ\nまさに チャンピオンへの みち なのよ!$"

VictoryRoad_1F_Text_HopeDefeat:
	.string "{JPN}すばらしい たたかいかた ね……$"

VictoryRoad_1F_Text_HopePostBattle:
	.string "{JPN}あなたには チャンピオンに なれる\nかのうせいが あるみたいね$"

VictoryRoad_1F_Text_QuincyIntro:
	.string "{JPN}チャンピオンロード とは なにか?\nかてたら おしえてやろう!$"

VictoryRoad_1F_Text_QuincyDefeat:
	.string "{JPN}うん いいだろう!$"

VictoryRoad_1F_Text_QuincyPostBattle:
	.string "{JPN}ここを ぶじに ぬけきるって ことが\nポケモンチャンピオンを めざす やつの\lさいごの しれん なんだ!\pだから チャンピオンロードと\nよばれて いるのさ$"

VictoryRoad_1F_Text_KatelynnIntro:
	.string "{JPN}ここまで きた ひとに\nなにも いうことは ない…… いくわよ!$"

VictoryRoad_1F_Text_KatelynnDefeat:
	.string "{JPN}くやしいわ……!$"

VictoryRoad_1F_Text_KatelynnPostBattle:
	.string "{JPN}ふん…… いいわよ\nあなた から さきに いきなさいよ$"
