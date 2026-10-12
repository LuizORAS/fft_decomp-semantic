#include "fft/battle_formula.h"
/* Formula 0x10: absorb (Y)% of max HP, hit F(MA+X)%; magic evade. */
void formula_magic_hp_absorb(void) {
    if (formula_calculate_magical_evade() == 0) {
        if (formula_calculate_magic_accuracy_no_elemental() == 0) {
            formula_calculate_hp_percent_damage();
            formula_apply_hp_absorption();
        }
    }
}
