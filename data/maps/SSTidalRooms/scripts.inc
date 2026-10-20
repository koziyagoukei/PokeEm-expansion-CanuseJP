SSTidalRooms_MapScripts::
	.byte 0

SSTidalRooms_EventScript_SnatchGiver::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM_SNATCH, SSTidalRooms_EventScript_ExplainSnatch
	msgbox SSTidalRooms_Text_NotSuspiciousTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_TM_SNATCH
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_SNATCH
	msgbox SSTidalRooms_Text_ExplainSnatch, MSGBOX_DEFAULT
	release
	end

SSTidalRooms_EventScript_ExplainSnatch::
	msgbox SSTidalRooms_Text_ExplainSnatch, MSGBOX_DEFAULT
	release
	end

SSTidalRooms_EventScript_Bed::
	lockall
	msgbox SSTidalRooms_Text_TakeRestOnBed, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	call SSTidalRooms_EventScript_ProgessCruiseAfterBed
	releaseall
	end

SSTidalRooms_EventScript_Colton::
	trainerbattle_single TRAINER_COLTON, SSTidalRooms_Text_ColtonIntro, SSTidalRooms_Text_ColtonDefeat
	msgbox SSTidalRooms_Text_ColtonPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Micah::
	trainerbattle_single TRAINER_MICAH, SSTidalRooms_Text_MicahIntro, SSTidalRooms_Text_MicahDefeat
	msgbox SSTidalRooms_Text_MicahPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Thomas::
	trainerbattle_single TRAINER_THOMAS, SSTidalRooms_Text_ThomasIntro, SSTidalRooms_Text_ThomasDefeat
	msgbox SSTidalRooms_Text_ThomasPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Jed::
	trainerbattle_double TRAINER_LEA_AND_JED, SSTidalRooms_Text_JedIntro, SSTidalRooms_Text_JedDefeat, SSTidalRooms_Text_JedNotEnoughMons
	msgbox SSTidalRooms_Text_JedPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Lea::
	trainerbattle_double TRAINER_LEA_AND_JED, SSTidalRooms_Text_LeaIntro, SSTidalRooms_Text_LeaDefeat, SSTidalRooms_Text_LeaNotEnoughMons
	msgbox SSTidalRooms_Text_LeaPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Garret::
	trainerbattle_single TRAINER_GARRET, SSTidalRooms_Text_GarretIntro, SSTidalRooms_Text_GarretDefeat
	msgbox SSTidalRooms_Text_GarretPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Naomi::
	trainerbattle_single TRAINER_NAOMI, SSTidalRooms_Text_NaomiIntro, SSTidalRooms_Text_NaomiDefeat
	msgbox SSTidalRooms_Text_NaomiPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_Text_TakeRestOnBed:
	.string "{JPN}ベッドが ある……\nやすんでいこう$"

SSTidalRooms_Text_ColtonIntro:
	.string "{JPN}ミナモシティには よく いくんだ\p"
	.string "コンテストの けんがくが\n"
	.string "わたしの たのしみ なんだよ$"

SSTidalRooms_Text_ColtonDefeat:
	.string "{JPN}たのしい しょうぶ だったよ$"

SSTidalRooms_Text_ColtonPostBattle:
	.string "{JPN}つぎの コンテストには\n"
	.string "どんな ポケモンが でるのか\l"
	.string "たのしみで たまらないよ$"

SSTidalRooms_Text_MicahIntro:
	.string "{JPN}きみの ともだちは つよいのかね？$"

SSTidalRooms_Text_MicahDefeat:
	.string "{JPN}つよい ともだち だね！$"

SSTidalRooms_Text_MicahPostBattle:
	.string "{JPN}にんげん だけが ともだち じゃない\n"
	.string "わたしに とっては ポケモンが\l"
	.string "たいせつな ともだち なんだよ！$"

SSTidalRooms_Text_ThomasIntro:
	.string "{JPN}おきゃくさん\n"
	.string "ノックは したかね？$"

SSTidalRooms_Text_ThomasDefeat:
	.string "{JPN}まけても\n"
	.string "あわてず さわがず$"

SSTidalRooms_Text_ThomasPostBattle:
	.string "{JPN}どんな ときでも とりみださないのが\n"
	.string "しんしの みだしなみ だよ$"

SSTidalRooms_Text_JedIntro:
	.string "{JPN}ジン『ちょっと はずかしいけど\n"
	.string "ぼくたちの ラブラブ パワー みせようかな$"

SSTidalRooms_Text_JedDefeat:
	.string "{JPN}ジン『はぁ……$"

SSTidalRooms_Text_JedPostBattle:
	.string "{JPN}ジン『ぼくたちの ラブラブ パワーでも\nかてないのは はじめて だよ!\lきみは すごい トレーナー なんだね!$"

SSTidalRooms_Text_JedNotEnoughMons:
	.string "{JPN}ジン『ポケモン いっぴき？\n"
	.string "それだと さみしくないか？$"

SSTidalRooms_Text_LeaIntro:
	.string "{JPN}カホ『ちょっと はずかしいけど\n"
	.string "ふたりの ラブラブ パワー みせてあげる！$"

SSTidalRooms_Text_LeaDefeat:
	.string "{JPN}カホ『やん！$"

SSTidalRooms_Text_LeaPostBattle:
	.string "{JPN}カホ『ふたりの ラブラブ パワーでも\n"
	.string "かてない だなんて\l"
	.string "あなた すごい トレーナー なのね！$"

SSTidalRooms_Text_LeaNotEnoughMons:
	.string "{JPN}カホ『しょうぶ したかったのに……\nポケモン 2ひき いないのね……$"

SSTidalRooms_Text_GarretIntro:
	.string "{JPN}キミ ちょうど いいところに きたね！\p"
	.string "ボク たいくつで たまらないんだ\n"
	.string "ちょっと たのしませて くれよ$"

SSTidalRooms_Text_GarretDefeat:
	.string "{JPN}……もう いいよ$"

SSTidalRooms_Text_GarretPostBattle:
	.string "{JPN}パパに たのんで ボク せんようの\n"
	.string "ヨットを かってもらおうかな？\l"
	.string "ポケモンと いっしょに のれる ヨット！$"

SSTidalRooms_Text_NaomiIntro:
	.string "{JPN}あら かわいい トレーナーさん\n"
	.string "おちゃに なさる？\l"
	.string "それとも しょうぶ かしら？$"

SSTidalRooms_Text_NaomiDefeat:
	.string "{JPN}かっぱつで いらっしゃるのね$"

SSTidalRooms_Text_NaomiPostBattle:
	.string "{JPN}ゆうがな きゃくせんで\nせかい いっしゅうも よろしいけど……\pれんらくせんで のんびり\nホウエンを みるのも すてき ですわ$"

SSTidalRooms_Text_NotSuspiciousTakeThis:
	.string "{JPN}わたし あやしく ないです！\n"
	.string "だから これを あなたに あげます！\p"
	.string "べつに だれか から\n"
	.string "よこどりした モノじゃ ないです！\l"
	.string "あんしん して つかって ください！$"

SSTidalRooms_Text_ExplainSnatch:
	.string "{JPN}よこどり という わざは\n"
	.string "あいてが つかおうとする わざを\l"
	.string "じぶんで つかって しまうのです！$"

