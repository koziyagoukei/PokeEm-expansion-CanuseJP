FiveIsland_WaterLabyrinth_Frlg_MapScripts::
	.byte 0

FiveIsland_WaterLabyrinth_EventScript_EggGentleman::
	lock
	faceplayer
	goto_if_set FLAG_GOT_TOGEPI_EGG, FiveIsland_WaterLabyrinth_EventScript_PostEggComment
	goto_if_set FLAG_NO_ROOM_FOR_TOGEPI_EGG, FiveIsland_WaterLabyrinth_EventScript_ReturnForEgg
	msgbox FiveIsland_WaterLabyrinth_Text_LetMeTakeLookAtMons
	specialvar VAR_RESULT, GetLeadMonFriendship
	goto_if_eq VAR_RESULT, 6, FiveIsland_WaterLabyrinth_EventScript_LeadMonMaxFriendship
	msgbox FiveIsland_WaterLabyrinth_Text_HmmISeeIsee
	release
	end

FiveIsland_WaterLabyrinth_EventScript_MonDaisyComment::
	famechecker FAMECHECKER_DAISY, 2
	msgbox FiveIsland_WaterLabyrinth_Text_CuteMonRemindsMeOfDaisy
	release
	end

FiveIsland_WaterLabyrinth_EventScript_LeadMonMaxFriendship::
	msgbox FiveIsland_WaterLabyrinth_Text_TreatMonRightHaveThis
	goto FiveIsland_WaterLabyrinth_EventScript_TryGiveEgg
	end

FiveIsland_WaterLabyrinth_EventScript_TryGiveEgg::
	getpartysize
	goto_if_eq VAR_RESULT, PARTY_SIZE, FiveIsland_WaterLabyrinth_EventScript_NoRoomForEgg
	setflag FLAG_GOT_TOGEPI_EGG
	giveegg SPECIES_TOGEPI
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_LEVEL_UP
	message FiveIsland_WaterLabyrinth_Text_ReceivedEggFromMan
	waitfanfare
	waitmessage
	call EventScript_RestorePrevTextColor
	goto FiveIsland_WaterLabyrinth_EventScript_PostEggComment
	end

@ If the player has a Togepi/Togetic from this game (assumed to be the one the man gave you) then comment on it
@ Otherwise make normal comment
FiveIsland_WaterLabyrinth_EventScript_PostEggComment::
	bufferspeciesname STR_VAR_2, SPECIES_TOGEPI
	setvar VAR_0x8004, SPECIES_TOGEPI
	specialvar VAR_RESULT, PlayerPartyContainsSpeciesWithPlayerID
	goto_if_eq VAR_RESULT, TRUE, FiveIsland_WaterLabyrinth_EventScript_MonDaisyComment
	bufferspeciesname STR_VAR_2, SPECIES_TOGETIC
	setvar VAR_0x8004, SPECIES_TOGETIC
	specialvar VAR_RESULT, PlayerPartyContainsSpeciesWithPlayerID
	goto_if_eq VAR_RESULT, TRUE, FiveIsland_WaterLabyrinth_EventScript_MonDaisyComment
	msgbox FiveIsland_WaterLabyrinth_Text_GladIMetSomeoneLikeYou
	release
	end

FiveIsland_WaterLabyrinth_EventScript_NoRoomForEgg::
	setflag FLAG_NO_ROOM_FOR_TOGEPI_EGG
	msgbox FiveIsland_WaterLabyrinth_Text_DontHaveSpaceInYourParty
	release
	end

FiveIsland_WaterLabyrinth_EventScript_ReturnForEgg::
	goto FiveIsland_WaterLabyrinth_EventScript_TryGiveEgg
	end

FiveIsland_WaterLabyrinth_Text_LetMeTakeLookAtMons::
	.string "{JPN}きみは いろいろな ところを\n"
	.string "たび して まわって\l"
	.string "ポケモンに むちゃは\l"
	.string "させて ないんだろうね…?\p"
	.string "ちょっと みせて ごらん\n"
	.string "……… ………$"

FiveIsland_WaterLabyrinth_Text_HmmISeeIsee::
	.string "{JPN}ほほう\n"
	.string "なるほど なるほど……$"

FiveIsland_WaterLabyrinth_Text_TreatMonRightHaveThis::
	.string "{JPN}おお\n"
	.string "とても ポケモンを\l"
	.string "たいせつに して いるんだね\p"
	.string "きみになら これを\n"
	.string "あずけられると おもう\l"
	.string "どうか もらってくれ$"

FiveIsland_WaterLabyrinth_Text_ReceivedEggFromMan::
	.string "{JPN}{PLAYER}は おじさんに\n"
	.string "タマゴを もらった!$"

FiveIsland_WaterLabyrinth_Text_DontHaveSpaceInYourParty::
	.string "{JPN}とは いっても\n"
	.string "てもちが いっぱいの ようだね\p"
	.string "また こんど おいで$"

@ Unused
FiveIsland_WaterLabyrinth_Text_YouveComeBackForTheEgg::
	.string "やあ\n"
	.string "タマゴを もらいに\l"
	.string "きて くれたんだね$"

FiveIsland_WaterLabyrinth_Text_GladIMetSomeoneLikeYou::
	.string "{JPN}わたしの たびの とちゅうで\n"
	.string "てにいれた タマゴだ\p"
	.string "キミの ような ひとに\n"
	.string "であえて よかったよ$"

FiveIsland_WaterLabyrinth_Text_CuteMonRemindsMeOfDaisy::
	.string "{JPN}おや かわいい {STR_VAR_2}だね\n"
	.string "かわいい ポケモンを みると\l"
	.string "たびで であった\l"
	.string "やさしい しょうじょを おもいだす\p"
	.string "ポケモンに それは やさしく\n"
	.string "けづくろいを して あげていた\l"
	.string "あの しょうじょは…\l"
	.string "たしか ナナミと いったか…$"

FiveIsland_WaterLabyrinth_Text_AlizeIntro::
	.string "{JPN}ポケモンを そだてるのに\n"
	.string "とても よい かんきょうだわ$"

FiveIsland_WaterLabyrinth_Text_AlizeDefeat::
	.string "{JPN}あなたの ポケモン\n"
	.string "よく そだって いるわ!$"

FiveIsland_WaterLabyrinth_Text_AlizePostBattle::
	.string "{JPN}あなた ブリーダーと しての\n"
	.string "さいのうも あるんじゃない?$"
