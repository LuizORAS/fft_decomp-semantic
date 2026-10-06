#include "fft/battle.h"

/* Formula 0x01, the strike of 112 weapons and of bare hands, and monster attacks such as Choco
 * Attack, Tackle and Bite: the physical evade check, the weapon damage
 * (battle_formula_calculate_weapon_damage) and, on the 19% roll, the added status
 * (battle_formula_apply_status). */
void battle_formula_weapon_damage(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        if (battle_formula_calculate_weapon_damage() == 0) {
            battle_formula_apply_status();
        }
    }
}
