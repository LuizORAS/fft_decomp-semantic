#include "fft/battle.h"
#include "psx/types.h"

/* Enter ILLEGAL_RANGE and open the out-of-range message. */
void battle_state_enter_illegal_range(void) {
    battle_unit_misc_data_t* unit;
    battle_stats_t* stats;

    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_ILLEGAL_RANGE;
    unit = battle_unit_get_source_misc_data();
    stats = unit->battle_data;
    battle_menu_init_system_function(
        4, 5, stats->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
}
