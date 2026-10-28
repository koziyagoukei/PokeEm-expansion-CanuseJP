const struct AbilityInfo gAbilitiesInfo[ABILITIES_COUNT] =
{
    [ABILITY_NONE] =
    {
        .name = _("{JPN}-------"),
        .description = COMPOUND_STRING("{JPN}特性なし"),
        .aiRating = 0,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_STENCH] =
    {
        .name = _("{JPN}あくしゅう"),
        .description = COMPOUND_STRING("{JPN}攻撃で10{ENG}%{JPN}の確率でひるませる"),
        .aiRating = 1,
    },

    [ABILITY_DRIZZLE] =
    {
        .name = _("{JPN}あめふらし"),
        .description = COMPOUND_STRING("{JPN}戦闘に でると 雨を ふらす"),
        .aiRating = 9,
    },

    [ABILITY_SPEED_BOOST] =
    {
        .name = _("{JPN}かそく"),
        .description = COMPOUND_STRING("{JPN}毎ターン 素早さが1段階上昇"),
        .aiRating = 9,
    },

    [ABILITY_BATTLE_ARMOR] =
    {
        .name = _("{JPN}カブトアーマー"),
        .description = COMPOUND_STRING("{JPN}技を 急所に 受けない"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_STURDY] =
    {
        .name = _("{JPN}がんじょう"),
        .description = COMPOUND_STRING("{JPN}HP満タンなら HP1で耐える"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_DAMP] =
    {
        .name = _("{JPN}しめりけ"),
        .description = COMPOUND_STRING("{JPN}だれも じばくが できなくなる"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_LIMBER] =
    {
        .name = _("{JPN}じゅうなん"),
        .description = COMPOUND_STRING("{JPN}まひ 状態に ならない"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SAND_VEIL] =
    {
        .name = _("{JPN}すながくれ"),
        .description = COMPOUND_STRING("{JPN}すなあらしで 回避率が 上がる"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_STATIC] =
    {
        .name = _("{JPN}せいでんき"),
        .description = COMPOUND_STRING("{JPN}さわった 相手を まひさせる"),
        .aiRating = 4,
    },

    [ABILITY_VOLT_ABSORB] =
    {
        .name = _("{JPN}ちくでん"),
        .description = COMPOUND_STRING("{JPN}でんきを 受けると 回復する"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_WATER_ABSORB] =
    {
        .name = _("{JPN}ちょすい"),
        .description = COMPOUND_STRING("{JPN}みずを 受けると 回復する"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_OBLIVIOUS] =
    {
        .name = _("{JPN}どんかん"),
        .description = COMPOUND_STRING("{JPN}メロメロ 状態に ならない"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_CLOUD_NINE] =
    {
        .name = _("{JPN}ノーてんき"),
        .description = COMPOUND_STRING("{JPN}天気の効果を 無効にする"),
        .aiRating = 5,
    },

    [ABILITY_COMPOUND_EYES] =
    {
        .name = _("{JPN}ふくがん"),
        .description = COMPOUND_STRING("{JPN}技の 命中率が 上がる"),
        .aiRating = 7,
    },

    [ABILITY_INSOMNIA] =
    {
        .name = _("{JPN}ふみん"),
        .description = COMPOUND_STRING("{JPN}眠り 状態に ならない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_COLOR_CHANGE] =
    {
        .name = _("{JPN}へんしょく"),
        .description = COMPOUND_STRING("{JPN}受けた 技の タイプに 変化する"),
        .aiRating = 2,
    },

    [ABILITY_IMMUNITY] =
    {
        .name = _("{JPN}めんえき"),
        .description = COMPOUND_STRING("{JPN}どく 状態に ならない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_FLASH_FIRE] =
    {
        .name = _("{JPN}もらいび"),
        .description = COMPOUND_STRING("{JPN}ほのおを 受けると 強くなる"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SHIELD_DUST] =
    {
        .name = _("{JPN}りんぷん"),
        .description = COMPOUND_STRING("{JPN}技の 追加効果を 受けない"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_OWN_TEMPO] =
    {
        .name = _("{JPN}マイペース"),
        .description = COMPOUND_STRING("{JPN}混乱 状態に ならない"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SUCTION_CUPS] =
    {
        .name = _("{JPN}きゅうばん"),
        .description = COMPOUND_STRING("{JPN}きゅうばんで 地面に はりつく"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_INTIMIDATE] =
    {
        .name = _("{JPN}いかく"),
        .description = COMPOUND_STRING("{JPN}登場時に 相手の攻撃を下げる"),
        .aiRating = 7,
    },

    [ABILITY_SHADOW_TAG] =
    {
        .name = _("{JPN}かげふみ"),
        .description = COMPOUND_STRING("{JPN}相手を 逃げられなくする"),
        .aiRating = 10,
    },

    [ABILITY_ROUGH_SKIN] =
    {
        .name = _("{JPN}さめはだ"),
        .description = COMPOUND_STRING("{JPN}さわった 相手を 傷つける"),
        .aiRating = 6,
    },

    [ABILITY_WONDER_GUARD] =
    {
        .name = _("{JPN}ふしぎなまもり"),
        .description = COMPOUND_STRING("{JPN}効果 ばつぐん しか あたらない"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .breakable = TRUE,
    },

    [ABILITY_LEVITATE] =
    {
        .name = _("{JPN}ふゆう"),
        .description = COMPOUND_STRING("{JPN}地面タイプの 技を 受けない"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_EFFECT_SPORE] =
    {
        .name = _("{JPN}ほうし"),
        .description = COMPOUND_STRING("{JPN}さわった 相手に ほうしがつく"),
        .aiRating = 4,
    },

    [ABILITY_SYNCHRONIZE] =
    {
        .name = _("{JPN}シンクロ"),
        .description = COMPOUND_STRING("{JPN}どく まひ やけどを 相手にうつす"),
        .aiRating = 4,
    },

    [ABILITY_CLEAR_BODY] =
    {
        .name = _("{JPN}クリアボディ"),
        .description = COMPOUND_STRING("{JPN}相手に能力を 下げられない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_NATURAL_CURE] =
    {
        .name = _("{JPN}しぜんかいふく"),
        .description = COMPOUND_STRING("{JPN}交代すると 状態異常が治る"),
        .aiRating = 7,
    },

    [ABILITY_LIGHTNING_ROD] =
    {
        .name = _("{JPN}ひらいしん"),
        .description = COMPOUND_STRING("{JPN}でんきタイプの 技が 自分にくる"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_SERENE_GRACE] =
    {
        .name = _("{JPN}てんのめぐみ"),
        .description = COMPOUND_STRING("{JPN}技の 追加効果が でやすい"),
        .aiRating = 8,
    },

    [ABILITY_SWIFT_SWIM] =
    {
        .name = _("{JPN}すいすい"),
        .description = COMPOUND_STRING("{JPN}雨のとき 素早さが 早くなる"),
        .aiRating = 6,
    },

    [ABILITY_CHLOROPHYLL] =
    {
        .name = _("{JPN}ようりょくそ"),
        .description = COMPOUND_STRING("{JPN}はれのとき 素早さが 早くなる"),
        .aiRating = 6,
    },

    [ABILITY_ILLUMINATE] =
    {
        .name = _("{JPN}はっこう"),
        .description = COMPOUND_STRING("{JPN}ポケモンに 遭遇 しやすくなる"),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_TRACE] =
    {
        .name = _("{JPN}トレース"),
        .description = COMPOUND_STRING("{JPN}相手と 同じ 特性になる"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE, //B_UPDATED_ABILITY_DATA >= GEN_4
    },

    [ABILITY_HUGE_POWER] =
    {
        .name = _("{JPN}ちからもち"),
        .description = COMPOUND_STRING("{JPN}自分の攻撃が 2倍になる"),
        .aiRating = 10,
    },

    [ABILITY_POISON_POINT] =
    {
        .name = _("{JPN}どくのトゲ"),
        .description = COMPOUND_STRING("{JPN}さわった相手に 毒を与える"),
        .aiRating = 4,
    },

    [ABILITY_INNER_FOCUS] =
    {
        .name = _("{JPN}せいしんりょく"),
        .description = COMPOUND_STRING("{JPN}ひるまない"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MAGMA_ARMOR] =
    {
        .name = _("{JPN}マグマのよろい"),
        .description = COMPOUND_STRING("{JPN}こおり 状態に ならない"),
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_WATER_VEIL] =
    {
        .name = _("{JPN}みずのベール"),
        .description = COMPOUND_STRING("{JPN}やけど 状態に ならない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_MAGNET_PULL] =
    {
        .name = _("{JPN}じりょく"),
        .description = COMPOUND_STRING("{JPN}はがねタイプが 逃げられなくなる"),
        .aiRating = 9,
    },

    [ABILITY_SOUNDPROOF] =
    {
        .name = _("{JPN}ぼうおん"),
        .description = COMPOUND_STRING("{JPN}おとの 技を 受けない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_RAIN_DISH] =
    {
        .name = _("{JPN}あめうけざら"),
        .description = COMPOUND_STRING("{JPN}雨のとき すこしずつ 回復"),
        .aiRating = 3,
    },

    [ABILITY_SAND_STREAM] =
    {
        .name = _("{JPN}すなおこし"),
        .description = COMPOUND_STRING("{JPN}戦闘で すなあらしを おこす"),
        .aiRating = 9,
    },

    [ABILITY_PRESSURE] =
    {
        .name = _("{JPN}プレッシャー"),
        .description = COMPOUND_STRING("{JPN}相手の PPが へりやすくなる"),
        .aiRating = 5,
    },

    [ABILITY_THICK_FAT] =
    {
        .name = _("{JPN}あついしぼう"),
        .description = COMPOUND_STRING("{JPN}ほのお こおりに 強い"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_EARLY_BIRD] =
    {
        .name = _("{JPN}はやおき"),
        .description = COMPOUND_STRING("{JPN}おきるのが 早くなる"),
        .aiRating = 4,
    },

    [ABILITY_FLAME_BODY] =
    {
        .name = _("{JPN}ほのおのからだ"),
        .description = COMPOUND_STRING("{JPN}さわった 相手を やけど させる"),
        .aiRating = 4,
    },

    [ABILITY_RUN_AWAY] =
    {
        .name = _("{JPN}にげあし"),
        .description = COMPOUND_STRING("{JPN}戦闘から 逃げやすい"),
        .aiRating = 0,
    },

    [ABILITY_KEEN_EYE] =
    {
        .name = _("{JPN}するどいめ"),
        .description = COMPOUND_STRING("{JPN}相手に 命中を さげられない"),
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_HYPER_CUTTER] =
    {
        .name = _("{JPN}かいりきバサミ"),
        .description = COMPOUND_STRING("{JPN}相手に 攻撃を さげられない"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_PICKUP] =
    {
        .name = _("{JPN}ものひろい"),
        .description = COMPOUND_STRING("{JPN}道具を ひろってくる ことがある"),
        .aiRating = 1,
    },

    [ABILITY_TRUANT] =
    {
        .name = _("{JPN}なまけ"),
        .description = COMPOUND_STRING("{JPN}攻撃が 連続で だせない"),
        .aiRating = -2,
        .cantBeOverwritten = TRUE,
    },

    [ABILITY_HUSTLE] =
    {
        .name = _("{JPN}はりきり"),
        .description = COMPOUND_STRING("{JPN}攻撃は 高いが はずれやすい"),
        .aiRating = 7,
    },

    [ABILITY_CUTE_CHARM] =
    {
        .name = _("{JPN}メロメロボディ"),
        .description = COMPOUND_STRING("{JPN}さわった 相手を メロメロにする"),
        .aiRating = 2,
    },

    [ABILITY_PLUS] =
    {
        .name = _("{JPN}プラス"),
        .description = COMPOUND_STRING("{JPN}マイナスが いると 強くなる"),
        .aiRating = 0,
    },

    [ABILITY_MINUS] =
    {
        .name = _("{JPN}マイナス"),
        .description = COMPOUND_STRING("{JPN}プラスが いると 強くなる"),
        .aiRating = 0,
    },

    [ABILITY_FORECAST] =
    {
        .name = _("{JPN}てんきや"),
        .description = COMPOUND_STRING("{JPN}天気で ポワルンが 変化する"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeTraced = B_UPDATED_ABILITY_DATA >= GEN_4,
        .failsOnImposter = B_UPDATED_ABILITY_DATA >= GEN_5,
    },

    [ABILITY_STICKY_HOLD] =
    {
        .name = _("{JPN}ねんちゃく"),
        .description = COMPOUND_STRING("{JPN}道具を とられない"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_SHED_SKIN] =
    {
        .name = _("{JPN}だっぴ"),
        .description = COMPOUND_STRING("{JPN}だっぴして からだを なおす"),
        .aiRating = 7,
    },

    [ABILITY_GUTS] =
    {
        .name = _("{JPN}こんじょう"),
        .description = COMPOUND_STRING("{JPN}いじょうのとき 攻撃が 上がる"),
        .aiRating = 6,
    },

    [ABILITY_MARVEL_SCALE] =
    {
        .name = _("{JPN}ふしぎなうろこ"),
        .description = COMPOUND_STRING("{JPN}いじょうのとき 防御が 上がる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_LIQUID_OOZE] =
    {
        .name = _("{JPN}ヘドロえき"),
        .description = COMPOUND_STRING("{JPN}すいとると HPが へる"),
        .aiRating = 3,
    },

    [ABILITY_OVERGROW] =
    {
        .name = _("{JPN}しんりょく"),
        .description = COMPOUND_STRING("{JPN}ピンチに くさの 威力が 上がる"),
        .aiRating = 5,
    },

    [ABILITY_BLAZE] =
    {
        .name = _("{JPN}もうか"),
        .description = COMPOUND_STRING("{JPN}ピンチに ほのおが 強くなる"),
        .aiRating = 5,
    },

    [ABILITY_TORRENT] =
    {
        .name = _("{JPN}げきりゅう"),
        .description = COMPOUND_STRING("{JPN}ピンチに みずの 威力が 上がる"),
        .aiRating = 5,
    },

    [ABILITY_SWARM] =
    {
        .name = _("{JPN}むしのしらせ"),
        .description = COMPOUND_STRING("{JPN}ピンチに むしの 威力が 上がる"),
        .aiRating = 5,
    },

    [ABILITY_ROCK_HEAD] =
    {
        .name = _("{JPN}いしあたま"),
        .description = COMPOUND_STRING("{JPN}ぶつかっても 反動を 受けない"),
        .aiRating = 5,
    },

    [ABILITY_DROUGHT] =
    {
        .name = _("{JPN}ひでり"),
        .description = COMPOUND_STRING("{JPN}戦闘で 日差しを 強くする"),
        .aiRating = 9,
    },

    [ABILITY_ARENA_TRAP] =
    {
        .name = _("{JPN}ありじごく"),
        .description = COMPOUND_STRING("{JPN}相手を 逃げられなくする"),
        .aiRating = 9,
    },

    [ABILITY_VITAL_SPIRIT] =
    {
        .name = _("{JPN}やるき"),
        .description = COMPOUND_STRING("{JPN}ねむらない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_WHITE_SMOKE] =
    {
        .name = _("{JPN}しろいけむり"),
        .description = COMPOUND_STRING("{JPN}相手に能力を 下げられない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_PURE_POWER] =
    {
        .name = _("{JPN}ヨガパワー"),
        .description = COMPOUND_STRING("{JPN}自分の攻撃が 2倍になる"),
        .aiRating = 10,
    },

    [ABILITY_SHELL_ARMOR] =
    {
        .name = _("{JPN}シェルアーマー"),
        .description = COMPOUND_STRING("{JPN}技を 急所に 受けない"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_AIR_LOCK] =
    {
        .name = _("{JPN}エアロック"),
        .description = COMPOUND_STRING("{JPN}天気の効果を 無効にする"),
        .aiRating = 5,
    },

    [ABILITY_TANGLED_FEET] =
    {
        .name = _("{JPN}ちどりあし"),
        .description = COMPOUND_STRING("{JPN}混乱のとき 回避率が 上がる"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MOTOR_DRIVE] =
    {
        .name = _("{JPN}でんきエンジン"),
        .description = COMPOUND_STRING("{JPN}でんきを受けると素早さが上がる"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_RIVALRY] =
    {
        .name = _("{JPN}とうそうしん"),
        .description = COMPOUND_STRING("{JPN}性別が 同じだと 強くなる"),
        .aiRating = 1,
    },

    [ABILITY_STEADFAST] =
    {
        .name = _("{JPN}ふくつのこころ"),
        .description = COMPOUND_STRING("{JPN}ひるむ たびに 素早さが 上がる"),
        .aiRating = 2,
    },

    [ABILITY_SNOW_CLOAK] =
    {
        .name = _("{JPN}ゆきがくれ"),
        .description = COMPOUND_STRING("{JPN}あられのとき 回避率が 上がる"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_GLUTTONY] =
    {
        .name = _("{JPN}くいしんぼう"),
        .description = COMPOUND_STRING("{JPN}きのみを いつもより 早く 使う"),
        .aiRating = 3,
    },

    [ABILITY_ANGER_POINT] =
    {
        .name = _("{JPN}いかりのつぼ"),
        .description = COMPOUND_STRING("{JPN}急所を 受けると 攻撃が上がる"),
        .aiRating = 4,
    },

    [ABILITY_UNBURDEN] =
    {
        .name = _("{JPN}かるわざ"),
        .description = COMPOUND_STRING("{JPN}道具がなくなると素早さが上がる"),
        .aiRating = 7,
    },

    [ABILITY_HEATPROOF] =
    {
        .name = _("{JPN}たいねつ"),
        .description = COMPOUND_STRING("{JPN}ほのおと やけどの ダメージを へらす"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SIMPLE] =
    {
        .name = _("{JPN}たんじゅん"),
        .description = COMPOUND_STRING("{JPN}能力変化が ばいになる"),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_DRY_SKIN] =
    {
        .name = _("{JPN}かんそうはだ"),
        .description = COMPOUND_STRING("{JPN}あつさに 弱いが みずで 回復"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_DOWNLOAD] =
    {
        .name = _("{JPN}ダウンロード"),
        .description = COMPOUND_STRING("{JPN}相手 によって 能力を かえる"),
        .aiRating = 7,
    },

    [ABILITY_IRON_FIST] =
    {
        .name = _("{JPN}てつのこぶし"),
        .description = COMPOUND_STRING("{JPN}パンチの 技の 威力が 上がる"),
        .aiRating = 6,
    },

    [ABILITY_POISON_HEAL] =
    {
        .name = _("{JPN}ポイズンヒール"),
        .description = COMPOUND_STRING("{JPN}毒で HPを 回復する"),
        .aiRating = 8,
    },

    [ABILITY_ADAPTABILITY] =
    {
        .name = _("{JPN}てきおうりょく"),
        .description = COMPOUND_STRING("{JPN}同タイプの技が強くなる"),
        .aiRating = 8,
    },

    [ABILITY_SKILL_LINK] =
    {
        .name = _("{JPN}スキルリンク"),
        .description = COMPOUND_STRING("{JPN}連続技の攻撃回数が最大に"),
        .aiRating = 7,
    },

    [ABILITY_HYDRATION] =
    {
        .name = _("{JPN}うるおいボディ"),
        .description = COMPOUND_STRING("{JPN}雨のとき 状態いじょうを なおす"),
        .aiRating = 4,
    },

    [ABILITY_SOLAR_POWER] =
    {
        .name = _("{JPN}サンパワー"),
        .description = COMPOUND_STRING("{JPN}はれで HPがへるが 特攻が上がる"),
        .aiRating = 3,
    },

    [ABILITY_QUICK_FEET] =
    {
        .name = _("{JPN}はやあし"),
        .description = COMPOUND_STRING("{JPN}状態いじょうで 素早さが 上がる"),
        .aiRating = 5,
    },

    [ABILITY_NORMALIZE] =
    {
        .name = _("{JPN}ノーマルスキン"),
        .description = COMPOUND_STRING("{JPN}技が 全て ノーマルタイプに なる"),
        .aiRating = -1,
    },

    [ABILITY_SNIPER] =
    {
        .name = _("{JPN}スナイパー"),
        .description = COMPOUND_STRING("{JPN}急所で 与えるダメージが ふえる"),
        .aiRating = 3,
    },

    [ABILITY_MAGIC_GUARD] =
    {
        .name = _("{JPN}マジックガード"),
        .description = COMPOUND_STRING("{JPN}攻撃以外のダメージを防ぐ"),
        .aiRating = 9,
    },

    [ABILITY_NO_GUARD] =
    {
        .name = _("{JPN}ノーガード"),
        .description = COMPOUND_STRING("{JPN}おたがいの 技が かならず あたる"),
        .aiRating = 8,
    },

    [ABILITY_STALL] =
    {
        .name = _("{JPN}あとだし"),
        .description = COMPOUND_STRING("{JPN}相手より こうどうが おそくなる"),
        .aiRating = -1,
    },

    [ABILITY_TECHNICIAN] =
    {
        .name = _("{JPN}テクニシャン"),
        .description = COMPOUND_STRING("{JPN}威力60以下の技を強くする"),
        .aiRating = 8,
    },

    [ABILITY_LEAF_GUARD] =
    {
        .name = _("{JPN}リーフガード"),
        .description = COMPOUND_STRING("{JPN}はれで 状態いじょうを 防ぐ"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_KLUTZ] =
    {
        .name = _("{JPN}ぶきよう"),
        .description = COMPOUND_STRING("{JPN}もっている 道具を つかえない"),
        .aiRating = -1,
    },

    [ABILITY_MOLD_BREAKER] =
    {
        .name = _("{JPN}かたやぶり"),
        .description = COMPOUND_STRING("{JPN}相手の 特性を 無視する"),
        .aiRating = 7,
    },

    [ABILITY_SUPER_LUCK] =
    {
        .name = _("{JPN}きょううん"),
        .description = COMPOUND_STRING("{JPN}技が 急所に あたりやすくなる"),
        .aiRating = 3,
    },

    [ABILITY_AFTERMATH] =
    {
        .name = _("{JPN}ゆうばく"),
        .description = COMPOUND_STRING("{JPN}たおされると ふれた 相手に ダメージ"),
        .aiRating = 5,
    },

    [ABILITY_ANTICIPATION] =
    {
        .name = _("{JPN}きけんよち"),
        .description = COMPOUND_STRING("{JPN}きけんな 技を さっちする"),
        .aiRating = 2,
    },

    [ABILITY_FOREWARN] =
    {
        .name = _("{JPN}よちむ"),
        .description = COMPOUND_STRING("{JPN}相手の 技を ひとつ よみとる"),
        .aiRating = 2,
    },

    [ABILITY_UNAWARE] =
    {
        .name = _("{JPN}てんねん"),
        .description = COMPOUND_STRING("{JPN}相手の能力変化を無視する"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_TINTED_LENS] =
    {
        .name = _("{JPN}いろめがね"),
        .description = COMPOUND_STRING("{JPN}いまひとつで 与える ダメージがふえる"),
        .aiRating = 7,
    },

    [ABILITY_FILTER] =
    {
        .name = _("{JPN}フィルター"),
        .description = COMPOUND_STRING("{JPN}ばつぐんで 受ける ダメージが へる"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SLOW_START] =
    {
        .name = _("{JPN}スロースタート"),
        .description = COMPOUND_STRING("{JPN}しばらく 攻撃と 素早さが 下がる"),
        .aiRating = -2,
    },

    [ABILITY_SCRAPPY] =
    {
        .name = _("{JPN}きもったま"),
        .description = COMPOUND_STRING("{JPN}ゴーストタイプに 技が あたる"),
        .aiRating = 6,
    },

    [ABILITY_STORM_DRAIN] =
    {
        .name = _("{JPN}よびみず"),
        .description = COMPOUND_STRING("{JPN}みずタイプの 技が 自分にくる"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_ICE_BODY] =
    {
        .name = _("{JPN}アイスボディ"),
        .description = COMPOUND_STRING("{JPN}あられのとき まいターン HPを回復"),
        .aiRating = 3,
    },

    [ABILITY_SOLID_ROCK] =
    {
        .name = _("{JPN}ハードロック"),
        .description = COMPOUND_STRING("{JPN}ばつぐんで 受ける ダメージが へる"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_SNOW_WARNING] =
    {
        .name = _("{JPN}ゆきふらし"),
    #if B_SNOW_WARNING >= GEN_9
        .description = COMPOUND_STRING("{JPN}戦闘に でると ゆきを ふらす"),
    #else
        .description = COMPOUND_STRING("{JPN}戦闘に でると あられを ふらす"),
    #endif
        .aiRating = 8,
    },

    [ABILITY_HONEY_GATHER] =
    {
        .name = _("{JPN}みつあつめ"),
        .description = COMPOUND_STRING("{JPN}あまいミツを ひろってくる ことがある"),
        .aiRating = 0,
    },

    [ABILITY_FRISK] =
    {
        .name = _("{JPN}おみとおし"),
        .description = COMPOUND_STRING("{JPN}相手の持ち物が わかる"),
        .aiRating = 3,
    },

    [ABILITY_RECKLESS] =
    {
        .name = _("{JPN}すてみ"),
        .description = COMPOUND_STRING("{JPN}反動を 受ける技が 強くなる"),
        .aiRating = 6,
    },

    [ABILITY_MULTITYPE] =
    {
        .name = _("{JPN}マルチタイプ"),
        .description = COMPOUND_STRING("{JPN}もっている プレートで タイプが かわる"),
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
        .description = COMPOUND_STRING("{JPN}はれのとき 味方がわが 強くなる"),
        .aiRating = 4,
        .cantBeCopied = TRUE,
        .cantBeTraced = B_UPDATED_ABILITY_DATA >= GEN_5,
        .breakable = TRUE,
    },

    [ABILITY_BAD_DREAMS] =
    {
        .name = _("{JPN}ナイトメア"),
        .description = COMPOUND_STRING("{JPN}ねむっている 相手に ダメージ"),
        .aiRating = 4,
    },

    [ABILITY_PICKPOCKET] =
    {
        .name = _("{JPN}わるいてぐせ"),
        .description = COMPOUND_STRING("{JPN}さわられると相手の道具をぬすむ"),
        .aiRating = 3,
    },

    [ABILITY_SHEER_FORCE] =
    {
        .name = _("{JPN}ちからずく"),
        .description = COMPOUND_STRING("{JPN}追加効果を消し技の威力上昇"),
        .aiRating = 8,
    },

    [ABILITY_CONTRARY] =
    {
        .name = _("{JPN}あまのじゃく"),
        .description = COMPOUND_STRING("{JPN}能力変化が ぎゃくになる"),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_UNNERVE] =
    {
        .name = _("{JPN}きんちょうかん"),
        .description = COMPOUND_STRING("{JPN}相手が きのみを たべられなくなる"),
        .aiRating = 3,
    },

    [ABILITY_DEFIANT] =
    {
        .name = _("{JPN}まけんき"),
        .description = COMPOUND_STRING("{JPN}能力が 下がると 攻撃を上げる"),
        .aiRating = 5,
    },

    [ABILITY_DEFEATIST] =
    {
        .name = _("{JPN}よわき"),
        .description = COMPOUND_STRING("{JPN}ピンチで 攻撃 特攻が下がる"),
        .aiRating = -1,
    },

    [ABILITY_CURSED_BODY] =
    {
        .name = _("{JPN}のろわれボディ"),
        .description = COMPOUND_STRING("{JPN}受けた 技を 封じる ことがある"),
        .aiRating = 4,
    },

    [ABILITY_HEALER] =
    {
        .name = _("{JPN}いやしのこころ"),
        .description = COMPOUND_STRING("{JPN}味方の 状態異常を なおす"),
        .aiRating = 0,
    },

    [ABILITY_FRIEND_GUARD] =
    {
        .name = _("{JPN}フレンドガード"),
        .description = COMPOUND_STRING("{JPN}味方の 受ける ダメージを へらす"),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_WEAK_ARMOR] =
    {
        .name = _("{JPN}くだけるよろい"),
        .description = COMPOUND_STRING("{JPN}物理で 防御が下がるが 早くなる"),
        .aiRating = 2,
    },

    [ABILITY_HEAVY_METAL] =
    {
        .name = _("{JPN}ヘヴィメタル"),
        .description = COMPOUND_STRING("{JPN}体重が おもくなる"),
        .aiRating = -1,
        .breakable = TRUE,
    },

    [ABILITY_LIGHT_METAL] =
    {
        .name = _("{JPN}ライトメタル"),
        .description = COMPOUND_STRING("{JPN}体重が かるくなる"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_MULTISCALE] =
    {
        .name = _("{JPN}マルチスケイル"),
        .description = COMPOUND_STRING("{JPN}HP満タンで 受ける ダメージが へる"),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_TOXIC_BOOST] =
    {
        .name = _("{JPN}どくぼうそう"),
        .description = COMPOUND_STRING("{JPN}どくのとき 攻撃が 上がる"),
        .aiRating = 6,
    },

    [ABILITY_FLARE_BOOST] =
    {
        .name = _("{JPN}ねつぼうそう"),
        .description = COMPOUND_STRING("{JPN}やけどのとき 特攻が 上がる"),
        .aiRating = 5,
    },

    [ABILITY_HARVEST] =
    {
        .name = _("{JPN}しゅうかく"),
        .description = COMPOUND_STRING("{JPN}つかった きのみを 復活させる"),
        .aiRating = 5,
    },

    [ABILITY_TELEPATHY] =
    {
        .name = _("{JPN}テレパシー"),
        .description = COMPOUND_STRING("{JPN}味方の 攻撃が あたらない"),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_MOODY] =
    {
        .name = _("{JPN}ムラっけ"),
        .description = COMPOUND_STRING("{JPN}能力が ランダムに かわる"),
        .aiRating = 10,
    },

    [ABILITY_OVERCOAT] =
    {
        .name = _("{JPN}ぼうじん"),
        .description = COMPOUND_STRING("{JPN}こなと 天気の ダメージを 防ぐ"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_POISON_TOUCH] =
    {
        .name = _("{JPN}どくしゅ"),
        .description = COMPOUND_STRING("{JPN}攻撃した 相手を 毒にする"),
        .aiRating = 4,
    },

    [ABILITY_REGENERATOR] =
    {
        .name = _("{JPN}さいせいりょく"),
        .description = COMPOUND_STRING("{JPN}交代時に 最大HPの1{ENG}/{JPN}3回復"),
        .aiRating = 8,
    },

    [ABILITY_BIG_PECKS] =
    {
        .name = _("{JPN}はとむね"),
        .description = COMPOUND_STRING("{JPN}相手に 防御を さげられない"),
        .aiRating = 1,
        .breakable = TRUE,
    },

    [ABILITY_SAND_RUSH] =
    {
        .name = _("{JPN}すなかき"),
        .description = COMPOUND_STRING("{JPN}すなあらしのとき 素早さが 早くなる"),
        .aiRating = 6,
    },

    [ABILITY_WONDER_SKIN] =
    {
        .name = _("{JPN}ミラクルスキン"),
        .description = COMPOUND_STRING("{JPN}変化技を うけにくい"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_ANALYTIC] =
    {
        .name = _("{JPN}アナライズ"),
        .description = COMPOUND_STRING("{JPN}さいごに 技をだすと 強くなる"),
        .aiRating = 5,
    },

    [ABILITY_ILLUSION] =
    {
        .name = _("{JPN}イリュージョン"),
        .description = COMPOUND_STRING("{JPN}手持ちの ポケモンに ばける"),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_IMPOSTER] =
    {
        .name = _("{JPN}かわりもの"),
        .description = COMPOUND_STRING("{JPN}相手の ポケモンに へんしんする"),
        .aiRating = 9,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_INFILTRATOR] =
    {
        .name = _("{JPN}すりぬけ"),
        .description = COMPOUND_STRING("{JPN}かべやみがわりのえいきょうを受けない"),
        .aiRating = 6,
    },

    [ABILITY_MUMMY] =
    {
        .name = _("{JPN}ミイラ"),
        .description = COMPOUND_STRING("{JPN}ふれられると 特性を ミイラに する"),
        .aiRating = 5,
    },

    [ABILITY_MOXIE] =
    {
        .name = _("{JPN}じしんかじょう"),
        .description = COMPOUND_STRING("{JPN}相手を 倒すと 攻撃が 上がる"),
        .aiRating = 7,
    },

    [ABILITY_JUSTIFIED] =
    {
        .name = _("{JPN}せいぎのこころ"),
        .description = COMPOUND_STRING("{JPN}あく技を 受けると 攻撃が 上がる"),
        .aiRating = 4,
    },

    [ABILITY_RATTLED] =
    {
        .name = _("{JPN}びびり"),
        .description = COMPOUND_STRING("{JPN}ゴーストむしあくで素早さが上がる"),
        .aiRating = 3,
    },

    [ABILITY_MAGIC_BOUNCE] =
    {
        .name = _("{JPN}マジックミラー"),
        .description = COMPOUND_STRING("{JPN}変化技を はねかえす"),
        .aiRating = 9,
        .breakable = TRUE,
    },

    [ABILITY_SAP_SIPPER] =
    {
        .name = _("{JPN}そうしょく"),
        .description = COMPOUND_STRING("{JPN}くさ技を 受けると 攻撃が 上がる"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_PRANKSTER] =
    {
        .name = _("{JPN}いたずらごころ"),
        .description = COMPOUND_STRING("{JPN}変化技を先制して使える"),
        .aiRating = 8,
    },

    [ABILITY_SAND_FORCE] =
    {
        .name = _("{JPN}すなのちから"),
        .description = COMPOUND_STRING("{JPN}すなで 地面 いわ はがねが強くなる"),
        .aiRating = 4,
    },

    [ABILITY_IRON_BARBS] =
    {
        .name = _("{JPN}てつのトゲ"),
        .description = COMPOUND_STRING("{JPN}さわった 相手を 傷つける"),
        .aiRating = 6,
    },

    [ABILITY_ZEN_MODE] =
    {
        .name = _("{JPN}ダルマモード"),
        .description = COMPOUND_STRING("{JPN}ピンチに なると 姿が かわる"),
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
        .description = COMPOUND_STRING("{JPN}味方がわの 命中率が 上がる"),
        .aiRating = 6,
    },

    [ABILITY_TURBOBLAZE] =
    {
        .name = _("{JPN}ターボブレイズ"),
        .description = COMPOUND_STRING("{JPN}相手の 特性を 無視する"),
        .aiRating = 7,
    },

    [ABILITY_TERAVOLT] =
    {
        .name = _("{JPN}テラボルテージ"),
        .description = COMPOUND_STRING("{JPN}相手の 特性を 無視する"),
        .aiRating = 7,
    },

    [ABILITY_AROMA_VEIL] =
    {
        .name = _("{JPN}アロマベール"),
        .description = COMPOUND_STRING("{JPN}味方がわの メンタル技を 防ぐ"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_FLOWER_VEIL] =
    {
        .name = _("{JPN}フラワーベール"),
        .description = COMPOUND_STRING("{JPN}味方の くさの 変化を 防ぐ"),
        .aiRating = 0,
        .breakable = TRUE,
    },

    [ABILITY_CHEEK_POUCH] =
    {
        .name = _("{JPN}ほおぶくろ"),
        .description = COMPOUND_STRING("{JPN}きのみを たべると HPも 回復する"),
        .aiRating = 4,
    },

    [ABILITY_PROTEAN] =
    {
        .name = _("{JPN}へんげんじざい"),
        .description = COMPOUND_STRING("{JPN}使う技と 同じタイプになる"),
        .aiRating = 8,
    },

    [ABILITY_FUR_COAT] =
    {
        .name = _("{JPN}ファーコート"),
        .description = COMPOUND_STRING("{JPN}物理技の ダメージを へらす"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_MAGICIAN] =
    {
        .name = _("{JPN}マジシャン"),
        .description = COMPOUND_STRING("{JPN}攻撃を あてたとき 道具を うばう"),
        .aiRating = 3,
    },

    [ABILITY_BULLETPROOF] =
    {
        .name = _("{JPN}ぼうだん"),
        .description = COMPOUND_STRING("{JPN}たまや ばくだんの 技を 受けない"),
        .breakable = TRUE,
        .aiRating = 7,
    },

    [ABILITY_COMPETITIVE] =
    {
        .name = _("{JPN}かちき"),
        .description = COMPOUND_STRING("{JPN}能力が 下がると 特攻を上げる"),
        .aiRating = 5,
    },

    [ABILITY_STRONG_JAW] =
    {
        .name = _("{JPN}がんじょうあご"),
        .description = COMPOUND_STRING("{JPN}キバの 技の 威力が 上がる"),
        .aiRating = 6,
    },

    [ABILITY_REFRIGERATE] =
    {
        .name = _("{JPN}フリーズスキン"),
        .description = COMPOUND_STRING("{JPN}ノーマル技が こおりになり 強くなる"),
        .aiRating = 8,
    },

    [ABILITY_SWEET_VEIL] =
    {
        .name = _("{JPN}スイートベール"),
        .description = COMPOUND_STRING("{JPN}自分と 味方が ねむらない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_STANCE_CHANGE] =
    {
        .name = _("{JPN}バトルスイッチ"),
        .description = COMPOUND_STRING("{JPN}攻撃じに 姿が かわる"),
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
        .description = COMPOUND_STRING("{JPN}HP満タンなら ひこう技先制"),
        .aiRating = 6,
    },

    [ABILITY_MEGA_LAUNCHER] =
    {
        .name = _("{JPN}メガランチャー"),
        .description = COMPOUND_STRING("{JPN}はどうの 技の 威力が 上がる"),
        .aiRating = 7,
    },

    [ABILITY_GRASS_PELT] =
    {
        .name = _("{JPN}くさのけがわ"),
        .description = COMPOUND_STRING("{JPN}くさむらで 防御が 上がる"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_SYMBIOSIS] =
    {
        .name = _("{JPN}きょうせい"),
        .description = COMPOUND_STRING("{JPN}味方に 道具を わたす"),
        .aiRating = 0,
    },

    [ABILITY_TOUGH_CLAWS] =
    {
        .name = _("{JPN}かたいツメ"),
        .description = COMPOUND_STRING("{JPN}ちょくせつ 攻撃が 強くなる"),
        .aiRating = 7,
    },

    [ABILITY_PIXILATE] =
    {
        .name = _("{JPN}フェアリースキン"),
        .description = COMPOUND_STRING("{JPN}ノーマル技が フェアリーになり 強い"),
        .aiRating = 8,
    },

    [ABILITY_GOOEY] =
    {
        .name = _("{JPN}ぬめぬめ"),
        .description = COMPOUND_STRING("{JPN}ふれた 相手の 素早さを 下げる"),
        .aiRating = 5,
    },

    [ABILITY_AERILATE] =
    {
        .name = _("{JPN}スカイスキン"),
        .description = COMPOUND_STRING("{JPN}ノーマル技が ひこうになり 強くなる"),
        .aiRating = 8,
    },

    [ABILITY_PARENTAL_BOND] =
    {
        .name = _("{JPN}おやこあい"),
        .description = COMPOUND_STRING("{JPN}おやこで 2回 攻撃する"),
        .aiRating = 10,
    },

    [ABILITY_DARK_AURA] =
    {
        .name = _("{JPN}ダークオーラ"),
        .description = COMPOUND_STRING("{JPN}全員の あく技が 強くなる"),
        .aiRating = 6,
        .breakable = B_UPDATED_ABILITY_DATA < GEN_8,
    },

    [ABILITY_FAIRY_AURA] =
    {
        .name = _("{JPN}フェアリーオーラ"),
        .description = COMPOUND_STRING("{JPN}全員の フェアリー技が 強くなる"),
        .aiRating = 6,
        .breakable = B_UPDATED_ABILITY_DATA < GEN_8,
    },

    [ABILITY_AURA_BREAK] =
    {
        .name = _("{JPN}オーラブレイク"),
        .description = COMPOUND_STRING("{JPN}オーラの 効果を ぎゃくにする"),
        .aiRating = 3,
        .breakable = TRUE,
    },

    [ABILITY_PRIMORDIAL_SEA] =
    {
        .name = _("{JPN}はじまりのうみ"),
        .description = COMPOUND_STRING("{JPN}天気を 強いあめに する"),
        .aiRating = 10,
    },

    [ABILITY_DESOLATE_LAND] =
    {
        .name = _("{JPN}おわりのだいち"),
        .description = COMPOUND_STRING("{JPN}日差しを とても 強くする"),
        .aiRating = 10,
    },

    [ABILITY_DELTA_STREAM] =
    {
        .name = _("{JPN}デルタストリーム"),
        .description = COMPOUND_STRING("{JPN}天気を らんきりゅうに する"),
        .aiRating = 10,
    },

    [ABILITY_STAMINA] =
    {
        .name = _("{JPN}じきゅうりょく"),
        .description = COMPOUND_STRING("{JPN}攻撃を 受けると 防御が 上がる"),
        .aiRating = 6,
    },

    [ABILITY_WIMP_OUT] =
    {
        .name = _("{JPN}にげごし"),
        .description = COMPOUND_STRING("{JPN}HPが 半分に なると 逃げだす"),
        .aiRating = 3,
    },

    [ABILITY_EMERGENCY_EXIT] =
    {
        .name = _("{JPN}ききかいひ"),
        .description = COMPOUND_STRING("{JPN}HPが 半分に なると 逃げだす"),
        .aiRating = 3,
    },

    [ABILITY_WATER_COMPACTION] =
    {
        .name = _("{JPN}みずがため"),
        .description = COMPOUND_STRING("{JPN}みずで 防御が ぐーんと 上がる"),
        .aiRating = 4,
    },

    [ABILITY_MERCILESS] =
    {
        .name = _("{JPN}ひとでなし"),
        .description = COMPOUND_STRING("{JPN}どくの 相手には 急所 になる"),
        .aiRating = 4,
    },

    [ABILITY_SHIELDS_DOWN] =
    {
        .name = _("{JPN}リミットシールド"),
        .description = COMPOUND_STRING("{JPN}HPが 半分に なると こわれる"),
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
        .description = COMPOUND_STRING("{JPN}交代した てきに ダメージ2ばい"),
        .aiRating = 6,
    },

    [ABILITY_WATER_BUBBLE] =
    {
        .name = _("{JPN}すいほう"),
        .description = COMPOUND_STRING("{JPN}ほのお技の 威力を 下げる"),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_STEELWORKER] =
    {
        .name = _("{JPN}はがねつかい"),
        .description = COMPOUND_STRING("{JPN}はがねの 技の 威力が 上がる"),
        .aiRating = 6,
    },

    [ABILITY_BERSERK] =
    {
        .name = _("{JPN}ぎゃくじょう"),
        .description = COMPOUND_STRING("{JPN}HPが 半分になると 強くなる"),
        .aiRating = 5,
    },

    [ABILITY_SLUSH_RUSH] =
    {
        .name = _("{JPN}ゆきかき"),
        .description = COMPOUND_STRING("{JPN}あられのとき 素早さが 早くなる"),
        .aiRating = 5,
    },

    [ABILITY_LONG_REACH] =
    {
        .name = _("{JPN}えんかく"),
        .description = COMPOUND_STRING("{JPN}技を せっしょく しないで だす"),
        .aiRating = 3,
    },

    [ABILITY_LIQUID_VOICE] =
    {
        .name = _("{JPN}うるおいボイス"),
        .description = COMPOUND_STRING("{JPN}おと技が みずタイプに なる"),
        .aiRating = 5,
    },

    [ABILITY_TRIAGE] =
    {
        .name = _("{JPN}ヒーリングシフト"),
        .description = COMPOUND_STRING("{JPN}回復技を 先制 できる"),
        .aiRating = 7,
    },

    [ABILITY_GALVANIZE] =
    {
        .name = _("{JPN}エレキスキン"),
        .description = COMPOUND_STRING("{JPN}ノーマル技が でんきになり 強くなる"),
        .aiRating = 8,
    },

    [ABILITY_SURGE_SURFER] =
    {
        .name = _("{JPN}サーフテール"),
        .description = COMPOUND_STRING("{JPN}エレキフィールドで 早くなる"),
        .aiRating = 4,
    },

    [ABILITY_SCHOOLING] =
    {
        .name = _("{JPN}ぎょぐん"),
        .description = COMPOUND_STRING("{JPN}HPが 多いと 強い"),
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
        .description = COMPOUND_STRING("{JPN}攻撃を 1どだけ 防ぐ"),
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
        .description = COMPOUND_STRING("{JPN}相手を 倒すと へんげする"),
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
        .description = COMPOUND_STRING("{JPN}HPが 半分になると 姿をかえる"),
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
        .description = COMPOUND_STRING("{JPN}だれでも 毒に することが できる"),
        .aiRating = 5,
    },

    [ABILITY_COMATOSE] =
    {
        .name = _("{JPN}ぜったいねむり"),
        .description = COMPOUND_STRING("{JPN}夢うつつで ぜったいに めざめない"),
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
        .description = COMPOUND_STRING("{JPN}相手は 先制技を だせない"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_INNARDS_OUT] =
    {
        .name = _("{JPN}とびだすなかみ"),
        .description = COMPOUND_STRING("{JPN}倒されたとき ダメージを 与える"),
        .aiRating = 5,
    },

    [ABILITY_DANCER] =
    {
        .name = _("{JPN}おどりこ"),
        .description = COMPOUND_STRING("{JPN}おどり技に つづいて おどりだす"),
        .aiRating = 5,
    },

    [ABILITY_BATTERY] =
    {
        .name = _("{JPN}バッテリー"),
        .description = COMPOUND_STRING("{JPN}味方の 特殊の 威力を上げる"),
        .aiRating = 0,
    },

    [ABILITY_FLUFFY] =
    {
        .name = _("{JPN}もふもふ"),
        .description = COMPOUND_STRING("{JPN}せっしょく 技を はんげんする"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_DAZZLING] =
    {
        .name = _("{JPN}ビビッドボディ"),
        .description = COMPOUND_STRING("{JPN}相手は 先制技を だせない"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SOUL_HEART] =
    {
        .name = _("{JPN}ソウルハート"),
        .description = COMPOUND_STRING("{JPN}ひんしに なるたびに 特攻が 上がる"),
        .aiRating = 7,
    },

    [ABILITY_TANGLING_HAIR] =
    {
        .name = _("{JPN}カーリーヘアー"),
        .description = COMPOUND_STRING("{JPN}ふれた 相手の 素早さを 下げる"),
        .aiRating = 5,
    },

    [ABILITY_RECEIVER] =
    {
        .name = _("{JPN}レシーバー"),
        .description = COMPOUND_STRING("{JPN}味方の 特性を 受け継ぐ"),
        .aiRating = 0,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_POWER_OF_ALCHEMY] =
    {
        .name = _("{JPN}かがくのちから"),
        .description = COMPOUND_STRING("{JPN}味方の 特性を 受け継ぐ"),
        .aiRating = 0,
        .cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_BEAST_BOOST] =
    {
        .name = _("{JPN}ビーストブースト"),
        .description = COMPOUND_STRING("{JPN}倒したとき 能力が 上がる"),
        .aiRating = 7,
    },

    [ABILITY_RKS_SYSTEM] =
    {
        .name = _("{JPN}ARシステム"),
        .description = COMPOUND_STRING("{JPN}メモリで 自分の タイプが かわる"),
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
        .description = COMPOUND_STRING("{JPN}エレキフィールドを はりめぐらせる"),
        .aiRating = 8,
    },

    [ABILITY_PSYCHIC_SURGE] =
    {
        .name = _("{JPN}サイコメイカー"),
        .description = COMPOUND_STRING("{JPN}サイコフィールドを はりめぐらせる"),
        .aiRating = 8,
    },

    [ABILITY_MISTY_SURGE] =
    {
        .name = _("{JPN}ミストメイカー"),
        .description = COMPOUND_STRING("{JPN}ミストフィールドを はりめぐらせる"),
        .aiRating = 8,
    },

    [ABILITY_GRASSY_SURGE] =
    {
        .name = _("{JPN}グラスメイカー"),
        .description = COMPOUND_STRING("{JPN}グラスフィールドを はりめぐらせる"),
        .aiRating = 8,
    },

    [ABILITY_FULL_METAL_BODY] =
    {
        .name = _("{JPN}メタルプロテクト"),
        .description = COMPOUND_STRING("{JPN}相手に能力を 下げられない"),
        .aiRating = 4,
    },

    [ABILITY_SHADOW_SHIELD] =
    {
        .name = _("{JPN}ファントムガード"),
        .description = COMPOUND_STRING("{JPN}HP満タンで 受ける ダメージが へる"),
        .aiRating = 8,
    },

    [ABILITY_PRISM_ARMOR] =
    {
        .name = _("{JPN}プリズムアーマー"),
        .description = COMPOUND_STRING("{JPN}ばつぐんになる 技を よわめる"),
        .aiRating = 6,
    },

    [ABILITY_NEUROFORCE] =
    {
        .name = _("{JPN}ブレインフォース"),
        .description = COMPOUND_STRING("{JPN}効果ばつぐんで 威力が 上がる"),
        .aiRating = 6,
    },

    [ABILITY_INTREPID_SWORD] =
    {
        .name = _("{JPN}ふとうのけん"),
        .description = COMPOUND_STRING("{JPN}とうじょうじに 攻撃が 上がる"),
        .aiRating = 3,
    },

    [ABILITY_DAUNTLESS_SHIELD] =
    {
        .name = _("{JPN}ふくつのたて"),
        .description = COMPOUND_STRING("{JPN}とうじょうじに 防御が 上がる"),
        .aiRating = 3,
    },

    [ABILITY_LIBERO] =
    {
        .name = _("{JPN}リベロ"),
        .description = COMPOUND_STRING("{JPN}使う技と 同じタイプになる"),
    },

    [ABILITY_BALL_FETCH] =
    {
        .name = _("{JPN}たまひろい"),
        .description = COMPOUND_STRING("{JPN}失敗した ボールを ひろってくる"),
        .aiRating = 0,
    },

    [ABILITY_COTTON_DOWN] =
    {
        .name = _("{JPN}わたげ"),
        .description = COMPOUND_STRING("{JPN}攻撃を 受けると みんなをおそくする"),
        .aiRating = 3,
    },

    [ABILITY_PROPELLER_TAIL] =
    {
        .name = _("{JPN}スクリューおびれ"),
        .description = COMPOUND_STRING("{JPN}技を引き受ける効果を無視する"),
        .aiRating = 2,
    },

    [ABILITY_MIRROR_ARMOR] =
    {
        .name = _("{JPN}ミラーアーマー"),
        .description = COMPOUND_STRING("{JPN}能力ダウンの 効果を はねかえす"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_GULP_MISSILE] =
    {
        .name = _("{JPN}うのミサイル"),
        .description = COMPOUND_STRING("{JPN}うみから えものを くわえてくる"),
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
        .description = COMPOUND_STRING("{JPN}技を引き受ける効果を無視する"),
        .aiRating = 2,
    },

    [ABILITY_STEAM_ENGINE] =
    {
        .name = _("{JPN}じょうききかん"),
        .description = COMPOUND_STRING("{JPN}みずかほのおを受けるとすばやくなる"),
        .aiRating = 3,
    },

    [ABILITY_PUNK_ROCK] =
    {
        .name = _("{JPN}パンクロック"),
        .description = COMPOUND_STRING("{JPN}おと技の 威力が 上がる"),
        .aiRating = 2,
        .breakable = TRUE,
    },

    [ABILITY_SAND_SPIT] =
    {
        .name = _("{JPN}すなはき"),
        .description = COMPOUND_STRING("{JPN}攻撃を受けると すなあらしを おこす"),
        .aiRating = 5,
    },

    [ABILITY_ICE_SCALES] =
    {
        .name = _("{JPN}こおりのりんぷん"),
        .description = COMPOUND_STRING("{JPN}特殊攻撃を はんげん する"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_RIPEN] =
    {
        .name = _("{JPN}じゅくせい"),
        .description = COMPOUND_STRING("{JPN}きのみの 効果が ばいに なる"),
        .aiRating = 4,
    },

    [ABILITY_ICE_FACE] =
    {
        .name = _("{JPN}アイスフェイス"),
        .description = COMPOUND_STRING("{JPN}あたまの こおりが みがわりに なる"),
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
        .description = COMPOUND_STRING("{JPN}自分 以外が 強くなる"),
        .aiRating = 2,
    },

    [ABILITY_MIMICRY] =
    {
        .name = _("{JPN}ぎたい"),
        .description = COMPOUND_STRING("{JPN}フィールドに よって タイプが かわる"),
        .aiRating = 2,
    },

    [ABILITY_SCREEN_CLEANER] =
    {
        .name = _("{JPN}バリアフリー"),
        .description = COMPOUND_STRING("{JPN}全ての バリアを けしさる"),
        .aiRating = 3,
    },

    [ABILITY_STEELY_SPIRIT] =
    {
        .name = _("{JPN}はがねのせいしん"),
        .description = COMPOUND_STRING("{JPN}味方の はがね 攻撃が 強くなる"),
        .aiRating = 2,
    },

    [ABILITY_PERISH_BODY] =
    {
        .name = _("{JPN}ほろびのボディ"),
        .description = COMPOUND_STRING("{JPN}技を受けると 3ターンご ひんしになる"),
        .aiRating = -1,
    },

    [ABILITY_WANDERING_SPIRIT] =
    {
        .name = _("{JPN}さまようたましい"),
        .description = COMPOUND_STRING("{JPN}せっしょくすると特性を入れ替える"),
        .aiRating = 2,
    },

    [ABILITY_GORILLA_TACTICS] =
    {
        .name = _("{JPN}ごりむちゅう"),
        .description = COMPOUND_STRING("{JPN}強くなるが 同じ 技しか だせない"),
        .aiRating = 4,
    },

    [ABILITY_NEUTRALIZING_GAS] =
    {
        .name = _("{JPN}かがくへんかガス"),
        .description = COMPOUND_STRING("{JPN}全ての 特性の 効果が きえる"),
        .aiRating = 5,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_PASTEL_VEIL] =
    {
        .name = _("{JPN}パステルベール"),
        .description = COMPOUND_STRING("{JPN}自分も 味方も 毒に ならない"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_HUNGER_SWITCH] =
    {
        .name = _("{JPN}はらぺこスイッチ"),
        .description = COMPOUND_STRING("{JPN}ターンのおわりに 姿を かえる"),
        .aiRating = 2,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_QUICK_DRAW] =
    {
        .name = _("{JPN}クイックドロウ"),
        .description = COMPOUND_STRING("{JPN}さきに こうどう できることが ある"),
        .aiRating = 4,
    },

    [ABILITY_UNSEEN_FIST] =
    {
        .name = _("{JPN}ふかしのこぶし"),
        .description = COMPOUND_STRING("{JPN}まもりを 無視して 攻撃 する"),
        .aiRating = 6,
    },

    [ABILITY_CURIOUS_MEDICINE] =
    {
        .name = _("{JPN}きみょうなくすり"),
        .description = COMPOUND_STRING("{JPN}味方の 能力を もとにもどす"),
        .aiRating = 3,
    },

    [ABILITY_TRANSISTOR] =
    {
        .name = _("{JPN}トランジスタ"),
        .description = COMPOUND_STRING("{JPN}でんきの 技の 威力が 上がる"),
        .aiRating = 6,
    },

    [ABILITY_DRAGONS_MAW] =
    {
        .name = _("{JPN}りゅうのあぎと"),
        .description = COMPOUND_STRING("{JPN}ドラゴンの 技の 威力が 上がる"),
        .aiRating = 6,
    },

    [ABILITY_CHILLING_NEIGH] =
    {
        .name = _("{JPN}しろのいななき"),
        .description = COMPOUND_STRING("{JPN}相手を 倒すと 攻撃が 上がる"),
        .aiRating = 7,
    },

    [ABILITY_GRIM_NEIGH] =
    {
        .name = _("{JPN}くろのいななき"),
        .description = COMPOUND_STRING("{JPN}相手を 倒すと 特攻が 上がる"),
        .aiRating = 7,
    },

    [ABILITY_AS_ONE_ICE_RIDER] =
    {
        .name = _("{JPN}じんばいったい"),
        .description = COMPOUND_STRING("{JPN}しろのいななきも あわせもつ"),
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
        .description = COMPOUND_STRING("{JPN}くろのいななきも あわせもつ"),
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
        .description = COMPOUND_STRING("{JPN}とれないにおいが 相手に うつる"),
        .aiRating = 5,
    },

    [ABILITY_SEED_SOWER] =
    {
        .name = _("{JPN}こぼれダネ"),
        .description = COMPOUND_STRING("{JPN}技を受けると グラスフィールドに なる"),
        .aiRating = 5,
    },

    [ABILITY_THERMAL_EXCHANGE] =
    {
        .name = _("{JPN}ねつこうかん"),
        .description = COMPOUND_STRING("{JPN}ほのおの 技を 受けると 強くなる"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_ANGER_SHELL] =
    {
        .name = _("{JPN}いかりのこうら"),
        .description = COMPOUND_STRING("{JPN}HPが 半分に なると 強くなる"),
        .aiRating = 3,
    },

    [ABILITY_PURIFYING_SALT] =
    {
        .name = _("{JPN}きよめのしお"),
        .description = COMPOUND_STRING("{JPN}状態いじょうに ならない"),
        .aiRating = 6,
        .breakable = TRUE,
    },

    [ABILITY_WELL_BAKED_BODY] =
    {
        .name = _("{JPN}こんがりボディ"),
        .description = COMPOUND_STRING("{JPN}ほのおの 技で 防御が 上がる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_WIND_RIDER] =
    {
        .name = _("{JPN}かぜのり"),
        .description = COMPOUND_STRING("{JPN}かぜで 攻撃が 上がる"),
        .aiRating = 4,
        .breakable = TRUE,
    },

    [ABILITY_GUARD_DOG] =
    {
        .name = _("{JPN}ばんけん"),
        .description = COMPOUND_STRING("{JPN}いかく されると 強くなる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_ROCKY_PAYLOAD] =
    {
        .name = _("{JPN}いわはこび"),
        .description = COMPOUND_STRING("{JPN}いわの 技の 威力が 上がる"),
        .aiRating = 6,
    },

    [ABILITY_WIND_POWER] =
    {
        .name = _("{JPN}ふうりょくでんき"),
        .description = COMPOUND_STRING("{JPN}かぜで じゅうでん する"),
        .aiRating = 4,
    },

    [ABILITY_ZERO_TO_HERO] =
    {
        .name = _("{JPN}マイティチェンジ"),
        .description = COMPOUND_STRING("{JPN}ひっこむと マイティチェンジ する"),
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
        .description = COMPOUND_STRING("{JPN}ヘイラッシャが いると しれいを だす"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_ELECTROMORPHOSIS] =
    {
        .name = _("{JPN}でんきにかえる"),
        .description = COMPOUND_STRING("{JPN}ダメージを 受けると じゅうでん する"),
        .aiRating = 5,
    },

    [ABILITY_PROTOSYNTHESIS] =
    {
        .name = _("{JPN}こだいかっせい"),
        .description = COMPOUND_STRING("{JPN}はれで 能力が 上がる"),
        .aiRating = 7,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_QUARK_DRIVE] =
    {
        .name = _("{JPN}クォークチャージ"),
        .description = COMPOUND_STRING("{JPN}エレキフィールドで 能力が 上がる"),
        .aiRating = 7,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_GOOD_AS_GOLD] =
    {
        .name = _("{JPN}おうごんのからだ"),
        .description = COMPOUND_STRING("{JPN}変化技を 無効化 する"),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_VESSEL_OF_RUIN] =
    {
        .name = _("{JPN}わざわいのうつわ"),
        .description = COMPOUND_STRING("{JPN}自分以外の 特攻が 下がる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_SWORD_OF_RUIN] =
    {
        .name = _("{JPN}わざわいのつるぎ"),
        .description = COMPOUND_STRING("{JPN}自分以外の 防御が 下がる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_TABLETS_OF_RUIN] =
    {
        .name = _("{JPN}わざわいのおふだ"),
        .description = COMPOUND_STRING("{JPN}自分以外の 攻撃が 下がる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_BEADS_OF_RUIN] =
    {
        .name = _("{JPN}わざわいのたま"),
        .description = COMPOUND_STRING("{JPN}自分以外の 特防が 下がる"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_ORICHALCUM_PULSE] =
    {
        .name = _("{JPN}ひひいろのこどう"),
        .description = COMPOUND_STRING("{JPN}戦闘で 日差しを 強くする"),
        .aiRating = 8,
    },

    [ABILITY_HADRON_ENGINE] =
    {
        .name = _("{JPN}ハドロンエンジン"),
        .description = COMPOUND_STRING("{JPN}エレキフィールドを はりめぐらせる"),
        .aiRating = 8,
    },

    [ABILITY_OPPORTUNIST] =
    {
        .name = _("{JPN}びんじょう"),
        .description = COMPOUND_STRING("{JPN}能力が 上がると 自分も 上がる"),
        .aiRating = 5,
    },

    [ABILITY_CUD_CHEW] =
    {
        .name = _("{JPN}はんすう"),
        .description = COMPOUND_STRING("{JPN}きのみを もう1回 だけ たべる"),
        .aiRating = 4,
    },

    [ABILITY_SHARPNESS] =
    {
        .name = _("{JPN}きれあじ"),
        .description = COMPOUND_STRING("{JPN}きる技の 威力が 上がる"),
        .aiRating = 7,
    },

    [ABILITY_SUPREME_OVERLORD] =
    {
        .name = _("{JPN}そうだいしょう"),
        .description = COMPOUND_STRING("{JPN}倒された 味方の かずで 強くなる"),
        .aiRating = 6,
    },

    [ABILITY_COSTAR] =
    {
        .name = _("{JPN}きょうえん"),
        .description = COMPOUND_STRING("{JPN}味方の 能力を コピーする"),
        .aiRating = 5,
    },

    [ABILITY_TOXIC_DEBRIS] =
    {
        .name = _("{JPN}どくげしょう"),
        .description = COMPOUND_STRING("{JPN}物理技を受けるとどくびし"),
        .aiRating = 4,
    },

    [ABILITY_ARMOR_TAIL] =
    {
        .name = _("{JPN}テイルアーマー"),
        .description = COMPOUND_STRING("{JPN}相手は 先制技を だせない"),
        .aiRating = 5,
        .breakable = TRUE,
    },

    [ABILITY_EARTH_EATER] =
    {
        .name = _("{JPN}どしょく"),
        .description = COMPOUND_STRING("{JPN}地面を 受けると 回復する"),
        .aiRating = 7,
        .breakable = TRUE,
    },

    [ABILITY_MYCELIUM_MIGHT] =
    {
        .name = _("{JPN}きんしのちから"),
        .description = COMPOUND_STRING("{JPN}変化技を 失敗 しない"),
        .aiRating = 2,
    },

    [ABILITY_HOSPITALITY] =
    {
        .name = _("{JPN}おもてなし"),
        .description = COMPOUND_STRING("{JPN}味方の HPを すこし 回復"),
        .aiRating = 5,
    },

    [ABILITY_MINDS_EYE] =
    {
        .name = _("{JPN}しんがん"),
        .description = COMPOUND_STRING("{JPN}ゴーストタイプに 技が あたる"),
        .aiRating = 8,
        .breakable = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_TEAL_MASK] =
    {
        .name = _("{JPN}おもかげやどし"),
        .description = COMPOUND_STRING("{JPN}自分の 素早さを 上げる"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_HEARTHFLAME_MASK] =
    {
        .name = _("{JPN}おもかげやどし"),
        .description = COMPOUND_STRING("{JPN}自分の 攻撃を 上げる"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_WELLSPRING_MASK] =
    {
        .name = _("{JPN}おもかげやどし"),
        .description = COMPOUND_STRING("{JPN}自分の 特防を 上げる"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_EMBODY_ASPECT_CORNERSTONE_MASK] =
    {
        .name = _("{JPN}おもかげやどし"),
        .description = COMPOUND_STRING("{JPN}自分の 防御を 上げる"),
        .aiRating = 6,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .failsOnImposter = TRUE,
    },

    [ABILITY_TOXIC_CHAIN] =
    {
        .name = _("{JPN}どくのくさり"),
        .description = COMPOUND_STRING("{JPN}猛毒に することが ある"),
        .aiRating = 8,
    },

    [ABILITY_SUPERSWEET_SYRUP] =
    {
        .name = _("{JPN}かんろなミツ"),
        .description = COMPOUND_STRING("{JPN}相手の 回避率を 下げる"),
        .aiRating = 5,
    },

    [ABILITY_TERA_SHIFT] =
    {
        .name = _("{JPN}テラスチェンジ"),
        .description = COMPOUND_STRING("{JPN}テラスタルフォルムに 変化 する"),
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
        .description = COMPOUND_STRING("{JPN}HP満タンなら 全て今ひとつ"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .breakable = TRUE,
    },

    [ABILITY_TERAFORM_ZERO] =
    {
        .name = _("{JPN}ゼロフォーミング"),
        .description = COMPOUND_STRING("{JPN}天気と フィールドを 全てゼロにする"),
        .aiRating = 10,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_POISON_PUPPETEER] =
    {
        .name = _("{JPN}どくくぐつ"),
        .description = COMPOUND_STRING("{JPN}毒に なった 相手は 混乱する"),
        .aiRating = 8,
        .cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },

    [ABILITY_PIERCING_DRILL] =
    {
        .name = _("{JPN}かんつうドリル"),
        .description = COMPOUND_STRING("{JPN}まもりを 無視して 攻撃 する"),
    },

    [ABILITY_DRAGONIZE] =
    {
        .name = _("{JPN}ドラゴンスキン"),
        .description = COMPOUND_STRING("{JPN}ノーマル技が ドラゴンになり 強い"),
    },

    [ABILITY_EELEVATE] =
    {
        .name = _("うなぎのぼり"),
        .description = COMPOUND_STRING("{JPN}倒すと 能力上がる ういてる"),
    },

    [ABILITY_314] =
    {
        .name = _("{JPN}-------"),
        .description = COMPOUND_STRING("{JPN}特性 なし"),
    },

    [ABILITY_MEGA_SOL] =
    {
        .name = _("{JPN}メガソーラー"),
        .description = COMPOUND_STRING("{JPN}にほんばれの 状態で 技を使う"),
    },

    [ABILITY_FIRE_MANE] =
    {
        .name = _("ほのおのたてがみ"),
        .description = COMPOUND_STRING("{JPN}ほのお技が 強くなる"),
    },

    [ABILITY_317] =
    {
        .name = _("{JPN}-------"),
        .description = COMPOUND_STRING("{JPN}特性 なし"),
    },

    [ABILITY_SPICY_SPRAY] =
    {
        .name = _("{JPN}とびだすハバネロ"),
        .description = COMPOUND_STRING("{JPN}ダメージを 受けたとき やけど させる"),
    },

    [ABILITY_AURA_GUARD] =
    {
        .name = _("{JPN}オーラガード"),
        .description = COMPOUND_STRING("{JPN}まだ 実装されていない"),
    },
};
