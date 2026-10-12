#include "fft/battle_formula.h"

/* Formula 0x30: absorb (PA * WP) HP; physical evade. */
void formula_absorb_hp_pa_times_wp(void) {
    if (formula_calculate_physical_evade() == 0) {
        formula_store_pa_and_weapon_power();
        formula_calculate_physical_damage();
        formula_apply_hp_absorption();
    }
}
