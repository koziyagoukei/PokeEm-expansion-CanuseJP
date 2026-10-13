#include "global.h"
#include "save.h"
#include "pokemon_storage_system.h"
#include "constants/decorations.h"

// Compile with the ROM's ARM ABI; never hard-code host-compiler save offsets.
const u32 gLocalRoomSaveLayout[] __attribute__((section(".rodata.local_room_save_layout"), used)) =
{
    0x4C52534C, 1,
    sizeof(struct SaveBlock1), sizeof(struct SaveBlock2), sizeof(struct SaveBlock3),
    sizeof(struct PokemonStorage),
    SECTOR_SIZE, SECTOR_DATA_SIZE, SECTOR_SIGNATURE,
    offsetof(struct SaveSector, id), offsetof(struct SaveSector, checksum),
    offsetof(struct SaveSector, signature), offsetof(struct SaveSector, counter),
    offsetof(struct SaveBlock1, secretBases), sizeof(struct SecretBase),
    offsetof(struct SecretBase, decorations), DECOR_MAX_SECRET_BASE,
    offsetof(struct SaveBlock1, playerRoomDecorations), DECOR_MAX_PLAYERS_HOUSE,
    offsetof(struct SaveBlock3, localRoom), sizeof(struct LocalRoomSave),
    offsetof(struct LocalRoomSave, decorations), offsetof(struct LocalRoomSave, checksum),
    offsetof(struct SaveBlock1, decorationDesks), sizeof(gSaveBlock1Ptr->decorationDesks),
    offsetof(struct SaveBlock1, decorationChairs), sizeof(gSaveBlock1Ptr->decorationChairs),
    offsetof(struct SaveBlock1, decorationPlants), sizeof(gSaveBlock1Ptr->decorationPlants),
    offsetof(struct SaveBlock1, decorationOrnaments), sizeof(gSaveBlock1Ptr->decorationOrnaments),
    offsetof(struct SaveBlock1, decorationMats), sizeof(gSaveBlock1Ptr->decorationMats),
    offsetof(struct SaveBlock1, decorationPosters), sizeof(gSaveBlock1Ptr->decorationPosters),
    offsetof(struct SaveBlock1, decorationDolls), sizeof(gSaveBlock1Ptr->decorationDolls),
    offsetof(struct SaveBlock1, decorationCushions), sizeof(gSaveBlock1Ptr->decorationCushions),
    NUM_DECORATIONS,
};
