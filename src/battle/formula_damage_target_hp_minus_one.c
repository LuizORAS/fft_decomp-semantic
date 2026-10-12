#include "fft/battle_formula.h"

/* Formula 0x17: damage equal to the target's current HP - 1, hit F(MA+X)%;
 * magic evade. */
void formula_damage_target_hp_minus_one(void) {
    if (formula_calculate_magical_evade() == 0) {
        if (formula_calculate_magic_accuracy_no_elemental() == 0) {
            formula_calculate_gravi2_damage();
        }
    }
}
