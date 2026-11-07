.set REWARD_TYPE, VAR_0x8004
.set REWARD_ITEM, VAR_0x8008

CeruleanCity_House4_Frlg_MapScripts::
	.byte 0

CeruleanCity_House4_EventScript_WonderNewsBerryMan::
	lock
	specialvar REWARD_TYPE, WonderNews_GetRewardInfo
	copyvar REWARD_ITEM, VAR_RESULT
	goto_if_eq REWARD_TYPE, NEWS_REWARD_NONE,       CeruleanCity_House4_EventScript_NoNews
	goto_if_eq REWARD_TYPE, NEWS_REWARD_RECV_SMALL, CeruleanCity_House4_EventScript_Reward_RecvSmall
	goto_if_eq REWARD_TYPE, NEWS_REWARD_RECV_BIG,   CeruleanCity_House4_EventScript_Reward_RecvBig
	goto_if_eq REWARD_TYPE, NEWS_REWARD_WAITING,    CeruleanCity_House4_EventScript_Waiting
	goto_if_eq REWARD_TYPE, NEWS_REWARD_SENT_SMALL, CeruleanCity_House4_EventScript_Reward_SentSmall
	goto_if_eq REWARD_TYPE, NEWS_REWARD_SENT_BIG,   CeruleanCity_House4_EventScript_Reward_SentBig
	goto_if_eq REWARD_TYPE, NEWS_REWARD_AT_MAX,     CeruleanCity_House4_EventScript_AtMax
	end

@ Mystery Gift is not enabled, or the player has no saved Wonder News
CeruleanCity_House4_EventScript_NoNews::
	msgbox CeruleanCity_House4_Text_NothingEntertaining
	release
	end

@ Small reward for receiving Wonder News from friend
CeruleanCity_House4_EventScript_Reward_RecvSmall::
	call CeruleanCity_House4_EventScript_MovementReactionToNews
	msgbox CeruleanCity_House4_Text_NewNewsInformativeHaveThis
	giveitem REWARD_ITEM
	goto_if_eq VAR_RESULT, FALSE, CeruleanCity_House4_EventScript_NoRoomForBerries
	release
	end

@ Big reward for receiving Wonder News from non-friend source
CeruleanCity_House4_EventScript_Reward_RecvBig::
	call CeruleanCity_House4_EventScript_MovementReactionToNews
	msgbox CeruleanCity_House4_Text_IncredibleNewsHaveBerries
	giveitem REWARD_ITEM, 4
	goto_if_eq VAR_RESULT, FALSE, CeruleanCity_House4_EventScript_NoRoomForBerries
	release
	end

@ Player has not recently sent Wonder News
CeruleanCity_House4_EventScript_Waiting::
	applymovement LOCALID_WONDER_NEWS_BERRY_MAN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox CeruleanCity_House4_Text_WishCouldShareNewsWithOthers
	release
	end

@ Small reward for sending Wonder News every 1-3 times
CeruleanCity_House4_EventScript_Reward_SentSmall::
	call CeruleanCity_House4_EventScript_MovementReactionToNews
	msgbox CeruleanCity_House4_Text_ThanksForSpreadingNewsTakeThis
	giveitem REWARD_ITEM
	goto_if_eq VAR_RESULT, FALSE, CeruleanCity_House4_EventScript_NoRoomForBerries
	release
	end

@ Big reward for sending Wonder News every 4th time
CeruleanCity_House4_EventScript_Reward_SentBig::
	call CeruleanCity_House4_EventScript_MovementReactionToNews
	msgbox CeruleanCity_House4_Text_MagnificentNewsSpreadHaveBerries
	giveitem REWARD_ITEM, 4
	goto_if_eq VAR_RESULT, FALSE, CeruleanCity_House4_EventScript_NoRoomForBerries
	release
	end

@ Player has hit reward limit and must wait to receive new rewards
CeruleanCity_House4_EventScript_AtMax::
	applymovement LOCALID_WONDER_NEWS_BERRY_MAN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox CeruleanCity_House4_Text_EnjoyingMyselfWithAllSortsOfNews
	release
	end

CeruleanCity_House4_EventScript_MovementReactionToNews::
	playse SE_PIN
	applymovement LOCALID_WONDER_NEWS_BERRY_MAN, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_WONDER_NEWS_BERRY_MAN, Common_Movement_FacePlayer
	waitmovement 0
	applymovement LOCALID_WONDER_NEWS_BERRY_MAN, Common_Movement_Delay48
	waitmovement 0
	return

CeruleanCity_House4_EventScript_NoRoomForBerries::
	msgbox CeruleanCity_House4_Text_YourBerryPouchIsFull
	release
	end

CeruleanCity_House4_Text_NothingEntertaining::
	.string "{JPN}うーむ\n"
	.string "ひま じゃのう……\p"
	.string "さいきん なにか おもしろい ことは\n"
	.string "おこって ないかのう$"

CeruleanCity_House4_Text_NewNewsInformativeHaveThis::
	.string "{JPN}おお!\n"
	.string "あたらしい ニュースじゃ!\p"
	.string "うーむ……\p"
	.string "なるほど!\p"
	.string "いや ためになった わい!\n"
	.string "わかいのに ニュースに きょうみが\l"
	.string "あるとは かんしん じゃのう\p"
	.string "わしに ニュースを みせてくれた\n"
	.string "おれいに これを やろう!$"

CeruleanCity_House4_Text_IncredibleNewsHaveBerries::
	.string "{JPN}おお!\n"
	.string "こっこのニュースは!\p"
	.string "……ふーむ\p"
	.string "すごい!\n"
	.string "すごい ニュースじゃ!\p"
	.string "こんな すごい ニュースを\n"
	.string "みたのは ひさしぶり じゃわい!\p"
	.string "いいニュースを みせてくれた\n"
	.string "おれいに たくさん きのみをやろう!$"

CeruleanCity_House4_Text_WishCouldShareNewsWithOthers::
	.string "{JPN}うーむ こんな いいニュース\n"
	.string "ひとりじめ するには もったいない\p"
	.string "だれかにも しらせたいのう\n"
	.string "みせて やりたいのう$"

CeruleanCity_House4_Text_ThanksForSpreadingNewsTakeThis::
	.string "{JPN}おお! ニュースを\n"
	.string "ひろめて くれたのか!\p"
	.string "いいニュースと いうものは\n"
	.string "いろんなひとに よまれてこそ\l"
	.string "ほんとうの かちが でるものじゃ!\p"
	.string "うむ! ニュースを ひろめてくれた\n"
	.string "おれいに これをやろう!$"

CeruleanCity_House4_Text_MagnificentNewsSpreadHaveBerries::
	.string "{JPN}すばらしい! またニュースを\n"
	.string "ひろめて くれたのか!\p"
	.string "ニュースも たくさんの ひとに\n"
	.string "よんでもらえて うれしかろう!\p"
	.string "うむ!\p"
	.string "これは ニュースと わしからの\n"
	.string "かんしゃの しるしじゃ!\p"
	.string "いつもより たくさん\n"
	.string "きのみを やろう!$"

CeruleanCity_House4_Text_EnjoyingMyselfWithAllSortsOfNews::
	.string "{JPN}いろんな ニュースが\n"
	.string "よめて おもしろいのう\p"
	.string "うむ まんぞくじゃ!\n"
	.string "しばらくは あきんわい$"

CeruleanCity_House4_Text_YourBerryPouchIsFull::
	.string "{JPN}むう!? きのみが いっぱいじゃな!\n"
	.string "じゃあ これは わしの ものじゃ$"

