@ All the LOCALID constants used in this file are essentially redundant.
@ They're all used to have the NPC the player talked to turn back so they're facing
@ their original direction (because they're watching/participating in the contests).
@ They could all be substituted for VAR_LAST_TALKED.
LilycoveCity_ContestHall_MapScripts::
	.byte 0

LilycoveCity_ContestHall_EventScript_Boy1::
	msgbox LilycoveCity_ContestHall_Text_TodayWonSmartnessContest, MSGBOX_NPC
	end

LilycoveCity_ContestHall_EventScript_Boy2::
	msgbox LilycoveCity_ContestHall_Text_EnteredBunchOfContests, MSGBOX_NPC
	end

LilycoveCity_ContestHall_EventScript_Girl::
	msgbox LilycoveCity_ContestHall_Text_ManWhoWonEarlierHadPokeblocks, MSGBOX_NPC
	end

LilycoveCity_ContestHall_EventScript_Sailor::
	msgbox LilycoveCity_ContestHall_Text_IsntThisPlaceHumongous, MSGBOX_NPC
	end

LilycoveCity_ContestHall_EventScript_SmartContestMC::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_GiveItBestSmartAppeal, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_MC, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestJudge::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_AreYouEnjoyingThisContest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_JUDGE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestant1::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_EnteredWrongContest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_CONTESTANT_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestant2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_RaisedMonToBeSmart, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_CONTESTANT_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestant3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_IfMonPullsSmartMoveNext, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_CONTESTANT_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestant4::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_DontAppreciateCuteLeechLife, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_CONTESTANT_4, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestAudience1::
	msgbox LilycoveCity_ContestHall_Text_YoureBeautifulGrandpa, MSGBOX_SIGN
	end

LilycoveCity_ContestHall_EventScript_SmartContestAudience2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_AllSeemToUseDifferentMoves, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_AUDIENCE_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestAudience3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_PokemonSmarterThanTrainers, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_AUDIENCE_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestAudience4::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_StillLoveSmartnessContests, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_AUDIENCE_4, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestMC::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_AreYouEnteringBeautyContest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_MC, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestJudge::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_EveryPokemonPristineBeauty, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_JUDGE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestant1::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_EyesWillBeGluedToMyBeauty, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_CONTESTANT_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestant2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_OverdidGrooming, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_CONTESTANT_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestant3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_JudgeWontSeeAuroraBeam, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_CONTESTANT_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestant4::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_PokemonLooksLikeYoungerMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_CONTESTANT_4, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestAudience1::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_WinBeautyContestMakesMeHappy, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_AUDIENCE_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestAudience2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_GanderAtAllThosePrettyPokemon, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_AUDIENCE_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestAudience3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_CantWinOnBeautyAlone, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_AUDIENCE_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestMC::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_InTheMiddleOfContest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_MC, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestJudge::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_SuchCharmingCuteAppeals, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_JUDGE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestant1::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_MyAzurillWasDistracted, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_CONTESTANT_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestant2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_NeverWonBattleButContest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_CONTESTANT_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestant3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_PetalDanceIsMarvel, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_CONTESTANT_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestant4::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_MyMonAppealSoMuchCuter, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_CONTESTANT_4, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestAudience1::
	lockall
	applymovement LOCALID_CUTE_AUDIENCE_1, Common_Movement_FacePlayer
	waitmovement 0
	msgbox LilycoveCity_ContestHall_Text_MyChildIsInContest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_AUDIENCE_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	delay 25
	msgbox LilycoveCity_ContestHall_Text_ComeOnDear, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_ContestHall_EventScript_CuteContestAudience2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_WantCuteMonOfMyOwn, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_AUDIENCE_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestAudience3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_ThatGirlThereIsCutest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_AUDIENCE_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyStageSign::
	msgbox LilycoveCity_ContestHall_Text_BeautyContestStage, MSGBOX_SIGN
	end

LilycoveCity_ContestHall_EventScript_CuteStageSign::
	msgbox LilycoveCity_ContestHall_Text_CuteContestStage, MSGBOX_SIGN
	end

LilycoveCity_ContestHall_EventScript_SmartStageSign::
	msgbox LilycoveCity_ContestHall_Text_SmartContestStage, MSGBOX_SIGN
	end

LilycoveCity_ContestHall_Text_TodayWonSmartnessContest:
	.string "{JPN}うつくしさ コンテストで\nゆうしょうした この ポケモンを\lきょうは かしこさ コンテストで\lゆうしょう させたんだぜ！\pああ じぶんの さいのうが こわい……$"

LilycoveCity_ContestHall_Text_EnteredBunchOfContests:
	.string "{JPN}なんども コンテストに でていて\nなんとなく わかって きたんだけど\pたとえば かっこいいコンテストで\nかっこいいわざを だすと うけるけど\lかしこいアピールや かわいいアピールは\lなぜか うけが わるいんだよな……$"

LilycoveCity_ContestHall_Text_ManWhoWonEarlierHadPokeblocks:
	.string "{JPN}さっき ゆうしょう した おにいさん\nすっごい たくさんの しゅるいの\lポロックを もってた！\pあれだけ もってれば\nゆうしょう できるのかな?$"

LilycoveCity_ContestHall_Text_IsntThisPlaceHumongous:
	.string "{JPN}かーっ ひろいね ここは！\nいなかの コンテスト かいじょう とは\lきんちょうかんが ちがうね！$"

LilycoveCity_ContestHall_Text_GiveItBestSmartAppeal:
	.string "{JPN}しかい『はあい かしこい ポケモンと\nそして トレーナーの みなさーん！\lがんばってくださーーい！\lレッツ！ アピール！！$"

LilycoveCity_ContestHall_Text_AreYouEnjoyingThisContest:
	.string "{JPN}しんさいん『コンテスト たのしんでますか?\pおともだちを 3にん つれてくれば\n4にん いっしょに さんか できますよ！$"

LilycoveCity_ContestHall_Text_EnteredWrongContest:
	.string "{JPN}あわわわ……\nでる コンテスト まちがえた……\pたくましい ポケモンを\nかしこさ コンテストに……\pよ よしっ\nかしこそうな いわくだき たのむぞ！$"

LilycoveCity_ContestHall_Text_RaisedMonToBeSmart:
	.string "{JPN}わしが ながねん\nちしきの すべてを かけて\lかしこく そだてた ポケモン！\pわかい やつらの ポケモン なんぞに\nそう やすやすと まけたりは せんぞ！\pばあさん みとるかー！?$"

LilycoveCity_ContestHall_Text_IfMonPullsSmartMoveNext:
	.string "{JPN}よし！ おれの ポケモンが\nこのつぎに かしこい わざをだせば\lいっきに かいじょうが エキサイトするぞ！$"

LilycoveCity_ContestHall_Text_DontAppreciateCuteLeechLife:
	.string "{JPN}やだあ！\pわたしの ズバットちゃんの\nきゅうけつが あんまり すてきで\lつづけて なんども だしてたら\lなんか がっかり されちゃってるー！$"

LilycoveCity_ContestHall_Text_YoureBeautifulGrandpa:
	.string "{JPN}キャー！ おじいさん すてきー！$"

LilycoveCity_ContestHall_Text_AllSeemToUseDifferentMoves:
	.string "{JPN}おなじ ポケモンを つかっていても\nトレーナーに よって\lぜんぜん ちがう わざで\lアピールして くるんだよね\pそれは しょうぶと いっしょだね！$"

LilycoveCity_ContestHall_Text_PokemonSmarterThanTrainers:
	.string "{JPN}トレーナーより ポケモンの ほうが\nあたま よさそうに みえるなあ$"

LilycoveCity_ContestHall_Text_StillLoveSmartnessContests:
	.string "{JPN}やっぱり かしこさ コンテストよ！\pちせいてきな みどりいろが\nうーん クールだわー！$"

LilycoveCity_ContestHall_Text_AreYouEnteringBeautyContest:
	.string "{JPN}しかい『あら? あなたも\nうつくしさ コンテストに でるの?\lがんばってね！$"

LilycoveCity_ContestHall_Text_EveryPokemonPristineBeauty:
	.string "{JPN}しんさいん『どの ポケモンも\nじつに うつくしい ですなぁ！\pおもわず さいてんを わすれますよ！$"

LilycoveCity_ContestHall_Text_EyesWillBeGluedToMyBeauty:
	.string "{JPN}こんなに うつくしい ポケモン\nほかに いないわよ ねえ\pみんなの めを くぎづけに しちゃうわ！$"

LilycoveCity_ContestHall_Text_OverdidGrooming:
	.string "{JPN}うわーん！\pこの コンテストの ために\nいっしょけんめい けづくろい したら\pやりすぎて あたしの ポケモン\nボサボサに なっちゃった！\pどうしよう……$"

LilycoveCity_ContestHall_Text_JudgeWontSeeAuroraBeam:
	.string "{JPN}この まぶしすぎる オーロラビームに\nしんさいんも めを あけられない！\p……あれ?\nそれじゃ いみが ないのか?$"

LilycoveCity_ContestHall_Text_PokemonLooksLikeYoungerMe:
	.string "{JPN}この きれいな ポケモン\nあたしの わかい ころに そっくり！\pって いったら…… とうちゃんが\nのんでた おちゃを ふきだしたよ\pあたし なんか へんなこと いったかね?$"

LilycoveCity_ContestHall_Text_WinBeautyContestMakesMeHappy:
	.string "{JPN}やっぱり じょせいと しては\pうつくしさコンテストで かてると\nいつもより ちょっぴり うれしいのよねー！$"

LilycoveCity_ContestHall_Text_GanderAtAllThosePrettyPokemon:
	.string "{JPN}うわっはっは！\pべっぴんな ポケモンちゃん が\nいっぱいだ……\lこりゃ はなやかで ええのう！$"

LilycoveCity_ContestHall_Text_CantWinOnBeautyAlone:
	.string "{JPN}ただ うつくしい だけじゃ\nなかなか かてないわよ！\pあたしの ポケモンみたいに\nおていれして ツヤツヤ じゃなきゃ！$"

LilycoveCity_ContestHall_Text_InTheMiddleOfContest:
	.string "{JPN}しかい『あら あらー！\nいまは コンテスト ちゅう ですよー！\pうけつけで エントリー してから\nでて くださーい！ ねっ！$"

LilycoveCity_ContestHall_Text_SuchCharmingCuteAppeals:
	.string "{JPN}しんさいん『おお\nみんな かわいい アピールだなあ！\pおおっ なんて かわいい\nみずあそびの アピールなんだろう！$"

LilycoveCity_ContestHall_Text_MyAzurillWasDistracted:
	.string "{JPN}ああ ぼくの ルリリちゃんが\nほかの ポケモンに きを とられちゃった！$"

LilycoveCity_ContestHall_Text_NeverWonBattleButContest:
	.string "{JPN}わたしの ポケモンって しょうぶだと\n1ども かった こと ないけど\lコンテストは おとくい なのよ！$"

LilycoveCity_ContestHall_Text_PetalDanceIsMarvel:
	.string "{JPN}ぼくの ポケモンの かれいな\nはなびらのまい……\lだれにも じゃまは させないぞ！$"

LilycoveCity_ContestHall_Text_MyMonAppealSoMuchCuter:
	.string "{JPN}どなたの ポケモンも\nほんとうに かわいいわねえ\pでも わたしの ポケモンが\nかわいく アピール すれば……\pきっと もっと かわいい はずよ$"

LilycoveCity_ContestHall_Text_MyChildIsInContest:
	.string "{JPN}うちのこが コンテストに でてるんですよ$"

LilycoveCity_ContestHall_Text_ComeOnDear:
	.string "{JPN}ほらっ がんばって！\nあなたの ポケモンが いちばんよっ！$"

LilycoveCity_ContestHall_Text_ThatGirlThereIsCutest:
	.string "{JPN}ワシは あの おんなのこが\nいちばん かわいいと おもうな……\pえっ? ポケモンの みためを\nくらべてるんか！?$"

LilycoveCity_ContestHall_Text_WantCuteMonOfMyOwn:
	.string "{JPN}ああん みてたら\nあの ポケモン ほしく なっちゃった\lつかまえに いこっと！$"

LilycoveCity_ContestHall_Text_BeautyContestStage:
	.string "{JPN}うつくしさコンテスト ステージ\nビューティフル ポケモンに クラクラ！$"

LilycoveCity_ContestHall_Text_CuteContestStage:
	.string "{JPN}かわいさコンテスト ステージ\nキュートな ポケモンに ウキウキ！$"

LilycoveCity_ContestHall_Text_SmartContestStage:
	.string "{JPN}かしこさコンテスト ステージ\nインテリ ポケモンに トロトロ！$"

