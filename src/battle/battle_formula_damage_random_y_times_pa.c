#include "fft/battle.h"

/* Formula 0x37, Dash, Throw Stone, Cat Kick and Tail Swing: the physical evade check; XA = PA with
 * Attack Up and Martial Arts and the physical status modifiers; damage of a random 1 to Y times XA,
 * and a Brave contest may knock the target back (battle_formula_apply_damage_and_knockback). */
void battle_formula_damage_random_y_times_pa(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        battle_formula_store_pa_and_y();
        battle_formula_apply_attack_up_and_martial_arts();
        battle_formula_apply_physical_status_xa_modifiers();
        battle_formula_apply_damage_and_knockback();
    }
}
