const struct AbilityInfo gAbilitiesInfo[ABILITIES_COUNT] =
{
    [ABILITY_NONE] =
    {
        .name = _("{JPN}-------"),
        .description = COMPOUND_STRING("とくせい なし"),
        .aiRating = 0,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_STENCH] =
    {
        .name = _("{JPN}あくしゅう"),
        .description = COMPOUND_STRING("ポケモンが よりつき にくくなる"),
        .aiRating = 1,
    },

    [ABILITY_DRIZZLE] =
    {
        .name = _("{JPN}あめふらし"),
        .description = COMPOUND_STRING("せんとうに でると あめを ふらす"),
        .aiRating = 9,
    },

    [ABILITY_SPEED_BOOST] =
    {
        .name = _("{JPN}かそく"),
        .description = COMPOUND_STRING("ちょっとずつ すばやく なっていく"),
        .aiRating = 9,
    },

    [ABILITY_BATTLE_ARMOR] =
    {
        .name = _("{JPN}カブトアーマー"),
        .description = COMPOUND_STRING("わざを きゅうしょに うけない"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_STURDY] =
    {
        .name = _("{JPN}がんじょう"),
        .description = COMPOUND_STRING("いちげきで たおされない"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_DAMP] =
    {
        .name = _("{JPN}しめりけ"),
        .description = COMPOUND_STRING("だれも じばくが できなくなる"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_LIMBER] =
    {
        .name = _("{JPN}じゅうなん"),
        .description = COMPOUND_STRING("まひ じょうたいに ならない"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SAND_VEIL] =
    {
        .name = _("{JPN}すながくれ"),
        .description = COMPOUND_STRING("すなあらしで かいひりつが あがる"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_STATIC] =
    {
        .name = _("{JPN}せいでんき"),
        .description = COMPOUND_STRING("さわった あいてを まひさせる"),
        .aiRating = 4,
    },

    [ABILITY_VOLT_ABSORB] =
    {
        .name = _("{JPN}ちくでん"),
        .description = COMPOUND_STRING("でんきを うけると かいふくする"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_WATER_ABSORB] =
    {
        .name = _("{JPN}ちょすい"),
        .description = COMPOUND_STRING("みずを うけると かいふくする"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_OBLIVIOUS] =
    {
        .name = _("{JPN}どんかん"),
        .description = COMPOUND_STRING("メロメロ じょうたいに ならない"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_CLOUD_NINE] =
    {
        .name = _("{JPN}ノーてんき"),
        .description = COMPOUND_STRING("てんきの えいきょうが なくなる"),
        .aiRating = 5,
    },

    [ABILITY_COMPOUND_EYES] =
    {
        .name = _("{JPN}ふくがん"),
        .description = COMPOUND_STRING("わざの めいちゅうりつが あがる"),
        .aiRating = 7,
    },

    [ABILITY_INSOMNIA] =
    {
        .name = _("{JPN}ふみん"),
        .description = COMPOUND_STRING("ねむり じょうたいに ならない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_COLOR_CHANGE] =
    {
        .name = _("{JPN}へんしょく"),
        .description = COMPOUND_STRING("うけた わざの タイプに へんかする"),
        .aiRating = 2,
    },

    [ABILITY_IMMUNITY] =
    {
        .name = _("{JPN}めんえき"),
        .description = COMPOUND_STRING("どく じょうたいに ならない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_FLASH_FIRE] =
    {
        .name = _("{JPN}もらいび"),
        .description = COMPOUND_STRING("ほのおを うけると つよくなる"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SHIELD_DUST] =
    {
        .name = _("{JPN}りんぷん"),
        .description = COMPOUND_STRING("わざの ついかこうかを うけない"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_OWN_TEMPO] =
    {
        .name = _("{JPN}マイペース"),
        .description = COMPOUND_STRING("こんらん じょうたいに ならない"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SUCTION_CUPS] =
    {
        .name = _("{JPN}きゅうばん"),
        .description = COMPOUND_STRING("きゅうばんで じめんに はりつく"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_INTIMIDATE] =
    {
        .name = _("{JPN}いかく"),
        .description = COMPOUND_STRING("あいての こうげきを さげる"),
        .aiRating = 7,
    },

    [ABILITY_SHADOW_TAG] =
    {
        .name = _("{JPN}かげふみ"),
        .description = COMPOUND_STRING("あいてを にげられなくする"),
        .aiRating = 10,
    },

    [ABILITY_ROUGH_SKIN] =
    {
        .name = _("{JPN}さめはだ"),
        .description = COMPOUND_STRING("さわった あいてを キズつける"),
        .aiRating = 6,
    },

    [ABILITY_WONDER_GUARD] =
    {
        .name = _("{JPN}ふしぎなまもり"),
        .description = COMPOUND_STRING("こうか ばつぐん しか あたらない"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .breakable = TRUE,
    },

    [ABILITY_LEVITATE] =
    {
        .name = _("{JPN}ふゆう"),
        .description = COMPOUND_STRING("じめんタイプの わざを うけない"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_EFFECT_SPORE] =
    {
        .name = _("{JPN}ほうし"),
        .description = COMPOUND_STRING("さわった あいてに ほうしがつく"),
        .aiRating = 4,
    },

    [ABILITY_SYNCHRONIZE] =
    {
        .name = _("{JPN}シンクロ"),
        .description = COMPOUND_STRING("どく まひ やけどを あいてにうつす"),
        .aiRating = 4,
    },

    [ABILITY_CLEAR_BODY] =
    {
        .name = _("{JPN}クリアボディ"),
        .description = COMPOUND_STRING("あいてに のうりょくを さげられない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_NATURAL_CURE] =
    {
        .name = _("{JPN}しぜんかいふく"),
        .description = COMPOUND_STRING("ひっこむと いじょうが なおる"),
        .aiRating = 7,
    },

    [ABILITY_LIGHTNING_ROD] =
    {
        .name = _("{JPN}ひらいしん"),
        .description = COMPOUND_STRING("でんきタイプの わざが じぶんにくる"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_SERENE_GRACE] =
    {
        .name = _("{JPN}てんのめぐみ"),
        .description = COMPOUND_STRING("わざの ついかこうかが でやすい"),
        .aiRating = 8,
    },

    [ABILITY_SWIFT_SWIM] =
    {
        .name = _("{JPN}すいすい"),
        .description = COMPOUND_STRING("あめのとき すばやさが はやくなる"),
        .aiRating = 6,
    },

    [ABILITY_CHLOROPHYLL] =
    {
        .name = _("{JPN}ようりょくそ"),
        .description = COMPOUND_STRING("はれのとき すばやさが はやくなる"),
        .aiRating = 6,
    },

    [ABILITY_ILLUMINATE] =
    {
        .name = _("{JPN}はっこう"),
        .description = COMPOUND_STRING("ポケモンに そうぐう しやすくなる"),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_TRACE] =
    {
        .name = _("{JPN}トレース"),
        .description = COMPOUND_STRING("あいてと おなじ とくせいになる"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE, //B_UPDATED_ABILITY_DATA >= GEN_4
    },

    [ABILITY_HUGE_POWER] =
    {
        .name = _("{JPN}ちからもち"),
        .description = COMPOUND_STRING("こうげきりょくが たかい"),
        .aiRating = 10,
    },

    [ABILITY_POISON_POINT] =
    {
        .name = _("{JPN}どくのトゲ"),
        .description = COMPOUND_STRING("さわったあいてに どくをあたえる"),
        .aiRating = 4,
    },

    [ABILITY_INNER_FOCUS] =
    {
        .name = _("{JPN}せいしんりょく"),
        .description = COMPOUND_STRING("ひるまない"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MAGMA_ARMOR] =
    {
        .name = _("{JPN}マグマのよろい"),
        .description = COMPOUND_STRING("こおり じょうたいに ならない"),
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_WATER_VEIL] =
    {
        .name = _("{JPN}みずのベール"),
        .description = COMPOUND_STRING("やけど じょうたいに ならない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_MAGNET_PULL] =
    {
        .name = _("{JPN}じりょく"),
        .description = COMPOUND_STRING("はがねタイプが にげられなくなる"),
        .aiRating = 9,
    },

    [ABILITY_SOUNDPROOF] =
    {
        .name = _("{JPN}ぼうおん"),
        .description = COMPOUND_STRING("おとの わざを うけない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_RAIN_DISH] =
    {
        .name = _("{JPN}あめうけざら"),
        .description = COMPOUND_STRING("あめのとき すこしずつ かいふく"),
        .aiRating = 3,
    },

    [ABILITY_SAND_STREAM] =
    {
        .name = _("{JPN}すなおこし"),
        .description = COMPOUND_STRING("せんとうで すなあらしを おこす"),
        .aiRating = 9,
    },

    [ABILITY_PRESSURE] =
    {
        .name = _("{JPN}プレッシャー"),
        .description = COMPOUND_STRING("あいての PPが へりやすくなる"),
        .aiRating = 5,
    },

    [ABILITY_THICK_FAT] =
    {
        .name = _("{JPN}あついしぼう"),
        .description = COMPOUND_STRING("ほのお こおりに つよい"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_EARLY_BIRD] =
    {
        .name = _("{JPN}はやおき"),
        .description = COMPOUND_STRING("おきるのが はやくなる"),
        .aiRating = 4,
    },

    [ABILITY_FLAME_BODY] =
    {
        .name = _("{JPN}ほのおのからだ"),
        .description = COMPOUND_STRING("さわった あいてを やけど させる"),
        .aiRating = 4,
    },

    [ABILITY_RUN_AWAY] =
    {
        .name = _("{JPN}にげあし"),
        .description = COMPOUND_STRING("せんとうから にげやすい"),
        .aiRating = 0,
    },

    [ABILITY_KEEN_EYE] =
    {
        .name = _("{JPN}するどいめ"),
        .description = COMPOUND_STRING("あいてに めいちゅうを さげられない"),
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_HYPER_CUTTER] =
    {
        .name = _("{JPN}かいりきバサミ"),
        .description = COMPOUND_STRING("あいてに こうげきを さげられない"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_PICKUP] =
    {
        .name = _("{JPN}ものひろい"),
        .description = COMPOUND_STRING("どうぐを ひろってくる ことがある"),
        .aiRating = 1,
    },

    [ABILITY_TRUANT] =
    {
        .name = _("{JPN}なまけ"),
        .description = COMPOUND_STRING("こうげきが れんぞくで だせない"),
        .aiRating = -2,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_HUSTLE] =
    {
        .name = _("{JPN}はりきり"),
        .description = COMPOUND_STRING("こうげきは たかいが はずれやすい"),
        .aiRating = 7,
    },

    [ABILITY_CUTE_CHARM] =
    {
        .name = _("{JPN}メロメロボディ"),
        .description = COMPOUND_STRING("さわった あいてを メロメロにする"),
        .aiRating = 2,
    },

    [ABILITY_PLUS] =
    {
        .name = _("{JPN}プラス"),
        .description = COMPOUND_STRING("マイナスが いると つよくなる"),
        .aiRating = 0,
    },

    [ABILITY_MINUS] =
    {
        .name = _("{JPN}マイナス"),
        .description = COMPOUND_STRING("プラスが いると つよくなる"),
        .aiRating = 0,
    },

    [ABILITY_FORECAST] =
    {
        .name = _("{JPN}てんきや"),
        .description = COMPOUND_STRING("てんきで ポワルンが へんかする"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeTraced = B_UPDATED_ABILITY_DATA >= GEN_4,
        .failsOnImposter = B_UPDATED_ABILITY_DATA >= GEN_5,
    },

    [ABILITY_STICKY_HOLD] =
    {
        .name = _("{JPN}ねんちゃく"),
        .description = COMPOUND_STRING("どうぐを とられない"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SHED_SKIN] =
    {
        .name = _("{JPN}だっぴ"),
        .description = COMPOUND_STRING("だっぴして からだを なおす"),
        .aiRating = 7,
    },

    [ABILITY_GUTS] =
    {
        .name = _("{JPN}こんじょう"),
        .description = COMPOUND_STRING("いじょうのとき こうげきが あがる"),
        .aiRating = 6,
    },

    [ABILITY_MARVEL_SCALE] =
    {
        .name = _("{JPN}ふしぎなうろこ"),
        .description = COMPOUND_STRING("いじょうのとき ぼうぎょが あがる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_LIQUID_OOZE] =
    {
        .name = _("{JPN}ヘドロえき"),
        .description = COMPOUND_STRING("すいとると たいりょくが へる"),
        .aiRating = 3,
    },

    [ABILITY_OVERGROW] =
    {
        .name = _("{JPN}しんりょく"),
        .description = COMPOUND_STRING("ピンチに くさの いりょくが あがる"),
        .aiRating = 5,
    },

    [ABILITY_BLAZE] =
    {
        .name = _("{JPN}もうか"),
        .description = COMPOUND_STRING("ピンチに ほのおが つよくなる"),
        .aiRating = 5,
    },

    [ABILITY_TORRENT] =
    {
        .name = _("{JPN}げきりゅう"),
        .description = COMPOUND_STRING("ピンチに みずの いりょくが あがる"),
        .aiRating = 5,
    },

    [ABILITY_SWARM] =
    {
        .name = _("{JPN}むしのしらせ"),
        .description = COMPOUND_STRING("ピンチに むしの いりょくが あがる"),
        .aiRating = 5,
    },

    [ABILITY_ROCK_HEAD] =
    {
        .name = _("{JPN}いしあたま"),
        .description = COMPOUND_STRING("ぶつかっても はんどうを うけない"),
        .aiRating = 5,
    },

    [ABILITY_DROUGHT] =
    {
        .name = _("{JPN}ひでり"),
        .description = COMPOUND_STRING("せんとうで ひざしを つよくする"),
        .aiRating = 9,
    },

    [ABILITY_ARENA_TRAP] =
    {
        .name = _("{JPN}ありじごく"),
        .description = COMPOUND_STRING("あいてを にげられなくする"),
        .aiRating = 9,
    },

    [ABILITY_VITAL_SPIRIT] =
    {
        .name = _("{JPN}やるき"),
        .description = COMPOUND_STRING("ねむらない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_WHITE_SMOKE] =
    {
        .name = _("{JPN}しろいけむり"),
        .description = COMPOUND_STRING("あいてに のうりょくを さげられない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_PURE_POWER] =
    {
        .name = _("{JPN}ヨガパワー"),
        .description = COMPOUND_STRING("こうげきりょくが たかい"),
        .aiRating = 10,
    },

    [ABILITY_SHELL_ARMOR] =
    {
        .name = _("{JPN}シェルアーマー"),
        .description = COMPOUND_STRING("わざを きゅうしょに うけない"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_AIR_LOCK] =
    {
        .name = _("{JPN}エアロック"),
        .description = COMPOUND_STRING("てんきの えいきょうが なくなる"),
        .aiRating = 5,
    },

    [ABILITY_TANGLED_FEET] =
    {
        .name = _("{JPN}ちどりあし"),
        .description = COMPOUND_STRING("こんらんのとき かいひりつが あがる"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MOTOR_DRIVE] =
    {
        .name = _("{JPN}でんきエンジン"),
        .description = COMPOUND_STRING("でんきを うけると すばやさが あがる"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_RIVALRY] =
    {
        .name = _("{JPN}とうそうしん"),
        .description = COMPOUND_STRING("せいべつが おなじだと つよくなる"),
        .aiRating = 1,
    },

    [ABILITY_STEADFAST] =
    {
        .name = _("{JPN}ふくつのこころ"),
        .description = COMPOUND_STRING("ひるむ たびに すばやさが あがる"),
        .aiRating = 2,
    },

    [ABILITY_SNOW_CLOAK] =
    {
        .name = _("{JPN}ゆきがくれ"),
        .description = COMPOUND_STRING("あられのとき かいひりつが あがる"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_GLUTTONY] =
    {
        .name = _("{JPN}くいしんぼう"),
        .description = COMPOUND_STRING("きのみを いつもより はやく つかう"),
        .aiRating = 3,
    },

    [ABILITY_ANGER_POINT] =
    {
        .name = _("{JPN}いかりのつぼ"),
        .description = COMPOUND_STRING("きゅうしょを うけると こうげきがあがる"),
        .aiRating = 4,
    },

    [ABILITY_UNBURDEN] =
    {
        .name = _("{JPN}かるわざ"),
        .description = COMPOUND_STRING("どうぐが なくなると すばやさが あがる"),
        .aiRating = 7,
    },

    [ABILITY_HEATPROOF] =
    {
        .name = _("{JPN}たいねつ"),
        .description = COMPOUND_STRING("ほのおと やけどの ダメージを へらす"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SIMPLE] =
    {
        .name = _("{JPN}たんじゅん"),
        .description = COMPOUND_STRING("のうりょく へんかが ばいになる"),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_DRY_SKIN] =
    {
        .name = _("{JPN}かんそうはだ"),
        .description = COMPOUND_STRING("あつさに よわいが みずで かいふく"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_DOWNLOAD] =
    {
        .name = _("{JPN}ダウンロード"),
        .description = COMPOUND_STRING("あいて によって のうりょくを かえる"),
        .aiRating = 7,
    },

    [ABILITY_IRON_FIST] =
    {
        .name = _("{JPN}てつのこぶし"),
        .description = COMPOUND_STRING("パンチの わざの いりょくが あがる"),
        .aiRating = 6,
    },

    [ABILITY_POISON_HEAL] =
    {
        .name = _("{JPN}ポイズンヒール"),
        .description = COMPOUND_STRING("どくで HPを かいふくする"),
        .aiRating = 8,
    },

    [ABILITY_ADAPTABILITY] =
    {
        .name = _("{JPN}てきおうりょく"),
        .description = COMPOUND_STRING("じぶんの タイプの いりょくが あがる"),
        .aiRating = 8,
    },

    [ABILITY_SKILL_LINK] =
    {
        .name = _("{JPN}スキルリンク"),
        .description = COMPOUND_STRING("れんぞくわざを さいだいまで だせる"),
        .aiRating = 7,
    },

    [ABILITY_HYDRATION] =
    {
        .name = _("{JPN}うるおいボディ"),
        .description = COMPOUND_STRING("あめのとき じょうたいいじょうを なおす"),
        .aiRating = 4,
    },

    [ABILITY_SOLAR_POWER] =
    {
        .name = _("{JPN}サンパワー"),
        .description = COMPOUND_STRING("はれで HPがへるが とくこうがあがる"),
        .aiRating = 3,
    },

    [ABILITY_QUICK_FEET] =
    {
        .name = _("{JPN}はやあし"),
        .description = COMPOUND_STRING("じょうたいいじょうで すばやさが あがる"),
        .aiRating = 5,
    },

    [ABILITY_NORMALIZE] =
    {
        .name = _("{JPN}ノーマルスキン"),
        .description = COMPOUND_STRING("わざが すべて ノーマルタイプに なる"),
        .aiRating = -1,
    },

    [ABILITY_SNIPER] =
    {
        .name = _("{JPN}スナイパー"),
        .description = COMPOUND_STRING("きゅうしょで あたえるダメージが ふえる"),
        .aiRating = 3,
    },

    [ABILITY_MAGIC_GUARD] =
    {
        .name = _("{JPN}マジックガード"),
        .description = COMPOUND_STRING("こうげき いがいで ダメージを うけない"),
        .aiRating = 9,
    },

    [ABILITY_NO_GUARD] =
    {
        .name = _("{JPN}ノーガード"),
        .description = COMPOUND_STRING("おたがいの わざが かならず あたる"),
        .aiRating = 8,
    },

    [ABILITY_STALL] =
    {
        .name = _("{JPN}あとだし"),
        .description = COMPOUND_STRING("あいてより こうどうが おそくなる"),
        .aiRating = -1,
    },

    [ABILITY_TECHNICIAN] =
    {
        .name = _("{JPN}テクニシャン"),
        .description = COMPOUND_STRING("よわい わざの いりょくが あがる"),
        .aiRating = 8,
    },

    [ABILITY_LEAF_GUARD] =
    {
        .name = _("{JPN}リーフガード"),
        .description = COMPOUND_STRING("はれで じょうたいいじょうを ふせぐ"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_KLUTZ] =
    {
        .name = _("{JPN}ぶきよう"),
        .description = COMPOUND_STRING("もっている どうぐを つかえない"),
        .aiRating = -1,
    },

    [ABILITY_MOLD_BREAKER] =
    {
        .name = _("{JPN}かたやぶり"),
        .description = COMPOUND_STRING("あいての とくせいを むしする"),
        .aiRating = 7,
    },

    [ABILITY_SUPER_LUCK] =
    {
        .name = _("{JPN}きょううん"),
        .description = COMPOUND_STRING("わざが きゅうしょに あたりやすくなる"),
        .aiRating = 3,
    },

    [ABILITY_AFTERMATH] =
    {
        .name = _("{JPN}ゆうばく"),
        .description = COMPOUND_STRING("たおされると ふれた あいてに ダメージ"),
        .aiRating = 5,
    },

    [ABILITY_ANTICIPATION] =
    {
        .name = _("{JPN}きけんよち"),
        .description = COMPOUND_STRING("きけんな わざを さっちする"),
        .aiRating = 2,
    },

    [ABILITY_FOREWARN] =
    {
        .name = _("{JPN}よちむ"),
        .description = COMPOUND_STRING("あいての わざを ひとつ よみとる"),
        .aiRating = 2,
    },

    [ABILITY_UNAWARE] =
    {
        .name = _("{JPN}てんねん"),
        .description = COMPOUND_STRING("あいての のうりょく へんかを むしする"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_TINTED_LENS] =
    {
        .name = _("{JPN}いろめがね"),
        .description = COMPOUND_STRING("いまひとつで あたえる ダメージがふえる"),
        .aiRating = 7,
    },

    [ABILITY_FILTER] =
    {
        .name = _("{JPN}フィルター"),
        .description = COMPOUND_STRING("ばつぐんで うける ダメージが へる"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SLOW_START] =
    {
        .name = _("{JPN}スロースタート"),
        .description = COMPOUND_STRING("しばらく こうげきと すばやさが さがる"),
        .aiRating = -2,
    },

    [ABILITY_SCRAPPY] =
    {
        .name = _("{JPN}きもったま"),
        .description = COMPOUND_STRING("ゴーストタイプに わざが あたる"),
        .aiRating = 6,
    },

    [ABILITY_STORM_DRAIN] =
    {
        .name = _("{JPN}よびみず"),
        .description = COMPOUND_STRING("みずタイプの わざが じぶんにくる"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_ICE_BODY] =
    {
        .name = _("{JPN}アイスボディ"),
        .description = COMPOUND_STRING("あられのとき まいターン HPをかいふく"),
        .aiRating = 3,
    },

    [ABILITY_SOLID_ROCK] =
    {
        .name = _("{JPN}ハードロック"),
        .description = COMPOUND_STRING("ばつぐんで うける ダメージが へる"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SNOW_WARNING] =
    {
        .name = _("{JPN}ゆきふらし"),
    #if B_SNOW_WARNING >= GEN_9
        .description = COMPOUND_STRING("せんとうに でると ゆきを ふらす"),
    #else
        .description = COMPOUND_STRING("せんとうに でると あられを ふらす"),
    #endif
        .aiRating = 8,
    },

    [ABILITY_HONEY_GATHER] =
    {
        .name = _("{JPN}みつあつめ"),
        .description = COMPOUND_STRING("あまいミツを ひろってくる ことがある"),
        .aiRating = 0,
    },

    [ABILITY_FRISK] =
    {
        .name = _("{JPN}おみとおし"),
        .description = COMPOUND_STRING("あいての もちものを みとおす"),
        .aiRating = 3,
    },

    [ABILITY_RECKLESS] =
    {
        .name = _("{JPN}すてみ"),
        .description = COMPOUND_STRING("はんどうを うけるわざが つよくなる"),
        .aiRating = 6,
    },

    [ABILITY_MULTITYPE] =
    {
        .name = _("{JPN}マルチタイプ"),
        .description = COMPOUND_STRING("もっている プレートで タイプが かわる"),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = B_UPDATED_ABILITY_DATA >= GEN_5,
    },

    [ABILITY_FLOWER_GIFT] =
    {
        .name = _("{JPN}フラワーギフト"),
        .description = COMPOUND_STRING("はれのとき みかたがわが つよくなる"),
        .aiRating = 4,
        .cantBeCopied = TRUE,
        .cantBeTraced = B_UPDATED_ABILITY_DATA >= GEN_5,
        .breakable = TRUE,
    },

    [ABILITY_BAD_DREAMS] =
    {
        .name = _("{JPN}ナイトメア"),
        .description = COMPOUND_STRING("ねむっている あいてに ダメージ"),
        .aiRating = 4,
    },

    [ABILITY_PICKPOCKET] =
    {
        .name = _("{JPN}わるいてぐせ"),
        .description = COMPOUND_STRING("さわられると あいての どうぐを ぬすむ"),
        .aiRating = 3,
    },

    [ABILITY_SHEER_FORCE] =
    {
        .name = _("{JPN}ちからずく"),
        .description = COMPOUND_STRING("ついかこうかを なくして わざをつよめる"),
        .aiRating = 8,
    },

    [ABILITY_CONTRARY] =
    {
        .name = _("{JPN}あまのじゃく"),
        .description = COMPOUND_STRING("のうりょく へんかが ぎゃくになる"),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_UNNERVE] =
    {
        .name = _("{JPN}きんちょうかん"),
        .description = COMPOUND_STRING("あいてが きのみを たべられなくなる"),
        .aiRating = 3,
    },

    [ABILITY_DEFIANT] =
    {
        .name = _("{JPN}まけんき"),
        .description = COMPOUND_STRING("のうりょくが さがると こうげきをあげる"),
        .aiRating = 5,
    },

    [ABILITY_DEFEATIST] =
    {
        .name = _("{JPN}よわき"),
        .description = COMPOUND_STRING("ピンチで こうげき とくこうがさがる"),
        .aiRating = -1,
    },

    [ABILITY_CURSED_BODY] =
    {
        .name = _("{JPN}のろわれボディ"),
        .description = COMPOUND_STRING("うけた わざを ふうじる ことがある"),
        .aiRating = 4,
    },

    [ABILITY_HEALER] =
    {
        .name = _("{JPN}いやしのこころ"),
        .description = COMPOUND_STRING("みかたの じょうたい いじょうを なおす"),
        .aiRating = 0,
    },

    [ABILITY_FRIEND_GUARD] =
    {
        .name = _("{JPN}フレンドガード"),
        .description = COMPOUND_STRING("みかたの うける ダメージを へらす"),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_WEAK_ARMOR] =
    {
        .name = _("{JPN}くだけるよろい"),
        .description = COMPOUND_STRING("ぶつりで ぼうぎょがさがるが はやくなる"),
        .aiRating = 2,
    },

    [ABILITY_HEAVY_METAL] =
    {
        .name = _("{JPN}ヘヴィメタル"),
        .description = COMPOUND_STRING("たいじゅうが おもくなる"),
        .aiRating = -1,
        .breakable = TRUE,
    },

    [ABILITY_LIGHT_METAL] =
    {
        .name = _("{JPN}ライトメタル"),
        .description = COMPOUND_STRING("たいじゅうが かるくなる"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MULTISCALE] =
    {
        .name = _("{JPN}マルチスケイル"),
        .description = COMPOUND_STRING("HPまんたんで うける ダメージが へる"),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_TOXIC_BOOST] =
    {
        .name = _("{JPN}どくぼうそう"),
        .description = COMPOUND_STRING("どくのとき こうげきが あがる"),
        .aiRating = 6,
    },

    [ABILITY_FLARE_BOOST] =
    {
        .name = _("{JPN}ねつぼうそう"),
        .description = COMPOUND_STRING("やけどのとき とくこうが あがる"),
        .aiRating = 5,
    },

    [ABILITY_HARVEST] =
    {
        .name = _("{JPN}しゅうかく"),
        .description = COMPOUND_STRING("つかった きのみを ふっかつさせる"),
        .aiRating = 5,
    },

    [ABILITY_TELEPATHY] =
    {
        .name = _("{JPN}テレパシー"),
        .description = COMPOUND_STRING("みかたの こうげきが あたらない"),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_MOODY] =
    {
        .name = _("{JPN}ムラっけ"),
        .description = COMPOUND_STRING("のうりょくが ランダムに かわる"),
        .aiRating = 10,
    },

    [ABILITY_OVERCOAT] =
    {
        .name = _("{JPN}ぼうじん"),
        .description = COMPOUND_STRING("こなと てんきの ダメージを ふせぐ"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_POISON_TOUCH] =
    {
        .name = _("{JPN}どくしゅ"),
        .description = COMPOUND_STRING("こうげきした あいてを どくにする"),
        .aiRating = 4,
    },

    [ABILITY_REGENERATOR] =
    {
        .name = _("{JPN}さいせいりょく"),
        .description = COMPOUND_STRING("ひっこむと HPが かいふくする"),
        .aiRating = 8,
    },

    [ABILITY_BIG_PECKS] =
    {
        .name = _("{JPN}はとむね"),
        .description = COMPOUND_STRING("あいてに ぼうぎょを さげられない"),
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_SAND_RUSH] =
    {
        .name = _("{JPN}すなかき"),
        .description = COMPOUND_STRING("すなあらしのとき すばやさが はやくなる"),
        .aiRating = 6,
    },

    [ABILITY_WONDER_SKIN] =
    {
        .name = _("{JPN}ミラクルスキン"),
        .description = COMPOUND_STRING("へんかわざを うけにくい"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_ANALYTIC] =
    {
        .name = _("{JPN}アナライズ"),
        .description = COMPOUND_STRING("さいごに わざをだすと つよくなる"),
        .aiRating = 5,
    },

    [ABILITY_ILLUSION] =
    {
        .name = _("{JPN}イリュージョン"),
        .description = COMPOUND_STRING("てもちの ポケモンに ばける"),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_IMPOSTER] =
    {
        .name = _("{JPN}かわりもの"),
        .description = COMPOUND_STRING("あいての ポケモンに へんしんする"),
        .aiRating = 9,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_INFILTRATOR] =
    {
        .name = _("{JPN}すりぬけ"),
        .description = COMPOUND_STRING("かべや みがわりの えいきょうをうけない"),
        .aiRating = 6,
    },

    [ABILITY_MUMMY] =
    {
        .name = _("{JPN}ミイラ"),
        .description = COMPOUND_STRING("ふれられると とくせいを ミイラに する"),
        .aiRating = 5,
    },

    [ABILITY_MOXIE] =
    {
        .name = _("{JPN}じしんかじょう"),
        .description = COMPOUND_STRING("あいてを たおすと こうげきが あがる"),
        .aiRating = 7,
    },

    [ABILITY_JUSTIFIED] =
    {
        .name = _("{JPN}せいぎのこころ"),
        .description = COMPOUND_STRING("あくわざを うけると こうげきが あがる"),
        .aiRating = 4,
    },

    [ABILITY_RATTLED] =
    {
        .name = _("{JPN}びびり"),
        .description = COMPOUND_STRING("ゴースト むし あくで すばやさがあがる"),
        .aiRating = 3,
    },

    [ABILITY_MAGIC_BOUNCE] =
    {
        .name = _("{JPN}マジックミラー"),
        .description = COMPOUND_STRING("へんかわざを はねかえす"),
        .aiRating = 9,
        .breakable = TRUE,
    },

    [ABILITY_SAP_SIPPER] =
    {
        .name = _("{JPN}そうしょく"),
        .description = COMPOUND_STRING("くさわざを うけると こうげきが あがる"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_PRANKSTER] =
    {
        .name = _("{JPN}いたずらごころ"),
        .description = COMPOUND_STRING("へんかわざが はやく だせる"),
        .aiRating = 8,
    },

    [ABILITY_SAND_FORCE] =
    {
        .name = _("{JPN}すなのちから"),
        .description = COMPOUND_STRING("すなで じめん いわ はがねがつよくなる"),
        .aiRating = 4,
    },

    [ABILITY_IRON_BARBS] =
    {
        .name = _("{JPN}てつのトゲ"),
        .description = COMPOUND_STRING("さわった あいてを キズつける"),
        .aiRating = 6,
    },

    [ABILITY_ZEN_MODE] =
    {
        .name = _("{JPN}ダルマモード"),
        .description = COMPOUND_STRING("ピンチに なると すがたが かわる"),
        .aiRating = -1,
        .cantBeCopied = TRUE,
        .cantBeSwapped = B_UPDATED_ABILITY_DATA >= GEN_7,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = B_UPDATED_ABILITY_DATA >= GEN_7,
        .cantBeOverwritten = B_UPDATED_ABILITY_DATA >= GEN_7,
        .failsOnImposter = TRUE,
    },

    [ABILITY_VICTORY_STAR] =
    {
        .name = _("{JPN}しょうりのほし"),
        .description = COMPOUND_STRING("みかたがわの めいちゅうりつが あがる"),
        .aiRating = 6,
    },

    [ABILITY_TURBOBLAZE] =
    {
        .name = _("{JPN}ターボブレイズ"),
        .description = COMPOUND_STRING("あいての とくせいを むしする"),
        .aiRating = 7,
    },

    [ABILITY_TERAVOLT] =
    {
        .name = _("{JPN}テラボルテージ"),
        .description = COMPOUND_STRING("あいての とくせいを むしする"),
        .aiRating = 7,
    },

    [ABILITY_AROMA_VEIL] =
    {
        .name = _("{JPN}アロマベール"),
        .description = COMPOUND_STRING("みかたがわの メンタルわざを ふせぐ"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_FLOWER_VEIL] =
    {
        .name = _("{JPN}フラワーベール"),
        .description = COMPOUND_STRING("みかたの くさの へんかを ふせぐ"),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_CHEEK_POUCH] =
    {
        .name = _("{JPN}ほおぶくろ"),
        .description = COMPOUND_STRING("きのみを たべると HPも かいふくする"),
        .aiRating = 4,
    },

    [ABILITY_PROTEAN] =
    {
        .name = _("{JPN}へんげんじざい"),
        .description = COMPOUND_STRING("つかったわざの タイプに へんかする"),
        .aiRating = 8,
    },

    [ABILITY_FUR_COAT] =
    {
        .name = _("{JPN}ファーコート"),
        .description = COMPOUND_STRING("ぶつりわざの ダメージを へらす"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_MAGICIAN] =
    {
        .name = _("{JPN}マジシャン"),
        .description = COMPOUND_STRING("こうげきを あてたとき どうぐを うばう"),
        .aiRating = 3,
    },

    [ABILITY_BULLETPROOF] =
    {
        .name = _("{JPN}ぼうだん"),
        .description = COMPOUND_STRING("たまや ばくだんの わざを うけない"),
        .breakable = TRUE,
        .aiRating = 7,
    },

    [ABILITY_COMPETITIVE] =
    {
        .name = _("{JPN}かちき"),
        .description = COMPOUND_STRING("のうりょくが さがると とくこうをあげる"),
        .aiRating = 5,
    },

    [ABILITY_STRONG_JAW] =
    {
        .name = _("{JPN}がんじょうあご"),
        .description = COMPOUND_STRING("キバの わざの いりょくが あがる"),
        .aiRating = 6,
    },

    [ABILITY_REFRIGERATE] =
    {
        .name = _("{JPN}フリーズスキン"),
        .description = COMPOUND_STRING("ノーマルわざが こおりになり つよくなる"),
        .aiRating = 8,
    },

    [ABILITY_SWEET_VEIL] =
    {
        .name = _("{JPN}スイートベール"),
        .description = COMPOUND_STRING("じぶんと みかたが ねむらない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_STANCE_CHANGE] =
    {
        .name = _("{JPN}バトルスイッチ"),
        .description = COMPOUND_STRING("こうげきじに すがたが かわる"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_GALE_WINGS] =
    {
        .name = _("{JPN}はやてのつばさ"),
        .description = COMPOUND_STRING("ひこうわざを さきに だせる"),
        .aiRating = 6,
    },

    [ABILITY_MEGA_LAUNCHER] =
    {
        .name = _("{JPN}メガランチャー"),
        .description = COMPOUND_STRING("はどうの わざの いりょくが あがる"),
        .aiRating = 7,
    },

    [ABILITY_GRASS_PELT] =
    {
        .name = _("{JPN}くさのけがわ"),
        .description = COMPOUND_STRING("くさむらで ぼうぎょが あがる"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_SYMBIOSIS] =
    {
        .name = _("{JPN}きょうせい"),
        .description = COMPOUND_STRING("みかたに どうぐを わたす"),
        .aiRating = 0,
    },

    [ABILITY_TOUGH_CLAWS] =
    {
        .name = _("{JPN}かたいツメ"),
        .description = COMPOUND_STRING("ちょくせつ こうげきが つよくなる"),
        .aiRating = 7,
    },

    [ABILITY_PIXILATE] =
    {
        .name = _("{JPN}フェアリースキン"),
        .description = COMPOUND_STRING("ノーマルわざが フェアリーになり つよい"),
        .aiRating = 8,
    },

    [ABILITY_GOOEY] =
    {
        .name = _("{JPN}ぬめぬめ"),
        .description = COMPOUND_STRING("ふれた あいての すばやさを さげる"),
        .aiRating = 5,
    },

    [ABILITY_AERILATE] =
    {
        .name = _("{JPN}スカイスキン"),
        .description = COMPOUND_STRING("ノーマルわざが ひこうになり つよくなる"),
        .aiRating = 8,
    },

    [ABILITY_PARENTAL_BOND] =
    {
        .name = _("{JPN}おやこあい"),
        .description = COMPOUND_STRING("おやこで 2かい こうげきする"),
        .aiRating = 10,
    },

    [ABILITY_DARK_AURA] =
    {
        .name = _("{JPN}ダークオーラ"),
        .description = COMPOUND_STRING("ぜんいんの あくわざが つよくなる"),
        .aiRating = 6,
        .breakable = B_UPDATED_ABILITY_DATA < GEN_8,
    },

    [ABILITY_FAIRY_AURA] =
    {
        .name = _("{JPN}フェアリーオーラ"),
        .description = COMPOUND_STRING("ぜんいんの フェアリーわざが つよくなる"),
        .aiRating = 6,
        .breakable = B_UPDATED_ABILITY_DATA < GEN_8,
    },

    [ABILITY_AURA_BREAK] =
    {
        .name = _("{JPN}オーラブレイク"),
        .description = COMPOUND_STRING("オーラの こうかを ぎゃくにする"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_PRIMORDIAL_SEA] =
    {
        .name = _("{JPN}はじまりのうみ"),
        .description = COMPOUND_STRING("てんきを つよいあめに する"),
        .aiRating = 10,
    },

    [ABILITY_DESOLATE_LAND] =
    {
        .name = _("{JPN}おわりのだいち"),
        .description = COMPOUND_STRING("ひざしを とても つよくする"),
        .aiRating = 10,
    },

    [ABILITY_DELTA_STREAM] =
    {
        .name = _("{JPN}デルタストリーム"),
        .description = COMPOUND_STRING("てんきを らんきりゅうに する"),
        .aiRating = 10,
    },

    [ABILITY_STAMINA] =
    {
        .name = _("{JPN}じきゅうりょく"),
        .description = COMPOUND_STRING("こうげきを うけると ぼうぎょが あがる"),
        .aiRating = 6,
    },

    [ABILITY_WIMP_OUT] =
    {
        .name = _("{JPN}にげごし"),
        .description = COMPOUND_STRING("HPが はんぶんに なると にげだす"),
        .aiRating = 3,
    },

    [ABILITY_EMERGENCY_EXIT] =
    {
        .name = _("{JPN}ききかいひ"),
        .description = COMPOUND_STRING("HPが はんぶんに なると にげだす"),
        .aiRating = 3,
    },

    [ABILITY_WATER_COMPACTION] =
    {
        .name = _("{JPN}みずがため"),
        .description = COMPOUND_STRING("みずで ぼうぎょが ぐーんと あがる"),
        .aiRating = 4,
    },

    [ABILITY_MERCILESS] =
    {
        .name = _("{JPN}ひとでなし"),
        .description = COMPOUND_STRING("どくの あいてには きゅうしょ になる"),
        .aiRating = 4,
    },

    [ABILITY_SHIELDS_DOWN] =
    {
        .name = _("{JPN}リミットシールド"),
        .description = COMPOUND_STRING("HPが はんぶんに なると こわれる"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_STAKEOUT] =
    {
        .name = _("{JPN}はりこみ"),
        .description = COMPOUND_STRING("こうたいした てきに ダメージ2ばい"),
        .aiRating = 6,
    },

    [ABILITY_WATER_BUBBLE] =
    {
        .name = _("{JPN}すいほう"),
        .description = COMPOUND_STRING("ほのおわざの いりょくを さげる"),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_STEELWORKER] =
    {
        .name = _("{JPN}はがねつかい"),
        .description = COMPOUND_STRING("はがねの わざの いりょくが あがる"),
        .aiRating = 6,
    },

    [ABILITY_BERSERK] =
    {
        .name = _("{JPN}ぎゃくじょう"),
        .description = COMPOUND_STRING("HPが はんぶんになると つよくなる"),
        .aiRating = 5,
    },

    [ABILITY_SLUSH_RUSH] =
    {
        .name = _("{JPN}ゆきかき"),
        .description = COMPOUND_STRING("あられのとき すばやさが はやくなる"),
        .aiRating = 5,
    },

    [ABILITY_LONG_REACH] =
    {
        .name = _("{JPN}えんかく"),
        .description = COMPOUND_STRING("わざを せっしょく しないで だす"),
        .aiRating = 3,
    },

    [ABILITY_LIQUID_VOICE] =
    {
        .name = _("{JPN}うるおいボイス"),
        .description = COMPOUND_STRING("おとわざが みずタイプに なる"),
        .aiRating = 5,
    },

    [ABILITY_TRIAGE] =
    {
        .name = _("{JPN}ヒーリングシフト"),
        .description = COMPOUND_STRING("かいふくわざを せんせい できる"),
        .aiRating = 7,
    },

    [ABILITY_GALVANIZE] =
    {
        .name = _("{JPN}エレキスキン"),
        .description = COMPOUND_STRING("ノーマルわざが でんきになり つよくなる"),
        .aiRating = 8,
    },

    [ABILITY_SURGE_SURFER] =
    {
        .name = _("{JPN}サーフテール"),
        .description = COMPOUND_STRING("エレキフィールドで はやくなる"),
        .aiRating = 4,
    },

    [ABILITY_SCHOOLING] =
    {
        .name = _("{JPN}ぎょぐん"),
        .description = COMPOUND_STRING("HPが おおいと つよい"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_DISGUISE] =
    {
        .name = _("{JPN}ばけのかわ"),
        .description = COMPOUND_STRING("こうげきを 1どだけ ふせぐ"),
        .aiRating = 8,
        .breakable = TRUE,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_BATTLE_BOND] =
    {
        .name = _("{JPN}きずなへんげ"),
        .description = COMPOUND_STRING("あいてを たおすと へんげする"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_POWER_CONSTRUCT] =
    {
        .name = _("{JPN}スワームチェンジ"),
        .description = COMPOUND_STRING("HPが はんぶんになると すがたをかえる"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_CORROSION] =
    {
        .name = _("{JPN}ふしょく"),
        .description = COMPOUND_STRING("だれでも どくに することが できる"),
        .aiRating = 5,
    },

    [ABILITY_COMATOSE] =
    {
        .name = _("{JPN}ぜったいねむり"),
        .description = COMPOUND_STRING("ゆめうつつで ぜったいに めざめない"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_QUEENLY_MAJESTY] =
    {
        .name = _("{JPN}じょおうのいげん"),
        .description = COMPOUND_STRING("あいては せんせいわざを だせない"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_INNARDS_OUT] =
    {
        .name = _("{JPN}とびだすなかみ"),
        .description = COMPOUND_STRING("たおされたとき ダメージを あたえる"),
        .aiRating = 5,
    },

    [ABILITY_DANCER] =
    {
        .name = _("{JPN}おどりこ"),
        .description = COMPOUND_STRING("おどりわざに つづいて おどりだす"),
        .aiRating = 5,
    },

    [ABILITY_BATTERY] =
    {
        .name = _("{JPN}バッテリー"),
        .description = COMPOUND_STRING("みかたの とくしゅの いりょくをあげる"),
        .aiRating = 0,
    },

    [ABILITY_FLUFFY] =
    {
        .name = _("{JPN}もふもふ"),
        .description = COMPOUND_STRING("せっしょく わざを はんげんする"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_DAZZLING] =
    {
        .name = _("{JPN}ビビッドボディ"),
        .description = COMPOUND_STRING("あいては せんせいわざを だせない"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SOUL_HEART] =
    {
        .name = _("{JPN}ソウルハート"),
        .description = COMPOUND_STRING("ひんしに なるたびに とくこうが あがる"),
        .aiRating = 7,
    },

    [ABILITY_TANGLING_HAIR] =
    {
        .name = _("{JPN}カーリーヘアー"),
        .description = COMPOUND_STRING("ふれた あいての すばやさを さげる"),
        .aiRating = 5,
    },

    [ABILITY_RECEIVER] =
    {
        .name = _("{JPN}レシーバー"),
        .description = COMPOUND_STRING("みかたの とくせいを うけつぐ"),
        .aiRating = 0,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_POWER_OF_ALCHEMY] =
    {
        .name = _("{JPN}かがくのちから"),
        .description = COMPOUND_STRING("みかたの とくせいを うけつぐ"),
        .aiRating = 0,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_BEAST_BOOST] =
    {
        .name = _("{JPN}ビーストブースト"),
        .description = COMPOUND_STRING("たおしたとき のうりょくが あがる"),
        .aiRating = 7,
    },

    [ABILITY_RKS_SYSTEM] =
    {
        .name = _("{JPN}ARシステム"),
        .description = COMPOUND_STRING("メモリで じぶんの タイプが かわる"),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_ELECTRIC_SURGE] =
    {
        .name = _("{JPN}エレキメイカー"),
        .description = COMPOUND_STRING("エレキフィールドを はりめぐらせる"),
        .aiRating = 8,
    },

    [ABILITY_PSYCHIC_SURGE] =
    {
        .name = _("{JPN}サイコメイカー"),
        .description = COMPOUND_STRING("サイコフィールドを はりめぐらせる"),
        .aiRating = 8,
    },

    [ABILITY_MISTY_SURGE] =
    {
        .name = _("{JPN}ミストメイカー"),
        .description = COMPOUND_STRING("ミストフィールドを はりめぐらせる"),
        .aiRating = 8,
    },

    [ABILITY_GRASSY_SURGE] =
    {
        .name = _("{JPN}グラスメイカー"),
        .description = COMPOUND_STRING("グラスフィールドを はりめぐらせる"),
        .aiRating = 8,
    },

    [ABILITY_FULL_METAL_BODY] =
    {
        .name = _("{JPN}メタルプロテクト"),
        .description = COMPOUND_STRING("あいてに のうりょくを さげられない"),
        .aiRating = 4,
    },

    [ABILITY_SHADOW_SHIELD] =
    {
        .name = _("{JPN}ファントムガード"),
        .description = COMPOUND_STRING("HPまんたんで うける ダメージが へる"),
        .aiRating = 8,
    },

    [ABILITY_PRISM_ARMOR] =
    {
        .name = _("{JPN}プリズムアーマー"),
        .description = COMPOUND_STRING("ばつぐんになる わざを よわめる"),
        .aiRating = 6,
    },

    [ABILITY_NEUROFORCE] =
    {
        .name = _("{JPN}ブレインフォース"),
        .description = COMPOUND_STRING("こうかばつぐんで いりょくが あがる"),
        .aiRating = 6,
    },

    [ABILITY_INTREPID_SWORD] =
    {
        .name = _("{JPN}ふとうのけん"),
        .description = COMPOUND_STRING("とうじょうじに こうげきが あがる"),
        .aiRating = 3,
    },

    [ABILITY_DAUNTLESS_SHIELD] =
    {
        .name = _("{JPN}ふくつのたて"),
        .description = COMPOUND_STRING("とうじょうじに ぼうぎょが あがる"),
        .aiRating = 3,
    },

    [ABILITY_LIBERO] =
    {
        .name = _("{JPN}リベロ"),
        .description = COMPOUND_STRING("つかったわざの タイプに へんかする"),
    },

    [ABILITY_BALL_FETCH] =
    {
        .name = _("{JPN}たまひろい"),
        .description = COMPOUND_STRING("しっぱいした ボールを ひろってくる"),
        .aiRating = 0,
    },

    [ABILITY_COTTON_DOWN] =
    {
        .name = _("{JPN}わたげ"),
        .description = COMPOUND_STRING("こうげきを うけると みんなをおそくする"),
        .aiRating = 3,
    },

    [ABILITY_PROPELLER_TAIL] =
    {
        .name = _("{JPN}スクリューおびれ"),
        .description = COMPOUND_STRING("わざを ひきうける こうかを むしする"),
        .aiRating = 2,
    },

    [ABILITY_MIRROR_ARMOR] =
    {
        .name = _("{JPN}ミラーアーマー"),
        .description = COMPOUND_STRING("のうりょくダウンの こうかを はねかえす"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_GULP_MISSILE] =
    {
        .name = _("{JPN}うのミサイル"),
        .description = COMPOUND_STRING("うみから えものを くわえてくる"),
        .aiRating = 3,
        .cantBeSwapped = B_UPDATED_ABILITY_DATA < GEN_9,
        .cantBeCopied = B_UPDATED_ABILITY_DATA < GEN_9,
        .cantBeTraced = B_UPDATED_ABILITY_DATA < GEN_9,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_STALWART] =
    {
        .name = _("{JPN}すじがねいり"),
        .description = COMPOUND_STRING("わざを ひきうける こうかを むしする"),
        .aiRating = 2,
    },

    [ABILITY_STEAM_ENGINE] =
    {
        .name = _("{JPN}じょうききかん"),
        .description = COMPOUND_STRING("みずか ほのおを うけると すばやくなる"),
        .aiRating = 3,
    },

    [ABILITY_PUNK_ROCK] =
    {
        .name = _("{JPN}パンクロック"),
        .description = COMPOUND_STRING("おとわざの いりょくが あがる"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_SAND_SPIT] =
    {
        .name = _("{JPN}すなはき"),
        .description = COMPOUND_STRING("こうげきをうけると すなあらしを おこす"),
        .aiRating = 5,
    },

    [ABILITY_ICE_SCALES] =
    {
        .name = _("{JPN}こおりのりんぷん"),
        .description = COMPOUND_STRING("とくしゅこうげきを はんげん する"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_RIPEN] =
    {
        .name = _("{JPN}じゅくせい"),
        .description = COMPOUND_STRING("きのみの こうかが ばいに なる"),
        .aiRating = 4,
    },

    [ABILITY_ICE_FACE] =
    {
        .name = _("{JPN}アイスフェイス"),
        .description = COMPOUND_STRING("あたまの こおりが みがわりに なる"),
        .aiRating = 4,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .breakable = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_POWER_SPOT] =
    {
        .name = _("{JPN}パワースポット"),
        .description = COMPOUND_STRING("じぶん いがいが つよくなる"),
        .aiRating = 2,
    },

    [ABILITY_MIMICRY] =
    {
        .name = _("{JPN}ぎたい"),
        .description = COMPOUND_STRING("フィールドに よって タイプが かわる"),
        .aiRating = 2,
    },

    [ABILITY_SCREEN_CLEANER] =
    {
        .name = _("{JPN}バリアフリー"),
        .description = COMPOUND_STRING("すべての バリアを けしさる"),
        .aiRating = 3,
    },

    [ABILITY_STEELY_SPIRIT] =
    {
        .name = _("{JPN}はがねのせいしん"),
        .description = COMPOUND_STRING("みかたの はがね こうげきが つよくなる"),
        .aiRating = 2,
    },

    [ABILITY_PERISH_BODY] =
    {
        .name = _("{JPN}ほろびのボディ"),
        .description = COMPOUND_STRING("わざをうけると 3ターンご ひんしになる"),
        .aiRating = -1,
    },

    [ABILITY_WANDERING_SPIRIT] =
    {
        .name = _("{JPN}さまようたましい"),
        .description = COMPOUND_STRING("せっしょくすると とくせいを いれかえる"),
        .aiRating = 2,
    },

    [ABILITY_GORILLA_TACTICS] =
    {
        .name = _("{JPN}ごりむちゅう"),
        .description = COMPOUND_STRING("つよくなるが おなじ わざしか だせない"),
        .aiRating = 4,
    },

    [ABILITY_NEUTRALIZING_GAS] =
    {
        .name = _("{JPN}かがくへんかガス"),
        .description = COMPOUND_STRING("すべての とくせいの こうかが きえる"),
        .aiRating = 5,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_PASTEL_VEIL] =
    {
        .name = _("{JPN}パステルベール"),
        .description = COMPOUND_STRING("じぶんも みかたも どくに ならない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_HUNGER_SWITCH] =
    {
        .name = _("{JPN}はらぺこスイッチ"),
        .description = COMPOUND_STRING("ターンのおわりに すがたを かえる"),
        .aiRating = 2,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_QUICK_DRAW] =
    {
        .name = _("{JPN}クイックドロウ"),
        .description = COMPOUND_STRING("さきに こうどう できることが ある"),
        .aiRating = 4,
    },

    [ABILITY_UNSEEN_FIST] =
    {
        .name = _("{JPN}ふかしのこぶし"),
        .description = COMPOUND_STRING("まもりを むしして こうげき する"),
        .aiRating = 6,
    },

    [ABILITY_CURIOUS_MEDICINE] =
    {
        .name = _("{JPN}きみょうなくすり"),
        .description = COMPOUND_STRING("みかたの のうりょくを もとにもどす"),
        .aiRating = 3,
    },

    [ABILITY_TRANSISTOR] =
    {
        .name = _("{JPN}トランジスタ"),
        .description = COMPOUND_STRING("でんきの わざの いりょくが あがる"),
        .aiRating = 6,
    },

    [ABILITY_DRAGONS_MAW] =
    {
        .name = _("{JPN}りゅうのあぎと"),
        .description = COMPOUND_STRING("ドラゴンの わざの いりょくが あがる"),
        .aiRating = 6,
    },

    [ABILITY_CHILLING_NEIGH] =
    {
        .name = _("{JPN}しろのいななき"),
        .description = COMPOUND_STRING("あいてを たおすと こうげきが あがる"),
        .aiRating = 7,
    },

    [ABILITY_GRIM_NEIGH] =
    {
        .name = _("{JPN}くろのいななき"),
        .description = COMPOUND_STRING("あいてを たおすと とくこうが あがる"),
        .aiRating = 7,
    },

    [ABILITY_AS_ONE_ICE_RIDER] =
    {
        .name = _("{JPN}じんばいったい"),
        .description = COMPOUND_STRING("しろのいななきも あわせもつ"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_AS_ONE_SHADOW_RIDER] =
    {
        .name = _("{JPN}じんばいったい"),
        .description = COMPOUND_STRING("くろのいななきも あわせもつ"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_LINGERING_AROMA] =
    {
        .name = _("{JPN}とれないにおい"),
        .description = COMPOUND_STRING("とれないにおいが あいてに うつる"),
        .aiRating = 5,
    },

    [ABILITY_SEED_SOWER] =
    {
        .name = _("{JPN}こぼれダネ"),
        .description = COMPOUND_STRING("わざをうけると グラスフィールドに なる"),
        .aiRating = 5,
    },

    [ABILITY_THERMAL_EXCHANGE] =
    {
        .name = _("{JPN}ねつこうかん"),
        .description = COMPOUND_STRING("ほのおの わざを うけると つよくなる"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_ANGER_SHELL] =
    {
        .name = _("{JPN}いかりのこうら"),
        .description = COMPOUND_STRING("HPが はんぶんに なると つよくなる"),
        .aiRating = 3,
    },

    [ABILITY_PURIFYING_SALT] =
    {
        .name = _("{JPN}きよめのしお"),
        .description = COMPOUND_STRING("じょうたいいじょうに ならない"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_WELL_BAKED_BODY] =
    {
        .name = _("{JPN}こんがりボディ"),
        .description = COMPOUND_STRING("ほのおの わざで ぼうぎょが あがる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_WIND_RIDER] =
    {
        .name = _("{JPN}かぜのり"),
        .description = COMPOUND_STRING("かぜで こうげきが あがる"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_GUARD_DOG] =
    {
        .name = _("{JPN}ばんけん"),
        .description = COMPOUND_STRING("いかく されると つよくなる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_ROCKY_PAYLOAD] =
    {
        .name = _("{JPN}いわはこび"),
        .description = COMPOUND_STRING("いわの わざの いりょくが あがる"),
        .aiRating = 6,
    },

    [ABILITY_WIND_POWER] =
    {
        .name = _("{JPN}ふうりょくでんき"),
        .description = COMPOUND_STRING("かぜで じゅうでん する"),
        .aiRating = 4,
    },

    [ABILITY_ZERO_TO_HERO] =
    {
        .name = _("{JPN}マイティチェンジ"),
        .description = COMPOUND_STRING("ひっこむと マイティチェンジ する"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_COMMANDER] =
    {
        .name = _("{JPN}しれいとう"),
        .description = COMPOUND_STRING("ヘイラッシャが いると しれいを だす"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_ELECTROMORPHOSIS] =
    {
        .name = _("{JPN}でんきにかえる"),
        .description = COMPOUND_STRING("ダメージを うけると じゅうでん する"),
        .aiRating = 5,
    },

    [ABILITY_PROTOSYNTHESIS] =
    {
        .name = _("{JPN}こだいかっせい"),
        .description = COMPOUND_STRING("はれで のうりょくが あがる"),
        .aiRating = 7,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_QUARK_DRIVE] =
    {
        .name = _("{JPN}クォークチャージ"),
        .description = COMPOUND_STRING("エレキフィールドで のうりょくが あがる"),
        .aiRating = 7,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_GOOD_AS_GOLD] =
    {
        .name = _("{JPN}おうごんのからだ"),
        .description = COMPOUND_STRING("へんかわざを むこうか する"),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_VESSEL_OF_RUIN] =
    {
        .name = _("{JPN}わざわいのうつわ"),
        .description = COMPOUND_STRING("じぶんいがいの とくこうが さがる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SWORD_OF_RUIN] =
    {
        .name = _("{JPN}わざわいのつるぎ"),
        .description = COMPOUND_STRING("じぶんいがいの ぼうぎょが さがる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_TABLETS_OF_RUIN] =
    {
        .name = _("{JPN}わざわいのおふだ"),
        .description = COMPOUND_STRING("じぶんいがいの こうげきが さがる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_BEADS_OF_RUIN] =
    {
        .name = _("{JPN}わざわいのたま"),
        .description = COMPOUND_STRING("じぶんいがいの とくぼうが さがる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_ORICHALCUM_PULSE] =
    {
        .name = _("{JPN}ひひいろのこどう"),
        .description = COMPOUND_STRING("せんとうで ひざしを つよくする"),
        .aiRating = 8,
    },

    [ABILITY_HADRON_ENGINE] =
    {
        .name = _("{JPN}ハドロンエンジン"),
        .description = COMPOUND_STRING("エレキフィールドを はりめぐらせる"),
        .aiRating = 8,
    },

    [ABILITY_OPPORTUNIST] =
    {
        .name = _("{JPN}びんじょう"),
        .description = COMPOUND_STRING("のうりょくが あがると じぶんも あがる"),
        .aiRating = 5,
    },

    [ABILITY_CUD_CHEW] =
    {
        .name = _("{JPN}はんすう"),
        .description = COMPOUND_STRING("きのみを もう1かい だけ たべる"),
        .aiRating = 4,
    },

    [ABILITY_SHARPNESS] =
    {
        .name = _("{JPN}きれあじ"),
        .description = COMPOUND_STRING("きるわざの いりょくが あがる"),
        .aiRating = 7,
    },

    [ABILITY_SUPREME_OVERLORD] =
    {
        .name = _("{JPN}そうだいしょう"),
        .description = COMPOUND_STRING("たおされた みかたの かずで つよくなる"),
        .aiRating = 6,
    },

    [ABILITY_COSTAR] =
    {
        .name = _("{JPN}きょうえん"),
        .description = COMPOUND_STRING("みかたの のうりょくを コピーする"),
        .aiRating = 5,
    },

    [ABILITY_TOXIC_DEBRIS] =
    {
        .name = _("{JPN}どくげしょう"),
        .description = COMPOUND_STRING("こうげきをうけると どくびしを ばらまく"),
        .aiRating = 4,
    },

    [ABILITY_ARMOR_TAIL] =
    {
        .name = _("{JPN}テイルアーマー"),
        .description = COMPOUND_STRING("あいては せんせいわざを だせない"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_EARTH_EATER] =
    {
        .name = _("{JPN}どしょく"),
        .description = COMPOUND_STRING("じめんを うけると かいふくする"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_MYCELIUM_MIGHT] =
    {
        .name = _("{JPN}きんしのちから"),
        .description = COMPOUND_STRING("へんかわざを しっぱい しない"),
        .aiRating = 2,
    },

    [ABILITY_HOSPITALITY] =
    {
        .name = _("{JPN}おもてなし"),
        .description = COMPOUND_STRING("みかたの HPを すこし かいふく"),
        .aiRating = 5,
    },

    [ABILITY_MINDS_EYE] =
    {
        .name = _("{JPN}しんがん"),
        .description = COMPOUND_STRING("ゴーストタイプに わざが あたる"),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_TEAL_MASK] =
    {
        .name = _("{JPN}おもかげやどし"),
        .description = COMPOUND_STRING("じぶんの すばやさを あげる"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_HEARTHFLAME_MASK] =
    {
        .name = _("{JPN}おもかげやどし"),
        .description = COMPOUND_STRING("じぶんの こうげきを あげる"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_WELLSPRING_MASK] =
    {
        .name = _("{JPN}おもかげやどし"),
        .description = COMPOUND_STRING("じぶんの とくぼうを あげる"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_CORNERSTONE_MASK] =
    {
        .name = _("{JPN}おもかげやどし"),
        .description = COMPOUND_STRING("じぶんの ぼうぎょを あげる"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_TOXIC_CHAIN] =
    {
        .name = _("{JPN}どくのくさり"),
        .description = COMPOUND_STRING("もうどくに することが ある"),
        .aiRating = 8,
    },

    [ABILITY_SUPERSWEET_SYRUP] =
    {
        .name = _("{JPN}かんろなミツ"),
        .description = COMPOUND_STRING("あいての かいひりつを さげる"),
        .aiRating = 5,
    },

    [ABILITY_TERA_SHIFT] =
    {
        .name = _("{JPN}テラスチェンジ"),
        .description = COMPOUND_STRING("テラスタルフォルムに へんか する"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_TERA_SHELL] =
    {
        .name = _("{JPN}テラスシェル"),
        .description = COMPOUND_STRING("HPまんたんで わざが いまひとつ"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .breakable = TRUE,
    },

    [ABILITY_TERAFORM_ZERO] =
    {
        .name = _("{JPN}ゼロフォーミング"),
        .description = COMPOUND_STRING("てんきと フィールドを すべてゼロにする"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_POISON_PUPPETEER] =
    {
        .name = _("{JPN}どくくぐつ"),
        .description = COMPOUND_STRING("どくに なった あいては こんらんする"),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_PIERCING_DRILL] =
    {
        .name = _("{JPN}かんつうドリル"),
        .description = COMPOUND_STRING("まもりを むしして こうげき する"),
    },

    [ABILITY_DRAGONIZE] =
    {
        .name = _("{JPN}ドラゴンスキン"),
        .description = COMPOUND_STRING("ノーマルわざが ドラゴンになり つよい"),
    },

    [ABILITY_EELEVATE] =
    {
        .name = _("うなぎのぼり"),
        .description = COMPOUND_STRING("たおすと のうりょくあがる ういてる"),
    },

    [ABILITY_314] =
    {
        .name = _("{JPN}-------"),
        .description = COMPOUND_STRING("とくせい なし"),
    },

    [ABILITY_MEGA_SOL] =
    {
        .name = _("{JPN}メガソーラー"),
        .description = COMPOUND_STRING("にほんばれの じょうたいで わざをつかう"),
    },

    [ABILITY_FIRE_MANE] =
    {
        .name = _("ほのおのたてがみ"),
        .description = COMPOUND_STRING("ほのおわざが つよくなる"),
    },

    [ABILITY_317] =
    {
        .name = _("{JPN}-------"),
        .description = COMPOUND_STRING("とくせい なし"),
    },

    [ABILITY_SPICY_SPRAY] =
    {
        .name = _("{JPN}とびだすハバネロ"),
        .description = COMPOUND_STRING("ダメージを うけたとき やけど させる"),
    },

    [ABILITY_AURA_GUARD] =
    {
        .name = _("{JPN}オーラガード"),
        .description = COMPOUND_STRING("まだ じっそうされていない"),
    },
};
