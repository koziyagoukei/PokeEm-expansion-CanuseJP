RustboroCity_PokemonSchool_MapScripts::
	.byte 0

RustboroCity_PokemonSchool_EventScript_Blackboard::
	lockall
	msgbox RustboroCity_PokemonSchool_Text_BlackboardListsStatusChanges, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic::
	message RustboroCity_PokemonSchool_Text_ReadWhichTopic
	waitmessage
	multichoicegrid 8, 1, MULTI_STATUS_INFO, 3, FALSE
	switch VAR_RESULT
	case 0, RustboroCity_PokemonSchool_EventScript_Poison
	case 1, RustboroCity_PokemonSchool_EventScript_Paralysis
	case 2, RustboroCity_PokemonSchool_EventScript_Sleep
	case 3, RustboroCity_PokemonSchool_EventScript_Burn
	case 4, RustboroCity_PokemonSchool_EventScript_Freeze
	case 5, RustboroCity_PokemonSchool_EventScript_ExitTopicSelect
	case MULTI_B_PRESSED, RustboroCity_PokemonSchool_EventScript_ExitTopicSelect
	end

RustboroCity_PokemonSchool_EventScript_Poison::
	msgbox RustboroCity_PokemonSchool_Text_ExplainPoison, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_Paralysis::
	msgbox RustboroCity_PokemonSchool_Text_ExplainParalysis, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_Sleep::
	msgbox RustboroCity_PokemonSchool_Text_ExplainSleep, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_Burn::
	msgbox RustboroCity_PokemonSchool_Text_ExplainBurn, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_Freeze::
	msgbox RustboroCity_PokemonSchool_Text_ExplainFreeze, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_ExitTopicSelect::
	releaseall
	end

RustboroCity_PokemonSchool_EventScript_GameboyKid1::
	msgbox RustboroCity_PokemonSchool_Text_TradingRightNow, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_GameboyKid2::
	msgbox RustboroCity_PokemonSchool_Text_AlwaysWantedSeedot, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_RichBoy::
	msgbox RustboroCity_PokemonSchool_Text_PokemontCantUseManMadeItems, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_Lass::
	msgbox RustboroCity_PokemonSchool_Text_ConfusedPokemonAttacksItself, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_SchoolKidM::
	msgbox RustboroCity_PokemonSchool_Text_PokemonHealItselfWithBerry, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_StudentNotebook::
	msgbox RustboroCity_PokemonSchool_Text_StudentsNotes, MSGBOX_SIGN
	end

RustboroCity_PokemonSchool_EventScript_Teacher::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_QUICK_CLAW, RustboroCity_PokemonSchool_EventScript_GaveQuickClaw
	call_if_eq VAR_FACING, DIR_EAST, RustboroCity_PokemonSchool_EventScript_TeacherCheckOnStudentsEast
	call_if_eq VAR_FACING, DIR_WEST, RustboroCity_PokemonSchool_EventScript_TeacherCheckOnStudentsWest
	msgbox RustboroCity_PokemonSchool_Text_StudentsWhoDontStudyGetQuickClaw, MSGBOX_DEFAULT
	giveitem ITEM_QUICK_CLAW
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	setflag FLAG_RECEIVED_QUICK_CLAW
	release
	end

RustboroCity_PokemonSchool_EventScript_TeacherCheckOnStudentsEast::
	applymovement VAR_LAST_TALKED, RustboroCity_PokemonSchool_Movement_TeacherCheckOnStudentsEast
	waitmovement 0
	return

RustboroCity_PokemonSchool_EventScript_TeacherCheckOnStudentsWest::
	applymovement VAR_LAST_TALKED, RustboroCity_PokemonSchool_Movement_TeacherCheckOnStudentsWest
	waitmovement 0
	return

RustboroCity_PokemonSchool_EventScript_GaveQuickClaw::
	msgbox RustboroCity_PokemonSchool_Text_ExplainQuickClaw, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	release
	end

RustboroCity_PokemonSchool_Movement_TeacherCheckOnStudentsWest:
	walk_left
	walk_down
	walk_down
	walk_right
	walk_in_place_faster_down
	delay_16
	delay_16
	delay_16
	walk_down
	walk_in_place_faster_left
	delay_16
	delay_16
	walk_right
	delay_16
	delay_16
	delay_8
	walk_up
	walk_left
	walk_left
	walk_up
	walk_up
	walk_right
	step_end

RustboroCity_PokemonSchool_Movement_TeacherCheckOnStudentsEast:
	walk_right
	walk_right
	walk_down
	walk_down
	walk_left
	walk_left
	walk_in_place_faster_down
	delay_16
	delay_16
	delay_16
	walk_down
	walk_in_place_faster_left
	delay_16
	delay_16
	walk_right
	delay_16
	delay_16
	delay_8
	walk_up
	walk_right
	walk_up
	walk_up
	walk_left
	walk_left
	step_end

RustboroCity_PokemonSchool_EventScript_Scott::
	lock
	faceplayer
	goto_if_set FLAG_MET_SCOTT_AFTER_OBTAINING_STONE_BADGE, RustboroCity_PokemonSchool_EventScript_ScottWatchStudents
	goto_if_set FLAG_MET_SCOTT_RUSTBORO, RustboroCity_PokemonSchool_EventScript_ScottSpokeAlready
	goto_if_set FLAG_BADGE01_GET, RustboroCity_PokemonSchool_EventScript_ScottGreetHasBadge
	msgbox RustboroCity_PokemonSchool_Text_ScottMetAlreadyCut, MSGBOX_DEFAULT
	addvar VAR_SCOTT_STATE, 1
	setflag FLAG_MET_SCOTT_RUSTBORO
	release
	end

RustboroCity_PokemonSchool_EventScript_ScottSpokeAlready::
	goto_if_set FLAG_BADGE01_GET, RustboroCity_PokemonSchool_EventScript_ScottNoticeBadge
	msgbox RustboroCity_PokemonSchool_Text_StudentTalentLevelUnknown, MSGBOX_DEFAULT
	release
	end

RustboroCity_PokemonSchool_EventScript_ScottGreetHasBadge::
	msgbox RustboroCity_PokemonSchool_Text_ScottMetAlreadyStoneBadge, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_MetScottAfterBadge
	end

RustboroCity_PokemonSchool_EventScript_ScottNoticeBadge::
	msgbox RustboroCity_PokemonSchool_Text_ScottStoneBadge, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_MetScottAfterBadge
	end

RustboroCity_PokemonSchool_EventScript_MetScottAfterBadge::
	addvar VAR_SCOTT_STATE, 1
	setflag FLAG_MET_SCOTT_AFTER_OBTAINING_STONE_BADGE
	release
	end

RustboroCity_PokemonSchool_EventScript_ScottWatchStudents::
	msgbox RustboroCity_PokemonSchool_Text_StudentTalentLevelUnknown, MSGBOX_DEFAULT
	release
	end

RustboroCity_PokemonSchool_Text_BlackboardListsStatusChanges:
	.string "{JPN}こくばんに\nポケモンが たたかってるときに おこる\lたいちょうの へんかに ついて かかれてる$"

RustboroCity_PokemonSchool_Text_ReadWhichTopic:
	.string "{JPN}どの こうもくを みますか?$"

RustboroCity_PokemonSchool_Text_ExplainPoison:
	.string "{JPN}どくを うけると\nたいりょくが へっていきます\pせんとうのあとも どくは のこるので\nあるくたびに たいりょくが へります\lどくけしで なおしましょう$"

RustboroCity_PokemonSchool_Text_ExplainParalysis:
	.string "{JPN}まひすると すばやさが さがり\nときどき わざが だせなくなります\pたたかい おわっても まひした ままです\nまひなおしで なおしましょう$"

RustboroCity_PokemonSchool_Text_ExplainSleep:
	.string "{JPN}ねむって しまうと\nその あいだ こうげき できません\pじぶんで めざめることも ありますが\nねむったまま せんとうが おわると\lずっと ねむっています\pねむけざましで おこして あげましょう$"

RustboroCity_PokemonSchool_Text_ExplainBurn:
	.string "{JPN}やけどは こうげきりょくが さがるだけでなく\nたいりょくも どんどん へっていきます\pせんとうが おわっても やけどの ままです\nやけどなおしで なおしましょう$"

RustboroCity_PokemonSchool_Text_ExplainFreeze:
	.string "{JPN}こおって しまうと なにも できなくなります\pたたかい おわっても こおりついた ままです\nこおりなおしで とかしましょう$"

RustboroCity_PokemonSchool_Text_StudentsWhoDontStudyGetQuickClaw:
	.string "{JPN}べんきょうを しない せいとは\nわたくしの ツメの あかを\lせんじて のませるのです!\pきみが べんきょう する しないは\nこの どうぐを つかいこなせるか どうかで\lわかること でしょう!$"

RustboroCity_PokemonSchool_Text_ExplainQuickClaw:
	.string "{JPN}ポケモンに せんせいのツメを もたせると\nときどき ですが すばやくなって\lせんせい こうげきを するのです!\pこのように ポケモンに もたせる\nどうぐの こと だけでも\lべんきょう することは いっぱい なのです!$"

RustboroCity_PokemonSchool_Text_TradingRightNow:
	.string "{JPN}いま ともだちと\nポケモンを こうかん してるんだ!$"

RustboroCity_PokemonSchool_Text_AlwaysWantedSeedot:
	.string "{JPN}ほしかった タネボーちゃん が\nやっと てに はいるよ!$"

RustboroCity_PokemonSchool_Text_PokemontCantUseManMadeItems:
	.string "{JPN}ポケモンも どうぐ もてるけど\nキズぐすりとか どくけしとか\lひとが つくった どうぐは\lつかいかたが わからない みたい$"

RustboroCity_PokemonSchool_Text_ConfusedPokemonAttacksItself:
	.string "{JPN}ポケモンの わざで\nこんらん することが あるでしょ?\pこんらん すると たまに まちがえて\nじぶんを こうげき しちゃうの……\pでも いちどでも せんとうから はなれると\nもとに もどるわよ!$"

RustboroCity_PokemonSchool_Text_PokemonHealItselfWithBerry:
	.string "{JPN}……ポケモンに きのみを もたせると\nじぶんで たいりょくを かいふくする\pポケモンが もてる どうぐは\nほかにも たくさん……\pノートを ちゃんと とるのは\nたいへんだなあ……$"

RustboroCity_PokemonSchool_Text_StudentsNotes:
	.string "{JPN}このこの ノートだ……\pポケモンは モンスターボールで つかまえて\nどうじに 6ぴき もちあるける\pそうして つかまえた ポケモンを\nそだてて たたかわせる ひとを\lポケモントレーナーと いう\pトレーナーの もくひょうは\nかくちの ポケモンジムに いる\lつよい トレーナーに かつこと!$"

RustboroCity_PokemonSchool_Text_ScottMetAlreadyCut:
	.string "{JPN}おや? きみとは たしか\nトウカシティで あった きがするね\pわたしの なまえは エニシダ\pポケモンしょうぶの さいのうを もった\nゆうしゅうな じんざいを もとめて\lかくちを たびして いるのさ\pスクールを のぞきにきた ということは\nきみも トレーナー?\pならば まずは いあいぎりを\nポケモンに おぼえさせると いいね!\pいあいぎりは たしか このまちの\nだれかが もっていた はずだよ$"

RustboroCity_PokemonSchool_Text_StudentTalentLevelUnknown:
	.string "{JPN}エニシダ『うーん ここに いる\nせいとさん たちの さいのうは\lまだまだ みちすう! これから だな!$"

RustboroCity_PokemonSchool_Text_ScottStoneBadge:
	.string "{JPN}エニシダ『ん それは……\nストーンバッジ だね\lなかなか やるじゃ ないか!\pだけど できれば\nたたかっている ところを みたかったな\lリーグバッジ だけでは きみが\lどんな トレーナー なのかは\lよく わからない からね$"

RustboroCity_PokemonSchool_Text_ScottMetAlreadyStoneBadge:
	.string "{JPN}おや? きみとは たしか\nトウカシティで あった きがするね\pわたしの なまえは エニシダ\pポケモンしょうぶの さいのうを もった\nゆうしゅうな じんざいを もとめて\lかくちを たびして いるのさ\pきみの それは……\nストーンバッジ だね\lトレーナーに なりたてに しては やるね\pだけど できれば\nたたかっている ところを みたかったな\lリーグバッジ だけでは きみが\lどんな トレーナー なのかは\lよく わからない からね$"

