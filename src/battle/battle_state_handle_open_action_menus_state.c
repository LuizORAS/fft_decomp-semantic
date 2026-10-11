#include "fft/battle.h"

/* OpenActionMenus game-state handler: once the window has closed (command 7, 8 or
 * 0xff) and the camera has settled, open the idle action menu of the unit under
 * the cursor. */
void battle_state_handle_open_action_menus_state(void) {
    s32 selected_command;

    battle_state_handle_free_cursor_input();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    selected_command = battle_menu_get_selected_command_address()->kind;
    if (selected_command >= 7 && (selected_command < 9 || selected_command == 0xff)) {
        g_battle_action_post_action = 1;
    }
    if ((g_battle_current_vector.vx | g_battle_current_vector.vy | g_battle_current_vector.vz) == 0
        && g_battle_camera_rotation_action == BATTLE_CAMERA_ROTATION_NONE && g_battle_action_post_action != 0) {
        battle_menu_dispatch_idle_action_menu();
    }
}
