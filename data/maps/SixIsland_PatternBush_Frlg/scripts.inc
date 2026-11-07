SixIsland_PatternBush_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SixIsland_PatternBush_OnTransition
	.byte 0

SixIsland_PatternBush_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_SIX_ISLAND_PATTERN_BUSH
	getplayerxy VAR_TEMP_1, VAR_TEMP_2
	call_if_ge VAR_TEMP_1, 50, SixIsland_PatternBush_EventScript_SetEscapeRightExit
	call_if_le VAR_TEMP_1, 49, SixIsland_PatternBush_EventScript_SetEscapeLeftExit
	end

SixIsland_PatternBush_EventScript_SetEscapeRightExit::
	setescapewarp MAP_SIX_ISLAND_GREEN_PATH, 255, 64, 10
	return

SixIsland_PatternBush_EventScript_SetEscapeLeftExit::
	setescapewarp MAP_SIX_ISLAND_GREEN_PATH, 255, 45, 10
	return

SixIsland_PatternBush_EventScript_Bethany::
	trainerbattle_single TRAINER_PKMN_BREEDER_BETHANY, SixIsland_PatternBush_Text_BethanyIntro, SixIsland_PatternBush_Text_BethanyDefeat
	msgbox SixIsland_PatternBush_Text_BethanyPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Allison::
	trainerbattle_single TRAINER_PKMN_BREEDER_ALLISON, SixIsland_PatternBush_Text_AllisonIntro, SixIsland_PatternBush_Text_AllisonDefeat
	msgbox SixIsland_PatternBush_Text_AllisonPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Garret::
	trainerbattle_single TRAINER_BUG_CATCHER_GARRET, SixIsland_PatternBush_Text_GarretIntro, SixIsland_PatternBush_Text_GarretDefeat
	msgbox SixIsland_PatternBush_Text_GarretPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Jonah::
	trainerbattle_single TRAINER_BUG_CATCHER_JONAH, SixIsland_PatternBush_Text_JonahIntro, SixIsland_PatternBush_Text_JonahDefeat
	msgbox SixIsland_PatternBush_Text_JonahPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Vance::
	trainerbattle_single TRAINER_BUG_CATCHER_VANCE, SixIsland_PatternBush_Text_VanceIntro, SixIsland_PatternBush_Text_VanceDefeat
	msgbox SixIsland_PatternBush_Text_VancePostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Nash::
	trainerbattle_single TRAINER_YOUNGSTER_NASH, SixIsland_PatternBush_Text_NashIntro, SixIsland_PatternBush_Text_NashDefeat
	msgbox SixIsland_PatternBush_Text_NashPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Cordell::
	trainerbattle_single TRAINER_YOUNGSTER_CORDELL, SixIsland_PatternBush_Text_CordellIntro, SixIsland_PatternBush_Text_CordellDefeat
	msgbox SixIsland_PatternBush_Text_CordellPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Dalia::
	trainerbattle_single TRAINER_LASS_DALIA, SixIsland_PatternBush_Text_DaliaIntro, SixIsland_PatternBush_Text_DaliaDefeat
	msgbox SixIsland_PatternBush_Text_DaliaPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Joana::
	trainerbattle_single TRAINER_LASS_JOANA, SixIsland_PatternBush_Text_JoanaIntro, SixIsland_PatternBush_Text_JoanaDefeat
	msgbox SixIsland_PatternBush_Text_JoanaPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Riley::
	trainerbattle_single TRAINER_CAMPER_RILEY, SixIsland_PatternBush_Text_RileyIntro, SixIsland_PatternBush_Text_RileyDefeat
	msgbox SixIsland_PatternBush_Text_RileyPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Marcy::
	trainerbattle_single TRAINER_PICNICKER_MARCY, SixIsland_PatternBush_Text_MarcyIntro, SixIsland_PatternBush_Text_MarcyDefeat
	msgbox SixIsland_PatternBush_Text_MarcyPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Layton::
	trainerbattle_single TRAINER_RUIN_MANIAC_LAYTON, SixIsland_PatternBush_Text_LaytonIntro, SixIsland_PatternBush_Text_LaytonDefeat
	msgbox SixIsland_PatternBush_Text_LaytonPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_Text_BethanyIntro::
	.string "{JPN}あなたは どんな ふうに\n"
	.string "ポケモンを そだてて いるの?$"

SixIsland_PatternBush_Text_BethanyDefeat::
	.string "{JPN}あいじょう たっぷりね!$"

SixIsland_PatternBush_Text_BethanyPostBattle::
	.string "{JPN}あなたのママが\n"
	.string "あなたを そだてた ように\l"
	.string "あなたも ポケモンを そだてるのよ$"

SixIsland_PatternBush_Text_AllisonIntro::
	.string "{JPN}この はやしで ポケモンの\n"
	.string "しぜんほごを おこなって いるの$"

SixIsland_PatternBush_Text_AllisonDefeat::
	.string "{JPN}わかいのに すごいのねえ$"

SixIsland_PatternBush_Text_AllisonPostBattle::
	.string "{JPN}ポケモンを つかまえちゃ ダメって\n"
	.string "わけでは ないの\l"
	.string "つかまえたら だいじに そだててね$"

SixIsland_PatternBush_Text_GarretIntro::
	.string "{JPN}このへんに しか いない\n"
	.string "むし ポケモン とったぞー$"

SixIsland_PatternBush_Text_GarretDefeat::
	.string "{JPN}へへ…\n"
	.string "ぼくの ポケモン いいでしょ$"

SixIsland_PatternBush_Text_GarretPostBattle::
	.string "{JPN}ヘラクロスの おおきさを\n"
	.string "はかって くれる おねえさんが\l"
	.string "はやしの ちかくに いるんだよ$"

SixIsland_PatternBush_Text_JonahIntro::
	.string "{JPN}やっぱ むしポケ じゃないと\n"
	.string "ポケモン やってる きが しない!$"

SixIsland_PatternBush_Text_JonahDefeat::
	.string "{JPN}いいよ! まけるのも\n"
	.string "ポケモンの だいごみ だもん$"

SixIsland_PatternBush_Text_JonahPostBattle::
	.string "{JPN}ところでさ\n"
	.string "だいごみ って なんだろ?\l"
	.string "でかいゴミの ことかな?$"

SixIsland_PatternBush_Text_VanceIntro::
	.string "{JPN}うきゃ うきゃ!\n"
	.string "むしポケモンが いっぱい だあ!$"

SixIsland_PatternBush_Text_VanceDefeat::
	.string "{JPN}よろこんでる うちに\n"
	.string "まけちまったい$"

SixIsland_PatternBush_Text_VancePostBattle::
	.string "{JPN}こんど おとうとも\n"
	.string "つれてきて やろーっと!$"

SixIsland_PatternBush_Text_NashIntro::
	.string "{JPN}ねえねえ!\n"
	.string "じめんに へんな もようが あるよ$"

SixIsland_PatternBush_Text_NashDefeat::
	.string "{JPN}すげー!$"

SixIsland_PatternBush_Text_NashPostBattle::
	.string "{JPN}じめんの へんな もよう…\p"
	.string "じいちゃんの きてる ふくの\n"
	.string "ガラに にてるぞ…$"

SixIsland_PatternBush_Text_CordellIntro::
	.string "{JPN}おまえ おれの こと\n"
	.string "いなかもん とか おもっただろ$"

SixIsland_PatternBush_Text_CordellDefeat::
	.string "{JPN}いなかもんは よわい とか\n"
	.string "おもった だろ ちきしょー$"

SixIsland_PatternBush_Text_CordellPostBattle::
	.string "{JPN}そんな カッコつけた ぼうし\n"
	.string "かぶっちゃってさー\p"
	.string "おれに くれよ!$"

SixIsland_PatternBush_Text_DaliaIntro::
	.string "{JPN}しんこきゅう して みて\n"
	.string "くうきが おいしいよ!$"

SixIsland_PatternBush_Text_DaliaDefeat::
	.string "{JPN}しんこきゅうの ほうほうを\n"
	.string "おしえて あげる!$"

SixIsland_PatternBush_Text_DaliaPostBattle::
	.string "{JPN}まず おもいきり いきを はいて\p"
	.string "もうダメって ほど はきだしたら\n"
	.string "しんせんな くうきを\l"
	.string "おおきく すいこむの!\p"
	.string "すっきり した?$"

SixIsland_PatternBush_Text_JoanaIntro::
	.string "{JPN}むし ポケモンが だいすきで\n"
	.string "ここに よく くるんだ!\l"
	.string "こんな おんなのこ あたしだけ?$"

SixIsland_PatternBush_Text_JoanaDefeat::
	.string "{JPN}まけたけど わらって いられるよ!\n"
	.string "こんな おんなのこ あたしだけ?$"

SixIsland_PatternBush_Text_JoanaPostBattle::
	.string "{JPN}これからも むし ポケモン\n"
	.string "いっぱい あつめるんだー!\l"
	.string "こんな おんなのこ あたしだけ?$"

SixIsland_PatternBush_Text_RileyIntro::
	.string "{JPN}きめたぞ\n"
	.string "ここに テントを はろうっと$"

SixIsland_PatternBush_Text_RileyDefeat::
	.string "{JPN}へなへな$"

SixIsland_PatternBush_Text_RileyPostBattle::
	.string "{JPN}ここで よぞらの\n"
	.string "かんさつを するのさ$"

SixIsland_PatternBush_Text_MarcyIntro::
	.string "{JPN}いやーん!\n"
	.string "ムシに さされちゃった かな?$"

SixIsland_PatternBush_Text_MarcyDefeat::
	.string "{JPN}ムシじゃ なくて\n"
	.string "くさで あしを きってた みたい$"

SixIsland_PatternBush_Text_MarcyPostBattle::
	.string "{JPN}この くらいの キズ\n"
	.string "なめときゃ なおるわね!$"

SixIsland_PatternBush_Text_LaytonIntro::
	.string "{JPN}きみ なにか\n"
	.string "きがついた ことは ないかね?$"

SixIsland_PatternBush_Text_LaytonDefeat::
	.string "{JPN}あしもとに ちゅうもく\n"
	.string "して みたかい?$"

SixIsland_PatternBush_Text_LaytonPostBattle::
	.string "{JPN}この はやし には\n"
	.string "くさが はえない ところが ある…\p"
	.string "どうして なんだろう?$"

