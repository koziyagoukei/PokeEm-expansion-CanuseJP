#include "global.h"
#include "text.h"
#include "dynamic_placeholder_text_util.h"
#include "string_util.h"

static EWRAM_DATA const u8 *sStringPointers[8] = {};

void DynamicPlaceholderTextUtil_Reset(void)
{
    int i;
    for (i = 0; i < (int)ARRAY_COUNT(sStringPointers); i++)
        sStringPointers[i] = NULL;
}

void DynamicPlaceholderTextUtil_SetPlaceholderPtr(u8 idx, const u8 *ptr)
{
    if (idx < ARRAY_COUNT(sStringPointers))
    {
        sStringPointers[idx] = ptr;
    }
}

u8 *DynamicPlaceholderTextUtil_ExpandPlaceholders(u8 *dest, const u8 *src)
{
    u8 mode = 0;

    while (*src != EOS)
    {
        if (*src == EXT_CTRL_CODE_BEGIN)
        {
            u32 length = 1 + max(1, GetExtCtrlCodeLength(src[1]));
            mode = UpdateTextModeCode(mode, src[1]);
            while (length--)
                *dest++ = *src++;
        }
        else if (*src == CHAR_EXTRA_SYMBOL || *src == CHAR_KEYPAD_ICON)
        {
            *dest++ = *src++;
            *dest++ = *src++;
        }
        else if (*src != CHAR_DYNAMIC)
        {
            *dest++ = *src++;
        }
        else
        {
            src++;
            const u8 *placeholder = DynamicPlaceholderTextUtil_GetPlaceholderPtr(*src);
            if (placeholder != NULL)
            {
                u8 restore = GetTextModeToRestore(mode, GetTextModeCode(placeholder, NULL));
                dest = StringCopy(dest, placeholder);
                if (restore != 0)
                {
                    *dest++ = EXT_CTRL_CODE_BEGIN;
                    *dest++ = restore;
                }
            }
            src++;
        }
    }
    *dest = EOS;
    return dest;
}

const u8 *DynamicPlaceholderTextUtil_GetPlaceholderPtr(u8 idx)
{
    return idx < ARRAY_COUNT(sStringPointers) ? sStringPointers[idx] : NULL;
}
