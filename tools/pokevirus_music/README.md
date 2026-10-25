# 解放戦BGM「動き出す記憶」

英題: The Heart Remembers。曲IDは `MUS_POKEVIRUS_RELEASE` (610)、曲シンボルは `mus_pokevirus_release`。

## 構成

- 冒頭約0.75秒は休符。止まっていた心音が小さい二段の鼓動として戻り、間隔を詰め、音を強める。
- 約5秒の導入で80 -> 108 -> 132 BPMと加速し、本編は144 BPM。心音は本編でも控えめに残る。
- C minorを中心としたオリジナルの旋律、低音、細かいピアノ、弦、軽い打楽器で戦闘へ移行する。
- ループの9～10小節目、19～20小節目、29～30小節目に既存 `mus_vs_trainer.mid` の短い回想を挟む。ブラスの C-G-F / E-F-E-D-C-E-Bb-C と、ピアノの半音下降の断片。曲全体は流用せず、回想が浮かんではオリジナルの曲調へ戻る。
- 本編32小節、約53.33秒でループ。導入・遅い心音はループに含めず、繰り返すたびに無音から始めない。初回は導入込みで約58秒。

## ゲームへの接続

`DoTrainerBattle` が `B_TRANSITION_POKEVIRUS_RELEASE` を選んだ場合のみ専用曲を渡す。前回の `trainerbattle_pokevirus` に加え、別の方法で戦闘を準備してから `SetPokevirusReleaseTransition` を使う場合も同じ選曲になる。

通常戦の選曲、フロンティアの固定・ランダム選曲は維持する。専用曲を明示的に渡した場合だけ、フロンティアの選曲設定に差し替えさせない。心音を覆わないよう、専用の鎖演出に入れていた `SE_M_BIND` は外した。

既存曲・SE・吟遊詩人の音素IDは変更せず、曲テーブルの末尾に610を追加した。既存の `START_MUS` / `END_MUS` は原作曲の範囲のまま。BGMショップへの追加や、セーブ・通信形式の変更は行わない。

## 素材・再生成

```sh
python3 tools/pokevirus_music/generate.py
python3 -m unittest discover -s tools/pokevirus_music -p test_generate.py -v
make -j$(nproc)
```

追加のPythonパッケージは不要。

- `sound/songs/midi/mus_pokevirus_release.mid`: 編集可能な8トラックの楽譜。
- `sound/voicegroups/pokevirus_release.inc`: 専用音色表。既存の低音・弦・ブラス・ピアノ・打楽器等を再利用。
- `sound/direct_sound_samples/custom/pokevirus_heartbeat.wav`: 13379 Hz、モノラルのオリジナル心音。減衰する低音と少量の濾波ノイズを合成した0.18秒の非ループ波形。
- `score.json`: テンポ、ループ、回想位置、楽譜ハッシュ。

MIDIと心音WAVは `generate.py` が生成する。ゲーム内では既存の `mid2agb` / `wav2agb` が変換するため、生成される `.s` / `.bin` を手編集しない。音色番号は専用の音色表用なので、一般的なGM音源でMIDIだけを再生してもゲーム内の音は再現されない。

## 試聴・検証

- 通常ビルド成功: `build/qa_pokevirus_music_20261026/final_build.log`。
- Python生成テスト5/5、専用曲のROMテスト4/4、既存フロンティアBGMテスト18/18、鎖演出テスト8/8成功。
- 指定の `mGBA-celio-rom128-4/mgba-celio.exe` で70秒超の連続再生を実行。ループ後も8トラックが有効で、曲ヘッダーが維持されることを確認した。
- 試聴ファイル: `build/qa_pokevirus_music_20261026/pokevirus_release_preview.wav` (68秒、22050 Hzステレオ)。mGBAで実際のGBAサウンドドライバの出力を録音し、前後の余白を切り出したもの。試聴用の末尾だけ0.7秒でフェードする。ゲーム本体のループにはフェードを入れていない。
- 全体ビルドが別作業中の未作成イベントで一時停止したため、試聴録音は既存ROMコピーの空き領域に同じ曲データを載せた独立確認用環境で行った。その後、全体ビルド済みROMでも解放戦を実際に開始し、鎖演出から技選択画面まで専用BGMが選択・維持されることを確認した。ログは `build/qa_pokevirus_release_20261026/runtime.log`。
- 戦闘確認したROMのSHA-256: `e7e8a6b855d703dc1512f03b243f0336c191f4cd24928750bd6ee0941453fc6b`。コピーROMと一致。元セーブは変更していない。実機の音量バランス・聴感の評価は未実施。
