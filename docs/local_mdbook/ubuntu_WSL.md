## ドキュメントサイトをローカルで動かす Ubuntu WSL1/WSL2

### 事前要件

- 方法 1: Rust ツールチェーン経由でインストールする
    - Rust ツールチェーンが未インストールの場合は、`sudo apt install cargo` コマンドでインストールします。
    - `cargo install mdbook` コマンドで mdBook をインストールします。完了すると次のようなメッセージが表示されます。`{USER}` は Ubuntu のユーザー名です。

        ```
        warning: be sure to add `/home/{USER}/.cargo/bin` to your PATH to be able to run the installed binaries
        ```

    - `/home/{USER}/.cargo/bin` を PATH に追加します。`{USER}` は Ubuntu のユーザー名に置き換えてください。
        - `nano ~/.profile` コマンドでファイルを編集します。
        - 次の行を追加します。***{USER} は Linux のユーザー名に置き換えてください。***

            ```diff
            # set PATH so it includes user's private bin if it exists
            if [ -d "$HOME/bin" ] ; then
                PATH="$HOME/bin:$PATH"
            fi

            # set PATH so it includes user's private bin if it exists
            if [ -d "$HOME/.local/bin" ] ; then
                PATH="$HOME/.local/bin:$PATH"
            fi

            +# set PATH so it includes cargo bin if it exists
            +if [ -d "/home/{USER}/.cargo/bin" ] ; then
            +    PATH="/home/{USER}/.cargo/bin:$PATH"
            +fi
            ```

        - `source ~/.profile` コマンドを実行し、現在のセッションの PATH を更新します。

- 方法 2: ダウンロードしたバイナリを直接インストールする
    - TODO: この手順のドキュメントを追加する。

### サイトを起動する

- リポジトリの `docs` フォルダへ移動します。
- `mdbook serve` を実行します。起動後、ブラウザで `http://127.0.0.1:3000` を開くとサイトを確認できます。
- `docs` フォルダへの変更は自動更新で反映されます。
- サーバーを止めてターミナルへ戻るには `Ctrl + C` を押します。

### サイトを編集する

- 左側のナビゲーションメニューは `docs/SUMMARY.md` で管理されています。追加したファイルを表示するには、このファイルのどこかへ追加する必要があります。追加しない場合は 404 エラーになります。
- `docs/` ディレクトリに追加した Markdown ファイル、つまり `.md` 拡張子のファイルは mdBook に自動で読み込まれます。
- `docs/` ディレクトリ外の Markdown ファイルを追加する場合は、空の `.md` ファイルを作り、次を追加できます。***"----" は含めないでください。***

    ```md
    {{ ----#include ../INSTALL.md}}`
    ```

    これにより、ルートディレクトリの `INSTALL.md` Markdown ファイルが読み込まれます。

セットアップが終われば、リポジトリへ push する前に変更を確認できます。

この仕組みにより、ドキュメントへのコントリビュートがしやすくなることを期待しています。
