#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "psx/types.h"

/* ACTION_CAST: once the event check has answered and the camera has stopped, enter
 * COMMENCE_ATTACK_PHASE. */
void battle_state_handle_action_cast_state(void) {
    s32 command;

    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command = battle_menu_get_selected_command_address()->kind;
    /* Selections 7, 8 and 0xff are the ones ACTION_EXECUTE_SETUP handles;
     * they mark the action as finished (same test as the
     * BATTLE_MESSAGE_DISPLAY handler). */
    if (command >= 7 && (command < 9 || command == 0xff)) {
        g_battle_action_post_action = 1;
    }
    /* Wait for the camera to settle before handing the action data over. */
    if ((g_battle_camera_step_vector.vx | g_battle_camera_step_vector.vy | g_battle_camera_step_vector.vz) == 0
        && g_battle_camera_rotation_action == BATTLE_CAMERA_ROTATION_NONE && g_battle_action_post_action != 0) {
        battle_state_enter_commence_attack_phase();
    }
}
