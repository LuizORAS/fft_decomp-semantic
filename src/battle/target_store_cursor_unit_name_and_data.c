#include "fft/battle.h"
#include "fft/battle_target.h"

/* Start the hovered-unit stats panel for the unit on the cursor tile, or the tile
 * info panel when the tile is empty. */
void target_store_cursor_unit_name_and_data(void) {
    battle_unit_t* unit;
    s32 mode;
    s32 unit_id;

    unit = battle_unit_find_selectable_at_tile(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    if (unit != 0) {
        mode = 2;
        unit_id = unit->battle_data->battle_id;
    } else {
        mode = 1;
        unit_id = 0xFF;
    }
    battle_unit_start_view_thread(mode, unit_id, 0xFF);
}
