PartyBathExpansion_EventScript_Call::
	lockall
	special PartyBath_GetPendingCall
	copyvar VAR_0x8004, VAR_RESULT
	switch VAR_RESULT
	case PARTY_BATH_ROCK, PartyBathExpansion_EventScript_CallSteven
	case PARTY_BATH_WATERFALL, PartyBathExpansion_EventScript_CallWallace
	case PARTY_BATH_SAUNA, PartyBathExpansion_EventScript_CallMaxie
	case PARTY_BATH_DEEP, PartyBathExpansion_EventScript_CallArchie
	releaseall
	end

PartyBathExpansion_EventScript_CallSteven:
	pokenavcall PartyBathExpansion_Text_CallSteven
	goto PartyBathExpansion_EventScript_FinishCall
PartyBathExpansion_EventScript_CallWallace:
	pokenavcall PartyBathExpansion_Text_CallWallace
	goto PartyBathExpansion_EventScript_FinishCall
PartyBathExpansion_EventScript_CallMaxie:
	pokenavcall PartyBathExpansion_Text_CallMaxie
	goto PartyBathExpansion_EventScript_FinishCall
PartyBathExpansion_EventScript_CallArchie:
	pokenavcall PartyBathExpansion_Text_CallArchie
PartyBathExpansion_EventScript_FinishCall:
	waitmessage
	special PartyBath_CompleteCall
	goto_if_eq VAR_RESULT, FALSE, PartyBathExpansion_EventScript_End
	msgbox PartyBathExpansion_Text_Completed, MSGBOX_DEFAULT
PartyBathExpansion_EventScript_End:
	releaseall
	end

PartyBathExpansion_EventScript_Menu::
	lockall
	msgbox PartyBathExpansion_Text_Guide, MSGBOX_DEFAULT
	closemessage
	dynmultipush PartyBathExpansion_Text_Rock, PARTY_BATH_ROCK
	dynmultipush PartyBathExpansion_Text_Waterfall, PARTY_BATH_WATERFALL
	dynmultipush PartyBathExpansion_Text_Sauna, PARTY_BATH_SAUNA
	dynmultipush PartyBathExpansion_Text_Deep, PARTY_BATH_DEEP
	dynmultipush PartyBathExpansion_Text_Cancel, PARTY_BATH_EXPANSION_COUNT
	dynmultistack 0, 0, FALSE, 5, 0, 0, DYN_MULTICHOICE_CB_NONE
	goto_if_ge VAR_RESULT, PARTY_BATH_EXPANSION_COUNT, PartyBathExpansion_EventScript_End
	copyvar VAR_0x8004, VAR_RESULT
	special PartyBath_CheckExpansion
	goto_if_eq VAR_RESULT, FALSE, PartyBathExpansion_EventScript_Locked
	switch VAR_0x8004
	case PARTY_BATH_ROCK, PartyBathExpansion_EventScript_Rock
	case PARTY_BATH_WATERFALL, PartyBathExpansion_EventScript_Waterfall
	case PARTY_BATH_SAUNA, PartyBathExpansion_EventScript_Sauna
	case PARTY_BATH_DEEP, PartyBathExpansion_EventScript_Deep
	goto PartyBathExpansion_EventScript_End
PartyBathExpansion_EventScript_Locked:
	msgbox PartyBathExpansion_Text_Locked, MSGBOX_DEFAULT
	goto PartyBathExpansion_EventScript_End
PartyBathExpansion_EventScript_Rock:
	warp MAP_PARTY_BATH_ROCK, WARP_ID_NONE, 12, 4
	waitstate
	goto PartyBathExpansion_EventScript_End
PartyBathExpansion_EventScript_Waterfall:
	warp MAP_PARTY_BATH_WATERFALL, WARP_ID_NONE, 12, 4
	waitstate
	goto PartyBathExpansion_EventScript_End
PartyBathExpansion_EventScript_Sauna:
	warp MAP_PARTY_BATH_SAUNA, WARP_ID_NONE, 12, 4
	waitstate
	goto PartyBathExpansion_EventScript_End
PartyBathExpansion_EventScript_Deep:
	warp MAP_PARTY_BATH_DEEP, WARP_ID_NONE, 12, 4
	waitstate
	goto PartyBathExpansion_EventScript_End

PartyBathExpansion_EventScript_Dive::
	lockall
	msgbox PartyBathExpansion_Text_Dive, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PartyBathExpansion_EventScript_End
	closemessage
	warp MAP_PARTY_BATH_UNDERWATER, WARP_ID_NONE, 12, 4
	waitstate
	goto PartyBathExpansion_EventScript_End
PartyBathExpansion_EventScript_Surface::
	lockall
	warp MAP_PARTY_BATH_DEEP, WARP_ID_NONE, 12, 4
	waitstate
	goto PartyBathExpansion_EventScript_End

PartyBathExpansion_Text_CallSteven::
	.string "{JPN}ダイゴ『やあ。ボクだよ。\n"
	.string "あれから いい石を見つけてね。\p"
	.string "温めると 気持ちのいい石なんだ。\n"
	.string "浴場に 岩盤浴を作っておいたよ。\p"
	.string "石の上で 横になってごらん。\n"
	.string "きっと ポケモンも喜ぶ。』$"
PartyBathExpansion_Text_CallWallace::
	.string "{JPN}ミクリ『やあ 私だよ。\n"
	.string "浴場に 滝を作っておいた。\p"
	.string "流れ落ちる水は 美しい。\n"
	.string "眺めても 打たれても いいものだ。\p"
	.string "今度は きみの目で\n"
	.string "その美しさを 確かめてほしい。』$"
PartyBathExpansion_Text_CallMaxie::
	.string "{JPN}マツブサ『私だ。\n"
	.string "少々 浴場に手を加えた。\p"
	.string "熱と 蒸気で 身体を温める。\n"
	.string "サウナというものだ。\p"
	.string "大地を増やすよりは\n"
	.string "よほど 有意義な仕事だろう？』$"
PartyBathExpansion_Text_CallArchie::
	.string "{JPN}アオギリ『よう！ おれだ！\n"
	.string "でっけえ風呂を 掘ってやったぜ！\p"
	.string "潜れるくらい 深くしてある。\n"
	.string "これなら 思いっきり泳げるだろ！\p"
	.string "海ほど 広かねえが\n"
	.string "ポケモンも 楽しめるはずだぜ！』$"
PartyBathExpansion_Text_Completed::
	.string "{JPN}浴場に {STR_VAR_1}が加わった！\p"
	.string "入浴の 効果が高まった！\n"
	.string "5秒ごとに {STR_VAR_2}ずつ 上がる！$"
PartyBathExpansion_Text_Guide::
	.string "{JPN}浴場の フロア案内だ。\nどこへ 行きますか？$"
PartyBathExpansion_Text_Locked::
	.string "{JPN}このフロアは まだ使えない。\n誰かが 開発してくれるかもしれない。$"
PartyBathExpansion_Text_Dive::
	.string "{JPN}深い湯へ降りる 階段がある。\n下へ 降りてみますか？$"
PartyBathExpansion_Text_Rock:
	.string "{JPN}岩盤浴$"
PartyBathExpansion_Text_Waterfall:
	.string "{JPN}滝の湯$"
PartyBathExpansion_Text_Sauna:
	.string "{JPN}サウナ$"
PartyBathExpansion_Text_Deep:
	.string "{JPN}深いお風呂$"
PartyBathExpansion_Text_Cancel:
	.string "{JPN}やめる$"
