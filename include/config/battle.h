#ifndef GUARD_CONFIG_BATTLE_H
#define GUARD_CONFIG_BATTLE_H

// 計算の設定。
#define B_CRIT_CHANCE               GEN_LATEST // 急所に当たる確率。CalcCritChanceStageを参照。第6世代以降では急所ランク3以上で必ず急所に当たる。第1世代では使用者の素早さ種族値の半分を基準にする。
#define B_CRIT_MULTIPLIER           GEN_LATEST // 第6世代以降では、急所のダメージ倍率が2倍から1.5倍になる。
#define B_PARALYSIS_SPEED           GEN_LATEST // 第7世代以降では、まひによる素早さ低下が75%から50%になる。
#define B_CONFUSION_SELF_DMG_CHANCE GEN_LATEST // 第7世代以降では、こんらんで自分を攻撃する確率が1/2から1/3になる。
#define B_MULTI_HIT_CHANCE          GEN_LATEST // 第5世代以降では、連続技の攻撃回数ごとの確率が変わる。値はSetRandomMultiHitCounterを参照。
#define B_WHITEOUT_MONEY            GEN_LATEST // 第4世代以降では、敗北時に失うお金をバッジ数から計算する。以前は所持金の半分を失う。FRLGにもある仕様だが、簡略化のためGEN_3ではRSEの挙動を使う。
#define B_LIGHT_BALL_ATTACK_BOOST   GEN_LATEST // 第4世代以降では、でんきだまで特攻に加えて攻撃も2倍になる。
#define B_PARALYSIS_CHANCE          GEN_LATEST // Championsでは、まひで動けない確率が25%から12.5%になる。

// 経験値の設定。
#define B_EXP_CATCH                 GEN_LATEST // 第6世代以降では、ポケモンを捕獲したときにも手持ちが経験値を得る。
#define B_TRAINER_EXP_MULTIPLIER    GEN_LATEST // 第7世代以降では、トレーナー戦の経験値を1.5倍にする補正がなくなる。
#define B_SPLIT_EXP                 GEN_LATEST // 第6世代以降では、戦闘参加者全員が全量の経験値を得る。第5世代以前では参加者に均等分配する。
#define B_SCALED_EXP                GEN_LATEST // 第5世代と第7世代以降では、味方と相手のレベル差によって経験値を補正する。
#define B_UNEVOLVED_EXP_MULTIPLIER  GEN_LATEST // 第6世代以降では、進化可能なレベル以上でも未進化の場合、経験値が約1.2倍になる。EVO_LEVELの進化方法を持つ種族だけが対象。
#define B_LEVEL_UP_NOTIFICATION     GEN_LATEST // 第9世代以降では、一度に複数レベル上がっても通知メッセージを1回だけ表示する。

// 能力値の設定。
#define B_BADGE_BOOST               GEN_LATEST // 第4世代以降では、ジムバッジによる能力値補正がなくなる。GEN_2にしても、ジムと同じタイプへの追加補正は含まない。
#define B_FRIENDSHIP_BOOST          FALSE      // LGPE限定の、なつき度に応じてHP以外を最大10%上げる補正。B_BADGE_BOOSTと異なり、元の能力値の計算時に反映する。
#define B_MAX_LEVEL_EV_GAINS        GEN_LATEST // 第5世代以降では、レベル100でも戦闘で努力値を得られる。
#define B_RECALCULATE_STATS         GEN_LATEST // 第5世代以降では、レベルアップやボックスへの預け入れ時だけでなく、戦闘終了時に参加ポケモンの能力値を再計算する。

// ダメージの設定。
#define B_BURN_DAMAGE               GEN_LATEST // 第1世代と第7世代以降では、やけどのダメージが最大HPの1/8から1/16になる。しもやけにも適用する。
#define B_BURN_FACADE_DMG           GEN_LATEST // 第6世代以降では、からげんきにやけどの攻撃半減を適用しない。
#define B_BINDING_DAMAGE            GEN_LATEST // 第6世代以降では、拘束ダメージが最大HPの1/16から1/8になる。しめつけバンド所持時は1/8から1/6になる。
#define B_PSYWAVE_DMG               GEN_LATEST // サイコウェーブのダメージ計算式。詳細はDoFixedDamageMoveCalcを参照。
#define B_PAYBACK_SWITCH_BOOST      GEN_LATEST // 第5世代以降では、相手の交代時にしっぺがえしの威力が2倍にならない。
#define B_HIDDEN_POWER_DMG          GEN_LATEST // 第6世代以降では、めざめるパワーの威力を60に固定する。以前は個体値から決める。
#define B_ROUGH_SKIN_DMG            GEN_LATEST // 第4世代以降では、さめはだの接触ダメージが最大HPの1/16から1/8になる。てつのトゲにも適用する。
#define B_KNOCK_OFF_DMG             GEN_LATEST // 第6世代以降では、対象が取り除ける持ち物を持っていると、はたきおとすのダメージが50%増える。
#define B_SPORT_DMG_REDUCTION       GEN_LATEST // 第5世代以降では、みずあそび・どろあそびによるほのお・でんき技のダメージ軽減が50%から67%になる。
#define B_EXPLOSION_DEFENSE         GEN_LATEST // 第5世代以降では、じばく・だいばくはつで対象の防御を半分として計算しない。
#define B_PARENTAL_BOND_DMG         GEN_LATEST // 第7世代以降では、おやこあいの2撃目のダメージが初撃の50%から25%になる。
#define B_MULTIPLE_TARGETS_DMG      GEN_LATEST // 第4世代以降では、複数対象技のダメージが75%になる。以前は50%で、場全体を攻撃する技は軽減しない。
#define B_STRUGGLE_RECOIL           GEN_LATEST // 第4世代以降では、わるあがきの反動は使用者の最大HPの1/4。第2～3世代では与ダメージの1/4、第1世代では与ダメージの1/2。
#define B_RETURN_FRUSTRATION_DMG    GEN_LATEST // 第3世代以降では、おんがえし・やつあたりは最低1ダメージを与える。第2世代では威力0になるなつき度だとダメージを与えない。

// タイプの設定。
#define B_GHOSTS_ESCAPE             GEN_LATEST // 第6世代以降では、ゴーストタイプに逃走・交代を防ぐ効果が効かず、野生戦からの逃走も必ず成功する。
#define B_PARALYZE_ELECTRIC         GEN_LATEST // 第6世代以降では、でんきタイプはまひしない。
#define B_POWDER_GRASS              GEN_LATEST // 第6世代以降では、くさタイプに粉・胞子の技が効かない。
#define B_UPDATED_TYPE_MATCHUPS     GEN_LATEST // タイプ相性を変更する。詳細はsrc/data/types_info.hを参照。
#define B_PRANKSTER_DARK_TYPES      GEN_LATEST // 第7世代以降では、いたずらごころで優先度が上がった変化技はあくタイプに効かない。
#define B_SHEER_COLD_IMMUNITY       GEN_LATEST // 第7世代以降では、こおりタイプにぜったいれいどが効かない。
#define B_ROOST_PURE_FLYING         GEN_LATEST // 第5世代以降では、ひこう単タイプがはねやすめを使うとノーマルタイプになる。
#define B_STATUS_TYPE_IMMUNITY      GEN_LATEST // 第1世代では、攻撃技と同じタイプを持つポケモンに、その技の追加効果が効かない。
#define B_FIXED_DMG_IGNORES_TYPE    GEN_LATEST // 第1世代では、固定ダメージ技（ソニックブーム・りゅうのいかり・いかりのまえば・サイコウェーブ・ちきゅうなげ・ナイトヘッド・がまん・カウンター）がタイプ相性と無効を無視する。

// ターンの設定。
#define B_BINDING_TURNS             GEN_LATEST // 第5世代以降では、拘束技の持続が2～5ターンから4～5ターンになる。ねばりのかぎづめ所持時は5ターンから7ターンになる。
#define B_UPROAR_TURNS              GEN_LATEST // 第5世代以降では、さわぐの持続が2～5ターンから3ターンになる。
#define B_UPROAR_IGNORE_SOUNDPROOF  GEN_LATEST // 第5世代以降では、さわぐの効果がぼうおんを無視する。
#define B_DISABLE_TURNS             GEN_LATEST // かなしばりの持続ターン。Cmd_disablelastusedattackを参照。
#define B_TAILWIND_TURNS            GEN_LATEST // 第5世代以降では、おいかぜの持続が3ターンから4ターンになる。
#define B_SLEEP_TURNS               GEN_LATEST // 第5世代以降では、ねむりで動けない期間は1～3ターン。第3～4世代は1～4ターン、第2世代は1～7ターン。
#define B_TAUNT_TURNS               GEN_LATEST // 第5世代以降では、ちょうはつは対象より先に使えば3ターン、対象の行動後なら4ターン続く。第4世代は2～4/3～5ターン、第3世代は2/3ターン。
#define B_ENCORE_TURNS              GEN_LATEST // 第5世代以降では、アンコールは対象が未行動なら3ターン、行動済みなら4ターン続く。第4世代は3～7ターン、第2～3世代は2～6ターン。
#define B_SPORT_TURNS               GEN_LATEST // 第6世代以降では、みずあそび・どろあそびは使用者が交代しても5ターン続く。第5世代以前は使用者が交代するまで続く。
#define B_MEGA_EVO_TURN_ORDER       GEN_LATEST // 第7世代では、行動順の判定にメガシンカ前ではなくメガシンカ後の素早さを使う。
#define B_RECALC_TURN_AFTER_ACTIONS GEN_LATEST // 第8世代以降では、交代や技の使用後に、そのターンの行動順を再計算する（動的な素早さ判定）。
#define B_FAINT_SWITCH_IN           GEN_LATEST // 第4世代以降では、ひんし後の補充はターン終了時に行う。以前は各行動の直後に行う。
#define B_MEGA_EVO_SPEED_SWAP       GEN_LATEST // Championsでは、スピードスワップで交換した素早さをメガシンカで上書きしない。
#define B_FREEZE_TURNS              GEN_LATEST // Championsより前では、こおりが解ける確率は20%。
                                               // Championsでは25%の確率で解け、持続は最大3ターンになる。

// 技データの設定。
#define B_UPDATED_MOVE_DATA         GEN_LATEST // gMovesInfoの威力・命中率・PP・分類・対象範囲・追加効果などを世代に合わせて更新する。
#define B_UPDATED_MOVE_TYPES        GEN_LATEST // gMovesInfoの技タイプを世代に合わせて更新する。
#define B_UPDATED_MOVE_FLAGS        GEN_LATEST // gMovesInfoの技フラグを世代に合わせて更新する。
#define B_PHYSICAL_SPECIAL_SPLIT    GEN_LATEST // 第1～3世代では、技タイプで物理・特殊を決める。ステータス画面の分類アイコンにも反映する。
#define B_RECOIL_IF_MISS_DMG        GEN_LATEST // 攻撃失敗時の反動ダメージの計算式。EFFECT_RECOIL_IF_MISSを参照。
#define B_KLUTZ_FLING_INTERACTION   GEN_LATEST // 第5世代以降では、ぶきようのポケモンはなげつけるを使えない。
#define B_UPDATED_CONVERSION        GEN_LATEST // 第6世代以降では、テクスチャーで最初の技と同じタイプになる。第5世代以前はランダムな技のタイプを選ぶ。
#define B_UPDATED_CONVERSION_2      GEN_LATEST // 第5世代以降では、テクスチャー2は選んだ対象の最後の技を軽減するタイプに変える。第4世代以前は、自分が最後に受けた技を参照し、わるあがきをノーマル技として扱う。
#define B_PP_REDUCED_BY_SPITE       GEN_LATEST // 第4世代以降では、うらみで対象の最後の技のPPを減らす量が2～5から4になる。
#define B_EXTRAPOLATED_MOVE_FLAGS   TRUE       // 公式には付いていないが、最新の本編作品に登場していれば付くと考えられる技フラグを追加する。
#define B_HIDDEN_POWER_COUNTER      GEN_LATEST // 第3世代以前では、カウンター・ミラーコートはめざめるパワーをタイプにかかわらず物理技として扱う。
#define B_MODERN_TRICK_CHOICE_LOCK  GEN_LATEST // 第5世代以降では、こだわり系の持ち物同士を交換した場合、トリック・すりかえの使用者は次に別の技を選んでから固定される。
#define B_PROTECT_FAILURE_RATE      GEN_LATEST // 第5世代以降では、防御技を連続使用するごとに成功率を1/3にする（以前は1/2）。

// 特性データの設定。
#define B_UPDATED_ABILITY_DATA      GEN_LATEST // 特性フラグを世代に合わせて変更する。

// 技の命中判定の設定。
#define B_TOXIC_NEVER_MISS          GEN_LATEST // 第6世代以降では、どくタイプが使うどくどくは命中判定を省略する。
#define B_MINIMIZE_DMG_ACC          GEN_LATEST // 第6世代以降では、ちいさくなる状態に2倍のダメージを与える技は命中判定を省略する。
#define B_BLIZZARD_HAIL             GEN_LATEST // 第4世代以降では、あられ中のふぶきは命中判定を省略する。
#define B_SHEER_COLD_ACC            GEN_LATEST // 第7世代以降では、こおりタイプ以外が使うぜったいれいどの命中率が20%になる。

// 技による能力変化の設定。
#define B_FELL_STINGER_STAT_RAISE   GEN_LATEST // 第7世代以降では、とどめばりで相手を倒した場合、攻撃が2段階ではなく3段階上がる。
#define B_KINGS_SHIELD_LOWER_ATK    GEN_LATEST // 第8世代以降では、キングシールドに攻撃したポケモンの攻撃低下が2段階から1段階になる。
#define B_SPEED_BUFFING_RAPID_SPIN  GEN_LATEST // 第8世代以降では、こうそくスピンが命中すると使用者の素早さが1段階上がる。
#define B_CHARGE_SPDEF_RAISE        GEN_LATEST // 第5世代以降では、じゅうでんで特防が1段階上がる。
#define B_MINIMIZE_EVASION          GEN_LATEST // 第5世代以降では、ちいさくなるの回避率上昇が1段階から2段階になる。
#define B_GROWTH_STAT_RAISE         GEN_LATEST // 第5世代以降では、せいちょうで特攻に加えて攻撃も上がる。通常は各1段階、晴れなら各2段階。
#define B_FOCUS_ENERGY_CRIT_RATIO   GEN_LATEST // 第3世代以降では、きあいだめの急所ランク上昇が1段階から2段階になる。
#define B_PSYCH_UP_CRIT_RATIO       GEN_LATEST // 第6世代以降では、じこあんじで対象の急所ランクもコピーする。

// その他の技の設定。
#define B_INCINERATE_GEMS               GEN_LATEST // 第6世代以降では、やきつくすでジュエルを破壊できる。
#define B_CAN_SPITE_FAIL                GEN_LATEST // 第4世代以降では、対象の最後の技の残りPPが1でもうらみが成功する。
#define B_CRASH_IF_TARGET_IMMUNE        GEN_LATEST // 第4世代以降では、失敗時に反動がある技は、タイプによって無効化された場合も反動を受ける。
#define B_MEMENTO_FAIL                  GEN_LATEST // 第4世代以降では、対象の攻撃・特攻が両方-6でもおきみやげが成功する。第5世代以降では、対象不在・まもる・みがわりの場合は失敗する。
#define B_PARTING_SHOT_SWITCH           GEN_LATEST // 第7世代以降では、すてゼリフで対象の能力を下げられなかった場合、使用者は交代しない。
#define B_BATON_PASS_TRAPPING           GEN_LATEST // 第5世代以降では、バトンタッチによる交代時に、他のポケモンへの交代封じを維持しない。
#define B_GLARE_GHOST                   GEN_LATEST // 第4世代以降では、へびにらみがゴーストタイプにも当たる。
#define B_SKILL_SWAP                    GEN_LATEST // 第4世代以降では、スキルスワップ後に登場時発動の特性を発動する。
#define B_BRICK_BREAK                   GEN_LATEST // 第4世代以降では、かわらわりで自分側の壁も壊せる。第5世代以降では、タイプで無効化された場合は壁を壊さない。
#define B_WISH_HP_SOURCE                GEN_LATEST // 第5世代以降では、ねがいごとの回復量は受け手ではなく使用者の最大HPの半分になる。
#define B_RAMPAGE_CONFUSION             GEN_LATEST // 第5世代以降では、あばれる系の技の解除をターン終了時ではなく技の直後に判定する。最終ターン以外で技が失敗すると持続カウンターを解除する。
#define B_HEAL_BLOCKING                 GEN_LATEST // 第5世代以降では、かいふくふうじでくろいヘドロ・たべのこし・かいがらのすずの回復を防ぐ。HP回復きのみ・きのみジュースを消費せず、回復特性は回復しないがダメージを防ぐ。
                                                   // 第6世代以降では、Zワザ以外のHP吸収技も使用できなくなる。
#define B_ROOTED_GROUNDING              GEN_LATEST // 第4世代以降では、ねをはるの使用者が地面に接地する。
#define B_METRONOME_MOVES               GEN_LATEST // ゆびをふるで呼び出せる技の上限世代を指定する。指定世代の技も含む。
#define B_TELEPORT_BEHAVIOR             GEN_LATEST // LGPE以降（この設定では第8世代以降）では、テレポートは逃走ではなく交代する技になる。
#define B_BEAT_UP                       GEN_LATEST // 第5世代以降では、ふくろだたきの計算式が変わり、あくタイプのダメージを与える。第4世代以前は各攻撃で手持ちの名前を表示する。
#define B_DARK_VOID_FAIL                GEN_6 // 第7世代以降では、ダークホールはダークライだけが使える。
#define B_HIT_THAW                      GEN_LATEST // 第6世代以降では、使用者のこおりを解く攻撃技は対象のこおりも解く。第3世代以降ではほのお技がこおりを解く。第1～2世代では、やけどの追加効果がある攻撃技で、やけど可能かに関係なくこおりが解ける。
#define B_HEALING_WISH_SWITCH           GEN_LATEST // 第5世代以降では、いやしのねがい・みかづきのまいの受け手をターン終了時に出す。第8世代以降では、傷ついた・状態異常のポケモンを出すまで効果を保持する。
#define B_DEFOG_EFFECT_CLEARING         GEN_LATEST // 第5世代以降では、みがわり状態の対象にきりばらいで回避率を下げない。第6世代以降では自分側の設置技も除去し、第8世代以降ではフィールドも解除する。
#define B_HAZE_FOCUS_ENERGY             GEN_LATEST // 第1・第4世代では、くろいきりできあいだめの効果も解除する。
#define B_AUTOTOMIZE_FORM_CHANGE        GEN_LATEST // 第6世代以降では、フォルム変更でボディパージの軽量化をリセットする。
#define B_STOCKPILE_RAISES_DEFS         GEN_LATEST // 第4世代以降では、たくわえるで防御・特防も上がる。はきだす・のみこむを使うと、その能力上昇を失う。
#define B_TRANSFORM_SEMI_INV_FAIL       GEN_LATEST // 第2世代以降では、そらをとぶなどで一時的に攻撃を受けない対象へのへんしんは失敗する。
#define B_TRANSFORM_TARGET_FAIL         GEN_LATEST // 第2世代以降では、すでにへんしんしている対象へのへんしんは失敗する。
#define B_TRANSFORM_USER_FAIL           GEN_LATEST // 第5世代以降では、すでにへんしんしている使用者のへんしんは失敗する。
#define B_TRANSFORM_SUBSTITUTE_FAIL     GEN_LATEST // 第5世代以降では、みがわり状態の対象へのへんしんは失敗する。
#define B_TRANSFORM_SHINY               GEN_LATEST // 第4世代以降では、へんしんで自分の色違い状態を維持せず、対象の色違い状態をコピーする。
#define B_TRANSFORM_BATTLE_REWARDS      GEN_LATEST // 第3～4世代では、へんしんしたポケモンを倒すと変身先の種族の経験値・努力値を得る。他の世代では元の種族を使う。
#define B_TRANSFORM_CATCH_RATE          GEN_LATEST // 第3～4世代では、へんしんしたポケモンの捕獲率は変身先の種族の値になる。他の世代では元の捕獲率を維持する。
#define B_TRANSFORM_FORM_CHANGES        GEN_LATEST // 第5世代以降では、へんしん中のポケモンはフォルムを変更できない。
#define B_WIDE_GUARD                    GEN_LATEST // 第5世代のみ、ワイドガードは連続使用すると失敗する可能性がある。
#define B_QUICK_GUARD                   GEN_LATEST // 第5世代のみ、ファストガードは連続使用すると失敗する可能性がある。
#define B_IMPRISON                      GEN_LATEST // 第5世代以降では、相手が使用者と同じ技を持たなくてもふういんが成功する。
#define B_TAUNT_ME_FIRST                GEN_LATEST // 第5世代以降では、ちょうはつでさきどりを防げない。
#define B_ALLY_SWITCH_FAIL_CHANCE       GEN_8 // 第9世代以降では、サイドチェンジは連続使用すると失敗する可能性がある。
#define B_SKETCH_BANS                   GEN_LATEST // 第9世代以降では、スケッチでコピーできない技が増える。
#define B_KNOCK_OFF_REMOVAL             GEN_LATEST // 第5世代以降では、はたきおとすは持ち物を使用不能にするのではなく取り除く。
                                                   // Championsでは、対象の持ち物で使用者のフォルムが変わるか（メガシンカ・ゲンシカイキ）を確認しない。
#define B_HEAL_BELL_SOUNDPROOF          GEN_LATEST // 第6世代以降では、場にいないぼうおんの味方にもいやしのすずが効く。第5世代は場にいる味方も含め全員、第4世代以前はぼうおんには効かない。
#define B_CHARGE                        GEN_LATEST // 第7世代以前では、次の技のタイプにかかわらずじゅうでん状態を失う。
#define B_POWDER_STATUS_HEAVY_RAIN      GEN_LATEST // 第7世代以降では、大雨中にほのお技を使ってもふんじんのダメージを受けない。
#define B_AFTER_YOU_TURN_ORDER          GEN_LATEST // 第8世代以降では、おさきにどうぞで行動順が変わらなくても失敗しない。
#define B_QUASH_TURN_ORDER              GEN_LATEST // 第8世代以降では、さきおくりを受けたポケモン同士は素早さ順で行動する。第7世代以前はさきおくりを受けた順に行動する。
#define B_DESTINY_BOND_FAIL             GEN_LATEST // 第7世代以降では、みちづれを連続使用すると失敗する。
#define B_FORESIGHT_FAIL                GEN_LATEST // 第2世代と第5世代以降では、すでにみやぶる状態の対象に使うと失敗する。
#define B_MIRACLE_EYE_FAIL              GEN_LATEST // 第5世代以降では、すでにミラクルアイ状態の対象に使うと失敗する。
#define B_PURSUIT_TARGET                GEN_LATEST // 第4世代以降では、おいうちは交代する相手を自動で対象にする。以前は交代する相手を対象に選んだ場合のみ攻撃する。
#define B_SKIP_RECHARGE                 GEN_LATEST // 第1世代のみ、反動で動けなくなる技で対象を倒した場合、次のターンも動ける。
#define B_ENCORE_TARGET                 GEN_LATEST // 第5世代以降では、アンコール中でも技の対象を選べる。
#define B_TIME_OF_DAY_HEALING_MOVES     GEN_LATEST // 第2世代のみ、あさのひざし・こうごうせい・つきのひかりの回復量が、それぞれ朝・昼・夜に2倍になる。
                                                   // OW_TIMES_OF_DAYがGEN_3なら、あさのひざしは朝ではなく昼に強化する。GEN_4以上なら、つきのひかりは夕方にも強化する。
#define B_DREAM_EATER_LIQUID_OOZE       GEN_LATEST // 第5世代以降では、ゆめくいがヘドロえきの影響を受ける。
#define B_DREAM_EATER_SUBSTITUTE        GEN_LATEST // 第5世代以降では、ゆめくいがみがわりにも命中し、HPを吸収できる。
#define B_SNATCH                        GEN_LATEST // 第5世代以降では、同じターンに他のポケモンのよこどりですでに奪われた技を、さらによこどりしない。
#define B_FOCUS_PUNCH_FAILURE           GEN_LATEST // きあいパンチの集中切れ判定。第7世代以降は現在の技と選択した技の両方、第5～6世代は選択した技、第4世代以前は現在の技がきあいパンチかを調べる。
                                                   // 第5世代以降では、PP消費やひるみなどより前に失敗判定を行う。第4世代以前はそれらの後に行う。
#define B_COUNTER_MIRROR_COAT_ALLY      GEN_LATEST // 第5世代以降では、味方からの攻撃をカウンター・ミラーコート・メタルバーストの対象に含めない。第4世代以前は最後に受けた攻撃が味方からなら失敗する。
#define B_COUNTER_TRY_HIT_PARTNER       GEN_LATEST // 第5世代以降では、最後に攻撃した相手が場にいなければ、カウンター・ミラーコート・メタルバーストの対象をその相方へ変える。第4世代以前は失敗する。
#define B_RAGE_BUILDS                   GEN_LATEST // 第4世代以降では、いかりの効果は命中した場合だけ有効になる。第3世代では外れたり失敗したりしても有効になる。
#define B_CHECK_USER_FAILURE            GEN_LATEST // 第5世代以降では、技の使用者自身の無効化要因を判定しない。例えば、ぼうおんでも自分のほろびのうたを防がない。
#define B_ABSORB_MESSAGE                GEN_LATEST // 第5世代以降では、使用者のHPが満タンならHP吸収メッセージを表示しない。
#define B_UPROAR                        GEN_LATEST // 第5世代以降では、さわぐが成功すると初ターンに全員を起こす。第3～4世代では、各自の行動前かターン終了時に起こす。
#define B_FIRST_TURN_MOVE               GEN_LATEST // Championsでは、登場した最初のターン以外にねこだまし・であいがしらを選択できない。
#define B_SALT_CURE_DAMAGE              GEN_LATEST // 第9世代では、しおづけのダメージは最大HPの1/8（はがね・みずは1/4）。
                                                   // Championsでは、最大HPの1/16（はがね・みずは1/8）になる。
#define B_BELCH_SELECTABLE              GEN_LATEST // 第6～9世代では、きのみを食べていないとゲップを選択できない。
                                                   // Championsでは、きのみを食べたかどうかに関係なく選択できる。
#define B_STUFF_CHEEKS_SELECTABLE       GEN_LATEST // 第8～9世代では、きのみを持っていないとほおばるを選択できない。
                                                   // Championsでは、きのみ所持の有無に関係なく選択できる。
#define B_SPIT_UP_SELECTABLE            GEN_LATEST // Championsでは、たくわえるの蓄積がないとはきだすを選択できない。
#define B_MOVES_THAT_REMOVE_TYPE        GEN_LATEST // Championsでは、ほのおタイプでないともえつきるを選択できない。
#define B_LAST_RESORT_SELECTABLE        GEN_LATEST // Championsでは、他の技をすべて使っていないととっておきを選択できない。
#define B_FAINT_MOVE_EFFECT_TIMING      GEN_LATEST // Championsでは、さめはだ・てつのトゲなどで使用者が倒れても一部の技効果が発動する。
                                                   // 対象: はたきおとす・どろぼう・こうそくスピン・キラースピン・アイススピナー。
                                                   // 対象外: ひけん・ちえなみ・がんせきアックス。
#define B_SHEER_FORCE_AGAINST_ABILITIES GEN_LATEST // Championsでは、ちからずくがぎゃくじょう・わるいてぐせの発動を防がない。
                                                   // だっしゅつパック・だっしゅつボタンなどの処理順を確定できる情報が不足しているため、現時点では効果がない。
#define B_ENCORE_PRIORITY               GEN_LATEST // Championsでは、選択した技ではなく、アンコールで固定された技の優先度を使う。
                                                   // 第9世代以前では、選択した技の優先度でアンコールの固定技を使う。
#define B_RAGE_FIST                     GEN_LATEST // Championsでは、交代・ひんしでふんどのこぶしの蓄積をリセットする。
#define B_MOVE_EFFECTS_BEFORE_MOVES     GEN_LATEST // Championsでは、別の技からアンコールでくちばしキャノンに固定された場合も効果が働く。
                                                   // この場合も準備アニメーションを行う。

// 特性の設定。
#define B_GALE_WINGS                    GEN_6 // 第7世代以降では、はやてのつばさの発動にはHP満タンが必要。
#define B_STANCE_CHANGE_FAIL            GEN_6 // 第7世代以降では、こんらん・まひなどで技を使えない場合、バトルスイッチが発動しない。
#define B_SHADOW_TAG_ESCAPE             GEN_LATEST // 第4世代以降では、双方にかげふみのポケモンがいる場合、全員が逃走・交代できる。以前は双方とも逃げられない。
#define B_MOODY_ACC_EVASION             GEN_7 // 第8世代以降では、ムラっけで命中率・回避率が上がらない。
#define B_FLASH_FIRE_FROZEN             GEN_LATEST // 第5世代以降では、こおり状態でももらいびが発動する。
#define B_SYNCHRONIZE_TOXIC             GEN_LATEST // 第5世代以降では、シンクロのポケモンがもうどくになると相手ももうどくになる。以前は相手を通常のどくにする。
#define B_UPDATED_INTIMIDATE            GEN_LATEST // 第8世代以降では、せいしんりょく・きもったま・マイペース・どんかんにいかくが効かず、びびりを発動させる。
#define B_OBLIVIOUS_TAUNT               GEN_LATEST // 第6世代以降では、どんかんのポケモンにちょうはつが効かない。
#define B_STURDY                        GEN_LATEST // 第5世代以降では、HP満タンから攻撃やこんらんの自傷で倒れる場合、がんじょうでHPを1残す。
#define B_PLUS_MINUS_INTERACTION        GEN_LATEST // 第5世代以降では、プラス・マイナスは互いに異なる特性だけでなく、同じ特性同士でも発動する。
#define B_WEATHER_FORMS                 GEN_LATEST // 第5世代以降では、ポワルン・チェリムが対応する特性を失うと元の姿に戻る。チェリムの変化にはフラワーギフトが必要。
#define B_SYMBIOSIS_GEMS                GEN_6 // 第7世代以降では、きょうせいはジュエルで強化した攻撃の後に持ち物を渡す。以前は命中前に渡すため、渡した持ち物の効果も適用する。
#define B_REDIRECT_ABILITY_IMMUNITY     GEN_LATEST // 第5世代以降では、ひらいしん・よびみずは技を引き寄せるだけでなく、でんき・みず技を無効にし、特攻を1段階上げる。
#define B_REDIRECT_ABILITY_ALLIES       GEN_LATEST // 第4世代以降では、ひらいしん・よびみずが味方の技も引き寄せる。
#define B_LEAF_GUARD_PREVENTS_REST      GEN_LATEST // 第5世代以降では、晴れ中のリーフガードがねむるの使用を防ぐ。
#define B_TRANSISTOR_BOOST              GEN_8 // 第9世代以降では、トランジスタのでんき技補正が1.5倍から1.3倍になる。
#define B_ILLUMINATE_EFFECT             GEN_LATEST // 第9世代以降では、はっこうが命中率低下を防ぎ、対象の回避率を無視する。以前は戦闘中の効果がない。
#define B_WEAK_ARMOR_SPEED              GEN_LATEST // 第7世代以降では、くだけるよろいで物理技を受けた際の素早さ上昇が1段階から2段階になる。
#define B_PROTEAN_LIBERO                GEN_8 // 第9世代以降では、へんげんじざい・リベロの発動回数を1回に制限する。
#define B_INTREPID_SWORD                GEN_8 // 第9世代以降では、ふとうのけんは1戦闘に1回だけ発動する。
#define B_DAUNTLESS_SHIELD              GEN_8 // 第9世代以降では、ふくつのたては1戦闘に1回だけ発動する。
#define B_DISGUISE_HP_LOSS              GEN_7 // 第8世代以降では、ばけのかわが壊れてミミッキュがばれたすがたになると、最大HPの1/8を失う。
#define B_ABILITY_TRIGGER_CHANCE        GEN_LATEST // 第4世代以降では、だっぴ・メロメロボディ・ほのおのからだ・せいでんき・どくのトゲの発動率が1/3から30%になる。
                                                   // 第5世代以降のほうしは、ねむり11%・どく9%・まひ10%。第4世代は合計30%で3種を均等に抽選し、第3世代は合計10%。
#define B_PICKUP_WILD                   GEN_LATEST // 第9世代以降では、野生戦のターン終了時にものひろいで自分が使った持ち物も拾える。
#define B_MAGIC_GUARD                   GEN_LATEST // 第4世代のみ、マジックガードでまひによる行動不能を無視する。
#define B_BATTLE_BOND                   GEN_7 // 第9世代以降では、きずなへんげんで1戦闘に1回、攻撃・特攻・素早さを各1段階上げる。以前はサトシゲッコウガに変化する。
#define B_ATE_MULTIPLIER                GEN_6 // 第7世代以降では、スカイスキン・エレキスキン・ノーマルスキン・フェアリースキン・フリーズスキンの補正は1.2倍。第6世代以前は1.3倍だが、ノーマルスキンには補正がない。
#define B_DEFIANT_STICKY_WEB            GEN_LATEST // 第9世代以降では、ねばねばネットの設置者に関係なくまけんきが発動する。第8世代では、味方のネットがコートチェンジで移った場合は発動しない。
#define B_MIRROR_ARMOR_STICKY_WEB       GEN_LATEST // 第9世代以降では、ねばねばネットの設置者が場にいても、ミラーアーマーで能力低下を反射しない。
#define B_POWDER_OVERCOAT               GEN_LATEST // 第6世代以降では、ぼうじんが粉・胞子の技を防ぐ。
#define B_INFILTRATOR_SUBSTITUTE        GEN_LATEST // 第6世代以降では、すりぬけがみがわりを無視する。ただし、へんしん・フリーフォールは除く。
#define B_DANCER_ORDER                  GEN_LATEST // 第8世代以降では、おどりこを補正込みの素早さ順で発動する。第7世代は補正前の素早さを使い、遅い順に発動する。
#define B_UNSEEN_FIST_PIERCING_DRILL    GEN_LATEST // 第8～9世代では、ふかしのこぶし・Piercing Drillは防御技を貫通して通常の100%のダメージを与え、防御技の接触効果も無視する。
                                                   // Championsでは、防御技へのダメージが25%になり、防御技の接触効果も無視しなくなる。

// 各種の一時的な状態の持続ターン。
#define B_CONFUSION_TURNS    5
#define B_UPROAR_TURN_COUNT  5
#define B_RAMPAGE_TURNS      3
#define B_DISABLE_TIMER      4
#define B_ENCORE_TIMER       4
#define B_PERISH_SONG_TIMER  3
#define B_TAUNT_TIMER        5
#define B_SLOW_START_TIMER   5
#define B_EMBARGO_TIMER      5
#define B_MAGNET_RISE_TIMER  5
#define B_TELEKINESIS_TIMER  3
#define B_HEAL_BLOCK_TIMER   5
#define B_LASER_FOCUS_TIMER  2
#define B_THROAT_CHOP_TIMER  2
#define B_WRAP_TURNS         7 // ねばりのかぎづめ所持時の最大持続ターン。
#define B_SYRUP_BOMB_TIMER   3
#define B_TORMENT_TIMER      3

// アイテムの設定。
#define B_CONFUSE_BERRIES_HEAL      GEN_LATEST // 第8世代以降では、フィラのみなどはHP25%以下で最大HPの1/3を回復する。第7世代は25%以下で1/2、第6世代以前は半分以下で1/8を回復する。
#define B_X_ITEMS_BUFF              GEN_LATEST // 第7世代以降では、プラスパワーなどの能力強化アイテムの上昇量が1段階から2段階になる。
#define B_X_ITEMS_CROSSUSE          TRUE       // 第3世代では場にいる自分のポケモンにのみ能力強化アイテムを使えるが、第7世代ではダブルの味方にも使える。変更された世代は未確定。
#define B_MENTAL_HERB               GEN_LATEST // 第5世代以降では、メンタルハーブでメロメロに加え、ちょうはつ・アンコール・いちゃもん・かいふくふうじ・かなしばりを治す。
#define B_TRAINERS_KNOCK_OFF_ITEMS  TRUE       // TRUEでは、トレーナーがプレイヤーの持ち物を奪う・交換することができる（きのみ以外は戦闘後に戻る）。原作では施設外のトレーナーは持ち物を奪えない。
#define B_RETURN_STOLEN_NPC_ITEMS   GEN_LATEST // 第5世代以降では、どろぼう・ほしがるでNPCの持ち物を奪っても戦闘後に持ち帰れない。
#define B_STEAL_WILD_ITEMS          GEN_LATEST // 第9世代以降では、どろぼう・ほしがるで奪った野生の持ち物をバッグへ送る。第8世代以前は技の使用者が持つ。
#define B_RESTORE_HELD_BATTLE_ITEMS GEN_LATEST // 第9世代以降では、きのみ以外の持ち物を戦闘後にすべて復元する。
#define B_SOUL_DEW_BOOST            GEN_6 // 第7世代以降では、こころのしずくでラティアス・ラティオスのエスパー・ドラゴン技を約20%強化する。第6世代以前は特攻・特防を各50%強化する。
#define B_NET_BALL_MODIFIER         GEN_LATEST // 第7世代以降では、ネットボールの捕獲倍率が3倍から3.5倍になる。
#define B_DIVE_BALL_MODIFIER        GEN_LATEST // 第4世代以降では、ダイブボールの補正がダイビング中に加えて、なみのり・釣り中にも適用される。
#define B_NEST_BALL_MODIFIER        GEN_LATEST // ネストボールの計算式を世代ごとに変える。Cmd_handleballthrowを参照。
#define B_REPEAT_BALL_MODIFIER      GEN_LATEST // 第7世代以降では、リピートボールの捕獲倍率が3倍から3.5倍になる。
#define B_TIMER_BALL_MODIFIER       GEN_LATEST // 第5世代以降では、タイマーボールの1ターンごとの倍率増加が0.1倍から0.3倍になる。
#define B_DUSK_BALL_MODIFIER        GEN_LATEST // 第7世代以降では、ダークボールの捕獲倍率が3.5倍から3倍になる。
#define B_QUICK_BALL_MODIFIER       GEN_LATEST // 第5世代以降では、クイックボールの捕獲倍率が4倍から5倍になる。
#define B_LURE_BALL_MODIFIER        GEN_LATEST // ルアーボールの捕獲倍率。第8世代以降は4倍、第7世代は5倍、第6世代以前は3倍。
#define B_HEAVY_BALL_MODIFIER       GEN_LATEST // ヘビーボールの重さの区分。Cmd_handleballthrowを参照。
#define B_DREAM_BALL_MODIFIER       GEN_LATEST // 第8世代以降では、対象がねむり状態またはぜったいねむりの場合、ドリームボールの捕獲倍率が4倍になる。
#define B_SPORT_BALL_MODIFIER       GEN_LATEST // 第8世代以降では、コンペボールの捕獲倍率が1.5倍から1倍になる。
#define B_SAFARI_BALL_MODIFIER      GEN_LATEST // サファリボールの捕獲倍率。第8世代以降は1倍、第2～6世代は1.5倍、第1世代は2倍。
#define B_FRIEND_BALL_MODIFIER      GEN_LATEST // 第8世代以降では、フレンドボールで捕まえたポケモンの初期なつき度が200から150になる。
#define B_SERENE_GRACE_BOOST        GEN_LATEST // 第5世代以降では、てんのめぐみがおうじゃのしるし・するどいキバのひるみ確率も上げる。
#define B_IRON_BALL                 GEN_LATEST // 第5世代以降では、くろいてっきゅうを持つひこうタイプは、他のタイプに関係なくじめん技を等倍で受ける。さかさバトルや、他の効果で接地している場合は除く。

// フラグの設定。
// 以下の機能を使う場合、0をinclude/constants/flags.hにあるフラグIDへ置き換える。未使用のフラグを推奨する。
// 例えばFLAG_UNUSED_0x264を分かりやすい名前に変更し、以下に指定できる。

// バッジによる能力補正のフラグ。
#define B_FLAG_BADGE_BOOST_ATTACK   FLAG_BADGE01_GET // 指定フラグが立ち、B_BADGE_BOOSTがGEN_3なら、プレイヤーのポケモンの攻撃を上げる。
#define B_FLAG_BADGE_BOOST_DEFENSE  FLAG_BADGE05_GET // 指定フラグが立ち、B_BADGE_BOOSTがGEN_3なら、プレイヤーのポケモンの防御を上げる。
#define B_FLAG_BADGE_BOOST_SPEED    FLAG_BADGE03_GET // 指定フラグが立ち、B_BADGE_BOOSTがGEN_3なら、プレイヤーのポケモンの素早さを上げる。
#define B_FLAG_BADGE_BOOST_SPATK    FLAG_BADGE07_GET // 指定フラグが立ち、B_BADGE_BOOSTがGEN_3なら、プレイヤーのポケモンの特攻を上げる。
#define B_FLAG_BADGE_BOOST_SPDEF    FLAG_BADGE07_GET // 指定フラグが立ち、B_BADGE_BOOSTがGEN_3なら、プレイヤーのポケモンの特防を上げる。
// その他の戦闘フラグ。
#define B_FLAG_INVERSE_BATTLE       FLAG_EXPANSION_INVERSE_BATTLE     // 指定フラグが立っている場合、タイプ相性を反転する。例えば、ほのおがみずに効果抜群になる。
#define B_FLAG_AI_VS_AI_BATTLE      FLAG_EXPANSION_AI_VS_AI_BATTLE     // 指定フラグが立っている場合、次の戦闘でプレイヤーのポケモンもAIが操作する。
#define B_FLAG_DYNAMAX_BATTLE       FLAG_EXPANSION_DYNAMAX_BATTLE     // 指定フラグが立っている場合、すべてのトレーナーが戦闘でダイマックスできる。
#define B_FLAG_TERA_ORB_CHARGED     FLAG_EXPANSION_TERA_ORB_CHARGED     // 指定フラグが立っている場合、テラスタルオーブが充填済みになる。設定すると回復時に立ち、テラスタル使用時に解除する。
#define B_FLAG_TERA_ORB_NO_COST     FLAG_EXPANSION_TERA_ORB_NO_COST     // 指定フラグが立っている場合、テラスタルしてもオーブの充填を消費しない。SVではテラパゴス関連イベント後の状態。
#define B_FLAG_SLEEP_CLAUSE         FLAG_EXPANSION_SLEEP_CLAUSE     // 指定フラグが立っている場合、同じ側で1匹がねむっていると別のポケモンをねむらせられないルールを使う。AIが判断するにはAI_FLAG_CHECK_BAD_MOVEが必要。
#define B_FLAG_NO_WHITEOUT          FLAG_EXPANSION_NO_WHITEOUT     // 指定フラグが立っている場合、トレーナー戦で全滅しても通常の全滅処理を行わない。手持ちを自動回復しない点に注意。

// 変数の設定。
// 以下の機能を使う場合、0をinclude/constants/vars.hにある変数IDへ置き換える。未使用の変数を推奨する。
// 例えばVAR_UNUSED_0x404Eを分かりやすい名前に変更し、以下に指定できる。
#define B_VAR_WILD_AI_FLAGS         VAR_EXPANSION_WILD_AI_FLAGS     // 0以外の変数IDを指定すると、標準の野生AIフラグに追加できる。重要: (1 << 15)を超えるフラグは指定不可。
                                          // プレイヤーがセーブするまで、この変数に0以外の値を残してはいけない。
                                          // より詳細な野生AIの制御は、src/battle_ai_main.cのGetWildAiFlags()を変更する。

#define B_VAR_DIFFICULTY            VAR_EXPANSION_DIFFICULTY     // 0以外の変数IDを指定すると、読み込むトレーナーの難易度を制御できる。NewGameInitDataの実行後に、Script_SetDifficultyで開発者が明示的に設定する。

// バッグ使用制限の設定。
#define NO_BAG_RESTRICTION       0
#define NO_BAG_AGAINST_TRAINER   1
#define NO_BAG_IN_BATTLE         2
#define NO_BAG_INVALID_VALUE     3

#define B_VAR_NO_BAG_USE         VAR_EXPANSION_NO_BAG_USE     // 指定変数の値が1ならトレーナー戦でバッグを使用不可にする。2なら野生戦でも使用不可にする。

// スカイバトル。
#define B_FLAG_SKY_BATTLE                 0     // フラグIDを指定すると、スクリプトでスカイバトルを開始できる。
#define B_VAR_SKY_BATTLE                  0     // 変数IDを指定すると、スカイバトルに参加したポケモンの手持ち位置を記録する。
#define B_SKY_BATTLE_STRICT_ELIGIBILITY   FALSE // TRUEではXYの参加条件を使う。FALSEでは、すべてのひこうタイプとふゆうのポケモンが参加できる。

// フラグ・変数の設定。
#define B_RESET_FLAGS_VARS_AFTER_WHITEOUT TRUE // TRUEでは、全滅時にOverworld_ResetBattleFlagsAndVarsで戦闘関連のフラグ・変数をリセットする。

// パートナー表示の設定。
#define B_SHOW_PARTNER_TARGET             FALSE // パートナーが攻撃対象にするポケモンを表示する。

// 技説明メニュー。
#define B_SHOW_MOVE_DESCRIPTION     TRUE       // 戦闘中に技の情報を表示する。

// 天気の設定。
// 技・種族ごとの天気との関係を調べる場合、rain・sunny day・hailを検索する。
#define B_ICE_WEATHER_BOTH              0
#define B_ICE_WEATHER_HAIL              1
#define B_ICE_WEATHER_SNOW              2

#define B_ABILITY_WEATHER               GEN_LATEST // 第6世代以降では、特性による天気は5ターン続く。第5世代以前は戦闘終了か別の天気への変更まで続く。
#define B_SANDSTORM_SPDEF_BOOST         GEN_LATEST // 第4世代以降では、すなあらしでいわタイプの特防が1.5倍になる。
#define B_SANDSTORM_SOLAR_BEAM          GEN_LATEST // 第3世代以降では、すなあらし中にソーラービームの威力が下がる。
#define B_OVERWORLD_FOG                 GEN_LATEST // 第8世代以降では、フィールドの霧で戦闘中にミストフィールドが発生する。第4世代のみ、戦闘専用の霧の天気になる。
#define B_OVERWORLD_SNOW                GEN_LATEST // 第9世代以降では、フィールドの雪は戦闘中にあられではなく雪を発生させる。
#define B_SNOW_WARNING                  GEN_LATEST // 第9世代以降では、ゆきふらしはあられではなく雪を発生させる。
#define B_PREFERRED_ICE_WEATHER         B_ICE_WEATHER_BOTH // あられを発生させる技の効果を雪に、またはその逆に切り替える。
#define B_OVERWORLD_WEATHER_OVERRIDE    GEN_LATEST // 第9世代以降では、フィールド由来の天気を上書きできない。

// フィールド効果の設定。
#define B_TERRAIN_BG_CHANGE         TRUE       // TRUEでは、フィールド技の効果が切れるまで、戦闘背景を対応する背景に変える。
#define B_THUNDERSTORM_TERRAIN      TRUE       // TRUEでは、第8世代と同様にフィールドの雷雨で雨とエレキフィールドが発生する。
#define B_TERRAIN_TYPE_BOOST        GEN_LATEST // 第8世代以降では、フィールドと同じタイプの技の威力上昇が50%から30%になる。
#define B_SECRET_POWER_EFFECT       GEN_LATEST // ひみつのちからの効果を地形・世代ごとに変える。SetMoveEffectのMOVE_EFFECT_SECRET_POWERを参照。
#define B_SECRET_POWER_ANIMATION    GEN_LATEST // ひみつのちからのアニメーションを地形・世代ごとに変える。
#define B_NATURE_POWER_MOVES        GEN_LATEST // しぜんのちからで呼び出す技を地形・世代ごとに変える。gBattleEnvironmentInfoを参照。
#define B_CAMOUFLAGE_TYPES          GEN_LATEST // ほごしょくで変わるタイプを地形・世代ごとに変える。sTerrainToTypeを参照。
#define B_NEW_TERRAIN_BACKGROUNDS   TRUE      // TRUEでは、エレキ・ミスト・グラス・サイコフィールドに新しい戦闘背景を使う。

// 表示・操作の設定。
#define B_FAST_INTRO_PKMN_TEXT              TRUE       // TRUEでは、登場アニメーションの終了を待たず、戦闘開始の文章をアニメーションと同時に表示する。
#define B_FAST_INTRO_NO_SLIDE               FALSE      // TRUEでは、戦闘開始時のスライド演出を省略する。
#define B_FAST_HP_DRAIN                     TRUE       // TRUEでは、HPバーの変化を速める。
#define B_FAST_EXP_GROW                     TRUE       // TRUEでは、経験値バーの変化を速める。
#define B_SHOW_TARGETS                      TRUE       // TRUEでは、ダブルバトルの技選択前に選択可能な対象をすべて表示する。
#define B_SHOW_CATEGORY_ICON                TRUE       // TRUEでは、ステータス画面・技思い出し画面に物理・特殊・変化の分類アイコンを表示する。
#define B_HIDE_HEALTHBOX_IN_ANIMS           TRUE       // TRUEでは、技アニメーション中にHP表示枠を隠す。
#define B_WAIT_TIME_MULTIPLIER              10         // 戦闘中の文章の待ち時間を指定する。原作は16。小さい値ほど戦闘が速く進む。
#define B_QUICK_MOVE_CURSOR_TO_RUN          FALSE      // TRUEでは、野生戦のコマンドメニューでBを押すと「にげる」へカーソルを移動する。
#define B_RUN_TRAINER_BATTLE                FALSE       // TRUEでは、第9世代以降と同様にトレーナー戦でも逃走できる。全滅扱いになる。
#define B_MOVE_DESCRIPTION_BUTTON           L_BUTTON   // B_LAST_USED_BALL_BUTTONと異なるボタンを指定すると、そのボタンで技説明メニューを開く。
#define B_SHOW_USELESS_Z_MOVE_INFO          TRUE      // TRUEでは、後の世代の変化技など追加効果のないZワザに「追加効果なし」の案内を表示する。
#define B_ANIMATE_MON_AFTER_KO              TRUE       // TRUEでは、相手側のポケモンがひんしになると、ひんしでないポケモンのアニメーションを再生する。
#define B_ANIMATE_MON_AFTER_FAILED_POKEBALL TRUE       // TRUEでは、野生ポケモンがボールから出て捕獲に失敗すると、そのポケモンのアニメーションを再生する。
#define B_SHOW_DYNAMAX_MESSAGE              TRUE      // TRUEでは、ダイマックス・キョダイマックス後に追加の戦闘メッセージを表示する。
#define B_HPBAR_COLOR_THRESHOLD             GEN_LATEST // 第5世代以降では、HPバーの色をピクセル長ではなく実際のHPから判定し、より正確に色を変える。
#define B_HP_PERCENTAGE_DISPLAY             FALSE      // TRUEでは、相手のHP割合をHPバーと併せて表示する。シングルでは枠を広げてバーの下に表示し、ダブルでは自分側のHP数値表示を切り替える操作で相手の割合表示も切り替える。

// 捕獲の設定。
#define B_SEMI_INVULNERABLE_CATCH       GEN_LATEST // 第4世代以降では、あなをほる・そらをとぶなどで一時的に攻撃を受けないポケモンにボールを投げられない。
#define B_CATCHING_CHARM_BOOST          100        // ゆれないおまもり所持時の捕獲クリティカル確率の増加率（%）。
#define B_INCAPACITATED_CATCH_BONUS     GEN_LATEST // 第5世代以降では、ねむり・こおりの捕獲率補正が2倍から2.5倍になる。
#define B_LOW_LEVEL_CATCH_BONUS         GEN_LATEST // 第9世代以降では、レベル13未満のポケモンの捕獲率にボーナスを加える。第8世代ではレベル20未満が対象。
#define B_MISSING_BADGE_CATCH_MALUS     GEN_LATEST // 第9世代以降では、現在の言うことを聞くレベルを5上回るポケモンを捕獲する際、バッジ数に応じた捕獲率ペナルティを適用する。
#define B_CRITICAL_CAPTURE              TRUE       // TRUEでは、捕獲クリティカルを有効にする。
#define B_CRITICAL_CAPTURE_LOCAL_DEX    TRUE       // FALSEでは、有効な世代から推定した全国図鑑を基準に、捕獲クリティカルの確率を決める。
#define B_CRITICAL_CAPTURE_IF_OWNED     GEN_LATEST // 第9世代以降では、すでに捕まえた種族の捕獲成功をクリティカルの演出で表示する。
#define B_FLAG_VICTORY_CATCH_RANDOM     0          // 指定フラグが立っている場合、相手を倒した後にボールを投げる案内を出し、ランダムな確率で捕獲する。他の方法では捕獲できない。
#define B_FLAG_VICTORY_CATCH_GUARANTEED 0          // 指定フラグが立っている場合、相手を倒した後にボールを投げる案内を出し、必ず捕獲する。他の方法では捕獲できない。

#define B_LAST_USED_BALL            TRUE       // TRUEでは、第7世代以降の「最後に使ったボール」をすぐ使う機能を有効にする。
#define B_LAST_USED_BALL_BUTTON     R_BUTTON   // 最後に使ったボールの機能が有効な場合、このボタン（または組み合わせ）で投げる。
#define B_LAST_USED_BALL_CYCLE      TRUE       // TRUEでは、B_LAST_USED_BALL_BUTTONを押しながら十字キーを押すと、使うボールを切り替える。
#define B_CATCH_SWAP_INTO_PARTY     GEN_LATEST // 第7世代以降では、捕獲したポケモンを手持ちに入れ、代わりに別の手持ちをボックスへ送る選択肢を表示する。
#define B_CATCH_SWAP_CHECK_HMS      TRUE       // TRUEでは、上の捕獲時入れ替え機能で、秘伝技を覚えたポケモンをボックスへ戻せないようにする。

// その他の設定。
#define B_MULTI_BATTLE_WHITEOUT         GEN_LATEST // 第4世代以降では、マルチバトルはプレイヤーとパートナーの両方が全滅すると終了する。
#define B_EVOLUTION_AFTER_WHITEOUT      GEN_LATEST // 第6世代以降では、戦闘後の進化条件を満たしたポケモンは、敗北しても進化する。
#define B_AFFECTION_MECHANICS           TRUE       // 第6世代以降の「なかよし度」による戦闘効果を有効にする。LGPE以降では、なかよし度の代わりになつき度を使う。
#define B_TRAINER_CLASS_POKE_BALLS      GEN_LATEST // 第7世代以降では、トレーナーの職業に応じた種類のボールを使う。
#define B_TRAINER_MON_RANDOM_ABILITY    0          // 特性未指定のトレーナーポケモンは、0なら第1特性、1なら隠れ特性以外からランダム、2なら隠れ特性を含め等確率で選ぶ。
#define B_OBEDIENCE_MECHANICS           GEN_3 // PLA以降（この設定ではGEN_8）では、自分で捕まえたポケモンにも命令制限を適用する。ただし現在のレベルではなく出会ったレベルを基準にする。
#define B_USE_FROSTBITE                 FALSE      // PLAと同様に、こおりをしもやけへ置き換える。技ごとにどちらを付与するか指定することも可能。フリーズドライ・ひみつのちから・トライアタックはこの設定に従う。
#define B_TOXIC_REVERSAL                GEN_LATEST // 第5世代以降では、戦闘終了時にもうどくを通常のどくへ変える。
#define B_TRY_CATCH_TRAINER_BALL        GEN_LATEST // 第4世代以降では、トレーナーのポケモンを捕まえようとしてもボールを消費しない。
#define B_SLEEP_CLAUSE                  FALSE      // ねむり重複禁止ルールを常時有効にし、B_FLAG_SLEEP_CLAUSEより優先する。個別に切り替える場合はフラグを使う。
#define B_PARTNER_MONS_MARKED_SEEN      FALSE      // TRUEでは、ダブルのパートナーが未遭遇のポケモンを出すと、図鑑の「見つけた」状態に登録する。
#define B_MULTI_HALF_TEAMS              FALSE      // TRUEでは、同じ側に2人いる戦闘の手持ちを各3匹までにする。FALSEでもtrainers.partyのMulti Party: Halfで戦闘ごとに制限できる。
#define B_TERA_ORB_ALWAYS_CHARGED       FALSE      // TRUEでは、テラスタルオーブを常に充填済みとし、B_FLAG_TERA_ORB_CHARGEDより優先する。個別に切り替える場合はフラグを使う。

#define NUM_BEEPS_GEN_LATEST            4                    // 4回繰り返す。
#define NUM_BEEPS_GEN_3                 -1                   // 無限に繰り返す。
#define NUM_BEEPS_OFF                   0                    // 再生しない。
#define B_NUM_LOW_HEALTH_BEEPS          NUM_BEEPS_GEN_LATEST // HP低下時の警告音の繰り返し回数。NUM_BEEPS_OFFで無効、NUM_BEEPS_GEN_3で無限にする。上の定数以外の任意の回数も指定できる。

// アニメーションの設定。
#define B_NEW_SWORD_PARTICLE            TRUE    // TRUEでは、つるぎのまいの剣エフェクトを更新する。
#define B_NEW_LEECH_SEED_PARTICLE       TRUE    // TRUEでは、やどりぎのタネのエフェクトを更新する。
#define B_NEW_HORN_ATTACK_PARTICLE      TRUE    // TRUEでは、つのでつくの角エフェクトを更新する。
#define B_NEW_ROCKS_PARTICLE            TRUE    // TRUEでは、岩のエフェクトを更新する。
#define B_NEW_LEAF_PARTICLE             TRUE    // TRUEでは、葉のエフェクトを更新する。
#define B_NEW_EMBER_PARTICLES           TRUE    // TRUEでは、ひのこの炎エフェクトを更新する。
#define B_NEW_MEAN_LOOK_PARTICLE        TRUE    // TRUEでは、くろいまなざしの目のエフェクトを更新する。
#define B_NEW_TEETH_PARTICLE            TRUE    // TRUEでは、かみつく・かみくだくの歯のエフェクトを更新する。
#define B_NEW_HANDS_FEET_PARTICLE       TRUE    // TRUEでは、チョップ・キック・パンチのエフェクトを更新する。
#define B_NEW_SPIKES_PARTICLE           TRUE    // TRUEでは、まきびしのエフェクトを更新する。
#define B_NEW_FLY_BUBBLE_PARTICLE       TRUE    // TRUEでは、そらをとぶの泡状エフェクトを更新する。
#define B_NEW_CURSE_NAIL_PARTICLE       TRUE    // TRUEでは、のろいのくぎのエフェクトを更新する。
#define B_NEW_BATON_PASS_BALL_PARTICLE  TRUE    // TRUEでは、バトンタッチのボールのエフェクトを更新する。
#define B_NEW_MORNING_SUN_STAR_PARTICLE TRUE    // TRUEでは、あさのひざしの星のエフェクトを更新する。
#define B_NEW_IMPACT_PALETTE            TRUE    // TRUEでは、基本の衝撃エフェクトのパレットを更新する。
#define B_NEW_SURF_PARTICLE_PALETTE     TRUE    // TRUEでは、なみのりの波のパレットを更新する。

// ボールのアニメーションと効果音。
#define B_ENEMY_THROW_BALLS          GEN_LATEST  // 第6世代以降では、相手のボールが地面に現れて開くのではなく、トレーナーが投げる演出を行う。
#define B_ENEMY_THROW_BALLS_SOUND    GEN_LATEST  // 第5世代以降では、相手がポケモンを出すためボールを投げるときに効果音を鳴らす。B_ENEMY_THROW_BALLSがGEN_6以降の場合だけ使用できる。
#define B_PLAYER_THROW_BALLS_SOUND   GEN_LATEST  // 第5世代以降では、プレイヤーがポケモンを出すためボールを投げるときに効果音を鳴らす。

#define SHOW_TYPES_NEVER    0                    // 戦闘中にタイプを表示しない。
#define SHOW_TYPES_ALWAYS   1                    // 戦闘中にタイプを常に表示する。
#define SHOW_TYPES_CAUGHT   2                    // その種族を捕獲済みの場合だけ表示する。
#define SHOW_TYPES_SEEN     3                    // その種族を見たことがある場合だけ表示する。
#define B_SHOW_TYPES        SHOW_TYPES_NEVER     // 対象を選んで技を選択するとき、HPバーの隣にタイプを表示する条件を指定する。

#define SHOW_EFFECTIVENESS_NEVER    0           // 技選択時に相性を表示しない。
#define SHOW_EFFECTIVENESS_ALWAYS   1           // 技選択時に相性を常に表示する。
#define SHOW_EFFECTIVENESS_CAUGHT   2           // その種族を捕獲済みの場合だけ相性を表示する。
#define SHOW_EFFECTIVENESS_SEEN     3           // その種族を見たことがある場合だけ相性を表示する。
#define B_SHOW_EFFECTIVENESS        SHOW_EFFECTIVENESS_ALWAYS // SHOW_EFFECTIVENESS_NEVER以外では、PPの表示を技と対象に基づくタイプ相性の表示へ置き換える。

// 戦闘ポケモンスプライトの設定。
#define B_ENEMY_MON_SHADOW_STYLE        GEN_3 // 第4世代以降では、相手のポケモン全員の足元に影を描く。

// 戦闘画面の設定。
#define B_MOVE_REARRANGEMENT_IN_BATTLE  GEN_LATEST  // 第4世代以降では、戦闘中に技の順番を入れ替えられない。

#define B_POOL_SETTING_CONSISTENT_RNG       FALSE    // TRUEでは、同じセーブデータの同じトレーナーは常に同じ候補プールを生成する。
#define B_POOL_SETTING_USE_FIXED_SEED       FALSE    // TRUEでは、乱数生成にB_POOL_SETTING_FIXED_SEEDの固定シードを使う。
#define B_POOL_SETTING_FIXED_SEED           0x1D4127 // 任意に選んだ「乱数」値。エメラルドのcharmapで「へだら」（he-da-ra）に相当する想定。
#define B_POOL_RULE_SPECIES_CLAUSE          FALSE    // 同じ全国図鑑番号のポケモンを1匹までにする。
#define B_POOL_RULE_EXCLUDE_FORMS           FALSE    // 同種族の重複制限で別フォルムを除外する。
#define B_POOL_RULE_ITEM_CLAUSE             FALSE    // 同じ持ち物を1回だけ選べるようにする。
#define B_POOL_RULES_USE_ITEM_EXCLUSIONS    FALSE    // poolItemClauseExclusionsに記載した持ち物を除外する。
#define B_POOL_RULE_MEGA_STONE_CLAUSE       FALSE    // メガストーンを持つポケモンを1匹までにする。
#define B_POOL_RULE_Z_CRYSTAL_CLAUSE        FALSE    // Zクリスタルを持つポケモンを1匹までにする。

#endif // GUARD_CONFIG_BATTLE_H
