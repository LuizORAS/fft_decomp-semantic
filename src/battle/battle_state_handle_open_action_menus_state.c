#include "fft/battle.h"

/* OPEN_ACTION_MENUS: update the units and the menu; once the menu answers (7, 8 or cancel) and
 * the camera has stopped, open the idle action menu. */
void battle_state_handle_open_action_menus_state(void) {
    s32 command;

    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
    command = *battle_menu_get_selected_command_address();
    if (command >= 7 && (command < 9 || command == 0xff)) {
        g_battle_action_post_action = 1;
    }
    if ((g_battle_current_vector.vx | g_battle_current_vector.vy | g_battle_current_vector.vz) == 0
        && g_battle_camera_rotation_action == 0 && g_battle_action_post_action != 0) {
        battle_menu_dispatch_idle_action_menu();
    }
}
