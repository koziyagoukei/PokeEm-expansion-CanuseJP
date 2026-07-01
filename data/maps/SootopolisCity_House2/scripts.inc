SootopolisCity_House2_MapScripts::
	.byte 0

SootopolisCity_House2_EventScript_ExpertF::
	lock
	faceplayer
	msgbox SootopolisCity_House2_Text_DidYouKnowAboutMtPyreOrbs, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, SootopolisCity_House2_EventScript_KnowAboutOrbs
	call_if_eq VAR_RESULT, NO, SootopolisCity_House2_EventScript_DontKnowAboutOrbs
	release
	end

SootopolisCity_House2_EventScript_KnowAboutOrbs::
	msgbox SootopolisCity_House2_Text_YesTwoOrbsSideBySide, MSGBOX_DEFAULT
	return

SootopolisCity_House2_EventScript_DontKnowAboutOrbs::
	msgbox SootopolisCity_House2_Text_OughtToVisitAndSee, MSGBOX_DEFAULT
	return

SootopolisCity_House2_Text_DidYouKnowAboutMtPyreOrbs:
	.string "{JPN}おくりびやま……\pちょうじょうに ふたつのたまが\nおかれて いるのは ごぞんじ ですか……?$"

SootopolisCity_House2_Text_YesTwoOrbsSideBySide:
	.string "{JPN}そう…… ふたつのたまが\nとなりあわせに おかれている ようすは\lなにか こころが やすらぎますよ……$"

SootopolisCity_House2_Text_OughtToVisitAndSee:
	.string "{JPN}そうですか……\nいちど いかれては どうでしょう……$"
