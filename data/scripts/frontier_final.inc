@ The final chapter: after the fourth rescue the rescued gather on the inn's beach and their words
@ open a rift there; down through the depths to Pokevirus, the last battle, and the ending in the
@ hub and at the resort. See also src/frontier_ending.c.

@ ---------------------------------------------------------------------------
@ The inn's beach

@ Called while the inn's beach loads (FLAG_TEMP_13 to FLAG_TEMP_1A are the gathered people,
@ FLAG_TEMP_1B the cook and Darkrai, FLAG_TEMP_1F the rift).
FinalStory_EventScript_SetupBeach::
	setflag FLAG_TEMP_13
	setflag FLAG_TEMP_14
	setflag FLAG_TEMP_15
	setflag FLAG_TEMP_16
	setflag FLAG_TEMP_17
	setflag FLAG_TEMP_18
	setflag FLAG_TEMP_19
	setflag FLAG_TEMP_1A
	setflag FLAG_TEMP_1B
	setflag FLAG_TEMP_1F
	call_if_set FLAG_STORY_PATH_OPEN, FinalStory_EventScript_ShowBeachRift
	special FrontierStory_ShouldGather
	call_if_eq VAR_RESULT, TRUE, FinalStory_EventScript_SetupGathering
	return

FinalStory_EventScript_ShowBeachRift:
	clearflag FLAG_TEMP_1F
	return

@ Everyone rescued stands on the beach instead of where they usually are.
FinalStory_EventScript_SetupGathering:
	setvar VAR_TEMP_1, 1
	setflag FLAG_TEMP_11
	setflag FLAG_TEMP_12
	call_if_set FLAG_STORY_RESCUED_TOWER, FinalStory_EventScript_GatherKazuki
	call_if_set FLAG_STORY_RESCUED_DOME, FinalStory_EventScript_GatherReika
	call_if_set FLAG_STORY_RESCUED_PALACE, FinalStory_EventScript_GatherIkue
	call_if_set FLAG_STORY_RESCUED_ARENA, FinalStory_EventScript_GatherEisuke
	call_if_set FLAG_STORY_RESCUED_FACTORY, FinalStory_EventScript_GatherToshio
	call_if_set FLAG_STORY_RESCUED_PIKE, FinalStory_EventScript_GatherYayoi
	call_if_set FLAG_STORY_RESCUED_PYRAMID, FinalStory_EventScript_GatherNobuo
	call_if_set FLAG_STORY_RESCUED_CONTEST, FinalStory_EventScript_GatherYoriko
	return

FinalStory_EventScript_GatherKazuki:
	clearflag FLAG_TEMP_13
	return

FinalStory_EventScript_GatherReika:
	clearflag FLAG_TEMP_14
	return

FinalStory_EventScript_GatherIkue:
	clearflag FLAG_TEMP_15
	return

FinalStory_EventScript_GatherEisuke:
	clearflag FLAG_TEMP_16
	return

FinalStory_EventScript_GatherToshio:
	clearflag FLAG_TEMP_17
	return

FinalStory_EventScript_GatherYayoi:
	clearflag FLAG_TEMP_18
	return

FinalStory_EventScript_GatherNobuo:
	clearflag FLAG_TEMP_19
	return

FinalStory_EventScript_GatherYoriko:
	clearflag FLAG_TEMP_1A
	return

@ Coming off the ferry after the fourth rescue: each of them says, in their own words, that they
@ want to live with others even if it is hard; and the way to the depths opens.
FinalStory_EventScript_Gathering::
	lockall
	setvar VAR_TEMP_1, 2
	applymovement OBJ_EVENT_ID_PLAYER, FinalStory_Movement_UpToGathering
	waitmovement 0
	msgbox FinalStory_Text_PeopleGathered, MSGBOX_DEFAULT
	closemessage
	playse SE_M_NIGHTMARE
	fadescreen FADE_TO_WHITE
	clearflag FLAG_TEMP_1B
	addobject LOCALID_INN_GATHER_COOK
	addobject LOCALID_INN_GATHER_DARKRAI
	fadescreen FADE_FROM_WHITE
	waitse
	msgbox FinalStory_Text_DarkraiSensesWords, MSGBOX_DEFAULT
	msgbox FinalStory_Text_CookListens, MSGBOX_DEFAULT
	closemessage
	call_if_set FLAG_STORY_RESCUED_TOWER, FinalStory_EventScript_KazukiSpeaks
	call_if_set FLAG_STORY_RESCUED_DOME, FinalStory_EventScript_ReikaSpeaks
	call_if_set FLAG_STORY_RESCUED_PALACE, FinalStory_EventScript_IkueSpeaks
	call_if_set FLAG_STORY_RESCUED_ARENA, FinalStory_EventScript_EisukeSpeaks
	call_if_set FLAG_STORY_RESCUED_FACTORY, FinalStory_EventScript_ToshioSpeaks
	call_if_set FLAG_STORY_RESCUED_PIKE, FinalStory_EventScript_YayoiSpeaks
	call_if_set FLAG_STORY_RESCUED_PYRAMID, FinalStory_EventScript_NobuoSpeaks
	call_if_set FLAG_STORY_RESCUED_CONTEST, FinalStory_EventScript_YorikoSpeaks
	msgbox FinalStory_Text_WordsOverlap, MSGBOX_DEFAULT
	closemessage
	playse SE_M_DETECT
	fadescreen FADE_TO_WHITE
	clearflag FLAG_TEMP_1F
	addobject LOCALID_INN_RIFT
	fadescreen FADE_FROM_WHITE
	waitse
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceLeft
	applymovement LOCALID_INN_GATHER_COOK, Common_Movement_FaceLeft
	applymovement LOCALID_INN_GATHER_DARKRAI, Common_Movement_FaceLeft
	waitmovement 0
	msgbox FinalStory_Text_RiftOpened, MSGBOX_DEFAULT
	msgbox FinalStory_Text_DarkraiExplainsRift, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceRight
	waitmovement 0
	msgbox FinalStory_Text_CookReady, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_STORY_PATH_OPEN
	fadescreen FADE_TO_BLACK
	call FinalStory_EventScript_EndGathering
	fadescreen FADE_FROM_BLACK
	releaseall
	end

@ Everyone goes back to how the beach usually is.
FinalStory_EventScript_EndGathering:
	removeobject LOCALID_INN_GATHER_COOK
	removeobject LOCALID_INN_GATHER_DARKRAI
	removeobject LOCALID_INN_GATHER_KAZUKI
	removeobject LOCALID_INN_GATHER_REIKA
	removeobject LOCALID_INN_GATHER_IKUE
	removeobject LOCALID_INN_GATHER_EISUKE
	removeobject LOCALID_INN_GATHER_TOSHIO
	removeobject LOCALID_INN_GATHER_YAYOI
	removeobject LOCALID_INN_GATHER_NOBUO
	removeobject LOCALID_INN_GATHER_YORIKO
	setflag FLAG_TEMP_1B
	setflag FLAG_TEMP_13
	setflag FLAG_TEMP_14
	setflag FLAG_TEMP_15
	setflag FLAG_TEMP_16
	setflag FLAG_TEMP_17
	setflag FLAG_TEMP_18
	setflag FLAG_TEMP_19
	setflag FLAG_TEMP_1A
	call_if_set FLAG_STORY_RESCUED_ARENA, FinalStory_EventScript_BackToBeachEisuke
	call_if_set FLAG_STORY_RESCUED_DOME, FinalStory_EventScript_BackToBeachReika
	return

FinalStory_EventScript_BackToBeachEisuke:
	clearflag FLAG_TEMP_11
	addobject LOCALID_INN_EISUKE
	addobject LOCALID_INN_STARMIE
	return

FinalStory_EventScript_BackToBeachReika:
	clearflag FLAG_TEMP_12
	addobject LOCALID_INN_REIKA
	addobject LOCALID_INN_LINOONE
	return

FinalStory_EventScript_KazukiSpeaks:
	applymovement LOCALID_INN_GATHER_KAZUKI, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox FinalStory_Text_KazukiWords, MSGBOX_DEFAULT
	closemessage
	return

FinalStory_EventScript_ReikaSpeaks:
	applymovement LOCALID_INN_GATHER_REIKA, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox FinalStory_Text_ReikaWords, MSGBOX_DEFAULT
	closemessage
	return

FinalStory_EventScript_IkueSpeaks:
	applymovement LOCALID_INN_GATHER_IKUE, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox FinalStory_Text_IkueWords, MSGBOX_DEFAULT
	closemessage
	return

FinalStory_EventScript_EisukeSpeaks:
	applymovement LOCALID_INN_GATHER_EISUKE, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox FinalStory_Text_EisukeWords, MSGBOX_DEFAULT
	closemessage
	return

FinalStory_EventScript_ToshioSpeaks:
	applymovement LOCALID_INN_GATHER_TOSHIO, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox FinalStory_Text_ToshioWords, MSGBOX_DEFAULT
	closemessage
	return

FinalStory_EventScript_YayoiSpeaks:
	applymovement LOCALID_INN_GATHER_YAYOI, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox FinalStory_Text_YayoiWords, MSGBOX_DEFAULT
	closemessage
	return

FinalStory_EventScript_NobuoSpeaks:
	applymovement LOCALID_INN_GATHER_NOBUO, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox FinalStory_Text_NobuoWords, MSGBOX_DEFAULT
	closemessage
	return

FinalStory_EventScript_YorikoSpeaks:
	applymovement LOCALID_INN_GATHER_YORIKO, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox FinalStory_Text_YorikoWords, MSGBOX_DEFAULT
	closemessage
	return

@ The rift on the beach: into the depths.
FinalStory_EventScript_BeachRift::
	lock
	goto_if_set FLAG_STORY_POKEVIRUS_FADED, FinalStory_EventScript_BeachRiftQuiet
	msgbox FinalStory_Text_BeachRift, MSGBOX_YESNO
	goto FinalStory_EventScript_EnterRift

FinalStory_EventScript_BeachRiftQuiet:
	msgbox FinalStory_Text_BeachRiftQuiet, MSGBOX_YESNO
FinalStory_EventScript_EnterRift:
	goto_if_eq VAR_RESULT, NO, FinalStory_EventScript_RiftStay
	closemessage
	playse SE_M_DETECT
	waitse
	warp MAP_POKEVIRUS_DEPTHS_APPROACH, 14, 28
	waitstate
	release
	end

FinalStory_EventScript_RiftStay:
	release
	end

@ ---------------------------------------------------------------------------
@ The depths

@ The rift the player came in by: back to the inn's beach.
FinalStory_EventScript_ExitRift::
	lock
	msgbox FinalStory_Text_ExitRift, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, FinalStory_EventScript_RiftStay
	closemessage
	playse SE_M_DETECT
	waitse
	warp MAP_RESTORATION_INN_OUTSIDE, 13, 25
	waitstate
	release
	end

@ The first time down: the cook and Darkrai look around, then come along behind the player.
FinalStory_EventScript_DepthsEntry::
	setvar VAR_TEMP_1, 1
	goto_if_set FLAG_STORY_DEPTHS_ENTERED, FinalStory_EventScript_Done
	goto_if_set FLAG_STORY_POKEVIRUS_FADED, FinalStory_EventScript_Done
	lockall
	playse SE_M_NIGHTMARE
	fadescreen FADE_TO_WHITE
	clearflag FLAG_TEMP_1D
	addobject LOCALID_DEPTHS_COOK
	addobject LOCALID_DEPTHS_DARKRAI
	fadescreen FADE_FROM_WHITE
	waitse
	msgbox FinalStory_Text_CookLooksAround, MSGBOX_DEFAULT
	msgbox FinalStory_Text_DarkraiScatteredMemories, MSGBOX_DEFAULT
	msgbox FinalStory_Text_CookOneWay, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_DEPTHS_COOK
	removeobject LOCALID_DEPTHS_DARKRAI
	setflag FLAG_TEMP_1D
	fadescreen FADE_FROM_BLACK
	msgbox FinalStory_Text_TheyFollow, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_STORY_DEPTHS_ENTERED
	releaseall
	end

@ Halfway down the spiral.
FinalStory_EventScript_HelixTalk::
	setvar VAR_TEMP_1, 1
	goto_if_set FLAG_STORY_DEPTHS_HELIX_TALK, FinalStory_EventScript_Done
	goto_if_set FLAG_STORY_POKEVIRUS_FADED, FinalStory_EventScript_Done
	lockall
	msgbox FinalStory_Text_DarkraiHearsHer, MSGBOX_DEFAULT
	msgbox FinalStory_Text_CookAsksPecharunt, MSGBOX_DEFAULT
	msgbox FinalStory_Text_DarkraiPecharuntDreams, MSGBOX_DEFAULT
	msgbox FinalStory_Text_CookWakeHer, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_STORY_DEPTHS_HELIX_TALK
	releaseall
	end

@ The core (FLAG_TEMP_1D: the cook and Darkrai, FLAG_TEMP_1E: Pokevirus).
FinalStory_EventScript_SetupCore::
	setflag FLAG_TEMP_1D
	setflag FLAG_TEMP_1E
	goto_if_set FLAG_STORY_POKEVIRUS_FADED, FinalStory_EventScript_CoreQuiet
	return

FinalStory_EventScript_CoreQuiet:
	setvar VAR_TEMP_1, 1
	return

@ Arriving at the core: Pokevirus stands in the way, and the last battle.
FinalStory_EventScript_CoreArrival::
	lockall
	setvar VAR_TEMP_1, 1
	applymovement OBJ_EVENT_ID_PLAYER, FinalStory_Movement_UpToRing
	waitmovement 0
	playse SE_M_CONFUSE_RAY
	fadescreen FADE_TO_WHITE
	clearflag FLAG_TEMP_1E
	addobject LOCALID_CORE_POKEVIRUS
	fadescreen FADE_FROM_WHITE
	waitse
	msgbox FinalStory_Text_PokevirusAppears, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CORE_POKEVIRUS, TowerStory_Movement_Tremble
	waitmovement 0
	msgbox FinalStory_Text_PokevirusStandsInWay, MSGBOX_DEFAULT
	closemessage
	playse SE_M_NIGHTMARE
	fadescreen FADE_TO_WHITE
	clearflag FLAG_TEMP_1D
	addobject LOCALID_CORE_COOK
	addobject LOCALID_CORE_DARKRAI
	fadescreen FADE_FROM_WHITE
	waitse
	msgbox FinalStory_Text_DarkraiItDoesNotKnow, MSGBOX_DEFAULT
	msgbox FinalStory_Text_CookNotADream, MSGBOX_DEFAULT
	msgbox FinalStory_Text_DarkraiAsksHelp, MSGBOX_DEFAULT
	goto FinalStory_EventScript_AskFinalBattle

@ Talking to Pokevirus again after backing out or losing.
FinalStory_EventScript_Pokevirus::
	lockall
	msgbox FinalStory_Text_CookOnceMore, MSGBOX_DEFAULT
FinalStory_EventScript_AskFinalBattle:
	msgbox FinalStory_Text_CookAsksFinalReady, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, FinalStory_EventScript_FinalNotYet
	call FrontierStory_EventScript_ChooseReleaseParty
	goto_if_eq VAR_RESULT, FALSE, FinalStory_EventScript_FinalNotYet
	setmultitrainerbattle TRAINER_POKEVIRUS_FINAL, FrontierStory_Text_PokevirusDefeat, TRAINER_POKEVIRUS_FINAL, FrontierStory_Text_PokevirusDefeat, PARTNER_ROCKET_COOK
	special FrontierStory_PrepareFinalBattle
	setvar VAR_0x8005, MULTI_BATTLE_2_VS_1 | MULTI_BATTLE_CHOOSE_MONS
	callnative BattleSetup_StartMultiBattle
	waitstate
	special FrontierStory_EndStoryBattle
	special LoadPlayerParty
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, FinalStory_EventScript_PokevirusSpeaks
	msgbox FinalStory_Text_CookLostFinal, MSGBOX_DEFAULT
	releaseall
	end

FinalStory_EventScript_FinalNotYet:
	msgbox FinalStory_Text_CookFinalLater, MSGBOX_DEFAULT
	releaseall
	end

@ Won: Darkrai goes up to Pokevirus and speaks to it, and for the first time it answers in words.
FinalStory_EventScript_PokevirusSpeaks:
	msgbox FinalStory_Text_PokevirusUnravels, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CORE_DARKRAI, FinalStory_Movement_DarkraiToPokevirus
	waitmovement 0
	msgbox FinalStory_Text_DarkraiFirstWords, MSGBOX_DEFAULT
	closemessage
	delay 30
	msgbox FinalStory_Text_PokevirusFirstWords, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CORE_COOK, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox FinalStory_Text_CookHeardIt, MSGBOX_DEFAULT
	msgbox FinalStory_Text_DarkraiItReached, MSGBOX_DEFAULT
	closemessage
	msgbox FinalStory_Text_PeopleWaking, MSGBOX_DEFAULT
	msgbox FinalStory_Text_PokevirusGlad, MSGBOX_DEFAULT
	closemessage
	playse SE_M_HAZE
	applymovement LOCALID_CORE_POKEVIRUS, TowerStory_Movement_PokevirusFades
	waitmovement 0
	removeobject LOCALID_CORE_POKEVIRUS
	setflag FLAG_TEMP_1E
	waitse
	setflag FLAG_STORY_POKEVIRUS_FADED
	delay 30
	applymovement LOCALID_CORE_DARKRAI, Common_Movement_FaceDown
	waitmovement 0
	msgbox FinalStory_Text_DarkraiFarewell, MSGBOX_DEFAULT
	msgbox FinalStory_Text_CookStrangeCreature, MSGBOX_DEFAULT
	msgbox FinalStory_Text_DarkraiBackToHub, MSGBOX_DEFAULT
	closemessage
	setvar VAR_STORY_ENDING_STATE, ENDING_STATE_HUB
	fadescreen FADE_TO_WHITE
	warp MAP_ULTRA_FRONTIE_HUB, 21, 15
	waitstate
	releaseall
	end

@ ---------------------------------------------------------------------------
@ The ending

@ Called while the hub loads: the cook stands beside Pecharunt for the waking.
FinalStory_EventScript_SetupEndingHub::
	goto_if_ne VAR_STORY_ENDING_STATE, ENDING_STATE_HUB, FrontierStory_EventScript_Return
	setobjectxyperm LOCALID_FRONTIER_HUB_INTRO_COOK, 20, 15
	setobjectmovementtype LOCALID_FRONTIER_HUB_INTRO_COOK, MOVEMENT_TYPE_FACE_RIGHT
	clearflag FLAG_TEMP_10
	return

@ Pecharunt wakes, remembering everything, and asks what nobody can answer.
FinalStory_EventScript_EndingHub::
	lockall
	setvar VAR_STORY_ENDING_STATE, ENDING_STATE_RESORT
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceRight
	waitmovement 0
	delay 30
	playmoncry SPECIES_PECHARUNT, CRY_MODE_NORMAL
	applymovement LOCALID_HUB_PECHARUNT, FinalStory_Movement_PecharuntWakes
	waitmovement 0
	waitmoncry
	msgbox FinalStory_Text_PecharuntWakes, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_HUB_PECHARUNT, FinalStory_Movement_PecharuntLooksAround
	waitmovement 0
	msgbox FinalStory_Text_PecharuntAsks, MSGBOX_DEFAULT
	closemessage
	delay 60
	msgbox FinalStory_Text_NoOneAnswers, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_HUB_PECHARUNT, Common_Movement_FaceUp
	waitmovement 0
	msgbox FinalStory_Text_PecharuntStaysByDarkrai, MSGBOX_DEFAULT
	msgbox FinalStory_Text_CookOffersFood, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_FRONTIER_HUB_INTRO_COOK
	setflag FLAG_TEMP_10
	warp MAP_PARTY_ROAMER_PARK, 13, 5
	waitstate
	releaseall
	end

@ Called while the resort loads: the parents wait south of the gate as when the player arrived.
FinalStory_EventScript_SetupEndingResort::
	goto_if_ne VAR_STORY_ENDING_STATE, ENDING_STATE_RESORT, FrontierStory_EventScript_Return
	setobjectxyperm LOCALID_PARTY_ROAMER_MOM, 12, 9
	setobjectmovementtype LOCALID_PARTY_ROAMER_MOM, MOVEMENT_TYPE_FACE_RIGHT
	setobjectxyperm LOCALID_PARTY_ROAMER_NORMAN, 13, 9
	setobjectmovementtype LOCALID_PARTY_ROAMER_NORMAN, MOVEMENT_TYPE_FACE_LEFT
	return

@ The family, and the cook: the rescues go on, so they stay, and make it a family trip.
FinalStory_EventScript_EndingResort::
	lockall
	setvar VAR_STORY_ENDING_STATE, ENDING_STATE_DONE
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_WalkDown
	waitmovement 0
	opendoor 13, 4
	waitdooranim
	clearflag FLAG_TEMP_10
	addobject LOCALID_PARTY_ROAMER_INTRO_COOK
	applymovement LOCALID_PARTY_ROAMER_INTRO_COOK, FrontierIntro_Movement_WalkDown
	waitmovement 0
	closedoor 13, 4
	waitdooranim
	playse SE_PIN
	applymovement LOCALID_PARTY_ROAMER_MOM, FrontierIntro_Movement_NoticeUp
	applymovement LOCALID_PARTY_ROAMER_NORMAN, FrontierIntro_Movement_NoticeUp
	waitmovement 0
	applymovement LOCALID_PARTY_ROAMER_MOM, FrontierIntro_Movement_MomRunUp
	applymovement LOCALID_PARTY_ROAMER_NORMAN, FrontierIntro_Movement_NormanWalkUp
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceLeft
	waitmovement 0
	msgbox FinalStory_Text_MomWelcomesBack, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceUp
	waitmovement 0
	msgbox FinalStory_Text_CookNotOverYet, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceRight
	waitmovement 0
	msgbox FinalStory_Text_DadWeStay, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceLeft
	waitmovement 0
	msgbox FinalStory_Text_MomFamilyTrip, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceUp
	waitmovement 0
	msgbox FinalStory_Text_CookLively, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_PARTY_ROAMER_INTRO_COOK
	setflag FLAG_TEMP_10
	special FrontierStory_ShowEnding
	waitstate
	call FinalStory_EventScript_TryNamePokevirus
	releaseall
	end

@ After every scenario rescue and every Brain/guest cure. The saved name is the
@ completion marker; returning from the naming screen or Pokedex cannot replay it.
FinalStory_EventScript_PokevirusEpilogue::
	setvar VAR_TEMP_6, 0
	lockall
	call FinalStory_EventScript_TryNamePokevirus
	releaseall
	end

FinalStory_EventScript_TryNamePokevirus::
	special FrontierPokevirus_CheckEpilogue
	goto_if_eq VAR_RESULT, FALSE, FrontierStory_EventScript_Return
	msgbox FinalStory_Text_EveryoneAwake, MSGBOX_DEFAULT
	msgbox FinalStory_Text_WantedToBePokemon, MSGBOX_DEFAULT
	msgbox FinalStory_Text_CannotBePokemon, MSGBOX_DEFAULT
	msgbox FinalStory_Text_NameDoesNotForgive, MSGBOX_DEFAULT
	closemessage
	delay 60
	msgbox FinalStory_Text_Nevertheless, MSGBOX_DEFAULT
	closemessage
	delay 60
	msgbox FinalStory_Text_GiveSpeciesName, MSGBOX_DEFAULT
FinalStory_EventScript_EnterPokevirusName:
	closemessage
	special FrontierPokevirus_Name
	waitstate
	special FrontierPokevirus_Register
	goto_if_eq VAR_RESULT, FALSE, FinalStory_EventScript_EnterPokevirusName
	msgbox FinalStory_Text_NameRemains, MSGBOX_DEFAULT
	closemessage
	special FrontierPokevirus_ShowRecord
	waitstate
	return

FinalStory_Text_EveryoneAwake::
	.string "{JPN}店主『……最後の ひとりだ。\n"
	.string "これで 皆 目を覚ました。』\p"
	.string "ダークライの 声が届いた。$"

FinalStory_Text_WantedToBePokemon::
	.string "{JPN}ダークライ『あれは ポケモンに\n"
	.string "なろうとしていたのだ。\p"
	.string "誰かに 呼ばれ 誰かと 生きる。\n"
	.string "そんな 生き物に。』$"

FinalStory_Text_CannotBePokemon::
	.string "{JPN}ダークライ『だが あれが\n"
	.string "ポケモンとして 成り立つには\l"
	.string "他の全てを 犠牲にするしかない。\p"
	.string "他の命も 心も 奪って\n"
	.string "自分の一部に してしまう。\p"
	.string "その存在を ポケモンだと\n"
	.string "認めることは できない。』$"

FinalStory_Text_NameDoesNotForgive::
	.string "{JPN}店主『……人の人生を 奪った。\n"
	.string "取り返せねえものも ある。\p"
	.string "名前を 付けたって\n"
	.string "それが 消えるわけじゃねえ。』$"

FinalStory_Text_Nevertheless::
	.string "{JPN}ダークライ『……それでも。』$"

FinalStory_Text_GiveSpeciesName::
	.string "{JPN}ダークライ『「はじめまして」と\n"
	.string "答えたものを\l"
	.string "なかったことには したくない。\p"
	.string "ヒトポケルスは わたしたちが\n"
	.string "災いに 付けた呼び名だ。\p"
	.string "{PLAYER}。\n"
	.string "あの生き物に 名前を。\p"
	.string "ポケモンとしての 種族名を\n"
	.string "あなたの図鑑に 残してほしい。』$"

FinalStory_Text_NameRemains::
	.string "{JPN}{STR_VAR_1}。\p"
	.string "図鑑に 番号は付かなかった。\n"
	.string "けれど 名前は残った。$"

@ ---------------------------------------------------------------------------
@ Movements

FinalStory_Movement_UpToGathering:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

FinalStory_Movement_UpToRing:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

FinalStory_Movement_DarkraiToPokevirus:
	walk_slow_up
	walk_slow_left
	face_up
	step_end

FinalStory_Movement_PecharuntWakes:
	jump_in_place_down
	delay_16
	step_end

FinalStory_Movement_PecharuntLooksAround:
	face_left
	delay_16
	face_up
	delay_16
	face_left
	delay_16
	step_end

@ ---------------------------------------------------------------------------
@ Text: the gathering

FinalStory_Text_PeopleGathered:
	.string "{JPN}浜辺に {PLAYER}が 助けた\n"
	.string "人たちが 集まっている……$"

FinalStory_Text_DarkraiSensesWords:
	.string "{JPN}ダークライ『……{PLAYER}。\n"
	.string "皆が 何かを 伝えようと している。』$"

FinalStory_Text_CookListens:
	.string "{JPN}店主『おう。 ……聞いてやろうぜ。』$"

FinalStory_Text_KazukiWords:
	.string "{JPN}カズキ『負けると くやしいし\n"
	.string "約束が 守れない 日も ある。\p"
	.string "それでも ぼくは だれかと 勝負して\n"
	.string "約束して 生きて いきたい！』$"

FinalStory_Text_ReikaWords:
	.string "{JPN}レイカ『分からない ことばかり ですわ。\n"
	.string "あの子の ことも 皆さんの ことも。\p"
	.string "それでも 分からない まま\n"
	.string "信じて いっしょに 歩きたいの。』$"

FinalStory_Text_IkueWords:
	.string "{JPN}イクエ『うまく いかない ことばかり。\n"
	.string "でも その たびに あの子を\l"
	.string "少しずつ 知って いけるの。\p"
	.string "私は だれかと いっしょに\n"
	.string "生きて いきたい。』$"

FinalStory_Text_EisukeWords:
	.string "{JPN}エイスケ『くやしい ことばっかりだ。\n"
	.string "それでも おれは だれかと 競って\l"
	.string "走って 生きて いくぜ！』$"

FinalStory_Text_ToshioWords:
	.string "{JPN}トシオ『いつかは わかれる 日も くる。\n"
	.string "さみしいけど……\p"
	.string "それでも ぼくは だれかと いっしょに\n"
	.string "戦って 生きたいんだ！』$"

FinalStory_Text_YayoiWords:
	.string "{JPN}ヤヨイ『好きな ものが 違えば\n"
	.string "すれちがう ことも あるわ。\p"
	.string "それでも 違う だれかと\n"
	.string "同じ テーブルに つきたいの。』$"

FinalStory_Text_NobuoWords:
	.string "{JPN}ノブオ『言葉に しないと\n"
	.string "伝わらない ことばかりだ。\p"
	.string "面倒でも…… おれは 自分の 言葉で\n"
	.string "頼りながら 生きて いくさ。』$"

FinalStory_Text_YorikoWords:
	.string "{JPN}ヨリコ『伝わらない ことも\n"
	.string "誤解される ことも あるわ。\p"
	.string "それでも 私は だれかの 前で\n"
	.string "私の 演技を 見せて 生きたいの。』$"

FinalStory_Text_WordsOverlap:
	.string "{JPN}ひとり ひとりの 言葉が\n"
	.string "波の 音に 重なって いく……$"

FinalStory_Text_RiftOpened:
	.string "{JPN}浜辺の 一角で\n"
	.string "空間が 裂けた！$"

FinalStory_Text_DarkraiExplainsRift:
	.string "{JPN}ダークライ『……皆の 言葉が\n"
	.string "道を 開いた。\p"
	.string "あの 裂け目の 先は\n"
	.string "皆の 心が 混ざり合う 場所だ。\p"
	.string "その 奥に ヒトポケルスが いる。\n"
	.string "そして…… モモワロウの 気配も。』$"

FinalStory_Text_CookReady:
	.string "{JPN}店主『……いよいよか。\p"
	.string "準備が できたら あの 裂け目に 来な。\n"
	.string "おれも ダークライも 一緒に 行く。\p"
	.string "急がなくて いい。\n"
	.string "やり残した ことが あるなら\l"
	.string "先に 済ませて こい。』$"

FinalStory_Text_BeachRift:
	.string "{JPN}裂け目の 向こうから\n"
	.string "大勢の 声が かすかに 聞こえる……\p"
	.string "裂け目に 入りますか？$"

FinalStory_Text_BeachRiftQuiet:
	.string "{JPN}裂け目の 向こうは\n"
	.string "静まり返って いる。\p"
	.string "裂け目に 入りますか？$"

FinalStory_Text_ExitRift:
	.string "{JPN}裂け目の 向こうに\n"
	.string "旅館の 浜辺が 見える。\p"
	.string "浜辺に 戻りますか？$"

@ ---------------------------------------------------------------------------
@ Text: the depths

FinalStory_Text_CookLooksAround:
	.string "{JPN}店主『……ここが 皆の 心が\n"
	.string "混ざり合う 場所か。』$"

FinalStory_Text_DarkraiScatteredMemories:
	.string "{JPN}ダークライ『見ろ。 この 景色……\n"
	.string "皆の 思い出が 散らばって いる。』$"

FinalStory_Text_CookOneWay:
	.string "{JPN}店主『道は 一本 みてえだな。\p"
	.string "帰りたく なったら いつでも\n"
	.string "あの 裂け目から 戻れる。\p"
	.string "……行こうぜ {PLAYER}。』$"

FinalStory_Text_TheyFollow:
	.string "{JPN}店主と ダークライは\n"
	.string "{PLAYER}の 後ろを ついてくる。$"

FinalStory_Text_DarkraiHearsHer:
	.string "{JPN}ダークライ『……聞こえるか。\n"
	.string "あの子の 声が 近い。』$"

FinalStory_Text_CookAsksPecharunt:
	.string "{JPN}店主『あの子って……\n"
	.string "モモワロウか？』$"

FinalStory_Text_DarkraiPecharuntDreams:
	.string "{JPN}ダークライ『ああ。 あの子は\n"
	.string "ずっと ここで 夢を 見ている。』$"

FinalStory_Text_CookWakeHer:
	.string "{JPN}店主『なら 起こしに\n"
	.string "行って やらねえとな。』$"

FinalStory_Text_PokevirusAppears:
	.string "{JPN}ヒトポケルスが\n"
	.string "姿を 現した！\p"
	.string "…… ……………… ……$"

FinalStory_Text_PokevirusStandsInWay:
	.string "{JPN}ヒトポケルスは {PLAYER}たちの\n"
	.string "前に 立ちふさがった。\p"
	.string "まるで 何かを\n"
	.string "引き止めようと している ように……$"

FinalStory_Text_DarkraiItDoesNotKnow:
	.string "{JPN}ダークライ『……あれは まだ\n"
	.string "分かって いない。\p"
	.string "皆が なぜ 離れて いくのか。\p"
	.string "ずっと 皆を つなぎとめて きた。\n"
	.string "幸せで あるように と。』$"

FinalStory_Text_CookNotADream:
	.string "{JPN}店主『悪気が ねえのは 分かってる。\p"
	.string "だがな…… 夢の 中に\n"
	.string "閉じこめて おくのは 違うだろ。』$"

FinalStory_Text_DarkraiAsksHelp:
	.string "{JPN}ダークライ『{PLAYER}。\n"
	.string "力を 貸して くれ。』$"

FinalStory_Text_CookOnceMore:
	.string "{JPN}店主『もう一度 行くか？』$"

FinalStory_Text_CookAsksFinalReady:
	.string "{JPN}店主『最後の 勝負だ。\n"
	.string "ポケモンを 3匹 選んで くれ。\p"
	.string "準備は いいか？』$"

FinalStory_Text_CookFinalLater:
	.string "{JPN}店主『……一旦 下がるか。\p"
	.string "準備が できたら\n"
	.string "ヒトポケルスの 前へ 来い。』$"

FinalStory_Text_CookLostFinal:
	.string "{JPN}店主『くっ……！ まだだ！\p"
	.string "立て直して\n"
	.string "もう一度 行くぞ！』$"

@ ---------------------------------------------------------------------------
@ Text: after the last battle

FinalStory_Text_PokevirusUnravels:
	.string "{JPN}ヒトポケルスの 体を 作る 点が\n"
	.string "ゆらゆらと ほどけ はじめた……$"

@ Darkrai's and Pokevirus's first words are the user's own and kept as written.
FinalStory_Text_DarkraiFirstWords:
	.string "{JPN}ダークライ『ずっと いえなかった。\n"
	.string "わたしは ダークライ。\p"
	.string "はじめまして\n"
	.string "わたしたちから うまれたあなた。』$"

FinalStory_Text_PokevirusFirstWords:
	.string "{JPN}ヒトポケルス『あ あああ は はい \n"
	.string "はじめ・ま・して…… \p"
	.string "わたし は あなたに \n"
	.string "あな たた ち・に …… \p"
	.string "あえ て うれしい で す』$"

FinalStory_Text_CookHeardIt:
	.string "{JPN}店主『……しゃべった……。\n"
	.string "今の…… おれにも 聞こえたぞ。』$"

FinalStory_Text_DarkraiItReached:
	.string "{JPN}ダークライ『ああ。\n"
	.string "……やっと 届いた。』$"

FinalStory_Text_PeopleWaking:
	.string "{JPN}ヒトポケルスの 中から\n"
	.string "声が ひとつ また ひとつ\l"
	.string "遠ざかって いく……\p"
	.string "皆が 目を 覚まし はじめたのだ。$"

FinalStory_Text_PokevirusGlad:
	.string "{JPN}ヒトポケルス『み んな……\n"
	.string "いって しまう……\p"
	.string "で も…… は じめ て……\n"
	.string "だれか と…… は な し た……\p"
	.string "う れ し い…… な』$"

FinalStory_Text_DarkraiFarewell:
	.string "{JPN}ダークライ『……さようなら。\n"
	.string "わたしたちから うまれた あなた。』$"

FinalStory_Text_CookStrangeCreature:
	.string "{JPN}店主『……ウイルスなんて 呼んで\n"
	.string "悪かったな。\p"
	.string "あいつも…… ポケモンと 同じ\n"
	.string "ふしぎな いきもの だったのかも\l"
	.string "しれねえ。』$"

FinalStory_Text_DarkraiBackToHub:
	.string "{JPN}ダークライ『……{PLAYER}。\n"
	.string "ハブへ 戻ろう。\p"
	.string "あの子が…… 目を 覚ます。』$"

@ ---------------------------------------------------------------------------
@ Text: the ending

FinalStory_Text_PecharuntWakes:
	.string "{JPN}モモワロウが 目を 覚ました！$"

@ Pecharunt's question is the user's own and kept as written.
FinalStory_Text_PecharuntAsks:
	.string "{JPN}モモワロウ『ワリャは ワリャは\n"
	.string "わるいこなの？』$"

FinalStory_Text_NoOneAnswers:
	.string "{JPN}…… …… ……\p"
	.string "その 声は 店主にも {PLAYER}にも\n"
	.string "言葉として 届かなかった。\p"
	.string "ダークライには 届いた。\n"
	.string "けれど ダークライは\l"
	.string "何も 答えられなかった。$"

FinalStory_Text_PecharuntStaysByDarkrai:
	.string "{JPN}モモワロウは 答えを もらえない まま\n"
	.string "ダークライの そばに\l"
	.string "小さく 座りこんだ。$"

FinalStory_Text_CookOffersFood:
	.string "{JPN}店主『……腹 へってるだろ。\n"
	.string "だんごでも 食うか。』$"

FinalStory_Text_MomWelcomesBack:
	.string "{JPN}ママ『{PLAYER}！ おかえりなさい。\n"
	.string "……終わったのね。』$"

FinalStory_Text_CookNotOverYet:
	.string "{JPN}店主『いや。 あいつの 意識は\n"
	.string "消えたが 感染した 連中は\l"
	.string "まだ 夢の 中だ。\p"
	.string "治療は これからも\n"
	.string "続けなきゃ ならねえ。』$"

FinalStory_Text_DadWeStay:
	.string "{JPN}パパ『そうか……。\p"
	.string "なら 最後の ひとりが 目を 覚ますまで\n"
	.string "みんなで ここに 残ろう。』$"

FinalStory_Text_MomFamilyTrip:
	.string "{JPN}ママ『せっかくだもの。\n"
	.string "家族旅行だと 思って\l"
	.string "この 世界を みんなで 楽しみましょ！』$"

FinalStory_Text_CookLively:
	.string "{JPN}店主『……ま おれの 店も\n"
	.string "ここに あるしな。\p"
	.string "モモワロウも ダークライも いる。\n"
	.string "にぎやかに なりそうだぜ。』$"

@ ---------------------------------------------------------------------------
@ Around the story: Darkrai in the hub, Pecharunt after waking, Pokevirus after the ending

FinalStory_EventScript_Done::
	end

FinalStory_EventScript_DarkraiGatherHint::
	msgbox FinalStory_Text_DarkraiGatherHint, MSGBOX_DEFAULT
	release
	end

FinalStory_EventScript_DarkraiPathOpen::
	msgbox FinalStory_Text_DarkraiPathOpen, MSGBOX_DEFAULT
	release
	end

FinalStory_EventScript_DarkraiAfterEnding::
	msgbox FinalStory_Text_DarkraiAfterEnding, MSGBOX_DEFAULT
	release
	end

FinalStory_EventScript_PecharuntAwake::
	faceplayer
	playmoncry SPECIES_PECHARUNT, CRY_MODE_NORMAL
	msgbox FinalStory_Text_PecharuntAwake, MSGBOX_DEFAULT
	waitmoncry
	release
	end

@ Once Pokevirus has faded, what is left of it still holds the people not yet released.
FinalStory_EventScript_PokevirusEcho::
	msgbox FinalStory_Text_CookPokevirusEcho, MSGBOX_DEFAULT
	return

FinalStory_Text_DarkraiGatherHint:
	.string "{JPN}ダークライ『……旅館の 方に\n"
	.string "皆の 気配が 集まって いる。\p"
	.string "行って みよう {PLAYER}。』$"

FinalStory_Text_DarkraiPathOpen:
	.string "{JPN}ダークライ『旅館の 浜辺の 裂け目……\n"
	.string "あの 先で ヒトポケルスが 待っている。\p"
	.string "準備が できたら 行こう。』$"

FinalStory_Text_DarkraiAfterEnding:
	.string "{JPN}ダークライ『あの子の 問いに\n"
	.string "わたしは まだ 答えられない。\p"
	.string "それでも そばに いる。\n"
	.string "それだけは 決めている。』$"

FinalStory_Text_PecharuntAwake:
	.string "{JPN}モモワロウは ダークライの そばで\n"
	.string "じっと {PLAYER}を 見つめている。$"

FinalStory_Text_CookPokevirusEcho:
	.string "{JPN}店主『……また 出やがったか。\p"
	.string "あいつの 意識は もう ねえ。\n"
	.string "残った 仕組みが 勝手に\l"
	.string "動いてる だけだ。\p"
	.string "……片付けて やろうぜ {PLAYER}！』$"
