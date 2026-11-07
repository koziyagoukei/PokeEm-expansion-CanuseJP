VermilionCity_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VermilionCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

VermilionCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_VERMILION_CITY
	end

VermilionCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

VermilionCity_PokemonCenter_1F_EventScript_Man::
	msgbox VermilionCity_PokemonCenter_1F_Text_PoisonedMonFaintedWhileWalking, MSGBOX_NPC
	end

VermilionCity_PokemonCenter_1F_EventScript_Hiker::
	msgbox VermilionCity_PokemonCenter_1F_Text_TrainerMonsStrongerThanWild, MSGBOX_NPC
	end

VermilionCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox VermilionCity_PokemonCenter_1F_Text_AllMonWeakToSpecificTypes, MSGBOX_NPC
	end

VermilionCity_PokemonCenter_1F_EventScript_VSSeekerWoman::
	lock
	faceplayer
	goto_if_set FLAG_GOT_VS_SEEKER, VermilionCity_PokemonCenter_1F_EventScript_ExplainVSSeeker
	msgbox VermilionCity_PokemonCenter_1F_Text_UrgeToBattleSomeoneAgain
	setflag FLAG_GOT_VS_SEEKER
	giveitem ITEM_VS_SEEKER
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	msgbox VermilionCity_PokemonCenter_1F_Text_UseDeviceForRematches
	release
	end

VermilionCity_PokemonCenter_1F_EventScript_ExplainVSSeeker::
	msgbox VermilionCity_PokemonCenter_1F_Text_ExplainVSSeeker
	release
	end

VermilionCity_PokemonCenter_1F_Text_TrainerMonsStrongerThanWild::
	.string "{JPN}おなじ レベルの ポケモンでも\n"
	.string "つよさは まちまち らしい\p"
	.string "それに ひとが そだてた\n"
	.string "ポケモンの ほうが\l"
	.string "やせい より つよい らしい$"

VermilionCity_PokemonCenter_1F_Text_PoisonedMonFaintedWhileWalking::
	.string "{JPN}おれの ポケモン\n"
	.string "どく くらったまま あるいてたら\l"
	.string "ちから つき おったで!$"

VermilionCity_PokemonCenter_1F_Text_AllMonWeakToSpecificTypes::
	.string "{JPN}たしかに レベルの たかい\n"
	.string "ポケモンは つよい けど…\p"
	.string "ポケモンの タイプに よって\n"
	.string "にがてな あいてが ある みたい\p"
	.string "ぜったいに つよい ポケモンは\n"
	.string "なかなか いない みたいだ$"

VermilionCity_PokemonCenter_1F_Text_UrgeToBattleSomeoneAgain::
	.string "{JPN}いちど たたかった トレーナーと\n"
	.string "また しょうぶ してみたい!\p"
	.string "…なんて おもうこと ない?\n"
	.string "あるわよね?\p"
	.string "わたし いろんな トレーナーと\n"
	.string "なんども たたかい たくて\l"
	.string "これ くばってるのよ\p"
	.string "もらって もらって!$"

VermilionCity_PokemonCenter_1F_Text_UseDeviceForRematches::
	.string "{JPN}それを つかうと\n"
	.string "もういちど しょうぶ したい\l"
	.string "トレーナーが みつかるの!\p"
	.string "じゅうでん して つかってね!$"

VermilionCity_PokemonCenter_1F_Text_ExplainVSSeeker::
	.string "{JPN}え バトルサーチャーの つかいかた?\n"
	.string "かんたんよ!\p"
	.string "ピピピ!と つかえば\n"
	.string "そばの トレーナーに つたわるの\p"
	.string "もういちど しょうぶ したい\n"
	.string "トレーナーが いれば\l"
	.string "すぐに おしえて くれるわ!\p"
	.string "じゅうでん して\n"
	.string "どうろで つかってね!$"
