#include "fft/battle_action.h"

/* Formula 0x2F: absorb (PA * WP) MP; physical evade. */
void formula_absorb_mp_pa_times_wp(void) {
    if (formula_calculate_physical_evade() == 0) {
        formula_store_pa_and_weapon_power();
        formula_calculate_physical_damage();
        formula_apply_mp_recovery();
    }
}
