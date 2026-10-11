#include "fft/battle.h"

/* Enter HIGHLIGHT_UNITS: let the d-pad pan the camera (saving the d-pad mode), tint every unit
 * by team and hide the map cursor. */
void battle_state_enter_highlight_units_by_team(void) {
    battle_state_enable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_HIGHLIGHT_UNITS;
    g_battle_cursor_input_mode_saved = g_battle_cursor_input_mode;
    g_battle_cursor_input_mode = BATTLE_CURSOR_INPUT_FREE_CURSOR;
    battle_gfx_tint_all_units_by_team();
    battle_target_hide_cursor();
}
