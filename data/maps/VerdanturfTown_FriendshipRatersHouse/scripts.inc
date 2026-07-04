VerdanturfTown_FriendshipRatersHouse_MapScripts::
	.byte 0

VerdanturfTown_FriendshipRatersHouse_EventScript_FriendshipRater::
	lock
	faceplayer
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_SeeHowMuchPokemonLikesYou, MSGBOX_DEFAULT
	specialvar VAR_RESULT, GetLeadMonFriendshipScore
	switch VAR_RESULT
	case FRIENDSHIP_NONE, VerdanturfTown_FriendshipRatersHouse_EventScript_DetestsYou
	case FRIENDSHIP_1_TO_49, VerdanturfTown_FriendshipRatersHouse_EventScript_VeryWary
	case FRIENDSHIP_50_TO_99, VerdanturfTown_FriendshipRatersHouse_EventScript_NotUsedToYou
	case FRIENDSHIP_100_TO_149, VerdanturfTown_FriendshipRatersHouse_EventScript_GettingUsedToYou
	case FRIENDSHIP_150_TO_199, VerdanturfTown_FriendshipRatersHouse_EventScript_LikesYouQuiteALot
	case FRIENDSHIP_200_TO_254, VerdanturfTown_FriendshipRatersHouse_EventScript_VeryHappy
	case FRIENDSHIP_MAX, VerdanturfTown_FriendshipRatersHouse_EventScript_AdoresYou
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_DetestsYou::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_DetestsYou, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_VeryWary::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_VeryWary, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_NotUsedToYou::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_NotUsedToYou, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_GettingUsedToYou::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_GettingUsedToYou, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_LikesYouQuiteALot::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_LikesYouQuiteALot, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_VeryHappy::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_VeryHappy, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_AdoresYou::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_AdoresYou, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_Pikachu::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIKACHU, CRY_MODE_NORMAL
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_Pikachu, MSGBOX_DEFAULT
	waitmoncry
	release
	end

VerdanturfTown_FriendshipRatersHouse_Text_SeeHowMuchPokemonLikesYou:
	.string "{JPN}あなたの ポケモンが\nどれくらい なついているか みてあげる\pあら……\nあなたの ポケモン$"

VerdanturfTown_FriendshipRatersHouse_Text_AdoresYou:
	.string "{JPN}これいじょうは ない ってぐらい\nあなたに なついているわね\lみている こっちも しあわせに なるわ!$"

VerdanturfTown_FriendshipRatersHouse_Text_VeryHappy:
	.string "{JPN}なんだか しあわせそう\nすごく あなたに なついているわね$"

VerdanturfTown_FriendshipRatersHouse_Text_LikesYouQuiteALot:
	.string "{JPN}かなり なついているわね\nあなたに あまえたそうに しているわよ$"

VerdanturfTown_FriendshipRatersHouse_Text_GettingUsedToYou:
	.string "{JPN}ちょっと なついてきた って かんじ かな\nあなたのこと しんじている みたい$"

VerdanturfTown_FriendshipRatersHouse_Text_NotUsedToYou:
	.string "{JPN}まだ あなたに なれていない ようね\nふつうの かんけい って かんじが するわ$"

VerdanturfTown_FriendshipRatersHouse_Text_VeryWary:
	.string "{JPN}…… なんだか けいかい しているみたい\nめつきも ちょっと こわいし\lぜんぜん あなたに なついていないわ$"

VerdanturfTown_FriendshipRatersHouse_Text_DetestsYou:
	.string "{JPN}…… ちょっと いいづらいんだけど\pあなた じぶんの ポケモンに\nそんなに きらわれていて だいじょうぶ?$"

VerdanturfTown_FriendshipRatersHouse_Text_Pikachu:
	.string "{JPN}ピカチュウ『ピカピカッ!$"

