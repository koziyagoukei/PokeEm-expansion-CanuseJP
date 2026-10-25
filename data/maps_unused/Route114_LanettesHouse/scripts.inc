Route114_LanettesHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route114_LanettesHouse_OnTransition
	.byte 0

Route114_LanettesHouse_OnTransition:
	setflag FLAG_LANDMARK_LANETTES_HOUSE
	end

Route114_LanettesHouse_EventScript_Lanette::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_DOLL_LANETTE, Route114_LanettesHouse_EventScript_OfferAdvice
	setflag FLAG_SYS_PC_LANETTE
	msgbox Route114_LanettesHouse_Text_EverythingClutteredKeepThis, MSGBOX_DEFAULT
	givedecoration DECOR_LOTAD_DOLL
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowNoRoomForDecor
	setflag FLAG_RECEIVED_DOLL_LANETTE
	release
	end

Route114_LanettesHouse_EventScript_OfferAdvice::
	msgbox Route114_LanettesHouse_Text_OrganizeYourBoxes, MSGBOX_DEFAULT
	release
	end

Route114_LanettesHouse_EventScript_Notebook::
	lockall
	msgbox Route114_LanettesHouse_Text_ResearchNotesPage1, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route114_LanettesHouse_EventScript_NotebookPage2
	msgbox Route114_LanettesHouse_Text_ClosedTheNotebook, MSGBOX_DEFAULT
	releaseall
	end

Route114_LanettesHouse_EventScript_NotebookPage2::
	msgbox Route114_LanettesHouse_Text_ResearchNotesPage2, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, Route114_LanettesHouse_EventScript_NotebookPage3
	releaseall
	end

Route114_LanettesHouse_EventScript_NotebookPage3::
	msgbox Route114_LanettesHouse_Text_ResearchNotesPage3, MSGBOX_DEFAULT
	return

Route114_LanettesHouse_EventScript_PC::
	msgbox Route114_LanettesHouse_Text_EmailFromBill, MSGBOX_SIGN
	end

Route114_LanettesHouse_Text_EverythingClutteredKeepThis:
	.string "マユミ『あっ! {PLAYER}{KUN}\pちらかっていて ごめんなさい……\nけんきゅうに むちゅうに なると\lつい こう なっちゃうんです……\pはずかしいから ヒミツに してくださいね\nこれ あげますから……$"

Route114_LanettesHouse_Text_OrganizeYourBoxes:
	.string "あずかりシステムを つくった\n"
	.string "あたしからの アドバイス です\p"
	.string "どの ボックスに どの ポケモンを\n"
	.string "あずけたか わかるように\l"
	.string "きちんと せいり したほうが いいですよ！$"

Route114_LanettesHouse_Text_ResearchNotesPage1:
	.string "けんきゅう ようの ノートだ\n"
	.string "ボックスに ついて かかれている！\p"
	.string "ボックス1つに 30ぴきの\n"
	.string "ポケモンを あずけられる ようにする\p"
	.string "パソコンには ぜんぶで 420ひきの\n"
	.string "ポケモンを あずけられる ようにする\p"
	.string "つづきを よみますか？$"

Route114_LanettesHouse_Text_ResearchNotesPage2:
	.string "ポケモンを マーキングすると ボックスが\n"
	.string "せいり しやすくなる ようにする\p"
	.string "ボックスの なまえ かべがみを\n"
	.string "かえられる ようにして あずけられた\l"
	.string "ポケモンが よろこぶ ようにする\p"
	.string "つづきを よみますか？$"

Route114_LanettesHouse_Text_ResearchNotesPage3:
	.string "ポケモンが てんそう される ボックスは\n"
	.string "さいごに みた ボックスにする\p"
	.string "その ボックスが いっぱいの ときは\n"
	.string "つぎの ボックスに てんそう する\p"
	.string "つまり ボックスを みることで\n"
	.string "ポケモンを てんそう したい ボックスに\l"
	.string "きりかえられる ようにする$"

Route114_LanettesHouse_Text_ClosedTheNotebook:
	.string "{PLAYER}は ノートを とじた$"

Route114_LanettesHouse_Text_EmailFromBill:
	.string "パソコンを みると\nなんと でんしメールが きていた!\p「…… …… ……\pきみの あずかり システム\nなかなか べんり やな\pいろんな ところに\nじょせい らしい きくばりが あって\lつかってて たのしいし\pいっしょに かいはつ した\nなかま として うれしいわ!\pほな これからも あずかりシステムの\nけんきゅう がんばってな\p…… …… ……」$"

