MauvilleCity_BikeShop_MapScripts::
	.byte 0

MauvilleCity_BikeShop_EventScript_Rydel::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_BIKE, MauvilleCity_BikeShop_EventScript_AskSwitchBikes
	goto_if_set FLAG_DECLINED_BIKE, MauvilleCity_BikeShop_EventScript_SkipGreeting
	msgbox MauvilleCity_BikeShop_Text_RydelGreeting, MSGBOX_DEFAULT
	msgbox MauvilleCity_BikeShop_Text_DidYouComeFromFarAway, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_BikeShop_EventScript_YesFar
	goto_if_eq VAR_RESULT, NO, MauvilleCity_BikeShop_EventScript_NotFar
	end

MauvilleCity_BikeShop_EventScript_SkipGreeting::
	msgbox MauvilleCity_BikeShop_Text_DidYouComeFromFarAway, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_BikeShop_EventScript_YesFar
	goto_if_eq VAR_RESULT, NO, MauvilleCity_BikeShop_EventScript_NotFar
	end

MauvilleCity_BikeShop_EventScript_ChooseBike::
	message MauvilleCity_BikeShop_Text_ExplainBikesChooseWhichOne
	waitmessage
	multichoice 21, 8, MULTI_BIKE, TRUE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_GetMachBike
	case 1, MauvilleCity_BikeShop_EventScript_GetAcroBike
	end

MauvilleCity_BikeShop_EventScript_NotFar::
	setflag FLAG_DECLINED_BIKE
	msgbox MauvilleCity_BikeShop_Text_GuessYouDontNeedBike, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_EventScript_YesFar::
	setflag FLAG_RECEIVED_BIKE
	goto MauvilleCity_BikeShop_EventScript_ChooseBike
	end

MauvilleCity_BikeShop_EventScript_GetMachBike::
	msgbox MauvilleCity_BikeShop_Text_ChoseMachBike, MSGBOX_DEFAULT
	giveitem ITEM_MACH_BIKE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_GetAcroBike::
	msgbox MauvilleCity_BikeShop_Text_ChoseAcroBike, MSGBOX_DEFAULT
	giveitem ITEM_ACRO_BIKE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes::
	msgbox MauvilleCity_BikeShop_Text_ComeBackToSwitchBikes, MSGBOX_DEFAULT
	special SwapRegisteredBike
	release
	end

MauvilleCity_BikeShop_EventScript_AskSwitchBikes::
	msgbox MauvilleCity_BikeShop_Text_WantToSwitchBikes, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_BikeShop_EventScript_SwitchBikes
	goto_if_eq VAR_RESULT, NO, MauvilleCity_BikeShop_EventScript_KeepBike
	end

@ If the player does not have a bike on them Rydel assumes its stored in the PC
MauvilleCity_BikeShop_EventScript_SwitchBikes::
	msgbox MauvilleCity_BikeShop_Text_IllSwitchBikes, MSGBOX_DEFAULT
	checkitem ITEM_ACRO_BIKE
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_BikeShop_EventScript_SwitchAcroForMach
	checkitem ITEM_MACH_BIKE
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_BikeShop_EventScript_SwitchMachForAcro
	msgbox MauvilleCity_BikeShop_Text_OhYourBikeIsInPC, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_EventScript_KeepBike::
	msgbox MauvilleCity_BikeShop_Text_HappyYouLikeIt, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_EventScript_SwitchAcroForMach::
	incrementgamestat GAME_STAT_TRADED_BIKES
	msgbox MauvilleCity_BikeShop_Text_ExchangedAcroForMach, MSGBOX_DEFAULT
	removeitem ITEM_ACRO_BIKE
	giveitem ITEM_MACH_BIKE
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_SwitchMachForAcro::
	incrementgamestat GAME_STAT_TRADED_BIKES
	msgbox MauvilleCity_BikeShop_Text_ExchangedMachForAcro, MSGBOX_DEFAULT
	removeitem ITEM_MACH_BIKE
	giveitem ITEM_ACRO_BIKE
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_Assistant::
	msgbox MauvilleCity_BikeShop_Text_HandbooksAreInBack, MSGBOX_NPC
	end

MauvilleCity_BikeShop_EventScript_MachBikeHandbook::
	message MauvilleCity_BikeShop_Text_MachHandbookWhichPage
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage::
	multichoice 0, 0, MULTI_MACH_BIKE_INFO, FALSE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_HowToRide
	case 1, MauvilleCity_BikeShop_EventScript_HowToTurn
	case 2, MauvilleCity_BikeShop_EventScript_SandySlopes
	case 3, MauvilleCity_BikeShop_EventScript_ExitMachHandbook
	case MULTI_B_PRESSED, MauvilleCity_BikeShop_EventScript_ExitMachHandbook
	end

MauvilleCity_BikeShop_EventScript_HowToRide::
	message MauvilleCity_BikeShop_Text_HowToRideMachBike
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_HowToTurn::
	message MauvilleCity_BikeShop_Text_HowToTurnMachBike
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_SandySlopes::
	message MauvilleCity_BikeShop_Text_SandySlopes
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ExitMachHandbook::
	release
	end

MauvilleCity_BikeShop_EventScript_AcroBikeHandbook::
	message MauvilleCity_BikeShop_Text_AcroHandbookWhichPage
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage::
	multichoice 0, 0, MULTI_ACRO_BIKE_INFO, FALSE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_Wheelies
	case 1, MauvilleCity_BikeShop_EventScript_BunnyHops
	case 2, MauvilleCity_BikeShop_EventScript_Jumps
	case 3, MauvilleCity_BikeShop_EventScript_ExitAcroHandbook
	case MULTI_B_PRESSED, MauvilleCity_BikeShop_EventScript_ExitAcroHandbook
	end

MauvilleCity_BikeShop_EventScript_Wheelies::
	message MauvilleCity_BikeShop_Text_Wheelies
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_BunnyHops::
	message MauvilleCity_BikeShop_Text_BunnyHops
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_Jumps::
	message MauvilleCity_BikeShop_Text_Jumps
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ExitAcroHandbook::
	release
	end

MauvilleCity_BikeShop_Text_RydelGreeting:
	.string "{JPN}……おや? こりゃまた ずいぶん\nげんきそうな おきゃくさんが きたのう!\pわたしは ひとよんで ミスターカゼノ\nこの サイクルショップの オーナーさ!"
	.string ""
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_DidYouComeFromFarAway:
	.string "{JPN}カゼノ『きみの ランニングシューズ\nものすごく よごれて いるけど\lどこか とおくから やってきたのかい?"
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_GuessYouDontNeedBike:
	.string "{JPN}カゼノ『そうかい?\nじゃあ わたしの じてんしゃは\lひつよう なさそうだね"
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_ExplainBikesChooseWhichOne:
	.string "{JPN}カゼノ『フムフム……\pなんと ミシロタウンから はるばると!?\nすごい とおく じゃないか?\pわたしの じてんしゃが あれば\nここちよい かぜを かんじながら\lらくに いろんな ところへ いけるのに!\pよーし!\nきみに じてんしゃを あげよう!\pいや ちょっと まて!\pじてんしゃ には 2しゅるいの\nタイプが あるのを わすれとった!\pそれはな……\nマッハじてんしゃと ダートじてんしゃだ!\pマッハじてんしゃは\nぜんしんで かぜを かんじたい ひとに!\pダートじてんしゃは\nテクニックを たのしみたい ひとに!\pおじさん しんせつ だから\nきみの すきな ほうを あげちゃうよ!\pさあ! どちらを えらぶ?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_ChoseMachBike:
	.string "{JPN}{PLAYER}は\nマッハじてんしゃを えらんだ!$"

MauvilleCity_BikeShop_Text_ChoseAcroBike:
	.string "{JPN}{PLAYER}は\nダートじてんしゃを えらんだ!$"

MauvilleCity_BikeShop_Text_ComeBackToSwitchBikes:
	.string "{JPN}カゼノ『もしも べつの タイプと\nこうかん したくなった ときには\lいつでも わたしに こえを かけてくれ!"
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_WantToSwitchBikes:
	.string "{JPN}カゼノ『おや? ちがう タイプの\nじてんしゃに のりかえたく なったのかな?"
	.string "$"

MauvilleCity_BikeShop_Text_IllSwitchBikes:
	.string "{JPN}カゼノ『じゃあ きみが いま もっている\nじてんしゃを こうかん してあげよう!"
	.string "$"

MauvilleCity_BikeShop_Text_ExchangedMachForAcro:
	.string "{JPN}{PLAYER}は マッハじてんしゃを\nダートじてんしゃに かえてもらった!"
	.string "$"

MauvilleCity_BikeShop_Text_ExchangedAcroForMach:
	.string "{JPN}{PLAYER}は ダートじてんしゃを\nマッハじてんしゃに かえてもらった!"
	.string "$"

MauvilleCity_BikeShop_Text_HappyYouLikeIt:
	.string "{JPN}カゼノ『そうか そうか!\nきにいってくれて うれしいぞ!"
	.string "$"

MauvilleCity_BikeShop_Text_OhYourBikeIsInPC:
	.string "{JPN}……おや?\nきみに あげた じてんしゃ どうした?\pそうか パソコンに あずけてあるのか\pパソコンから とりだして もってきたら\nこうかんして あげる からな!\pきみの ぼうけんに……\nおいかぜが ふくことを いのってるよ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_HandbooksAreInBack:
	.string "{JPN}ぼくは ここで はたらき ながら\nじてんしゃの べんきょうを してるんだ\pじてんしゃの のりかたで\nなにか わからないことが あったら\pおくのへやに かいせつしょが あるから\nそれを よんでみると いいよ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_MachHandbookWhichPage:
	.string "{JPN}マッハじてんしゃの\nかいせつしょが おいてある!\pどの ページを よもうかな?"
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_HowToRideMachBike:
	.string "{JPN}じてんしゃに のって じゅうじボタンを\nおしたほうに じてんしゃは はしります!\pすこし はしっていく うちに\nスピードが あがっていく でしょう!\pとまりたいときは じゅうじボタンから\nてを はなせば スピードが おちて\lしぜんに とまって くれるはずです!\p……べつの ページを よみますか?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_HowToTurnMachBike:
	.string "{JPN}マッハじてんしゃは スピードが はやくて\nきゅうに とまることは できません\pだから かどを まがるときには\nちょっとした コツが ひつようです!\pまがりたい かどの すこし てまえで\nじゅうじボタンから てを はなして\lスピードを おとして まがりましょう!\p……べつの ページを よみますか?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_SandySlopes:
	.string "{JPN}ホウエンちほうには ところどころに\nちいさな すなのさかが あります!\pそこは すなが くずれやすい ために\nふつうは のぼることが できませんが\pマッハじてんしゃに のっていれば\nのぼることが できるのです!\p……べつの ページを よみますか?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_AcroHandbookWhichPage:
	.string "{JPN}ダートじてんしゃの\nかいせつしょが おいてある!\pどの ページを よもうかな?"
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_Wheelies:
	.string "{JPN}じてんしゃに のって Bボタンを おすと\nそのばで ぜんりんが もちあがります!\pそのままで じゅうじボタンを おせば\nぜんりんを あげた じょうたいで\lはしることが できるのです!\pそれが ウイリーという テクニック!\p……べつの ページを よみますか?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_BunnyHops:
	.string "{JPN}Bボタンを おしたままに していると\nウイリーしながら そのばで ピョンピョン\lとびはねることが できます!\pそれが ダニエルという テクニック!\pダニエルのまま はしることも できます!\p……べつの ページを よみますか?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_BikeShop_Text_Jumps:
	.string "{JPN}ダートじてんしゃに のっているとき\nじゅうじボタンと Bボタンを\lどうじに おすと ジャンプします!\pよこへ おせば よこに ジャンプ!\pうしろへ おせば ジャンプしながら\nほうこうを かえることが できます!\p……べつの ページを よみますか?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"
