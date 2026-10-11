#include "fft/battle.h"
#include "psx/types.h"

/* Open the action menu for the unit on the cursor tile, which becomes the casting
 * unit.
 *
 * The source unit, with the status menu closed, gets its own turn menu when the
 * player controls it and otherwise restarts the menu-to-targeting state. Any
 * other unit gets the non-active-unit menu. An empty tile does nothing. */
void battle_menu_dispatch_idle_action_menu(void) {
    battle_unit_misc_data_t* misc;

    misc = battle_unit_get_selectable_misc_data_at_map_coords(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    if (misc == 0) {
        return;
    }
    g_battle_casting_unit_id = misc->unit_id;
    if (g_battle_status_menu_open == 0 && misc->unit_id == g_battle_unit_source_misc_id) {
        if ((misc->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) != 0) {
            battle_menu_open_active_unit_idle_action_menu();
        } else {
            battle_state_enter_ai_command();
        }
    } else {
        battle_menu_open_non_active_unit_idle_action_menu();
    }
}
