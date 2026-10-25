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
	.string "まってたぜ。\p"
	.string "おれは この さきの リゾートで\n"
	.string "めしやを やってる もんだ。\l"
	.string "てんしゅ とでも よんでくれ。\p"
	.string "で…… ここへ くるまえに\n"
	.string "はなしは きいてきたか？$"

FrontierIntro_Text_OfCourse:
	.string "{JPN}てんしゅ『……だろうな。\p"
	.string "おれも あいぼうの ジガルデに\n"
	.string "おしえられて やっと わかったんだ。\p"
	.string "まずは おちついて きけよ。$"

FrontierIntro_Text_WhatHappened:
	.string "{JPN}てんしゅ『ここは バトルフロンティア。\n"
	.string "だが いまは ようすが ちがう。\p"
	.string "ヒトポケルスって やつの せいで\n"
	.string "ひとと ポケモンの きおくや きもちが\l"
	.string "まざりあって いるんだ。\p"
	.string "みんな しあわせな ゆめの なかで\n"
	.string "いつもどおり くらしている。\p"
	.string "じぶんと だれかの ちがいすら\n"
	.string "わからなく なりながらな。$"

FrontierIntro_Text_YourRole:
	.string "{JPN}てんしゅ『そこで おまえの でばんだ。\p"
	.string "おまえは その わの そとから\n"
	.string "やってきた トレーナーだ。\p"
	.string "おまえに しょうぶで まけることが\n"
	.string "じぶんを おもいだす\l"
	.string "きっかけに なるかもしれない。\p"
	.string "……もっとも それで なおるのかは\n"
	.string "まだ わからねえ。\p"
	.string "まずは はなして しょうぶしてみろ。\n"
	.string "あとは いっしょに かんがえようぜ。$"

FrontierIntro_Text_Preparations:
	.string "{JPN}てんしゅ『ここは じゅんびの ハブだ。\n"
	.string "ポケモンも どうぐも そろえてある。\p"
	.string "なかまを ふやすのも わざの そうだんも\n"
	.string "ここの れんちゅうに たのめばいい。\p"
	.string "パソコンの ボックスは\n"
	.string "おまえの せかいに つながっている。\p"
	.string "まあ じゅんびは あとで いい。\n"
	.string "そのまえに あわせたい ひとが いるんだ。$"

FrontierIntro_Text_MeetSomeone:
	.string "{JPN}てんしゅ『となりの リゾートだ。\n"
	.string "ついてきな。$"

FrontierIntro_Text_CookCallsOut:
	.string "{JPN}てんしゅ『おーい！\n"
	.string "つれてきたぞ！$"

FrontierIntro_Text_MomCallsOut:
	.string "{JPN}ママ『{PLAYER}！$"

FrontierIntro_Text_MomWelcome:
	.string "{JPN}ママ『{PLAYER}！\n"
	.string "ぶじに ついたのね！\p"
	.string "ふふっ びっくりした？\n"
	.string "ママたちも ひとあしさきに きてたのよ。$"

FrontierIntro_Text_DadWelcome:
	.string "{JPN}パパ『よく きたな {PLAYER}。\p"
	.string "ホウエンの たびを おえて すぐ\n"
	.string "また おおきな やくめだな。\p"
	.string "こんどは パパたちにも\n"
	.string "てつだわせて ほしい。\p"
	.string "ママと ふたりで ここを ととのえて\n"
	.string "おまえが くるのを まっていたんだ。$"

FrontierIntro_Text_CookFamily:
	.string "{JPN}てんしゅ『おれも こっちへ きてから\n"
	.string "ふたりには せわに なりっぱなしだ。\p"
	.string "ここに いるのは みんな\n"
	.string "おまえを たすけに きた なかまだ。\p"
	.string "めしも ふろも ある。\n"
	.string "ひとりで かかえこむんじゃ ないぞ。$"

FrontierIntro_Text_MomRest:
	.string "{JPN}ママ『せっかく きたんだもの。\n"
	.string "ここでの くらしも たのしまなくちゃ！\p"
	.string "ポケモンを あそばせたり\n"
	.string "いっしょに ポロックを つくったり。\p"
	.string "つかれたら いつでも\n"
	.string "かえって いらっしゃい！$"

FrontierIntro_Text_DadFreedom:
	.string "{JPN}パパ『7つの バトルしせつと\n"
	.string "コンテストかいじょうが あるそうだ。\p"
	.string "どこから むかうかは\n"
	.string "おまえと ポケモンで きめて ごらん。\p"
	.string "むりに いそぐことは ない。\n"
	.string "パパたちは ここに いるからな。$"

FrontierIntro_Text_CookGuide:
	.string "{JPN}てんしゅ『きたの ゲートを ぬければ\n"
	.string "フロンティアの みなみひろばだ。\p"
	.string "ハブも コンテストも びじゅつかんも\n"
	.string "その ひろばに そろってる。\p"
	.string "そこから さらに きたへ すすむと\n"
	.string "7つの バトルしせつが ある。\p"
	.string "こまったら メニューの TIPSも よめ。\n"
	.string "おれは めしやに もどってる。\p"
	.string "……はらが へったら こいよ。$"

FrontierIntro_Text_MomSendOff:
	.string "{JPN}ママ『さあ {PLAYER}も\n"
	.string "まずは ゆっくり みてまわってね！$"
