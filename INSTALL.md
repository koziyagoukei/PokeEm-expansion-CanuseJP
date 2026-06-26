Windows + WSL 環境でのビルド手順

この文書は、Windows 10 / Windows 11 上で WSL を使い、pokeemerald / pokeemerald-expansion 系プロジェクトをビルドするための日本語メモです。
本家 pret/pokeemerald の INSTALL.md を参考にしていますが、本プロジェクト向けに内容を絞っています。
正確な最新情報は、必ず本家の INSTALL.md も確認してください。

対象環境

この文書では、以下の環境を想定しています。

Windows 10 または Windows 11
WSL / Ubuntu、Git、make、arm-none-eabi 系ツール、libpng

この文書では、msys2 / Cygwin / macOS / Linux単体環境については扱いません。

1. WSLをインストールする
まず、WindowsにWSLを入れます。
PowerShellを「管理者として実行」し、以下を実行します。

wsl --install -d Ubuntu --enable-wsl1

完了したら、Windowsを再起動します。
再起動後、もう一度 PowerShell を「管理者として実行」し、UbuntuをWSL1に設定します。

wsl --set-version Ubuntu 1

すでにWSLを導入済みの場合は、この手順を飛ばしても構いません。

2. Ubuntuを初期設定する

スタートメニューから Ubuntu を起動します。

初回起動時に、Linux側のユーザー名とパスワードを作成します。
パスワード入力中は画面に文字が表示されませんが、入力はされています。何も出ないからといって「なにもしてないのにこわれた」と言わないでください。

3. Ubuntuを更新する

Ubuntuを開いた状態で、以下を実行します。

sudo apt update && sudo apt upgrade

途中で確認が出た場合は、内容を確認して Y を入力します。

4. 必要なパッケージを入れる

ビルドに必要なパッケージをインストールします。

sudo apt install build-essential binutils-arm-none-eabi git libpng-dev

もし apt でうまくいかない場合は、以下のように apt-get を使ってください。

sudo apt-get install build-essential binutils-arm-none-eabi git libpng-dev

5. 作業フォルダを作る

この文書では、Windows側のデスクトップに decomps フォルダを作って作業する例で説明します。
Windowsのエクスプローラーで、以下のようなフォルダを作成してください。

C:\Users\<Windowsユーザー名>\Desktop\decomps

WSLから見ると、WindowsのCドライブは /mnt/c/ にあります。
Ubuntu上で、以下のように移動します。

cd /mnt/c/Users/<Windowsユーザー名>/Desktop/decomps

例:
cd /mnt/c/Users/Emerald/Desktop/decomps

パスに空白がある場合は、ダブルクォートで囲みます。

cd "/mnt/c/Users/<Windowsユーザー名>/Desktop/decomp folder"

6. リポジトリを取得する

通常の pokeemerald を使う場合は、以下のように取得します。

git clone https://github.com/pret/pokeemerald.git

cd pokeemerald

pokeemerald-expansion 系や、本プロジェクトのフォークを使う場合は、対象リポジトリのURLに置き換えてください。

例:
git clone https://github.com/<ユーザー名>/<リポジトリ名>.git
cd <リポジトリ名>

7. agbccを用意する

pokeemerald 系プロジェクトでは、GBA向けの古いCコンパイラである agbcc が必要になる場合があります。

agbcc の導入方法は、使用しているプロジェクトや環境によって差が出ることがあります。
この手順は一例です。うまくいかない場合は、本家 pret/pokeemerald の INSTALL.md や、使用しているリポジトリの説明を確認してください。

7-1. 作業フォルダの構成

この文書では、以下のような構成を想定します。

decomps/
  agbcc/
  pokeemerald/

または、pokeemerald-expansion 系の場合は以下のようになります。

decomps/
  agbcc/
  pokeemerald-expansion/

つまり、agbcc と pokeemerald / pokeemerald-expansion を同じ階層に置きます。

例:

cd /mnt/c/Users/<Windowsユーザー名>/Desktop/decomps

7-2. agbccを取得する

作業フォルダで、以下を実行します。

git clone https://github.com/pret/agbcc.git

取得後、以下のような構成になっていればOKです。

decomps/
  agbcc/
  pokeemerald/

7-3. agbccをビルドする

agbcc フォルダに移動します。

cd agbcc

ビルドします。

./build.sh

環境によっては、実行権限がないと言われる場合があります。

その場合は、以下を実行してからもう一度試してください。

chmod +x build.sh
./build.sh

7-4. pokeemerald側へ戻る

agbcc の準備が終わったら、プロジェクト側へ戻ります。

cd ../pokeemerald

pokeemerald-expansion の場合は、以下のように移動します。

cd ../pokeemerald-expansion

その後、通常どおりビルドします。

make -j5

7-5. agbcc関連で失敗する場合

agbcc 周りで失敗する場合、以下を確認してください。

- agbcc と pokeemerald が同じ階層にあるか
- agbcc/build.sh を実行したか
- build.sh に実行権限があるか
- WSL上で作業しているか
- WindowsのPowerShellやcmdではなく、Ubuntu側で実行しているか

想定する配置は以下です。

decomps/
  agbcc/
  pokeemerald/

ありがちな失敗例です。

pokeemerald/
  agbcc/

このように、pokeemerald フォルダの中に agbcc を入れてしまうと、プロジェクト側から見つからない場合があります。

また、agbcc の導入手順はプロジェクト側の更新で変わる可能性があります。
この手順で失敗する場合は、本家の INSTALL.md を確認してください。

8. ビルドする

リポジトリのルートで、以下を実行します。

make -j5

CPUに余裕がある場合は、数字を増やしても構いません。

例:
make -j8

ビルドに成功すると、.gba ファイルが生成されます。

10. よくあるエラー

arm-none-eabi-gcc
が見つからない

必要なパッケージが入っていない可能性があります。

sudo apt install binutils-arm-none-eabi

環境によっては、追加で gcc-arm-none-eabi が必要になる場合があります。

sudo apt install gcc-arm-none-eabi


png.h
が見つからない

libpng-dev が入っていない可能性があります。

sudo apt install libpng-dev

パスに空白がある

WSLでは、空白を含むパスはダブルクォートで囲む必要があります。

cd "/mnt/c/Users/<Windowsユーザー名>/Desktop/decomp folder"

ただし、トラブルを減らすため、作業フォルダ名には空白を入れない方が安全です。
おすすめ:


decomps
pokeemerald-expansion

避けたい例:
decomp folder
pokemon hack project

Windows側で編集したファイルがWSLから見えない

WSL上で作業している場合、Windows側のファイルは /mnt/c/ 以下にあります。

例:
C:\Users\<Windowsユーザー名>\Desktop\decomps

は、WSLでは以下になります。

/mnt/c/Users/<Windowsユーザー名>/Desktop/decomps

devkitARM / ARMツールチェインがない

このプロジェクトのビルドには、GBA向けの ARM コンパイラ環境が必要です。

Windows + WSL 環境では、以下のような arm-none-eabi 系パッケージでビルドできる場合があります。

sudo apt update
sudo apt install binutils-arm-none-eabi gcc-arm-none-eabi

日本語化プロジェクト向けの注意

本プロジェクトでは、日本語表示のために charmap やフォント画像、テキストデータを変更している場合があります。
そのため、翻訳作業中に以下のようなエラーが出ることがあります。

unknown character U+....

これは、ソース内に charmap 未定義の文字が含まれている場合に発生します。
特に混ざりやすい文字は以下です。

全角スペース
全角数字
全角の波括弧 ｛ ｝
全角英字
未定義の記号
{JPN} や {ENG} などの制御コードは、必ず半角で書いてください。
正しい例:
COMPOUND_STRING("{JPN}バッグ")
誤った例:
COMPOUND_STRING("｛JPN｝バッグ")

見た目は似ていますが、内部的には別文字です。コンパイラは空気を読んでくれません。

ROMや公式素材をGitHubに上げない

以下のようなファイルは、GitHubにコミットしないでください。
baserom.gba
*.gba
*.sav
*.srm
公式ROMから直接抽出した素材

一応、初期状態では設定済みにしてありますが、必要に応じて .gitignore に追加してください。

例:
baserom.gba
*.gba
*.sav
*.srm
JP/

10.うまくいったか確認する

ビルド後、生成された .gba をエミュレータで起動して確認します。
最低限、以下を確認してください。

タイトル画面が表示される
メニューが開ける
戦闘に入れる
日本語テキストが文字化けしていない
セーブ・ロードできる
日本語化作業中は、ビルドが通っても表示が崩れる場合があります。
その場合は、直近で変更したテキスト、フォント、charmapを確認してください。
