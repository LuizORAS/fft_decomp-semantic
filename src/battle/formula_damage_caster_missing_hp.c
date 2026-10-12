#include "fft/battle_formula.h"

/* Formula 0x4F: damage equal to the caster's missing HP, hit (MA+X)%; physical
 * evade. */
void formula_damage_caster_missing_hp(void) {
    if (formula_calculate_physical_evade() == 0) {
        formula_store_ma_and_x();
        if (formula_calculate_physical_status_accuracy() == 0) {
            formula_43_damage_caster_missing_hp();
        }
    }
}
