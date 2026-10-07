#include "fft/battle.h"

/* Formula 0x05: a weapon strike without the weather and night penalty on bows
 * (battle_formula_calculate_physical_evade_without_weather), the weapon's element or its Strengthen:
 * XA and YA by weapon type, Charge's power, the physical damage and, on the 19% roll, the added
 * status. No retail weapon, ability or item uses formula 5; Charge strikes with the weapon's own
 * formula and only adds its power (battle_formula_apply_charge). */
void battle_formula_weapon_damage_without_element(void) {
    if (battle_formula_calculate_physical_evade_without_weather() == 0) {
        battle_formula_calculate_base_xa();
        battle_formula_apply_charge();
        battle_formula_calculate_physical_damage();
        if (battle_formula_roll_conditional_status_proc() == 0) {
            battle_formula_apply_status();
        }
    }
}
