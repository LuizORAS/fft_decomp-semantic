#include "fft/battle_formula.h"

/* Formula 0x37: damage (PA * rnd(1..Y)); physical evade. Knockback when a roll
 * below the target's Brave is under a roll below the attacker's Brave. */
void formula_damage_random_y_times_pa(void) {
    if (formula_calculate_physical_evade() == 0) {
        formula_store_pa_and_y();
        formula_apply_attack_up_and_martial_arts();
        formula_apply_physical_status_support_compatibility();
        formula_apply_damage_and_knockback();
    }
}
