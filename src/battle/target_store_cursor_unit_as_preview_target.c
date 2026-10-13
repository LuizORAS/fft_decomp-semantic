#include "fft/battle.h"
#include "fft/battle_target.h"

/* Start the right-side stats panel for the unit on the cursor tile, or the tile
 * info panel when the tile is empty. */
void target_store_cursor_unit_as_preview_target(void) {
    battle_unit_t* misc
        = battle_unit_get_selectable_misc_data_at_map_coords(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    battle_unit_get_source_misc_data();
    if (misc != 0) {
        battle_unit_start_view_thread(UNIT_VIEW_MODE_RIGHT_UNIT, 0xFF, misc->battle_data->battle_id);
    } else {
        battle_unit_start_view_thread(UNIT_VIEW_MODE_TILE_INFO, 0xFF, 0xFF);
    }
}
