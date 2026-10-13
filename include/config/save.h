#ifndef GUARD_CONFIG_SAVE_H
#define GUARD_CONFIG_SAVE_H

// メニューの設定
#define SKIP_SAVE_CONFIRMATION              FALSE   // TRUEなら、レポート上書き時の「すでにレポートがある」確認を省略する。

// SaveBlock1の設定
#define FREE_EXTRA_SEEN_FLAGS_SAVEBLOCK1    FALSE   // 未使用の図鑑の発見フラグ領域を解放する（52バイト）。
#define FREE_TRAINER_HILL                   FALSE   // トレーナーヒルのデータ領域を解放する（28バイト）。
#define FREE_TRAINER_TOWER                  FALSE   // トレーナータワーのデータ領域を解放する（バイト数未記載）。
#define FREE_MYSTERY_EVENT_BUFFERS          FALSE   // ramScriptの領域を解放する（1104バイト）。
#define FREE_MATCH_CALL                     FALSE   // エントリーコールと再戦・バトルサーチャーのデータ領域を解放する（104バイト）。
#define FREE_UNION_ROOM_CHAT                FALSE   // ユニオンルームのチャット領域を解放する（212バイト）。
#define FREE_ENIGMA_BERRY                   FALSE   // カードeリーダーのナゾのみデータ領域を解放する（52バイト）。
#define FREE_LINK_BATTLE_RECORDS            FALSE   // 通信対戦記録の領域を解放する（88バイト）。
#define FREE_MYSTERY_GIFT                   FALSE   // ふしぎなおくりもののデータ領域を解放する（876バイト）。
                                            // SaveBlock1の合計: 2516バイト。
// SaveBlock2の設定
#define FREE_BATTLE_TOWER_E_READER          FALSE   // バトルタワーのカードeリーダーデータ領域を解放する（188バイト）。
#define FREE_POKEMON_JUMP                   FALSE   // ポケモンジャンプのデータ領域を解放する（16バイト）。
#define FREE_RECORD_MIXING_HALL_RECORDS     FALSE   // レコード混ぜで共有する記録の殿堂のデータ領域を解放する（1032バイト）。
#define FREE_EXTRA_SEEN_FLAGS_SAVEBLOCK2    FALSE   // 未使用の図鑑の発見フラグ領域を解放する（108バイト）。
                                            // SaveBlock2の合計: 1274バイト。

                                            // 総合計: 3790バイト。

#endif // GUARD_CONFIG_SAVE_H
