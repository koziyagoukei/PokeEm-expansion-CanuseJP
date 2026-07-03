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
	.string "{JPN}マユミ『あっ! {PLAYER}{KUN}\pちらかっていて ごめんなさい……\nけんきゅうに むちゅうに なると\lつい こう なっちゃうんです……\pはずかしいから ヒミツに してくださいね\nこれ あげますから……$"

Route114_LanettesHouse_Text_OrganizeYourBoxes:
	.string "{JPN}あずかりシステムを つくった\nあたしからの アドバイス です\pどの ボックスに どの ポケモンを\nあずけたか わかるように\lきちんと せいり したほうが いいですよ!$"

Route114_LanettesHouse_Text_ResearchNotesPage1:
	.string "{JPN}けんきゅう ようの ノートだ\nボックスに ついて かかれている!\pボックス1つに 30ぴきの\nポケモンを あずけられる ようにする\pパソコンには ぜんぶで 420ひきの\nポケモンを あずけられる ようにする\pつづきを よみますか?$"

Route114_LanettesHouse_Text_ResearchNotesPage2:
	.string "{JPN}ポケモンを マーキングすると ボックスが\nせいり しやすくなる ようにする\pボックスの なまえ かべがみを\nかえられる ようにして あずけられた\lポケモンが よろこぶ ようにする\pつづきを よみますか?$"

Route114_LanettesHouse_Text_ResearchNotesPage3:
	.string "{JPN}ポケモンが てんそう される ボックスは\nさいごに みた ボックスにする\pその ボックスが いっぱいの ときは\nつぎの ボックスに てんそう する\pつまり ボックスを みることで\nポケモンを てんそう したい ボックスに\lきりかえられる ようにする$"

Route114_LanettesHouse_Text_ClosedTheNotebook:
	.string "{JPN}{PLAYER}は ノートを とじた$"

Route114_LanettesHouse_Text_EmailFromBill:
	.string "{JPN}パソコンを みると\nなんと でんしメールが きていた!\p「…… …… ……\pきみの あずかり システム\nなかなか べんり やな\pいろんな ところに\nじょせい らしい きくばりが あって\lつかってて たのしいし\pいっしょに かいはつ した\nなかま として うれしいわ!\pほな これからも あずかりシステムの\nけんきゅう がんばってな\p…… …… ……」$"
