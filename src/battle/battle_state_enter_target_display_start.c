#include "fft/battle.h"

/* Enter TARGET_DISPLAY_START: store empty unit names, open the command's message and show the
 * map cursor. */
void battle_state_enter_target_display_start(void) {
    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_TARGET_DISPLAY_START;
    battle_menu_store_unit_names_and_event_block_data(1, 0xFF, 0xFF);
    battle_menu_init_system_function(1, 0, battle_unit_get_casting_misc_data()->battle_data->misc_unit_id, 0, 1);
    g_battle_action_post_action = 0;
    battle_target_show_cursor();
}
