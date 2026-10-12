#ifndef GUARD_FOLLOWER_NPC_OVERWORLD_H
#define GUARD_FOLLOWER_NPC_OVERWORLD_H

// 同行NPC
#define FNPC_ENABLE_NPC_FOLLOWERS                 TRUE  // DPPのように、スクリプトマクロでNPCをプレイヤーの後ろに同行させる機能を有効にする。SaveBlock3のサイズが少し増える。
#define FNPC_FLAG_HEAL_AFTER_FOLLOWER_BATTLE      FLAG_EXPANSION_FOLLOWER_HEAL_AFTER_BATTLE     // 指定フラグで、同行NPCとの協力戦後に手持ちを自動回復するか切り替える。フラグを使わず常に有効にする場合はFNPC_ALWAYSを指定する。
#define FNPC_FLAG_PARTNER_WILD_BATTLES            FLAG_EXPANSION_FOLLOWER_PARTNER_WILD_BATTLES  // 指定フラグで、同行NPCが野生ポケモン戦に参加するか切り替える。フラグを使わず常に有効にする場合はFNPC_ALWAYSを指定する。
#define FNPC_NPC_FOLLOWER_WILD_BATTLE_VS_2        TRUE  // TRUEなら、協力戦で野生ポケモンが1匹ではなく2匹出現する。
#define FNPC_NPC_FOLLOWER_PARTY_PREVIEW           TRUE  // TRUEなら、トレーナー戦の前にプレイヤーと同行NPCの手持ちを表示する。
#define FNPC_FACE_NPC_FOLLOWER_ON_DOOR_EXIT       TRUE  // TRUEなら、出入口を出た際にプレイヤーが同行NPCの方を向く。
#define FNPC_NPC_FOLLOWER_SHOW_AFTER_LEAVE_ROUTE  TRUE  // TRUEなら、そらをとぶ・あなぬけのヒモ・テレポートの使用後に、同行NPCがプレイヤーから歩き出して再表示される。

#endif // GUARD_FOLLOWER_NPC_OVERWORLD_H
