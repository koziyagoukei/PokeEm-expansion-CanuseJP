# スタイルガイドと方針

## 命名規則

関数名と構造体名は `PascalCase` にしてください。

```c
void ThisIsCorrect(void);

struct MyStruct
{
    u8 firstField;
    u16 secondField;
    ...
};
```

変数名と構造体メンバー名は `camelCase` にしてください。

```c
int thisIsCorrect = 0;
```

グローバル変数には `g` を、static 変数には `s` を接頭辞として付けてください。

```c
extern s32 gMyGlobalVariable;

static u8 sMyStaticVariable = 0;
```

マクロと定数には `CAPS_WITH_UNDERSCORES` を使ってください。

```c
#define MAX_LEVEL 100

enum
{
    COLOR_RED,
    COLOR_BLUE,
    COLOR_GREEN,
};

#define ADD_FIVE(x) ((x) + 5)
```

## コーディングスタイル

### コメント

理想的には、変数名、関数名、定数名だけで動作を説明できるようにし、コメントに頼りすぎないようにします。コメントを使う場合は、特定の仕組みやコンポーネントがその形になっている _理由_ を説明してください。

システムやコンポーネントを詳しく説明する場合は、ブロックコメントを使ってください。

```c
/*
 * This is an in-depth description of the save block format. Its format is as follows:
 *
 * Sectors  0 - 13: Save Slot 1
 * Sectors 14 - 27: Save Slot 2
 * ...
 */
```

関数やコードブロックを短く説明する場合は、単独行のコメントを使ってください。
`//` の右側には空白を 1 つ入れてください。

```c
// This is supplemental information for the function. If there is a bunch of info, it should
// carry on to the next line.
void ProcessSingleTask(void)
{
   // Short comment describing some noteworthy aspect of the code immediately following.
   ...
   // Comments should be capitalized and end in a period.
}
```

`enum` や重要な値に対応するデータ構造へ注釈を付ける場合は、コメントをコードと同じ行に置いてください。

```c
const u8 gPlantlikeMons[] =
{
    FALSE, // SPECIES_BULBASAUR
    FALSE, // SPECIES_IVYSAUR
    TRUE,  // SPECIES_VENUSAUR
    FALSE, // SPECIES_CHARMANDER
    ...
};
```

### 空白

すべての `.c` と `.h` ファイルでは、タブではなく 4 スペースを使ってください。
アセンブラファイル、つまり `.s` ではタブを使います。
スクリプトファイル、つまり `.inc` でもタブを使います。

### 演算子

代入演算子と比較演算子の両側には空白を 1 つ入れてください。

```c
int i = 0; // correct
int i=0;   // incorrect

a > b // correct
a>b   // incorrect
```

インクリメント演算子とデクリメント演算子には空白を入れないでください。

```c
i++;  // correct
i ++; // incorrect
```

制御文では、キーワードと式の間に空白を入れ、開始波括弧は次の行に置いてください。

```c
for (...)
{
    // correct
}

for(...) {
    // incorrect
}
```

`switch` 文の `case` は、`switch` ブロックと同じ位置に左揃えしてください。

```c
switch (foo)
{
case 0: // correct
    ...
    break;
}

switch (foo)
{
    case 0: // incorrect
        ...
        break;
}
```

ブロックの後には空行を 1 行入れてください。

```c
int MyFunction(int bar)
{
    int foo = 0;
    if (bar)
        foo++;

    return foo; // correct
}

int MyFunction(int bar)
{
    int foo = 0;
    if (bar)
        foo++;
    return foo; // incorrect
}
```

`if-else` の連鎖で、条件またはブロックが 1 行を超える場合は波括弧を使ってください。すべてのブロックと条件が 1 行だけの場合は、波括弧なしでもかまいません。

```c
if (foo) // correct
    return 1;

if (foo
 && bar) // correct
{
    return 1;
}

if (foo) // correct
{
    return 1;
}
else
{
    MyFunction();
    return 0;
}

if (foo) // correct
{
    return 1;
}
else if (foo
      && bar)
{
    return 0;
}

if (foo) // incorrect
{
    return 1;
}

if (foo
 && bar) // incorrect
    return 1;

if (foo) // incorrect
    return 1;
else
{
    MyFunction();
    return 0;
}

if (foo) // incorrect
    return 1;
else if (foo
      && bar)
    return 0;
```

例外として、`assertf` に復帰処理がある場合は、条件と処理が 1 行ずつでも常に波括弧を使ってください。

```c
assertf(true); // correct

assertf(true) // correct
{
    return NULL;
}

assertf(true) // incorrect
    return NULL;
```

### 制御構造

値が `0` と等しいかどうかを比較する場合、必要な場面を除いて明示的に書かないでください。

```c
if (runTasks) // correct
    RunTasks();

if (runTasks != 0) // incorrect
    RunTasks();

if (!PlayerIsOutside()) // correct
    RemoveSunglasses();

if (PlayerIsOutside() == 0) // incorrect
    RemoveSunglasses();
```

本体のない `for` または `while` ループを書く場合は、空の波括弧ではなく同じ行にセミコロン `;` を置いてください。

```c
for (i = 0; gParty[i].species != SPECIES_NONE; i++); // correct

for (i = 0; gParty[i].species != SPECIES_NONE; i++) // incorrect
{ }
```

### インライン設定

設定で制御される機能を追加する場合、実行時にデータ構造の変更が必要な場合を除き、define の確認は関数の通常の制御フロー内で行ってください。

```c
void SetCurrentDifficultyLevel(enum DifficultyLevel desiredDifficulty)
{
#ifdef B_VAR_DIFFICULTY
    return; // Incorrect
#endif

    if (desiredDifficulty > DIFFICULTY_MAX)
        desiredDifficulty = DIFFICULTY_MAX;

    VarSet(B_VAR_DIFFICULTY, desiredDifficulty);
}
```

```c
void SetCurrentDifficultyLevel(enum DifficultyLevel desiredDifficulty)
{
    if (!B_VAR_DIFFICULTY) // Correct
        return;

    if (desiredDifficulty > DIFFICULTY_MAX)
        desiredDifficulty = DIFFICULTY_MAX;

    VarSet(B_VAR_DIFFICULTY, desiredDifficulty);
}
```

```c
    [MOVE_VINE_WHIP] =
    {
        .name = COMPOUND_STRING("Vine Whip"),
        .description = COMPOUND_STRING(
            "Strikes the foe with\n"
            "slender, whiplike vines."),
        #if B_UPDATED_MOVE_DATA >= GEN_6 // Correct
            .pp = 25,
        #elif B_UPDATED_MOVE_DATA >= GEN_4
            .pp = 15,
        #else
            .pp = 10,
        #endif
        .effect = EFFECT_HIT,
        .power = B_UPDATED_MOVE_DATA >= GEN_6 ? 45 : 35,
    },
```

### 変数宣言

ループイテレータは、よほど強い理由がない限りループの一部として宣言してください。

```C
for (u32 i = 0; i < LOOP_ITERATIONS; i++)
{
    dst1[i] = i;
    dst2[i] = i;
}
```

## データ型のサイズ

変数に数値を使う場合、一般的には `u32`、符号付きなら `s32` を使ってください。ただし、次の例外があります。

* セーブブロックに保存される値は、可能な限り小さいデータ型にしてください。
* `EWRAM` 変数は、可能な限り小さいデータ型にしてください。
* グローバル変数とグローバル構造体メンバーは、可能な限り小さいデータ型にしてください。

## 定数、enum、型チェック

可能な限りマジックナンバーは避けてください。定数を使うことで、その値が使われる理由が明確になります。

```c
// Incorrect
        if (gimmick == 5 && mon->teraType != 0)
            return TRUE;
        if (gimmick == 4 && mon->shouldUseDynamax)
            return TRUE;
```

```c
// Correct
#define TYPE_NONE 0
#define GIMMICK_DYNAMAX 4
#define GIMMICK_TERA 5

        if (gimmick == GIMMICK_TERA && mon->teraType != TYPE_NONE)
            return TRUE;
        if (gimmick == GIMMICK_DYNAMAX && mon->shouldUseDynamax)
            return TRUE;
```

連続した複数の数値を使い、その値がセーブブロックで使われない場合は、代わりに enum を使ってください。

```c
//Correct
enum Gimmick
{
    GIMMICK_NONE,
    GIMMICK_MEGA,
    GIMMICK_ULTRA_BURST,
    GIMMICK_Z_MOVE,
    GIMMICK_DYNAMAX,
    GIMMICK_TERA,
    GIMMICKS_COUNT,
};

        if (gimmick == GIMMICK_TERA && mon->teraType != TYPE_NONE)
            return TRUE;
        if (gimmick == GIMMICK_DYNAMAX && mon->shouldUseDynamax)
            return TRUE;
```

enum を使う場合は、誤った値の設定を防ぐため、通常の数値型ではなく enum 型を使ってください。

```c
// Incorrect
bool32 CanActivateGimmick(u32 battler, u32 gimmick)
{
    return gGimmicksInfo[gimmick].CanActivate != NULL && gGimmicksInfo[gimmick].CanActivate(battler);
}

u32 GetCurrentDifficultyLevel(void)
{
    if (!B_VAR_DIFFICULTY)
        return DIFFICULTY_NORMAL;

    return VarGet(B_VAR_DIFFICULTY);
}
```

```c
//Correct

bool32 CanActivateGimmick(u32 battler, enum Gimmick gimmick)
{
    return gGimmicksInfo[gimmick].CanActivate != NULL && gGimmicksInfo[gimmick].CanActivate(battler);
}

enum DifficultyLevel GetCurrentDifficultyLevel(void)
{
    if (!B_VAR_DIFFICULTY)
        return DIFFICULTY_NORMAL;

    return VarGet(B_VAR_DIFFICULTY);
}
```

## データファイル形式

外部データファイルには JSON を使ってください。

## 方針

### できるだけ侵襲を小さくする

新機能は、既存ファイルへの影響を可能な限り小さくしてください。大量の新規コードを追加する場合は、専用ファイルへ分離するのが望ましいです。

[`B_VAR_DIFFICULTY`](https://patch-diff.githubusercontent.com/raw/rh-hideout/pokeemerald-expansion/pull/5337.diff) のプルリクエストは、大量の新規コードを低侵襲な形で導入している良い例です。

### `UNUSED`

関数やデータを追加しても呼び出されない場合は、`UNUSED` と指定します。必要がない限り、`UNUSED` 関数は追加しないでください。

```c
static void UNUSED PadString(const u8 *src, u8 *dst)
{
    u32 i;

    for (i = 0; i < 17 && src[i] != EOS; i++)
        dst[i] = src[i];

    for (; i < 17; i++)
        dst[i] = CHAR_SPACE;

    dst[i] = EOS;
}
```

### 設定に関する考え方

セーブデータを変更する可能性があるブランチでは、その機能を設定で制御できるようにし、デフォルトでは無効にしてください。

ブランチに次のいずれかを行う設定がある場合、その設定はデフォルトで有効にしてください。

* バックエンドや開発者の作業効率を改善する。
* 現在のモダンなポケモンを再現する。

唯一の例外は、Pokemon Champions だけを出典とするコンテンツです。Champions コンテンツは `GEN_CHAMPIONS` 設定を使うべきですが、メンテナーが明示しない限り `GEN_LATEST` は `GEN_9` のままです。

Game Freak の方針が一貫していない挙動をブランチが扱う場合、その設定のデフォルト挙動はメンテナーが議論してください。

その他の設定はすべてデフォルトで無効にしてください。

### セーブに関する考え方

[セーブ移行](https://discord.com/channels/419213663107416084/1108733346864963746)が実装されるまでは、既存のゲームセーブを強制的に壊さないブランチだけがマージされます。

`pokeemerald-expansion` が、新機能のためにセーブ破壊を避けられない段階に到達した場合は、できるだけ多くの[セーブ破壊を伴う機能](https://discord.com/channels/419213663107416084/1202774957776441427)をまとめてマージし、プロジェクトのメジャーバージョン番号を上げます。

# 帰属

* このスタイルガイドの大部分は、[garakmon](https://github.com/garakmon) によって、[pokefirered への PR](<https://github.com/pret/pokefirered/pull/63>) の一部として書かれました。
