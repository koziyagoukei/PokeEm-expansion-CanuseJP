# 本プロジェクトをご利用いただく前に、以下の内容をご確認ください。

1. 必要なROMについて

ゲームROM本体は含まれていません。
ご自身で正規に所有している対応ROM(BPEJ)をご用意ください。
違法にダウンロードしたROMや、不正に入手したROMへの使用は禁止します。

2. 配布について

ROM本体（パッチ適用済みを含む）の配布は禁止します。
本プロジェクトを改変したものを配布・公開する場合は、改変版であることを明記してください。
本プロジェクトのみを無断で転載・再配布することはご遠慮ください。配布/公開する場合は、元ページをご案内ください。

3. 本プロジェクトを利用した生成物の動画配信・配信活動について

収益化（広告収益・メンバーシップ・投げ銭等を含む）を伴う実況動画・配信での利用は禁止します。

4. 禁止事項

以下の行為は禁止します。

違法ダウンロードしたROMの使用
ROM本体または完成ROMの配布
本プロジェクトを利用した違法行為
作者や権利者になりすます行為
作者や他者への誹謗中傷、迷惑行為を目的とした利用

5. サポートについて

不具合報告を行う際は、再現可能であることをご確認ください。
個別環境で発生するすべての問題への対応を保証するものではありません。

6. 免責事項

本プロジェクトは非公式のファンプロジェクトであり、権利者とは一切関係ありません。
本プロジェクトの利用によって生じた損害について、作者は責任を負いません。
権利者からの要請やその他の事情により、本プロジェクトの公開・配布を予告なく終了する場合があります。

この規約は、より良い運営のため予告なく変更される場合があります。


# Pokeemerald-expansion日本語実装計画
やぁ。日本語(ひらがな、カタカナ)が使ってエメラルドのポケハックが気軽にできるようにがんばってるやつ。

一旦はGBAで使われてれる範囲のひらがなとカタカナは使えるよ。
ヴがなかったから使えるようにしてるよ。日本語のキーボード入力の対応まだだよ。
テキスト表示させる時にUTFのひらがなカタカナ入れたら内部の日本語フォントに照らし合わせた上で表示されるようにしてるよ。
※ゲーム内部をUTF-8化したわけではなく、charmapでUTF-8文字を既存の1バイト文字コードへ変換して表示しているよ。

[Discord](https://discord.gg/eYtXJDuYjX)で作業とか色々進捗とか載せてるよ。

後中身のテキストデータの翻訳は完全にはできてないよ。さいしょからはじめるで博士に喋らせることで日本語フォントのテストしてるよ。
テキストの表示(どんな感じでテキスト送りされるのか)とかそこらへんは完全には整備できてないよ。

まだ完全に日本語基盤蘇生が終わってるわけじゃないから、「ほーんおもろ」って眺める程度にしてね。
これで作ったデータで日本版エメラルドに当てるパッチ作ったら無事に当てれるっぽいです。

## 日本語対応状況

このプロジェクトでは pokeemerald-expansion に対して
日本語フォント表示の実験を行っています。

### 現在確認済み

- ひらがな表示
- カタカナ表示
- 日本語テキスト表示
- 日本語英語テキスト同時表示
- 技名翻訳
- アイテム名翻訳
- 一部テキストデータ翻訳(手作業のため完全にはできていません、シナリオデータの翻訳は優先度を下げています。)

### 実装方式

既存英語フォントを破壊せず、
追加文字領域を利用して日本語文字を割り当てています。

英語と日本語はテキスト制御コードにより切り替え可能です。

例:

{JPN}こんにちは
{ENG}HELLO
{AUTO}ボンジュール

基本は行の先頭が日本語か、英数字かで切り替えていますが、日本語と英語を矯正させれるように描写モードを変えれます。

AUTOは次に続く文字が日本語か英数字かが不明な場合に使うコードです。

作成したROMに関しては、差分パッチ以外の形式での配布を禁止します。
また、公式のコピーライトは削除しないようにした上で、作成したROMを利用した金銭のやり取りが関わる行為を禁止します。
収益化しているチャンネルでの実況配信や、ハックロムのデータを書き込んだGBA ROMの販売はやめてください。

「どうやってこのプロジェクト扱うの?」に関してはここを読んでね。
[インストール手順](https://github.com/koziyagoukei/PokeEm-expansion-CanuseJP/blob/master/INSTALL.md)
[機能説明](https://github.com/koziyagoukei/PokeEm-expansion-CanuseJP/blob/master/FEATURES.md)

## 翻訳にご協力いただいた方々
9:02PM 様

ありがとうございます！

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
Based off RHH's pokeemerald-expansion 1.16.2 https://github.com/rh-hideout/pokeemerald-expansion/
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
