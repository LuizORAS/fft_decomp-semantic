#include "psx/types.h"

/* Read the 4-bit pixel at (`index`, `row`) of an image `stride` pixels wide: an odd index takes the
 * byte's high nibble, an even one the low nibble. */
s32 battle_menu_read_packed_nibble(u8* data, s32 index, s32 row, s32 stride) {
    u8* base = data;
    s32 mask;
    s32 value;

    mask = index & 1;
    if (mask == 0) {
        mask = 0xF;
    } else {
        mask = 0xF0;
    }
    value = *(base + ((row * stride + index) >> 1)) & mask;
    if (mask == 0xF0) {
        value >>= 4;
    }
    return value;
}
