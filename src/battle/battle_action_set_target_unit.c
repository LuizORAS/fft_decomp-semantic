#include "fft/battle.h"
#include "psx/types.h"

/* Make unit the action target (g_battle_action_target, its result record,
 * g_current_ability.target_id) and reset its result. Poison, Regen, expiring statuses and the removal
 * of Transparent apply their effects through it. */
void battle_action_set_target_unit(battle_stats_t* unit) {
    battle_action_data_t* action = &unit->action;
    g_battle_action_target = unit;
    g_battle_action_target_data = action;
    g_current_ability.target_id = unit->misc_unit_id;
    battle_action_clear_current_data(action);
}
