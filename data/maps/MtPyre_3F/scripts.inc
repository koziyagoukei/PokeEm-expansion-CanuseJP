MtPyre_3F_MapScripts::
	.byte 0

MtPyre_3F_EventScript_William::
	trainerbattle_single TRAINER_WILLIAM, MtPyre_3F_Text_WilliamIntro, MtPyre_3F_Text_WilliamDefeat
	msgbox MtPyre_3F_Text_WilliamPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_3F_EventScript_Kayla::
	trainerbattle_single TRAINER_KAYLA, MtPyre_3F_Text_KaylaIntro, MtPyre_3F_Text_KaylaDefeat
	msgbox MtPyre_3F_Text_KaylaPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_3F_EventScript_Gabrielle::
	trainerbattle_single TRAINER_GABRIELLE_1, MtPyre_3F_Text_GabrielleIntro, MtPyre_3F_Text_GabrielleDefeat, MtPyre_3F_EventScript_RegisterGabrielle
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MtPyre_3F_EventScript_RematchGabrielle
	msgbox MtPyre_3F_Text_GabriellePostBattle, MSGBOX_DEFAULT
	release
	end

MtPyre_3F_EventScript_RegisterGabrielle::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MtPyre_3F_Text_GabrielleRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_GABRIELLE_1
	release
	end

MtPyre_3F_EventScript_RematchGabrielle::
	trainerbattle_rematch TRAINER_GABRIELLE_1, MtPyre_3F_Text_GabrielleRematchIntro, MtPyre_3F_Text_GabrielleRematchDefeat
	msgbox MtPyre_3F_Text_GabriellePostRematch, MSGBOX_AUTOCLOSE
	end

MtPyre_3F_Text_WilliamIntro:
	.string "{JPN}れいげん あらたかな やまの ちからで\nサイコパワーの アップした わたしに\lこどもの おまえが かてるかな!?$"

MtPyre_3F_Text_WilliamDefeat:
	.string "{JPN}アップ アップ……$"

MtPyre_3F_Text_WilliamPostBattle:
	.string "{JPN}わたしの サイコパワーは かくじつに\nつよく なっている はずなのに……"
	.string "$"

MtPyre_3F_Text_KaylaIntro:
	.string "{JPN}おほほほほっ!\pここは あなたの ような こどもが\nくる ばしょ では ないのよっ!"
	.string ""
	.string "$"

MtPyre_3F_Text_KaylaDefeat:
	.string "{JPN}あっさり まけたわっ……!$"

MtPyre_3F_Text_KaylaPostBattle:
	.string "{JPN}まだまだ しゅぎょうが たりないのね……\nさんちょうを めざすわよ……!\pファイト! わたし!"
	.string ""
	.string ""
	.string ""
	.string "$"

MtPyre_3F_Text_GabrielleIntro:
	.string "{JPN}あなたは なぜ ここへ……?$"

MtPyre_3F_Text_GabrielleDefeat:
	.string "{JPN}すごいわ!\nあなたは すてきな トレーナーさんね"
	.string "$"

MtPyre_3F_Text_GabriellePostBattle:
	.string "{JPN}いまはもう いない ポケモン\nいま いっしょに いる ポケモン\pそれに\nこれから であう ポケモン……\pすべて たいせつな ポケモン なのよ\nおぼえて おいて くださいね"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MtPyre_3F_Text_GabrielleRegister:
	.string "{JPN}あなたの ポケモンが そだったら\nみてみたいわ……\lさあ ポケナビを だして"
	.string ""
	.string "$"

MtPyre_3F_Text_GabrielleRematchIntro:
	.string "{JPN}あら あなたは……\nそだてた ポケモン みせてくれるの?"
	.string ""
	.string "$"

MtPyre_3F_Text_GabrielleRematchDefeat:
	.string "{JPN}ほんとうに すごいわ!\nあなたは すてきね"
	.string "$"

MtPyre_3F_Text_GabriellePostRematch:
	.string "{JPN}いまはもう いない ポケモン\nいま いっしょに いる ポケモン\pそれに\nこれから であう ポケモン……\pすべて たいせつな ポケモン なのよ\nおぼえて くれて いたのね"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"
