#include "fft/battle.h"

/* Formula 0x05: weapon damage as 0x01 without the weapon's element and the bow
 * weather penalty; physical evade; 19% status. */
void battle_formula_weapon_damage_charge(void) {
    if (battle_formula_calculate_physical_evade_charge() == 0) {
        battle_formula_calculate_base_xa();
        battle_formula_apply_charge();
        battle_formula_calculate_physical_damage();
        if (battle_formula_roll_conditional_status_proc() == 0) {
            battle_formula_apply_status();
        }
    }
}
