SSAnne_CaptainsOffice_Frlg_MapScripts::
	.byte 0

SSAnne_CaptainsOffice_EventScript_Captain::
	lock
	goto_if_set FLAG_GOT_HM01, SSAnne_CaptainsOffice_EventScript_AlreadyGotCut
	msgbox SSAnne_CaptainsOffice_Text_CaptainIFeelSeasick
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message SSAnne_CaptainsOffice_Text_RubbedCaptainsBack
	waitmessage
	playfanfare MUS_HEAL
	waitfanfare
	call EventScript_RestorePrevTextColor
	delay 50
	applymovement LOCALID_SS_ANNE_CAPTAIN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SSAnne_CaptainsOffice_Text_ThankYouHaveHMForCut
	giveitem_msg SSAnne_CaptainsOffice_Text_ObtainedHM01FromCaptain, ITEM_HM01, 1, MUS_RG_OBTAIN_KEY_ITEM
	msgbox SSAnne_CaptainsOffice_Text_ExplainCut
	setflag FLAG_GOT_HM01
	setvar VAR_MAP_SCENE_VERMILION_CITY, 1
	release
	end

@ Unused
SSAnne_CaptainsOffice_EventScript_NoRoomForCut::
	msgbox SSAnne_CaptainsOffice_Text_YouHaveNoRoomForThis
	closemessage
	applymovement LOCALID_SS_ANNE_CAPTAIN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SSAnne_CaptainsOffice_EventScript_AlreadyGotCut::
	applymovement LOCALID_SS_ANNE_CAPTAIN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SSAnne_CaptainsOffice_Text_SSAnneWillSetSailSoon
	release
	end

SSAnne_CaptainsOffice_EventScript_TrashCan::
	msgbox SSAnne_CaptainsOffice_Text_YuckShouldntHaveLooked, MSGBOX_SIGN
	end

SSAnne_CaptainsOffice_EventScript_Book::
	msgbox SSAnne_CaptainsOffice_Text_HowToConquerSeasickness, MSGBOX_SIGN
	end

SSAnne_CaptainsOffice_Text_CaptainIFeelSeasick::
	.string "{JPN}せんちょう『おえー…!\n"
	.string "ふなよいで… すっかり ダウンだ\l"
	.string "くるしい… … うえ!$"

SSAnne_CaptainsOffice_Text_RubbedCaptainsBack::
	.string "{JPN}{PLAYER}は せんちょうの\n"
	.string "せなかを さすって あげた!\p"
	.string "すりすり… すりすり…\n"
	.string "すりすり… すりすり…$"

SSAnne_CaptainsOffice_Text_ThankYouHaveHMForCut::
	.string "{JPN}せんちょう『ふう… ありがとう\n"
	.string "だいぶ ラクに なった みたいだ\p"
	.string "…なになに なんだ\n"
	.string "ひでんの わざを みに きたのか\p"
	.string "げんきな ときなら じまんの\n"
	.string "いあいぎりを みせるんだがなあ…\p"
	.string "そうだ!\n"
	.string "かわりに これを あげよう!\p"
	.string "これを きみの\n"
	.string "ポケモンに おしえれば\l"
	.string "たたかって いない ときでも\l"
	.string "いつでも いあいぎりが みられる!$"

SSAnne_CaptainsOffice_Text_ObtainedHM01FromCaptain::
	.string "{JPN}{PLAYER}は せんちょう から\n"
	.string "ひでんマシン01を もらった!$"

SSAnne_CaptainsOffice_Text_ExplainCut::
	.string "{JPN}いあいぎりを つかえば\n"
	.string "ほそい きも きれる!\p"
	.string "ためしに… クチバシティに ある\n"
	.string "きでも きって みたまえ$"

SSAnne_CaptainsOffice_Text_SSAnneWillSetSailSoon::
	.string "{JPN}せんちょう『…ふう!\p"
	.string "らくに なったし…\n"
	.string "そろそろ じかんだ!\p"
	.string "もうすぐ サント・アンヌは\n"
	.string "しゅっぱつ するよ!\p"
	.string "また クチバに くる ときまで\n"
	.string "ごきげんよう!$"

SSAnne_CaptainsOffice_Text_YouHaveNoRoomForThis::
	.string "{JPN}なんとー!\n"
	.string "にもつが いっぱいだ!$"

SSAnne_CaptainsOffice_Text_YuckShouldntHaveLooked::
	.string "{JPN}あんまり みないほうが\n"
	.string "いい みたいだ…$"

SSAnne_CaptainsOffice_Text_HowToConquerSeasickness::
	.string "{JPN}ふなよい しない\n"
	.string "ふなのり にゅうもん…\l"
	.string "…これは\l"
	.string "せんちょうが よんでいる ほんだ!$"

