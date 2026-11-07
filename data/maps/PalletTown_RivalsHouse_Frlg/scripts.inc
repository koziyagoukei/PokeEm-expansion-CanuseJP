.equ RECEIVED_TOWN_MAP, VAR_TEMP_1

PalletTown_RivalsHouse_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PalletTown_RivalsHouse_OnTransition
	.byte 0

PalletTown_RivalsHouse_OnTransition::
	call_if_lt VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, PalletTown_RivalsHouse_EventScript_MoveDaisyToTable
	call_if_ge VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, PalletTown_RivalsHouse_EventScript_AlreadyReceivedTownMap
	end

PalletTown_RivalsHouse_EventScript_MoveDaisyToTable::
	setobjectmovementtype LOCALID_DAISY, MOVEMENT_TYPE_FACE_RIGHT
	setobjectxyperm LOCALID_DAISY, 5, 4
	return

PalletTown_RivalsHouse_EventScript_AlreadyReceivedTownMap::
	setvar RECEIVED_TOWN_MAP, TRUE
	return

PalletTown_RivalsHouse_EventScript_Daisy::
	lock
	faceplayer
	famechecker FAMECHECKER_DAISY, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	goto_if_set FLAG_SYS_GAME_CLEAR, PalletTown_RivalsHouse_EventScript_GroomMon
	goto_if_eq RECEIVED_TOWN_MAP, TRUE, PalletTown_RivalsHouse_EventScript_PleaseGiveMonsRest
	goto_if_eq VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, PalletTown_RivalsHouse_EventScript_ExplainTownMap
	goto_if_eq VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 1, PalletTown_RivalsHouse_EventScript_GiveTownMap
	goto_if_ge VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 1, PalletTown_RivalsHouse_EventScript_HeardBattledRival
	msgbox PalletTown_RivalsHouse_Text_HiBrothersAtLab
	closemessage
	applymovement LOCALID_DAISY, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

PalletTown_RivalsHouse_EventScript_HeardBattledRival::
	msgbox PalletTown_RivalsHouse_Text_HeardYouBattledRival
	release
	end

PalletTown_RivalsHouse_EventScript_GroomMon::
	goto_if_lt VAR_MASSAGE_COOLDOWN_STEP_COUNTER, 500, PalletTown_RivalsHouse_EventScript_RateMonFriendship
	msgbox PalletTown_RivalsHouse_Text_LikeMeToGroomMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PalletTown_RivalsHouse_EventScript_DeclineGrooming
	msgbox PalletTown_RivalsHouse_Text_GroomWhichOne
	special ChoosePartyMon
	lock
	faceplayer
	goto_if_ge VAR_0x8004, PARTY_SIZE, PalletTown_RivalsHouse_EventScript_DeclineGrooming
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, PalletTown_RivalsHouse_EventScript_CantGroomEgg
	msgbox PalletTown_RivalsHouse_Text_LookingNiceInNoTime
	closemessage
	fadescreen FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	special DaisyMassageServices
	fadescreen FADE_FROM_BLACK
	special BufferMonNickname
	msgbox PalletTown_RivalsHouse_Text_ThereYouGoAllDone
	release
	end

PalletTown_RivalsHouse_EventScript_CantGroomEgg::
	msgbox PalletTown_RivalsHouse_Text_CantGroomAnEgg
	release
	end

PalletTown_RivalsHouse_EventScript_DeclineGrooming::
	msgbox PalletTown_RivalsHouse_Text_DontNeedAnyGrooming
	release
	end

PalletTown_RivalsHouse_EventScript_RateMonFriendship::
	msgbox PalletTown_RivalsHouse_Text_MayISeeFirstMon
	specialvar VAR_RESULT, GetLeadMonFriendship
	switch VAR_RESULT
	case 0, PalletTown_RivalsHouse_EventScript_MonFriendshipLowest
	case 1, PalletTown_RivalsHouse_EventScript_MonFriendshipLower
	case 2, PalletTown_RivalsHouse_EventScript_MonFriendshipLow
	case 3, PalletTown_RivalsHouse_EventScript_MonFriendshipMid
	case 4, PalletTown_RivalsHouse_EventScript_MonFriendshipHigh
	case 5, PalletTown_RivalsHouse_EventScript_MonFriendshipHigher
	case 6, PalletTown_RivalsHouse_EventScript_MonFriendshipHighest
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipLowest::
	msgbox PalletTown_RivalsHouse_Text_WhyWouldMonHateYouSoMuch
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipLower::
	msgbox PalletTown_RivalsHouse_Text_DontLikeWayItGlaresAtYou
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipLow::
	msgbox PalletTown_RivalsHouse_Text_NotFamiliarWithYouYet
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipMid::
	msgbox PalletTown_RivalsHouse_Text_ItsWarmingUpToYou
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipHigh::
	msgbox PalletTown_RivalsHouse_Text_ItsQuiteFriendly
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipHigher::
	msgbox PalletTown_RivalsHouse_Text_ItLooksVeryHappy
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipHighest::
	msgbox PalletTown_RivalsHouse_Text_CouldntLoveYouMore
	release
	end

PalletTown_RivalsHouse_EventScript_PleaseGiveMonsRest::
	msgbox PalletTown_RivalsHouse_Text_PleaseGiveMonsRest
	release
	end

PalletTown_RivalsHouse_EventScript_GiveTownMap::
	msgbox PalletTown_RivalsHouse_Text_ErrandForGrandpaThisWillHelp
	closemessage
	checkitemspace ITEM_TOWN_MAP
	goto_if_eq VAR_RESULT, FALSE, PalletTown_RivalsHouse_EventScript_NoRoomForTownMap
	applymovement LOCALID_DAISY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	removeobject LOCALID_TOWN_MAP
	setvar VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2
	delay 15
	applymovement LOCALID_DAISY, Common_Movement_FacePlayer
	waitmovement 0
	delay 12
	giveitem_msg PalletTown_RivalsHouse_Text_ReceivedTownMapFromDaisy, ITEM_TOWN_MAP, 1, MUS_RG_OBTAIN_KEY_ITEM
	release
	end

PalletTown_RivalsHouse_EventScript_NoRoomForTownMap::
	msgbox PalletTown_RivalsHouse_Text_DontHaveSpaceForThis
	release
	end

PalletTown_RivalsHouse_EventScript_ExplainTownMap::
	msgbox PalletTown_RivalsHouse_Text_ExplainTownMap
	release
	end

PalletTown_RivalsHouse_EventScript_TownMap::
	msgbox PalletTown_RivalsHouse_Text_ItsBigMapOfKanto, MSGBOX_NPC
	end

PalletTown_RivalsHouse_EventScript_Bookshelf::
	msgbox PalletTown_RivalsHouse_Text_ShelvesCrammedFullOfBooks, MSGBOX_SIGN
	end

PalletTown_RivalsHouse_EventScript_Picture::
	msgbox PalletTown_RivalsHouse_Text_LovelyAndSweetClefairy, MSGBOX_SIGN
	end

PalletTown_RivalsHouse_Text_HiBrothersAtLab::
	.string "{JPN}ナナミ『こんにちは\n"
	.string "{PLAYER}{KUN}!\p"
	.string "おとうとの {RIVAL}なら\n"
	.string "おじいちゃんの けんきゅうじょよ$"

PalletTown_RivalsHouse_Text_HeardYouBattledRival::
	.string "{JPN}ナナミ『{PLAYER}{KUN}\n"
	.string "{RIVAL}と\l"
	.string "しょうぶ したんですってね\p"
	.string "みたかったわ!$"

PalletTown_RivalsHouse_Text_ErrandForGrandpaThisWillHelp::
	.string "{JPN}オーキドの おじいちゃん\n"
	.string "おしごと たのんだんだって?\l"
	.string "たいへんねー\l"
	.string "これ あげるから つかって!$"

PalletTown_RivalsHouse_Text_ReceivedTownMapFromDaisy::
	.string "{JPN}{PLAYER}は ナナミから\n"
	.string "タウンマップを もらった!$"

PalletTown_RivalsHouse_Text_DontHaveSpaceForThis::
	.string "{JPN}にもつ いっぱいよ$"

PalletTown_RivalsHouse_Text_ExplainTownMap::
	.string "{JPN}じぶんの いる ばしょや\n"
	.string "まちの なまえが しりたいとき\l"
	.string "タウンマップ つかうと いいわよ$"

PalletTown_RivalsHouse_Text_PleaseGiveMonsRest::
	.string "{JPN}ナナミ『にんげんと おなじように\n"
	.string "ポケモンも いきてるの!\l"
	.string "たいりょくが ないときは\l"
	.string "やすませて あげて!$"

PalletTown_RivalsHouse_Text_ItsBigMapOfKanto::
	.string "{JPN}カントー ちほうの ちずだ!\n"
	.string "…… もらえたら うれしい?$"

PalletTown_RivalsHouse_Text_ShelvesCrammedFullOfBooks::
	.string "{JPN}ほんだなに ならんでいるのは\n"
	.string "ポケモンの ほん ばかりだ$"

PalletTown_RivalsHouse_Text_LikeMeToGroomMon::
	.string "{JPN}ナナミ『こんにちは {PLAYER}{KUN}\n"
	.string "ちょうど よかったわ!\l"
	.string "これから おちゃの じかんなの\p"
	.string "あら? あなたの ポケモン\n"
	.string "すこし よごれちゃってる みたい!\l"
	.string "けづくろい しましょうか?$"

PalletTown_RivalsHouse_Text_DontNeedAnyGrooming::
	.string "{JPN}けづくろい しないの?\n"
	.string "じゃあ おちゃに しましょう$"

PalletTown_RivalsHouse_Text_GroomWhichOne::
	.string "{JPN}どの ポケモンに する?$"

PalletTown_RivalsHouse_Text_LookingNiceInNoTime::
	.string "{JPN}ナナミ『じゃあ\n"
	.string "けづくろい しましょうね!$"

PalletTown_RivalsHouse_Text_ThereYouGoAllDone::
#ifdef BUGFIX @ The localizers missed what should be a textcolor change in the localizations.
	.string "{JPN}{COLOR DARK_GRAY}{STR_VAR_1}は\n"
	.string "とても きもちよさそう!\p"
	.string "{COLOR RED}ナナミ『はい!\n"
#else @ In the JP games, gender-based text used a different font instead of different colors.
	.string "{JPN}{FONT_NORMAL}{STR_VAR_1}は\n"
	.string "とても きもちよさそう!\p"
	.string "{FONT_FEMALE}ナナミ『はい!\n"
#endif
	.string "とっても きれいに なったわ!\p"
	.string "うふふ\n"
	.string "かわいい ポケモンね!$"

PalletTown_RivalsHouse_Text_CantGroomAnEgg::
	.string "{JPN}んー…\n"
	.string "タマゴは けづくろい できないわ$"

PalletTown_RivalsHouse_Text_MayISeeFirstMon::
	.string "{JPN}ナナミ『ポケモンも\n"
	.string "たいせつに そだてて いると\l"
	.string "なついて くれるのよ\p"
	.string "{PLAYER}{KUN}の ポケモンは……$"

PalletTown_RivalsHouse_Text_CouldntLoveYouMore::
	.string "{JPN}これいじょうは ない くらい\n"
	.string "なついて いるのね…!\l"
	.string "あなたの ポケモンは しあわせね!$"

PalletTown_RivalsHouse_Text_ItLooksVeryHappy::
	.string "{JPN}しあわせそうに みえるわ\n"
	.string "{RIVAL}にも\l"
	.string "みならって ほしい くらいね$"

PalletTown_RivalsHouse_Text_ItsQuiteFriendly::
	.string "{JPN}なかなか なついて いるのね\n"
	.string "その ちょうしで かわいがってね$"

PalletTown_RivalsHouse_Text_ItsWarmingUpToYou::
	.string "{JPN}すこし なついてきた のね\n"
	.string "しんらいが うまれて きたみたい$"

PalletTown_RivalsHouse_Text_NotFamiliarWithYouYet::
	.string "{JPN}まだ あまり なれていない のね\n"
	.string "であった ときは\l"
	.string "だれでも このくらい なのよ$"

PalletTown_RivalsHouse_Text_DontLikeWayItGlaresAtYou::
	.string "{JPN}{PLAYER}{KUN}の こと\n"
	.string "すこし にらんで いるみたい…\l"
	.string "もうすこし かわいがって\l"
	.string "あげられない かしら?$"

PalletTown_RivalsHouse_Text_WhyWouldMonHateYouSoMuch::
	.string "{JPN}……あの いいづらいん だけど\p"
	.string "ポケモンに そんなに\n"
	.string "きらわれている のは\l"
	.string "なにか わけが あるのかしら……$"

PalletTown_RivalsHouse_Text_LovelyAndSweetClefairy::
	.string "{JPN}『ラブリー スウィート\n"
	.string "ピッピ ちゃん』$"

