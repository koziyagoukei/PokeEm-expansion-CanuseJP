# ファイアレッド／リーフグリーン（カントー）基盤でビルドする

このプロジェクトは、既定のエメラルド（ホウエン）に加えて、pokeemerald-expansion 本家と同じく
ファイアレッド／リーフグリーン（FRLG、カントー）を基盤にしたROMもビルドできます。
このページでは、FRLG でビルドするときに必要なものと手順を説明します。

開発環境（WSL・arm-none-eabi-gcc など）の準備は [INSTALL.md](../INSTALL.md) と同じです。
FRLG 版の作り方そのもの（Porymap の設定など）は本家の [How to use FRLG](tutorials/how_to_frlg.md) も参照してください。

## 必要なROM

ビルドする版ごとに、元になる日本版ROMが1本必要です。ROMはプロジェクトのルートに置きます。

| ビルド | 必要なROM | ファイル名 |
| --- | --- | --- |
| エメラルド（`make`） | 日本版エメラルド | `baserom.gba` |
| ファイアレッド（`make firered`）／リーフグリーン（`make leafgreen`） | 日本版ファイアレッド **または** 日本版リーフグリーン | `baserom_fr.gba` または `baserom_lg.gba` |

- FRLG のビルドに、エメラルドのROM（`baserom.gba`）は要りません。
- `baserom_fr.gba` と `baserom_lg.gba` は、どちらか1本あればファイアレッド・リーフグリーンのどちらもビルドできます。
  両方ある場合は、`make firered` は `baserom_fr.gba` を、`make leafgreen` は `baserom_lg.gba` を使います。
- 改造していない吸い出しそのまま（16MiB）のROMだけを受け付けます。使えるのは次の4本です（SHA1で確認します）。

| ROM | ゲームコード | SHA1 |
| --- | --- | --- |
| ポケットモンスター ファイアレッド（初版） | BPRJ | `04139887b6cd8f53269aca098295b006ddba6cfe` |
| ポケットモンスター ファイアレッド（Rev 1） | BPRJ | `7c7107b87c3ccf6e3dbceb9cf80ceeffb25a1857` |
| ポケットモンスター リーフグリーン（初版） | BPGJ | `5946f1b59e8d71cc61249661464d864185c92a5f` |
| ポケットモンスター リーフグリーン（Rev 1） | BPGJ | `de9d5a844f9bfb63a4448cccd4a2d186ecf455c3` |

海外版（BPRE／BPGE）やパッチを当てたROM、32MiB に拡張したROMは使えません。
ROMファイルは配布・コミットしないでください（`*.gba` は `.gitignore` 済みです）。

## ビルドする

リポジトリのルートで実行します。

```bash
make firered -j8
```

成功すると `pokefirered.gba` ができます。リーフグリーンは次のとおりです（`pokeleafgreen.gba`）。

```bash
make leafgreen -j8
```

いつもの `make` はエメラルド（`pokeemerald.gba`）のままです。
`make firered` の代わりに `make BUILD=firered` と書くこともできます（`BUILD` の使い方は本家の
[How to use FRLG](tutorials/how_to_frlg.md) を参照してください）。

エメラルドとFRLGを切り替えるときに `make clean` は要りません。
マップのデータとROMから取り出したアセットは、切り替えたときに自動で作り直されます
（オブジェクトは `build/emerald`・`build/firered`・`build/leafgreen` に分かれています）。

## ROMから取り出すもの

エメラルドのビルドは、日本語の画像や効果音など一部のアセットを `baserom.gba` から取り出して
`build/extracted_*` に置きます（`tools/extract_bpej_*.py`）。
FRLG のビルドでは、同じファイルを `baserom_fr.gba`／`baserom_lg.gba` から作ります（`tools/extract_frlg_assets.py`）。

1. エメラルドで使う中身と同じデータを FRLG のROMの中から探し、SHA1 が一致したものを使います
   （効果音・楽器の音・アイテムやきのみの絵など、両方のROMにあるもの）。
   圧縮のしかたが違っても、展開した中身が同じなら使います。
2. FRLG のROMにないもののうち、図鑑と戦闘背景の画像はリポジトリにある元の画像を使います。
3. それ以外の、FRLG にないもの（コンテスト、ポケナビ、エメラルドのオープニングなど、ホウエンだけの画面や曲）は、
   同じ大きさ・形式の空の画像や無音のデータにします。FRLG ではこれらの画面に来ないので、ビルドを通すためのものです。

どれをどこから作ったかは `build/generated/frlg_*_extracted.ok` の `origins` に出ます
（`rom` がROMから、`fallback` がリポジトリの画像、`blank`／`silent` が空のデータ）。
`graphics/custom/bpej/` に置いた差し替えは、エメラルドと同じく FRLG でも優先されます。

ROMの中を探すための手がかり（各アセットの一部のバイトとその位置）は `tools/data/frlg_asset_probes.json` にあります。
エメラルド用の `tools/data/bpej_*_manifest.json` や `symbols/bpej/` を変えたときは、
エメラルドで一度ビルドしてから、日本版エメラルドROMを使って作り直してください。

```bash
python3 tools/generate_frlg_asset_probes.py --baserom baserom.gba
```

## うまくいかないとき

### 「日本版ファイアレッドROMを baserom_fr.gba、または日本版リーフグリーンROMを baserom_lg.gba としてプロジェクトルートに配置してください」

`detail:` の行に理由が出ます。

- `neither baserom_fr.gba nor baserom_lg.gba was found.`：ファイル名と置き場所（リポジトリのルート）を確認してください。
- `size is ...`：16MiB の吸い出しそのままのROMを使ってください（拡張・パッチ済みのROMは不可）。
- `game code is ...`：日本版のファイアレッド（BPRJ）かリーフグリーン（BPGJ）ではありません。
- `sha1 ... is not an unmodified ...`：上の表のどれとも一致しません。改造されていないかを確認してください。

### エメラルドに戻したら絵や音がおかしい

`build/extracted_*` は版を切り替えたときに作り直されますが、途中で止めた場合などに古いまま残ることがあります。
`make clean-generated` のあとにもう一度ビルドしてください。
