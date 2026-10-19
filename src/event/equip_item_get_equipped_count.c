#include "fft/event_equip.h"
#include "psx/types.h"

/* Number of copies of an item the party has equipped: the total held (g_equip_item_inventory_totals)
 * minus the inventory count (read with a zero delta). */
s32 equip_item_get_equipped_count(s32 item_entry) {
    s32 item_id;

    item_id = item_entry & EQUIP_ITEM_ID_MASK;
    return g_equip_item_inventory_totals[item_id] - equip_item_adjust_inventory_count(item_id, 0);
}
