#include "fft/battle.h"
#include "fft/battle_camera.h"

/* Enter the target-select state: show the panel for the cursor tile and tint the
 * units the casting unit's selection command may pick, the other teams' red for a
 * targeted selection and its own team's blue for a recovery selection. */
void battle_state_enter_target_select(void) {
    camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_TARGET_SELECT;
    target_store_cursor_unit_name_and_data();
    battle_gfx_highlight_all_units_blue_or_red(battle_unit_get_casting_misc_data()->target_select_command);
}
