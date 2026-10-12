#include "fft/battle.h"

/* ACTION_HELP_MENU: once the help window has closed, reopen the idle action menu: the active
 * unit's when the casting unit is the source unit, else the other unit's. */
void battle_state_handle_action_help_menu_state(void) {
    battle_unit_misc_data_t* misc_data;

    if (battle_menu_get_thread_activity() != 2) {
        g_battle_menu_help_opening = 0;
        misc_data = battle_unit_get_casting_misc_data();
        if (misc_data != 0) {
            if (misc_data->unit_id == g_battle_unit_source_misc_id) {
                battle_menu_open_active_unit_idle_action_menu();
            } else {
                battle_menu_open_non_active_unit_idle_action_menu();
            }
        }
    }
    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
}
