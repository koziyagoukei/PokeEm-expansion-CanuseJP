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
	fadescreen FADE_TO_WHITE
	waitse
	fadescreen FADE_FROM_WHITE
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
	switch VAR_RESULT
	case STORY_TARGET_REIKA, DomeStory_EventScript_ReleaseBattle
	case STORY_TARGET_EISUKE, ArenaStory_EventScript_ReleaseBattle
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

ArenaStory_EventScript_ReleaseBattle::
	setmultitrainerbattle TRAINER_EISUKE_RELEASE, ArenaStory_Text_EisukeReleaseDefeat, TRAINER_POKEVIRUS, FrontierStory_Text_PokevirusDefeat, PARTNER_ROCKET_COOK
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
@ The inn: everyone cured rests here. Kazuki in the lobby, Yoriko and Seviper in the west wing,
@ Eisuke and Starmie on the beach outside and Reika walking Linoone there, and in each part of the
@ building a few of the cured Brains and guests, picked on every visit.

InnStory_EventScript_SetupLobby::
	setflag FLAG_TEMP_11
	call_if_set FLAG_STORY_RESCUED_TOWER, InnStory_EventScript_ShowKazuki
	setvar VAR_0x8004, INN_AREA_LOBBY
	special FrontierStory_PickInnGuests
	return

InnStory_EventScript_ShowKazuki::
	clearflag FLAG_TEMP_11
	return

InnStory_EventScript_SetupEastWing::
	setvar VAR_0x8004, INN_AREA_EAST_WING
	special FrontierStory_PickInnGuests
	return

InnStory_EventScript_SetupWestWing::
	setflag FLAG_TEMP_12
	call_if_set FLAG_STORY_RESCUED_CONTEST, InnStory_EventScript_ShowYoriko
	setvar VAR_0x8004, INN_AREA_WEST_WING
	special FrontierStory_PickInnGuests
	return

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
	.string "そういう やつは きっと\n"
	.string "リゾートに たずねてくるだろうよ。』$"

FrontierStory_Text_CookKnownCure:
	.string "{JPN}てんしゅ『しあげは こいつだ。』\p"
	.string "てんしゅは きびだんごを\n"
	.string "ひとつ たべさせた。$"

FrontierStory_Text_CookSendsToInn:
	.string "{JPN}てんしゅ『りょかんなら\n"
	.string "めしも ふろも ある。 ゆっくり やすめよ。』$"

FrontierStory_Text_CookCarriesToHub:
	.string "{JPN}てんしゅ『おい しっかりしろ！\n"
	.string "……いきは ある。\p"
	.string "……だが うわごとを いってやがる。\n"
	.string "まだ なおっちゃ いねえんだ。\p"
	.string "ハブに はこぶぞ！\n"
	.string "めしを くわせりゃ ちからも もどる！』$"

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
	.string "{JPN}ダークライ『なおった ものには\n"
	.string "こころを やすめる ばしょが いる。\p"
	.string "うみの むこうに ふるい りょかんが\n"
	.string "ある。 いまは だれも つかっていない。\p"
	.string "ひろい へやが いくつも ある。\n"
	.string "これから もどる ものたちも\l"
	.string "そこで やすめるだろう。\p"
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

TowerStory_Text_KazukiGoesToInn:
	.string "{JPN}カズキ『ぼく りょかんで まってるね！\n"
	.string "こんどは ぼくと しょうぶしてよ！』$"

TowerStory_Text_InnKazukiChallenge:
	.string "{JPN}カズキ『あ！ {PLAYER}！\p"
	.string "ぼくね いつか この せかいの\n"
	.string "あの ひとに もういちど いどむって\l"
	.string "きめたんだ。\p"
	.string "そのために いまは\n"
	.string "きみと しょうぶ したい！\p"
	.string "しょうぶ してくれる？』$"

TowerStory_Text_InnKazukiHere:
	.string "{JPN}カズキ『やった！\n"
	.string "グラエナ！ いくよ！』$"

TowerStory_Text_InnKazukiDefeat:
	.string "{JPN}くやしい！ でも この くやしさは\n"
	.string "ぼくの ものだ！$"

TowerStory_Text_InnKazukiAfterLoss:
	.string "{JPN}カズキ『また まけちゃった！\n"
	.string "でも ちゃんと くやしいんだ。\p"
	.string "つぎは もっと つよく なって\n"
	.string "いどむからね！』$"

TowerStory_Text_InnKazukiWon:
	.string "{JPN}カズキ『かった……！ ぼくが かった！\p"
	.string "……あの ひとにも\n"
	.string "とどくかな。 えへへ。』$"

TowerStory_Text_InnKazukiLater:
	.string "{JPN}カズキ『そっか！\n"
	.string "また こんど しようね！』$"

@ ---------------------------------------------------------------------------
@ Text: the contest's scenes about the world

ContestStory_Text_YorikoApplause:
	.string "{JPN}ヨリコ『{PLAYER}さん！\n"
	.string "2つめの ぶもんも ゆうしょうね。\p"
	.string "さっきの はくしゅ きこえた？\n"
	.string "みんな おなじ ところで\l"
	.string "おなじ だけ てを たたくの。\p"
	.string "まるで ひとつの おおきな\n"
	.string "おとに つつまれる みたい。\p"
	.string "……でも あなたが ほめてくれた\n"
	.string "しっぽの こと まだ かんがえるの。』$"

ContestStory_Text_DarkraiApplause:
	.string "{JPN}ダークライ『さっきの はくしゅを\n"
	.string "きいて どう かんじた。\p"
	.string "この かいじょうは げんじつの\n"
	.string "たてものと こころの せかいが\l"
	.string "かさなっている。\p"
	.string "ここで ひびく はくしゅには\n"
	.string "この ばに いない ものの\l"
	.string "こころも まじっている。\p"
	.string "だから ひとつの おとの ように\n"
	.string "ぴたりと そろうのだ。\p"
	.string "つながった ものたちは\n"
	.string "きもちも きおくも ながれこみ\l"
	.string "じぶんと たにんの さかいが\l"
	.string "あいまいに なっている。\p"
	.string "だが みなが ひとつの こころに\n"
	.string "なった わけでは ない。\l"
	.string "ヨリコは ヨリコの ままだ。\p"
	.string "えがおの うらで くるしんでいる\n"
	.string "わけでも ない。\l"
	.string "ヨリコは ほんとうに しあわせなのだ。\p"
	.string "あなたの かんそうが ちがったのは\n"
	.string "あなたが つながっていないからだ。\p"
	.string "ヨリコは はじめて\n"
	.string "つながりの そとから とどいた\l"
	.string "ことばを きいたのだ。』$"

ContestStory_Text_YorikoHandshake:
	.string "{JPN}ヨリコ『3つめの ぶもんも\n"
	.string "ゆうしょうね。 おめでとう。\p"
	.string "……ねえ あくしゅ しましょう。\n"
	.string "てを つなぐと きもちが\l"
	.string "もっと つたわる きが するの。』\p"
	.string "{PLAYER}は ヨリコと\n"
	.string "あくしゅした。\p"
	.string "ヨリコ『……ふしぎ。\n"
	.string "あなたの きもちは\l"
	.string "ながれこんで こないのね。\p"
	.string "てを つないで いるのに。』$"

ContestStory_Text_DarkraiHandshake:
	.string "{JPN}ダークライ『……しんぱい いらない。\n"
	.string "あなたには うつらない。\p"
	.string "かたちを かえた ポケルスは\n"
	.string "ドクモチ という もちや\l"
	.string "ふれあい みずを とおして\l"
	.string "ひとと ポケモンに ひろがった。\p"
	.string "だが あなたや てんしゅ\n"
	.string "あなたの りょうしんの ように\l"
	.string "とおい せかいから きた ひとには\l"
	.string "うつらないのだ。\p"
	.string "……ひとつだけ きを つけてほしい。\n"
	.string "ドクモチ だけは べつだ。\p"
	.string "うつらない ものでも あれを たべれば\n"
	.string "こころを しばられて しまう。\p"
	.string "どこかで みかけても\n"
	.string "けっして くちに しては いけない。』$"

ContestStory_Text_YorikoPartners:
	.string "{JPN}ヨリコ『4つめの ぶもんも ゆうしょう！\n"
	.string "すごいわ。\p"
	.string "……ねえ ひとつ きづいたの。\n"
	.string "あなたの ポケモン\l"
	.string "えんぎの あいだ ときどき\l"
	.string "あなたの ほうを うかがって いたわ。\p"
	.string "まだ すこし えんりょ してる みたい。\n"
	.string "それなのに あんなに\l"
	.string "すてきな えんぎが できるのね。』$"

ContestStory_Text_DarkraiPartners:
	.string "{JPN}ダークライ『ヨリコの いう とおりだ。\p"
	.string "あなたの てもちの おおくは\n"
	.string "じぶんの いしで この せかいへ\l"
	.string "きたのでは ない。\p"
	.string "とつぜん つれて こられたのだ。\n"
	.string "なついて いないのは その ためだ。\l"
	.string "あなたの せいでは ない。\p"
	.string "そして あの こたちは みな\n"
	.string "ふつうの ポケルスに\l"
	.string "かかった ことが ある。\p"
	.string "ふつうの ポケルスに かかった\n"
	.string "ポケモンには かたちを かえた\l"
	.string "ポケルスは うつらない。\l"
	.string "かかっている さいちゅうでも だ。\p"
	.string "……だから あの こたちが\n"
	.string "えらばれたのだろう。\p"
	.string "この せかいで\n"
	.string "あなたと たたかう ために。』$"

ContestStory_Text_YorikoSameDay:
	.string "{JPN}ヨリコ『5つ ぜんぶの ぶもんで\n"
	.string "ゆうしょう…… おめでとう。\p"
	.string "……ねえ うけつけの はりがみ みた？\n"
	.string "「つぎの コンテストは あした」って\l"
	.string "かいて あるの。\p"
	.string "でも あの はりがみ\n"
	.string "ずっと まえから おなじ なのよ。\p"
	.string "だれも こまらないわ。\n"
	.string "きょうと おなじ あしたが\l"
	.string "くるだけ だもの。\p"
	.string "……そう おもって いたのに。』$"

ContestStory_Text_DarkraiSameDay:
	.string "{JPN}ダークライ『この せかいの ひとびとは\n"
	.string "ゆめを みた まま くらしている。\p"
	.string "わらい はたらき はくしゅを する。\n"
	.string "だが すすむ ことも\l"
	.string "もどる ことも なく\l"
	.string "ただ きょうを くりかえしている。\p"
	.string "もし そとから わざわいが きても\n"
	.string "それを とめようと する ちからは\l"
	.string "もう はたらかない。\p"
	.string "この せかいは そうして\n"
	.string "しずかに おわりを むかえて しまう。\p"
	.string "しあわせが にせものだから\n"
	.string "よびもどすのでは ない。\p"
	.string "かわっていく げんじつに\n"
	.string "むきあう ちからを\l"
	.string "とりもどして ほしいのだ。\p"
	.string "ここは げんじつと こころの せかいが\n"
	.string "かさなる ばしょ。\p"
	.string "ここで ぶつける きもちは\n"
	.string "きっと ヨリコの こころの\l"
	.string "おくまで とどく。』$"

@ ---------------------------------------------------------------------------
@ Text: release battles at the facilities and the contest (the rest is in
@ src/data/frontier_story_targets.h)

FrontierStory_Text_TargetCollapses:
	.string "{JPN}ヒトポケルスの すがたが\n"
	.string "かききえた！\p"
	.string "{STR_VAR_1}は その ばに\n"
	.string "くずれおちた！$"

DomeStory_Text_ReikaReleaseDefeat:
	.string "{JPN}……わたくし…… まけましたの……\n"
	.string "わたくしの しょうぶで……$"

ArenaStory_Text_EisukeReleaseDefeat:
	.string "{JPN}……おれ…… まけた……\n"
	.string "おれが まけたんだ……$"

ContestStory_Text_YorikoReleaseDefeat:
	.string "{JPN}……わたし…… まけた……\n"
	.string "わたしの えんぎが……$"

@ ---------------------------------------------------------------------------
@ Text: Yoriko at the inn

ContestStory_Text_InnYorikoWelcome:
	.string "{JPN}ヨリコ『あら {PLAYER}さん！\n"
	.string "ちょうど よかった。\p"
	.string "ハブネークと あたらしい えんぎを\n"
	.string "れんしゅう していたの。\p"
	.string "みていって！』$"

ContestStory_Text_InnYorikoAsks:
	.string "{JPN}ヨリコ『……どう？\n"
	.string "どこが よかったかしら？』$"

ContestStory_Text_InnChoiceGlare:
	.string "{JPN}するどい まなざし$"

ContestStory_Text_InnChoiceTail:
	.string "{JPN}しっぽの うごき$"

ContestStory_Text_InnChoiceScales:
	.string "{JPN}つやつやの うろこ$"

ContestStory_Text_InnChoiceTogether:
	.string "{JPN}ふたりの いきの あいかた$"

ContestStory_Text_InnYorikoGlare:
	.string "{JPN}ヨリコ『まなざしに きづいてくれたのね！\p"
	.string "わたしが いちばん\n"
	.string "みせたかった ところよ。\p"
	.string "……ふふ。 とどくと\n"
	.string "やっぱり うれしいわ。』$"

ContestStory_Text_InnYorikoTail:
	.string "{JPN}ヨリコ『また しっぽね！\n"
	.string "ふふっ あなたらしいわ。\p"
	.string "じゃあ つぎは しっぽが\n"
	.string "もっと いきる えんぎを\l"
	.string "かんがえようかしら。』$"

ContestStory_Text_InnYorikoScales:
	.string "{JPN}ヨリコ『うろこ！\n"
	.string "そこは かんがえて いなかったわ。\p"
	.string "ひかりの あてかたで\n"
	.string "もっと かがやくかも。\p"
	.string "おもしろいわね。』$"

ContestStory_Text_InnYorikoTogether:
	.string "{JPN}ヨリコ『わたしたち ふたりを\n"
	.string "みてくれたのね。\p"
	.string "……うれしい。\n"
	.string "ハブネークも ほこらしそうよ。』$"

ContestStory_Text_InnYorikoOthers:
	.string "{JPN}ヨリコ『ここの みんなにも\n"
	.string "みてもらったの。\p"
	.string "こわい かおが いいって ひとも\n"
	.string "かわいいって ひとも いたわ。\p"
	.string "みる ひとに よって\n"
	.string "ぜんぜん ちがうのね。\p"
	.string "それが こんなに たのしいなんて\n"
	.string "しらなかった。\p"
	.string "つぎは どんな えんぎに\n"
	.string "しようかしら。 ね ハブネーク。』$"

ContestStory_Text_InnSeviper:
	.string "{JPN}ハブネーク『シャーッ！』$"

@ ---------------------------------------------------------------------------
@ Text: Eisuke at the inn

ArenaStory_Text_InnEisuke:
	.string "{JPN}エイスケ『よう {PLAYER}！\n"
	.string "いい うみだろ？\p"
	.string "あの 1ターンの こと\n"
	.string "もう ふりかえらないって きめたんだ。\p"
	.string "くやしいのは ほんとう だけどな。\n"
	.string "あの しあいは あれで おわり。\p"
	.string "つぎの レースに むけて\n"
	.string "スターミーと およいでくる！\p"
	.string "みてろよ！』$"

ArenaStory_Text_InnEisukeSwimsOff:
	.string "{JPN}エイスケと スターミーは\n"
	.string "うみへ とびこんだ！\p"
	.string "ふたりの すがたが\n"
	.string "なみの むこうへ とおざかっていく……$"

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
	.string "なにが あの こを ひきつけたのか\n"
	.string "わたくしには わかりませんけれど……\p"
	.string "それで いいんですの。\n"
	.string "ここで まっていますわ。』$"

DomeStory_Text_InnReikaWelcomesBack:
	.string "{JPN}レイカ『ふふ。 おかえりなさい。\p"
	.string "つぎは どちらへ いきましょうか。』$"

DomeStory_Text_InnLinoone:
	.string "{JPN}マッスグマは くんくんと\n"
	.string "じめんの においを かいでいる。$"
