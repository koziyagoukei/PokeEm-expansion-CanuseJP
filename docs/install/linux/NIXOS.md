# NixOS 向け手順

必要なパッケージを含む対話シェルを起動するには、次のコマンドを実行してください。

```bash
nix-shell -p pkgsCross.arm-embedded.stdenv.cc git pkg-config libpng
```
