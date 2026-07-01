SlateportCity_OceanicMuseum_1F_MapScripts::
	.byte 0

SlateportCity_OceanicMuseum_1F_EventScript_EntranceAttendant::
	msgbox SlateportCity_OceanicMuseum_1F_Text_PleaseEnjoyYourself, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFeeLeft::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFee
	end

SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFeeRight::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFee
	end

SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFee::
	showmoneybox 0, 0
	msgbox SlateportCity_OceanicMuseum_1F_Text_WouldYouLikeToEnter, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SlateportCity_OceanicMuseum_1F_EventScript_CheckMoneyForFee
	closemessage
	hidemoneybox
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PushPlayerBackFromCounter
	waitmovement 0
	releaseall
	end

SlateportCity_OceanicMuseum_1F_EventScript_CheckMoneyForFee::
	checkmoney 50
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_OceanicMuseum_1F_EventScript_NotEnoughMoney
	playse SE_SHOP
	removemoney 50
	updatemoneybox
	msgbox SlateportCity_OceanicMuseum_1F_Text_PleaseEnjoyYourself, MSGBOX_DEFAULT
	setvar VAR_SLATEPORT_MUSEUM_1F_STATE, 1
	hidemoneybox
	releaseall
	end

SlateportCity_OceanicMuseum_1F_EventScript_NotEnoughMoney::
	goto_if_unset FLAG_DELIVERED_DEVON_GOODS, SlateportCity_OceanicMuseum_1F_EventScript_AllowEntranceAnyway
	msgbox SlateportCity_OceanicMuseum_1F_Text_NotEnoughMoney, MSGBOX_DEFAULT
	closemessage
	hidemoneybox
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PushPlayerBackFromCounter
	waitmovement 0
	releaseall
	end

SlateportCity_OceanicMuseum_1F_EventScript_AllowEntranceAnyway::
	msgbox SlateportCity_OceanicMuseum_1F_Text_CatchUpWithYourGroup, MSGBOX_DEFAULT
	setvar VAR_SLATEPORT_MUSEUM_1F_STATE, 1
	hidemoneybox
	releaseall
	end

SlateportCity_OceanicMuseum_1F_Movement_PushPlayerBackFromCounter:
	walk_down
	step_end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt1::
	msgbox SlateportCity_OceanicMuseum_1F_Text_AquaExistForGoodOfAll, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt2::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OurBossIsntHere, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt3::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WouldStuffHereMakeMeRich, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt4::
	msgbox SlateportCity_OceanicMuseum_1F_Text_CanLearnForNefariousDeeds, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt5::
	msgbox SlateportCity_OceanicMuseum_1F_Text_RustboroBungled, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt6::
	msgbox SlateportCity_OceanicMuseum_1F_Text_DidntHaveMoney, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_WhirlpoolExperiment::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WhirlpoolExperiment, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_WaterfallExperiment::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WaterfallExperiment, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanSoilDisplay::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanSoilDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_BeachSandDisplay::
	msgbox SlateportCity_OceanicMuseum_1F_Text_BeachSandDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanicMinifact1::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact1, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanicMinifact2::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact2, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanicMinifact3::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact3, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_FossilDisplay::
	msgbox SlateportCity_OceanicMuseum_1F_Text_FossilDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_DepthMeasuringMachine::
	msgbox SlateportCity_OceanicMuseum_1F_Text_DepthMeasuringMachine, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron1::
	msgbox SlateportCity_OceanicMuseum_1F_Text_LearnAboutSeaForBattling, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron2::
	msgbox SlateportCity_OceanicMuseum_1F_Text_SternIsRoleModel, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron3::
	msgbox SlateportCity_OceanicMuseum_1F_Text_MustBePokemonWeDontKnow, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron4::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WantSeaPokemon, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGrunt::
	lock
	faceplayer
	delay 8
	playse SE_PIN
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, Common_Movement_Delay48
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_1F_Text_RememberMeTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_TM_THIEF
	goto_if_eq VAR_RESULT, 0, SlateportCity_OceanicMuseum_1F_EventScript_NoRoomForThief
	setflag FLAG_RECEIVED_TM_THIEF
	msgbox SlateportCity_OceanicMuseum_1F_Text_HopeINeverSeeYouAgain, MSGBOX_DEFAULT
	closemessage
	goto_if_eq VAR_FACING, DIR_NORTH, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitNorth
	goto_if_eq VAR_FACING, DIR_SOUTH, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitSouth
	goto_if_eq VAR_FACING, DIR_WEST, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitWestEast
	goto_if_eq VAR_FACING, DIR_EAST, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitWestEast
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitNorth::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitNorth
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExitNorth
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitSouth::
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExit
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitWestEast::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitWestEast
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExit
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited::
	setflag FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_FAMILIAR_AQUA_GRUNT
	playse SE_EXIT
	removeobject LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT
	release
	end

SlateportCity_OceanicMuseum_1F_EventScript_NoRoomForThief::
	msgbox SlateportCity_OceanicMuseum_1F_Text_YouHaveToTakeThis, MSGBOX_DEFAULT
	release
	end

SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitNorth:
	delay_16
	delay_8
	delay_4
	walk_in_place_faster_down
	step_end

SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitWestEast:
	delay_16
	walk_in_place_faster_down
	step_end

SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExit:
	face_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	delay_8
	step_end

SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExitNorth:
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	delay_8
	step_end

SlateportCity_OceanicMuseum_1F_Text_WouldYouLikeToEnter:
	.string "{JPN}うみの かがく はくぶつかんへ\nようこそ いらっしゃいませ!\pにゅうじょうりょう 50えんに なります\nけんがく して いきますか?"
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_PleaseEnjoyYourself:
	.string "{JPN}ごゆっくり どうぞ!$"

SlateportCity_OceanicMuseum_1F_Text_NotEnoughMoney:
	.string "{JPN}……あら おかねが たりませんわ$"

SlateportCity_OceanicMuseum_1F_Text_CatchUpWithYourGroup:
	.string "{JPN}……あら さっきの だんたいさん?\nあなただけ おくれちゃって しかたないわね\lはやく とおっちゃって!"
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_AquaExistForGoodOfAll:
	.string "{JPN}みんなの ために\nわれわれ アクアだんは いるのよ!"
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_OurBossIsntHere:
	.string "{JPN}げんちしゅうごう で\nここに あつまったのは いいけど……\pかんじんの リーダーが まだ こないんだ"
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_WouldStuffHereMakeMeRich:
	.string "{JPN}ここに おいてある もの\nかっぱらうと もうかるかな?"
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_CanLearnForNefariousDeeds:
	.string "{JPN}この ちしきを いかして\nわるいこと してやるか……"
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_RustboroBungled:
	.string "{JPN}なかまが カナズミで ドジらなければ\nここに こなくて すんだのによ!"
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_DidntHaveMoney:
	.string "{JPN}さっき 50えんが なくて\nうけつけで すごい じかん かかったよ……"
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_LearnAboutSeaForBattling:
	.string "{JPN}うみのこと べんきょう して\nポケモンしょうぶに やくだてたいの"
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_SternIsRoleModel:
	.string "{JPN}わたくしッ\nうみを みると こうふん するのですッ!\pそんな わたくしに とって\nクスノキかんちょうは\lあこがれの ひと ナンバー1 なのですッ!"
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_MustBePokemonWeDontKnow:
	.string "{JPN}うみは はてしなく ひろく\nどこまでも ふかい……\pきっと われわれが しらない ポケモンも\nいっぱい いるのだろうね"
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_WantSeaPokemon:
	.string "{JPN}あたし うみの ポケモンが ほしいの\pだって ひんやりと してて\nだきしめたら きもちよさそう だもの"
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_RememberMeTakeThis:
	.string "{JPN}ひえー!!\nなんで おまえ ここに いるんだよ!?\pおれ? おれは カナシダ トンネルで\nおまえに コテンパンに された\lアクアだん だよ\pこれを やるから ゆるしてくれよ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_HopeINeverSeeYouAgain:
	.string "{JPN}その わざマシン\nおれより おまえのほうが にあうぜ!\pじゃあな わはははー!"
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_YouHaveToTakeThis:
	.string "{JPN}たのむから\nなんとかして うけとってくれよ!"
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_WhirlpoolExperiment:
	.string "{JPN}ガラスの なかで あおい みずが\nくるくると まわって いる!\p「これは かぜの ちからで じんこうてきに\n うずしおを おこす じっけん です」"
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_WaterfallExperiment:
	.string "{JPN}ガラスの なかで あかい ボールが\nうかんだり しずんだり している!\p「これは ボールの ふりょくを りようした\n たきのぼりの げんりの じっけん です」"
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_OceanSoilDisplay:
	.string "{JPN}うみの そこの つちの みほんだ!\p「かいていには ながい ねんげつを かけて\n せいぶつの しがいが ふりつもり\l ちそうが かたち づくられて いきます」\p「これらを ぶんせき することで\n とおい むかしの ことが わかるのです」"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_BeachSandDisplay:
	.string "{JPN}うみべの すなの みほんだ!\p「やまから かわを つたって きた いしは\n みずの なかで ころがる うちに\l くだかれたり けずれたり していきます」\p「それらが ちいさな つぶに なって\n うみべの すなが できて いくのです」"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact1:
	.string "{JPN}「うみの まめちしき その1\n なぜ うみの みずは あおいの?」\p「ひかりには いくつもの いろが あって\n ほとんどの ひかりは みずに はいると\l その いろが なくなって いきます」\p「けれど あおい ひかり だけは\n そのままの いろで のこる ため\l うみは あおく みえるのです」"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact2:
	.string "{JPN}「うみの まめちしき その2\n なぜ うみの みずは しょっぱいの?」\p「かいすい には いわから にじみでた\p しおの もとに なる\n ナトリウムイオン と えんそイオン\l という ぶっしつが おおく ふくまれ\l だから なめると しょっぱいのです」"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact3:
	.string "{JPN}「うみの まめちしき その3\n うみと りくは どちらが ひろいの?」\p「だいたい うみが 70パーセントで\n りくが 30パーセント くらいです\p りくちに くらべて うみは\n その 2ばい いじょうも あるのです」"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_FossilDisplay:
	.string "{JPN}デコボコに なみうった かせきが ある!\p「うみの そこに たまった つちには\n かいすいが ながれる えいきょうで\l なみの ような もようが できます」\p「それが そのまま かせきに なったものを\n リップルマークと よんでいます」"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_1F_Text_DepthMeasuringMachine:
	.string "{JPN}まるい ドームの なかで なんだか\nよく わからない きかいが まわってる!\pなにかの ふかさを しらべるのかな?"
	.string ""
	.string "$"
