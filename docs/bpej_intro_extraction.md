# タイトル・オープニング素材のROM抽出

## 対象

利用者自身の日本版エメラルドROM（BPEJ）から、公式オープニング、タイトル画面の共通素材、オープニングと共有するエンディング素材を取り出します。

定義元は `tools/data/bpej_intro_manifest.json` です。98項目を抽出します。

| 分類 | 件数 | 内容 |
|---|---:|---|
| オープニング | 59 | 著作権画面、GAME FREAK、水滴、シルエット、背景、主人公と登場ポケモン、グラードン・カイオーガ・レックウザの各場面、雲・稲妻・泡、関連パレット・タイルマップ |
| 共有のエンディング素材 | 32 | 草地、昼・夕・夜のパレット、雲・木・家、自転車、主人公、ラティアス・ラティオス。既存の共有ファイル内にある素材だけが対象 |
| タイトル共通素材 | 7 | レックウザ画像とマップ、雲画像、ロゴの光、スタート・著作権表示とパレット、既存の未使用パレット |

96項目は、既存素材の変換後バイト列とROMのデータが一致します。スタート表示のパレットは `LoadSpritePalette` が実際に読む先頭32バイトを比較しており、PNG変換時の未使用パレット末尾は含めません。比較範囲は `verification.comparison_range` に記録しています。

残る2項目は、起動時の著作権画像とタイトルのスタート・著作権画像です。公式日本語版の対応画像へ意図的に替えるため、英語版との `byte_match` は `false` のまま記録します。GAME FREAKやスタート表示など、日本語版自体が英語で描いている箇所を独自に翻訳・描き直す処理ではありません。

`pokeemerald-jp` の `asm/intro.s`、`asm/intro_credits_graphics.s`、`asm/title_screen.s`、`data/data.s` と、同フォルダのタイトル・オープニング用シンボルDBを調査資料にしています。DBにない素材も今回ROMとのバイト一致とデータ範囲を独立に確認しています。調査資料のフォルダは通常ビルドに不要です。

## 独自画像は保持する

現在の「ウルトラエメラルド／ULTRA」ロゴは独自画像であり、BPEJ内の公式ロゴと同じデータではありません。これを無断で公式ロゴへ戻さないため、次の6宣言は既存経路を維持します。

- `gTitleScreenPokemonLogoGfx`
- `gTitleScreenPokemonLogoTilemap`
- `gTitleScreenBgPalettes`
- `gTitleScreenEmeraldVersionGfx`
- `gTitleScreenEmeraldVersionPal`
- `gTitleScreenCloudsTilemap`

独自ロゴ用の配置・移動・パレット構成も維持します。日本版ロゴのタイル数・マップサイズ・パレット割り当ては異なるため、一部だけを混ぜません。公式ロゴへの移行は、この6宣言と描画設定をそろえて検証する別の変更になります。

`src/expansion_intro.c` のExpansion専用起動画面、FRLG画面、ROMヘッダ、フォント、音源も今回の対象外です。ポケモン本体の汎用画像ではなく、オープニング専用に配置された画像を使います。

## ビルドと抽出

日本版エメラルドROMをプロジェクトルートの `baserom.gba` に配置します。既存のROM検証処理を使い、サイズ・ゲームコード・SHA1・CRC32を検証します。

```sh
make -j4 --output-sync=target

# この分類だけを抽出・検査する場合
make extract-bpej-intro
make check-bpej-intro-outputs

python3 tools/extract_bpej_intro.py --print-outputs
python3 tools/extract_bpej_intro.py --dry-run
python3 tools/extract_bpej_intro.py \
  --check --stamp build/generated/bpej_intro_extracted.ok
```

抽出先は `build/extracted_intro/`、stampは `build/generated/bpej_intro_extracted.ok` です。ROM本体・抽出物・stampはGit管理しません。元のPNG・PAL・BINのうち今回置換した77ファイルは削除済みです。削除前のファイルサイズ・SHA1はmanifestの `removed_sources` に記録しています。

通常のmakeで不足・破損・customの変更を検出して再生成します。同じ内容ならmtimeを変えません。並列makeでも、生成と検査を終えてから対象オブジェクトをコンパイルします。失敗時は該当ラベル・出力パスとROM配置案内を表示して停止し、英語の元画像へフォールバックしません。

graphics全体のinclude pathは変更しません。次の既存ラベルの初期化子だけを明示的な `INCBIN_U*` に替えています。

- `src/graphics.c`
- `src/title_screen.c`
- `src/intro.c`
- `src/intro_credits_graphics.c`
- `src/data/graphics/intro_scene.h`

型・固定配列の寸法・スプライトID・読み込み順・イベント進行は変えません。既存の解凍処理がGBA LZ77を受け付けるため、圧縮画像とマップはROM上のLZ77のまま、非圧縮パレットはrawのまま出力します。圧縮末尾の4バイト境界用ゼロ埋めは検証し、不明な形式は推測しません。

## manifestとcustom

各項目に以下を記録します。

- symbol ID、既存ラベル、参照するCファイル、型・配列寸法
- ROM offset、抽出サイズ、圧縮方式、圧縮本体の長さ
- 出力先、customの配置先
- ROM範囲・出力・展開後データのSHA1と展開後サイズ
- 元素材と変換指定、比較したサイズ・SHA1、バイト一致の有無
- 逆アセンブル側の参照位置と、ROMを含む `data/data.s` の範囲

`tools/extract_bpej_intro.py` はコンテスト抽出の共通検証・バイナリ処理を再利用します。ただし、許可する出力ルート・素材ルート・参照先・分類はそれぞれ独立して検査します。コンテストのmanifestがタイトル用の場所へ出力するような拡張はしません。

customは `graphics/custom/bpej/intro/` 以下に、出力先の `build/extracted_intro/` より後ろと同じ相対パスで置きます。

```text
graphics/custom/bpej/intro/intro/copyright.4bpp.lz
graphics/custom/bpej/intro/intro/copyright.bin.lz
graphics/custom/bpej/intro/intro/copyright.gbapal
```

PNGではなく、manifest指定のrawまたはGBA LZ77ファイルを置きます。展開サイズと型の整列条件を守ってください。画像・マップ・パレットは組として整合させます。custom使用時も元ROMの検証は省略しません。

`make clean` は抽出物とstampを削除し、`graphics/custom/` は削除しません。独自タイトルの元画像やExpansion追加画像も残ります。

```sh
make clean-bpej-intro-assets
```

このコマンドでは今回の抽出物とstampだけを消します。

## 検証方法

```sh
python3 -m unittest discover -s tools/tests -v
python3 tools/check_jp_text.py
git diff --check
make clean
make -j4 --output-sync=target
```

テストは元ファイルの不在、全登録範囲のハッシュと展開、保存済みの比較指紋、宣言の型・寸法、著作権画面・最初の背景のタイル参照範囲、タイトルの既存スプライトフレーム、出力件数、custom優先・削除、欠落・破損の検出・修復、mtime維持を確認します。ビルド済みROMがある環境では主要配列が抽出物と同じバイト列でリンクされたことも確認します。

実機・エミュレータでの確認対象は、電源投入から著作権表示、GAME FREAK、走行場面、伝説ポケモンの各場面、タイトルのロゴと光・スタート点滅、Aでスキップ、タイトルからメニューへの遷移、共有画像を使うエンディングです。静的な画像再構成やハッシュ検査と、ゲーム内での動作確認は区別して記録してください。

## 今回の確認結果

- 元素材163ファイルを削除後、`make clean` で両分類の抽出物・stampが消えることを確認し、`make -j4 --output-sync=target` でROM生成まで成功しました。
- オープニング・タイトル98項目、コンテスト85項目の全出力・stamp検査に成功しました。
- Pythonテスト78件がスキップなしで成功しました。生成ROM内の主要配列の照合と、clean処理がcustomを残すテストを含みます。
- `python3 tools/check_jp_text.py` と `git diff --check` に成功しました。
- 抽出データから著作権画面、タイトルのスタート・著作権表示、冒頭の背景4層を静的に再構成し、画像とタイル参照範囲を確認しました。
- 実機・エミュレータ上での再生・操作確認は未実施です。
