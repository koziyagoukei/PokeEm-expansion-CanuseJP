# Pokeemerald-expansion 日本語実装計画

英語版の `pokeemerald-expansion` の機能を維持しながら、ひらがな・カタカナで日本語のテキストを記述・表示できるようにする非公式の開発用フォークです。日本語でポケモンのデコンプハックを制作するための土台づくりを進めています。

ゲームROM本体は含まれていません。ビルドには、ご自身で正規に所有している **日本版『ポケットモンスター エメラルド』（BPEJ）の未改変ROM** が必要です。利用前に、下記の[利用上の注意](#利用上の注意)を必ずご確認ください。

[インストール手順](INSTALL.md) / [機能説明](FEATURES.md) / [開発用ドキュメント](docs/) / [Discord](https://discord.gg/eYtXJDuYjX)

## このプロジェクトについて

`pokeemerald-expansion` は、`pret/pokeemerald` の逆解析・再構成を基にした開発用プロジェクトです。本フォークもそのソースコードを基盤としており、公式から公開・提供されたソースコードを使用しているわけではありません。

目標は、日本版エメラルドのシステムへ丸ごと置き換えることではなく、Expansionのシステムを使いながら日本語の作品を制作できるようにすることです。フォント表示、テキスト翻訳、画面ごとの表示調整、ROMからの素材抽出を段階的に進めています。

翻訳や画面調整は継続中です。全テキストの翻訳完了や、全画面での正常表示を保証する状態ではありません。未翻訳、文字のはみ出し、表示位置のずれなどが残っている場合があります。

このフォークの開発では、調査・実装・テスト・文書作成などの一部に生成AIを補助的に使用しています。AIの出力にも誤りがあり得るため、変更内容や動作の確認を行っています。

## 日本語対応

### 対応を進めている範囲

- ひらがな・カタカナの表示と、日本語・英語の表示モード切り替え。
- ポケモンの種族名、技名・技説明、アイテム名などの日本語化。
- メニュー、戦闘メッセージ、イベント会話、バトルフロンティア関連テキストの日本語化。
- 日本語版ROM由来の画像内文字を含む、対応するUI素材の抽出・差し替え。

各項目は対応範囲を示すもので、すべての翻訳・動作確認が完了していることを示すものではありません。

### 文字コードとフォント

ソース内の日本語はUTF-8で記述しますが、**ゲーム内部の文字列をUTF-8化しているわけではありません**。[charmap.txt](charmap.txt) に従い、ひらがな・カタカナをゲーム用の1バイト文字コードへ変換して表示します。

文字コードには通常文字と制御コード、`EMOJI_XXXX` などの定義が混在しています。EMOJI定義は通常のASCII記号とは別のものです。また、同じ文字コードでも表示モードによって参照するフォントが異なるため、単に日本語を追加しただけですべての英語表示と互換になるわけではありません。

フォントは `graphics/fonts/` 以下のPNGから生成します。日本語フォントには `japanese_normal`、`japanese_small`、`japanese_short`、`japanese_bold` を使用します。使える文字・記号はcharmapとフォント側の対応範囲に限られ、任意の漢字や全角記号がそのまま使えるわけではありません。

漢字は表示文字列へ記述すると、ビルド時に使用字の検出・ID登録・影付きフォント生成を行います。既存25字のIDを維持し、最大2,214字に対応します。プレイヤー名・ニックネーム、セーブや通信に入る文字列への漢字は非対応で、既知の経路は漢字禁止・バイト長のビルド時検査で保護します。導入方法と検査の限界は [漢字の自動検出とフォント生成](docs/kanji.md) を参照してください。

図鑑説明文は、既存のかな版と、新しい作品を優先した漢字版をコンパイル時に選択できます。初期設定は漢字版です。初回ビルド前に `make fetch-pokedex-text` で参照データを取得してください。取得・設定方法は [図鑑説明文のかな／漢字切り替え](docs/pokedex_kanji.md) を参照してください。

### 表示モードの指定

| 指定 | 用途 |
| --- | --- |
| 指定なし | 日本語表示モードで開始する（デフォルト設定） |
| `{JPN}` | 日本語表示モードを指定する |
| `{ENG}` | 英語表示モードを指定する |
| `{AUTO}` | 続く文字を基に表示モードを再判定する（数字と記号は読み飛ばし、次のかな・英字で決める） |

日本語の文字列は、先頭の `{JPN}` を省略できます。既存の `{JPN}` 付き文字列もそのまま使用できます。英語フォントで表示したい文字列には `{ENG}` を付けてください。

```c
const u8 gText_JapaneseExample[] = _("こんにちは");
const u8 gText_EnglishExample[] = _("{ENG}HELLO");
```

`{ENG}` / `{JPN}` の切り替えは、同じ文字列の後続部分にも有効です。ただし `{STR_VAR_1}` などの変数で挿入された文字列の中の切り替えは、挿入が終わると元の文字列のモードに戻ります（`{ENG}` 付きの数字や名前の後ろの日本語が英語フォントにならないように）。英語と日本語が混在する文章では、意図したモードで表示されるかを確認してください。英語フォント固有の記号やアクセント文字にも `{ENG}` が必要です。既存のラベル名・内部定数・制御コード・プレースホルダーは、翻訳時にも保持します。

デフォルトは [include/config/text.h](include/config/text.h) の `TEXT_JAPANESE_BY_DEFAULT` で切り替えられます。

## 導入とビルド

### ビルド環境

Windows + WSL / Ubuntuでの環境構築は [INSTALL.md](INSTALL.md) を参照してください。Git、make、Python 3、ARM向けツールチェインなどが必要です。以下は、必要なビルド環境を準備した後の手順です。

```sh
git clone https://github.com/koziyagoukei/PokeEm-expansion-CanuseJP.git
cd PokeEm-expansion-CanuseJP
```

Gitの履歴を使って更新・マージを行うため、開発にはGitでのcloneを使用してください。

### 日本版ROMを配置する

プロジェクトルートに、日本版エメラルドの未改変ROMを **`baserom.gba`** として配置してください。ファイル名が `baserom_jp.gba` のままでは、現在のビルドでは認識されません。

```text
PokeEm-expansion-CanuseJP/
  baserom.gba
  Makefile
  charmap.txt
  src/
  tools/
```

ビルド時に `tools/check_baserom_jp.py` が以下を検査します。

| 項目 | 対応するROMの値 |
| --- | --- |
| ゲームコード | `BPEJ` |
| ファイルサイズ | 16,777,216バイト（16 MiB） |
| SHA1 | `d7cf8f156ba9c455d164e1ea780a6bf1945465c2` |
| CRC32 | `4881f3f8` |

別言語版や改変済みのROMは使用できません。ROMが見つからない場合や検証に失敗した場合は停止します。ROMの入手先の案内や、ROM本体の提供は行いません。

### ビルドする

リポジトリのルートで実行します。

```sh
make -j4 --output-sync=target
```

ROMの検証後、必要な音源・画像を自動抽出し、ビルドに使用します。標準設定では `pokeemerald.gba` が生成されます。抽出元の `baserom.gba` は書き換えません。

生成物を消してからビルドし直す場合は、以下を実行します。抽出物も削除されるため、次回のビルドにも `baserom.gba` が必要です。

```sh
make clean
make -j4 --output-sync=target
```

## ROMからのアセット抽出

公式ROM由来の音源・画像をリポジトリに直接含めない構成へ、段階的に移行しています。現在は、対応を確認したBGM・SE・DirectSound、図鑑UI、戦闘背景、コンテスト・コンディション、タイトル・オープニング、ポケナビ関連などに抽出経路があります。

抽出対象はmanifestやシンボルDBで管理し、対象のROM範囲・サイズ・ハッシュなどを検証します。対応が確定していない素材や独自編集素材を、一律にROM由来のものへ置き換える方針ではありません。

- 抽出物は `build/extracted_*`、検証結果などは `build/generated/` に生成し、Git管理しません。
- `baserom.gba`、完成ROM、抽出済みバイナリをコミット・配布しないでください。
- 移行済みの素材には、元のPNGなどを削除し、ROMからの抽出が必須となっているものがあります。
- 独自素材への差し替えに対応する分野では、各文書で指定した `graphics/custom/bpej/` 以下の配置先を使用します。対応する独自素材は `make clean` で削除しません。

**すべての素材の抽出移行が完了した状態ではありません。** また、この仕組みによって権利面の課題がすべて解消されることを保証するものではありません。

## 関連ドキュメント

| 内容 | 文書 |
| --- | --- |
| 環境構築・ビルド手順 | [INSTALL.md](INSTALL.md) |
| Expansionの機能 | [FEATURES.md](FEATURES.md) |
| Poké Corpusを使った誤訳・未翻訳の点検 | [poke_corpus_localization.md](docs/poke_corpus_localization.md) |
| 図鑑説明文のかな／漢字切り替え | [pokedex_kanji.md](docs/pokedex_kanji.md) |
| 漢字表示の試作・表示確認と制約 | [kanji_font_poc.md](docs/kanji_font_poc.md) |
| 漢字の自動検出・生成と保存/通信データ検査 | [kanji.md](docs/kanji.md) |
| フォントのROM容量と表示方式の調査 | [font_engine_audit.md](docs/font_engine_audit.md) |
| 本流の開発者・貢献者 | [CREDITS.md](CREDITS.md) |

作業の進捗や補足情報は、[プロジェクトのDiscord](https://discord.gg/eYtXJDuYjX) と [Scrapbox](https://scrapbox.io/EmEX-JP/) でも案内しています。

## 翻訳への協力と不具合報告

翻訳に協力いただく際は、対象ファイル・ラベル名・変更前後の文章が分かる形でご提案ください。表示文字列以外のIDやイベント命令を変更せず、`charmap.txt` で使用できる文字と既存の制御コードを使ってください。

テキスト変更後の確認には、次のコマンドを使用できます。

```sh
python3 tools/check_jp_text.py
```

この検査はゲーム内での表示・操作確認の代わりにはなりません。文字のはみ出しや改ページ、変数展開後の表示も確認してください。

不具合を報告する際は、使用したコミット、ビルド環境、再現手順、発生する画面、可能であればスクリーンショットや動画を添えてください。使用したROMが修正前・修正後のどちらのソースからビルドしたものかも確認をお願いします。ROM本体や抽出物は添付しないでください。

## 利用上の注意

### 必要なROM

ゲームROM本体は含まれていません。ご自身で正規に所有している対応ROM（BPEJ）をご用意ください。違法にダウンロードしたROMや、不正に入手したROMの使用は禁止します。

本プロジェクトではROM本体や完成ROMの配布は行いません。公開物に公式ROMそのものや抽出物を含めないよう配慮していますが、デコンパイルベースのプロジェクトとして権利面の課題が完全に存在しないとは考えていません。

### 配布・公開

- ROM本体（パッチ適用済みを含む）や完成ROMの配布は禁止します。作成したROMの公開は、差分パッチ形式に限ります。
- 公式のコピーライトは削除しないでください。
- 本プロジェクトを改変したものを配布・公開する場合は、改変版であることを明記してください。
- 本プロジェクトそのものの無断転載・再配布はご遠慮ください。紹介する場合は、元ページをご案内ください。

### 動画・配信・金銭のやり取り

作成したROMを利用した、金銭のやり取りが関わる行為は禁止します。収益化（広告収益・メンバーシップ・投げ銭等を含む）を伴う実況動画・配信での利用や、収益化しているチャンネルでの実況配信はしないでください。ハックROMのデータを書き込んだGBAカートリッジの販売も禁止します。

### その他の禁止事項

- 本プロジェクトを利用した違法行為。
- 作者や権利者になりすます行為。
- 作者や他者への誹謗中傷、迷惑行為を目的とした利用。

### サポート・免責事項

不具合報告を行う際は、再現可能であることをご確認ください。個別環境で発生するすべての問題への対応を保証するものではありません。

本プロジェクトは非公式のファンプロジェクトであり、権利者とは一切関係ありません。本プロジェクトの利用によって生じた損害について、作者は責任を負いません。権利者からの要請やその他の事情により、公開・配布を予告なく終了する場合があります。

この規約は、より良い運営のため予告なく変更される場合があります。

## 翻訳にご協力いただいた方々

9:02PM 様

ご協力ありがとうございます。

# About `pokeemerald-expansion`

![Gif that shows debugging functionality that is unique to pokeemerald-expansion such as rerolling Trainer ID, Cheat Start, PC from Debug Menu, Debug PC Fill, Pokémon Sprite Visualizer, Debug Warp to Map, and Battle Debug Menu](https://github.com/user-attachments/assets/cf9dfbee-4c6b-4bca-8e0a-07f116ef891c) ![Gif that shows overworld functionality that is unique to pokeemerald-expansion such as indoor running, BW2 style map popups, overworld followers, DNA Splicers, Gen 1 style fishing, OW Item descriptions, Quick Run from Battle, Use Last Ball, Wild Double Battles, and Catch from EXP](https://github.com/user-attachments/assets/383af243-0904-4d41-bced-721492fbc48e) ![Gif that shows off a number of modern Pokémon battle mechanics happening in the pokeemerald-expansion engine: 2 vs 1 battles, modern Pokémon, items, moves, abilities, fully customizable opponents and partners, Trainer Slides, and generational gimmicks](https://github.com/user-attachments/assets/50c576bc-415e-4d66-a38f-ad712f3316be)

<!-- If you want to re-record or change these gifs, here are some notes that I used: https://files.catbox.moe/05001g.md -->

**`pokeemerald-expansion`** is a GBA ROM hack base that equips developers with a comprehensive toolkit for creating Pokémon ROM hacks. **`pokeemerald-expansion`** is built on top of [pret's `pokeemerald`](https://github.com/pret/pokeemerald) decompilation project. **It is not a playable Pokémon game on its own.**

# [Features](FEATURES.md)

**`pokeemerald-expansion`** offers hundreds of features from various [core series Pokémon games](https://bulbapedia.bulbagarden.net/wiki/Core_series), along with popular quality-of-life enhancements designed to streamline development and improve the player experience. A full list of those features can be found in [`FEATURES.md`](FEATURES.md).

# [Credits](CREDITS.md)

 [![](https://img.shields.io/github/all-contributors/rh-hideout/pokeemerald-expansion/upcoming)](CREDITS.md)

If you use **`pokeemerald-expansion`**, please credit **RHH (Rom Hacking Hideout)**. Optionally, include the version number for clarity.

```
Based off RHH's pokeemerald-expansion 1.17.1 https://github.com/rh-hideout/pokeemerald-expansion/
```

Please consider [crediting all contributors](CREDITS.md) involved in the project!

# Choosing `pokeemerald` or **`pokeemerald-expansion`**

- **`pokeemerald-expansion`** supports multiplayer functionality with other games built on **`pokeemerald-expansion`**. It is not compatible with official Pokémon games.
- If compatibility with official games is important, use [`pokeemerald`](https://github.com/pret/pokeemerald). Otherwise, we recommend using **`pokeemerald-expansion`**.
- **`pokeemerald-expansion`** incorporates regular updates from `pokeemerald`, including bug fixes and documentation improvements.

# [Getting Started](INSTALL.md)

❗❗ **Important**: Do not use GitHub's "Download Zip" option as it will not include commit history. This is necessary if you want to update or merge other feature branches.

If you're new to git and GitHub, [Team Aqua's Asset Repo](https://github.com/Pawkkie/Team-Aquas-Asset-Repo/) has a [guide to forking and cloning the repository](https://github.com/Pawkkie/Team-Aquas-Asset-Repo/wiki/The-Basics-of-GitHub). Then you can follow one of the following guides:

## 📥 [Installing **`pokeemerald-expansion`**](INSTALL.md)
## 🏗️ [Building **`pokeemerald-expansion`**](INSTALL.md#Building-pokeemerald-expansion)
## 🚚 [Migrating from **`pokeemerald`**](INSTALL.md#Migrating-from-pokeemerald)
## 🚀 [Updating **`pokeemerald-expansion`**](INSTALL.md#Updating-pokeemerald-expansion)

# [Documentation](https://rh-hideout.github.io/pokeemerald-expansion/)

For detailed documentation, visit the [pokeemerald-expansion documentation page](https://rh-hideout.github.io/pokeemerald-expansion/).

# [Contributions](CONTRIBUTING.md)
If you are looking to [report a bug](CONTRIBUTING.md#Bug-Report), [open a pull request](CONTRIBUTING.md#Pull-Requests), or [request a feature](CONTRIBUTING.md#Feature-Request), our [`CONTRIBUTING.md`](CONTRIBUTING.md) has guides for each.

# [Community](https://discord.gg/6CzjAG6GZk)

[![](https://dcbadge.limes.pink/api/server/6CzjAG6GZk)](https://discord.gg/6CzjAG6GZk)

Our community uses the [ROM Hacking Hideout (RHH) Discord server](https://discord.gg/6CzjAG6GZk) to communicate and organize. Most of our discussions take place there, and we welcome anybody to join us!

# ExpansionStudio MCP

This fork includes a local stdio MCP wrapper for ExpansionStudio CLI.
See [docs/ExpansionStudio_MCP.md](docs/ExpansionStudio_MCP.md) for startup commands, ChatGPT Desktop/MCP client configuration examples, safety rules, and smoke tests.

# ExpansionStudio PNG palette tools

ExpansionStudio can inspect PNG assets, convert PNG files to indexed PNG, and generate gbagfx-readable JASC-PAL `.pal` files from PNG images in the Asset tab.
The CLI commands are also available:

```powershell
python -m expansionstudio analyze-png --root "C:\path\to\pokeemerald-expansion" --png graphics/example.png --pretty
python -m expansionstudio png-to-indexed --root "C:\path\to\pokeemerald-expansion" --png graphics/example.png --output graphics/example_indexed.png --colors 16 --pretty
python -m expansionstudio png-to-jasc-pal --root "C:\path\to\pokeemerald-expansion" --png graphics/example.png --output graphics/example.pal --colors 16 --pretty
```

Supported palette sizes are `16`, `224`, and `256`. Existing JASC-PAL files can be passed with `--base-pal` to prefer their palette order. `png-to-indexed` reduces colors by default when needed; use `--no-quantize` to fail instead. Missing JASC-PAL entries are padded with `0 0 0`.
