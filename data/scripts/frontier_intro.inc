@ Only NewGameInitData arms this sequence. Zero remains inert for older saves.
FrontierIntro_EventScript_Arrival::
	lockall
	delay 30
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_LookAround
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_FRONTIER_HUB_INTRO_COOK, FrontierIntro_Movement_Notice
	waitmovement 0
	playbgm MUS_RG_ENCOUNTER_ROCKET, FALSE
	applymovement LOCALID_FRONTIER_HUB_INTRO_COOK, FrontierIntro_Movement_CookApproach
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_WatchCookApproach
	waitmovement 0
	msgbox FrontierIntro_Text_FirstMeeting, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_Question
	waitmovement 0
	msgbox FrontierIntro_Text_OfCourse, MSGBOX_DEFAULT
	fadenewbgm MUS_MT_PYRE_EXTERIOR
	msgbox FrontierIntro_Text_WhatHappened, MSGBOX_DEFAULT
	msgbox FrontierIntro_Text_YourRole, MSGBOX_DEFAULT
	fadedefaultbgm
	msgbox FrontierIntro_Text_Preparations, MSGBOX_DEFAULT
	msgbox FrontierIntro_Text_MeetSomeone, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_HUB_INTRO_COOK, FrontierIntro_Movement_CookLeaveHub
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_WatchCookLeave
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_WalkDown
	waitmovement 0
	@ The new game hid the hub's camp-area name banner; the camp should show its own.
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	setvar VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_FAMILY
	warp MAP_PARTY_ROAMER_PARK, 0
	waitstate
	end

FrontierIntro_EventScript_Family::
	lockall
	@ The reunion is staged at the gate. Any other arrival goes through it first.
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_ne VAR_0x8004, 13, FrontierIntro_EventScript_FamilyFromGate
	goto_if_ne VAR_0x8005, 5, FrontierIntro_EventScript_FamilyFromGate
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_WalkDown
	waitmovement 0
	opendoor 13, 4
	waitdooranim
	clearflag FLAG_TEMP_10
	addobject LOCALID_PARTY_ROAMER_INTRO_COOK
	applymovement LOCALID_PARTY_ROAMER_INTRO_COOK, FrontierIntro_Movement_WalkDown
	waitmovement 0
	closedoor 13, 4
	waitdooranim
	msgbox FrontierIntro_Text_CookCallsOut, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_PARTY_ROAMER_MOM, FrontierIntro_Movement_NoticeUp
	applymovement LOCALID_PARTY_ROAMER_NORMAN, FrontierIntro_Movement_NoticeUp
	waitmovement 0
	fadenewbgm MUS_LITTLEROOT
	msgbox FrontierIntro_Text_MomCallsOut, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PARTY_ROAMER_MOM, FrontierIntro_Movement_MomRunUp
	applymovement LOCALID_PARTY_ROAMER_NORMAN, FrontierIntro_Movement_NormanWalkUp
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceLeft
	waitmovement 0
	msgbox FrontierIntro_Text_MomWelcome, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_Surprise
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceRight
	waitmovement 0
	msgbox FrontierIntro_Text_DadWelcome, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceUp
	waitmovement 0
	msgbox FrontierIntro_Text_CookFamily, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceLeft
	waitmovement 0
	msgbox FrontierIntro_Text_MomRest, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceRight
	waitmovement 0
	msgbox FrontierIntro_Text_DadFreedom, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceUp
	waitmovement 0
	msgbox FrontierIntro_Text_CookGuide, MSGBOX_DEFAULT
	closemessage
	@ The cook returns to the diner; no persistent follower slot is consumed.
	applymovement LOCALID_PARTY_ROAMER_INTRO_COOK, FrontierIntro_Movement_CookToDiner
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_WatchCookToDiner
	waitmovement 0
	opendoor 18, 4
	waitdooranim
	applymovement LOCALID_PARTY_ROAMER_INTRO_COOK, FrontierIntro_Movement_EnterDoor
	waitmovement 0
	closedoor 18, 4
	waitdooranim
	removeobject LOCALID_PARTY_ROAMER_INTRO_COOK
	setflag FLAG_TEMP_10
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceLeft
	waitmovement 0
	msgbox FrontierIntro_Text_MomSendOff, MSGBOX_DEFAULT
	closemessage
	fadedefaultbgm
	setvar VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_COMPLETE
	releaseall
	end

FrontierIntro_EventScript_FamilyFromGate:
	warp MAP_PARTY_ROAMER_PARK, 0
	waitstate
	end

FrontierIntro_EventScript_Cook::
	msgbox FrontierIntro_Text_CookGuide, MSGBOX_NPC
	end

FrontierIntro_Movement_LookAround:
	face_left
	delay_16
	delay_8
	face_right
	delay_16
	delay_8
	face_down
	delay_16
	step_end

FrontierIntro_Movement_Notice:
	emote_exclamation_mark
	face_down
	delay_8
	step_end

FrontierIntro_Movement_CookApproach:
	walk_down
	walk_down
	walk_down
	step_end

FrontierIntro_Movement_WatchCookApproach:
	delay_16
	face_up
	step_end

FrontierIntro_Movement_CookLeaveHub:
	walk_right
	walk_down
	walk_down
	walk_down
	set_invisible
	step_end

FrontierIntro_Movement_WatchCookLeave:
	delay_16
	face_right
	delay_16
	face_down
	step_end

FrontierIntro_Movement_WalkDown:
	walk_down
	step_end

FrontierIntro_Movement_FaceUp:
	face_up
	step_end

FrontierIntro_Movement_FaceLeft:
	face_left
	step_end

FrontierIntro_Movement_FaceRight:
	face_right
	step_end

FrontierIntro_Movement_Question:
	emote_question_mark
	step_end

FrontierIntro_Movement_Surprise:
	emote_exclamation_mark
	step_end

FrontierIntro_Movement_NoticeUp:
	face_up
	emote_exclamation_mark
	step_end

FrontierIntro_Movement_MomRunUp:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	face_right
	step_end

FrontierIntro_Movement_NormanWalkUp:
	walk_right
	walk_up
	walk_up
	walk_up
	face_left
	step_end

FrontierIntro_Movement_CookToDiner:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	face_up
	step_end

FrontierIntro_Movement_WatchCookToDiner:
	delay_16
	face_right
	step_end

FrontierIntro_Movement_EnterDoor:
	walk_up
	set_invisible
	step_end

FrontierIntro_Text_FirstMeeting:
	.string "{JPN}？？？『……おまえが {PLAYER}か。\n"
	.string "待ってたぜ。\p"
	.string "おれは この先のリゾートで\n"
	.string "飯屋をやってる。\l"
	.string "店主とでも 呼んでくれ。\p"
	.string "で……ここへ来る前に \n"
	.string "事情は 聞いてきたか？』$"

FrontierIntro_Text_OfCourse:
	.string "{JPN}店主『……だろうな。\p"
	.string "おれも 相棒のジガルデに\n"
	.string "教えられて やっと分かったんだ。\p"
	.string "まずは 落ち着いて聞けよ。』$"

FrontierIntro_Text_WhatHappened:
	.string "{JPN}店主『ここは バトルフロンティア。\n"
	.string "……だが 今は 様子が違う。\p"
	.string "ヒトポケルスってやつのせいで \n"
	.string "人と ポケモンの記憶や気持ちが\l"
	.string "混ざり合っているんだ。\p"
	.string "皆 幸せな夢の中で\n"
	.string "いつも通りに 暮らしてる。\p"
	.string "自分と 誰かの違いすら \n"
	.string "分からなくなりながらな。』$"

FrontierIntro_Text_YourRole:
	.string "{JPN}店主『そこで おまえの出番だ。\p"
	.string "おまえは その繋がりの外から来た\n"
	.string "トレーナーだろう。\p"
	.string "おまえとの勝負が 自分を思い出す\n"
	.string "きっかけに なるかもしれない。\p"
	.string "……もっとも それで治るかは\n"
	.string "まだ 分からねえ。\p"
	.string "まずは 話して 勝負してみろ。\n"
	.string "その先は 一緒に考えようぜ。』$"

FrontierIntro_Text_Preparations:
	.string "{JPN}店主『ここは 準備のためのハブだ。\n"
	.string "ポケモンも 道具も揃えてある。\p"
	.string "仲間を増やすのも 技の相談も \n"
	.string "ここの連中に 頼めばいい。\p"
	.string "パソコンの ボックスは\n"
	.string "おまえの 世界に 繋がっている。\p"
	.string "……まあ 準備は 後でいい。\n"
	.string "その前に 会わせたい人がいるんだ。』$"

FrontierIntro_Text_MeetSomeone:
	.string "{JPN}店主『隣のリゾートだ。\n"
	.string "ついてきな。』$"

FrontierIntro_Text_CookCallsOut:
	.string "{JPN}店主『おーい！\n"
	.string "連れてきたぞ！』$"

FrontierIntro_Text_MomCallsOut:
	.string "{JPN}ママ『{PLAYER}！』$"

FrontierIntro_Text_MomWelcome:
	.string "{JPN}ママ『{PLAYER}！\n"
	.string "無事に 着いたのね！\p"
	.string "ふふっ びっくりした？\n"
	.string "ママ達も 一足先に来ていたのよ。』$"

FrontierIntro_Text_DadWelcome:
	.string "{JPN}パパ『よく来たな {PLAYER}。\p"
	.string "ホウエンの旅を 終えてすぐ \n"
	.string "また 大きな役目だな。\p"
	.string "今度は パパ達にも\n"
	.string "手伝わせてほしい。\p"
	.string "ママと二人で ここを整えて \n"
	.string "おまえが来るのを 待っていたんだ。』$"

FrontierIntro_Text_CookFamily:
	.string "{JPN}店主『おれも こっちへ来てから \n"
	.string "二人には 世話になりっぱなしだ。\p"
	.string "ここに いるのは みんな\n"
	.string "おまえを 助けに来た仲間だ。\p"
	.string "飯も 風呂も ある。\n"
	.string "一人で 抱え込むんじゃないぞ。』$"

FrontierIntro_Text_MomRest:
	.string "{JPN}ママ『せっかく 来たんだもの。\n"
	.string "ここでの暮らしも 楽しまなくちゃ！\p"
	.string "ポケモンを 遊ばせたり\n"
	.string "一緒に ポロックを作ったり。\p"
	.string "疲れたら いつでも\n"
	.string "帰っていらっしゃい！』$"

FrontierIntro_Text_DadFreedom:
	.string "{JPN}パパ『7つのバトル施設と\n"
	.string "コンテスト会場が あるそうだ。\p"
	.string "どこから 向かうかは \n"
	.string "おまえと ポケモンで決めてごらん。\p"
	.string "無理に 急ぐことはない。\n"
	.string "パパ達は ここにいるからな。』$"

FrontierIntro_Text_CookGuide:
	.string "{JPN}店主『北のゲートを 抜ければ \n"
	.string "フロンティアの 南広場だ。\p"
	.string "ハブも コンテストも 美術館も \n"
	.string "その広場に 揃ってる。\p"
	.string "そこから さらに北へ進むと \n"
	.string "7つのバトル施設がある。\p"
	.string "困ったら メニューのTIPSも読め。\n"
	.string "おれは 飯屋に戻ってる。\p"
	.string "……腹が減ったら 来いよ。』$"

FrontierIntro_Text_MomSendOff:
	.string "{JPN}ママ『さあ {PLAYER}も\n"
	.string "まずは ゆっくり見て回ってね！』$"
