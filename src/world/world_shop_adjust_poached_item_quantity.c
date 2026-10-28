#include "fft/world.h"

/* Add delta to the fur shop's stock of an item (g_main_item_poached_quantities, indexed by
 * the low 10 bits of the id), clamped to 0..255, and return the new count; 0 reads it. */
s32 world_shop_adjust_poached_item_quantity(s16 index, s32 delta) {
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
