#include "fft/battle_text.h"
#include "psx/types.h"

/* Mirror a rectangle of a 4-bit image left to right, pixel by pixel. battle_menu_build_window_image
 * draws a window frame's right-hand pieces as mirrored copies of the left-hand ones with it. */
void battle_menu_mirror_packed_nibble_region(u8* data, const world_glyph_blit_t* region) {
    s32 row;
    s32 col;
    s32 value;
    s32 offset;

    for (row = 0; row < region->rect.h; row++) {
        for (col = 0; col < region->rect.w / 2; col++) {
            value = battle_menu_read_packed_nibble(data, region->rect.x + col, region->rect.y + row, region->stride);
            offset = 1;
            battle_menu_write_packed_nibble(data, region->rect.x + col, region->rect.y + row, region->stride,
                battle_menu_read_packed_nibble(
                    data, region->rect.x + region->rect.w - (col + offset), region->rect.y + row, region->stride));
            battle_menu_write_packed_nibble(
                data, region->rect.x + region->rect.w - (col + offset), region->rect.y + row, region->stride, value);
        }
    }
}
