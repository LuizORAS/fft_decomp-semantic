#include "fft/battle.h"
#include "fft/battle_target.h"
#include "fft/menu.h"

/* Show the unit under the map cursor in the unit status panel (panel mode 2); with no unit there,
 * panel mode 1. */
void battle_target_show_cursor_unit_panel(void) {
    battle_unit_t* unit;
    s32 mode;
    s32 unit_id;

    unit = battle_unit_find_selectable_at_tile(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    if (unit != 0) {
        mode = UNIT_VIEW_MODE_HOVERED_UNIT;
        unit_id = unit->stats->battle_id;
    } else {
        mode = UNIT_VIEW_MODE_TILE_INFO;
        unit_id = 0xFF;
    }
    battle_unit_start_view_thread(mode, unit_id, 0xFF);
}
