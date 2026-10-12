#ifndef GUARD_CONFIG_POKERUS_H
#define GUARD_CONFIG_POKERUS_H

// ここでの「感染」は対戦相手（トレーナー・野生）からポケルスを得ること、「伝播」は感染済みの手持ちからほかの手持ちに移ることを指す。
#define P_POKERUS_ENABLED                TRUE       // FALSEなら、ポケルスの効果・表示・新規感染を無効にする。セーブデータ自体には影響しない。
#define P_POKERUS_STRAIN_DISTRIBUTION    GEN_LATEST // ポケルスの16種類の系統の出現確率を世代に合わせる。GEN_3はルビー・サファイア、GEN_4はエメラルドおよび第4世代の分布を使う。
#define P_POKERUS_SPREAD_ADJACENCY       GEN_LATEST // 第2世代では隣接する1匹へ、第3世代以降では両隣のポケモンへ伝播する。
#define P_POKERUS_SPREAD_DAYS_LEFT       GEN_LATEST // 第2世代では伝播直後に系統ごとの最大感染日数を得る。第3世代以降では伝播元の残り日数を引き継ぐ。
#define P_POKERUS_INFECT_AGAIN           GEN_LATEST // 第2世代では、すでにポケルス感染済みの手持ちがいても新たな感染判定を行う。手持ち内の伝播には影響しない。
#define P_POKERUS_VISIBLE_ON_EGG         GEN_LATEST // タマゴのつよさをみる画面にポケルスマークを表示するか。第3〜6世代では有効、第2・7・8世代では無効。
#define P_POKERUS_INFECT_EGG             FALSE      // TRUEなら、タマゴは感染・伝播の両方でポケルスを得る（第2世代）。FALSEなら伝播のみ（第3世代）。ほかの世代の挙動は不明。
#define P_POKERUS_FLAG_INFECTION         0          // 指定フラグが立っている場合、または0で未指定の場合に感染を許可する。第2世代のコガネシティ訪問後に感染可能になる仕組みを再現する。伝播には影響しない。

// 不具合とも考えられるポケルスの特殊な挙動。原作エメラルドでは有効だが、Expansionの既定値はFALSE（ほかの世代の挙動は不明）。
#define P_POKERUS_HERD_IMMUNITY          FALSE      // TRUEなら、感染済みで免疫を得たポケモンも感染対象の抽選に含むため、新規感染が起こる確率が下がる。
#define P_POKERUS_WEAK_VARIANT           FALSE      // TRUEなら、系統0のポケルスが、伝播時により強い系統で上書きされることがある。

#define P_POKERUS_INFECTION_ODDS         3          // 実際の感染確率はP_POKERUS_INFECTION_ODDS/65536。
#define P_POKERUS_SPREAD_ODDS            21846      // 実際の伝播確率はP_POKERUS_SPREAD_ODDS/65536。21846は原作のRandom() % 3相当。

#endif // GUARD_CONFIG_POKERUS_H
