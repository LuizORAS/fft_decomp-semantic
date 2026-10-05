#include "fft/battle.h"

/* Show the unit under the map cursor in the unit status panel (panel mode 2); with no unit there,
 * panel mode 1. */
void battle_target_show_cursor_unit_panel(void) {
    battle_unit_misc_data_t* misc;
    s32 mode;
    s32 unit_id;

    misc = battle_unit_get_selectable_misc_data_at_map_coords(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    if (misc != 0) {
        mode = 2;
        unit_id = misc->battle_data->misc_unit_id;
    } else {
        mode = 1;
        unit_id = 0xFF;
    }
    battle_menu_store_unit_names_and_event_block_data(mode, unit_id, 0xFF);
}
