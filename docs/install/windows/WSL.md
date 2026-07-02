# Windows WSL 向け手順

## WSL バージョンの選択

プロジェクトを Windows ファイルシステム上、つまり `/mnt/c/` 配下に置く必要がある場合は WSL1 を使ってください。
コンパイル時の性能を重視し、Windows 側の干渉による問題を減らしたい場合は、WSL2 を使い、プロジェクトを Linux ファイルシステム上、つまり `~/` 配下に置いてください。

## WSL のインストール

1. [Windows PowerShell を管理者として開き](https://i.imgur.com/QKmVbP9.png)、次のコマンドを実行します。PowerShell では右クリックまたは Shift+Insert で貼り付けできます。

    ```powershell
    wsl --install -d Ubuntu --enable-wsl1
    ```

2. 処理が終わったら PC を再起動します。

### WSL1

3. 再起動後、もう一度 Windows PowerShell を管理者として開き、Ubuntu を WSL1 で使うように次のコマンドを実行します。

    ```powershell
    wsl --set-version Ubuntu 1
    ```

### WSL2

3. 再起動後、もう一度 Windows PowerShell を管理者として開き、Ubuntu を WSL2 で使うように次のコマンドを実行します。

    ```powershell
    wsl --set-version Ubuntu 2
    ```

    <details>
        <summary><i>補足...</i></summary>

    > 再起動後に WSL が自動で開く場合がありますが、この時点では無視してかまいません。
    </details>

## 依存パッケージのインストール

作業前の補足:

- WSL でのコピーと貼り付けは、次のどちらかで行います。
    - **右クリック**: 選択して右クリックでコピー、未選択で右クリックすると貼り付け。
    - **Ctrl+Shift+C/Ctrl+Shift+V**: タイトルバーを右クリックしてプロパティを開き、「Ctrl+Shift+C/V をコピー/貼り付けとして使う」を有効にします。
- 実行するコマンドによっては、WSL のパスワードや確認入力を求められます。必要に応じて WSL のパスワード、または yes に相当する入力を行ってください。

1. 検索などから **Ubuntu** を開きます。
2. WSL/Ubuntu は初回起動時に独自のセットアップを行います。完了すると、ユーザー名とパスワードの入力を求められます。

    <details>
        <summary><i>補足...</i></summary>

    > パスワード入力中は画面に文字が表示されませんが、入力自体は受け付けられています。
    </details>

3. 続行する前に WSL/Ubuntu を更新します。次のコマンドを実行してください。完了まで時間がかかる場合があります。

    ```bash
    sudo apt update && sudo apt upgrade
    ```

4. pokeemerald Expansion のビルドに必要なパッケージをインストールします。

    ```bash
    sudo apt install build-essential binutils-arm-none-eabi gcc-arm-none-eabi libnewlib-arm-none-eabi git libpng-dev python3
    ```

## pokeemerald Expansion の保存場所を選ぶ WSL1

WSL には Windows から直接扱いにくい独自のファイルシステムがありますが、WSL から Windows のファイルにはアクセスできます。そのため WSL1 では pokeemerald Expansion を Windows 側に置くことになります。

たとえば pokeemerald Expansion を **C:\Users\\_\<user>_\Desktop\decomps** に置きたい場合、まずそのフォルダが存在することを確認してください。その後、次のコマンドでそのフォルダへ移動します。*\<user>* は **Windows** のユーザー名に置き換えてください。

```bash
cd /mnt/c/Users/<user>/Desktop/decomps
```

<details>
    <summary><i>補足...</i></summary>

> 補足 1: Windows の C:\ ドライブは WSL では `/mnt/c/` です。
> 補足 2: パスにスペースが含まれる場合は、`cd "/mnt/c/users/<user>/Desktop/decomp folder"` のように引用符で囲んでください。
> 補足 3: Windows のパス名は大文字小文字を区別しないため、厳密な大文字小文字は不要です。
</details>

## pokeemerald Expansion の保存場所を選ぶ WSL2

WSL には Windows から直接扱いにくい独自のファイルシステムがありますが、WSL から Windows のファイルにはアクセスできます。ただし WSL2 から Windows ファイルシステム上のファイルへアクセスすると非常に遅いため、pokeemerald Expansion は WSL2 側に置いてください。

Windows から WSL ファイルシステム上のファイルへアクセスするには、ファイルエクスプローラーで WSL ファイルシステムをネットワークストレージとして開きます。左サイドバーの下の方に "Ubuntu" として表示されます。

そのため、WSL ファイルシステム内にいることを確認し、必要なら decomps フォルダを作成して、そのフォルダへ移動します。

```bash
cd ~/
mkdir decomps
cd decomps
```
