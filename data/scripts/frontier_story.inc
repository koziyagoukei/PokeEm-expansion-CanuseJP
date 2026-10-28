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
	setflag FLAG_TEMP_13
	call_if_set FLAG_STORY_DARKRAI_JOINED, FrontierStory_EventScript_ShowHubDarkrai
	call_if_eq VAR_TOWER_STORY_STATE, TOWER_STORY_RETURNED, TowerStory_EventScript_SetupHubCare
	special FrontierStory_FindReturnedTarget
	call_if_ne VAR_RESULT, STORY_TARGET_COUNT, FrontierStory_EventScript_SetupHubRescue
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
	goto_if_set FLAG_STORY_POKEVIRUS_FADED, FinalStory_EventScript_PecharuntAwake
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
	goto_if_set FLAG_STORY_POKEVIRUS_FADED, FinalStory_EventScript_DarkraiAfterEnding
	goto_if_set FLAG_STORY_PATH_OPEN, FinalStory_EventScript_DarkraiPathOpen
	goto_if_set FLAG_STORY_CURE_FOUND, FrontierStory_EventScript_DarkraiAfterCure
	msgbox FrontierStory_Text_DarkraiWatching, MSGBOX_DEFAULT
	release
	end

FrontierStory_EventScript_DarkraiAfterCure::
	special FrontierStory_ShouldGather
	goto_if_eq VAR_RESULT, TRUE, FinalStory_EventScript_DarkraiGatherHint
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

@ Once per visit, after any challenge result (VAR_TEMP_2 0 -> 1). The first time the lobby is
@ entered at each step of his story, Kazuki comes over to the player and talks at once.
TowerStory_EventScript_LobbyArrival::
	setvar VAR_TEMP_2, 1
	special TowerStory_CheckKazukiApproach
	goto_if_eq VAR_RESULT, FALSE, TowerStory_EventScript_LobbyArrivalEnd
	lockall
	@ Loads him if the player came in on the far side of the lobby.
	addobject LOCALID_TOWER_LOBBY_KAZUKI
	call TowerStory_EventScript_KazukiExclaims
	setvar VAR_0x8004, LOCALID_TOWER_LOBBY_KAZUKI
	special FrontierStory_ApproachPlayer
	goto_if_eq VAR_RESULT, FALSE, TowerStory_EventScript_KazukiTalk
	waitmovement LOCALID_TOWER_LOBBY_KAZUKI
	waitmovement LOCALID_PLAYER
	@ He walks back to his place afterwards.
	setvar VAR_TEMP_3, 1
	goto TowerStory_EventScript_KazukiTalk

@ Then (VAR_TEMP_2 1 -> 2): Darkrai calls the player to the hub.
TowerStory_EventScript_DarkraiCall::
	setvar VAR_TEMP_2, 2
	special FrontierStory_CanDarkraiJoin
	goto_if_eq VAR_RESULT, FALSE, TowerStory_EventScript_LobbyArrivalEnd
	lockall
	playse SE_M_NIGHTMARE
	waitse
	msgbox FrontierStory_Text_DarkraiCallsFromAfar, MSGBOX_DEFAULT
	releaseall
TowerStory_EventScript_LobbyArrivalEnd::
	end

TowerStory_EventScript_KazukiExclaims::
	playse SE_PIN
	applymovement LOCALID_TOWER_LOBBY_KAZUKI, Common_Movement_ExclamationMark
	waitmovement 0
	return

@ If Kazuki came over on his own, he goes back to his place.
TowerStory_EventScript_KazukiDone::
	goto_if_ne VAR_TEMP_3, 1, TowerStory_EventScript_KazukiRelease
	setvar VAR_TEMP_3, 0
	closemessage
	special FrontierStory_ReturnFromApproach
	waitmovement LOCALID_TOWER_LOBBY_KAZUKI
TowerStory_EventScript_KazukiRelease::
	release
	end

@ What Kazuki feels about the others in the network (FrontierStory_BufferTargetBranch).
TowerStory_EventScript_KazukiHappyBranch::
	setvar VAR_0x8005, TARGET_TALK_HAPPY
	goto TowerStory_EventScript_KazukiBranch

TowerStory_EventScript_KazukiWishBranch::
	setvar VAR_0x8005, TARGET_TALK_WISH
TowerStory_EventScript_KazukiBranch::
	setvar VAR_0x8004, STORY_TARGET_KAZUKI
	special FrontierStory_BufferTargetBranch
	call_if_eq VAR_RESULT, TRUE, FrontierStory_EventScript_ShowBufferedText
	return

TowerStory_EventScript_Kazuki::
	lock
	faceplayer
TowerStory_EventScript_KazukiTalk::
	switch VAR_TOWER_STORY_STATE
	case TOWER_STORY_NOT_MET, TowerStory_EventScript_KazukiFirstMeeting
	case TOWER_STORY_MET, TowerStory_EventScript_KazukiMet
	case TOWER_STORY_WISH, TowerStory_EventScript_KazukiWish
	release
	end

TowerStory_EventScript_KazukiFirstMeeting::
	@ Coming over, he already showed his surprise.
	call_if_ne VAR_TEMP_3, 1, TowerStory_EventScript_KazukiExclaims
	msgbox TowerStory_Text_KazukiMistakesPlayer, MSGBOX_DEFAULT
	applymovement LOCALID_TOWER_LOBBY_KAZUKI, Common_Movement_QuestionMark
	waitmovement 0
	msgbox TowerStory_Text_KazukiNoticesDifference, MSGBOX_DEFAULT
	msgbox TowerStory_Text_PlayerExplains, MSGBOX_DEFAULT
	msgbox TowerStory_Text_KazukiUnderstands, MSGBOX_DEFAULT
	call TowerStory_EventScript_KazukiHappyBranch
	setvar VAR_TOWER_STORY_STATE, TOWER_STORY_MET
	goto_if_set FLAG_SYS_TOWER_SILVER, TowerStory_EventScript_KazukiWishSurfaces
	goto TowerStory_EventScript_KazukiDone

TowerStory_EventScript_KazukiMet::
	goto_if_set FLAG_SYS_TOWER_SILVER, TowerStory_EventScript_KazukiWishSurfaces
	msgbox TowerStory_Text_KazukiHappyTogether, MSGBOX_DEFAULT
	call TowerStory_EventScript_KazukiHappyBranch
	goto TowerStory_EventScript_KazukiDone

TowerStory_EventScript_KazukiWishSurfaces::
	msgbox TowerStory_Text_KazukiSawSilverBattle, MSGBOX_DEFAULT
	call TowerStory_EventScript_KazukiWishBranch
	setvar VAR_TOWER_STORY_STATE, TOWER_STORY_WISH
	goto_if_set FLAG_SYS_TOWER_GOLD, TowerStory_EventScript_KazukiFever
	goto TowerStory_EventScript_KazukiDone

TowerStory_EventScript_KazukiWish::
	goto_if_set FLAG_SYS_TOWER_GOLD, TowerStory_EventScript_KazukiFever
	msgbox TowerStory_Text_KazukiWantsRematch, MSGBOX_DEFAULT
	call TowerStory_EventScript_KazukiWishBranch
	goto TowerStory_EventScript_KazukiDone

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
	call FrontierStory_EventScript_PokevirusAppears
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
	@ Darkrai has just asked the player to the hub; no second call on this visit.
	setvar VAR_TEMP_2, 2
	goto TowerStory_EventScript_KazukiDone

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
	msgbox FrontierStory_Text_CookCarriesToHub, MSGBOX_DEFAULT
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
	call FrontierStory_EventScript_ServeFirstDessert
	msgbox TowerStory_Text_KazukiWakesUp, MSGBOX_DEFAULT
	call FrontierStory_EventScript_CureFound
	goto TowerStory_EventScript_KazukiCured

TowerStory_EventScript_GiveKnownCure::
	call FrontierStory_EventScript_GiveKnownCure
	msgbox TowerStory_Text_KazukiWakesUp, MSGBOX_DEFAULT
TowerStory_EventScript_KazukiCured::
	msgbox TowerStory_Text_KazukiRemembersAll, MSGBOX_DEFAULT
	msgbox TowerStory_Text_KazukiAsksPokenav, MSGBOX_DEFAULT
	setflag FLAG_ADDED_MATCH_CALL_TO_POKENAV
	register_matchcall TRAINER_CALVIN_1
	call FrontierStory_EventScript_OpenInnIfNeeded
	msgbox TowerStory_Text_KazukiGoesToInn, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookSendsToInn, MSGBOX_DEFAULT
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
	fadescreenswapbuffers FADE_TO_WHITE
	waitse
	fadescreenswapbuffers FADE_FROM_WHITE
	return

@ ---------------------------------------------------------------------------
@ Shared by every release battle and the cure that follows it

@ Pokevirus has just appeared beside the person: the first time, Darkrai names it.
FrontierStory_EventScript_PokevirusAppears::
	goto_if_set FLAG_STORY_POKEVIRUS_REVEALED, FrontierStory_EventScript_PokevirusAgain
	msgbox FrontierStory_Text_CookArrives, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookSeesPokevirus, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_DarkraiNamesPokevirus, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookResolves, MSGBOX_DEFAULT
	setflag FLAG_STORY_POKEVIRUS_REVEALED
	return

FrontierStory_EventScript_PokevirusAgain::
	goto_if_set FLAG_STORY_POKEVIRUS_FADED, FinalStory_EventScript_PokevirusEcho
	msgbox FrontierStory_Text_CookPokevirusAgain, MSGBOX_DEFAULT
	return

@ The first rescue anywhere finds the cure: the cook's dessert, then, once the person has
@ woken up, what it means.
FrontierStory_EventScript_ServeFirstDessert::
	msgbox FrontierStory_Text_CookServesDessert, MSGBOX_DEFAULT
	call FrontierStory_EventScript_CureFlash
	return

FrontierStory_EventScript_CureFound::
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
	return

FrontierStory_EventScript_GiveKnownCure::
	msgbox FrontierStory_Text_CookKnownCure, MSGBOX_DEFAULT
	call FrontierStory_EventScript_CureFlash
	return

@ The cured rest at the inn; Darkrai finds it for the first of them.
FrontierStory_EventScript_OpenInnIfNeeded::
	goto_if_set FLAG_STORY_INN_OPEN, FrontierStory_EventScript_Return
	call FrontierStory_EventScript_DarkraiSuggestsInn
	return

@ ---------------------------------------------------------------------------
@ The other lobbies: the person the network holds at each facility and the contest.
@ Their lines are in src/data/frontier_story_targets.h. VAR_0x8004 is STORY_TARGET_*.

@ On the map's transition. FLAG_TEMP_1F hides them once the network has let them go.
FrontierStory_EventScript_SetupTarget::
	setflag FLAG_TEMP_1F
	special FrontierStory_IsTargetHere
	goto_if_eq VAR_RESULT, FALSE, FrontierStory_EventScript_Return
	clearflag FLAG_TEMP_1F
	return

@ On entering the lobby (VAR_TEMP_E 0 -> 1): they come over to the player the first time, and
@ the first time after the Silver Symbol (Master Rank) step.
FrontierStory_EventScript_TargetArrival::
	setvar VAR_TEMP_E, 1
	special FrontierStory_CheckTargetApproach
	goto_if_eq VAR_RESULT, FALSE, FrontierStory_EventScript_ArrivalEnd
	lockall
	playse SE_PIN
	special FrontierStory_TargetComesOver
	waitmovement VAR_0x8005
	goto_if_eq VAR_RESULT, FALSE, FrontierStory_EventScript_TargetTalk
	waitmovement LOCALID_PLAYER
	@ They walk back to their place afterwards.
	setvar VAR_TEMP_F, 1
	goto FrontierStory_EventScript_TargetTalk

@ Then (VAR_TEMP_E 1 -> 2): Darkrai calls the player to the hub, as in the Tower.
FrontierStory_EventScript_LobbyDarkraiCall::
	setvar VAR_TEMP_E, 2
	special FrontierStory_CanDarkraiJoin
	goto_if_eq VAR_RESULT, FALSE, FrontierStory_EventScript_ArrivalEnd
	lockall
	playse SE_M_NIGHTMARE
	waitse
	msgbox FrontierStory_Text_DarkraiCallsFromAfar, MSGBOX_DEFAULT
	releaseall
FrontierStory_EventScript_ArrivalEnd::
	end

FrontierStory_EventScript_Target::
	lock
	faceplayer
FrontierStory_EventScript_TargetTalk::
	special FrontierStory_CheckTargetFever
	goto_if_eq VAR_RESULT, TARGET_FEVER_RELEASE, FrontierStory_EventScript_TargetFever
	goto_if_eq VAR_RESULT, TARGET_FEVER_ALONE, FrontierStory_EventScript_TargetFeverAlone
	special FrontierStory_BufferTargetTalk
	copyvar VAR_0x8006, VAR_RESULT
	msgbox gStringVar4, MSGBOX_DEFAULT
	special FrontierStory_BufferTargetBranch
	call_if_eq VAR_RESULT, TRUE, FrontierStory_EventScript_ShowBufferedText
	@ Met for the first time with the symbol already won: their next step follows at once.
	goto_if_eq VAR_0x8006, TRUE, FrontierStory_EventScript_TargetTalk
FrontierStory_EventScript_TargetDone::
	goto_if_ne VAR_TEMP_F, 1, FrontierStory_EventScript_TargetRelease
	setvar VAR_TEMP_F, 0
	closemessage
	special FrontierStory_ReturnFromApproach
	waitmovement VAR_0x8005
FrontierStory_EventScript_TargetRelease::
	release
	end

FrontierStory_EventScript_ShowBufferedText::
	msgbox gStringVar4, MSGBOX_DEFAULT
	return

DomeStory_EventScript_Reika::
	setvar VAR_0x8004, STORY_TARGET_REIKA
	goto FrontierStory_EventScript_Target

DomeStory_EventScript_LobbyArrival::
	setvar VAR_0x8004, STORY_TARGET_REIKA
	goto FrontierStory_EventScript_TargetArrival

PalaceStory_EventScript_Ikue::
	setvar VAR_0x8004, STORY_TARGET_IKUE
	goto FrontierStory_EventScript_Target

PalaceStory_EventScript_LobbyArrival::
	setvar VAR_0x8004, STORY_TARGET_IKUE
	goto FrontierStory_EventScript_TargetArrival

ArenaStory_EventScript_Eisuke::
	setvar VAR_0x8004, STORY_TARGET_EISUKE
	goto FrontierStory_EventScript_Target

ArenaStory_EventScript_LobbyArrival::
	setvar VAR_0x8004, STORY_TARGET_EISUKE
	goto FrontierStory_EventScript_TargetArrival

FactoryStory_EventScript_Toshio::
	setvar VAR_0x8004, STORY_TARGET_TOSHIO
	goto FrontierStory_EventScript_Target

FactoryStory_EventScript_LobbyArrival::
	setvar VAR_0x8004, STORY_TARGET_TOSHIO
	goto FrontierStory_EventScript_TargetArrival

PikeStory_EventScript_Yayoi::
	setvar VAR_0x8004, STORY_TARGET_YAYOI
	goto FrontierStory_EventScript_Target

PikeStory_EventScript_LobbyArrival::
	setvar VAR_0x8004, STORY_TARGET_YAYOI
	goto FrontierStory_EventScript_TargetArrival

PyramidStory_EventScript_Nobuo::
	setvar VAR_0x8004, STORY_TARGET_NOBUO
	goto FrontierStory_EventScript_Target

PyramidStory_EventScript_LobbyArrival::
	setvar VAR_0x8004, STORY_TARGET_NOBUO
	goto FrontierStory_EventScript_TargetArrival

ContestStory_EventScript_Yoriko::
	setvar VAR_0x8004, STORY_TARGET_YORIKO
	goto FrontierStory_EventScript_Target

ContestStory_EventScript_LobbyArrival::
	setvar VAR_0x8004, STORY_TARGET_YORIKO
	goto FrontierStory_EventScript_TargetArrival

@ Then (VAR_TEMP_E 2 -> 3): after each category won at Master Rank past the first, Yoriko comes
@ over with something of the network on her mind, and Darkrai tells the player what it is.
@ Their lines that change with the rescues so far are in src/data/frontier_story_contest.h.
ContestStory_EventScript_LobbyScene::
	setvar VAR_TEMP_E, 3
	special FrontierStory_CheckContestScene
	goto_if_eq VAR_RESULT, FALSE, FrontierStory_EventScript_ArrivalEnd
	lockall
	playse SE_PIN
	setvar VAR_0x8004, STORY_TARGET_YORIKO
	special FrontierStory_TargetComesOver
	waitmovement VAR_0x8005
	goto_if_eq VAR_RESULT, FALSE, ContestStory_EventScript_SceneTalk
	waitmovement LOCALID_PLAYER
	setvar VAR_TEMP_F, 1
ContestStory_EventScript_SceneTalk::
	switch VAR_CONTEST_STORY_SCENES
	case 0, ContestStory_EventScript_SceneApplause
	case 1, ContestStory_EventScript_SceneHandshake
	case 2, ContestStory_EventScript_ScenePartners
	case 3, ContestStory_EventScript_SceneSameDay
	releaseall
	end

ContestStory_EventScript_SceneApplause::
	msgbox ContestStory_Text_YorikoApplause, MSGBOX_DEFAULT
	call ContestStory_EventScript_DarkraiTakesOver
	msgbox ContestStory_Text_DarkraiApplause, MSGBOX_DEFAULT
	goto ContestStory_EventScript_SceneEnd

ContestStory_EventScript_SceneHandshake::
	msgbox ContestStory_Text_YorikoHandshake, MSGBOX_DEFAULT
	call ContestStory_EventScript_DarkraiTakesOver
	msgbox ContestStory_Text_DarkraiHandshake, MSGBOX_DEFAULT
	goto ContestStory_EventScript_SceneEnd

ContestStory_EventScript_ScenePartners::
	msgbox ContestStory_Text_YorikoPartners, MSGBOX_DEFAULT
	call ContestStory_EventScript_DarkraiTakesOver
	msgbox ContestStory_Text_DarkraiPartners, MSGBOX_DEFAULT
	goto ContestStory_EventScript_SceneEnd

ContestStory_EventScript_SceneSameDay::
	msgbox ContestStory_Text_YorikoSameDay, MSGBOX_DEFAULT
	call ContestStory_EventScript_DarkraiTakesOver
	msgbox ContestStory_Text_DarkraiSameDay, MSGBOX_DEFAULT
	goto ContestStory_EventScript_SceneEnd

@ Yoriko's lines for the rescues so far; she goes back to her place, and Darkrai's voice comes.
ContestStory_EventScript_DarkraiTakesOver::
	setvar VAR_0x8004, CONTEST_SCENE_YORIKO
	special FrontierStory_BufferContestSceneBranch
	msgbox gStringVar4, MSGBOX_DEFAULT
	closemessage
	@ Her fever follows the last scene, so then she stays by the player.
	goto_if_eq VAR_CONTEST_STORY_SCENES, CONTEST_STORY_SCENE_COUNT - 1, ContestStory_EventScript_DarkraiSpeaks
	call_if_eq VAR_TEMP_F, 1, ContestStory_EventScript_YorikoGoesBack
ContestStory_EventScript_DarkraiSpeaks:
	playse SE_M_NIGHTMARE
	waitse
	msgbox FrontierStory_Text_DarkraiReachesOut, MSGBOX_DEFAULT
	return

ContestStory_EventScript_YorikoGoesBack::
	setvar VAR_TEMP_F, 0
	special FrontierStory_ReturnFromApproach
	waitmovement VAR_0x8005
	return

ContestStory_EventScript_SceneEnd::
	setvar VAR_0x8004, CONTEST_SCENE_DARKRAI
	special FrontierStory_BufferContestSceneBranch
	msgbox gStringVar4, MSGBOX_DEFAULT
	addvar VAR_CONTEST_STORY_SCENES, 1
	@ After the last scene comes her fever and the release battle.
	setvar VAR_0x8004, STORY_TARGET_YORIKO
	special FrontierStory_CheckTargetFever
	goto_if_eq VAR_RESULT, TARGET_FEVER_RELEASE, FrontierStory_EventScript_TargetFever
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Release battles and cures of the people the network holds at the facilities and the contest
@ (Kazuki's are his own, above). Their lines are in src/data/frontier_story_targets.h.

FrontierStory_EventScript_ShowRescueText::
	special FrontierStory_BufferRescueText
	goto_if_eq VAR_RESULT, FALSE, FrontierStory_EventScript_Return
	msgbox gStringVar4, MSGBOX_DEFAULT
	return

@ VAR_0x8004: STORY_TARGET_*. Their fever before Darkrai has joined: Darkrai stops the player and
@ asks them to the hub.
FrontierStory_EventScript_TargetFeverAlone::
	special FrontierStory_StartRescueScene
	setvar VAR_0x8004, RESCUE_TEXT_FEVER_STARTING
	call FrontierStory_EventScript_ShowRescueText
	closemessage
	playse SE_M_NIGHTMARE
	waitse
	msgbox FrontierStory_Text_DarkraiWarnsAlone, MSGBOX_DEFAULT
	@ Darkrai has just asked the player to the hub; no second call on this visit.
	setvar VAR_TEMP_E, 2
	goto FrontierStory_EventScript_TargetDone

@ VAR_0x8004: STORY_TARGET_*. Their fever; the cook and Pokevirus appear in a flash of white, and
@ the release battle. In every lobby FLAG_TEMP_1D hides the cook and FLAG_TEMP_1E Pokevirus.
FrontierStory_EventScript_TargetFever::
	lockall
	special FrontierStory_StartRescueScene
	setvar VAR_0x8004, RESCUE_TEXT_FEVER
	call FrontierStory_EventScript_ShowRescueText
	closemessage
	special FrontierStory_GetRescueScene
	applymovement VAR_0x8004, TowerStory_Movement_Tremble
	waitmovement 0
	playse SE_M_CONFUSE_RAY
	fadescreen FADE_TO_WHITE
	clearflag FLAG_TEMP_1D
	clearflag FLAG_TEMP_1E
	special FrontierStory_GetRescueScene
	addobject VAR_0x8005
	addobject VAR_0x8006
	special FrontierStory_ArrangeReleaseScene
	waitse
	fadescreen FADE_FROM_WHITE
	call FrontierStory_EventScript_PokevirusAppears
	msgbox FrontierStory_Text_CookAsksReady, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, FrontierStory_EventScript_TargetRetreat
	call FrontierStory_EventScript_ChooseReleaseParty
	goto_if_eq VAR_RESULT, FALSE, FrontierStory_EventScript_TargetRetreat
	special FrontierStory_GetRescueScene
FrontierStory_EventScript_TargetReleaseDispatch::
	switch VAR_RESULT
	case STORY_TARGET_REIKA, DomeStory_EventScript_ReleaseBattle
	case STORY_TARGET_IKUE, PalaceStory_EventScript_ReleaseBattle
	case STORY_TARGET_TOSHIO, FactoryStory_EventScript_ReleaseBattle
	case STORY_TARGET_EISUKE, ArenaStory_EventScript_ReleaseBattle
	case STORY_TARGET_YAYOI, PikeStory_EventScript_ReleaseBattle
	case STORY_TARGET_NOBUO, PyramidStory_EventScript_ReleaseBattle
	case STORY_TARGET_YORIKO, ContestStory_EventScript_ReleaseBattle
	special LoadPlayerParty
FrontierStory_EventScript_TargetRetreat::
	msgbox FrontierStory_Text_CookRetreats, MSGBOX_DEFAULT
FrontierStory_EventScript_ClearTargetScene::
	closemessage
	fadescreen FADE_TO_BLACK
	setflag FLAG_TEMP_1D
	setflag FLAG_TEMP_1E
	special FrontierStory_GetRescueScene
	removeobject VAR_0x8005
	removeobject VAR_0x8006
	fadescreen FADE_FROM_BLACK
	releaseall
	end

@ After each person's own release battle; VAR_RESULT is the B_OUTCOME_*.
FrontierStory_EventScript_TargetBattleDone::
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, FrontierStory_EventScript_TargetReleased
	msgbox FrontierStory_Text_CookLostRelease, MSGBOX_DEFAULT
	goto FrontierStory_EventScript_ClearTargetScene

FrontierStory_EventScript_TargetReleased::
	setvar VAR_0x8004, RESCUE_TEXT_VOICES_FADE
	call FrontierStory_EventScript_ShowRescueText
	setvar VAR_0x8004, RESCUE_TEXT_CHOOSES
	call FrontierStory_EventScript_ShowRescueText
	closemessage
	playse SE_M_HAZE
	special FrontierStory_GetRescueScene
	applymovement VAR_0x8006, TowerStory_Movement_PokevirusFades
	waitmovement 0
	removeobject VAR_0x8006
	setflag FLAG_TEMP_1E
	waitse
	msgbox FrontierStory_Text_TargetCollapses, MSGBOX_DEFAULT
	msgbox FrontierStory_Text_CookCarriesToHub, MSGBOX_DEFAULT
	closemessage
	special FrontierStory_ReturnRescueTarget
	warp MAP_ULTRA_FRONTIE_HUB, 7, 5
	waitstate
	end

DomeStory_EventScript_ReleaseBattle::
	setmultitrainerbattle TRAINER_REIKA_RELEASE, DomeStory_Text_ReikaReleaseDefeat, TRAINER_POKEVIRUS, FrontierStory_Text_PokevirusDefeat, PARTNER_ROCKET_COOK
	call FrontierStory_EventScript_StartReleaseBattle
	goto FrontierStory_EventScript_TargetBattleDone

PalaceStory_EventScript_ReleaseBattle::
	setmultitrainerbattle TRAINER_IKUE_RELEASE, PalaceStory_Text_IkueReleaseDefeat, TRAINER_POKEVIRUS, FrontierStory_Text_PokevirusDefeat, PARTNER_ROCKET_COOK
	call FrontierStory_EventScript_StartReleaseBattle
	goto FrontierStory_EventScript_TargetBattleDone

FactoryStory_EventScript_ReleaseBattle::
	setmultitrainerbattle TRAINER_TOSHIO_RELEASE, FactoryStory_Text_ToshioReleaseDefeat, TRAINER_POKEVIRUS, FrontierStory_Text_PokevirusDefeat, PARTNER_ROCKET_COOK
	call FrontierStory_EventScript_StartReleaseBattle
	goto FrontierStory_EventScript_TargetBattleDone

ArenaStory_EventScript_ReleaseBattle::
	setmultitrainerbattle TRAINER_EISUKE_RELEASE, ArenaStory_Text_EisukeReleaseDefeat, TRAINER_POKEVIRUS, FrontierStory_Text_PokevirusDefeat, PARTNER_ROCKET_COOK
	call FrontierStory_EventScript_StartReleaseBattle
	goto FrontierStory_EventScript_TargetBattleDone

PikeStory_EventScript_ReleaseBattle::
	setmultitrainerbattle TRAINER_YAYOI_RELEASE, PikeStory_Text_YayoiReleaseDefeat, TRAINER_POKEVIRUS, FrontierStory_Text_PokevirusDefeat, PARTNER_ROCKET_COOK
	call FrontierStory_EventScript_StartReleaseBattle
	goto FrontierStory_EventScript_TargetBattleDone

PyramidStory_EventScript_ReleaseBattle::
	setmultitrainerbattle TRAINER_NOBUO_RELEASE, PyramidStory_Text_NobuoReleaseDefeat, TRAINER_POKEVIRUS, FrontierStory_Text_PokevirusDefeat, PARTNER_ROCKET_COOK
	call FrontierStory_EventScript_StartReleaseBattle
	goto FrontierStory_EventScript_TargetBattleDone

ContestStory_EventScript_ReleaseBattle::
	setmultitrainerbattle TRAINER_YORIKO_RELEASE, ContestStory_Text_YorikoReleaseDefeat, TRAINER_POKEVIRUS, FrontierStory_Text_PokevirusDefeat, PARTNER_ROCKET_COOK
	call FrontierStory_EventScript_StartReleaseBattle
	goto FrontierStory_EventScript_TargetBattleDone

@ In the hub straight after their release battle (VAR_TEMP_5 0 -> 1 -> 0), at the table on the
@ left, shown as themselves (VAR_HUB_RESCUED_GFX).
FrontierStory_EventScript_SetupHubRescue::
	special FrontierStory_PrepareHubRescue
	clearflag FLAG_TEMP_13
	clearflag FLAG_TEMP_10
	setobjectxyperm LOCALID_FRONTIER_HUB_INTRO_COOK, 4, 6
	setobjectmovementtype LOCALID_FRONTIER_HUB_INTRO_COOK, MOVEMENT_TYPE_FACE_UP
	setvar VAR_TEMP_5, 1
	return

FrontierStory_EventScript_HubRescue::
	lockall
	setvar VAR_TEMP_5, 0
	special FrontierStory_PrepareHubRescue
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	setvar VAR_0x8004, RESCUE_TEXT_DELIRIOUS
	call FrontierStory_EventScript_ShowRescueText
	msgbox FrontierStory_Text_CookForcesMeal, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_HUB_INTRO_COOK, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	setvar VAR_0x8004, RESCUE_TEXT_EATS
	call FrontierStory_EventScript_ShowRescueText
	goto_if_set FLAG_STORY_CURE_FOUND, FrontierStory_EventScript_HubRescueKnownCure
	call FrontierStory_EventScript_ServeFirstDessert
	setvar VAR_0x8004, RESCUE_TEXT_WAKES_UP
	call FrontierStory_EventScript_ShowRescueText
	call FrontierStory_EventScript_CureFound
	goto FrontierStory_EventScript_HubRescueCured

FrontierStory_EventScript_HubRescueKnownCure::
	call FrontierStory_EventScript_GiveKnownCure
	setvar VAR_0x8004, RESCUE_TEXT_WAKES_UP
	call FrontierStory_EventScript_ShowRescueText
FrontierStory_EventScript_HubRescueCured::
	setvar VAR_0x8004, RESCUE_TEXT_REMEMBERS
	call FrontierStory_EventScript_ShowRescueText
	setvar VAR_0x8004, RESCUE_TEXT_AFTER_CURE
	call FrontierStory_EventScript_ShowRescueText
	setvar VAR_0x8004, RESCUE_TEXT_ASKS_POKENAV
	call FrontierStory_EventScript_ShowRescueText
	setflag FLAG_ADDED_MATCH_CALL_TO_POKENAV
	@ register_matchcall, for whoever this is.
	special FrontierStory_GetRescueMatchCall
	special SetMatchCallRegisteredFlag
	callstd STD_REGISTER_MATCH_CALL
	call FrontierStory_EventScript_OpenInnIfNeeded
	setvar VAR_0x8004, RESCUE_TEXT_GOES_TO_INN
	call FrontierStory_EventScript_ShowRescueText
	msgbox FrontierStory_Text_CookSendsToInn, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	setflag FLAG_TEMP_13
	removeobject LOCALID_HUB_RESCUED
	setflag FLAG_TEMP_10
	removeobject LOCALID_FRONTIER_HUB_INTRO_COOK
	special FrontierStory_FinishRescue
	fadescreen FADE_FROM_BLACK
	releaseall
	end

@ ---------------------------------------------------------------------------
@ The camp: visitors who remembered themselves come for the kibidango

FrontierStory_EventScript_SetupCamp::
	setflag FLAG_TEMP_18
	special FrontierStory_PickCampVisitor
	goto_if_eq VAR_RESULT, FALSE, FrontierStory_EventScript_Return
	clearflag FLAG_TEMP_18
	return

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
FrontierStory_EventScript_CampVisitorFadeOut::
	fadescreenswapbuffers FADE_TO_BLACK
	setflag FLAG_TEMP_18
	removeobject LOCALID_PARTY_ROAMER_VISITOR
	setvar VAR_CAMP_VISITOR, NAMED_STORY_NONE
FrontierStory_EventScript_CampVisitorFadeIn::
	fadescreenswapbuffers FADE_FROM_BLACK
	call FrontierStory_EventScript_TrySuggestInn
	release
	end

FrontierStory_EventScript_VisitorNotYet::
	msgbox FrontierStory_Text_VisitorWillWait, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ Passers-by around the Frontier: each talk, a line at random for how far the rescues have come,
@ in a voice for how they look.

FrontierStory_EventScript_CrowdTalk::
	special FrontierStory_BufferCrowdTalk
	msgbox gStringVar4, MSGBOX_NPC
	end

@ The facility attendants, after their usual welcome: once a visit, a word for how far the rescues
@ have come.
FrontierStory_EventScript_StaffAside::
	goto_if_set FLAG_STAFF_ASIDE_SHOWN, FrontierStory_EventScript_StaffAsideDone
	special FrontierStory_BufferStaffAside
	goto_if_eq VAR_RESULT, FALSE, FrontierStory_EventScript_StaffAsideDone
	setflag FLAG_STAFF_ASIDE_SHOWN
	msgbox gStringVar4, MSGBOX_DEFAULT
FrontierStory_EventScript_StaffAsideDone:
	return

@ VAR_0x8004: REGULAR_*. Their own line for before the rescues, while they go on, or after the
@ ending.
FrontierStory_EventScript_RegularTalk::
	special FrontierStory_BufferRegularTalk
	msgbox gStringVar4, MSGBOX_NPC
	end

@ The same, for someone who keeps facing whoever they are with.
FrontierStory_EventScript_RegularAside::
	lock
	special FrontierStory_BufferRegularTalk
	msgbox gStringVar4, MSGBOX_DEFAULT
	release
	end

@ ---------------------------------------------------------------------------
@ The inn: everyone cured rests here. Kazuki, Toshio with his Aggron, and Yayoi with Roserade at a
@ low table, in the lobby; Ikue and Delcatty in the east wing; Yoriko and Seviper, Nobuo and
@ Sandslash in the west wing;
@ Eisuke and Starmie on the beach outside and Reika walking Linoone there; and in each part of the
@ building a few of the cured Brains and guests, picked on every visit.

InnStory_EventScript_SetupLobby::
	setflag FLAG_TEMP_11
	setflag FLAG_TEMP_12
	setflag FLAG_TEMP_15
	call_if_set FLAG_STORY_RESCUED_TOWER, InnStory_EventScript_ShowKazuki
	call_if_set FLAG_STORY_RESCUED_FACTORY, InnStory_EventScript_ShowToshio
	call_if_set FLAG_STORY_RESCUED_PIKE, InnStory_EventScript_ShowYayoi
	setvar VAR_0x8004, INN_AREA_LOBBY
	special FrontierStory_PickInnGuests
	return

InnStory_EventScript_ShowKazuki::
	clearflag FLAG_TEMP_11
	return

InnStory_EventScript_ShowToshio::
	clearflag FLAG_TEMP_12
	return

@ He has returned Gen-san, the Bastiodon he rented, and tells his own Aggron about their battles.
InnStory_EventScript_Toshio::
	lock
	faceplayer
	msgbox FactoryStory_Text_InnToshio, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_INN_TOSHIO, Common_Movement_FaceRight
	waitmovement 0
	msgbox FactoryStory_Text_InnToshioTellsAggron, MSGBOX_DEFAULT
	closemessage
	playmoncry SPECIES_AGGRON, CRY_MODE_NORMAL
	applymovement LOCALID_INN_AGGRON, InnStory_Movement_Nod
	waitmovement 0
	waitmoncry
	msgbox FactoryStory_Text_InnAggronNods, MSGBOX_DEFAULT
	release
	end

InnStory_EventScript_Aggron::
	lock
	faceplayer
	playmoncry SPECIES_AGGRON, CRY_MODE_NORMAL
	msgbox FactoryStory_Text_InnAggron, MSGBOX_DEFAULT
	waitmoncry
	release
	end

InnStory_EventScript_ShowYayoi::
	clearflag FLAG_TEMP_15
	return

@ Yayoi pours tea at a low table. The player sits on the cushion across from her, and each of them
@ drinks a tea of the scent they like.
InnStory_EventScript_Yayoi::
	lock
	faceplayer
	msgbox PikeStory_Text_InnYayoiInvites, MSGBOX_DEFAULT
	closemessage
	specialvar VAR_RESULT, GetPlayerFacingDirection
	call_if_eq VAR_RESULT, DIR_EAST, InnStory_EventScript_SitFromYayoisSide
	call_if_eq VAR_RESULT, DIR_SOUTH, InnStory_EventScript_SitFromBehindYayoi
	call InnStory_EventScript_YayoiTea
	release
	end

InnStory_EventScript_SitFromYayoisSide:
	applymovement LOCALID_PLAYER, InnStory_Movement_FromYayoisSide
	waitmovement 0
	return

InnStory_EventScript_SitFromBehindYayoi:
	applymovement LOCALID_PLAYER, InnStory_Movement_FromBehindYayoi
	waitmovement 0
	return

@ The table between them, from the cushion across from her.
InnStory_EventScript_YayoiTable::
	goto_if_set FLAG_STORY_RESCUED_PIKE, InnStory_EventScript_YayoiTableTea
	end

InnStory_EventScript_YayoiTableTea:
	lockall
	call InnStory_EventScript_YayoiTea
	releaseall
	end

InnStory_EventScript_YayoiTea:
	turnobject LOCALID_INN_YAYOI, DIR_SOUTH
	message PikeStory_Text_InnYayoiAsksTea
	waitmessage
	multichoice 0, 0, MULTI_INN_TEA, TRUE
	switch VAR_RESULT
	case 0, InnStory_EventScript_TeaMint
	case 1, InnStory_EventScript_TeaLemon
	case 2, InnStory_EventScript_TeaChamomile
	msgbox PikeStory_Text_InnTeaJasmine, MSGBOX_DEFAULT
	goto InnStory_EventScript_YayoiPoursTea

InnStory_EventScript_TeaMint:
	msgbox PikeStory_Text_InnTeaMint, MSGBOX_DEFAULT
	goto InnStory_EventScript_YayoiPoursTea

InnStory_EventScript_TeaLemon:
	msgbox PikeStory_Text_InnTeaLemon, MSGBOX_DEFAULT
	goto InnStory_EventScript_YayoiPoursTea

InnStory_EventScript_TeaChamomile:
	msgbox PikeStory_Text_InnTeaChamomile, MSGBOX_DEFAULT
InnStory_EventScript_YayoiPoursTea:
	msgbox PikeStory_Text_InnYayoiTea, MSGBOX_DEFAULT
	closemessage
	playmoncry SPECIES_ROSERADE, CRY_MODE_NORMAL
	applymovement LOCALID_INN_ROSERADE, InnStory_Movement_Hop
	waitmovement 0
	waitmoncry
	msgbox PikeStory_Text_InnRoseradeHops, MSGBOX_DEFAULT
	return

InnStory_EventScript_Roserade::
	lock
	faceplayer
	playmoncry SPECIES_ROSERADE, CRY_MODE_NORMAL
	msgbox PikeStory_Text_InnRoserade, MSGBOX_DEFAULT
	waitmoncry
	release
	end

InnStory_EventScript_SetupEastWing::
	setflag FLAG_TEMP_11
	call_if_set FLAG_STORY_RESCUED_PALACE, InnStory_EventScript_ShowIkue
	setvar VAR_0x8004, INN_AREA_EAST_WING
	special FrontierStory_PickInnGuests
	return

InnStory_EventScript_ShowIkue::
	clearflag FLAG_TEMP_11
	return

@ Delcatty picks the game today: chasing its own tail. Ikue doesn't choose for it; she joins in.
InnStory_EventScript_Ikue::
	lock
	faceplayer
	msgbox PalaceStory_Text_InnIkue, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_INN_DELCATTY, InnStory_Movement_ChaseTail
	waitmovement 0
	playmoncry SPECIES_DELCATTY, CRY_MODE_NORMAL
	applymovement LOCALID_INN_IKUE, InnStory_Movement_ChaseTail
	waitmovement 0
	waitmoncry
	faceplayer
	msgbox PalaceStory_Text_InnIkueJoins, MSGBOX_DEFAULT
	release
	end

InnStory_EventScript_Delcatty::
	lock
	faceplayer
	playmoncry SPECIES_DELCATTY, CRY_MODE_NORMAL
	msgbox PalaceStory_Text_InnDelcatty, MSGBOX_DEFAULT
	waitmoncry
	release
	end

InnStory_EventScript_SetupWestWing::
	setflag FLAG_TEMP_11
	setflag FLAG_TEMP_12
	call_if_set FLAG_STORY_RESCUED_PYRAMID, InnStory_EventScript_ShowNobuo
	call_if_set FLAG_STORY_RESCUED_CONTEST, InnStory_EventScript_ShowYoriko
	setvar VAR_0x8004, INN_AREA_WEST_WING
	special FrontierStory_PickInnGuests
	return

InnStory_EventScript_ShowNobuo::
	clearflag FLAG_TEMP_11
	return

@ Nobuo asks for directions aloud. Neither answer moves the player or blocks the corridor.
InnStory_EventScript_Nobuo::
	lock
	faceplayer
	msgbox PyramidStory_Text_InnNobuoAsks, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, InnStory_EventScript_NobuoLater
	msgbox PyramidStory_Text_InnNobuoDirections, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_INN_NOBUO, Common_Movement_FaceRight
	waitmovement 0
	playmoncry SPECIES_SANDSLASH, CRY_MODE_NORMAL
	applymovement LOCALID_INN_SANDSLASH, InnStory_Movement_Nod
	waitmovement 0
	waitmoncry
	faceplayer
	msgbox PyramidStory_Text_InnNobuoThanks, MSGBOX_DEFAULT
	release
	end

InnStory_EventScript_NobuoLater::
	msgbox PyramidStory_Text_InnNobuoLater, MSGBOX_DEFAULT
	release
	end

InnStory_EventScript_Sandslash::
	lock
	faceplayer
	playmoncry SPECIES_SANDSLASH, CRY_MODE_NORMAL
	msgbox PyramidStory_Text_InnSandslash, MSGBOX_DEFAULT
	waitmoncry
	release
	end

InnStory_EventScript_ShowYoriko::
	clearflag FLAG_TEMP_12
	return

InnStory_EventScript_Guest0::
	setvar VAR_0x8004, 0
	goto InnStory_EventScript_Guest

InnStory_EventScript_Guest1::
	setvar VAR_0x8004, 1
InnStory_EventScript_Guest::
	lock
	faceplayer
	special FrontierStory_BufferInnGuestText
	msgbox gStringVar4, MSGBOX_DEFAULT
	release
	end

InnStory_EventScript_Kazuki::
	lock
	faceplayer
	msgbox TowerStory_Text_InnKazukiChallenge, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, InnStory_EventScript_KazukiDecline
	msgbox TowerStory_Text_InnKazukiHere, MSGBOX_DEFAULT
	closemessage
	special SavePlayerParty
	special FrontierStory_PrepareMatchedBattle
	setflag FLAG_EXPANSION_NO_WHITEOUT
	trainerbattle_no_intro TRAINER_KAZUKI_RESORT, TowerStory_Text_InnKazukiDefeat
	special FrontierStory_EndStoryBattle
	clearflag FLAG_EXPANSION_NO_WHITEOUT
	special LoadPlayerParty
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, InnStory_EventScript_KazukiLost
	msgbox TowerStory_Text_InnKazukiWon, MSGBOX_DEFAULT
	release
	end

InnStory_EventScript_KazukiLost::
	msgbox TowerStory_Text_InnKazukiAfterLoss, MSGBOX_DEFAULT
	release
	end

InnStory_EventScript_KazukiDecline::
	msgbox TowerStory_Text_InnKazukiLater, MSGBOX_DEFAULT
	release
	end

@ Yoriko and Seviper show the player a small performance and ask what they liked best.
InnStory_EventScript_Yoriko::
	lock
	faceplayer
	msgbox ContestStory_Text_InnYorikoWelcome, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_INN_SEVIPER, InnStory_Movement_SeviperPerforms
	waitmovement 0
	playmoncry SPECIES_SEVIPER, CRY_MODE_NORMAL
	waitmoncry
	message ContestStory_Text_InnYorikoAsks
	waitmessage
	dynmultipush ContestStory_Text_InnChoiceGlare, 0
	dynmultipush ContestStory_Text_InnChoiceTail, 1
	dynmultipush ContestStory_Text_InnChoiceScales, 2
	dynmultipush ContestStory_Text_InnChoiceTogether, 3
	dynmultistack 0, 0, TRUE, 4, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case 0, InnStory_EventScript_YorikoGlare
	case 1, InnStory_EventScript_YorikoTail
	case 2, InnStory_EventScript_YorikoScales
	msgbox ContestStory_Text_InnYorikoTogether, MSGBOX_DEFAULT
	goto InnStory_EventScript_YorikoNext

InnStory_EventScript_YorikoGlare::
	msgbox ContestStory_Text_InnYorikoGlare, MSGBOX_DEFAULT
	goto InnStory_EventScript_YorikoNext

InnStory_EventScript_YorikoTail::
	msgbox ContestStory_Text_InnYorikoTail, MSGBOX_DEFAULT
	goto InnStory_EventScript_YorikoNext

InnStory_EventScript_YorikoScales::
	msgbox ContestStory_Text_InnYorikoScales, MSGBOX_DEFAULT
InnStory_EventScript_YorikoNext::
	msgbox ContestStory_Text_InnYorikoOthers, MSGBOX_DEFAULT
	release
	end

InnStory_EventScript_Seviper::
	lock
	faceplayer
	playmoncry SPECIES_SEVIPER, CRY_MODE_NORMAL
	msgbox ContestStory_Text_InnSeviper, MSGBOX_DEFAULT
	waitmoncry
	release
	end

@ Outside: Eisuke and Starmie on the beach, Reika walking Linoone.
InnStory_EventScript_SetupOutside::
	setflag FLAG_TEMP_11
	setflag FLAG_TEMP_12
	call_if_set FLAG_STORY_RESCUED_ARENA, InnStory_EventScript_ShowEisuke
	call_if_set FLAG_STORY_RESCUED_DOME, InnStory_EventScript_ShowReika
	call FinalStory_EventScript_SetupBeach
	return

InnStory_EventScript_ShowEisuke::
	clearflag FLAG_TEMP_11
	return

InnStory_EventScript_ShowReika::
	clearflag FLAG_TEMP_12
	return

@ Linoone trots off after something; Reika doesn't need to know what, and waits with a smile.
InnStory_EventScript_Reika::
	lock
	faceplayer
	msgbox DomeStory_Text_InnReika, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_INN_LINOONE, InnStory_Movement_LinooneWanders
	waitmovement 0
	applymovement LOCALID_INN_REIKA, Common_Movement_FaceRight
	waitmovement 0
	msgbox DomeStory_Text_InnReikaWaits, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_INN_LINOONE, InnStory_Movement_LinooneReturns
	waitmovement 0
	playmoncry SPECIES_LINOONE, CRY_MODE_NORMAL
	faceplayer
	msgbox DomeStory_Text_InnReikaWelcomesBack, MSGBOX_DEFAULT
	waitmoncry
	release
	end

InnStory_EventScript_Linoone::
	lock
	faceplayer
	playmoncry SPECIES_LINOONE, CRY_MODE_NORMAL
	msgbox DomeStory_Text_InnLinoone, MSGBOX_DEFAULT
	waitmoncry
	release
	end

@ He has put the lost three turns behind him: instead of fighting that match again, he and Starmie
@ dive in and swim off to train for the next race. They are back on the next visit.
InnStory_EventScript_Eisuke::
	lock
	faceplayer
	msgbox ArenaStory_Text_InnEisuke, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_INN_EISUKE, InnStory_Movement_DiveIn
	applymovement LOCALID_INN_STARMIE, InnStory_Movement_DiveIn
	waitmovement LOCALID_INN_EISUKE
	waitmovement LOCALID_INN_STARMIE
	playse SE_M_SURF
	removeobject LOCALID_INN_EISUKE
	removeobject LOCALID_INN_STARMIE
	setflag FLAG_TEMP_11
	waitse
	msgbox ArenaStory_Text_InnEisukeSwimsOff, MSGBOX_DEFAULT
	release
	end

InnStory_EventScript_Starmie::
	lock
	faceplayer
	playmoncry SPECIES_STARMIE, CRY_MODE_NORMAL
	msgbox ArenaStory_Text_InnStarmie, MSGBOX_DEFAULT
	waitmoncry
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

InnStory_Movement_Nod:
	jump_in_place_left
	step_end

InnStory_Movement_Hop:
	jump_in_place_down
	step_end

@ To the cushion across the low table from Yayoi, from beside her or from behind her.
InnStory_Movement_FromYayoisSide:
	walk_down
	walk_down
	walk_right
	face_up
	step_end

InnStory_Movement_FromBehindYayoi:
	walk_left
	walk_down
	walk_down
	walk_down
	walk_right
	face_up
	step_end

InnStory_Movement_ChaseTail:
	face_left
	delay_4
	face_up
	delay_4
	face_right
	delay_4
	face_down
	delay_4
	face_left
	delay_4
	face_up
	delay_4
	face_right
	delay_4
	face_down
	delay_8
	jump_in_place_down
	step_end

InnStory_Movement_LinooneWanders:
	walk_right
	walk_right
	walk_right
	face_up
	delay_16
	face_right
	delay_16
	face_down
	delay_16
	step_end

InnStory_Movement_LinooneReturns:
	walk_left
	walk_left
	walk_left
	face_down
	step_end

InnStory_Movement_DiveIn:
	face_down
	delay_16
	jump_down
	set_invisible
	step_end

InnStory_Movement_SeviperPerforms:
	face_left
	delay_8
	face_up
	delay_8
	face_right
	delay_8
	face_down
	delay_8
	jump_in_place_down
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
	.string "{JPN}？？？『……聞こえるか。\p"
	.string "シンボルを得た トレーナー。\n"
	.string "あなたに 頼みたいことがある。\p"
	.string "ハブで 待っている。』$"

FrontierStory_Text_DarkraiWarnsAlone:
	.string "{JPN}？？？『……いけない。\n"
	.string "一人で 近づいては駄目だ。\p"
	.string "まずは ハブへ来てほしい。』$"

FrontierStory_Text_DarkraiCallsInHub:
	.string "{JPN}？？？『……来てくれたか。\n"
	.string "私は ここだ。』$"

FrontierStory_Text_CookHeardVoice:
	.string "{JPN}店主『{PLAYER}！\n"
	.string "今の 声…… 聞こえたか！？\p"
	.string "頭の中に 直接 \n"
	.string "響いてきやがった……！』$"

FrontierStory_Text_DarkraiIntroduces:
	.string "{JPN}？？？『驚かせて すまない。\p"
	.string "私は ダークライ。\n"
	.string "あなたの心に 直接語りかけている。\p"
	.string "そこで 眠っている モモワロウを\n"
	.string "ずっと そばで 見守ってきた。』$"

FrontierStory_Text_CookAboutPecharunt:
	.string "{JPN}店主『モモワロウ……\n"
	.string "ハブの隅で 眠ってるあいつか。\p"
	.string "何をしても 目を覚まさねえから \n"
	.string "そっとしておいたんだが……。』$"

FrontierStory_Text_DarkraiAboutPecharunt:
	.string "{JPN}ダークライ『あの子は 遠い世界から\n"
	.string "一人で 迷い込んできた。\p"
	.string "私は しばらく あの子と\n"
	.string "共に暮らしていた。\p"
	.string "……そのあの子に ポケルスが感染した。\n"
	.string "そして 形を変えて 広がったのだ。\p"
	.string "人と ポケモンの心を \n"
	.string "混ぜ合わせるものとして。』$"

FrontierStory_Text_DarkraiWantsToHelp:
	.string "{JPN}ダークライ『{PLAYER}。\n"
	.string "あなたが シンボルを得た時 \l"
	.string "繋がりの中で 誰かの心が\l"
	.string "揺れるのを 感じた。\p"
	.string "私も 力を貸したい。\n"
	.string "……あの子を 助けたいのだ。』$"

FrontierStory_Text_CookWelcomesDarkrai:
	.string "{JPN}店主『……ジガルデの やつも\n"
	.string "黙って 頷いてやがる。\p"
	.string "分かった。 ダークライ。\n"
	.string "おまえも おれたちの 仲間だ。』$"

FrontierStory_Text_DarkraiFavoriteFood:
	.string "{JPN}ダークライ『……ひとつ 思い出した。\p"
	.string "モモワロウは よく話していた。\n"
	.string "四角くて 甘い団子が好きだと。\p"
	.string "昔 あの子を 拾ってくれた家で\n"
	.string "食べていたものだそうだ。\p"
	.string "……きびだんご と 呼んでいた。』$"

FrontierStory_Text_CookKnowsRecipe:
	.string "{JPN}店主『四角い きびだんご……？\p"
	.string "そりゃ うちに昔から\n"
	.string "伝わってるのと 同じだ。\p"
	.string "……そういや ばあちゃんが言ってたな。\n"
	.string "ずっと昔 ご先祖様が\l"
	.string "不思議なポケモンを 預かってたって。\p"
	.string "……ま 昔話だ。\n"
	.string "だが レシピなら 覚えてる。\p"
	.string "作っておいてやる。あいつが\n"
	.string "目を覚ましたら 食わせてやろうぜ。』$"

FrontierStory_Text_DarkraiThanks:
	.string "{JPN}ダークライ『……ありがとう。』$"

FrontierStory_Text_CookExplainsReleaseBattles:
	.string "{JPN}店主『{PLAYER}。\n"
	.string "金色のシンボルを 取った施設で \l"
	.string "様子のおかしいやつが いたら\l"
	.string "そいつに 話しかけてみろ。\p"
	.string "何かあったら おれとダークライが\n"
	.string "すぐに 駆けつける。\p"
	.string "その時は 二人で組んで\n"
	.string "戦うぞ！』$"

FrontierStory_Text_PecharuntSleeping:
	.string "{JPN}モモワロウが 眠っている。\p"
	.string "ゆすっても 呼びかけても\n"
	.string "まったく 反応がない……。$"

FrontierStory_Text_PecharuntStillSleeping:
	.string "{JPN}モモワロウが 静かに 眠っている。\p"
	.string "顔色は よくなったが\n"
	.string "目を覚ます気配は ない……。$"

FrontierStory_Text_DarkraiWatching:
	.string "{JPN}ダークライ『私は ここで\n"
	.string "あの子を 見守っている。\p"
	.string "なにか あれば あなたの 心に\n"
	.string "呼びかけよう。』$"

FrontierStory_Text_DarkraiSearching:
	.string "{JPN}ダークライ『身体は 治った。\n"
	.string "だが 心が 戻ってこない。\p"
	.string "あの子は きっと まだどこかで\n"
	.string "夢を見ている……。』$"

FrontierStory_Text_CookArrives:
	.string "{JPN}店主『{PLAYER}！ 間に合ったか！』$"

FrontierStory_Text_CookSeesPokevirus:
	.string "{JPN}店主『……なんだ ありゃあ……！？』$"

FrontierStory_Text_DarkraiNamesPokevirus:
	.string "{JPN}ダークライ『……あれが ヒトポケルスだ。\p"
	.string "皆の心を 繋いでいるものが \n"
	.string "初めて 形を持って現れた。\p"
	.string "離れようとする心を\n"
	.string "引き止めようと しているのだ。』$"

FrontierStory_Text_CookResolves:
	.string "{JPN}店主『ジガルデが 言ってたやつか……！\p"
	.string "悪気があるか どうかなんざ\n"
	.string "知らねえが……。\p"
	.string "離れたいってやつを\n"
	.string "引き止める筋合いは ねえ！』$"

FrontierStory_Text_CookPokevirusAgain:
	.string "{JPN}店主『また でやがったな……！\n"
	.string "{PLAYER}！ いくぞ！』$"

FrontierStory_Text_CookAsksReady:
	.string "{JPN}店主『おれと組んで 戦うぞ。\n"
	.string "ポケモンを 3匹選んでくれ。\p"
	.string "準備は いいか？』$"

FrontierStory_Text_CookRetreats:
	.string "{JPN}店主『……一旦 下がるぞ。\p"
	.string "準備が できたら\n"
	.string "もう一度 話しかけてやれ。』$"

FrontierStory_Text_CookLostRelease:
	.string "{JPN}店主『くっ……！ 立て直すぞ！\p"
	.string "何度でも いい。\n"
	.string "必ず 引き剥がしてやる！』$"

FrontierStory_Text_PokevirusDefeat:
	.string "{JPN}…… ……………… ……$"

FrontierStory_Text_CookForcesMeal:
	.string "{JPN}店主『ほら 食え！\n"
	.string "食わなきゃ もたねえぞ！』$"

FrontierStory_Text_CookServesDessert:
	.string "{JPN}店主『……よし 全部 食ったな。\n"
	.string "仕上げに デザートだ。\p"
	.string "ダークライの 話で\n"
	.string "作っておいた きびだんごだ。\p"
	.string "甘いもんは 心に効くぜ。』\p"
	.string "店主は 四角い きびだんごを\n"
	.string "ひとつ 食べさせた。$"

FrontierStory_Text_CookWhatHappened:
	.string "{JPN}店主『……おい。\n"
	.string "今 何が起きた？』$"

FrontierStory_Text_DarkraiSensesCure:
	.string "{JPN}ダークライ『……分かる。\n"
	.string "その子から ウイルスの気配が\l"
	.string "消えた。\p"
	.string "きびだんごが…… 治したのだ。』$"

FrontierStory_Text_CookAmazed:
	.string "{JPN}店主『まじかよ……！\n"
	.string "うちの きびだんごが 薬に？\p"
	.string "……だったら モモワロウにも\n"
	.string "食わせてみる！』$"

FrontierStory_Text_FeedPecharunt:
	.string "{JPN}店主は 眠っている モモワロウの\n"
	.string "口元へ きびだんごを運んだ。\p"
	.string "モモワロウは 目を閉じたまま \n"
	.string "小さく 口を動かした……。$"

FrontierStory_Text_DarkraiPecharuntStillAsleep:
	.string "{JPN}ダークライ『……モモワロウの身体からも\n"
	.string "ウイルスが 消えた。\p"
	.string "……だが 目を覚まさない。\n"
	.string "心が まだ 戻っていないようだ。』$"

FrontierStory_Text_CookWillSearch:
	.string "{JPN}店主『治ったのに 起きねえ……？\p"
	.string "……理由を探すしか ねえな。\n"
	.string "何か気づいたら 教えてくれ。』$"

FrontierStory_Text_CookHandsKibidango:
	.string "{JPN}店主『きびだんごは おれが\n"
	.string "作り置きしておく。\p"
	.string "{PLAYER}。 これからは おまえも\n"
	.string "持っていけ。\p"
	.string "負けて 自分を 思い出した やつが\n"
	.string "いたら 渡してやってくれ。\p"
	.string "そういう やつは きっと\n"
	.string "リゾートに 訪ねてくるだろうよ。』$"

FrontierStory_Text_CookKnownCure:
	.string "{JPN}店主『仕上げは こいつだ。』\p"
	.string "店主は きびだんごを\n"
	.string "ひとつ 食べさせた。$"

FrontierStory_Text_CookSendsToInn:
	.string "{JPN}店主『旅館なら\n"
	.string "飯も 風呂も ある。 ゆっくり 休めよ。』$"

FrontierStory_Text_CookCarriesToHub:
	.string "{JPN}店主『おい しっかりしろ！\n"
	.string "……息はある。\p"
	.string "……だが うわ言を 言ってやがる。\n"
	.string "まだ 治っちゃ いねえんだ。\p"
	.string "ハブに 運ぶぞ！\n"
	.string "飯を 食わせりゃ 力も 戻る！』$"

FrontierStory_Text_OfferKibidango:
	.string "{JPN}{STR_VAR_1}に\n"
	.string "おもいでのきびだんごを 渡しますか？$"

FrontierStory_Text_VisitorEats:
	.string "{JPN}{PLAYER}は {STR_VAR_1}に\n"
	.string "きびだんごを 渡した。\p"
	.string "{STR_VAR_1}は 静かに\n"
	.string "きびだんごを 食べた……。$"

FrontierStory_Text_VisitorWillWait:
	.string "{JPN}……そうか。\n"
	.string "私も まだ 迷っているのかも。\p"
	.string "また 来るよ。$"

FrontierStory_Text_DarkraiReachesOut:
	.string "{JPN}ダークライ『……{PLAYER}。\n"
	.string "聞こえるか。』$"

FrontierStory_Text_DarkraiSuggestsInn:
	.string "{JPN}ダークライ『治った者には\n"
	.string "心を休める場所が 必要だ。\p"
	.string "海の向こうに 古い旅館がある。\n"
	.string "今は 誰も使っていない。\p"
	.string "広い部屋が いくつもある。\n"
	.string "これから 戻ってくる者達も\l"
	.string "そこで休めるだろう。\p"
	.string "港の船乗りには \n"
	.string "私から 声を届けておこう。\p"
	.string "西の港から 渡るといい。』$"

FrontierStory_Text_InnOpened:
	.string "{JPN}海の むこうの 旅館へ\n"
	.string "船で わたれるように なった！$"

FrontierStory_Text_HarborHeardVoice:
	.string "{JPN}あっ ちょうど よかった！\p"
	.string "さっき 頭の 中に\n"
	.string "不思議な 声が ひびいたんです。\p"
	.string "海の むこうの 旅館まで\n"
	.string "船を だしてほしいって……。\p"
	.string "なんだか ほうっておけなくて\n"
	.string "船を だせるように しておきました！$"

FrontierStory_Text_HarborToInn:
	.string "{JPN}海の むこうの 旅館へ\n"
	.string "むかいますか？$"

FrontierStory_Text_HarborDeparting:
	.string "{JPN}では 出航 します！\n"
	.string "どうぞ おのりください！$"

FrontierStory_Text_HarborLater:
	.string "{JPN}また いつでも どうぞ！$"

FrontierStory_Text_InnSailorAsks:
	.string "{JPN}フロンティアへ 戻るかい？$"

FrontierStory_Text_InnSailorDeparting:
	.string "{JPN}よし 出航だ！\n"
	.string "のってくれ！$"

FrontierStory_Text_InnSailorStay:
	.string "{JPN}そうかい。\n"
	.string "戻るときは 声を かけてくれ。$"

@ ---------------------------------------------------------------------------
@ Text: Kazuki

TowerStory_Text_KazukiMistakesPlayer:
	.string "{JPN}カズキ『あっ！ きみ……！\n"
	.string "やっと あえた！\p"
	.string "覚えてる？ ぼく カズキ！\n"
	.string "102ばんどうろで しょうぶした！\p"
	.string "あのとき ぼく 負けちゃったけど\n"
	.string "また 勝負 しようねって……』$"

TowerStory_Text_KazukiNoticesDifference:
	.string "{JPN}カズキ『……あれ？\p"
	.string "きみの ポケモン\n"
	.string "みたことない こばっかり。\p"
	.string "それに なんだか 初めて\n"
	.string "あったみたいな 顔 してる……。』$"

TowerStory_Text_PlayerExplains:
	.string "{JPN}{PLAYER}は べつの 世界から\n"
	.string "ここへ きたことを 話した。$"

TowerStory_Text_KazukiUnderstands:
	.string "{JPN}カズキ『べつの 世界の……\n"
	.string "そっくりさん？\p"
	.string "そっか！ だから そっくりなんだ！\n"
	.string "びっくりしたあ！\p"
	.string "でも いいんだ。\n"
	.string "ここでは みんな 一緒 だから。\p"
	.string "誰が 勝っても みんなの 勝ち。\n"
	.string "きみが タワーで 勝ったら\l"
	.string "ぼくたちも 嬉しいよ！』$"

TowerStory_Text_KazukiHappyTogether:
	.string "{JPN}カズキ『ここでは みんな 一緒。\n"
	.string "勝っても 負けても みんなの 気持ち。\p"
	.string "だから ぼく ぜんぜん\n"
	.string "悔しくないんだ！ えへへ。』$"

TowerStory_Text_KazukiSawSilverBattle:
	.string "{JPN}カズキ『見てたよ！ シンボルを\n"
	.string "かけた 勝負！\p"
	.string "すごかったなあ……\n"
	.string "みんなも 喜んでた。\p"
	.string "……でも なんでだろう。\n"
	.string "胸の 奥が ざわざわするんだ。\p"
	.string "ぼく あの 人に……\n"
	.string "この 世界の きみに\l"
	.string "もういちど 挑みたい。\p"
	.string "みんなで かてれば いいはずなのに\n"
	.string "ぼくが 挑みたいって\l"
	.string "おもっちゃうんだ。\p"
	.string "……へんだよね。 えへへ。』$"

TowerStory_Text_KazukiWantsRematch:
	.string "{JPN}カズキ『あの 人 いまごろ\n"
	.string "どうしてるかな。\p"
	.string "ぼく 負けた ままじゃ いやなんだ。\n"
	.string "……みんなは そう おもわないのにね。』$"

TowerStory_Text_KazukiFeverStarting:
	.string "{JPN}カズキ『……あれ？\n"
	.string "なんだか 頭が ぼーっと する……\p"
	.string "金色の シンボル……\n"
	.string "みんなの…… ぼくの……』$"

TowerStory_Text_KazukiFever:
	.string "{JPN}カズキ『……あ……\n"
	.string "金色の シンボル……\p"
	.string "みんなで かてば みんなで 嬉しい。\n"
	.string "みんなで 負けても みんなで 平気。\p"
	.string "でも ぼくは…… ぼくが…… 挑み……\p"
	.string "違う。 ぼくたちは。 ぼく は。\n"
	.string "ぼく……\p"
	.string "熱い。 頭が 熱い。\p"
	.string "どうして。 どうして 離れるの？\n"
	.string "どうして 一人に なろうと するの？』$"

TowerStory_Text_KazukiReleaseDefeat:
	.string "{JPN}……ぼく…… 負けた……\n"
	.string "ぼくが 負けたんだ……$"

TowerStory_Text_KazukiVoicesFade:
	.string "{JPN}カズキ『……みんなの 声が\n"
	.string "遠く なっていく……\p"
	.string "やだな……\n"
	.string "みんなと 一緒だと\l"
	.string "なんにも 怖くなかったのに。』$"

TowerStory_Text_KazukiChooses:
	.string "{JPN}カズキ『でも ぼく 今 負けたんだ。\n"
	.string "ぼく 自身が。\p"
	.string "悔しい。\n"
	.string "……悔しいって 思えた。\p"
	.string "ぼく もういちど あの 人に\n"
	.string "自分の 気持ちで 挑みたい。\p"
	.string "……ごめんね みんな。\n"
	.string "ぼく いくよ。』$"

TowerStory_Text_KazukiCollapses:
	.string "{JPN}ヒトポケルスの 姿が\n"
	.string "かききえた！\p"
	.string "カズキは その ばに\n"
	.string "ぐったりと たおれこんだ！$"

TowerStory_Text_KazukiDelirious:
	.string "{JPN}カズキ『……みんな…… どこ……\n"
	.string "勝つのも…… 負けるのも……\l"
	.string "みんなの……』$"

TowerStory_Text_KazukiEats:
	.string "{JPN}店主は カズキの 口に\n"
	.string "飯を どんどん はこんだ！\p"
	.string "カズキ『もが…… もぐ……』$"

TowerStory_Text_KazukiWakesUp:
	.string "{JPN}カズキ『……あれ？\n"
	.string "ここ どこ？\p"
	.string "頭が すっきりしてる……\n"
	.string "みんなの 声が…… 聞こえない。』$"

TowerStory_Text_KazukiRemembersAll:
	.string "{JPN}カズキ『……全部 覚えてるよ。\p"
	.string "みんなと 繋がってるの\n"
	.string "すごく 安心 だったんだ。\p"
	.string "誰が 勝っても ぼくの 勝ちで\n"
	.string "誰も 悔しく なくて……\p"
	.string "でも 負けて 悔しいのも\n"
	.string "ぼくだけの ものなんだね。\p"
	.string "いつか この 世界の あの 人に\n"
	.string "もういちど 挑むよ。\l"
	.string "ぼくの 力で。\p"
	.string "……助けて くれて ありがとう！』$"

TowerStory_Text_KazukiAsksPokenav:
	.string "{JPN}カズキ『ねえ ポケナビ もってる？\n"
	.string "連絡先 こうかんしようよ！』$"

TowerStory_Text_KazukiGoesToInn:
	.string "{JPN}カズキ『ぼく 旅館で 待ってるね！\n"
	.string "今度は ぼくと しょうぶしてよ！』$"

TowerStory_Text_InnKazukiChallenge:
	.string "{JPN}カズキ『あ！ {PLAYER}！\p"
	.string "ぼくね いつか この 世界の\n"
	.string "あの 人に もういちど 挑むって\l"
	.string "決めたんだ。\p"
	.string "そのために 今は\n"
	.string "きみと 勝負 したい！\p"
	.string "勝負 してくれる？』$"

TowerStory_Text_InnKazukiHere:
	.string "{JPN}カズキ『やった！\n"
	.string "グラエナ！ いくよ！』$"

TowerStory_Text_InnKazukiDefeat:
	.string "{JPN}悔しい！ でも この 悔しさは\n"
	.string "ぼくの ものだ！$"

TowerStory_Text_InnKazukiAfterLoss:
	.string "{JPN}カズキ『また 負けちゃった！\n"
	.string "でも ちゃんと 悔しいんだ。\p"
	.string "次は もっと 強く なって\n"
	.string "挑むからね！』$"

TowerStory_Text_InnKazukiWon:
	.string "{JPN}カズキ『勝った……！ ぼくが 勝った！\p"
	.string "……あの 人にも\n"
	.string "届くかな。 えへへ。』$"

TowerStory_Text_InnKazukiLater:
	.string "{JPN}カズキ『そっか！\n"
	.string "また 今度 しようね！』$"

@ ---------------------------------------------------------------------------
@ Text: the contest's scenes about the world

ContestStory_Text_YorikoApplause:
	.string "{JPN}ヨリコ『{PLAYER}さん！\n"
	.string "2つめの 部門も 優勝ね。\p"
	.string "さっきの 拍手 聞こえた？\n"
	.string "みんな 同じ ところで\l"
	.string "同じ だけ 手を たたくの。\p"
	.string "まるで ひとつの おおきな\n"
	.string "音に つつまれる みたい。\p"
	.string "……でも あなたが 褒めてくれた\n"
	.string "しっぽの こと まだ 考えるの。』$"

ContestStory_Text_DarkraiApplause:
	.string "{JPN}ダークライ『さっきの 拍手を\n"
	.string "聞いて どう感じた。\p"
	.string "この会場は 現実の建物と\n"
	.string "心象世界が 重なっている。\p"
	.string "ここにいない者の心さえ\n"
	.string "あの拍手には 混じっていた。\p"
	.string "気持ちも 記憶も流れ込み \n"
	.string "自他の境は 曖昧になる。\p"
	.string "……だが 皆が 同じ一人になった\n"
	.string "訳ではない。ヨリコは ヨリコのままだ。\p"
	.string "笑顔の裏で 苦しんでいるのでもない。\n"
	.string "彼女は 本当に 幸福なのだ。\p"
	.string "あなたの感想が 違ったのは \n"
	.string "その繋がりの 外にいるからだ。\p"
	.string "ヨリコは 初めて 外から届いた\n"
	.string "言葉に 耳を傾けたのだ。』$"

ContestStory_Text_YorikoHandshake:
	.string "{JPN}ヨリコ『3つめの 部門も\n"
	.string "優勝ね。 おめでとう。\p"
	.string "……ねえ 握手しましょう。\n"
	.string "手を繋ぐと 気持ちが\l"
	.string "もっと 伝わる気がするの。』\p"
	.string "{PLAYER}は ヨリコと\n"
	.string "握手した。\p"
	.string "ヨリコ『……不思議。\n"
	.string "あなたの 気持ちは\l"
	.string "流れ込んで こないのね。\p"
	.string "手を繋いでいるのに。』$"

ContestStory_Text_DarkraiHandshake:
	.string "{JPN}ダークライ『……心配はいらない。\n"
	.string "あなたには 感染しない。\p"
	.string "変質したポケルスは ドクモチや\n"
	.string "水 人との接触を通して 広がった。\p"
	.string "だが あなたや 店主 あなたの両親の\n"
	.string "ように 遠い世界から来た人には\l"
	.string "感染しないのだ。\p"
	.string "……ただ ひとつだけ。\n"
	.string "ドクモチだけは 別だ。\p"
	.string "感染しない者でも あれを食べれば\n"
	.string "心を 縛られてしまう。\p"
	.string "どこかで見かけても \n"
	.string "決して 口にしてはいけない。』$"

ContestStory_Text_YorikoPartners:
	.string "{JPN}ヨリコ『4つめの 部門も 優勝！\n"
	.string "すごいわ。\p"
	.string "……ねえ ひとつ 気づいたの。\n"
	.string "あなたの ポケモン\l"
	.string "演技の 間 ときどき\l"
	.string "あなたの ほうを うかがって いたわ。\p"
	.string "まだ 少し 遠慮 してる みたい。\n"
	.string "それなのに あんなに\l"
	.string "すてきな 演技が できるのね。』$"

ContestStory_Text_DarkraiPartners:
	.string "{JPN}ダークライ『ヨリコの言う通りだ。\p"
	.string "あなたの手持ちの多くは 自らの意志で\n"
	.string "この世界へ 来たのではない。\p"
	.string "突然 連れてこられたのだ。\n"
	.string "まだ懐いていないのも 無理はない。\l"
	.string "あなたのせいでは ないのだ。\p"
	.string "そして あの子達は皆\n"
	.string "普通のポケルスに 感染した経験がある。\p"
	.string "一度でも 普通のポケルスに感染した\n"
	.string "ポケモンには ヒトポケルスは感染しない。\l"
	.string "今も 感染している最中であってもだ。\p"
	.string "……だからこそ 選ばれたのだろう。\n"
	.string "この世界で あなたと戦うために。』$"

ContestStory_Text_YorikoSameDay:
	.string "{JPN}ヨリコ『5つ 全部の 部門で\n"
	.string "優勝…… おめでとう。\p"
	.string "……ねえ 受付の 貼り紙 見た？\n"
	.string "「次の コンテストは 明日」って\l"
	.string "かいて あるの。\p"
	.string "でも あの 貼り紙\n"
	.string "ずっと 前から 同じ なのよ。\p"
	.string "誰も こまらないわ。\n"
	.string "今日と 同じ 明日が\l"
	.string "くるだけ だもの。\p"
	.string "……そう 思って いたのに。』$"

ContestStory_Text_DarkraiSameDay:
	.string "{JPN}ダークライ『この世界の人々は \n"
	.string "夢を見たまま 暮らしている。\p"
	.string "笑い 働き 拍手をする。\n"
	.string "だが 先へ進むことも 戻ることもなく \l"
	.string "ただ 今日を繰り返している。\p"
	.string "外から災いが来ても \n"
	.string "それに抗おうとする力は もう働かない。\p"
	.string "……そうして この世界は\n"
	.string "静かに 終わりを迎えてしまう。\p"
	.string "幸福が 偽物だから\n"
	.string "呼び戻すのではない。\p"
	.string "変わりゆく現実に 向き合う力を \n"
	.string "取り戻してほしいのだ。\p"
	.string "ここは 現実と心象世界の重なる場所。\n"
	.string "あなたの言葉も ここなら\l"
	.string "ヨリコの心の奥まで 届くはずだ。』$"

@ ---------------------------------------------------------------------------
@ Text: release battles at the facilities and the contest (the rest is in
@ src/data/frontier_story_targets.h)

FrontierStory_Text_TargetCollapses:
	.string "{JPN}ヒトポケルスの 姿が\n"
	.string "かききえた！\p"
	.string "{STR_VAR_1}は その ばに\n"
	.string "くずれおちた！$"

DomeStory_Text_ReikaReleaseDefeat:
	.string "{JPN}……わたくし…… 負けましたの……\n"
	.string "わたくしの 勝負で……$"

PalaceStory_Text_IkueReleaseDefeat:
	.string "{JPN}……私…… 負けたのね……\n"
	.string "私と エネコロロが……$"

FactoryStory_Text_ToshioReleaseDefeat:
	.string "{JPN}……ぼく…… 負けちゃった……\n"
	.string "ぼくと ゲンさんが……$"

ArenaStory_Text_EisukeReleaseDefeat:
	.string "{JPN}……おれ…… 負けた……\n"
	.string "おれが 負けたんだ……$"

PikeStory_Text_YayoiReleaseDefeat:
	.string "{JPN}……どうして……\n"
	.string "同じ 道を 選んだのに……$"

PyramidStory_Text_NobuoReleaseDefeat:
	.string "{JPN}……ああ 負けたな。\n"
	.string "……おれが。$"

ContestStory_Text_YorikoReleaseDefeat:
	.string "{JPN}……私…… 負けた……\n"
	.string "私の 演技が……$"

@ ---------------------------------------------------------------------------
@ Text: Yoriko at the inn

ContestStory_Text_InnYorikoWelcome:
	.string "{JPN}ヨリコ『あら {PLAYER}さん！\n"
	.string "ちょうど よかった。\p"
	.string "ハブネークと あたらしい 演技を\n"
	.string "練習 していたの。\p"
	.string "見ていって！』$"

ContestStory_Text_InnYorikoAsks:
	.string "{JPN}ヨリコ『……どう？\n"
	.string "どこが よかったかしら？』$"

ContestStory_Text_InnChoiceGlare:
	.string "{JPN}鋭い まなざし$"

ContestStory_Text_InnChoiceTail:
	.string "{JPN}しっぽの うごき$"

ContestStory_Text_InnChoiceScales:
	.string "{JPN}つやつやの うろこ$"

ContestStory_Text_InnChoiceTogether:
	.string "{JPN}二人の いきの あいかた$"

ContestStory_Text_InnYorikoGlare:
	.string "{JPN}ヨリコ『眼差しに 気づいてくれたのね！\p"
	.string "私が 一番\n"
	.string "見せたかった ところよ。\p"
	.string "……ふふ。 届くと\n"
	.string "やっぱり 嬉しいわ。』$"

ContestStory_Text_InnYorikoTail:
	.string "{JPN}ヨリコ『また しっぽね！\n"
	.string "ふふっ あなたらしいわ。\p"
	.string "じゃあ 次は しっぽが\n"
	.string "もっと いきる 演技を\l"
	.string "考えようかしら。』$"

ContestStory_Text_InnYorikoScales:
	.string "{JPN}ヨリコ『鱗！\n"
	.string "そこは 考えて いなかったわ。\p"
	.string "光の あてかたで\n"
	.string "もっと 輝くかも。\p"
	.string "おもしろいわね。』$"

ContestStory_Text_InnYorikoTogether:
	.string "{JPN}ヨリコ『私達 二人を\n"
	.string "見てくれたのね。\p"
	.string "……嬉しい。\n"
	.string "ハブネークも ほこらしそうよ。』$"

ContestStory_Text_InnYorikoOthers:
	.string "{JPN}ヨリコ『ここの みんなにも\n"
	.string "見てもらったの。\p"
	.string "怖い 顔が いいって 人も\n"
	.string "かわいいって 人も いたわ。\p"
	.string "見る 人に よって\n"
	.string "ぜんぜん 違うのね。\p"
	.string "それが こんなに 楽しいなんて\n"
	.string "知らなかった。\p"
	.string "次は どんな 演技に\n"
	.string "しようかしら。 ね ハブネーク。』$"

ContestStory_Text_InnSeviper:
	.string "{JPN}ハブネーク『シャーッ！』$"

@ ---------------------------------------------------------------------------
@ Text: Eisuke at the inn

ArenaStory_Text_InnEisuke:
	.string "{JPN}エイスケ『よう {PLAYER}！\n"
	.string "いい うみだろ？\p"
	.string "あの 1ターンの こと\n"
	.string "もう 振り返らないって 決めたんだ。\p"
	.string "悔しいのは 本当 だけどな。\n"
	.string "あの 試合は あれで おわり。\p"
	.string "次の レースに むけて\n"
	.string "スターミーと 泳いでくる！\p"
	.string "見てろよ！』$"

ArenaStory_Text_InnEisukeSwimsOff:
	.string "{JPN}エイスケと スターミーは\n"
	.string "海へ 飛び込んだ！\p"
	.string "二人の 姿が\n"
	.string "波の むこうへ とおざかっていく……$"

ArenaStory_Text_InnStarmie:
	.string "{JPN}スターミーの コアが\n"
	.string "きらりと ひかった！$"

@ ---------------------------------------------------------------------------
@ Text: Reika at the inn

DomeStory_Text_InnReika:
	.string "{JPN}レイカ『あら {PLAYER}さん。\n"
	.string "ごきげんよう。\p"
	.string "マッスグマと おさんぽ ちゅうですの。』$"

DomeStory_Text_InnReikaWaits:
	.string "{JPN}レイカ『あら また なにか\n"
	.string "みつけたのね。\p"
	.string "なにが あの子を ひきつけたのか\n"
	.string "わたくしには 分かりませんけれど……\p"
	.string "それで いいんですの。\n"
	.string "ここで 待っていますわ。』$"

DomeStory_Text_InnReikaWelcomesBack:
	.string "{JPN}レイカ『ふふ。 おかえりなさい。\p"
	.string "次は どちらへ いきましょうか。』$"

DomeStory_Text_InnLinoone:
	.string "{JPN}マッスグマは くんくんと\n"
	.string "じめんの においを かいでいる。$"

@ ---------------------------------------------------------------------------
@ Text: Ikue at the inn

PalaceStory_Text_InnIkue:
	.string "{JPN}イクエ『あら {PLAYER}さん。\p"
	.string "今 エネコロロの 遊びに\n"
	.string "つきあってるの。\p"
	.string "今日は なにを えらぶかしら。』$"

PalaceStory_Text_InnIkueJoins:
	.string "{JPN}イクエ『ふふ…… しっぽを おいかける\n"
	.string "遊びね。 私も まねしちゃった。\p"
	.string "私が 決めた あそびじゃ なくて\n"
	.string "この子が 選んだ 遊び。\p"
	.string "一緒に 過ごしてると\n"
	.string "少しずつ 分かって くるのね。』$"

PalaceStory_Text_InnDelcatty:
	.string "{JPN}エネコロロは ごろごろと\n"
	.string "のどを ならしている。$"

@ ---------------------------------------------------------------------------
@ Text: Toshio at the inn

FactoryStory_Text_InnToshio:
	.string "{JPN}トシオ『あ {PLAYER}！\p"
	.string "ゲンさん ちゃんと\n"
	.string "ファクトリーに かえしてきたよ。\p"
	.string "最後に 頭を なでたら\n"
	.string "硬い 頭で ごつんって\l"
	.string "してくれたんだ。 えへへ。\p"
	.string "もう 一緒には いないけど\n"
	.string "一緒に 勝った ことは\l"
	.string "全部 覚えてる。』$"

FactoryStory_Text_InnToshioTellsAggron:
	.string "{JPN}トシオ『それでね ボスゴドラ。\n"
	.string "今日の 勝負 すごかったんだ！\p"
	.string "ゲンさんが 相手の わざを\n"
	.string "全部 うけとめて……\p"
	.string "今度は きみと\n"
	.string "あんな 勝負を しような！』$"

FactoryStory_Text_InnAggronNods:
	.string "{JPN}ボスゴドラは\n"
	.string "おおきく 頷いた！$"

FactoryStory_Text_InnAggron:
	.string "{JPN}ボスゴドラは トシオの 話を\n"
	.string "じっと 聞いている。$"

@ ---------------------------------------------------------------------------
@ Text: Yayoi at the inn

PikeStory_Text_InnYayoiInvites:
	.string "{JPN}ヤヨイ『いらっしゃい {PLAYER}。\n"
	.string "ちょうど お茶を いれる ところなの。\p"
	.string "むかいに すわって。』$"

PikeStory_Text_InnYayoiAsksTea:
	.string "{JPN}ヤヨイ『私は ローズティー。\n"
	.string "ロズレイドと 同じ 香りなの。\p"
	.string "みんなが 好きだから じゃなくて\n"
	.string "私が 好きだから えらぶの。\p"
	.string "{PLAYER}は どれに する？』$"

PikeStory_Text_InnTeaMint:
	.string "{JPN}ヤヨイ『ミントね。\n"
	.string "すっと する 香り。\l"
	.string "頭が すっきり しそう。』$"

PikeStory_Text_InnTeaLemon:
	.string "{JPN}ヤヨイ『レモンね。\n"
	.string "さわやかで すっぱい 香り。\l"
	.string "元気が でそうね。』$"

PikeStory_Text_InnTeaChamomile:
	.string "{JPN}ヤヨイ『カモミールね。\n"
	.string "りんごみたいな 甘い 香り。\l"
	.string "ほっと するわね。』$"

PikeStory_Text_InnTeaJasmine:
	.string "{JPN}ヤヨイ『ジャスミンね。\n"
	.string "はなやかで 甘い 香り。\p"
	.string "同じ はなの お茶でも\n"
	.string "ローズとは ぜんぜん 違うのね。』$"

PikeStory_Text_InnYayoiTea:
	.string "{JPN}ヤヨイは ふたりぶんの お茶を\n"
	.string "いれて テーブルに ならべた。\p"
	.string "…… …… ……\p"
	.string "ヤヨイ『……ねえ ひとくち\n"
	.string "もらっても いい？』\p"
	.string "{PLAYER}は お茶を\n"
	.string "ヤヨイに 少し わけた。\p"
	.string "ヤヨイ『……ふふ。\n"
	.string "やっぱり 私は ローズが 好き。\p"
	.string "でも あなたが この 香りを\n"
	.string "好きな 訳 少しだけ\l"
	.string "分かった 気がするわ。\p"
	.string "前は みんなと 同じ 香りで\n"
	.string "同じ 道を 選んで いたけれど……\p"
	.string "違う お茶を ならべて\n"
	.string "おしゃべり するのも 楽しいわね。\p"
	.string "同じ ものが すきじゃ なくても\n"
	.string "一緒に いられるのね。』$"

PikeStory_Text_InnRoseradeHops:
	.string "{JPN}ロズレイドは 嬉しそうに\n"
	.string "りょうての 花束を ゆらした！$"

PikeStory_Text_InnRoserade:
	.string "{JPN}ロズレイドの 花束から\n"
	.string "優しい ローズの 香りが する。$"

@ ---------------------------------------------------------------------------
@ Nobuo and Sandslash: asking someone is part of finding the way.

PyramidStory_Text_InnNobuoAsks:
	.string "{JPN}ノブオ『おう {PLAYER}。\n"
	.string "いいところに来た。\p"
	.string "……実は 船着き場までの道が\n"
	.string "分からなくなっちまってな。\p"
	.string "来た時は 覚えたつもりだったんだが\n"
	.string "どの廊下も 同じに見えてさ。\p"
	.string "教えてくれないか？』$"

PyramidStory_Text_InnNobuoDirections:
	.string "{JPN}{PLAYER}は 広間から外へ出て\n"
	.string "海の方へ進む道を 教えた。\p"
	.string "ノブオ『……ああ そうだった。\n"
	.string "ありがとうな。\p"
	.string "聞いてみりゃ すぐ分かった。\n"
	.string "おれの頭に 道が流れてこなくても。\p"
	.string "……行けそうか サンドパン。』$"

PyramidStory_Text_InnNobuoThanks:
	.string "{JPN}ノブオ『……おれ達だけでも\n"
	.string "歩いてみるさ。\p"
	.string "また 分からなくなったら\n"
	.string "誰かに聞く。\p"
	.string "何度も聞かれちゃ 面倒だろうが……\n"
	.string "その時は また教えてくれよ。\p"
	.string "……助かった。ありがとうな。』$"

PyramidStory_Text_InnNobuoLater:
	.string "{JPN}ノブオ『ああ 忙しいか。\n"
	.string "引き止めて 悪かったな。\p"
	.string "もう少し 自分達で歩いてみるよ。\n"
	.string "迷ったら また誰かに聞くさ。』$"

PyramidStory_Text_InnSandslash:
	.string "{JPN}サンドパンは ノブオの方を向くと\n"
	.string "小さく 鼻を鳴らした。\p"
	.string "ノブオが 名前を呼ぶ。\n"
	.string "サンドパンは その声を聞いて\l"
	.string "ゆっくりと 振り返った。$"
