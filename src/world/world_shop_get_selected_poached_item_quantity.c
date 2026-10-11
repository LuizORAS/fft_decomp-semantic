#include "fft/world.h"

/* Menu value callback 8 of the fur shop's buy list: the stock of the highlighted item, or 0
 * when the list is empty. */
s32 world_shop_get_selected_poached_item_quantity(s32 unused_row) {
    if (g_world_menu_option_count != 0)
        return world_shop_adjust_poached_item_quantity(g_world_menu_entry_ids[g_world_menu_cursor_position], 0);
    return 0;
}
