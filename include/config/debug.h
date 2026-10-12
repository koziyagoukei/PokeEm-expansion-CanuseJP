#ifndef GUARD_CONFIG_DEBUG_H
#define GUARD_CONFIG_DEBUG_H

// フィールドのデバッグ
#define DEBUG_OVERWORLD_MENU            FALSE // フラグ・変数の変更やポケモンの入手などを行うデバッグメニューを有効にする。既定ではフィールドでRを押しながらSTARTを押して開く。
#define DEBUG_OVERWORLD_HELD_KEYS       (R_BUTTON)          // デバッグメニューを開く際に押し続ける必要があるボタン。
#define DEBUG_OVERWORLD_TRIGGER_EVENT   pressedStartButton  // DEBUG_OVERWORLD_HELD_KEYSのボタンを押している間に、メニューを開くきっかけとなるイベント。
#define DEBUG_OVERWORLD_IN_MENU         FALSE               // ボタンの組み合わせによる起動を、STARTメニュー内の項目（図鑑の上）に置き換える。

// 戦闘のデバッグメニュー
#define DEBUG_BATTLE_MENU               FALSE // TRUEなら、戦闘中にSELECTで開けるデバッグメニューを有効にする。
#define DEBUG_AI_DELAY_TIMER            FALSE // TRUEなら、AIが技を選ぶまでにかかったフレーム数を行動選択時のメッセージの代わりに表示する。AI変更時の処理時間確認に使用できる。

// ポケモンのデバッグ
#define DEBUG_POKEMON_SPRITE_VISUALIZER FALSE // つよさをみる画面でSELECTを押すと開く、ポケモンのスプライト・アイコン確認用メニューを有効にする。

#endif // GUARD_CONFIG_DEBUG_H
