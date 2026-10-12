#include "fft/battle_action.h"

void battle_formula_apply_quick_effect(void) {
    g_action_target_data->ct_change = ACTION_CT_CHANGE_QUICK;
    g_action_target_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
}
