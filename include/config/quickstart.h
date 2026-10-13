#ifndef GUARD_CONFIG_QUICKSTART_H
#define GUARD_CONFIG_QUICKSTART_H

#define GENDER_MALE              0
#define GENDER_FEMALE            1
#define GENDER_RANDOM            2

// クイックスタートの設定
#define ENABLE_QUICKSTART            TRUE  // TRUEなら、タイトル画面でSELECTを押して新しいゲームをすぐ開始できる（リリースビルドでは無効）。
#define QUICKSTART_HUD               TRUE  // クイックスタート有効時に、タイトル画面へ小さな表示を追加する。
#define QUICKSTART_GENDER            GENDER_RANDOM

#define QUICKSTART_HUD_X             (DISPLAY_WIDTH - 32) // クイックスタート表示のX座標。
#define QUICKSTART_HUD_Y             (16)                 // クイックスタート表示のY座標。

#endif // GUARD_CONFIG_QUICKSTART_H
