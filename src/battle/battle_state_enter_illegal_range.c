#include "fft/battle.h"
#include "fft/battle_camera.h"

/* Enter ILLEGAL_RANGE, reached when the chosen tile lacks the ability-range mark, and open the
 * out-of-range message (action-confirm option 5) for the source unit. */
void battle_state_enter_illegal_range(void) {
    battle_unit_t* unit;
    battle_stats_t* stats;

    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_ILLEGAL_RANGE;
    unit = battle_unit_get_source();
    stats = unit->stats;
    battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_ACTION_CONFIRM, 5, stats->battle_id, 0,
        unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
}
