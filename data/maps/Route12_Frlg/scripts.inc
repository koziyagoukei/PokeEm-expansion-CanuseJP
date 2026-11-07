Route12_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, Route12_OnResume
	.byte 0

Route12_OnResume::
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, Route12_EventScript_TryRemoveSnorlax
	end

Route12_EventScript_TryRemoveSnorlax::
	removeobject VAR_LAST_TALKED
	return

Route12_EventScript_Snorlax::
	lock
	faceplayer
	goto_if_unset FLAG_GOT_POKE_FLUTE, Route12_EventScript_SnorlaxNoPokeFlute
	msgbox Text_WantToUsePokeFlute, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route12_EventScript_DontUsePokeFlute
	call EventScript_AwakenSnorlax
	setwildbattle SPECIES_SNORLAX, 30
	waitse
	playmoncry SPECIES_SNORLAX, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_HIDE_ROUTE_12_SNORLAX
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	setflag FLAG_WOKE_UP_ROUTE_12_SNORLAX
	dowildbattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route12_EventScript_FoughtSnorlax
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Route12_EventScript_FoughtSnorlax
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Route12_EventScript_FoughtSnorlax
	release
	end

Route12_EventScript_DontUsePokeFlute::
	release
	end

Route12_EventScript_FoughtSnorlax::
	msgbox Text_SnorlaxReturnedToMountains
	release
	end

Route12_EventScript_SnorlaxNoPokeFlute::
	msgbox Route12_Text_MonSprawledOutInSlumber
	release
	end

Route12_EventScript_RouteSign::
	msgbox Route12_Text_RouteSign, MSGBOX_SIGN
	end

Route12_EventScript_FishingSign::
	msgbox Route12_Text_SportfishingArea, MSGBOX_SIGN
	end

Route12_Text_MonSprawledOutInSlumber::
	.string "{JPN}ポケモンが とても\n"
	.string "きもち よさそうに ねてます$"

Text_SnorlaxWokeUp::
	.string "{JPN}カビゴンが めを さました!\p"
	.string "カビゴンは\n"
	.string "ねぼけて おそってきた!$"

Text_SnorlaxReturnedToMountains::
	.string "{JPN}カビゴンは おとなしく なった\n"
	.string "おおきな あくびを すると…\l"
	.string "やまおくへ さって いった!$"

Text_WantToUsePokeFlute::
	.string "{JPN}ポケモンのふえを つかいますか?$"

Text_PlayedPokeFlute::
	.string "{JPN}{PLAYER}は\n"
	.string "ポケモンのふえを つかった!$"

Route12_Text_NedIntro::
	.string "{JPN}おっ おーッ!\n"
	.string "ひいてる ひいてる!$"

Route12_Text_NedDefeat::
	.string "{JPN}ちッ!\n"
	.string "げどう だったか$"

Route12_Text_NedPostBattle::
	.string "{JPN}まって くれ!\n"
	.string "つりいとが こんがらがった!$"

Route12_Text_ChipIntro::
	.string "{JPN}… あわてるな!\n"
	.string "まつ ことも また!\l"
	.string "つりの たのしみ である$"

Route12_Text_ChipDefeat::
	.string "{JPN}おっと…!\n"
	.string "ちょっと まって くれ$"

Route12_Text_ChipPostBattle::
	.string "{JPN}いい つりざおを つかえば\n"
	.string "いい ポケモンが つれるんだが…$"

Route12_Text_JustinIntro::
	.string "{JPN}つきのいし さがしてるんだ けど\n"
	.string "きみ もって ない?$"

Route12_Text_JustinDefeat::
	.string "{JPN}ありゃりゃ…!$"

Route12_Text_JustinPostBattle::
	.string "{JPN}つきのいしが あれば\n"
	.string "ポケモンが しんか して\l"
	.string "ぼく かってた かも しれないよ$"

Route12_Text_LucaIntro::
	.string "{JPN}ぼくの せんもん でんき だから\n"
	.string "うみ ポケモンは くわしく ないよ$"

Route12_Text_LucaDefeat::
	.string "{JPN}ふにゃ ふにゃ…$"

Route12_Text_LucaPostBattle::
	.string "{JPN}うーん… たしか みずは\n"
	.string "でんきを よく とおす から\l"
	.string "わざが きく はず だよね…$"

Route12_Text_HankIntro::
	.string "{JPN}おうッ! つり バカ\n"
	.string "ポケモン バカに であう!$"

Route12_Text_HankDefeat::
	.string "{JPN}さすが つよい!$"

Route12_Text_HankPostBattle::
	.string "{JPN}すき こそ\n"
	.string "ものの じょうず なれ… だな!\l"
	.string "ぼくも つり なら まけないぞ!$"

Route12_Text_ElliotIntro::
	.string "{JPN}つり ばっかり して ないで\n"
	.string "もっと しごとも できれば\l"
	.string "さいこう なんだが…$"

Route12_Text_ElliotDefeat::
	.string "{JPN}なかなか…\n"
	.string "うまく いかん もの じゃ$"

Route12_Text_ElliotPostBattle::
	.string "{JPN}ははは… だいじょうぶ!\n"
	.string "まけた くらいで はらは たてんよ$"

Route12_Text_AndrewIntro::
	.string "{JPN}… なにが つれるかって?\n"
	.string "そりゃ つって みなけりゃ\l"
	.string "わからんよ!$"

Route12_Text_AndrewDefeat::
	.string "{JPN}しくじったな$"

Route12_Text_AndrewPostBattle::
	.string "{JPN}ああ コイキング… ね\n"
	.string "よく つれる けど\l"
	.string "よわすぎるんだよなあ…$"

Route12_Text_RouteSign::
	.string "{JPN}ここは 12ばん どうろ\n"
	.string "きた … シオン タウン$"

Route12_Text_SportfishingArea::
	.string "{JPN}ここは つりの めいしょ$"

Route12_Text_JesIntro::
	.string "{JPN}シゲ『か かったら\n"
	.string "アユに プロポーズ するぞ…!$"

Route12_Text_JesDefeat::
	.string "{JPN}シゲ『たのむよ\n"
	.string "かたせて くれよお$"

Route12_Text_JesPostBattle::
	.string "{JPN}シゲ『アユ ごめんね ごめんね$"

Route12_Text_JesNotEnoughMons::
	.string "{JPN}シゲ『ぼくと アユは\n"
	.string "ずーーーっと いっしょ なんだ!\l"
	.string "だから ポケモンも\l"
	.string "2たい2 じゃないと やらないよ$"

Route12_Text_GiaIntro::
	.string "{JPN}アユ『ねえ シゲ…\p"
	.string "もし かてたら\n"
	.string "けっこん して あげる!$"

Route12_Text_GiaDefeat::
	.string "{JPN}アユ『あーん なんでー?$"

Route12_Text_GiaPostBattle::
	.string "{JPN}アユ『シゲの バカバカあー!$"

Route12_Text_GiaNotEnoughMons::
	.string "{JPN}アユ『シゲと べつべつに なんて\n"
	.string "しょうぶ できない!\l"
	.string "ポケモン もう1ぴき いない?$"
