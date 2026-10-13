#include "fft/battle_action.h"

/* Make the unit the current target and reset its action result record. */
void action_set_target_variables(unit_stats_t* unit) {
    action_data_t* action = &unit->action;
    g_action_target = unit;
    g_action_target_data = action;
    g_action_current_ability.target_id = unit->battle_id;
    action_clear_current_data(action);
}
