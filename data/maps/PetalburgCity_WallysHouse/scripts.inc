PetalburgCity_WallysHouse_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PetalburgCity_WallysHouse_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PetalburgCity_WallysHouse_OnWarp
	.byte 0

PetalburgCity_WallysHouse_OnWarp:
	map_script_2 VAR_PETALBURG_CITY_STATE, 4, PetalburgCity_WallysHouse_EventScript_PlayerWallysDadFaceEachOther
	.2byte 0

PetalburgCity_WallysHouse_EventScript_PlayerWallysDadFaceEachOther::
	turnobject LOCALID_PLAYER, DIR_EAST
	turnobject LOCALID_WALLYS_HOUSE_WALLYS_DAD, DIR_WEST
	end

PetalburgCity_WallysHouse_OnFrame:
	map_script_2 VAR_PETALBURG_CITY_STATE, 4, PetalburgCity_WallysHouse_EventScript_GiveHMSurf
	.2byte 0

PetalburgCity_WallysHouse_EventScript_GiveHMSurf::
	lockall
	msgbox PetalburgCity_WallysHouse_Text_PleaseExcuseUs, MSGBOX_DEFAULT
	giveitem ITEM_HM_SURF
	setflag FLAG_RECEIVED_HM_SURF
	msgbox PetalburgCity_WallysHouse_Text_SurfGoAllSortsOfPlaces, MSGBOX_DEFAULT
	setvar VAR_PETALBURG_CITY_STATE, 5
	releaseall
	end

PetalburgCity_WallysHouse_EventScript_WallysDad::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_WALLY_VICTORY_ROAD, PetalburgCity_WallysHouse_EventScript_DefeatedWallyInVictoryRoad
	goto_if_set FLAG_RECEIVED_HM_SURF, PetalburgCity_WallysHouse_EventScript_ReceievedHMSurf
	goto_if_set FLAG_THANKED_FOR_PLAYING_WITH_WALLY, PetalburgCity_WallysHouse_EventScript_PlayedWithWally
	msgbox PetalburgCity_WallysHouse_Text_ThanksForPlayingWithWally, MSGBOX_DEFAULT
	setflag FLAG_THANKED_FOR_PLAYING_WITH_WALLY
	release
	end

PetalburgCity_WallysHouse_EventScript_ReceievedHMSurf::
	msgbox PetalburgCity_WallysHouse_Text_WallyIsComingHomeSoon, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_DefeatedWallyInVictoryRoad::
	msgbox PetalburgCity_WallysHouse_Text_YouMetWallyInEverGrandeCity, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_PlayedWithWally::
	msgbox PetalburgCity_WallysHouse_Text_WonderHowWallyIsDoing, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_WallysMom::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_SURF, PetalburgCity_WallysHouse_EventScript_ReceivedHMSurf
	msgbox PetalburgCity_WallysHouse_Text_WallyWasReallyHappy, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_ReceivedHMSurf::
	msgbox PetalburgCity_WallysHouse_Text_WallyLeftWithoutTelling, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_Text_ThanksForPlayingWithWally:
	.string "{JPN}きみは……\nそうか {PLAYER}{KUN} だね?\pさっきは ミツルの あいてを してくれて\nありがとう……\pあのこは うまれた ときから\nからだが じょうぶでは なくてね\pそれで すこしの あいだ シダケタウンの\nわたしの しんせきの うちで\lあずかってもらう ことに したんだよ\pあそこは ここよりも ずっと\nくうきが きれいな ところ だからね!\p……で ミツルかい? ミツルなら\nもう しゅっぱつ していったよ\pいまごろは どの あたりかなあ……$"

PetalburgCity_WallysHouse_Text_WonderHowWallyIsDoing:
	.string "{JPN}ミツルは どうしてるかなあ……$"

PetalburgCity_WallysHouse_Text_PleaseExcuseUs:
	.string "{JPN}{PLAYER}{KUN}!\nむりに よびだして すまなかったね!\pミツルは シダケタウンに いってから\nすっかり げんきを とりもどしたよ!\pそれも みんな きみの おかげさ!\pなにしろ このまちを でるときに\nミツルが ポケモンを つかまえるのを\lてつだって くれただろう?\pあのときの ことが ミツルは とっても\nうれしかった らしいんだ!\pいや ミツル だけじゃない\nちちおやの わたしも うれしいよ!\pあのこに きみの ような\nやさしい ともだちが できたことが ね!\pそのおれい という わけでは ないけど\nこれを うけとって くれないか!$"

PetalburgCity_WallysHouse_Text_SurfGoAllSortsOfPlaces:
	.string "{JPN}なみのりを つかえるように なれば\nいろんな ばしょに いけるように なるよ!$"

PetalburgCity_WallysHouse_Text_WallyIsComingHomeSoon:
	.string "{JPN}もうすぐ ミツルが かえってくるぞ!\nたのしみだなあ……!$"

PetalburgCity_WallysHouse_Text_YouMetWallyInEverGrandeCity:
	.string "{JPN}ええっ!? サイユウシティで\nミツルと であった だって……?\pんもう {PLAYER}{KUN}ってばー\pいくら あのこが げんきに なったからって\nそんな とおくの まち まで\lひとりで いけるわけ ないよねえ……$"

PetalburgCity_WallysHouse_Text_WallyWasReallyHappy:
	.string "{JPN}ミツルが ポケモンを つかまえた って\nすごく うれしそうに はなしていたの\pあのこの あんな えがお ひさしぶり$"

PetalburgCity_WallysHouse_Text_WallyLeftWithoutTelling:
	.string "{JPN}うちの しゅじんには ないしょ だけど\pじつは ミツルったら シダケタウンを\nかってに でて いっちゃった らしいの……\pあのこ からだは よわかったけど\nあんがい しっかりした せいかく だから\lかならず ぶじで かえってくると\lわたしは しんじてるわ!$"
