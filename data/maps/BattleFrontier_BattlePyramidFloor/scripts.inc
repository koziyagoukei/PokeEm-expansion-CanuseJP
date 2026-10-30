BattleFrontier_BattlePyramidFloor_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, BattleFrontier_BattlePyramidFloor_OnResume
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_BattlePyramidFloor_OnFrame
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_BattlePyramidFloor_OnTransition
	.byte 0

BattleFrontier_BattlePyramidFloor_OnFrame:
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_LOST, BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_SAVING, BattleFrontier_BattlePyramid_EventScript_WarpToLobby
	map_script_2 VAR_TEMP_D, 1, BattleFrontier_BattlePyramidFloor_EventScript_UpdateLight
	map_script_2 VAR_TEMP_PLAYING_PYRAMID_MUSIC, 0, BattleFrontier_BattlePyramidFloor_EventScript_PlayPyramidMusic
	map_script_2 VAR_TEMP_F, 1, BattleFrontier_BattlePyramidFloor_EventScript_ShowMapName
	.2byte 0

BattleFrontier_BattlePyramidFloor_EventScript_UpdateLight::
	lockall
	@ pyramid_updatelight, cant use macro because it straddles the loop
	setvar VAR_0x8004, BATTLE_PYRAMID_FUNC_UPDATE_LIGHT
	setvar VAR_0x8005, 4
	setvar VAR_0x8006, PYRAMID_LIGHT_INCR_RADIUS
	setvar VAR_0x8007, SE_SAVE
	setvar VAR_RESULT, 0
BattleFrontier_BattlePyramidFloor_EventScript_UpdateLightLoop::
	special CallBattlePyramidFunction
	delay 2
	goto_if_ne VAR_RESULT, 2, BattleFrontier_BattlePyramidFloor_EventScript_UpdateLightLoop
	setvar VAR_TEMP_D, 0
	releaseall
	end

BattleFrontier_BattlePyramidFloor_EventScript_ShowMapName::
	special ShowMapNamePopup
	setvar VAR_TEMP_F, 0
	end

BattleFrontier_BattlePyramidFloor_EventScript_PlayPyramidMusic::
	playbgm MUS_B_PYRAMID, FALSE
	setvar VAR_TEMP_PLAYING_PYRAMID_MUSIC, 1
	end

BattleFrontier_BattlePyramidFloor_OnResume:
	pyramid_setfloorpal
	frontier_getstatus
	switch VAR_TEMP_CHALLENGE_STATUS
	case 0, BattleFrontier_BattlePyramidFloor_EventScript_ReadyChallenge
	case CHALLENGE_STATUS_SAVING, BattleFrontier_BattlePyramid_EventScript_WaitForLobbyWarp
	case CHALLENGE_STATUS_PAUSED, BattleFrontier_BattlePyramidFloor_EventScript_ReadyChallenge
	frontier_get FRONTIER_DATA_BATTLE_OUTCOME
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, BattleFrontier_BattlePyramidFloor_EventScript_ResetParty
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, BattleFrontier_BattlePyramidFloor_EventScript_ResetParty
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattlePyramidFloor_EventScript_ResetParty
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, BattleFrontier_BattlePyramid_EventScript_QueueLobbyWarpLost
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, BattleFrontier_BattlePyramid_EventScript_QueueLobbyWarpLost
	goto_if_eq VAR_RESULT, B_OUTCOME_FORFEITED, BattleFrontier_BattlePyramid_EventScript_QueueLobbyWarpLost
	frontier_isbattletype BATTLE_TYPE_TRAINER  @ VAR_RESULT seems to be ignored here
	setvar VAR_TEMP_D, 1
BattleFrontier_BattlePyramidFloor_EventScript_ResetParty::
	pyramid_resetparty
	end

@ OnResume runs immediately and cannot wait for a warp task.
BattleFrontier_BattlePyramid_EventScript_QueueLobbyWarpLost::
	setvar VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_LOST
BattleFrontier_BattlePyramid_EventScript_WaitForLobbyWarp::
	end

BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost::
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, CHALLENGE_STATUS_LOST
	pyramid_set PYRAMID_DATA_TRAINER_FLAGS, 255
BattleFrontier_BattlePyramid_EventScript_WarpToLobby::
	pyramid_updatelight 0, PYRAMID_LIGHT_SET_RADIUS
	pyramid_clearhelditems
	special HealPlayerParty
	warpsilent MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_LOBBY, 7, 13
	waitstate
	end

BattleFrontier_BattlePyramidFloor_EventScript_ReadyChallenge::
	pyramid_save CHALLENGE_STATUS_SAVING
	special SavePlayerParty
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	pyramid_settrainers
	frontier_setpartyorder FRONTIER_PARTY_SIZE
	setvar VAR_TEMP_F, 1
	end

BattleFrontier_BattlePyramidFloor_OnTransition:
	call BattleFrontier_BattlePyramidFloor_EventScript_SetLightRadius
	setvar VAR_TEMP_F, 1
	end

BattleFrontier_BattlePyramidFloor_EventScript_SetLightRadius::
	pyramid_updatelight 32, PYRAMID_LIGHT_SET_RADIUS
	return

BattlePyramid_WarpToNextFloor::
	call BattleFrontier_EventScript_IncrementWinStreak
	frontier_get FRONTIER_DATA_BATTLE_NUM  @ Floor number
	addvar VAR_RESULT, 1
	frontier_set FRONTIER_DATA_BATTLE_NUM, VAR_RESULT
	goto_if_eq VAR_RESULT, 7, BattlePyramid_WarpToTop
	pyramid_seedfloor
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	setvar VAR_RESULT, 0
	warp MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR, 1, 1
	waitstate
	end

BattlePyramid_WarpToTop::
	warp MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_TOP, 17, 17
	waitstate
	end

BattlePyramid_TrainerBattle::
	facilitytrainerbattle FACILITY_BATTLE_PYRAMID
	goto_if_eq VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_LOST, BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost
	pyramid_showhint
	waitmessage
	waitbuttonpress
	closemessage
	releaseall
	end

BattlePyramid_FindItemBall::
	pyramid_setitem
	callstd STD_FIND_ITEM
	goto_if_eq VAR_0x8007, 0, BattlePyramid_FindItemBallEnd
	pyramid_hideitem
BattlePyramid_FindItemBallEnd::
	end

BattlePyramid_Retire::
	goto BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost

@ Also used by Trainer Hill
BattleFacility_TrainerBattle_PlaceholderText:
	.string "{JPN}これは テストメッセージです$"

gText_BattlePyramidConfirmRest::
	.string "{JPN}バトルピラミッドの できごとを\nレポートに 書いて 中断 します\lよろしいですか?$"

gText_BattlePyramidConfirmRetire::
	.string "{JPN}バトルピラミッドの 挑戦を\nやめて 本当に よろしいですか?$"

BattlePyramid_Text_ExitHintUp1::
	.string "{JPN}この だんの 出口は\n{UP_ARROW}の 方向に あるんだよ$"

BattlePyramid_Text_ExitHintLeft1::
	.string "{JPN}この だんの 出口は\n{LEFT_ARROW}の 方向に あるんだよ$"

BattlePyramid_Text_ExitHintRight1::
	.string "{JPN}この だんの 出口は\n{RIGHT_ARROW}の 方向に あるんだよ$"

BattlePyramid_Text_ExitHintDown1::
	.string "{JPN}この だんの 出口は\n{DOWN_ARROW}の 方向に あるんだよ$"

BattlePyramid_Text_ExitHintUp2::
	.string "{JPN}この だんの 出口は\n{UP_ARROW}の 方向に あるの$"

BattlePyramid_Text_ExitHintLeft2::
	.string "{JPN}この だんの 出口は\n{LEFT_ARROW}の 方向に あるの$"

BattlePyramid_Text_ExitHintRight2::
	.string "{JPN}この だんの 出口は\n{RIGHT_ARROW}の 方向に あるの$"

BattlePyramid_Text_ExitHintDown2::
	.string "{JPN}この だんの 出口は\n{DOWN_ARROW}の 方向に あるの$"

BattlePyramid_Text_ExitHintUp3::
	.string "{JPN}出口は\n{UP_ARROW}の 方向に あるよ$"

BattlePyramid_Text_ExitHintLeft3::
	.string "{JPN}出口は\n{LEFT_ARROW}の 方向に あるよ$"

BattlePyramid_Text_ExitHintRight3::
	.string "{JPN}出口は\n{RIGHT_ARROW}の 方向に あるよ$"

BattlePyramid_Text_ExitHintDown3::
	.string "{JPN}出口は\n{DOWN_ARROW}の 方向に あるよ$"

BattlePyramid_Text_ExitHintUp4::
	.string "{JPN}この だんの 出口は\n{UP_ARROW}の 方向の どこかに あるわ$"

BattlePyramid_Text_ExitHintLeft4::
	.string "{JPN}この だんの 出口は\n{LEFT_ARROW}の 方向の どこかに あるわ$"

BattlePyramid_Text_ExitHintRight4::
	.string "{JPN}この だんの 出口は\n{RIGHT_ARROW}の 方向の どこかに あるわ$"

BattlePyramid_Text_ExitHintDown4::
	.string "{JPN}この だんの 出口は\n{DOWN_ARROW}の 方向の どこかに あるわ$"

BattlePyramid_Text_ExitHintUp5::
	.string "{JPN}出口は\n{UP_ARROW}の 方向に あるぞ$"

BattlePyramid_Text_ExitHintLeft5::
	.string "{JPN}出口は\n{LEFT_ARROW}の 方向に あるぞ$"

BattlePyramid_Text_ExitHintRight5::
	.string "{JPN}出口は\n{RIGHT_ARROW}の 方向に あるぞ$"

BattlePyramid_Text_ExitHintDown5::
	.string "{JPN}出口は\n{DOWN_ARROW}の 方向に あるぞ$"

BattlePyramid_Text_ExitHintUp6::
	.string "{JPN}出口は\n{UP_ARROW}の 方向に あるわね$"

BattlePyramid_Text_ExitHintLeft6::
	.string "{JPN}出口は\n{LEFT_ARROW}の 方向に あるわね$"

BattlePyramid_Text_ExitHintRight6::
	.string "{JPN}出口は\n{RIGHT_ARROW}の 方向に あるわね$"

BattlePyramid_Text_ExitHintDown6::
	.string "{JPN}出口は\n{DOWN_ARROW}の 方向に あるわね$"

BattlePyramid_Text_EightItemsRemaining1::
	.string "{JPN}道具を 探しているの?\p道具は\nあと 8つ 落ちているんだよ!$"

BattlePyramid_Text_SevenItemsRemaining1::
	.string "{JPN}道具を 探しているの?\p道具は\nあと 7つ 落ちているんだよ!$"

BattlePyramid_Text_SixItemsRemaining1::
	.string "{JPN}道具を 探しているの?\p道具は\nあと 6つ 落ちているんだよ!$"

BattlePyramid_Text_FiveItemsRemaining1::
	.string "{JPN}道具を 探しているの?\p道具は\nあと 5つ 落ちているんだよ!$"

BattlePyramid_Text_FourItemsRemaining1::
	.string "{JPN}道具を 探しているの?\p道具は\nあと 4つ 落ちているんだよ!$"

BattlePyramid_Text_ThreeItemsRemaining1::
	.string "{JPN}道具を 探しているの?\p道具は\nあと 3つ 落ちているんだよ!$"

BattlePyramid_Text_TwoItemsRemaining1::
	.string "{JPN}道具を 探しているの?\p道具は\nあと 2つ 落ちているんだよ!$"

BattlePyramid_Text_OneItemRemaining1::
	.string "{JPN}道具を 探しているの?\p道具は\nあと 1つ 落ちているんだよ!$"

BattlePyramid_Text_ZeroItemsRemaining1::
	.string "{JPN}道具を 探しているの?\p道具は\nもう 落ちていないんだよ!$"

BattlePyramid_Text_EightItemsRemaining2::
	.string "{JPN}勝った ご褒美に\nヒミツを 教えて あげちゃう!\pここには 道具が\nあと 8つ 落ちているの$"

BattlePyramid_Text_SevenItemsRemaining2::
	.string "{JPN}勝った ご褒美に\nヒミツを 教えて あげちゃう!\pここには 道具が\nあと 7つ 落ちているの$"

BattlePyramid_Text_SixItemsRemaining2::
	.string "{JPN}勝った ご褒美に\nヒミツを 教えて あげちゃう!\pここには 道具が\nあと 6つ 落ちているの$"

BattlePyramid_Text_FiveItemsRemaining2::
	.string "{JPN}勝った ご褒美に\nヒミツを 教えて あげちゃう!\pここには 道具が\nあと 5つ 落ちているの$"

BattlePyramid_Text_FourItemsRemaining2::
	.string "{JPN}勝った ご褒美に\nヒミツを 教えて あげちゃう!\pここには 道具が\nあと 4つ 落ちているの$"

BattlePyramid_Text_ThreeItemsRemaining2::
	.string "{JPN}勝った ご褒美に\nヒミツを 教えて あげちゃう!\pここには 道具が\nあと 3つ 落ちているの$"

BattlePyramid_Text_TwoItemsRemaining2::
	.string "{JPN}勝った ご褒美に\nヒミツを 教えて あげちゃう!\pここには 道具が\nあと 2つ 落ちているの$"

BattlePyramid_Text_OneItemRemaining2::
	.string "{JPN}勝った ご褒美に\nヒミツを 教えて あげちゃう!\pここには 道具が\nあと 1つ 落ちているの$"

BattlePyramid_Text_ZeroItemsRemaining2::
	.string "{JPN}勝った ご褒美に\nヒミツを 教えて あげちゃう!\pここには 道具が\nもう 落ちていないの$"

BattlePyramid_Text_EightItemsRemaining3::
	.string "{JPN}道具は たりている?\pこの だんには\n道具が あと 8つほど あるそうだよ!$"

BattlePyramid_Text_SevenItemsRemaining3::
	.string "{JPN}道具は たりている?\pこの だんには\n道具が あと 7つほど あるそうだよ!$"

BattlePyramid_Text_SixItemsRemaining3::
	.string "{JPN}道具は たりている?\pこの だんには\n道具が あと 6つほど あるそうだよ!$"

BattlePyramid_Text_FiveItemsRemaining3::
	.string "{JPN}道具は たりている?\pこの だんには\n道具が あと 5つほど あるそうだよ!$"

BattlePyramid_Text_FourItemsRemaining3::
	.string "{JPN}道具は たりている?\pこの だんには\n道具が あと 4つほど あるそうだよ!$"

BattlePyramid_Text_ThreeItemsRemaining3::
	.string "{JPN}道具は たりている?\pこの だんには\n道具が あと 3つほど あるそうだよ!$"

BattlePyramid_Text_TwoItemsRemaining3::
	.string "{JPN}道具は たりている?\pこの だんには\n道具が あと 2つほど あるそうだよ!$"

BattlePyramid_Text_OneItemRemaining3::
	.string "{JPN}道具は たりている?\pこの だんには\n道具が あと 1つだけ あるそうだよ!$"

BattlePyramid_Text_ZeroItemsRemaining3::
	.string "{JPN}道具は たりている?\pこの だんには\nもう 道具が ないらしいから\lきを つけてね!$"

BattlePyramid_Text_EightItemsRemaining4::
	.string "{JPN}強い あなたに ヒントを あげる!\p道具は\nあと 8つ 落ちているらしいわ!$"

BattlePyramid_Text_SevenItemsRemaining4::
	.string "{JPN}強い あなたに ヒントを あげる!\p道具は\nあと 7つ 落ちているらしいわ!$"

BattlePyramid_Text_SixItemsRemaining4::
	.string "{JPN}強い あなたに ヒントを あげる!\p道具は\nあと 6つ 落ちているらしいわ!$"

BattlePyramid_Text_FiveItemsRemaining4::
	.string "{JPN}強い あなたに ヒントを あげる!\p道具は\nあと 5つ 落ちているらしいわ!$"

BattlePyramid_Text_FourItemsRemaining4::
	.string "{JPN}強い あなたに ヒントを あげる!\p道具は\nあと 4つ 落ちているらしいわ!$"

BattlePyramid_Text_ThreeItemsRemaining4::
	.string "{JPN}強い あなたに ヒントを あげる!\p道具は\nあと 3つ 落ちているらしいわ!$"

BattlePyramid_Text_TwoItemsRemaining4::
	.string "{JPN}強い あなたに ヒントを あげる!\p道具は\nあと 2つ 落ちているらしいわ!$"

BattlePyramid_Text_OneItemRemaining4::
	.string "{JPN}強い あなたに ヒントを あげる!\p道具は\nあと 1つ 落ちているらしいわ!$"

BattlePyramid_Text_ZeroItemsRemaining4::
	.string "{JPN}強い あなたに ヒントを あげる!\p道具は\nもう 1つも 落ちてないらしいわ!$"

BattlePyramid_Text_EightItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\n道具が 8つは あるそうだ……$"

BattlePyramid_Text_SevenItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\n道具が 7つは あるそうだ……$"

BattlePyramid_Text_SixItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\n道具が 6つは あるそうだ……$"

BattlePyramid_Text_FiveItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\n道具が 5つは あるそうだ……$"

BattlePyramid_Text_FourItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\n道具が 4つは あるそうだ……$"

BattlePyramid_Text_ThreeItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\n道具が 3つは あるそうだ……$"

BattlePyramid_Text_TwoItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\n道具が 2つは あるそうだ……$"

BattlePyramid_Text_OneItemRemaining5::
	.string "{JPN}ピラミッドの この だんには\n道具が 1つは あるそうだ……$"

BattlePyramid_Text_ZeroItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\n道具は ないそうだ……$"

BattlePyramid_Text_EightItemsRemaining6::
	.string "{JPN}道具は 持っているかい?\pこの だんには\n道具が あと 8つは あるそうだよ$"

BattlePyramid_Text_SevenItemsRemaining6::
	.string "{JPN}道具は 持っているかい?\pこの だんには\n道具が あと 7つは あるそうだよ$"

BattlePyramid_Text_SixItemsRemaining6::
	.string "{JPN}道具は 持っているかい?\pこの だんには\n道具が あと 6つは あるそうだよ$"

BattlePyramid_Text_FiveItemsRemaining6::
	.string "{JPN}道具は 持っているかい?\pこの だんには\n道具が あと 5つは あるそうだよ$"

BattlePyramid_Text_FourItemsRemaining6::
	.string "{JPN}道具は 持っているかい?\pこの だんには\n道具が あと 4つは あるそうだよ$"

BattlePyramid_Text_ThreeItemsRemaining6::
	.string "{JPN}道具は 持っているかい?\pこの だんには\n道具が あと 3つは あるそうだよ$"

BattlePyramid_Text_TwoItemsRemaining6::
	.string "{JPN}道具は 持っているかい?\pこの だんには\n道具が あと 2つは あるそうだよ$"

BattlePyramid_Text_OneItemRemaining6::
	.string "{JPN}道具は 持っているかい?\pこの だんには\n道具が あと 1つは あるそうだよ$"

BattlePyramid_Text_ZeroItemsRemaining6::
	.string "{JPN}道具は 持っているかい?\pこの だんには\n道具が もう ないそうだよ$"

BattlePyramid_Text_SevenTrainersRemaining1::
	.string "{JPN}とっても 強かったよー!!\pでも 強い トレーナーは\nまだまだ 7人も いるからねー!$"

BattlePyramid_Text_SixTrainersRemaining1::
	.string "{JPN}とっても 強かったよー!!\pでも 強い トレーナーは\nまだまだ 6人も いるからねー!$"

BattlePyramid_Text_FiveTrainersRemaining1::
	.string "{JPN}とっても 強かったよー!!\pでも 強い トレーナーは\nまだまだ 5人も いるからねー!$"

BattlePyramid_Text_FourTrainersRemaining1::
	.string "{JPN}とっても 強かったよー!!\pでも 強い トレーナーは\nまだまだ 4人も いるからねー!$"

BattlePyramid_Text_ThreeTrainersRemaining1::
	.string "{JPN}とっても 強かったよー!!\pでも 強い トレーナーは\nまだまだ 3人も いるからねー!$"

BattlePyramid_Text_TwoTrainersRemaining1::
	.string "{JPN}とっても 強かったよー!!\pでも 強い トレーナーは\nまだ 2人 いるからねー!$"

BattlePyramid_Text_OneTrainersRemaining1::
	.string "{JPN}とっても 強かったよー!!\pでも 強い トレーナーは\nまだ 1人 いるからねー!$"

BattlePyramid_Text_ZeroTrainersRemaining1::
	.string "{JPN}とっても 強かったよー!!\pもう だれも\n勝てる ひとは いないよー!!$"

BattlePyramid_Text_SevenTrainersRemaining2::
	.string "{JPN}悔しいー!\pでも あと 7人 いる トレーナーが\nきっと 倒してくれるわ$"

BattlePyramid_Text_SixTrainersRemaining2::
	.string "{JPN}悔しいー!\pでも あと 6人 いる トレーナーが\nきっと 倒してくれるわ$"

BattlePyramid_Text_FiveTrainersRemaining2::
	.string "{JPN}悔しいー!\pでも あと 5人 いる トレーナーが\nきっと 倒してくれるわ$"

BattlePyramid_Text_FourTrainersRemaining2::
	.string "{JPN}悔しいー!\pでも あと 4人 いる トレーナーが\nきっと 倒してくれるわ$"

BattlePyramid_Text_ThreeTrainersRemaining2::
	.string "{JPN}悔しいー!\pでも あと 3人 いる トレーナーが\nきっと 倒してくれるわ$"

BattlePyramid_Text_TwoTrainersRemaining2::
	.string "{JPN}悔しいー!\pでも あと 2人 いる トレーナーが\nきっと 倒してくれるわ$"

BattlePyramid_Text_OneTrainersRemaining2::
	.string "{JPN}悔しいー!\pでも あと 1人 いる トレーナーが\nきっと 倒してくれるわ$"

BattlePyramid_Text_ZeroTrainersRemaining2::
	.string "{JPN}悔しいー!\pでも もう 戦える\nトレーナーが いないのー$"

BattlePyramid_Text_SevenTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 7人も いる\nトレーナーを 倒せるかな?$"

BattlePyramid_Text_SixTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 6人も いる\nトレーナーを 倒せるかな?$"

BattlePyramid_Text_FiveTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 5人も いる\nトレーナーを 倒せるかな?$"

BattlePyramid_Text_FourTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 4人 いる\nトレーナーを 倒せるかな?$"

BattlePyramid_Text_ThreeTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 3人 いる\nトレーナーを 倒せるかな?$"

BattlePyramid_Text_TwoTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 2人 いる\nトレーナーを 倒せるかな?$"

BattlePyramid_Text_OneTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 1人 いる\nトレーナーを 倒せるかな?$"

BattlePyramid_Text_ZeroTrainersRemaining3::
	.string "{JPN}やるねー!\pこの だんには 戦える トレーナーは\nもう だれも いないよ$"

BattlePyramid_Text_SevenTrainersRemaining4::
	.string "{JPN}あなたなら まだ 戦っていない\n7人の トレーナーを 倒せるかも……$"

BattlePyramid_Text_SixTrainersRemaining4::
	.string "{JPN}あなたなら まだ 戦っていない\n6人の トレーナーを 倒せるかも……$"

BattlePyramid_Text_FiveTrainersRemaining4::
	.string "{JPN}あなたなら まだ 戦っていない\n5人の トレーナーを 倒せるかも……$"

BattlePyramid_Text_FourTrainersRemaining4::
	.string "{JPN}あなたなら まだ 戦っていない\n4人の トレーナーを 倒せるかも……$"

BattlePyramid_Text_ThreeTrainersRemaining4::
	.string "{JPN}あなたなら まだ 戦っていない\n3人の トレーナーを 倒せるかも……$"

BattlePyramid_Text_TwoTrainersRemaining4::
	.string "{JPN}あなたなら まだ 戦っていない\n2人の トレーナーを 倒せるかも……$"

BattlePyramid_Text_OneTrainersRemaining4::
	.string "{JPN}あなたなら まだ 戦っていない\n1人の トレーナーを 倒せるかも……$"

BattlePyramid_Text_ZeroTrainersRemaining4::
	.string "{JPN}あなたを 倒せる トレーナーは\nもう だれも いないわ……$"

BattlePyramid_Text_SevenTrainersRemaining5::
	.string "{JPN}きみならば\nあと 7人 いる 凄腕 トレーナーを\l倒せるかも しれないな$"

BattlePyramid_Text_SixTrainersRemaining5::
	.string "{JPN}きみならば\nあと 6人 いる 凄腕 トレーナーを\l倒せるかも しれないな$"

BattlePyramid_Text_FiveTrainersRemaining5::
	.string "{JPN}きみならば\nあと 5人 いる 凄腕 トレーナーを\l倒せるかも しれないな$"

BattlePyramid_Text_FourTrainersRemaining5::
	.string "{JPN}きみならば\nあと 4人 いる 凄腕 トレーナーを\l倒せるかも しれないな$"

BattlePyramid_Text_ThreeTrainersRemaining5::
	.string "{JPN}きみならば\nあと 3人 いる 凄腕 トレーナーを\l倒せるかも しれないな$"

BattlePyramid_Text_TwoTrainersRemaining5::
	.string "{JPN}きみならば\nあと 2人 いる 凄腕 トレーナーを\l倒せるかも しれないな$"

BattlePyramid_Text_OneTrainersRemaining5::
	.string "{JPN}きみならば\nあと 1人 いる 凄腕 トレーナーを\l倒せるかも しれないな$"

BattlePyramid_Text_ZeroTrainersRemaining5::
	.string "{JPN}きみは 本当に 強い\pもう ここには どこにも\nきみを 倒せる トレーナーは おらんよ$"

BattlePyramid_Text_SevenTrainersRemaining6::
	.string "{JPN}残り 7人の\nトレーナーを 倒せるかい?$"

BattlePyramid_Text_SixTrainersRemaining6::
	.string "{JPN}残り 6人の\nトレーナーを 倒せるかい?$"

BattlePyramid_Text_FiveTrainersRemaining6::
	.string "{JPN}残り 5人の\nトレーナーを 倒せるかい?$"

BattlePyramid_Text_FourTrainersRemaining6::
	.string "{JPN}残り 4人の\nトレーナーを 倒せるかい?$"

BattlePyramid_Text_ThreeTrainersRemaining6::
	.string "{JPN}残り 3人の\nトレーナーを 倒せるかい?$"

BattlePyramid_Text_TwoTrainersRemaining6::
	.string "{JPN}残り 2人の\nトレーナーを 倒せるかい?$"

BattlePyramid_Text_OneTrainersRemaining6::
	.string "{JPN}残り 1人の\nトレーナーを 倒せるかい?$"

BattlePyramid_Text_ZeroTrainersRemaining6::
	.string "{JPN}ここには あんたと 戦える\nトレーナーは もう いないよ……$"
