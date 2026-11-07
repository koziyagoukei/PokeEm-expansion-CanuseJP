Route2_ViridianForest_NorthEntrance_Frlg_MapScripts::
	.byte 0

Route2_ViridianForest_NorthEntrance_EventScript_Youngster::
	msgbox Route2_ViridianForest_NorthEntrance_Text_ManyMonsOnlyInForests, MSGBOX_NPC
	end

Route2_ViridianForest_NorthEntrance_EventScript_OldMan::
	msgbox Route2_ViridianForest_NorthEntrance_Text_CanCutSkinnyTrees, MSGBOX_NPC
	end

Route2_ViridianForest_NorthEntrance_EventScript_CooltrainerF::
	msgbox Route2_ViridianForest_NorthEntrance_Text_CanCancelEvolution, MSGBOX_NPC
	end

Route2_ViridianForest_NorthEntrance_Text_ManyMonsOnlyInForests::
	.string "{JPN}もりや どうくつ しか\n"
	.string "いない ポケモンも おおいよ\p"
	.string "いろんな ポケモン とるなら\n"
	.string "しつこく さがしまわる ことだね$"

Route2_ViridianForest_NorthEntrance_Text_CanCutSkinnyTrees::
	.string "{JPN}どうろの あちこちに\n"
	.string "ほそい きが あったろ?\p"
	.string "あそこは ポケモンの わざで\n"
	.string "きって とおれる そうじゃ$"

Route2_ViridianForest_NorthEntrance_Text_CanCancelEvolution::
	.string "{JPN}しんか キャンセル つかってる?\p"
	.string "しんか しようと するのを\n"
	.string "とちゅうで やめさせて\l"
	.string "そのままの かたちで\l"
	.string "そだてる テクニックよ$"

