#include "fft/battle_action.h"

/* Brave loss of Y: the amount is stored without
 * ACTION_STAT_CHANGE_INCREASE. */
void battle_formula_set_brave_loss_y(void) {
    g_action_target_data->brave_change = g_action_current_ability.range_data.y & ACTION_STAT_CHANGE_VALUE_MASK;
    g_action_target_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
}
