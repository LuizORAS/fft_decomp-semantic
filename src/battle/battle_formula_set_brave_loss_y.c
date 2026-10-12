#include "fft/battle_action.h"

/* Brave loss of Y: the amount is stored without
 * BATTLE_ACTION_STAT_CHANGE_INCREASE. */
void battle_formula_set_brave_loss_y(void) {
    g_battle_action_target_data->brave_change
        = g_battle_action_current_ability.range_data.y & BATTLE_ACTION_STAT_CHANGE_VALUE_MASK;
    g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
}
