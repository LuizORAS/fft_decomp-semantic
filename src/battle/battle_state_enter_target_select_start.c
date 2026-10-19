#include "fft/battle.h"
#include "fft/battle_camera.h"

/* Enter TARGET_SELECT_START: open the command's prompt (an enemy for Auto-Battle command 0xc,
 * an ally for 0xe) and show the map cursor. */
void battle_state_enter_target_select_start(void) {
    battle_unit_t* unit;
    battle_stats_t* stats;

    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_TARGET_SELECT_START;
    unit = battle_unit_get_casting();
    stats = unit->stats;
    battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_TARGETING_PROMPT, 0, stats->battle_id, 0, 1);
    battle_target_show_cursor();
}
