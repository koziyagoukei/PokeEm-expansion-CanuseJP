SaffronCity_CopycatsHouse_2F_Frlg_MapScripts::
	.byte 0

SaffronCity_CopycatsHouse_2F_EventScript_Doduo::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_DODUO, CRY_MODE_NORMAL
	msgbox SaffronCity_CopycatsHouse_2F_Text_Doduo
	waitmoncry
	release
	end

SaffronCity_CopycatsHouse_2F_EventScript_Doll::
	lock
	msgbox SaffronCity_CopycatsHouse_2F_Text_RareMonOnlyDoll
	release
	end

SaffronCity_CopycatsHouse_2F_EventScript_Copycat::
	lock
	faceplayer
	checkitem ITEM_POKE_DOLL
	goto_if_eq VAR_RESULT, TRUE, EventScript_MimicTutor
	checkplayergender
	call_if_eq VAR_RESULT, MALE, SaffronCity_CopycatsHouse_2F_EventScript_MimicPlayerMale
	call_if_eq VAR_RESULT, FEMALE, SaffronCity_CopycatsHouse_2F_EventScript_MimicPlayerFemale
	release
	end

SaffronCity_CopycatsHouse_2F_EventScript_MimicPlayerMale::
	msgbox SaffronCity_CopycatsHouse_2F_Text_MimickingFavoriteHobbyMale
	return

SaffronCity_CopycatsHouse_2F_EventScript_MimicPlayerFemale::
	msgbox SaffronCity_CopycatsHouse_2F_Text_MimickingFavoriteHobbyFemale
	return

SaffronCity_CopycatsHouse_2F_EventScript_Computer::
	msgbox SaffronCity_CopycatsHouse_2F_Text_MySecrets, MSGBOX_SIGN
	end

SaffronCity_CopycatsHouse_2F_EventScript_Game::
	msgbox SaffronCity_CopycatsHouse_2F_Text_MarioWearingABucket, MSGBOX_SIGN
	end

SaffronCity_CopycatsHouse_2F_Text_MimickingFavoriteHobbyMale::
	.string "{JPN}{PLAYER}『やあ! こんにちは!\n"
	.string "きみ ポケモン すきかい?\p"
	.string "{PLAYER}『ぼく じゃなくって\n"
	.string "きみに きいてるんだ けど\p"
	.string "{PLAYER}『… えー なんだよ!\n"
	.string "ヘンな やつ だなあ!\p"
	.string "モノマネむすめ『… なに?\n"
	.string "ひとの マネ すんなって?\p"
	.string "だって あたし \n"
	.string "ものまね しゅみ なんだ もん!$"

SaffronCity_CopycatsHouse_2F_Text_MimickingFavoriteHobbyFemale::
	.string "{JPN}{PLAYER}『こんにちは!\n"
	.string "ポケモン すきなのね?\p"
	.string "{PLAYER}『わたし じゃなくって\n"
	.string "あなたに きいてるんだ けど\p"
	.string "{PLAYER}『… えー なんなの!\n"
	.string "おかしなこ ねー!\p"
	.string "モノマネむすめ『… なに?\n"
	.string "ひとの マネ すんなって?\p"
	.string "だって あたし\n"
	.string "ものまね しゅみ なんだ もん!$"

Text_MimicTeach::
	.string "{JPN}わあー!\n"
	.string "ピッピにんぎょう くれるのー!\p"
	.string "うれしーい!\p"
	.string "… じゃあねー!\n"
	.string "わたし からは ものまねの わざ\l"
	.string "おしえて あげるー!$"

Text_MimicDeclined::
	.string "{JPN}ものまね… キライ?$"

Text_MimicWhichMon::
	.string "{JPN}わたしの だいすきな ものまね!\n"
	.string "だれが おぼえて くれるの?$"

Text_MimicTaughtMale::
	.string "{JPN}{PLAYER}『やあ! さっきは\n"
	.string " ありがとう!\p"
	.string "{PLAYER}『… なーに?\p"
	.string "{PLAYER}『ぼくの まね して\n"
	.string "そんなに たのしい かい?\p"
	.string "モノマネむすめ『うん…!\n"
	.string "とっても たのしいー!$"

Text_MimicTaughtFemale::
	.string "{JPN}{PLAYER}『あ! さっきは\n"
	.string " ありがとう!\p"
	.string "{PLAYER}『… なーに?\p"
	.string "{PLAYER}『わたしの まね して\n"
	.string "そんなに たのしい?\p"
	.string "モノマネむすめ『うん…!\n"
	.string "とっても たのしいー!$"

SaffronCity_CopycatsHouse_2F_Text_Doduo::
	.string "{JPN}ドードー『ギー ギーッ!\p"
	.string "… カガミ ヨ カガミ!\n"
	.string "セカイデ イチバン カワイイ\l"
	.string "オンナノコハ ダーレ?$"

SaffronCity_CopycatsHouse_2F_Text_RareMonOnlyDoll::
	.string "{JPN}これは めずらしい ポケモン!?\n"
	.string "… あれ? ぬいぐるみ だった$"

SaffronCity_CopycatsHouse_2F_Text_MarioWearingABucket::
	.string "{JPN}これは…!\n"
	.string "マリオが バケツを かぶって\l"
	.string "あるいて いく ゲームだ!$"

SaffronCity_CopycatsHouse_2F_Text_MySecrets::
	.string "{JPN}… … …\p"
	.string "わたしの ひみつ…!\p"
	.string "とくいなもの … ものまね!\n"
	.string "しゅみ … ぬいぐるみ!\l"
	.string "すきなもの … ピッピ!\l"
	.string "… … …$"

