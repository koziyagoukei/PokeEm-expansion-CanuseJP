@ The Frontier story after the opening: Kazuki at the Battle Tower, the release battles,
@ Darkrai joining, the first cure, and the camp visitors. See src/frontier_story.c.

FrontierStory_EventScript_Return::
	return

@ ---------------------------------------------------------------------------
@ Release battles

@ Chooses three Pokémon for a release battle. VAR_RESULT is FALSE if the player backed out,
@ in which case the party is already restored.
FrontierStory_EventScript_ChooseReleaseParty::
	special SavePlayerParty
	fadescreen FADE_TO_BLACK
	special ChooseHalfPartyForBattle
	goto_if_ne VAR_RESULT, FALSE, FrontierStory_EventScript_ReduceReleaseParty
	special LoadPlayerParty
	return

FrontierStory_EventScript_ReduceReleaseParty::
	special ReducePlayerPartyToSelectedMons
	setvar VAR_RESULT, TRUE
	return

@ Call after setmultitrainerbattle. Fights beside the cook, then puts the whole party back
@ the way it was; VAR_RESULT is the B_OUTCOME_*.
FrontierStory_EventScript_StartReleaseBattle::
	special FrontierStory_PrepareReleaseBattle
	setvar VAR_0x8005, MULTI_BATTLE_2_VS_2 | MULTI_BATTLE_CHOOSE_MONS
	callnative BattleSetup_StartMultiBattle
	waitstate
	special FrontierStory_EndStoryBattle
	special LoadPlayerParty
	return

@ ---------------------------------------------------------------------------
@ Hub: Pecharunt, Darkrai, and the scenes that happen there

FrontierStory_EventScript_SetupHub::
	setflag FLAG_TEMP_11
	setflag FLAG_TEMP_12
	call_if_set FLAG_STORY_DARKRAI_JOINED, FrontierStory_EventScript_ShowHubDarkrai
	call_if_eq VAR_TOWER_STORY_STATE, TOWER_STORY_RETURNED, TowerStory_EventScript_SetupHubCare
	return

FrontierStory_EventScript_ShowHubDarkrai::
	clearflag FLAG_TEMP_11
	return

FrontierStory_EventScript_HubArrival::
	setvar VAR_TEMP_2, 1
	special FrontierStory_CanDarkraiJoin
	goto_if_eq VAR_RESULT, TRUE, FrontierStory_EventScript_DarkraiJoins
	end

FrontierStory_EventScript_DarkraiJoins::
	lockall
	playse SE_M_NIGHTMARE
	waitse
	msgbox FrontierStory_Text_DarkraiCallsInHub, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	clearflag FLAG_TEMP_11
	addobject LOCALID_HUB_DARKRAI
	clearflag FLAG_TEMP_10
	addobject LOCALID_FRONTIER_HUB_INTRO_COOK
	setvar VAR_0x8004, LOCALID_FRONTIER_HUB_INTRO_COOK
	special FrontierStory_PlaceBesidePlayer
	fadescreen FADE_FROM_BLACK
	msgbox FrontierStory_Text_CookHeardVoice, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_DarkraiIntroduces, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookAboutPecharunt, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_DarkraiAboutPecharunt, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_DarkraiWantsToHelp, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookWelcomesDarkrai, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_DarkraiFavoriteFood, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookKnowsRecipe, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_DarkraiThanks, MSGBOX_DEFAULT
	setflag FLAG_STORY_DARKRAI_JOINED
	msgbox FrontierStory_Text_CookExplainsReleaseBattles, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	setflag FLAG_TEMP_10
	removeobject LOCALID_FRONTIER_HUB_INTRO_COOK
	fadescreen FADE_FROM_BLACK
	releaseall
	end

FrontierStory_EventScript_Pecharunt::
	lock
	goto_if_set FLAG_STORY_CURE_FOUND, FrontierStory_EventScript_PecharuntCured
	msgbox FrontierStory_Text_PecharuntSleeping, MSGBOX_DEFAULT
	release
	end

FrontierStory_EventScript_PecharuntCured::
	msgbox FrontierStory_Text_PecharuntStillSleeping, MSGBOX_DEFAULT
	release
	end

FrontierStory_EventScript_Darkrai::
	lock
	faceplayer
	goto_if_set FLAG_STORY_CURE_FOUND, FrontierStory_EventScript_DarkraiAfterCure
	msgbox FrontierStory_Text_DarkraiWatching, MSGBOX_DEFAULT
	release
	end

FrontierStory_EventScript_DarkraiAfterCure::
	special FrontierStory_ShouldSuggestInn
	goto_if_eq VAR_RESULT, TRUE, FrontierStory_EventScript_DarkraiSuggestsInnInHub
	msgbox FrontierStory_Text_DarkraiSearching, MSGBOX_DEFAULT
	release
	end

FrontierStory_EventScript_DarkraiSuggestsInnInHub::
	call FrontierStory_EventScript_DarkraiSuggestsInn
	release
	end

@ ---------------------------------------------------------------------------
@ The inn across the sea, and the ferry from the Frontier harbor

@ After a cure at the camp: Darkrai calls from the hub once more than one person is cured.
FrontierStory_EventScript_TrySuggestInn::
	special FrontierStory_ShouldSuggestInn
	goto_if_eq VAR_RESULT, FALSE, FrontierStory_EventScript_Return
	playse SE_M_NIGHTMARE
	waitse
	msgbox FrontierStory_Text_DarkraiReachesOut, MSGBOX_DEFAULT
	call FrontierStory_EventScript_DarkraiSuggestsInn
	return

FrontierStory_EventScript_DarkraiSuggestsInn::
	msgbox FrontierStory_Text_DarkraiSuggestsInn, MSGBOX_DEFAULT
	setflag FLAG_STORY_INN_OPEN
	playfanfare MUS_OBTAIN_ITEM
	message FrontierStory_Text_InnOpened
	waitmessage
	waitfanfare
	waitbuttonpress
	closemessage
	return

FrontierStory_EventScript_HarborFerry::
	goto_if_set FLAG_STORY_INN_VISITED, FrontierStory_EventScript_HarborFerryAgain
	msgbox FrontierStory_Text_HarborHeardVoice, MSGBOX_DEFAULT
FrontierStory_EventScript_HarborFerryAgain::
	msgbox FrontierStory_Text_HarborToInn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, FrontierStory_EventScript_HarborFerryLater
	msgbox FrontierStory_Text_HarborDeparting, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_FERRY_ATTENDANT, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 30
	hideobjectat LOCALID_FRONTIER_FERRY_ATTENDANT, MAP_BATTLE_FRONTIER_OUTSIDE_WEST
	call FrontierStory_EventScript_BoardFerry
	@ The harbor's south pier lies east of the berth, so the ship turns and leaves westward.
	applymovement LOCALID_FRONTIER_SS_TIDAL, FrontierStory_Movement_FerrySailWest
	waitmovement 0
	warp MAP_RESTORATION_INN_OUTSIDE, 18, 29
	waitstate
	release
	end

FrontierStory_EventScript_HarborFerryLater::
	msgbox FrontierStory_Text_HarborLater, MSGBOX_DEFAULT
	release
	end

FrontierStory_EventScript_InnSailor::
	lock
	faceplayer
	msgbox FrontierStory_Text_InnSailorAsks, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, FrontierStory_EventScript_InnSailorStay
	msgbox FrontierStory_Text_InnSailorDeparting, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_INN_SAILOR, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 30
	hideobjectat LOCALID_INN_SAILOR, MAP_RESTORATION_INN_OUTSIDE
	call FrontierStory_EventScript_BoardFerry
	applymovement LOCALID_INN_SS_TIDAL, FrontierStory_Movement_FerrySailEast
	waitmovement 0
	warp MAP_BATTLE_FRONTIER_OUTSIDE_WEST, 19, 67
	waitstate
	release
	end

FrontierStory_EventScript_InnSailorStay::
	msgbox FrontierStory_Text_InnSailorStay, MSGBOX_DEFAULT
	release
	end

@ The player steps onto the crew member's spot and goes aboard; a following Pokémon returns
@ to its Poké Ball first, or it would stay on the pier.
FrontierStory_EventScript_BoardFerry::
	hidefollower
	call_if_eq VAR_FACING, DIR_SOUTH, FrontierStory_EventScript_BoardSouth
	call_if_eq VAR_FACING, DIR_EAST, FrontierStory_EventScript_BoardEast
	delay 30
	hideplayer
	delay 60
	return

FrontierStory_EventScript_BoardSouth::
	applymovement LOCALID_PLAYER, Common_Movement_WalkDown
	waitmovement 0
	return

FrontierStory_EventScript_BoardEast::
	applymovement LOCALID_PLAYER, Common_Movement_WalkRight
	waitmovement 0
	return

@ ---------------------------------------------------------------------------
@ Battle Tower: Kazuki

TowerStory_EventScript_SetupLobby::
	setflag FLAG_TEMP_11
	setflag FLAG_TEMP_12
	setflag FLAG_TEMP_13
	goto_if_ne VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_COMPLETE, FrontierStory_EventScript_Return
	goto_if_ge VAR_TOWER_STORY_STATE, TOWER_STORY_RETURNED, FrontierStory_EventScript_Return
	clearflag FLAG_TEMP_11
	return

@ Once per visit, after any challenge result: Darkrai calls the player to the hub.
TowerStory_EventScript_LobbyArrival::
	setvar VAR_TEMP_2, 1
	special FrontierStory_CanDarkraiJoin
	goto_if_eq VAR_RESULT, FALSE, TowerStory_EventScript_LobbyArrivalEnd
	lockall
	playse SE_M_NIGHTMARE
	waitse
	msgbox FrontierStory_Text_DarkraiCallsFromAfar, MSGBOX_DEFAULT
	releaseall
TowerStory_EventScript_LobbyArrivalEnd::
	end

TowerStory_EventScript_Kazuki::
	lock
	faceplayer
	switch VAR_TOWER_STORY_STATE
	case TOWER_STORY_NOT_MET, TowerStory_EventScript_KazukiFirstMeeting
	case TOWER_STORY_MET, TowerStory_EventScript_KazukiMet
	case TOWER_STORY_WISH, TowerStory_EventScript_KazukiWish
	release
	end

TowerStory_EventScript_KazukiFirstMeeting::
	playse SE_PIN
	applymovement LOCALID_TOWER_LOBBY_KAZUKI, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox TowerStory_Text_KazukiMistakesPlayer, MSGBOX_DEFAULT
	applymovement LOCALID_TOWER_LOBBY_KAZUKI, Common_Movement_QuestionMark
	waitmovement 0
	msgbox TowerStory_Text_KazukiNoticesDifference, MSGBOX_DEFAULT
	msgbox TowerStory_Text_PlayerExplains, MSGBOX_DEFAULT
	msgbox TowerStory_Text_KazukiUnderstands, MSGBOX_DEFAULT
	setvar VAR_TOWER_STORY_STATE, TOWER_STORY_MET
	goto_if_set FLAG_SYS_TOWER_SILVER, TowerStory_EventScript_KazukiWishSurfaces
	release
	end

TowerStory_EventScript_KazukiMet::
	goto_if_set FLAG_SYS_TOWER_SILVER, TowerStory_EventScript_KazukiWishSurfaces
	msgbox TowerStory_Text_KazukiHappyTogether, MSGBOX_DEFAULT
	release
	end

TowerStory_EventScript_KazukiWishSurfaces::
	msgbox TowerStory_Text_KazukiSawSilverBattle, MSGBOX_DEFAULT
	setvar VAR_TOWER_STORY_STATE, TOWER_STORY_WISH
	goto_if_set FLAG_SYS_TOWER_GOLD, TowerStory_EventScript_KazukiFever
	release
	end

TowerStory_EventScript_KazukiWish::
	goto_if_set FLAG_SYS_TOWER_GOLD, TowerStory_EventScript_KazukiFever
	msgbox TowerStory_Text_KazukiWantsRematch, MSGBOX_DEFAULT
	release
	end

TowerStory_EventScript_KazukiFever::
	goto_if_unset FLAG_STORY_DARKRAI_JOINED, TowerStory_EventScript_KazukiFeverAlone
	lockall
	msgbox TowerStory_Text_KazukiFever, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_TOWER_LOBBY_KAZUKI, TowerStory_Movement_Tremble
	waitmovement 0
	playse SE_M_CONFUSE_RAY
	fadescreen FADE_TO_WHITE
	clearflag FLAG_TEMP_12
	addobject LOCALID_TOWER_LOBBY_COOK
	clearflag FLAG_TEMP_13
	addobject LOCALID_TOWER_LOBBY_POKEVIRUS
	setvar VAR_0x8004, LOCALID_TOWER_LOBBY_KAZUKI
	setvar VAR_0x8005, LOCALID_TOWER_LOBBY_COOK
	setvar VAR_0x8006, LOCALID_TOWER_LOBBY_POKEVIRUS
	special FrontierStory_ArrangeReleaseScene
	waitse
	fadescreen FADE_FROM_WHITE
	goto_if_set FLAG_STORY_POKEVIRUS_REVEALED, TowerStory_EventScript_PokevirusAgain
	msgbox FrontierStory_Text_CookArrives, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookSeesPokevirus, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_DarkraiNamesPokevirus, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookResolves, MSGBOX_DEFAULT
	setflag FLAG_STORY_POKEVIRUS_REVEALED
	goto TowerStory_EventScript_AskReleaseBattle

TowerStory_EventScript_PokevirusAgain::
	msgbox FrontierStory_Text_CookPokevirusAgain, MSGBOX_DEFAULT
TowerStory_EventScript_AskReleaseBattle::
	msgbox FrontierStory_Text_CookAsksReady, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, TowerStory_EventScript_ReleaseRetreat
	call FrontierStory_EventScript_ChooseReleaseParty
	goto_if_eq VAR_RESULT, FALSE, TowerStory_EventScript_ReleaseRetreat
	setmultitrainerbattle TRAINER_KAZUKI_RELEASE, TowerStory_Text_KazukiReleaseDefeat, TRAINER_POKEVIRUS, FrontierStory_Text_PokevirusDefeat, PARTNER_ROCKET_COOK
	call FrontierStory_EventScript_StartReleaseBattle
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, TowerStory_EventScript_KazukiReleased
	msgbox FrontierStory_Text_CookLostRelease, MSGBOX_DEFAULT
	goto TowerStory_EventScript_ClearReleaseScene

TowerStory_EventScript_ReleaseRetreat::
	msgbox FrontierStory_Text_CookRetreats, MSGBOX_DEFAULT
TowerStory_EventScript_ClearReleaseScene::
	closemessage
	fadescreen FADE_TO_BLACK
	setflag FLAG_TEMP_12
	removeobject LOCALID_TOWER_LOBBY_COOK
	setflag FLAG_TEMP_13
	removeobject LOCALID_TOWER_LOBBY_POKEVIRUS
	fadescreen FADE_FROM_BLACK
	release
	end

TowerStory_EventScript_KazukiFeverAlone::
	msgbox TowerStory_Text_KazukiFeverStarting, MSGBOX_DEFAULT
	closemessage
	playse SE_M_NIGHTMARE
	waitse
	msgbox FrontierStory_Text_DarkraiWarnsAlone, MSGBOX_DEFAULT
	release
	end

TowerStory_EventScript_KazukiReleased::
	msgbox TowerStory_Text_KazukiVoicesFade, MSGBOX_DEFAULT
	msgbox TowerStory_Text_KazukiChooses, MSGBOX_DEFAULT
	closemessage
	playse SE_M_HAZE
	applymovement LOCALID_TOWER_LOBBY_POKEVIRUS, TowerStory_Movement_PokevirusFades
	waitmovement 0
	removeobject LOCALID_TOWER_LOBBY_POKEVIRUS
	setflag FLAG_TEMP_13
	waitse
	msgbox TowerStory_Text_KazukiCollapses, MSGBOX_DEFAULT
	msgbox TowerStory_Text_CookCarriesKazuki, MSGBOX_DEFAULT
	closemessage
	setvar VAR_TOWER_STORY_STATE, TOWER_STORY_RETURNED
	warp MAP_ULTRA_FRONTIE_HUB, 7, 5
	waitstate
	end

@ In the hub straight after the release battle. The first rescue anywhere finds the cure.
TowerStory_EventScript_SetupHubCare::
	clearflag FLAG_TEMP_12
	clearflag FLAG_TEMP_10
	setobjectxyperm LOCALID_FRONTIER_HUB_INTRO_COOK, 4, 6
	setobjectmovementtype LOCALID_FRONTIER_HUB_INTRO_COOK, MOVEMENT_TYPE_FACE_UP
	return

TowerStory_EventScript_HubCare::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox TowerStory_Text_KazukiDelirious, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookForcesMeal, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_HUB_INTRO_COOK, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox TowerStory_Text_KazukiEats, MSGBOX_DEFAULT
	goto_if_set FLAG_STORY_CURE_FOUND, TowerStory_EventScript_GiveKnownCure
	msgbox FrontierStory_Text_CookServesDessert, MSGBOX_DEFAULT
	call FrontierStory_EventScript_CureFlash
	msgbox TowerStory_Text_KazukiWakesUp, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookWhatHappened, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_DarkraiSensesCure, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookAmazed, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	msgbox FrontierStory_Text_FeedPecharunt, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_FROM_BLACK
	msgbox FrontierStory_Text_DarkraiPecharuntStillAsleep, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookWillSearch, MSGBOX_DEFAULT
	setflag FLAG_STORY_CURE_FOUND
	msgbox FrontierStory_Text_CookHandsKibidango, MSGBOX_DEFAULT
	goto TowerStory_EventScript_KazukiCured

TowerStory_EventScript_GiveKnownCure::
	msgbox FrontierStory_Text_CookKnownCure, MSGBOX_DEFAULT
	call FrontierStory_EventScript_CureFlash
	msgbox TowerStory_Text_KazukiWakesUp, MSGBOX_DEFAULT
TowerStory_EventScript_KazukiCured::
	msgbox TowerStory_Text_KazukiRemembersAll, MSGBOX_DEFAULT
	msgbox TowerStory_Text_KazukiAsksPokenav, MSGBOX_DEFAULT
	setflag FLAG_ADDED_MATCH_CALL_TO_POKENAV
	register_matchcall TRAINER_CALVIN_1
	msgbox TowerStory_Text_KazukiGoesToResort, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookSendsToResort, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	setflag FLAG_TEMP_12
	removeobject LOCALID_HUB_KAZUKI
	setflag FLAG_TEMP_10
	removeobject LOCALID_FRONTIER_HUB_INTRO_COOK
	setvar VAR_TOWER_STORY_STATE, TOWER_STORY_RESCUED
	setflag FLAG_STORY_RESCUED_TOWER
	fadescreen FADE_FROM_BLACK
	releaseall
	end

FrontierStory_EventScript_CureFlash::
	closemessage
	playse SE_M_HEAL_BELL
	fadescreen FADE_TO_WHITE
	waitse
	fadescreen FADE_FROM_WHITE
	return

@ ---------------------------------------------------------------------------
@ The camp: Kazuki after his rescue, and visitors who remembered themselves

FrontierStory_EventScript_SetupCamp::
	setflag FLAG_TEMP_17
	call_if_eq VAR_TOWER_STORY_STATE, TOWER_STORY_RESCUED, TowerStory_EventScript_ShowResortKazuki
	setflag FLAG_TEMP_18
	special FrontierStory_PickCampVisitor
	goto_if_eq VAR_RESULT, FALSE, FrontierStory_EventScript_Return
	clearflag FLAG_TEMP_18
	return

TowerStory_EventScript_ShowResortKazuki::
	clearflag FLAG_TEMP_17
	return

TowerStory_EventScript_ResortKazuki::
	lock
	faceplayer
	msgbox TowerStory_Text_ResortKazukiChallenge, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, TowerStory_EventScript_ResortKazukiDecline
	msgbox TowerStory_Text_ResortKazukiHere, MSGBOX_DEFAULT
	closemessage
	special SavePlayerParty
	special FrontierStory_PrepareMatchedBattle
	setflag FLAG_EXPANSION_NO_WHITEOUT
	trainerbattle_no_intro TRAINER_KAZUKI_RESORT, TowerStory_Text_ResortKazukiDefeat
	special FrontierStory_EndStoryBattle
	clearflag FLAG_EXPANSION_NO_WHITEOUT
	special LoadPlayerParty
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, TowerStory_EventScript_ResortKazukiLost
	msgbox TowerStory_Text_ResortKazukiWon, MSGBOX_DEFAULT
	release
	end

TowerStory_EventScript_ResortKazukiLost::
	msgbox TowerStory_Text_ResortKazukiAfterLoss, MSGBOX_DEFAULT
	release
	end

TowerStory_EventScript_ResortKazukiDecline::
	msgbox TowerStory_Text_ResortKazukiLater, MSGBOX_DEFAULT
	release
	end

FrontierStory_EventScript_CampVisitor::
	lock
	faceplayer
	setvar VAR_0x8004, CAMP_VISITOR_TEXT_GREETING
	special FrontierStory_BufferVisitorText
	msgbox gStringVar4, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_OfferKibidango, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, FrontierStory_EventScript_VisitorNotYet
	msgbox FrontierStory_Text_VisitorEats, MSGBOX_DEFAULT
	call FrontierStory_EventScript_CureFlash
	special FrontierStory_RescueVisitor
	setvar VAR_0x8004, CAMP_VISITOR_TEXT_CURED
	special FrontierStory_BufferVisitorText
	msgbox gStringVar4, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	setflag FLAG_TEMP_18
	removeobject LOCALID_PARTY_ROAMER_VISITOR
	setvar VAR_CAMP_VISITOR, NAMED_STORY_NONE
	fadescreen FADE_FROM_BLACK
	call FrontierStory_EventScript_TrySuggestInn
	release
	end

FrontierStory_EventScript_VisitorNotYet::
	msgbox FrontierStory_Text_VisitorWillWait, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ Movements

TowerStory_Movement_Tremble:
	walk_in_place_faster_left
	walk_in_place_faster_right
	walk_in_place_faster_left
	walk_in_place_faster_right
	delay_16
	step_end

TowerStory_Movement_PokevirusFades:
	set_invisible
	delay_4
	set_visible
	delay_4
	set_invisible
	delay_8
	set_visible
	delay_8
	set_invisible
	step_end

FrontierStory_Movement_FerrySailWest:
	face_left
	delay_16
	walk_slow_left
	walk_slow_left
	walk_slow_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

FrontierStory_Movement_FerrySailEast:
	walk_slow_right
	walk_slow_right
	walk_slow_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

@ ---------------------------------------------------------------------------
@ Text: shared

FrontierStory_Text_DarkraiCallsFromAfar:
	.string "{JPN}？？？『……きこえるか。\p"
	.string "シンボルを えた トレーナー。\n"
	.string "あなたに たのみたい ことが ある。\p"
	.string "ハブで まっている。』$"

FrontierStory_Text_DarkraiWarnsAlone:
	.string "{JPN}？？？『……いけない。\n"
	.string "ひとりで ちかづいては だめだ。\p"
	.string "まずは ハブへ きてほしい。』$"

FrontierStory_Text_DarkraiCallsInHub:
	.string "{JPN}？？？『……きてくれたか。\n"
	.string "わたしは ここだ。』$"

FrontierStory_Text_CookHeardVoice:
	.string "{JPN}てんしゅ『{PLAYER}！\n"
	.string "いまの こえ…… きこえたか！？\p"
	.string "あたまの なかに ちょくせつ\n"
	.string "ひびいて きやがった……！』$"

FrontierStory_Text_DarkraiIntroduces:
	.string "{JPN}？？？『おどろかせて すまない。\p"
	.string "わたしは ダークライ。\n"
	.string "こころに ことばを とどけている。\p"
	.string "そこで ねむっている モモワロウを\n"
	.string "ずっと そばで みまもってきた。』$"

FrontierStory_Text_CookAboutPecharunt:
	.string "{JPN}てんしゅ『モモワロウ……\n"
	.string "ハブの すみで ねむってる あいつか。\p"
	.string "なにを しても めを さまさねえから\n"
	.string "そっと しておいたんだが……。』$"

FrontierStory_Text_DarkraiAboutPecharunt:
	.string "{JPN}ダークライ『あの こは とおい せかいから\n"
	.string "ひとりで まよいこんできた。\p"
	.string "わたしは あの こと しばらく\n"
	.string "いっしょに くらしていた。\p"
	.string "……その あの こに ポケルスが うつり\n"
	.string "かたちを かえて ひろがった。\p"
	.string "ひとと ポケモンの こころを\n"
	.string "まぜあわせる ものとして。』$"

FrontierStory_Text_DarkraiWantsToHelp:
	.string "{JPN}ダークライ『{PLAYER}。\n"
	.string "あなたが シンボルを えたとき\l"
	.string "つながりの なかで だれかの こころが\l"
	.string "ゆれるのを かんじた。\p"
	.string "わたしも ちからを かしたい。\n"
	.string "……あの こを たすけたいのだ。』$"

FrontierStory_Text_CookWelcomesDarkrai:
	.string "{JPN}てんしゅ『……ジガルデの やつも\n"
	.string "だまって うなずいてやがる。\p"
	.string "わかった。 ダークライ。\n"
	.string "おまえも おれたちの なかまだ。』$"

FrontierStory_Text_DarkraiFavoriteFood:
	.string "{JPN}ダークライ『ひとつ おもいだした\n"
	.string "ことが ある。\p"
	.string "モモワロウは よく はなしていた。\n"
	.string "しかくくて あまい だんごが すきだと。\p"
	.string "むかし あの こを ひろってくれた\n"
	.string "いえで たべていた……\p"
	.string "きびだんご と よんでいた。』$"

FrontierStory_Text_CookKnowsRecipe:
	.string "{JPN}てんしゅ『しかくい きびだんご……？\p"
	.string "そりゃ うちに むかしから\n"
	.string "つたわってる きびだんごと おなじだ。\p"
	.string "……そういや ばあちゃんが いってたな。\n"
	.string "ずっと むかし ごせんぞさまが\l"
	.string "ふしぎな ポケモンを\l"
	.string "あずかってた ことが あるって。\p"
	.string "……ま むかしばなしだ。\n"
	.string "だが レシピなら おぼえてる。\p"
	.string "つくって おいてやる。 あいつが\n"
	.string "めを さましたら くわせてやろうぜ。』$"

FrontierStory_Text_DarkraiThanks:
	.string "{JPN}ダークライ『……ありがとう。』$"

FrontierStory_Text_CookExplainsReleaseBattles:
	.string "{JPN}てんしゅ『{PLAYER}。\n"
	.string "きんいろの シンボルを とった しせつで\l"
	.string "ようすの おかしい やつが いたら\l"
	.string "そいつに はなしかけてみろ。\p"
	.string "なにか おきたら おれと ダークライが\n"
	.string "すぐに かけつける。\p"
	.string "そのときは ふたりで くんで\n"
	.string "たたかうぞ！』$"

FrontierStory_Text_PecharuntSleeping:
	.string "{JPN}モモワロウが ねむっている。\p"
	.string "ゆすっても よびかけても\n"
	.string "まったく はんのうが ない……。$"

FrontierStory_Text_PecharuntStillSleeping:
	.string "{JPN}モモワロウが しずかに ねむっている。\p"
	.string "かおいろは よくなったが\n"
	.string "めを さます けはいは ない……。$"

FrontierStory_Text_DarkraiWatching:
	.string "{JPN}ダークライ『わたしは ここで\n"
	.string "あの こを みまもっている。\p"
	.string "なにか あれば あなたの こころに\n"
	.string "よびかけよう。』$"

FrontierStory_Text_DarkraiSearching:
	.string "{JPN}ダークライ『からだは なおった。\n"
	.string "だが こころが もどってこない。\p"
	.string "あの こは きっと まだ どこかで\n"
	.string "ゆめを みている……。』$"

FrontierStory_Text_CookArrives:
	.string "{JPN}てんしゅ『{PLAYER}！ まにあったか！』$"

FrontierStory_Text_CookSeesPokevirus:
	.string "{JPN}てんしゅ『……なんだ ありゃあ……！？』$"

FrontierStory_Text_DarkraiNamesPokevirus:
	.string "{JPN}ダークライ『……あれが ヒトポケルスだ。\p"
	.string "みんなの こころを つないでいる ものが\n"
	.string "はじめて かたちを もって あらわれた。\p"
	.string "はなれようと する こころを\n"
	.string "ひきとめようと しているのだ。』$"

FrontierStory_Text_CookResolves:
	.string "{JPN}てんしゅ『ジガルデが いってた やつか……！\p"
	.string "わるぎが あるのか ないのか\n"
	.string "しらねえが……\p"
	.string "はなれたいって やつを\n"
	.string "ひきとめる すじあいは ねえ！』$"

FrontierStory_Text_CookPokevirusAgain:
	.string "{JPN}てんしゅ『また でやがったな……！\n"
	.string "{PLAYER}！ いくぞ！』$"

FrontierStory_Text_CookAsksReady:
	.string "{JPN}てんしゅ『おれと くんで たたかうぞ。\n"
	.string "ポケモンを 3びき えらんでくれ。\p"
	.string "じゅんびは いいか？』$"

FrontierStory_Text_CookRetreats:
	.string "{JPN}てんしゅ『……いったん さがるぞ。\p"
	.string "じゅんびが できたら\n"
	.string "もういちど はなしかけてやれ。』$"

FrontierStory_Text_CookLostRelease:
	.string "{JPN}てんしゅ『くっ……！ たてなおすぞ！\p"
	.string "なんどでも いい。\n"
	.string "かならず ひきはがしてやる！』$"

FrontierStory_Text_PokevirusDefeat:
	.string "{JPN}…… ……………… ……$"

FrontierStory_Text_CookForcesMeal:
	.string "{JPN}てんしゅ『ほら くえ！\n"
	.string "くわなきゃ もたねえぞ！』$"

FrontierStory_Text_CookServesDessert:
	.string "{JPN}てんしゅ『……よし ぜんぶ くったな。\n"
	.string "しあげに デザートだ。\p"
	.string "ダークライの はなしで\n"
	.string "つくっておいた きびだんごだ。\p"
	.string "あまいもんは こころに きくぜ。』\p"
	.string "てんしゅは しかくい きびだんごを\n"
	.string "ひとつ たべさせた。$"

FrontierStory_Text_CookWhatHappened:
	.string "{JPN}てんしゅ『……おい。\n"
	.string "いま なにが おきた？』$"

FrontierStory_Text_DarkraiSensesCure:
	.string "{JPN}ダークライ『……わかる。\n"
	.string "その こから ウイルスの けはいが\l"
	.string "きえた。\p"
	.string "きびだんごが…… なおしたのだ。』$"

FrontierStory_Text_CookAmazed:
	.string "{JPN}てんしゅ『まじかよ……！\n"
	.string "うちの きびだんごが くすりに？\p"
	.string "……だったら モモワロウにも\n"
	.string "くわせてみる！』$"

FrontierStory_Text_FeedPecharunt:
	.string "{JPN}てんしゅは ねむっている モモワロウの\n"
	.string "くちもとへ きびだんごを はこんだ。\p"
	.string "モモワロウは めを とじた まま\n"
	.string "ちいさく くちを うごかした……。$"

FrontierStory_Text_DarkraiPecharuntStillAsleep:
	.string "{JPN}ダークライ『……モモワロウの からだからも\n"
	.string "ウイルスが きえた。\p"
	.string "……だが めを さまさない。\n"
	.string "こころが まだ もどっていないようだ。』$"

FrontierStory_Text_CookWillSearch:
	.string "{JPN}てんしゅ『なおったのに おきねえ……？\p"
	.string "……わけを さがすしか ねえな。\n"
	.string "なにか きづいたら おしえてくれ。』$"

FrontierStory_Text_CookHandsKibidango:
	.string "{JPN}てんしゅ『きびだんごは おれが\n"
	.string "つくりおきして おく。\p"
	.string "{PLAYER}。 これからは おまえも\n"
	.string "もっていけ。\p"
	.string "まけて じぶんを おもいだした やつが\n"
	.string "いたら わたしてやってくれ。\p"
	.string "なおった やつらは リゾートに くる。\n"
	.string "そこで まってりゃ あえるだろうよ。』$"

FrontierStory_Text_CookKnownCure:
	.string "{JPN}てんしゅ『しあげは こいつだ。』\p"
	.string "てんしゅは きびだんごを\n"
	.string "ひとつ たべさせた。$"

FrontierStory_Text_CookSendsToResort:
	.string "{JPN}てんしゅ『リゾートなら\n"
	.string "めしも ふろも ある。 ゆっくり やすめよ。』$"

FrontierStory_Text_OfferKibidango:
	.string "{JPN}{STR_VAR_1}に\n"
	.string "おもいでの きびだんごを わたしますか？$"

FrontierStory_Text_VisitorEats:
	.string "{JPN}{PLAYER}は {STR_VAR_1}に\n"
	.string "きびだんごを わたした。\p"
	.string "{STR_VAR_1}は しずかに\n"
	.string "きびだんごを たべた……。$"

FrontierStory_Text_VisitorWillWait:
	.string "{JPN}……そうか。\n"
	.string "わたしも まだ まよって いるのかも。\p"
	.string "また くるよ。$"

FrontierStory_Text_DarkraiReachesOut:
	.string "{JPN}ダークライ『……{PLAYER}。\n"
	.string "きこえるか。』$"

FrontierStory_Text_DarkraiSuggestsInn:
	.string "{JPN}ダークライ『なおった ものが\n"
	.string "ふえてきたな。\p"
	.string "だが みなを あつめるには\n"
	.string "リゾートでは せますぎるだろう。\p"
	.string "うみの むこうに ふるい りょかんが\n"
	.string "ある。 いまは だれも つかっていない。\p"
	.string "ひろい へやが いくつも ある。\n"
	.string "こころを やすめるには ちょうど いい。\p"
	.string "みなとの ふねの ものには\n"
	.string "わたしから こえを とどけておこう。\p"
	.string "にしの みなとから わたるといい。』$"

FrontierStory_Text_InnOpened:
	.string "{JPN}うみの むこうの りょかんへ\n"
	.string "ふねで わたれるように なった！$"

FrontierStory_Text_HarborHeardVoice:
	.string "{JPN}あっ ちょうど よかった！\p"
	.string "さっき あたまの なかに\n"
	.string "ふしぎな こえが ひびいたんです。\p"
	.string "うみの むこうの りょかんまで\n"
	.string "ふねを だしてほしいって……。\p"
	.string "なんだか ほうっておけなくて\n"
	.string "ふねを だせるように しておきました！$"

FrontierStory_Text_HarborToInn:
	.string "{JPN}うみの むこうの りょかんへ\n"
	.string "むかいますか？$"

FrontierStory_Text_HarborDeparting:
	.string "{JPN}では しゅっこう します！\n"
	.string "どうぞ おのりください！$"

FrontierStory_Text_HarborLater:
	.string "{JPN}また いつでも どうぞ！$"

FrontierStory_Text_InnSailorAsks:
	.string "{JPN}フロンティアへ もどるかい？$"

FrontierStory_Text_InnSailorDeparting:
	.string "{JPN}よし しゅっこうだ！\n"
	.string "のってくれ！$"

FrontierStory_Text_InnSailorStay:
	.string "{JPN}そうかい。\n"
	.string "もどるときは こえを かけてくれ。$"

@ ---------------------------------------------------------------------------
@ Text: Kazuki

TowerStory_Text_KazukiMistakesPlayer:
	.string "{JPN}カズキ『あっ！ きみ……！\n"
	.string "やっと あえた！\p"
	.string "おぼえてる？ ぼく カズキ！\n"
	.string "102ばんどうろで しょうぶした！\p"
	.string "あのとき ぼく まけちゃったけど\n"
	.string "また しょうぶ しようねって……』$"

TowerStory_Text_KazukiNoticesDifference:
	.string "{JPN}カズキ『……あれ？\p"
	.string "きみの ポケモン\n"
	.string "みたことない こばっかり。\p"
	.string "それに なんだか はじめて\n"
	.string "あったみたいな かお してる……。』$"

TowerStory_Text_PlayerExplains:
	.string "{JPN}{PLAYER}は べつの せかいから\n"
	.string "ここへ きたことを はなした。$"

TowerStory_Text_KazukiUnderstands:
	.string "{JPN}カズキ『べつの せかいの……\n"
	.string "そっくりさん？\p"
	.string "そっか！ だから そっくりなんだ！\n"
	.string "びっくりしたあ！\p"
	.string "でも いいんだ。\n"
	.string "ここでは みんな いっしょ だから。\p"
	.string "だれが かっても みんなの かち。\n"
	.string "きみが タワーで かったら\l"
	.string "ぼくたちも うれしいよ！』$"

TowerStory_Text_KazukiHappyTogether:
	.string "{JPN}カズキ『ここでは みんな いっしょ。\n"
	.string "かっても まけても みんなの きもち。\p"
	.string "だから ぼく ぜんぜん\n"
	.string "くやしくないんだ！ えへへ。』$"

TowerStory_Text_KazukiSawSilverBattle:
	.string "{JPN}カズキ『みてたよ！ シンボルを\n"
	.string "かけた しょうぶ！\p"
	.string "すごかったなあ……\n"
	.string "みんなも よろこんでた。\p"
	.string "……でも なんでだろう。\n"
	.string "むねの おくが ざわざわするんだ。\p"
	.string "ぼく あの ひとに……\n"
	.string "この せかいの きみに\l"
	.string "もういちど いどみたい。\p"
	.string "みんなで かてれば いいはずなのに\n"
	.string "ぼくが いどみたいって\l"
	.string "おもっちゃうんだ。\p"
	.string "……へんだよね。 えへへ。』$"

TowerStory_Text_KazukiWantsRematch:
	.string "{JPN}カズキ『あの ひと いまごろ\n"
	.string "どうしてるかな。\p"
	.string "ぼく まけた ままじゃ いやなんだ。\n"
	.string "……みんなは そう おもわないのにね。』$"

TowerStory_Text_KazukiFeverStarting:
	.string "{JPN}カズキ『……あれ？\n"
	.string "なんだか あたまが ぼーっと する……\p"
	.string "きんいろの シンボル……\n"
	.string "みんなの…… ぼくの……』$"

TowerStory_Text_KazukiFever:
	.string "{JPN}カズキ『……あ……\n"
	.string "きんいろの シンボル……\p"
	.string "みんなで かてば みんなで うれしい。\n"
	.string "みんなで まけても みんなで へいき。\p"
	.string "でも ぼくは…… ぼくが…… いどみ……\p"
	.string "ちがう。 ぼくたちは。 ぼく は。\n"
	.string "ぼく……\p"
	.string "あつい。 あたまが あつい。\p"
	.string "どうして。 どうして はなれるの？\n"
	.string "どうして ひとりに なろうと するの？』$"

TowerStory_Text_KazukiReleaseDefeat:
	.string "{JPN}……ぼく…… まけた……\n"
	.string "ぼくが まけたんだ……$"

TowerStory_Text_KazukiVoicesFade:
	.string "{JPN}カズキ『……みんなの こえが\n"
	.string "とおく なっていく……\p"
	.string "やだな……\n"
	.string "みんなと いっしょだと\l"
	.string "なんにも こわくなかったのに。』$"

TowerStory_Text_KazukiChooses:
	.string "{JPN}カズキ『でも ぼく いま まけたんだ。\n"
	.string "ぼく じしんが。\p"
	.string "くやしい。\n"
	.string "……くやしいって おもえた。\p"
	.string "ぼく もういちど あの ひとに\n"
	.string "じぶんの きもちで いどみたい。\p"
	.string "……ごめんね みんな。\n"
	.string "ぼく いくよ。』$"

TowerStory_Text_KazukiCollapses:
	.string "{JPN}ヒトポケルスの すがたが\n"
	.string "かききえた！\p"
	.string "カズキは その ばに\n"
	.string "ぐったりと たおれこんだ！$"

TowerStory_Text_CookCarriesKazuki:
	.string "{JPN}てんしゅ『おい しっかりしろ！\n"
	.string "……いきは ある。\p"
	.string "……だが うわごとを いってやがる。\n"
	.string "まだ なおっちゃ いねえんだ。\p"
	.string "ハブに はこぶぞ！\n"
	.string "めしを くわせりゃ ちからも もどる！』$"

TowerStory_Text_KazukiDelirious:
	.string "{JPN}カズキ『……みんな…… どこ……\n"
	.string "かつのも…… まけるのも……\l"
	.string "みんなの……』$"

TowerStory_Text_KazukiEats:
	.string "{JPN}てんしゅは カズキの くちに\n"
	.string "めしを どんどん はこんだ！\p"
	.string "カズキ『もが…… もぐ……』$"

TowerStory_Text_KazukiWakesUp:
	.string "{JPN}カズキ『……あれ？\n"
	.string "ここ どこ？\p"
	.string "あたまが すっきりしてる……\n"
	.string "みんなの こえが…… きこえない。』$"

TowerStory_Text_KazukiRemembersAll:
	.string "{JPN}カズキ『……ぜんぶ おぼえてるよ。\p"
	.string "みんなと つながってるの\n"
	.string "すごく あんしん だったんだ。\p"
	.string "だれが かっても ぼくの かちで\n"
	.string "だれも くやしく なくて……\p"
	.string "でも まけて くやしいのも\n"
	.string "ぼくだけの ものなんだね。\p"
	.string "いつか この せかいの あの ひとに\n"
	.string "もういちど いどむよ。\l"
	.string "ぼくの ちからで。\p"
	.string "……たすけて くれて ありがとう！』$"

TowerStory_Text_KazukiAsksPokenav:
	.string "{JPN}カズキ『ねえ ポケナビ もってる？\n"
	.string "れんらくさき こうかんしようよ！』$"

TowerStory_Text_KazukiGoesToResort:
	.string "{JPN}カズキ『ぼく リゾートで まってるね！\n"
	.string "こんどは ぼくと しょうぶしてよ！』$"

TowerStory_Text_ResortKazukiChallenge:
	.string "{JPN}カズキ『あ！ {PLAYER}！\p"
	.string "ぼくね いつか この せかいの\n"
	.string "あの ひとに もういちど いどむって\l"
	.string "きめたんだ。\p"
	.string "そのために いまは\n"
	.string "きみと しょうぶ したい！\p"
	.string "しょうぶ してくれる？』$"

TowerStory_Text_ResortKazukiHere:
	.string "{JPN}カズキ『やった！\n"
	.string "グラエナ！ いくよ！』$"

TowerStory_Text_ResortKazukiDefeat:
	.string "{JPN}くやしい！ でも この くやしさは\n"
	.string "ぼくの ものだ！$"

TowerStory_Text_ResortKazukiAfterLoss:
	.string "{JPN}カズキ『また まけちゃった！\n"
	.string "でも ちゃんと くやしいんだ。\p"
	.string "つぎは もっと つよく なって\n"
	.string "いどむからね！』$"

TowerStory_Text_ResortKazukiWon:
	.string "{JPN}カズキ『かった……！ ぼくが かった！\p"
	.string "……あの ひとにも\n"
	.string "とどくかな。 えへへ。』$"

TowerStory_Text_ResortKazukiLater:
	.string "{JPN}カズキ『そっか！\n"
	.string "また こんど しようね！』$"
