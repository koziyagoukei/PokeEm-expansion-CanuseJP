MtChimney_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, MtChimney_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, MtChimney_OnTransition
	.byte 0

MtChimney_OnTransition:
	setvar VAR_JAGGED_PASS_ASH_WEATHER, 1
	end

MtChimney_OnResume:
	setstepcallback STEP_CB_ASH
	end

MtChimney_EventScript_Archie::
	lock
	faceplayer
	call_if_unset FLAG_EVIL_LEADER_PLEASE_STOP, MtChimney_EventScript_ArchieGoStopTeamMagma
	call_if_set FLAG_EVIL_LEADER_PLEASE_STOP, MtChimney_EventScript_ArchieBusyFighting
	closemessage
	applymovement LOCALID_MT_CHIMNEY_ARCHIE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag FLAG_EVIL_LEADER_PLEASE_STOP
	release
	end

MtChimney_EventScript_ArchieGoStopTeamMagma::
	msgbox MtChimney_Text_ArchieGoStopTeamMagma, MSGBOX_DEFAULT
	return

MtChimney_EventScript_ArchieBusyFighting::
	msgbox MtChimney_Text_ArchieIHaveMyHandsFull, MSGBOX_DEFAULT
	return

MtChimney_EventScript_Maxie::
	lockall
	playbgm MUS_ENCOUNTER_MAGMA, FALSE
	msgbox MtChimney_Text_MeteoriteWillActivateVolcano, MSGBOX_DEFAULT
	applymovement LOCALID_MT_CHIMNEY_MAXIE, Common_Movement_FacePlayer
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_MT_CHIMNEY_MAXIE, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_MT_CHIMNEY_MAXIE, Common_Movement_Delay48
	waitmovement 0
	msgbox MtChimney_Text_MaxieIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_MAXIE_MT_CHIMNEY, MtChimney_Text_MaxieDefeat
	msgbox MtChimney_Text_MaxieYouHaventSeenLastOfMagma, MSGBOX_DEFAULT
	closemessage
	delay 30
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_MT_CHIMNEY_MAXIE
	removeobject LOCALID_MT_CHIMNEY_MAGMA_GRUNT_1
	removeobject LOCALID_MT_CHIMNEY_TABITHA
	removeobject LOCALID_MT_CHIMNEY_MAGMA_GRUNT_2
	setflag FLAG_HIDE_MT_CHIMNEY_TEAM_MAGMA
	fadescreenswapbuffers FADE_FROM_BLACK
	setobjectxyperm LOCALID_MT_CHIMNEY_ARCHIE, 10, 12
	addobject LOCALID_MT_CHIMNEY_ARCHIE
	call_if_eq VAR_FACING, DIR_EAST, MtChimney_EventScript_ArchieApproachPlayerEast
	call_if_eq VAR_FACING, DIR_NORTH, MtChimney_EventScript_ArchieApproachPlayerNorth
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox MtChimney_Text_ArchieThankYou, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_FACING, DIR_EAST, MtChimney_EventScript_ArchieExitEast
	call_if_eq VAR_FACING, DIR_NORTH, MtChimney_EventScript_ArchieExitNorth
	removeobject LOCALID_MT_CHIMNEY_ARCHIE
	setflag FLAG_HIDE_MT_CHIMNEY_TEAM_AQUA
	setflag FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY
	clearflag FLAG_HIDE_FALLARBOR_HOUSE_PROF_COZMO
	setflag FLAG_HIDE_METEOR_FALLS_1F_1R_COZMO
	clearflag FLAG_HIDE_MT_CHIMNEY_LAVA_COOKIE_LADY
	releaseall
	end

MtChimney_EventScript_ArchieApproachPlayerEast::
	applymovement LOCALID_MT_CHIMNEY_ARCHIE, MtChimney_Movement_ArchieApproachPlayerEast
	waitmovement 0
	return

MtChimney_EventScript_ArchieApproachPlayerNorth::
	applymovement LOCALID_MT_CHIMNEY_ARCHIE, MtChimney_Movement_ArchieApproachPlayerNorth
	waitmovement 0
	return

MtChimney_EventScript_ArchieExitEast::
	applymovement LOCALID_MT_CHIMNEY_ARCHIE, MtChimney_Movement_ArchieExitEast
	waitmovement 0
	return

MtChimney_EventScript_ArchieExitNorth::
	applymovement LOCALID_MT_CHIMNEY_ARCHIE, MtChimney_Movement_ArchieExitNorth
	waitmovement 0
	return

MtChimney_EventScript_LavaCookieLady::
	lock
	faceplayer
	showmoneybox 0, 0
	msgbox MtChimney_Text_LavaCookiesJust200, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtChimney_EventScript_DeclineLavaCookie
	checkmoney 200
	goto_if_eq VAR_RESULT, FALSE, MtChimney_EventScript_NotEnoughMoney
	msgbox MtChimney_Text_ThankYouDear, MSGBOX_DEFAULT
	checkitemspace ITEM_LAVA_COOKIE
	call_if_eq VAR_RESULT, TRUE, MtChimney_EventScript_RemoveMoney
.if OW_SHOW_ITEM_DESCRIPTIONS == OW_ITEM_DESCRIPTIONS_OFF
	giveitem ITEM_LAVA_COOKIE
	goto_if_eq VAR_RESULT, FALSE, MtChimney_EventScript_BagIsFull
	hidemoneybox
	release
	end

MtChimney_EventScript_BagIsFull::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	hidemoneybox
	release
	end
.else
	hidemoneybox
	giveitem ITEM_LAVA_COOKIE
	goto_if_eq VAR_RESULT, FALSE, MtChimney_EventScript_BagIsFull
	release
	end

MtChimney_EventScript_BagIsFull::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	release
	end
.endif @ OW_SHOW_ITEM_DESCRIPTIONS


MtChimney_EventScript_RemoveMoney::
	removemoney 200
	updatemoneybox
	return

MtChimney_EventScript_DeclineLavaCookie::
	msgbox MtChimney_Text_OhFineThen, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

MtChimney_EventScript_NotEnoughMoney::
	msgbox MtChimney_Text_YouveNotGotTheMoney, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

MtChimney_Movement_ArchieApproachPlayerEast:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	step_end

MtChimney_Movement_ArchieExitEast:
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_ArchieApproachPlayerNorth:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	step_end

MtChimney_Movement_ArchieExitNorth:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused1:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused2:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused3:
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused4:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused5:
	delay_16
	delay_16
	delay_16
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused6:
	delay_16
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused7:
	delay_16
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused8:
	delay_16
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_down
	step_end

MtChimney_Movement_Unused9:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused10:
	walk_down
	step_end

MtChimney_Movement_Unused11:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused12:
	delay_16
	delay_16
	walk_in_place_faster_left
	delay_16
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused13:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	delay_16
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused14:
	lock_facing_direction
	walk_fast_left
	unlock_facing_direction
	face_right
	step_end

MtChimney_Movement_Unused15:
	walk_left
	walk_in_place_faster_right
	delay_16
	step_end

MtChimney_EventScript_BusyAquaGrunt1::
	msgbox MtChimney_Text_MagmaOutnumbersUs, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyAquaGrunt2::
	msgbox MtChimney_Text_LessHabitatForWaterPokemon, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyAquaGrunt3::
	msgbox MtChimney_Text_MagmasNameSimilar, MSGBOX_SIGN
	end

MtChimney_EventScript_Tabitha::
	trainerbattle_single TRAINER_TABITHA_MT_CHIMNEY, MtChimney_Text_TabithaIntro, MtChimney_Text_TabithaDefeat
	msgbox MtChimney_Text_TabithaPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MT_CHIMNEY_2, MtChimney_Text_Grunt2Intro, MtChimney_Text_Grunt2Defeat
	msgbox MtChimney_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_BusyMagmaGrunt1::
	msgbox MtChimney_Text_TeamAquaAlwaysMessingWithPlans, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt2::
	msgbox MtChimney_Text_MeteoritesPackAmazingPower, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt3::
	msgbox MtChimney_Text_YouBetterNotMessWithUs, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt4::
	msgbox MtChimney_Text_AquasNameSimilar, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt5::
	msgbox MtChimney_Text_DouseThemInFire, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt6::
	msgbox MtChimney_Text_KeepMakingMoreLand, MSGBOX_SIGN
	end

MtChimney_EventScript_MagmaPoochyena::
	msgbox MtChimney_Text_Bufoh, MSGBOX_SIGN
	end

MtChimney_EventScript_AquaPoochyena::
	msgbox MtChimney_Text_Bushaa, MSGBOX_SIGN
	end

MtChimney_EventScript_MeteoriteMachine::
	lockall
	goto_if_unset FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY, MtChimney_EventScript_MachineOn
	goto_if_set FLAG_RECEIVED_METEORITE, MtChimney_EventScript_MachineOff
	msgbox MtChimney_Text_RemoveTheMeteorite, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtChimney_EventScript_LeaveMeteoriteAlone
	msgbox MtChimney_Text_PlayerRemovedMeteorite, MSGBOX_DEFAULT
	giveitem ITEM_METEORITE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_METEORITE
	releaseall
	end

MtChimney_EventScript_LeaveMeteoriteAlone::
	msgbox MtChimney_Text_PlayerLeftMeteorite, MSGBOX_DEFAULT
	releaseall
	end

MtChimney_EventScript_MachineOff::
	msgbox MtChimney_Text_MachineMakesNoResponse, MSGBOX_DEFAULT
	releaseall
	end

MtChimney_EventScript_MachineOn::
	msgbox MtChimney_Text_MeteoriteFittedOnMachine, MSGBOX_DEFAULT
	releaseall
	end

MtChimney_EventScript_RouteSign::
	msgbox MtChimney_Text_RouteSign, MSGBOX_SIGN
	end

MtChimney_EventScript_Shelby::
	trainerbattle_single TRAINER_SHELBY_1, MtChimney_Text_ShelbyIntro, MtChimney_Text_ShelbyDefeat, MtChimney_EventScript_DefeatedShelby
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MtChimney_EventScript_RematchShelby
	msgbox MtChimney_Text_ShelbyPostBattle, MSGBOX_DEFAULT
	release
	end

MtChimney_EventScript_DefeatedShelby::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MtChimney_Text_ShelbyRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_SHELBY_1
	release
	end

MtChimney_EventScript_RematchShelby::
	trainerbattle_rematch TRAINER_SHELBY_1, MtChimney_Text_ShelbyRematchIntro, MtChimney_Text_ShelbyRematchDefeat
	msgbox MtChimney_Text_ShelbyPostRematch, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Melissa::
	trainerbattle_single TRAINER_MELISSA, MtChimney_Text_MelissaIntro, MtChimney_Text_MelissaDefeat
	msgbox MtChimney_Text_MelissaPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Sheila::
	trainerbattle_single TRAINER_SHEILA, MtChimney_Text_SheilaIntro, MtChimney_Text_SheilaDefeat
	msgbox MtChimney_Text_SheilaPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Shirley::
	trainerbattle_single TRAINER_SHIRLEY, MtChimney_Text_ShirleyIntro, MtChimney_Text_ShirleyDefeat
	msgbox MtChimney_Text_ShirleyPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MT_CHIMNEY_1, MtChimney_Text_Grunt1Intro, MtChimney_Text_Grunt1Defeat
	msgbox MtChimney_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Sawyer::
	trainerbattle_single TRAINER_SAWYER_1, MtChimney_Text_SawyerIntro, MtChimney_Text_SawyerDefeat, MtChimney_EventScript_SawyerDefeated
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MtChimney_EventScript_SawyerRematch
	msgbox MtChimney_Text_SawyerPostBattle, MSGBOX_DEFAULT
	release
	end

MtChimney_EventScript_SawyerDefeated::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MtChimney_Text_SawyerRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_SAWYER_1
	release
	end

MtChimney_EventScript_SawyerRematch::
	trainerbattle_rematch TRAINER_SAWYER_1, MtChimney_Text_SawyerRematchIntro, MtChimney_Text_SawyerRematchDefeat
	msgbox MtChimney_Text_SawyerPostRematch, MSGBOX_AUTOCLOSE
	end

MtChimney_Text_MeteoriteWillActivateVolcano:
	.string "{JPN}いんせきに ひめられた ちから\pこの そうちで ぞうふく すれば\nたちまちの うちに えんとつやまの\lかざん かつどうは かっぱつになる……\pその エネルギーが\nかこうの おくふかくに はたらきかけ\lそして ふふふ……"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MtChimney_Text_MaxieIntro:
	.string "{JPN}マツブサ『ん? だれだ おまえは\p…… …… ……\n…… …… ……\pさいきん アクアだんの かつどうを\nじゃまする こどもがいる と\lアオギリの やつが ぼやいて いたが\lそれは おまえの ことか\p……ふん!\nわれわれ マグマだんの じゃまも\lしてくれようって わけだな!\pいいか? いきものは りくちに あがり\nしんかを くりかえしてきた\pそう だいちこそが すべてなのだ!\pだから われわれ マグマだんは\nだいちを ふやすことで\lひとと ポケモンの はってんを めざす!\pそのために ひつよう なのが\nこのやまに ねむる アイツ……\pおっと! そんなことまで\nおまえが しる ひつようは なかったな\pそんなことより\nわれわれの じゃまを したら どうなるか\lおまえに わからせて やろう!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
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

MtChimney_Text_MaxieDefeat:
	.string "{JPN}なに!?\pこの マツブサとも あろう ものが\nゆだん したか!?"
	.string "$"

MtChimney_Text_MaxieYouHaventSeenLastOfMagma:
	.string "{JPN}マツブサ『まあ いい\nこのばは ひとまず あきらめよう……\pだが マグマだんの かつどうは\nこれで おわったわけでは ないのだ!\pふふふ……\pいんせきなど つかわなくとも\nあの たまが てにはいれば ふふふ……"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MtChimney_Text_TabithaIntro:
	.string "{JPN}ウヒョヒョ!\pおまえ ここまで きたのか!\pだけど おそかったぜ\nりゅうせいのたきで みつけた\lいんせきは リーダーに わたしたからな!"
	.string ""
	.string ""
	.string ""
	.string "$"

MtChimney_Text_TabithaDefeat:
	.string "{JPN}ウヒョヒョ……\pオレが まけても リーダーが ちゃんと\nアイツを めざめさせて……"
	.string ""
	.string "$"

MtChimney_Text_TabithaPostBattle:
	.string "{JPN}リーダー! はやく!\nいんせきの パワーを アイツに!\pウヒョヒョ……!"
	.string ""
	.string "$"

MtChimney_Text_Grunt2Intro:
	.string "{JPN}オレら マグマだんは\nみんなの ために がんばってるんだ!\pだって アイツの ちからで\nだいちが ふえれば みんなも\lすむところが ふえて うれしいだろ?"
	.string ""
	.string ""
	.string ""
	.string "$"

MtChimney_Text_Grunt2Defeat:
	.string "{JPN}あれー?\nオレ まけたのー!?"
	.string "$"

MtChimney_Text_Grunt2PostBattle:
	.string "{JPN}オレらの リーダーは\n「みんなが よろこぶ ことだ」って\lいってるんだ!\pなのに なんで みんな ジャマするんだ?"
	.string ""
	.string ""
	.string "$"

MtChimney_Text_Grunt1Intro:
	.string "{JPN}りくちが ふえて くれたら\nおおきな マイホーム だって もてるわ!\pようがんの うえに ひっこし するのよ!"
	.string ""
	.string "$"

MtChimney_Text_Grunt1Defeat:
	.string "{JPN}わたしの マイホームの ゆめが……$"

MtChimney_Text_Grunt1PostBattle:
	.string "{JPN}あんた なんか うみに プカプカ\nうかんで いれば いいのよ!"
	.string "$"

MtChimney_Text_TeamAquaAlwaysMessingWithPlans:
	.string "{JPN}アクアだんの やつらめ……\nいちいち オレたちの じゃまを しやがって!"
	.string "$"

MtChimney_Text_MeteoritesPackAmazingPower:
	.string "{JPN}いんせきには すごい パワーが あるのだ!$"

MtChimney_Text_YouBetterNotMessWithUs:
	.string "{JPN}オレたちの じゃまを するな!\pアイツを ふっかつ させるのは\nみんなの ために やってること なんだ!"
	.string ""
	.string "$"

MtChimney_Text_AquasNameSimilar:
	.string "{JPN}オレたちは マグマだん!\pやつらは アクアだん!\pにたような なまえを つけやがって\nいまいましい やつらだ!"
	.string ""
	.string ""
	.string "$"

MtChimney_Text_DouseThemInFire:
	.string "{JPN}あつい たたかい だったわ"
	.string "$"

MtChimney_Text_KeepMakingMoreLand:
	.string "{JPN}もっと もっと だいちを ふやすぜ!!$"

MtChimney_Text_Bufoh:
	.string "{JPN}ぶふぉー!!$"

MtChimney_Text_ArchieGoStopTeamMagma:
	.string "{JPN}アオギリ『むむむ {PLAYER}!\nやっぱり ここにも あらわれたか!\pほら! マグマだんの れんちゅうが\nなにを たくらんで いるのか みてみろ!\pやつらは うばった いんせきの パワーを\nかざんに ながしこもうと しているんだ!\pそんなことを したら\nかざんが だいふんか するじゃないか!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MtChimney_Text_ArchieIHaveMyHandsFull:
	.string "{JPN}アオギリ『うーむ むむむ!\pマツブサの やつを とめに いきたいが\nさすがの おれも\l3にん どうじに あいてを していては\lてが はなせない!"
	.string ""
	.string ""
	.string ""
	.string "$"

MtChimney_Text_ArchieThankYou:
	.string "{JPN}アオギリ『{PLAYER}…… たすかったよ\nおまえの おかげで マグマだんの\lはかいかつどうを ふせぐことが できた\pだが おまえは……\nどちらの みかた なんだ……\pまあ いい\pわれわれは ひきつづき\nマグマだんを おいかけると しよう!\p{PLAYER} おまえとは また\nどこかで あうことに なりそうだな……"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MtChimney_Text_MagmaOutnumbersUs:
	.string "{JPN}くそう マグマだんが おおすぎる……\nこのままでは まずいぞ!"
	.string "$"

MtChimney_Text_LessHabitatForWaterPokemon:
	.string "{JPN}だいちを ふやしたら\nみずポケモンが すむ ばしょが\lへってしまう じゃねーか!"
	.string "$"

MtChimney_Text_MagmasNameSimilar:
	.string "{JPN}オレたちは アクアだん!\pやつらは マグマだん!\pまぎらわしい なまえを つけやがって\nしゃくにさわる やつらだ!"
	.string ""
	.string ""
	.string "$"

MtChimney_Text_Bushaa:
	.string "{JPN}ぷしゃー!!$"

MtChimney_Text_LavaCookiesJust200:
	.string "{JPN}えんとつやま めいぶつ\nフエンせんべい ひとつ 200えん!\lかって おいきよ"
	.string ""
	.string "$"

MtChimney_Text_ThankYouDear:
	.string "{JPN}どうも ありがとね$"

MtChimney_Text_YouveNotGotTheMoney:
	.string "{JPN}おやまあ おかねが なくちゃ\nものは かえないよ"
	.string "$"

MtChimney_Text_OhFineThen:
	.string "{JPN}あら そうかい$"

MtChimney_Text_MeteoriteFittedOnMachine:
	.string "{JPN}なぞの そうちに\nいんせきが はめられている……\pなにかの エネルギーを\nためている さいちゅうの ようだ"
	.string ""
	.string ""
	.string "$"

MtChimney_Text_RemoveTheMeteorite:
	.string "{JPN}なぞの そうちに\nいんせきが はめられている……\pいんせきを とりますか?"
	.string ""
	.string "$"

MtChimney_Text_PlayerRemovedMeteorite:
	.string "{JPN}{PLAYER}は いんせきを\nそうちから とりはずした!"
	.string "$"

MtChimney_Text_PlayerLeftMeteorite:
	.string "{JPN}{PLAYER}は いんせきを\nそのままに しておいた"
	.string "$"

MtChimney_Text_MachineMakesNoResponse:
	.string "{JPN}なぞの そうち……\nまったく はんのう しない……"
	.string "$"

MtChimney_Text_RouteSign:
	.string "{JPN}{DOWN_ARROW} デコボコさんどう\nとおりぬけると フエンタウン"
	.string "$"

MtChimney_Text_ShelbyIntro:
	.string "{JPN}おんせんに つかって\nリフレッシュ してきたからねえ\lいまの あたしゃあ つよいよ!"
	.string ""
	.string "$"

MtChimney_Text_ShelbyDefeat:
	.string "{JPN}おんや まぁ\nたいした もんだよ"
	.string "$"

MtChimney_Text_ShelbyPostBattle:
	.string "{JPN}まけて しもうたら\nたつじん とは いえないねえ"
	.string "$"

MtChimney_Text_ShelbyRegister:
	.string "{JPN}マゴと たたかってる ようで たのしかった\nよかったら また あいてを しておくれ!"
	.string ""
	.string ""
	.string "$"

MtChimney_Text_ShelbyRematchIntro:
	.string "{JPN}ポケモンと こころを ひとつに できれば\nすばらしいことが できそうじゃの"
	.string ""
	.string "$"

MtChimney_Text_ShelbyRematchDefeat:
	.string "{JPN}おんや まぁ\nたいした もんだよ"
	.string "$"

MtChimney_Text_ShelbyPostRematch:
	.string "{JPN}おまえさんの こころと\nポケモンの こころは ひとつ なのかもな"
	.string "$"

MtChimney_Text_MelissaIntro:
	.string "{JPN}なんだか もえあがってきて\nたたかいたくて たまらないのっ!"
	.string "$"

MtChimney_Text_MelissaDefeat:
	.string "{JPN}よーし!\nほのおを ぶっかけてやれー!!$"

MtChimney_Text_MelissaPostBattle:
	.string "{JPN}えんとつやまの あつさが\nおんなを もえあがらせるの"
	.string "$"

MtChimney_Text_SheilaIntro:
	.string "{JPN}せっかく えんとつやまに きたんだから\nここでも ポケモンを たたかわせたいの!"
	.string "$"

MtChimney_Text_SheilaDefeat:
	.string "{JPN}その たたかいかた\nまさに えんとつやま だいふんか!"
	.string "$"

MtChimney_Text_SheilaPostBattle:
	.string "{JPN}せっかく えんとつやまに きたんだから\nかんこう だけじゃ もったいないわ\pしょうぶも たのしみたいし\nおみやげに せんべい かわなきゃ!"
	.string ""
	.string ""
	.string ""
	.string "$"

MtChimney_Text_ShirleyIntro:
	.string "{JPN}おんせんに はいったら\nちょうし バッチリ なのよね!\lぜったい かてるわ!"
	.string ""
	.string "$"

MtChimney_Text_ShirleyDefeat:
	.string "{JPN}うーん ゆざめ しちゃう"
	.string "$"

MtChimney_Text_ShirleyPostBattle:
	.string "{JPN}もういちど おんせんに はいらなきゃ!\nあなたも いっしょに はいる?\pなんてねっ!"
	.string ""
	.string "$"

MtChimney_Text_SawyerIntro:
	.string "{JPN}ここは いい やまだ!\nあっつい おねいさんたちが いっぱいだあ!"
	.string "$"

MtChimney_Text_SawyerDefeat:
	.string "{JPN}おおっ あんたも あついねぇ!$"

MtChimney_Text_SawyerPostBattle:
	.string "{JPN}いまから あつい おねいさんたちを つれて\nフエンおんせん でも いこうかな"
	.string "$"

MtChimney_Text_SawyerRegister:
	.string "{JPN}あつい トレーナーは すきだなあ!\nポケナビに とうろくしても いいかい"
	.string "$"

MtChimney_Text_SawyerRematchIntro:
	.string "{JPN}あっつい おねいさん たちに\nかこまれて しあわせな おれは まけんぜ!"
	.string "$"

MtChimney_Text_SawyerRematchDefeat:
	.string "{JPN}おおっ やっぱり\nあんたの ほうが あついねぇ!"
	.string "$"

MtChimney_Text_SawyerPostRematch:
	.string "{JPN}しかし ほんとに あついや……\nふく いっぱい きすぎちゃった かな"
	.string "$"
