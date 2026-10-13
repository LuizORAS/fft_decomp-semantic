#include "fft/battle.h"
#include "psx/pad.h"

/* HighlightUnits game-state handler: releasing square restores the cursor input
 * mode and the unit palettes and returns to the free cursor. */
void battle_state_handle_highlight_units_state(void) {
    if (g_main_input_released_buttons & PSX_PAD_SQUARE) {
        g_battle_cursor_input_mode = g_battle_cursor_input_mode_saved;
        battle_state_set_free_cursor();
        battle_gfx_update_unit_palettes();
    }
    camera_handle_rotation_input();
    camera_update_zoom_input();
    camera_update_tilt_input();
    battle_state_handle_free_cursor_input();
}
