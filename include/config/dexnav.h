#ifndef GUARD_CONFIG_DEXNAV_H
#define GUARD_CONFIG_DEXNAV_H

#define DEXNAV_ENABLED                TRUE   // ずかんナビを有効にするか。TRUEの場合、下記のフラグ・変数IDはすべて0以外にする必要がある。
#define USE_DEXNAV_SEARCH_LEVELS      FALSE  /* 注意: 1種族につき1バイト必要なため、セーブ領域を超える可能性がある。 */

// フラグ・変数の定義
#define DN_FLAG_SEARCHING             FLAG_EXPANSION_DEXNAV_SEARCHING      // ポケモンをサーチ中であることを示すフラグ。
#define DN_FLAG_DEXNAV_GET            FLAG_EXPANSION_DEXNAV_DISPLAY  // STARTメニューにずかんナビを表示するフラグ。
#define DN_FLAG_DETECTOR_MODE         FLAG_EXPANSION_DEXNAV_DETECT   // 隠れたポケモンを発見できるようにするフラグ。
#define DN_VAR_SPECIES                VAR_EXPANSION_DEXNAV_SPECIES         // ずかんナビに登録した種族。
#define DN_VAR_STEP_COUNTER           VAR_EXPANSION_DEXNAV_STEP_COUNTER    // 隠れたポケモンを探すための歩数。

// サーチの設定
#define DEXNAV_TIMEOUT                  15  // サーチの制限時間（秒）。この値は15秒。最大1092秒まで指定できる。
#define SNEAKING_PROXIMITY              4   // 距離をマス数で指定する。
#define CREEPING_PROXIMITY              2
#define MAX_PROXIMITY                   20

#define DEXNAV_CHAIN_MAX                100 // 連続サーチ回数の上限。

// 隠れたポケモンの設定。原作の十分な情報がないため、数値は近似値を使用する。
#define HIDDEN_MON_STEP_COUNT       100  // 指定歩数ごとに隠れたポケモンの発見判定を行う。
#define HIDDEN_MON_SEARCH_RATE      25   // 歩数による判定時に、隠れたポケモンを発見する確率（%）。
#define HIDDEN_MON_PROBABILTY       15   // 通常の遭遇データと比べて、隠れたポケモンを選ぶ確率（%）。

//// サーチレベル別の確率
// 参照: https://bulbapedia.bulbagarden.net/wiki/DexNav#Benefits
// 各サーチレベルでタマゴ技を持つポケモンが出現する確率。
#define SEARCHLEVEL0_MOVECHANCE         0
#define SEARCHLEVEL5_MOVECHANCE         21
#define SEARCHLEVEL10_MOVECHANCE        46
#define SEARCHLEVEL25_MOVECHANCE        58
#define SEARCHLEVEL50_MOVECHANCE        63
#define SEARCHLEVEL100_MOVECHANCE       83
// 各サーチレベルで隠れ特性を持つポケモンが出現する確率。
#define SEARCHLEVEL0_ABILITYCHANCE      0
#define SEARCHLEVEL5_ABILITYCHANCE      0
#define SEARCHLEVEL10_ABILITYCHANCE     5
#define SEARCHLEVEL25_ABILITYCHANCE     15
#define SEARCHLEVEL50_ABILITYCHANCE     20
#define SEARCHLEVEL100_ABILITYCHANCE    23
// 持ち物を持つポケモンが出現する確率。
#define SEARCHLEVEL0_ITEM               0
#define SEARCHLEVEL5_ITEM               0
#define SEARCHLEVEL10_ITEM              1
#define SEARCHLEVEL25_ITEM              7
#define SEARCHLEVEL50_ITEM              6
#define SEARCHLEVEL100_ITEM             12
// 能力の潜在評価が星1つとなる確率。
#define SEARCHLEVEL0_ONESTAR            0
#define SEARCHLEVEL5_ONESTAR            14
#define SEARCHLEVEL10_ONESTAR           17
#define SEARCHLEVEL25_ONESTAR           17
#define SEARCHLEVEL50_ONESTAR           15
#define SEARCHLEVEL100_ONESTAR          8
// 能力の潜在評価が星2つとなる確率。
#define SEARCHLEVEL0_TWOSTAR            0
#define SEARCHLEVEL5_TWOSTAR            1
#define SEARCHLEVEL10_TWOSTAR           9
#define SEARCHLEVEL25_TWOSTAR           16
#define SEARCHLEVEL50_TWOSTAR           17
#define SEARCHLEVEL100_TWOSTAR          24
// 能力の潜在評価が星3つとなる確率。
#define SEARCHLEVEL0_THREESTAR          0
#define SEARCHLEVEL5_THREESTAR          0
#define SEARCHLEVEL10_THREESTAR         1
#define SEARCHLEVEL25_THREESTAR         7
#define SEARCHLEVEL50_THREESTAR         6
#define SEARCHLEVEL100_THREESTAR        12

#endif // GUARD_CONFIG_DEXNAV_H
