PacifidlogTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PacifidlogTown_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, PacifidlogTown_OnResume
	.byte 0

PacifidlogTown_OnTransition:
	setflag FLAG_VISITED_PACIFIDLOG_TOWN
	end

PacifidlogTown_OnResume:
	setstepcallback STEP_CB_PACIFIDLOG_BRIDGE
	end

PacifidlogTown_EventScript_NinjaBoy::
	msgbox PacifidlogTown_Text_NeatHousesOnWater, MSGBOX_NPC
	end

PacifidlogTown_EventScript_Girl::
	msgbox PacifidlogTown_Text_FastRunningCurrent, MSGBOX_NPC
	end

PacifidlogTown_EventScript_Fisherman::
	msgbox PacifidlogTown_Text_SkyPillarTooScary, MSGBOX_NPC
	end

PacifidlogTown_EventScript_TownSign::
	msgbox PacifidlogTown_Text_TownSign, MSGBOX_SIGN
	end

PacifidlogTown_Text_FastRunningCurrent:
	.string "{JPN}キナギから カイナへは\nしおの ながれが とても はげしいの\pうかつに なみのり すると\nおもわぬ ほうこうに ながされちゃう$"

PacifidlogTown_Text_NeatHousesOnWater:
	.string "{JPN}ね! ね! すごいでしょ!\nみずの うえに おうちが たってるの!\pぼく ここで うまれたんだよ!$"

PacifidlogTown_Text_SkyPillarTooScary:
	.string "{JPN}そらのはしら?\p……ああ このさきに たっている\nたかーーい とうの ことだね\pあんな たかい ところ\nおっかなくて のぼれないなー\pぼくは やっぱり キナギでの\nすいめん スレスレな せいかつが\lはだに あうんだよねえー$"

PacifidlogTown_Text_TownSign:
	.string "{JPN}ここは キナギ タウン\nあさひが みずべを てらす まち$"
