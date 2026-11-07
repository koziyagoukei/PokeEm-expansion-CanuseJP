FiveIsland_MemorialPillar_Frlg_MapScripts::
	.byte 0

FiveIsland_MemorialPillar_EventScript_MemorialMan::
	lock
	goto_if_set FLAG_GOT_TM42_AT_MEMORIAL_PILLAR, FiveIsland_MemorialPillar_EventScript_AlreadyGotTM42
	goto_if_set FLAG_NO_ROOM_FOR_TM42_AT_MEMORIAL_PILLAR, FiveIsland_MemorialPillar_EventScript_ReturnedForTM42
	msgbox FiveIsland_MemorialPillar_Text_ScrubScrub
	closemessage
	applymovement LOCALID_MEMORIAL_MAN, Common_Movement_FacePlayer
	waitmovement 0
	delay 45
	msgbox FiveIsland_MemorialPillar_Text_YourMonsLookHealthy
	applymovement LOCALID_MEMORIAL_MAN, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox FiveIsland_MemorialPillar_Text_ThisIsWhereIBuriedMyOnix
	release
	end

FiveIsland_MemorialPillar_EventScript_AlreadyGotTM42::
	msgbox FiveIsland_MemorialPillar_Text_ScrubScrubTectonix
	release
	end

FiveIsland_MemorialPillar_EventScript_ReturnedForTM42::
	msgbox FiveIsland_MemorialPillar_Text_StillHaveThingAsMyThanks
	giveitem ITEM_TM42
	goto_if_eq VAR_RESULT, FALSE, FiveIsland_MemorialPillar_EventScript_NoRoomForTM42
	call FiveIsland_MemorialPillar_EventScript_ReceivedTM42
	release
	end

FiveIsland_MemorialPillar_EventScript_Memorial::
	lockall
	goto_if_set FLAG_GOT_TM42_AT_MEMORIAL_PILLAR, FiveIsland_MemorialPillar_EventScript_MemorialLemonadeAlreadyPlaced
	goto_if_set FLAG_NO_ROOM_FOR_TM42_AT_MEMORIAL_PILLAR, FiveIsland_MemorialPillar_EventScript_MemorialLemonadeAlreadyPlaced
	msgbox FiveIsland_MemorialPillar_Text_HereLiesTectonixLemonadeOffering
	checkitem ITEM_LEMONADE
	goto_if_eq VAR_RESULT, TRUE, FiveIsland_MemorialPillar_EventScript_AskPlaceLemonade
	releaseall
	end

FiveIsland_MemorialPillar_EventScript_AskPlaceLemonade::
	msgbox FiveIsland_MemorialPillar_Text_LeaveAnotherLemonadeOffering, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, FiveIsland_MemorialPillar_EventScript_PlaceLemonade
	releaseall
	end

FiveIsland_MemorialPillar_EventScript_PlaceLemonade::
	removeitem ITEM_LEMONADE
	msgbox FiveIsland_MemorialPillar_Text_PlacedCanOfLemonade
	closemessage
	applymovement LOCALID_MEMORIAL_MAN, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 45
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox FiveIsland_MemorialPillar_Text_ThankYouPleaseTakeThis
	giveitem ITEM_TM42
	goto_if_eq VAR_RESULT, FALSE, FiveIsland_MemorialPillar_EventScript_NoRoomForTM42
	call FiveIsland_MemorialPillar_EventScript_ReceivedTM42
	releaseall
	end

FiveIsland_MemorialPillar_EventScript_ReceivedTM42::
	msgbox FiveIsland_MemorialPillar_Text_BeGoodToYourMonsToo
	applymovement LOCALID_MEMORIAL_MAN, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	setflag FLAG_GOT_TM42_AT_MEMORIAL_PILLAR
	return

FiveIsland_MemorialPillar_EventScript_NoRoomForTM42::
	setflag FLAG_NO_ROOM_FOR_TM42_AT_MEMORIAL_PILLAR
	msgbox FiveIsland_MemorialPillar_Text_DontHaveRoomForIt
	releaseall
	end

FiveIsland_MemorialPillar_EventScript_MemorialLemonadeAlreadyPlaced::
	msgbox FiveIsland_MemorialPillar_Text_HereLiesTectonix
	releaseall
	end

FiveIsland_MemorialPillar_Text_MiloIntro::
	.string "{JPN}おれ とりきょうだい ちょうなん!\p"
	.string "トリは やっぱり\n"
	.string "あの クチバシが いいんだよなあ$"

FiveIsland_MemorialPillar_Text_MiloDefeat::
	.string "{JPN}うーん するどいっ!$"

FiveIsland_MemorialPillar_Text_MiloPostBattle::
	.string "{JPN}ここから だと\n"
	.string "4のしまに とんで いけるぞ$"

FiveIsland_MemorialPillar_Text_ChazIntro::
	.string "{JPN}おれ とりきょうだい じなん!\p"
	.string "トリは やっぱり\n"
	.string "あの つばさが いいんだよなあ$"

FiveIsland_MemorialPillar_Text_ChazDefeat::
	.string "{JPN}ばさ ばさっ!$"

FiveIsland_MemorialPillar_Text_ChazPostBattle::
	.string "{JPN}むこうに\n"
	.string "くらい おとこが いたぞ$"

FiveIsland_MemorialPillar_Text_HaroldIntro::
	.string "{JPN}おれ とりきょうだい すえっこ!\p"
	.string "トリは やっぱり\n"
	.string "あの うもうが いいんだよなあ$"

FiveIsland_MemorialPillar_Text_HaroldDefeat::
	.string "{JPN}うもう ふわふわ…\n"
	.string "ねむーい$"

FiveIsland_MemorialPillar_Text_HaroldPostBattle::
	.string "{JPN}とりポケモンに あまえ られると\n"
	.string "しあわせ…$"

FiveIsland_MemorialPillar_Text_ScrubScrub::
	.string "{JPN}よいしょ よいしょ$"

FiveIsland_MemorialPillar_Text_YourMonsLookHealthy::
	.string "{JPN}こんにちは…\n"
	.string "きみの ポケモン げんきそうだね$"

FiveIsland_MemorialPillar_Text_ThisIsWhereIBuriedMyOnix::
	.string "{JPN}これは ボクの イワークの…\n"
	.string "イワキチの おはか なんだ$"

FiveIsland_MemorialPillar_Text_HereLiesTectonixLemonadeOffering::
	.string "{JPN}『イワキチ ここに ねむる』と\n"
	.string "ほってある\p"
	.string "おはかの まえには\n"
	.string "ミックスオレが おいてある$"

FiveIsland_MemorialPillar_Text_LeaveAnotherLemonadeOffering::
	.string "{JPN}ミックスオレを もうひとつ\n"
	.string "おそなえして あげますか?$"

FiveIsland_MemorialPillar_Text_PlacedCanOfLemonade::
	.string "{JPN}{PLAYER}は おはかに\n"
	.string "ミックスオレを おそなえした$"

FiveIsland_MemorialPillar_Text_ThankYouPleaseTakeThis::
	.string "{JPN}あ… ありがとう……\p"
	.string "それ イワキチが だいすきな\n"
	.string "のみもの だったんだ\p"
	.string "みずしらずの きみに\n"
	.string "やさしく して もらって\l"
	.string "すこし げんきが でたよ\p"
	.string "おれいに これを もらってください$"

FiveIsland_MemorialPillar_Text_BeGoodToYourMonsToo::
	.string "{JPN}きみも ポケモンを\n"
	.string "だいじに して あげてね$"

FiveIsland_MemorialPillar_Text_DontHaveRoomForIt::
	.string "{JPN}いっぱい なら\n"
	.string "つぎに あった ときまで\l"
	.string "とって おくね$"

FiveIsland_MemorialPillar_Text_StillHaveThingAsMyThanks::
	.string "{JPN}あ このまえの…\p"
	.string "この おれい\n"
	.string "うけとって ください$"

FiveIsland_MemorialPillar_Text_ScrubScrubTectonix::
	.string "{JPN}よいしょ よいしょ\p"
	.string "………\n"
	.string "イワキチ……$"

FiveIsland_MemorialPillar_Text_HereLiesTectonix::
	.string "{JPN}ていねいに いしが つまれている\p"
	.string "『イワキチ ここに ねむる』と\n"
	.string "ほってある$"

