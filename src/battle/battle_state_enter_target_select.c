#include "fft/battle.h"
#include "psx/types.h"

/* Enter TARGET_SELECT: store the cursor unit's name and data and highlight the units for the
 * command (battle_gfx_highlight_all_units_blue_or_red). */
void battle_state_enter_target_select(void) {
    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_TARGET_SELECT;
    battle_target_show_cursor_unit_panel();
    battle_gfx_highlight_all_units_blue_or_red(battle_unit_get_casting_misc_data()->target_select_command);
}
