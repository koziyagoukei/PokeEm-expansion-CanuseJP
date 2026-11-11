# GC Extension Mode（gba-gc-extension 連携）

GBAで所持しているパーティを、JoyBus経由でGameCube側の専用ソフト（gba-gc-extension の `companion.dol`）へ送るテストモードです。GameCube側ではポケモンのsprite・名前・技などをTVに表示します。

- **主な対象:** Dolphin + Integrated GBA（GCポート2）
- **実機対応:** 現時点ではプロジェクト要件ではありません

## 入り方

非release buildで、STARTメニュー → デバッグ → べんりきのう → **GC Extension Mode**（最後の項目）を選びます。通常のゲーム進行からは呼ばれません。

| 状態 | 操作 |
|---|---|
| GCを待っている間・転送中 | B: キャンセルしてfieldへ戻る |
| `GC CONNECTED > PARTY` | A: パーティ一覧 / B: 終了 |
| パーティ一覧 | 上下: 選択（TVが追従）/ A: 決定 / B: 戻る |
| GCが終了したとき | A: 再接続 / B: 終了 |

## 構成

| ファイル | 役割 |
|---|---|
| `src/gc_companion.c` | special `GcCompanion_Start`、state machine、GBA画面、field復帰 |
| `src/gc_companion_link.c` | JoyBus IRQキュー（LinkCube方式のC移植）、CRC・ACK・再送応答 |
| `src/gc_companion_protocol.c` | CAPS / APP / ASSET / BYE の汎用メッセージ処理 |
| `src/gc_companion_pokemon.c` | PokemonRecordV1、sprite解決、ゲームフォントでのラベル描画、状態digest |
| `src/gc_ext/`, `include/gc_ext/` | gba-gc-extension の `protocol/` のコピー（直接編集しない） |

上記以外の変更は、`src/debug.c` のメニュー1項目、`data/scripts/debug.inc` のスクリプト1本、`data/specials.inc` の1行だけです。

`gc_ext/` を更新するときは、gba-gc-extension 側で次を実行します。

```powershell
./tools/sync-pokeemerald.ps1 -Target <このリポジトリ>
```

## 安全性

このモードはゲーム状態を読むだけです。party・bag・flags・vars・saveには書き込みません。

- **digestの確認:** 開始時と終了時に、party / bag（復号済みの道具ID・個数）/ flags / vars / SaveBlock1 のCRCを比較します。結果は戻る直前に画面へ表示します。
- **JoyBusの後片付け:** 終了時にRCNT・IE・serial割り込みベクタを元に戻します。その後 `CB2_ReturnToFieldContinueScript` でスクリプトへ戻ります。

プロトコル仕様・計測結果・制約は、gba-gc-extension の `docs/phase2.md` を参照してください。
