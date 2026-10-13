# Local Room: 調査・実装・確認手順

## 実装内容

- ミニミニべっそう左上の建物の入口 `(3, 6)` を `MiniMiniVilla_LocalRoom` へ接続。
- 室内は16x14、PCは `(1, 3)`、出口は `(7, 12)` と `(8, 12)`。退出先は同じ建物の入口。
- 通常の秘密基地ではなく `MAP_TYPE_INDOOR` の独立した室内。基地owner・trainer・registry・friend base・secretBaseIdは持たない。
- PCから既存の模様替えUI、床、壁、徘徊数、全家具撤去を選択可能。
- 配置変更は原作UIと同じく「片付ける -> 飾る」。基地撤去・登録・他人の基地メニューは表示しない。
- 配置上限は16。人形・クッションのオブジェクト上限は12で、プレイヤー1人＋徘徊3匹を含め `OBJECT_EVENTS_COUNT` の16枠以内。
- 徘徊は「はなさない / 1ぴき / 2ひき / 3びき」。卵を除いた手持ち順で選び、家具とプレイヤーを避けて3x3の空き床を探す。空間が不足する場合は生成できる匹数だけ表示する。
- 模様替え中は徘徊を一時停止・非表示。終了後と手持ち画面からの復帰時に手持ちから再生成する。
- 家具を置いた後も出入口からPCへ歩けることを確認してから配置する。PC・操作位置・出口のみ固定で保護し、それ以外の床は自由に使える。室内からキャンプへSELECTで再入場する処理は無効にして、元の動的帰還先を保護する。

## 既存実装の調査結果

### 模様替えcontext

`src/decoration.c` の `sDecorationContext` は、もともと `items`、`pos`、`size`、`isPlayerRoom` を持ち、保存先だけを切り替えていた。

| Context | 家具配列 | 座標配列 | 上限 |
|---|---|---|---|
| 通常秘密基地 | `secretBases[0].decorations` | `secretBases[0].decorationPositions` | `DECOR_MAX_SECRET_BASE = 16` |
| 元のプレイヤー自室 | `playerRoomDecorations` | `playerRoomDecorationPositions` | `DECOR_MAX_PLAYERS_HOUSE = 12` |
| 新ローカル部屋 | `gSaveBlock3Ptr->localRoom.decorations` | `gSaveBlock3Ptr->localRoom.decorationPositions` | `LOCAL_ROOM_DECOR_COUNT = 16` |

`DoSecretBaseDecorationMenu()` と `DoPlayerRoomDecorationMenu()` は従来の保存先を維持。`DoLocalRoomDecorationMenu()` を追加し、boolを `enum DecorationContextType` に変更した。`InitDecorationContextItems()` は3種類を明示的に再バインドする。

原作自室だけに適用される「人形・クッション限定」条件は維持し、新ローカル部屋には適用しない。共有家具インベントリの使用中判定を3部屋に拡張し、配置中の家具コピーを重複使用・売却・交換できないようにした。

### 保存構造

`include/global.h` の `SaveBlock1`、`SaveBlock2`、`SecretBase`、`SecretBaseParty` の宣言は変更していない。通常秘密基地と元の自室の配列オフセットは不変。

このチェックアウトには `SaveBlock3` があり、`src/save.c` の `CopyToSaveBlock3()` / `CopyFromSaveBlock3()` が、各flash sectorの未使用116バイト領域へ追加データを保存する。最大保存容量は1624バイト。

新しい `LocalRoomSave` は既存 `SaveBlock3` の28バイトの後ろへ追加した44バイト。構成はmagic、version、floorStyle、wallStyle、roamerCount、家具16バイト、座標16バイト、CRC16、reserved。

- `SaveBlock3`: 28 -> 72バイト。既存フィールドは移動しない。全体が最初の116バイトの拡張保存領域へ収まる。
- 未初期化、旧セーブの未使用領域、未知のversion、CRC不一致はローカル部屋だけを空状態へ初期化する。
- 家具ID、家具サイズ、座標、設定値、スプライト個数を検証してから描画する。
- 配置・撤去・設定変更時にCRCを更新する。新規ゲームの既存 `ClearSav3()` により初期状態は空。
- 配置座標は原作と同じ4bit X / 4bit Yで、16x14の室内は範囲内。
- 将来のセーブ構造更新では `localRoom` より前へフィールドを挿入しない。オフセット28を自動テストで固定している。
- 旧ROMへ戻して上書き保存すると追加領域は維持されない可能性があるため、バックアップを使う。

既存セーブの通常領域のチェックサム対象サイズ、sector配置、`SaveBlock1` / `SaveBlock2` は変更していない。

### マップ初期化

`data/scripts/shared_secret_base.inc` は基地owner、基地変数、registry、初回入室処理を含むため、新しい部屋からは取り込んでいない。

新マップ専用のスクリプトで以下を呼ぶ。

1. `LocalRoom_PrepareMap()` で専用セーブ検証と家具・徘徊objectの非表示フラグを準備。
2. `LocalRoom_LoadDecorations()` でROMの床・壁を復元し、保存済みの非スプライト家具を描画。
3. `LocalRoom_InitObjects()` で家具objectを一度除去し、必要な家具だけを生成。その後、空き床へ徘徊を生成。
4. `LocalRoom_Resume()` とタイルセットcallbackで床・壁のグラフィックとパレットを復元。

`InitSecretBaseDecorationSprites()` の描画部分だけを再利用し、ローカルマップの場合は専用配列へ分岐する。`CurMapIsSecretBase()` は新マップでFALSEとなり、他人の基地用の人形・クッションscriptへの変更は起きない。

片付け後は専用scriptを使う。原作scriptにある `setflag VAR_0x8005` の間接flag指定に依存せず、ローカルcontextの場合だけC側で実際の家具flagを立てる。通常秘密基地用scriptは変更していない。

## 通信との分離

`src/record_mixing.c` は `gSaveBlock1Ptr->secretBases` を `dest->secretBases` へ、その配列サイズだけコピーする。ローカルデータをその配列へ入れていない。

`struct SecretBase` と `SecretBaseParty` のサイズ・宣言、record-mixing構造体、link packet、基地受信・登録処理は不変。`LINK_COMPAT_VERSION` / `LINK_DATA_LAYOUT_VERSION` も変更しない。

確認コマンド:

```sh
rg -n 'localRoom|LocalRoom|LocalRoomSave|gSaveBlock3Ptr' \
  src/record_mixing.c src/link.c src/link_rfu_2.c src/link_rfu_3.c \
  src/link_compat.c include/config/link_compat.h
```

結果: **一致0件**。

`tools/local_room/check_assets.py` では、これらの通信ファイルが変更されていないこと、保存構造が維持されていること、ローカル部屋のC・scriptにowner/registry/record-mixing処理が入っていないことも検査する。

## 家具と床・壁

専用タイルセット `gTileset_LocalRoom` は秘密基地家具のmetatile ID・behaviorを維持し、部屋の枠だけを通常民家のチップへ置き換える。元の民家・秘密基地の画像、metatiles、パレットは編集していない。

- 床: 主人公宅の木床、一般民家の木床、ミナモ美術館のタイル床。
- 壁: 主人公宅の白壁、同じ壁チップの青色パレット版、海の家の木目壁（カーテンなし）。
- 床と壁を独立して変更する。家具の下とポスターの背後も同じ選択へ追従する。
- 床は `MB_NORMAL`、ポスター用北壁は `MB_SECRET_BASE_NORTH_WALL`。
- ローカル部屋の人形・クッションは、普通の床と机・マット上へ配置できる。通常秘密基地と元の自室では、机・マット等の `MB_HOLDS_SMALL_DECORATION` / `MB_HOLDS_LARGE_DECORATION` 条件を維持する。
- 大型机、植物、マット、ポスターなどは原作のpermission・shapeで配置判定する。
- 風船・泥だんご・壊せるドアの歩行演出は専用step callbackで再利用し、基地訪問・他人の家具使用記録は更新しない。
- 砂の置物は描画演出のみを再利用する。

**配置対象から除外した家具はない。** 全120種類に室内へ収まる配置寸法があることをテストした。ただし通常配置の条件は必要。

秘密基地固有で使用しない機能:

- 部屋に穴は設けていないため、ソリッドボードの「穴を塞ぐ」用途はない。家具としての配置は可能。
- テレビ・シールドは配置と専用の短い説明のみ。秘密基地訪問記録・番組連動・通信相手の基地関連処理は呼ばない。
- 基地owner、トレーナー、基地撤去、登録、他人の基地表示はない。

## 検証結果

- `make -j4 --output-sync=target`: 成功。
- `python3 tools/check_jp_text.py`: 成功。
- `python3 tools/local_room/check_assets.py`: 成功。Pillowを導入したPythonで `--textures` も成功。
- `make -j4 --output-sync=target check TESTS='Local room:'`: mGBA上で26件成功。
- `git -c core.whitespace=cr-at-eol diff --check`: 成功。

26件は初期化、旧セーブ領域、直列化往復、未知version、CRC不一致、設定上限、無効家具・座標、12sprite上限、3部屋の別保存、通常基地撤去、ローカル全撤去、入口/PC通路保護、ポスター/大型家具の境界、描画の再読込・撤去後復元、新map種別、全家具の配置寸法、日本語制御コードと色指定の独立性、全120種類の家具名の表示幅・一覧バッファ上限に加え、メニューが実際に使う配置判定、3種類の床、床への人形配置、PCへの経路、家具重複、通常秘密基地の配置条件を確認している。

テストランナーが別の既存 `secret_power.c` について表示する4件のASSUME診断はこの26件の対象外で、対象テストの失敗数には含まれていない。そのファイルや戦闘処理は変更していない。

EWRAM: 227280 -> 227348バイト（+68バイト、86.73%）。IWRAM: 28592バイトのまま。ROM: 26589364バイト（GBAファイルは32MBにパディング）。床配置修正による静的EWRAM/IWRAMの追加はない。

通常ROMをmGBA 0.10.5コアで起動し、実際のボタン入力と描画によりPCから机・ドール・マットの配置、床3種の切り替え、マップ再読込を確認済み。検証ではセーブをメモリ上へコピーし、検証専用コードでマイルームへ移動した。

**人間による、通常の移動経路での退出・flash保存・ゲーム再起動・再入室の通し確認と、2台での通信実行は未実施。** 自動テストのセーブ往復は保存構造体の直列化テストであり、flash保存後の手動再起動テストとは区別する。

## 2026-10-13 動画の表示不具合修正

提供動画 `2026-10-13 17-28-19.mp4` をデコードし、0〜34秒の画面を確認した。

### 文字化け

家具カテゴリ、所持数、家具名に共通の不要な文字列が付く原因は、参照先の欠落ではなく制御コードの破損。

`ColorMenuItemString()` は `gText_Color161Shadow161` の2番目・5番目のバイトを色番号へ書き換える。しかし日本語化により文字列の先頭へ `{JPN}` の2バイトが追加され、この書き換えが色制御コード自体を破壊していた。

文字列を `{COLOR 161}{SHADOW 161}{JPN}` の順へ変更し、旧来の色引数位置を維持した。日本語指定は残す。文字列のバイト数は不変。通常秘密基地・元の自室にも共通する家具メニューの表示修正で、家具ID・インベントリ・配置データは変更しない。

### 床・壁

- 旧「明るい木床」は、海の家のテーブルmetatile `0x218` の下地のみを読んでいた。一般民家の床 `0x224` へ変更。
- 旧「タイル床」は海の家の床模様だった。ミナモ美術館のタイル床 `0x269` と、そのmetatileが実際に使うパレット9へ変更。
- 旧「青い壁」は木壁とカーテンを参照していた。主人公宅の白壁チップを使い、同じ民家パレットに含まれる青色3段階で壁の配色を変更。
- 木目壁にカーテンは使わない。
- 素材の元metatileからパレット番号を取得し、`floor_palettes.bin` / `wall_palettes.bin` を生成。実行時もこの対応するパレットを使用する。
- 素材の上層に家具・カーテン等が含まれる場合や、複数のパレットを混用する場合は、生成時にエラーとする。
- `--assets-only` を追加。素材だけ再生成するときに、手修正したマップ配置・イベントを上書きしない。今回もこのオプションを使用し、マップ配置を維持した。

床3種×壁3種のチップ合成確認画像: `build/local_room_styles.png`。これはROMの素材と同じタイル・パレットを合成した確認画像であり、エミュレータの実操作スクリーンショットではない。

### 今回の変更ファイル

- `src/strings.c`
- `src/local_room.c`
- `tools/local_room/build_assets.py`
- `tools/local_room/check_assets.py`
- `test/local_room.c`
- `docs/local_room.md`
- `data/tilesets/secondary/local_room/floors.bin`
- `data/tilesets/secondary/local_room/walls.bin`
- `data/tilesets/secondary/local_room/floor_palettes.bin`（追加）
- `data/tilesets/secondary/local_room/wall_palettes.bin`（追加）

再生成した同一内容のチップデータ: `tiles.bin`、`metatiles.bin`、`metatile_attributes.bin`（いずれも同じ `local_room` ディレクトリ）。

生成・検証用ファイル: `pokeemerald.gba` / `.elf` / `.map`、テストELF、`build/` 配下の素材比較・動画フレーム・合成preview・検証用スクリプトと動画デコードライブラリ。

今回は `pokeemerald.sav` を変更していない。元の民家・秘密基地の素材、通常戦闘処理、通信レイアウト、保存構造も変更していない。

## 2026-10-13 床へ配置できない問題の修正

旧判定は、床の `(x=7,8 / y>=4)` と `(y=4 / x=1..8)` を家具の種類にかかわらず固定で保護していた。PCから模様替えへ入る際のカーソルは `(1,4)` にあり、この列では右側の一部しか配置候補にならなかった。

- 固定保護をPC `(1,3)`、PC操作位置 `(1,4)`、出口 `(7,12)` / `(8,12)` に限定。
- 実際の家具配置判定で、現在の床のcollision・家具object・候補家具を反映し、出口からPC操作位置へ到達できるか調べる。PCへの最後の経路を塞ぐ候補だけを拒否する。
- 判定用の一時配列はスタック上へ置く。静的EWRAMは増やさない。
- ローカル部屋の人形・クッションは普通の床へ配置可能。壁、家具objectとの重複、12sprite上限などの条件は維持。
- 保存済み人形の生成も、ローカル部屋の普通の床を許可。マップ再読込で人形が消えない。
- 保存の検証から旧固定通路の制限を外し、従来有効だった配置を維持する。
- 通常秘密基地・元の自室の人形配置条件はテストで維持を確認。

3種類の床について、PCの実処理で床を変更してから、全120種類の家具を実際の `CanPlaceDecoration()` で検証する自動テストを追加。ポスターは壁、それ以外は空き床へ配置する。

通常ROMの入力・描画確認では、旧禁止列の `(3,4)` にちいさいつくえ、`(5,4)` にピチュードールを置き、両者を残したまま床0→1→2へ切り替えた。タイル床の `(6,5)` にキラキラマットも配置し、マップ再読込後に3点が再表示されることを確認した。

確認画像（実行中のmGBAコア出力）:

- `build/local_room_qa_fixed_desk.png`
- `build/local_room_qa_fixed_doll.png`
- `build/local_room_qa_floor1.png`
- `build/local_room_qa_floor2.png`
- `build/local_room_qa_fixed_mat.png`
- `build/local_room_qa_all_furniture.png`

今回の変更ファイル:

- `include/decoration.h`
- `include/local_room.h`
- `src/decoration.c`
- `src/local_room.c`
- `src/secret_base.c`
- `test/local_room.c`
- `docs/local_room.md`

ビルド生成物: `pokeemerald.gba` / `.elf` / `.map`、テストELF、`build/` 配下の検証用コード・mGBAコア・画面画像。検証用コードは製品ROMに含まない。

今回も `pokeemerald.sav` は変更していない。

## 手動テスト用セーブ

`pokeemerald.sav` の有効な2枠（counter 36 / 35）に全120種類の家具を追加済み。

バックアップ: `pokeemerald.before-all-decorations-20261013-170220.sav`

更新前にARMコンパイラから現行セーブのオフセットを抽出し、両セーブ枠のsector ID・署名・チェックサムを検証。家具インベントリと対応するチェックサム以外の変更がないことを確認した。手持ち、通常秘密基地、元の自室、ストーリー進行、ローカル部屋データ、セーブ末尾16バイトは保持している。

家具はバッグの通常アイテムではなく、模様替えPCの家具インベントリに入っている。

最新ROMを通常起動してセーブから続ける。以前のステートセーブをロードすると、追加前のインベントリが復元される場合がある。

## 手動チェックリスト

1. キャンプ南のミニミニべっそうへ進み、左上の建物へ入ってPCまで歩けること。
2. PCから机、植物、マット、ポスターを配置する。人形・クッションは普通の床または机・マット上へ置く。
3. 大型家具で出入口やPC操作位置、PCへの最後の経路を塞ぐと拒否されること。それ以外の旧通路部分には配置できること。
4. 片付けた家具を同じ編集セッションで再配置できること。ゴーストobject・metatileが残らないこと。
5. 床3種類・壁3種類をそれぞれ切り替え、家具とポスターの背景も変わること。
6. 徘徊0〜3匹を切り替える。編集時は消え、終了時に戻ること。家具で床が埋まった場合は無理に生成しないこと。
7. 退出・再入室で家具と床・壁・徘徊設定が維持されること。
8. 通常のレポートを書き、ゲームを再起動して再入室しても維持されること。
9. 通常秘密基地を作成・模様替えし、元の自室とローカル部屋の配置が別々に保たれること。
10. 通常秘密基地を撤去してもローカル部屋が残り、ローカルを全撤去しても通常秘密基地が残ること。
11. 通信相手の基地一覧・registry・record mixingへローカル部屋が現れないこと。

## 全変更ファイル

既存ファイル:

- `data/event_scripts.s`
- `data/layouts/layouts.json`
- `data/maps/MiniMiniVilla/map.json`
- `data/maps/map_groups.json`
- `data/specials.inc`
- `include/constants/field_tasks.h`
- `include/constants/script_menu.h`
- `include/decoration.h`
- `include/global.h`
- `include/secret_base.h`
- `src/data/region_map/region_map_sections.json`
- `src/data/script_menu.h`
- `src/data/tilesets/graphics.h`
- `src/data/tilesets/headers.h`
- `src/data/tilesets/metatiles.h`
- `src/decoration.c`
- `src/field_control_avatar.c`
- `src/field_tasks.c`
- `src/party_menu.c`
- `src/secret_base.c`
- `src/strings.c`
- `src/tilesets.c`

追加ファイル:

- `data/layouts/MiniMiniVilla_LocalRoom/border.bin`
- `data/layouts/MiniMiniVilla_LocalRoom/map.bin`
- `data/maps/MiniMiniVilla_LocalRoom/map.json`
- `data/maps/MiniMiniVilla_LocalRoom/scripts.inc`
- `data/tilesets/secondary/local_room/floors.bin`
- `data/tilesets/secondary/local_room/floor_palettes.bin`
- `data/tilesets/secondary/local_room/metatile_attributes.bin`
- `data/tilesets/secondary/local_room/metatiles.bin`
- `data/tilesets/secondary/local_room/tiles.bin`
- `data/tilesets/secondary/local_room/walls.bin`
- `data/tilesets/secondary/local_room/wall_palettes.bin`
- `include/constants/local_room.h`
- `include/local_room.h`
- `src/local_room.c`
- `test/local_room.c`
- `tools/local_room/build_assets.py`
- `tools/local_room/check_assets.py`
- `tools/local_room/grant_decorations.py`
- `tools/local_room/save_layout.c`
- `docs/local_room.md`

生成・更新した実行用データ:

- `pokeemerald.gba` / `pokeemerald.elf` / `pokeemerald.map`
- `pokeemerald.sav` と上記バックアップ
- 新マップの `connections.inc` / `events.inc` / `header.inc`、map/layout/constants等の通常ビルド生成ファイル
- `build/` 配下のテスト・レイアウト確認・チップ参照・室内previewデータ

`build_assets.py` は新マップと専用バイナリの再生成ツール。室内をPorymapで手修正した後、素材だけ更新する場合は `--assets-only` を必ず付ける。指定なしでは生成元の長方形へ戻る。通常のmakeからは実行しない。
