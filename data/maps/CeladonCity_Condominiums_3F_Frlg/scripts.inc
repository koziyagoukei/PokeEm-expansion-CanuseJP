CeladonCity_Condominiums_3F_Frlg_MapScripts::
	.byte 0

CeladonCity_Condominiums_3F_EventScript_Programmer::
	msgbox CeladonCity_Condominiums_3F_Text_ImTheProgrammer, MSGBOX_NPC
	end

CeladonCity_Condominiums_3F_EventScript_GraphicArtist::
	msgbox CeladonCity_Condominiums_3F_Text_ImTheGraphicArtist, MSGBOX_NPC
	end

CeladonCity_Condominiums_3F_EventScript_Writer::
	msgbox CeladonCity_Condominiums_3F_Text_IWroteTheStory, MSGBOX_NPC
	end

CeladonCity_Condominiums_3F_EventScript_Designer::
	lock
	faceplayer
	specialvar VAR_RESULT, HasAllKantoMons
	goto_if_eq VAR_RESULT, TRUE, CeladonCity_Condominiums_3F_EventScript_CompletedPokedex
	msgbox CeladonCity_Condominiums_3F_Text_ImGameDesignerShowMeFinishedPokedex
	release
	end

CeladonCity_Condominiums_3F_EventScript_CompletedPokedex::
	goto CeladonCity_Condominiums_3F_EventScript_ShowDiploma
	end

CeladonCity_Condominiums_3F_EventScript_ShowDiploma::
	message CeladonCity_Condominiums_3F_Text_CompletedPokedexCongratulations
	waitmessage
	delay 60
	special Special_ShowDiploma
	release
	end

CeladonCity_Condominiums_3F_EventScript_DevelopmentRoomSign::
	msgbox CeladonCity_Condominiums_3F_Text_GameFreakDevelopmentRoom, MSGBOX_SIGN
	end

CeladonCity_Condominiums_3F_EventScript_Computer1::
	msgbox CeladonCity_Condominiums_3F_Text_ItsTheGameProgram, MSGBOX_SIGN
	end

CeladonCity_Condominiums_3F_EventScript_Computer2::
	msgbox CeladonCity_Condominiums_3F_Text_SomeonesPlayingGame, MSGBOX_SIGN
	end

CeladonCity_Condominiums_3F_EventScript_Computer3::
	msgbox CeladonCity_Condominiums_3F_Text_ItsScriptBetterNotLookAtEnding, MSGBOX_SIGN
	end

CeladonCity_Condominiums_3F_Text_ImTheProgrammer::
	.string "{JPN}おれ? プログラマー だよ!$"

CeladonCity_Condominiums_3F_Text_ImTheGraphicArtist::
	.string "{JPN}おれ グラフィッカー!\n"
	.string "おまえを かいたの おれさ!$"

CeladonCity_Condominiums_3F_Text_IWroteTheStory::
	.string "{JPN}ぼく シナリオ かいた!\n"
	.string "あの… エリカ かわいい でしょ\p"
	.string "カスミも いいよね!\p"
	.string "ナツメも いいよね!$"

CeladonCity_Condominiums_3F_Text_ImGameDesignerShowMeFinishedPokedex::
	.string "{JPN}そーですかー!\p"
	.string "ぼくは ゲーム デザイナー\p"
	.string "ポケモンずかん ぜんぶ あつめるの\n"
	.string "たいへん だけど がんばって!\l"
	.string "もし ぜんぶ あつめたら\p"
	.string "おしえてね!$"

CeladonCity_Condominiums_3F_Text_CompletedPokedexCongratulations::
	.string "{JPN}わあ すごい!\n"
	.string "ポケモンずかん ぜんぶの ページ\l"
	.string "とうとう あつめたんだね!\l"
	.string "おめでとう!\l"
	.string "……$"

CeladonCity_Condominiums_3F_Text_ItsTheGameProgram::
	.string "{JPN}ゲームの プログラムだ!\n"
	.string "これ かえたら バグる かも$"

CeladonCity_Condominiums_3F_Text_SomeonesPlayingGame::
	.string "{JPN}パソコンで しごと ちゅう…\n"
	.string "と おもったら ゲーム だった!$"

CeladonCity_Condominiums_3F_Text_ItsScriptBetterNotLookAtEnding::
	.string "{JPN}… ポケモンの シナリオだ!\n"
	.string "これは エンディングだ!\l"
	.string "いまは みないで おこう…!$"

CeladonCity_Condominiums_3F_Text_GameFreakDevelopmentRoom::
	.string "{JPN}ゲームフリーク かいはつ しつ$"

