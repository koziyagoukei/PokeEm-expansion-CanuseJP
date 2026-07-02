# 汎用情報ビューア

## 1. 情報ビューアとは

情報ビューアは、イベントスクリプトやCコードから開けるページ式の読み物画面です。

通常の `msgbox` では扱いにくい長文説明を、ほぼ全画面のウィンドウで複数ページ表示できます。フロンティアルールのヘルプだけでなく、日記、メモ、モノローグ、施設説明、イベント資料、簡易ADV風の分岐表示にも使えます。

実装本体は `src/ultra_help.c` にあります。既存の `ultra_help` 互換を残しつつ、現在は `Special_OpenInfoViewer` / `StartInfoViewer` を使う汎用ビューアとして動作します。

表示データは主に次の2ファイルで管理します。

- `include/constants/info_viewer.h`
- `src/data/info_viewer.h`

## 2. スクリプトからの呼び出し方法

イベントスクリプトから開く場合は、呼び出し前に `VAR_0x8004` へ表示したい情報IDを入れます。

```asm
setvar VAR_0x8004, INFO_FRONTIER_RULES
special Special_OpenInfoViewer
waitstate
```

`Special_OpenInfoViewer` は `gSpecialVar_0x8004` を読み、対応する情報IDのページセットを開きます。`waitstate` で待てるため、ビューアを閉じると元のイベントスクリプトへ戻ります。

分岐テスト用の例です。

```asm
setvar VAR_0x8004, INFO_CHOICE_TEST
special Special_OpenInfoViewer
waitstate
```

既存互換として `special ultra_help` も残っています。新規用途では `Special_OpenInfoViewer` を使ってください。

## 3. C側からの呼び出し方法

Cコードから開く場合は `include/ultra_help.h` を include し、`StartInfoViewer` を呼びます。

```c
#include "ultra_help.h"

StartInfoViewer(INFO_FRONTIER_RULES, CB2_ReturnToFieldContinueScriptPlayMapMusic);
```

第1引数は情報IDです。第2引数はビューア終了後に戻る `MainCallback` です。

既存のボイスチェッカーなどで使っている互換APIも残っています。

```c
StartUltraHelp(ULTRA_HELP_TOPIC_FRONTIER_RULES, exitCallback);
```

`StartUltraHelp` は内部で `StartInfoViewer` を呼ぶラッパーです。古い呼び出しを壊さないために残しています。

## 4. 情報IDの追加方法

情報IDは `include/constants/info_viewer.h` に追加します。

```c
#define INFO_FRONTIER_RULES  0
#define INFO_BANNED_MONS     1
#define INFO_GIMMICK_HELP    2
#define INFO_DIARY_TEST      3
#define INFO_MONOLOGUE_TEST  4
#define INFO_CHOICE_TEST     5
#define INFO_COUNT           6
```

新しいIDを追加する場合は、末尾に追加して `INFO_COUNT` を増やします。

```c
#define INFO_MY_MEMO         6
#define INFO_COUNT           7
```

特殊な遷移先として、次の値が使えます。

```c
#define INFO_VIEWER_CLOSE    0xFFFE
#define INFO_VIEWER_BACK     0xFFFF
```

`INFO_VIEWER_CLOSE` はビューアを閉じて呼び出し元へ戻ります。`INFO_VIEWER_BACK` は直前に表示していたInfoViewer IDへ戻ります。

## 5. 本文ページの追加方法

本文データは `src/data/info_viewer.h` に追加します。

まずタイトルと本文ページを定義します。日本語表示にする場合は、文字列の先頭に `{JPN}` を付けてください。

```c
static const u8 sInfoText_TitleMyMemo[] = _("{JPN}メモ");

static const u8 sInfoText_MyMemoPage1[] = _(
    "{JPN}ここに メモを かきます\n"
    "ページ1です");

static const u8 sInfoText_MyMemoPage2[] = _(
    "{JPN}ページ2です\n"
    "Aで つぎへ すすみます");
```

次にページ配列を作ります。

```c
static const u8 *const sInfoViewerPages_MyMemo[] =
{
    sInfoText_MyMemoPage1,
    sInfoText_MyMemoPage2,
};
```

最後に `sInfoViewerEntries` へ登録します。

```c
[INFO_MY_MEMO] =
{
    .title = sInfoText_TitleMyMemo,
    .pages = sInfoViewerPages_MyMemo,
    .pageCount = ARRAY_COUNT(sInfoViewerPages_MyMemo),
    .choices = NULL,
    .choiceCount = 0,
    .style = &sInfoViewerStyle_Default,
},
```

主なフィールドは次の通りです。

- `title`: 画面上部に出すタイトル
- `pages`: 本文ページ配列
- `pageCount`: ページ数
- `choices`: 選択肢配列。選択肢なしなら `NULL`
- `choiceCount`: 選択肢数。選択肢なしなら `0`
- `style`: 背景色、外枠パレット、文字色のスタイル

現在は白背景の `sInfoViewerStyle_Default` と、別枠パレットを使う `sInfoViewerStyle_BlueFrame` があります。

## 6. 選択肢分岐の追加方法

選択肢分岐は `struct InfoViewerChoice` で定義します。

```c
static const u8 sInfoText_ChoiceReadMemo[] = _("{JPN}メモを よむ");
static const u8 sInfoText_ChoiceClose[] = _("{JPN}とじる");

static const struct InfoViewerChoice sInfoViewerChoices_MyMenu[] =
{
    {
        .text = sInfoText_ChoiceReadMemo,
        .nextInfoId = INFO_MY_MEMO,
    },
    {
        .text = sInfoText_ChoiceClose,
        .nextInfoId = INFO_VIEWER_CLOSE,
    },
};
```

`nextInfoId` に通常の情報IDを入れると、その情報IDへ遷移します。

```c
.nextInfoId = INFO_DIARY_TEST
```

`INFO_VIEWER_CLOSE` を入れるとビューアを閉じます。

```c
.nextInfoId = INFO_VIEWER_CLOSE
```

`INFO_VIEWER_BACK` を入れると直前のInfoViewer IDへ戻ります。戻る履歴がない場合はビューアを閉じます。

```c
.nextInfoId = INFO_VIEWER_BACK
```

選択肢を持つエントリでは、`sInfoViewerEntries` に `choices` と `choiceCount` を設定します。

```c
[INFO_CHOICE_TEST] =
{
    .title = sInfoText_TitleChoiceTest,
    .pages = sInfoViewerPages_ChoiceTest,
    .pageCount = ARRAY_COUNT(sInfoViewerPages_ChoiceTest),
    .choices = sInfoViewerChoices_ChoiceTest,
    .choiceCount = ARRAY_COUNT(sInfoViewerChoices_ChoiceTest),
    .style = &sInfoViewerStyle_BlueFrame,
},
```

`choices == NULL` または `choiceCount == 0` の場合は、従来通りページ表示だけで動作します。最後のページでAを押すと閉じます。

## 7. 操作方法

ページ表示中:

- A: 次ページ。最終ページで選択肢がなければ閉じる。選択肢があれば選択肢表示へ移る
- R: 次ページ。最終ページでは何もしない
- B: 前ページ。1ページ目では閉じる
- L: 前ページ。1ページ目では何もしない
- START: 閉じる

選択肢表示中:

- 上下: 選択肢を移動
- A: 決定
- B: 選択肢表示をやめて、最後に表示していたページへ戻る
- START: 閉じる

## 実装上の注意

情報ビューアのIDは `include/constants/info_viewer.h` の `INFO_*` だけで管理します。人物ID、ボイスチェッカーの人物定数、イベントフラグとは混ぜないでください。

本文や選択肢の文字列は `charmap.txt` で定義されている文字だけを使ってください。日本語本文は `{JPN}` を付けることを推奨します。
