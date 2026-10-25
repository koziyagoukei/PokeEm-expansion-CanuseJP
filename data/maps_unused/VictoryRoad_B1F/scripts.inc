VictoryRoad_B1F_MapScripts::
	.byte 0

VictoryRoad_B1F_EventScript_Samuel::
	trainerbattle_single TRAINER_SAMUEL, VictoryRoad_B1F_Text_SamuelIntro, VictoryRoad_B1F_Text_SamuelDefeat
	msgbox VictoryRoad_B1F_Text_SamuelPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Shannon::
	trainerbattle_single TRAINER_SHANNON, VictoryRoad_B1F_Text_ShannonIntro, VictoryRoad_B1F_Text_ShannonDefeat
	msgbox VictoryRoad_B1F_Text_ShannonPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Michelle::
	trainerbattle_single TRAINER_MICHELLE, VictoryRoad_B1F_Text_MichelleIntro, VictoryRoad_B1F_Text_MichelleDefeat
	msgbox VictoryRoad_B1F_Text_MichellePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Mitchell::
	trainerbattle_single TRAINER_MITCHELL, VictoryRoad_B1F_Text_MitchellIntro, VictoryRoad_B1F_Text_MitchellDefeat
	msgbox VictoryRoad_B1F_Text_MitchellPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Halle::
	trainerbattle_single TRAINER_HALLE, VictoryRoad_B1F_Text_HalleIntro, VictoryRoad_B1F_Text_HalleDefeat
	msgbox VictoryRoad_B1F_Text_HallePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_Text_SamuelIntro:
	.string "ポケモンリーグに ちかづいていると おもうと\nなんだか ドキドキ してきたよ……$"

VictoryRoad_B1F_Text_SamuelDefeat:
	.string "なにも できなかった……$"

VictoryRoad_B1F_Text_SamuelPostBattle:
	.string "ポケモンリーグが とおのいたよ……\nなんだか がっかり するな……$"

VictoryRoad_B1F_Text_ShannonIntro:
	.string "ポケモンリーグで かちぬくには\nポケモンへの しんらいが ひつよう なの$"

VictoryRoad_B1F_Text_ShannonDefeat:
	.string "しんらい かんけい バッチリね!$"

VictoryRoad_B1F_Text_ShannonPostBattle:
	.string "いつも いっしょに いることで\nポケモンと トレーナーとの あいだに\lしんらい かんけいが できるの$"

VictoryRoad_B1F_Text_MichelleIntro:
	.string "ここは ゴール じゃないの\nポケモンリーグへの つうかてん よ!$"

VictoryRoad_B1F_Text_MichelleDefeat:
	.string "その ちょうし!$"

VictoryRoad_B1F_Text_MichellePostBattle:
	.string "あなたなら だいじょうぶよ きっと!\nポケモンも やるき まんまん だしね!$"

VictoryRoad_B1F_Text_MitchellIntro:
	.string "わたしの ポケモンは すばらしいですよ!$"

VictoryRoad_B1F_Text_MitchellDefeat:
	.string "あなた みたいな かた\nはじめて です$"

VictoryRoad_B1F_Text_MitchellPostBattle:
	.string "あなたと あなたの ポケモンが\nいっしょに いるだけで\lすごい パワーを かんじます$"

VictoryRoad_B1F_Text_HalleIntro:
	.string "オーケイ そんなに かまえないで!\nリラックス して いきましょ!$"

VictoryRoad_B1F_Text_HalleDefeat:
	.string "ワンダフル!$"

VictoryRoad_B1F_Text_HallePostBattle:
	.string "チャンピオンロード だって\nいままでの みちのりと かわらないわ\lのこりも エンジョイしてね$"
