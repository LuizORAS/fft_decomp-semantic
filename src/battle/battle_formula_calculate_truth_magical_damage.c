#include "fft/battle.h"
#include "psx/types.h"

/* Magical damage without Faith: elemental strengthen, magical modifiers, XA *
 * YA with weather and element, absorption, 19% status. */
void battle_formula_calculate_truth_magical_damage(void) {
    battle_formula_apply_elemental_strengthen();
    battle_formula_apply_magical_support_status_compatibility();
    if (battle_formula_calculate_elemental_xa_times_ya() == 0) {
        battle_formula_apply_elemental_absorption_and_status();
    }
}
