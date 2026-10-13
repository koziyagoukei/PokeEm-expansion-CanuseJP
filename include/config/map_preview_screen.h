#ifndef GUARD_CONFIG_MAP_PREVIEW_H
#define GUARD_CONFIG_MAP_PREVIEW_H

#define MPS_ENABLE_MAP_PREVIEWS     IS_FRLG // マップのプレビュー画面を有効にする。

#define MPS_DURATION_LONG       120 // 初めてそのマップに入ったときのプレビュー表示時間（フレーム数）。
#define MPS_DURATION_SHORT      40  // 訪問済みのマップに入ったときのプレビュー表示時間（フレーム数）。
#define MPS_DURATION_NO_FLAG    40  // マップのflagIdがMPS_FLAG_NULLの場合のプレビュー表示時間（フレーム数）。
#define MPS_BASIC_FADE_SPEED    1   // MPS_TYPE_BASICで、プレビューが暗転する速さを設定する。
                                    // 1未満の値では暗転時間が短くなる（負の値も指定可能）。
                                    // 1より大きい値では暗転時間が長くなる。

#endif // GUARD_CONFIG_MAP_PREVIEW_H
