# 定食屋の店主専用スプライト

黒髪ショート、三白眼、白い割烹着、胸元の赤いR、黒いインナーとズボンの成人男性。既存のぼっちゃん画像そのものは変更しない。

## ゲーム用素材

| ファイル | 仕様 |
| --- | --- |
| `front.png` | 戦闘正面立ち絵、64x64 |
| `back.png` | マルチ相方用背面、64x256、64x64の4コマを縦並び |
| `overworld.png` | SD歩行、144x32、16x32の9コマを横並び |
| `overworld_front.png` | SD正面静止、16x32、確認用 |
| `palette.pal` | 共通16色、JASC-PAL |
| `front_preview_8x.png` | 正面の整数8倍プレビュー |
| `back_preview_4x.png` | 背面4コマの整数4倍プレビュー |
| `overworld_preview_8x.png` | 歩行9コマの整数8倍プレビュー |

ゲーム用PNGは4bit indexed、16色以内、透過インデックス0、アルファは0または255。パレットはGBAの色精度に合わせてRGB各成分を8刻みにした。SD正面のRは縮小で潰れないよう、実寸で3x4ドットの文字として保持している。

コマ順は既存の `sAnimTable_Standard` と一致する: 正面静止、背面静止、左静止、正面歩行2コマ、背面歩行2コマ、左歩行2コマ。右向きは既存の左右反転を使用する。

## 組み込み

- `OBJ_EVENT_GFX_ROCKET_COOK`: 店主専用SD。
- `OBJ_EVENT_PAL_TAG_ROCKET_COOK`: 店主専用パレット。他NPCのパレットを置換しない。
- `TRAINER_PIC_ROCKET_COOK`: 正面立ち絵とマルチ相方用背面。既存トレーナーの画像IDは並べ替えていない。
- `Party_eat_time` の店主、`ultra_frontie_hub` と `PartyRoamerPark` の導入イベント用店主にSDを適用。

背面は既存のHoenn形式に合わせて4コマ・1コマ2048バイトで登録した。コマ0が投球準備、1が振りかぶり、2が投球、3が待機。既存の `sBackAnims_Hoenn` を使い、待機・投球・指示の各アニメーションから呼べる。正面と同じ16色パレットを使用し、背中には胸元のRを描かない。実データが4コマ分であることは `STATIC_ASSERT` でも検証する。

相方データの `trainerPic` に `TRAINER_PIC_ROCKET_COOK` を指定すれば、既存のカスタム相方用描画経路でこの背面を使用できる。店主を選ぶ会話、専用パーティ、対戦開始イベントを新設する変更ではない。通常トレーナーや通信プレイヤーの画像選択処理も変更していない。

## 生成と形式変換

画像生成には内蔵 `image_gen` を使用。採用した原画は `source/` に残した。原画はそのままではゲーム用寸法・パレットではない。

`prepare_assets.mjs` の `prepareAssets(PNG, directory)` に `pngjs` の `PNG` とこのフォルダの絶対パスを渡すと、原画から最近傍縮小・16色化・固定寸法化したPNGを再生成する。ゲーム用に新しい画像ライブラリ依存を加えていない。

生成指示の要点:

> Adult Japanese male diner proprietor; short black hair, sanpaku eyes with small high pupils and lower whites, deadpan expression; white long-sleeved Japanese kappogi with gathered cuffs, red capital R on chest, black undershirt/trousers/shoes; no hat or beard. Generation III GBA trainer sprite proportions, dark one-pixel outlines, economical flat pixel clusters, no gradients/antialiasing, transparent background, at most 15 opaque colors. Trainer front: 64x64 logical canvas, full body three-quarter front, one hand on hip and one holding a ball. Overworld: 16x32 per frame, large-head SD proportions, nine frames in south/north/west idle and walking order, consistent scale, no R on back.

採用原画の生成プロンプト全文は `source/prompts.md` に保存した。

寸法・色数・透明度の検査結果は `validation.json` に記録する。製品側の素材変換は既存の `INCGFX_U32` / `INCGFX_U16` に任せている。

## 検証

- 通常 `make -j$(nproc)` 成功。
- `make -j$(nproc) check TESTS=test/rocket_cook.c`: 背面追加後は4/4成功。既存素材との独立性、4コマ登録、共有パレット、アニメーション参照、コピー先前後の破壊がないことを検証。
- decomps内の `mGBA-celio-rom128-4/mgba-celio.exe` でコピーROM・QAセーブのみを起動。3マップのSD/専用パレット、8種類の向き・歩行アニメーション、正面立ち絵の作成・展開・破棄を確認。
- 定食屋で方向入力により店主の前へ移動し、Aボタンで会話が開始することを確認。画像は `build/qa_rocket_cook_20261025/diner_talk.png`。
- `build/qa_rocket_cook_20261025/` にLua・一時EWRAMフック・ログ・画像を記録。導入シーンの隠しNPCはQA側で一時的に表示して素材を検証しており、製品側の表示条件は変更していない。
- 正面/SD追加時のROM SHA-256: `bd8b67b9fbeb340e8e6935cc2d6f2152e134301ea8bfd987eb6f4d4c06f9bfc3`。背面追加後のものとは区別する。
- 実際のマルチ戦は未実行。背面素材の登録は、店主の専用パーティ・相方選択イベントの追加を意味しない。
- 背面追加後のROM SHA-256: `05311baf1f91db6cdd5fe385050ed83491604781ca3d9f35adaee18eb2c34c50`。`build/qa_rocket_cook_back_20261025/cook_back_qa.gba` と一致。
- 同じdecomps内mGBAで、コピーセーブを使用して `CreateTrainerPicSprite` の背面コピー・描画、待機、投球、指示、スプライト解放を確認。投球と指示は終端まで進み、各投球コマを画像で確認した。ログ・画像は `build/qa_rocket_cook_back_20261025/`。この表示試験も一時EWRAMフックを用いたもので、マルチ戦の通しプレイではない。

再ビルド時に、マップ削減後の受付から削除済みの `LOCALID_FRONTIER_RECEPTION_SCOTT` を参照する旧演出がリンクエラーになった。NPCは復活させず、`BattleFrontier_ReceptionGate/scripts.inc` のパス交付後のエニシダ登場部分だけ削除した。パス交付と通常の施設案内は維持している。
