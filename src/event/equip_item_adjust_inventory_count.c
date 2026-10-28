#include "fft/event_equip.h"
#include "psx/types.h"

/* Add delta to an item's inventory quantity (never below 0) and return the new quantity; item 0 is
 * ignored and returns 0. EQUIP's twin of world_item_adjust_inventory_count: only the low 10 bits of
 * item_entry are the item id. */
s32 equip_item_adjust_inventory_count(s32 item_entry, s32 delta) {
    s32 item_id;
    s32 new_count;

    item_id = item_entry & EQUIP_ITEM_ID_MASK;
    if (item_id == 0) {
        return 0;
    }
    new_count = g_main_item_quantities[item_id] + delta;
    if (new_count < 0) {
        new_count = 0;
    }
    g_main_item_quantities[item_id] = new_count;
    return new_count;
}
