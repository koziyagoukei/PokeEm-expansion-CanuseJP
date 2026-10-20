ApricornTreeScript::
	lock
	faceplayer
	message ApricornTree_Text_Intro
	waitmessage
	waitbuttonpress
	special ObjectEventInteractionGetApricornTreeData
	goto_if_gt VAR_0x8005, 0, ApricornTree_EventScript_WantToPick
	message ApricornTree_Text_Empty
	waitmessage
	waitbuttonpress
	release
	end

ApricornTree_EventScript_WantToPick::
	buffernumberstring STR_VAR_2, VAR_0x8005
	msgbox ApricornTree_Text_WantToPick, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, ApricornTree_EventScript_PickApricorn
	goto_if_eq VAR_RESULT, NO, ApricornTree_EventScript_CancelPickingApricorn

.set APRICORN_NORMAL_BAG_FULL, 0
.set APRICORN_NORMAL_SPACE_IN_BAG, 1

ApricornTree_EventScript_PickApricorn::
	special ObjectEventInteractionPickApricornTree
	call EventScript_BufferPocketNameAndTryFanfare
	goto_if_eq VAR_0x8006, APRICORN_NORMAL_BAG_FULL, ApricornTree_EventScript_PocketFull
	message ApricornTree_Text_PickedTheApricorn
.if OW_SHOW_ITEM_DESCRIPTIONS != OW_ITEM_DESCRIPTIONS_OFF
	copyvar VAR_0x8006 VAR_0x8004
.endif
	delay 10
.if OW_SHOW_ITEM_DESCRIPTIONS != OW_ITEM_DESCRIPTIONS_OFF
	showberrydescription
.endif
	playfanfare MUS_OBTAIN_BERRY
	waitmessage
	waitfanfare
	waitbuttonpress
	message ApricornTree_Text_PutAwayApricorn
	waitmessage
	waitbuttonpress
.if OW_SHOW_ITEM_DESCRIPTIONS != OW_ITEM_DESCRIPTIONS_OFF
	hideitemdescription
.endif
	release
	end

ApricornTree_EventScript_PocketFull::
	message ApricornTree_Text_PocketFull
	waitmessage
	waitbuttonpress
	release
	end

ApricornTree_EventScript_CancelPickingApricorn::
	message ApricornTree_Text_ApricornLeftUnpicked
	waitmessage
	waitbuttonpress
	release
	end

ApricornTree_Text_Intro:
	.string "{JPN}ぼんぐりの なる きだ!$"

ApricornTree_Text_Empty:
	.string "{JPN}ぼんぐりは なっていない……$"

ApricornTree_Text_WantToPick:
	.string "{JPN}{STR_VAR_1}が {STR_VAR_2}こ なっている!\n"
	.string "{STR_VAR_1}を とりますか?$"

ApricornTree_Text_PickedTheApricorn:
	.string "{JPN}{PLAYER}は {STR_VAR_1}を\n"
	.string "{STR_VAR_2}こ とった!$"

ApricornTree_Text_PutAwayApricorn:
	.string "{JPN}{PLAYER}は {STR_VAR_1}を\n"
	.string "{STR_VAR_3}ポケットに しまった!$"

ApricornTree_Text_PocketFull:
	.string "{JPN}バッグの {STR_VAR_3}ポケットが いっぱいだ\p"
	.string "{PLAYER}は {STR_VAR_1}を\n"
	.string "そのままに しておいた$"

ApricornTree_Text_ApricornLeftUnpicked:
	.string "{JPN}{PLAYER}は {STR_VAR_1}を\n"
	.string "そのままに しておいた$"
