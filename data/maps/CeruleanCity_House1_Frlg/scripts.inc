CeruleanCity_House1_Frlg_MapScripts::
	.byte 0

CeruleanCity_House1_EventScript_BadgeGuy::
	lock
	faceplayer
	msgbox CeruleanCity_House1_Text_BadgesHaveAmazingSecrets
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeAnotherBadge::
	message CeruleanCity_House1_Text_DescribeWhichBadge
	waitmessage
	setvar VAR_0x8004, SCROLL_MULTI_BADGES
	special ShowScrollableMultichoice
	switch VAR_RESULT
	case 0, CeruleanCity_House1_EventScript_DescribeBoulderBadge
	case 1, CeruleanCity_House1_EventScript_DescribeCascadeBadge
	case 2, CeruleanCity_House1_EventScript_DescribeThunderBadge
	case 3, CeruleanCity_House1_EventScript_DescribeRainbowBadge
	case 4, CeruleanCity_House1_EventScript_DescribeSoulBadge
	case 5, CeruleanCity_House1_EventScript_DescribeMarshBadge
	case 6, CeruleanCity_House1_EventScript_DescribeVolcanoBadge
	case 7, CeruleanCity_House1_EventScript_DescribeEarthBadge
	case 8, CeruleanCity_House1_EventScript_StopDescribingBadges
	case 127, CeruleanCity_House1_EventScript_StopDescribingBadges
	end

CeruleanCity_House1_EventScript_DescribeBoulderBadge::
	msgbox CeruleanCity_House1_Text_AttackStatFlash
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeCascadeBadge::
	msgbox CeruleanCity_House1_Text_ObeyLv30Cut
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeThunderBadge::
	msgbox CeruleanCity_House1_Text_SpeedStatFly
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeRainbowBadge::
	msgbox CeruleanCity_House1_Text_ObeyLv50Strength
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeSoulBadge::
	msgbox CeruleanCity_House1_Text_DefenseStatSurf
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeMarshBadge::
	msgbox CeruleanCity_House1_Text_ObeyLv70RockSmash
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeVolcanoBadge::
	msgbox CeruleanCity_House1_Text_SpStatsWaterfall
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeEarthBadge::
	msgbox CeruleanCity_House1_Text_AllMonsWillObeyYou
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_StopDescribingBadges::
	msgbox CeruleanCity_House1_Text_ComeVisitAnytime
	release
	end

CeruleanCity_House1_Text_BadgesHaveAmazingSecrets::
	.string "{JPN}うでが あがった\n"
	.string "ポケモントレーナーだけが もらえる\l"
	.string "ポケモン バッジ\p"
	.string "あなたも いくつか もってるね\p"
	.string "その バッジには\n"
	.string "すごい ひみつが あるのです$"

CeruleanCity_House1_Text_DescribeWhichBadge::
	.string "{JPN}では…\p"
	.string "ポケモン バッジ 8つの ひみつ\n"
	.string "どれに ついて はなし ましょう?$"

CeruleanCity_House1_Text_ComeVisitAnytime::
	.string "{JPN}はなしを ききたく なったら\n"
	.string "また いらっしゃい$"

CeruleanCity_House1_Text_AttackStatFlash::
	.string "{JPN}すべての\n"
	.string "ポケモンの こうげき りょくが\l"
	.string "すこし たかく なります\p"
	.string "それから フラッシュの わざは\n"
	.string "たたかって いない ときでも\l"
	.string "つかえるように なります$"

CeruleanCity_House1_Text_ObeyLv30Cut::
	.string "{JPN}レベル30までの ポケモンは\n"
	.string "ひとから もらった ポケモンでも\l"
	.string "おとなしく いうことを ききます\p"
	.string "それいじょう レベルが たかいと\n"
	.string "うまく あつかえません\p"
	.string "それから いあいぎりの わざは\n"
	.string "たたかって いない ときでも\l"
	.string "つかえるように なります$"

CeruleanCity_House1_Text_SpeedStatFly::
	.string "{JPN}すべての\n"
	.string "ポケモンの すばやさが\l"
	.string "すこし たかく なります\p"
	.string "それから そらをとぶ わざは\n"
	.string "たたかって いない ときでも\l"
	.string "つかえるように なります$"

CeruleanCity_House1_Text_ObeyLv50Strength::
	.string "{JPN}レベル50までの ポケモンは\n"
	.string "ひとから もらった ポケモンでも\l"
	.string "おとなしく いうことを ききます\p"
	.string "それいじょう レベルが たかいと\n"
	.string "うまく あつかえません\p"
	.string "それから かいりきの わざは\n"
	.string "たたかって いない ときでも\l"
	.string "つかえるように なります$"

CeruleanCity_House1_Text_DefenseStatSurf::
	.string "{JPN}すべての\n"
	.string "ポケモンの ぼうぎょりょくが\l"
	.string "すこし たかく なります\p"
	.string "それから なみのりの わざは\n"
	.string "たたかって いない ときでも\l"
	.string "つかえるように なります$"

CeruleanCity_House1_Text_ObeyLv70RockSmash::
	.string "{JPN}レベル70までの ポケモンは\n"
	.string "ひとから もらった ポケモンでも\l"
	.string "おとなしく いうことを ききます\p"
	.string "それいじょう レベルが たかいと\n"
	.string "うまく あつかえません\p"
	.string "それから いわくだきの わざは\n"
	.string "たたかって いない ときでも\l"
	.string "つかえるように なります$"

CeruleanCity_House1_Text_SpStatsWaterfall::
	.string "{JPN}すべての\n"
	.string "ポケモンの とくこう とくぼうが\l"
	.string "すこし たかく なります\p"
	.string "それから たきのぼりの わざは\n"
	.string "たたかって いない ときでも\l"
	.string "つかえるように なります$"

CeruleanCity_House1_Text_AllMonsWillObeyYou::
	.string "{JPN}すべての ポケモンは\n"
	.string "おとなしく いうことを ききます$"

