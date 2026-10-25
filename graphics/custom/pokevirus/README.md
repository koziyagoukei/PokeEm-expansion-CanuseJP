# ポケウイルス用グラフィック

顔・目・口・頭部・手足を持たない、ウイルス状粒子の集合体。球状カプシド、多面体状の殻、分岐した短い繊維を不規則に重ね、通常のポケモンの動物的なシルエットから外したデザイン。

「じゃどくのくさり」を連想する暗い赤紫の節状の鎖が、二重螺旋として集合体を貫く。毒の継ぎ目には黄緑を少量使用し、体は暗い紫、殻の明部は灰紫。顔を作らず、輪郭と陰影はGBA世代のポケモンのドット絵を基準にしている。

## 素材

| ファイル | 寸法・用途 |
| --- | --- |
| `front.png` | 対戦相手用トレーナー正面、64x64 |
| `overworld.png` | 大型SD、384x64。64x64の6コマを横並び |
| `overworld_idle.png` | 大型SD正面静止、64x64 |
| `palette.pal` | 共通16色、JASC-PAL |
| `front_preview_8x.png` | 正面の整数8倍プレビュー |
| `overworld_preview_4x.png` | SD6コマを2列3段で表示する整数4倍プレビュー |
| `comparison_preview_4x.png` | 左が戦闘正面、右がSD正面。整数4倍 |

ゲーム用PNGは4bit indexed、16色以内、透明色はインデックス0、アルファは0または255。RGB各成分はGBAの色精度に合わせた8刻み。半透明・ぼかし・発光合成は使用しない。

大型SDのサイズは、このリポジトリのGroudon連れ歩き画像に設定されている `SIZE_64x64` を基準にした。6コマの順序は `sAnimTable_Following` に合わせ、正面静止・正面脈動・背面静止・背面脈動・左向き静止・左向き脈動。右向きは既存方式と同様、左向きの左右反転を想定する。足のある歩行ではなく、集合体が膨張・収縮する動きである。

## 変更範囲

**マップ用SDと対戦相手用画像として登録済み。ゲーム内の配置や対戦イベントは未追加。** 既存の種族、ポケモン画像、NPC配置、マップ、ストーリーテキスト、対戦ルール、セーブ形式は変更していない。新しい種族IDや特性、専用技も追加していない。

## 使用方法

感染者解放戦の上下7本の鎖演出と専用戦闘コマンドは [release_transition/README.md](release_transition/README.md) を参照。

マップのobject eventの `graphics_id` に `OBJ_EVENT_GFX_POKEVIRUS` を指定する。`OBJ_EVENT_PAL_TAG_POKEVIRUS` の専用パレットを持ち、人物や店主の色を置換しない。64x64のOAMとサブスプライト構成、6コマ用の `sAnimTable_Following` を使用する。静止した位置で脈動させる場合、`movement_type` は `MOVEMENT_TYPE_WALK_IN_PLACE_DOWN` 等の足踏み系を指定できる。浮遊するデザインのため足跡は生成しない。

画像変換は `INCGFX_U32("graphics/custom/pokevirus/overworld.png", ".4bpp", "-mwidth 8 -mheight 8")` とし、1コマを8x8タイルとして扱っている。9コマ用の通常人物アニメーションへ接続しないこと。画像が大型でも、通行判定は通常object eventのアンカー座標が基準であり、絵の範囲すべてを塞ぐ仕組みは追加していない。

対戦相手の画像IDは `TRAINER_PIC_POKEVIRUS`。`src/data/trainers.party` のトレーナーに次の指定を加えると、既存の対戦相手用画像表示経路で戦闘正面を利用できる。

```text
Pic: POKEVIRUS
```

Cのトレーナー定義では `.trainerPic = TRAINER_PIC_POKEVIRUS`。生成済みの `src/data/trainers.h` を直接編集しないこと。背面は用意していないため、プレイヤーやマルチ相方の背面IDとしては使わない。

## 生成と確認

内蔵 `image_gen` で正面とSDを生成した。原画は `source/`、採用プロンプトは `source/prompts.md`。原画自体はゲーム用の固定寸法・パレットではない。

`prepare_assets.mjs` の `prepareAssets(PNG, directory)` に `pngjs` の `PNG` と本フォルダの絶対パスを渡すと、固定寸法化・最近傍縮小・共通16色化した素材を再生成する。SDの6コマは共通倍率で処理し、中心を揃えている。ゲーム本体への画像ライブラリ依存追加はない。

`validation.json` で寸法、色数、透過、6コマがそれぞれ異なることを確認済み。既存の `tools/gbagfx/gbagfx` による4bpp・GBAパレット変換も成功した。正面は2048バイト、SDは12288バイト、各パレットは32バイトを想定し、`build/qa_pokevirus_20261025/` に変換結果を保存した。

登録用の回帰テストは `test/pokevirus_graphics.c`。大型サイズ・パレット・アニメーション参照、動的グラフィック変数16個からの参照、6コマの差異、対戦相手画像の展開範囲を検証する。実際のイベント位置での通行判定や表示優先度は、配置するマップ側で確認すること。

## 登録後の検証

- 2026-10-25: 通常ビルド成功。`make -j$(nproc) check TESTS=test/pokevirus_graphics.c` は4/4、店主の `test/rocket_cook.c` も4/4成功。
- decomps内の `mGBA-celio-rom128-4/mgba-celio.exe` でコピーROM・コピーセーブを使用。一時EWRAMフックで64x64のマップNPCを生成し、専用パレット・脈動アニメーションを確認した。対戦相手画像も `CreateTrainerPicSprite` で作成・展開し、両スプライトの解放を確認した。
- QA用ROMのSHA-256: `3d9602150c351986a71f8bb7f1618ae56fd3d34a2a059ce57df42176763ed91c`。ログ・画像・一時フックは `build/qa_pokevirus_20261025/`。製品ROMへQAコードを追加せず、元セーブも変更していない。
- 2026-10-26: 他のマップ改修を含む最新作業ツリーでも通常ビルド成功。画像関連の回帰テストも再実行し、ポケウイルス4/4・店主4/4成功。記録は同フォルダの `final_build.log`、`final_tests.log`、`final_rocket_cook_regression.log`。上記のmGBA表示試験とは別のビルドである。
- 製品マップへの配置、対戦相手のパーティ、対戦開始イベントは追加していない。実際の対戦の通しプレイも未実施。
