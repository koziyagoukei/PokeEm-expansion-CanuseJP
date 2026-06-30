MeteorFalls_StevensCave_MapScripts::
	.byte 0

MeteorFalls_StevensCave_EventScript_Steven::
	lock
	goto_if_set FLAG_DEFEATED_METEOR_FALLS_STEVEN, MeteorFalls_StevensCave_EventScript_Defeated
	waitse
	playse SE_PIN
	applymovement LOCALID_METEOR_FALLS_STEVEN, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_METEOR_FALLS_STEVEN, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_METEOR_FALLS_STEVEN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MeteorFalls_StevensCave_Text_ShouldKnowHowGoodIAmExpectWorst, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_STEVEN, MeteorFalls_StevensCave_Text_StevenDefeat
	msgbox MeteorFalls_StevensCave_Text_MyPredictionCameTrue, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_METEOR_FALLS_STEVEN
	release
	end

MeteorFalls_StevensCave_EventScript_Defeated::
	applymovement LOCALID_METEOR_FALLS_STEVEN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MeteorFalls_StevensCave_Text_MyPredictionCameTrue, MSGBOX_DEFAULT
	release
	end

MeteorFalls_StevensCave_Text_ShouldKnowHowGoodIAmExpectWorst:
	.string "{JPN}ダイゴ『へぇ {PLAYER}{KUN}\nボクが ここにいるって よく わかったね\pもしかして きみ…… ボクを\nただの いしマニアだと おもってる?\p……いや トクサネの うちゅうセンターで\nいっしょに たたかったから\pボクが どれくらい つよいのか\nきみだって しってるはず だよね\pよし {PLAYER}{KUN}\pほんきで しょうぶ するなら\nかくごして くれよな!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MeteorFalls_StevensCave_Text_StevenDefeat:
	.string "{JPN}きみは……\nこれほどまでに つよくなって いたのか"
	.string "$"

MeteorFalls_StevensCave_Text_MyPredictionCameTrue:
	.string "{JPN}ダイゴ『そういえば……\nムロ タウンの いしのどうくつで\lはじめて あった ときから\pいずれ きみは チャンピオンに\nなるような きが していた\pボクの よそうは\nだいたい いつも あたるんだよ\pじゃあ……\nつぎに きみが むかうのは……\p…… …… ……\n…… …… ……\pふふふ さすがの ボクも\nそんなこと までは わからないな"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"
