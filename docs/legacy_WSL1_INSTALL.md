### WSL1 のセットアップ レガシー部分

1. pokeemerald のビルドにはいくつかのパッケージが必要です。次のコマンドでインストールしてください。

    ```bash
    sudo apt install build-essential git libpng-dev gdebi-core
    ```

    > 補足: 上のコマンドが動作しない場合は、`apt` を `apt-get` に置き換えて試してください。

2. パッケージのインストールが終わったら、devkitPro pacman パッケージを[ここ](https://github.com/devkitPro/pacman/releases)からダウンロードします。ダウンロードするファイルは `devkitpro-pacman.amd64.deb` です。

3. WSL には Windows からアクセスできない独自のファイルシステムがありますが、WSL から Windows のファイルにはアクセスできます。devkitPro パッケージをインストールするには、パッケージファイルを保存したディレクトリへ移動する必要があります。

    たとえばパッケージファイルを **C:\Users\\_\<user>_\Downloads** に保存した場合、多くの環境では Downloads フォルダになります。*\<user>* を **Windows** のユーザー名に置き換えて、次のコマンドを入力してください。

    ```bash
    cd /mnt/c/Users/<user>/Downloads
    ```

    > 補足 1: Windows の C:\ ドライブは WSL では `/mnt/c/` です。
    > 補足 2: パスにスペースが含まれる場合は、`cd "/mnt/c/users/<user>/Downloads folder"` のように引用符で囲んでください。
    > 補足 3: Windows のパス名は大文字小文字を区別しないため、厳密な大文字小文字は不要です。

4. devkitPro pacman パッケージがあるフォルダへ移動したら、次のコマンドで devkitARM をインストールします。

    ```bash
    sudo gdebi devkitpro-pacman.amd64.deb
    sudo dkp-pacman -Sy
    sudo dkp-pacman -S gba-dev
    ```

    最後のコマンドではインストールするパッケージの選択を求められます。Enter を押してすべてを選択し、その後 Y を入力してインストールを続行してください。

    > 補足: `devkitpro-pacman.amd64.deb` は、ダウンロードした devkitPro パッケージの想定ファイル名です。ファイル名が異なる場合は、そのファイル名を使ってください。

5. 次のコマンドで devkitPro 関連の環境変数を設定します。WSL を閉じて開き直してもかまいません。

    ```bash
    source /etc/profile.d/devkit-env.sh
    ```

続きは現在の INSTALL.md の [pokeemerald の保存場所を選ぶ WSL1](/INSTALL.md#choosing-where-to-store-pokeemerald-expansion-WSL1) に進んでください。
