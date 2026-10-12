#include "fft/battle_action.h"

/* Formula 0x55: -PA (Y), hit (MA+X)%. */
void battle_formula_lower_pa_y(void) {
    if (battle_formula_calculate_magical_evade() != 0) {
        return;
    }
    if (battle_formula_calculate_magic_accuracy_without_faith() != 0) {
        return;
    }
    g_action_target_data->pa_change = g_action_current_ability.range_data.y & ACTION_STAT_CHANGE_VALUE_MASK;
    g_action_target_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
}
