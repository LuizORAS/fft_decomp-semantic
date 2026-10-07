#include "fft/battle.h"

/* Formula 0x17, Gravi 2: the magical evade check and hit chance (MA + X with both Faiths, no
 * Strengthen), then damage of the target's current HP - 1 (battle_formula_calculate_damage_leaving_one_hp). */
void battle_formula_damage_target_hp_minus_one(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy_without_strengthen() == 0) {
            battle_formula_calculate_damage_leaving_one_hp();
        }
    }
}
