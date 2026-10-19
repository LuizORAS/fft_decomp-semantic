#include "fft/world.h"

/* The sell price shown for menu row `entry_index`: half the item's price, with
 * MENU_NUMBER_VALUE_DISABLED set for an unequippable entry. Twin of
 * world_menu_get_item_price_display_value. */
s32 world_menu_get_item_half_price_display_value(s32 entry_index) {
    s32 display_value = world_item_get_price(g_world_menu_entry_ids[entry_index]) >> 1;

    if ((g_world_menu_entry_ids[entry_index] & WORLD_ITEM_LIST_ENTRY_UNEQUIPPABLE) != 0)
        display_value |= MENU_NUMBER_VALUE_DISABLED;
    return display_value;
}
