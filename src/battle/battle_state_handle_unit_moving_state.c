#include "fft/battle.h"

/* UNIT_MOVING: walk the casting unit along its path, the camera free to rotate, zoom and tilt;
 * at the destination, refresh its display and open the move confirmation. */
void battle_state_handle_unit_moving_state(void) {
    battle_unit_t* casting_unit;

    battle_camera_handle_rotation_input();
    battle_camera_handle_zoom_input();
    battle_camera_handle_tilt_input();
    casting_unit = battle_unit_get_casting();
    battle_move_update_path_step(casting_unit);
    if (casting_unit->step_phase == BATTLE_MOVE_STEP_PHASE_NONE
        && casting_unit->movement_path_offset >= casting_unit->path.count
        && g_battle_state_animation_continue_check == 0) {
        battle_unit_update_display_by_runtime_id(casting_unit->runtime_id);
        battle_state_enter_move_confirm_menu();
    }
    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
}
