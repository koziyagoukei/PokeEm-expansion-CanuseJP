MtPyre_6F_MapScripts::
	.byte 0

MtPyre_6F_EventScript_Valerie::
	trainerbattle_single TRAINER_VALERIE_1, MtPyre_6F_Text_ValerieIntro, MtPyre_6F_Text_ValerieDefeat, MtPyre_6F_EventScript_RegisterValerie
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MtPyre_6F_EventScript_RematchValerie
	msgbox MtPyre_6F_Text_ValeriePostBattle, MSGBOX_DEFAULT
	release
	end

MtPyre_6F_EventScript_RegisterValerie::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MtPyre_6F_Text_ValerieRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_VALERIE_1
	release
	end

MtPyre_6F_EventScript_RematchValerie::
	trainerbattle_rematch TRAINER_VALERIE_1, MtPyre_6F_Text_ValerieRematchIntro, MtPyre_6F_Text_ValerieRematchDefeat
	msgbox MtPyre_6F_Text_ValeriePostRematch, MSGBOX_AUTOCLOSE
	end

MtPyre_6F_EventScript_Cedric::
	trainerbattle_single TRAINER_CEDRIC, MtPyre_6F_Text_CedricIntro, MtPyre_6F_Text_CedricDefeat
	msgbox MtPyre_6F_Text_CedricPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_6F_Text_ValerieIntro:
	.string "ここに いると……\nなにかの ちからが はいってくるの……"
	.string "$"

MtPyre_6F_Text_ValerieDefeat:
	.string "ちからが ぬけていく……$"

MtPyre_6F_Text_ValeriePostBattle:
	.string "ここに ねむっている ポケモンたちの\nちからが はいって くるのかもよ……?"
	.string "$"

MtPyre_6F_Text_ValerieRegister:
	.string "ふふふふ しょうぶには まけたけど\nわたし…… こんな ことが できるの……\pあなたの ポケナビに いっさい てを\nふれなくても…… きえーいっ……!"
	.string ""
	.string ""
	.string "$"

MtPyre_6F_Text_ValerieRematchIntro:
	.string "あなたの うしろ……\nなにが いるの……?"
	.string "$"

MtPyre_6F_Text_ValerieRematchDefeat:
	.string "なにかが きえたわ……$"

MtPyre_6F_Text_ValeriePostRematch:
	.string "ここに ねむっている ポケモンたち……\nときどき あそんでいる みたいなの……"
	.string "$"

MtPyre_6F_Text_CedricIntro:
	.string "きみ まいごなのか?\nおれが きた からには あんしんだ!"
	.string "$"

MtPyre_6F_Text_CedricDefeat:
	.string "まいご じゃ なかったか?$"

MtPyre_6F_Text_CedricPostBattle:
	.string "まいごの トレーナーは あわてて いるから\nかてる きが するんだよな\pずるいから もう やめとくよ……"
	.string ""
	.string "$"
