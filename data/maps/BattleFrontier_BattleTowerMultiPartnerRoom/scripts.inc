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
	.string "ここに あつまった トレーナーから\n"
	.string "パートナーを 探してください$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_QuitLookingForPartner:
	.string "{PLAYER}さんは タッグの\n"
	.string "パートナーを まだ 見つけていません\p"
	.string "パートナーさがしを やめて\n"
	.string "受付へ 戻りますか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PleaseFindPartner2:
	.string "それでは ここに あつまった\n"
	.string "トレーナーから パートナーを 探してください$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ThankYouForChoosingPartner::
	.string "パートナーを 選んでいただき\n"
	.string "ありがとうございます\p"
	.string "マルチバトルルームへ\n"
	.string "ご案内します$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Intro::
	.string "{JPN}ぼくは {STR_VAR_1}さんの {STR_VAR_2}番弟子\n"
	.string "{STR_VAR_3}です!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Mon1::
	.string "{JPN}{STR_VAR_1}さんの アドバイスで\n"
	.string "{STR_VAR_2}を 持った {STR_VAR_3}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持った {STR_VAR_2}を\nつれてきました\p"
	.string "どうか タッグを 組んでください!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Accept::
	.string "{JPN}ありがとうございます!\n"
	.string "さっそく 登録してきます!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Reject::
	.string "{JPN}{PLAYER}さんと タッグを\n"
	.string "組みたかったです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Intro::
	.string "{JPN}わたしは {STR_VAR_1}さんの {STR_VAR_2}番弟子\n"
	.string "{STR_VAR_3}と いいます$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Mon1::
	.string "{JPN}{STR_VAR_1}さんに そうだんして\n"
	.string "{STR_VAR_2}を 持った {STR_VAR_3}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持った {STR_VAR_2}を\n選びました\p"
	.string "おねがいします! タッグを 組みましょう!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Accept::
	.string "{JPN}どうも ありがとうございます!\n"
	.string "すぐに 登録を すませてきます!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Reject::
	.string "{JPN}{PLAYER}さんと タッグを\n"
	.string "組めると 思ったのに……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Intro::
	.string "{JPN}ぼくは {STR_VAR_3} って いって\n"
	.string "{STR_VAR_1}さんの {STR_VAR_2}番 弟子です\l"
	.string "グスっ……\p"
	.string "ごめんなさい!\n"
	.string "緊張 して 涙が……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Mon1::
	.string "{JPN}ぼく {STR_VAR_3}さんの アドバイスで\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\nつれているんです\pグスン…… ぼ ぼくと\nタッグを 組んで ください!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Accept::
	.string "{JPN}えー ほんとですか!\nやった! うれしいよう! うぉぉーん!\pあ…… ごめん なさい\nうれしくて なみだが……\pさっ さっそく 受付に\n行ってきますから 待ってて ください!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Reject::
	.string "{JPN}そ そんな……\nう うう うわーん!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Intro::
	.string "{JPN}こんにちはっ ボクは {STR_VAR_3}!\n"
	.string "{STR_VAR_1}さんの {STR_VAR_2}番 弟子なんだ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Mon1::
	.string "{JPN}{STR_VAR_3}さんに 進めて もらった\n{STR_VAR_1}を 持つ {STR_VAR_2}とね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}が\nいるんだよ いいでしょ?\lねーっ ボクと 一緒に\lタッグを くもうよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Accept::
	.string "{JPN}わーいっ やった!\nじゃあねー ボクが\l受付に いってくるからね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Reject::
	.string "{JPN}えーっ どうして?\n{PLAYER}さんと 組みたかったなー$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Intro::
	.string "{JPN}ぼくは あの 有名な\n{STR_VAR_1}さんの {STR_VAR_2}ばんの でし\l{STR_VAR_3}です!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Mon1::
	.string "{JPN}{STR_VAR_3}さん から\nアドバイスを いただいて\l{STR_VAR_1}を 使う {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}を\nつれているんです\pおねがいです!\nぼくと タッグを 組んでください!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Accept::
	.string "{JPN}ありがとうございます!\nさっそく 受付を すませてきますね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Reject::
	.string "{JPN}{PLAYER}さんと\nぜひ タッグを 組みたかったです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Intro::
	.string "{JPN}あの…… あたし なんかに\n"
	.string "声を かけてくれて うれしいです\l"
	.string "あたしは {STR_VAR_3}です\l"
	.string "{STR_VAR_1}さんの {STR_VAR_2}番弟子で……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Mon1::
	.string "{JPN}{STR_VAR_3}さんが あたし なんかにも\nアドバイスを くださったので\l{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\nつれています…… おそれ 多いけど……\pムリ だと 思います けど……\nあたしと タッグを 組んでいただけますか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Accept::
	.string "{JPN}本当 ですか? 信じられない!\nあたし なんかと 組んでもらえる なんて!\lが……がんばります!\pあのっ あたしで よかったら\n今から 受付に いって きます!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Reject::
	.string "{JPN}や やっぱり……\nあたし なんか ダメですよね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Intro::
	.string "{JPN}わたしは {STR_VAR_3}\n"
	.string "{STR_VAR_1}さんの {STR_VAR_2}番 弟子なの!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Mon1::
	.string "{JPN}{STR_VAR_3}さんが いいって いったから\n{STR_VAR_1} 使いの {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Mon2Ask::
	.string "{JPN}{STR_VAR_1} 使いの {STR_VAR_2}を\n持って いるわけ!\lわるくない でしょ?\lあなたと タッグを 組んで あげようか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Accept::
	.string "{JPN}うん じゃあ よろしくね\nわたしの あしを ひっぱらないでよね!\lさっそく 受付に いってくるわ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Reject::
	.string "{JPN}え? なんで ことわるの?\n見るめ ないんじゃない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Intro::
	.string "{JPN}オレは トライアスリートの {STR_VAR_3}!\nジョギング トレーニング ラッピングと\l毎日 いそがしい なか\l{STR_VAR_1}の {STR_VAR_2}ばんでし やってる!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Mon1::
	.string "{JPN}{STR_VAR_3}の オススメ!\n{STR_VAR_1}を 持つ {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\nいそがしい なか がんばって 育てたぜ!\pオレと タッグを 組んで がんばろうぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Accept::
	.string "{JPN}サンキュー そう こなくっちゃ!\nんじゃ オレ 受付を すませてくるわ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Reject::
	.string "{JPN}おいおい 忙しい 中 来たのに\n"
	.string "そりゃ ないぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Intro::
	.string "{JPN}ハーイ! 調子は どう?\n{STR_VAR_1}さんの {STR_VAR_2}ばんの\lでしトレーナー {STR_VAR_3}よ よろしくぅ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Mon1::
	.string "{JPN}聞いてきいて! {STR_VAR_3}さんが\nおすすめ してくれた ポケモンはね\l{STR_VAR_1}を 持つ {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}なの\nもちろん バッチリ 育てたわ!\pというわけで わたしと 組まない?\nきっと いいカンジよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Accept::
	.string "{JPN}わーい 楽しく なりそうー!\nじゃ わたしが 受付に いってくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Reject::
	.string "{JPN}もー いじわる!\nわたし オススメなのに!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Intro::
	.string "{JPN}{STR_VAR_3}と 申します\nわたくし {STR_VAR_1}さまの {STR_VAR_2}ばんの\lでしで ございますの……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Mon1::
	.string "{JPN}{STR_VAR_3}さまに 教えを こい\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\n育て あげましたの!\pおねがい わたくしと タッグを\n組んで くださると おっしゃって!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Accept::
	.string "{JPN}うっ うれしい ですわ……\nわたくし 全力で つとめますわ!\pもちろん 受付は わたくしが!\n少し お待ちくださいませ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Reject::
	.string "{JPN}{PLAYER}さんと\nぜひ タッグを 組みたかったです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Intro::
	.string "{JPN}キャー! 話しかけられたっ!\nかっ かんげきっ……!\pわたし! {STR_VAR_3}と いいますっ!\n{STR_VAR_1} 先生の\l{STR_VAR_2}ばん でし ですっ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Mon1::
	.string "{JPN}{STR_VAR_3} 先生の アドバイスで\n{STR_VAR_1}をもつ {STR_VAR_2}ちゃんと$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Mon2Ask::
	.string "{JPN}{STR_VAR_1}をもつ {STR_VAR_2}ちゃんを\n育てましたーっ!\pあの おねがいが!\nわたしと タッグをくんで ほしいのですっ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Accept::
	.string "{JPN}キャー! かっ かんげきっ……!\nおありがとう ございますう!\lではっ 受付を して まいりますっ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Reject::
	.string "{JPN}うっわあああ!\n"
	.string "血も 涙も ないですねっ! でもステキ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Intro::
	.string "{JPN}いえーい! {STR_VAR_1}さんの\n{STR_VAR_2}ばん 弟子の おじさんは\l{STR_VAR_3}です ハロウ ハロウ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Mon1::
	.string "{JPN}{STR_VAR_3} ティーチャー おすすめの\n{STR_VAR_1} マスター {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Mon2Ask::
	.string "{JPN}{STR_VAR_1} マスター {STR_VAR_2}を\nトレイン したんだよー!\lグッドタイミングだね!\lもちろん おじさんと タッグを 組むだろ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Accept::
	.string "{JPN}オーケイ エクセレント!\nじゃ バーっと 受付を すませてくるね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Reject::
	.string "{JPN}ぐわーっ!\n{PLAYER}さん ってのは クールだね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Intro::
	.string "{JPN}お おいっ 大変だ!\nバトルタワー きょうで おしまいらしいぞ!\p…… …… ……\nなんてね うっそ うそ!\pおいらは {STR_VAR_3} って 言うんだ\n{STR_VAR_1}の 1000番目の でしだ!\lうそ ほんとは {STR_VAR_2}ばん!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Mon1::
	.string "{JPN}{STR_VAR_3}が アドバイスを くれてね\n{STR_VAR_1}を 持つ {STR_VAR_2}とね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}をね\n10ぴきずつ 持って いまーす!\lうそ ほんとは 1ぴきずつだよ!\pひとつ どうだろ? こんな おいらと\nタッグバトルを して みないかな?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Accept::
	.string "{JPN}やったー!\nおれいに ポケモン あげちゃおう!\pなんて じょうだん だけど……\nとりあえず 受付を すませてくるよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Reject::
	.string "{JPN}そんな つめたい こと 言うと\nバトルタワーを こわして しまうぞ!\pなんてね うそ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Intro::
	.string "{JPN}ご・き・げ・ん・よ・う!\n"
	.string "はーい ごきげんよう!\p"
	.string "わしは ノリノリ ふなのり {STR_VAR_3}!\n"
	.string "{STR_VAR_1}さんの {STR_VAR_2}番の 弟子だよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Mon1::
	.string "{JPN}{STR_VAR_3}さん アドバイスの たまもの\n{STR_VAR_1}を 持つ {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}が\nわしの じまんの ポケモン!\pさあ この であいを 記念して\nわしと タッグバトルを やってください!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Accept::
	.string "{JPN}ようよう! ノリが いいね\nよろしく 頼みますよ!\lじゃ 受付は わしが いってくるよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Reject::
	.string "{JPN}そんな {PLAYER}さん!\n"
	.string "ノリが 悪いじゃ ないか!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Intro::
	.string "{JPN}イャァアッホオオォォウウ!\n"
	.string "あ 逃げないでェ 帰らないでェ!\p"
	.string "オスっ オレっ {STR_VAR_3}!\n"
	.string "{STR_VAR_1}さんの {STR_VAR_2}番 弟子なのさ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Mon1::
	.string "{JPN}{STR_VAR_3}さんったら オレに!\n{STR_VAR_1}を 持つ {STR_VAR_2}と!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を!\n使えって 言うから そうしたぜエェ!\pって ことで どうする オレ?\nさっそく 頼んでみる?\pおねがいです!\nオレと タッグを 組んでエェ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Accept::
	.string "{JPN}やったラッキー うれしーよ オレ!!\nさっそく 受付に いってくる!?\lじゃ ちょっと 待っててェ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Reject::
	.string "{JPN}って 断られてる オレっ!?\n"
	.string "{PLAYER}さん つれないよォ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Intro::
	.string "{JPN}ゴホっ!\pあっ ゴメン カゼ ひいてて……\nポケモンは 元気だから ダイジョブだよ\pおれは {STR_VAR_3}\n{STR_VAR_1}さんの {STR_VAR_2}ばんでしだよ\l……ゴホっ ゴホっ よろしく!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Mon1::
	.string "{JPN}{STR_VAR_3}さんの アドバイスで\n{STR_VAR_1}を 持つ {STR_VAR_2}とさ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\n雨のひも 風のひも 育てあげたよ\l……それで カゼを ひいたんだ きっと\pまあ そういう わけだからさ\nこんな おれと 一緒に 組まないかい?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Accept::
	.string "{JPN}ありがとう わるいね\n……ゴホ ゴホ!\l受付を すませてくるから……\lゴホ 待っててね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Reject::
	.string "{JPN}えー なんで!\n{PLAYER}さんと オレって いいけどなあ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Intro::
	.string "{JPN}あっ ど どうも!\n"
	.string "照れるなあ\p"
	.string "あの {STR_VAR_3}です! {STR_VAR_1}さんの\n"
	.string "{STR_VAR_2}番の 弟子 やってます……!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Mon1::
	.string "{JPN}{STR_VAR_3}さんが よいと いったので\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\n育ててるんだ…… はずかしながら\pその はずかしい けど ワシと……\nタッグを 組んで もらえる かな……?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Accept::
	.string "{JPN}あ ありがとう!\nはずかしい けど がんばって 見るよ\pじゃ…… う 受付 してくるね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Reject::
	.string "{JPN}こ 断らないでくれ!\n"
	.string "ワシに 恥を かかせないで くれ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Intro::
	.string "{JPN}わたしは {STR_VAR_3}だ\n"
	.string "うそじゃない!\l"
	.string "{STR_VAR_1}さんの {STR_VAR_2}番の 弟子だ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Mon1::
	.string "{JPN}{STR_VAR_3}さんから 進められて\nどうやら 本気 みたい だったから\l{STR_VAR_1}を 持つ {STR_VAR_2}と$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Mon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\nつれているんだ 本当だよ!\pところで もし よかったら わたしを\nキミの パートナーに 認めて ほしい$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Accept::
	.string "{JPN}本当に 認めて くれるのかね?\nうそだったら いやすぎる からな\pいや 本当なら いいんだ すまなかった\nさっそく 受付を すませてくる$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Reject::
	.string "{JPN}え? なぜ 断るのかね?\n"
	.string "悪い 冗談 では ないのかね?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassIntro::
	.string "{JPN}わたし\nミニスカート {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassMon1::
	.string "{JPN}わたしはね……\n{STR_VAR_1}を 使う {STR_VAR_2}とね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}を\nつれて 歩いているの!\pどう?\nパートナーに なってくれない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassAccept::
	.string "{JPN}ありがとうー!\n今から 登録 してくるわね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassReject::
	.string "{JPN}わたしの パートナーに ならないなんて\nきっと 後悔 するから!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterIntro::
	.string "{JPN}あのね!\pぼくは\nたんパンこぞう {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterMon1::
	.string "{JPN}ぼくはね……\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}を\n持っているよ!\pねぇねぇ\n一緒に タッグを くもうよ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterAccept::
	.string "{JPN}やったー!\n受付で 登録 してくるねー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterReject::
	.string "{JPN}ぼくの ポケモンと タッグを 組めば\nきっと 強いのにー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerIntro::
	.string "{JPN}ヤッホー!\pわし\nやまおとこ {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerMon1::
	.string "{JPN}わし\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\nつれて 歩いています!\pどうですかな?\n一緒に タッグを 組みませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerAccept::
	.string "{JPN}ヤッホー!\p今から 受付で\n登録を すませてきますよ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerReject::
	.string "{JPN}きみと 一緒に\nバトルを したかったです$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyIntro::
	.string "{JPN}こんにちは!\pわたし\n大人のおねえさん {STR_VAR_1}よ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyMon1::
	.string "{JPN}わたし\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}を\n育てているのよ!\pどう?\n一緒に タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyAccept::
	.string "{JPN}うれしい!\pすぐに 登録を すませるわね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyReject::
	.string "{JPN}ざんねんね!\pあなたと わたしが\nくんだら 最高 だったのに……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanIntro::
	.string "{JPN}よー!\pわし\nつりびと {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanMon1::
	.string "{JPN}わしは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}を\nつれているぞ\pどうだね?\n一緒に 戦うかね?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanAccept::
	.string "{JPN}よしよし!\nまかせとけー\p今から 登録を するからな!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanReject::
	.string "{JPN}きみと わしの 相性は\n"
	.string "バッチリ だったのにな……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyIntro::
	.string "{JPN}わたくし\nおじょうさま {STR_VAR_1}と\l申します!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyMon1::
	.string "{JPN}わたくし\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}を\nつれていますの\pどうですか?\pわたくしと 一緒に\nタッグを 組んで いただけませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyAccept::
	.string "{JPN}ありがとうございます!\pでは わたくしは \n登録を すませてきますわ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyReject::
	.string "{JPN}わたくしの パートナーに ならないと\nきっと 後悔 しますわよ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFIntro::
	.string "{JPN}わたし\nトライアスリート {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFMon1::
	.string "{JPN}わたしはね……\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\n持っているの!\pおねがい!\n一緒に タッグを 組んでよ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFAccept::
	.string "{JPN}ありがとうー!\n受付で 登録 してくるー$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFReject::
	.string "{JPN}わたしと タッグを くんだら\nきっと 強いのにー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherIntro::
	.string "{JPN}やあ!\pぼく\nむしとりしょうねん {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherMon1::
	.string "{JPN}ぼく\n{STR_VAR_1}を 覚えた {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherMon2Ask::
	.string "{JPN}{STR_VAR_1}を 覚えた {STR_VAR_2}を\n持っているよ!\pねー?\nぼくと 一緒に タッグを 組まない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherAccept::
	.string "{JPN}やったね!\pじゃ 今から 受付で\n登録を すませてくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherReject::
	.string "{JPN}ぼくの ポケモン 最高 なのに!\n後悔 しないでよ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMIntro::
	.string "{JPN}こんにちは!\pぼくは\nじゅくがえり {STR_VAR_1} です$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMMon1::
	.string "{JPN}ぼくは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}を\n育てています!\pどうです?\pぼくと\n一緒に タッグを 組みませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMAccept::
	.string "{JPN}ありがとうございます!\pでは\n受付で 登録を すませてきますね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMReject::
	.string "{JPN}ざんねんです……\pきみと 一緒に タッグを 組んで\nべんきょう したかったです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyIntro::
	.string "{JPN}よう!\pオレ\nおぼっちゃま {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyMon1::
	.string "{JPN}オレ\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\n持っているんだぜ!\pオレ 一緒に\nタッグを 組んでもいいぜ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyAccept::
	.string "{JPN}よし!\pじゃあ 今から\n受付で 登録を すませてくるぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyReject::
	.string "{JPN}オレと タッグを 組まない なんて\nきっと 後悔 するぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltIntro::
	.string "{JPN}オッス!\pカラテおう {STR_VAR_1}\nと 申します$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltMon1::
	.string "{JPN}いま わたくし\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\nつれております!\pおねがいします!\pわたくしと\nタッグを 組んで くれませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltAccept::
	.string "{JPN}オッス!\pさっそく\n受付に 登録 してまいります!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltReject::
	.string "{JPN}オッス……\p次の きかいは\nぜひ タッグを 組んでください……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFIntro::
	.string "{JPN}こんにちは!\pわたし\nうきわガール {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFMon1::
	.string "{JPN}わたし\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\nつれているの!\pねぇ?\p一緒に\nタッグを 組んでよー?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFAccept::
	.string "{JPN}ありがとうー!\pじゃあ 今から\n受付で 登録を すませてくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFReject::
	.string "{JPN}わたしと タッグを くんだら\nとっても 強かったのにー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacIntro::
	.string "{JPN}こんにちは……\pオカルトマニア {STR_VAR_1}と\n申します……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacMon1::
	.string "{JPN}わたくし……\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\n持っております……\pどうか わたくしと\nタッグを 組んでいただけませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacAccept::
	.string "{JPN}ありがとうございます……\n受付で 登録を してきます……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacReject::
	.string "{JPN}あなたと タッグを\n組みたかったです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMIntro::
	.string "{JPN}やあ!\pぼく\nポケモンブリーダー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMMon1::
	.string "{JPN}ぼくは いま\n{STR_VAR_1}を 持つ {STR_VAR_2}とね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\n育てているんだ!\pどう?\pぼくと\n一緒に タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMAccept::
	.string "{JPN}ありがとう!\pでは さっそく\n受付で 登録を すませてきますね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMReject::
	.string "{JPN}きみと 一緒に タッグを\n組みたかったよ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFIntro::
	.string "{JPN}こんにちは!\pわたし\nトライアスリート {STR_VAR_1}よ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFMon1::
	.string "{JPN}わたしは\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\nつれて 走っているの!\pどう?\nわたしと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFAccept::
	.string "{JPN}やったね!\pじゃ さっそく 受付で\n登録を すませてくるわー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFReject::
	.string "{JPN}わたしと タッグを くんだら\n最高なのに ざんねんね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMIntro::
	.string "{JPN}やあ!\pぼく\nトライアスリート {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMMon1::
	.string "{JPN}ぼく\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\nつれて 走っているんだ!\pどうだい?\nぼくと タッグを 組んでみないかい?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMAccept::
	.string "{JPN}やったー!\pいま 受付で\n登録を すませてくるよー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMReject::
	.string "{JPN}きみと タッグを 組んで\n一緒に 戦いたかったよ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlIntro::
	.string "{JPN}わたし\nバトルガール {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlMon1::
	.string "{JPN}わたし\n{STR_VAR_1}を 持つ {STR_VAR_2}……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\n鍛えているの!\pどう?\nわたしと タッグを 組まない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlAccept::
	.string "{JPN}ありがとう!\p今から 受付で\n登録を すませてくるからねー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlReject::
	.string "{JPN}あなたとなら 最強の タッグを\n組めると 思ったのに……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMIntro::
	.string "{JPN}オレ\nトライアスリート {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMMon1::
	.string "{JPN}オレ\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\nつれて 走っているんだぜ!\pどうだい?\nオレと 一緒に タッグを 組まない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMAccept::
	.string "{JPN}サンキュー!\p今から 受付で\n登録を すませてくるぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMReject::
	.string "{JPN}きみとなら 最強の タッグを\n組めたのに ざんねんだよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMIntro::
	.string "{JPN}ぼくはね……\nうきわボーイ {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMMon1::
	.string "{JPN}ぼくは\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\nつれているんだよ!\pねぇ?\nぼくと 一緒に タッグを くもうよ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMAccept::
	.string "{JPN}やったねー!\p受付で 登録 してくるよー!\nがんばろうねー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMReject::
	.string "{JPN}ぼくの ポケモン とっても 強いのに……\nタッグに なれなくて ざんねん!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristIntro::
	.string "{JPN}イエーイ!\pオレは\nギタリスト {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristMon1::
	.string "{JPN}オレは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}を\nつれているんだぜー!\pイエーイ?\nオレと 一緒に タッグを くもうぜ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristAccept::
	.string "{JPN}イエーイ!\p今から\n受付で 登録 してくるぜー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristReject::
	.string "{JPN}オレの ポケモン とっても 強いぜー!\n後悔 しないように するんだぜー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanIntro::
	.string "{JPN}こんにちは\pジェントルマン {STR_VAR_1}と\n申します$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanMon1::
	.string "{JPN}わたくし\n{STR_VAR_1}を 持った {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持った {STR_VAR_2}を\nつれておりますぞ!\pどうですかな?\nわたくしの パートナーに なりませんかな?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanAccept::
	.string "{JPN}よろしく おねがいしますぞ!\pでは\n受付で 登録 してきます!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanReject::
	.string "{JPN}ざんねん ですな……\n次の きかいを 期待して おりますぞ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMIntro::
	.string "{JPN}わたくし\nだいすきクラブの {STR_VAR_1}と\l申します$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMMon1::
	.string "{JPN}わたくしは いま\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}を\nつれて おります!\pどうでしょう?\nわたくしと タッグを 組みませんかな?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMAccept::
	.string "{JPN}ありがとう ございます!\nそれでは 登録に いってまいります!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMReject::
	.string "{JPN}わたくしの ポケモン\n最高なのに ざんねんです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMIntro::
	.string "{JPN}うむ!\p"
	.string "わしは\n"
	.string "達人 {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMMon1::
	.string "{JPN}わしの 鍛えあげた ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}じゃ\pどうじゃ?\nわしと タッグを 組まぬか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMAccept::
	.string "{JPN}うむ!\p今から 登録を すませてくるぞ!\nおたがい がんばろうぞ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMReject::
	.string "{JPN}おぬしの 選択が\n正しいことを いのっておるぞ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFIntro::
	.string "{JPN}……はいはい\p"
	.string "わたしは\n"
	.string "達人 {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFMon1::
	.string "{JPN}わたしの 鍛えあげた ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2} ですよ\pどうですかね?\nわたしと タッグを 組みませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFAccept::
	.string "{JPN}……はいはい\n今から 登録を してきますよ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFReject::
	.string "{JPN}次に あったときには\nタッグを 組みたいものですね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerIntro::
	.string "{JPN}オレは\nドラゴンつかい {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerMon1::
	.string "{JPN}オレの 鍛えあげた ポケモンは\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2} だ!\pどうだ?\nオレと タッグを 組んでみないか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerAccept::
	.string "{JPN}……よしっ! がんばるぜー!\pすぐに 登録を すませて くるからな!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerReject::
	.string "{JPN}オレより 強い パートナーは\nそうそう いないぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperIntro::
	.string "{JPN}ぼくは\nとりつかい {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperMon1::
	.string "{JPN}ぼくの ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2} だね\pどうだい?\nボクと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperAccept::
	.string "{JPN}ありがとー!\p今から 登録を してくるよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperReject::
	.string "{JPN}ぼくの ポケモン\nほんとに 強いのに ざんねんだよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyIntro::
	.string "{JPN}ぼく\nにんじゃごっこ {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyMon1::
	.string "{JPN}ぼくの 持っている ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}だよ\pねー?\nぼくと タッグを くもうよ?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyAccept::
	.string "{JPN}やったー!\nすぐ 登録を してくるねー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyReject::
	.string "{JPN}ぼくの ポケモンの 強さに\nきっと 後悔 するよー$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyIntro::
	.string "{JPN}こんにちは!\pわたし\nパラソルおねえさん {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyMon1::
	.string "{JPN}わたしは いま\n{STR_VAR_1}を 持った {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持った {STR_VAR_2}を\nつれているの!\pどう?\nわたしの ポケモンと タッグを 組まない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyAccept::
	.string "{JPN}ありがとうねー!\pいま 受付で 登録 してくるから\nがんばろうねー!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyReject::
	.string "{JPN}あたしの ポケモン とっても 強いのに\nざんねんよ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacIntro::
	.string "{JPN}わたしは\nこんちゅうマニア {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacMon1::
	.string "{JPN}わたしの 見つけた ポケモンは\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2} ですよ!\pどうですか?\nわたしと タッグを 組みませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacAccept::
	.string "{JPN}はい!\nりょうかい しました\pいま 登録を してきますからね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacReject::
	.string "{JPN}わたしが 見つけた ポケモンと 組めば\n負けないのに……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorIntro::
	.string "{JPN}オッス!\pオレは\nふなのり {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorMon1::
	.string "{JPN}オレの じまんの ポケモンは\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2} だ!\pなあ! \nもちろん オレと タッグを 組むだろう?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorAccept::
	.string "{JPN}とうぜん だよな!\n今から 登録 してくるぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorReject::
	.string "{JPN}オレと 組んだら 楽勝 なのに\n"
	.string "残念 だぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorIntro::
	.string "{JPN}やあ\pぼくは\nポケモンコレクター {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorMon1::
	.string "{JPN}ぼくの じまんの ポケモンは\n{STR_VAR_1}を 持った {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持った {STR_VAR_2}だよ\pどうだい?\nぼくと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorAccept::
	.string "{JPN}おおー!\pじゃあ さっそく 受付で\n登録 してくるよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorReject::
	.string "{JPN}ぼくの ポケモンの 魅力に\nきが つかないなんて ガッカリだよ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMIntro::
	.string "{JPN}ぼくは\nポケモンレンジャー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMMon1::
	.string "{JPN}ぼくの つれている ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}だ!\pどうだい?\nぼくと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMAccept::
	.string "{JPN}よしー!\nすぐ 受付で 登録 してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMReject::
	.string "{JPN}次に あったときは\nぼくの ポケモン 選んでくれよ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFIntro::
	.string "{JPN}わたしは\nポケモンレンジャー {STR_VAR_1}よ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFMon1::
	.string "{JPN}わたしが つれている ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}よ!\pどう?\nわたしと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFAccept::
	.string "{JPN}がんばるわよー!\nすぐ 受付で 登録 してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFReject::
	.string "{JPN}次こそ\nわたしの ポケモンを 選んでよね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyIntro::
	.string "{JPN}わたくし\nアロマなおねえさん {STR_VAR_1}と\l申します$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyMon1::
	.string "{JPN}わたくしの つれている ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}です\pどうでしょう?\nわたくしと タッグを 組んでみませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyAccept::
	.string "{JPN}こうえいです\nすぐ 受付で 登録 してきますね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyReject::
	.string "{JPN}次に おあいした ときには\nタッグを 組みたいものですね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacIntro::
	.string "{JPN}わし\n遺跡マニア {STR_VAR_1}じゃ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacMon1::
	.string "{JPN}わしの 持っている ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}じゃ!\pどうじゃ?\nわしと タッグを 組んでみないか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacAccept::
	.string "{JPN}よしよし!\nすぐ 受付で 登録 してくるぞい!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacReject::
	.string "{JPN}うーむ……\nわしの ポケモン 強いと 思うぞ……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMIntro::
	.string "{JPN}ぼく\nエリートトレーナー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMMon1::
	.string "{JPN}ぼくの つれている ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}だよ\pどう?\nぼくと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMAccept::
	.string "{JPN}よしっ!\nすぐ 受付で 登録 してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMReject::
	.string "{JPN}きみとなら 最強の チームに なると\n思ったんだけどな……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFIntro::
	.string "{JPN}わたしは\nエリートトレーナー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFMon1::
	.string "{JPN}わたしの 育てた ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2} だよ\pどう?\nわたしと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFAccept::
	.string "{JPN}やったわ!\nすぐ 受付で 登録 してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFReject::
	.string "{JPN}あなたとなら 強い タッグチームが\nできると 思ったのに……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacIntro::
	.string "{JPN}オレは\nかいじゅうマニア {STR_VAR_1}だ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacMon1::
	.string "{JPN}オレの 持っている ポケモンは\n{STR_VAR_1}を 持った {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持った {STR_VAR_2}だ!\pどうだ?\nオレと タッグを 組んでみないか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacAccept::
	.string "{JPN}やったぜ!\nすぐ 受付で 登録 してくるぜ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacReject::
	.string "{JPN}オレの ポケモン 強いぞ!\n後悔 しても 知らないぞ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerIntro::
	.string "{JPN}オレ\nキャンプファイヤー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerMon1::
	.string "{JPN}オレの 育てあげた ポケモンは\n{STR_VAR_1}を 持った {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持った {STR_VAR_2} だよ\pどうだい?\nオレと タッグを 組んでみないか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerAccept::
	.string "{JPN}やったー!\nいま 受付で 登録 してくるからな!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerReject::
	.string "{JPN}次に あったときには\nタッグを 組んでくれよな!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperIntro::
	.string "{JPN}ぼくは\nキャンプボーイ {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperMon1::
	.string "{JPN}ぼくの 育てた ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2} だよ\pどう?\nぼくと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperAccept::
	.string "{JPN}やったね!\nすぐ 受付で 登録 してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperReject::
	.string "{JPN}次に あったときには\nタッグを 組んでよね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerIntro::
	.string "{JPN}わたしは\nピクニックガール {STR_VAR_1}です!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerMon1::
	.string "{JPN}わたしの つれている ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}です\pどうすか?\nわたしと タッグを 組んでみませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerAccept::
	.string "{JPN}ありがとうございます!\nすぐ 受付で 登録 してきます!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerReject::
	.string "{JPN}次に あったときには\nタッグを 組んでくださいね$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMIntro::
	.string "{JPN}ぼくは\nサイキッカー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMMon1::
	.string "{JPN}ぼくの 育てた ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2} だよ\pどう?\nぼくと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMAccept::
	.string "{JPN}やったね!\nすぐ 受付で 登録 してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMReject::
	.string "{JPN}次に あったときには\nタッグを 組みたいね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFIntro::
	.string "{JPN}わたし\nサイキッカー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFMon1::
	.string "{JPN}わたしが つれている ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}よ!\pどう?\nわたしと タッグを 組まない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFAccept::
	.string "{JPN}ありがとう!\nすぐ 受付で 登録 してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFReject::
	.string "{JPN}次に あったときには\nタッグを 組みたいわね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFIntro::
	.string "{JPN}わたし\nじゅくがえり {STR_VAR_1}です!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFMon1::
	.string "{JPN}わたしの つれている ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}です!\pどうですか?\nわたしと タッグを 組んでくれませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFAccept::
	.string "{JPN}ありがとうございます!\nすぐ 受付で 登録 してきます!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFReject::
	.string "{JPN}次に あったときには\nタッグを 組んでくださいね……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFIntro::
	.string "{JPN}わたし\nポケモンブリーダー {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFMon1::
	.string "{JPN}わたしの 育てた ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2} なの\pどう?\nわたしと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFAccept::
	.string "{JPN}やったね!\nすぐ 受付で 登録 してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFReject::
	.string "{JPN}次に あったときには\n絶対に タッグを 組んでよね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFIntro::
	.string "{JPN}わたくし\nだいすきクラブ {STR_VAR_1}と\l申しますのよ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFMon1::
	.string "{JPN}わたくしの 育てた ポケモンちゃんは\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2} ですの\pどうでしょう?\nわたくしと タッグを 組みませんか?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFAccept::
	.string "{JPN}ありがとうございます!\pわたくし さっそく\n受付で 登録 してきますわ$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFReject::
	.string "{JPN}わたくしの ポケモンちゃん\n最高なのに ざんねんです……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFIntro::
	.string "{JPN}わたし\nビキニのおねえさん {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFMon1::
	.string "{JPN}わたしの 育てた ポケモンは\n{STR_VAR_1}を 持った {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持った {STR_VAR_2}よっ\pどう?\nわたしと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFAccept::
	.string "{JPN}やったねっ!\nすぐ 受付で 登録 してくるねっ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFReject::
	.string "{JPN}次に あったときには\nタッグを 組んでよねっ!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMIntro::
	.string "{JPN}オレ\nトライアスリート {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMMon1::
	.string "{JPN}オレの 育てた ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2} だよ\pどう?\nオレと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMAccept::
	.string "{JPN}やったね!\nすぐ 受付で 登録 してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMReject::
	.string "{JPN}次に あったときには\nタッグを 組んでくれよな!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFIntro::
	.string "{JPN}わたし\nトライアスリート {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFMon1::
	.string "{JPN}わたしの 育てた ポケモンは\n{STR_VAR_1}を 使う {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFMon2Ask::
	.string "{JPN}{STR_VAR_1}を 使う {STR_VAR_2}よ\pどう?\nわたしと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFAccept::
	.string "{JPN}やったね!\nすぐ 受付で 登録 してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFReject::
	.string "{JPN}次に あったときには\nタッグを 組んでよね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMIntro::
	.string "{JPN}オレ\nかいパンやろう {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMMon1::
	.string "{JPN}オレが 育てた ポケモンは\n{STR_VAR_1}を 持つ {STR_VAR_2}と……$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMMon2Ask::
	.string "{JPN}{STR_VAR_1}を 持つ {STR_VAR_2}だよ\pどう?\nオレと タッグを 組んでみない?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMAccept::
	.string "{JPN}やったね!\nすぐ 受付で 登録 してくるね!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMReject::
	.string "{JPN}次に あったときには\nタッグを 組んでくれよな!$"
