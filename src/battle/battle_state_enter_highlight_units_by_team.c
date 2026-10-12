#include "fft/battle.h"
#include "fft/battle_camera.h"

/* Enter the highlight-units state: tint every unit by its team, save the cursor
 * input mode and switch to the free cursor with map panning. */
void battle_state_enter_highlight_units_by_team(void) {
    camera_enable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_HIGHLIGHT_UNITS;
    g_battle_cursor_input_mode_saved = g_battle_cursor_input_mode;
    g_battle_cursor_input_mode = BATTLE_CURSOR_INPUT_FREE_CURSOR;
    battle_gfx_tint_all_units_by_team();
    action_clear_at_list_active();
}
