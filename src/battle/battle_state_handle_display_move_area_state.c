#include "fft/battle.h"
#include "psx/pad.h"

/* DISPLAY_MOVE_AREA: show a unit's move range from the free cursor, the camera free to rotate,
 * zoom and tilt, until Cross clears the tiles and returns to FREE_CURSOR. */
void battle_state_handle_display_move_area_state(void) {
    if (g_main_input_pressed_buttons & PSX_PAD_CROSS) {
        battle_target_tint_marked_tiles(BATTLE_TILE_MARK_NONE, 1);
        g_battle_cursor_input_mode = g_battle_cursor_input_mode_saved;
        main_sound_play_sfx(MAIN_SFX_CANCEL);
        battle_state_enter_free_cursor();
    }
    battle_camera_handle_rotation_input();
    battle_camera_handle_zoom_input();
    battle_camera_handle_tilt_input();
    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
}
