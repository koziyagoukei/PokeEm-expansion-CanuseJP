.equ PLAYER_X_POS, VAR_0x8004
.equ PLAYER_Y_POS, VAR_0x8005

FiveIsland_RocketWarehouse_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, FiveIsland_RocketWarehouse_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, FiveIsland_RocketWarehouse_OnTransition
	.byte 0

FiveIsland_RocketWarehouse_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_FIVE_ISLAND_ROCKET_WAREHOUSE
	end

FiveIsland_RocketWarehouse_OnLoad::
	call_if_defeated TRAINER_TEAM_ROCKET_ADMIN, FiveIsland_RocketWarehouse_EventScript_SetArrowsForReEntry
	end

FiveIsland_RocketWarehouse_EventScript_SetArrowsForReEntry::
	setmetatile 26, 20, METATILE_SilphCo_Arrow_Up, 0
	setmetatile 27, 20, METATILE_SilphCo_Arrow_Up, 0
	return

FiveIsland_RocketWarehouse_EventScript_Cage::
	lockall
	goto_if_set FLAG_DEFEATED_ROCKETS_IN_WAREHOUSE, FiveIsland_RocketWarehouse_EventScript_CageUnlocked
	msgbox FiveIsland_RocketWarehouse_Text_ManyMonsLockedInPen
	releaseall
	end

FiveIsland_RocketWarehouse_EventScript_CageUnlocked::
	msgbox FiveIsland_RocketWarehouse_Text_PenUnlockedMonsFled
	releaseall
	end

FiveIsland_RocketWarehouse_EventScript_Computer::
	msgbox FiveIsland_RocketWarehouse_Text_ReportWithGraphsAndText, MSGBOX_SIGN
	end

FiveIsland_RocketWarehouse_EventScript_Admin2Trigger::
	lockall
	applymovement LOCALID_WAREHOUSE_ADMIN2, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	setvar VAR_MAP_SCENE_ROCKET_WAREHOUSE, 1
	releaseall
	end

FiveIsland_RocketWarehouse_EventScript_Gideon::
	trainerbattle_single TRAINER_SCIENTIST_GIDEON, FiveIsland_RocketWarehouse_Text_GideonIntro, FiveIsland_RocketWarehouse_Text_GideonDefeat, FiveIsland_RocketWarehouse_EventScript_DefeatedGideon
	goto_if_set FLAG_IS_CHAMPION, FiveIsland_RocketWarehouse_EventScript_MentionGiovannisKid
	msgbox FiveIsland_RocketWarehouse_Text_GetLostLeaveMeBe, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_MentionGiovannisKid::
	famechecker FAMECHECKER_GIOVANNI, 5
	msgbox FiveIsland_RocketWarehouse_Text_GiovannisKidHasRedHair, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_DefeatedGideon::
	msgbox FiveIsland_RocketWarehouse_Text_GideonPostBattle
	setflag FLAG_RECOVERED_SAPPHIRE
	giveitem ITEM_SAPPHIRE
	release
	end

FiveIsland_RocketWarehouse_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_47, FiveIsland_RocketWarehouse_Text_Grunt2Intro, FiveIsland_RocketWarehouse_Text_Grunt2Defeat
	msgbox FiveIsland_RocketWarehouse_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_Grunt3::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_48, FiveIsland_RocketWarehouse_Text_Grunt3Intro, FiveIsland_RocketWarehouse_Text_Grunt3Defeat
	msgbox FiveIsland_RocketWarehouse_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_Admin2::
	trainerbattle_single TRAINER_TEAM_ROCKET_ADMIN_2, FiveIsland_RocketWarehouse_Text_Admin2Intro, FiveIsland_RocketWarehouse_Text_Admin2Defeat, FiveIsland_RocketWarehouse_EventScript_DefeatedAdmin2
	msgbox FiveIsland_RocketWarehouse_Text_Admin2PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_DefeatedAdmin2::
	getplayerxy PLAYER_X_POS, PLAYER_Y_POS
	call_if_le PLAYER_X_POS, 24, FiveIsland_RocketWarehouse_EventScript_PlayerFaceAdmin2
	msgbox FiveIsland_RocketWarehouse_Text_Admin2PostBattle
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_WAREHOUSE_GRUNT2
	removeobject LOCALID_WAREHOUSE_GRUNT3
	removeobject LOCALID_WAREHOUSE_ADMIN2
	removeobject LOCALID_WAREHOUSE_ADMIN1
	setflag FLAG_HIDE_FIVE_ISLAND_ROCKETS
	fadescreen FADE_FROM_BLACK
	setvar VAR_MAP_SCENE_ROCKET_WAREHOUSE, 1
	setflag FLAG_DEFEATED_ROCKETS_IN_WAREHOUSE
	release
	end

FiveIsland_RocketWarehouse_EventScript_PlayerFaceAdmin2::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

FiveIsland_RocketWarehouse_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_42, FiveIsland_RocketWarehouse_Text_Grunt1Intro, FiveIsland_RocketWarehouse_Text_Grunt1Defeat
	msgbox FiveIsland_RocketWarehouse_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_Admin1::
	trainerbattle_single TRAINER_TEAM_ROCKET_ADMIN, FiveIsland_RocketWarehouse_Text_Admin1Intro, FiveIsland_RocketWarehouse_Text_Admin1Defeat, FiveIsland_RocketWarehouse_EventScript_DefeatedAdmin1
	msgbox FiveIsland_RocketWarehouse_Text_MadeItSoYouCanComeBackThrough, MSGBOX_AUTOCLOSE
	end

FiveIsland_RocketWarehouse_EventScript_DefeatedAdmin1::
	getplayerxy PLAYER_X_POS, PLAYER_Y_POS
	specialvar VAR_RESULT, GetPlayerFacingDirection
	call_if_eq VAR_RESULT, DIR_NORTH, FiveIsland_RocketWarehouse_EventScript_PlayerFaceAdmin1
	msgbox FiveIsland_RocketWarehouse_Text_Admin1PostBattle
	closemessage
	call_if_le PLAYER_X_POS, 24, FiveIsland_RocketWarehouse_EventScript_AdminWalkToSwitchFar
	call_if_eq PLAYER_X_POS, 25, FiveIsland_RocketWarehouse_EventScript_AdminWalkToSwitch
	call_if_eq PLAYER_X_POS, 26, FiveIsland_RocketWarehouse_EventScript_AdminFaceSwitch
	call_if_eq PLAYER_X_POS, 27, FiveIsland_RocketWarehouse_EventScript_AdminFaceSwitch
	playse SE_PIN
	call FiveIsland_RocketWarehouse_EventScript_SetArrowsForReEntry
	special DrawWholeMapView
	waitse
	call_if_le PLAYER_X_POS, 24, FiveIsland_RocketWarehouse_EventScript_AdminFacePlayerLeft
	call_if_eq PLAYER_X_POS, 25, FiveIsland_RocketWarehouse_EventScript_AdminFacePlayerLeft
	call_if_eq PLAYER_X_POS, 26, FiveIsland_RocketWarehouse_EventScript_AdminFacePlayerLeft
	call_if_eq PLAYER_X_POS, 27, FiveIsland_RocketWarehouse_EventScript_AdminFacePlayerDown
	msgbox FiveIsland_RocketWarehouse_Text_MadeItSoYouCanComeBackThrough
	release
	end

FiveIsland_RocketWarehouse_EventScript_PlayerFaceAdmin1::
	goto_if_eq PLAYER_X_POS, 27, Common_EventScript_NopReturn
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

FiveIsland_RocketWarehouse_EventScript_AdminWalkToSwitchFar::
	applymovement LOCALID_WAREHOUSE_ADMIN1, FiveIsland_RocketWarehouse_Movement_AdminWalkToSwitchFar
	waitmovement 0
	return

FiveIsland_RocketWarehouse_EventScript_AdminWalkToSwitch::
	applymovement LOCALID_WAREHOUSE_ADMIN1, FiveIsland_RocketWarehouse_Movement_AdminWalkToSwitch
	waitmovement 0
	return

FiveIsland_RocketWarehouse_EventScript_AdminFaceSwitch::
	applymovement LOCALID_WAREHOUSE_ADMIN1, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	return

FiveIsland_RocketWarehouse_EventScript_AdminFacePlayerLeft::
	applymovement LOCALID_WAREHOUSE_ADMIN1, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

FiveIsland_RocketWarehouse_EventScript_AdminFacePlayerDown::
	applymovement LOCALID_WAREHOUSE_ADMIN1, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	return

FiveIsland_RocketWarehouse_Movement_AdminWalkToSwitchFar::
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

FiveIsland_RocketWarehouse_Movement_AdminWalkToSwitch::
	walk_right
	walk_in_place_faster_up
	step_end

FiveIsland_RocketWarehouse_Text_ManyMonsLockedInPen::
	.string "{JPN}おりの なかに\n"
	.string "ポケモンが とじこめられてる……$"

FiveIsland_RocketWarehouse_Text_PenUnlockedMonsFled::
	.string "{JPN}おりの かぎが あいている\n"
	.string "ポケモンは みんな にげたようだ$"

FiveIsland_RocketWarehouse_Text_Grunt2Intro::
	.string "{JPN}ここが ロケットだんの そうこと\n"
	.string "しってて きたんか?$"

FiveIsland_RocketWarehouse_Text_Grunt2Defeat::
	.string "{JPN}なにすんじゃ コラァ!$"

FiveIsland_RocketWarehouse_Text_Grunt2PostBattle::
	.string "{JPN}ロケットだんは かいさん したァ?\n"
	.string "なに バカ いってんだ?$"

FiveIsland_RocketWarehouse_Text_Grunt1Intro::
	.string "{JPN}あら つよそうな ポケモン\n"
	.string "うって くれるの?\p"
	.string "えっ たたかう つもりなの!$"

FiveIsland_RocketWarehouse_Text_Grunt1Defeat::
	.string "{JPN}え えーっ!\n"
	.string "すごい!$"

FiveIsland_RocketWarehouse_Text_Grunt1PostBattle::
	.string "{JPN}たかく かうから\n"
	.string "あんたの ポケモン よこしなさい!$"

FiveIsland_RocketWarehouse_Text_Grunt3Intro::
	.string "{JPN}なかまから れんらくが きたぞ\p"
	.string "こんな ガキが\n"
	.string "サカキさまの しりあいな\l"
	.string "わけ ねえだろが!$"

FiveIsland_RocketWarehouse_Text_Grunt3Defeat::
	.string "{JPN}ふ・ざ・け・ん・な$"

FiveIsland_RocketWarehouse_Text_Grunt3PostBattle::
	.string "{JPN}おとなの しごとばを あらすなや!\n"
	.string "こなまいきな ガキが!$"

FiveIsland_RocketWarehouse_Text_Admin1Intro::
	.string "{JPN}わたしたちの している ことが\n"
	.string "ただしいか どうか なんて\l"
	.string "どうでも いい…\p"
	.string "わたしは サカキさまを しんじて\n"
	.string "ただ ついて きたのよ!$"

FiveIsland_RocketWarehouse_Text_Admin1Defeat::
	.string "{JPN}ま まけたわ$"

FiveIsland_RocketWarehouse_Text_Admin1PostBattle::
	.string "{JPN}あなたも\n"
	.string "あなたの しんじるように\l"
	.string "うごいている って わけね…\p"
	.string "わかったわ$"

FiveIsland_RocketWarehouse_Text_MadeItSoYouCanComeBackThrough::
	.string "{JPN}そこから はいれるように したわ\n"
	.string "いちど あたまを\l"
	.string "ひやしてから くれば?\p"
	.string "この さきの かんぶは\n"
	.string "わたし より つよいわよ\l"
	.string "こころして たちむかうのね!$"

FiveIsland_RocketWarehouse_Text_Admin2Intro::
	.string "{JPN}せいぎのみかた ごっこも\n"
	.string "たいがいに するんだな\p"
	.string "ロケットだんが かいさんした\n"
	.string "などと ウソを いって\l"
	.string "われわれを まどわす\l"
	.string "つもり だろうが……\p"
	.string "こども ひとりに だまされるほど\n"
	.string "われわれは おろかでは ない!\p"
	.string "おとなの こわさを\n"
	.string "おもいしるが いい!$"

FiveIsland_RocketWarehouse_Text_Admin2Defeat::
	.string "{JPN}くっ……\n"
	.string "つよいと おもったら\l"
	.string "それは サカキさまの バッジ……!\p"
	.string "ロケットだんが かいさん したのは\n"
	.string "ほんとう だったのか……$"

FiveIsland_RocketWarehouse_Text_Admin2PostBattle::
	.string "{JPN}この そうこは てばなす\p"
	.string "だが わたしは\n"
	.string "こんな ことでは あきらめない\p"
	.string "サカキさまを さがして\n"
	.string "ロケットだんを ふっかつ させる!\l"
	.string "このわたしが かならず…!\p"
	.string "では さらばだ!$"

FiveIsland_RocketWarehouse_Text_ReportWithGraphsAndText::
	.string "{JPN}パソコンの がめん に\n"
	.string "グラフや レポートが うつっている\p"
	.string "『ポケモンを しんか させる\n"
	.string "でんぱの けんきゅうに ついて』\l"
	.string "……… ………\p"
	.string "ないようは むずかしくて\n"
	.string "よく わからない$"

FiveIsland_RocketWarehouse_Text_GideonIntro::
	.string "{JPN}もうすこしで サファイアが\n"
	.string "たかく うれる ところだった のに\l"
	.string "おまえの せいで\l"
	.string "とりひきが だいなしだ!\p"
	.string "ふふ ふふふふ……\n"
	.string "ぜーっったい ゆるさん!$"

FiveIsland_RocketWarehouse_Text_GideonDefeat::
	.string "{JPN}ちきしょう\n"
	.string "ちきしょう ちきしょう!$"

FiveIsland_RocketWarehouse_Text_GideonPostBattle::
	.string "{JPN}ふん! かねにも なんないし\n"
	.string "もってけよ こんなもん!$"

FiveIsland_RocketWarehouse_Text_GetLostLeaveMeBe::
	.string "{JPN}なんだよお…\n"
	.string "はやく あっち いけよ!$"

FiveIsland_RocketWarehouse_Text_GiovannisKidHasRedHair::
	.string "{JPN}なんだよ また きたのかよ\p"
	.string "まさか おまえ…\n"
	.string "サカキとか いう やつの\l"
	.string "こども なんじゃ ないのか?\p"
	.string "…いや サカキの こどもは\n"
	.string "かみのけが あかい らしいから\l"
	.string "おまえ とは ちがうか\p"
	.string "かんけい ないなら ほっとけよ!$"

