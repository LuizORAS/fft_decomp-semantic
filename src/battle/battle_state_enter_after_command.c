#include "fft/battle.h"

/* Enter AFTER_COMMAND at 60 fps: move the cursor to the source unit and open no window
 * (command 6 runs system function 8, the one used when a window is turned off). */
void battle_state_enter_after_command(void) {
    g_battle_state_vsync_interval = 1;
    g_battle_game_state = BATTLE_GAME_STATE_AFTER_COMMAND;
    g_battle_action_post_action = 0;
    battle_target_move_cursor_to_unit(battle_unit_get_source_misc_data());
    battle_menu_init_system_function(6, 0, 0, 0, 0);
}
