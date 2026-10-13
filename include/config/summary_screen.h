#ifndef GUARD_CONFIG_SUMMARY_SCREEN_H
#define GUARD_CONFIG_SUMMARY_SCREEN_H

// 基本設定
#define P_SUMMARY_SCREEN_NATURE_COLORS   TRUE   // TRUEなら、つよさをみる画面で性格による能力上昇を赤、低下を青で表示する。
#define P_SUMMARY_SCREEN_RENAME          TRUE   // TRUEなら、情報ページの戻る案内をニックネーム変更の案内に置き換える。

// 個体値（IV）・努力値（EV）の設定
#define P_SUMMARY_SCREEN_IV_EV_INFO      TRUE   // TRUEなら、能力ページで能力値・個体値・努力値の表示を切り替えられる。
#define P_SUMMARY_SCREEN_IV_EV_BOX_ONLY  FALSE  // TRUEなら、ボックス内でのつよさをみる画面に限り、能力値・個体値・努力値の表示を切り替えられる。
#define P_SUMMARY_SCREEN_IV_HYPERTRAIN   TRUE   // TRUEなら、すごいとっくん済みの能力の個体値を31またはSと表示する。
#define P_SUMMARY_SCREEN_IV_EV_TILESET   FALSE  // TRUEなら、能力ページの「STATS」表示を変更できる別のタイルセットを読み込む。設定変更後も以前のタイルセットが使われる場合は、ビルド前に`make clean`が必要なことがある。
#define P_SUMMARY_SCREEN_IV_EV_VALUES    TRUE   // TRUEなら、ランク記号の代わりに個体値の数値を表示する。
/*
ランク記号と個体値の対応:

    F = 0
    D = 1 - 15
    C = 16 - 25
    B = 26 - 29
    A = 30
    S = 31

参照元: https://bulbapedia.bulbagarden.net/wiki/Stats_judge.
*/
#define P_SUMMARY_SCREEN_IV_ONLY         FALSE  // TRUEなら、個体値の情報だけを表示する。
#define P_SUMMARY_SCREEN_EV_ONLY         FALSE  // TRUEなら、努力値の情報だけを表示する。

// 個体値・努力値表示のフラグ
#define P_FLAG_SUMMARY_SCREEN_IV_EV_INFO FLAG_EXPANSION_SUMMARY_SCREEN_IV_EV_INFO // このフラグが立つと、能力ページで能力値・個体値・努力値を切り替えられる。P_SUMMARY_SCREEN_IV_EV_INFOがTRUEの場合、このフラグは使わない。

// 技思い出しの設定
#define P_ENABLE_MOVE_RELEARNERS         TRUE    // TRUEなら、タマゴ技・技マシン・教え技の思い出しを有効にする。個別の設定・フラグは下記を参照。
#define P_SORT_MOVES                     TRUE    // TRUEなら、技思い出しの一覧を名前順に並べる。

// レベルアップ技の思い出し
#define P_PRE_EVO_MOVES                  TRUE    // TRUEなら、進化前のポケモンが覚える技も習得できる。
#define P_ENABLE_ALL_LEVEL_UP_MOVES      TRUE    // TRUEなら、現在のレベルに関係なくすべてのレベルアップ技を習得できる。

// 技マシンの思い出し
#define P_TM_MOVES_RELEARNER             TRUE    // TRUEなら、技マシンの技思い出しを有効にする。
#define P_ENABLE_ALL_TM_MOVES            TRUE    // TRUEなら、バッグ内の所持状況に関係なく、対応するすべての技マシンの技を習得できる。

// 技思い出しのフラグ。P_ENABLE_MOVE_RELEARNERSがTRUEなら不要だが、種類ごとに解禁したい場合に使用できる。
// スクリプトから制御するには、0の項目に使用するフラグIDを指定する。
// 例: FLAG_UNUSED_0x264を指定すると、そのフラグで機能を切り替えられる。
#define P_FLAG_EGG_MOVES                 FLAG_EXPANSION_EGG_MOVE_RELEARNER    // このフラグが立つと、タマゴ技の思い出しを有効にする。
#define P_FLAG_TUTOR_MOVES               FLAG_EXPANSION_TUTOR_MOVE_RELEARNER  // このフラグが立つと、教え技の思い出しを有効にする。

// つよさをみる画面での技思い出し
#define P_SUMMARY_SCREEN_MOVE_RELEARNER  TRUE   // TRUEなら、技ページに技思い出しの選択肢を表示する。
#define P_SUMMARY_MOVE_RELEARNER_FULL_PP TRUE   // TRUEなら、この画面で思い出した技のPPを全回復する。

#endif // GUARD_CONFIG_SUMMARY_SCREEN_H
