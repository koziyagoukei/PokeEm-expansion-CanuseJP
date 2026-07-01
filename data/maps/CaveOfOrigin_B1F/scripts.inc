CaveOfOrigin_B1F_MapScripts::
	.byte 0

CaveOfOrigin_B1F_EventScript_Wallace::
	lock
	faceplayer
	msgbox CaveOfOrigin_B1F_Text_WallaceStory, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 60
	playse SE_PIN
	applymovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_Delay48
	waitmovement 0
	delay 30
	applymovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_FacePlayer
	waitmovement 0
	message CaveOfOrigin_B1F_Text_WhereIsRayquaza
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_WheresRayquaza::
	multichoice 0, 0, MULTI_WHERES_RAYQUAZA, FALSE
	switch VAR_RESULT
	case 0, CaveOfOrigin_B1F_EventScript_AtCaveOfOrigin
	case 1, CaveOfOrigin_B1F_EventScript_AtMtPyre
	case 2, CaveOfOrigin_B1F_EventScript_AtSkyPillar
	case 3, CaveOfOrigin_B1F_EventScript_DontRemember
	goto CaveOfOrigin_B1F_EventScript_DontRemember
	end

CaveOfOrigin_B1F_EventScript_AtCaveOfOrigin::
	message CaveOfOrigin_B1F_Text_ButWereInCaveOfOrigin
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_AtMtPyre::
	message CaveOfOrigin_B1F_Text_OldLadyDidntMentionThat
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_DontRemember::
	message CaveOfOrigin_B1F_Text_CantYouRememberSomehow
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_AtSkyPillar::
	msgbox CaveOfOrigin_B1F_Text_WellHeadToSkyPillar, MSGBOX_DEFAULT
	closemessage
	playse SE_EXIT
	fadescreenspeed FADE_TO_BLACK, 4
	setflag FLAG_WALLACE_GOES_TO_SKY_PILLAR
	setvar VAR_SOOTOPOLIS_CITY_STATE, 3
	removeobject LOCALID_CAVE_OF_ORIGIN_WALLACE
	clearflag FLAG_HIDE_SKY_PILLAR_WALLACE
	fadescreen FADE_FROM_BLACK
	release
	end

CaveOfOrigin_B1F_Text_WallaceStory:
	.string "{JPN}そうか きみが {PLAYER}{KUN}……\nきみの かつやくは きいているよ\pわたしの なまえは ミクリ\nルネの ジムリーダーを していたけれど\lちょっと わけが あってね\pいまは ししょうの アダンさんに\nジムのことは おまかせして いるのさ\p…… …… ……\n…… …… ……\pいま このまちで あばれている\nグラードンと カイオーガは\lちょうこだい ポケモンと いわれている\pけれど ちょうこだい ポケモンは\nあの2ひき だけじゃ なかった……\lどこかに もう1ひき\pそう…… レックウザと よばれる\nちょうこだい ポケモンが いるんだよ\pとおい むかしに\nあの2ひきの たたかいを しずめたのも\lレックウザ だと いわれている\pだが レックウザが どこに いるかは\nわたしにも わからない……$"

CaveOfOrigin_B1F_Text_WhereIsRayquaza:
	.string "{JPN}ミクリ『{PLAYER}{KUN} ひょっとして\nきみは レックウザが どこに いるか\lしっているのかい?\pよかったら おしえて くれないか……$"

CaveOfOrigin_B1F_Text_ButWereInCaveOfOrigin:
	.string "{JPN}ミクリ『めざめのほこらか……\pって それは ここのこと じゃないか\nたのむよ しっかり してくれ!\pレックウザが どこに いるか\nもういちど かんがえて みてくれ……$"

CaveOfOrigin_B1F_Text_OldLadyDidntMentionThat:
	.string "{JPN}ミクリ『おくりびやまか……\pしかし このまえ オババさまに\nあったときは なにも いってなかった\pあのひとが わたしに\nうそを つくとも おもえないし……\p{PLAYER}{KUN} もういちど\nよく かんがえて くれないか……$"

CaveOfOrigin_B1F_Text_CantYouRememberSomehow:
	.string "{JPN}ミクリ『えっ おぼえてない って!?\nうーん それは こまったな……\pなんとか\nおもいだして もらえない だろうか……$"

CaveOfOrigin_B1F_Text_WellHeadToSkyPillar:
	.string "{JPN}ミクリ『そらのはしら……?\pそうか! そらのはしら だったか!\pよし {PLAYER}{KUN}\nこうしては いられない\lいますぐ そらのはしらへ いくぞ!$"
