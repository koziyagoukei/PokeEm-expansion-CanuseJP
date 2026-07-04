AbandonedShip_Rooms2_1F_MapScripts::
	.byte 0

AbandonedShip_Rooms2_1F_EventScript_Dan::
	trainerbattle_double TRAINER_KIRA_AND_DAN_1, AbandonedShip_Rooms2_1F_Text_DanIntro, AbandonedShip_Rooms2_1F_Text_DanDefeat, AbandonedShip_Rooms2_1F_Text_DanNotEnoughMons, AbandonedShip_Rooms2_1F_EventScript_RegisterDan
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, AbandonedShip_Rooms2_1F_EventScript_DanRematch
	msgbox AbandonedShip_Rooms2_1F_Text_DanPostBattle, MSGBOX_DEFAULT
	release
	end

AbandonedShip_Rooms2_1F_EventScript_RegisterDan::
	msgbox AbandonedShip_Rooms2_1F_Text_KiraRegister, MSGBOX_DEFAULT  @ Kira speaks for both when registering KiraAndDan
	register_matchcall TRAINER_KIRA_AND_DAN_1
	release
	end

AbandonedShip_Rooms2_1F_EventScript_DanRematch::
	trainerbattle_rematch_double TRAINER_KIRA_AND_DAN_1, AbandonedShip_Rooms2_1F_Text_DanRematchIntro, AbandonedShip_Rooms2_1F_Text_DanRematchDefeat, AbandonedShip_Rooms2_1F_Text_DanRematchNotEnoughMons
	msgbox AbandonedShip_Rooms2_1F_Text_DanPostRematch, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms2_1F_EventScript_Kira::
	trainerbattle_double TRAINER_KIRA_AND_DAN_1, AbandonedShip_Rooms2_1F_Text_KiraIntro, AbandonedShip_Rooms2_1F_Text_KiraDefeat, AbandonedShip_Rooms2_1F_Text_KiraNotEnoughMons, AbandonedShip_Rooms2_1F_EventScript_RegisterKira
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, AbandonedShip_Rooms2_1F_EventScript_KiraRematch
	msgbox AbandonedShip_Rooms2_1F_Text_KiraPostBattle, MSGBOX_DEFAULT
	release
	end

AbandonedShip_Rooms2_1F_EventScript_RegisterKira::
	msgbox AbandonedShip_Rooms2_1F_Text_KiraRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_KIRA_AND_DAN_1
	release
	end

AbandonedShip_Rooms2_1F_EventScript_KiraRematch::
	trainerbattle_rematch_double TRAINER_KIRA_AND_DAN_1, AbandonedShip_Rooms2_1F_Text_KiraRematchIntro, AbandonedShip_Rooms2_1F_Text_KiraRematchDefeat, AbandonedShip_Rooms2_1F_Text_KiraRematchNotEnoughMons
	msgbox AbandonedShip_Rooms2_1F_Text_KiraPostRematch, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms2_1F_EventScript_Jani::
	trainerbattle_single TRAINER_JANI, AbandonedShip_Rooms2_1F_Text_JaniIntro, AbandonedShip_Rooms2_1F_Text_JaniDefeat
	msgbox AbandonedShip_Rooms2_1F_Text_JaniPostBattle, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms2_1F_EventScript_Garrison::
	trainerbattle_single TRAINER_GARRISON, AbandonedShip_Rooms2_1F_Text_GarrisonIntro, AbandonedShip_Rooms2_1F_Text_GarrisonDefeat
	msgbox AbandonedShip_Rooms2_1F_Text_GarrisonPostBattle, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms2_1F_Text_DanIntro:
	.string "{JPN}フミ『おたから さがしていたら\nトレーナー はっけん!$"

AbandonedShip_Rooms2_1F_Text_DanDefeat:
	.string "{JPN}フミ『ぼくたち ふたりの ちからでも\nかてなかったのか……$"

AbandonedShip_Rooms2_1F_Text_DanPostBattle:
	.string "{JPN}フミ『おたから みつからないんだよね……\nもう だれかが もっていったのかな?$"

AbandonedShip_Rooms2_1F_Text_DanNotEnoughMons:
	.string "{JPN}フミ『ポケモン 2ひき いないんだね\nそれだと ぼくたちには かてないよ!$"

AbandonedShip_Rooms2_1F_Text_KiraIntro:
	.string "{JPN}ナツ『あら? おたから さがしていたのに\nトレーナー はっけん しちゃった!$"

AbandonedShip_Rooms2_1F_Text_KiraDefeat:
	.string "{JPN}ナツ『つよいんだ!$"

AbandonedShip_Rooms2_1F_Text_KiraPostBattle:
	.string "{JPN}ナツ『おたから どこかな?\nみつかったら なに かおうか きめてるのよ!$"

AbandonedShip_Rooms2_1F_Text_KiraNotEnoughMons:
	.string "{JPN}ナツ『あら ポケモン 2ひき いないの?\nじゃあ しょうぶは おあずけね!$"

AbandonedShip_Rooms2_1F_Text_KiraRegister:
	.string "{JPN}ナツ『もう くやしいな!\nあなたなんて とうろく しちゃうんだから!$"

AbandonedShip_Rooms2_1F_Text_DanRematchIntro:
	.string "{JPN}フミ『ずっと おたから さがしていて\nポケモンも つよく なったんだよねー!\lちょっと あいて してみてよ$"

AbandonedShip_Rooms2_1F_Text_DanRematchDefeat:
	.string "{JPN}フミ『あいかわらず つよいな!$"

AbandonedShip_Rooms2_1F_Text_DanPostRematch:
	.string "{JPN}フミ『おたからは みつからないし\nポケモンは まけるし いえに かえりたいよ\pでも そんなこと いうと\nかのじょ おこりだすからな……$"

AbandonedShip_Rooms2_1F_Text_DanRematchNotEnoughMons:
	.string "{JPN}フミ『ポケモン 2ひき いないんだね\nそれだと ぼくたちには かてないよ!$"

AbandonedShip_Rooms2_1F_Text_KiraRematchIntro:
	.string "{JPN}ナツ『あら? また であったわね!\nあたしたちと いっしょで あきらめずに\lおたから さがしているのね!\pしょうぶして まけた ほうは\nあきらめることに しない?$"

AbandonedShip_Rooms2_1F_Text_KiraRematchDefeat:
	.string "{JPN}ナツ『また まけちゃった……$"

AbandonedShip_Rooms2_1F_Text_KiraPostRematch:
	.string "{JPN}ナツ『もっと ポケモン そだてて\nおたから みつけるまで かえらないわよ!$"

AbandonedShip_Rooms2_1F_Text_KiraRematchNotEnoughMons:
	.string "{JPN}ナツ『あら ポケモン 2ひき いないの?\nじゃあ しょうぶは おあずけね!$"

AbandonedShip_Rooms2_1F_Text_JaniIntro:
	.string "{JPN}およぐの ヘタだけど\nしょうぶは つよいもーん$"

AbandonedShip_Rooms2_1F_Text_JaniDefeat:
	.string "{JPN}あれ だめだったかあ$"

AbandonedShip_Rooms2_1F_Text_JaniPostBattle:
	.string "{JPN}ふねの なかを はだしで あるくと\nちょっと きもちわるいなあ$"

AbandonedShip_Rooms2_1F_Text_GarrisonIntro:
	.string "{JPN}つよさも やさしさも\nこれ トレーナーの たから!$"

AbandonedShip_Rooms2_1F_Text_GarrisonDefeat:
	.string "{JPN}おお あなたには\nキラリと ひかるものが あるな$"

AbandonedShip_Rooms2_1F_Text_GarrisonPostBattle:
	.string "{JPN}ふねの どこかの へやに\nキラリと ひかるものが あったんだが……$"
