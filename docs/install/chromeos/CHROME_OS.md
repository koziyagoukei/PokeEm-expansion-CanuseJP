# ChromeOS 向け手順

1. [このページ](https://chromeos.dev/en/productivity/terminal)の手順に従って Linux ターミナルを有効にしてください。Linux のインストール用に十分な容量を割り当ててください。
2. Linux ターミナルのインストールが終わったら、ターミナルで次のコマンドを実行して更新してください。

    ```console
    sudo apt update && apt upgrade
    ```

3. 次のコマンドで依存パッケージをすべてインストールします。

    ```console
    sudo apt install build-essential binutils-arm-none-eabi gcc-arm-none-eabi libnewlib-arm-none-eabi git libpng-dev python3
    ```

**注意**: このプロジェクトは Linux ファイルシステム内のディレクトリに置く必要があります。例: `~/Decomps/pokeemerald-expansion`
