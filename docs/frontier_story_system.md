# フロンティアの物語進行（技術メモ）

物語の内容・台詞は扱わない。保存領域・データ・処理の差し込み箇所だけをまとめる。

## 保存領域

既存セーブで0の未使用領域だけを使う。SaveBlock・通信データの配置は変更しない。

| 名前 | 実体 | 内容 |
| --- | --- | --- |
| `VAR_TOWER_STORY_STATE` | `VAR_UNUSED_0x40FA` | タワー編の段階（`TOWER_STORY_*`） |
| `FLAG_STORY_DARKRAI_JOINED` ほか2つ | `FLAG_UNUSED_0x020`〜`0x022` | 全シナリオ共通の進行 |
| `FLAG_STORY_RESCUED_TOWER`〜`CONTEST` | `FLAG_UNUSED_0x023`〜`0x02A` | 8シナリオの完了 |
| `FLAG_STORY_INN_OPEN`、`FLAG_STORY_INN_VISITED` | `FLAG_UNUSED_0x02B`、`0x02C` | 旅館への船の開通、初回の到着 |
| `FLAG_STORY_NAMED_STATES_START`〜`END` | `FLAG_UNUSED_0x493`〜`0x4D4` | ブレーン7人・7戦目ゲスト26組の状態。1人2フラグ（`NAMED_STORY_*`） |

`VAR_FRONTIER_INTRO_STATE` が `FRONTIER_INTRO_COMPLETE` でないセーブでは、ブレーン・ゲストの台詞と状態は従来のまま。

## データ

- トレーナー: `TRAINER_KAZUKI_RELEASE`（855）、`TRAINER_POKEVIRUS`（856）、`TRAINER_KAZUKI_RESORT`（857）を末尾に追加。`TRAINERS_COUNT` は858、残りの空きは6。
- 相方: `PARTNER_ROCKET_COOK`（2）。
- トレーナークラス: `TRAINER_CLASS_NO_TITLE`（名前が空）。戦闘メッセージの「肩書き＋名前」は、肩書きが空なら空白を入れない。
- 乱数タグ: `RNG_FRONTIER_RELEASE_BOOST`。
- 戦闘文: `STRINGID_FRONTIERRELEASEBOOST`、`BattleScript_FrontierReleaseBoost`。

## 処理（`src/frontier_story.c`）

- `trainer_util.c` の `GenerateMonFromTrainerMon`: 物語戦の間だけ、相手と相方のレベルを主人公の手持ちの最高レベル（最低50）にする。
- `battle_script_commands.c` の `UpdateSentMonFlags`: ひんしの後の交代かどうかを記録する。
- `battle_switch_in.c` の `SWITCH_IN_EVENTS_RELEASE_BOOST`: 解放戦の相手側だけ、戦闘開始時とひんし後の交代時に能力を1つ2段階上げる。
- `battle_main.c` の `HandleEndTurn_FinishBattle`: フロンティア戦の勝利でブレーン・ゲストの状態を進め、物語戦の一時状態を消す。通信・バトルビデオは対象外。
- `frontier_util.c` の `CopyFrontierBrainText`、`frontier_guest.c` の `CopyFrontierGuestText`: 状態に応じて戦闘中の台詞を差し替える。
- 7施設のブレーン戦の部屋: `FrontierStory_BufferBrainIntro` で戦闘前の場面を差し替え、既存の戦闘処理へ合流する。
- `battle_setup.c` の `SetPokevirusReleaseTransition`: マルチバトルから解放戦の画面切り替えを使う。
- ポケモン生成（`frontier_creator.c`）は常にLv100。
- `FrontierStory_ShouldSuggestInn`: 治療法の判明後、治った人数（8シナリオの完了＋救出済みのブレーン・ゲスト）が2人以上で、旅館が未開通なら真。キャンプでの治療の直後と、ハブのダークライとの会話で使う。
- 旅館への船: 西エリアの港の係員（`BattleFrontier_OutsideWest`）が `FLAG_STORY_INN_OPEN` で行き先を出し、`RestorationInn_Outside` の桟橋へ移動する。帰りは桟橋の船員から西エリアの港（19,67）へ戻る。港の南の桟橋が船の東側にあるため、フロンティアからの出港は西向き。乗船前に連れ歩きのポケモンをボールへ戻す。

スクリプトは `data/scripts/frontier_story.inc`。タワーのロビー・ハブ・キャンプ・西エリアの港の `scripts.inc` と、旅館の桟橋の船員から呼ぶ。

## 検査

```sh
make -j4 --output-sync=target check TESTS='Frontier story'
python3 tools/check_jp_text.py
python3 tools/link_compat/check_fingerprint.py
```
