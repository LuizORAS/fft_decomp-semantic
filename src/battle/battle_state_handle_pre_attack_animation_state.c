#include "fft/battle.h"

/* PreAttackAnimation game-state handler: execute the ability once the source
 * unit's animation has ended or looped, or its state frame counter has reached
 * 0x3d, and the camera has settled. */
void battle_state_handle_pre_attack_animation_state(void) {
    battle_unit_misc_data_t* unit;

    battle_state_handle_free_cursor_input();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    unit = battle_unit_get_source_misc_data();
    if ((unit->animation.wait == 0 || unit->animation.restart_count != 0 || unit->state_frame_counter >= 0x3d)
        && (g_battle_current_vector.vx | g_battle_current_vector.vy | g_battle_current_vector.vz) == 0
        && g_battle_camera_rotation_action == BATTLE_CAMERA_ROTATION_NONE) {
        battle_action_execute_ability();
    }
}
