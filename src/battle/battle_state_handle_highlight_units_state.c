#include "fft/battle.h"
#include "psx/pad.h"

/* HIGHLIGHT_UNITS: while Square stays held the units keep their team tint and the camera can
 * rotate, zoom and tilt; on release, restore the d-pad mode, the palettes and FREE_CURSOR. */
void battle_state_handle_highlight_units_state(void) {
    if (g_controller_input_released & PSX_PAD_SQUARE) {
        g_battle_controller_input = g_controller_input_copy_12;
        battle_state_enter_free_cursor();
        battle_gfx_update_unit_palettes();
    }
    battle_camera_handle_rotation_input();
    battle_camera_call_zoom_map();
    battle_camera_call_toggle_tilt();
    battle_state_update_units();
}
