#ifndef GUARD_CONFIG_FISHING_H
#define GUARD_CONFIG_FISHING_H

#define I_FISHING_BITE_ODDS         GEN_LATEST // 第1・2世代ではボロのつりざお100%、いいつりざお66%、すごいつりざお50%。第3世代では全種類の基本確率が50%。第4世代以降ではそれぞれ25%、50%、75%になる。
#define I_FISHING_MINIGAME          GEN_3      // 食いついたポケモンを釣り上げる操作は世代ごとに異なる。注意: 現在は第1・2世代と第3世代の操作のみ実装済み。
#define I_FISHING_ENVIRONMENT       GEN_LATEST // 第3世代ではプレイヤーの立つマスを戦闘の地形に使う。第4世代以降では釣り先のマスを使うため、通常は水上の地形になる。
#define I_FISHING_STICKY_BOOST      GEN_LATEST // 第3世代では先頭の特性がきゅうばん・ねんちゃくの場合、食いつく確率が約35%増加する。第4世代以降では基本確率が2倍になる。
#define I_FISHING_FOLLOWER_BOOST    FALSE      // HGSSでは、連れ歩いているポケモンのなつき度に応じて食いつく確率が増加する。
#define I_FISHING_CHAIN             FALSE      // XYの連続釣り。同じポケモンを続けて釣ると、そのポケモンが色違いになる確率が上がる。XYの仕様は十分に解析されていないため、この実装は近似である。
#define I_FISHING_PROXIMITY         FALSE      // XYでは、釣り先に隣接するなみのり不可のマスの数に応じて食いつく確率が上がる。
#define I_FISHING_TIME_OF_DAY_BOOST FALSE      // XYでは、朝・夕方に食いつく確率が上がる。

#endif // GUARD_CONFIG_FISHING_H
