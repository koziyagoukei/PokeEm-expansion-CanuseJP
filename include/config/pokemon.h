#ifndef GUARD_CONFIG_POKEMON_H
#define GUARD_CONFIG_POKEMON_H

// 種族データの設定
#define P_UPDATED_TYPES             GEN_LATEST  // 登場後に変更された、一部のポケモンのタイプを指定世代に合わせる。
#define P_UPDATED_STATS             GEN_LATEST  // 登場後に変更された、一部のポケモンの種族値を指定世代に合わせる。
#define P_UPDATED_ABILITIES         GEN_LATEST  // 第4世代以降の、一部のポケモンの特性変更を適用する。
#define P_UPDATED_EGG_GROUPS        GEN_LATEST  // 第8世代以降の、一部のポケモンのタマゴグループ追加を適用する。
#define P_UPDATED_FRIENDSHIP        GEN_LATEST  // 第8世代以降の、一部のポケモンの初期なつき度変更を適用する。
#define P_UPDATED_EVS               GEN_LATEST  // 第4世代以降の、一部のポケモンから得る努力値の変更を適用する。
#define P_UPDATED_EXP_YIELDS        GEN_LATEST  // 第5世代以降の、一部のポケモンの基礎経験値変更を適用する。

// 習得技
#define P_LVL_UP_LEARNSETS          GEN_LATEST  // 使用するレベルアップ習得技の世代を選ぶ。
                                                // その世代では未登場の種族は初登場時のデータを使う（例: GEN_1指定時のチコリータは第2世代）。
                                                // 登場済みでもその世代に収録されていない種族は直近の登場時のデータを使う（例: GEN_8指定時のツタージャは第7世代）。
                                                // GEN_1/2/3/4/5/6/7は、それぞれピカチュウ版・クリスタル・RSE・HGSS・B2W2・ORAS・USUMの習得技を使う。
                                                // GEN_8は、第1〜4世代のポケモンにはBDSP、PLAで登場した種族にはPLA、剣盾収録の種族には剣盾をこの順で優先し、それ以外はGEN_7を使う。
                                                // GEN_9はSV収録の種族にはSV、それ以外はGEN_8を使う。
#define P_EVOLUTION_LEVEL_1_LEARN   GEN_LATEST  // 第8世代以降では、レベル1で進化した際にレベル1の技を覚える機会がなくなる。以前は習得可能だった。

// 進化の設定
#define P_FRIENDSHIP_EVO_THRESHOLD   GEN_LATEST  // 第8世代以降では、なつき進化に必要ななつき度が220以上から160以上になる。

// タマゴ・遺伝の設定
#define P_NIDORAN_M_DITTO_BREED        GEN_LATEST                // 第5世代以降では、ニドラン♂とメタモンからニドラン♀も生まれる。以前は♂のみ。同じ変更をバルビートにも適用する。
#define P_INCENSE_BREEDING             GEN_LATEST                // 第9世代以降では、後の世代で追加されたベビィポケモンを生むために親へおこうを持たせる必要がない。
#define P_EGG_HATCH_LEVEL              GEN_LATEST                // 第4世代以降では、ふ化時のレベルが5から1になる。
#define P_BALL_INHERITANCE             GEN_LATEST                // 第6世代以降では母親のボールを遺伝する。第7世代以降では両親が同じ種族なら父親のボールも遺伝できる。
#define P_MOVE_INHERITANCE             GEN_LATEST                // 第6世代より前は父親のタマゴ技、父親の技マシン技、両親共通のレベル技の順で遺伝する。第6世代以降は両親共通のレベル技、父親のタマゴ技、母親のタマゴ技の順になる。
#define P_NATURE_INHERITANCE           GEN_LATEST                // 第3世代では、かわらずのいしを持つメタモンまたは母親から50%で性格を遺伝する。第4世代以降はどちらの親からも遺伝でき、第5世代以降は確率が100%になる。
#define P_ABILITY_INHERITANCE          GEN_LATEST                // B2W2では♂と預けた♀の特性を80%で遺伝する。第6世代以降は通常特性80%、隠れ特性60%で、メタモンとならどちらの性別からも遺伝できる。BWの「隠れ特性60%、通常特性はランダム」という仕様は未実装。
#define P_EGG_MOVE_TRANSFER            GEN_LATEST                // 第8世代以降では、同種2匹を預けて一方がタマゴ技を覚え、もう一方に技の空きがあれば、その技を空き枠へ移せる。第9世代では、ものまねハーブを持つポケモンが種族を問わずほかのポケモンからタマゴ技を習得できる。
#define P_SCATTERBUG_LINE_FORM_BREED   SPECIES_SCATTERBUG_FANCY  // ビビヨン・コフーライ・コフキムシから生まれるコフキムシのフォルムを選ぶ。原作のゲームの地域設定に相当する。

// 種族固有の設定
#define P_SHEDINJA_BALL             GEN_LATEST  // 第4世代以降では、ヌケニンの入手にモンスターボールが必要。第3世代ではツチニンのボールを引き継ぐ。
#define P_KADABRA_EVERSTONE         GEN_LATEST  // 第4世代以降では、ユンゲラーはかわらずのいしを持っていても進化できる。
#define P_SHUCKLE_BERRY_JUICE       GEN_LATEST  // 第2世代では、ツボツボが持つ「きのみ」を1/16で「きのみジュース」に変える。有効にすると、旧「きのみ」に相当するオレンのみでこの挙動を再現する。

// 種族画像の設定
#define P_GENDER_DIFFERENCES            TRUE        // TRUEなら、♀の見た目の違いを表示する。ニャオニクスのように♀が独立したフォルムである種族には影響しない。
#define P_CUSTOM_GENDER_DIFF_ICONS      TRUE        // TRUEなら、ヒポポタス・カバルドンなどにも独自の♀用アイコンを使用する。
#define P_ARCEUS_UNIQUE_FORM_ICONS      GEN_LATEST  // 第9世代以降では、アルセウスの現在のフォルムに合わせてアイコンを変える。
#define P_FOOTPRINTS                    TRUE        // TRUEなら、第5世代までとBDSPのように足跡を表示する。無効にするとROM容量を節約できる。
#define P_TWO_FRAME_FRONT_SPRITES       TRUE        // エメラルドの正面スプライトは2コマ構成。無効にすると、ほかの第3世代作品と同じく最初の1コマのみを使用する。
#define P_GBA_STYLE_SPECIES_GFX         FALSE       // 既定の第4・5世代風スプライトをGBA風に変更する。現在、この画像を使う場合はB_ENEMY_MON_SHADOW_STYLEが無効になる。
#define P_GBA_STYLE_SPECIES_ICONS       FALSE       // 既定の更新済みアイコンをGBA風に変更する。
#define P_GBA_STYLE_SPECIES_FOOTPRINTS  FALSE       // 既定の更新済み足跡をGBA風に変更する。

// その他の設定
#define P_CRIES_ENABLED                  TRUE        // TRUEなら鳴き声を有効にする。無効化で大きくROM容量を節約できる（元の説明では25%以上）が、代わりにinclude/config/species_enabled.hで未使用の種族系列を個別に無効化することを推奨する。
#define P_LEGENDARY_PERFECT_IVS          GEN_LATEST  // 第6世代以降では、野生・贈り物の伝説・幻・ウルトラビーストの個体値を最低3か所最大にする。
#define P_EV_CAP                         GEN_LATEST  // 第6世代以降では、各能力の努力値上限が255から252になる。
#define P_SHOW_TERA_TYPE                 GEN_LATEST       // 第9世代以降では、つよさをみる画面にテラタイプを表示する。
#define P_TM_LITERACY                    GEN_LATEST  // 第6世代以降では、通常は技マシン非対応のポケモンも、その技がレベルアップ習得技に含まれていれば技マシンで習得できる。
#define P_CAN_FORGET_HIDDEN_MOVE         TRUE       // TRUEなら、秘伝技を含むすべての技を忘れられる。
#define P_ASK_MOVE_CONFIRMATION          FALSE       // FALSEなら、技を覚えないと選んだ際の再確認を省略する。
#define P_EGG_CYCLE_LENGTH               GEN_LATEST  // 第8世代以降では、タマゴの1周期に必要な歩数が以前の半分になる。以前の世代ではおおむね255歩前後で差がある。
#define P_ONLY_OBTAINABLE_SHINIES        FALSE       // TRUEなら、バトルピラミッドや捕獲禁止中の野生ポケモンを色違いにしない。
#define P_NO_SHINIES_WITHOUT_POKEBALLS   FALSE       // TRUEなら、ボールを所持していないときの野生ポケモンを色違いにしない。
#define P_SHOW_DYNAMIC_TYPES             TRUE       // TRUEなら、タイプが変化するすべての技について、現在のタイプを戦闘・つよさをみる画面へ反映する。原作では一部の技のみ対象。
#define P_BASE_HP_1_SHEDINJA_HANDLING    FALSE       // TRUEなら、HP種族値が1の種族をすべてヌケニンと同じ方式で扱う。

// 習得技生成ツールの設定
#define P_LEARNSET_HELPER_TEACHABLE      TRUE   // TRUEなら、tools/learnset_helpers/make_teachables.pyが付属のJSONと利用可能な技マシン・教え技からteachable_learnsets.hを生成する。FALSEにする場合は、対応する習得技ヘッダを.gitignoreの対象から外す必要がある。

// フラグの設定
// スクリプトから制御するには、0の項目に使用するフラグIDを指定する。
// 例: FLAG_UNUSED_0x264を指定すると、そのフラグで機能を切り替えられる。
#define P_FLAG_FORCE_SHINY               FLAG_EXPANSION_FORCE_SHINY  // このフラグが立つと、野生・贈り物のポケモンを必ず色違いにする。
#define P_FLAG_FORCE_NO_SHINY            FLAG_EXPANSION_FORCE_NO_SHINY  // このフラグが立つと、野生・贈り物のポケモンを必ず通常色にする。

// 特定のポケモンの進化系列を無効にする場合は、このファイルで設定する。
#include "config/species_enabled.h"

#include "constants/sound.h"

// 音声の設定
#define P_MODIFIED_MEGA_CRIES            P_MEGA_EVOLUTIONS  // TRUEなら、メガシンカ専用の追加鳴き声を使用する（元の説明ではROM容量の約3%）。FALSEなら通常の鳴き声をP_MODIFIED_MEGA_CRY_MODEで加工する。既定ではメガシンカの有効・無効に連動する。
#define P_MODIFIED_MEGA_CRY_MODE         CRY_MODE_HIGH_PITCH

// ポケモン図鑑
#define POKEDEX_TEXT_KANA                 0
#define POKEDEX_TEXT_KANJI                1
#ifndef P_POKEDEX_TEXT_STYLE
#define P_POKEDEX_TEXT_STYLE              POKEDEX_TEXT_KANA // KANA: 現在のかな本文。KANJI: コーパスの最新世代優先の漢字本文（要事前取得）。
#endif
#if P_POKEDEX_TEXT_STYLE != POKEDEX_TEXT_KANA && P_POKEDEX_TEXT_STYLE != POKEDEX_TEXT_KANJI
#error "Invalid P_POKEDEX_TEXT_STYLE"
#endif

#define DONT_SKIP_GAPS                   0              // 図鑑の未発見の空欄を省略しない。
#define SKIP_GAPS_EXCEPT_ONE             1              // 発見済みの項目の間に空欄を1つだけ残し、ほかの空欄を省略する。
#define SKIP_GAPS_EXCEPT_BEFORE_AFTER    2              // 発見済みの項目の前後に空欄を1つずつ残し、ほかを省略する。SVに最も近い表示。
#define SKIP_ALL_GAPS                    3              // 図鑑の空欄をすべて省略する。
#define P_SKIP_POKEDEX_GAPS              DONT_SKIP_GAPS // SVのように、図鑑の長い未発見区間を省略する方法を選ぶ。

#endif // GUARD_CONFIG_POKEMON_H
