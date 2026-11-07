CeladonCity_Hotel_Frlg_MapScripts::
	.byte 0

CeladonCity_Hotel_EventScript_Receptionist::
	msgbox CeladonCity_Hotel_Text_ThisHotelIsForPeople, MSGBOX_NPC
	end

CeladonCity_Hotel_EventScript_Beauty::
	msgbox CeladonCity_Hotel_Text_OnVacationWithBrotherAndBoyfriend, MSGBOX_NPC
	end

CeladonCity_Hotel_EventScript_BeautyBoyfriend::
	msgbox CeladonCity_Hotel_Text_WhyDidSheBringBrother, MSGBOX_NPC
	end

CeladonCity_Hotel_EventScript_BeautyBrother::
	msgbox CeladonCity_Hotel_Text_SisBroughtMeOnVacation, MSGBOX_NPC
	end

CeladonCity_Hotel_Text_ThisHotelIsForPeople::
	.string "{JPN}はあ ポケモン…?\n"
	.string "ああ…! ここは\l"
	.string "ひとが とまる りょかん ですじゃ\p"
	.string "あいにく へや いっぱい でな$"

CeladonCity_Hotel_Text_OnVacationWithBrotherAndBoyfriend::
	.string "{JPN}あたし! おとうと と かれし\n"
	.string "3にんで りょこうに きたの!\p"
	.string "タマムシ シティって\n"
	.string "とても きれいな まち なのね$"

CeladonCity_Hotel_Text_WhyDidSheBringBrother::
	.string "{JPN}なんで…? かのじょと\n"
	.string "ふたり きりの りょこうの はずが!$"

CeladonCity_Hotel_Text_SisBroughtMeOnVacation::
	.string "{JPN}わーい! りょこう だぞー!\n"
	.string "おねえちゃん だいすき!$"

