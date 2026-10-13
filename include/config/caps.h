#ifndef GUARD_CONFIG_CAPS_H
#define GUARD_CONFIG_CAPS_H

// レベル上限の定数
#define EXP_CAP_NONE                    0 // 通常の挙動。レベル上限による制限を適用しない。
#define EXP_CAP_HARD                    1 // レベルが上限以上のポケモンは経験値を獲得できない。
#define EXP_CAP_SOFT                    2 // レベルが上限以上のポケモンは獲得経験値が減少する。

#define LEVEL_CAP_NONE                  0 // レベル上限なし。B_EXP_CAP_TYPEがEXP_CAP_NONEの場合にのみ使用する。
#define LEVEL_CAP_FLAG_LIST             1 // sLevelCapFlagMap内で最初に未設定となっているフラグに応じて上限を決める。
#define LEVEL_CAP_VARIABLE              2 // B_LEVEL_CAP_VARIABLEで指定したイベント変数の値から上限を決める。

// レベル上限の設定
#define B_EXP_CAP_TYPE                  EXP_CAP_NONE   // [EXP_CAP_NONE, EXP_CAP_HARD, EXP_CAP_SOFT] 上限到達時の経験値制限方式を選ぶ。
#define B_LEVEL_CAP_TYPE                LEVEL_CAP_NONE // [LEVEL_CAP_NONE, LEVEL_CAP_FLAG_LIST, LEVEL_CAP_VARIABLE] レベル上限の決定方法を選ぶ。
#define B_LEVEL_CAP_VARIABLE            0 // B_LEVEL_CAP_TYPEがLEVEL_CAP_VARIABLEの場合に、レベル上限を取得するイベント変数。

#define B_RARE_CANDY_CAP                FALSE // TRUEなら、ふしぎなアメでレベル上限を超えられない。
#define B_LEVEL_CAP_EXP_UP              FALSE // TRUEなら、レベル上限未満のポケモンの獲得経験値を増やす。

// 努力値上限の定数
#define EV_CAP_NONE                     0 // 通常の挙動。追加の努力値上限を適用しない。
#define EV_CAP_FLAG_LIST                1 // sEVCapFlagMap内で最初に未設定となっているフラグに応じて上限を決める。
#define EV_CAP_VARIABLE                 2 // B_EV_CAP_VARIABLEで指定したイベント変数の値から上限を決める。
#define EV_CAP_NO_GAIN                  3 // 努力値を獲得できない。

// 努力値上限の設定
#define B_EV_CAP_TYPE                   EV_CAP_NONE   // [EV_CAP_NONE, EV_CAP_FLAG_LIST, EV_CAP_VARIABLE, EV_CAP_NO_GAIN] 努力値上限の方式を選ぶ。
#define B_EV_CAP_VARIABLE               8 // B_EV_CAP_TYPEがEV_CAP_VARIABLEの場合に、努力値上限を取得するイベント変数。

#define B_EV_ITEMS_CAP                  FALSE // TRUEなら、努力値を増やす道具で努力値上限を超えられない。

#endif /* GUARD_CONFIG_CAPS_H */
