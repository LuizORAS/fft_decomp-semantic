#include "fft/battle.h"
#include "fft/battle_action.h"

/* Start the hovered-unit stats panel for the unit on the cursor tile, or the tile
 * info panel when the tile is empty. */
void battle_target_store_cursor_unit_name_and_data(void) {
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
    battle_unit_start_view_thread(mode, unit_id, 0xFF);
}
