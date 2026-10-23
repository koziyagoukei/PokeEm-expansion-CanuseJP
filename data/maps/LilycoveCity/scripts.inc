LilycoveCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LilycoveCity_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, LilycoveCity_OnLoad
	.byte 0

LilycoveCity_OnTransition:
	setflag FLAG_VISITED_LILYCOVE_CITY
	setvar VAR_CONTEST_HALL_STATE, 0
	setflag FLAG_HIDE_LILYCOVE_CONTEST_HALL_REPORTER
	call_if_set FLAG_SYS_WEATHER_CTRL, Common_EventScript_SetAbnormalWeather
	call Common_EventScript_SetupRivalGfxId
	end

LilycoveCity_OnLoad:
	call_if_unset FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE, LilycoveCity_EventScript_SetWailmerMetatiles
	end

LilycoveCity_EventScript_SetWailmerMetatiles::
	setmetatile 76, 12, METATILE_Lilycove_Wailmer0, TRUE
	setmetatile 77, 12, METATILE_Lilycove_Wailmer1, TRUE
	setmetatile 76, 13, METATILE_Lilycove_Wailmer2, TRUE
	setmetatile 77, 13, METATILE_Lilycove_Wailmer3, TRUE
	setmetatile 76, 14, METATILE_Lilycove_Wailmer0_Alt, TRUE
	setmetatile 77, 14, METATILE_Lilycove_Wailmer1_Alt, TRUE
	setmetatile 76, 15, METATILE_Lilycove_Wailmer2, TRUE
	setmetatile 77, 15, METATILE_Lilycove_Wailmer3, TRUE
	setmetatile 77, 16, METATILE_Lilycove_Wailmer0_Alt, TRUE
	setmetatile 78, 16, METATILE_Lilycove_Wailmer1_Alt, TRUE
	setmetatile 77, 17, METATILE_Lilycove_Wailmer2, TRUE
	setmetatile 78, 17, METATILE_Lilycove_Wailmer3, TRUE
	return

LilycoveCity_EventScript_BerryGentleman::
	lock
	faceplayer
	dotimebasedevents
	goto_if_set FLAG_DAILY_LILYCOVE_RECEIVED_BERRY, LilycoveCity_EventScript_ReceivedBerry
	msgbox LilycoveCity_Text_BerrySuitsYou, MSGBOX_DEFAULT
	giverandomberry BERRY_ID_CHERI, BERRY_ID_SITRUS
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_DAILY_LILYCOVE_RECEIVED_BERRY
	msgbox LilycoveCity_Text_BecauseYoureTrainer, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_ReceivedBerry::
	msgbox LilycoveCity_Text_PokeblocksSuitPokemon, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Man3::
	msgbox LilycoveCity_Text_ContestHallInTown, MSGBOX_NPC
	end

LilycoveCity_EventScript_Girl::
	lock
	faceplayer
	goto_if_set FLAG_BADGE07_GET, LilycoveCity_EventScript_GirlAquaGone
	msgbox LilycoveCity_Text_StrangeCaveInCove, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_GirlAquaGone::
	msgbox LilycoveCity_Text_GoingToMoveDeleterForHMs, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_RichBoy::
	msgbox LilycoveCity_Text_ImFromKanto, MSGBOX_NPC
	end

LilycoveCity_EventScript_Sailor2::
	lock
	faceplayer
	goto_if_set FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE, LilycoveCity_EventScript_Sailor2AquaGone
	msgbox LilycoveCity_Text_TeamAquaBeenTrainingWailmer, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Sailor2AquaGone::
	msgbox LilycoveCity_Text_SomeonePuntedTeamAquaOut, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Woman2::
	lock
	faceplayer
	goto_if_set FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE, LilycoveCity_EventScript_Woman2AquaGone
	msgbox LilycoveCity_Text_SomeoneStoleMyPokemon, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Woman2AquaGone::
	msgbox LilycoveCity_Text_MissingPokemonCameBack, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Man2::
	msgbox LilycoveCity_Text_ImArtDealer, MSGBOX_NPC
	end

LilycoveCity_EventScript_ExpertM1::
	msgbox LilycoveCity_Text_SeaRemainsForeverYoung, MSGBOX_SIGN
	end

LilycoveCity_EventScript_ExpertF::
	msgbox LilycoveCity_Text_SixtyYearsAgoHusbandProposed, MSGBOX_SIGN
	end

LilycoveCity_EventScript_ExpertM2::
	lock
	faceplayer
	goto_if_set FLAG_BADGE07_GET, LilycoveCity_EventScript_ExpertM2AquaGone
	msgbox LilycoveCity_Text_TeamAquaRenovatedCavern, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_ExpertM2AquaGone::
	msgbox LilycoveCity_Text_TeamAquaLotGoneForGood, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Sailor1::
	msgbox LilycoveCity_Text_HeardTowerCalledSkyPillar, MSGBOX_NPC
	end

LilycoveCity_EventScript_FatMan::
	msgbox LilycoveCity_Text_SawTallTowerOnRoute131, MSGBOX_NPC
	end

LilycoveCity_EventScript_Man1::
	lockall
	msgbox LilycoveCity_Text_JustArrivedAndSawRarePokemon, MSGBOX_NPC
	applymovement LOCALID_LILYCOVE_MAN_1, Common_Movement_FaceOriginalDirection
	end

LilycoveCity_EventScript_Woman1::
	lockall
	msgbox LilycoveCity_Text_HoneymoonVowToSeeRarePokemon, MSGBOX_NPC
	applymovement LOCALID_LILYCOVE_WOMAN_1, Common_Movement_FaceOriginalDirection
	end

LilycoveCity_EventScript_CitySign::
	msgbox LilycoveCity_Text_CitySign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_ContestHallSign::
	msgbox LilycoveCity_Text_ContestHallSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_MotelSign::
	msgbox LilycoveCity_Text_MotelSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_MuseumSign::
	lockall
	specialvar VAR_0x8004, CountPlayerMuseumPaintings
	switch VAR_0x8004
	case 0, LilycoveCity_EventScript_MuseumSignNoPaintings
	msgbox LilycoveCity_Text_MuseumSignPlayersExhibit, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_MuseumSignNoPaintings::
	msgbox LilycoveCity_Text_MuseumSign, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_HarborSign::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, LilycoveCity_EventScript_HarborSignFerryReady
	msgbox LilycoveCity_Text_HarborSignUnderConstruction, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_HarborSignFerryReady::
	msgbox LilycoveCity_Text_HarborSign, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_TrainerFanClubSign::
	msgbox LilycoveCity_Text_TrainerFanClubSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_DepartmentStoreSign::
	msgbox LilycoveCity_Text_DepartmentStoreSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_MoveDeletersHouseSign::
	msgbox LilycoveCity_Text_MoveDeletersHouseSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_WailmerTrainerGrunt::
	lockall
	goto_if_set FLAG_MET_WAILMER_TRAINER, LilycoveCity_EventScript_MetWailmerTrainer
	msgbox LilycoveCity_Text_WailmerLeapOutOfWater, MSGBOX_DEFAULT
	applymovement VAR_LAST_TALKED, Common_Movement_FacePlayer
	waitmovement 0
	msgbox LilycoveCity_Text_GetLostMessingUpTraining, MSGBOX_DEFAULT
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag FLAG_MET_WAILMER_TRAINER
	releaseall
	end

LilycoveCity_EventScript_MetWailmerTrainer::
	msgbox LilycoveCity_Text_WailmerLeapOutOfWater, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_AquaGrunt1::
	msgbox LilycoveCity_Text_MovedLootIntoHideoutToday, MSGBOX_NPC
	end

LilycoveCity_EventScript_AquaGrunt2::
	msgbox LilycoveCity_Text_ChanceToDoBigThings, MSGBOX_NPC
	end

LilycoveCity_EventScript_AquaGrunt3::
	msgbox LilycoveCity_Text_DontGoNearCaveInCove, MSGBOX_NPC
	end

LilycoveCity_EventScript_AquaGrunt4::
	msgbox LilycoveCity_Text_IfWorldBecomesOurs, MSGBOX_NPC
	end

LilycoveCity_EventScript_Rival::
	lock
	faceplayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LilycoveCity_EventScript_May
	goto_if_eq VAR_RESULT, FEMALE, LilycoveCity_EventScript_Brendan
	end

LilycoveCity_EventScript_May::
	playbgm MUS_ENCOUNTER_MAY, TRUE
	call_if_set FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE, LilycoveCity_EventScript_MayAskToBattleAgain
	call_if_unset FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE, LilycoveCity_EventScript_MayAskToBattle
	goto_if_eq VAR_RESULT, NO, LilycoveCity_EventScript_DeclineMayBattle
	msgbox LilycoveCity_Text_MayWontBeBeaten, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 0, LilycoveCity_EventScript_BattleMayTreecko
	case 1, LilycoveCity_EventScript_BattleMayTorchic
	case 2, LilycoveCity_EventScript_BattleMayMudkip
	end

LilycoveCity_EventScript_MayAskToBattleAgain::
	msgbox LilycoveCity_Text_MayBattleMe, MSGBOX_YESNO
	return

LilycoveCity_EventScript_MayAskToBattle::
	msgbox LilycoveCity_Text_MayShoppingLetsBattle, MSGBOX_YESNO
	return

LilycoveCity_EventScript_DeclineMayBattle::
	setflag FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE
	msgbox LilycoveCity_Text_MayNotRaisingPokemon, MSGBOX_DEFAULT
	savebgm MUS_DUMMY
	fadedefaultbgm
	release
	end

LilycoveCity_EventScript_Brendan::
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	call_if_set FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE, LilycoveCity_EventScript_BrendanAskToBattleAgain
	call_if_unset FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE, LilycoveCity_EventScript_BrendanAskToBattle
	goto_if_eq VAR_RESULT, NO, LilycoveCity_EventScript_DeclineBrendanBattle
	msgbox LilycoveCity_Text_BrendanWontBeBeaten, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 0, LilycoveCity_EventScript_BattleBrendanTreecko
	case 1, LilycoveCity_EventScript_BattleBrendanTorchic
	case 2, LilycoveCity_EventScript_BattleBrendanMudkip
	end

LilycoveCity_EventScript_BrendanAskToBattleAgain::
	msgbox LilycoveCity_Text_BrendanBattleMe, MSGBOX_YESNO
	return

LilycoveCity_EventScript_BrendanAskToBattle::
	msgbox LilycoveCity_Text_BrendanShoppingLetsBattle, MSGBOX_YESNO
	return

LilycoveCity_EventScript_DeclineBrendanBattle::
	setflag FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE
	msgbox LilycoveCity_Text_BrendanNoConfidence, MSGBOX_DEFAULT
	savebgm MUS_DUMMY
	fadedefaultbgm
	release
	end

LilycoveCity_EventScript_BattleMayTreecko::
	trainerbattle_no_intro TRAINER_MAY_LILYCOVE_TREECKO, LilycoveCity_Text_MayDefeat
	goto LilycoveCity_EventScript_DefeatedMay
	end

LilycoveCity_EventScript_BattleMayTorchic::
	trainerbattle_no_intro TRAINER_MAY_LILYCOVE_TORCHIC, LilycoveCity_Text_MayDefeat
	goto LilycoveCity_EventScript_DefeatedMay
	end

LilycoveCity_EventScript_BattleMayMudkip::
	trainerbattle_no_intro TRAINER_MAY_LILYCOVE_MUDKIP, LilycoveCity_Text_MayDefeat
	goto LilycoveCity_EventScript_DefeatedMay
	end

LilycoveCity_EventScript_BattleBrendanTreecko::
	trainerbattle_no_intro TRAINER_BRENDAN_LILYCOVE_TREECKO, LilycoveCity_Text_BrendanDefeat
	goto LilycoveCity_EventScript_DefeatedBrendan
	end

LilycoveCity_EventScript_BattleBrendanTorchic::
	trainerbattle_no_intro TRAINER_BRENDAN_LILYCOVE_TORCHIC, LilycoveCity_Text_BrendanDefeat
	goto LilycoveCity_EventScript_DefeatedBrendan
	end

LilycoveCity_EventScript_BattleBrendanMudkip::
	trainerbattle_no_intro TRAINER_BRENDAN_LILYCOVE_MUDKIP, LilycoveCity_Text_BrendanDefeat
	goto LilycoveCity_EventScript_DefeatedBrendan
	end

LilycoveCity_EventScript_DefeatedMay::
	msgbox LilycoveCity_Text_MayGoingBackToLittleroot, MSGBOX_DEFAULT
	setvar VAR_RESULT, FALSE
	call_if_set FLAG_BADGE06_GET, LilycoveCity_EventScript_CheckFinalBadge
	call_if_eq VAR_RESULT, FALSE, LilycoveCity_EventScript_MayCollectBadges
	call_if_eq VAR_RESULT, TRUE, LilycoveCity_EventScript_MayPokemonLeague
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_BEDROOM
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_2F_PICHU_DOLL
	goto LilycoveCity_EventScript_RivalFlyAway
	end

LilycoveCity_EventScript_CheckFinalBadge::
	goto_if_set FLAG_BADGE08_GET, LilycoveCity_EventScript_HasFinalBadge
	return

LilycoveCity_EventScript_HasFinalBadge::
	setvar VAR_RESULT, TRUE
	return

LilycoveCity_EventScript_MayCollectBadges::
	msgbox LilycoveCity_Text_MayYouGoingToCollectBadges, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_MayPokemonLeague::
	goto_if_set FLAG_SYS_GAME_CLEAR, LilycoveCity_EventScript_MayBattleFrontier
	msgbox LilycoveCity_Text_MayYouGoingToPokemonLeague, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_MayBattleFrontier::
	msgbox LilycoveCity_Text_MayYouGoingToBattleFrontier, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_DefeatedBrendan::
	msgbox LilycoveCity_Text_BrendanGoingBackToLittleroot, MSGBOX_DEFAULT
	setvar VAR_RESULT, FALSE
	call_if_set FLAG_BADGE06_GET, LilycoveCity_EventScript_CheckFinalBadge
	call_if_eq VAR_RESULT, FALSE, LilycoveCity_EventScript_BrendanCollectBadges
	call_if_eq VAR_RESULT, TRUE, LilycoveCity_EventScript_BrendanPokemonLeague
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_BEDROOM
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_SWABLU_DOLL
	goto LilycoveCity_EventScript_RivalFlyAway
	end

LilycoveCity_EventScript_BrendanCollectBadges::
	msgbox LilycoveCity_Text_BrendanYouGoingToCollectBadges, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_BrendanPokemonLeague::
	goto_if_set FLAG_SYS_GAME_CLEAR, LilycoveCity_EventScript_BrendanBattleFrontier
	msgbox LilycoveCity_Text_BrendanYouGoingToPokemonLeague, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_BrendanBattleFrontier::
	msgbox LilycoveCity_Text_BrendanYouGoingToBattleFrontier, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_RivalFlyAway::
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 50
	setfieldeffectargument 0, 1
	dofieldeffect FLDEFF_NPCFLY_OUT
	delay 15
	removeobject VAR_LAST_TALKED
	waitfieldeffect FLDEFF_NPCFLY_OUT
	savebgm MUS_DUMMY
	fadedefaultbgm
	setflag FLAG_MET_RIVAL_LILYCOVE
	release
	end

LilycoveCity_EventScript_SchoolKidM::
	lock
	faceplayer
	msgbox LilycoveCity_Text_DoYouKnowAboutBerryBlender, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, LilycoveCity_EventScript_KnowAboutBerryBlender
	call_if_eq VAR_RESULT, NO, LilycoveCity_EventScript_DontKnowAboutBerryBlender
	release
	end

LilycoveCity_EventScript_KnowAboutBerryBlender::
	msgbox LilycoveCity_Text_FasterSpinBetterPokeblocks, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_DontKnowAboutBerryBlender::
	msgbox LilycoveCity_Text_ExplainBerryBlender, MSGBOX_DEFAULT
	return

LilycoveCity_Text_MayShoppingLetsBattle:
	.string "ハルカ『あら!?\n{PLAYER}{KUN}も おかいもの?\pあたし ぬいぐるみとか ポスター\nいっぱい かっちゃった!\pもちろん ポケモン いっぱい つかまえたし\nちゃんと そだてて いるんだから!\pあたし としては {PLAYER}{KUN}が\nポケモン そだててるか しんぱい だな\pしょうぶ してあげるから\n{PLAYER}{KUN}の ポケモン みせてよ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_MayNotRaisingPokemon:
	.string "ハルカ『えー なんで?\n{PLAYER}{KUN} ポケモン そだててないの?\pそれじゃあ トレーナーとして\nまだまだ ね!"
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_MayBattleMe:
	.string "ハルカ『しょうぶ してあげるから\n{PLAYER}{KUN}の ポケモン みせてよ!"
	.string "$"

LilycoveCity_Text_MayWontBeBeaten:
	.string "ハルカ『あたしの そだてた ポケモンが\n{PLAYER}{KUN}の ポケモンに\lまけるわけ ないんだから!"
	.string "$"

LilycoveCity_Text_MayDefeat:
	.string "…… …… ……\p103ばん どうろで\n{PLAYER}{KUN}と しょうぶ したこと\lおもいだしちゃった!\pあの しょうぶが あったから\n{PLAYER}{KUN} ここまで\lつよく なったんだね!"
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_MayGoingBackToLittleroot:
	.string "ハルカ『あたし そろそろ\nミシロタウンに かえる つもりなの\pいろいろ ポケモン つかまえて\nずかんも けっこう できてきたから\lおとうさんに みせるんだ!"
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_MayYouGoingToCollectBadges:
	.string "{PLAYER}{KUN}は どうするの?\pあちこちの ジムバッジを あつめて\nポケモンリーグ ちょうせん するの?\pじゃあ あたし {PLAYER}{KUN}が\nバッジを あつめている あいだに\lポケモンずかんを かんせい させちゃお!\pじゃね!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_MayYouGoingToPokemonLeague:
	.string "{PLAYER}{KUN}は どうするの?\pポケモンリーグ ちょうせん するの?\pそれとも ポケモンずかん\lかんせい させるの?\pポケモンしょうぶ じゃ まけちゃったけど\nずかんは あたしの ほうが\lさきに かんせい させるんだから!\pじゃね!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_MayYouGoingToBattleFrontier:
	.string "{PLAYER}{KUN}は どうするの?\pバトルフロンティアに ちょうせん する?\nそれとも ずかんを かんせい させる?\pポケモンしょうぶ じゃ まけちゃったけど\nずかんは あたしの ほうが\lさきに かんせい させるんだから!\pじゃね!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_BrendanShoppingLetsBattle:
	.string "ユウキ『なんだ {PLAYER}か\pオレ とうさんに たのまれた\nかいものを してたんだ\lべつに ぬいぐるみ なんか かってないよ\pそれよりも ひさしぶりに あったんだ\pどっちが うまく\nポケモン そだてているか\lしょうぶ してやろうか?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_BrendanNoConfidence:
	.string "ユウキ『なんだよ\nじしんが ないのかよ"
	.string "$"

LilycoveCity_Text_BrendanBattleMe:
	.string "ユウキ『どっちが うまく\nポケモン そだてているか\lしょうぶ してやろうか?"
	.string "$"

LilycoveCity_Text_BrendanWontBeBeaten:
	.string "ユウキ『まあ オレが\n{PLAYER}に まけるわけ ないんだけど"
	.string "$"

LilycoveCity_Text_BrendanDefeat:
	.string "ふうん……\nかなり そだててるんだな\pちょっと くやしいな\nトレーナーとしては オレの ほうが\lセンパイ だったのに……"
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_BrendanGoingBackToLittleroot:
	.string "ユウキ『オレ……\nミシロタウンに もどるつもり なんだ\pとうさんの てつだいで つくっている\nポケモンずかんも けっこう できてきたし\lいちど とうさんに みせておかないと"
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_BrendanYouGoingToCollectBadges:
	.string "{PLAYER}は どうする?\pやっぱり ぜんぶの ジムバッジ あつめて\nポケモンリーグ ちょうせん するのか?\pオレも そうしようかな\pまあ いいや\nとにかく がんばれよ!"
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_BrendanYouGoingToPokemonLeague:
	.string "{PLAYER}は どうする?\pそんなに つよいんだから\nポケモンリーグに ちょうせん するのか?\pオレだって もうちょっと\nポケモン そだてれば {PLAYER}よりも\lつよくなれる はずだからな\pまあ いいや\nとにかく がんばれよ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_BrendanYouGoingToBattleFrontier:
	.string "{PLAYER}は どうする?\pバトルフロンティアに ちょうせん する?\nそれとも ずかんを かんせい させる?\pそんなに つよいんだから\nバトルフロンティアに ちょうせん するか?\pオレだって もうちょっと\nポケモン そだてれば {PLAYER}よりも\lつよくなれる はずだからな\lバトルフロンティアに いってみようかな?\pまあ いいや\nとにかく がんばれよ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_MovedLootIntoHideoutToday:
	.string "ふふふ……\pきょうも うばった ブツを\nアジトに はこびこんだぞ……\pなっ なんだ おまえ!\nいっ いまのは ひとりごと だぞ!"
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_ChanceToDoBigThings:
	.string "リーダーが なに かんがえてるのか\nしたっぱの オレには わからんが\pアクアだんに いると\nすごいこと できそうだぜ!\pなっ なんだ おまえ!\nいっ いまのは ひとりごと だぞ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_DontGoNearCaveInCove:
	.string "おい おまえ!\pいりえに ある\nどうくつには ちかづくなよ!\pなぜって? りゆうは どうでも いいんだ!\nおとなの いうこと きけよな!!"
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_IfWorldBecomesOurs:
	.string "この ひろい せかい すべてが\nおれら アクアだんの ものに なったら\lポケモンも しあわせに なれるはずだ!"
	.string ""
	.string "$"

LilycoveCity_Text_WailmerLeapOutOfWater:
	.string "それっ! ホエルコ!\nそこで すいめんから ジャンプするのだ!"
	.string "$"

LilycoveCity_Text_GetLostMessingUpTraining:
	.string "……ん? なんだ おまえは?\pトレーニングの じゃまに なるから\nあっちに いっててくれ!"
	.string ""
	.string "$"

LilycoveCity_Text_ContestHallInTown:
	.string "この まちに ポケモンコンテストの\nかいじょうが あるんだな!\pということは よく そだった ポケモンが\nいっぱい あつまって いるんだな!\pわしの ハートは わくわく なんだな!"
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_StrangeCaveInCove:
	.string "まちの はずれの いりえに\nおかしな どうくつが あるの みた?"
	.string "$"

LilycoveCity_Text_GoingToMoveDeleterForHMs:
	.string "あたしの ポケモン\nコンテストの ために あたらしい わざを\lおぼえさせる つもりなの!\pだからね わざわすれ じいさんに たのんで\nひでんわざを わすれさせて もらうのよ!"
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_ImFromKanto:
	.string "ぼくは カントーから きました!\pホウエン ちほうは みずと みどりに\nあふれた うつくしい ところ ですね!\p……ん? ということは?\pこの とち だけに せいそくする\nめずらしい ポケモンが いるのかも?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_TeamAquaBeenTrainingWailmer:
	.string "アクアだんの れんちゅうが\nいりえで ホエルコを\lトレーニング してるんだよ!\pおいらたち ふなのり\nこのままじゃ うみに でられないよ!"
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_SomeonePuntedTeamAquaOut:
	.string "あんた しってるかい?\pだれかが アクアだんを\nけちらしてくれた らしいんだ!\pおかげで ホエルコ いなくなって\nうみに でられるように なったよ!"
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_SomeoneStoleMyPokemon:
	.string "うみべの やどで なみの おとを\nききながら のんびり おひるね\l……を していたら\pわたしの ポケモン とられちゃった!\pはんにんは だれー!?\nアクアだん? アクアだん なのー!?"
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_MissingPokemonCameBack:
	.string "いなくなってた わたしの ポケモン\nいつのまにか かえってきたの!"
	.string "$"

LilycoveCity_Text_ImArtDealer:
	.string "わたしは ガショウ です!\pいや そういう なまえ なのではなく\nかちのある えを かったり うったりする\lしょくぎょうの なまえ なんです!\pこのまちの びじゅつかんに すばらしい\nめいが が あると きいたの ですが……"
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_SeaRemainsForeverYoung:
	.string "わしらが しわくちゃに なっても\nこのうみは いつまでも わかわかしく\lいのちの かがやきに あふれとる\pふほほ ふほほほほ……"
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_SixtyYearsAgoHusbandProposed:
	.string "60ねんまえ ここで おじいしゃんが\nわしに プロポーズ してくれた ときから\lこのうみは うつくしいまま ですのじゃ\pむふふ むふふふふ……"
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_TeamAquaRenovatedCavern:
	.string "この いりえの ところに おかしな\nどうくつの いりぐちが ある じゃろう?\pいぜんは てんねんの どうくつ だったが\nアクアだん とかいう れんちゅうが\lかいぞうして あんなふうに したんじゃ!\p「しぜんを あいする アクアだん!」\n……と いっておるが\pいうことと やることが ちがっとる!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_TeamAquaLotGoneForGood:
	.string "いりえに あった どうくつ……\nいつのまにやら カラッポに なっておる\pアクアだんの れんちゅう\nどこかへ いって しまったらしい のう"
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_CitySign:
	.string "ここは ミナモ シティ\nりくちの さいはて うみの はじまり"
	.string ""
	.string "$"

LilycoveCity_Text_ContestHallSign:
	.string "ポケモンコンテスト かいじょう\nあつまれ! ポケモン トレーナー"
	.string "$"

LilycoveCity_Text_MotelSign:
	.string "「みんしゅく モナミ」\nミナモのモナミ と おぼえて ください"
	.string ""
	.string "$"

LilycoveCity_Text_MuseumSign:
	.string "「ミナモ びじゅつかん」\nポケモン めいがてん かいさい ちゅう!"
	.string ""
	.string "$"

LilycoveCity_Text_MuseumSignPlayersExhibit:
	.string "「ミナモ びじゅつかん」\p{PLAYER}の ポケモン コレクション\nかいさい ちゅう!"
	.string ""
	.string "$"

LilycoveCity_Text_HarborSignUnderConstruction:
	.string "「ミナモ シティ ふねのりば」\pれんらくせん タイドリップごう は\nカイナシティにて けんぞう ちゅう!\pもう しばらく おまちください!"
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_HarborSign:
	.string "「ミナモ シティ ふねのりば」\pれんらくせん タイドリップごうで\nふねの たびを おたのしみ ください!"
	.string ""
	.string "$"

LilycoveCity_Text_TrainerFanClubSign:
	.string "「ポケモン トレーナー ファンクラブ」\pかんばんに いろんな トレーナーの\nなまえが らくがき されている……"
	.string ""
	.string "$"

LilycoveCity_Text_DepartmentStoreSign:
	.string "「ミナモ デパート」\pグッズ いっぱい! ドキドキ いっぱい!\nなにかが みつかる すてきな ところ"
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_MoveDeletersHouseSign:
	.string "「わざわすれ じいさんの いえ」\nポケモンの わざ わすれさせます"
	.string "$"

LilycoveCity_Text_DoYouKnowAboutBerryBlender:
	.string "きのみブレンダー っていう\nマシンを しってるかい?"
	.string "$"

LilycoveCity_Text_FasterSpinBetterPokeblocks:
	.string "みんなで こうそくで まわし まくるんだ\pすっげー はやいと\nすっげー いいポロックが できるらしいぜ!"
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_ExplainBerryBlender:
	.string "ポロックっていう おかしが できるんだぜ\pいいポロックを つくると ポケモンの\nコンディションも ぐーんと アップさ!"
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_HeardTowerCalledSkyPillar:
	.string "あのな どこかの すいどうに\nそらのはしら とかいう とうが\lたってる らしいぜ"
	.string ""
	.string "$"

LilycoveCity_Text_SawTallTowerOnRoute131:
	.string "オレな 131ばん すいどうの あたりで\nたかーい とうを みたんだけど\lもしかして それって……?"
	.string ""
	.string "$"

LilycoveCity_Text_JustArrivedAndSawRarePokemon:
	.string "ぼくら しんこん りょこうで\nこのまちに きたばかり なんだけど\pドラゴンタイプの ポケモンが\nそらを とんで いくのを みたんだよ!\pホウエン ちほうには\nあんな かっこいい ポケモンが\lせいそく してるのかい?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

LilycoveCity_Text_HoneymoonVowToSeeRarePokemon:
	.string "わたしたちの しんこん りょこうは\nかくちの めずらしい ポケモンを\lできるだけ たくさん みること なんです\pこの まちに とうちゃく したら\nいきなり めずらしい ポケモンが みられて\lとても ハッピー だわ!"
	.string ""
	.string ""
	.string ""
	.string "$"
