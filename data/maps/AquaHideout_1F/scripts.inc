AquaHideout_1F_MapScripts::
	.byte 0

@ The below two entrance guards give hints about what to do to progress the story
AquaHideout_1F_EventScript_HideoutEntranceGrunt1::
	lock
	faceplayer
	goto_if_set FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT, AquaHideout_1F_EventScript_SlateportHint1
	goto_if_set FLAG_RECEIVED_RED_OR_BLUE_ORB, AquaHideout_1F_EventScript_MagmaHideoutHint1
	msgbox AquaHideout_1F_Text_OurBossIsSnatchingSomething, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_MagmaHideoutHint1::
	msgbox AquaHideout_1F_Text_WhereMightMagmaHideoutBe, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_SlateportHint1::
	msgbox AquaHideout_1F_Text_BossWentToJackASubmarine, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_HideoutEntranceGrunt2::
	lock
	faceplayer
	goto_if_set FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT, AquaHideout_1F_EventScript_SlateportHint2
	goto_if_set FLAG_RECEIVED_RED_OR_BLUE_ORB, AquaHideout_1F_EventScript_MagmaHideoutHint2
	msgbox AquaHideout_1F_Text_BossIsOnRoute122, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_MagmaHideoutHint2::
	msgbox AquaHideout_1F_Text_TeamMagmaAtMtChimney, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_SlateportHint2::
	msgbox AquaHideout_1F_Text_BossIsInSlateportCity, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_AQUA_HIDEOUT_1, AquaHideout_1F_Text_Grunt1Intro, AquaHideout_1F_Text_Grunt1Defeat, AquaHideout_1F_EventScript_Grunt1Defeated
	msgbox AquaHideout_1F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

AquaHideout_1F_EventScript_Grunt1Defeated::
	msgbox AquaHideout_1F_Text_Grunt1PostBattle, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_Text_OurBossIsSnatchingSomething:
	.string "{JPN}なんだ なんだ!\nおれたち アクアだんに なんの ようだ!\pリーダーだったら いま だいじな ものを\nぶんどりに でかけていて るすだぞ!\p……どこに いったのか だって?\pわはははっ! そんな だいじな ことを\nおしえられる わけ ないだろうが!$"

AquaHideout_1F_Text_WhereMightMagmaHideoutBe:
	.string "{JPN}なんだ なんだ!\nおまえは マグマだんの したっぱか!?\pマグマだんめ アジトで すごいポケモンを\nめざめさせる つもり らしいな!\pえーい! アジトは\nいったい どこに あるって いうんだ!$"

AquaHideout_1F_Text_BossWentToJackASubmarine:
	.string "{JPN}なんだ なんだ!\nおれたち アクアだんに なんの ようだ!\pリーダーだったら いま せんすいていを\nぶんどりに でかけていて るすだぞ!\p……どこに いったのか だって?\pわはははっ! そんな だいじな ことを\nおしえられる わけ ないだろうが!$"

AquaHideout_1F_Text_BossIsOnRoute122:
	.string "{JPN}なんだ なんだ!\nおれたち アクアだんに なんの ようだ!\pリーダーは いま 122ばん すいどうの\nおくりびやまに むかっていて るすだぞ!\p……なにしに いったのか だって?\pわはははっ! そんな だいじな ことを\nおしえられる わけ ないだろうが!$"

AquaHideout_1F_Text_TeamMagmaAtMtChimney:
	.string "{JPN}なんだ なんだ!\nおまえは マグマだんの したっぱか!?\pえんとつやまには マグマだんが ねらう\nすごい ポケモンが いるらしいな!\pえーい! そいつは\nいったい どんな ポケモン なんだ!$"

AquaHideout_1F_Text_BossIsInSlateportCity:
	.string "{JPN}なんだ なんだ!\nおれたち アクアだんに なんの ようだ!\pリーダーは いま カイナシティに\nむかっていて るすだぞ!\p……なにしに いったのか だって?\pわはははっ! そんな だいじな ことを\nおしえられる わけ ないだろうが!$"

AquaHideout_1F_Text_Grunt1Intro:
	.string "{JPN}ややや!\nあやしい じんぶつを はっけーん!$"

AquaHideout_1F_Text_Grunt1Defeat:
	.string "{JPN}うーむ まけちまったかー!$"

AquaHideout_1F_Text_Grunt1PostBattle:
	.string "{JPN}しょうぶには まけたが\nおれは やくめを はたしたのだ……$"
