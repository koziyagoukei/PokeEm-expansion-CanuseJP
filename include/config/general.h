#ifndef GUARD_CONFIG_GENERAL_H
#define GUARD_CONFIG_GENERAL_H

// 第3世代では、各種デバッグビルドでアサートが使用されていた。
// ルビー・サファイアとエメラルドには残っていないが、ファイアレッドの
// ROMには残っている。発売前にNDEBUGを定義し忘れたためである。
// ただし、ルビーの実際のデバッグビルドはAGBPrintを使用しないため、
// このプロジェクトでは元の扱いを変更している。
//
// `make release`を使うとNDEBUGを自動で有効にする。
#ifdef RELEASE
#define NDEBUG
#endif

// printfによるデバッグ出力は既定で有効。各種AGBPrint関数を使用できる
// （include/gba/isagbprint.hを参照）。
// 書式付き出力の方式は下の設定で切り替える。
// printfデバッグを無効にするには、`make release`でリリースビルドを作成する。

#ifndef NDEBUG

#define PRETTY_PRINT_MINI_PRINTF (0)
#define PRETTY_PRINT_LIBC (1)

#define LOG_HANDLER_AGB_PRINT (0)
#define LOG_HANDLER_NOCASH_PRINT (1)
#define LOG_HANDLER_MGBA_PRINT (2)

// 書式付き出力を処理する方式を選ぶ。
// 注意: mini_printfは、preprocで変換した文字列を表示する独自の%S書式に対応する。
//       一部のlibc（特にdkp arm-libc）では、書式付き出力のリンクに失敗する。
#define PRETTY_PRINT_HANDLER (PRETTY_PRINT_MINI_PRINTF)

// printfの出力先を選ぶ。
// 注意: 各方式は対応するエミュレータでのみ動作するため、配布用ビルドでは使用しない。
//       未対応のエミュレータや実機ではクラッシュする可能性がある。
//       AGB_PRINTは対応するデバッグ機材で使用できる。

#define LOG_HANDLER (LOG_HANDLER_MGBA_PRINT)
#endif

// 一部の既知の軽微な不具合を修正する。無効の場合はコメントアウトを外して有効にする。
#define BUGFIX

// 未定義動作に関する不具合は、新しいコンパイラでのコンパイルを妨げることがある。
// そのため、modernコンパイラを使用する場合は常に修正を有効にする。
#if MODERN || defined(BUGFIX)
#ifndef UBFIX
#define UBFIX
#endif
#endif

// 他のプロジェクトがpokeemerald-expansionを判別するための互換用定義。
#define RHH_EXPANSION

// 後方互換性のために残している、旧ブランチ構成の定義。
#define BATTLE_ENGINE
#define POKEMON_EXPANSION
#define ITEM_EXPANSION

// 設定で世代ごとの挙動を指定するための定数。
#define GEN_1 0
#define GEN_2 1
#define GEN_3 2
#define GEN_4 3
#define GEN_5 4
#define GEN_6 5
#define GEN_7 6
#define GEN_8 7
#define GEN_9 8
#define GEN_CHAMPIONS GEN_9 + 1
#define GEN_COUNT GEN_CHAMPIONS + 1
// GEN_LATESTを別の世代に変更すると、これを参照する設定をまとめて変更できる。
#define GEN_LATEST GEN_9

// 全般設定
#define EXPANSION_INTRO              TRUE    // TRUEなら、原作の著作権表示後にRHH独自のイントロを再生する。
#define PHONEMES_SHARED              FALSE   // TRUEなら、吟遊詩人の音素をすべて同じ音声（sound/direct_sound_samples/phonemes/shared.bin）にしてROM容量を節約する。

// UNITSに指定する単位系の定数。
#define UNITS_IMPERIAL               0       // インチ・フィート・ポンド。
#define UNITS_METRIC                 1       // メートル・キログラム。
#define UNITS_METRIC_CM              2       // センチメートル・キログラム。

#define UNITS                        UNITS_METRIC_CM
#define CHAR_DEC_SEPARATOR           CHAR_PERIOD // 小数点の区切り文字。CHAR_PERIODはピリオド、CHAR_COMMAはコンマを使用する。地域の表記に合わせて選ぶ。

// 名前入力画面
#define AUTO_LOWERCASE_KEYBOARD      GEN_LATEST  // 第6世代以降では、最初の大文字を入力した後にキーボードが小文字へ切り替わる。

#endif // GUARD_CONFIG_GENERAL_H
