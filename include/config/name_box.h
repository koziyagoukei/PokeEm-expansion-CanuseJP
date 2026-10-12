#ifndef GUARD_CONFIG_NAME_BOX_H
#define GUARD_CONFIG_NAME_BOX_H

#define OW_FLAG_SUPPRESS_NAME_BOX   0  // このフラグが立っている間は、マクロ・コードのどちらから呼び出した名前ウィンドウも表示しない。

// 話者名ウィンドウの設定
#define OW_NAME_BOX_USE_DYNAMIC_WIDTH TRUE  // TRUEなら、話者名の長さに応じてウィンドウ幅を変える。
#define OW_NAME_BOX_NPC_TRAINER       FALSE // TRUEなら、近づいてくるNPCトレーナーの名前を自動表示する。名前はトレーナーデータから取得する。
#define OW_NAME_BOX_DEFAULT_WIDTH     8     // OW_NAME_BOX_USE_DYNAMIC_WIDTHによる可変幅の上限。可変幅が無効、またはこの値を超えた場合の既定幅でもある。
#define OW_NAME_BOX_DEFAULT_HEIGHT    2     // 名前ウィンドウの高さの上限。

// 名前ウィンドウの文字色。数値はパレットのインデックスを指定する。
// 背景色は常に0である必要があるため、設定項目を設けていない。
#define OW_NAME_BOX_FOREGROUND_COLOR  1
#define OW_NAME_BOX_SHADOW_COLOR      2

#endif // GUARD_CONFIG_NAME_BOX_H
