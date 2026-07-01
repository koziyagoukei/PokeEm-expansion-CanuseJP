SootopolisCity_House3_MapScripts::
	.byte 0

SootopolisCity_House3_EventScript_Woman::
	lock
	faceplayer
	msgbox SootopolisCity_House3_Text_JuanHasManyFansDoYou, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SootopolisCity_House3_EventScript_HaveFans
	msgbox SootopolisCity_House3_Text_LonesomeTryWorkingHarder, MSGBOX_DEFAULT
	release
	end

SootopolisCity_House3_EventScript_HaveFans::
	msgbox SootopolisCity_House3_Text_YouMustBePrettyStrong, MSGBOX_DEFAULT
	release
	end

SootopolisCity_House3_EventScript_Girl::
	msgbox SootopolisCity_House3_Text_TrainerFanClubWasWild, MSGBOX_NPC
	end

SootopolisCity_House3_Text_JuanHasManyFansDoYou:
	.string "{JPN}あなた ポケモン トレーナーね?\pルネシティの アダンさまは\nおでしさんの ミクリさま より\lさらに たくさんの ファンが いるのよ!\pあなたに ファンは いるのかしら?$"

SootopolisCity_House3_Text_YouMustBePrettyStrong:
	.string "{JPN}へえー\nじゃあ けっこう つよいのね!$"

SootopolisCity_House3_Text_LonesomeTryWorkingHarder:
	.string "{JPN}あららん……\nそれは ちょっと さみしいわね\pファンが できるよう がんばりなさいよ!$"

SootopolisCity_House3_Text_TrainerFanClubWasWild:
	.string "{JPN}ねっしんな ファンは ホウエン いがいの\nちいきからも やってくるの!\pあたしが ミナモの\nトレーナー ファンクラブに いったときも\lとっても すごかったもの!$"
