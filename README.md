# Pokeemerald-expansion日本語実装計画
やぁ。日本語(ひらがな、カタカナ)が使えるようにがんばってるやつ。

一旦はGBAで使われてれる範囲のひらがなとカタカナは使えるよ。
ヴがなかったから使えるようにしてるよ。日本語のキーボード入力の対応まだだよ。
テキスト表示させる時にUTFのひらがなカタカナ入れたら内部の日本語フォントに照らし合わせた上で表示されるようにしてるよ。
※ゲーム内部をUTF-8化したわけではなく、charmapでUTF-8文字を既存の1バイト文字コードへ変換して表示しているよ。

charmap変換なんだけど、「これいらんやろ」みたいなの削ったからそこに漢字とか対応させれるかもね。

後中身のテキストデータの翻訳は完全にはできてないよ。さいしょからはじめるで博士に喋らせることで日本語フォントのテストしてるよ。
テキストの表示(どんな感じでテキスト送りされるのか)とかそこらへんも整備できてないよ。

まだ完全に日本語基盤蘇生が終わってるわけじゃないから、「ほーんおもろ」って眺める程度にしてね。
これで作ったデータで日本版エメラルドに当てるパッチ作ったら無事に当てれるっぽいです。

## 日本語対応状況

このプロジェクトでは pokeemerald-expansion に対して
日本語フォント表示の実験を行っています。

### 現在確認済み

- ひらがな表示
- カタカナ表示
- 日本語テキスト表示
- 日本語タイトルロゴ表示（実験段階）
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

「どうやってこのプロジェクト扱うの?」に関してはここを読んでね。
https://github.com/koziyagoukei/PokeEm-expansion-CanuseJP/blob/master/INSTALL.md

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
Based off RHH's pokeemerald-expansion 1.16.1 https://github.com/rh-hideout/pokeemerald-expansion/
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
