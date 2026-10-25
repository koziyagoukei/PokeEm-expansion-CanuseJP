@ Only NewGameInitData arms this sequence. Zero remains inert for older saves.
FrontierIntro_EventScript_Arrival::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceUp
	waitmovement 0
	applymovement LOCALID_FRONTIER_HUB_INTRO_COOK, FrontierIntro_Movement_Approach
	waitmovement 0
	msgbox FrontierIntro_Text_FirstMeeting, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_Question
	waitmovement 0
	msgbox FrontierIntro_Text_OfCourse, MSGBOX_DEFAULT
	msgbox FrontierIntro_Text_WhatHappened, MSGBOX_DEFAULT
	msgbox FrontierIntro_Text_YourRole, MSGBOX_DEFAULT
	msgbox FrontierIntro_Text_Preparations, MSGBOX_DEFAULT
	msgbox FrontierIntro_Text_MeetFamily, MSGBOX_DEFAULT
	closemessage
	setvar VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_FAMILY
	warp MAP_PARTY_ROAMER_PARK, 9, 8
	waitstate
	end

FrontierIntro_EventScript_Family::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_FaceUp
	applymovement LOCALID_PARTY_ROAMER_MOM, FrontierIntro_Movement_FaceDown
	applymovement LOCALID_PARTY_ROAMER_NORMAN, FrontierIntro_Movement_FaceDown
	waitmovement 0
	msgbox FrontierIntro_Text_MomWelcome, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, FrontierIntro_Movement_Surprise
	waitmovement 0
	msgbox FrontierIntro_Text_DadWelcome, MSGBOX_DEFAULT
	msgbox FrontierIntro_Text_CookFamily, MSGBOX_DEFAULT
	msgbox FrontierIntro_Text_MomRest, MSGBOX_DEFAULT
	msgbox FrontierIntro_Text_DadFreedom, MSGBOX_DEFAULT
	msgbox FrontierIntro_Text_CookGuide, MSGBOX_DEFAULT
	closemessage
	@ The cook returns to the diner; no persistent follower slot is consumed.
	applymovement LOCALID_PARTY_ROAMER_INTRO_COOK, FrontierIntro_Movement_Leave
	waitmovement 0
	removeobject LOCALID_PARTY_ROAMER_INTRO_COOK
	setflag FLAG_TEMP_10
	setvar VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_COMPLETE
	releaseall
	end

FrontierIntro_EventScript_Cook::
	msgbox FrontierIntro_Text_CookGuide, MSGBOX_NPC
	end

FrontierIntro_Movement_Approach:
	walk_down
	face_down
	step_end

FrontierIntro_Movement_FaceUp:
	face_up
	step_end

FrontierIntro_Movement_FaceDown:
	face_down
	step_end

FrontierIntro_Movement_Question:
	emote_question_mark
	step_end

FrontierIntro_Movement_Surprise:
	emote_exclamation_mark
	step_end

FrontierIntro_Movement_Leave:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	step_end

FrontierIntro_Text_FirstMeeting:
	.string "{JPN}……おまえが {PLAYER}か。\n"
	.string "おれは むこうで めしやを やってる。\p"
	.string "ここへ くるまえに\n"
	.string "はなしは きいてきたか？$"

FrontierIntro_Text_OfCourse:
	.string "{JPN}てんしゅ『……だろうな。\p"
	.string "おれも ジガルデに きいて\n"
	.string "ようやく わかったんだ。\p"
	.string "まずは おちついて きけよ。$"

FrontierIntro_Text_WhatHappened:
	.string "{JPN}てんしゅ『ここは バトルフロンティア。\n"
	.string "だが いまは すこし かわっている。\p"
	.string "ヒトポケルスって やつの せいで\n"
	.string "ひとと ポケモンの きおくや きもちが\l"
	.string "まざりあって いるんだ。\p"
	.string "みんな しあわせな ゆめの なかで\n"
	.string "いつもどおり くらしている。\p"
	.string "じぶんと だれかの ちがいまで\n"
	.string "わからなく なりながらな。$"

FrontierIntro_Text_YourRole:
	.string "{JPN}てんしゅ『おまえは その わの そとから\n"
	.string "やってきた トレーナーだ。\p"
	.string "おまえと しょうぶして まけたことが\n"
	.string "じぶんを おもいだす\l"
	.string "きっかけに なるかもしれない。\p"
	.string "もちろん それだけで\n"
	.string "なおると わかったわけじゃない。\p"
	.string "まずは はなして しょうぶしてみよう。\n"
	.string "あとは いっしょに かんがえるぞ。$"

FrontierIntro_Text_Preparations:
	.string "{JPN}てんしゅ『ここは じゅんびの ハブだ。\n"
	.string "ポケモンと どうぐは そろえてある。\p"
	.string "なかまを ふやすのも わざの そうだんも\n"
	.string "ここの れんちゅうに たのめばいい。\p"
	.string "パソコンの ボックスは\n"
	.string "おまえの せかいに つながっている。\p"
	.string "じゅんびは あわてなくて いい。\n"
	.string "まず あわせたい ひとが いるんだ。$"

FrontierIntro_Text_MeetFamily:
	.string "{JPN}てんしゅ『となりの リゾートまで\n"
	.string "ついてきな。\p"
	.string "おまえの とうさんと かあさんも\n"
	.string "まってるぞ。$"

FrontierIntro_Text_MomWelcome:
	.string "{JPN}ママ『{PLAYER}！\n"
	.string "ぶじに ついたのね！\p"
	.string "ふふっ びっくりした？\n"
	.string "ママたちも ひとあしさきに きたのよ。$"

FrontierIntro_Text_DadWelcome:
	.string "{JPN}パパ『ホウエンの たびに つづいて\n"
	.string "また おおきな やくめだな。\p"
	.string "こんどは パパたちにも\n"
	.string "てつだわせて ほしい。\p"
	.string "みんなで ここを ととのえて\n"
	.string "おまえが くるのを まっていたんだ。$"

FrontierIntro_Text_CookFamily:
	.string "{JPN}てんしゅ『おれも こっちへ きてから\n"
	.string "ふたりと いっしょに じゅんびした。\p"
	.string "ここに いるのは おまえと おなじ\n"
	.string "たすけに きた なかまたちだ。\p"
	.string "めしも ふろも ある。\n"
	.string "ひとりで かかえこむんじゃ ないぞ。$"

FrontierIntro_Text_MomRest:
	.string "{JPN}ママ『せっかくだもの\n"
	.string "ここでの くらしも たのしまなくちゃ。\p"
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
	.string "{JPN}てんしゅ『にしへ あるけば\n"
	.string "ハブと コンテストと びじゅつかんだ。\p"
	.string "そこから きたへ すすむと\n"
	.string "7つの バトルしせつが ある。\p"
	.string "こまったら メニューの TIPSも よめ。\n"
	.string "おれは めしやに もどってる。\p"
	.string "……はらが へったら こいよ。$"
