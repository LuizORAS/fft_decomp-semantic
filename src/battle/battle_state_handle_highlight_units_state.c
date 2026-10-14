#include "fft/battle.h"
#include "psx/pad.h"

/* HIGHLIGHT_UNITS: while Square stays held the units keep their team tint and the camera can
 * rotate, zoom and tilt; on release, restore the d-pad mode, the palettes and FREE_CURSOR. */
void battle_state_handle_highlight_units_state(void) {
    if (g_main_input_released_buttons & PSX_PAD_SQUARE) {
        g_battle_cursor_input_mode = g_battle_cursor_input_mode_saved;
        battle_state_enter_free_cursor();
        battle_gfx_update_unit_palettes();
    }
    battle_camera_handle_rotation_input();
    battle_camera_handle_zoom_input();
    battle_camera_handle_tilt_input();
    battle_state_update_units();
}
