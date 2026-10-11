#include "fft/battle.h"

/* Enter TARGET_DISPLAY_START: store empty unit names, open the command's message and show the
 * map cursor. */
void battle_state_enter_target_display_start(void) {
    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_TARGET_DISPLAY_START;
    battle_unit_start_view_thread(UNIT_VIEW_MODE_TILE_INFO, 0xFF, 0xFF);
    battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_TARGETING_PROMPT, 0,
        battle_unit_get_casting_misc_data()->battle_data->misc_unit_id, 0, 1);
    g_battle_action_post_action = 0;
    battle_target_show_cursor();
}
