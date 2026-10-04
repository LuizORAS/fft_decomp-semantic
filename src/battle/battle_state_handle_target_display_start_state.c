#include "fft/battle.h"
#include "psx/types.h"

/* TARGET_DISPLAY_START: once the message has been answered and the camera has stopped, enter
 * TARGET_DISPLAY. */
void battle_state_handle_target_display_start_state(void) {
    s32 command;

    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
    command = *battle_menu_get_selected_command_address();
    if ((command >= 7) && ((command < 9) || (command == 0xFF))) {
        g_battle_action_post_action = 1;
    }
    if ((g_battle_current_vector.vx | g_battle_current_vector.vy | g_battle_current_vector.vz) == 0) {
        if (g_battle_camera_rotation_action == 0) {
            if (g_battle_action_post_action != 0) {
                battle_state_enter_target_display();
            }
        }
    }
}
