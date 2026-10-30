@ All the LOCALID constants used in this file are essentially redundant.
@ They're all used to have the NPC the player talked to turn back so they're facing
@ their original direction (because they're watching/participating in the contests).
@ They could all be substituted for VAR_LAST_TALKED.
LilycoveCity_ContestHall_MapScripts::
	.byte 0

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

LilycoveCity_ContestHall_Text_GiveItBestSmartAppeal:
	.string "{JPN}司会『はあい かしこい ポケモンと\nそして トレーナーの みなさーん！\l頑張ってくださーーい！\lレッツ！ アピール！！$"

LilycoveCity_ContestHall_Text_AreYouEnjoyingThisContest:
	.string "{JPN}審査員『コンテスト たのしんでますか?\pおともだちを 3にん つれてくれば\n4にん いっしょに 参加 できますよ！$"

LilycoveCity_ContestHall_Text_EnteredWrongContest:
	.string "{JPN}あわわわ……\n出る コンテスト 間違えた……\pたくましい ポケモンを\n賢さ コンテストに……\pよ よしっ\nかしこそうな いわくだき 頼むぞ！$"

LilycoveCity_ContestHall_Text_RaisedMonToBeSmart:
	.string "{JPN}わしが ながねん\nちしきの すべてを かけて\lかしこく 育てた ポケモン！\pわかい やつらの ポケモン なんぞに\nそう やすやすと 負けたりは せんぞ！\pばあさん みとるかー！?$"

LilycoveCity_ContestHall_Text_IfMonPullsSmartMoveNext:
	.string "{JPN}よし！ おれの ポケモンが\nこの次に かしこい わざをだせば\lいっきに 会場が エキサイトするぞ！$"

LilycoveCity_ContestHall_Text_DontAppreciateCuteLeechLife:
	.string "{JPN}やだあ！\pわたしの ズバットちゃんの\nきゅうけつが あんまり すてきで\l続けて なんども だしてたら\lなんか がっかり されちゃってるー！$"

LilycoveCity_ContestHall_Text_YoureBeautifulGrandpa:
	.string "{JPN}キャー！ おじいさん 素敵ー！$"

LilycoveCity_ContestHall_Text_AllSeemToUseDifferentMoves:
	.string "{JPN}同じ ポケモンを 使っていても\nトレーナーに よって\lぜんぜん 違う わざで\lアピールして くるんだよね\pそれは しょうぶと いっしょだね！$"

LilycoveCity_ContestHall_Text_PokemonSmarterThanTrainers:
	.string "{JPN}トレーナーより ポケモンの ほうが\nあたま よさそうに 見えるなあ$"

LilycoveCity_ContestHall_Text_StillLoveSmartnessContests:
	.string "{JPN}やっぱり 賢さ コンテストよ！\pちせいてきな みどりいろが\nうーん クールだわー！$"

LilycoveCity_ContestHall_Text_AreYouEnteringBeautyContest:
	.string "{JPN}司会『あら? あなたも\nうつくしさ コンテストに でるの?\l頑張ってね！$"

LilycoveCity_ContestHall_Text_EveryPokemonPristineBeauty:
	.string "{JPN}審査員『どの ポケモンも\nじつに うつくしい ですなぁ！\pおもわず さいてんを わすれますよ！$"

LilycoveCity_ContestHall_Text_EyesWillBeGluedToMyBeauty:
	.string "{JPN}こんなに うつくしい ポケモン\nほかに いないわよ ねえ\pみんなの 目を くぎづけに しちゃうわ！$"

LilycoveCity_ContestHall_Text_OverdidGrooming:
	.string "{JPN}うわーん！\pこの コンテストの ために\n一生けんめい けづくろい したら\pやりすぎて あたしの ポケモン\nボサボサに なっちゃった！\pどうしよう……$"

LilycoveCity_ContestHall_Text_JudgeWontSeeAuroraBeam:
	.string "{JPN}この まぶしすぎる オーロラビームに\n審査員も 目を あけられない！\p……あれ?\nそれじゃ いみが ないのか?$"

LilycoveCity_ContestHall_Text_PokemonLooksLikeYoungerMe:
	.string "{JPN}この きれいな ポケモン\n"
	.string "あたしの 若い 頃に そっくり！\p"
	.string "って 言ったら…… 父ちゃんが\n"
	.string "飲んでた お茶を 吹き出したよ\p"
	.string "あたし なんか 変なこと 言ったかね?$"

LilycoveCity_ContestHall_Text_WinBeautyContestMakesMeHappy:
	.string "{JPN}やっぱり じょせいと しては\p美しさコンテストで 勝てると\nいつもより ちょっぴり うれしいのよねー！$"

LilycoveCity_ContestHall_Text_GanderAtAllThosePrettyPokemon:
	.string "{JPN}うわっはっは！\p"
	.string "べっぴんな ポケモンちゃん が\n"
	.string "いっぱいだ……\l"
	.string "こりゃ 華やかで ええのう！$"

LilycoveCity_ContestHall_Text_CantWinOnBeautyAlone:
	.string "{JPN}ただ 美しい だけじゃ\nなかなか 勝てないわよ！\pあたしの ポケモンみたいに\nおていれして ツヤツヤ じゃなきゃ！$"

LilycoveCity_ContestHall_Text_InTheMiddleOfContest:
	.string "{JPN}司会『あら あらー！\nいまは コンテスト ちゅう ですよー！\p受付で エントリー してから\nでて くださーい！ ねっ！$"

LilycoveCity_ContestHall_Text_SuchCharmingCuteAppeals:
	.string "{JPN}審査員『おお\nみんな かわいい アピールだなあ！\pおおっ なんて かわいい\nみずあそびの アピールなんだろう！$"

LilycoveCity_ContestHall_Text_MyAzurillWasDistracted:
	.string "{JPN}ああ ぼくの ルリリちゃんが\nほかの ポケモンに 気を とられちゃった！$"

LilycoveCity_ContestHall_Text_NeverWonBattleButContest:
	.string "{JPN}わたしの ポケモンって 勝負だと\n1ども 勝った こと ないけど\lコンテストは おとくい なのよ！$"

LilycoveCity_ContestHall_Text_PetalDanceIsMarvel:
	.string "{JPN}ぼくの ポケモンの 華麗な\n"
	.string "はなびらのまい……\l"
	.string "誰にも 邪魔は させないぞ！$"

LilycoveCity_ContestHall_Text_MyMonAppealSoMuchCuter:
	.string "{JPN}どなたの ポケモンも\n本当に かわいいわねえ\pでも わたしの ポケモンが\nかわいく アピール すれば……\pきっと もっと かわいい はずよ$"

LilycoveCity_ContestHall_Text_MyChildIsInContest:
	.string "{JPN}うちのこが コンテストに 出てるんですよ$"

LilycoveCity_ContestHall_Text_ComeOnDear:
	.string "{JPN}ほらっ 頑張って！\nあなたの ポケモンが いちばんよっ！$"

LilycoveCity_ContestHall_Text_ThatGirlThereIsCutest:
	.string "{JPN}ワシは あの おんなのこが\nいちばん かわいいと おもうな……\pえっ? ポケモンの みた目を\nくらべてるんか！?$"

LilycoveCity_ContestHall_Text_WantCuteMonOfMyOwn:
	.string "{JPN}ああん 見てたら\nあの ポケモン ほしく なっちゃった\lつかまえに いこっと！$"

LilycoveCity_ContestHall_Text_BeautyContestStage:
	.string "{JPN}美しさコンテスト ステージ\nビューティフル ポケモンに クラクラ！$"

LilycoveCity_ContestHall_Text_CuteContestStage:
	.string "{JPN}かわいさコンテスト ステージ\nキュートな ポケモンに ウキウキ！$"

LilycoveCity_ContestHall_Text_SmartContestStage:
	.string "{JPN}賢さコンテスト ステージ\nインテリ ポケモンに トロトロ！$"

