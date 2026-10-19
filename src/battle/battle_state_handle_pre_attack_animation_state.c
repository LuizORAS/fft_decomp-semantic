#include "fft/battle.h"
#include "fft/battle_camera.h"

/* PRE_ATTACK_ANIMATION: once the source unit's animation ends (or its counter reaches 61) and
 * the camera has stopped, run the ability (battle_action_execute_ability): it starts charging,
 * acts now or jumps. */
void battle_state_handle_pre_attack_animation_state(void) {
    battle_unit_t* unit;

    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    unit = battle_unit_get_source();
    if ((unit->animation.wait == 0 || unit->animation.restart_count != 0 || unit->state_frame_counter >= 0x3d)
        && (g_battle_camera_step_vector.vx | g_battle_camera_step_vector.vy | g_battle_camera_step_vector.vz) == 0
        && g_battle_camera_rotation_action == BATTLE_CAMERA_ROTATION_NONE) {
        battle_action_execute_ability();
    }
}
