#include "fft/battle_action.h"
#include "psx/types.h"

/* Damage of formulas 0x01 and 0x02: XA and YA by weapon type, charge power,
 * weapon element strengthen, physical modifiers, critical roll, XA * YA, then
 * the target's affinity to the weapon's elements. Returns 0 when the added
 * effect applies (battle_formula_roll_conditional_status_proc), 1 when it does
 * not or the element cancelled the attack. */
s32 battle_formula_calculate_weapon_damage(void) {
    battle_formula_calculate_base_xa();
    battle_formula_apply_charge();
    battle_formula_apply_weapon_element_strengthen();
    battle_formula_calculate_physical_damage();
    battle_formula_modify_elemental_damage();
    if (g_battle_action_target_data->hit == 0)
        return 1;
    battle_formula_apply_elemental_absorption();
    return battle_formula_roll_conditional_status_proc();
}
