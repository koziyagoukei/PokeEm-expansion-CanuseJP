BattleFrontier_BattleTowerMultiPartnerRoom_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, BattleFrontier_BattleTowerMultiPartnerRoom_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_BattleTowerMultiPartnerRoom_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, BattleFrontier_BattleTowerMultiPartnerRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_BattleTowerMultiPartnerRoom_OnFrame
	.byte 0

BattleFrontier_BattleTowerMultiPartnerRoom_OnResume:
	pyramid_resetparty
	end

BattleFrontier_BattleTowerMultiPartnerRoom_OnTransition:
	goto_if_set FLAG_CHOSEN_MULTI_BATTLE_NPC_PARTNER, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_ChosePartner
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_1
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_2
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_3
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_4
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_5
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_6
	setflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_ALT_1
	setflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_ALT_2
	tower_loadpartners
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_ChosePartner::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_1, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_2, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_3, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_4, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_5, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_6, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_7, OBJ_EVENT_GFX_BOY_1
	setvar VAR_TEMP_1, 1
	setvar VAR_TEMP_3, 1
	setobjectxyperm LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, 10, 2
	end

BattleFrontier_BattleTowerMultiPartnerRoom_OnWarp:
	map_script_2 VAR_TEMP_3, 1, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TurnPlayerNorth
	.2byte 0

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

BattleFrontier_BattleTowerMultiPartnerRoom_OnFrame:
	map_script_2 VAR_TEMP_1, 0, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_EnterRoom
	map_script_2 VAR_TEMP_3, 1, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_ExitRoom
	.2byte 0

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_ExitRoom::
	lockall
	setvar VAR_TEMP_3, 0
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_EnterElevator
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_EnterRoom::
	lockall
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterRoom
	waitmovement 0
	applymovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantBlockExit
	waitmovement 0
	copyobjectxytoperm LOCALID_TOWER_PARTNER_ROOM_ATTENDANT
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox BattleFrontier_BattleTowerMultiPartnerRoom_Text_PleaseFindPartner, MSGBOX_DEFAULT
	special HealPlayerParty
	setvar VAR_TEMP_1, 1
	releaseall
	end

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterRoom:
	walk_down
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantBlockExit:
	walk_left
	walk_in_place_faster_down
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Attendant::
	lock
	faceplayer
	message BattleFrontier_BattleTowerMultiPartnerRoom_Text_QuitLookingForPartner
	waitmessage
	multichoicedefault 20, 8, MULTI_YESNO, 1, FALSE
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_QuitChallenge
	msgbox BattleFrontier_BattleTowerMultiPartnerRoom_Text_PleaseFindPartner2, MSGBOX_DEFAULT
	release
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_QuitChallenge::
	frontier_set FRONTIER_DATA_RECORD_DISABLED, TRUE
	goto BattleFrontier_BattleTower_EventScript_WarpToLobbyLost
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_EnterElevator::
	msgbox BattleFrontier_BattleTowerMultiPartnerRoom_Text_ThankYouForChoosingPartner, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	opendoor 10, 1
	waitdooranim
	call BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_MoveToElevator
	closedoor 10, 1
	waitdooranim
	warp MAP_BATTLE_FRONTIER_BATTLE_TOWER_ELEVATOR, 1, 6
	waitstate
	releaseall
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_MoveToElevator::
	applymovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantEnterElevator
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevator
	waitmovement 0
	return

@ Unused
BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_MoveToElevatorEast:
	applymovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantEnterElevator
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevatorEast
	waitmovement 0
	return

@ Unused
BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_MoveToElevatorWest:
	applymovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantEnterElevator
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevatorWest
	waitmovement 0
	return

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevator:
	walk_up
	walk_up
	set_invisible
	step_end

@ Functionally unused
BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevatorEast:
	walk_right
	walk_up
	set_invisible
	step_end

@ Functionally unused
BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevatorWest:
	walk_left
	walk_up
	set_invisible
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantEnterElevator:
	walk_up
	set_invisible
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner1::
	lock
	faceplayer
	setvar VAR_TEMP_2, 0
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner::
	goto_if_set FLAG_CHOSEN_MULTI_BATTLE_NPC_PARTNER, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_RejectPartner
	tower_dopartnermsg PARTNER_MSGID_INTRO
	waitmessage
	waitbuttonpress
	tower_dopartnermsg PARTNER_MSGID_MON1
	waitmessage
	waitbuttonpress
	tower_dopartnermsg PARTNER_MSGID_MON2_ASK
	waitmessage
	multichoicedefault 20, 8, MULTI_YESNO, 1, FALSE
	switch VAR_RESULT
	case 1, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_RejectPartner
	case MULTI_B_PRESSED, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_RejectPartner
	tower_dopartnermsg PARTNER_MSGID_ACCEPT
	waitmessage
	waitbuttonpress
	closemessage
	call_if_ne VAR_FACING, DIR_SOUTH, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_PartnerExit
	call_if_eq VAR_FACING, DIR_SOUTH, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_PartnerExitSouth
	removeobject VAR_LAST_TALKED
	setflag FLAG_CHOSEN_MULTI_BATTLE_NPC_PARTNER
	warpsilent MAP_BATTLE_FRONTIER_BATTLE_TOWER_MULTI_PARTNER_ROOM, 10, 3
	waitstate
	release
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_RejectPartner::
	tower_dopartnermsg PARTNER_MSGID_REJECT
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_PartnerExit::
	applymovement VAR_LAST_TALKED, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PartnerExit
	waitmovement 0
	return

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_PartnerExitSouth::
	applymovement VAR_LAST_TALKED, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PartnerExitSouth
	waitmovement 0
	return

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PartnerExit:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PartnerExitSouth:
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner2::
	lock
	faceplayer
	setvar VAR_TEMP_2, 1
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner3::
	lock
	faceplayer
	setvar VAR_TEMP_2, 2
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner4::
	lock
	faceplayer
	setvar VAR_TEMP_2, 3
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner5::
	lock
	faceplayer
	setvar VAR_TEMP_2, 4
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner6::
	lock
	faceplayer
	setvar VAR_TEMP_2, 5
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner7::
	lock
	faceplayer
	setvar VAR_TEMP_2, 6
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner8::
	lock
	faceplayer
	setvar VAR_TEMP_2, 7
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PleaseFindPartner:
	.string "{JPN}ここに あつまった トレーナーから\n"
	.string "パートナーを さがしてください$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_QuitLookingForPartner:
	.string "{JPN}{PLAYER}さんは タッグの\n"
	.string "パートナーを まだ みつけていません\p"
	.string "パートナーさがしを やめて\n"
	.string "うけつけへ もどりますか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PleaseFindPartner2:
	.string "{JPN}それでは ここに あつまった\n"
	.string "トレーナーから パートナーを さがしてください$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ThankYouForChoosingPartner::
	.string "{JPN}パートナーを えらんでいただき\n"
	.string "ありがとうございます\p"
	.string "マルチバトルルームへ\n"
	.string "ごあんないします$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Intro::
	.string "{JPN}ぼくは {STR_VAR_1}さんの {STR_VAR_2}ばんでし\n"
	.string "{STR_VAR_3}です!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Mon1::
	.string "{JPN}{STR_VAR_1}さんの アドバイスで\n"
	.string "{STR_VAR_2}を もった {STR_VAR_3}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もった {STR_VAR_2}を\nつれてきました\p"
	.string "どうか タッグを くんでください!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Accept::
	.string "{JPN}ありがとうございます!\n"
	.string "さっそく とうろくしてきます!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Reject::
	.string "{JPN}{PLAYER}さんと タッグを\n"
	.string "くみたかったです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Intro::
	.string "{JPN}わたしは {STR_VAR_1}さんの {STR_VAR_2}ばんでし\n"
	.string "{STR_VAR_3}と いいます$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Mon1::
	.string "{JPN}{STR_VAR_1}さんに そうだんして\n"
	.string "{STR_VAR_2}を もった {STR_VAR_3}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もった {STR_VAR_2}を\nえらびました\p"
	.string "おねがいします! タッグを くみましょう!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Accept::
	.string "{JPN}どうも ありがとうございます!\n"
	.string "すぐに とうろくを すませてきます!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Reject::
	.string "{JPN}{PLAYER}さんと タッグを\n"
	.string "くめると おもったのに……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Intro::
	.string "{JPN}ぼくは {STR_VAR_3} って いって\n{STR_VAR_1}さんの {STR_VAR_2}ばん でしです\lグスっ……\pごめんなさい!\nきんちょう して なみだが……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Mon1::
	.string "{JPN}ぼく {STR_VAR_3}さんの アドバイスで\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nつれているんです\pグスン…… ぼ ぼくと\nタッグを くんで ください!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Accept::
	.string "{JPN}えー ほんとですか!\nやった! うれしいよう! うぉぉーん!\pあ…… ごめん なさい\nうれしくて なみだが……\pさっ さっそく うけつけに\nいってきますから まってて ください!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Reject::
	.string "{JPN}そ そんな……\nう うう うわーん!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Intro::
	.string "{JPN}こんにちはっ ボクは {STR_VAR_3}!\n{STR_VAR_1}さんの {STR_VAR_2}ばん でしなんだ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Mon1::
	.string "{JPN}{STR_VAR_3}さんに すすめて もらった\n{STR_VAR_1}を もつ {STR_VAR_2}とね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}が\nいるんだよ いいでしょ?\lねーっ ボクと いっしょに\lタッグを くもうよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Accept::
	.string "{JPN}わーいっ やった!\nじゃあねー ボクが\lうけつけに いってくるからね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Reject::
	.string "{JPN}えーっ どうして?\n{PLAYER}さんと くみたかったなー$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Intro::
	.string "{JPN}ぼくは あの ゆうめいな\n{STR_VAR_1}さんの {STR_VAR_2}ばんの でし\l{STR_VAR_3}です!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Mon1::
	.string "{JPN}{STR_VAR_3}さん から\nアドバイスを いただいて\l{STR_VAR_1}を つかう {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Mon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}を\nつれているんです\pおねがいです!\nぼくと タッグを くんでください!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Accept::
	.string "{JPN}ありがとうございます!\nさっそく うけつけを すませてきますね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Reject::
	.string "{JPN}{PLAYER}さんと\nぜひ タッグを くみたかったです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Intro::
	.string "{JPN}あの…… あたし なんかに\nこえを かけてくれて うれしいです\lあたしは {STR_VAR_3}です\l{STR_VAR_1}さんの {STR_VAR_2}ばんでしで……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Mon1::
	.string "{JPN}{STR_VAR_3}さんが あたし なんかにも\nアドバイスを くださったので\l{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nつれています…… おそれ おおいけど……\pムリ だと おもいます けど……\nあたしと タッグを くんでいただけますか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Accept::
	.string "{JPN}ほんとう ですか? しんじられない!\nあたし なんかと くんでもらえる なんて!\lが……がんばります!\pあのっ あたしで よかったら\nいまから うけつけに いって きます!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Reject::
	.string "{JPN}や やっぱり……\nあたし なんか ダメですよね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Intro::
	.string "{JPN}わたしは {STR_VAR_3}\n{STR_VAR_1}さんの {STR_VAR_2}ばん でしなの!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Mon1::
	.string "{JPN}{STR_VAR_3}さんが いいって いったから\n{STR_VAR_1} つかいの {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Mon2Ask::
	.string "{JPN}{STR_VAR_1} つかいの {STR_VAR_2}を\nもって いるわけ!\lわるくない でしょ?\lあなたと タッグを くんで あげようか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Accept::
	.string "{JPN}うん じゃあ よろしくね\nわたしの あしを ひっぱらないでよね!\lさっそく うけつけに いってくるわ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Reject::
	.string "{JPN}え? なんで ことわるの?\nみるめ ないんじゃない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Intro::
	.string "{JPN}オレは トライアスリートの {STR_VAR_3}!\nジョギング トレーニング ラッピングと\lまいにち いそがしい なか\l{STR_VAR_1}の {STR_VAR_2}ばんでし やってる!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Mon1::
	.string "{JPN}{STR_VAR_3}の オススメ!\n{STR_VAR_1}を もつ {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nいそがしい なか がんばって そだてたぜ!\pオレと タッグを くんで がんばろうぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Accept::
	.string "{JPN}サンキュー そう こなくっちゃ!\nんじゃ オレ うけつけを すませてくるわ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Reject::
	.string "{JPN}おいおい いそがしい なか きたのに\nそりゃ ないぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Intro::
	.string "{JPN}ハーイ! ちょうしは どう?\n{STR_VAR_1}さんの {STR_VAR_2}ばんの\lでしトレーナー {STR_VAR_3}よ よろしくぅ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Mon1::
	.string "{JPN}きいてきいて! {STR_VAR_3}さんが\nおすすめ してくれた ポケモンはね\l{STR_VAR_1}を もつ {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}なの\nもちろん バッチリ そだてたわ!\pというわけで わたしと くまない?\nきっと いいカンジよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Accept::
	.string "{JPN}わーい たのしく なりそうー!\nじゃ わたしが うけつけに いってくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Reject::
	.string "{JPN}もー いじわる!\nわたし オススメなのに!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Intro::
	.string "{JPN}{STR_VAR_3}と もうします\nわたくし {STR_VAR_1}さまの {STR_VAR_2}ばんの\lでしで ございますの……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Mon1::
	.string "{JPN}{STR_VAR_3}さまに おしえを こい\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nそだて あげましたの!\pおねがい わたくしと タッグを\nくんで くださると おっしゃって!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Accept::
	.string "{JPN}うっ うれしい ですわ……\nわたくし ぜんりょくで つとめますわ!\pもちろん うけつけは わたくしが!\nすこし おまちくださいませ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Reject::
	.string "{JPN}{PLAYER}さんと\nぜひ タッグを くみたかったです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Intro::
	.string "{JPN}キャー! はなしかけられたっ!\nかっ かんげきっ……!\pわたし! {STR_VAR_3}と いいますっ!\n{STR_VAR_1} せんせいの\l{STR_VAR_2}ばん でし ですっ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Mon1::
	.string "{JPN}{STR_VAR_3} せんせいの アドバイスで\n{STR_VAR_1}をもつ {STR_VAR_2}ちゃんと$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Mon2Ask::
	.string "{JPN}{STR_VAR_1}をもつ {STR_VAR_2}ちゃんを\nそだてましたーっ!\pあの おねがいが!\nわたしと タッグをくんで ほしいのですっ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Accept::
	.string "{JPN}キャー! かっ かんげきっ……!\nおありがとう ございますう!\lではっ うけつけを して まいりますっ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Reject::
	.string "{JPN}うっわあああ!\nちも なみだも ないですねっ! でもステキ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Intro::
	.string "{JPN}いえーい! {STR_VAR_1}さんの\n{STR_VAR_2}ばん でしの おじさんは\l{STR_VAR_3}です ハロウ ハロウ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Mon1::
	.string "{JPN}{STR_VAR_3} ティーチャー おすすめの\n{STR_VAR_1} マスター {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Mon2Ask::
	.string "{JPN}{STR_VAR_1} マスター {STR_VAR_2}を\nトレイン したんだよー!\lグッドタイミングだね!\lもちろん おじさんと タッグを くむだろ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Accept::
	.string "{JPN}オーケイ エクセレント!\nじゃ バーっと うけつけを すませてくるね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Reject::
	.string "{JPN}ぐわーっ!\n{PLAYER}さん ってのは クールだね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Intro::
	.string "{JPN}お おいっ たいへんだ!\nバトルタワー きょうで おしまいらしいぞ!\p…… …… ……\nなんてね うっそ うそ!\pおいらは {STR_VAR_3} って いうんだ\n{STR_VAR_1}の 1000ばんめの でしだ!\lうそ ほんとは {STR_VAR_2}ばん!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Mon1::
	.string "{JPN}{STR_VAR_3}が アドバイスを くれてね\n{STR_VAR_1}を もつ {STR_VAR_2}とね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}をね\n10ぴきずつ もって いまーす!\lうそ ほんとは 1ぴきずつだよ!\pひとつ どうだろ? こんな おいらと\nタッグバトルを して みないかな?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Accept::
	.string "{JPN}やったー!\nおれいに ポケモン あげちゃおう!\pなんて じょうだん だけど……\nとりあえず うけつけを すませてくるよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Reject::
	.string "{JPN}そんな つめたい こと いうと\nバトルタワーを こわして しまうぞ!\pなんてね うそ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Intro::
	.string "{JPN}ご・き・げ・ん・よ・う!\nはーい ごきげんよう!\pわしは ノリノリ ふなのり {STR_VAR_3}!\n{STR_VAR_1}さんの {STR_VAR_2}ばんの でしだよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Mon1::
	.string "{JPN}{STR_VAR_3}さん アドバイスの たまもの\n{STR_VAR_1}を もつ {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}が\nわしの じまんの ポケモン!\pさあ この であいを きねんして\nわしと タッグバトルを やってください!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Accept::
	.string "{JPN}ようよう! ノリが いいね\nよろしく たのみますよ!\lじゃ うけつけは わしが いってくるよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Reject::
	.string "{JPN}そんな {PLAYER}さん!\nノリが わるいじゃ ないか!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Intro::
	.string "{JPN}イャァアッホオオォォウウ!\nあ にげないでェ かえらないでェ!\pオスっ オレっ {STR_VAR_3}!\n{STR_VAR_1}さんの {STR_VAR_2}ばん でしなのさ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Mon1::
	.string "{JPN}{STR_VAR_3}さんったら オレに!\n{STR_VAR_1}を もつ {STR_VAR_2}と!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を!\nつかえって いうから そうしたぜエェ!\pって ことで どうする オレ?\nさっそく たのんでみる?\pおねがいです!\nオレと タッグを くんでエェ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Accept::
	.string "{JPN}やったラッキー うれしーよ オレ!!\nさっそく うけつけに いってくる!?\lじゃ ちょっと まっててェ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Reject::
	.string "{JPN}って ことわられてる オレっ!?\n{PLAYER}さん つれないよォ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Intro::
	.string "{JPN}ゴホっ!\pあっ ゴメン カゼ ひいてて……\nポケモンは げんきだから ダイジョブだよ\pおれは {STR_VAR_3}\n{STR_VAR_1}さんの {STR_VAR_2}ばんでしだよ\l……ゴホっ ゴホっ よろしく!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Mon1::
	.string "{JPN}{STR_VAR_3}さんの アドバイスで\n{STR_VAR_1}を もつ {STR_VAR_2}とさ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nあめのひも かぜのひも そだてあげたよ\l……それで カゼを ひいたんだ きっと\pまあ そういう わけだからさ\nこんな おれと いっしょに くまないかい?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Accept::
	.string "{JPN}ありがとう わるいね\n……ゴホ ゴホ!\lうけつけを すませてくるから……\lゴホ まっててね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Reject::
	.string "{JPN}えー なんで!\n{PLAYER}さんと オレって いいけどなあ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Intro::
	.string "{JPN}あっ ど どうも!\nてれるなあ\pあの {STR_VAR_3}です! {STR_VAR_1}さんの\n{STR_VAR_2}ばんの でし やってます……!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Mon1::
	.string "{JPN}{STR_VAR_3}さんが よいと いったので\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nそだててるんだ…… はずかしながら\pその はずかしい けど ワシと……\nタッグを くんで もらえる かな……?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Accept::
	.string "{JPN}あ ありがとう!\nはずかしい けど がんばって みるよ\pじゃ…… う うけつけ してくるね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Reject::
	.string "{JPN}こ ことわらないでくれ!\nワシに はじを かかせないで くれ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Intro::
	.string "{JPN}わたしは {STR_VAR_3}だ\nうそじゃない!\l{STR_VAR_1}さんの {STR_VAR_2}ばんの でしだ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Mon1::
	.string "{JPN}{STR_VAR_3}さんから すすめられて\nどうやら ほんき みたい だったから\l{STR_VAR_1}を もつ {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Mon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nつれているんだ ほんとうだよ!\pところで もし よかったら わたしを\nキミの パートナーに みとめて ほしい$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Accept::
	.string "{JPN}ほんとうに みとめて くれるのかね?\nうそだったら いやすぎる からな\pいや ほんとうなら いいんだ すまなかった\nさっそく うけつけを すませてくる$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Reject::
	.string "{JPN}え? なぜ ことわるのかね?\nわるい じょうだん では ないのかね?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassIntro::
	.string "{JPN}わたし\nミニスカート {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassMon1::
	.string "{JPN}わたしはね……\n{STR_VAR_1}を つかう {STR_VAR_2}とね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}を\nつれて あるいているの!\pどう?\nパートナーに なってくれない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassAccept::
	.string "{JPN}ありがとうー!\nいまから とうろく してくるわね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassReject::
	.string "{JPN}わたしの パートナーに ならないなんて\nきっと こうかい するから!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterIntro::
	.string "{JPN}あのね!\pぼくは\nたんパンこぞう {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterMon1::
	.string "{JPN}ぼくはね……\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}を\nもっているよ!\pねぇねぇ\nいっしょに タッグを くもうよ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterAccept::
	.string "{JPN}やったー!\nうけつけで とうろく してくるねー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterReject::
	.string "{JPN}ぼくの ポケモンと タッグを くめば\nきっと つよいのにー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerIntro::
	.string "{JPN}ヤッホー!\pわし\nやまおとこ {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerMon1::
	.string "{JPN}わし\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nつれて あるいています!\pどうですかな?\nいっしょに タッグを くみませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerAccept::
	.string "{JPN}ヤッホー!\pいまから うけつけで\nとうろくを すませてきますよ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerReject::
	.string "{JPN}きみと いっしょに\nバトルを したかったです$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyIntro::
	.string "{JPN}こんにちは!\pわたし\nおとなのおねえさん {STR_VAR_1}よ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyMon1::
	.string "{JPN}わたし\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}を\nそだてているのよ!\pどう?\nいっしょに タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyAccept::
	.string "{JPN}うれしい!\pすぐに とうろくを すませるわね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyReject::
	.string "{JPN}ざんねんね!\pあなたと わたしが\nくんだら さいこう だったのに……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanIntro::
	.string "{JPN}よー!\pわし\nつりびと {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanMon1::
	.string "{JPN}わしは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}を\nつれているぞ\pどうだね?\nいっしょに たたかうかね?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanAccept::
	.string "{JPN}よしよし!\nまかせとけー\pいまから とうろくを するからな!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanReject::
	.string "{JPN}きみと わしの あいしょうは\nバッチリ だったのにな……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyIntro::
	.string "{JPN}わたくし\nおじょうさま {STR_VAR_1}と\lもうします!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyMon1::
	.string "{JPN}わたくし\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}を\nつれていますの\pどうですか?\pわたくしと いっしょに\nタッグを くんで いただけませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyAccept::
	.string "{JPN}ありがとうございます!\pでは わたくしは \nとうろくを すませてきますわ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyReject::
	.string "{JPN}わたくしの パートナーに ならないと\nきっと こうかい しますわよ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFIntro::
	.string "{JPN}わたし\nトライアスリート {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFMon1::
	.string "{JPN}わたしはね……\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nもっているの!\pおねがい!\nいっしょに タッグを くんでよ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFAccept::
	.string "{JPN}ありがとうー!\nうけつけで とうろく してくるー$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFReject::
	.string "{JPN}わたしと タッグを くんだら\nきっと つよいのにー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherIntro::
	.string "{JPN}やあ!\pぼく\nむしとりしょうねん {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherMon1::
	.string "{JPN}ぼく\n{STR_VAR_1}を おぼえた {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherMon2Ask::
	.string "{JPN}{STR_VAR_1}を おぼえた {STR_VAR_2}を\nもっているよ!\pねー?\nぼくと いっしょに タッグを くまない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherAccept::
	.string "{JPN}やったね!\pじゃ いまから うけつけで\nとうろくを すませてくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherReject::
	.string "{JPN}ぼくの ポケモン さいこう なのに!\nこうかい しないでよ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMIntro::
	.string "{JPN}こんにちは!\pぼくは\nじゅくがえり {STR_VAR_1} です$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMMon1::
	.string "{JPN}ぼくは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}を\nそだてています!\pどうです?\pぼくと\nいっしょに タッグを くみませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMAccept::
	.string "{JPN}ありがとうございます!\pでは\nうけつけで とうろくを すませてきますね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMReject::
	.string "{JPN}ざんねんです……\pきみと いっしょに タッグを くんで\nべんきょう したかったです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyIntro::
	.string "{JPN}よう!\pオレ\nおぼっちゃま {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyMon1::
	.string "{JPN}オレ\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nもっているんだぜ!\pオレ いっしょに\nタッグを くんでもいいぜ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyAccept::
	.string "{JPN}よし!\pじゃあ いまから\nうけつけで とうろくを すませてくるぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyReject::
	.string "{JPN}オレと タッグを くまない なんて\nきっと こうかい するぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltIntro::
	.string "{JPN}オッス!\pカラテおう {STR_VAR_1}\nと もうします$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltMon1::
	.string "{JPN}いま わたくし\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nつれております!\pおねがいします!\pわたくしと\nタッグを くんで くれませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltAccept::
	.string "{JPN}オッス!\pさっそく\nうけつけに とうろく してまいります!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltReject::
	.string "{JPN}オッス……\pつぎの きかいは\nぜひ タッグを くんでください……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFIntro::
	.string "{JPN}こんにちは!\pわたし\nうきわガール {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFMon1::
	.string "{JPN}わたし\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nつれているの!\pねぇ?\pいっしょに\nタッグを くんでよー?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFAccept::
	.string "{JPN}ありがとうー!\pじゃあ いまから\nうけつけで とうろくを すませてくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFReject::
	.string "{JPN}わたしと タッグを くんだら\nとっても つよかったのにー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacIntro::
	.string "{JPN}こんにちは……\pオカルトマニア {STR_VAR_1}と\nもうします……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacMon1::
	.string "{JPN}わたくし……\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nもっております……\pどうか わたくしと\nタッグを くんでいただけませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacAccept::
	.string "{JPN}ありがとうございます……\nうけつけで とうろくを してきます……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacReject::
	.string "{JPN}あなたと タッグを\nくみたかったです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMIntro::
	.string "{JPN}やあ!\pぼく\nポケモンブリーダー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMMon1::
	.string "{JPN}ぼくは いま\n{STR_VAR_1}を もつ {STR_VAR_2}とね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nそだてているんだ!\pどう?\pぼくと\nいっしょに タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMAccept::
	.string "{JPN}ありがとう!\pでは さっそく\nうけつけで とうろくを すませてきますね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMReject::
	.string "{JPN}きみと いっしょに タッグを\nくみたかったよ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFIntro::
	.string "{JPN}こんにちは!\pわたし\nトライアスリート {STR_VAR_1}よ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFMon1::
	.string "{JPN}わたしは\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nつれて はしっているの!\pどう?\nわたしと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFAccept::
	.string "{JPN}やったね!\pじゃ さっそく うけつけで\nとうろくを すませてくるわー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFReject::
	.string "{JPN}わたしと タッグを くんだら\nさいこうなのに ざんねんね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMIntro::
	.string "{JPN}やあ!\pぼく\nトライアスリート {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMMon1::
	.string "{JPN}ぼく\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nつれて はしっているんだ!\pどうだい?\nぼくと タッグを くんでみないかい?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMAccept::
	.string "{JPN}やったー!\pいま うけつけで\nとうろくを すませてくるよー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMReject::
	.string "{JPN}きみと タッグを くんで\nいっしょに たたかいたかったよ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlIntro::
	.string "{JPN}わたし\nバトルガール {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlMon1::
	.string "{JPN}わたし\n{STR_VAR_1}を もつ {STR_VAR_2}……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nきたえているの!\pどう?\nわたしと タッグを くまない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlAccept::
	.string "{JPN}ありがとう!\pいまから うけつけで\nとうろくを すませてくるからねー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlReject::
	.string "{JPN}あなたとなら さいきょうの タッグを\nくめると おもったのに……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMIntro::
	.string "{JPN}オレ\nトライアスリート {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMMon1::
	.string "{JPN}オレ\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nつれて はしっているんだぜ!\pどうだい?\nオレと いっしょに タッグを くまない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMAccept::
	.string "{JPN}サンキュー!\pいまから うけつけで\nとうろくを すませてくるぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMReject::
	.string "{JPN}きみとなら さいきょうの タッグを\nくめたのに ざんねんだよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMIntro::
	.string "{JPN}ぼくはね……\nうきわボーイ {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMMon1::
	.string "{JPN}ぼくは\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nつれているんだよ!\pねぇ?\nぼくと いっしょに タッグを くもうよ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMAccept::
	.string "{JPN}やったねー!\pうけつけで とうろく してくるよー!\nがんばろうねー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMReject::
	.string "{JPN}ぼくの ポケモン とっても つよいのに……\nタッグに なれなくて ざんねん!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristIntro::
	.string "{JPN}イエーイ!\pオレは\nギタリスト {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristMon1::
	.string "{JPN}オレは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}を\nつれているんだぜー!\pイエーイ?\nオレと いっしょに タッグを くもうぜ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristAccept::
	.string "{JPN}イエーイ!\pいまから\nうけつけで とうろく してくるぜー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristReject::
	.string "{JPN}オレの ポケモン とっても つよいぜー!\nこうかい しないように するんだぜー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanIntro::
	.string "{JPN}こんにちは\pジェントルマン {STR_VAR_1}と\nもうします$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanMon1::
	.string "{JPN}わたくし\n{STR_VAR_1}を もった {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanMon2Ask::
	.string "{JPN}{STR_VAR_1}を もった {STR_VAR_2}を\nつれておりますぞ!\pどうですかな?\nわたくしの パートナーに なりませんかな?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanAccept::
	.string "{JPN}よろしく おねがいしますぞ!\pでは\nうけつけで とうろく してきます!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanReject::
	.string "{JPN}ざんねん ですな……\nつぎの きかいを きたいして おりますぞ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMIntro::
	.string "{JPN}わたくし\nだいすきクラブの {STR_VAR_1}と\lもうします$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMMon1::
	.string "{JPN}わたくしは いま\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}を\nつれて おります!\pどうでしょう?\nわたくしと タッグを くみませんかな?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMAccept::
	.string "{JPN}ありがとう ございます!\nそれでは とうろくに いってまいります!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMReject::
	.string "{JPN}わたくしの ポケモン\nさいこうなのに ざんねんです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMIntro::
	.string "{JPN}うむ!\pわしは\nたつじん {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMMon1::
	.string "{JPN}わしの きたえあげた ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}じゃ\pどうじゃ?\nわしと タッグを くまぬか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMAccept::
	.string "{JPN}うむ!\pいまから とうろくを すませてくるぞ!\nおたがい がんばろうぞ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMReject::
	.string "{JPN}おぬしの せんたくが\nただしいことを いのっておるぞ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFIntro::
	.string "{JPN}……はいはい\pわたしは\nたつじん {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFMon1::
	.string "{JPN}わたしの きたえあげた ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2} ですよ\pどうですかね?\nわたしと タッグを くみませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFAccept::
	.string "{JPN}……はいはい\nいまから とうろくを してきますよ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFReject::
	.string "{JPN}つぎに あったときには\nタッグを くみたいものですね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerIntro::
	.string "{JPN}オレは\nドラゴンつかい {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerMon1::
	.string "{JPN}オレの きたえあげた ポケモンは\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2} だ!\pどうだ?\nオレと タッグを くんでみないか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerAccept::
	.string "{JPN}……よしっ! がんばるぜー!\pすぐに とうろくを すませて くるからな!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerReject::
	.string "{JPN}オレより つよい パートナーは\nそうそう いないぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperIntro::
	.string "{JPN}ぼくは\nとりつかい {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperMon1::
	.string "{JPN}ぼくの ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2} だね\pどうだい?\nボクと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperAccept::
	.string "{JPN}ありがとー!\pいまから とうろくを してくるよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperReject::
	.string "{JPN}ぼくの ポケモン\nほんとに つよいのに ざんねんだよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyIntro::
	.string "{JPN}ぼく\nにんじゃごっこ {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyMon1::
	.string "{JPN}ぼくの もっている ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}だよ\pねー?\nぼくと タッグを くもうよ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyAccept::
	.string "{JPN}やったー!\nすぐ とうろくを してくるねー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyReject::
	.string "{JPN}ぼくの ポケモンの つよさに\nきっと こうかい するよー$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyIntro::
	.string "{JPN}こんにちは!\pわたし\nパラソルおねえさん {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyMon1::
	.string "{JPN}わたしは いま\n{STR_VAR_1}を もった {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyMon2Ask::
	.string "{JPN}{STR_VAR_1}を もった {STR_VAR_2}を\nつれているの!\pどう?\nわたしの ポケモンと タッグを くまない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyAccept::
	.string "{JPN}ありがとうねー!\pいま うけつけで とうろく してくるから\nがんばろうねー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyReject::
	.string "{JPN}あたしの ポケモン とっても つよいのに\nざんねんよ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacIntro::
	.string "{JPN}わたしは\nこんちゅうマニア {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacMon1::
	.string "{JPN}わたしの みつけた ポケモンは\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2} ですよ!\pどうですか?\nわたしと タッグを くみませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacAccept::
	.string "{JPN}はい!\nりょうかい しました\pいま とうろくを してきますからね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacReject::
	.string "{JPN}わたしが みつけた ポケモンと くめば\nまけないのに……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorIntro::
	.string "{JPN}オッス!\pオレは\nふなのり {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorMon1::
	.string "{JPN}オレの じまんの ポケモンは\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2} だ!\pなあ! \nもちろん オレと タッグを くむだろう?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorAccept::
	.string "{JPN}とうぜん だよな!\nいまから とうろく してくるぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorReject::
	.string "{JPN}オレと くんだら らくしょう なのに\nざんねん だぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorIntro::
	.string "{JPN}やあ\pぼくは\nポケモンコレクター {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorMon1::
	.string "{JPN}ぼくの じまんの ポケモンは\n{STR_VAR_1}を もった {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorMon2Ask::
	.string "{JPN}{STR_VAR_1}を もった {STR_VAR_2}だよ\pどうだい?\nぼくと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorAccept::
	.string "{JPN}おおー!\pじゃあ さっそく うけつけで\nとうろく してくるよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorReject::
	.string "{JPN}ぼくの ポケモンの みりょくに\nきが つかないなんて ガッカリだよ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMIntro::
	.string "{JPN}ぼくは\nポケモンレンジャー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMMon1::
	.string "{JPN}ぼくの つれている ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}だ!\pどうだい?\nぼくと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMAccept::
	.string "{JPN}よしー!\nすぐ うけつけで とうろく してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMReject::
	.string "{JPN}つぎに あったときは\nぼくの ポケモン えらんでくれよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFIntro::
	.string "{JPN}わたしは\nポケモンレンジャー {STR_VAR_1}よ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFMon1::
	.string "{JPN}わたしが つれている ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}よ!\pどう?\nわたしと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFAccept::
	.string "{JPN}がんばるわよー!\nすぐ うけつけで とうろく してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFReject::
	.string "{JPN}つぎこそ\nわたしの ポケモンを えらんでよね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyIntro::
	.string "{JPN}わたくし\nアロマなおねえさん {STR_VAR_1}と\lもうします$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyMon1::
	.string "{JPN}わたくしの つれている ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}です\pどうでしょう?\nわたくしと タッグを くんでみませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyAccept::
	.string "{JPN}こうえいです\nすぐ うけつけで とうろく してきますね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyReject::
	.string "{JPN}つぎに おあいした ときには\nタッグを くみたいものですね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacIntro::
	.string "{JPN}わし\nいせきマニア {STR_VAR_1}じゃ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacMon1::
	.string "{JPN}わしの もっている ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}じゃ!\pどうじゃ?\nわしと タッグを くんでみないか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacAccept::
	.string "{JPN}よしよし!\nすぐ うけつけで とうろく してくるぞい!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacReject::
	.string "{JPN}うーむ……\nわしの ポケモン つよいと おもうぞ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMIntro::
	.string "{JPN}ぼく\nエリートトレーナー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMMon1::
	.string "{JPN}ぼくの つれている ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}だよ\pどう?\nぼくと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMAccept::
	.string "{JPN}よしっ!\nすぐ うけつけで とうろく してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMReject::
	.string "{JPN}きみとなら さいきょうの チームに なると\nおもったんだけどな……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFIntro::
	.string "{JPN}わたしは\nエリートトレーナー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFMon1::
	.string "{JPN}わたしの そだてた ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2} だよ\pどう?\nわたしと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFAccept::
	.string "{JPN}やったわ!\nすぐ うけつけで とうろく してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFReject::
	.string "{JPN}あなたとなら つよい タッグチームが\nできると おもったのに……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacIntro::
	.string "{JPN}オレは\nかいじゅうマニア {STR_VAR_1}だ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacMon1::
	.string "{JPN}オレの もっている ポケモンは\n{STR_VAR_1}を もった {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacMon2Ask::
	.string "{JPN}{STR_VAR_1}を もった {STR_VAR_2}だ!\pどうだ?\nオレと タッグを くんでみないか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacAccept::
	.string "{JPN}やったぜ!\nすぐ うけつけで とうろく してくるぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacReject::
	.string "{JPN}オレの ポケモン つよいぞ!\nこうかい しても しらないぞ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerIntro::
	.string "{JPN}オレ\nキャンプファイヤー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerMon1::
	.string "{JPN}オレの そだてあげた ポケモンは\n{STR_VAR_1}を もった {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerMon2Ask::
	.string "{JPN}{STR_VAR_1}を もった {STR_VAR_2} だよ\pどうだい?\nオレと タッグを くんでみないか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerAccept::
	.string "{JPN}やったー!\nいま うけつけで とうろく してくるからな!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerReject::
	.string "{JPN}つぎに あったときには\nタッグを くんでくれよな!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperIntro::
	.string "{JPN}ぼくは\nキャンプボーイ {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperMon1::
	.string "{JPN}ぼくの そだてた ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2} だよ\pどう?\nぼくと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperAccept::
	.string "{JPN}やったね!\nすぐ うけつけで とうろく してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperReject::
	.string "{JPN}つぎに あったときには\nタッグを くんでよね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerIntro::
	.string "{JPN}わたしは\nピクニックガール {STR_VAR_1}です!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerMon1::
	.string "{JPN}わたしの つれている ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}です\pどうすか?\nわたしと タッグを くんでみませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerAccept::
	.string "{JPN}ありがとうございます!\nすぐ うけつけで とうろく してきます!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerReject::
	.string "{JPN}つぎに あったときには\nタッグを くんでくださいね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMIntro::
	.string "{JPN}ぼくは\nサイキッカー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMMon1::
	.string "{JPN}ぼくの そだてた ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2} だよ\pどう?\nぼくと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMAccept::
	.string "{JPN}やったね!\nすぐ うけつけで とうろく してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMReject::
	.string "{JPN}つぎに あったときには\nタッグを くみたいね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFIntro::
	.string "{JPN}わたし\nサイキッカー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFMon1::
	.string "{JPN}わたしが つれている ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}よ!\pどう?\nわたしと タッグを くまない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFAccept::
	.string "{JPN}ありがとう!\nすぐ うけつけで とうろく してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFReject::
	.string "{JPN}つぎに あったときには\nタッグを くみたいわね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFIntro::
	.string "{JPN}わたし\nじゅくがえり {STR_VAR_1}です!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFMon1::
	.string "{JPN}わたしの つれている ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}です!\pどうですか?\nわたしと タッグを くんでくれませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFAccept::
	.string "{JPN}ありがとうございます!\nすぐ うけつけで とうろく してきます!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFReject::
	.string "{JPN}つぎに あったときには\nタッグを くんでくださいね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFIntro::
	.string "{JPN}わたし\nポケモンブリーダー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFMon1::
	.string "{JPN}わたしの そだてた ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2} なの\pどう?\nわたしと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFAccept::
	.string "{JPN}やったね!\nすぐ うけつけで とうろく してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFReject::
	.string "{JPN}つぎに あったときには\nぜったいに タッグを くんでよね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFIntro::
	.string "{JPN}わたくし\nだいすきクラブ {STR_VAR_1}と\lもうしますのよ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFMon1::
	.string "{JPN}わたくしの そだてた ポケモンちゃんは\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2} ですの\pどうでしょう?\nわたくしと タッグを くみませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFAccept::
	.string "{JPN}ありがとうございます!\pわたくし さっそく\nうけつけで とうろく してきますわ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFReject::
	.string "{JPN}わたくしの ポケモンちゃん\nさいこうなのに ざんねんです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFIntro::
	.string "{JPN}わたし\nビキニのおねえさん {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFMon1::
	.string "{JPN}わたしの そだてた ポケモンは\n{STR_VAR_1}を もった {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFMon2Ask::
	.string "{JPN}{STR_VAR_1}を もった {STR_VAR_2}よっ\pどう?\nわたしと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFAccept::
	.string "{JPN}やったねっ!\nすぐ うけつけで とうろく してくるねっ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFReject::
	.string "{JPN}つぎに あったときには\nタッグを くんでよねっ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMIntro::
	.string "{JPN}オレ\nトライアスリート {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMMon1::
	.string "{JPN}オレの そだてた ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2} だよ\pどう?\nオレと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMAccept::
	.string "{JPN}やったね!\nすぐ うけつけで とうろく してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMReject::
	.string "{JPN}つぎに あったときには\nタッグを くんでくれよな!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFIntro::
	.string "{JPN}わたし\nトライアスリート {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFMon1::
	.string "{JPN}わたしの そだてた ポケモンは\n{STR_VAR_1}を つかう {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFMon2Ask::
	.string "{JPN}{STR_VAR_1}を つかう {STR_VAR_2}よ\pどう?\nわたしと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFAccept::
	.string "{JPN}やったね!\nすぐ うけつけで とうろく してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFReject::
	.string "{JPN}つぎに あったときには\nタッグを くんでよね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMIntro::
	.string "{JPN}オレ\nかいパンやろう {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMMon1::
	.string "{JPN}オレが そだてた ポケモンは\n{STR_VAR_1}を もつ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMMon2Ask::
	.string "{JPN}{STR_VAR_1}を もつ {STR_VAR_2}だよ\pどう?\nオレと タッグを くんでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMAccept::
	.string "{JPN}やったね!\nすぐ うけつけで とうろく してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMReject::
	.string "{JPN}つぎに あったときには\nタッグを くんでくれよな!$"
