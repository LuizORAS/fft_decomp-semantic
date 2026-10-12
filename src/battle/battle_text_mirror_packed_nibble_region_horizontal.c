#include "fft/battle_text.h"
#include "psx/types.h"

/* Mirrors a packed nibble rectangle horizontally, swapping each nibble with its opposite. */
void battle_text_mirror_packed_nibble_region_horizontal(u8* data, glyph_blit_t* region) {
    s32 row;
    s32 col;
    s32 value;
    s32 offset;

    for (row = 0; row < region->rect.h; row++) {
        for (col = 0; col < region->rect.w / 2; col++) {
            value = battle_text_read_packed_nibble(data, region->rect.x + col, region->rect.y + row, region->stride);
            offset = 1;
            battle_write_packed_nibble(data, region->rect.x + col, region->rect.y + row, region->stride,
                battle_text_read_packed_nibble(
                    data, region->rect.x + region->rect.w - (col + offset), region->rect.y + row, region->stride));
            battle_write_packed_nibble(
                data, region->rect.x + region->rect.w - (col + offset), region->rect.y + row, region->stride, value);
        }
    }
}
