# とくしゅドール

## 仕様

- ミニミニべっそうの室内PCに「とくしゅドール」を追加。
- 「かざる」「かたづける」「すてる」は既存の模様替え操作を利用する。
- 所持品は通常家具と独立した64個の保管枠。配置は最大3体。
- 通常のスプライト家具と合計して12体まで。机・マット等の背景家具はこの12体には含めない。
- 部屋の通常家具16個の保存配列とは別に3体の位置を保存する。
- 対象は専用の歩行画像があるメガ48形態、ゲンシ2形態、ウルトラネクロズマの計51種類。
- 歩行画像の正面1フレームを静止表示する。フォルム変更や手持ちの生成は行わない。
- BPショップは家具カテゴリ別。とくしゅドールも1個10BP。
- とくしゅドールはこのローカル部屋専用。通常秘密基地・プレイヤー自室には配置しない。

## 保存・互換性

`SaveBlock3`の既存72バイトの後ろに、独立した80バイトの`SpecialDollSave`を追加する。
`LocalRoomSave`の位置・大きさ・バージョン・チェックサム範囲は変更しない。
新領域は専用magic、version、CRCで検証し、旧セーブの未使用データや破損データは特殊ドール領域だけ初期化する。

通常の`secretBases[]`、プレイヤー自室、通常家具の所持配列は変更しない。
通信構造体、record mixing、リンクパケットには追加しない。
`LINK_COMPAT_VERSION`および`LINK_DATA_LAYOUT_VERSION`は変更しない。

ドールIDは`include/constants/special_doll_list.h`の順序で保存するため、既存行を並べ替えず、新規追加は末尾に行う。
フィールド画像は固定範囲`0x0800`以降、パレットは`0x1200`以降を使い、既存の画像ID・動的画像変数の値をずらさない。
配置時には部屋の既存家具用オブジェクト枠を共用し、徘徊3匹と主人公の枠を維持する。
部屋内では主人公・天候用の固定パレット領域を避け、配置プレビューも独立したパレットを使用する。

## 検証結果

- `make -j4 --output-sync=target` 成功。出力は `pokeemerald.gba`。
- mGBA上の自動テストは、特殊ドール11件、ショップ12件、ローカル部屋30件の計53件が成功。
- 日本語テキスト検査とリンク互換性のfingerprint検査が成功。互換バージョンは従来の9/5のまま。
- 実画面でカテゴリ選択、特殊ドールの10BP購入、3体上限、配置・片付け・再配置を確認。
- 通常家具9体・特殊ドール3体・徘徊3匹・主人公の同時表示と、バッグ開閉後の復帰を確認。
- 元の `pokeemerald.sav` は変更せず、一時コピーで検証。保存内容のシリアライズ往復は自動テスト済み。
- 実際のセーブ・再起動と通信機器を使った検証は未実施。通信関連ソースには新データへの参照を追加していない。

## 手動確認

1. ショップで家具のカテゴリを切り替え、机・ドール・クッション等が別々に並ぶことを確認。
2. 「とくしゅドール」で交換し、BPが10減り、通常ドールの所持数が変わらないことを確認。
3. 室内PCから特殊ドール3体を配置し、4体目を置けないことを確認。
4. 通常スプライト家具9体と特殊ドール3体を配置し、追加のスプライト家具を置けないことを確認。
5. 特殊ドールを1体片付け、通常スプライト家具を1体追加できることを確認。
6. 大型ドール、徘徊ポケモン3匹、バッグの開閉、退出・再入室を確認。
7. セーブ・再起動後、所持品・配置・床壁・通常家具が維持されることを確認。
8. 「ぜんぶ かたづける」で通常家具と特殊ドールの配置が消え、両方の所持品が残ることを確認。
9. 通常秘密基地とプレイヤー自室の模様替えが従来どおり動き、配置が相互に混入しないことを確認。

## 変更ファイル

- `data/maps/MiniMiniVilla_LocalRoom/scripts.inc`
- `data/maps/MiniMiniVilla_Shop/scripts.inc`
- `data/specials.inc`
- `include/constants/special_doll_list.h`（追加）
- `include/constants/special_dolls.h`（追加）
- `include/constants/villa_shop.h`
- `include/decoration.h`
- `include/global.h`
- `include/local_room.h`
- `include/special_doll.h`（追加）
- `include/villa_shop.h`
- `src/data/script_menu.h`
- `src/data/special_doll_graphics.h`（追加）
- `src/decoration.c`
- `src/event_object_movement.c`
- `src/local_room.c`
- `src/special_doll.c`（追加）
- `src/villa_shop.c`
- `test/local_room.c`
- `test/frontier_pool_sets.c`（既存テストのビットフィールド比較に型を明示し、テストビルドを修正）
- `test/special_doll.c`（追加）
- `test/villa_shop.c`
- `docs/special_dolls.md`（本書）
