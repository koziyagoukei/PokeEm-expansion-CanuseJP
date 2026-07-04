SafariZone_EventScript_OutOfBallsMidBattle::
#if IS_FRLG
	setvar VAR_MAP_SCENE_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE, 3
#else
	setvar VAR_SAFARI_ZONE_STATE, 1
#endif
	special ExitSafariMode
#if IS_FRLG
	setwarp MAP_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE, 4, 1
#else
	setwarp MAP_ROUTE121_SAFARI_ZONE_ENTRANCE, 2, 5
#endif
	end

SafariZone_EventScript_Exit::
#if IS_FRLG
	setvar VAR_MAP_SCENE_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE, 1
#else
	setvar VAR_SAFARI_ZONE_STATE, 1
#endif
	special ExitSafariMode
#if IS_FRLG
	warp MAP_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE, 4, 1
#else
	warp MAP_ROUTE121_SAFARI_ZONE_ENTRANCE, 2, 5
#endif
	waitstate
	end

SafariZone_EventScript_RetirePrompt::
	lockall
	msgbox SafariZone_Text_WouldYouLikeToExit, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SafariZone_EventScript_Retire
	releaseall
	end

SafariZone_EventScript_Retire::
	goto SafariZone_EventScript_Exit

SafariZone_EventScript_TimesUp::
	lockall
	playse SE_DING_DONG
	message SafariZone_Text_TimesUp
	waitmessage
	waitbuttonpress
	releaseall
	goto SafariZone_EventScript_Exit

SafariZone_EventScript_OutOfBalls::
	lockall
	playse SE_DING_DONG
	message SafariZone_Text_OutOfBalls
	waitmessage
	waitbuttonpress
	releaseall
	goto SafariZone_EventScript_Exit

EventScript_PokeBlockFeeder::
	lockall
	special GetPokeblockFeederInFront
	goto_if_ne VAR_RESULT, 0xFFFF, SafariZone_EventScript_PokeblockPresent
	msgbox SafariZone_Text_PlacePokeblockOnFeeder, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SafariZone_EventScript_ChoosePokeblock
	releaseall
	end

SafariZone_EventScript_ChoosePokeblock::
	fadescreen FADE_TO_BLACK
	special OpenPokeblockCaseOnFeeder
	goto_if_ne VAR_RESULT, 0xFFFF, SafariZone_EventScript_PokeblockPlaced
#ifdef BUGFIX
	releaseall  @ Only gets called from EventScript_PokeBlockFeeder which uses lockall.
#endif
	end

SafariZone_EventScript_PokeblockPlaced::
	message SafariZone_Text_PokeblockWasPlaced
	waitmessage
	waitbuttonpress
	releaseall
	end

SafariZone_EventScript_PokeblockPresent::
	message SafariZone_Text_PokeblockStillHere
	waitmessage
	waitbuttonpress
	releaseall
	end

SafariZone_Text_WouldYouLikeToExit:
	.string "{JPN}サファリゾーンを\nこのまま しゅうりょう しますか?$"

SafariZone_Text_TimesUp:
	.string "{JPN}ピンポーン! じかん でーす!\nサファリゲームを しゅうりょう します$"

SafariZone_Text_OutOfBalls:
	.string "{JPN}サファリボールを つかいきったので\nサファリゲームを しゅうりょう します$"

SafariZone_Text_PlacePokeblockOnFeeder:
	.string "{JPN}ポロックおきばに ポロックを おきますか?$"

SafariZone_Text_PokeblockStillHere:
	.string "{JPN}まだ {STR_VAR_1}が のこっています$"

SafariZone_Text_PokeblockWasPlaced:
	.string "{JPN}{STR_VAR_1}を\nおきました!$"

Route121_SafariZoneEntrance_Text_WelcomeToSafariZone:
	.string "{JPN}サファリゾーンへ ようこそ!\pここでは ホウエンでは なかなか\nみることの できない ポケモンを\pしぜんの すがたの まま\nめに することが できます\pまた ポケモントレーナーの\nようぼうに おこたえ して\pポケモンを\nつかまえる ことも できます!\pサファリゾーンを たのしんで いってね!$"

Route121_SafariZoneEntrance_Text_WelcomeFirstTime:
	.string "{JPN}サファリゾーンへ ようこそ!\nサファリゾーンは はじめて ですか?$"

Route121_SafariZoneEntrance_Text_ComeInAndEnjoy:
	.string "{JPN}サファリゾーンを たのしんで いってね!$"

Route121_SafariZoneEntrance_Text_FirstTimeInfo:
	.string "{JPN}サファリゾーンでは\n30この サファリボールを つかって\lポケモンを つかまえる ことが できます\pサファリボールを つかいきるか\n500ぽ あるくと\pゲームは しゅうりょう です\pサファリゾーンを たのしんで いってね!$"

Route121_SafariZoneEntrance_Text_WouldYouLikeToPlay:
	.string "{JPN}サファリゾーンへ ようこそ!\p500えんで ポケモン つかまえほうだい!\nサファリゲームに さんか しますか?$"

Route121_SafariZoneEntrance_Text_PlayAnotherTime:
	.string "{JPN}そうですか\nこんど ぜひ さんか してください!$"

Route121_SafariZoneEntrance_Text_NotEnoughMoney:
	.string "{JPN}おかねが たりませんよ!$"

Route121_SafariZoneEntrance_Text_ThatWillBe500Please:
	.string "{JPN}それでは 500えん いただきます$"

Route121_SafariZoneEntrance_Text_HereAreYourSafariBalls:
	.string "{JPN}では サファリボールを どうぞ!$"

Route121_SafariZoneEntrance_Text_Received30SafariBalls:
	.string "{JPN}{PLAYER}は サファリボールを\n30こ うけとった!$"

Route121_SafariZoneEntrance_Text_PleaseEnjoyYourself:
	.string "{JPN}じかんに なりましたら\nこちらから おしらせ しますので\pぞんぶんに おたのしみ ください!\nでは いってらっしゃい!$"

Route121_SafariZoneEntrance_Text_PCIsFull:
	.string "{JPN}おきゃくさん!\nボックスが いっぱい ですよ!$"

Route121_SafariZoneEntrance_Text_YouNeedPokeblockCase:
	.string "{JPN}あ! おきゃくさん\nポロックケースを おもちで ないですね?\pサファリゾーンは ポロックを つかうと\nより おたのしみ いただけますので\pぜひ ポロックケースを\nおもちに なって おこし ください\pポロックケースは\nミナモシティの コンテストかいじょうで\lうけとることが できますよ$"

SafariZone_South_Text_StillHaveTimeExit:
	.string "{JPN}まだ じかんが のこっていますが\nサファリゾーンを やめますか?$"

SafariZone_South_Text_EnjoyTheRestOfYourAdventure:
	.string "{JPN}では ひきつづき おたのしみ ください!$"

SafariZone_South_Text_ExitEarlyThankYouForPlaying:
	.string "{JPN}わかりました\pそれでは のこりの サファリボールを\nおあずかり します\pおつかれさまでした\nまた ごりよう ください$"

SafariZone_South_Text_GoodLuck:
	.string "{JPN}それでは いってらっしゃい!\lなにか ありましたら\nわたしに こえを かけて くださいね$"

SafariZone_South_Text_Boy:
	.string "{JPN}しってた?\pあの しかくい はこに ポロックを おくと\nポケモンが あつまって くるんだよ!$"

SafariZone_South_Text_Man:
	.string "{JPN}この さきに いきたい けど\nじてんしゃ もって くるの わすれた……\pおくに めずらしい ポケモンが\nいそうな きが するのに!$"

SafariZone_Southwest_Text_Woman:
	.string "{JPN}ポケモンに ポロックを なげても\nしらんぷり される ときが あるのよ\pポケモンにも すききらいが あるのかしら?$"

SafariZone_Northwest_Text_Man:
	.string "{JPN}ぜぇ…… ぜぇ……\nやっと ここまで きたけど……\pつかれちゃって ポケモン つかまえる\nげんきも のこって ないよ……$"

SafariZone_North_Text_Fisherman:
	.string "{JPN}ホウエンに いない みずポケモンを\nさがしに きたんだけど……\pみずうみは どこにあるのか\nきみ しってるかい?$"

SafariZone_North_Text_Man:
	.string "{JPN}ここで めずらしい ポケモンを\nたくさん つかまえて\lともだちと こうかん するんだ!$"

SafariZone_South_Text_Youngster:
	.string "{JPN}ポロックおきばに おいた ポロックが\nいつのまにか なくなっていた……\pしらない うちに\nポケモンが たべちゃったのかな?$"

Route121_SafariZoneEntrance_Text_TrainerTip:
	.string "{JPN}サファリゾーンの おとくな じょうほう!\pであった ポケモンに\nポロックを なげると にげにくく なります!$"

SafariZone_Southwest_Text_RestHouseSign:
	.string "{JPN}あるき つかれたら ちょっと ひといき!\n「みんなの きゅうけいじょ」$"

SafariZone_RestHouse_Text_Youngster:
	.string "{JPN}おれ ポロック もってないけど\nポケモン たくさん つかまえたよ\pちかづいて ボールを なげる だけで\nけっこう つかまえられる もんだよ!$"

SafariZone_RestHouse_Text_PsychicM:
	.string "{JPN}ポロックを つかうと いつもより\nちょっぴり にげにくく なるみたい\pだから にげにくい ポケモンに つかっても\nあんまり いみが ないかもね$"

SafariZone_RestHouse_Text_FatMan:
	.string "{JPN}ポロックおきばに ポロック おいて\nポケモンを つかまえると\pおなじ ような せいかくの ポケモンが\nあつまる きが するなあ$"

SafariZone_South_Text_AreaOffLimits1:
	.string "{JPN}このさきは ただいま こうじちゅう のため\nたちいり きんしに なっています$"

SafariZone_Southeast_Text_ExpansionIsFinished:
	.string "{JPN}サファリゾーンの かくちょう こうじが\nしゅうりょう しました\pあたらしい エリアも\nたのしんで くださいね!$"

SafariZone_South_Text_AreaOffLimits2:
	.string "{JPN}このさきは ただいま こうじちゅう のため\nたちいり きんしに なっています$"

SafariZone_Southeast_Text_LittleGirl:
	.string "{JPN}わーい わーい!\nはじめて みる ポケモンが いっぱい!$"

SafariZone_Southeast_Text_FatMan:
	.string "{JPN}このへんに いる ポケモンは\nはじめて みるやつ ばっかりだ!\pそんな めずらしい ポケモンが\nつかまえ ほうだい だなんて すげー!$"

SafariZone_Southeast_Text_RichBoy:
	.string "{JPN}このへんに いる ポケモンは\nホウエンちほう いがいの とちに\lすんでる ポケモン らしいぜ$"

SafariZone_Northeast_Text_Boy:
	.string "{JPN}サファリボールが あと すこししか\nのこって ないんだよ\lなにを つかまえるか まよっちゃうなあ$"

SafariZone_Northeast_Text_Woman:
	.string "{JPN}ピカチュウが ここで みられるって\nきいたんだけど どこに いるのかしら$"

SafariZone_Northeast_Text_Girl:
	.string "{JPN}あーん ポケモンが\nなかなか つかまらないわっ!\pこのままじゃ にゅうじょうりょうの\nモトが とれないわっ!$"
