#include "fft/battle.h"
#include "fft/battle_action.h"

/* Start the unit panels for a selected target tile: the attack preview for the
 * source unit against the unit on the cursor tile, or the source unit's own stats
 * when the tile is empty. */
void target_store_cursor_casting_unit_name_and_data(void) {
    battle_unit_misc_data_t* target;
    battle_unit_misc_data_t* source;

    target
        = battle_unit_get_selectable_misc_data_at_map_coords(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    source = battle_unit_get_source_misc_data();
    if (target != 0) {
        battle_unit_start_view_thread(
            UNIT_VIEW_MODE_ATTACK_PREVIEW, source->battle_data->misc_unit_id, target->battle_data->misc_unit_id);
    } else {
        battle_unit_start_view_thread(UNIT_VIEW_MODE_HOVERED_UNIT, source->battle_data->misc_unit_id, 0xFF);
    }
}
