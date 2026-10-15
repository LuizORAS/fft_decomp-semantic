#include "fft/world.h"

/* Add delta to an item's inventory quantity (never below 0) and return the new quantity; delta 0
 * just reads it. Item 0 (nothing) is ignored and returns 0. The equipment screens call it when an item
 * is put on or taken off.
 *
 * Only the low 10 bits select the inventory slot; callers explicitly sign-extend flagged 16-bit entries
 * where the target uses `lh`. */
s32 world_item_adjust_inventory_count(s16 item_id, s32 delta) {
    s32 idx;
    s32 quantity;

    idx = item_id & WORLD_ITEM_ID_MASK;
    quantity = 0;
    if (idx != ITEM_ID_NOTHING) {
        quantity = g_main_item_quantities[idx] + delta;
        if (quantity < 0) {
            quantity = 0;
        }
        g_main_item_quantities[idx] = quantity;
    }
    return quantity;
}
