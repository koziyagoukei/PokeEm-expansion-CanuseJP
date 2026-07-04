LilycoveCity_CoveLilyMotel_2F_MapScripts::
	.byte 0

LilycoveCity_CoveLilyMotel_2F_EventScript_GameDesigner::
	lock
	faceplayer
	call_if_unset FLAG_TEMP_2, LilycoveCity_CoveLilyMotel_2F_EventScript_ShowMeCompletedDex
	call_if_set FLAG_TEMP_2, LilycoveCity_CoveLilyMotel_2F_EventScript_ShowDiploma
	specialvar VAR_RESULT, HasAllHoennMons
	goto_if_eq VAR_RESULT, TRUE, LilycoveCity_CoveLilyMotel_2F_EventScript_AllHoennMonsFanfare
	release
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_ShowMeCompletedDex::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_ShowMeCompletedDex, MSGBOX_DEFAULT
	return

LilycoveCity_CoveLilyMotel_2F_EventScript_AllHoennMonsFanfare::
	setflag FLAG_TEMP_2
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	goto LilycoveCity_CoveLilyMotel_2F_EventScript_ShowDiploma
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_ShowDiploma::
	message LilycoveCity_CoveLilyMotel_2F_Text_FilledPokedexGiveYouThis
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	special Special_ShowDiploma
	release
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_Programmer::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_ImTheProgrammer, MSGBOX_NPC
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_GraphicArtist::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_ImTheGraphicArtist, MSGBOX_NPC
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_FatMan::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_GirlsAreCute, MSGBOX_NPC
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_Woman::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_SeaBreezeTicklesHeart, MSGBOX_NPC
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_GameBoyKid::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_NeverLeaveWithoutGameBoy, MSGBOX_NPC
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_Scott::
	lock
	faceplayer
	goto_if_set FLAG_MET_SCOTT_IN_LILYCOVE, LilycoveCity_CoveLilyMotel_2F_EventScript_MetScott
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_SnoozingPreferBattles, MSGBOX_DEFAULT
	addvar VAR_SCOTT_STATE, 1
	setflag FLAG_MET_SCOTT_IN_LILYCOVE
	release
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_MetScott::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_ContestsDoTakeStrategy, MSGBOX_DEFAULT
	release
	end

LilycoveCity_CoveLilyMotel_2F_Text_ShowMeCompletedDex:
	.string "{JPN}ぼくは ゲーム デザイナー\pへー\nポケモンずかん あつめてるの?\pぜんぶ あつめるの\nたいへん だけど がんばって!\pもし ぜんぶ あつめたら おしえてよ!$"

LilycoveCity_CoveLilyMotel_2F_Text_FilledPokedexGiveYouThis:
	.string "{JPN}すごいなー!\nいやー すごいよ きみ!!\pこの ポケモンずかん\nぜんぶ あつまって いるんだね!\lそれだけ ポケモンを あいしてるんだね!\pかんどう しました!\nきみを ひょうしょう させてもらいますね!$"

LilycoveCity_CoveLilyMotel_2F_Text_ImTheProgrammer:
	.string "{JPN}おれさま?\pプログラマーだよ! こっちの\nスロットは どんなのか たのしみだな!$"

LilycoveCity_CoveLilyMotel_2F_Text_ImTheGraphicArtist:
	.string "{JPN}おれ グラフィッカー!\nホウエンの ポケモンは おもしろいね$"

LilycoveCity_CoveLilyMotel_2F_Text_GirlsAreCute:
	.string "{JPN}うきわガール かわいい でしょ!\nうきわガールと しょうぶ なんて……\pくー! たまらんー!\pふたごちゃん かわいい でしょ!\nふたごちゃんと 2たい2 なんて……\pくー! たまらんー!$"

LilycoveCity_CoveLilyMotel_2F_Text_SeaBreezeTicklesHeart:
	.string "{JPN}なんだか しおかぜが こころを くすぐって\nきもち いいですね!$"

LilycoveCity_CoveLilyMotel_2F_Text_NeverLeaveWithoutGameBoy:
	.string "{JPN}いつ どこで たいせんや しょうぶを\nいどまれるか わからないからな\lゲームボーイ アドバンスは てばなせないよ!$"

LilycoveCity_CoveLilyMotel_2F_Text_SnoozingPreferBattles:
	.string "{JPN}エニシダ『…… ……\n…… …… ……ズ\p…… …… ……!\nあ ああ {PLAYER}{KUN}\lごめんごめん ひるね してたんだ\pポケモンコンテスト ってのが\nどんな ものか みに きたんだ\lなかなか おもしろそう だけど……\pまあ わたしは やっぱり\nじゅんすいに たたかって つよい\lトレーナーが すき だけどね\pうん {PLAYER}{KUN}は\nジムに コンテストに バトルテント\lなんでも たのしんで くれよ!$"

LilycoveCity_CoveLilyMotel_2F_Text_ContestsDoTakeStrategy:
	.string "{JPN}エニシダ『コンテストに かつための\nせんりゃくを ねるのも\lつよい トレーナーに なるための\lほうほうの ひとつでは あるね$"

