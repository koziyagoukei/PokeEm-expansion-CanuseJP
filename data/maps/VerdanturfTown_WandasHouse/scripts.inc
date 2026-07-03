VerdanturfTown_WandasHouse_MapScripts::
	.byte 0

VerdanturfTown_WandasHouse_EventScript_Wally::
	lock
	faceplayer
	goto_if_set FLAG_WALLY_SPEECH, VerdanturfTown_WandasHouse_EventScript_WallyShortSpeech
	msgbox VerdanturfTown_WandasHouse_Text_StrongerSpeech, MSGBOX_DEFAULT
	setflag FLAG_WALLY_SPEECH
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallyShortSpeech::
	msgbox VerdanturfTown_WandasHouse_Text_StrongerSpeechShort, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysUncle::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_WALLY_VICTORY_ROAD, VerdanturfTown_WandasHouse_EventScript_WallysUncleEverGrande
	goto_if_set FLAG_DEFEATED_LAVARIDGE_GYM, VerdanturfTown_WandasHouse_EventScript_WallysUncleSlippedOff
	msgbox VerdanturfTown_WandasHouse_Text_WallysNextDoor, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysUncleSlippedOff::
	msgbox VerdanturfTown_WandasHouse_Text_WallySlippedOff, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysUncleEverGrande::
	msgbox VerdanturfTown_WandasHouse_Text_WallyGoneThatFar, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WandasBoyfriend::
	msgbox VerdanturfTown_WandasHouse_Text_CanSeeGirlfriendEveryDay, MSGBOX_NPC
	end

VerdanturfTown_WandasHouse_EventScript_Wanda::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_LAVARIDGE_GYM, VerdanturfTown_WandasHouse_EventScript_WandaDontWorry
	goto_if_set FLAG_DEFEATED_WALLY_MAUVILLE, VerdanturfTown_WandasHouse_EventScript_MeetWanda
	msgbox VerdanturfTown_WandasHouse_Text_DontWorryAboutWally, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_MeetWanda::
	msgbox VerdanturfTown_WandasHouse_Text_MeetWanda, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WandaDontWorry::
	msgbox VerdanturfTown_WandasHouse_Text_DontWorryAboutWally, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysAunt::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_WALLY_VICTORY_ROAD, VerdanturfTown_WandasHouse_EventScript_WallysAuntEverGrande
	goto_if_set FLAG_DEFEATED_LAVARIDGE_GYM, VerdanturfTown_WandasHouse_EventScript_WallysAuntAnythingHappened
	goto_if_set FLAG_RUSTURF_TUNNEL_OPENED, VerdanturfTown_WandasHouse_EventScript_WallysAuntTunnelOpen
	msgbox VerdanturfTown_WandasHouse_Text_DaughtersBoyfriendDriven, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysAuntTunnelOpen::
	msgbox VerdanturfTown_WandasHouse_Text_DaughtersBoyfriendWasDigging, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysAuntAnythingHappened::
	msgbox VerdanturfTown_WandasHouse_Text_IfAnythingHappenedToWally, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysAuntEverGrande::
	msgbox VerdanturfTown_WandasHouse_Text_WallyWasInEverGrande, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_Text_StrongerSpeech:
	.string "{JPN}ミツル『{PLAYER}さんに まけたけど\nもう おちこんでないよ\pだって ラルトスと いっしょに\nあちこちの ジムで うでだめし して\lすごい トレーナーに なるって\lもくひょうが できたから!\pいつか {PLAYER}さん よりも\nつよくなって もういちど\lポケモンしょうぶを おねがい するからね!$"

VerdanturfTown_WandasHouse_Text_StrongerSpeechShort:
	.string "{JPN}ミツル『いつか {PLAYER}さん よりも\nつよくなって もういちど\lポケモンしょうぶを おねがい するからね!$"

VerdanturfTown_WandasHouse_Text_WallysNextDoor:
	.string "{JPN}おじさん『おお! {PLAYER}{KUN}\nミツルくんは となりに いるよ!\pしかし たいした もんだね!\pしぜんに かこまれた かんきょうが\nミツルくんを げんきに させている!\p……いや もしかすると\nあのこに きぼうを あたえて いるのは\pポケモンなのかも しれないな……$"

VerdanturfTown_WandasHouse_Text_WallySlippedOff:
	.string "{JPN}ミツルくん……\nいつのまにか いなくなっちゃった……$"

VerdanturfTown_WandasHouse_Text_WallyGoneThatFar:
	.string "{JPN}おじさん『そうですか……\nあの ミツルくんが\lひとりで そんな とおくまで……\pさすがは わたしの おとうとの むすこだ!$"

VerdanturfTown_WandasHouse_Text_MeetWanda:
	.string "{JPN}ミチル『あなたは……?\n……あ そうか!\pミツルくんが いってた\n{PLAYER}さんって あなたの ことね!\pわたしは ミツルくんの いとこ なの!\pどう? あのこ こっちに きてから\nずいぶん げんきに なったでしょう?$"

VerdanturfTown_WandasHouse_Text_DontWorryAboutWally:
	.string "{JPN}ミチル『ミツルくんの ことなら\nだいじょうぶ! しんぱい ないわよ!\pだって わたしの いとこ だし\nなにより ポケモンが いっしょ ですもの!$"

VerdanturfTown_WandasHouse_Text_CanSeeGirlfriendEveryDay:
	.string "{JPN}きみの おかげで\nまいにち かのじょに あえて\lおれは ハッピーだよ!$"

VerdanturfTown_WandasHouse_Text_DaughtersBoyfriendDriven:
	.string "{JPN}うちの むすめの こいびとは\nずいぶん じょうねつてきな ひとでね\pむすめに あうため だけに\nひたすら トンネルを ほってるってよ!\pむすめも しんぱい なのか\nいつも トンネルに あしを はこんでるよ$"

VerdanturfTown_WandasHouse_Text_DaughtersBoyfriendWasDigging:
	.string "{JPN}なんと! むすめの こいびと ときたら\nすでで トンネルを ほっていたって!\pそりゃあ たまげた もんだ!$"

VerdanturfTown_WandasHouse_Text_IfAnythingHappenedToWally:
	.string "{JPN}ミツルくんに もしものことが あったら\nあたしゃ トウカにいる ごりょうしんに\lあわせる かおが ないよ……$"

VerdanturfTown_WandasHouse_Text_WallyWasInEverGrande:
	.string "{JPN}ミツルくんが サイユウに いたって!?\pそりゃ トウカの ごりょうしんも\nびっくり するだろうねぇ$"

