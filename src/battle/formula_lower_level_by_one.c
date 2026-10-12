#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x59: -Level (1), hit (MA+X)%. */
void formula_lower_level_by_one(void) {
    action_data_t* action;

    if (formula_calculate_magical_evade() == 0) {
        if (formula_calculate_magic_accuracy_without_faith() == 0) {
            if (g_action_target->level < 2) {
                formula_force_attack_miss();
            }
            action = g_action_target_data;
            /* The target writes the combined special-effect field as one 16-bit value. */
            action->special_effect = ACTION_SPECIAL_EFFECT_LEVEL_DOWN;
            action->attack_type = ACTION_TYPE_PSEUDO_STATUS;
        }
    }
}
