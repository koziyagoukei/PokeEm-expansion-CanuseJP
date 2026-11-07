PalletTown_PlayersHouse_1F_Frlg_MapScripts::
	.byte 0

PalletTown_PlayersHouse_1F_EventScript_Mom::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_RIVAL_IN_OAKS_LAB, PalletTown_PlayersHouse_1F_EventScript_MomHeal
	checkplayergender
	call_if_eq VAR_RESULT, MALE, PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouMale
	call_if_eq VAR_RESULT, FEMALE, PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouFemale
	closemessage
	applymovement LOCALID_PLAYERS_HOUSE_1F_MOM, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouMale::
	msgbox PalletTown_PlayersHouse_1F_Text_AllBoysLeaveOakLookingForYou
	return

PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouFemale::
	msgbox PalletTown_PlayersHouse_1F_Text_AllGirlsLeaveOakLookingForYou
	return

PalletTown_PlayersHouse_1F_EventScript_MomHeal::
	msgbox PalletTown_PlayersHouse_1F_Text_YouShouldTakeQuickRest
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox PalletTown_PlayersHouse_1F_Text_LookingGreatTakeCare
	release
	end

@ Displays special text if interacted with from side or back (which are normally inaccessible)
PalletTown_PlayersHouse_1F_EventScript_TV::
	lockall
	goto_if_eq VAR_FACING, DIR_NORTH, PalletTown_PlayersHouse_1F_EventScript_TVScreen
	msgbox PalletTown_PlayersHouse_1F_Text_OopsWrongSide
	releaseall
	end

PalletTown_PlayersHouse_1F_EventScript_TVScreen::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, PalletTown_PlayersHouse_1F_EventScript_TVScreenMale
	call_if_eq VAR_RESULT, FEMALE, PalletTown_PlayersHouse_1F_EventScript_TVScreenFemale
	releaseall
	end

PalletTown_PlayersHouse_1F_EventScript_TVScreenMale::
	msgbox PalletTown_PlayersHouse_1F_Text_MovieOnTVFourBoysOnRailroad
	return

PalletTown_PlayersHouse_1F_EventScript_TVScreenFemale::
	msgbox PalletTown_PlayersHouse_1F_Text_MovieOnTVGirlOnBrickRoad
	return

PalletTown_PlayersHouse_1F_Text_AllBoysLeaveOakLookingForYou::
	.string "{JPN}おかあさん『… そうね\n"
	.string "おとこのこは いつか\l"
	.string "たびに でるもの なのよ\l"
	.string "うん… テレビの はなしよ!\p"
	.string "そういえば\n"
	.string "となりの オーキドはかせが\l"
	.string "あなたを よんでたわよ$"

PalletTown_PlayersHouse_1F_Text_AllGirlsLeaveOakLookingForYou::
	.string "{JPN}おかあさん『… そうね\n"
	.string "おんなのこって いつも\l"
	.string "たびを ゆめみて いるものよ\l"
	.string "うん… テレビの はなしよ!\p"
	.string "そういえば\n"
	.string "となりの オーキドはかせが\l"
	.string "あなたを よんでたわよ$"

PalletTown_PlayersHouse_1F_Text_YouShouldTakeQuickRest::
	.string "{JPN}おかあさん『{PLAYER}…!\n"
	.string "すこし やすんで いったら\l"
	.string "どうかしら…?\l"
	.string "… … …$"

PalletTown_PlayersHouse_1F_Text_LookingGreatTakeCare::
	.string "{JPN}おかあさん『あらあら!\n"
	.string "あなたも ポケモンも\l"
	.string "げんき いっぱいね!\l"
	.string "それじゃ きを つけて!\l"
	.string "いってらっしゃい!$"

PalletTown_PlayersHouse_1F_Text_MovieOnTVFourBoysOnRailroad::
	.string "{JPN}テレビで えいがを やってる!\n"
	.string "おとこのこが 4にん\l"
	.string "せんろのうえを あるいてる…\p"
	.string "… ぼくも もう いかなきゃ!$"

PalletTown_PlayersHouse_1F_Text_MovieOnTVGirlOnBrickRoad::
	.string "{JPN}テレビで えいがを やってる!\n"
	.string "おさげの おんなのこが\l"
	.string "れんがの みちを あるいてる…\p"
	.string "…もう いかなきゃ!$"

PalletTown_PlayersHouse_1F_Text_OopsWrongSide::
	.string "{JPN}みえない……$"
