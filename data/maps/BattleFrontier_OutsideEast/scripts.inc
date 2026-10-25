BattleFrontier_OutsideEast_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, BattleFrontier_OutsideEast_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_OutsideEast_OnTransition
	.byte 0

BattleFrontier_OutsideEast_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, BattleFrontier_OutsideEast_EventScript_TryRemoveSudowoodo
	end

BattleFrontier_OutsideEast_EventScript_TryRemoveSudowoodo::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

BattleFrontier_OutsideEast_OnTransition:
	setvar VAR_BRAVO_TRAINER_BATTLE_TOWER_ON, 0
	setflag FLAG_HIDE_BATTLE_TOWER_REPORTER
	call_if_unset FLAG_DEFEATED_SUDOWOODO, BattleFrontier_OutsideEast_EventScript_ShowSudowoodo
	end

BattleFrontier_OutsideEast_EventScript_ShowSudowoodo::
	clearflag FLAG_HIDE_BATTLE_FRONTIER_SUDOWOODO
	return

BattleFrontier_OutsideEast_EventScript_BattleTowerSign::
	msgbox BattleFrontier_OutsideEast_Text_BattleTowerSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_BattlePalaceSign::
	msgbox BattleFrontier_OutsideEast_Text_BattlePalaceSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_BattleArenaSign::
	msgbox BattleFrontier_OutsideEast_Text_BattleArenaSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_BattlePyramidSign::
	msgbox BattleFrontier_OutsideEast_Text_BattlePyramidSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_NinjaBoy::
	msgbox BattleFrontier_OutsideEast_Text_BattleTowerFeelsSpecial, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Man1::
	msgbox BattleFrontier_OutsideEast_Text_ConquerLeagueAndFrontier, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Hiker::
	msgbox BattleFrontier_OutsideEast_Text_PyramidTooHarsh, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_HexManiac::
	msgbox BattleFrontier_OutsideEast_Text_ThriveInDarkness, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_BlackBelt1::
	msgbox BattleFrontier_OutsideEast_Text_PutTogetherUltimateTeam, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Cook::
	lock
	msgbox BattleFrontier_OutsideEast_Text_BelieveInYouBuddy, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox BattleFrontier_OutsideEast_Text_ZigzagoonLooksVacant, MSGBOX_DEFAULT
	waitmoncry
	release
	end

BattleFrontier_OutsideEast_EventScript_RichBoy::
	msgbox BattleFrontier_OutsideEast_Text_PeopleCallMeBusybody, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_ExpertF::
	msgbox BattleFrontier_OutsideEast_Text_OnceBeatGymLeader, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_TriathleteF::
	msgbox BattleFrontier_OutsideEast_Text_FastOnBikeAndBattles, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Twin::
	msgbox BattleFrontier_OutsideEast_Text_BetterThanDaddyAtPokemon, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Man2::
	lock
	msgbox BattleFrontier_OutsideEast_Text_GoRackUpSomeWinsForDaddy, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_TriathleteM::
	msgbox BattleFrontier_OutsideEast_Text_DidScottBringYouHere, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Sudowoodo::
	lock
	faceplayer
	waitse
	playse SE_SUDOWOODO_SHAKE
	applymovement LOCALID_FRONTIER_SUDOWOODO, BattleFrontier_OutsideEast_Movement_SudowoodoShake
	waitmovement 0
	release
	end

BattleFrontier_OutsideEast_EventScript_WaterSudowoodo::
	lock
	faceplayer
	special DoWateringBerryTreeAnim
	waitse
	playse SE_SUDOWOODO_SHAKE
	applymovement LOCALID_FRONTIER_SUDOWOODO, BattleFrontier_OutsideEast_Movement_SudowoodoShake
	waitmovement 0
	msgbox gText_Sudowoodo_Attacked, MSGBOX_DEFAULT
	closemessage
	waitse
	playmoncry SPECIES_SUDOWOODO, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_FRONTIER_SUDOWOODO
	setwildbattle SPECIES_SUDOWOODO, 40
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	dowildbattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo
	setflag FLAG_DEFEATED_SUDOWOODO
	release
	end

BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo::
	setflag FLAG_DEFEATED_SUDOWOODO
	goto Common_EventScript_RemoveStaticPokemon
	end

BattleFrontier_OutsideEast_Movement_SudowoodoShake:
	face_right
	delay_8
	face_down
	delay_8
	face_left
	delay_8
	face_down
	delay_8
	face_right
	delay_8
	face_down
	delay_8
	face_left
	delay_8
	face_down
	step_end

BattleFrontier_OutsideEast_EventScript_Maniac1::
	msgbox BattleFrontier_OutsideEast_Text_HeardPrettyGirlAtBattleArena, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Girl::
	msgbox BattleFrontier_OutsideEast_Text_SometimesImportantOldManInThere, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_PsychicM::
	msgbox BattleFrontier_OutsideEast_Text_LegendOfBattlePyramid, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Man3::
	msgbox BattleFrontier_OutsideEast_Text_GotWipedOut, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Woman1::
	msgbox BattleFrontier_OutsideEast_Text_ToughTrainerInBattleTower, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Woman2::
	msgbox BattleFrontier_OutsideEast_Text_EnoughBattlePointsForDoll, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_BlackBelt2::
	msgbox BattleFrontier_OutsideEast_Text_LikeToHaveNameRecordedHere, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Maniac2::
	lock
	msgbox BattleFrontier_OutsideEast_Text_CanDoAnythingWithYou, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_Woman3::
	lock
	msgbox BattleFrontier_OutsideEast_Text_PowerOfOurLoveWillOvercome, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_RankingHallSign::
	msgbox BattleFrontier_OutsideEast_Text_RankingHallSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_ExchangeCornerSign::
	msgbox BattleFrontier_OutsideEast_Text_ExchangeCornerSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_Gentleman::
	msgbox BattleFrontier_OutsideEast_Text_FrontierNotExclusivelyForToughTrainers, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_OldWoman::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_SUDOWOODO, BattleFrontier_OutsideEast_EventScript_OldWomanSudowoodoGone
	msgbox BattleFrontier_OutsideEast_Text_OddTreeHereSeemsToWiggle, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_OldWomanSudowoodoGone::
	msgbox BattleFrontier_OutsideEast_Text_OnceAnOddTreePastHere, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_Camper::
	msgbox BattleFrontier_OutsideEast_Text_StickyMonWithLongTail, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_Text_BattleTowerSign:
	.string "ここは バトルタワー!\nかちぬけ! さいきょう トレーナー!$"

BattleFrontier_OutsideEast_Text_BattlePalaceSign:
	.string "ここは バトルパレス!\nみまもれ! ポケモンバトル!$"

BattleFrontier_OutsideEast_Text_BattleArenaSign:
	.string "ここは バトルアリーナ!\nつどえ! さいきょう チーム!$"

BattleFrontier_OutsideEast_Text_BattlePyramidSign:
	.string "ここは バトルピラミッド!\nすすめ! バトルアドベンチャー!$"

BattleFrontier_OutsideEast_Text_RankingHallSign:
	.string "バトルフロンティア ランキングホール\nきざめ! さいこうの きろく!$"

BattleFrontier_OutsideEast_Text_ExchangeCornerSign:
	.string "バトルポイント ひきかえサービス コーナー\nひきかえろ! キミの バトルポイント!$"

BattleFrontier_OutsideEast_Text_BattleTowerFeelsSpecial:
	.string "{JPN}うっひゃー でかい！\nこの タワー なんど みても ドキドキする！$"

BattleFrontier_OutsideEast_Text_ConquerLeagueAndFrontier:
	.string "{JPN}わたしの やぼうは\nフロンティアの ちょうてんに たつことだ！\pそれから ミミちゃんと ともだちに……\n……そうだ もう ともだちだったな！$"

BattleFrontier_OutsideEast_Text_PyramidTooHarsh:
	.string "{JPN}ピラミッドで まいごに なったんだ！\nでも もどってくると また のぼりたくなる。\pまわりから のぼるのは やめとくか。\nちゃんと なかを たんけんしたいもんな！$"

BattleFrontier_OutsideEast_Text_ThriveInDarkness:
	.string "{JPN}くらいところにも\nひかる どうぐや でぐちが あるの。\pだれかの みつけた ひかりを\nわたしも おぼえているのよ。$"

BattleFrontier_OutsideEast_Text_PutTogetherUltimateTeam:
	.string "{JPN}さいきょうの チームを かんがえたぜ！\nあれ？ となりの やつも おなじ かおだ！\pよーし どっちが つよいか\nアリーナで ためそうぜ！$"

BattleFrontier_OutsideEast_Text_DidScottBringYouHere:
	.string "{JPN}きみ ここは はじめて？\nそれなら まだ しらない はなしを\lたくさん きかせてくれそうだ！$"

BattleFrontier_OutsideEast_Text_BelieveInYouBuddy:
	.string "{JPN}ジグちゃん！ しょうぶの あとは\nいっしょに おやつに しようね！$"

BattleFrontier_OutsideEast_Text_ZigzagoonLooksVacant:
	.string "{JPN}ジグザグマは のんびりしている。\nだれかの おやつの じかんを\lまっているようだ。$"

BattleFrontier_OutsideEast_Text_PeopleCallMeBusybody:
	.string "{JPN}ぼうし よし！ もちもの よし！\n……あっ それ きみの じゅんびだった！\pパレスでは ポケモンに まかせるんだよな。\nおれも おちついて みまもるぞ！$"

BattleFrontier_OutsideEast_Text_OnceBeatGymLeader:
	.string "{JPN}むかしの しょうぶの きもちが\nいまでも はっきり うかぶのよ。\pまた ひとはな さかせてみようかしら。\nあなたも がんばってね！$"

BattleFrontier_OutsideEast_Text_FastOnBikeAndBattles:
	.string "{JPN}はやさには じしんが あるわ！\nじてんしゃも ポケモンもね。\pでも ときどき はなの まえで\nとまってみるのも いいものよ。$"

BattleFrontier_OutsideEast_Text_BetterThanDaddyAtPokemon:
	.string "{JPN}パパより わたしのほうが つよいよ！\nパパの さくせんも わかっちゃうもん！$"

BattleFrontier_OutsideEast_Text_GoRackUpSomeWinsForDaddy:
	.string "{JPN}ははは！ パパの さくせんが\nつたわってたら そりゃ てごわいな！\pいっしょに たのしんで おいで！$"

BattleFrontier_OutsideEast_Text_HeardPrettyGirlAtBattleArena:
	.string "{JPN}アリーナは おうえんまで あついんだ！\nつい こっちも こえが でちゃう！\pきみは どんな チームで いくの？$"

BattleFrontier_OutsideEast_Text_SometimesImportantOldManInThere:
	.string "{JPN}パレスの おじいさんは\nポケモンと ゆっくり はなしてるの。\pなにを いってるか わからなくても\nたのしいって きもちは わかるわ！$"

BattleFrontier_OutsideEast_Text_LegendOfBattlePyramid:
	.string "{JPN}きいたか！ ピラミッドの でんせつを！\nゆうきある ものが いただきへ……\p……その さきは まだ かんがえてない！\nきみが のぼったら きかせてくれよ！$"

BattleFrontier_OutsideEast_Text_GotWipedOut:
	.string "{JPN}あんなに つよい あいてが いたなんて！\nまだ あたまの なかが いっぱいだ！\pどう たたかったのか みんなで\nなんども おもいかえしてるんだよ。$"

BattleFrontier_OutsideEast_Text_ToughTrainerInBattleTower:
	.string "{JPN}このひと つよい トレーナーに\nあってから ずっと この ちょうしなの。\pとなりに いると わたしまで\nその しあいを みたような きぶんよ。$"

BattleFrontier_OutsideEast_Text_EnoughBattlePointsForDoll:
	.string "{JPN}BPが たまったわ！\nこんどは べっそうの みせで\lドールを えらんでみようかしら！$"

BattleFrontier_OutsideEast_Text_LikeToHaveNameRecordedHere:
	.string "{JPN}ここが ランキングホールっスね！\nじぶんの なまえが のると いいなあ！$"

BattleFrontier_OutsideEast_Text_CanDoAnythingWithYou:
	.string "{JPN}きみの たのしい きもちが\nそのまま ぼくの たのしさに なるんだ。\pいっしょに マルチへ いこうよ。$"

BattleFrontier_OutsideEast_Text_PowerOfOurLoveWillOvercome:
	.string "{JPN}そうね！ おなじ きもちで\nいっしょに たたかいましょ！\pきょうも あしたも\nふたりで ここに いたいわね！$"

BattleFrontier_OutsideEast_Text_FrontierNotExclusivelyForToughTrainers:
	.string "{JPN}つよさを ためすのも\nただ はなしに くるのも よいものだ。\pみなみには えや コンテストも ある。\nすきな ばしょを さがして ごらん。$"

BattleFrontier_OutsideEast_Text_OddTreeHereSeemsToWiggle:
	.string "{JPN}あの きが ゆれるのを みると\nなんだか わしまで むずむずするんじゃ。\pおまえさんも ちょいと\nみていって おくれ。$"

BattleFrontier_OutsideEast_Text_OnceAnOddTreePastHere:
	.string "{JPN}あそこに へんな きが あったじゃろ。\nいまも そこに いる きぶんになるのう。\pおや きみも おぼえているかい？$"

BattleFrontier_OutsideEast_Text_StickyMonWithLongTail:
	.string "{JPN}いわの ほうから しっぽの ながい\nポケモンが のぞいていたんだ！\pぼくを みてたのかな？\nあいさつして おけば よかった！$"
