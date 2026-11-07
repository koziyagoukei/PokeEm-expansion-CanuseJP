CeruleanCity_Gym_Frlg_MapScripts::
	.byte 0

CeruleanCity_Gym_EventScript_Misty::
	famechecker FAMECHECKER_MISTY, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_MISTY, CeruleanCity_Gym_Text_MistyIntro, CeruleanCity_Gym_Text_MistyDefeat, CeruleanCity_Gym_EventScript_MistyDefeated, NO_MUSIC
	goto_if_unset FLAG_GOT_TM03_FROM_MISTY, CeruleanCity_Gym_EventScript_GiveTM03
	msgbox CeruleanCity_Gym_Text_ExplainTM03
	release
	end

CeruleanCity_Gym_EventScript_MistyDefeated::
	famechecker FAMECHECKER_MISTY, 1
	setflag FLAG_DEFEATED_MISTY
	setflag FLAG_BADGE02_GET
	setrespawn HEAL_LOCATION_CERULEAN_CITY
	set_gym_trainers_frlg 2
	goto CeruleanCity_Gym_EventScript_GiveTM03
	end

CeruleanCity_Gym_EventScript_GiveTM03::
	msgbox CeruleanCity_Gym_Text_ExplainCascadeBadge
	checkitemspace ITEM_TM03
	goto_if_eq VAR_RESULT, FALSE, CeruleanCity_Gym_EventScript_NoRoomForTM03
	giveitem_msg CeruleanCity_Gym_Text_ReceivedTM03FromMisty, ITEM_TM03
	setflag FLAG_GOT_TM03_FROM_MISTY
	msgbox CeruleanCity_Gym_Text_ExplainTM03
	release
	end

CeruleanCity_Gym_EventScript_NoRoomForTM03::
	msgbox CeruleanCity_Gym_Text_BetterMakeRoomForThis
	release
	end

CeruleanCity_Gym_EventScript_Diana::
	trainerbattle_single TRAINER_PICNICKER_DIANA, CeruleanCity_Gym_Text_DianaIntro, CeruleanCity_Gym_Text_DianaDefeat
	msgbox CeruleanCity_Gym_Text_DianaPostBattle, MSGBOX_AUTOCLOSE
	end

CeruleanCity_Gym_EventScript_Luis::
	trainerbattle_single TRAINER_SWIMMER_MALE_LUIS, CeruleanCity_Gym_Text_LuisIntro, CeruleanCity_Gym_Text_LuisDefeat
	famechecker FAMECHECKER_MISTY, 2
	msgbox CeruleanCity_Gym_Text_LuisPostBattle, MSGBOX_AUTOCLOSE
	end

CeruleanCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_MISTY, CeruleanCity_Gym_EventScript_GymGuyPostVictory
	msgbox CeruleanCity_Gym_Text_GymGuyAdvice
	release
	end

CeruleanCity_Gym_EventScript_GymGuyPostVictory::
	msgbox CeruleanCity_Gym_Text_WeMakePrettyGoodTeam
	release
	end

CeruleanCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE02_GET, CeruleanCity_Gym_EventScript_GymStatuePostVictory
	msgbox CeruleanCity_Gym_Text_GymStatue
	releaseall
	end

CeruleanCity_Gym_EventScript_GymStatuePostVictory::
	msgbox CeruleanCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

CeruleanCity_Gym_Text_MistyIntro::
	.string "{JPN}あのね きみ!\p"
	.string "ポケモン そだてる にも\n"
	.string "ポリシーが ある やつ だけが\l"
	.string "プロに なれるの!\p"
	.string "あなたは ポケモン つかまえて\n"
	.string "そだてる とき\l"
	.string "なにを かんがえてる?\p"
	.string "わたしの ポリシーはね…\p"
	.string "みず タイプ ポケモンで せめて\n"
	.string "せめて …せめまくる ことよ!{PLAY_BGM MUS_RG_ENCOUNTER_GYM_LEADER}$"

CeruleanCity_Gym_Text_ExplainTM03::
	.string "{JPN}わざマシン03は みずのはどうを\n"
	.string "ポケモンに おしえるの\p"
	.string "みずに すむ ポケモンに\n"
	.string "つかって あげて!$"

CeruleanCity_Gym_Text_ExplainCascadeBadge::
	.string "{JPN}ブルー バッジが あれば\n"
	.string "レベル30までの ポケモンは\l"
	.string "おとなしく いうこと きくわ!\p"
	.string "それが たとえ\n"
	.string "ひとから もらった ポケモンでもね\p"
	.string "それと たたかって なくても\n"
	.string "いあいぎりが つかえるわ\p"
	.string "ほそい きを きりたおして\n"
	.string "とおれる ように なるの\p"
	.string "… あと これは わたしの\n"
	.string "じまんの わざマシンよ!$"

CeruleanCity_Gym_Text_ReceivedTM03FromMisty::
	.string "{JPN}{PLAYER}は カスミから\n"
	.string "わざマシン03を もらった!$"

CeruleanCity_Gym_Text_BetterMakeRoomForThis::
	.string "{JPN}にもつ いっぱいね$"

CeruleanCity_Gym_Text_MistyDefeat::
	.string "{JPN}うーん…!\n"
	.string "わたしの まけね\p"
	.string "しょうが ない!\p"
	.string "わたしに かった しょうこに\n"
	.string "ブルー バッジを あげる!$"

CeruleanCity_Gym_Text_DianaIntro::
	.string "{JPN}きみ なんて\n"
	.string "あたしで じゅうぶん!\p"
	.string "カスミが でる まく じゃないわ$"

CeruleanCity_Gym_Text_DianaDefeat::
	.string "{JPN}まいったわ!$"

CeruleanCity_Gym_Text_DianaPostBattle::
	.string "{JPN}いろんな ポケモン トレーナーと\n"
	.string "たたかって みないと\l"
	.string "じぶんの つよさ わからない ものね$"

CeruleanCity_Gym_Text_LuisIntro::
	.string "{JPN}おわっぷ!\p"
	.string "まずは おれが あいてだ!\n"
	.string "かかって こい!$"

CeruleanCity_Gym_Text_LuisDefeat::
	.string "{JPN}こんな はずは ない$"

CeruleanCity_Gym_Text_LuisPostBattle::
	.string "{JPN}カスミは これから まだまだ\n"
	.string "つよくなる トレーナーだ!\p"
	.string "おまえ なんかにゃ\n"
	.string "まけたり しないよ$"

CeruleanCity_Gym_Text_GymGuyAdvice::
	.string "{JPN}おーす!\n"
	.string "みらいの チャンピオン!\p"
	.string "アドバイス しよう!\p"
	.string "ここの リーダー カスミは\n"
	.string "みずに すむ ポケモンを つかう\l"
	.string "プロフェッショナルだ!\p"
	.string "こんな ときは しょくぶつ タイプで\n"
	.string "みずを すいとる さくせんだ\p"
	.string "… それか でんき タイプで\n"
	.string "しびれ させるのも いいぜ$"

CeruleanCity_Gym_Text_WeMakePrettyGoodTeam::
	.string "{JPN}カスミに かったな!\n"
	.string "おれの いった とおり だったろ?\p"
	.string "おまえも すごいが\n"
	.string "おれも すごいだろ?$"

CeruleanCity_Gym_Text_GymStatue::
	.string "{JPN}ハナダ ポケモン ジム\n"
	.string "カスミ にんてい トレーナー!\l"
	.string "{RIVAL}$"

CeruleanCity_Gym_Text_GymStatuePlayerWon::
	.string "{JPN}ハナダ ポケモン ジム\n"
	.string "カスミ にんてい トレーナー!\l"
	.string "{RIVAL} {PLAYER}$"

