# BPEJサウンド抽出

このプロジェクトでは、公式ROM由来の音データをリポジトリに直接含めない方向へ段階移行します。

現時点の実装は小さい検証用manifestです。いきなり全音源を置き換えず、BPEJ由来と確認できた一部のBGM/SE/DirectSound sampleだけを、ビルド時に `baserom.gba` から抽出します。

## 入力と出力

- 入力ROM: プロジェクトルートの `baserom.gba`
- ROM条件: 日本版エメラルド `BPEJ`
- manifest: `tools/data/bpej_sound_manifest.json`
- 抽出先: `build/extracted_sound/`
- 成功stamp: `build/generated/bpej_sound_extracted.ok`

`build/extracted_sound/` と `build/generated/` はgit管理しません。

## 現在の抽出対象

`tools/data/bpej_sound_manifest.json` には、各項目ごとに以下を明示しています。

- 抽出元ROM offset
- size
- 出力path
- 既存ラベル名
- SHA1
- BPEJ由来と判断した根拠

初期manifestに入れているのは以下だけです。

- `mus_dummy`
- `mus_level_up`
- `se_use_item`
- `DirectSoundWaveData_sc88pro_glockenspiel`
- `DirectSoundWaveData_bicycle_bell`

根拠は、同階層の `../pokeemerald-jp` にある日本版エメラルド逆アセンブル資産と、BPEJ ROM内の実データを参照したものです。JP逆アセンブル側は現在、Expansionのような分割済み音ラベルを保持していないため、manifestでは「BPEJ ROM offset」と「現行Expansion側の同名音源byte列との一致」を併用して確認しています。

## BPEJ対象外の音源

Expansion追加曲、FRLG由来曲、第4世代以降cryなど、BPEJ由来と確定できない音源はmanifestに入れません。

これらは当面、既存の `sound/` 配下のデータを使います。削除対象にするのは、manifestでBPEJ由来と確認でき、抽出ビルドが安定したものだけです。

## ビルド時の動き

`make` の初期段階で以下を実行します。

```sh
python3 tools/check_baserom_jp.py --baserom baserom.gba --output build/generated/bpej_verified.ok
python3 tools/extract_bpej_sound.py --baserom baserom.gba --manifest tools/data/bpej_sound_manifest.json --out build/extracted_sound --stamp build/generated/bpej_sound_extracted.ok
```

抽出に失敗した場合、どの抽出ファイルが不足しているかを表示して停止します。

## BGM wrapper

BGM/SEの `.s` wrapper は `build/extracted_sound/sound/songs/midi/` に生成します。

wrapperの外部公開ラベルは既存と完全一致します。例えば `mus_level_up` は生成wrapper側でも `mus_level_up` として定義されます。差し替え対象は `audio_rules.mk` の明示リスト `BPEJ_EXTRACTED_MID_NAMES` にある曲だけです。

include path全体の優先順位は変えていません。BPEJ対象外の既存ファイルが誤って置き換わらないように、対象曲だけ個別ルールで抽出wrapperをassembleします。

## DirectSound sample

DirectSound sampleは、manifest対象だけ `sound/direct_sound_data.inc` の `.incbin` pathを `build/extracted_sound/...` へ変更します。

対象外sampleの `.incbin` は既存 `sound/direct_sound_samples/...` のままです。

## 対象を増やす手順

1. BPEJ ROM内で対象byte列のoffsetとsizeを確認します。
2. 同名Expansion音源と一致するか確認します。
3. `tools/data/bpej_sound_manifest.json` にoffset、size、出力path、ラベル名、SHA1、根拠を追加します。
4. BGM/SEの場合は `BPEJ_EXTRACTED_MID_NAMES` にラベルを追加します。
5. DirectSound sampleの場合は対象 `.incbin` だけ抽出先へ変更します。
6. `make -j4 --output-sync=target` で確認します。
