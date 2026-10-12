#include "fft/battle_action.h"

/* Formula 0x06: weapon damage (XA * YA), absorbed by the attacker as HP
 * (reversed against undead); physical evade; no element. */
void formula_weapon_absorb_hp(void) {
    if (formula_calculate_physical_evade() == 0) {
        formula_calculate_base_xa();
        formula_apply_charge();
        formula_calculate_physical_damage();
        formula_apply_hp_absorption();
    }
}
