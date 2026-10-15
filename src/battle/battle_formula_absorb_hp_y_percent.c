#include "fft/battle.h"

/* Formula 0x10, Life Drain and Drain: the magical evade check and hit chance (MA + X with both
 * Faiths, no Strengthen), then Y% of the target's max HP drained to the attacker, the other way
 * round for an undead target (battle_formula_apply_hp_absorption). */
void battle_formula_absorb_hp_y_percent(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy_without_strengthen() == 0) {
            battle_formula_calculate_hp_percent_damage();
            battle_formula_apply_hp_absorption();
        }
    }
}
