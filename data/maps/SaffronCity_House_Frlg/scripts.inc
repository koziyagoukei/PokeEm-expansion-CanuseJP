SaffronCity_House_Frlg_MapScripts::
	.byte 0

SaffronCity_House_EventScript_Lass::
	msgbox SaffronCity_House_Text_DontLookAtMyLetter, MSGBOX_NPC
	end

SaffronCity_House_EventScript_Youngster::
	msgbox SaffronCity_House_Text_GettingCopycatPokeDoll, MSGBOX_NPC
	end

SaffronCity_House_EventScript_Pidgey::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIDGEY, CRY_MODE_NORMAL
	msgbox SaffronCity_House_Text_Pidgey
	waitmoncry
	release
	end

SaffronCity_House_EventScript_Letter::
	msgbox SaffronCity_House_Text_ExplainPPUp, MSGBOX_SIGN
	end

SaffronCity_House_Text_DontLookAtMyLetter::
	.string "{JPN}… おてがみ ありがとう!\n"
	.string "はやく あなたに あいたいな…\p"
	.string "いやーん!\n"
	.string "てがみ かいてんだから みないで!$"

SaffronCity_House_Text_Pidgey::
	.string "{JPN}ポッポ『クルックー$"

SaffronCity_House_Text_GettingCopycatPokeDoll::
	.string "{JPN}モノマネ むすめって かわいい!\n"
	.string "かのじょに ピッピにんぎょう\l"
	.string "プレゼント するんだ!$"

SaffronCity_House_Text_ExplainPPUp::
	.string "{JPN}… この あいだ\n"
	.string "ポイント アップ もらったの\p"
	.string "すごく つよい わざって\n"
	.string "パワー ポイント すくないけど\l"
	.string "これで ふやせば いいのね…?$"

