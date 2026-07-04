LilycoveCity_House3_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LilycoveCity_House3_OnTransition
	.byte 0

LilycoveCity_House3_OnTransition:
	random 4
	copyvar VAR_TEMP_1, VAR_RESULT
	end

LilycoveCity_House3_EventScript_PokefanF::
	lock
	faceplayer
	msgbox LilycoveCity_House3_Text_LearnFromMasterOfPokeblocks, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LilycoveCity_House3_EventScript_DeclinePokeblockLearn
	msgbox LilycoveCity_House3_Text_ExplainPokeblocks, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_House3_EventScript_DeclinePokeblockLearn::
	msgbox LilycoveCity_House3_Text_OhAreYouSure, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_House3_EventScript_Man::
	lock
	faceplayer
	msgbox LilycoveCity_House3_Text_HappyToHaveQuadruplets, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_House3_EventScript_GameBoyKid1::
	lock
	faceplayer
	switch VAR_TEMP_1
	case 0, LilycoveCity_House3_EventScript_WereDoingMultiBattle
	case 1, LilycoveCity_House3_EventScript_WereMixingRecords
	case 2, LilycoveCity_House3_EventScript_WereBlendingBerries
	case 3, LilycoveCity_House3_EventScript_WereDoingContest
	end

LilycoveCity_House3_EventScript_GameBoyKid2::
	lock
	faceplayer
	switch VAR_TEMP_1
	case 0, LilycoveCity_House3_EventScript_WereDoingMultiBattle
	case 1, LilycoveCity_House3_EventScript_WereMixingRecords
	case 2, LilycoveCity_House3_EventScript_WereBlendingBerries
	case 3, LilycoveCity_House3_EventScript_WereDoingContest
	end

LilycoveCity_House3_EventScript_GameBoyKid3::
	lock
	faceplayer
	switch VAR_TEMP_1
	case 0, LilycoveCity_House3_EventScript_WereDoingMultiBattle
	case 1, LilycoveCity_House3_EventScript_WereMixingRecords
	case 2, LilycoveCity_House3_EventScript_WereBlendingBerries
	case 3, LilycoveCity_House3_EventScript_WereDoingContest
	end

LilycoveCity_House3_EventScript_GameBoyKid4::
	lock
	faceplayer
	switch VAR_TEMP_1
	case 0, LilycoveCity_House3_EventScript_WereDoingMultiBattle
	case 1, LilycoveCity_House3_EventScript_WereMixingRecords
	case 2, LilycoveCity_House3_EventScript_WereBlendingBerries
	case 3, LilycoveCity_House3_EventScript_WereDoingContest
	end

LilycoveCity_House3_EventScript_WereDoingMultiBattle::
	msgbox LilycoveCity_House3_Text_GoingToWinMultiBattles, MSGBOX_DEFAULT
	release
	end

LilycoveCity_House3_EventScript_WereMixingRecords::
	msgbox LilycoveCity_House3_Text_LikeMixingAtRecordCorner, MSGBOX_DEFAULT
	release
	end

LilycoveCity_House3_EventScript_WereBlendingBerries::
	msgbox LilycoveCity_House3_Text_MakePokeblocksWithBerryBlender, MSGBOX_DEFAULT
	release
	end

LilycoveCity_House3_EventScript_WereDoingContest::
	msgbox LilycoveCity_House3_Text_GoingToEnterContest, MSGBOX_DEFAULT
	release
	end

LilycoveCity_House3_Text_LearnFromMasterOfPokeblocks:
	.string "{JPN}あら あら あら！ あなた わかいのに\nひとりたび? えらいわねえー！\lうちのこ たちにも みならわせたいわあ\pおばさんは ポロックめいじん！\nちょいと ほんきを だせば\lすごく いい ポロックを つくれるのよー！\pポロックめいじんの おばさんが\nいろいろと おしえてあげようか?$"

LilycoveCity_House3_Text_OhAreYouSure:
	.string "{JPN}あらあ そう?\nなんでも じぶん ひとりだけで\lがんばろうと しちゃ ダメよー！$"

LilycoveCity_House3_Text_ExplainPokeblocks:
	.string "{JPN}あらあ すなおな いいコねえー！\nじゃ ちょっとながいけど ききなさいよ\pいい?\pポロックには なめらかさって もんが あるの\nポロックを みれば わかるでしょ?\lこの すうじが ひくいほど なめらかなの！\lここ まちがっちゃ ダメよ！\pなめらかさの すうじが ひくくて\nレベルが たかいのが いい ポロック！\lいい ポロックは たくさん\lポケモンが たべて くれるんだから！\pそれと いい?\pなめらかな ポロックが つくりたかったら\nけちらないで いろんな しゅるいの\lきのみで つくってみるのよ！\lまぜた きのみによって なめらかさが\lちがって くるんだから！\pそれと いい?\pなるべく おおぜいで つくったほうが\nなめらかな ポロックが できるのよ！\lだれでも いいから おねがいして\lいっしょに つくって もらいなさいよ！\pさてと このくらい おぼえて おけば\nあなたも いちにんまえに なれるわよ！\pだいすきな ポケモンと なめらか ポロック\nそして なかよし かぞくが いれば\lこのよは しあわせ！\lあなたも がんばりなさいな！$"

LilycoveCity_House3_Text_HappyToHaveQuadruplets:
	.string "{JPN}よつごが うまれた ときは\nすごく ビックリ したが\lみんなで なかよく あそんでいる すがたを\lみていると しあわせ きぶんに なるよ！$"

LilycoveCity_House3_Text_GoingToWinMultiBattles:
	.string "{JPN}マルチバトル してるけど\nぼくが かつに きまってるぜ！$"

LilycoveCity_House3_Text_LikeMixingAtRecordCorner:
	.string "{JPN}レコードコーナーで まぜまくる！\nって なにが まざってるんだ?$"

LilycoveCity_House3_Text_MakePokeblocksWithBerryBlender:
	.string "{JPN}きのみブレンダーで\nとびっきりの ポロック つくっちゃうぜ！$"

LilycoveCity_House3_Text_GoingToEnterContest:
	.string "{JPN}ポケモンの たくましさ じまん したいから\nみんなで コンテスト してるんだ！$"

