#include "fft/battle_formula.h"

/* Formula 0x05: weapon damage as 0x01 without the weapon's element and the bow
 * weather penalty; physical evade; 19% status. */
void formula_weapon_damage_charge(void) {
    if (formula_calculate_physical_evade_charge() == 0) {
        formula_calculate_base_xa();
        formula_apply_charge();
        formula_calculate_physical_damage();
        if (formula_roll_conditional_status_proc() == 0) {
            formula_apply_status();
        }
    }
}
