#include "fft/battle_action.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Show the next AT list entry and advance the AT list cursor past it.
 *
 * Empty entries are skipped. A charged action moves the cursor to its target and
 * marks the tiles it will hit; a unit's turn moves the cursor to the unit and
 * removes the tile marks. The free cursor's Start button and the status-menu
 * request call it. The loop has no exit when every entry is empty. */
void action_show_next_at_list_entry(void) {
    s32 id;
    battle_unit_t* unit;

    for (;;) {
        id = action_calculate_at_list_and_get_specific_unit_id(g_action_at_list_id);
        if (id >= 0) {
            unit = battle_unit_get_by_battle_id(id & 0xFF);
            if ((id & 0x100) != 0) {
                target_move_cursor_to_action_target(unit);
                target_calculate_for_menu_types(&unit->stats->action_command);
                target_set_tile_background_color(BATTLE_TILE_MARK_SHOW_TARGETS, 3);
            } else {
                target_move_cursor_to_unit(unit);
                target_set_tile_background_color(BATTLE_TILE_MARK_NONE, 0);
            }
            break;
        }
        action_increment_at_list_id();
    }
    action_increment_at_list_id();
}
