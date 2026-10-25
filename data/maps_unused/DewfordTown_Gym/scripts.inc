DewfordTown_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, DewfordTown_Gym_OnTransition
	.byte 0

DewfordTown_Gym_OnTransition:
	call DewfordTown_Gym_EventScript_SetFlashLevel
	end

DewfordTown_Gym_EventScript_SetFlashLevel::
	goto_if_defeated TRAINER_BRAWLY_1, DewfordTown_Gym_EventScript_SetLightsOn
	call DewfordTown_Gym_EventScript_CountTrainersDefeated
	copyvar VAR_0x8001, VAR_0x8000
	goto_if_eq VAR_0x8000, 0, DewfordTown_Gym_EventScript_SetFlashLevel7
	goto_if_eq VAR_0x8000, 1, DewfordTown_Gym_EventScript_SetFlashLevel6
	goto_if_eq VAR_0x8000, 2, DewfordTown_Gym_EventScript_SetFlashLevel5
	goto_if_eq VAR_0x8000, 3, DewfordTown_Gym_EventScript_SetFlashLevel4
	goto_if_eq VAR_0x8000, 4, DewfordTown_Gym_EventScript_SetFlashLevel3
	goto_if_eq VAR_0x8000, 5, DewfordTown_Gym_EventScript_SetFlashLevel2
	goto DewfordTown_Gym_EventScript_SetFlashLevel1

DewfordTown_Gym_EventScript_SetLightsOn::
	setflashlevel 0
	return

@ Brightest
DewfordTown_Gym_EventScript_SetFlashLevel1::
	setflashlevel 1
	return

DewfordTown_Gym_EventScript_SetFlashLevel2::
	setflashlevel 2
	return

DewfordTown_Gym_EventScript_SetFlashLevel3::
	setflashlevel 3
	return

DewfordTown_Gym_EventScript_SetFlashLevel4::
	setflashlevel 4
	return

DewfordTown_Gym_EventScript_SetFlashLevel5::
	setflashlevel 5
	return

DewfordTown_Gym_EventScript_SetFlashLevel6::
	setflashlevel 6
	return

@ Darkest
DewfordTown_Gym_EventScript_SetFlashLevel7::
	setflashlevel 7
	return

DewfordTown_Gym_EventScript_BrightenRoom::
	call DewfordTown_Gym_EventScript_CountTrainersDefeated
	nop1
	goto_if_eq VAR_0x8000, VAR_0x8001, DewfordTown_Gym_EventScript_NoLightChange
	copyvar VAR_0x8001, VAR_0x8000
	goto_if_eq VAR_0x8000, 1, DewfordTown_Gym_EventScript_AnimateFlash1Trainer
	goto_if_eq VAR_0x8000, 2, DewfordTown_Gym_EventScript_AnimateFlash2Trainers
	goto_if_eq VAR_0x8000, 3, DewfordTown_Gym_EventScript_AnimateFlash3Trainers
	goto_if_eq VAR_0x8000, 4, DewfordTown_Gym_EventScript_AnimateFlash4Trainers
	goto_if_eq VAR_0x8000, 5, DewfordTown_Gym_EventScript_AnimateFlash5Trainers
	goto_if_eq VAR_0x8000, 6, DewfordTown_Gym_EventScript_AnimateFlash6Trainers

DewfordTown_Gym_EventScript_NoLightChange::
	return

@ NOTE: A little confusingly, a larger animateflash param value is a smaller flash radius
DewfordTown_Gym_EventScript_AnimateFlash1Trainer::
	playse SE_SWITCH
	animateflash 6
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash2Trainers::
	playse SE_SWITCH
	animateflash 5
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash3Trainers::
	playse SE_SWITCH
	animateflash 4
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash4Trainers::
	playse SE_SWITCH
	animateflash 3
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash5Trainers::
	playse SE_SWITCH
	animateflash 2
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash6Trainers::
	playse SE_SWITCH
	animateflash 1
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlashFullBrightness::
	playse SE_SWITCH
	animateflash 0
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_CountTrainersDefeated::
	setvar VAR_0x8000, 0
	goto_if_not_defeated TRAINER_TAKAO, DewfordTown_Gym_EventScript_CheckJocelyn
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckJocelyn::
	goto_if_not_defeated TRAINER_JOCELYN, DewfordTown_Gym_EventScript_CheckLaura
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckLaura::
	goto_if_not_defeated TRAINER_LAURA, DewfordTown_Gym_EventScript_CheckBrenden
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckBrenden::
	goto_if_not_defeated TRAINER_BRENDEN, DewfordTown_Gym_EventScript_CheckCristian
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckCristian::
	goto_if_not_defeated TRAINER_CRISTIAN, DewfordTown_Gym_EventScript_CheckLilith
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckLilith::
	goto_if_not_defeated TRAINER_LILITH, DewfordTown_Gym_EventScript_StopCountingTrainers
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_StopCountingTrainers::
	return

DewfordTown_Gym_EventScript_Brawly::
	trainerbattle_single TRAINER_BRAWLY_1, DewfordTown_Gym_Text_BrawlyIntro, DewfordTown_Gym_Text_BrawlyDefeat, DewfordTown_Gym_EventScript_BrawlyDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, DewfordTown_Gym_EventScript_BrawlyRematch
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, DewfordTown_Gym_EventScript_GiveBulkUp2
	msgbox DewfordTown_Gym_Text_BrawlyPostBattle, MSGBOX_DEFAULT
	release
	end

DewfordTown_Gym_EventScript_BrawlyDefeated::
	call DewfordTown_Gym_EventScript_AnimateFlashFullBrightness
	message DewfordTown_Gym_Text_ReceivedKnuckleBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox DewfordTown_Gym_Text_KnuckleBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_DEWFORD_GYM
	setflag FLAG_BADGE02_GET
	addvar VAR_PETALBURG_GYM_STATE, 1
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	setvar VAR_0x8008, 2
	call Common_EventScript_SetGymTrainers
	call DewfordTown_Gym_EventScript_GiveBulkUp
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox DewfordTown_Gym_Text_RegisteredBrawly, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_BRAWLY_MATCH_CALL
	setvar VAR_ROXANNE_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

DewfordTown_Gym_EventScript_GiveBulkUp::
	giveitem ITEM_TM_BULK_UP
	goto_if_eq VAR_RESULT, 0, Common_EventScript_BagIsFull
	msgbox DewfordTown_Gym_Text_ExplainBulkUp, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_BULK_UP
	return

DewfordTown_Gym_EventScript_GiveBulkUp2:
	giveitem ITEM_TM_BULK_UP
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	msgbox DewfordTown_Gym_Text_ExplainBulkUp, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_BULK_UP
	release
	end

DewfordTown_Gym_EventScript_BrawlyRematch::
	trainerbattle_rematch_double TRAINER_BRAWLY_1, DewfordTown_Gym_Text_BrawlyPreRematch, DewfordTown_Gym_Text_BrawlyRematchDefeat, DewfordTown_Gym_Text_BrawlyRematchNeedTwoMons
	msgbox DewfordTown_Gym_Text_BrawlyPostRematch, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_Takao::
	trainerbattle_single TRAINER_TAKAO, DewfordTown_Gym_Text_TakaoIntro, DewfordTown_Gym_Text_TakaoDefeat, DewfordTown_Gym_EventScript_TakaoBrightenRoom
	msgbox DewfordTown_Gym_Text_TakaoPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_TakaoBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Jocelyn::
	trainerbattle_single TRAINER_JOCELYN, DewfordTown_Gym_Text_JocelynIntro, DewfordTown_Gym_Text_JocelynDefeat, DewfordTown_Gym_EventScript_JocelynBrightenRoom
	msgbox DewfordTown_Gym_Text_JocelynPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_JocelynBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Laura::
	trainerbattle_single TRAINER_LAURA, DewfordTown_Gym_Text_LauraIntro, DewfordTown_Gym_Text_LauraDefeat, DewfordTown_Gym_EventScript_LauraBrightenRoom
	msgbox DewfordTown_Gym_Text_LauraPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_LauraBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Brenden::
	trainerbattle_single TRAINER_BRENDEN, DewfordTown_Gym_Text_BrendenIntro, DewfordTown_Gym_Text_BrendenDefeat, DewfordTown_Gym_EventScript_BrendenBrightenRoom
	msgbox DewfordTown_Gym_Text_BrendenPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_BrendenBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Cristian::
	trainerbattle_single TRAINER_CRISTIAN, DewfordTown_Gym_Text_CristianIntro, DewfordTown_Gym_Text_CristianDefeat, DewfordTown_Gym_EventScript_CristianBrightenRoom
	msgbox DewfordTown_Gym_Text_CristianPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_CristianBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Lilith::
	trainerbattle_single TRAINER_LILITH, DewfordTown_Gym_Text_LilithIntro, DewfordTown_Gym_Text_LilithDefeat, DewfordTown_Gym_EventScript_LilithBrightenRoom
	msgbox DewfordTown_Gym_Text_LilithPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_LilithBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_DEWFORD_GYM, DewfordTown_Gym_EventScript_GymGuidePostVictory
	msgbox DewfordTown_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

DewfordTown_Gym_EventScript_GymGuidePostVictory::
	msgbox DewfordTown_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

DewfordTown_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE02_GET, DewfordTown_Gym_EventScript_GymStatueCertified
	goto DewfordTown_Gym_EventScript_GymStatue
	end

DewfordTown_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE02_GET, DewfordTown_Gym_EventScript_GymStatueCertified
	goto DewfordTown_Gym_EventScript_GymStatue
	end

DewfordTown_Gym_EventScript_GymStatueCertified::
	msgbox DewfordTown_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Gym_EventScript_GymStatue::
	msgbox DewfordTown_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Gym_Text_GymGuideAdvice:
	.string "げんき しとぉや!!\n"
	.string "{JPN}チャンピオンを めざす {PLAYER}!!\p"
	.string "{JPN}ムロタウン ジムの リーダー トウキは\n"
	.string "{JPN}かくとうタイプ ポケモンの つかいてだ!\p"
	.string "{JPN}ノーマルタイプで むかうと\n"
	.string "{JPN}きびしぃー いちげきを くらうから\l"
	.string "{JPN}きを つけたほうが いいぞ!\p"
	.string "{JPN}しかも! この ジムの なかは\n"
	.string "{JPN}うみのそこ みたいに くらいんだ!\p"
	.string "{JPN}でも! トレーナーを ひとり たおすごとに\n"
	.string "{JPN}だんだん あかるく なって いくぞ!\p"
	.string "{JPN}じゃ きばってこォー!!$"

DewfordTown_Gym_Text_GymGuidePostVictory:
	.string "うおォ! ずいぶん あかるく なったな!\p"
	.string "{JPN}きみの みらいも あかるいぞ!$"

DewfordTown_Gym_Text_TakaoIntro:
	.string "ちから こそ せいぎっ!\n"
	.string "{JPN}ドリャーッ!!$"

DewfordTown_Gym_Text_TakaoDefeat:
	.string "ちからぶそく だった……!$"

DewfordTown_Gym_Text_TakaoPostBattle:
	.string "じゅう よく ごうを せいす!\n"
	.string "{JPN}みごとな たたかいぶり だった!$"

DewfordTown_Gym_Text_JocelynIntro:
	.string "トウキさまと たたかうまでも ないわ!\n"
	.string "{JPN}わたしが たおして あげる!$"

DewfordTown_Gym_Text_JocelynDefeat:
	.string "まさか……\n"
	.string "{JPN}こんなに かんたんに やられるなんて$"

DewfordTown_Gym_Text_JocelynPostBattle:
	.string "この まけを むだに しないため\n"
	.string "{JPN}もっと とっくん する!\p"
	.string "{JPN}そして いつかは ジムリーダーに……\p"
	.string "{JPN}いえ ポケモンリーグの\n"
	.string "{JPN}チャンピオンに なるわよ!$"

DewfordTown_Gym_Text_LauraIntro:
	.string "あたしを あまく みない ことね!\n"
	.string "{JPN}ないても しらないわよ!$"

DewfordTown_Gym_Text_LauraDefeat:
	.string "わぁーーーん!\n"
	.string "{JPN}まけたーーーっ!$"

DewfordTown_Gym_Text_LauraPostBattle:
	.string "くらやみの なかで たたかうことで\n"
	.string "{JPN}こころを きたえるの\p"
	.string "{JPN}でも まっくら だから\n"
	.string "{JPN}トウキさまの おかおを みられない……$"

DewfordTown_Gym_Text_LilithIntro:
	.string "うふふ……!\n"
	.string "{JPN}くらくて まよっているの かな?$"

DewfordTown_Gym_Text_LilithDefeat:
	.string "なによ もう\n"
	.string "{JPN}しらない!$"

DewfordTown_Gym_Text_LilithPostBattle:
	.string "あなた なんて このまま まよって\n"
	.string "{JPN}いりぐちに もどっちゃえば いーのよ!$"

DewfordTown_Gym_Text_BrendenIntro:
	.string "うみの おとこの いじを\n"
	.string "{JPN}みせつけて やるぜぃ!$"

DewfordTown_Gym_Text_BrendenDefeat:
	.string "どうしたぃ!\n"
	.string "{JPN}おれと した ことが……$"

DewfordTown_Gym_Text_BrendenPostBattle:
	.string "おう! あんたなら\n"
	.string "{JPN}ジムリーダーに いどむに ふさわしいぜぃ!$"

DewfordTown_Gym_Text_CristianIntro:
	.string "ここを とおる なら\n"
	.string "{JPN}この わしを たおしてから じゃ!$"

DewfordTown_Gym_Text_CristianDefeat:
	.string "むうーん!\n"
	.string "{JPN}すごく つよい……!$"

DewfordTown_Gym_Text_CristianPostBattle:
	.string "わしの まけじゃ!\n"
	.string "{JPN}がんばって バッジ とってこいよ!$"

DewfordTown_Gym_Text_BrawlyIntro:
	.string "ボクは ムロ ポケモンジム\n"
	.string "{JPN}リーダーの トウキ!\p"
	.string "{JPN}この あたりの\n"
	.string "{JPN}はげしい なみに もまれ……\p"
	.string "{JPN}くらい どうくつで\n"
	.string "{JPN}しゅぎょう して いるのさ!\p"
	.string "{JPN}ボクに ちょうせん しよう という\n"
	.string "{JPN}キミの じつりょく……\l"
	.string "{JPN}どれほどの ものか みせて もらうよ!$"

DewfordTown_Gym_Text_BrawlyDefeat:
	.string "ううーん よそう していたより\n"
	.string "{JPN}キミは ビッグウェーブ!\p"
	.string "{JPN}わかった!\n"
	.string "{JPN}この バッジを もっていきな!$"

DewfordTown_Gym_Text_ReceivedKnuckleBadge:
	.string "{PLAYER}は トウキから\n"
	.string "{JPN}ナックルバッジを もらった!$"

DewfordTown_Gym_Text_KnuckleBadgeInfoTakeThis:
	.string "ナックルバッジには\n"
	.string "{JPN}レベル30までの ポケモンなら\l"
	.string "{JPN}ひとから もらった ポケモンでも\l"
	.string "{JPN}すなおに する こうかが ある!\p"
	.string "{JPN}それに たたかって いなくても\n"
	.string "{JPN}ひでんわざの フラッシュを つかえる!\p"
	.string "{JPN}この わざマシン\n"
	.string "{JPN}よかったら つかいな!$"

DewfordTown_Gym_Text_ExplainBulkUp:
	.string "その わざマシン08 には\n"
	.string "{JPN}ビルドアップが はいっている!\p"
	.string "{JPN}こうげきりょくと ぼうぎょりょくを\n"
	.string "{JPN}あげる わざ なのさ!\p"
	.string "{JPN}…… …… ……$"

DewfordTown_Gym_Text_RegisteredBrawly:
	.string "ジムリーダー トウキを\n"
	.string "{JPN}ポケナビに とうろく した!$"

DewfordTown_Gym_Text_BrawlyPostBattle:
	.string "キミの さいのうは\n"
	.string "{JPN}いつのひか おおきな なみと なって\p"
	.string "{JPN}ポケモン トレーナーたちの あいだに\n"
	.string "{JPN}おどろきの あらしを まきおこすだろう!$"

DewfordTown_Gym_Text_GymStatue:
	.string "ムロタウン ポケモンジム$"

DewfordTown_Gym_Text_GymStatueCertified:
	.string "ムロタウン ポケモンジム\p"
	.string "{JPN}トウキ にんてい トレーナー!\n"
	.string "{JPN}{PLAYER}$"

DewfordTown_Gym_Text_BrawlyPreRematch:
	.string "トウキ『なみというのは いちど ひいても\n"
	.string "{JPN}いつか かならず はまべに もどってくる\p"
	.string "{JPN}キミほどの ビッグウェーブだ\n"
	.string "{JPN}きっと また くると おもっていたよ!\p"
	.string "{JPN}さあ どれほど たかさを ましたのか\n"
	.string "{JPN}ボクに みせてくれ!$"

DewfordTown_Gym_Text_BrawlyRematchDefeat:
	.string "わーおっ!\n"
	.string "{JPN}キミに のみこまれそうだ!$"

DewfordTown_Gym_Text_BrawlyPostRematch:
	.string "トウキ『やっぱり ポケモン しょうぶは\n"
	.string "{JPN}そこが みえないほど ふかく ひろい!\l"
	.string "{JPN}まるで ホウエンの うみの ようだね!$"

DewfordTown_Gym_Text_BrawlyRematchNeedTwoMons:
	.string "トウキ『なみというのは いちど ひいても\n"
	.string "{JPN}いつか かならず はまべに もどってくる\p"
	.string "{JPN}キミほどの ビッグウェーブだ\n"
	.string "{JPN}きっと また くると おもっていたよ!\p"
	.string "{JPN}……あれ? たたかえる ポケモンを\n"
	.string "{JPN}1ひきしか もっていないんだね\p"
	.string "{JPN}すまないけど 2ひき いじょうの\n"
	.string "{JPN}ポケモンを もって また きてくれよ$"
