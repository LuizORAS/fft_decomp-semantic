#include "fft/battle.h"

/* UnitMoving game-state handler: advance the casting unit along its path. When
 * the last step has ended and no animation is pending, refresh its display and
 * open the move confirmation. */
void battle_state_handle_unit_moving_state(void) {
    battle_unit_misc_data_t* casting_misc_data;

    battle_camera_handle_rotation_input();
    battle_camera_call_zoom_map();
    battle_camera_call_toggle_tilt();
    casting_misc_data = battle_unit_get_casting_misc_data();
    battle_move_update_path_step(casting_misc_data);
    if (casting_misc_data->step_phase == BATTLE_MOVE_STEP_PHASE_NONE
        && casting_misc_data->movement_path_offset >= casting_misc_data->path.count
        && g_battle_state_animation_continue_check == 0) {
        battle_unit_update_display_by_misc_id(casting_misc_data->unit_id);
        battle_menu_open_move_confirm();
    }
    battle_state_handle_free_cursor_input();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
}
