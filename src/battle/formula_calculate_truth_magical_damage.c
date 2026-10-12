#include "fft/battle_action.h"

/* Magical damage without Faith: elemental strengthen, magical modifiers, XA *
 * YA with weather and element, absorption, 19% status. */
void formula_calculate_truth_magical_damage(void) {
    formula_apply_elemental_strengthen();
    formula_apply_magical_support_status_compatibility();
    if (formula_calculate_elemental_xa_times_ya() == 0) {
        formula_apply_elemental_absorption_and_status();
    }
}
