#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "fft/unit.h"

/* Formula 0x15: CT to 0, hit F(MA+X)%; magic evade. */
void formula_set_ct_zero(void) {
    if (formula_calculate_magical_evade() == 0) {
        if (formula_calculate_magic_accuracy_no_elemental() == 0) {
            g_action_target_data->ct_change = ACTION_CT_CHANGE_ZERO;
            g_action_target_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
        }
    }
}
