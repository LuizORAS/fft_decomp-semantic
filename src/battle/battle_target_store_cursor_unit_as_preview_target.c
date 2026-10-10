#include "fft/battle.h"

/* Start the right-side stats panel for the unit on the cursor tile, or the tile
 * info panel when the tile is empty. */
void battle_target_store_cursor_unit_as_preview_target(void) {
    battle_unit_misc_data_t* misc
        = battle_unit_get_selectable_misc_data_at_map_coords(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    battle_unit_get_source_misc_data();
    if (misc != 0) {
        battle_unit_start_view_thread(UNIT_VIEW_MODE_RIGHT_UNIT, 0xFF, misc->battle_data->misc_unit_id);
    } else {
        battle_unit_start_view_thread(UNIT_VIEW_MODE_TILE_INFO, 0xFF, 0xFF);
    }
}
