#ifndef GUARD_CONFIG_WILD_ENCOUNTER_H
#define GUARD_CONFIG_WILD_ENCOUNTER_H

// 原作方式の遭遇。
#define WE_VANILLA_RANDOM           TRUE    // TRUEでは、原作と同様に野生遭遇が可能なタイルでランダムにポケモンと遭遇する。
#define WE_FLAG_NO_ENCOUNTER        0       // 指定したフラグが立っている場合、野生遭遇を無効にする。

// 野生のダブルバトル。
#define WE_DOUBLE_WILD_CHANCE            0          // 野生遭遇で2匹同時に出現する確率（%）。
#define WE_DOUBLE_WILD_REQUIRE_2_MONS    FALSE      // TRUEでは、戦闘可能な手持ちが1匹だけの場合、WE_DOUBLE_WILD_CHANCEとWE_FLAG_FORCE_DOUBLE_WILDにかかわらずシングルバトルにする。
#define WE_WILD_NATURAL_ENEMIES          TRUE       // TRUEでは、野生ダブルバトルで組んだ特定の種族同士が攻撃し合う（例: ザングースとハブネーク）。

// 野生戦のフラグ。
#define WE_FLAG_FORCE_DOUBLE_WILD    0     // 指定したフラグが立っている場合、草むら・なみのり中の野生戦をすべてダブルバトルにする。
#define WE_SMART_WILD_AI_FLAG        0     // 指定したフラグが立っている場合、野生ポケモンにすべてのAIフラグを有効化する。
#define WE_FLAG_NO_CATCHING          0     // 指定したフラグが立っている場合、野生ポケモンの捕獲を禁止する。
#define WE_FLAG_NO_RUNNING           0     // 指定したフラグが立っている場合、野生戦から逃げられなくする。ほえる・ふきとばしと、第8世代未満のテレポートも失敗する。

// フィールド上に見える野生ポケモンとの遭遇（OWE）。
#define WE_OW_ENCOUNTERS                        FALSE   // TRUEでは、現在のマップにOWEとして野生ポケモンを配置できる。OW_POKEMON_OBJECT_EVENTSが必要。
                                                        // TRUEにする場合、VRAMの問題を避けるためOW_GFX_COMPRESSはFALSEを推奨する。
#define WE_OWE_FLAG_DISABLED                    0       // 0をフラグIDに置き換えると、自動生成するOWEの有効・無効を切り替えられる。
#define WE_OWE_SPECIAL_ONLY                     FALSE   // TRUEでは、自動生成するOWEを特殊な出現だけに限定する。
#define WE_OWE_BATTLE_PIKE                      TRUE    // TRUEではバトルチューブでOWEを生成し、FALSEでは従来のランダム遭遇を使う。WE_OW_ENCOUNTERSがTRUEである必要がある。
#define WE_OWE_BATTLE_PYRAMID                   TRUE    // TRUEではバトルピラミッドでOWEを生成し、FALSEでは従来のランダム遭遇を使う。WE_OW_ENCOUNTERSがTRUEである必要がある。
#define WE_OWE_RESTRICT_METATILE                TRUE    // TRUEでは、現在のタイルに遭遇用の属性がある場合、同じ遭遇属性のタイル内だけを移動する。
#define WE_OWE_RESTRICT_MAP                     TRUE    // TRUEでは、OWEの移動を現在のマップ範囲内に制限する。
#define WE_OWE_UNRESTRICT_SIGHT                 FALSE   // TRUEでは、プレイヤーに反応する移動タイプのOWEがプレイヤーを見つけた場合、すべての移動制限を無視する。
#define WE_OWE_SPAWN_REPLACEMENT                FALSE   // TRUEでは、一定時間後に最も古いOWEを消し、可能なら新しいOWEに置き換える。
#define WE_OWE_FLEE_DESPAWN                     TRUE    // TRUEでは、逃走中のOWEが一定時間移動できない場合に消す。
#define WE_OWE_SHINY_SPARKLE                    FALSE   // TRUEでは、色違いのOWE出現時に光るアニメーションと色違いの効果音を再生する。
#define WE_OWE_FEEBAS_SPOTS                     FALSE   // TRUEでは、ヒンバスを釣れる可能性がある場所にヒンバスのOWEを生成できる。
#define WE_OWE_DESPAWN_SOUND                    FALSE   // TRUEでは、OWEを消すときにSE_FLEEを再生する。
#define WE_OWE_APPROACH_FOR_BATTLE              TRUE    // TRUEでは、OWEがプレイヤーの隣まで移動してから戦闘を始める。
#define WE_OWE_PREVENT_SHINY_DESPAWN            FALSE   // TRUEでは、プレイヤーと同じマップにいる色違いOWEを画面外でも消さず、WE_OWE_SPAWN_REPLACEMENTによる置き換えからも保護する。
#define WE_OWE_PREVENT_FEEBAS_DESPAWN           FALSE   // TRUEでは、WE_OWE_FEEBAS_SPOTSで専用の釣り地点から出現したヒンバスOWEを、同じマップの画面外でも消さず、WE_OWE_SPAWN_REPLACEMENTによる置き換えからも保護する。
#define WE_OWE_PREVENT_SPECIAL_MOVEMENT_DESPAWN TRUE    // TRUEでは、WE_OWE_PREVENT_SHINY_DESPAWNで保護した色違いOWEと、WE_OWE_PREVENT_FEEBAS_DESPAWNで保護したヒンバスOWEが、移動タイプの処理（例: OWE_DESPAWN_ON_NOTICE）で消えないようにする。
#define WE_OWE_DESPAWN_ON_ENTER_TOWN            TRUE    // TRUEでは、市（MAP_TYPE_CITY）や町（MAP_TYPE_TOWN）へ入るとすべてのOWEを消す。
#define WE_OWE_NO_REPEL_DEXNAV_COLLISION        FALSE   // TRUEでは、むしよけまたはDexNavが有効な間、接触によるOWEとの戦闘を開始しない。

#endif // GUARD_CONFIG_WILD_ENCOUNTER_H
