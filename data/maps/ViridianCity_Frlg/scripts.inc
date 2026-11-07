ViridianCity_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianCity_OnTransition
	.byte 0

ViridianCity_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_VIRIDIAN_CITY
	call_if_eq VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 0, ViridianCity_EventScript_SetOldManBlockingRoad
	call_if_eq VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 1, ViridianCity_EventScript_SetOldManStandingByRoad
	call_if_ge VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 2, ViridianCity_EventScript_SetOldManNormal
	call_if_eq VAR_MAP_SCENE_VIRIDIAN_CITY_GYM_DOOR, 0, ViridianCity_EventScript_TryUnlockGym
	end

ViridianCity_EventScript_SetOldManNormal::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_OLD_MAN_1
	return

ViridianCity_EventScript_SetOldManStandingByRoad::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_OLD_MAN_1
	setobjectxyperm LOCALID_TUTORIAL_MAN, 21, 8
	setobjectmovementtype LOCALID_TUTORIAL_MAN, MOVEMENT_TYPE_LOOK_AROUND
	return

ViridianCity_EventScript_SetOldManBlockingRoad::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_OLD_MAN_LYING_DOWN
	setobjectxyperm LOCALID_TUTORIAL_MAN, 21, 11
	setobjectmovementtype LOCALID_TUTORIAL_MAN, MOVEMENT_TYPE_FACE_DOWN
	return

ViridianCity_EventScript_TryUnlockGym::
	goto_if_unset FLAG_BADGE02_GET, Common_EventScript_NopReturn
	goto_if_unset FLAG_BADGE03_GET, Common_EventScript_NopReturn
	goto_if_unset FLAG_BADGE04_GET, Common_EventScript_NopReturn
	goto_if_unset FLAG_BADGE05_GET, Common_EventScript_NopReturn
	goto_if_unset FLAG_BADGE06_GET, Common_EventScript_NopReturn
	goto_if_unset FLAG_BADGE07_GET, Common_EventScript_NopReturn
	setvar VAR_MAP_SCENE_VIRIDIAN_CITY_GYM_DOOR, 1
	return

ViridianCity_EventScript_GymDoorLocked::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 20
	msgbox ViridianCity_Text_GymDoorsAreLocked
	closemessage
	applymovement LOCALID_PLAYER, ViridianCity_Movement_JumpDownLedge
	waitmovement 0
	releaseall
	end

ViridianCity_Movement_JumpDownLedge::
	jump_2_down
	step_end

ViridianCity_EventScript_CitySign::
	msgbox ViridianCity_Text_CitySign, MSGBOX_SIGN
	end

ViridianCity_EventScript_TrainerTips1::
	msgbox ViridianCity_Text_CatchMonsForEasierBattles, MSGBOX_SIGN
	end

ViridianCity_EventScript_TrainerTips2::
	msgbox ViridianCity_Text_MovesLimitedByPP, MSGBOX_SIGN
	end

ViridianCity_EventScript_GymSign::
	msgbox ViridianCity_Text_GymSign, MSGBOX_SIGN
	end

ViridianCity_EventScript_GymDoor::
	msgbox ViridianCity_Text_GymDoorsAreLocked, MSGBOX_SIGN
	end

ViridianCity_EventScript_Boy::
	msgbox ViridianCity_Text_CanCarryMonsAnywhere, MSGBOX_NPC
	end

@ Other old man, not the tutorial old man
ViridianCity_EventScript_OldMan::
	lock
	faceplayer
	goto_if_eq VAR_MAP_SCENE_VIRIDIAN_CITY_GYM_DOOR, 1, ViridianCity_EventScript_OldManGymLeaderReturned
	msgbox ViridianCity_Text_GymClosedWonderWhoLeaderIs
	closemessage
	applymovement LOCALID_VIRIDIAN_OLD_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

ViridianCity_EventScript_OldManGymLeaderReturned::
	msgbox ViridianCity_Text_ViridiansGymLeaderReturned
	release
	end

ViridianCity_EventScript_TutorialOldMan::
	lock
	faceplayer
	goto_if_set FLAG_BADGE01_GET, ViridianCity_EventScript_AskIfTeachyTVHelpful
	goto_if_ge VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 2, ViridianCity_EventScript_TutorialCompleted
	goto_if_eq VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 1, ViridianCity_EventScript_TutorialStart
	goto_if_eq VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 0, ViridianCity_EventScript_TutorialNotReady
	end

ViridianCity_EventScript_AskIfTeachyTVHelpful::
	msgbox ViridianCity_Text_HowsTeachyTVHelping, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ViridianCity_EventScript_TeachyTVNotHelpful
	msgbox ViridianCity_Text_MyGrandsonOnTheShow
	release
	end

ViridianCity_EventScript_TeachyTVNotHelpful::
	msgbox ViridianCity_Text_TooBusyForTeachyTV
	release
	end

ViridianCity_EventScript_TutorialCompleted::
	msgbox ViridianCity_Text_WeakenMonsFirstToCatch
	release
	end

ViridianCity_EventScript_TutorialStart::
	call ViridianCity_EventScript_DoTutorialBattle
	release
	end

@ Unused
ViridianCity_EventScript_WatchToLearnBasics::
	msgbox ViridianCity_Text_WatchThatToLearnBasics
	release
	end

@ Unused. Starts battle after the post battle text, which is odd.
ViridianCity_EventScript_TutorialUnused::
	msgbox ViridianCity_Text_ThatWasEducationalTakeThis
	special StartOldManTutorialBattle
	waitstate
	release
	end

ViridianCity_EventScript_TutorialNotReady::
	msgbox ViridianCity_Text_ThisIsPrivateProperty
	closemessage
	release
	end

ViridianCity_EventScript_Youngster::
	lock
	faceplayer
	msgbox ViridianCity_Text_WantToKnowAboutCaterpillarMons, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, ViridianCity_EventScript_YoungsterExplainCaterpillars
	goto_if_eq VAR_RESULT, NO, ViridianCity_EventScript_YoungsterDeclineExplanation
	end

ViridianCity_EventScript_YoungsterExplainCaterpillars::
	msgbox ViridianCity_Text_ExplainCaterpieWeedle
	release
	end

ViridianCity_EventScript_YoungsterDeclineExplanation::
	msgbox ViridianCity_Text_OhOkayThen
	release
	end

ViridianCity_EventScript_Woman::
	lock
	faceplayer
	goto_if_eq VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 0, ViridianCity_EventScript_WomanRoadBlocked
	msgbox ViridianCity_Text_GoShoppingInPewterOccasionally
	release
	end

ViridianCity_EventScript_WomanRoadBlocked::
	msgbox ViridianCity_Text_GrandpaHasntHadCoffeeYet
	closemessage
	applymovement LOCALID_VIRIDIAN_WOMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

ViridianCity_EventScript_DreamEaterTutor::
	goto EventScript_DreamEaterTutor
	end

ViridianCity_EventScript_RoadBlocked::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	msgbox ViridianCity_Text_ThisIsPrivateProperty
	closemessage
	applymovement LOCALID_PLAYER, ViridianCity_Movement_WalkDown
	waitmovement 0
	releaseall
	end

ViridianCity_Movement_WalkDown::
	walk_down
	step_end

ViridianCity_EventScript_TutorialTriggerLeft::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_TUTORIAL_MAN, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	call ViridianCity_EventScript_DoTutorialBattle
	release
	end

ViridianCity_EventScript_TutorialTriggerRight::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_TUTORIAL_MAN, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	call ViridianCity_EventScript_DoTutorialBattle
	release
	end

ViridianCity_EventScript_DoTutorialBattle::
	msgbox ViridianCity_Text_ShowYouHowToCatchMons
	closemessage
	special StartOldManTutorialBattle
	waitstate
	lock
	faceplayer
	msgbox ViridianCity_Text_ThatWasEducationalTakeThis
	setvar VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 2
	giveitem ITEM_TEACHY_TV
	msgbox ViridianCity_Text_WatchThatToLearnBasics
	return

ViridianCity_Text_CanCarryMonsAnywhere::
	.string "{JPN}きみが こしに つけてる\n"
	.string "その モンスターボールは…!\l"
	.string "まさしく ポケモンだね!\p"
	.string "いつでも ポケモンの だしいれが\n"
	.string "じゆうに できる なんて\l"
	.string "べんりな よのなかだね$"

ViridianCity_Text_GymClosedWonderWhoLeaderIs::
	.string "{JPN}いつ きても\n"
	.string "この ポケモン ジムは しまっとる\p"
	.string "いったい どんな ヤツが\n"
	.string "リーダーを しとるんじゃろか?$"

ViridianCity_Text_ViridiansGymLeaderReturned::
	.string "{JPN}トキワ ジムの リーダーが\n"
	.string "かえって きたぞ!$"

ViridianCity_Text_WantToKnowAboutCaterpillarMons::
	.string "{JPN}いもむし ポケモンには\n"
	.string "2しゅるい いるって しらない?$"

ViridianCity_Text_OhOkayThen::
	.string "{JPN}それなら いいんだ!$"

ViridianCity_Text_ExplainCaterpieWeedle::
	.string "{JPN}キャタピーは どくが ないけど\n"
	.string "ビードルには どくが あるよ\p"
	.string "ポケモンが さされないように\n"
	.string "きを つけようね$"

ViridianCity_Text_GrandpaHasntHadCoffeeYet::
	.string "{JPN}あらら じいちゃん!\n"
	.string "こんな ところで ねちゃって\l"
	.string "しょーが ないわね!\l"
	.string "よいが さめるまで まつしかないわ$"

ViridianCity_Text_GoShoppingInPewterOccasionally::
	.string "{JPN}ときどき ニビシティまで\n"
	.string "おかいものに いきますけど\p"
	.string "トキワのもり って\n"
	.string "みちが まがり くねってるのよ$"

ViridianCity_Text_ThisIsPrivateProperty::
	.string "{JPN}ういーっ! ひっく… まちやがれ!\n"
	.string "わしの はなしを きけ!\p"
	.string "… こら!\n"
	.string "いくな! と いっとろーが!$"

ViridianCity_Text_ShowYouHowToCatchMons::
	.string "{JPN}うーん…\n"
	.string "よっぱらってた みたいじゃ!\p"
	.string "おや?\n"
	.string "その あかい はこは なんじゃ?\p"
	.string "ほっほう!\n"
	.string "ポケモンずかん つくっとるか\l"
	.string "なら わしから アドバイスじゃ!\p"
	.string "ポケモンを つかまえて しらべれば\n"
	.string "じどうてきに ページが\l"
	.string "ふえて いくんじゃよ!\p"
	.string "なんじゃー\n"
	.string "つかまえかたを しらんのか!\p"
	.string "では… わしが\n"
	.string "おてほんを みせて やるかな!$"

ViridianCity_Text_ThatWasEducationalTakeThis::
	.string "{JPN}どうじゃ!\n"
	.string "すこしは やくに たったじゃろう?\p"
	.string "あとは これを もっていけ$"

ViridianCity_Text_WatchThatToLearnBasics::
	.string "{JPN}わからん ことが あれば\n"
	.string "それを みてみると よいじゃろう\l"
	.string "ポケモントレーナーの\l"
	.string "きほんを しることが できるぞ!$"

ViridianCity_Text_WeakenMonsFirstToCatch::
	.string "{JPN}うーん…\n"
	.string "よっぱらってた みたいじゃ!\p"
	.string "あたまが いたい…\p"
	.string "ときに…\n"
	.string "ポケモンずかんは うまっとるかな?\p"
	.string "はじめの うちは\n"
	.string "ポケモンを よわらせてから\l"
	.string "とるのが コツじゃぞ!$"

ViridianCity_Text_HowsTeachyTVHelping::
	.string "{JPN}うーん…\n"
	.string "よっぱらってた みたいじゃ!\p"
	.string "あたまが いたい…\p"
	.string "ときに…\n"
	.string "おしえテレビは やくにたっとる?$"

ViridianCity_Text_MyGrandsonOnTheShow::
	.string "{JPN}わっはっは!\n"
	.string "わしの マゴが でてきて\l"
	.string "おしえて くれるん だから\l"
	.string "とうぜん じゃな!$"

ViridianCity_Text_TooBusyForTeachyTV::
	.string "{JPN}ふむ… おしえテレビを\n"
	.string "みる じかんも ないとは…\p"
	.string "タイム イズ マネー…\n"
	.string "… ときは かねなりか$"

ViridianCity_Text_CitySign::
	.string "{JPN}ここは トキワシティ\n"
	.string "トキワは みどり えいえんのいろ$"

ViridianCity_Text_CatchMonsForEasierBattles::
	.string "{JPN}… おとくな けいじばん!\p"
	.string "とにかく ポケモンを つかまえて\n"
	.string "ドンドン ふやそう!\l"
	.string "トレーナーとの しょうぶも\l"
	.string "たくさん もってると ゆうり!$"

ViridianCity_Text_MovesLimitedByPP::
	.string "{JPN}… おとくな けいじばん!\p"
	.string "ポケモンには たいりょくの ほか\n"
	.string "わざを くりだす げんきの もと\l"
	.string "パワーポイントが あります\p"
	.string "わざ ごとの パワーポイントが\n"
	.string "なくなった ときも\l"
	.string "ポケモンセンターで やすませて!$"

ViridianCity_Text_GymSign::
	.string "{JPN}トキワ ポケモンジム$"

ViridianCity_Text_GymDoorsAreLocked::
	.string "{JPN}トキワ ジムのドアには\n"
	.string "カギが かかって いた…!$"

