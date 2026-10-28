BattleFrontier_OutsideWest_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_OutsideWest_OnTransition
	.byte 0

BattleFrontier_OutsideWest_OnTransition:
	setvar VAR_BRAVO_TRAINER_BATTLE_TOWER_ON, 0
	setflag FLAG_HIDE_BATTLE_TOWER_REPORTER
	end

BattleFrontier_OutsideWest_EventScript_FerryAttendant::
	lock
	faceplayer
	goto_if_set FLAG_STORY_INN_OPEN, FrontierStory_EventScript_HarborFerry
	msgbox BattleFrontier_OutsideWest_Text_FerrySuspended, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_Text_FerrySuspended:
	.string "ただいま ふねは おやすみちゅうです\nみなみの ひろばへは あるいて どうぞ！$"

BattleFrontier_OutsideWest_EventScript_BattleDomeSign::
	msgbox BattleFrontier_OutsideWest_Text_BattleDomeSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideWest_EventScript_BattleFactorySign::
	msgbox BattleFrontier_OutsideWest_Text_BattleFactorySign, MSGBOX_SIGN
	end

BattleFrontier_OutsideWest_EventScript_BattlePikeSign::
	msgbox BattleFrontier_OutsideWest_Text_BattlePikeSign, MSGBOX_SIGN
	end

@ Unused. Of note, Battle Tower is in the East section in the final release
BattleFrontier_OutsideWest_EventScript_UnusedNPC1::
	msgbox BattleFrontier_OutsideWest_Text_ThisIsBattleTower, MSGBOX_NPC
	end

@ Unused
BattleFrontier_OutsideWest_EventScript_UnusedNPC2::
	msgbox BattleFrontier_OutsideWest_Text_CantFindBattleTower, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Fisherman2::
	lock
	faceplayer
	message BattleFrontier_OutsideWest_Text_GotSeasickOnWayHere
	waitmessage
	applymovement LOCALID_FRONTIER_FISHERMAN_2, Common_Movement_FaceAwayPlayer
	waitmovement 0
	waitbuttonpress
	release
	end

BattleFrontier_OutsideWest_EventScript_Maniac1::
	lock
	goto BattleFrontier_OutsideWest_EventScript_FactoryChallengersTalk
	end

BattleFrontier_OutsideWest_EventScript_Maniac2::
	lock
	goto BattleFrontier_OutsideWest_EventScript_FactoryChallengersTalk
	end

BattleFrontier_OutsideWest_EventScript_FactoryChallengersTalk::
	applymovement LOCALID_FRONTIER_MANIAC_1, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox BattleFrontier_OutsideWest_Text_SureWeCanChallengeWithNoMons, MSGBOX_DEFAULT
	applymovement LOCALID_FRONTIER_MANIAC_2, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox BattleFrontier_OutsideWest_Text_BigGuySaidIllLendYouMons, MSGBOX_DEFAULT
	closemessage
	delay 25
	applymovement LOCALID_FRONTIER_MANIAC_1, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_FRONTIER_MANIAC_2, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	release
	end

BattleFrontier_OutsideWest_EventScript_Camper::
	lock
	faceplayer
	delay 20
	call_if_eq VAR_FACING, DIR_NORTH, BattleFrontier_OutsideWest_EventScript_CamperFaceFactory
	call_if_eq VAR_FACING, DIR_SOUTH, BattleFrontier_OutsideWest_EventScript_CamperAlreadyFacingFactory
	call_if_eq VAR_FACING, DIR_WEST, BattleFrontier_OutsideWest_EventScript_CamperFaceFactory
	call_if_eq VAR_FACING, DIR_EAST, BattleFrontier_OutsideWest_EventScript_CamperFaceFactory
	msgbox BattleFrontier_OutsideWest_Text_WhosRaisingThoseRentalMons, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_CamperFaceFactory::
	applymovement LOCALID_FRONTIER_CAMPER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	return

BattleFrontier_OutsideWest_EventScript_CamperAlreadyFacingFactory::
	return

BattleFrontier_OutsideWest_EventScript_Girl::
	lock
	faceplayer
	message BattleFrontier_OutsideWest_Text_ScaredOfPikeBecauseSeviper
	waitmessage
	call_if_eq VAR_FACING, DIR_NORTH, BattleFrontier_OutsideWest_EventScript_GirlShudderNorth
	call_if_eq VAR_FACING, DIR_SOUTH, BattleFrontier_OutsideWest_EventScript_GirlShudderSouth
	call_if_eq VAR_FACING, DIR_WEST, BattleFrontier_OutsideWest_EventScript_GirlShudderWest
	call_if_eq VAR_FACING, DIR_EAST, BattleFrontier_OutsideWest_EventScript_GirlShudderEast
	waitbuttonpress
	release
	end

BattleFrontier_OutsideWest_EventScript_GirlShudderNorth::
	applymovement LOCALID_FRONTIER_GIRL, BattleFrontier_OutsideWest_Movement_GirlShudderNorth
	waitmovement 0
	return

BattleFrontier_OutsideWest_EventScript_GirlShudderSouth::
	applymovement LOCALID_FRONTIER_GIRL, BattleFrontier_OutsideWest_Movement_GirlShudderSouth
	waitmovement 0
	return

BattleFrontier_OutsideWest_EventScript_GirlShudderWest::
	applymovement LOCALID_FRONTIER_GIRL, BattleFrontier_OutsideWest_Movement_GirlShudderWest
	waitmovement 0
	return

BattleFrontier_OutsideWest_EventScript_GirlShudderEast::
	applymovement LOCALID_FRONTIER_GIRL, BattleFrontier_OutsideWest_Movement_GirlShudderEast
	waitmovement 0
	return

BattleFrontier_OutsideWest_Movement_GirlShudderNorth:
	walk_in_place_faster_down
	walk_in_place_faster_down
	step_end

BattleFrontier_OutsideWest_Movement_GirlShudderSouth:
	walk_in_place_faster_up
	walk_in_place_faster_up
	step_end

BattleFrontier_OutsideWest_Movement_GirlShudderWest:
	walk_in_place_faster_right
	walk_in_place_faster_right
	step_end

BattleFrontier_OutsideWest_Movement_GirlShudderEast:
	walk_in_place_faster_left
	walk_in_place_faster_left
	step_end

BattleFrontier_OutsideWest_EventScript_Woman2::
	lock
	faceplayer
	msgbox BattleFrontier_OutsideWest_Text_LetsPlayRockPaperScissors, MSGBOX_DEFAULT
	random 2
	goto_if_eq VAR_RESULT, 1, BattleFrontier_OutsideWest_EventScript_WomanWonRockPaperScissors
	goto BattleFrontier_OutsideWest_EventScript_WomanLostRockPaperScissors
	end

BattleFrontier_OutsideWest_EventScript_WomanWonRockPaperScissors::
	msgbox BattleFrontier_OutsideWest_Text_WonIllTakePikeChallenge, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_WomanLostRockPaperScissors::
	msgbox BattleFrontier_OutsideWest_Text_LostIllPutOffPikeChallenge, MSGBOX_DEFAULT
	release
	end

@ Unused
BattleFrontier_OutsideWest_EventScript_UnusedNPC3::
	msgbox BattleFrontier_OutsideWest_Text_DomeIsHereGrandpa, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Woman3::
	lock
	msgbox BattleFrontier_OutsideWest_Text_LastTimeOurEyesMet, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_Boy2::
	lock
	msgbox BattleFrontier_OutsideWest_Text_DomeAceLookedBecauseOfMyCheering, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_OldMan::
	msgbox BattleFrontier_OutsideWest_Text_DomeAceIsMine, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Man4::
	lock
	faceplayer
	msgbox BattleFrontier_OutsideWest_Text_FansOverThereUsedToBeTrainers, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_MAN_4, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

BattleFrontier_OutsideWest_Text_BattleDomeSign:
	.string "ここは バトルドーム!\nめざせ! むてきの スーパースター!$"

BattleFrontier_OutsideWest_Text_BattleFactorySign:
	.string "ここは バトルファクトリー!\nさがせ! さいきょう ポケモン!$"

BattleFrontier_OutsideWest_Text_BattlePikeSign:
	.string "ここは バトルチューブ!\nえらべ! 3つの みち!$"

BattleFrontier_OutsideWest_Text_ThisIsBattleTower:
	.string "ここは バトルタワー さ!\pトレーナーは きたえあげた ポケモンで\nひたすら かちぬいて いくのさ!$"

BattleFrontier_OutsideWest_Text_CantFindBattleTower:
	.string "バトルタワーに いきたいのに\nフロンティアパスの マップを みても\lなにが なんだか…… ひろすぎる……$"

BattleFrontier_OutsideWest_Text_GotSeasickOnWayHere:
	.string "{JPN}うっぷ…… ふねに よった きがする。\pもう ここに ついているんだから\nうみかぜに あたって おこうっと。$"

BattleFrontier_OutsideWest_Text_SureWeCanChallengeWithNoMons:
	.string "{JPN}なあなあ にいちゃん。\nぼくらの ポケモンが いなくても\lファクトリーなら だいじょうぶなんだね？$"

BattleFrontier_OutsideWest_Text_BigGuySaidIllLendYouMons:
	.string "{JPN}だいじょうぶ！ かしてもらうこにも\nちゃんと あいさつするんだよ。\p……って ぼくも いま\nおしえて もらった きがする！$"

BattleFrontier_OutsideWest_Text_WhosRaisingThoseRentalMons:
	.string "{JPN}ファクトリーで かりた ポケモンと\nわかれるとき ちょっと さみしいんだ。\pまた あえたら さいしょから\nおたがい わかりあえそうな きがするよ。$"

BattleFrontier_OutsideWest_Text_ScaredOfPikeBecauseSeviper:
	.string "{JPN}ハブネークの いりぐちって\nちかくで みると はくりょく あるわね。\pこわいけど はいってみたい！\nこの きもちまで みんなと おそろいよ。$"

BattleFrontier_OutsideWest_Text_LetsPlayRockPaperScissors:
	.string "{JPN}ジャンケンで さいしょの みちを\nきめるのも たのしいよね！\pじゃーん けーん ぽん！$"

BattleFrontier_OutsideWest_Text_WonIllTakePikeChallenge:
	.string "{JPN}やったあ！ かっちゃった！\nきょうは まんなかの みちに しよう！$"

BattleFrontier_OutsideWest_Text_LostIllPutOffPikeChallenge:
	.string "{JPN}あっ まけちゃった！\nじゃあ ひだりの みちに してみよう！$"

BattleFrontier_OutsideWest_Text_DomeIsHereGrandpa:
	.string "{JPN}じいちゃん ここが バトルドーム！\nわたし おうえん してるからね！$"

BattleFrontier_OutsideWest_Text_LastTimeOurEyesMet:
	.string "{JPN}あのひと こっちを みたわ！\nわたしの おうえんが きこえたのね！$"

BattleFrontier_OutsideWest_Text_DomeAceLookedBecauseOfMyCheering:
	.string "{JPN}ぼくも おなじこと おもった！\nねえ つぎは いっしょに おうえんしよう！$"

BattleFrontier_OutsideWest_Text_DomeAceIsMine:
	.string "{JPN}だったら わしも まぜなさい！\nみんなの こえなら よく とおるぞ！$"

BattleFrontier_OutsideWest_Text_FansOverThereUsedToBeTrainers:
	.string "{JPN}あのひとたち しょうぶも つよいけど\nいまは おうえんが たのしいんだって。\pまいにち ここで まちあわせ。\nずっと いっしょで いいかお してるよ。$"

