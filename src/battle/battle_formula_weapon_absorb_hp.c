#include "fft/battle.h"

/* Formula 0x06, Blood Sword and Bloody Strings: the physical evade check, XA and YA by weapon type,
 * Charge's power and the physical damage, which the attacker drains
 * (battle_formula_apply_hp_absorption). No element or status. */
void battle_formula_weapon_absorb_hp(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        battle_formula_calculate_base_xa();
        battle_formula_apply_charge();
        battle_formula_calculate_physical_damage();
        battle_formula_apply_hp_absorption();
    }
}
