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
	.string "かぞく ぜんいんで\nポケモンの しゅぎょうを してたんだ!\lだれにも まけないぜ!$"

VictoryRoad_B2F_Text_VitoDefeat:
	.string "おれの かぞく より つよい!?\nそんなことって あるのか!?$"

VictoryRoad_B2F_Text_VitoPostBattle:
	.string "かぞくの だれよりも つよかった おれ……\nいままで だれにも まけなかった おれ……\pだけど じしん なくしちゃったよ\nいえに かえろうかなあ$"

VictoryRoad_B2F_Text_OwenIntro:
	.string "つよい こどもが いるって ウワサだ!\nそれって きみの ことかい?$"

VictoryRoad_B2F_Text_OwenDefeat:
	.string "たしかに つよい!$"

VictoryRoad_B2F_Text_OwenPostBattle:
	.string "おれが きいた ウワサだと\nその つよい こども ってのは\lトウカ しゅっしん らしいな!$"

VictoryRoad_B2F_Text_CarolineIntro:
	.string "そろそろ へばって きたかな?$"

VictoryRoad_B2F_Text_CarolineDefeat:
	.string "ぜんぜん へばってないわ!$"

VictoryRoad_B2F_Text_CarolinePostBattle:
	.string "この チャンピオンロードも\nポケモンリーグも ながちょうば!\lたいりょくには ちゅうい してね!$"

VictoryRoad_B2F_Text_JulieIntro:
	.string "ジムバッジを たくさん もってるからって\nゆだん しないでよ\lうえには うえが いるんだから!$"

VictoryRoad_B2F_Text_JulieDefeat:
	.string "あなたの ほうが うえね!$"

VictoryRoad_B2F_Text_JuliePostBattle:
	.string "じぶんの あつめた ジムバッジを みて\nいままでの トレーナーを おもいだすのよ$"

VictoryRoad_B2F_Text_FelixIntro:
	.string "ここまで きて\nきんちょう して おなかが いたいんです!$"

VictoryRoad_B2F_Text_FelixDefeat:
	.string "ああ\n……いたい$"

VictoryRoad_B2F_Text_FelixPostBattle:
	.string "もうすぐ ポケモンリーグと おもうと\nきんちょう して きんちょう して!\lみため ごまかすのが せいいっぱい です$"

VictoryRoad_B2F_Text_DianneIntro:
	.string "エリート ちゅうの エリートが あつまる\nこの どうくつは どうかしら?$"

VictoryRoad_B2F_Text_DianneDefeat:
	.string "ぜんぜん どうじて いないわ……!$"

VictoryRoad_B2F_Text_DiannePostBattle:
	.string "あなた どきょうが あるのね\nきにいったわ がんばって!$"
