#include "fft/battle_action.h"

/* Formula 0x3A: +Brave (Y). */
void battle_formula_add_brave_y(void) {
    g_action_target_data->brave_change = g_action_current_ability.range_data.y | ACTION_STAT_CHANGE_INCREASE;
    g_action_target_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
}
