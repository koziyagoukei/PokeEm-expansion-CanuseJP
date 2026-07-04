RustboroCity_CuttersHouse_MapScripts::
	.byte 0

RustboroCity_CuttersHouse_EventScript_Cutter::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_CUT, RustboroCity_CuttersHouse_EventScript_ExplainCut
	msgbox RustboroCity_CuttersHouse_Text_YouCanPutThisHMToGoodUse, MSGBOX_DEFAULT
	giveitem ITEM_HM_CUT
	setflag FLAG_RECEIVED_HM_CUT
	msgbox RustboroCity_CuttersHouse_Text_ExplainCut, MSGBOX_DEFAULT
	release
	end

RustboroCity_CuttersHouse_EventScript_ExplainCut::
	msgbox RustboroCity_CuttersHouse_Text_ExplainCut, MSGBOX_DEFAULT
	release
	end

RustboroCity_CuttersHouse_EventScript_Lass::
	msgbox RustboroCity_CuttersHouse_Text_DadHelpedClearLandOfTrees, MSGBOX_NPC
	end

RustboroCity_CuttersHouse_Text_YouCanPutThisHMToGoodUse:
	.string "{JPN}その めつき\nしなやかな みの こなし……\lそして きたえられた ポケモン……\pきみは できる トレーナーだな!\pなにも いわなくて いい\nわたしには わかる わかるのだよ!\pきみならば この ひでんマシン\nうまく つかえるだろう\lさあ えんりょ せずに もっていきたまえ!$"

RustboroCity_CuttersHouse_Text_ExplainCut:
	.string "{JPN}その ひでんマシンは いあいぎり!\pひでんは たたかっていない ときでも\nポケモンが つかえる わざ!\pいあいぎりを おぼえた ポケモンは\nストーンバッジを もっていれば\lほそい きを きれるように なるのだよ!\pしかも ひでんマシンは わざマシンと ちがい\nつかっても なくならないぞ!$"

RustboroCity_CuttersHouse_Text_DadHelpedClearLandOfTrees:
	.string "{JPN}この カナズミが おおきくなるとき\nおとうさん みんなの ために\lいあいぎりを つかって\lあたりを きりひらいたんだって!$"

