MtEmber_Exterior_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtEmber_Exterior_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, MtEmber_Exterior_OnLoad
	.byte 0

MtEmber_Exterior_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_MT_EMBER_EXTERIOR
	call_if_eq VAR_MAP_SCENE_MT_EMBER_EXTERIOR, 2, MtEmber_Exterior_EventScript_RocketsFaceDown
	end

MtEmber_Exterior_EventScript_RocketsFaceDown::
	setobjectmovementtype LOCALID_MT_EMBER_GRUNT1, MOVEMENT_TYPE_FACE_DOWN
	setobjectmovementtype LOCALID_MT_EMBER_GRUNT2, MOVEMENT_TYPE_FACE_DOWN
	return

MtEmber_Exterior_OnLoad::
	call_if_ge VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 4, MtEmber_Exterior_EventScript_OpenCave
	end

MtEmber_Exterior_EventScript_OpenCave::
	setmetatile 42, 39, METATILE_MtEmber_CaveEntrance, 0
	return

MtEmber_Exterior_EventScript_Grunt1::
	lock
	goto_if_defeated TRAINER_TEAM_ROCKET_GRUNT_43, MtEmber_Exterior_EventScript_Grunt1Defeated
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 4, MtEmber_Exterior_EventScript_BattleGrunt1
	msgbox MtEmber_Exterior_Text_WellTryDiggingHere
	release
	end

MtEmber_Exterior_EventScript_Grunt1Defeated::
	msgbox MtEmber_Exterior_Text_Grunt1PostBattle
	release
	end

MtEmber_Exterior_EventScript_BattleGrunt1::
	applymovement LOCALID_MT_EMBER_GRUNT1, Common_Movement_FacePlayer
	waitmovement 0
	playbgm MUS_RG_ENCOUNTER_ROCKET, 0
	msgbox MtEmber_Exterior_Text_Grunt1Intro
	trainerbattle_no_intro TRAINER_TEAM_ROCKET_GRUNT_43, MtEmber_Exterior_Text_Grunt1Defeat
	msgbox MtEmber_Exterior_Text_Grunt1PostBattle
	goto_if_defeated TRAINER_TEAM_ROCKET_GRUNT_44, MtEmber_Exterior_EventScript_DefeatedBothGrunts
	release
	end

MtEmber_Exterior_EventScript_DefeatedBothGrunts::
	msgbox MtEmber_Exterior_Text_WellRegroupDontStepInsideThere
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_MT_EMBER_GRUNT1
	removeobject LOCALID_MT_EMBER_GRUNT2
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

MtEmber_Exterior_EventScript_Grunt2::
	lock
	faceplayer
	goto_if_defeated TRAINER_TEAM_ROCKET_GRUNT_44, MtEmber_Exterior_EventScript_DefeatedGrunt2
	goto_if_eq VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F, 4, MtEmber_Exterior_EventScript_BattleGrunt2
	msgbox MtEmber_Exterior_Text_YoureInTheWayGetLost
	closemessage
	applymovement LOCALID_MT_EMBER_GRUNT2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

MtEmber_Exterior_EventScript_DefeatedGrunt2::
	msgbox MtEmber_Exterior_Text_Grunt2PostBattle
	release
	end

MtEmber_Exterior_EventScript_BattleGrunt2::
	playbgm MUS_RG_ENCOUNTER_ROCKET, 0
	msgbox MtEmber_Exterior_Text_Grunt2Intro
	trainerbattle_no_intro TRAINER_TEAM_ROCKET_GRUNT_44, MtEmber_Exterior_Text_Grunt2Defeat
	msgbox MtEmber_Exterior_Text_Grunt2PostBattle
	goto_if_defeated TRAINER_TEAM_ROCKET_GRUNT_43, MtEmber_Exterior_EventScript_DefeatedBothGrunts
	release
	end

MtEmber_Exterior_EventScript_RocketPasswordScene::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	msgbox MtEmber_Exterior_Text_PunchedThroughAtLast
	message MtEmber_Exterior_Text_WhatsPasswordAgain
	waitmessage
	playse SE_PIN
	applymovement LOCALID_MT_EMBER_GRUNT2, Common_Movement_QuestionMark
	waitmovement 0
	waitbuttonpress
	msgbox MtEmber_Exterior_Text_FirstPasswordGoldeen
	applymovement LOCALID_MT_EMBER_GRUNT1, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_MT_EMBER_GRUNT1, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_MT_EMBER_GRUNT1, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_MT_EMBER_GRUNT2, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	call MtEmber_Exterior_EventScript_RocketsFaceDown
	msgbox MtEmber_Exterior_Text_SnoopsBeenListeningIn
	setvar VAR_MAP_SCENE_MT_EMBER_EXTERIOR, 2
	releaseall
	end

MtEmber_Exterior_EventScript_Logan::
	trainerbattle_single TRAINER_PKMN_RANGER_LOGAN, MtEmber_Exterior_Text_LoganIntro, MtEmber_Exterior_Text_LoganDefeat
	msgbox MtEmber_Exterior_Text_LoganPostBattle, MSGBOX_AUTOCLOSE
	end

MtEmber_Exterior_EventScript_Beth::
	trainerbattle_single TRAINER_PKMN_RANGER_BETH, MtEmber_Exterior_Text_BethIntro, MtEmber_Exterior_Text_BethDefeat
	msgbox MtEmber_Exterior_Text_BethPostBattle, MSGBOX_AUTOCLOSE
	end

MtEmber_Exterior_EventScript_Jocelyn::
	trainerbattle_single TRAINER_CRUSH_GIRL_JOCELYN, MtEmber_Exterior_Text_JocelynIntro, MtEmber_Exterior_Text_JocelynDefeat
	msgbox MtEmber_Exterior_Text_JocelynPostBattle, MSGBOX_AUTOCLOSE
	end

Text_ExplosionTeach::
	.string "{JPN}こ この かざんが\n"
	.string "もし だいばくはつ したら\l"
	.string "お おれら ひとたまりも\l"
	.string "ないなあ …ブルブル!\p"
	.string "こ こわい ついでに\n"
	.string "だいばくはつ でも おぼえてみる?$"

Text_ExplosionDeclined::
	.string "{JPN}や やっぱ こわいもんな\n"
	.string "ブルブル$"

Text_ExplosionWhichMon::
	.string "{JPN}つ つよきだね!\n"
	.string "じゃあ どの ポケモンに する?$"

Text_ExplosionTaught::
	.string "{JPN}かざんで だいばくはつを…\n"
	.string "す すごい スリル!$"

MtEmber_Exterior_Text_WellTryDiggingHere::
	.string "{JPN}ここを ほりすすんで みようぜ\p"
	.string "かんぶが いってた おたから\n"
	.string "この おくかも しれねえ$"

MtEmber_Exterior_Text_YoureInTheWayGetLost::
	.string "{JPN}なんだ おまえ?\n"
	.string "じゃまだから あっち いけよ!$"

MtEmber_Exterior_Text_PunchedThroughAtLast::
	.string "{JPN}フーっ やっと あいたぜ!\n"
	.string "おたからを さがしに いこーぜ$"

MtEmber_Exterior_Text_WhatsPasswordAgain::
	.string "{JPN}おたからが みつかったら\n"
	.string "そうこに もって かえろーぜ!\p"
	.string "……ところで\n"
	.string "そうこの パスワード って\l"
	.string "なんだったっけ?$"

MtEmber_Exterior_Text_FirstPasswordGoldeen::
	.string "{JPN}なんだよ わすれたのか\n"
	.string "パスワードは ふたつ あるぜ\p"
	.string "ひとつめは……\n"
	.string "『またまた タマタマ』 だろ\p"
	.string "ふたつめは……$"

MtEmber_Exterior_Text_SnoopsBeenListeningIn::
	.string "{JPN}オイっ だれか きいてる!$"

MtEmber_Exterior_Text_Grunt1Intro::
	.string "{JPN}ぬすみぎき して\n"
	.string "いやがったな!$"

MtEmber_Exterior_Text_Grunt1Defeat::
	.string "{JPN}えぇー$"

MtEmber_Exterior_Text_Grunt1PostBattle::
	.string "{JPN}なんで そこで\n"
	.string "かつんだよ$"

MtEmber_Exterior_Text_Grunt2Intro::
	.string "{JPN}おたからを よこどり\n"
	.string "するき だな!$"

MtEmber_Exterior_Text_Grunt2Defeat::
	.string "{JPN}あれー$"

MtEmber_Exterior_Text_Grunt2PostBattle::
	.string "{JPN}さっき きいたことは わすれろよ!$"

MtEmber_Exterior_Text_WellRegroupDontStepInsideThere::
	.string "{JPN}くそー…\n"
	.string "いちど ひきあげるか\p"
	.string "オマエ! この おくに\n"
	.string "いきやがったら\l"
	.string "ただじゃ おかねえぞ!$"

MtEmber_Exterior_Text_LoganIntro::
	.string "{JPN}ふんか かつどうを しているか\n"
	.string "ふんかしそうな かざんを\l"
	.string "かっかざんと いうぞ!$"

MtEmber_Exterior_Text_LoganDefeat::
	.string "{JPN}ふう…\n"
	.string "あつい しょうぶで あせだくだよ!$"

MtEmber_Exterior_Text_LoganPostBattle::
	.string "{JPN}かっかざんは せかいに\n"
	.string "1500 いじょう あるというな\l"
	.string "かんがえた だけで あついぜ!$"

MtEmber_Exterior_Text_BethIntro::
	.string "{JPN}かざんの どうくつは\n"
	.string "どうやって できるか しってる?$"

MtEmber_Exterior_Text_BethDefeat::
	.string "{JPN}あら! すごいのね$"

MtEmber_Exterior_Text_BethPostBattle::
	.string "{JPN}マグマの ひょうめんが かたまって\n"
	.string "なかの あつい マグマが\l"
	.string "ながれだして しまうと\l"
	.string "なかに どうくつが できるのよ$"

MtEmber_Exterior_Text_JocelynIntro::
	.string "{JPN}しつれんを いやす ために\n"
	.string "おけいこ がんばって いたら\l"
	.string "つよく なったわ! きえーいっ!$"

MtEmber_Exterior_Text_JocelynDefeat::
	.string "{JPN}ちからが はいりすぎてた かな?$"

MtEmber_Exterior_Text_JocelynPostBattle::
	.string "{JPN}あなたの たたかい かた\n"
	.string "あの カレを おもいだして\l"
	.string "つらい…$"

