#include "fft/battle.h"
#include "psx/types.h"

void battle_state_handle_target_display_start_state(void) {
    s32 id;

    battle_state_handle_free_cursor_input();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    id = *battle_menu_get_selected_command_address();
    if ((id >= 7) && ((id < 9) || (id == 0xFF))) {
        g_battle_action_post_action = 1;
    }
    if ((g_battle_current_vector.vx | g_battle_current_vector.vy | g_battle_current_vector.vz) == 0) {
        if (g_battle_camera_rotation_action == BATTLE_CAMERA_ROTATION_NONE) {
            if (g_battle_action_post_action != 0) {
                battle_state_enter_target_display();
            }
        }
    }
}
