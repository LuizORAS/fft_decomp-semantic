#include "fft/battle_action.h"

/* Formula 0x01: weapon damage (XA * YA by weapon type) with the weapon's
 * element; physical evade; 19% status. */
void formula_weapon_damage(void) {
    if (formula_calculate_physical_evade() == 0) {
        if (formula_calculate_weapon_damage() == 0) {
            formula_apply_status();
        }
    }
}
