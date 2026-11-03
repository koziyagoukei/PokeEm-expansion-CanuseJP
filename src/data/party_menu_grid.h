// The grid changes presentation only; party ownership and slot IDs stay intact.
static bool32 UsePartyGrid(void)
{
    return gPartyMenu.layout < PARTY_LAYOUT_COUNT;
}

#define PARTY_GRID_FRAME_TILE 0x3F0

static u16 PartyFrameTile(void)
{
    return UsePartyGrid() ? PARTY_GRID_FRAME_TILE : 0x4F;
}

#define GRID_WINDOW(n, x, y) {0, x, y, 15, 5, 3 + n, 1 + 75 * n}
static const struct WindowTemplate sGridWindows[] =
{
    GRID_WINDOW(0, 0, 2), GRID_WINDOW(1, 15, 2),
    GRID_WINDOW(2, 0, 7), GRID_WINDOW(3, 15, 7),
    GRID_WINDOW(4, 0, 12), GRID_WINDOW(5, 15, 12),
    [WIN_MSG] = {2, 1, 15, 28, 4, 14, 0x1DF},
    {1, 0, 0, 30, 2, 0, 64},
    DUMMY_WIN_TEMPLATE
};
static const struct WindowTemplate sGridShowcaseWindows[] =
{
    GRID_WINDOW(0, 0, 2), GRID_WINDOW(1, 0, 7), GRID_WINDOW(2, 0, 12),
    {2, 15, 2, 15, 5, 6, 226}, {2, 15, 7, 15, 5, 7, 301}, {2, 15, 12, 15, 5, 8, 376},
    [WIN_MSG] = {2, 1, 15, 28, 4, 14, 0x1DF},
    {1, 0, 0, 30, 2, 0, 64},
    DUMMY_WIN_TEMPLATE
};
#undef GRID_WINDOW

static const struct WindowTemplate sGridFooterWindow = {2, 1, 17, 21, 3, 0, 0x24F};
static const struct WindowTemplate sGridCancelWindow = {2, 24, 18, 6, 2, 12, 0x1C7};
static const struct WindowTemplate sGridConfirmWindow = {2, 20, 18, 4, 2, 12, 0x1D3};
static const struct WindowTemplate sGridMultiCancelWindow = {2, 25, 18, 5, 2, 12, 0x1C7};

static const u8 sGridSpriteCoords[PARTY_SIZE][8] =
{
    {20, 36, 30, 46, 96, 36, 8, 20}, {140, 36, 150, 46, 216, 36, 128, 20},
    {20, 76, 30, 86, 96, 76, 8, 60}, {140, 76, 150, 86, 216, 76, 128, 60},
    {20, 116, 30, 126, 96, 116, 8, 100}, {140, 116, 150, 126, 216, 116, 128, 100},
};
static const u8 sGridShowcaseSpriteCoords[PARTY_SIZE][8] =
{
    {20, 36, 30, 46, 96, 36, 8, 20}, {20, 76, 30, 86, 96, 76, 8, 60},
    {20, 116, 30, 126, 96, 116, 8, 100}, {140, 36, 150, 46, 216, 36, 128, 20},
    {140, 76, 150, 86, 216, 76, 128, 60}, {140, 116, 150, 126, 216, 116, 128, 100}
};

static const struct PartyMenuBoxInfoRects sGridInfo =
{
    BlitBitmapToPartyWindow_Grid,
    {36, 2, 78, 12, 36, 14, 32, 12, 72, 14, 8, 12,
     79, 26, 16, 12, 94, 26, 24, 12, 36, 29, 30, 3},
    36, 26, 78, 12
};

static const u16 sGridSlotPalette[] =
{
    RGB(2, 7, 6), RGB(31, 31, 31), RGB(31, 31, 31), RGB(3, 5, 8),
    RGB(4, 14, 25), RGB(28, 30, 31), RGB(17, 26, 31), RGB(26, 5, 7),
    RGB(18, 23, 28), RGB(3, 17, 30), RGB(12, 24, 31), RGB(4, 14, 25),
    RGB(4, 14, 25), RGB(23, 26, 29), RGB(26, 5, 7), RGB(31, 31, 31)
};
static const u16 sGridBackgroundPalette[] =
{
    RGB(2, 7, 6), RGB(2, 7, 6), RGB(3, 9, 8), RGB(31, 31, 31),
    RGB(4, 14, 25), RGB(16, 23, 21), RGB(2, 7, 6), RGB(2, 7, 6),
    RGB(2, 7, 6), RGB(2, 7, 6), RGB(2, 7, 6), RGB(2, 7, 6),
    RGB(2, 7, 6), RGB(2, 7, 6), RGB(2, 7, 6), RGB(2, 7, 6)
};
static const u8 sGridTextColors[] = {1, 3, 1};
static const u8 sGridButtonColors[] = {1, 3, 1};
static const u8 sText_GridParty[] = _("ポケモン");
static const u8 sText_GridPartner[] = _("なかまの ポケモン");
static const u8 sText_GridNoItem[] = _("もちもの なし");
static const u8 sText_GridPartnerBadge[] = _("{ENG}P2");
static const u8 sText_GridBothTeams[] = _("じぶん");
static const u8 sText_GridOtherTeam[] = _("なかま");

static const struct OamData sGridItemOam =
{
    .shape = SPRITE_SHAPE(32x32), .size = SPRITE_SIZE(32x32),
    .affineMode = ST_OAM_AFFINE_OFF, .bpp = ST_OAM_4BPP, .priority = 1
};
#define GRID_ITEM_TEMPLATE(n) { .tileTag = 0xD800 + n, .paletteTag = 0xD800 + n, \
    .oam = &sGridItemOam, .anims = gDummySpriteAnimTable, \
    .affineAnims = gDummySpriteAffineAnimTable, .callback = SpriteCallbackDummy }
static const struct SpriteTemplate sGridItemTemplates[] =
{
    GRID_ITEM_TEMPLATE(0), GRID_ITEM_TEMPLATE(1), GRID_ITEM_TEMPLATE(2),
    GRID_ITEM_TEMPLATE(3), GRID_ITEM_TEMPLATE(4), GRID_ITEM_TEMPLATE(5)
};
#undef GRID_ITEM_TEMPLATE

static void LoadPartyGridBackground(void)
{
    static const u32 solidTile[8] = {
        0x11111112, 0x11111111, 0x11111111, 0x11111111,
        0x11121111, 0x11111111, 0x11111111, 0x11111111
    };
    LoadBgTiles(1, solidTile, sizeof(solidTile), 0);
    CpuFill16(0, sPartyBgTilemapBuffer, BG_SCREEN_SIZE);
    LoadPalette(sGridBackgroundPalette, 0, sizeof(sGridBackgroundPalette));
    for (u32 i = 3; i <= 8; i++)
        LoadPalette(sGridSlotPalette, BG_PLTT_ID(i), sizeof(sGridSlotPalette));
    LoadPalette(sGridSlotPalette, BG_PLTT_ID(12), sizeof(sGridSlotPalette));
    ScheduleBgCopyTilemapToVram(1);
}

static void DrawPartyGridHeader(void)
{
    const u8 *title = gPartyMenu.layout == PARTY_LAYOUT_MULTI_FULL_PARTNER
        || gPartyMenu.layout == PARTY_LAYOUT_MULTI_FULL_SHOWCASE_PARTNER ? sText_GridPartner
        : gPartyMenu.layout == PARTY_LAYOUT_MULTI_SHOWCASE ? sText_GridBothTeams : sText_GridParty;
    FillWindowPixelBuffer(7, PIXEL_FILL(1));
    FillWindowPixelRect(7, PIXEL_FILL(4), 0, 14, 240, 2);
    FillWindowPixelRect(7, PIXEL_FILL(4), 4, 2, 2, 9);
    AddTextPrinterParameterized3(7, FONT_SMALL, 8, 1, sGridTextColors, TEXT_SKIP_DRAW, title);
    if (gPartyMenu.layout == PARTY_LAYOUT_MULTI_SHOWCASE)
        AddTextPrinterParameterized3(7, FONT_SMALL, 128, 1, sGridTextColors, TEXT_SKIP_DRAW, sText_GridOtherTeam);
    if (gPartyMenu.layout != PARTY_LAYOUT_MULTI_SHOWCASE)
    {
        u8 count[12];
        enum BattleTrainer trainer = gPartyMenu.layout == PARTY_LAYOUT_MULTI_FULL_PARTNER ? B_TRAINER_PARTNER : B_TRAINER_PLAYER;
        u32 size = gPartiesCount[trainer];
        if (gPartyMenu.layout == PARTY_LAYOUT_MULTI)
            size = min(PARTY_SIZE, size + gPartiesCount[B_TRAINER_PARTNER]);
        u8 *end = ConvertIntToDecimalStringN(count, size, STR_CONV_MODE_LEFT_ALIGN, 1);
        StringCopy(end, COMPOUND_STRING("{ENG}/6"));
        AddTextPrinterParameterized3(7, FONT_SMALL, 212, 1, sGridTextColors, TEXT_SKIP_DRAW, count);
    }
    PutWindowTilemap(7);
    CopyWindowToVram(7, COPYWIN_FULL);
}

static void DrawPartyGridFooter(void)
{
    u8 window = sPartyMenuInternal->windowId[1];
    u8 name[POKEMON_NAME_BUFFER_SIZE];
    u8 item[ITEM_NAME_LENGTH + 1];
    if (!UsePartyGrid() || sPartyMenuInternal->gridMessage != PARTY_MSG_CHOOSE_MON || window == WINDOW_NONE)
        return;
    FillWindowPixelBuffer(window, PIXEL_FILL(1));
    if (gPartyMenu.slotId < PARTY_SIZE)
    {
        struct Pokemon *mon = GetPartyMonFromPartyMenuId(gPartyMenu.slotId);
        GetMonNickname(mon, name);
        if (GetMonData(mon, MON_DATA_HELD_ITEM) == ITEM_NONE)
            StringCopy(item, sText_GridNoItem);
        else
            CopyItemName(GetMonData(mon, MON_DATA_HELD_ITEM), item);
        AddTextPrinterParameterized3(window, GetFontIdToFit(name, FONT_SMALL, 0, WindowWidthPx(window)), 0, 0, sGridTextColors, TEXT_SKIP_DRAW, name);
        AddTextPrinterParameterized3(window, GetFontIdToFit(item, FONT_SMALL, 0, WindowWidthPx(window)), 0, 12, sGridTextColors, TEXT_SKIP_DRAW, item);
    }
    PutWindowTilemap(window);
    // Closing a modal can clear the buttons' tilemap entries on the same BG.
    for (u32 i = 0; i < ARRAY_COUNT(sPartyMenuInternal->gridButtons); i++)
    {
        if (sPartyMenuInternal->gridButtons[i] != WINDOW_NONE)
            PutWindowTilemap(sPartyMenuInternal->gridButtons[i]);
    }
    CopyWindowToVram(window, COPYWIN_FULL);
}

static void DrawPartyGridButton(u8 window, const u8 *label, bool32 selected)
{
    u32 width = WindowWidthPx(window);
    FillWindowPixelBuffer(window, PIXEL_FILL(1));
    FillWindowPixelRect(window, PIXEL_FILL(selected ? 7 : 4), 0, 0, width, 1);
    FillWindowPixelRect(window, PIXEL_FILL(selected ? 7 : 4), 0, 15, width, 1);
    AddTextPrinterParameterized3(window, FONT_SMALL, GetStringCenterAlignXOffset(FONT_SMALL, label, width), 2, sGridButtonColors, TEXT_SKIP_DRAW, label);
    PutWindowTilemap(window);
    CopyWindowToVram(window, COPYWIN_FULL);
}

static void CreatePartyGridButtons(bool32 chooseHalf)
{
    sPartyMenuInternal->gridButtons[0] = WINDOW_NONE;
    if (chooseHalf)
    {
        sPartyMenuInternal->gridButtons[0] = AddWindow(&sGridConfirmWindow);
        DrawPartyGridButton(sPartyMenuInternal->gridButtons[0], gMenuText_Confirm, gPartyMenu.slotId == PARTY_SIZE);
    }
    sPartyMenuInternal->gridButtons[1] = AddWindow(chooseHalf ? &sGridMultiCancelWindow : &sGridCancelWindow);
    DrawPartyGridButton(sPartyMenuInternal->gridButtons[1], gText_Cancel, gPartyMenu.slotId == PARTY_SIZE + 1);
}

static void UpdatePartySelectionGrid(s8 *slot, s8 direction)
{
    s8 next = *slot;
    if (next >= PARTY_SIZE)
    {
        if (direction == MENU_DIR_LEFT || direction == MENU_DIR_RIGHT)
            next = sPartyMenuInternal->chooseHalf && next == PARTY_SIZE + 1 ? PARTY_SIZE : PARTY_SIZE + 1;
        else
        {
            s8 step = direction == MENU_DIR_UP ? -1 : 1;
            next = direction == MENU_DIR_UP ? PARTY_SIZE - 1 : 0;
            while (next >= 0 && next < PARTY_SIZE && GetMonData(GetPartyMonFromPartyMenuId(next), MON_DATA_SPECIES) == SPECIES_NONE)
                next += step;
            if (next < 0 || next >= PARTY_SIZE)
                return;
        }
    }
    else if (direction == MENU_DIR_LEFT || direction == MENU_DIR_RIGHT)
    {
        next ^= 1;
        if (GetMonData(GetPartyMonFromPartyMenuId(next), MON_DATA_SPECIES) == SPECIES_NONE)
            return;
    }
    else
    {
        s8 step = direction == MENU_DIR_UP ? -2 : 2;
        do { next += step; }
        while (next >= 0 && next < PARTY_SIZE && GetMonData(GetPartyMonFromPartyMenuId(next), MON_DATA_SPECIES) == SPECIES_NONE);
        if (next < 0 || next >= PARTY_SIZE)
            next = direction == MENU_DIR_DOWN && sPartyMenuInternal->chooseHalf ? PARTY_SIZE : PARTY_SIZE + 1;
    }
    *slot = next;
}

static void UpdatePartyGridItemIcon(enum Item item, struct PartyMenuBox *box)
{
    u16 tag = 0xD800 + box->windowId;
    if (box->itemSpriteId < MAX_SPRITES && gSprites[box->itemSpriteId].inUse)
    {
        struct Sprite *old = &gSprites[box->itemSpriteId];
        tag = old->data[6];
        if (old->oam.affineMode != ST_OAM_AFFINE_OFF)
            FreeSpriteOamMatrix(old);
        DestroySprite(old);
        FreeSpriteTilesByTag(tag);
        FreeSpritePaletteByTag(tag);
    }
    box->itemSpriteId = AddItemIconSprite(tag, tag, item);
    if (box->itemSpriteId >= MAX_SPRITES)
        box->itemSpriteId = CreateInvisibleSprite(SpriteCallbackDummy);
    struct Sprite *sprite = &gSprites[box->itemSpriteId];
    // AddItemIconSprite's temporary template is freed before it returns.
    // Keep a ROM-owned template for later sprite animation/resource callbacks.
    sprite->template = &sGridItemTemplates[tag - 0xD800];
    sprite->data[6] = tag;
    sprite->x = GetWindowAttribute(box->windowId, WINDOW_TILEMAP_LEFT) * 8 + 108;
    sprite->y = GetWindowAttribute(box->windowId, WINDOW_TILEMAP_TOP) * 8 + 8;
    sprite->oam.priority = GetWindowAttribute(box->windowId, WINDOW_BG) == 2 ? 0 : 1;
    u8 matrix = AllocOamMatrix();
    if (matrix != 0xFF)
    {
        sprite->oam.affineMode = ST_OAM_AFFINE_NORMAL;
        sprite->oam.matrixNum = matrix;
        sprite->affineAnimPaused = TRUE;
        SetOamMatrix(matrix, 512, 0, 0, 512);
        CalcCenterToCornerVec(sprite, sprite->oam.shape, sprite->oam.size, sprite->oam.affineMode);
    }
    sprite->invisible = item == ITEM_NONE || matrix == 0xFF;
}

static void DrawPartyGridName(u8 window, const u8 *name, const u8 *position, u32 width)
{
    u8 clipped[POKEMON_NAME_BUFFER_SIZE + 16];
    u8 adjusted[2] = {position[0], position[1]};
    u32 copied = 0;
    if (gPartyMenu.layout == PARTY_LAYOUT_MULTI && (window == 1 || window == 4 || window == 5))
    {
        static const u8 badgeColors[] = {1, 4, 1};
        AddTextPrinterParameterized3(window, FONT_SMALL, position[0], position[1], badgeColors, 0, sText_GridPartnerBadge);
        adjusted[0] += 14;
        width -= 14;
        position = adjusted;
    }
    if (GetStringWidth(FONT_SMALL, name, 0) <= width)
    {
        AddTextPrinterParameterized3(window, FONT_SMALL, position[0], position[1], sFontColorTable[0], 0, name);
        return;
    }
    while (*name != EOS)
    {
        u32 length = *name == EXT_CTRL_CODE_BEGIN ? 1 + max(1, GetExtCtrlCodeLength(name[1]))
            : *name == CHAR_EXTRA_SYMBOL ? 2 : 1;
        if (copied + length + 2 > sizeof(clipped))
            break;
        memcpy(clipped + copied, name, length);
        clipped[copied + length] = CHAR_ELLIPSIS;
        clipped[copied + length + 1] = EOS;
        if (GetStringWidth(FONT_SMALL, clipped, 0) > width)
            break;
        copied += length;
        name += length;
    }
    clipped[copied] = CHAR_ELLIPSIS;
    clipped[copied + 1] = EOS;
    AddTextPrinterParameterized3(window, FONT_SMALL, position[0], position[1], sFontColorTable[0], 0, clipped);
}
