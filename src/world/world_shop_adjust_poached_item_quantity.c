#include "fft/world.h"

/* Adjust a Fur Shop poached-item quantity, with the target's ten-bit index
 * mask and saturating byte range. */
s32 world_shop_adjust_poached_item_quantity(s32 index, s32 delta) {
    s32 slot = index & WORLD_ITEM_ID_MASK;
    s32 value = g_main_item_poached_quantities[slot] + delta;

    if (value < 0) {
        value = 0;
    }
    if (value >= 0x100) {
        value = 0xFF;
    }
    g_main_item_poached_quantities[slot] = value;
    return value;
}
