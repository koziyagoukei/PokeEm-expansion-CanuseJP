MysteryGiftScript_VisitingTrainer::
	setvaddress MysteryGiftScript_VisitingTrainer
	special ValidateEReaderTrainer
	vgoto_if_eq VAR_RESULT, 0, MysteryGiftScript_VisitingTrainerArrived
	lock
	faceplayer
	vmessage sText_MysteryGiftVisitingTrainerInstructions
	waitmessage
	waitbuttonpress
	release
	end

MysteryGiftScript_VisitingTrainerArrived:
	lock
	faceplayer
	vmessage sText_MysteryGiftVisitingTrainerArrived
	waitmessage
	waitbuttonpress
	release
	end

sText_MysteryGiftVisitingTrainerInstructions:
	.string "{JPN}ふしぎなおくりもの を ごりよう\nいただき ありがとう ございます\pこの ふしぎなカードを もっていると\nフレンドリーショップの アンケートで\pいろいろな トレーナーを ルネシティに\nよぶことが できますよ!\p……ないしょで ひとつ アンケートの\nあいことばを おしえて あげましょう\p「すごい トレーナー\n くれ くれ」\pこのことばを アンケートに かいて\nぜひ ジョイスポットと\lつうしんして みてください!$"

sText_MysteryGiftVisitingTrainerArrived:
	.string "{JPN}ふしぎなおくりもの を ごりよう\nいただき ありがとう ございます\pルネシティに トレーナーが\nきている ようですよ\pぜひ たいせんを\nたのしんで くださいませ!\pほかの あいことば でも\nべつの トレーナーが よべますので\pあいことばを いろいろと\nさがして みて ください$"
