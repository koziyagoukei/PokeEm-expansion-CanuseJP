#ifndef GUARD_CONFIG_OVERWORLD_H
#define GUARD_CONFIG_OVERWORLD_H

// 移動の設定。
#define OW_RUNNING_INDOORS          GEN_LATEST  // 第4世代以降では、屋内でも走れる。
#define SLOW_MOVEMENT_ON_STAIRS     FALSE       // 有効にすると、ファイアレッドのように階段の上り下りで移動速度を落とす。

// その他の設定。
#define OW_POISON_DAMAGE                GEN_LATEST // 第4世代ではフィールド上のどくでひんしにならず、第5世代以降ではフィールド上のどくダメージ自体がなくなる。
#define OW_DOUBLE_APPROACH_WITH_ONE_MON FALSE      // 有効にすると、戦闘可能な手持ちが1匹だけでも、2人のトレーナーに同時に見つかる。
#define OW_HIDE_REPEAT_MAP_POPUP        FALSE      // 有効にすると、直前と同じMap Section IDのマップへ入った場合、地名表示を出さない。
#define OW_WHITEOUT_CUTSCENE            GEN_LATEST // 第4世代以降では、全滅時に追加メッセージと回復NPCによる復帰イベントを表示する。FRLGにもある仕様だが、簡略化のためGEN_3ではRSEの挙動を使う。
#define OW_DEFOG_FIELD_MOVE             FALSE      // 有効にすると、DPPtのようにきりばらいをフィールドで使える。
#define OW_ROCK_CLIMB_FIELD_MOVE        FALSE      // 有効にすると、DPPtのようにロッククライムをフィールドで使える。
#define OW_CHOOSE_FROM_PC_AND_PARTY     TRUE       // 有効にすると、技教えや交換などのNPCで、手持ちだけでなくボックスからもポケモンを選べる。

// アイテム入手時の説明枠。
#define OW_ITEM_DESCRIPTIONS_OFF        0   // 説明を表示しない。
#define OW_ITEM_DESCRIPTIONS_FIRST_TIME 1   // 初回のみ表示する。注意: セーブ互換性を壊す変更になるため、struct SaveBlock3を参照する。
#define OW_ITEM_DESCRIPTIONS_ALWAYS     2   // 毎回説明を表示する。
#define OW_SHOW_ITEM_DESCRIPTIONS       OW_ITEM_DESCRIPTIONS_ALWAYS // アイテム入手時の説明・画像の表示方法を指定する。

// 以下の世代定義は、きのみとOW_PC_MOVE_ORDERの設定でのみ区別する。
#define GEN_6_XY GEN_6
#define GEN_6_ORAS GEN_COUNT + 1

// パソコンの設定。
#define OW_PC_PRESS_B               GEN_LATEST // 第4世代では、ポケモンをつかんだ状態のBボタンで置く操作を行う。第3世代では「ポケモンをつかんでいる」というエラーになる。
#define OW_PC_JAPAN_WALDA_ICONS     TRUE       // 米国版エメラルドで空白になったアヤミの壁紙用アイコン（十字・稲妻・プラスル）を、日本版と同様に復元する。
#define OW_PC_HEAL                  GEN_LATEST // 第8世代以降では、ボックスに預けてもポケモンを回復しない。
#define OW_PC_MOVE_ORDER            GEN_LATEST // 第4世代以降では、パソコンのメニュー項目の順番が変わる。
#define OW_PC_RELEASE_ITEM          GEN_LATEST // 第8世代以降では、持ち物を持ったポケモンを逃がすと持ち物がバッグへ戻る。

#define OW_BERRY_MUTATIONS             FALSE      // 有効にすると、隣に植えたきのみに応じて突然変異が起こる。
#define OW_BERRY_MUTATION_CHANCE       25         // 突然変異が起こる確率（%）。
#define OW_BERRY_MOISTURE              FALSE      // 有効にすると、成長段階ごとに一度水をやる方式ではなく、土の湿り気を維持する方式になる。
#define OW_BERRY_ALWAYS_WATERABLE      FALSE      // OW_BERRY_MOISTUREが有効な場合、常に水をやれるようにし、乾燥で収穫量が減る第4世代方式を使う。無効なら乾いたときだけ水をやれて、収穫量が増える第6世代方式になる。
#define OW_BERRY_MULCH_USAGE           FALSE      // 有効にすると、土にこやしを使える。無効では使用不可になる。湿り気への効果にはOW_BERRY_MOISTUREも必要。
#define OW_BERRY_WEEDS                 FALSE      // 有効にすると、きのみの木に雑草が生え、手入れが必要になる。OW_BERRY_MOISTUREが無効なら、草取りのボーナスは切り捨てになる。
#define OW_BERRY_PESTS                 FALSE      // 有効にすると、きのみの木に害虫が寄り、手入れが必要になる。OW_BERRY_MOISTUREが無効なら、害虫対処のボーナスは切り捨てになる。
#define OW_BERRY_SIX_STAGES            FALSE      // XYのように成長段階を4段階から6段階にする。収穫までの時間は変わらない。OW_BERRY_MOISTUREが無効なら、追加の2段階は水やり判定でBERRY_STAGE_TALLERとして扱う。

#define OW_BERRY_GROWTH_RATE           GEN_3      // 各きのみの成長時間に使う世代別設定。
#define OW_BERRY_YIELD_RATE            GEN_3      // 各きのみの収穫量に使う世代別設定。
#define OW_BERRY_DRAIN_RATE            GEN_6_ORAS // OW_BERRY_MOISTUREが有効な場合の土の乾燥速度。GEN_4はきのみごとに異なり、GEN_6_XYは24時間（対応するこやし使用時は4時間）、GEN_6_ORASは4時間で乾く。他の値は使用不可。
#define OW_BERRY_IMMORTAL              FALSE      // 有効にすると、実ったきのみの木はプレイヤーが収穫するまで消えない。
#define OW_BERRY_COLORS                GEN_6_ORAS // GEN_6_XYまたはGEN_6_ORASにすると、カゴ・ウイ・ブリー・モコシ・シーヤ・ベリブ・リュガ・カイス・タポル・ノワキ・カシブ・ナモ・サン・アッキ・ウタン・ロゼル・チイラ・ナゾ・タラプのみの色を対応作品に合わせる。他の値は使用不可。

// フィールド上のポケモン。
#define OW_POKEMON_OBJECT_EVENTS       TRUE       // 全種族のオブジェクトイベント情報を追加する。OBJ_EVENT_GFX_SPECIESマクロでNPCに使用できる（例: OBJ_EVENT_GFX_SPECIES(BULBASAUR)）。
#define OW_SUBSTITUTE_PLACEHOLDER      TRUE       // フィールド用スプライトがないポケモンに、みがわりのスプライトを使う。
#define OW_LARGE_OW_SUPPORT            TRUE       // TRUEでは、フィールド処理に少量の負荷を追加し、大型スプライト（48x48、64x64）を橋の下などでも正しく表示する。
#define OW_PKMN_OBJECTS_SHARE_PALETTES FALSE      // 注意: 作業中で、すべてのパレットは未調整。TRUEでは連れ歩き用パレットを戦闘スプライトから取得する。
#define OW_GFX_COMPRESS                TRUE       // 圧縮されたフィールド画像に対応する。連れ歩き用画像も圧縮する。
                                                  // 重要: 繰り返しの展開を避けるため画像をVRAMに読み込む。VRAMを節約したい場合や多数のポケモンを同時表示する場合は、この設定を無効にする。
                                                  // 圧縮画像は、各辺が2の累乗でないスプライトサイズに対応しない。
                                                  // 圧縮画像に48x48のスプライト・テーブルを使わないこと。
                                                  // 16x32、32x32、64x64などは使用可能。
#define OW_MON_WANDER_WALK             TRUE       // TRUEでは、MOVEMENT_TYPE_WANDERのフィールドポケモンが移動の合間にもその場で足踏みする。
// ポケモンの連れ歩き。
#define OW_FOLLOWERS_ENABLED           TRUE       // HGSS方式の連れ歩きを有効にする。OW_POKEMON_OBJECT_EVENTSが必要。完全な対応には追加スクリプトが必要になる場合がある。
#define OW_FOLLOWERS_BOBBING           TRUE       // TRUEでは、連れ歩きポケモンが待機・歩行アニメーション中に上下に揺れる。
#define OW_FOLLOWERS_POKEBALLS         TRUE       // TRUEでは、通常のモンスターボールではなく、そのポケモンを捕獲したボールから出てくる。
#define OW_FOLLOWERS_WEATHER_FORMS     FALSE      // TRUEでは、ポワルン・チェリムにFORM_CHANGE_OVERWORLD_WEATHERを追加し、フィールドの天気に合わせて姿を変える。
#define OW_FOLLOWERS_COPY_WILD_PKMN    FALSE      // TRUEでは、へんしんを覚えているかイリュージョン・かわりものを持つ連れ歩きポケモンが、ランダムに野生ポケモンの姿をコピーする。
#define OW_BATTLE_ONLY_FORMS           TRUE       // TRUEでは、メガシンカなど戦闘専用フォルムのフィールドスプライトを読み込む。OW_POKEMON_OBJECT_EVENTSが必要。
#define B_FLAG_FOLLOWERS_DISABLED      FLAG_EXPANSION_FOLLOWING_MON_DISABLED // フラグで連れ歩きを有効・無効にする。イベント中など一時的に無効化する場合に使用する。
#define OW_FOLLOWERS_SCRIPT_MOVEMENT   TRUE       // TRUEでは、初期状態でFLAG_SAFE_FOLLOWER_MOVEMENTを立て、プレイヤー以外との衝突時にのみ連れ歩きポケモンをボールへ戻す。
// 連れ歩きの対象制限。
// 値を指定すると、種族・出会った場所・出会ったレベルの指定条件に一致するポケモンだけを連れ歩ける。VAR_TEMP_1などの変数も使用可能。
// 設定例はdocs/tutorials/how_to_new_pokemon.mdを参照する。
#define OW_FOLLOWERS_ALLOWED_SPECIES (0)
#define OW_FOLLOWERS_ALLOWED_MET_LVL (0)
#define OW_FOLLOWERS_ALLOWED_MET_LOC (0)

// 戦闘外での特性の効果。
#define OW_SYNCHRONIZE_NATURE       GEN_LATEST // 第8世代以降では、先頭がシンクロのポケモンなら野生ポケモンの性格が必ず一致する（以前は50%）。もらうポケモンは対象外。
                                               // USUM（この設定ではGEN_7）では、先頭がシンクロなら、もらうポケモンのタマゴグループに関係なく性格が一致する。
                                               // ORAS（この設定ではGEN_6）では、先頭がシンクロなら、タマゴ未発見グループのもらうポケモンの性格が一致する。
#define OW_SUPER_LUCK               GEN_LATEST // 第8世代以降では、先頭がきょううんのポケモンなら、野生の持ち物所持率が60%/20%に上がる。
#define OW_INFILTRATOR              GEN_LATEST // 第8世代以降では、先頭がすりぬけのポケモンなら、野生遭遇率が半分になる。
#define OW_HARVEST                  GEN_LATEST // 第8世代以降では、先頭がしゅうかくのポケモンなら、50%の確率でくさタイプと遭遇する。
#define OW_LIGHTNING_ROD            GEN_LATEST // 第8世代以降では、先頭がひらいしんのポケモンなら、50%の確率ででんきタイプと遭遇する。
#define OW_STORM_DRAIN              GEN_LATEST // 第8世代以降では、先頭がよびみずのポケモンなら、50%の確率でみずタイプと遭遇する。
#define OW_FLASH_FIRE               GEN_LATEST // 第8世代以降では、先頭がもらいびのポケモンなら、50%の確率でほのおタイプと遭遇する。

// 以下の定義は、OW_ALTERED_TIME_RATIOの設定でのみ区別する。
#define GEN_8_PLA                       GEN_COUNT + 2
#define TIME_DEBUG                      GEN_COUNT + 3

// 時刻。
#define OW_TIMES_OF_DAY                 GEN_LATEST   // 世代ごとに、朝・昼・夜などが切り替わる時刻を変える。
#define OW_USE_FAKE_RTC                 FALSE        // TRUEでは、ゲーム内時計の秒をplayTimeVBlanksが60進むごと（60フレームごと）に進める。
#define OW_ALTERED_TIME_RATIO           GEN_LATEST   // GEN_8_PLAでは時計の1秒につきゲーム内で60秒、GEN_9では20秒進む。TIME_DEBUGはデバッグ用の1:1。OW_USE_FAKE_RTCがFALSEなら無効。
#define OW_TIME_OF_DAY_ENCOUNTERS       TRUE        // TRUEでは、時間帯ごとに異なる遭遇テーブルを定義・使用できる。
#define OW_TIME_OF_DAY_DISABLE_FALLBACK FALSE        // TRUEでは、指定マップ・時間帯の遭遇テーブルが空なら、従来のテーブルへ戻さず、その場所で遭遇しないようにする。
#define OW_TIME_OF_DAY_FALLBACK         TIME_MORNING // 遭遇テーブルがない場合に使う代替時間帯。OW_TIMES_OF_DAYをGEN_3にする場合、TIME_DAYへ変更しないと遭遇できなくなる。

// 明るさ。
#define OW_SHADOW_INTENSITY             4       // 0～16。0は完全に透明、16は黒。
#define OW_OBJECT_SUBPRIORITY           148     // 値が大きいほど、他のスプライトより奥に表示する。影はオブジェクトイベントの背面に置く。
#define OW_ENABLE_DNS                   TRUE    // TRUEでは、時間帯に応じてフィールドの色調を変える。

// オブジェクトイベントの影。
#define OW_OBJECT_VANILLA_SHADOWS      FALSE    // FALSEでは、すべてのフィールドオブジェクトに影を付ける。注意: 各オブジェクトが1個ではなく2個のスプライトを使用する。TRUEではジャンプ中だけ影を表示する。

// フィールド用のフラグ。
// 以下の機能をスクリプトから切り替える場合、0を割り当てたいフラグIDに置き換える。
// 例えばFLAG_UNUSED_0x264に置き換えると、そのフラグで機能を切り替えられる。
#define OW_FLAG_PAUSE_TIME          0  // 指定フラグが立ち、OW_USE_FAKE_RTCも有効な場合、ゲーム内時計の秒を進めない。
#define OW_FLAG_NO_TRAINER_SEE      FLAG_EXPANSION_TRAINER_SEE_DISABLED  // 指定フラグが立っている場合、話しかけない限りトレーナー戦を開始しない。
#define OW_FLAG_NO_COLLISION        FLAG_EXPANSION_COLLISION_DISABLED    // 指定フラグが立っている場合、衝突判定のあるタイルも歩いて通過できる。主にデバッグ用。
#define OW_FLAG_POKE_RIDER          FLAG_EXPANSION_POKE_RIDER      // 指定フラグが立っている場合、ポケナビの地図やたいせつなもののタウンマップで、飛行可能な地点に合わせてRを押すとそらをとぶを使える。

#define BATTLE_PYRAMID_RANDOM_ENCOUNTERS    FALSE    // TRUEでは、src/data/battle_frontier/battle_pyramid_level_50_wild_mons.h（またはopen_level_wild_mons.h）の固定データではなく、その周回の課題に応じてポケモンをランダム生成する。

// 地名ポップアップの設定。
#define OW_POPUP_GENERATION        GEN_5    // 世代ごとに、フィールドでの地名表示方式を変える。
                                            // 対応する値はGEN_3とGEN_5のみ。他の値では第3世代方式を使う。
                                            // docs/team_procedures/scope.mdに記載したプロジェクト範囲の変更により、
                                            // Expansionにはこれ以外の地名表示方式を追加しない。

// 第5世代方式の地名ポップアップ設定。
// 定数。
#define OW_POPUP_BW_TIME_NONE      0   // 時刻を表示しない。
#define OW_POPUP_BW_TIME_12_HR     1   // 12時間制（AM/PM）で表示する。
#define OW_POPUP_BW_TIME_24_HR     2   // 24時間制で表示する。

#define OW_POPUP_BW_COLOR_BLACK    0   // ブラック2の黒いポップアップ。
#define OW_POPUP_BW_COLOR_WHITE    1   // ホワイト2の白いポップアップ。

// 設定。
#define OW_POPUP_BW_COLOR          OW_POPUP_BW_COLOR_WHITE  // B2W2で異なる地名ポップアップの色を選ぶ。
#define OW_POPUP_BW_TIME_MODE      OW_POPUP_BW_TIME_24_HR    // 時刻の表示形式を指定する。
#define OW_POPUP_BW_ALPHA_BLEND    FALSE                    // ポップアップの半透明合成を有効にする。主に黒色の表示向け。
                                                            // TRUEでは、昼夜の色調変更システムが有効な場合に表示不具合が起こる。
                                                            // ポップアップが消える際、影や反射スプライトの透明度調整にも軽微な表示不具合が起こる。

// ポケモンセンター。
#define OW_IGNORE_EGGS_ON_HEAL           GEN_LATEST         // 第4世代以降では、回復装置でタマゴを回復対象に含めない。
#define OW_UNION_DISABLE_CHECK           FALSE              // TRUEでは、ユニオンルームで待つトレーナーがいても受付で通知しない。ポケモンセンターの読み込みを速める。
#define OW_FLAG_MOVE_UNION_ROOM_CHECK    0                  // 指定フラグが立っている場合、入場時ではなく回復時だけユニオンルームの利用者を確認し、入場時の読み込みを速める。OW_UNION_DISABLE_CHECKがTRUEなら無視する。

// きのみブレンダー。
#define BERRY_BLENDER_THROW_ALL_BERRIES_AT_ONCE TRUE        // 全参加者がきのみを同時に投入し、投入アニメーションを短縮する。

// トレーナーとの再戦。
#define OW_REMATCH_BADGE_COUNT      5 // エントリーコールやバトルサーチャーで再戦するために必要なバッジ数。

// フィールドの環境音としての鳴き声。
// 定数。
#define OW_AMBIENT_CRIES_NONE           0   // 環境音の鳴き声を再生しない。
#define OW_AMBIENT_CRIES_VANILLA        1   // 原作と同様に、遭遇テーブルのポケモンの鳴き声を再生する。
#define OW_AMBIENT_CRIES_OWE_PRIORITY   2   // 出現中のOWEの鳴き声を優先し、OWEがいなければ原作方式を使う。
#define OW_AMBIENT_CRIES_OWE_ONLY       3   // 出現中のOWEの鳴き声だけを再生する。
                                            // OWEの鳴き声は、プレイヤーとの位置関係に応じて再生する。

// 設定。
#define OW_AMBIENT_CRIES                OW_AMBIENT_CRIES_OWE_ONLY // 環境音の鳴き声の再生方法を指定する。どの設定でも、原作と同様に草むら・水上の遭遇テーブルがないマップでは再生しない。


// 日替わり乱数シード。
#define OW_USE_DAILY_SEED_FOR_VANILLA_VARIABLES     FALSE // TRUEでは、マボロシじまの出現判定とくじ番号の計算に日替わりシードを使う。
                                                          // 副作用として、マボロシじまの抽選にある決定的な挙動・セーブ再読込による引き直し防止の性質が失われる。

// スクリプトの設定。
#define OW_CHECK_FOR_TOTAL_EVS TRUE        // FALSEでは、givemonとcreatemonで新しいポケモンを生成する際に努力値合計を確認しない。

#endif // GUARD_CONFIG_OVERWORLD_H
