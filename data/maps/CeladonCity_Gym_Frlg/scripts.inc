CeladonCity_Gym_Frlg_MapScripts::
	.byte 0

CeladonCity_Gym_EventScript_Erika::
	famechecker FAMECHECKER_ERIKA, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_ERIKA, CeladonCity_Gym_Text_ErikaIntro, CeladonCity_Gym_Text_ErikaDefeat, CeladonCity_Gym_EventScript_DefeatedErika, NO_MUSIC
	goto_if_unset FLAG_GOT_TM19_FROM_ERIKA, CeladonCity_Gym_EventScript_GiveTM19
	famechecker FAMECHECKER_ERIKA, 4
	msgbox CeladonCity_Gym_Text_ErikaPostBattle
	release
	end

CeladonCity_Gym_EventScript_DefeatedErika::
	famechecker FAMECHECKER_ERIKA, 1
	clearflag FLAG_HIDE_FAME_CHECKER_ERIKA_JOURNALS
	setflag FLAG_DEFEATED_ERIKA
	setflag FLAG_BADGE04_GET
	set_gym_trainers_frlg 4
	goto CeladonCity_Gym_EventScript_GiveTM19
	end

CeladonCity_Gym_EventScript_GiveTM19::
	msgbox CeladonCity_Gym_Text_ExplainRainbowBadgeTakeThis
	checkitemspace ITEM_TM19
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_Gym_EventScript_NoRoomForTM19
	giveitem_msg CeladonCity_Gym_Text_ReceivedTM19FromErika, ITEM_TM19
	setflag FLAG_GOT_TM19_FROM_ERIKA
	msgbox CeladonCity_Gym_Text_ExplainTM19
	release
	end

CeladonCity_Gym_EventScript_NoRoomForTM19::
	msgbox CeladonCity_Gym_Text_ShouldMakeRoomForThis
	release
	end

CeladonCity_Gym_EventScript_Kay::
	trainerbattle_single TRAINER_LASS_KAY, CeladonCity_Gym_Text_KayIntro, CeladonCity_Gym_Text_KayDefeat
	msgbox CeladonCity_Gym_Text_KayPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Lisa::
	trainerbattle_single TRAINER_LASS_LISA, CeladonCity_Gym_Text_LisaIntro, CeladonCity_Gym_Text_LisaDefeat
	famechecker FAMECHECKER_ERIKA, 2
	msgbox CeladonCity_Gym_Text_LisaPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Tina::
	trainerbattle_single TRAINER_PICNICKER_TINA, CeladonCity_Gym_Text_TinaIntro, CeladonCity_Gym_Text_TinaDefeat
	msgbox CeladonCity_Gym_Text_TinaPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Bridget::
	trainerbattle_single TRAINER_BEAUTY_BRIDGET, CeladonCity_Gym_Text_BridgetIntro, CeladonCity_Gym_Text_BridgetDefeat
	msgbox CeladonCity_Gym_Text_BridgetPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Tamia::
	trainerbattle_single TRAINER_BEAUTY_TAMIA, CeladonCity_Gym_Text_TamiaIntro, CeladonCity_Gym_Text_TamiaDefeat
	famechecker FAMECHECKER_ERIKA, 3
	msgbox CeladonCity_Gym_Text_TamiaPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Lori::
	trainerbattle_single TRAINER_BEAUTY_LORI, CeladonCity_Gym_Text_LoriIntro, CeladonCity_Gym_Text_LoriDefeat
	msgbox CeladonCity_Gym_Text_LoriPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Mary::
	trainerbattle_single TRAINER_COOLTRAINER_MARY, CeladonCity_Gym_Text_MaryIntro, CeladonCity_Gym_Text_MaryDefeat
	msgbox CeladonCity_Gym_Text_MaryPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE04_GET, CeladonCity_Gym_EventScript_GymStatuePostVictory
	msgbox CeladonCity_Gym_Text_GymStatue
	releaseall
	end

CeladonCity_Gym_EventScript_GymStatuePostVictory::
	msgbox CeladonCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

CeladonCity_Gym_Text_ErikaIntro::
	.string "{JPN}はあーい…\n"
	.string "よい おてんきね\l"
	.string "きもち いい…\p"
	.string "… すー… すー\n"
	.string "… あら いけない\l"
	.string "ねて しまったわ… ようこそ\p"
	.string "わたくし タマムシ ジムの\n"
	.string "エリカと もうします\p"
	.string "おはなを いけるのが しゅみで\n"
	.string "ポケモンは くさタイプ ばかり\p"
	.string "… あら やだ\n"
	.string "しあいの もうしこみ ですの?\p"
	.string "そんな…\n"
	.string "わたくし まけませんわよ{PLAY_BGM MUS_RG_ENCOUNTER_GYM_LEADER}$"

CeladonCity_Gym_Text_ErikaDefeat::
	.string "{JPN}…!\n"
	.string "まいり ましたわ\l"
	.string "ほんとうに おつよい かた…\p"
	.string "この レインボーバッジ\n"
	.string "さしあげ なくては なりませんね$"

CeladonCity_Gym_Text_ErikaPostBattle::
	.string "{JPN}あら まあ…\n"
	.string "ずかんを つくって ますのね\l"
	.string "ほんとに えらいわ\p"
	.string "わたし でしたら\n"
	.string "きれいな ポケモンしか\l"
	.string "ほしく なりません もの$"

CeladonCity_Gym_Text_ExplainRainbowBadgeTakeThis::
	.string "{JPN}レインボー バッジで\n"
	.string "レベル50 までの ポケモンが\l"
	.string "おとなしく いうこと きき ます\p"
	.string "そして… かいりきの わざが\n"
	.string "たたかって なくても つかえます\p"
	.string "それから…\n"
	.string "よろしかったら\l"
	.string "これも おつかいに なって!$"

CeladonCity_Gym_Text_ReceivedTM19FromErika::
	.string "{JPN}{PLAYER}は エリカ から\n"
	.string "わざマシン19を もらった!$"

CeladonCity_Gym_Text_ExplainTM19::
	.string "{JPN}わざマシン19の なかは\n"
	.string "ギガドレイン です\p"
	.string "あたえた ダメージの はんぶんが\n"
	.string "ポケモンの えいように なる\l"
	.string "すばらしい わざ です$"

CeladonCity_Gym_Text_ShouldMakeRoomForThis::
	.string "{JPN}おにもつ いっぱい$"

CeladonCity_Gym_Text_KayIntro::
	.string "{JPN}ここは ねー!\p"
	.string "おしとやかな おんなのこ しか\n"
	.string "はいっちゃ いけないの!$"

CeladonCity_Gym_Text_KayDefeat::
	.string "{JPN}らんぼう なんだから$"

CeladonCity_Gym_Text_KayPostBattle::
	.string "{JPN}べー!\n"
	.string "エリカさんに\l"
	.string "やられ ちゃえば いいのよ$"

CeladonCity_Gym_Text_BridgetIntro::
	.string "{JPN}あら いらっしゃい\n"
	.string "たいくつ してたのよ!$"

CeladonCity_Gym_Text_BridgetDefeat::
	.string "{JPN}やんなっちゃう$"

CeladonCity_Gym_Text_BridgetPostBattle::
	.string "{JPN}くさに よわい のは\n"
	.string "みず タイプ だけ じゃ ないわ\p"
	.string "じめん タイプ と いわ タイプも\n"
	.string "わたし たちの えじきよ!$"

CeladonCity_Gym_Text_TinaIntro::
	.string "{JPN}あれー! あんた さっき\n"
	.string "ここ のぞいて なかった…?$"

CeladonCity_Gym_Text_TinaDefeat::
	.string "{JPN}おめめ ぱちくり$"

CeladonCity_Gym_Text_TinaPostBattle::
	.string "{JPN}エリカさんを みてたのか…\n"
	.string "あたしじゃ なかったのね$"

CeladonCity_Gym_Text_TamiaIntro::
	.string "{JPN}ねえ… みて!\n"
	.string "これ わたしの ポケモン!\p"
	.string "くさ タイプは そだてる のが\n"
	.string "らくな ところが いいわ$"

CeladonCity_Gym_Text_TamiaDefeat::
	.string "No!$"

CeladonCity_Gym_Text_TamiaPostBattle::
	.string "{JPN}うちの ジムは つかう ポケモン\n"
	.string "ぜーんぶ くさ タイプ!\p"
	.string "だって ポケモンジムの ほかに\n"
	.string "いけばな きょうしつ やってるから$"

CeladonCity_Gym_Text_LisaIntro::
	.string "{JPN}こら! むし ポケモン とか\n"
	.string "ほのお ポケモン は きらい なの\l"
	.string "ここに もち こまないでよ!$"

CeladonCity_Gym_Text_LisaDefeat::
	.string "{JPN}あ! こいつう!$"

CeladonCity_Gym_Text_LisaPostBattle::
	.string "{JPN}きみねー! リーダーの エリカさんは\n"
	.string "おしとやか だけど このへん じゃ\l"
	.string "ゆうめいな トレーナー なんだから!$"

CeladonCity_Gym_Text_LoriIntro::
	.string "{JPN}あなた ごしゅみは…?\n"
	.string "… はい!\l"
	.string "ポケモンを しょうしょう$"

CeladonCity_Gym_Text_LoriDefeat::
	.string "{JPN}けっこうな おてまえで$"

CeladonCity_Gym_Text_LoriPostBattle::
	.string "{JPN}わたし らいしゅう おみあい なの\n"
	.string "ポケモン さそわれても はしたない\l"
	.string "たたかい しない ように しなきゃ$"

CeladonCity_Gym_Text_MaryIntro::
	.string "{JPN}タマムシ ジムに ようこそ!\p"
	.string "やさしそうな おねえさん たちに\n"
	.string "ゆだん しない ほうが いいわよ!$"

CeladonCity_Gym_Text_MaryDefeat::
	.string "{JPN}やる じゃない!$"

CeladonCity_Gym_Text_MaryPostBattle::
	.string "{JPN}きょうは つよい ポケモン\n"
	.string "もって こなかった から…\p"
	.string "こんど やる ときは まけないわ$"

CeladonCity_Gym_Text_GymStatue::
	.string "{JPN}タマムシ ポケモン ジム\n"
	.string "エリカ にんてい トレーナー!\l"
	.string "{RIVAL}$"

CeladonCity_Gym_Text_GymStatuePlayerWon::
	.string "{JPN}タマムシ ポケモン ジム\n"
	.string "エリカ にんてい トレーナー!\l"
	.string "{RIVAL} {PLAYER}$"

