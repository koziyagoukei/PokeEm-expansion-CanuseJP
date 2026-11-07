.set SPOKE_TO_WOMAN_LAST,   FLAG_TEMP_2
.set SPOKE_TO_FAT_MAN_LAST, FLAG_TEMP_3

VermilionCity_PokemonFanClub_Frlg_MapScripts::
	.byte 0

VermilionCity_PokemonFanClub_EventScript_Chairman::
	lock
	faceplayer
	goto_if_set FLAG_GOT_BIKE_VOUCHER, VermilionCity_PokemonFanClub_EventScript_AlreadyHeardStory
	msgbox VermilionCity_PokemonFanClub_Text_DidYouComeToHearAboutMyMons, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, VermilionCity_PokemonFanClub_EventScript_ChairmanStory
	msgbox VermilionCity_PokemonFanClub_Text_ComeBackToHearStory
	release
	end

VermilionCity_PokemonFanClub_EventScript_AlreadyHeardStory::
	msgbox VermilionCity_PokemonFanClub_Text_DidntComeToSeeAboutMonsAgain
	release
	end

VermilionCity_PokemonFanClub_EventScript_ChairmanStory::
	msgbox VermilionCity_PokemonFanClub_Text_ChairmansStory
	checkitemspace ITEM_BIKE_VOUCHER
	goto_if_eq VAR_RESULT, FALSE, VermilionCity_PokemonFanClub_EventScript_NoRoomForBikeVoucher
	setflag FLAG_GOT_BIKE_VOUCHER
	giveitem_msg VermilionCity_PokemonFanClub_Text_ReceivedBikeVoucherFromChairman, ITEM_BIKE_VOUCHER, 1, MUS_RG_OBTAIN_KEY_ITEM
	msgbox VermilionCity_PokemonFanClub_Text_ExplainBikeVoucher
	release
	end

VermilionCity_PokemonFanClub_EventScript_NoRoomForBikeVoucher::
	msgbox VermilionCity_PokemonFanClub_Text_MakeRoomForThis
	release
	end

VermilionCity_PokemonFanClub_EventScript_WorkerF::
	lock
	faceplayer
	goto_if_set FLAG_SYS_GAME_CLEAR, VermilionCity_PokemonFanClub_EventScript_WorkerFGameClear
	msgbox VermilionCity_PokemonFanClub_Text_ChairmanVeryVocalAboutPokemon
	release
	end

VermilionCity_PokemonFanClub_EventScript_WorkerFGameClear::
	famechecker FAMECHECKER_DAISY, 1
	msgbox VermilionCity_PokemonFanClub_Text_ChairmanReallyAdoresHisMons
	release
	end

VermilionCity_PokemonFanClub_EventScript_Woman::
	lock
	faceplayer
	goto_if_set SPOKE_TO_FAT_MAN_LAST, VermilionCity_PokemonFanClub_EventScript_WomanSpokeToFatMan
	msgbox VermilionCity_PokemonFanClub_Text_AdoreMySeel
	closemessage
	applymovement LOCALID_POKEMON_FAN_CLUB_WOMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag SPOKE_TO_WOMAN_LAST
	release
	end

VermilionCity_PokemonFanClub_EventScript_WomanSpokeToFatMan::
	msgbox VermilionCity_PokemonFanClub_Text_SeelFarMoreAttractive
	closemessage
	applymovement LOCALID_POKEMON_FAN_CLUB_WOMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	clearflag SPOKE_TO_FAT_MAN_LAST
	release
	end

VermilionCity_PokemonFanClub_EventScript_FatMan::
	lock
	faceplayer
	goto_if_set SPOKE_TO_WOMAN_LAST, VermilionCity_PokemonFanClub_EventScript_FatManSpokeToWoman
	msgbox VermilionCity_PokemonFanClub_Text_AdmirePikachusTail
	closemessage
	applymovement LOCALID_POKEMON_FAN_CLUB_FAT_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag SPOKE_TO_FAT_MAN_LAST
	release
	end

VermilionCity_PokemonFanClub_EventScript_FatManSpokeToWoman::
	msgbox VermilionCity_PokemonFanClub_Text_PikachuTwiceAsCute
	closemessage
	applymovement LOCALID_POKEMON_FAN_CLUB_FAT_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	clearflag SPOKE_TO_WOMAN_LAST
	release
	end

VermilionCity_PokemonFanClub_EventScript_Pikachu::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIKACHU, CRY_MODE_NORMAL
	msgbox VermilionCity_PokemonFanClub_Text_Pikachu
	waitmoncry
	release
	end

VermilionCity_PokemonFanClub_EventScript_Seel::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SEEL, CRY_MODE_NORMAL
	msgbox VermilionCity_PokemonFanClub_Text_Seel
	waitmoncry
	release
	end

VermilionCity_PokemonFanClub_EventScript_RulesSign1::
	msgbox VermilionCity_PokemonFanClub_Text_ListenPolitelyToOtherTrainers, MSGBOX_SIGN
	end

VermilionCity_PokemonFanClub_EventScript_RulesSign2::
	msgbox VermilionCity_PokemonFanClub_Text_SomeoneBragsBragBack, MSGBOX_SIGN
	end

VermilionCity_PokemonFanClub_Text_AdmirePikachusTail::
	.string "{JPN}みてよ! うちの ピカチュウ\n"
	.string "なんて かわいい しっぽ!$"

VermilionCity_PokemonFanClub_Text_PikachuTwiceAsCute::
	.string "{JPN}なにを!\p"
	.string "うちの ピカチュウの ほうが\n"
	.string "その ばいは かわいい わい!$"

VermilionCity_PokemonFanClub_Text_AdoreMySeel::
	.string "{JPN}あたしの いとしい パウワウ!\p"
	.string "だきしめると\n"
	.string "きゅー! って なくのよ!$"

VermilionCity_PokemonFanClub_Text_SeelFarMoreAttractive::
	.string "{JPN}おほほ!\p"
	.string "あたしの パウワウの ほうが\n"
	.string "その ばいは かわいいわよ!$"

VermilionCity_PokemonFanClub_Text_Pikachu::
	.string "{JPN}ピカチュウ『ちゅー ぴかちゅ$"

VermilionCity_PokemonFanClub_Text_Seel::
	.string "{JPN}パウワウ『きゅー$"

VermilionCity_PokemonFanClub_Text_DidYouComeToHearAboutMyMons::
	.string "{JPN}ポケモン だいすき クラブの\n"
	.string "かいちょうは わし じゃ!\p"
	.string "かってる ポケモンは\n"
	.string "100ぴきを こえとる!\p"
	.string "ポケモンに かんしては\n"
	.string "ホント うるさい! ですぞ!\p"
	.string "で…\p"
	.string "きみは わしの ポケモン じまんを\n"
	.string "ききに きたの かね?$"

VermilionCity_PokemonFanClub_Text_ChairmansStory::
	.string "{JPN}そうか!\n"
	.string "では さっそく はじめるか!\p"
	.string "あのな… わしの おきにいりの\n"
	.string "ギャロップがな… …\p"
	.string "… でな… が…\l"
	.string "… … かわいくてな…\l"
	.string "たまらん… くう…\l"
	.string "… さらに… もう…\l"
	.string "すごすぎ… … で…\l"
	.string "… そう おもうか…\l"
	.string "どうして… … すき…\l"
	.string "… はー!\p"
	.string "… … だきしめて…\l"
	.string "ねるときも…\l"
	.string "… じゃろ… …\l"
	.string "… … すばらし…!\l"
	.string "… うつくし…\l"
	.string "… … ありゃ!\l"
	.string "もう こんな じかんか!\l"
	.string "ちょっと しゃべり すぎたわい\p"
	.string "わしの ポケモン じまんを\n"
	.string "おとなしく きいてた おれいに\l"
	.string "…これは きもち じゃ!$"

VermilionCity_PokemonFanClub_Text_ReceivedBikeVoucherFromChairman::
	.string "{JPN}{PLAYER}は かいちょうから\n"
	.string "ひきかえけんを もらった!$"

VermilionCity_PokemonFanClub_Text_ExplainBikeVoucher::
	.string "{JPN}その ひきかえけんを\n"
	.string "ハナダシティの\l"
	.string "ミラクルサイクルに もっていくと\l"
	.string "タダで じてんしゃが もらえる!\p"
	.string "なに わしには おきにいりの\n"
	.string "とりポケモン オニドリルが おる\l"
	.string "いきたい ところへ ひとっとびじゃ\p"
	.string "だから じてんしゃ など いらん!\p"
	.string "じてんしゃは きみが\n"
	.string "すきに のったら ええ!$"

VermilionCity_PokemonFanClub_Text_ComeBackToHearStory::
	.string "{JPN}なんじゃ… つまらん\n"
	.string "きく きに なったら きてくれい!$"

VermilionCity_PokemonFanClub_Text_DidntComeToSeeAboutMonsAgain::
	.string "{JPN}やー やー {PLAYER}{KUN}!\p"
	.string "また わしの ポケモン じまんを\n"
	.string "ききに きたの かね!\p"
	.string "… え ちがう?\n"
	.string "なんじゃ… つまらん$"

VermilionCity_PokemonFanClub_Text_MakeRoomForThis::
	.string "{JPN}にもつが いっぱい じゃ!$"

VermilionCity_PokemonFanClub_Text_ChairmanVeryVocalAboutPokemon::
	.string "{JPN}うちの かいちょうは\n"
	.string "ホント ポケモンに うるさいの!$"

VermilionCity_PokemonFanClub_Text_ListenPolitelyToOtherTrainers::
	.string "{JPN}かいぬしの じまん ばなしには\n"
	.string "しずかに みみを かたむけよう!$"

VermilionCity_PokemonFanClub_Text_SomeoneBragsBragBack::
	.string "{JPN}ひとの じまん ばなしは\n"
	.string "10ばい にして かえそう!$"

VermilionCity_PokemonFanClub_Text_ChairmanReallyAdoresHisMons::
	.string "{JPN}うちの かいちょうは ほんとうに\n"
	.string "ポケモンが だいすきだけど…\p"
	.string "ポケモンに いちばん\n"
	.string "すかれている ひとと いえば\l"
	.string "やっぱり ナナミさんかしら!$"
