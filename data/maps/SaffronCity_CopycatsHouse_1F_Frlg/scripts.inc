SaffronCity_CopycatsHouse_1F_Frlg_MapScripts::
	.byte 0

SaffronCity_CopycatsHouse_1F_EventScript_CopycatsMom::
	msgbox SaffronCity_CopycatsHouse_1F_Text_DaughterIsSelfCentered, MSGBOX_NPC
	end

SaffronCity_CopycatsHouse_1F_EventScript_CopycatsDad::
	msgbox SaffronCity_CopycatsHouse_1F_Text_DaughterLikesToMimicPeople, MSGBOX_NPC
	end

SaffronCity_CopycatsHouse_1F_EventScript_Chansey::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_CHANSEY, CRY_MODE_NORMAL
	msgbox SaffronCity_CopycatsHouse_1F_Text_Chansey
	waitmoncry
	release
	end

SaffronCity_CopycatsHouse_1F_Text_DaughterIsSelfCentered::
	.string "{JPN}たくの むすめは\n"
	.string "ほんと わがままに そだって…\l"
	.string "おともだちも あまり おりませんの$"

SaffronCity_CopycatsHouse_1F_Text_DaughterLikesToMimicPeople::
	.string "{JPN}… こまった ことに むすめは\n"
	.string "ひとの まねが すきでな!\p"
	.string "しょっちゅう ひとを からかう ので\n"
	.string "きんじょ じゃ\l"
	.string "モノマネ むすめと よばれとる!$"

SaffronCity_CopycatsHouse_1F_Text_Chansey::
	.string "{JPN}ラッキー『ラッ きぃ!$"

