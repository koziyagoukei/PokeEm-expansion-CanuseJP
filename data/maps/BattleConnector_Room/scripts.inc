@ Battle Connector: a room (src/battle_connector.c). What it holds starts by itself; the doorway at the top leads on
@ once it is done (VAR_TEMP_1 = 1).
BattleConnector_Room_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, BattleConnector_Room_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, BattleConnector_Room_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleConnector_Room_OnFrame
	.byte 0

BattleConnector_Room_OnTransition:
	setflag FLAG_TEMP_HIDE_FOLLOWER
	special BattleConnector_SetUpRoomObjects
	end

BattleConnector_Room_OnWarp:
	map_script_2 VAR_TEMP_4, 0, BattleConnector_EventScript_TurnPlayerNorth
	.2byte 0

BattleConnector_Room_OnFrame:
	map_script_2 VAR_TEMP_1, 0, BattleConnector_Room_EventScript_RunRoom
	.2byte 0

BattleConnector_Room_EventScript_Npc::
	end

BattleConnector_Room_EventScript_RunRoom::
	lockall
	setvar VAR_TEMP_1, 1
	delay 16
	specialvar VAR_RESULT, BattleConnector_IsPanicRush
	goto_if_eq VAR_RESULT, TRUE, BattleConnector_Room_EventScript_PanicRoom
	specialvar VAR_RESULT, BattleConnector_GetRoomType
	switch VAR_RESULT
	case CONNECTOR_ROOM_DARK_TRAINER, BattleConnector_Room_EventScript_TrainerRoom
	case CONNECTOR_ROOM_STRONG_TRAINER, BattleConnector_Room_EventScript_TrainerRoom
	case CONNECTOR_ROOM_HEAL, BattleConnector_Room_EventScript_HealRoom
	case CONNECTOR_ROOM_CALM, BattleConnector_Room_EventScript_CalmRoom
	case CONNECTOR_ROOM_ITEM, BattleConnector_Room_EventScript_ItemRoom
	case CONNECTOR_ROOM_BOSS, BattleConnector_Room_EventScript_BossRoom
	goto BattleConnector_Room_EventScript_ShadowRoom
	end

@ Panic Rush: pair after pair, until the player stops or loses; the chain is paid in BP at the reception
BattleConnector_Room_EventScript_PanicRoom::
	msgbox BattleConnector_Room_Text_PanicAppears, MSGBOX_DEFAULT
	closemessage
BattleConnector_Room_EventScript_PanicBattle::
	special BattleConnector_StartPanicBattle
	waitstate
	goto_if_eq VAR_RESULT, CONNECTOR_BATTLE_LOST, BattleConnector_Room_EventScript_PanicLost
	special BattleConnector_BufferPanicChain
	msgbox BattleConnector_Room_Text_PanicCalmed, MSGBOX_DEFAULT
	msgbox BattleConnector_Room_Text_PanicAskContinue, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, BattleConnector_Room_EventScript_PanicEnd
	msgbox BattleConnector_Room_Text_PanicNext, MSGBOX_DEFAULT
	closemessage
	goto BattleConnector_Room_EventScript_PanicBattle
	end

BattleConnector_Room_EventScript_PanicLost::
	msgbox BattleConnector_Room_Text_PanicLost, MSGBOX_DEFAULT
BattleConnector_Room_EventScript_PanicEnd::
	closemessage
	special BattleConnector_EndPanicRush
	warp MAP_ARTISAN_CAVE_1F, 6, 12
	waitstate
	end

BattleConnector_Room_EventScript_ShadowRoom::
	special BattleConnector_BufferRoomText
	msgbox BattleConnector_Room_Text_ShadowAppears, MSGBOX_DEFAULT
	goto BattleConnector_Room_EventScript_ShadowBattle
	end

BattleConnector_Room_EventScript_BossRoom::
	special BattleConnector_BufferRoomText
	msgbox BattleConnector_Room_Text_BossAppears, MSGBOX_DEFAULT
	goto BattleConnector_Room_EventScript_ShadowBattle
	end

BattleConnector_Room_EventScript_ShadowBattle::
	closemessage
	special BattleConnector_StartRoomBattle
	waitstate
	goto_if_eq VAR_RESULT, CONNECTOR_BATTLE_LOST, BattleConnector_EventScript_Lost
	removeobject LOCALID_CONNECTOR_ROOM_NPC
	call BattleConnector_Room_EventScript_Protect
	releaseall
	end

BattleConnector_Room_EventScript_TrainerRoom::
	special BattleConnector_BufferRoomText
	msgbox gStringVar4, MSGBOX_DEFAULT
	closemessage
	special BattleConnector_StartRoomBattle
	waitstate
	goto_if_eq VAR_RESULT, CONNECTOR_BATTLE_LOST, BattleConnector_EventScript_Lost
	special BattleConnector_BufferTrainerAfterText
	msgbox gStringVar4, MSGBOX_DEFAULT
	closemessage
	call BattleConnector_Room_EventScript_NpcStepAside
	call BattleConnector_Room_EventScript_Protect
	releaseall
	end

@ Offers each defeated Shadow Pokemon in turn
BattleConnector_Room_EventScript_Protect::
	specialvar VAR_RESULT, BattleConnector_BufferDefeatedShadowMon
	goto_if_eq VAR_RESULT, FALSE, BattleConnector_Room_EventScript_ProtectEnd
	msgbox BattleConnector_Room_Text_AskProtect, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, BattleConnector_Room_EventScript_ProtectDeclined
	specialvar VAR_RESULT, BattleConnector_ProtectDefeatedMon
	switch VAR_RESULT
	case CONNECTOR_PROTECT_JOINED, BattleConnector_Room_EventScript_ProtectJoined
	case CONNECTOR_PROTECT_PARTY_FULL, BattleConnector_Room_EventScript_ProtectPartyFull
	case CONNECTOR_PROTECT_LIST_FULL, BattleConnector_Room_EventScript_ProtectListFull
	goto BattleConnector_Room_EventScript_Protect
	end

BattleConnector_Room_EventScript_ProtectDeclined::
	msgbox BattleConnector_Room_Text_ProtectDeclined, MSGBOX_DEFAULT
	goto BattleConnector_Room_EventScript_Protect
	end

BattleConnector_Room_EventScript_ProtectJoined::
	playfanfare MUS_OBTAIN_ITEM
	msgbox BattleConnector_Room_Text_ProtectJoined, MSGBOX_DEFAULT
	waitfanfare
	goto BattleConnector_Room_EventScript_Protect
	end

BattleConnector_Room_EventScript_ProtectPartyFull::
	playfanfare MUS_OBTAIN_ITEM
	msgbox BattleConnector_Room_Text_ProtectPartyFull, MSGBOX_DEFAULT
	waitfanfare
	msgbox BattleConnector_Room_Text_AskSwap, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, BattleConnector_Room_EventScript_ProtectKept
	special ChoosePartyMon
	goto_if_ge VAR_0x8004, PARTY_SIZE, BattleConnector_Room_EventScript_ProtectKept
	specialvar VAR_RESULT, BattleConnector_SwapIntoParty
	goto_if_eq VAR_RESULT, FALSE, BattleConnector_Room_EventScript_ProtectKept
	msgbox BattleConnector_Room_Text_Swapped, MSGBOX_DEFAULT
	goto BattleConnector_Room_EventScript_Protect
	end

BattleConnector_Room_EventScript_ProtectKept::
	msgbox BattleConnector_Room_Text_ProtectKept, MSGBOX_DEFAULT
	goto BattleConnector_Room_EventScript_Protect
	end

BattleConnector_Room_EventScript_ProtectListFull::
	msgbox BattleConnector_Room_Text_ProtectListFull, MSGBOX_DEFAULT
	goto BattleConnector_Room_EventScript_Protect
	end

BattleConnector_Room_EventScript_ProtectEnd::
	return

BattleConnector_Room_EventScript_HealRoom::
	msgbox BattleConnector_Room_Text_Heal, MSGBOX_DEFAULT
	special BattleConnector_HealParty
	playfanfare MUS_HEAL
	waitfanfare
	msgbox BattleConnector_Room_Text_Healed, MSGBOX_DEFAULT
	closemessage
	call BattleConnector_Room_EventScript_NpcStepAside
	releaseall
	end

BattleConnector_Room_EventScript_CalmRoom::
	msgbox BattleConnector_Room_Text_CalmScent, MSGBOX_DEFAULT
	specialvar VAR_RESULT, BattleConnector_CalmParty
	goto_if_eq VAR_RESULT, 0, BattleConnector_Room_EventScript_CalmNoShadow
	playse SE_M_HEAL_BELL
	waitse
	msgbox BattleConnector_Room_Text_Calmed, MSGBOX_DEFAULT
	closemessage
	call BattleConnector_Room_EventScript_NpcStepAside
	releaseall
	end

BattleConnector_Room_EventScript_CalmNoShadow::
	msgbox BattleConnector_Room_Text_CalmNoShadow, MSGBOX_DEFAULT
	closemessage
	call BattleConnector_Room_EventScript_NpcStepAside
	releaseall
	end

BattleConnector_Room_EventScript_ItemRoom::
	special BattleConnector_PickRoomItem
	removeobject LOCALID_CONNECTOR_ROOM_NPC
	giveitem VAR_0x8004
	releaseall
	end

@ The person in the room moves out of the doorway's way (the only way to the doorway is the tile in front of it)
BattleConnector_Room_EventScript_NpcStepAside::
	applymovement LOCALID_CONNECTOR_ROOM_NPC, BattleConnector_Room_Movement_StepAside
	waitmovement 0
	return

BattleConnector_Room_Movement_StepAside:
	walk_left
	face_down
	step_end

BattleConnector_Room_EventScript_Exit::
	lockall
	specialvar VAR_RESULT, BattleConnector_LeaveRoom
	switch VAR_RESULT
	case CONNECTOR_NEXT_ROOM, BattleConnector_Room_EventScript_ExitToRoom
	case CONNECTOR_NEXT_RESULTS, BattleConnector_Room_EventScript_ExitToResults
	warpsilent MAP_BATTLE_CONNECTOR_BRANCH, 6, 9
	waitstate
	end

BattleConnector_Room_EventScript_ExitToRoom::
	msgbox BattleConnector_Room_Text_StrongPresence, MSGBOX_DEFAULT
	closemessage
	warpsilent MAP_BATTLE_CONNECTOR_ROOM, 4, 7
	waitstate
	end

BattleConnector_Room_EventScript_ExitToResults::
	msgbox BattleConnector_Room_Text_Cleared, MSGBOX_DEFAULT
	closemessage
	warp MAP_ARTISAN_CAVE_1F, 6, 12
	waitstate
	end

BattleConnector_Room_Text_PanicAppears:
	.string "{JPN}パニックに なった ポケモンたちが\nあばれている！$"

BattleConnector_Room_Text_PanicCalmed:
	.string "{JPN}ポケモンたちは おちついて\nどこかへ かえっていった。\p{STR_VAR_1}れんさ！ いまなら {STR_VAR_2}BPです。$"

BattleConnector_Room_Text_PanicAskContinue:
	.string "{JPN}まだ パニックの ポケモンが いる……\nつづけますか？$"

BattleConnector_Room_Text_PanicNext:
	.string "{JPN}つぎの ポケモンたちが\nとびだしてきた！$"

BattleConnector_Room_Text_PanicLost:
	.string "{JPN}パニックの いきおいに\nおされてしまった……$"

BattleConnector_Room_Text_ShadowAppears:
	.string "{JPN}くろい オーラを まとった\n{STR_VAR_1}が おそいかかってきた！$"

BattleConnector_Room_Text_BossAppears:
	.string "{JPN}……ここが さいしんぶ。\pすさまじい ダークはんのう……！\p{STR_VAR_1}が こちらを\nにらみつけている！$"

BattleConnector_Room_Text_AskProtect:
	.string "{JPN}{STR_VAR_1}は ちからなく\nこちらを みている……\p{STR_VAR_1}を ほごしますか？$"

BattleConnector_Room_Text_ProtectDeclined:
	.string "{JPN}{STR_VAR_1}は\nくらやみの おくへ きえていった……$"

BattleConnector_Room_Text_ProtectJoined:
	.string "{JPN}{STR_VAR_1}を ほごした！\p{STR_VAR_1}が いっしょに\nたたかって くれるようだ！$"

BattleConnector_Room_Text_ProtectPartyFull:
	.string "{JPN}{STR_VAR_1}を ほごした！$"

BattleConnector_Room_Text_AskSwap:
	.string "{JPN}てもちが いっぱいだ。\nいれかえて いっしょに たたかいますか？$"

BattleConnector_Room_Text_Swapped:
	.string "{JPN}{STR_VAR_1}が てもちに くわわった！$"

BattleConnector_Room_Text_ProtectKept:
	.string "{JPN}{STR_VAR_1}は かえるまで\nだいじに ほごしておこう。$"

BattleConnector_Room_Text_ProtectListFull:
	.string "{JPN}これいじょうは ほごできない……$"

BattleConnector_Room_Text_Heal:
	.string "{JPN}やわらかな ひかりが\nへやを みたしている……$"

BattleConnector_Room_Text_Healed:
	.string "{JPN}てもちの ポケモンが\nげんきを とりもどした！$"

BattleConnector_Room_Text_CalmScent:
	.string "{JPN}あまく やさしい かおりが\nただよっている……$"

BattleConnector_Room_Text_Calmed:
	.string "{JPN}ダークポケモンたちの こころが\nすこし ひらいた！\pつぎの わかれみちの けはいが\nはっきり わかりそうだ。$"

BattleConnector_Room_Text_CalmNoShadow:
	.string "{JPN}こころが すっと おちついた。\pつぎの わかれみちの けはいが\nはっきり わかりそうだ。$"

BattleConnector_Room_Text_StrongPresence:
	.string "{JPN}この さきから\nひときわ つよい けはいが する……$"

BattleConnector_Room_Text_Cleared:
	.string "{JPN}ダークはんのうが しずまった……\pうけつけへ もどろう。$"
