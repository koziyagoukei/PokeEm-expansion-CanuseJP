#ifndef GUARD_CONFIG_SAVE_H
#define GUARD_CONFIG_SAVE_H

// Increment the format signature whenever the saved data layout changes.
#define SAVE_FORMAT_SIGNATURE               0x46520002

// Reassign the Hall of Fame, Trainer Hill and battle video sectors to the two
// save slots (16 sectors each), which lets the PC hold 18 boxes. Hall of Fame
// records, e-Reader Trainer Hill data and Frontier battle videos are not saved.
// Setting this to FALSE restores 14 boxes and needs a new SAVE_FORMAT_SIGNATURE.
#define FREE_SPECIAL_SAVE_SECTORS           TRUE

// Keep the implementation available, but omit all e-Reader entrypoints.
#define ENABLE_E_READER                     FALSE
// To restore Mystery Gift, set FREE_MYSTERY_GIFT and
// FREE_MYSTERY_EVENT_BUFFERS to FALSE, then bump SAVE_FORMAT_SIGNATURE.
// Restoring e-Reader also requires FREE_ENIGMA_BERRY and
// FREE_BATTLE_TOWER_E_READER to be FALSE.

// メニューの設定
#define SKIP_SAVE_CONFIRMATION              FALSE   // TRUEなら、レポート上書き時の「すでにレポートがある」確認を省略する。

// SaveBlock1の設定
#define FREE_EXTRA_SEEN_FLAGS_SAVEBLOCK1    FALSE   // 未使用の図鑑の発見フラグ領域を解放する（52バイト）。
#define FREE_TRAINER_HILL                   TRUE    // トレーナーヒルのデータ領域を解放する（28バイト）。
#define FREE_TRAINER_TOWER                  FALSE   // トレーナータワーのデータ領域を解放する（バイト数未記載）。
#define FREE_MYSTERY_EVENT_BUFFERS          TRUE    // ramScriptの領域を解放する（1104バイト）。
#define FREE_MATCH_CALL                     FALSE   // エントリーコールと再戦・バトルサーチャーのデータ領域を解放する（104バイト）。
#define FREE_UNION_ROOM_CHAT                FALSE   // ユニオンルームのチャット領域を解放する（212バイト）。
#define FREE_ENIGMA_BERRY                   TRUE    // カードeリーダーのナゾのみデータ領域を解放する（52バイト）。
#define FREE_LINK_BATTLE_RECORDS            FALSE   // 通信対戦記録の領域を解放する（88バイト）。
#define FREE_MYSTERY_GIFT                   TRUE    // ふしぎなおくりもののデータ領域を解放する（876バイト）。
                                            // SaveBlock1の合計: 2516バイト。
// SaveBlock2の設定
#define FREE_BATTLE_TOWER_E_READER          TRUE    // バトルタワーのカードeリーダーデータ領域を解放する（188バイト）。
#define FREE_POKEMON_JUMP                   FALSE   // ポケモンジャンプのデータ領域を解放する（16バイト）。
#define FREE_RECORD_MIXING_HALL_RECORDS     FALSE   // レコード混ぜで共有する記録の殿堂のデータ領域を解放する（1032バイト）。
#define FREE_EXTRA_SEEN_FLAGS_SAVEBLOCK2    FALSE   // 未使用の図鑑の発見フラグ領域を解放する（108バイト）。
                                            // SaveBlock2の合計: 1274バイト。

                                            // 総合計: 3790バイト。

#endif // GUARD_CONFIG_SAVE_H
