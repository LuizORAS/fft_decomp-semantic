#include "fft/battle.h"
/* Formula 0x10: absorb (Y)% of max HP, hit F(MA+X)%; magic evade. */
void battle_formula_magic_hp_absorb(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy_no_elemental() == 0) {
            battle_formula_calculate_hp_percent_damage();
            battle_formula_apply_hp_absorption();
        }
    }
}
