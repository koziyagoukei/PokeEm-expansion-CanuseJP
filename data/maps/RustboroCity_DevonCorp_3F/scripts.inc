RustboroCity_DevonCorp_3F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RustboroCity_DevonCorp_3F_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, RustboroCity_DevonCorp_3F_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, RustboroCity_DevonCorp_3F_OnFrame
	.byte 0

RustboroCity_DevonCorp_3F_OnTransition:
	call_if_eq VAR_DEVON_CORP_3F_STATE, 0, RustboroCity_DevonCorp_3F_EventScript_SetEmployeePos
	end

RustboroCity_DevonCorp_3F_EventScript_SetEmployeePos::
	setobjectxyperm LOCALID_DEVON_CORP_3F_EMPLOYEE, 3, 2
	setobjectmovementtype LOCALID_DEVON_CORP_3F_EMPLOYEE, MOVEMENT_TYPE_FACE_LEFT
	return

RustboroCity_DevonCorp_3F_OnWarp:
	map_script_2 VAR_DEVON_CORP_3F_STATE, 0, RustboroCity_DevonCorp_3F_EventScript_PlayerFaceEast
	.2byte 0

RustboroCity_DevonCorp_3F_EventScript_PlayerFaceEast::
	turnobject LOCALID_PLAYER, DIR_EAST
	end

RustboroCity_DevonCorp_3F_OnFrame:
	map_script_2 VAR_DEVON_CORP_3F_STATE, 0, RustboroCity_DevonCorp_3F_EventScript_MeetPresident
	.2byte 0

RustboroCity_DevonCorp_3F_EventScript_MeetPresident::
	lockall
	msgbox RustboroCity_DevonCorp_3F_Text_ThisIs3rdFloorWaitHere, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DEVON_CORP_3F_EMPLOYEE, RustboroCity_DevonCorp_3F_Movement_EmployeeWalkOffscreen
	waitmovement 0
	delay 80
	applymovement LOCALID_DEVON_CORP_3F_EMPLOYEE, RustboroCity_DevonCorp_3F_Movement_EmployeeReturnToPlayer
	waitmovement 0
	msgbox RustboroCity_DevonCorp_3F_Text_WordWithPresidentComeWithMe, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_FOLLOW_ME, FALSE
	applymovement LOCALID_DEVON_CORP_3F_EMPLOYEE, RustboroCity_DevonCorp_3F_Movement_LeadPlayerToPresident
	applymovement LOCALID_PLAYER, RustboroCity_DevonCorp_3F_Movement_PlayerFollowToPresident
	waitmovement 0
	msgbox RustboroCity_DevonCorp_3F_Text_PleaseGoAhead, MSGBOX_DEFAULT
	closemessage
	fadedefaultbgm
	applymovement LOCALID_DEVON_CORP_3F_EMPLOYEE, RustboroCity_DevonCorp_3F_Movement_EmployeeFaceDesk
	applymovement LOCALID_PLAYER, RustboroCity_DevonCorp_3F_Movement_PlayerApproachDesk
	waitmovement 0
	msgbox RustboroCity_DevonCorp_3F_Text_MrStoneIHaveFavor, MSGBOX_DEFAULT
	giveitem ITEM_LETTER
	msgbox RustboroCity_DevonCorp_3F_Text_MrStoneWantYouToHaveThis, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message RustboroCity_DevonCorp_3F_Text_ReceivedPokenav
	waitfanfare
	setflag FLAG_SYS_POKENAV_GET
	setflag FLAG_RECEIVED_POKENAV
	setflag FLAG_HIDE_RUSTBORO_CITY_POKEMON_SCHOOL_SCOTT
	msgbox RustboroCity_DevonCorp_3F_Text_MrStoneExplainPokenavRestUp, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox RustboroCity_DevonCorp_3F_Text_MrStoneGoWithCautionAndCare, MSGBOX_DEFAULT
	setflag FLAG_HIDE_ROUTE_116_WANDAS_BOYFRIEND
	clearflag FLAG_HIDE_RUSTURF_TUNNEL_WANDAS_BOYFRIEND
	clearflag FLAG_HIDE_RUSTURF_TUNNEL_WANDA
	clearflag FLAG_HIDE_BRINEYS_HOUSE_MR_BRINEY
	clearflag FLAG_HIDE_BRINEYS_HOUSE_PEEKO
	setvar VAR_BRINEY_LOCATION, 1
	clearflag FLAG_HIDE_RUSTBORO_CITY_RIVAL
	setvar VAR_DEVON_CORP_3F_STATE, 1
	setvar VAR_RUSTBORO_CITY_STATE, 6
	releaseall
	end

RustboroCity_DevonCorp_3F_Movement_Unused:
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_left
	step_end

RustboroCity_DevonCorp_3F_Movement_LeadPlayerToPresident:
	walk_right
	walk_right
	walk_right
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_left
	step_end

RustboroCity_DevonCorp_3F_Movement_EmployeeFaceDesk:
	delay_16
	walk_in_place_faster_down
	step_end

RustboroCity_DevonCorp_3F_Movement_EmployeeWalkOffscreen:
	walk_right
	walk_right
	walk_right
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

RustboroCity_DevonCorp_3F_Movement_EmployeeReturnToPlayer:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_left
	walk_left
	walk_left
	step_end

RustboroCity_DevonCorp_3F_Movement_PlayerFollowToPresident:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

RustboroCity_DevonCorp_3F_Movement_PlayerApproachDesk:
	delay_16
	delay_16
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

RustboroCity_DevonCorp_3F_EventScript_MrStone::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_EXP_SHARE, RustboroCity_DevonCorp_3F_EventScript_MrStoneAfterFavor
	goto_if_set FLAG_DELIVERED_STEVEN_LETTER, RustboroCity_DevonCorp_3F_EventScript_GiveExpShare
	msgbox RustboroCity_DevonCorp_3F_Text_CountingOnYou, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

RustboroCity_DevonCorp_3F_EventScript_GiveExpShare::
	msgbox RustboroCity_DevonCorp_3F_Text_ThankYouForDeliveringLetter, MSGBOX_DEFAULT
	giveitem ITEM_EXP_SHARE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_EXP_SHARE
	msgbox RustboroCity_DevonCorp_3F_Text_ExplainExpShare, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

RustboroCity_DevonCorp_3F_EventScript_MrStoneAfterFavor::
	msgbox RustboroCity_DevonCorp_3F_Text_NotFamiliarWithTrends, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

RustboroCity_DevonCorp_3F_EventScript_Employee::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_REPEAT_BALL, RustboroCity_DevonCorp_3F_EventScript_EmployeeBalls
	msgbox RustboroCity_DevonCorp_3F_Text_VisitCaptSternShipyard, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_3F_EventScript_EmployeeBalls::
	msgbox RustboroCity_DevonCorp_3F_Text_RepeatAndTimerHugelyPopular, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_3F_EventScript_RareRocksDisplay::
	msgbox RustboroCity_DevonCorp_3F_Text_RareRocksDisplay, MSGBOX_SIGN
	end

RustboroCity_DevonCorp_3F_Text_MrStoneIHaveFavor:
	.string "{JPN}わしが デボン コーポレーション\nしゃちょうの ツワブキ だ!\pきみの ことは さっき きいたよ!\pなんでも うちの けんきゅういんを\n2ども たすけてくれた とか\p……で そんな すごい きみに\nたのみごとを したいのだ!\pきみは にもつを とどけに\nカイナの ぞうせんじょに いくんだろう?\pその とちゅうに\nムロタウンにも よって ほしいのだよ!\pいやー ムロタウンに\nダイゴという おとこが いるはずでね\lちょいと てがみを とどけてほしいのだ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_DevonCorp_3F_Text_MrStoneWantYouToHaveThis:
	.string "{JPN}ツワブキ『もちろん わしは\nすごーい しゃちょうで あるからな!\pただで おねがいすると いうような\nケチな まねは しないのだ!\pきみに こいつを プレゼントしよう!"
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_DevonCorp_3F_Text_ReceivedPokenav:
	.string "{JPN}{PLAYER}は\nポケナビを てにいれた!"
	.string "$"

RustboroCity_DevonCorp_3F_Text_MrStoneExplainPokenavRestUp:
	.string "{JPN}ツワブキ『そいつは……\pポケモン ナビゲーター!\nりゃくして ポケナビ といってな\pぼうけんを する トレーナーの\nひつじゅひん なのだよ!\pそいつで ホウエンマップを みれば\nムロや カイナの ばしょ だって\lばっちり わかると いうものだ!\p……さいきんは アクアだの マグマだの\nといった あやしげな れんちゅうが\lあちこちで わるさを してるらしい\pまずは ひとやすみ していきなさい!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_DevonCorp_3F_Text_MrStoneGoWithCautionAndCare:
	.string "{JPN}ツワブキ『それでは {PLAYER}{KUN}\nじゅうぶん きをつけて いくのだよ!"
	.string "$"

RustboroCity_DevonCorp_3F_Text_CountingOnYou:
	.string "{JPN}ツワブキ『よろしく たのむよ!$"

RustboroCity_DevonCorp_3F_Text_ThankYouForDeliveringLetter:
	.string "{JPN}ツワブキ『てがみを とどけて くれたんだね\nいやいや ありがとう!\pこいつは おれい だよ\nトレーナーの きみに やくだつ はずだ!"
	.string ""
	.string ""
	.string "$"

RustboroCity_DevonCorp_3F_Text_ExplainExpShare:
	.string "{JPN}ツワブキ『がくしゅうそうちを もたせた\nポケモンは しょうぶを しなくても\lけいけんちを わけて もらえる!\pよわい ポケモンを そだてるのに\nなかなか べんりな どうぐ なのだよ!"
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_DevonCorp_3F_Text_NotFamiliarWithTrends:
	.string "{JPN}ツワブキ『わたしは わかいころ から\nしごと ばかり だったから\lいまいち わからんのだが……\pわかものは やはり トレーナーとして\nポケモンリーグを めざす もの なのかね"
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_DevonCorp_3F_Text_ThisIs3rdFloorWaitHere:
	.string "{JPN}ここが デボン コーポレーション 3かい\nしゃちょうの おへや なのよね!\pいやー! きみには\nかんしゃ かんげき あめ あられ なのです\pところで…… きみが\nとりもどしてくれた にもつ\pそいつを カイナシティの ぞうせんじょ\nつまり ふねの こうじょうへ\lとどけて ほしいのです!\pまた おかしな やつらに\nうばわれては たまりません からね\pえへへ……\nよろしく おねがい しますよ\pちょっと まっていて くださいね"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_DevonCorp_3F_Text_WordWithPresidentComeWithMe:
	.string "{JPN}しゃちょうが ぜひ\nおはなし したいそうです\pわたしに ついて きてください"
	.string ""
	.string "$"

RustboroCity_DevonCorp_3F_Text_PleaseGoAhead:
	.string "{JPN}ささ どうぞ!$"

RustboroCity_DevonCorp_3F_Text_VisitCaptSternShipyard:
	.string "{JPN}カイナの ぞうせんじょ には\nクスノキさん という ひとが いるから\lかれを たずねていくと いいですよ!"
	.string ""
	.string "$"

RustboroCity_DevonCorp_3F_Text_RepeatAndTimerHugelyPopular:
	.string "{JPN}デボンの しんせいひん\nリピートボールと タイマーボール\lトレーナーに だいにんき なのよね!"
	.string ""
	.string "$"

RustboroCity_DevonCorp_3F_Text_RareRocksDisplay:
	.string "{JPN}しゃちょうの あつめた\nめずらしい いしが かざってある!"
	.string "$"


