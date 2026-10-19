#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Damage of a weapon strike (formulas 0x01 and 0x02): XA and YA by weapon type
 * (battle_formula_calculate_base_xa), Charge's power, the weapon element's strengthen, the
 * physical modifiers, a critical hit and XA * YA, then the element's effect on the damage.
 * Returns 1 on a miss, also when the element cancels the hit, else the status roll's result
 * (battle_formula_roll_conditional_status_proc: 0 when the added effect applies). */
s32 battle_formula_calculate_weapon_damage(void) {
    battle_formula_calculate_base_xa();
    battle_formula_apply_charge();
    battle_formula_apply_weapon_element_strengthen();
    battle_formula_calculate_physical_damage();
    battle_formula_apply_weapon_element();
    if (g_battle_action_target_data->hit == 0)
        return 1;
    battle_formula_apply_elemental_absorption();
    return battle_formula_roll_conditional_status_proc();
}
