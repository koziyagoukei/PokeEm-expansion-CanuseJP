CeladonCity_Condominiums_RoofRoom_Frlg_MapScripts::
	.byte 0

CeladonCity_Condominiums_RoofRoom_EventScript_BlackBelt::
	msgbox CeladonCity_Condominiums_RoofRoom_Text_TheresNothingIDontKnow, MSGBOX_NPC
	end

CeladonCity_Condominiums_RoofRoom_EventScript_EeveeBall::
	lock
	faceplayer
	setvar VAR_TEMP_1, SPECIES_EEVEE
	givemon SPECIES_EEVEE, 25
	goto_if_eq VAR_RESULT, 0, CeladonCity_Condominiums_RoofRoom_EventScript_GetEeveeParty
	goto_if_eq VAR_RESULT, 1, CeladonCity_Condominiums_RoofRoom_EventScript_GetEeveePC
	goto_if_eq VAR_RESULT, 2, Common_EventScript_NoMoreRoomForPokemon
	release
	end

CeladonCity_Condominiums_RoofRoom_EventScript_GetEeveeParty::
	removeobject LOCALID_EEVEE_POKEBALL
	playfanfare MUS_LEVEL_UP
	message CeladonCity_Condominiums_RoofRoom_Text_ObtainedAnEevee
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_EEVEE
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeladonCity_Condominiums_RoofRoom_EventScript_SetGotEevee
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto CeladonCity_Condominiums_RoofRoom_EventScript_SetGotEevee
	end

CeladonCity_Condominiums_RoofRoom_EventScript_GetEeveePC::
	removeobject LOCALID_EEVEE_POKEBALL
	playfanfare MUS_LEVEL_UP
	message CeladonCity_Condominiums_RoofRoom_Text_ObtainedAnEevee
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_EEVEE
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeladonCity_Condominiums_RoofRoom_EventScript_TransferEeveeToPC
	call Common_EventScript_NameReceivedBoxMon
	goto CeladonCity_Condominiums_RoofRoom_EventScript_TransferEeveeToPC
	end

CeladonCity_Condominiums_RoofRoom_EventScript_TransferEeveeToPC::
	call Common_EventScript_TransferredToPC
	goto CeladonCity_Condominiums_RoofRoom_EventScript_SetGotEevee
	end

CeladonCity_Condominiums_RoofRoom_EventScript_SetGotEevee::
	setflag FLAG_GOT_EEVEE
	release
	end

CeladonCity_Condominiums_RoofRoom_EventScript_TMsPamphlet::
	msgbox CeladonCity_Condominiums_RoofRoom_Text_PamphletOnTMs, MSGBOX_SIGN
	end

CeladonCity_Condominiums_RoofRoom_EventScript_Blackboard::
	lockall
	msgbox CeladonCity_Condominiums_RoofRoom_Text_WirelessAdapterLecture
	message CeladonCity_Condominiums_RoofRoom_Text_ReadWhichHeading
	waitmessage
	setvar VAR_0x8004, 4
	multichoice 0, 0, MULTI_LINKED_DIRECT_UNION, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_Condominiums_RoofRoom_EventScript_WirelessClub
	case 1, CeladonCity_Condominiums_RoofRoom_EventScript_DirectCorner
	case 2, CeladonCity_Condominiums_RoofRoom_EventScript_UnionRoom
	case 3, CeladonCity_Condominiums_RoofRoom_EventScript_ExitBlackboard
	case 127, CeladonCity_Condominiums_RoofRoom_EventScript_ExitBlackboard
	end

CeladonCity_Condominiums_RoofRoom_EventScript_ReadAnotherHeading::
	message CeladonCity_Condominiums_RoofRoom_Text_ReadWhichHeading
	waitmessage
	multichoice 0, 0, MULTI_LINKED_DIRECT_UNION, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_Condominiums_RoofRoom_EventScript_WirelessClub
	case 1, CeladonCity_Condominiums_RoofRoom_EventScript_DirectCorner
	case 2, CeladonCity_Condominiums_RoofRoom_EventScript_UnionRoom
	case 3, CeladonCity_Condominiums_RoofRoom_EventScript_ExitBlackboard
	case 127, CeladonCity_Condominiums_RoofRoom_EventScript_ExitBlackboard
	end

CeladonCity_Condominiums_RoofRoom_EventScript_WirelessClub::
	msgbox CeladonCity_Condominiums_RoofRoom_Text_ExplainWirelessClub
	goto CeladonCity_Condominiums_RoofRoom_EventScript_ReadAnotherHeading
	end

CeladonCity_Condominiums_RoofRoom_EventScript_DirectCorner::
	msgbox CeladonCity_Condominiums_RoofRoom_Text_ExplainDirectCorner
	goto CeladonCity_Condominiums_RoofRoom_EventScript_ReadAnotherHeading
	end

CeladonCity_Condominiums_RoofRoom_EventScript_UnionRoom::
	msgbox CeladonCity_Condominiums_RoofRoom_Text_ExplainUnionRoom
	goto CeladonCity_Condominiums_RoofRoom_EventScript_ReadAnotherHeading
	end

CeladonCity_Condominiums_RoofRoom_EventScript_ExitBlackboard::
	releaseall
	end

CeladonCity_Condominiums_RoofRoom_Text_TheresNothingIDontKnow::
	.string "{JPN}こくばんにも かいたが…\n"
	.string "わしに わからぬ ことは ない\l"
	.string "ゲームボーイの なかに\l"
	.string "せかいが あることも!\p"
	.string "ぜひ! ともだちを さそって\n"
	.string "ポケモンの こうかんを\l"
	.string "たのしんで くれたまえ!$"

CeladonCity_Condominiums_RoofRoom_Text_ObtainedAnEevee::
	.string "{JPN}{PLAYER}は\n"
	.string "イーブイを てにいれた!$"

CeladonCity_Condominiums_RoofRoom_Text_BoxIsFull::
	.string "ポケモンが いっぱいだ\n"
	.string "ボックスを かえて きなさい$"

CeladonCity_Condominiums_RoofRoom_Text_WirelessAdapterLecture::
	.string "{JPN}… ポケモン こうざ!\p"
	.string "ワイヤレスアダプタを つかった\n"
	.string "あそびかたに ついて$"

CeladonCity_Condominiums_RoofRoom_Text_ReadWhichHeading::
	.string "{JPN}どの こうもくを よみますか?$"

CeladonCity_Condominiums_RoofRoom_Text_ExplainWirelessClub::
	.string "{JPN}ポケモンセンターの 2かいは\n"
	.string "ポケモン ワイヤレス クラブ!\p"
	.string "ともだちと いっしょに\n"
	.string "ワイヤレスアダプタを つないで\l"
	.string "つうしんが たのしめます$"

CeladonCity_Condominiums_RoofRoom_Text_ExplainDirectCorner::
	.string "{JPN}きまった ともだちと\n"
	.string "つうしんが したい ときは\l"
	.string "ポケモン ワイヤレス クラブの\l"
	.string "みぎがわの カウンター\l"
	.string "ダイレクト コーナーに いって\l"
	.string "トレードコーナーや コロシアムなど\l"
	.string "いきたい へやを えらんで\l"
	.string "いっしょに はいりましょう!$"

CeladonCity_Condominiums_RoofRoom_Text_ExplainUnionRoom::
	.string "{JPN}みしらぬ ともだちと\n"
	.string "コミュニケーション したい ときは\l"
	.string "ポケモン ワイヤレス クラブの\l"
	.string "ひだりがわの カウンター\l"
	.string "ユニオン ルームに いって\l"
	.string "いろいろな トレーナーと\l"
	.string "であいを たのしみましょう!$"

CeladonCity_Condominiums_RoofRoom_Text_PamphletOnTMs::
	.string "{JPN}わざマシンの パンフレットだ!\n"
	.string "… …\p"
	.string "わざマシンは\n"
	.string "ぜんぶで 50しゅるい!\p"
	.string "ひでんマシンは\n"
	.string "つかっても なくならない タイプ\l"
	.string "ぜんぶで 7しゅるい あります\p"
	.string "… シルフ カンパニー$"

