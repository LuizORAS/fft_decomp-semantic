#include "fft/battle.h"
#include "fft/battle_target.h"

/* While targeting, show the unit under the map cursor in the target status panel (panel mode 4);
 * with no unit there, panel mode 1. */
void battle_target_show_cursor_target_panel(void) {
    battle_unit_t* unit = battle_unit_find_selectable_at_tile(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    battle_unit_get_source();
    if (unit != 0) {
        battle_unit_start_view_thread(UNIT_VIEW_MODE_RIGHT_UNIT, 0xFF, unit->stats->battle_id);
    } else {
        battle_unit_start_view_thread(UNIT_VIEW_MODE_TILE_INFO, 0xFF, 0xFF);
    }
}
