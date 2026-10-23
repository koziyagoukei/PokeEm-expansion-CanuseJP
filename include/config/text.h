#ifndef GUARD_CONFIG_TEXT_H
#define GUARD_CONFIG_TEXT_H

// テキストの設定。
#define TEXT_JAPANESE_BY_DEFAULT     TRUE    // 無指定の文字列は日本語表示。FALSEなら従来の文字判定を使う。{ENG}/{JPN}/{AUTO}はどちらでも有効。
#define AUTO_SCROLL_TEXT             FALSE   // TRUEなら、NUM_FRAMES_AUTO_SCROLL_DELAY経過後に次の行へ自動で送る。A_BUTTON・B_BUTTONによる手動送りも可能。
#define NUM_FRAMES_AUTO_SCROLL_DELAY 49

// 速度補正はおおむね倍率として働き、1なら原作と同じ速度になる。スクロールや下向き矢印のアニメーションの待ち時間にも使うため、A/B入力と文字表示がずれないよう上限を31にしている。
// テスト上、18〜20はほぼ瞬間表示と同じ速度になる。
#define TEXT_SPEED_SLOW_MODIFIER     1       // 「おそい」を選んだときの文字表示速度の補正。
#define TEXT_SPEED_MEDIUM_MODIFIER   1       // 「ふつう」を選んだときの文字表示速度の補正。
#define TEXT_SPEED_FAST_MODIFIER     1       // 「はやい」を選んだときの文字表示速度の補正。
#define TEXT_SPEED_INSTANT_MODIFIER  12      // 瞬間表示時のアニメーション待ち時間にのみ使用する。
#define TEXT_SPEED_INSTANT           FALSE   // 全テキストを可能な限り速く、ほぼ瞬間表示する。FLAG_TEXT_SPEED_INSTANTとゲーム内の設定より優先する。

// 文字表示速度のフラグ。
#define FLAG_TEXT_SPEED_INSTANT      0       // 瞬間表示をフラグで切り替える場合に指定する。

#endif // GUARD_CONFIG_TEXT_H
