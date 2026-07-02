# macOS 向け手順

1. Xcode Command Line Tools が未インストールの場合は、[ここ](https://developer.apple.com/xcode/resources/)からツールをダウンロードし、ターミナルを開いて次のコマンドを実行してください。

    ```bash
    xcode-select --install
    ```

2.  - libpng が**未インストール**の場合は、[libpng のインストール macOS](#libpng-のインストール-macos)へ進みます。
    - pkg-config が**未インストール**の場合は、[pkg-config のインストール macOS](#pkg-config-のインストール-macos)へ進みます。
    - devkitARM が**未インストール**の場合は、[devkitARM のインストール macOS](#devkitarm-のインストール-macos)へ進みます。
    - それ以外の場合は、**ターミナルを開き**、[pokeemerald-expansion の保存場所を選ぶ macOS](#pokeemerald-expansion-の保存場所を選ぶ-macos)へ進みます。

3. **任意: テストを実行する場合**、Homebrew 環境が未インストールなら、[この案内](https://brew.sh)を参照してパッケージマネージャをインストールします。ターミナルを開いて次のコマンドを実行してください。

    ```bash
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    brew install coreutils
    ```

4. **任意: Rosetta 経由でテストを実行する場合**
    - これはかなり遅いため、通常は推奨しません。多くのユーザーはネイティブツールを使えますが、Intel 専用のカスタムツールなどを使う事情がある場合は、この構成が必要になることがあります。
    - Intel 互換の Homebrew インストールが必要です。入手方法は[こちら](https://github.com/Homebrew/brew/issues/9173#issuecomment-729206868)を参照してください。
    - 手順 3 と同じように `coreutils` をインストールします。ただし Intel 互換の Homebrew を使ってください。

### libpng のインストール macOS

<details>
    <summary><i>上級者向け補足...</i></summary>

> この手順では、もっとも簡単な方法として Homebrew 経由で libpng をインストールします。必要であれば、上級者は別の方法で libpng をインストールしてもかまいません。
</details>

1. ターミナルを開きます。
2. Homebrew が未インストールの場合は、[Homebrew](https://brew.sh/) のサイトの手順に従ってインストールします。
3. 次のコマンドで libpng をインストールします。

    ```bash
    brew install libpng
    ```

    これで libpng がインストールされます。

    **pkg-config が未インストール**の場合は [pkg-config のインストール macOS](#pkg-config-のインストール-macos)へ進みます。**devkitARM が未インストール**の場合は [devkitARM のインストール macOS](#devkitarm-のインストール-macos)へ進みます。

    pkg-config と devkitARM の両方がすでにインストールされている場合は、[pokeemerald-expansion の保存場所を選ぶ macOS](#pokeemerald-expansion-の保存場所を選ぶ-macos)へ進みます。

### pkg-config のインストール macOS

<details>
    <summary><i>上級者向け補足...</i></summary>

> この手順では、もっとも簡単な方法として Homebrew 経由で pkg-config をインストールします。必要であれば、上級者は別の方法で pkg-config をインストールしてもかまいません。
</details>

1. ターミナルを開きます。
2. Homebrew が未インストールの場合は、[Homebrew](https://brew.sh/) のサイトの手順に従ってインストールします。
3. 次のコマンドで pkg-config をインストールします。

    ```bash
    brew install pkg-config
    ```

    これで pkg-config がインストールされます。

    **devkitARM が未インストール**の場合は [devkitARM のインストール macOS](#devkitarm-のインストール-macos)へ進みます。それ以外の場合は、[pokeemerald-expansion の保存場所を選ぶ macOS](#pokeemerald-expansion-の保存場所を選ぶ-macos)へ進みます。

### devkitARM のインストール macOS

1. [ここ](https://github.com/devkitPro/pacman/releases)から `devkitpro-pacman-installer.pkg` パッケージをダウンロードします。
2. パッケージを開き、devkitPro pacman をインストールします。
3. ターミナルで次のコマンドを実行し、devkitARM をインストールします。

    ```bash
    sudo dkp-pacman -Sy
    sudo dkp-pacman -S gba-dev
    sudo dkp-pacman -S devkitarm-rules
    ```

    `gba-dev` のコマンドでは、インストールするパッケージの選択を求められます。Enter を押してすべてを選択し、その後 Y を入力してインストールを続行してください。

4. ツールのインストール後、devkitARM をシステムのどこからでも使えるようにする必要があります。次のコマンドを実行してください。

    ```bash
    export DEVKITPRO=/opt/devkitpro
    echo "export DEVKITPRO=$DEVKITPRO" >> ~/.zshrc
    export DEVKITARM=$DEVKITPRO/devkitARM
    echo "export DEVKITARM=$DEVKITARM" >> ~/.zshrc

    echo "if [ -f ~/.zshrc ]; then . ~/.zshrc; fi" >> ~/.zprofile
    ```

    *補足: macOS 10.15 以降、標準の Unix シェルは zsh です。古い macOS から移行した場合は bash を使っている可能性があります。ターミナルで `echo $0` を実行すると確認できます。*

    <details>
        <summary><i>ターミナルが zsh ではなく bash を使っている場合...</i></summary>

    ```bash
    export DEVKITPRO=/opt/devkitpro
    echo "export DEVKITPRO=$DEVKITPRO" >> ~/.bashrc
    export DEVKITARM=$DEVKITPRO/devkitARM
    echo "export DEVKITARM=$DEVKITARM" >> ~/.bashrc

    echo "if [ -f ~/.bashrc ]; then . ~/.bashrc; fi" >> ~/.bash_profile
    ```
    </details>

### Python のインストール macOS

1. [ここ](https://www.python.org/downloads/)から最新の Python パッケージをダウンロードします。
2. パッケージを開き、Python をインストールします。

これで Python がインストールされます。

### pokeemerald-expansion の保存場所を選ぶ macOS

任意の作業用ディレクトリに移動し、リポジトリを配置してください。例として `~/Decomps` を使う場合は、次のようにします。

```bash
mkdir -p ~/Decomps
cd ~/Decomps
```

その後、通常の手順に従ってリポジトリを取得し、ビルドしてください。
