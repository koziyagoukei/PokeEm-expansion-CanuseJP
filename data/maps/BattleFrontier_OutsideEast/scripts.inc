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
	.string "{JPN}ここは バトルタワー！\n"
	.string "さいきょう トレーナーとして\l"
	.string "かちぬけ！$"

BattleFrontier_OutsideEast_Text_BattlePalaceSign:
	.string "{JPN}ここは バトルパレス！\n"
	.string "ポケモンの たたかいを みまもれ！$"

BattleFrontier_OutsideEast_Text_BattleArenaSign:
	.string "{JPN}ここは バトルアリーナ！\n"
	.string "つよき チームよ あつまれ！$"

BattleFrontier_OutsideEast_Text_BattlePyramidSign:
	.string "{JPN}ここは バトルピラミッド！\n"
	.string "バトルクエストを つきすすめ！$"

BattleFrontier_OutsideEast_Text_RankingHallSign:
	.string "{JPN}バトルフロンティア ランキングホール\n"
	.string "あらたな きろくを めざせ！$"

BattleFrontier_OutsideEast_Text_ExchangeCornerSign:
	.string "{JPN}バトルポイント こうかんサービスコーナー\n"
	.string "バトルポイントを こうかんしよう！$"

BattleFrontier_OutsideEast_Text_BattleTowerFeelsSpecial:
	.string "{JPN}うっひゃー でかい!\pやはり バトルタワーは\nとくべつ って かんじが するな$"

BattleFrontier_OutsideEast_Text_ConquerLeagueAndFrontier:
	.string "{JPN}わたしの やぼうは\nバトルフロンティア そして ポケモンリーグ\lすべての ちょうてんに たつ ことだ!\pそして もし それが かなったら!\pとっ となりの いえの ミミちゃんに\nおともだちに なって くださいって いうぞ!$"

BattleFrontier_OutsideEast_Text_PyramidTooHarsh:
	.string "{JPN}バトルピラミッドは むずかし すぎて\nどうしても ちょうじょうに まで いけん!\pしかたが ないから いっそのこと\nそとがわを のぼって やろうかしら$"

BattleFrontier_OutsideEast_Text_ThriveInDarkness:
	.string "{JPN}くらやみが だいすきな わたし……\nそう…… わたしに ふさわしい のは……\lやはり この バトルピラミッド……\pネ…… あなたも くらやみの なかを\nひっしに さまよって みない……?$"

BattleFrontier_OutsideEast_Text_PutTogetherUltimateTeam:
	.string "{JPN}いっしゅうかん ねないで かんがえた\nオレの さいきょう チーム……\lまけるきが しねえぜ!\pぃよーし アリーナに なぐりこみだー!$"

BattleFrontier_OutsideEast_Text_DidScottBringYouHere:
	.string "{JPN}あんたも エニシダさんに\nつれて こられたの?\lあのひと なんなんだろうね\pつよい トレーナーを みぬく めは\nたしからしい けど……\lほんにんが たたかって いる ところは\lいちども みたことが ないんだよなあ$"

BattleFrontier_OutsideEast_Text_BelieveInYouBuddy:
	.string "{JPN}ジグちゃん! しんじてるからね!\pかったら ジグちゃんの すきな おやつ\nなーんでも つくって あげるからね!$"

BattleFrontier_OutsideEast_Text_ZigzagoonLooksVacant:
	.string "{JPN}ジグザグマは ぼんやり してる……$"

BattleFrontier_OutsideEast_Text_PeopleCallMeBusybody:
	.string "{JPN}お おれは でしゃばり だ!\nオイあんた! ぼうしが ずれてるぜ!\lおっと じめんに ゴミが おちてる!\lああっと! もうすぐ ごはんだぜ!\p…… ……バトルパレスで\nしょうぶを ただ みまもる だけなんて\lおれに できるだろーか……?$"

BattleFrontier_OutsideEast_Text_OnceBeatGymLeader:
	.string "{JPN}こう みえても むかしは\nジムリーダーに かった ことも あるのよ\pもう ひとはな さかせて みようかしらねぇ$"

BattleFrontier_OutsideEast_Text_FastOnBikeAndBattles:
	.string "{JPN}じてんしゃも はやいけど\nポケモン だって じしんあるわ!\lめにも とまらぬ はやさで かっちゃうわよ!$"

BattleFrontier_OutsideEast_Text_BetterThanDaddyAtPokemon:
	.string "{JPN}わたしのほうがねえー パパより\nポケモン つよいんだよ!$"

BattleFrontier_OutsideEast_Text_GoRackUpSomeWinsForDaddy:
	.string "{JPN}むすめよ!\nがんばって かちぬいて こい!\lおまえは ママ そっくり だから つよいぞ!$"

BattleFrontier_OutsideEast_Text_HeardPrettyGirlAtBattleArena:
	.string "{JPN}バトルアリーナに めちゃくちゃ\nぷりてぃーな こが いるって きいたから\lここまで がんばって きたのにーっ!\pあせくさーくて あつくるしーい\nかくとうか ばっかり だったよ!$"

BattleFrontier_OutsideEast_Text_SometimesImportantOldManInThere:
	.string "{JPN}ときどきねー ここの たてものに\nすっごく つよそーな ポケモン つれて\lすっごく えらそー だけど\lけっこー やさしそーな\lじーちゃんが はいってくのよ!$"

BattleFrontier_OutsideEast_Text_LegendOfBattlePyramid:
	.string "{JPN}きみは しって いるか!?\nバトルピラミッドの でんせつを!!\pみなぎる ゆうきを もつ トレーナー\nきんじとうの いただきを めざすとき\lおとこの なかの おとこ あらわれる\p……どうだ? しらないだろー!\nだって これ さっき おれが\lかんがえたんだ もんな!\pなに? どういう いみ かって?\nチッチッ!! それは おしえられないな!$"

BattleFrontier_OutsideEast_Text_GotWipedOut:
	.string "{JPN}くそーっ!\pいままで だれにも いちども\nまけたこと なかった この ぼくが\lても あしも でない うえに\lさいのうが ない だと!?\pくそーっ! くそーっ! くそーっ!$"

BattleFrontier_OutsideEast_Text_ToughTrainerInBattleTower:
	.string "{JPN}このひと バトルタワーに\nちょうせん している ときに\lおそろしく つよい トレーナーと\lたたかった みたい なの……\pあなたも バトルタワーに\nちょうせんする ときは ちゅういして……$"

BattleFrontier_OutsideEast_Text_EnoughBattlePointsForDoll:
	.string "{JPN}さて! バトルポイントも たまったし!\nまた おおきな ぬいぐるみ でも\lもらいに いこーっと!$"

BattleFrontier_OutsideEast_Text_LikeToHaveNameRecordedHere:
	.string "{JPN}おほほほーいっ!\nここが ランキングホール っスねー!\pわしも ここに なを のこす ような\nえいこうの スーパー チャンプに\lはやく なりたい っスよ!$"

BattleFrontier_OutsideEast_Text_CanDoAnythingWithYou:
	.string "{JPN}もし きみが そばに いてくれたら\nぼくは なんだって できるのさ……$"

BattleFrontier_OutsideEast_Text_PowerOfOurLoveWillOvercome:
	.string "{JPN}ぁあん やっぱ ダーリンは ス テ キ!\p…… ……あー!\nもー! わたし たまんないっ!!\pいますぐ ふたりで マルチ バトルルームに\nとつげき しましょうよーっ!!\pバトルサロンで いっしょに なったら\nふたりの ラブパワーで\lどいつも こいつも ギタギタよ!\lねぇー ダーリン!!$"

BattleFrontier_OutsideEast_Text_FrontierNotExclusivelyForToughTrainers:
	.string "{JPN}バトルフロンティアには\nつよい トレーナーだけでは なく\lかわった ちからを もった ひとたちも\lまねかれて いるな……\p……エニシダと いう おとこ……\pじつは なにか おおきな もくてきが\nあって ここを つくったのかも しれないぞ$"

BattleFrontier_OutsideEast_Text_OddTreeHereSeemsToWiggle:
	.string "{JPN}もしもし あんたさんは\nめは いいほう ですかい?\pあたしには このさきの きが ときどき\nぷるぷる してる ように みえるんですわ…$"

BattleFrontier_OutsideEast_Text_OnceAnOddTreePastHere:
	.string "{JPN}もしもし あんたさんは\nものおぼえは いいほう ですかい?\pあたしには このさきに へんな きが\nあった ような きが するんですわ…$"

BattleFrontier_OutsideEast_Text_StickyMonWithLongTail:
	.string "{JPN}ぼぼぼ ぼく みちゃったんだっ!!\pこのさきの いわ から しっぽが ながくって\nなんか ベタベタした ポケモンが\lぼくの こと じーっと のぞいてたんだっ!\pきっと こわい ポケモン だよーっ!!$"
