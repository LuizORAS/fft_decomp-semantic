#include "fft/battle.h"

/* Formula 0x01: weapon damage (XA * YA by weapon type) with the weapon's
 * element; physical evade; 19% status. */
void battle_formula_weapon_damage(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        if (battle_formula_calculate_weapon_damage() == 0) {
            battle_formula_apply_status();
        }
    }
}
