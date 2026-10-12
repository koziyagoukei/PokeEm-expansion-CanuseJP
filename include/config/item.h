#ifndef GUARD_CONFIG_ITEM_H
#define GUARD_CONFIG_ITEM_H

// 道具の設定
#define I_SHINY_CHARM_ADDITIONAL_ROLLS  2           // ひかるおまもり所持時に追加する色違い抽選の回数。0なら効果を無効にする。
#define I_KEY_FOSSILS                   GEN_LATEST  // 第4世代以降では、化石をたいせつなものとして扱わない。
#define I_KEY_ESCAPE_ROPE               GEN_LATEST  // 第8世代以降では、あなぬけのヒモをたいせつなものにする。ショップで無料になる点に注意。
#define I_HEALTH_RECOVERY               GEN_LATEST  // 第7世代以降では、一部の回復道具のHP回復量が変わる。
#define I_SITRUS_BERRY_HEAL             GEN_LATEST  // 第4世代以降では、オボンのみの回復量が固定30HPから最大HPの25%になる。
#define I_VITAMIN_EV_CAP                GEN_LATEST  // 第8世代以降では、栄養ドリンクの各能力の努力値100までという制限をなくす。
#define I_BERRY_EV_JUMP                 GEN_LATEST  // 第4世代のみ、努力値を下げるきのみを使うと、その能力の努力値が100超なら100まで下がる。
#define I_GRISEOUS_ORB_FORM_CHANGE      GEN_LATEST  // 第9世代以降では、はっきんだまを持たせてもギラティナのフォルムは変わらない。
#define I_GEM_BOOST_POWER               GEN_LATEST  // 第6世代以降では、ジュエルの威力補正が1.5倍から1.3倍になる。
#define I_USE_EVO_HELD_ITEMS_FROM_BAG   TRUE       // TRUEなら、PLAのようにエレキブースターなどの進化用持ち物をバッグから使用して進化させられる。
#define I_TYPE_BOOST_POWER              GEN_LATEST  // 第4世代以降では、タイプ強化の持ち物の威力補正が1.1倍（うしおのおこうは1.05倍）から1.2倍になる。
#define I_SELL_VALUE_FRACTION           GEN_LATEST  // 第9世代以降では、売却価格が購入価格の1/2から1/4になる。
#define I_PRICE                         GEN_LATEST  // 世代によって異なる道具の価格を設定する。具体的な値はsrc/data/items.hを参照。
#define I_BERRY_PRICE                   GEN_7       // 第8世代以降では、きのみを植えられなくなったことに伴って価格が上がる。
#define I_POWER_ITEM_BOOST              GEN_LATEST  // 第7世代以降では、パワー系の道具で追加される努力値が4から8になる。
#define I_PREMIER_BALL_BONUS            GEN_LATEST  // LGPE以降（この設定ではGEN_8以降）は、種類を問わず一度に購入したボール10個ごとにプレミアボールを1個もらえる。以前はモンスターボールのみが対象で、1回の購入につき最大1個。
#define I_ROTOM_CATALOG_THUNDER_SHOCK   GEN_LATEST  // 第9世代以降では、ロトムを通常の姿に戻すと、ほかの技を覚えていてもでんきショックを習得する。
#define I_REPEL_INCLUDE_FAINTED         GEN_LATEST  // 第1世代と第6世代以降では、先頭がひんしでもそのレベルをむしよけ判定に使う。第2〜5世代では、ひんしでない最初のポケモンのレベルを使う。
#define I_LAX_INCENSE_BOOST             GEN_LATEST  // 第4世代以降では、のんきのおこうによる命中低下が5%から10%になる。

// 技マシンの設定
#define I_REUSABLE_TMS          TRUE       // 第5〜8世代では技マシンを何度でも使用できる。TRUEなら原作の技マシンすべてを再使用可能にする。各道具のimportanceを1にすれば、個別にも指定できる。

// がくしゅうそうちの設定
// フラグで切り替える場合は、0の項目に使用するフラグIDを指定する。
// 例: FLAG_UNUSED_0x264を指定すると、そのフラグで機能を切り替えられる。
#define I_EXP_SHARE_FLAG        FLAG_EXPANSION_EXP_SHARE           // このフラグが立つと、戦闘に参加したかどうかに関係なく手持ち全員が経験値を得る。
#define I_EXP_SHARE_ITEM        GEN_5       // 第6世代以降では、がくしゅうそうちを上記効果の切り替え用のたいせつなものにする。

// むしよけ・コロンの設定
// この2つの設定は独立して使用でき、併用もできる。
#define VAR_LAST_REPEL_LURE_USED    VAR_EXPANSION_LAST_REPEL_LURE_USED       // 変数を指定すると最後に使ったむしよけ・コロンを記録する。I_REPEL_LURE_MENUがTRUEでなければ、効果終了時に原作と同じはい・いいえの確認を表示する。
#define I_REPEL_LURE_MENU           TRUE    // TRUEなら、効果終了時に次に使うむしよけ・コロンを選べる。VAR_LAST_REPEL_LURE_USEDが0以外ならカーソル位置を保存する。

// バトルサーチャー
#define I_VS_SEEKER_CHARGING        FLAG_EXPANSION_VS_SEEKER_CHARGING                     // フラグを指定するとバトルサーチャーを有効にする。所持している間はエントリーコールの再戦機能が動作しなくなる。詳しくはdocs/tutorials/vs_seeker.mdを参照。
#define I_SHOW_NO_ID_TRAINER        DISABLED_ON_RELEASE   // TRUEなら、トレーナー型でありながらトレーナーIDを判別できないオブジェクトに「?」を表示する。スクリプト冒頭にvsseeker_rematchid TRAINER_IDを追加すると、再戦時の反応を判別できる。

// ORASのダウジングマシン
#define I_ORAS_DOWSING_FLAG         FLAG_EXPANSION_ORAS_DOWSING       // 未使用のフラグを指定すると、ORAS式のダウジングマシンを有効にする。
#define I_ORAS_DOWSING_SOUNDS       TRUE    // TRUEなら、隠し道具までの距離に応じた音を鳴らす。
#define I_ORAS_DOWSING_COLOR_PAL    15      // 道具への近さに応じて色を変更する、パレット内のインデックス。

// ORAS式ダウジングの距離・演出ごとの色。
#define I_ORAS_DOWSING_COLOR_NONE   RGB_GRAY
#define I_ORAS_DOWSING_COLOR_SLOW   RGB2GBA(56, 120, 255)
#define I_ORAS_DOWSING_COLOR_NORMAL RGB2GBA(24, 216, 24)
#define I_ORAS_DOWSING_COLOR_FAST   RGB2GBA(255, 255, 40)
#define I_ORAS_DOWSING_COLOR_FASTER RGB_RED

#endif // GUARD_CONFIG_ITEM_H
