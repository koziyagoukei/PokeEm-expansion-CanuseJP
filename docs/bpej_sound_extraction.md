# BPEJサウンド抽出

このプロジェクトでは、公式ROM由来の音データをリポジトリへ直接含めない構成へ段階的に移行します。

現時点では検証用の小さいmanifestだけを使い、BPEJ由来と確認できた一部のBGM/SE wrapperとDirectSound sampleのみを、ビルド時に `baserom.gba` から抽出します。

## 入力と出力

- 入力ROM: プロジェクトルートの `baserom.gba`
- ROM条件: 日本版エメラルド `BPEJ`
- manifest: `tools/data/bpej_sound_manifest.json`
- 抽出先: `build/extracted_sound/`
- 成功stamp: `build/generated/bpej_sound_extracted.ok`

`build/extracted_sound/` と `build/generated/` はgit管理しません。

## 現在の抽出対象

`tools/data/bpej_sound_manifest.json` には、各項目ごとに以下を明示しています。

- `rom_offset`
- `size`
- `output_path`
- `existing_file`
- `byte_match`
- SHA1
- BPEJ由来と判断した根拠

現在のBGM/SE wrapper対象は以下です。

- `mus_dummy`
- `mus_level_up`
- `se_use_item`
- `se_pc_login`
- `se_pc_off`
- `se_pc_on`
- `se_select`
- `se_win_open`
- `se_wall_hit`
- `se_door`
- `se_exit`
- `se_bike_bell`
- `se_flee`
- `se_not_effective`
- `se_effective`
- `se_super_effective`
- `se_ball_open`
- `se_faint`
- `se_sliding_door`
- `se_ship`
- `se_bang`
- `se_pin`
- `se_boo`
- `se_ball`
- `se_a`
- `se_i`
- `se_u`
- `se_e`
- `se_o`
- `se_n`
- `se_success`
- `se_failure`
- `se_bike_hop`
- `se_switch`
- `se_click`
- `se_fu_zaku`
- `se_contest_condition_lose`
- `se_lavaridge_fall_warp`
- `se_ice_stairs`
- `se_ice_break`
- `se_ice_crack`
- `se_fall`
- `se_unlock`
- `se_warp_in`
- `se_warp_out`
- `se_rotating_gate`
- `se_truck_stop`
- `se_truck_unload`
- `se_truck_door`
- `se_save`
- `se_ball_bounce_1`
- `se_ball_bounce_2`
- `se_ball_bounce_3`
- `se_ball_bounce_4`
- `se_ball_trade`
- `se_ball_throw`
- `se_note_c`
- `se_note_d`
- `se_note_e`
- `se_note_f`
- `se_note_g`
- `se_note_a`
- `se_note_b`
- `se_note_c_high`
- `se_puddle`
- `se_bridge_walk`
- `se_itemfinder`
- `se_ding_dong`

現在のDirectSound sample対象は以下です。

- `DirectSoundWaveData_sc88pro_glockenspiel`
- `DirectSoundWaveData_sc88pro_organ2`
- `DirectSoundWaveData_sc88pro_fretless_bass`
- `DirectSoundWaveData_sc88pro_slap_bass`
- `DirectSoundWaveData_bicycle_bell`
- `DirectSoundWaveData_sc88pro_synth_bass`
- `DirectSoundWaveData_sc88pro_timpani`
- `DirectSoundWaveData_classical_choir_voice_ahhs`
- `DirectSoundWaveData_sd90_classical_oboe`
- `DirectSoundWaveData_ethnic_flavours_ohtsuzumi`
- `DirectSoundWaveData_dance_drums_ride_bell`
- `DirectSoundWaveData_drum_and_percussion_kick`
- `DirectSoundWaveData_ethnic_flavours_atarigane`
- `DirectSoundWaveData_ethnic_flavours_hyoushigi`
- `DirectSoundWaveData_ethnic_flavours_kotsuzumi`
- `DirectSoundWaveData_heart_of_asia_gamelan`
- `DirectSoundWaveData_register_noise`
- `DirectSoundWaveData_sc88pro_bubbles`
- `DirectSoundWaveData_sc88pro_church_organ3_high`
- `DirectSoundWaveData_sc88pro_church_organ3_low`

根拠は、同階層の `../pokeemerald-jp` にある日本版エメラルド逆アセンブル資産と、BPEJ ROM内の実データを参照したものです。JP逆アセンブル側は現在、Expansionのような分割済み音ラベルを保持していないため、manifestでは「BPEJ ROM offset」と「現行Expansion側の同名音源byte列との一致」を併用して確認しています。

## BPEJ対象外の音源

以下はmanifestへ入れません。

- cry
- FRLG曲
- Expansion追加曲
- 第4世代以降由来など、BPEJ由来と確定できない音源
- BPEJ byte列と一致確認できていない音源

これらは当面、既存の `sound/` 配下のデータを使います。削除対象にするのは、manifestでBPEJ由来と確認でき、抽出ビルドが安定したものだけです。

## ビルド時の動き

`make` の初期段階で以下を実行します。

```sh
python3 tools/check_baserom_jp.py --baserom baserom.gba --output build/generated/bpej_verified.ok
python3 tools/extract_bpej_sound.py --baserom baserom.gba --manifest tools/data/bpej_sound_manifest.json --out build/extracted_sound --stamp build/generated/bpej_sound_extracted.ok
```

抽出に失敗した場合、どの抽出ファイルが不足しているかを表示して停止します。

## BGM/SE wrapper

BGM/SEの `.s` wrapper は `build/extracted_sound/sound/songs/midi/` に生成します。

wrapperの外部公開ラベルは既存と完全一致します。例えば `se_select` は生成wrapper側でも `se_select` として定義されます。差し替え対象は `audio_rules.mk` の明示リスト `BPEJ_EXTRACTED_MID_NAMES` にある曲だけです。

include path全体の優先順位は変えていません。BPEJ対象外の既存ファイルが誤って置き換わらないように、対象曲だけ個別ルールで抽出wrapperをassembleします。

## DirectSound sample

DirectSound sampleは、manifest対象だけ `sound/direct_sound_data.inc` の `.incbin` pathを `build/extracted_sound/...` へ変更します。

対象外sampleの `.incbin` は既存 `sound/direct_sound_samples/...` のままです。

## 生成ROMの音確認対象

今回の段階で実機またはエミュレータ確認する対象は以下です。

- PC起動/終了/ログイン系SE: `se_pc_login`, `se_pc_off`, `se_pc_on`
- 決定/選択系SE: `se_select`, `se_use_item`
- メニュー/フィールド系SE: `se_win_open`, `se_wall_hit`, `se_door`, `se_exit`
- 自転車ベル/逃走SE: `se_bike_bell`, `se_flee`
- 戦闘効果音: `se_not_effective`, `se_effective`, `se_super_effective`, `se_ball_open`, `se_faint`
- 汎用効果音: `se_sliding_door`, `se_ship`, `se_bang`, `se_pin`, `se_boo`, `se_ball`
- 音声母音SE: `se_a`, `se_i`, `se_u`, `se_e`
- 追加確認SE: `se_o`, `se_n`, `se_success`, `se_failure`, `se_bike_hop`, `se_switch`, `se_click`, `se_fu_zaku`, `se_contest_condition_lose`, `se_lavaridge_fall_warp`, `se_ice_stairs`, `se_ice_break`, `se_ice_crack`, `se_fall`, `se_unlock`, `se_warp_in`, `se_warp_out`, `se_rotating_gate`, `se_truck_stop`, `se_truck_unload`
- 追加確認SE 2: `se_truck_door`, `se_save`, `se_ball_bounce_1`, `se_ball_bounce_2`, `se_ball_bounce_3`, `se_ball_bounce_4`, `se_ball_trade`, `se_ball_throw`, `se_note_c`, `se_note_d`, `se_note_e`, `se_note_f`, `se_note_g`, `se_note_a`, `se_note_b`, `se_note_c_high`, `se_puddle`, `se_bridge_walk`, `se_itemfinder`, `se_ding_dong`
- レベルアップファンファーレ: `mus_level_up`
- DirectSound sample組み込み確認: `sc88pro_glockenspiel`, `sc88pro_organ2`, `sc88pro_fretless_bass`, `sc88pro_slap_bass`, `bicycle_bell`, `sc88pro_synth_bass`, `sc88pro_timpani`, `classical_choir_voice_ahhs`, `sd90_classical_oboe`, `ethnic_flavours_ohtsuzumi`, `dance_drums_ride_bell`, `drum_and_percussion_kick`, `ethnic_flavours_atarigane`, `ethnic_flavours_hyoushigi`, `ethnic_flavours_kotsuzumi`, `heart_of_asia_gamelan`, `register_noise`, `sc88pro_bubbles`, `sc88pro_church_organ3_high`, `sc88pro_church_organ3_low`

DirectSound sampleは複数曲や複数SEから参照されるため、まずはビルド対象として正しく組み込まれることを確認し、聴感確認は対象サンプルを使う曲/SEを特定して段階的に進めます。

## 対象を増やす手順

1. BPEJ ROM内で対象byte列のoffsetとsizeを確認します。
2. 同名Expansion音源とbyte一致するか確認します。
3. `tools/data/bpej_sound_manifest.json` に `rom_offset`、`size`、`output_path`、`existing_file`、`byte_match`、SHA1、根拠を追加します。
4. BGM/SEの場合は `BPEJ_EXTRACTED_MID_NAMES` にラベルを追加します。
5. DirectSound sampleの場合は対象 `.incbin` だけ抽出先へ変更します。
6. `make clean && make -j4 --output-sync=target` で確認します。
