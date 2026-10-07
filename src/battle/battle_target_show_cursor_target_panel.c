#include "fft/battle.h"

/* While targeting, show the unit under the map cursor in the target status panel (panel mode 4);
 * with no unit there, panel mode 1. */
void battle_target_show_cursor_target_panel(void) {
    battle_unit_misc_data_t* misc
        = battle_unit_get_selectable_misc_data_at_map_coords(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    battle_unit_get_source_misc_data();
    if (misc != 0) {
        battle_menu_store_unit_names_and_event_block_data(4, 0xFF, misc->battle_data->misc_unit_id);
    } else {
        battle_menu_store_unit_names_and_event_block_data(1, 0xFF, 0xFF);
    }
}
