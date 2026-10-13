#ifndef GUARD_CONFIG_AI_H
#define GUARD_CONFIG_AI_H

// 交代判定が考慮する具体的な条件は、src/battle_ai_switch.cのShouldSwitchから呼ばれる各関数を参照する。
// 以下の設定は、交代条件をすべて満たしたとAIが判断した場合に、実際に交代する確率を指定する。
// 例えば、ふしぎなまもりに対抗するため交代が必要と判断したとき、その判断を実行する割合を設定する。

// AIの交代確率。より複雑な挙動にする場合はGetSwitchChanceを変更する。
#define SHOULD_SWITCH_WONDER_GUARD_PERCENTAGE                   100
#define SHOULD_SWITCH_TRUANT_PERCENTAGE                         100
#define SHOULD_SWITCH_ALL_MOVES_BAD_PERCENTAGE                  100
#define STAY_IN_STATS_RAISED                                    2  // 一部の交代判定で居残るために必要な、各能力の上昇ランクの合計。

// AIの高度な交代判断の確率。より複雑な挙動にする場合はGetSwitchChanceを変更する。
#define SHOULD_SWITCH_ABSORBS_MOVE_PERCENTAGE                       100
#define SHOULD_SWITCH_ABSORBS_HIDDEN_POWER_PERCENTAGE               50
#define SHOULD_SWITCH_TRAPPER_PERCENTAGE                            100
#define SHOULD_SWITCH_FREE_TURN_PERCENTAGE                          50
#define STAY_IN_ABSORBING_PERCENTAGE                                66  // 場にいるポケモンがプレイヤーに効果抜群の技を持つ場合、攻撃を吸収できるポケモンへの交代を見送る確率。
#define SHOULD_SWITCH_HASBADODDS_PERCENTAGE                         50
#define SHOULD_SWITCH_ENCORE_STATUS_PERCENTAGE                      100
#define SHOULD_SWITCH_ENCORE_DAMAGE_PERCENTAGE                      50
#define SHOULD_SWITCH_CHOICE_LOCKED_PERCENTAGE                      100 // こだわり効果で変化技に固定されている場合のみ適用する。
#define SHOULD_SWITCH_ATTACKING_STAT_MINUS_TWO_PERCENTAGE           50
#define SHOULD_SWITCH_ATTACKING_STAT_MINUS_THREE_PLUS_PERCENTAGE    100
#define SHOULD_SWITCH_ALL_SCORES_BAD_PERCENTAGE                     100
#define SHOULD_SWITCH_DYN_FUNC_PERCENTAGE                           50 // 動的な交代判定関数で交代する確率。
#define SHOULD_SWITCH_LOSES_1V1_PERCENTAGE                          0 // 初期設定で無効にしておくと、開発時の交代挙動の評価・テストが容易になる。

// 不利な状態に対するAIの高度な交代判断の確率。
#define SHOULD_SWITCH_PERISH_SONG_PERCENTAGE                    100
#define SHOULD_SWITCH_YAWN_PERCENTAGE                           100
#define SHOULD_SWITCH_BADLY_POISONED_PERCENTAGE                 50
#define SHOULD_SWITCH_BADLY_POISONED_STATS_RAISED_PERCENTAGE    20
#define SHOULD_SWITCH_CURSED_PERCENTAGE                         50
#define SHOULD_SWITCH_CURSED_STATS_RAISED_PERCENTAGE            20
#define SHOULD_SWITCH_NIGHTMARE_PERCENTAGE                      33
#define SHOULD_SWITCH_NIGHTMARE_STATS_RAISED_PERCENTAGE         15
#define SHOULD_SWITCH_SEEDED_PERCENTAGE                         25
#define SHOULD_SWITCH_SEEDED_STATS_RAISED_PERCENTAGE            10
#define SHOULD_SWITCH_INFATUATION_PERCENTAGE                    100

// 交代で有利になる特性に対するAIの高度な交代判断の確率。
#define SHOULD_SWITCH_NATURAL_CURE_STRONG_PERCENTAGE                66
#define SHOULD_SWITCH_NATURAL_CURE_STRONG_STATS_RAISED_PERCENTAGE   10
#define SHOULD_SWITCH_NATURAL_CURE_WEAK_PERCENTAGE                  25
#define SHOULD_SWITCH_NATURAL_CURE_WEAK_STATS_RAISED_PERCENTAGE     10
#define SHOULD_SWITCH_REGENERATOR_PERCENTAGE                        50
#define SHOULD_SWITCH_REGENERATOR_STATS_RAISED_PERCENTAGE           20
#define SHOULD_SWITCH_INTIMIDATE_PERCENTAGE                         25
#define SHOULD_SWITCH_INTIMIDATE_STATS_RAISED_PERCENTAGE            10
#define SHOULD_SWITCH_WISH_PASSING_PERCENTAGE                       50

// AIの交代先選択に関する設定。
#define ALL_MOVES_BAD_STATUS_MOVES_BAD                          FALSE // 対象に有効な技がない場合、ShouldSwitchIfAllMovesBadが交代を促す。TRUEでは、この判定時に対象へ有効な変化技も無視する。
#define AI_BAD_SCORE_THRESHOLD                                  90 // 交代判定で、この値未満の技スコアを「悪い」とみなす。
#define AI_GOOD_SCORE_THRESHOLD                                 100 // 交代判定で、この値を超える技スコアを「良い」とみなす。
#define ALL_MOVES_BAD_NEEDS_GOOD_SWITCHIN                       FALSE // 良い交代先がある場合にのみShouldSwitchIfAllMovesBadを実行する。
#define ALL_SCORES_BAD_NEEDS_GOOD_SWITCHIN                      FALSE // 良い交代先がある場合にのみShouldSwitchIfAllScoresBadを実行する。
#define AI_DEFENSIVE_KO_THRESHOLD                               3 // 「耐久向き」とみなすには、倒されるまでにこの回数を超える攻撃を耐える必要がある。
#define AI_TYPE_MATCHUP_THRESHOLD                               UQ_4_12(2.0) // 良い相性とみなすには、この値を上回る必要がある。GetBattlerTypeMatchupでは2.0が標準の「互角」。
#define AI_WISH_HEAL_THRESHOLD                                  4 // ねがいごとの良い受け手とみなすために必要なHP回復割合の分母。4なら1/4、つまり25%。
#define AI_SWITCHIN_DAMAGE_THRESHOLD                            0 // 交代先として許容するために必要なダメージの下限。この値を超える必要がある。交代を断念する前の最終候補判定に使うため、十分低く設定する。
#define AI_REVERSE_BATTLER_LOGIC_ORDER_CHANCE                   50 // ダブルバトルでAIの処理順を逆転する確率。両方が交代を希望し交代先が1匹だけなら、先に処理した側が交代するため、先に判定するスロットをこの確率で入れ替える。

// 持ち物に基づくAIの技評価。
#define LOW_ACCURACY_THRESHOLD                                  75 // 命中率がこの値以下の技を低命中率とみなす。

// AIの技評価。
#define STATUS_MOVE_FOCUS_PUNCH_CHANCE                          50 // プレイヤーの最善手がきあいパンチの場合、AIが変化技を使う確率。
#define BOOST_INTO_HAZE_CHANCE                                  0 // プレイヤーがくろいきりを使った場合、AIが能力上昇技を使う確率。
#define SHOULD_RECOVER_CHANCE                                   50 // HPがENABLE_RECOVERY_THRESHOLD未満で差し迫った危険がない場合、回復技のスコアを上げる確率。
#define ENABLE_RECOVERY_THRESHOLD                               60 // SHOULD_RECOVER_CHANCEを適用するHP割合の境界。この値未満で有効になる。
#define SUCKER_PUNCH_CHANCE                                     50 // プレイヤーが変化技を持つ場合、AIがふいうちを使わない確率。
#define SUCKER_PUNCH_PREDICTION_CHANCE                          50 // SUCKER_PUNCH_CHANCEの判定が外れ、変化技を予測している場合に追加でふいうちを見送る確率。
#define PRIORITIZE_LAST_CHANCE_CHANCE                           50 // 後手で倒す手よりLast Chance（先に倒される状況で先制技を使う手）を優先する確率。
#define LAST_MON_PREFERS_NOT_SACRIFICE                          FALSE // 最後の1匹で、だいばくはつ・いのちがけなど自分も倒れる技を使うのを控えるかどうか。
#define EXPLOSION_LOWER_HP_THRESHOLD                            10 // HP割合がこの値以下なら爆発技の使用確率は90%。上下の境界間ではHPに応じて変化する。
#define EXPLOSION_HIGHER_HP_THRESHOLD                           90 // HP割合がこの値以上なら爆発技の使用確率は0%。上下の境界間ではHPに応じて変化する。
#define EXPLOSION_MINIMUM_CHANCE                                0 // 現在のHPに基づく爆発技の使用確率の最小値（%）。
#define EXPLOSION_MAXIMUM_CHANCE                                90 // 現在のHPに基づく爆発技の使用確率の最大値（%）。
#define FINAL_GAMBIT_CHANCE                                     50 // プレイヤーより速く、自分のHPが多いと判断した場合、いのちがけの使用を検討する確率。
#define SHOULD_PIVOT_BREAK_SASH_CHANCE                          50 // 良い交代先があり、マルチスケイル・きあいのタスキなどを削る場合にShouldPivotがTRUEを返す確率。
#define FAKE_OUT_SAVE_ALLY_CHANCE                               50 // 味方が相手2匹のどちらにも先に倒される場合、ねこだましで味方を守る確率。

// AIのダメージ計算に関する設定。
#define RISKY_AI_CRIT_STAGE_THRESHOLD                           2   // AI_FLAG_RISKYが急所に当たると仮定する急所ランクの境界。
#define RISKY_AI_CRIT_THRESHOLD_GEN_1                           128 // 第1世代方式でAI_FLAG_RISKYが急所と仮定する境界。値Xは急所確率X/255に相当する。
#define AI_DAMAGES_THROUGH_BERRIES                              TRUE // 必要な攻撃回数を判定するダメージ計算で、半減きのみの消費後も考慮する。倒すための最善手の比較では、実際に一撃で倒せる技を選べるようにする。
#define AI_IGNORE_BERRY_KO_THRESHOLD                            2   // 半減きのみを無視するための撃破回数の境界。2なら「半減される初撃と半減されない次撃で倒せる場合、初撃の計算で半減を無視する」。AI_DAMAGES_THROUGH_BERRIESが必要。

// AIのダメージ乱数の設定。
#define AI_ROLL_MIN                                             1
#define AI_ROLL_MEDIAN                                          2
#define AI_ROLL_MAX                                             3
#define AI_ROLL_RANDOM                                          4
#define AI_ROLL_TYPE_COUNT                                      5

// 各用途で使うダメージ乱数の種類を指定する。AI_FLAG_RISKYとAI_FLAG_CONSERVATIVEの指定が優先される。
#define AI_ROLL_ATTACKING                                       AI_ROLL_MAX
#define AI_ROLL_DEFENDING                                       AI_ROLL_MEDIAN
#define AI_ROLL_SWITCHIN_ATTACKING                              AI_ROLL_MEDIAN
#define AI_ROLL_SWITCHIN_DEFENDING                              AI_ROLL_MEDIAN
#define AI_ROLL_SHOULD_SETUP_DEFENDING                          AI_ROLL_MAX
#define AI_ROLL_ATTACKING_PARTNER                               AI_ROLL_MAX

// AIの行動予測確率。
#define PREDICT_SWITCH_CHANCE                                   50
#define PREDICT_MOVE_CHANCE                                     100

// AIのテラスタル判断の確率。
#define AI_CONSERVE_TERA_CHANCE_PER_MON                         10 // 高度なテラスタル判断を使うAIが、防御面の利点を検討する前に温存する確率は、この値*(X-1)。Xはテラスタル可能でひんしでないポケモン数。
#define AI_TERA_PREDICT_CHANCE                                  40 // 高度なテラスタル判断を使うAIが、ある技による撃破は防げるが別の技では倒され得る状況でテラスタルする確率。

// AI_FLAG_PP_STALL_PREVENTIONの設定。
#define PP_STALL_DISREGARD_MOVE_PERCENTAGE                      50 // 各乱数判定でPP枯らしを検出する確率。
#define PP_STALL_SCORE_REDUCTION                                20 // PP枯らし検出の判定が1つでも成立した場合に減らすスコア。

// AI_FLAG_ASSUME_STABの設定。
#define ASSUME_STAB_SEES_ABILITY                                FALSE // プレイヤーの特性も確定情報として扱う。未知の特性を予測させる場合は、代わりにAI_FLAG_WEIGH_ABILITY_PREDICTIONを使用できる。

// AI_FLAG_ASSUME_STATUS_MOVESの設定。
#define ASSUME_STATUS_MOVES_HAS_TUNING                  TRUE // 変化技の種類ごとに、持っていると予測する確率を変える。
                                                             // FALSEでは、ねこだまし・いかりのまえばに対する警戒も行わない。
#define ASSUME_STATUS_HIGH_ODDS                         90 // キノコのほうしなど、そのポケモンが持つ可能性が非常に高い技を予測する確率。
#define ASSUME_STATUS_MEDIUM_ODDS                       70 // まもるなど、そのポケモンが持つ可能性が中程度の技を予測する確率。
#define ASSUME_STATUS_LOW_ODDS                          40 // なかまづくりなど、持つ可能性はあるが採用されにくい技を予測する確率。
#define ASSUME_ALL_STATUS_ODDS                          25 // 種類を問わず変化技を持っていると予測する確率。

// AI_FLAG_SMART_SWITCHINGの設定。
#define SMART_SWITCHING_OMNISCIENT                              FALSE // 他の全情報参照設定にかかわらず、交代計算では未知の情報も参照する。

// AI_FLAG_RANDOMIZE_SWITCHINの設定。
#define RANDOMIZE_SWITCHIN_ANY_VALID                            TRUE // 良い交代先がなくても、手持ち順で最後の候補を選ぶのではなく、交代可能な候補すべてからランダムに選ぶ。

// AI_FLAG_DOUBLE_BATTLE専用の設定。
#define FRIENDLY_FIRE_RISKY_THRESHOLD             2 // AI_FLAG_RISKYが許容する、味方への巻き添え攻撃で倒れるまでの攻撃回数。
#define FRIENDLY_FIRE_NORMAL_THRESHOLD            3 // 通常のAIが許容する、味方への巻き添え攻撃で倒れるまでの攻撃回数。
#define FRIENDLY_FIRE_CONSERVATIVE_THRESHOLD      4 // AI_FLAG_CONSERVATIVEが許容する、味方への巻き添え攻撃で倒れるまでの攻撃回数。
// 相手がまもるで時間を稼ぐと予測した場合の対策。
#define DOUBLE_TRICK_ROOM_ON_LAST_TURN_CHANCE    35 // トリックルーム終了ターンに相手2匹がまもると予測し、味方2匹ともトリックルームを使って持続ターンを更新する確率。
#define TAILWIND_IN_TRICK_ROOM_CHANCE            35 // トリックルーム終了ターンに相手2匹がまもると予測し、おいかぜを使う確率。

#define AI_FLAG_ATTACKS_PARTNER_FOCUSES_PARTNER  FALSE  // TRUEでは、AI_FLAG_ATTACKS_PARTNERは味方よりパートナーへの攻撃を優先する。
                                                        // AIを使う野生戦では、設定にかかわらずTRUEとして扱う。

// ガードシェア・パワーシェアでAIが望む能力変化の割合（%）。
#define GUARD_SPLIT_ALLY_PERCENTAGE     200
#define GUARD_SPLIT_ENEMY_PERCENTAGE    50
#define POWER_SPLIT_ALLY_PERCENTAGE     150
#define POWER_SPLIT_ENEMY_PERCENTAGE    50

// 変化技のZワザを使うHP割合の境界。
#define Z_EFFECT_FOLLOW_ME_THRESHOLD    30
#define Z_EFFECT_RESTORE_HP_LOWER_THRESHOLD   ENABLE_RECOVERY_THRESHOLD // 複数回使う可能性がある技に適用する境界。
#define Z_EFFECT_RESTORE_HP_HIGHER_THRESHOLD  90                        // 一度限りの技、またはHPを減らす技に適用する境界。

#endif // GUARD_CONFIG_AI_H
