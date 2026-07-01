VictoryRoad_B2F_MapScripts::
	.byte 0

VictoryRoad_B2F_EventScript_Vito::
	trainerbattle_single TRAINER_VITO, VictoryRoad_B2F_Text_VitoIntro, VictoryRoad_B2F_Text_VitoDefeat
	msgbox VictoryRoad_B2F_Text_VitoPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Owen::
	trainerbattle_single TRAINER_OWEN, VictoryRoad_B2F_Text_OwenIntro, VictoryRoad_B2F_Text_OwenDefeat
	msgbox VictoryRoad_B2F_Text_OwenPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Caroline::
	trainerbattle_single TRAINER_CAROLINE, VictoryRoad_B2F_Text_CarolineIntro, VictoryRoad_B2F_Text_CarolineDefeat
	msgbox VictoryRoad_B2F_Text_CarolinePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Julie::
	trainerbattle_single TRAINER_JULIE, VictoryRoad_B2F_Text_JulieIntro, VictoryRoad_B2F_Text_JulieDefeat
	msgbox VictoryRoad_B2F_Text_JuliePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Felix::
	trainerbattle_single TRAINER_FELIX, VictoryRoad_B2F_Text_FelixIntro, VictoryRoad_B2F_Text_FelixDefeat
	msgbox VictoryRoad_B2F_Text_FelixPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Dianne::
	trainerbattle_single TRAINER_DIANNE, VictoryRoad_B2F_Text_DianneIntro, VictoryRoad_B2F_Text_DianneDefeat
	msgbox VictoryRoad_B2F_Text_DiannePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_Text_VitoIntro:
	.string "{JPN}かぞく ぜんいんで\nポケモンの しゅぎょうを してたんだ!\lだれにも まけないぜ!$"

VictoryRoad_B2F_Text_VitoDefeat:
	.string "{JPN}おれの かぞく より つよい!?\nそんなことって あるのか!?$"

VictoryRoad_B2F_Text_VitoPostBattle:
	.string "{JPN}かぞくの だれよりも つよかった おれ……\nいままで だれにも まけなかった おれ……\pだけど じしん なくしちゃったよ\nいえに かえろうかなあ$"

VictoryRoad_B2F_Text_OwenIntro:
	.string "{JPN}つよい こどもが いるって ウワサだ!\nそれって きみの ことかい?$"

VictoryRoad_B2F_Text_OwenDefeat:
	.string "{JPN}たしかに つよい!$"

VictoryRoad_B2F_Text_OwenPostBattle:
	.string "{JPN}おれが きいた ウワサだと\nその つよい こども ってのは\lトウカ しゅっしん らしいな!$"

VictoryRoad_B2F_Text_CarolineIntro:
	.string "{JPN}そろそろ へばって きたかな?$"

VictoryRoad_B2F_Text_CarolineDefeat:
	.string "{JPN}ぜんぜん へばってないわ!$"

VictoryRoad_B2F_Text_CarolinePostBattle:
	.string "{JPN}この チャンピオンロードも\nポケモンリーグも ながちょうば!\lたいりょくには ちゅうい してね!$"

VictoryRoad_B2F_Text_JulieIntro:
	.string "{JPN}ジムバッジを たくさん もってるからって\nゆだん しないでよ\lうえには うえが いるんだから!$"

VictoryRoad_B2F_Text_JulieDefeat:
	.string "{JPN}あなたの ほうが うえね!$"

VictoryRoad_B2F_Text_JuliePostBattle:
	.string "{JPN}じぶんの あつめた ジムバッジを みて\nいままでの トレーナーを おもいだすのよ$"

VictoryRoad_B2F_Text_FelixIntro:
	.string "{JPN}ここまで きて\nきんちょう して おなかが いたいんです!$"

VictoryRoad_B2F_Text_FelixDefeat:
	.string "{JPN}ああ\n……いたい$"

VictoryRoad_B2F_Text_FelixPostBattle:
	.string "{JPN}もうすぐ ポケモンリーグと おもうと\nきんちょう して きんちょう して!\lみため ごまかすのが せいいっぱい です$"

VictoryRoad_B2F_Text_DianneIntro:
	.string "{JPN}エリート ちゅうの エリートが あつまる\nこの どうくつは どうかしら?$"

VictoryRoad_B2F_Text_DianneDefeat:
	.string "{JPN}ぜんぜん どうじて いないわ……!$"

VictoryRoad_B2F_Text_DiannePostBattle:
	.string "{JPN}あなた どきょうが あるのね\nきにいったわ がんばって!$"
