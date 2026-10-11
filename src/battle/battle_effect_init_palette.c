#include "fft/battle.h"

/* Send an effect file's palette section to VRAM: its two 256-colour blocks go to
 * the CLUT rows MAP_CLUT_ROW_EFFECT and MAP_CLUT_ROW_EFFECT_SECOND, and the
 * texture image that follows them to (0x180, 0).
 *
 * The image is preceded by its byte size as a 24-bit little-endian value and a
 * width flag: nonzero for an image 0x80 VRAM words (256 bytes) wide, zero for
 * 0x40 words (128 bytes). The height is the size over the row length.
 * g_battle_effect_palette_ptr keeps the section's address. */
void battle_effect_init_palette(s16 index) {
    RECT rect;
    effect_file_header_t* base;
    u8* data;
    s32 size;
    s32 raw_size;
    s32 height;

    base = g_battle_effect_data_ptrs[index];
    data = (u8*)base + base->palette;
    g_battle_effect_palette_ptr = data;
    battle_map_load_palette_data((const u16*)data, MAP_CLUT_ROW_EFFECT, 0, 1);
    battle_map_load_palette_data(
        (const u16*)(data + EFFECT_FILE_PALETTE_SECOND_BLOCK_OFFSET), MAP_CLUT_ROW_EFFECT_SECOND, 0, 1);
    raw_size = data[EFFECT_FILE_PALETTE_SIZE_OFFSET] + (data[EFFECT_FILE_PALETTE_SIZE_OFFSET + 1] << 8);
    raw_size = (data[EFFECT_FILE_PALETTE_SIZE_OFFSET + 2] << 16) + raw_size;
    height = raw_size;
    size = height;
    if (data[EFFECT_FILE_PALETTE_SIZE_OFFSET + 3] != 0) {
        height = size / 0x100;
        rect.x = 0x180;
        rect.y = 0;
        rect.w = 0x80;
        rect.h = height;
        LoadImage(&rect, (u32*)(data + EFFECT_FILE_PALETTE_IMAGE_OFFSET));
    } else {
        height = size;
        height = height / 0x80;
        rect.x = 0x180;
        rect.y = 0;
        rect.w = 0x40;
        rect.h = height;
        LoadImage(&rect, (u32*)(data + EFFECT_FILE_PALETTE_IMAGE_OFFSET));
    }
}
