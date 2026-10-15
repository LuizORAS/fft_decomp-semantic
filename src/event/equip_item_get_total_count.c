#include "fft/event_equip.h"

/* The copies of an item the player holds, in the inventory or equipped by the roster
 * (g_equip_item_inventory_totals, built by equip_item_build_inventory_totals). */
s32 equip_item_get_total_count(s16 item_id) {
    return g_equip_item_inventory_totals[item_id & EQUIP_ITEM_ID_MASK];
}
