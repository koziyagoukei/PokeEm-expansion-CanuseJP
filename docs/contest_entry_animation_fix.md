# コンテスト1次審査クラッシュの修正

> 以下は `frontier-hack` の `1311d01582` から取り込んだ修正履歴です。
> 記載の検証件数・通信互換番号・メモリ使用量は移植元での記録であり、
> このブランチでの今回の検証結果ではありません。
> 今回の移植範囲と確認結果は [統合記録](contest_frontier_integration.md) を参照してください。
> `{AUTO}` の終端処理はmasterにも既存のため、今回 `src/string_util.c` は変更していません。

## 主原因

紹介文へ埋め込む名前は `Contest_CopyDisplayName()` が末尾に `{AUTO}` を付ける。
表示処理は対応していたが、`StringExpandPlaceholders()` はこの制御コードを
「引数1バイト付き」として処理していた。そのため、直後の文字列終端 `EOS` を
引数として読み飛ばし、名前の後ろのメモリまで展開・書き込み続ける場合があった。

`src/string_util.c` で `{AUTO}` を引数なしとして扱い、制御コード長の表にも登録した。
名前やメッセージの内容は変更していない。レックウザ固有の画像データの差し替えでもない。

以前の「受付時に未確保のコンテスト用メモリへアクセスする問題」は別件。
その修正は現行コードにも残っている。受付時は `gContestMons` に保存し、
アピール開始時の `AllocContestResources()` の後でギミック状態を初期化する。
未確保状態での受付テストをレックウザでも実行し、正常なことを確認した。

## 併せて修正した表示処理

- 画像を閉じる際、対象スプライトに属する遅延・実行中アニメーションタスクを停止する。
  レックウザには60フレームの開始待機があり、早く閉じると破棄済みスプライトを
  後から操作するタスクが残っていた。
- `DestroySprite()` が参照するフレーム画像を、スプライト破棄より先に解放しない。
- 表示開始時のアニメーション用callbackを、直後に待機callbackで上書きしない。
- ウィンドウ生成前の終了、重複終了、スプライト生成失敗を安全に扱う。
- 通常戦闘外の前面アニメーション終了時、未確保の `gBattleStruct` に触れない。

アニメーション中止APIは今回の1次審査の終了処理からのみ使用する。
通常戦闘の技アニメ、コンテストcutaway、得点・妨害・コンボ計算は変更していない。
共通文字列処理の変更は `{AUTO}` の正しい解釈を追加するもの。

## 検証

- 修正前：終端読み飛ばしの単体テスト2件が失敗することを確認。
- 修正前ROMと実セーブのコピー：1次審査の紹介メッセージ開始時にメモリ破損・進行停止を確認。
  動画のエラーダイアログと同一のジャンプ先アドレスまでは照合していない。
- 修正後：コンテスト関連53テスト成功。受付、文字列展開、遅延アニメの停止、
  破棄順、コンボ、NPC、ギミック、報酬等を含む。
- 修正後ROMと実セーブのコピー：保存済みの色違いレックウザを使用し、
  かっこよさのノーマル・スーパー双方で4体の紹介を完了、2次審査へ遷移。
  1次審査中の `gContestResources` は未確保のままで正常に進行した。
- `make -j4 --output-sync=target` 成功。
- Pythonテスト98件成功（WSL）。Windows実行は一時ファイル関連のエラーで中止した。
- 日本語テキスト検査、コンボ一覧再生成チェック、通信fingerprint、`git diff --check` 成功。
- EWRAMは227,532バイトで増加なし。通信互換番号は12、データレイアウトは6で変更なし。

セーブは `build/contest_crash_user_save_20261022.sav` にコピーし、
mGBAの `loadTemporarySave` で検証した。元の `pokeemerald.sav` には書き込んでいない。
検証前後の元セーブとコピーのSHA-256は以下で一致した。

`8819fe64da9c7e89895fc5abfd61fd6fc2d5ae379ee7daa41b6c579a9cb5bb5c`

この再現確認は1次審査から2次審査への遷移まで。
全種族・全ランク・全カテゴリの通しプレイと2台通信は網羅していない。

## 今回変更したソース・資料の全一覧

コンビネーション追加の変更も含む。

- `include/constants/contest.h`
- `include/contest_util.h`
- `include/pokemon.h`
- `include/pokemon_animation.h`
- `src/contest_util.c`
- `src/data/moves_info.h`
- `src/pokemon.c`
- `src/pokemon_animation.c`
- `src/string_util.c`
- `test/contest_npc.c`
- `test/contest_entry.c`
- `test/contest_combo_examples.inc`
- `tools/export_contest_combos.py`
- `docs/contest_combinations.md`
- `docs/contest_entry_animation_fix.md`

生成物は `pokeemerald.gba`、ELF/MAPおよびテストROM。
再現用コード・ログ・動画抽出画像・解析ツールはGit管理外の `build/` 配下に置いた。
元の動画・画像・セーブは変更していない。
