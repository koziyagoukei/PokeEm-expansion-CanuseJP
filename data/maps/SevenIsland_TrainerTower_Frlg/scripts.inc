SevenIsland_TrainerTower_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SevenIsland_TrainerTower_OnTransition
	.byte 0

SevenIsland_TrainerTower_OnTransition::
	setvar VAR_MAP_SCENE_TRAINER_TOWER, 0
	end

SevenIsland_TrainerTower_EventScript_TrainerTowerSign::
	msgbox SevenIsland_TrainerTower_Text_TrainerTowerSign, MSGBOX_SIGN
	end

SevenIsland_TrainerTower_EventScript_TrainerTowerAheadSign::
	msgbox SevenIsland_TrainerTower_Text_TrainerTowerAhead, MSGBOX_SIGN
	end

SevenIsland_TrainerTower_Text_DarioIntro::
	.string "{JPN}きみの けはいが\n"
	.string "ちかづくのを かんじて いたよ!$"

SevenIsland_TrainerTower_Text_DarioDefeat::
	.string "{JPN}ギブアップ$"

SevenIsland_TrainerTower_Text_DarioPostBattle::
	.string "{JPN}このさき きみは\n"
	.string "たくさんの ひとと\l"
	.string "ふしぎな ちからで\l"
	.string "つながって いく きがする$"

SevenIsland_TrainerTower_Text_RodetteIntro::
	.string "{JPN}この しまの どこかに\n"
	.string "ふしぎな ポケモンたちが\l"
	.string "ねむっている…$"

SevenIsland_TrainerTower_Text_RodetteDefeat::
	.string "{JPN}すごい ひと…!$"

SevenIsland_TrainerTower_Text_RodettePostBattle::
	.string "{JPN}この しまの どこかに\n"
	.string "ねむっている\l"
	.string "ふしぎな ポケモンたちが\l"
	.string "まっている のは… あなた?$"

SevenIsland_TrainerTower_Text_TrainerTowerSign::
	.string "{JPN}トレーナータワー いりぐち\n"
	.string "つよき トレーナーよ かけあがれ!$"

SevenIsland_TrainerTower_Text_TrainerTowerAhead::
	.string "{JPN}このさき トレーナータワー$"

