MauvilleCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MauvilleCity_OnTransition
	.byte 0

MauvilleCity_OnTransition:
	setflag FLAG_VISITED_MAUVILLE_CITY
	clearflag FLAG_FORCE_MIRAGE_TOWER_VISIBLE
	clearflag FLAG_HIDE_SLATEPORT_MUSEUM_POPULATION
	setflag FLAG_SYS_TV_START
	clearflag FLAG_MAUVILLE_GYM_BARRIERS_STATE
	setvar VAR_MAUVILLE_GYM_STATE, 0
	call_if_set FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON, MauvilleCity_EventScript_MoveWattsonBackToGym
	end

MauvilleCity_EventScript_MoveWattsonBackToGym::
	clearflag FLAG_HIDE_MAUVILLE_GYM_WATTSON
	setflag FLAG_HIDE_MAUVILLE_CITY_WATTSON
	setflag FLAG_WATTSON_REMATCH_AVAILABLE
	return

MauvilleCity_EventScript_Boy::
	msgbox MauvilleCity_Text_NurseHurtMonBackToHealth, MSGBOX_NPC
	end

MauvilleCity_EventScript_Maniac::
	msgbox MauvilleCity_Text_AllSortsOfPeopleComeThrough, MSGBOX_NPC
	end

MauvilleCity_EventScript_Woman::
	msgbox MauvilleCity_Text_RydelVeryGenerous, MSGBOX_NPC
	end

MauvilleCity_EventScript_RichBoy::
	msgbox MauvilleCity_Text_PokemonCanJumpYouOnBike, MSGBOX_NPC
	end

MauvilleCity_EventScript_CitySign::
	msgbox MauvilleCity_Text_CitySign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_GymSign::
	msgbox MauvilleCity_Text_GymSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_BikeShopSign::
	msgbox MauvilleCity_Text_BikeShopSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_GameCornerSign::
	msgbox MauvilleCity_Text_GameCornerSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_SchoolKidM::
	lock
	faceplayer
	goto_if_set FLAG_TV_EXPLAINED, MauvilleCity_EventScript_TVExplained
	msgbox MauvilleCity_Text_ExplainTV, MSGBOX_DEFAULT
	setflag FLAG_TV_EXPLAINED
	release
	end

MauvilleCity_EventScript_TVExplained::
	msgbox MauvilleCity_Text_BeenCheckingOutTV, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_WallysUncle::
	lock
	faceplayer
	goto_if_set FLAG_DECLINED_WALLY_BATTLE_MAUVILLE, MauvilleCity_EventScript_UncleAskPlayerToBattleWally
	msgbox MauvilleCity_Text_UncleHesTooPeppy, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

MauvilleCity_EventScript_UncleAskPlayerToBattleWally::
	msgbox MauvilleCity_Text_UncleCanYouBattleWally, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

MauvilleCity_EventScript_Wally::
	lockall
	goto_if_set FLAG_DECLINED_WALLY_BATTLE_MAUVILLE, MauvilleCity_EventScript_WallyRequestBattleAgain
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox MauvilleCity_Text_WallyWantToChallengeGym, MSGBOX_DEFAULT
	msgbox MauvilleCity_Text_UncleYourePushingIt, MSGBOX_DEFAULT
	msgbox MauvilleCity_Text_WallyWeCanBeatAnyone, MSGBOX_DEFAULT
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_FacePlayer
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_Delay48
	waitmovement 0
	msgbox MauvilleCity_Text_WallyWillYouBattleMe, MSGBOX_YESNO
	goto MauvilleCity_EventScript_BattleWallyPrompt
	end

MauvilleCity_EventScript_BattleWallyPrompt::
	call_if_eq VAR_RESULT, YES, MauvilleCity_EventScript_BattleWally
	goto_if_eq VAR_RESULT, NO, MauvilleCity_EventScript_DeclineWallyBattle
	closemessage
	switch VAR_FACING
	case DIR_NORTH, MauvilleCity_EventScript_WallyAndUncleExitNorth
	case DIR_EAST, MauvilleCity_EventScript_WallyAndUncleExitEast
	end

MauvilleCity_EventScript_WallyAndUncleExitNorth::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitNorth1
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitNorth1
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitNorth1
	waitmovement 0
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 30
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerFaceUncleNorth
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleApproachPlayerNorth
	waitmovement 0
	msgbox MauvilleCity_Text_UncleVisitUsSometime, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitNorth2
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitNorth2
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitNorth2
	waitmovement 0
	goto MauvilleCity_EventScript_DefeatedWally
	end

MauvilleCity_EventScript_WallyAndUncleExitEast::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitEast1
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitEast1
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitEast1
	waitmovement 0
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 30
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleApproachPlayerEast
	waitmovement 0
	msgbox MauvilleCity_Text_UncleVisitUsSometime, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitEast2
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitEast2
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitEast2
	waitmovement 0
	goto MauvilleCity_EventScript_DefeatedWally
	end

MauvilleCity_EventScript_DefeatedWally::
	removeobject LOCALID_MAUVILLE_WALLY
	removeobject LOCALID_MAUVILLE_WALLYS_UNCLE
	clearflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY
	clearflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLYS_UNCLE
	setflag FLAG_DEFEATED_WALLY_MAUVILLE
	setvar VAR_WALLY_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_FIRST_WALLY_POKENAV_CALL
	call_if_eq VAR_FACING, DIR_NORTH, MauvilleCity_EventScript_ScottApproachPlayerNorth
	call_if_eq VAR_FACING, DIR_EAST, MauvilleCity_EventScript_ScottApproachPlayerEast
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox MauvilleCity_Text_ScottYouDidntHoldBack, MSGBOX_DEFAULT
	closemessage
	addvar VAR_SCOTT_STATE, 1
	call_if_eq VAR_FACING, DIR_NORTH, MauvilleCity_EventScript_ScottExitNorth
	call_if_eq VAR_FACING, DIR_EAST, MauvilleCity_EventScript_ScottExitEast
	removeobject LOCALID_MAUVILLE_SCOTT
	releaseall
	end

MauvilleCity_EventScript_ScottApproachPlayerNorth::
	addobject LOCALID_MAUVILLE_SCOTT
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottApproachPlayerNorth
	waitmovement 0
	return

MauvilleCity_EventScript_ScottApproachPlayerEast::
	setobjectxyperm LOCALID_MAUVILLE_SCOTT, 12, 13
	addobject LOCALID_MAUVILLE_SCOTT
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottApproachPlayerEast
	waitmovement 0
	return

MauvilleCity_EventScript_ScottExitNorth::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchScottExitNorth
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottExitNorth
	waitmovement 0
	return

MauvilleCity_EventScript_ScottExitEast::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchScottExitEast
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottExitEast
	waitmovement 0
	return

MauvilleCity_EventScript_BattleWally::
	msgbox MauvilleCity_Text_WallyHereICome, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLY_MAUVILLE, MauvilleCity_Text_WallyDefeat
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox MauvilleCity_Text_WallyIllGoBackToVerdanturf, MSGBOX_DEFAULT
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MauvilleCity_Text_ThankYouNotEnoughToBattle, MSGBOX_DEFAULT
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox MauvilleCity_Text_UncleNoNeedToBeDown, MSGBOX_DEFAULT
	return

MauvilleCity_EventScript_DeclineWallyBattle::
	setflag FLAG_DECLINED_WALLY_BATTLE_MAUVILLE
	msgbox MauvilleCity_Text_WallyMyUncleWontKnowImStrong, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_WallyRequestBattleAgain::
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MauvilleCity_Text_WallyPleaseBattleMe, MSGBOX_YESNO
	goto MauvilleCity_EventScript_BattleWallyPrompt
	end

MauvilleCity_Movement_WallyExitNorth1:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	step_end

MauvilleCity_Movement_WallyExitEast1:
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_WallyExitNorth2:
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	delay_8
	step_end

MauvilleCity_Movement_WallyExitEast2:
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	delay_8
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitNorth2:
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitEast2:
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchScottExitNorth:
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchScottExitEast:
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitEast1:
	delay_16
	walk_in_place_faster_down
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitNorth1:
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_WallysUncleExitNorth1:
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	step_end

MauvilleCity_Movement_WallysUncleExitEast1:
	walk_left
	walk_down
	walk_down
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_PlayerFaceUncleNorth:
	delay_16
	delay_8
	delay_4
	walk_in_place_faster_down
	step_end

MauvilleCity_Movement_WallysUncleApproachPlayerNorth:
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

MauvilleCity_Movement_WallysUncleApproachPlayerEast:
	walk_right
	walk_up
	step_end

MauvilleCity_Movement_WallysUncleExitNorth2:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_WallysUncleExitEast2:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_ScottApproachPlayerNorth:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

MauvilleCity_Movement_ScottApproachPlayerEast:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

MauvilleCity_Movement_ScottExitNorth:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_ScottExitEast:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_EventScript_Wattson::
	lock
	faceplayer
	goto_if_set FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON, MauvilleCity_EventScript_ReceivedThunderbolt
	goto_if_eq VAR_NEW_MAUVILLE_STATE, 2, MauvilleCity_EventScript_CompletedNewMauville
	goto_if_set FLAG_GOT_BASEMENT_KEY_FROM_WATTSON, MauvilleCity_EventScript_BegunNewMauville
	msgbox MauvilleCity_Text_WattsonNeedFavorTakeKey, MSGBOX_DEFAULT
	giveitem ITEM_BASEMENT_KEY
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_GOT_BASEMENT_KEY_FROM_WATTSON
	msgbox MauvilleCity_Text_WattsonWontBeChallenge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_BegunNewMauville::
	msgbox MauvilleCity_Text_WattsonWontBeChallenge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_CompletedNewMauville::
	msgbox MauvilleCity_Text_WattsonThanksTakeTM, MSGBOX_DEFAULT
	giveitem ITEM_TM_THUNDERBOLT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON
	msgbox MauvilleCity_Text_WattsonYoungTakeCharge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_ReceivedThunderbolt::
	msgbox MauvilleCity_Text_WattsonYoungTakeCharge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_RegisterWallyCall::
	lockall
	pokenavcall MauvilleCity_Text_WallyPokenavCall
	waitmessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox MauvilleCity_Text_RegisteredWally, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WALLY_MATCH_CALL
	clearflag FLAG_ENABLE_FIRST_WALLY_POKENAV_CALL
	releaseall
	end

MauvilleCity_Text_UncleHesTooPeppy:
	.string "{JPN}おじさん『このこは ポケモンの おかげで\nげんきに なれたような もんだけど\lちょっと げんきに なりすぎ なんだ……"
	.string ""
	.string "$"

MauvilleCity_Text_WallyWantToChallengeGym:
	.string "{JPN}ミツル『おじさん おねがい だから!\pじぶんが どれくらい つよくなったか\nこのジムで ためして みたいんだ!\pね ね いいでしょう!?"
	.string ""
	.string ""
	.string "$"

MauvilleCity_Text_UncleYourePushingIt:
	.string "{JPN}おじさん『まあまあ ミツルくん!\pたしかに ポケモンと いっしょに\nくらすように なってから\lきみは ずいぶん げんきに なった!\pだからって いきなり ポケモンジムに\nちょうせん なんて むり してないかい?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Text_WallyWeCanBeatAnyone:
	.string "{JPN}ミツル『むり なんか してません\pぼくと ラルトスが ちからを あわせれば\nだれにだって かてる はずです!"
	.string ""
	.string "$"

MauvilleCity_Text_WallyWillYouBattleMe:
	.string "{JPN}ミツル『あっ! {PLAYER}さん!\pぼく あれから つよくなったんだ!\pそれを {PLAYER}さんにも\nおじさんにも わかって もらいたいんです!\p{PLAYER}さん おねがいです!\nぼくと しょうぶ してください"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Text_WallyMyUncleWontKnowImStrong:
	.string "{JPN}ミツル『{PLAYER}さんが\nぼくと しょうぶ してくれないと……\pおじさんに ぼくが つよくなったこと\nわかって もらえないよ……"
	.string ""
	.string ""
	.string "$"

MauvilleCity_Text_UncleCanYouBattleWally:
	.string "{JPN}おじさん『{PLAYER}{KUN}とやら\nわるいけど ミツルくんと\lしょうぶ してあげて くれないかな?\pこのままだと なにを いっても\nきいて もらえそうに ないよ"
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Text_WallyPleaseBattleMe:
	.string "{JPN}ミツル『{PLAYER}さん おねがいです!\nぼくと しょうぶ してください"
	.string "$"

MauvilleCity_Text_WallyHereICome:
	.string "{JPN}ミツル『ありがとう {PLAYER}さん\pそれでは…… いきますね!"
	.string "$"

MauvilleCity_Text_WallyDefeat:
	.string "{JPN}ミツル『…… …… …… ……\p…… …… …… ……\pぼくの まけ です……"
	.string ""
	.string "$"

MauvilleCity_Text_WallyIllGoBackToVerdanturf:
	.string "{JPN}ミツル『おじさん……\nぼく シダケに もどります"
	.string "$"

MauvilleCity_Text_ThankYouNotEnoughToBattle:
	.string "{JPN}{PLAYER}さん ありがとう\nやっぱり トレーナーって すごいんですね\pただ ポケモンを もっていて\nたたかわせる だけじゃ ほんとうの いみで\lトレーナーには なれないんだ……"
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Text_UncleNoNeedToBeDown:
	.string "{JPN}おじさん『ミツルくん\nそんなに しょげることは ないよ\pこれから もっともっと\nつよくなれば いいじゃないか!\pさあ うちに かえろう\nみんな まってるよ"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Text_UncleVisitUsSometime:
	.string "{JPN}おじさん『なるほど ミツルくんが\nポケモンを つかまえるのを\lみまもって くれたのは {PLAYER}{KUN}!\lきみ だったんだね\pよかったら シダケにも\nあそびに きてくれないか?\lミツルくん きっと よろこぶから"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"


MauvilleCity_Text_WallyPokenavCall:
	.string "{JPN}…… …… ……\n…… …… …… ピッ!\pミツル『あっ {PLAYER}さん!\nあのね おじさんが ぼくのために\lポケナビを てにいれて くれたんです!\pこれで いつでも {PLAYER}さんと\nれんらくが とれますね!\p…… …… ……\n…… …… …… ツー"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Text_RegisteredWally:
	.string "{JPN}ミツルを\nポケナビに とうろく した!"
	.string "$"

MauvilleCity_Text_ScottYouDidntHoldBack:
	.string "{JPN}エニシダ『へへ……\nいまの しょうぶ みていたよ!\pきみは あの ミツルくん とかいう\nしょうねんと ともだち なんだろう?\pそれなのに てかげん せずに たたかい\nみごとに うちまかした……\pうん! それでこそ ほんとうの\nポケモン しょうぶ と いうものだ!\pわたしは そういう トレーナーが\nだいすきだなー!\p…… …… ……\nこれからも おうえん しているよ!"
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

MauvilleCity_Text_WattsonNeedFavorTakeKey:
	.string "{JPN}テッセン『おお {PLAYER}{KUN}\nげんきそうに しておるのう\lわっはははは! いいこと じゃよ!\pよしっ! きめた!\n{PLAYER}{KUN}に おねがいを するぞい!\pじつは この キンセツには\nニューキンセツ という ちかとしが ある\pで {PLAYER}{KUN} には\nそこの はつでんそうちの スイッチを\lきってきて ほしいんじゃよ!\pさいきん はつでんそうちが\nぼうそうぎみで なにかと あぶないからのう\pほら これが ニューキンセツの\nとびらを あける カギじゃ!"
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

MauvilleCity_Text_WattsonWontBeChallenge:
	.string "{JPN}テッセン『なーに\nおまえさんなら だいじょうぶ じゃ\pちなみに ニューキンセツの いりぐちは\n110ばんどうろで なみのりを して\lすぐの ところに あるからのう\pでは よろしく たのんだぞ!\nわっはははは!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Text_WattsonThanksTakeTM:
	.string "{JPN}テッセン『わっはははは!\pさすがは {PLAYER}{KUN}! おまえさんに\nたのんで せいかい じゃったよ!\pこれは おれいじゃ\n10まんボルトの わざマシン\lえんりょせずに もっていくが いいぞ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Text_WattsonYoungTakeCharge:
	.string "{JPN}テッセン『わっはははは!\pわかものの たのもしい すがたを みるのは\nよろこばしい こと じゃな!"
	.string ""
	.string "$"

MauvilleCity_Text_NurseHurtMonBackToHealth:
	.string "{JPN}ポケモンしょうぶを するのも いいけど\pもしも じぶんの ポケモンが\nたたかいで きずついたり したなら\lやさしく いたわって あげなきゃね!"
	.string ""
	.string ""
	.string "$"

MauvilleCity_Text_AllSortsOfPeopleComeThrough:
	.string "{JPN}この まちは とうざい なんぼく に\nみちが つうじて いるから\lいろんな ひとが やってきます!"
	.string ""
	.string "$"

MauvilleCity_Text_RydelVeryGenerous:
	.string "{JPN}あなた サイクルショップ には\nもう いってみた?\pあそこの ごしゅじんの カゼノさんは\nすごく きまえが いいのよね!"
	.string ""
	.string ""
	.string "$"

MauvilleCity_Text_PokemonCanJumpYouOnBike:
	.string "{JPN}じてんしゃに のっていても\nやせいの ポケモンは とびだしてくるよ!"
	.string "$"

MauvilleCity_Text_CitySign:
	.string "{JPN}ここは キンセツ シティ\nあかるく かがやく たのしい まち"
	.string "$"

MauvilleCity_Text_GymSign:
	.string "{JPN}キンセツ シティ ポケモンジム\nリーダー テッセン\lあっかるい でんげき オヤジ!"
	.string ""
	.string "$"

MauvilleCity_Text_BikeShopSign:
	.string "{JPN}じゃりみち はしれば こころも はずむ!\n「サイクルショップ カゼノ」"
	.string "$"

MauvilleCity_Text_GameCornerSign:
	.string "{JPN}だれもが あそべる すてきな スポット!\n「キンセツ ゲームコーナー」"
	.string "$"

MauvilleCity_Text_ExplainTV:
	.string "{JPN}ねえ きみ\nテレビって チェック してるかい?\pさいきんに なって おもしろい\nばんぐみが きゅうに ふえたんだぜ!\pいいよね テレビって\nいきなり ともだちの ことが\lしょうかい されたり\pしらない うちに じぶんが\nテレビに でていたり するんだよ\pこんど ぼくも テレビに\nうつんない かなー\pきみも こんどから\nちゃんと チェックを してみなよ!"
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

MauvilleCity_Text_BeenCheckingOutTV:
	.string "{JPN}やあ!\nテレビは ちゃんと チェック してる?"
	.string "$"
