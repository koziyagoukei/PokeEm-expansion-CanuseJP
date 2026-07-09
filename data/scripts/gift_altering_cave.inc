MysteryGiftScript_AlteringCave::
	setvaddress MysteryGiftScript_AlteringCave
	addvar VAR_ALTERING_CAVE_WILD_SET, 1
	vgoto_if_ne VAR_ALTERING_CAVE_WILD_SET, (NUM_ALTERING_CAVE_TABLES + 1), MysteryGiftScript_AlteringCave_
	setvar VAR_ALTERING_CAVE_WILD_SET, 0
MysteryGiftScript_AlteringCave_:
	lock
	faceplayer
	vmessage sText_MysteryGiftAlteringCave
	waitmessage
	waitbuttonpress
	release
	end

sText_MysteryGiftAlteringCave:
	.string "{JPN}ふしぎなおくりもの を ごりよう\nいただき ありがとう ございます!\pさいきん 103ばんどうろ にある\nへんげのどうくつ で\pめずらしい ポケモンが でる という\nうわさが ながれてる ようです\pぜひ いって たしかめて みては\nいかがで しょうか?$"
