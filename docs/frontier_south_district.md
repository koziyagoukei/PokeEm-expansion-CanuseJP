# フロンティア南広場・初版

## 配置と移動

- 新マップ `BattleFrontier_OutsideSouth` は56×42マス。北は既存西エリア、東はリゾートへ道路接続。
- 西エリアの桟橋を南へ8マス延長。既存の施設・門・イベント座標は移動しない。
- ハブは広場の北西、コンテスト会場は北東、美術館は南東。
- 外観はミナモのモーテル・コンテスト会場・美術館の既存メタタイルを使用。
- リゾートから南の別荘・ショップ・マイルームへの既存経路は維持。
- 7施設の退館先は、それぞれの屋外入口前。受付・連戦・リタイア・手持ち復元のスクリプトは変更しない。
- ハブの既存コンテスト用出入口は南広場へ接続。通信室への出入口と各種サービスNPCは維持。
- 手持ちSELECTでのリゾート移動は禁止条件を維持。元の地点は記録せず、キャンプ北の出口は南広場へ固定。
- フィールドの「そらをとぶ」は使用不可。戦闘中の技は変更しない。船の係員から島外へは出られない。
- ハブ・南広場で全滅時の復帰先をフロンティアのポケモンセンターへ設定。

## 保存・通信

新マップとレイアウトは登録リストの末尾に追加。既存IDをずらさない。
コンテスト・美術館の室内ID、階段、審査・絵画データ、セーブ構造、通信パケットは維持する。
セーブファイルの編集・上書きは行わない。

異なるセカンダリタイルセットの接続では、境界から7マスの表示範囲を共通のプライマリタイルだけで構成する。
新地区北端・東端、西エリア南端、リゾート西端を静的検査する。

## 検査・生成

```sh
python3 tools/frontier_district/build_map.py --check
python3 tools/frontier_district/test_maps.py -v
python3 tools/check_jp_text.py
python3 tools/link_compat/check_fingerprint.py
make -j4 --output-sync=target
```

配置画像を生成する場合はPillowが必要。

```sh
python3 tools/frontier_district/build_map.py --render BattleFrontier_OutsideSouth --output build/frontier_south.png
```

`build_map.py` を引数なしで実行すると、南広場全体と既存2マップの接続部分を再生成する。
Porymap等で配置を手修正した後は、意図せず再生成しないこと。通常のmakeでは実行されない。

## 手動確認

1. 既存セーブからハブを出て、南広場へ移動する。入口2マスと旧コンテスト側の出口も確認。
2. 北へ歩き、従来の受付ゲートを通って西・東エリアへ移動。7施設に入退館する。
3. 通常挑戦・連戦・敗北・リタイア後に受付へ戻り、退館すると各施設前へ出ることを確認。
4. 南広場からコンテストに参加・終了し、退館すると会場前へ戻ることを確認。リンクコンテストも別途確認。
5. 美術館の両入口・2Fの絵画・風呂場の既存展示を確認。
6. 広場東端とキャンプ西端を徒歩で往復し、別荘・ショップ・マイルームも確認。
7. SELECTでキャンプへ移動後、北中央の出口から南広場へ戻る。元の地点へ戻らないことを確認。
8. 施設挑戦中・リゾート・別荘等では、従来どおりSELECT移動ができないことを確認。
9. 接続境界でセーブ・再起動し、描画・当たり判定・帰還先を確認。
10. フィールドで「そらをとぶ」が使えず、船が運休中であることを確認。

静的検査・配置画像・ビルドは、エミュレータによる上記プレイ確認の代わりにはならない。

## 初版の検査結果（2026-10-24）

- 静的テスト9件通過、日本語文字検査通過、`git diff --check` 通過。
- 変更前HEADと照合し、既存マップID・レイアウトIDを維持していることを確認。
- 通信fingerprintは変更なし（compat 12、data 6）。
- WSLの `make -j4 --output-sync=target` 通過、`pokeemerald.gba` を生成。
- EWRAM 227,532B、IWRAM 28,592B。追加の常駐画面保存バッファは設けていない。
- エミュレータの歩行・施設挑戦・通信プレイは未実施。セーブファイルは未変更。

## 変更ファイル一覧

- `data/event_scripts.s`
- `data/layouts/layouts.json`
- `data/layouts/BattleFrontier_OutsideSouth/map.bin`
- `data/layouts/BattleFrontier_OutsideSouth/border.bin`
- `data/layouts/BattleFrontier_OutsideWest/map.bin`
- `data/layouts/PartyRoamerPark/map.bin`
- `data/maps/map_groups.json`
- `data/maps/BattleFrontier_OutsideSouth/map.json`
- `data/maps/BattleFrontier_OutsideSouth/scripts.inc`
- `data/maps/BattleFrontier_OutsideWest/map.json`
- `data/maps/BattleFrontier_OutsideWest/scripts.inc`
- `data/maps/BattleFrontier_BattleTowerLobby/map.json`
- `data/maps/BattleFrontier_BattleDomeLobby/map.json`
- `data/maps/BattleFrontier_BattleFactoryLobby/map.json`
- `data/maps/BattleFrontier_BattlePalaceLobby/map.json`
- `data/maps/BattleFrontier_BattleArenaLobby/map.json`
- `data/maps/BattleFrontier_BattlePikeLobby/map.json`
- `data/maps/BattleFrontier_BattlePyramidLobby/map.json`
- `data/maps/ultra_frontie_hub/map.json`
- `data/maps/ultra_frontie_hub/scripts.inc`
- `data/maps/LilycoveCity_ContestLobby/map.json`
- `data/maps/LilycoveCity_ContestHall/map.json`
- `data/maps/LilycoveCity_LilycoveMuseum_1F/map.json`
- `data/maps/LilycoveCity_LilycoveMuseum_2F/map.json`
- `data/maps/PartyRoamerPark/map.json`
- `src/party_menu.c`
- `src/party_roamer_park.c`
- `src/data/resort_tips.h`
- `tools/frontier_district/build_map.py`
- `tools/frontier_district/test_maps.py`
- `docs/frontier_south_district.md`
