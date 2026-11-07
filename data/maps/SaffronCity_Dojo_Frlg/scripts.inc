SaffronCity_Dojo_Frlg_MapScripts::
	.byte 0

SaffronCity_Dojo_EventScript_TriggerMasterBattleLeft::
	lockall
	setvar VAR_MAP_SCENE_SAFFRON_CITY_DOJO, 1
	applymovement LOCALID_KARATE_MASTER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	releaseall
	end

SaffronCity_Dojo_EventScript_TriggerMasterBattleRight::
	lockall
	setvar VAR_MAP_SCENE_SAFFRON_CITY_DOJO, 1
	applymovement LOCALID_KARATE_MASTER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	releaseall
	end

SaffronCity_Dojo_EventScript_HitmonleeBall::
	lock
	faceplayer
	goto_if_set FLAG_GOT_HITMON_FROM_DOJO, SaffronCity_Dojo_EventScript_AlreadyGotHitmon
	showmonpic SPECIES_HITMONLEE, 10, 3
	setvar VAR_TEMP_1, SPECIES_HITMONLEE
	applymovement LOCALID_KARATE_MASTER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox SaffronCity_Dojo_Text_YouWantHitmonlee, MSGBOX_YESNO
	call EventScript_RestorePrevTextColor
	goto_if_eq VAR_RESULT, YES, SaffronCity_Dojo_EventScript_GiveHitmon
	hidemonpic
	release
	end

SaffronCity_Dojo_EventScript_AlreadyGotHitmon::
	msgbox SaffronCity_Dojo_Text_BetterNotGetGreedy
	release
	end

SaffronCity_Dojo_EventScript_HitmonchanBall::
	lock
	faceplayer
	goto_if_set FLAG_GOT_HITMON_FROM_DOJO, SaffronCity_Dojo_EventScript_AlreadyGotHitmon
	showmonpic SPECIES_HITMONCHAN, 10, 3
	setvar VAR_TEMP_1, SPECIES_HITMONCHAN
	applymovement LOCALID_KARATE_MASTER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox SaffronCity_Dojo_Text_YouWantHitmonchan, MSGBOX_YESNO
	call EventScript_RestorePrevTextColor
	goto_if_eq VAR_RESULT, YES, SaffronCity_Dojo_EventScript_GiveHitmon
	hidemonpic
	release
	end

SaffronCity_Dojo_EventScript_GiveHitmon::
	hidemonpic
	givemon VAR_TEMP_1, 25
	goto_if_eq VAR_RESULT, 0, SaffronCity_Dojo_EventScript_ReceivedHitmonParty
	goto_if_eq VAR_RESULT, 1, SaffronCity_Dojo_EventScript_ReceivedHitmonPC
	goto_if_eq VAR_RESULT, 2, Common_EventScript_NoMoreRoomForPokemon
	release
	end

SaffronCity_Dojo_EventScript_ReceivedHitmonParty::
	removeobject VAR_LAST_TALKED
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	playfanfare MUS_LEVEL_UP
	message SaffronCity_Dojo_Text_ReceivedMonFromKarateMaster
	waitmessage
	waitfanfare
	setflag FLAG_GOT_HITMON_FROM_DOJO
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SaffronCity_Dojo_EventScript_EndGiveMon
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto SaffronCity_Dojo_EventScript_EndGiveMon
	end

SaffronCity_Dojo_EventScript_ReceivedHitmonPC::
	removeobject VAR_LAST_TALKED
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	playfanfare MUS_LEVEL_UP
	message SaffronCity_Dojo_Text_ReceivedMonFromKarateMaster
	waitmessage
	waitfanfare
	setflag FLAG_GOT_HITMON_FROM_DOJO
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SaffronCity_Dojo_EventScript_TransferredHitmonToPC
	call Common_EventScript_NameReceivedBoxMon
	goto SaffronCity_Dojo_EventScript_TransferredHitmonToPC
	end

SaffronCity_Dojo_EventScript_TransferredHitmonToPC::
	call Common_EventScript_TransferredToPC
	goto SaffronCity_Dojo_EventScript_EndGiveMon
	end

SaffronCity_Dojo_EventScript_EndGiveMon::
	release
	end

SaffronCity_Dojo_EventScript_Statue::
	msgbox SaffronCity_Dojo_Text_FightingDojo, MSGBOX_SIGN
	end

SaffronCity_Dojo_EventScript_LeftScroll::
	msgbox SaffronCity_Dojo_Text_EnemiesOnEverySide, MSGBOX_SIGN
	end

SaffronCity_Dojo_EventScript_RightScroll::
	msgbox SaffronCity_Dojo_Text_GoesAroundComesAround, MSGBOX_SIGN
	end

SaffronCity_Dojo_EventScript_Hitoshi::
	trainerbattle_single TRAINER_BLACK_BELT_HITOSHI, SaffronCity_Dojo_Text_HitoshiIntro, SaffronCity_Dojo_Text_HitoshiDefeat
	msgbox SaffronCity_Dojo_Text_HitoshiPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_Hideki::
	trainerbattle_single TRAINER_BLACK_BELT_HIDEKI, SaffronCity_Dojo_Text_HidekiIntro, SaffronCity_Dojo_Text_HidekiDefeat
	msgbox SaffronCity_Dojo_Text_HidekiPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_Aaron::
	trainerbattle_single TRAINER_BLACK_BELT_AARON, SaffronCity_Dojo_Text_AaronIntro, SaffronCity_Dojo_Text_AaronDefeat
	msgbox SaffronCity_Dojo_Text_AaronPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_Mike::
	trainerbattle_single TRAINER_BLACK_BELT_MIKE, SaffronCity_Dojo_Text_MikeIntro, SaffronCity_Dojo_Text_MikeDefeat
	msgbox SaffronCity_Dojo_Text_MikePostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_MasterKoichi::
	trainerbattle_single TRAINER_BLACK_BELT_KOICHI, SaffronCity_Dojo_Text_MasterKoichiIntro, SaffronCity_Dojo_Text_MasterKoichiDefeat, SaffronCity_Dojo_EventScript_DefeatedMasterKoichi
	goto_if_set FLAG_GOT_HITMON_FROM_DOJO, SaffronCity_Dojo_EventScript_MasterKoichiAlreadyGotHitmon
	msgbox SaffronCity_Dojo_Text_ChoosePrizedFightingMon, MSGBOX_AUTOCLOSE
	end

SaffronCity_Dojo_EventScript_MasterKoichiAlreadyGotHitmon::
	msgbox SaffronCity_Dojo_Text_StayAndTrainWithUs
	release
	end

SaffronCity_Dojo_EventScript_DefeatedMasterKoichi::
	setvar VAR_MAP_SCENE_SAFFRON_CITY_DOJO, 1
	release
	end

SaffronCity_Dojo_Text_MasterKoichiIntro::
	.string "{JPN}オスッ!\p"
	.string "わしが かくとう どうじょうの\n"
	.string "しはん カラテ だいおう である!\p"
	.string "おぬしは どうじょう やぶりか!\n"
	.string "ならば ようしゃは せんぞ!\p"
	.string "トオリャー!$"

SaffronCity_Dojo_Text_MasterKoichiDefeat::
	.string "{JPN}ウオリャ!\n"
	.string "だー! やられたあー!$"

SaffronCity_Dojo_Text_ChoosePrizedFightingMon::
	.string "{JPN}わしは たしかに まけた!\p"
	.string "しかし どうじょうの かんばん\n"
	.string "… だけは!\l"
	.string "もって いかないで くれい!\p"
	.string "かわりに わしの だいじな\n"
	.string "かくとう ポケモンを わたす!\p"
	.string "どうか!\n"
	.string "すきな ほうを えらんで くれい!$"

SaffronCity_Dojo_Text_StayAndTrainWithUs::
	.string "{JPN}オスッ!\p"
	.string "どうだ? ついでに ここで\n"
	.string "カラテ れんしゅう していくか!$"

SaffronCity_Dojo_Text_MikeIntro::
	.string "{JPN}ウオースッ!\n"
	.string "きさま! どうじょう やぶり か!$"

SaffronCity_Dojo_Text_MikeDefeat::
	.string "{JPN}ま まいった!$"

SaffronCity_Dojo_Text_MikePostBattle::
	.string "{JPN}あんしん するのは\n"
	.string "しはんに かって からに しろ!\p"
	.string "おれに かっても\n"
	.string "たいした こと ないぜ! オスッ!$"

SaffronCity_Dojo_Text_HidekiIntro::
	.string "{JPN}オスッ! うでが たつ らしいな!\n"
	.string "えんりょ なく いくぜ!$"

SaffronCity_Dojo_Text_HidekiDefeat::
	.string "{JPN}ウオッス! わざあり!$"

SaffronCity_Dojo_Text_HidekiPostBattle::
	.string "{JPN}しはんは かくとうかの かみさまだ!\n"
	.string "いどむと いうなら かくご していけ$"

SaffronCity_Dojo_Text_AaronIntro::
	.string "{JPN}チェストー!\n"
	.string "かたい もの など こわく ない!\p"
	.string "まいにち こぶしで\n"
	.string "いわを わる れんしゅう してる!$"

SaffronCity_Dojo_Text_AaronDefeat::
	.string "{JPN}あたっ! オスッ!$"

SaffronCity_Dojo_Text_AaronPostBattle::
	.string "{JPN}かくとうかが こわい もの など\n"
	.string "ちょうのうりょく ぐらいだ!オスッ!$"

SaffronCity_Dojo_Text_HitoshiIntro::
	.string "{JPN}オスッ!\p"
	.string "ここを かくとう どうじょうと\n"
	.string "しっての ぶれい か!$"

SaffronCity_Dojo_Text_HitoshiDefeat::
	.string "{JPN}ぐッ! まいった!$"

SaffronCity_Dojo_Text_HitoshiPostBattle::
	.string "{JPN}ここは ぜんこくの かくとうか が\n"
	.string "あつまる どうじょう だ! オスッ!$"

SaffronCity_Dojo_Text_YouWantHitmonlee::
	.string "{JPN}ウスッ! キック わざのおに!\n"
	.string "サワムラーを とるか?$"

SaffronCity_Dojo_Text_ReceivedMonFromKarateMaster::
	.string "{JPN}{PLAYER}は カラテ だいおう から\n"
	.string "{STR_VAR_1}を もらった!$"

SaffronCity_Dojo_Text_YouWantHitmonchan::
	.string "{JPN}ウスッ! うなる こぶし!\n"
	.string "エビワラーに するか?$"

SaffronCity_Dojo_Text_ReceivedMonFromKarateMaster2::
	.string "{PLAYER}は カラテ だいおう から\n"
	.string "{STR_VAR_1}を もらった!$"

SaffronCity_Dojo_Text_BetterNotGetGreedy::
	.string "{JPN}よくばるのは よそう…$"

SaffronCity_Dojo_Text_EnemiesOnEverySide::
	.string "{JPN}しめん そか$"

SaffronCity_Dojo_Text_GoesAroundComesAround::
	.string "{JPN}いんが おうほう$"

SaffronCity_Dojo_Text_FightingDojo::
	.string "{JPN}かくとう どうじょう!$"

