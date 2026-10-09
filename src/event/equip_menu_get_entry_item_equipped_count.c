#include "fft/event_equip.h"

/* Get the available inventory count for an equipment-menu row.
 *
 * The high result bit preserves whether the row represents an equipped item.
 */
s32 equip_menu_get_entry_item_equipped_count(s32 row) {
    u16 entry = g_equip_item_list_entries[row];
    s32 result = equip_item_get_available_count(entry & EQUIP_ITEM_LIST_ENTRY_ITEM_ID_MASK);
    if (g_equip_item_list_entries[row] & EQUIP_ITEM_LIST_ENTRY_EQUIPPED) {
        result |= EQUIP_ITEM_COUNT_EQUIPPED_FLAG;
    }
    return result;
}
