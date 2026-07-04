RustboroCity_Flat1_2F_MapScripts::
	.byte 0

RustboroCity_Flat1_2F_EventScript_WaldasDad::
	lock
	faceplayer
	specialvar VAR_RESULT, TryBufferWaldaPhrase
	goto_if_eq VAR_RESULT, FALSE, RustboroCity_Flat1_2F_EventScript_WaldasDadFirstPhrase
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Flat1_2F_EventScript_WaldasDadNewPhrase

RustboroCity_Flat1_2F_EventScript_GivePhrase::
	special DoWaldaNamingScreen
	goto_if_eq VAR_0x8004, 1, RustboroCity_Flat1_2F_EventScript_CancelGivePhrase
	goto_if_eq VAR_0x8004, 2, RustboroCity_Flat1_2F_EventScript_CancelGiveFirstPhrase
	specialvar VAR_RESULT, TryGetWallpaperWithWaldaPhrase
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Flat1_2F_EventScript_WaldaLikesPhrase
	goto_if_eq VAR_RESULT, FALSE, RustboroCity_Flat1_2F_EventScript_WaldaDoesntLikePhrase
	end

RustboroCity_Flat1_2F_EventScript_WaldasDadFirstPhrase::
	msgbox RustboroCity_Flat1_2F_Text_HelloDoYouKnowFunnyPhrase, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RustboroCity_Flat1_2F_EventScript_DeclineGivePhrase
	msgbox RustboroCity_Flat1_2F_Text_WonderfulLetsHearSuggestion, MSGBOX_DEFAULT
	goto RustboroCity_Flat1_2F_EventScript_GivePhrase

RustboroCity_Flat1_2F_EventScript_WaldasDadNewPhrase::
	msgbox RustboroCity_Flat1_2F_Text_BeenSayingXDoYouKnowBetterPhrase, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RustboroCity_Flat1_2F_EventScript_DeclineGivePhrase
	msgbox RustboroCity_Flat1_2F_Text_WonderfulLetsHearSuggestion, MSGBOX_DEFAULT
	goto RustboroCity_Flat1_2F_EventScript_GivePhrase

RustboroCity_Flat1_2F_EventScript_DeclineGivePhrase::
	msgbox RustboroCity_Flat1_2F_Text_OhIsThatRight, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_CancelGivePhrase::
	msgbox RustboroCity_Flat1_2F_Text_OhYouDontKnowAny, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_CancelGiveFirstPhrase::
	msgbox RustboroCity_Flat1_2F_Text_ThinkOfMyOwnPhrase, MSGBOX_DEFAULT
	call RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda
	msgbox RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll2, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_WaldaLikesPhrase::
	msgbox RustboroCity_Flat1_2F_Text_LetsGiveItATry2, MSGBOX_DEFAULT
	call RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda
	msgbox RustboroCity_Flat1_2F_Text_OhShesLaughing, MSGBOX_DEFAULT
	applymovement LOCALID_WALDAS_DAD, Common_Movement_FacePlayer
	waitmovement 0
	msgbox RustboroCity_Flat1_2F_Text_ThankYouIllGiveYouWallpaper, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_WaldaDoesntLikePhrase::
	msgbox RustboroCity_Flat1_2F_Text_LetsGiveItATry, MSGBOX_DEFAULT
	call RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda
	msgbox RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda::
	turnobject LOCALID_WALDAS_DAD, DIR_EAST
	return

RustboroCity_Flat1_2F_EventScript_WaldasMom::
	msgbox RustboroCity_Flat1_2F_Text_ComingUpWithMealsIsHard, MSGBOX_NPC
	end

RustboroCity_Flat1_2F_EventScript_PokeDoll::
	msgbox RustboroCity_Flat1_2F_Text_ItsAPokemonPlushDoll, MSGBOX_SIGN
	end

RustboroCity_Flat1_2F_Text_ComingUpWithMealsIsHard:
	.string "{JPN}もうね たいへん なのよ!\pなにが たいへんって\nそんなの きまってるでしょ!\pまいにちの おこんだて よッ!\pまいにち まいにち かんがえるのって\nほんとうに たいへん なんだからッ!$"

RustboroCity_Flat1_2F_Text_HelloDoYouKnowFunnyPhrase:
	.string "{JPN}やあ いらっしゃい\nハコガミけ へ ようこそ!\pちょっと しつもんが あるんだけど\nきみ こもり したこと ある?\pおじさん はじめて\nいくじ してるんだけどさ\pうちの アヤノ\nなかなか わらって くれないんだよね\pおもしろーい ことば で\nわらって くれないかなーって\lおもうんだけど\lおもしろーい ことば おしえて くれない?$"

RustboroCity_Flat1_2F_Text_BeenSayingXDoYouKnowBetterPhrase:
	.string "{JPN}いまは {STR_VAR_1} って\nことばを つかってるんだけど\pおもしろそうな ことば\nみつけて きたの?$"

RustboroCity_Flat1_2F_Text_WonderfulLetsHearSuggestion:
	.string "{JPN}そうか そうか\nじゃあ さっそく おしえて くれよ$"

RustboroCity_Flat1_2F_Text_OhIsThatRight:
	.string "{JPN}そうか そうか\nなにか おもしろそうな ことばを\lみつけたら おしえて くれよ$"

RustboroCity_Flat1_2F_Text_LetsGiveItATry2:
	.string "{JPN}なるほどー\nじゃあ ためして みようかな$"

RustboroCity_Flat1_2F_Text_OhShesLaughing:
	.string "{JPN}{STR_VAR_1}\n{STR_VAR_1}\pわらってる わらってる!\nなんだか うれしいなあ$"

RustboroCity_Flat1_2F_Text_LetsGiveItATry:
	.string "{JPN}なるほどー\nじゃあ ためして みようかな$"

RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll:
	.string "{JPN}{STR_VAR_1}\n{STR_VAR_1}\pうーん わらわないなあ\nこのこ クール なのかな$"

RustboroCity_Flat1_2F_Text_ThinkOfMyOwnPhrase:
	.string "{JPN}いい ことば しらないかー\nじゃあ じぶんで かんがえるかー\pうーん\n{STR_VAR_1} とか どうかな\lよし! ためしてみよう$"

RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll2:
	.string "{JPN}{STR_VAR_1}\n{STR_VAR_1}\pうーん わらわないなあ\nこのこ クール なのかな$"

RustboroCity_Flat1_2F_Text_OhYouDontKnowAny:
	.string "{JPN}いい ことば しらないかー\nじゃあ まえの ことばで\lがんばって あやすかー\pおもしろそうな ことば みつけたら\nおじさんに おしえてね$"

RustboroCity_Flat1_2F_Text_ThankYouIllGiveYouWallpaper:
	.string "{JPN}ありがとう!\pきみの おかげで アヤノが\nわらって くれたよ!\pじつは おじさん\nこう みえても\pデボンコーポレーションの\nすごうで けんきゅういん なんだよ\pそうだなー じゃあ おれいに\nきみが ポケモンを あずけている\lボックスの かべがみを\lふやして あげよう!\pかべがみの なかから\n「だいすき」って いうのを えらぶと\lあたらしい かべがみが えらべるよ!$"

RustboroCity_Flat1_2F_Text_ItsAPokemonPlushDoll:
	.string "{JPN}ポケモンの ぬいぐるみ だ!$"

