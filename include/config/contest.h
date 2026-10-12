#ifndef GUARD_CONFIG_CONTEST_H
#define GUARD_CONFIG_CONTEST_H

// コンテスト情報の非表示
#define C_HIDE_CONTEST_DATA         FALSE      // TRUEなら、つよさをみる画面と技思い出し画面でコンテスト情報を非表示にする。
                                               // 注意: データ自体は削除せず、表示だけを隠す。この設定を有効にしてもセーブ互換性は失われない。

// 技データの設定
#define C_UPDATED_MOVE_CATEGORIES   GEN_LATEST // 指定世代のコンテスト部門を使用する。
#define C_UPDATED_MOVE_EFFECTS      GEN_LATEST // 指定世代のコンテスト効果を使用する。

#endif // GUARD_CONFIG_CONTEST_H
