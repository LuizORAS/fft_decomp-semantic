#include "fft/battle.h"
#include "psx/types.h"

/* CHANGE_TURN: once the event check has answered and the camera has stopped, start the turn
 * event: a charged ability (ACTION_CAST), an action result or Mime event (STATUS_EXECUTE), or a
 * unit's turn (its action menus). */
void battle_state_handle_change_turn_state(void) {
    s32 command;
    s32 type;

    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command = *battle_menu_get_selected_command_address();
    if (command >= 7) {
        if (command < 9) {
            g_battle_action_post_action = 1;
        } else if (command == 0xFF) {
            g_battle_action_post_action = 1;
        }
    }
    if (g_battle_action_post_action != 0) {
        if ((g_battle_camera_step_vector.vx | g_battle_camera_step_vector.vy | g_battle_camera_step_vector.vz) == 0) {
            if (g_battle_camera_rotation_action == BATTLE_CAMERA_ROTATION_NONE) {
                battle_unit_get_source_misc_data();
                type = g_battle_turn_event;
                if (type == BATTLE_TURN_EVENT_ABILITY_READY) {
                    battle_state_enter_action_cast();
                    return;
                }
                if ((type == BATTLE_TURN_EVENT_ACTION_RESULT) || (type == BATTLE_TURN_EVENT_MIME)) {
                    battle_state_enter_status_execute();
                    return;
                }
                battle_state_enter_open_action_menus();
            }
        }
    }
}
