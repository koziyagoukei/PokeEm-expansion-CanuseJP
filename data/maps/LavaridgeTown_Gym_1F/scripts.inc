LavaridgeTown_Gym_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LavaridgeTown_Gym_1F_OnTransition
	.byte 0

LavaridgeTown_Gym_1F_OnTransition:
	call LavaridgeTown_Gym_1F_EventScript_SetTrainerTempVars
	call LavaridgeTown_Gym_1F_EventScript_CheckBuryTrainers
	end

@ Unclear where/if these temp vars are getting checked
LavaridgeTown_Gym_1F_EventScript_SetTrainerTempVars::
	setvar VAR_TEMP_B, 0
	setvar VAR_TEMP_C, 0
	setvar VAR_TEMP_D, 0
	setvar VAR_TEMP_E, 0
	setvar VAR_TEMP_F, 0
	goto_if_defeated TRAINER_COLE, LavaridgeTown_Gym_1F_EventScript_SetGeraldTempVar
	setvar VAR_TEMP_B, 1
LavaridgeTown_Gym_1F_EventScript_SetGeraldTempVar::
	goto_if_defeated TRAINER_GERALD, LavaridgeTown_Gym_1F_EventScript_SetAxleTempVar
	setvar VAR_TEMP_C, 1
LavaridgeTown_Gym_1F_EventScript_SetAxleTempVar::
	goto_if_defeated TRAINER_AXLE, LavaridgeTown_Gym_1F_EventScript_SetDanielleTempVar
	setvar VAR_TEMP_D, 1
LavaridgeTown_Gym_1F_EventScript_SetDanielleTempVar::
	goto_if_defeated TRAINER_DANIELLE, LavaridgeTown_Gym_1F_EventScript_EndSetTrainerTempVars
	setvar VAR_TEMP_E, 1
LavaridgeTown_Gym_1F_EventScript_EndSetTrainerTempVars::
	return

LavaridgeTown_Gym_1F_EventScript_CheckBuryTrainers::
	goto_if_defeated TRAINER_COLE, LavaridgeTown_Gym_1F_EventScript_CheckBuryGerald
	setobjectmovementtype LOCALID_COLE, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_CheckBuryGerald::
	goto_if_defeated TRAINER_GERALD, LavaridgeTown_Gym_1F_EventScript_CheckBuryAxle
	setobjectmovementtype LOCALID_GERALD, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_CheckBuryAxle::
	goto_if_defeated TRAINER_AXLE, LavaridgeTown_Gym_1F_EventScript_CheckBuryDanielle
	setobjectmovementtype LOCALID_AXLE, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_CheckBuryDanielle::
	goto_if_defeated TRAINER_DANIELLE, LavaridgeTown_Gym_1F_EventScript_EndCheckBuryTrainers
	setobjectmovementtype LOCALID_DANIELLE, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_EndCheckBuryTrainers::
	return

LavaridgeTown_Gym_1F_EventScript_Flannery::
	trainerbattle_single TRAINER_FLANNERY_1, LavaridgeTown_Gym_1F_Text_FlanneryIntro, LavaridgeTown_Gym_1F_Text_FlanneryDefeat, LavaridgeTown_Gym_1F_EventScript_FlanneryDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, LavaridgeTown_Gym_1F_EventScript_FlanneryRematch
	goto_if_unset FLAG_RECEIVED_TM_OVERHEAT, LavaridgeTown_Gym_1F_EventScript_GiveOverheat2
	msgbox LavaridgeTown_Gym_1F_Text_FlanneryPostBattle, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_Gym_1F_EventScript_FlanneryDefeated::
	message LavaridgeTown_Gym_1F_Text_ReceivedHeatBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LavaridgeTown_Gym_1F_Text_ExplainHeatBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_WHITEOUT_TO_LAVARIDGE
	setflag FLAG_DEFEATED_LAVARIDGE_GYM
	setflag FLAG_BADGE04_GET
	addvar VAR_PETALBURG_GYM_STATE, 1
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	setvar VAR_0x8008, 4
	call Common_EventScript_SetGymTrainers
	setflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY
	setvar VAR_LAVARIDGE_TOWN_STATE, 1
	call LavaridgeTown_Gym_1F_EventScript_GiveOverheat
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LavaridgeTown_Gym_1F_Text_RegisteredFlannery, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_FLANNERY_MATCH_CALL
	release
	end

LavaridgeTown_Gym_1F_EventScript_GiveOverheat2::
	giveitem ITEM_TM_OVERHEAT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox LavaridgeTown_Gym_1F_Text_ExplainOverheat, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_OVERHEAT
	release
	end

LavaridgeTown_Gym_1F_EventScript_GiveOverheat::
	giveitem ITEM_TM_OVERHEAT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox LavaridgeTown_Gym_1F_Text_ExplainOverheat, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_OVERHEAT
	return

LavaridgeTown_Gym_1F_EventScript_FlanneryRematch::
	trainerbattle_rematch_double TRAINER_FLANNERY_1, LavaridgeTown_Gym_1F_Text_FlanneryPreRematch, LavaridgeTown_Gym_1F_Text_FlanneryRematchDefeat, LavaridgeTown_Gym_1F_Text_FlanneryRematchNeedTwoMons
	msgbox LavaridgeTown_Gym_1F_Text_FlanneryPostRematch, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_Cole::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, LOCALID_COLE, TRAINER_COLE, LavaridgeTown_Gym_1F_Text_ColeIntro, LavaridgeTown_Gym_1F_Text_ColeDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
	msgbox LavaridgeTown_Gym_1F_Text_ColePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_EventScript_CheckTrainerScript::
	call LavaridgeTown_Gym_1F_EventScript_SetTrainerTempVars
	release
	special ShouldTryGetTrainerScript
	goto_if_eq VAR_RESULT, 1, EventScript_GotoTrainerScript
	end

LavaridgeTown_Gym_1F_EventScript_Axle::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, LOCALID_AXLE, TRAINER_AXLE, LavaridgeTown_Gym_1F_Text_AxleIntro, LavaridgeTown_Gym_1F_Text_AxleDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
	msgbox LavaridgeTown_Gym_1F_Text_AxlePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Keegan::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, LOCALID_KEEGAN, TRAINER_KEEGAN, LavaridgeTown_Gym_B1F_Text_KeeganIntro, LavaridgeTown_Gym_B1F_Text_KeeganDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
	msgbox LavaridgeTown_Gym_B1F_Text_KeeganPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_Danielle::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, LOCALID_DANIELLE, TRAINER_DANIELLE, LavaridgeTown_Gym_1F_Text_DanielleIntro, LavaridgeTown_Gym_1F_Text_DanielleDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
	msgbox LavaridgeTown_Gym_1F_Text_DaniellePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_Gerald::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, LOCALID_GERALD, TRAINER_GERALD, LavaridgeTown_Gym_1F_Text_GeraldIntro, LavaridgeTown_Gym_1F_Text_GeraldDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
	msgbox LavaridgeTown_Gym_1F_Text_GeraldPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Jace::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, LOCALID_JACE, TRAINER_JACE, LavaridgeTown_Gym_B1F_Text_JaceIntro, LavaridgeTown_Gym_B1F_Text_JaceDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
	msgbox LavaridgeTown_Gym_B1F_Text_JacePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Jeff::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, LOCALID_JEFF, TRAINER_JEFF, LavaridgeTown_Gym_B1F_Text_JeffIntro, LavaridgeTown_Gym_B1F_Text_JeffDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
	msgbox LavaridgeTown_Gym_B1F_Text_JeffPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Eli::
	trainerbattle TRAINER_BATTLE_CONTINUE_SCRIPT, LOCALID_ELI, TRAINER_ELI, LavaridgeTown_Gym_B1F_Text_EliIntro, LavaridgeTown_Gym_B1F_Text_EliDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript, OBJ_ID_NONE, TRAINER_NONE, NULL, NULL, NULL, NULL, NULL, FALSE, TRUE, FALSE, FALSE
	msgbox LavaridgeTown_Gym_B1F_Text_EliPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_LAVARIDGE_GYM, LavaridgeTown_Gym_1F_EventScript_GymGuidePostVictory
	msgbox LavaridgeTown_Gym_1F_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_Gym_1F_EventScript_GymGuidePostVictory::
	msgbox LavaridgeTown_Gym_1F_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_Gym_1F_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE04_GET, LavaridgeTown_Gym_1F_EventScript_GymStatueCertified
	goto LavaridgeTown_Gym_1F_EventScript_GymStatue
	end

LavaridgeTown_Gym_1F_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE04_GET, LavaridgeTown_Gym_1F_EventScript_GymStatueCertified
	goto LavaridgeTown_Gym_1F_EventScript_GymStatue
	end

LavaridgeTown_Gym_1F_EventScript_GymStatueCertified::
	msgbox LavaridgeTown_Gym_1F_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

LavaridgeTown_Gym_1F_EventScript_GymStatue::
	msgbox LavaridgeTown_Gym_1F_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

LavaridgeTown_Gym_1F_Text_GymGuideAdvice:
	.string "{JPN}げんき しとぉや!!\nチャンピオンを めざす {PLAYER}!!\pフエンタウン ジムの リーダー アスナは\nほのおタイプ ポケモンの つかいてだ!\pポケモンに たいする じょうねつは\nかざん よりも はげしく あちィ!\pうっかり ちかづくと やけど するから\nみずで ひやしながら\lきばってこー な!!$"

LavaridgeTown_Gym_1F_Text_GymGuidePostVictory:
	.string "{JPN}いやあー あちィ たたかい だった!$"

LavaridgeTown_Gym_1F_Text_ColeIntro:
	.string "{JPN}あちちちちちっ!\nぷはーっ! あつかったーっ!$"

LavaridgeTown_Gym_1F_Text_ColeDefeat:
	.string "{JPN}あせが めに しみるぜ……$"

LavaridgeTown_Gym_1F_Text_ColePostBattle:
	.string "{JPN}すなブロは けっこうが よくなって\nかんせつの いたみに こうかてき だぜ!$"

LavaridgeTown_Gym_1F_Text_AxleIntro:
	.string "{JPN}ストレスを かいしょう する\nおれの ひとときを ジャマ するなーっ!$"

LavaridgeTown_Gym_1F_Text_AxleDefeat:
	.string "{JPN}アスナさんに まけてしまえーっ!$"

LavaridgeTown_Gym_1F_Text_AxlePostBattle:
	.string "{JPN}はあ ふう……\pすなブロに ながく はいっていると\nけっこう つかれちゃう もんだよ……$"

LavaridgeTown_Gym_B1F_Text_KeeganIntro:
	.string "{JPN}さすがに つかれてきた ようだね\nすなブロで ひとやすみ したいだろう?\pだが きりょくを たもつ ことも\nトレーナーに ひつような さいのう だよ$"

LavaridgeTown_Gym_B1F_Text_KeeganDefeat:
	.string "{JPN}ひあそびは けがのもと だったか……$"

LavaridgeTown_Gym_B1F_Text_KeeganPostBattle:
	.string "{JPN}きみの ちからは ほんものだ……\nでも リーダー アスナさんは つよいよ!\pあまく みていると おおやけど するぜ!$"

LavaridgeTown_Gym_1F_Text_GeraldIntro:
	.string "{JPN}おまえの ポケモン\n200どの あつさに たえられるかよ!?$"

LavaridgeTown_Gym_1F_Text_GeraldDefeat:
	.string "{JPN}もえかたが たりなかったか……$"

LavaridgeTown_Gym_1F_Text_GeraldPostBattle:
	.string "{JPN}200どの あつさ ってのは\nようがんの おんど!\pおまえの ポケモン おれに かてたんだから\nようがんの なかでも へいき だろうな!$"

LavaridgeTown_Gym_1F_Text_DanielleIntro:
	.string "{JPN}えーっと!\nじゃあ しょうぶの あいて しますね!$"

LavaridgeTown_Gym_1F_Text_DanielleDefeat:
	.string "{JPN}なんて つよいの!?$"

LavaridgeTown_Gym_1F_Text_DaniellePostBattle:
	.string "{JPN}あたしも アスナさんと いっしょに\nきれいで つよい トレーナーに なるんだ!$"

LavaridgeTown_Gym_B1F_Text_JaceIntro:
	.string "{JPN}ほら ほらっ!\nきもちが さめない うちに いくぜっ!$"

LavaridgeTown_Gym_B1F_Text_JaceDefeat:
	.string "{JPN}こんなに あついのに\nおれの こころが さむい……$"

LavaridgeTown_Gym_B1F_Text_JacePostBattle:
	.string "{JPN}そのくらい とうしを もやして いれば\nもしかしたら リーダーにも かてるかもな$"

LavaridgeTown_Gym_B1F_Text_JeffIntro:
	.string "{JPN}みよ この もえさかる ほのおを!\nオレの しょうりを よげんして いるのだ!$"

LavaridgeTown_Gym_B1F_Text_JeffDefeat:
	.string "{JPN}おかしいなあ$"

LavaridgeTown_Gym_B1F_Text_JeffPostBattle:
	.string "{JPN}いいもんね! オレなんて\nたきびの うえを はだしで はしれるぜ!\p…… おまえは まね するなよ$"

LavaridgeTown_Gym_B1F_Text_EliIntro:
	.string "{JPN}やまの なかでも とくに\nかざんが だいすき でして……$"

LavaridgeTown_Gym_B1F_Text_EliDefeat:
	.string "{JPN}あれよ あれよと いうまに\nまけちまった わけでして$"

LavaridgeTown_Gym_B1F_Text_EliPostBattle:
	.string "{JPN}アスナさんの つよさに ほれて\nわしは ここに いるわけ でして……\lへへへへ$"

LavaridgeTown_Gym_1F_Text_FlanneryIntro:
	.string "{JPN}ようこそ ……じゃなくて\pよくぞ ここまで きた ものだな!\pわたしは ここの ジムリーダーを\nつとめさせて いただく ……じゃなくて\pジムリーダーを まかされた アスナだ!\pくう……\nジムリーダーに なりたて だからって\lゆだん など しないことだ!\pおじいちゃん ゆずりの さいのうと\nこのとちで みがきあげた ホットな わざ\lあなたに みせてやる!$"

LavaridgeTown_Gym_1F_Text_FlanneryDefeat:
	.string "{JPN}くう……\nかたに ちから はいっちゃって……\pあたし……\nジムリーダーに なりたて だからって\lちょっと むり していたのかな……\pやっぱ じぶんらしく でないと\nいっしょに たたかってくれる\lポケモンも こまっちゃうよね!\pそれを おしえてくれた\nあなたに これ あげる!$"

LavaridgeTown_Gym_1F_Text_ReceivedHeatBadge:
	.string "{JPN}{PLAYER}は アスナから\nヒートバッジを もらった!$"

LavaridgeTown_Gym_1F_Text_ExplainHeatBadgeTakeThis:
	.string "{JPN}ヒートバッジが あれば\nレベル50 までなら\lひとから もらった ポケモンでも\lいうことを きいて くれるんだ!\pそれに ひでんわざの かいりき が\nつかえるようにも なるよ!\pこれは かんしゃの きもち\nえんりょ せずに もらって!$"

LavaridgeTown_Gym_1F_Text_ExplainOverheat:
	.string "{JPN}わざマシン50 には\nオーバーヒートが はいってるよ!\pその わざは あいてに\nおおきな ダメージを あたえるんだ!\pだけど とくこうが\nがくっと さがっちゃう から\lながい しょうぶには むかないかも\p…… …… ……$"

LavaridgeTown_Gym_1F_Text_RegisteredFlannery:
	.string "{JPN}ジムリーダー アスナを\nポケナビに とうろく した!$"

LavaridgeTown_Gym_1F_Text_FlanneryPostBattle:
	.string "{JPN}あなたの つよさ\nだれかを おもいださせるの よね……\pそうだ! トウカシティの ジムリーダー\nセンリさんに にているんだ!$"

LavaridgeTown_Gym_1F_Text_GymStatue:
	.string "{JPN}フエンタウン ポケモンジム$"

LavaridgeTown_Gym_1F_Text_GymStatueCertified:
	.string "{JPN}フエンタウン ポケモンジム\pアスナ にんてい トレーナー!\n{PLAYER}$"

LavaridgeTown_Gym_1F_Text_FlanneryPreRematch:
	.string "{JPN}アスナ『しょうぶに まけた ぐらいで\nあたしは へこたれたり しない!\pなぜなら あたしは ポケモンが すきで\nしょうぶを するのが すきで\lそれに…… このジムが すきだから!\pさあ もういちど いっしょに\nホットな わざを ぶつけあいましょう!$"

LavaridgeTown_Gym_1F_Text_FlanneryRematchDefeat:
	.string "{JPN}ふぅ! ばくはつ すんぜん!$"

LavaridgeTown_Gym_1F_Text_FlanneryPostRematch:
	.string "{JPN}アスナ『しょうぶには まけたけど\nこんなに ヒートする たたかいが できて\lあたしは まんぞくよ!\pそのうち また しょうぶ しましょうね!$"

LavaridgeTown_Gym_1F_Text_FlanneryRematchNeedTwoMons:
	.string "{JPN}アスナ『しょうぶに まけた ぐらいで\nあたしは へこたれたり しない!\pなぜなら あたしは ポケモンが すきで\nしょうぶを するのが すきで\lそれに…… このジムが すきだから!\pさあ もういちど いっしょに\nホットな わざを ぶつけあいましょう!\p……あら? たたかえる ポケモンを\n1ひきしか もって いないんですね\pすみませんけど 2ひき いじょうの\nポケモンを もって きてください$"
