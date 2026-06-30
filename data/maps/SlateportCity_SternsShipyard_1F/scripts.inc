SlateportCity_SternsShipyard_1F_MapScripts::
	.byte 0

SlateportCity_SternsShipyard_1F_EventScript_Dock::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_SternsShipyard_1F_EventScript_FerryReady
	goto_if_set FLAG_BADGE07_GET, SlateportCity_SternsShipyard_1F_EventScript_BrineyJoined
	goto_if_set FLAG_DELIVERED_DEVON_GOODS, SlateportCity_SternsShipyard_1F_EventScript_NeedVeteran
	goto_if_set FLAG_DOCK_REJECTED_DEVON_GOODS, SlateportCity_SternsShipyard_1F_EventScript_GoFindStern
	msgbox SlateportCity_SternsShipyard_1F_Text_CantMakeHeadsOrTails, MSGBOX_DEFAULT
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_MeetDockDeliverToStern, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DOCK, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag FLAG_DOCK_REJECTED_DEVON_GOODS
	setflag FLAG_HIDE_SLATEPORT_CITY_TEAM_AQUA
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_FerryReady::
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_FerryIsReady, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_BrineyJoined::
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_BrineyJoinedUs, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_GoFindStern::
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_CouldYouFindStern, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DOCK, Common_Movement_FaceOriginalDirection
	waitmovement 0
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_NeedVeteran::
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_CouldUseAdviceFromVeteran, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_Scientist1::
	msgbox SlateportCity_SternsShipyard_1F_Text_SeaIsLikeLivingThing, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_1F_EventScript_Scientist2::
	msgbox SlateportCity_SternsShipyard_1F_Text_GetSeasickEasily, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_1F_EventScript_Briney::
	msgbox SlateportCity_SternsShipyard_1F_Text_DecidedToHelpDock, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_1F_Text_CantMakeHeadsOrTails:
	.string "{JPN}えーと…… これが こーなって\nそいでもって あれが あーなって\lと いうことは それと これは どれ?\pああ もう わけが わからん!"
	.string ""
	.string ""
	.string "$"

SlateportCity_SternsShipyard_1F_Text_MeetDockDeliverToStern:
	.string "{JPN}……ん?\pぼくは クスノキかんちょう から\nれんらくせんの せっけいを まかされた\lツガという ものだけど……\pあっ!?\nそれは デボンのにもつ!\pうーん……\nこまったな……\pクスノキかんちょう\nほかにも しごとが あるからって\lどこかに いっちゃったんだ\pわるいけど きみ クスノキかんちょう\nさがして にもつを わたしてあげてよ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_SternsShipyard_1F_Text_CouldYouFindStern:
	.string "{JPN}ツガ『クスノキかんちょう\nどこに いっちゃったのかな?\pわるいけど きみ クスノキかんちょう\nさがして にもつを わたしてあげてよ!"
	.string ""
	.string ""
	.string "$"

SlateportCity_SternsShipyard_1F_Text_CouldUseAdviceFromVeteran:
	.string "{JPN}ツガ『ふねをつくる っていうのは\nつくえの うえでの けいさん だけでは\lわからないことも おおくてね\pやっぱり うみのことを しりつくした\nベテランの ちえが ほしいところだよ……"
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_SternsShipyard_1F_Text_BrineyJoinedUs:
	.string "{JPN}ツガ『やあ! ハギろうじんが ここで\nてつだって くれるように なったよ!\pでんせつの ふなのりに てつだってもらえて\nれんらくせんは ちゃくちゃくと\lかんせいに むかっているよ!"
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_SternsShipyard_1F_Text_FerryIsReady:
	.string "{JPN}ツガ『ついに ふねは かんせいしたよ!\pできた れんらくせん タイドリップごうは\nまさに さいこう けっさく!\pだけど もっと いいものが できそうな……\nそんな きも するんだよ\pそう……\nものづくりに おわりは ないのさ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_SternsShipyard_1F_Text_DecidedToHelpDock:
	.string "{JPN}ハギ『おお! {PLAYER}{KUN}!\nひさしぶり じゃのう!\pいやあ きみと であってから\nふなのりの ちが さわいでな……\pツガくんに きょうりょく して\nれんらくせんを つくりたく なったんじゃ!\pれんらくせんが できると おおくの\nひとを はこべるように なるからのう\pしかし…… ふむう……\nツガくんは すごいのう……\pかれの ぎじゅつと わしの けいけんを\nがったい させれば かならずや\lすごい ふねが できるに ちがいない!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_SternsShipyard_1F_Text_SeaIsLikeLivingThing:
	.string "{JPN}きせつや てんきや つきのいち など\nさまざまな じょうけん によって\lうみは つねに すがたを かえていきます\pそう! うみ というのは\nいきもの のような もの なんですよ!"
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_SternsShipyard_1F_Text_GetSeasickEasily:
	.string "{JPN}ぼくは すぐに ふなよい するのよね\nだから ここで おてつだい してるのよね"
	.string "$"
