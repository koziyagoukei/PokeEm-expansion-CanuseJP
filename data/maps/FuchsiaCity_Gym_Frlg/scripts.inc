FuchsiaCity_Gym_Frlg_MapScripts::
	.byte 0

FuchsiaCity_Gym_EventScript_Koga::
	famechecker FAMECHECKER_KOGA, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_KOGA, FuchsiaCity_Gym_Text_KogaIntro, FuchsiaCity_Gym_Text_KogaDefeat, FuchsiaCity_Gym_EventScript_DefeatedKoga, NO_MUSIC
	goto_if_unset FLAG_GOT_TM06_FROM_KOGA, FuchsiaCity_Gym_EventScript_GiveTM06
	msgbox FuchsiaCity_Gym_Text_KogaPostBattle
	release
	end

FuchsiaCity_Gym_EventScript_DefeatedKoga::
	famechecker FAMECHECKER_KOGA, 1
	clearflag FLAG_HIDE_FAME_CHECKER_KOGA_JOURNAL
	setflag FLAG_DEFEATED_KOGA
	setflag FLAG_BADGE05_GET
	set_gym_trainers_frlg 5
	goto FuchsiaCity_Gym_EventScript_GiveTM06
	end

FuchsiaCity_Gym_EventScript_GiveTM06::
	msgbox FuchsiaCity_Gym_Text_KogaExplainSoulBadge
	checkitemspace ITEM_TM06
	goto_if_eq VAR_RESULT, FALSE, FuchsiaCity_Gym_EventScript_NoRoomForTM06
	giveitem_msg FuchsiaCity_Gym_Text_ReceivedTM06FromKoga, ITEM_TM06
	setflag FLAG_GOT_TM06_FROM_KOGA
	msgbox FuchsiaCity_Gym_Text_KogaExplainTM06
	release
	end

FuchsiaCity_Gym_EventScript_NoRoomForTM06::
	msgbox FuchsiaCity_Gym_Text_MakeSpaceForThis
	release
	end

FuchsiaCity_Gym_EventScript_Phil::
	trainerbattle_single TRAINER_TAMER_PHIL, FuchsiaCity_Gym_Text_PhilIntro, FuchsiaCity_Gym_Text_PhilDefeat
	msgbox FuchsiaCity_Gym_Text_PhilPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Edgar::
	trainerbattle_single TRAINER_TAMER_EDGAR, FuchsiaCity_Gym_Text_EdgarIntro, FuchsiaCity_Gym_Text_EdgarDefeat
	msgbox FuchsiaCity_Gym_Text_EdgarPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Kirk::
	trainerbattle_single TRAINER_JUGGLER_KIRK, FuchsiaCity_Gym_Text_KirkIntro, FuchsiaCity_Gym_Text_KirkDefeat
	famechecker FAMECHECKER_KOGA, 2
	msgbox FuchsiaCity_Gym_Text_KirkPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Shawn::
	trainerbattle_single TRAINER_JUGGLER_SHAWN, FuchsiaCity_Gym_Text_ShawnIntro, FuchsiaCity_Gym_Text_ShawnDefeat
	msgbox FuchsiaCity_Gym_Text_ShawnPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Kayden::
	trainerbattle_single TRAINER_JUGGLER_KAYDEN, FuchsiaCity_Gym_Text_KaydenIntro, FuchsiaCity_Gym_Text_KaydenDefeat
	msgbox FuchsiaCity_Gym_Text_KaydenPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Nate::
	trainerbattle_single TRAINER_JUGGLER_NATE, FuchsiaCity_Gym_Text_NateIntro, FuchsiaCity_Gym_Text_NateDefeat
	msgbox FuchsiaCity_Gym_Text_NatePostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_KOGA, FuchsiaCity_Gym_EventScript_GymGuyPostVictory
	msgbox FuchsiaCity_Gym_Text_GymGuyAdvice
	release
	end

FuchsiaCity_Gym_EventScript_GymGuyPostVictory::
	msgbox FuchsiaCity_Gym_Text_GymGuyPostVictory
	release
	end

FuchsiaCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE05_GET, FuchsiaCity_Gym_EventScript_GymStatuePostVictory
	msgbox FuchsiaCity_Gym_Text_GymStatue
	releaseall
	end

FuchsiaCity_Gym_EventScript_GymStatuePostVictory::
	msgbox FuchsiaCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

FuchsiaCity_Gym_Text_KogaIntro::
	.string "{JPN}キョウ『… ファ ファ ファ!\p"
	.string "こわっぱ ごときが\n"
	.string "せっしゃに たたかいを いどむとは\l"
	.string "かたはら いたいわ!\p"
	.string "… どくを くらったら じめつ!\n"
	.string "ねむって しまったら むていこう\p"
	.string "… しのびの わざの ごくい!\n"
	.string "どくポケモンの おそろしさ!\l"
	.string "うけて みるが よい!{PLAY_BGM MUS_RG_ENCOUNTER_GYM_LEADER}$"

FuchsiaCity_Gym_Text_KogaDefeat::
	.string "{JPN}ふん…!\n"
	.string "おぬし やりおるな!\p"
	.string "そら!\n"
	.string "ピンク バッジを うけとれ!$"

FuchsiaCity_Gym_Text_KogaPostBattle::
	.string "{JPN}どくどくは どくを あびたら\n"
	.string "1ターン ごとに どんどん\l"
	.string "ダメージが おおきく なる!\p"
	.string "これは あいてに\n"
	.string "きょうふを あたえる わざ なのだ!$"

FuchsiaCity_Gym_Text_KogaExplainSoulBadge::
	.string "{JPN}ピンク バッジを もって おれば\n"
	.string "おぬしの ポケモンの\l"
	.string "ぼうぎょ りょくを あげる!\p"
	.string "たたかって ない ときでも\n"
	.string "なみのりの わざを\l"
	.string "つかえる ように なる!\p"
	.string "そうだ…! これを おぬしに!$"

FuchsiaCity_Gym_Text_ReceivedTM06FromKoga::
	.string "{JPN}{PLAYER}は キョウ から\n"
	.string "わざマシン06を もらった!$"

FuchsiaCity_Gym_Text_KogaExplainTM06::
	.string "{JPN}400ねん むかし より\n"
	.string "わがやに つたわる ポケモン わざ\p"
	.string "わざマシン06には ごくい!\n"
	.string "どくどくが ふういん してある!$"

FuchsiaCity_Gym_Text_MakeSpaceForThis::
	.string "{JPN}にもつが いっぱいだ$"

FuchsiaCity_Gym_Text_KaydenIntro::
	.string "{JPN}ただ つよい だけでは だめだ!\n"
	.string "… わかるか?\p"
	.string "ポケモンは テクニックだ!\p"
	.string "この ジムに きた から には\n"
	.string "そういう ポケモンの おく ふかさ\l"
	.string "タップリ おしえて やる!$"

FuchsiaCity_Gym_Text_KaydenDefeat::
	.string "{JPN}…!\n"
	.string "ただもの では ない!$"

FuchsiaCity_Gym_Text_KaydenPostBattle::
	.string "{JPN}こどもの トレーナー とはいえ\n"
	.string "パワー だけでなく\l"
	.string "テクニックも みがいてるな!$"

FuchsiaCity_Gym_Text_KirkIntro::
	.string "{JPN}マジシャン だった おれは\n"
	.string "ニンジャに あこがれて\l"
	.string "この セキチク ジムに はいった!$"

FuchsiaCity_Gym_Text_KirkDefeat::
	.string "{JPN}… やられた!$"

FuchsiaCity_Gym_Text_KirkPostBattle::
	.string "{JPN}たとえ まけても おれは\n"
	.string "ニンジャ マスター\l"
	.string "キョウの おしえに いきる つもりだ$"

FuchsiaCity_Gym_Text_NateIntro::
	.string "{JPN}おれの ポケモン あやしの わざ\n"
	.string "たっぷりと あじわいな!$"

FuchsiaCity_Gym_Text_NateDefeat::
	.string "{JPN}… そうか!\n"
	.string "おまえも わざ つかいか?$"

FuchsiaCity_Gym_Text_NatePostBattle::
	.string "{JPN}たたかい おわった あとでも\n"
	.string "どく ねむり といった こうかは\l"
	.string "のこる ところが すき だぜ!$"

FuchsiaCity_Gym_Text_PhilIntro::
	.string "{JPN}おっと! まちな!\p"
	.string "セキチク ジムの めいぶつ\n"
	.string "みえない かべ システムは どうだ$"

FuchsiaCity_Gym_Text_PhilDefeat::
	.string "{JPN}… ほお!\n"
	.string "やる じゃないか!$"

FuchsiaCity_Gym_Text_PhilPostBattle::
	.string "{JPN}おまえは ほねが ありそう だ!\n"
	.string "とくべつに ヒントを おしえよう!\p"
	.string "みえない かべは よく みると\n"
	.string "しきりが みえて くる…!$"

FuchsiaCity_Gym_Text_EdgarIntro::
	.string "{JPN}この おれも\n"
	.string "ニンジャ キョウの もんかせい!\p"
	.string "ニンジャは むかし から\n"
	.string "よく どうぶつを つかった という$"

FuchsiaCity_Gym_Text_EdgarDefeat::
	.string "{JPN}がおーん…!$"

FuchsiaCity_Gym_Text_EdgarPostBattle::
	.string "{JPN}まだ まだ!\n"
	.string "おれには しゅぎょうが たりない$"

FuchsiaCity_Gym_Text_ShawnIntro::
	.string "{JPN}ここの リーダー キョウは\n"
	.string "イガ ニンジャの しそんだ!\p"
	.string "おいッ!\n"
	.string "おまえは なにの しそん なんだ?$"

FuchsiaCity_Gym_Text_ShawnDefeat::
	.string "{JPN}なかなか できるな!$"

FuchsiaCity_Gym_Text_ShawnPostBattle::
	.string "{JPN}… ひかり ある ところに\n"
	.string "かげが ある!\p"
	.string "ひかりと かげ!\n"
	.string "おまえは どちらを えらぶ?$"

FuchsiaCity_Gym_Text_GymGuyAdvice::
	.string "{JPN}おーす!\n"
	.string "みらいの チャンピオン!\p"
	.string "セキチク ジムは からくり やしき\n"
	.string "みえない かべで しきられてる!\p"
	.string "キョウの やつは すぐ そこに\n"
	.string "いる ように みえるが…\p"
	.string "みえない かべの とぎれてる とこを\n"
	.string "さがし ださないと あえないぜ!$"

FuchsiaCity_Gym_Text_GymGuyPostVictory::
	.string "{JPN}げんだいに なお いき のこる\n"
	.string "ニンジャの こわさを みたな!$"

FuchsiaCity_Gym_Text_GymStatue::
	.string "{JPN}セキチク ポケモン ジム\n"
	.string "キョウ にんてい トレーナー!\l"
	.string "{RIVAL}$"

FuchsiaCity_Gym_Text_GymStatuePlayerWon::
	.string "{JPN}セキチク ポケモン ジム\n"
	.string "キョウ にんてい トレーナー!\l"
	.string "{RIVAL} {PLAYER}$"

