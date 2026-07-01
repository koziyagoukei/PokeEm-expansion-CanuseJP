BattleFrontier_BattlePyramidFloor_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, BattleFrontier_BattlePyramidFloor_OnResume
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_BattlePyramidFloor_OnFrame
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_BattlePyramidFloor_OnTransition
	.byte 0

BattleFrontier_BattlePyramidFloor_OnFrame:
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
	case CHALLENGE_STATUS_SAVING, BattleFrontier_BattlePyramid_EventScript_WarpToLobby
	case CHALLENGE_STATUS_PAUSED, BattleFrontier_BattlePyramidFloor_EventScript_ReadyChallenge
	frontier_get FRONTIER_DATA_BATTLE_OUTCOME
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, BattleFrontier_BattlePyramidFloor_EventScript_ResetParty
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, BattleFrontier_BattlePyramidFloor_EventScript_ResetParty
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattlePyramidFloor_EventScript_ResetParty
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost
	goto_if_eq VAR_RESULT, B_OUTCOME_FORFEITED, BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost
	frontier_isbattletype BATTLE_TYPE_TRAINER  @ VAR_RESULT seems to be ignored here
	setvar VAR_TEMP_D, 1
BattleFrontier_BattlePyramidFloor_EventScript_ResetParty::
	pyramid_resetparty
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

@ TRAINER_PHILLIP is used as a placeholder
BattlePyramid_TrainerBattle::
	facilitytrainerbattle FACILITY_BATTLE_PYRAMID
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
	.string "This is a sample message.$"

gText_BattlePyramidConfirmRest::
	.string "{JPN}バトルピラミッドの できごとを\nレポートに かいて ちゅうだん します\lよろしいですか?$"

gText_BattlePyramidConfirmRetire::
	.string "{JPN}バトルピラミッドの ちょうせんを\nやめて ほんとうに よろしいですか?$"

BattlePyramid_Text_ExitHintUp1::
	.string "{JPN}この だんの でぐちは\n{UP_ARROW}の ほうこうに あるんだよ$"

BattlePyramid_Text_ExitHintLeft1::
	.string "{JPN}この だんの でぐちは\n{LEFT_ARROW}の ほうこうに あるんだよ$"

BattlePyramid_Text_ExitHintRight1::
	.string "{JPN}この だんの でぐちは\n{RIGHT_ARROW}の ほうこうに あるんだよ$"

BattlePyramid_Text_ExitHintDown1::
	.string "{JPN}この だんの でぐちは\n{DOWN_ARROW}の ほうこうに あるんだよ$"

BattlePyramid_Text_ExitHintUp2::
	.string "{JPN}この だんの でぐちは\n{UP_ARROW}の ほうこうに あるの$"

BattlePyramid_Text_ExitHintLeft2::
	.string "{JPN}この だんの でぐちは\n{LEFT_ARROW}の ほうこうに あるの$"

BattlePyramid_Text_ExitHintRight2::
	.string "{JPN}この だんの でぐちは\n{RIGHT_ARROW}の ほうこうに あるの$"

BattlePyramid_Text_ExitHintDown2::
	.string "{JPN}この だんの でぐちは\n{DOWN_ARROW}の ほうこうに あるの$"

BattlePyramid_Text_ExitHintUp3::
	.string "{JPN}でぐちは\n{UP_ARROW}の ほうこうに あるよ$"

BattlePyramid_Text_ExitHintLeft3::
	.string "{JPN}でぐちは\n{LEFT_ARROW}の ほうこうに あるよ$"

BattlePyramid_Text_ExitHintRight3::
	.string "{JPN}でぐちは\n{RIGHT_ARROW}の ほうこうに あるよ$"

BattlePyramid_Text_ExitHintDown3::
	.string "{JPN}でぐちは\n{DOWN_ARROW}の ほうこうに あるよ$"

BattlePyramid_Text_ExitHintUp4::
	.string "{JPN}この だんの でぐちは\n{UP_ARROW}の ほうこうの どこかに あるわ$"

BattlePyramid_Text_ExitHintLeft4::
	.string "{JPN}この だんの でぐちは\n{LEFT_ARROW}の ほうこうの どこかに あるわ$"

BattlePyramid_Text_ExitHintRight4::
	.string "{JPN}この だんの でぐちは\n{RIGHT_ARROW}の ほうこうの どこかに あるわ$"

BattlePyramid_Text_ExitHintDown4::
	.string "{JPN}この だんの でぐちは\n{DOWN_ARROW}の ほうこうの どこかに あるわ$"

BattlePyramid_Text_ExitHintUp5::
	.string "{JPN}でぐちは\n{UP_ARROW}の ほうこうに あるぞ$"

BattlePyramid_Text_ExitHintLeft5::
	.string "{JPN}でぐちは\n{LEFT_ARROW}の ほうこうに あるぞ$"

BattlePyramid_Text_ExitHintRight5::
	.string "{JPN}でぐちは\n{RIGHT_ARROW}の ほうこうに あるぞ$"

BattlePyramid_Text_ExitHintDown5::
	.string "{JPN}でぐちは\n{DOWN_ARROW}の ほうこうに あるぞ$"

BattlePyramid_Text_ExitHintUp6::
	.string "{JPN}でぐちは\n{UP_ARROW}の ほうこうに あるわね$"

BattlePyramid_Text_ExitHintLeft6::
	.string "{JPN}でぐちは\n{LEFT_ARROW}の ほうこうに あるわね$"

BattlePyramid_Text_ExitHintRight6::
	.string "{JPN}でぐちは\n{RIGHT_ARROW}の ほうこうに あるわね$"

BattlePyramid_Text_ExitHintDown6::
	.string "{JPN}でぐちは\n{DOWN_ARROW}の ほうこうに あるわね$"

BattlePyramid_Text_EightItemsRemaining1::
	.string "{JPN}どうぐを さがしているの?\pどうぐは\nあと 8つ おちているんだよ!$"

BattlePyramid_Text_SevenItemsRemaining1::
	.string "{JPN}どうぐを さがしているの?\pどうぐは\nあと 7つ おちているんだよ!$"

BattlePyramid_Text_SixItemsRemaining1::
	.string "{JPN}どうぐを さがしているの?\pどうぐは\nあと 6つ おちているんだよ!$"

BattlePyramid_Text_FiveItemsRemaining1::
	.string "{JPN}どうぐを さがしているの?\pどうぐは\nあと 5つ おちているんだよ!$"

BattlePyramid_Text_FourItemsRemaining1::
	.string "{JPN}どうぐを さがしているの?\pどうぐは\nあと 4つ おちているんだよ!$"

BattlePyramid_Text_ThreeItemsRemaining1::
	.string "{JPN}どうぐを さがしているの?\pどうぐは\nあと 3つ おちているんだよ!$"

BattlePyramid_Text_TwoItemsRemaining1::
	.string "{JPN}どうぐを さがしているの?\pどうぐは\nあと 2つ おちているんだよ!$"

BattlePyramid_Text_OneItemRemaining1::
	.string "{JPN}どうぐを さがしているの?\pどうぐは\nあと 1つ おちているんだよ!$"

BattlePyramid_Text_ZeroItemsRemaining1::
	.string "{JPN}どうぐを さがしているの?\pどうぐは\nもう おちていないんだよ!$"

BattlePyramid_Text_EightItemsRemaining2::
	.string "{JPN}かった ごほうびに\nヒミツを おしえて あげちゃう!\pここには どうぐが\nあと 8つ おちているの$"

BattlePyramid_Text_SevenItemsRemaining2::
	.string "{JPN}かった ごほうびに\nヒミツを おしえて あげちゃう!\pここには どうぐが\nあと 7つ おちているの$"

BattlePyramid_Text_SixItemsRemaining2::
	.string "{JPN}かった ごほうびに\nヒミツを おしえて あげちゃう!\pここには どうぐが\nあと 6つ おちているの$"

BattlePyramid_Text_FiveItemsRemaining2::
	.string "{JPN}かった ごほうびに\nヒミツを おしえて あげちゃう!\pここには どうぐが\nあと 5つ おちているの$"

BattlePyramid_Text_FourItemsRemaining2::
	.string "{JPN}かった ごほうびに\nヒミツを おしえて あげちゃう!\pここには どうぐが\nあと 4つ おちているの$"

BattlePyramid_Text_ThreeItemsRemaining2::
	.string "{JPN}かった ごほうびに\nヒミツを おしえて あげちゃう!\pここには どうぐが\nあと 3つ おちているの$"

BattlePyramid_Text_TwoItemsRemaining2::
	.string "{JPN}かった ごほうびに\nヒミツを おしえて あげちゃう!\pここには どうぐが\nあと 2つ おちているの$"

BattlePyramid_Text_OneItemRemaining2::
	.string "{JPN}かった ごほうびに\nヒミツを おしえて あげちゃう!\pここには どうぐが\nあと 1つ おちているの$"

BattlePyramid_Text_ZeroItemsRemaining2::
	.string "{JPN}かった ごほうびに\nヒミツを おしえて あげちゃう!\pここには どうぐが\nもう おちていないの$"

BattlePyramid_Text_EightItemsRemaining3::
	.string "{JPN}どうぐは たりている?\pこの だんには\nどうぐが あと 8つほど あるそうだよ!$"

BattlePyramid_Text_SevenItemsRemaining3::
	.string "{JPN}どうぐは たりている?\pこの だんには\nどうぐが あと 7つほど あるそうだよ!$"

BattlePyramid_Text_SixItemsRemaining3::
	.string "{JPN}どうぐは たりている?\pこの だんには\nどうぐが あと 6つほど あるそうだよ!$"

BattlePyramid_Text_FiveItemsRemaining3::
	.string "{JPN}どうぐは たりている?\pこの だんには\nどうぐが あと 5つほど あるそうだよ!$"

BattlePyramid_Text_FourItemsRemaining3::
	.string "{JPN}どうぐは たりている?\pこの だんには\nどうぐが あと 4つほど あるそうだよ!$"

BattlePyramid_Text_ThreeItemsRemaining3::
	.string "{JPN}どうぐは たりている?\pこの だんには\nどうぐが あと 3つほど あるそうだよ!$"

BattlePyramid_Text_TwoItemsRemaining3::
	.string "{JPN}どうぐは たりている?\pこの だんには\nどうぐが あと 2つほど あるそうだよ!$"

BattlePyramid_Text_OneItemRemaining3::
	.string "{JPN}どうぐは たりている?\pこの だんには\nどうぐが あと 1つだけ あるそうだよ!$"

BattlePyramid_Text_ZeroItemsRemaining3::
	.string "{JPN}どうぐは たりている?\pこの だんには\nもう どうぐが ないらしいから\lきを つけてね!$"

BattlePyramid_Text_EightItemsRemaining4::
	.string "{JPN}つよい あなたに ヒントを あげる!\pどうぐは\nあと 8つ おちているらしいわ!$"

BattlePyramid_Text_SevenItemsRemaining4::
	.string "{JPN}つよい あなたに ヒントを あげる!\pどうぐは\nあと 7つ おちているらしいわ!$"

BattlePyramid_Text_SixItemsRemaining4::
	.string "{JPN}つよい あなたに ヒントを あげる!\pどうぐは\nあと 6つ おちているらしいわ!$"

BattlePyramid_Text_FiveItemsRemaining4::
	.string "{JPN}つよい あなたに ヒントを あげる!\pどうぐは\nあと 5つ おちているらしいわ!$"

BattlePyramid_Text_FourItemsRemaining4::
	.string "{JPN}つよい あなたに ヒントを あげる!\pどうぐは\nあと 4つ おちているらしいわ!$"

BattlePyramid_Text_ThreeItemsRemaining4::
	.string "{JPN}つよい あなたに ヒントを あげる!\pどうぐは\nあと 3つ おちているらしいわ!$"

BattlePyramid_Text_TwoItemsRemaining4::
	.string "{JPN}つよい あなたに ヒントを あげる!\pどうぐは\nあと 2つ おちているらしいわ!$"

BattlePyramid_Text_OneItemRemaining4::
	.string "{JPN}つよい あなたに ヒントを あげる!\pどうぐは\nあと 1つ おちているらしいわ!$"

BattlePyramid_Text_ZeroItemsRemaining4::
	.string "{JPN}つよい あなたに ヒントを あげる!\pどうぐは\nもう 1つも おちてないらしいわ!$"

BattlePyramid_Text_EightItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\nどうぐが 8つは あるそうだ……$"

BattlePyramid_Text_SevenItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\nどうぐが 7つは あるそうだ……$"

BattlePyramid_Text_SixItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\nどうぐが 6つは あるそうだ……$"

BattlePyramid_Text_FiveItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\nどうぐが 5つは あるそうだ……$"

BattlePyramid_Text_FourItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\nどうぐが 4つは あるそうだ……$"

BattlePyramid_Text_ThreeItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\nどうぐが 3つは あるそうだ……$"

BattlePyramid_Text_TwoItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\nどうぐが 2つは あるそうだ……$"

BattlePyramid_Text_OneItemRemaining5::
	.string "{JPN}ピラミッドの この だんには\nどうぐが 1つは あるそうだ……$"

BattlePyramid_Text_ZeroItemsRemaining5::
	.string "{JPN}ピラミッドの この だんには\nどうぐは ないそうだ……$"

BattlePyramid_Text_EightItemsRemaining6::
	.string "{JPN}どうぐは もっているかい?\pこの だんには\nどうぐが あと 8つは あるそうだよ$"

BattlePyramid_Text_SevenItemsRemaining6::
	.string "{JPN}どうぐは もっているかい?\pこの だんには\nどうぐが あと 7つは あるそうだよ$"

BattlePyramid_Text_SixItemsRemaining6::
	.string "{JPN}どうぐは もっているかい?\pこの だんには\nどうぐが あと 6つは あるそうだよ$"

BattlePyramid_Text_FiveItemsRemaining6::
	.string "{JPN}どうぐは もっているかい?\pこの だんには\nどうぐが あと 5つは あるそうだよ$"

BattlePyramid_Text_FourItemsRemaining6::
	.string "{JPN}どうぐは もっているかい?\pこの だんには\nどうぐが あと 4つは あるそうだよ$"

BattlePyramid_Text_ThreeItemsRemaining6::
	.string "{JPN}どうぐは もっているかい?\pこの だんには\nどうぐが あと 3つは あるそうだよ$"

BattlePyramid_Text_TwoItemsRemaining6::
	.string "{JPN}どうぐは もっているかい?\pこの だんには\nどうぐが あと 2つは あるそうだよ$"

BattlePyramid_Text_OneItemRemaining6::
	.string "{JPN}どうぐは もっているかい?\pこの だんには\nどうぐが あと 1つは あるそうだよ$"

BattlePyramid_Text_ZeroItemsRemaining6::
	.string "{JPN}どうぐは もっているかい?\pこの だんには\nどうぐが もう ないそうだよ$"

BattlePyramid_Text_SevenTrainersRemaining1::
	.string "{JPN}とっても つよかったよー!!\pでも つよい トレーナーは\nまだまだ 7にんも いるからねー!$"

BattlePyramid_Text_SixTrainersRemaining1::
	.string "{JPN}とっても つよかったよー!!\pでも つよい トレーナーは\nまだまだ 6にんも いるからねー!$"

BattlePyramid_Text_FiveTrainersRemaining1::
	.string "{JPN}とっても つよかったよー!!\pでも つよい トレーナーは\nまだまだ 5にんも いるからねー!$"

BattlePyramid_Text_FourTrainersRemaining1::
	.string "{JPN}とっても つよかったよー!!\pでも つよい トレーナーは\nまだまだ 4にんも いるからねー!$"

BattlePyramid_Text_ThreeTrainersRemaining1::
	.string "{JPN}とっても つよかったよー!!\pでも つよい トレーナーは\nまだまだ 3にんも いるからねー!$"

BattlePyramid_Text_TwoTrainersRemaining1::
	.string "{JPN}とっても つよかったよー!!\pでも つよい トレーナーは\nまだ 2にん いるからねー!$"

BattlePyramid_Text_OneTrainersRemaining1::
	.string "{JPN}とっても つよかったよー!!\pでも つよい トレーナーは\nまだ 2にん いるからねー!$"

BattlePyramid_Text_ZeroTrainersRemaining1::
	.string "{JPN}とっても つよかったよー!!\pもう だれも\nかてる ひとは いないよー!!$"

BattlePyramid_Text_SevenTrainersRemaining2::
	.string "{JPN}くやしいー!\pでも あと 7にん いる トレーナーが\nきっと たおしてくれるわ$"

BattlePyramid_Text_SixTrainersRemaining2::
	.string "{JPN}くやしいー!\pでも あと 6にん いる トレーナーが\nきっと たおしてくれるわ$"

BattlePyramid_Text_FiveTrainersRemaining2::
	.string "{JPN}くやしいー!\pでも あと 5にん いる トレーナーが\nきっと たおしてくれるわ$"

BattlePyramid_Text_FourTrainersRemaining2::
	.string "{JPN}くやしいー!\pでも あと 4にん いる トレーナーが\nきっと たおしてくれるわ$"

BattlePyramid_Text_ThreeTrainersRemaining2::
	.string "{JPN}くやしいー!\pでも あと 3にん いる トレーナーが\nきっと たおしてくれるわ$"

BattlePyramid_Text_TwoTrainersRemaining2::
	.string "{JPN}くやしいー!\pでも あと 2にん いる トレーナーが\nきっと たおしてくれるわ$"

BattlePyramid_Text_OneTrainersRemaining2::
	.string "{JPN}くやしいー!\pでも あと 1にん いる トレーナーが\nきっと たおしてくれるわ$"

BattlePyramid_Text_ZeroTrainersRemaining2::
	.string "{JPN}くやしいー!\pでも もう たたかえる\nトレーナーが いないのー$"

BattlePyramid_Text_SevenTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 7にんも いる\nトレーナーを たおせるかな?$"

BattlePyramid_Text_SixTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 6にんも いる\nトレーナーを たおせるかな?$"

BattlePyramid_Text_FiveTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 5にんも いる\nトレーナーを たおせるかな?$"

BattlePyramid_Text_FourTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 4にん いる\nトレーナーを たおせるかな?$"

BattlePyramid_Text_ThreeTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 3にん いる\nトレーナーを たおせるかな?$"

BattlePyramid_Text_TwoTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 2にん いる\nトレーナーを たおせるかな?$"

BattlePyramid_Text_OneTrainersRemaining3::
	.string "{JPN}やるねー!\pでも この だんに あと 1にん いる\nトレーナーを たおせるかな?$"

BattlePyramid_Text_ZeroTrainersRemaining3::
	.string "{JPN}やるねー!\pこの だんには たたかえる トレーナーは\nもう だれも いないよ$"

BattlePyramid_Text_SevenTrainersRemaining4::
	.string "{JPN}あなたなら まだ たたかっていない\n7にんの トレーナーを たおせるかも……$"

BattlePyramid_Text_SixTrainersRemaining4::
	.string "{JPN}あなたなら まだ たたかっていない\n6にんの トレーナーを たおせるかも……$"

BattlePyramid_Text_FiveTrainersRemaining4::
	.string "{JPN}あなたなら まだ たたかっていない\n5にんの トレーナーを たおせるかも……$"

BattlePyramid_Text_FourTrainersRemaining4::
	.string "{JPN}あなたなら まだ たたかっていない\n4にんの トレーナーを たおせるかも……$"

BattlePyramid_Text_ThreeTrainersRemaining4::
	.string "{JPN}あなたなら まだ たたかっていない\n3にんの トレーナーを たおせるかも……$"

BattlePyramid_Text_TwoTrainersRemaining4::
	.string "{JPN}あなたなら まだ たたかっていない\n2にんの トレーナーを たおせるかも……$"

BattlePyramid_Text_OneTrainersRemaining4::
	.string "{JPN}あなたなら まだ たたかっていない\n1にんの トレーナーを たおせるかも……$"

BattlePyramid_Text_ZeroTrainersRemaining4::
	.string "{JPN}あなたを たおせる トレーナーは\nもう だれも いないわ……$"

BattlePyramid_Text_SevenTrainersRemaining5::
	.string "{JPN}きみならば\nあと 7にん いる すごうで トレーナーを\lたおせるかも しれないな$"

BattlePyramid_Text_SixTrainersRemaining5::
	.string "{JPN}きみならば\nあと 6にん いる すごうで トレーナーを\lたおせるかも しれないな$"

BattlePyramid_Text_FiveTrainersRemaining5::
	.string "{JPN}きみならば\nあと 5にん いる すごうで トレーナーを\lたおせるかも しれないな$"

BattlePyramid_Text_FourTrainersRemaining5::
	.string "{JPN}きみならば\nあと 4にん いる すごうで トレーナーを\lたおせるかも しれないな$"

BattlePyramid_Text_ThreeTrainersRemaining5::
	.string "{JPN}きみならば\nあと 3にん いる すごうで トレーナーを\lたおせるかも しれないな$"

BattlePyramid_Text_TwoTrainersRemaining5::
	.string "{JPN}きみならば\nあと 2にん いる すごうで トレーナーを\lたおせるかも しれないな$"

BattlePyramid_Text_OneTrainersRemaining5::
	.string "{JPN}きみならば\nあと 1にん いる すごうで トレーナーを\lたおせるかも しれないな$"

BattlePyramid_Text_ZeroTrainersRemaining5::
	.string "{JPN}きみは ほんとうに つよい\pもう ここには どこにも\nきみを たおせる トレーナーは おらんよ$"

BattlePyramid_Text_SevenTrainersRemaining6::
	.string "{JPN}のこり 7にんの\nトレーナーを たおせるかい?$"

BattlePyramid_Text_SixTrainersRemaining6::
	.string "{JPN}のこり 6にんの\nトレーナーを たおせるかい?$"

BattlePyramid_Text_FiveTrainersRemaining6::
	.string "{JPN}のこり 5にんの\nトレーナーを たおせるかい?$"

BattlePyramid_Text_FourTrainersRemaining6::
	.string "{JPN}のこり 4にんの\nトレーナーを たおせるかい?$"

BattlePyramid_Text_ThreeTrainersRemaining6::
	.string "{JPN}のこり 3にんの\nトレーナーを たおせるかい?$"

BattlePyramid_Text_TwoTrainersRemaining6::
	.string "{JPN}のこり 2にんの\nトレーナーを たおせるかい?$"

BattlePyramid_Text_OneTrainersRemaining6::
	.string "{JPN}のこり 1にんの\nトレーナーを たおせるかい?$"

BattlePyramid_Text_ZeroTrainersRemaining6::
	.string "{JPN}ここには あんたと たたかえる\nトレーナーは もう いないよ……$"
