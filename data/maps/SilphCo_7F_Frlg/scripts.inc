SilphCo_7F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_7F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, SilphCo_7F_OnTransition
	.byte 0

SilphCo_7F_OnLoad::
	call_if_unset FLAG_SILPH_7F_DOOR_1, EventScript_Close7FDoor1
	call_if_unset FLAG_SILPH_7F_DOOR_2, EventScript_Close7FDoor2
	call_if_unset FLAG_SILPH_7F_DOOR_3, EventScript_Close7FDoor3
	end

SilphCo_7F_OnTransition::
	call_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_7F_EventScript_SetObjRocketsGone
	end

SilphCo_7F_EventScript_SetObjRocketsGone::
	setobjectmovementtype LOCALID_SILPH_CO_7F_WORKER_M2, MOVEMENT_TYPE_FACE_UP
	return

SilphCo_7F_EventScript_RivalTriggerTop::
	lockall
	setvar VAR_TEMP_1, 0
	goto SilphCo_7F_EventScript_RivalScene
	end

SilphCo_7F_EventScript_RivalTriggerBottom::
	lockall
	setvar VAR_TEMP_1, 1
	goto SilphCo_7F_EventScript_RivalScene
	end

SilphCo_7F_EventScript_RivalScene::
	textcolor NPC_TEXT_COLOR_MALE
	playbgm MUS_RG_ENCOUNTER_RIVAL, 0
	applymovement LOCALID_SILPH_CO_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_SILPH_CO_RIVAL, Common_Movement_Delay48
	waitmovement 0
	msgbox SilphCo_7F_Text_RivalWhatKeptYou
	call_if_eq VAR_TEMP_1, 0, SilphCo_7F_EventScript_RivalApproachTop
	call_if_eq VAR_TEMP_1, 1, SilphCo_7F_EventScript_RivalApproachBottom
	msgbox SilphCo_7F_Text_RivalIntro
	setvar VAR_LAST_TALKED, LOCALID_SILPH_CO_RIVAL
	call_if_eq VAR_STARTER_MON, 2, SilphCo_7F_EventScript_RivalSquirtle
	call_if_eq VAR_STARTER_MON, 1, SilphCo_7F_EventScript_RivalBulbasaur
	call_if_eq VAR_STARTER_MON, 0, SilphCo_7F_EventScript_RivalCharmander
	msgbox SilphCo_7F_Text_RivalPostBattle
	closemessage
	playbgm MUS_RG_RIVAL_EXIT, 0
	call_if_eq VAR_TEMP_1, 0, SilphCo_7F_EventScript_RivalExitTop
	call_if_eq VAR_TEMP_1, 1, SilphCo_7F_EventScript_RivalExitBottom
	playse SE_WARP_IN
	fadedefaultbgm
	removeobject LOCALID_SILPH_CO_RIVAL
	waitse
	setvar VAR_MAP_SCENE_SILPH_CO_7F, 1
	releaseall
	end

SilphCo_7F_EventScript_RivalApproachTop::
	closemessage
	applymovement LOCALID_SILPH_CO_RIVAL, SilphCo_7F_Movement_RivalApproachTop
	waitmovement 0
	return

SilphCo_7F_EventScript_RivalApproachBottom::
	return

SilphCo_7F_EventScript_RivalSquirtle::
	trainerbattle_no_intro TRAINER_RIVAL_SILPH_SQUIRTLE, SilphCo_7F_Text_RivalDefeat
	return

SilphCo_7F_EventScript_RivalBulbasaur::
	trainerbattle_no_intro TRAINER_RIVAL_SILPH_BULBASAUR, SilphCo_7F_Text_RivalDefeat
	return

SilphCo_7F_EventScript_RivalCharmander::
	trainerbattle_no_intro TRAINER_RIVAL_SILPH_CHARMANDER, SilphCo_7F_Text_RivalDefeat
	return

SilphCo_7F_EventScript_RivalExitTop::
	applymovement LOCALID_SILPH_CO_RIVAL, SilphCo_7F_Movement_RivalExitTop
	waitmovement 0
	return

SilphCo_7F_EventScript_RivalExitBottom::
	applymovement LOCALID_SILPH_CO_RIVAL, SilphCo_7F_Movement_RivalExitBottom
	waitmovement 0
	return

SilphCo_7F_Movement_RivalApproachTop::
	walk_up
	step_end

SilphCo_7F_Movement_RivalExitTop::
	walk_right
	walk_right
	walk_right
	walk_up
	delay_16
	step_end

SilphCo_7F_Movement_RivalExitBottom::
	walk_left
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	delay_16
	step_end

SilphCo_7F_EventScript_LaprasGuy::
	lock
	faceplayer
	goto_if_set FLAG_GOT_LAPRAS_FROM_SILPH, SilphCo_7F_EventScript_AlreadyGotLapras
	msgbox SilphCo_7F_Text_HaveMonForSavingUs
	setvar VAR_TEMP_1, SPECIES_LAPRAS
	givemon SPECIES_LAPRAS, 25
	goto_if_eq VAR_RESULT, 0, SilphCo_7F_EventScript_ReceiveLaprasParty
	goto_if_eq VAR_RESULT, 1, SilphCo_7F_EventScript_ReceiveLaprasPC
	goto_if_eq VAR_RESULT, 2, Common_EventScript_NoMoreRoomForPokemon
	release
	end

SilphCo_7F_EventScript_ReceiveLaprasParty::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_LEVEL_UP
	message SilphCo_7F_Text_ObtainedLaprasFromEmployee
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_LAPRAS
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SilphCo_7F_EventScript_EndReceiveLapras
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto SilphCo_7F_EventScript_EndReceiveLapras
	end

SilphCo_7F_EventScript_ReceiveLaprasPC::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_LEVEL_UP
	message SilphCo_7F_Text_ObtainedLaprasFromEmployee
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_LAPRAS
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SilphCo_7F_EventScript_LaprasTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
	goto SilphCo_7F_EventScript_LaprasTransferredToPC
	end

SilphCo_7F_EventScript_LaprasTransferredToPC::
	call Common_EventScript_TransferredToPC
	goto SilphCo_7F_EventScript_EndReceiveLapras
	end

SilphCo_7F_EventScript_EndReceiveLapras::
	textcolor NPC_TEXT_COLOR_MALE
	msgbox SilphCo_7F_Text_ExplainLapras
	setflag FLAG_GOT_LAPRAS_FROM_SILPH
	release
	end

SilphCo_7F_EventScript_AlreadyGotLapras::
	msgbox SilphCo_7F_Text_RocketBossWentToBoardroom
	release
	end

SilphCo_7F_EventScript_WorkerM1::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_7F_EventScript_WorkerM1RocketsGone
	msgbox SilphCo_7F_Text_RocketsAfterMasterBall
	release
	end

SilphCo_7F_EventScript_WorkerM1RocketsGone::
	msgbox SilphCo_7F_Text_CanceledMasterBallProject
	release
	end

SilphCo_7F_EventScript_WorkerM2::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_7F_EventScript_WorkerM2RocketsGone
	msgbox SilphCo_7F_Text_BadIfTeamRocketTookOver
	release
	end

SilphCo_7F_EventScript_WorkerM2RocketsGone::
	msgbox SilphCo_7F_Text_WowYouChasedOffTeamRocket
	release
	end

SilphCo_7F_EventScript_WorkerF::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_7F_EventScript_WorkerFRocketsGone
	msgbox SilphCo_7F_Text_ReallyDangerousHere
	release
	end

SilphCo_7F_EventScript_WorkerFRocketsGone::
	msgbox SilphCo_7F_Text_ThankYouSoMuch
	release
	end

SilphCo_7F_EventScript_FloorSign::
	msgbox SilphCo_7F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_7F_EventScript_Joshua::
	trainerbattle_single TRAINER_SCIENTIST_JOSHUA, SilphCo_7F_Text_JoshuaIntro, SilphCo_7F_Text_JoshuaDefeat
	msgbox SilphCo_7F_Text_JoshuaPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_7F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_33, SilphCo_7F_Text_Grunt1Intro, SilphCo_7F_Text_Grunt1Defeat
	msgbox SilphCo_7F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_7F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_34, SilphCo_7F_Text_Grunt2Intro, SilphCo_7F_Text_Grunt2Defeat
	msgbox SilphCo_7F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_7F_EventScript_Grunt3::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_35, SilphCo_7F_Text_Grunt3Intro, SilphCo_7F_Text_Grunt3Defeat
	msgbox SilphCo_7F_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_7F_Text_HaveMonForSavingUs::
	.string "{JPN}あ… ひとだ!\n"
	.string "ロケットだん じゃ ないね\l"
	.string "きみ…\l"
	.string "たすけに きてくれたのか!\l"
	.string "おお ありがとう!\p"
	.string "そうだ!\n"
	.string "たすけに きて くれた きみに\l"
	.string "この ポケモンを わたそう!$"

SilphCo_7F_Text_ObtainedLaprasFromEmployee::
	.string "{JPN}{PLAYER}は しゃいん から\n"
	.string "ラプラスを もらった!$"

SilphCo_7F_Text_ExplainLapras::
	.string "{JPN}こいつは ラプラスと いって\n"
	.string "とても あたまのいい ポケモンだ!\p"
	.string "シルフの けんきゅうしつで\n"
	.string "かっていたが…\l"
	.string "ここに いる よりは\l"
	.string "ずっと いい はずだ!\p"
	.string "きみなら かわいがって くれそうだし\n"
	.string "ラプラスも よろこぶ だろう!\p"
	.string "およぎが とくい だから\n"
	.string "うえに のって\l"
	.string "うみを すすんでも いいぞ!$"

SilphCo_7F_Text_RocketBossWentToBoardroom::
	.string "{JPN}さっき ロケットだんの ボスが\n"
	.string "しゃちょうしつに あがってったが\l"
	.string "… しゃちょうが しんぱいだ!$"

SilphCo_7F_Text_RocketsAfterMasterBall::
	.string "{JPN}ロケットだんの ねらいは\n"
	.string "ポケモンが かならず とれる\l"
	.string "モンスターボール!\l"
	.string "… マスターボール だろう$"

SilphCo_7F_Text_CanceledMasterBallProject::
	.string "{JPN}じけんの せいで…\n"
	.string "マスターボールは\l"
	.string "はつばい ちゅうしに なったよ$"

SilphCo_7F_Text_BadIfTeamRocketTookOver::
	.string "{JPN}シルフも ポケモンも…\n"
	.string "ロケットだんの いいなりに\l"
	.string "なったら やだなあ$"

SilphCo_7F_Text_WowYouChasedOffTeamRocket::
	.string "{JPN}すごいなあ!\n"
	.string "ロケットだんの れんちゅうを\l"
	.string "おっぱらったんだって?$"

SilphCo_7F_Text_ReallyDangerousHere::
	.string "{JPN}あなた…!\l"
	.string "ここは あぶないわよ!\l"
	.string "たすけに きた?\l"
	.string "… だめよ$"

SilphCo_7F_Text_ThankYouSoMuch::
	.string "{JPN}たすかったわ ありがと!$"

SilphCo_7F_Text_Grunt3Intro::
	.string "{JPN}おっとー!\n"
	.string "こねずみを みつけたぞ!$"

SilphCo_7F_Text_Grunt3Defeat::
	.string "{JPN}がっくしだ!$"

SilphCo_7F_Text_Grunt3PostBattle::
	.string "{JPN}ちょろちょろ してる だけじゃ\n"
	.string "ボスには たどり つけないぜ$"

SilphCo_7F_Text_JoshuaIntro::
	.string "{JPN}ふひゃひゃ!\p"
	.string "シルフの しゃいんだと\n"
	.string "おもったかい?$"

SilphCo_7F_Text_JoshuaDefeat::
	.string "{JPN}もう ポケモン ない$"

SilphCo_7F_Text_JoshuaPostBattle::
	.string "{JPN}こども なのに…\n"
	.string "なかなかの つかいて だな$"

SilphCo_7F_Text_Grunt1Intro::
	.string "{JPN}おれこそは\n"
	.string "ロケット 4きょうだいの ひとり!$"

SilphCo_7F_Text_Grunt1Defeat::
	.string "{JPN}にいさん まけたよ!$"

SilphCo_7F_Text_Grunt1PostBattle::
	.string "{JPN}まあ いい\n"
	.string "あにきが かたきを とって くれる$"

SilphCo_7F_Text_Grunt2Intro::
	.string "{JPN}シルフに しんにゅうした こども\n"
	.string "… おまえの こと だな!$"

SilphCo_7F_Text_Grunt2Defeat::
	.string "{JPN}まけました!$"

SilphCo_7F_Text_Grunt2PostBattle::
	.string "{JPN}ボスが おこる まえに\n"
	.string "かえって ねた ほうが いいぞ$"

SilphCo_7F_Text_RivalWhatKeptYou::
	.string "{JPN}{RIVAL}『まってたぜ {PLAYER}!$"

SilphCo_7F_Text_RivalIntro::
	.string "{JPN}{RIVAL}『ひゃはは!\n"
	.string "ここで まってれば\l"
	.string "くるんじゃねえかと おもったのさ\p"
	.string "しかし ロケットだん あいてに\n"
	.string "てこずってる みたいだな!\l"
	.string "ま… おれさまには\l"
	.string "かんけいの ないこと だけど!\p"
	.string "ヤマブキ シティで {PLAYER}が\n"
	.string "あるいてんのを ちょこっと\l"
	.string "みかけた もの だからさ\p"
	.string "すこしは… つよくなったのか\n"
	.string "なあーんちゃって おもってさあ!$"

SilphCo_7F_Text_RivalDefeat::
	.string "{JPN}おう おうッ!\n"
	.string "ロケットだんの ボスに\l"
	.string "いどむだけの ことは あるじゃん!$"

SilphCo_7F_Text_RivalPostBattle::
	.string "{JPN}ほんじゃま {PLAYER}!\p"
	.string "おれさまは\n"
	.string "ひとあし さきに いくぜ!\p"
	.string "ポケモンずかん やってる うちに\n"
	.string "どういう ポケモンが つよい とか\l"
	.string "なにに しんか するか とか\l"
	.string "わかって きちゃったんだよねえ!\l"
	.string "おれって てんさい?\p"
	.string "ま… そういう ことで\n"
	.string "これからは\l"
	.string "ポケモン リーグの してんのうを\l"
	.string "ばっさ ばっさと たおして\p"
	.string "さいきょうの トレーナーに\n"
	.string "なってやろう と きめたのさ!\p"
	.string "{PLAYER}も\n"
	.string "せいぜい がんばって くれい\l"
	.string "じゃあな!\l"
	.string "バイビー!$"

SilphCo_7F_Text_FloorSign::
	.string "{JPN}シルフ カンパニー ほんしゃ\n"
	.string "7かい$"

