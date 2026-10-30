# Windows + WSL 環境でのビルド手順

この文書は、Windows 10 / Windows 11 上で WSL を使い、`pokeemerald` / `pokeemerald-expansion` 系プロジェクトをビルドするための日本語メモです。

本家 `pret/pokeemerald` の `INSTALL.md` を参考にしていますが、本プロジェクト向けに内容を絞っています。

正確な最新情報は、必ず本家の `INSTALL.md` や、使用しているリポジトリの説明も確認してください。

## 0. この文書について

この文書では、以下の環境を想定しています。

```text
Windows 10 または Windows 11
WSL / Ubuntu
Git
make
arm-none-eabi 系ツールチェイン
libpng
agbcc
```

この文書では、以下の環境については扱いません。

```text
msys2
Cygwin
macOS
Linux単体環境
```

また、導入手順は環境やリポジトリの更新によって変わる場合があります。

この文書は、Windows + WSL 環境での一例です。

## 1. WSLをインストールする

まず、WindowsにWSLを入れます。

PowerShellを「管理者として実行」し、以下を実行します。

```powershell
wsl --install -d Ubuntu
```

完了したら、Windowsを再起動します。

再起動後、PowerShellで以下を実行し、UbuntuのWSLバージョンを確認します。

```powershell
wsl -l -v
```

WSL1で使う場合は、以下を実行します。

```powershell
wsl --set-version Ubuntu 1
```

環境によっては変換に時間がかかります。

すでにWSLを導入済みの場合は、この手順を飛ばしても構いません。

## 2. Ubuntuを初期設定する

スタートメニューから `Ubuntu` を起動します。

初回起動時に、Linux側のユーザー名とパスワードを作成します。

パスワード入力中は画面に文字が表示されませんが、入力はされています。

何も表示されないからといって「なにもしてないのにこわれた」と言わないでください。そういう仕様です。

## 3. Ubuntuを更新する

Ubuntuを開いた状態で、以下を実行します。

```bash
sudo apt update
sudo apt upgrade
```

途中で確認が出た場合は、内容を確認して `Y` を入力します。

## 4. 必要なパッケージを入れる

ビルドに必要なパッケージをインストールします。

```bash
sudo apt install build-essential git libpng-dev binutils-arm-none-eabi gcc-arm-none-eabi
```

もし `apt` でうまくいかない場合は、以下のように `apt-get` を使ってください。

```bash
sudo apt-get install build-essential git libpng-dev binutils-arm-none-eabi gcc-arm-none-eabi
```

`gcc-arm-none-eabi` は環境によってインストール容量が大きくなる場合があります。

ただし、`arm-none-eabi-gcc` が見つからない場合は必要になります。

## 5. ARMツールチェインを確認する

このプロジェクトのビルドには、GBA向けの ARM ツールチェインが必要です。

このガイドでは、Windows + WSL / Ubuntu 環境で `arm-none-eabi` 系パッケージを使う例を扱います。

一部の手順やプロジェクトでは、これに相当する環境を `devkitARM` と呼ぶことがあります。

以下を実行して、コマンドが使えるか確認します。

```bash
arm-none-eabi-gcc --version
arm-none-eabi-as --version
arm-none-eabi-ld --version
```

`command not found` が出る場合は、以下をインストールしてください。

```bash
sudo apt install binutils-arm-none-eabi gcc-arm-none-eabi
```

## 6. 作業フォルダを作る

この文書では、Windows側のデスクトップに `decomps` フォルダを作って作業する例で説明します。

Windowsのエクスプローラーで、以下のようなフォルダを作成してください。

```text
C:\Users\<Windowsユーザー名>\Desktop\decomps
```

WSLから見ると、WindowsのCドライブは `/mnt/c/` にあります。

Ubuntu上で、以下のように移動します。

```bash
cd /mnt/c/Users/<Windowsユーザー名>/Desktop/decomps
```

例:

```bash
cd /mnt/c/Users/Emerald/Desktop/decomps
```

パスに空白がある場合は、ダブルクォートで囲みます。

```bash
cd "/mnt/c/Users/<Windowsユーザー名>/Desktop/decomp folder"
```

ただし、トラブルを減らすため、作業フォルダ名には空白を入れない方が安全です。

おすすめ:

```text
decomps
pokeemerald-expansion
```

避けたい例:

```text
decomp folder
pokemon hack project
```

## 7. リポジトリを取得する

通常の `pokeemerald` を使う場合は、以下のように取得します。

```bash
git clone https://github.com/pret/pokeemerald.git
cd pokeemerald
```

`pokeemerald-expansion` 系や、本プロジェクトのフォークを使う場合は、対象リポジトリのURLに置き換えてください。

例:

```bash
git clone https://github.com/<ユーザー名>/<リポジトリ名>.git
cd <リポジトリ名>
```

本プロジェクトを取得する場合の例:

```bash
git clone https://github.com/koziyagoukei/PokeEm-expansion-CanuseJP.git
cd PokeEm-expansion-CanuseJP
```

## 8. agbccを用意する

`pokeemerald` 系プロジェクトでは、GBA向けの古いCコンパイラである `agbcc` が必要になる場合があります。

`agbcc` の導入方法は、使用しているプロジェクトや環境によって差が出ることがあります。

この手順は一例です。

うまくいかない場合は、本家 `pret/pokeemerald` の `INSTALL.md` や、使用しているリポジトリの説明を確認してください。

### 8-1. 作業フォルダの構成

この文書では、以下のような構成を想定します。

```text
decomps/
  agbcc/
  pokeemerald/
```

または、`pokeemerald-expansion` 系の場合は以下のようになります。

```text
decomps/
  agbcc/
  pokeemerald-expansion/
```

本プロジェクトの場合は、以下のようになります。

```text
decomps/
  agbcc/
  PokeEm-expansion-CanuseJP/
```

つまり、`agbcc` と `pokeemerald` / `pokeemerald-expansion` / 本プロジェクトを同じ階層に置きます。

作業フォルダに移動します。

```bash
cd /mnt/c/Users/<Windowsユーザー名>/Desktop/decomps
```

### 8-2. agbccを取得する

作業フォルダで、以下を実行します。

```bash
git clone https://github.com/pret/agbcc.git
```

取得後、以下のような構成になっていればOKです。

```text
decomps/
  agbcc/
  pokeemerald/
```

または:

```text
decomps/
  agbcc/
  PokeEm-expansion-CanuseJP/
```

### 8-3. agbccをビルドする

`agbcc` フォルダに移動します。

```bash
cd agbcc
```

ビルドします。

```bash
./build.sh
```

環境によっては、実行権限がないと言われる場合があります。

その場合は、以下を実行してからもう一度試してください。

```bash
chmod +x build.sh
./build.sh
```

### 8-4. プロジェクト側へ戻る

`agbcc` の準備が終わったら、プロジェクト側へ戻ります。

`pokeemerald` の場合:

```bash
cd ../pokeemerald
```

`pokeemerald-expansion` の場合:

```bash
cd ../pokeemerald-expansion
```

本プロジェクトの場合:

```bash
cd ../PokeEm-expansion-CanuseJP
```

### 8-5. agbcc関連で失敗する場合

`agbcc` 周りで失敗する場合、以下を確認してください。

```text
agbcc と pokeemerald 系プロジェクトが同じ階層にあるか
agbcc/build.sh を実行したか
build.sh に実行権限があるか
WSL上で作業しているか
WindowsのPowerShellやcmdではなく、Ubuntu側で実行しているか
```

想定する配置は以下です。

```text
decomps/
  agbcc/
  pokeemerald/
```

または:

```text
decomps/
  agbcc/
  PokeEm-expansion-CanuseJP/
```

ありがちな失敗例です。

```text
pokeemerald/
  agbcc/
```

このように、`pokeemerald` フォルダの中に `agbcc` を入れてしまうと、プロジェクト側から見つからない場合があります。

また、`agbcc` の導入手順はプロジェクト側の更新で変わる可能性があります。

この手順で失敗する場合は、本家の `INSTALL.md` を確認してください。

## 9. baserom.gbaを配置する

ビルドには、元になるROMファイルが必要です。

通常の `pokeemerald` 系プロジェクトでは、リポジトリのルートに以下の名前で配置します。

```text
baserom.gba
```

例:

```text
pokeemerald/
  baserom.gba
  Makefile
  src/
  data/
```

本プロジェクトの場合も、基本的にはリポジトリのルートに `baserom.gba` を配置します。

例:

```text
PokeEm-expansion-CanuseJP/
  baserom.gba
  Makefile
  src/
  data/
```

ROMファイルそのものは配布しないでください。

また、GitHubに誤ってコミットしないように注意してください。

## 10. ビルドする

本プロジェクトは図鑑の漢字説明を初期設定にしています。初回ビルド前に、リポジトリのルートで参照データを取得してください。この取得時だけインターネット接続が必要です。

```bash
make fetch-pokedex-text
```

これは `python3 tools/pokedex_kanji.py fetch` と同じ処理です。取得したデータは `build/reference/poke-corpus/` に保存され、固定リビジョンとSHA256で検証されます。Gitには含まれないため、別PCや新しいcloneでは取得し直してください。通常の `make` はネットワークへ接続しません。

リポジトリのルートで、以下を実行します。

```bash
make -j5
```

CPUに余裕がある場合は、数字を増やしても構いません。

例:

```bash
make -j8
```

ビルドに成功すると、`.gba` ファイルが生成されます。

## 11. よくあるエラー

### `missing .../build/reference/poke-corpus/...; run python3 tools/pokedex_kanji.py fetch`

漢字図鑑用の参照データが未取得です。マップやセーブデータの破損ではありません。リポジトリのルートで取得してから再ビルドしてください。

```bash
make fetch-pokedex-text
make -j8
```

ネットワークが使えない場合は、取得済みの `build/reference/poke-corpus/` を同じリビジョンの作業環境から移してください。かな版で一時的にビルドする場合は `make -j8 POKEDEX_TEXT_STYLE=0` を使用できます。設定を省略した次のビルドは漢字版へ戻ります。

### `arm-none-eabi-gcc` が見つからない

必要なパッケージが入っていない可能性があります。

```bash
sudo apt install binutils-arm-none-eabi gcc-arm-none-eabi
```

インストール後、以下を確認してください。

```bash
arm-none-eabi-gcc --version
```

### `png.h` が見つからない

`libpng-dev` が入っていない可能性があります。

```bash
sudo apt install libpng-dev
```

### `baserom.gba` が見つからない

ROMファイルの名前と場所を確認してください。

プロジェクトのルートに、以下の名前で置かれている必要があります。

```text
baserom.gba
```

例:

```text
PokeEm-expansion-CanuseJP/
  baserom.gba
  Makefile
```

### `agbcc` が見つからない、またはビルドに失敗する

`agbcc` の配置を確認してください。

正しい例:

```text
decomps/
  agbcc/
  PokeEm-expansion-CanuseJP/
```

失敗しやすい例:

```text
PokeEm-expansion-CanuseJP/
  agbcc/
```

また、`agbcc` 側で `build.sh` を実行しているか確認してください。

```bash
cd ../agbcc
./build.sh
```

### パスに空白がある

WSLでは、空白を含むパスはダブルクォートで囲む必要があります。

```bash
cd "/mnt/c/Users/<Windowsユーザー名>/Desktop/decomp folder"
```

ただし、トラブルを減らすため、作業フォルダ名には空白を入れない方が安全です。

### Windows側で編集したファイルがWSLから見えない

WSL上で作業している場合、Windows側のファイルは `/mnt/c/` 以下にあります。

例:

```text
C:\Users\<Windowsユーザー名>\Desktop\decomps
```

は、WSLでは以下になります。

```text
/mnt/c/Users/<Windowsユーザー名>/Desktop/decomps
```

## 12. 日本語化プロジェクト向けの注意

本プロジェクトでは、日本語表示のために `charmap` やフォント画像、テキストデータを変更している場合があります。

そのため、翻訳作業中に以下のようなエラーが出ることがあります。

```text
unknown character U+....
```

これは、ソース内に `charmap` 未定義の文字が含まれている場合に発生します。

特に混ざりやすい文字は以下です。

```text
全角スペース
全角数字
全角の波括弧 ｛ ｝
全角英字
未定義の記号
```

`{JPN}` や `{ENG}` などの制御コードは、必ず半角で書いてください。

正しい例:

```c
COMPOUND_STRING("{JPN}バッグ")
```

誤った例:

```c
COMPOUND_STRING("｛JPN｝バッグ")
```

見た目は似ていますが、内部的には別文字です。

コンパイラは空気を読んでくれません。

## 13. ROMや公式素材をGitHubに上げない

以下のようなファイルは、GitHubにコミットしないでください。

```text
baserom.gba
*.gba
*.sav
*.srm
公式ROMから直接抽出した素材
```

一応、初期状態では設定済みにしてありますが、必要に応じて `.gitignore` に追加してください。

例:

```gitignore
baserom.gba
*.gba
*.sav
*.srm
JP/
```

ROM本体や公式素材をリポジトリに含めないように注意してください。

## 14. うまくいったか確認する

ビルド後、生成された `.gba` をエミュレータで起動して確認します。

最低限、以下を確認してください。

```text
タイトル画面が表示される
メニューが開ける
戦闘に入れる
日本語テキストが文字化けしていない
セーブ・ロードできる
```

日本語化作業中は、ビルドが通っても表示が崩れる場合があります。

その場合は、直近で変更したテキスト、フォント、`charmap` を確認してください。

## 15. 補足

この文書は、Windows + WSL 環境向けの簡易導入手順です。

環境によっては、この手順だけではうまくいかない場合があります。

その場合は、以下も確認してください。

```text
本家 pret/pokeemerald の INSTALL.md
使用しているリポジトリの README
使用しているリポジトリの INSTALL.md
GitHub Issues
```

本プロジェクトは、`pokeemerald-expansion` 系の日本語対応を試すためのものです。

完全な日本版エメラルドの再現ではなく、英語版ベースの decomp / expansion 環境で日本語表示や日本語テキストを扱いやすくすることを目的としています。
