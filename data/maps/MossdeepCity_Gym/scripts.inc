MossdeepCity_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MossdeepCity_Gym_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, MossdeepCity_Gym_OnLoad
	.byte 0

@ NOTE: Mossdeep Gym was redesigned between R/S and E. Leftover (and now functionally unused) scripts are commented below

MossdeepCity_Gym_OnTransition:
	setflag FLAG_TEMP_HIDE_FOLLOWER @ Hide follower since it can collide with moving statues
	end

@ All the below checks are leftover from RS. FLAG_MOSSDEEP_GYM_SWITCH_X is never set
MossdeepCity_Gym_OnLoad:
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_1, MossdeepCity_Gym_EventScript_SetSwitch1Metatiles
	goto MossdeepCity_Gym_EventScript_CheckSwitch2
	end

MossdeepCity_Gym_EventScript_CheckSwitch2::
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_2, MossdeepCity_Gym_EventScript_SetSwitch2Metatiles
	goto MossdeepCity_Gym_EventScript_CheckSwitch3
	end

MossdeepCity_Gym_EventScript_CheckSwitch3::
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_3, MossdeepCity_Gym_EventScript_SetSwitch3Metatiles
	goto MossdeepCity_Gym_EventScript_CheckSwitch4
	end

MossdeepCity_Gym_EventScript_CheckSwitch4::
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_4, MossdeepCity_Gym_EventScript_SetSwitch4Metatiles
	end

@ All the below set metatile scripts are leftover from RS and are functionally unused
MossdeepCity_Gym_EventScript_SetSwitch1Metatiles::
	setmetatile 5, 5, METATILE_RSMossdeepGym_RedArrow_Right, FALSE
	setmetatile 2, 7, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_CheckSwitch2
	end

MossdeepCity_Gym_EventScript_SetSwitch2Metatiles::
	setmetatile 8, 14, METATILE_RSMossdeepGym_RedArrow_Right, FALSE
	setmetatile 8, 10, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_CheckSwitch3
	end

MossdeepCity_Gym_EventScript_SetSwitch3Metatiles::
	setmetatile 15, 17, METATILE_RSMossdeepGym_RedArrow_Left, FALSE
	setmetatile 17, 15, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_CheckSwitch4
	end

MossdeepCity_Gym_EventScript_SetSwitch4Metatiles::
	setmetatile 1, 23, METATILE_RSMossdeepGym_RedArrow_Up, FALSE
	setmetatile 5, 24, METATILE_RSMossdeepGym_Switch_Down, TRUE
	end

MossdeepCity_Gym_EventScript_TateAndLiza::
	trainerbattle_double TRAINER_TATE_AND_LIZA_1, MossdeepCity_Gym_Text_TateAndLizaIntro, MossdeepCity_Gym_Text_TateAndLizaDefeat, MossdeepCity_Gym_Text_TateAndLizaNeedTwoMons, MossdeepCity_Gym_EventScript_TateAndLizaDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MossdeepCity_Gym_EventScript_TateAndLizaRematch
	goto_if_unset FLAG_RECEIVED_TM_CALM_MIND, MossdeepCity_Gym_EventScript_GiveCalmMind2
	msgbox MossdeepCity_Gym_Text_TateAndLizaPostBattle, MSGBOX_DEFAULT
	release
	end

MossdeepCity_Gym_EventScript_TateAndLizaDefeated::
	message MossdeepCity_Gym_Text_ReceivedMindBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox MossdeepCity_Gym_Text_ExplainMindBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_MOSSDEEP_GYM
	setflag FLAG_BADGE07_GET
	setflag FLAG_HIDE_AQUA_HIDEOUT_GRUNTS
	clearflag FLAG_HIDE_SLATEPORT_CITY_STERNS_SHIPYARD_MR_BRINEY
	clearflag FLAG_UNKNOWN_0x393
	setvar VAR_0x8008, 7
	call Common_EventScript_SetGymTrainers
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_PATRONS
	clearflag FLAG_HIDE_MOSSDEEP_CITY_TEAM_MAGMA
	clearflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_1F_TEAM_MAGMA
	clearflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_2F_TEAM_MAGMA
	clearflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_2F_STEVEN
	setvar VAR_MOSSDEEP_CITY_STATE, 1
	setflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_1F_STEVEN
	setvar VAR_MOSSDEEP_SPACE_CENTER_STATE, 1
	call MossdeepCity_Gym_EventScript_GiveCalmMind
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox MossdeepCity_Gym_Text_RegisteredTateAndLiza, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_TATE_AND_LIZA_MATCH_CALL
	release
	end

MossdeepCity_Gym_EventScript_GiveCalmMind2::
	giveitem ITEM_TM_CALM_MIND
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox MossdeepCity_Gym_Text_ExplainCalmMind, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_CALM_MIND
	release
	end

MossdeepCity_Gym_EventScript_GiveCalmMind::
	giveitem ITEM_TM_CALM_MIND
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox MossdeepCity_Gym_Text_ExplainCalmMind, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_CALM_MIND
	return

MossdeepCity_Gym_EventScript_TateAndLizaRematch::
	trainerbattle_rematch_double TRAINER_TATE_AND_LIZA_1, MossdeepCity_Gym_Text_TateAndLizaPreRematch, MossdeepCity_Gym_Text_TateAndLizaRematchDefeat, MossdeepCity_Gym_Text_TateAndLizaRematchNeedTwoMons
	msgbox MossdeepCity_Gym_Text_TateAndLizaPostRematch, MSGBOX_AUTOCLOSE
	end

@ All the below switch scripts are leftover from RS and are functionally unused
MossdeepCity_Gym_EventScript_Switch1::
	lockall
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_1, MossdeepCity_Gym_EventScript_ClearSwitch1
	setflag FLAG_MOSSDEEP_GYM_SWITCH_1
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 5, 5, METATILE_RSMossdeepGym_RedArrow_Right, FALSE
	setmetatile 2, 7, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed::
	special DrawWholeMapView
	playse SE_CLICK
	releaseall
	end

MossdeepCity_Gym_EventScript_ClearSwitch1::
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_1
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 5, 5, METATILE_RSMossdeepGym_RedArrow_Left, FALSE
	setmetatile 2, 7, METATILE_RSMossdeepGym_Switch_Up, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_Switch2::
	lockall
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_2, MossdeepCity_Gym_EventScript_ClearSwitch2
	setflag FLAG_MOSSDEEP_GYM_SWITCH_2
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 8, 14, METATILE_RSMossdeepGym_RedArrow_Right, FALSE
	setmetatile 8, 10, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_ClearSwitch2::
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_2
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 8, 14, METATILE_RSMossdeepGym_RedArrow_Down, FALSE
	setmetatile 8, 10, METATILE_RSMossdeepGym_Switch_Up, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_Switch3::
	lockall
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_3, MossdeepCity_Gym_EventScript_ClearSwitch3
	setflag FLAG_MOSSDEEP_GYM_SWITCH_3
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 15, 17, METATILE_RSMossdeepGym_RedArrow_Left, FALSE
	setmetatile 17, 15, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_ClearSwitch3::
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_3
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 15, 17, METATILE_RSMossdeepGym_RedArrow_Right, FALSE
	setmetatile 17, 15, METATILE_RSMossdeepGym_Switch_Up, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_Switch4::
	lockall
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_4, MossdeepCity_Gym_EventScript_ClearSwitch4
	setflag FLAG_MOSSDEEP_GYM_SWITCH_4
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 1, 23, METATILE_RSMossdeepGym_RedArrow_Up, FALSE
	setmetatile 5, 24, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_ClearSwitch4::
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_4
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 1, 23, METATILE_RSMossdeepGym_RedArrow_Right, FALSE
	setmetatile 5, 24, METATILE_RSMossdeepGym_Switch_Up, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_WarpToEntrance::
	lockall
	warpmossdeepgym MAP_MOSSDEEP_CITY_GYM, 7, 30
	waitstate
	releaseall
	end

MossdeepCity_Gym_EventScript_Preston::
	trainerbattle_single TRAINER_PRESTON, MossdeepCity_Gym_Text_PrestonIntro, MossdeepCity_Gym_Text_PrestonDefeat
	msgbox MossdeepCity_Gym_Text_PrestonPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Virgil::
	trainerbattle_single TRAINER_VIRGIL, MossdeepCity_Gym_Text_VirgilIntro, MossdeepCity_Gym_Text_VirgilDefeat
	msgbox MossdeepCity_Gym_Text_VirgilPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Blake::
	trainerbattle_single TRAINER_BLAKE, MossdeepCity_Gym_Text_BlakeIntro, MossdeepCity_Gym_Text_BlakeDefeat
	msgbox MossdeepCity_Gym_Text_BlakePostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Hannah::
	trainerbattle_single TRAINER_HANNAH, MossdeepCity_Gym_Text_HannahIntro, MossdeepCity_Gym_Text_HannahDefeat
	msgbox MossdeepCity_Gym_Text_HannahPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Samantha::
	trainerbattle_single TRAINER_SAMANTHA, MossdeepCity_Gym_Text_SamanthaIntro, MossdeepCity_Gym_Text_SamanthaDefeat
	msgbox MossdeepCity_Gym_Text_SamanthaPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Maura::
	trainerbattle_single TRAINER_MAURA, MossdeepCity_Gym_Text_MauraIntro, MossdeepCity_Gym_Text_MauraDefeat
	msgbox MossdeepCity_Gym_Text_MauraPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Sylvia::
	trainerbattle_single TRAINER_SYLVIA, MossdeepCity_Gym_Text_SylviaIntro, MossdeepCity_Gym_Text_SylviaDefeat
	msgbox MossdeepCity_Gym_Text_SylviaPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Nate::
	trainerbattle_single TRAINER_NATE, MossdeepCity_Gym_Text_NateIntro, MossdeepCity_Gym_Text_NateDefeat
	msgbox MossdeepCity_Gym_Text_NatePostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Macey::
	trainerbattle_single TRAINER_MACEY, MossdeepCity_Gym_Text_MaceyIntro, MossdeepCity_Gym_Text_MaceyDefeat
	msgbox MossdeepCity_Gym_Text_MaceyPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Clifford::
	trainerbattle_single TRAINER_CLIFFORD, MossdeepCity_Gym_Text_CliffordIntro, MossdeepCity_Gym_Text_CliffordDefeat
	msgbox MossdeepCity_Gym_Text_CliffordPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Nicholas::
	trainerbattle_single TRAINER_NICHOLAS, MossdeepCity_Gym_Text_NicholasIntro, MossdeepCity_Gym_Text_NicholasDefeat
	msgbox MossdeepCity_Gym_Text_NicholasPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Kathleen::
	trainerbattle_single TRAINER_KATHLEEN, MossdeepCity_Gym_Text_KathleenIntro, MossdeepCity_Gym_Text_KathleenDefeat
	msgbox MossdeepCity_Gym_Text_KathleenPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_MOSSDEEP_GYM, MossdeepCity_Gym_EventScript_GymGuidePostVictory
	msgbox MossdeepCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

MossdeepCity_Gym_EventScript_GymGuidePostVictory::
	msgbox MossdeepCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

@ Leftover from RS, functionally unused
MossdeepCity_Gym_Movement_WaitAfterSwitchUse:
	delay_16
	delay_16
	step_end

MossdeepCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE07_GET, MossdeepCity_Gym_EventScript_GymStatueCertified
	goto MossdeepCity_Gym_EventScript_GymStatue
	end

MossdeepCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE07_GET, MossdeepCity_Gym_EventScript_GymStatueCertified
	goto MossdeepCity_Gym_EventScript_GymStatue
	end

MossdeepCity_Gym_EventScript_GymStatueCertified::
	msgbox MossdeepCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

MossdeepCity_Gym_EventScript_GymStatue::
	msgbox MossdeepCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

MossdeepCity_Gym_EventScript_YellowFloorSwitch::
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle FALSE
	moverotatingtileobjects 0
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

MossdeepCity_Gym_EventScript_BlueFloorSwitch::
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle FALSE
	moverotatingtileobjects 1
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

MossdeepCity_Gym_EventScript_GreenFloorSwitch::
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle FALSE
	moverotatingtileobjects 2
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

MossdeepCity_Gym_EventScript_PurpleFloorSwitch::
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle FALSE
	moverotatingtileobjects 3
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

MossdeepCity_Gym_EventScript_RedFloorSwitch::
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle FALSE
	moverotatingtileobjects 4
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

MossdeepCity_Gym_Text_GymGuideAdvice:
	.string "{JPN}げんき しとぉや!!\nチャンピオンを めざす {PLAYER}!!\pこのジムの ジムリーダーは\nエスパータイプの ポケモンつかい!\pかくとうタイプや どくタイプの\nポケモンで いどむと\lものすごい ダメージを くらうぞ!\pしかも ばつぐんの\nコンビネーションで せめてくる!\pだから きみも かつために ポケモンとの\nきずなを きばって みせて やれ!$"

MossdeepCity_Gym_Text_GymGuidePostVictory:
	.string "{JPN}こりゃあ たまげた!!\nきみは すごか トレーナーだ!$"

MossdeepCity_Gym_Text_PrestonIntro:
	.string "{JPN}しょうぶは こころの つよさで きまる\nこころ よわき ものは かてない!$"

MossdeepCity_Gym_Text_PrestonDefeat:
	.string "{JPN}まけたか!\nオレも きたえ なおさねばっ!$"

MossdeepCity_Gym_Text_PrestonPostBattle:
	.string "{JPN}まよった ほうが まける\nオレからの ちゅうこくだ!$"

MossdeepCity_Gym_Text_VirgilIntro:
	.string "{JPN}キミの さいのうを みせて もらおうか!$"

MossdeepCity_Gym_Text_VirgilDefeat:
	.string "{JPN}すばらしい さいのうの もちぬしだ!$"

MossdeepCity_Gym_Text_VirgilPostBattle:
	.string "{JPN}よのなかに てんさいが いるならば\nここの ジムリーダーが そうだ……\pだが もしかすると キミの ほうが\nさいのうは うえかも しれない……$"

MossdeepCity_Gym_Text_BlakeIntro:
	.string "{JPN}ふふふ……\nねんりきで モンスターボールを\lくうちゅうに うかせて みせよう……\pうおおぉぉぉぉぉ……!\p…… …… ……\n…… …… ……\pちょうしの わるい ときも ある……$"

MossdeepCity_Gym_Text_BlakeDefeat:
	.string "{JPN}ポケモン しょうぶも\nちょうし わりーっ!$"

MossdeepCity_Gym_Text_BlakePostBattle:
	.string "{JPN}モンスターボールは おおきすぎた\nこの わたぼこり なら ぜったいに……\pふううううううっ……!\p…… …… ……\n…… …… ……\pちがうぞ!\nはないきで とばして なんか いないぞ!$"

MossdeepCity_Gym_Text_HannahIntro:
	.string "{JPN}あなたが アタシに まけた としても\nおちこまない ことね!\pあなたが よわい わけじゃ なくて\nアタシが つよすぎる からよ!$"

MossdeepCity_Gym_Text_HannahDefeat:
	.string "{JPN}あなたは さらに つよすぎた!$"

MossdeepCity_Gym_Text_HannahPostBattle:
	.string "{JPN}まけたから って クヨクヨ しないわ\nもっと しゅぎょう すれば いいんだもの!\pまえむきな ところが アタシの とりえ!$"

MossdeepCity_Gym_Text_SamanthaIntro:
	.string "{JPN}みえる……\nあなたの みらいが みえる……\pわたくしに かてたら\nあなたの みらいを おしえて あげる!$"

MossdeepCity_Gym_Text_SamanthaDefeat:
	.string "{JPN}まいりました……!$"

MossdeepCity_Gym_Text_SamanthaPostBattle:
	.string "{JPN}あなたと ジムリーダーさまの しょうぶ……\nものすごく はげしい てんかい……\pけっかは……\nわたくしの くちからは いえない……!$"

MossdeepCity_Gym_Text_MauraIntro:
	.string "{JPN}あなたが これまで あゆんできた みちのり\nあなたが けいけん してきた ことがら\pすべてを ぶつけて きなさい!$"

MossdeepCity_Gym_Text_MauraDefeat:
	.string "{JPN}すばらしい みちのり だわ……!$"

MossdeepCity_Gym_Text_MauraPostBattle:
	.string "{JPN}がんばった ものには あかるい みらいが\nなまけた ものには ぜつぼうの あしたが\pいんがおうほう……\nすべては じぶんに はねかえる ものよ……$"

MossdeepCity_Gym_Text_SylviaIntro:
	.string "{JPN}いのちを けずって でも\nこの しょうぶ かってみせるわ……!$"

MossdeepCity_Gym_Text_SylviaDefeat:
	.string "{JPN}あなたは\nおそろしい ことを……$"

MossdeepCity_Gym_Text_SylviaPostBattle:
	.string "{JPN}あたしに かったわね……\nなにが おきても しらないわよ……!$"

MossdeepCity_Gym_Text_NateIntro:
	.string "{JPN}ほっほっほ\pエスパータイプの てごわさを\nあなたに おしえて さしあげましょう$"

MossdeepCity_Gym_Text_NateDefeat:
	.string "{JPN}こ これは これは!\nすばらしい たたかいかた ですな$"

MossdeepCity_Gym_Text_NatePostBattle:
	.string "{JPN}わ わたくしは どんなに まけても\nとりみだしは しませんから$"

MossdeepCity_Gym_Text_KathleenIntro:
	.string "{JPN}おそろしいめに あわせて やろうかーっ!$"

MossdeepCity_Gym_Text_KathleenDefeat:
	.string "{JPN}いやあああーっ!$"

MossdeepCity_Gym_Text_KathleenPostBattle:
	.string "{JPN}あなたの げんきが うつるわ\nはやく いっちゃいなさいよ!$"

MossdeepCity_Gym_Text_CliffordIntro:
	.string "{JPN}としより だからと いって\nあまく みて もらっては こまりますよ$"

MossdeepCity_Gym_Text_CliffordDefeat:
	.string "{JPN}あふれる わかさを かんじる!$"

MossdeepCity_Gym_Text_CliffordPostBattle:
	.string "{JPN}この ジムの リーダーも そりゃもう!\nわかくて げんき はつらつ ですぞ$"

MossdeepCity_Gym_Text_MaceyIntro:
	.string "{JPN}まだよ\nまだ ジムリーダーには あわせないわよ!$"

MossdeepCity_Gym_Text_MaceyDefeat:
	.string "{JPN}ちょっと! なんなの その つよさ!$"

MossdeepCity_Gym_Text_MaceyPostBattle:
	.string "{JPN}フン あんたは つよいけど\nエスパーむき では ないわ!\lなんだか たたかいかたが がさつ だもん!$"

MossdeepCity_Gym_Text_NicholasIntro:
	.string "{JPN}うぉーっし! おれの\nスーパーポケモンしょうぶつよいパワー を\lうけて みろーっ!$"

MossdeepCity_Gym_Text_NicholasDefeat:
	.string "{JPN}やられたあああ!$"

MossdeepCity_Gym_Text_NicholasPostBattle:
	.string "{JPN}よし! つぎは\nスペシャルポケモンまけないパワー でも\lかいはつ するかな?$"

MossdeepCity_Gym_Text_TateAndLizaIntro:
	.string "{JPN}フウ『へへへ…… ジムリーダーが\pラン『ふふふ…… ジムリーダーが\pフウ『ふたりも いるんで おどろいた?\nラン『ふたりも いるんで おどろいた?\pフウ『ぼくたち ふたご!\nラン『あたしたち ふたご!\pフウ『なにも ことばに しなくても\nラン『おたがいの かんがえて いることが\pフウ『あたまの なかに うかぶから\nラン『つうじあうことが できるのヨ!\pフウ『そんな ぼくたちの コンビネーション\nラン『きみに やぶることが できる?$"

MossdeepCity_Gym_Text_TateAndLizaDefeat:
	.string "{JPN}フウ『ぼっ ぼくたちの\nラン『コンビネーションが……!\pフウ『やぶれた からには しかたない\nラン『まけを みとめて これを あげるヨ!$"

MossdeepCity_Gym_Text_ReceivedMindBadge:
	.string "{JPN}{PLAYER}は フウと ランから\nマインドバッジを もらった!$"

MossdeepCity_Gym_Text_ExplainMindBadgeTakeThis:
	.string "{JPN}フウ『マインドバッジには ポケモンの\nとくこうと とくぼうを たかめる\lこうかが ひめられている!\pラン『それから ダイビング という\nひでんわざも つかえるように なるヨ!\pフウ『あと これも もっていきなよ!$"

MossdeepCity_Gym_Text_ExplainCalmMind:
	.string "{JPN}フウ『その わざマシン04には\nラン『めいそうが はいってるヨ!\pフウ『ポケモンの とくこうと\nラン『ポケモンの とくぼうを\pフウ『たかめてくれる わざ なんだ!\nラン『エスパーポケモンに ぴったりだヨ!\p…… …… ……$"

MossdeepCity_Gym_Text_RegisteredTateAndLiza:
	.string "{JPN}ジムリーダー フウとランを\nポケナビに とうろく した!$"

MossdeepCity_Gym_Text_TateAndLizaPostBattle:
	.string "{JPN}フウ『きょうだいの きずな より\nきみと ポケモンとの きずなの ほうが\lはるかに つよかった みたいだね……\pラン『あなたなら もっと つよくなれる!\nたたかった あたしたちには わかるわ!$"

MossdeepCity_Gym_Text_TateAndLizaNeedTwoMons:
	.string "{JPN}フウ『へへへ…… ジムリーダーが\pラン『ふたりも いるんで おどろいた?\pフウ『あら? たたかえる ポケモンが\nラン『いっぴき しか いないヨ!\pフウ『それでは たたかえないね!\nラン『しょうぶを たのしみたいから\lもっと ポケモン つれてきてヨ!$"

MossdeepCity_Gym_Text_GymStatue:
	.string "{JPN}トクサネシティ ポケモンジム$"

MossdeepCity_Gym_Text_GymStatueCertified:
	.string "{JPN}トクサネシティ ポケモンジム\pフウ ラン にんてい トレーナー!\n{PLAYER}$"

MossdeepCity_Gym_Text_TateAndLizaPreRematch:
	.string "{JPN}フウ『ポケモンは……\nラン『ポケモンは……\pフウ『くみあわせを かえると\nラン『たたかいかたも かわるよね\pフウ『でも ぼくは……\nラン『でも あたしは……\pフウ『いつでも ランと いっしょ!\nラン『いつでも フウと いっしょ!\pフウ『ぼくたちの コンビネーション\nラン『なんどでも みせて あげるわ!$"

MossdeepCity_Gym_Text_TateAndLizaRematchDefeat:
	.string "{JPN}フウ『きみと ポケモンって\nラン『まるで きょうだい みたい!$"

MossdeepCity_Gym_Text_TateAndLizaPostRematch:
	.string "{JPN}フウ『なんど たおされたって\nまた たちなおって みせるからね!\pラン『だって あたしたち ふたり\nちからを あわせて がんばれるもの!$"

MossdeepCity_Gym_Text_TateAndLizaRematchNeedTwoMons:
	.string "{JPN}フウ『また ぼくたちと……\nラン『また あたしたちと……\pフウ『しょうぶを しに きてくれたんだね\nラン『しょうぶを しに きてくれたんだね\pフウ『でも……\nラン『あなた たたかえる ポケモンを\pフウ『1ひきしか もって いないよ!\nラン『1ひきしか もって いないわ!\pフウ『できれば 2ひき いじょうの\nラン『ポケモンを もって また きてね!$"
