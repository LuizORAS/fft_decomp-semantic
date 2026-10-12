#include "fft/battle_action.h"

/* Formula 0x56: -MA (Y), hit (MA+X)%. */
void formula_lower_ma_y(void) {
    if (formula_calculate_magical_evade() != 0) {
        return;
    }
    if (formula_calculate_magic_accuracy_without_faith() != 0) {
        return;
    }
    g_action_target_data->ma_change = g_action_current_ability.range_data.y & ACTION_STAT_CHANGE_VALUE_MASK;
    g_action_target_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
}
