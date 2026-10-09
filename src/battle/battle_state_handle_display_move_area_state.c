#include "fft/battle.h"
#include "psx/pad.h"

void battle_state_handle_display_move_area_state(void) {
    if (g_main_input_pressed_buttons & PSX_PAD_CROSS) {
        battle_target_set_tile_background_color(0, 1);
        g_battle_controller_input = g_battle_controller_input_copy_12;
        main_sound_play_sfx(MAIN_SFX_CANCEL);
        battle_state_set_free_cursor();
    }
    battle_camera_handle_rotation_input();
    battle_camera_call_zoom_map();
    battle_camera_call_toggle_tilt();
    battle_state_handle_free_cursor_input();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
}
