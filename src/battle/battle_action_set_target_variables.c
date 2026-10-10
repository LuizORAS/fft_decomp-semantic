#include "fft/battle.h"
#include "psx/types.h"

/* Make the unit the current target and reset its action result record. */
void battle_action_set_target_variables(battle_stats_t* unit) {
    battle_action_data_t* action = &unit->action;
    g_battle_action_target = unit;
    g_battle_action_target_data = action;
    g_battle_action_current_ability.target_id = unit->misc_unit_id;
    battle_action_clear_current_data(action);
}
