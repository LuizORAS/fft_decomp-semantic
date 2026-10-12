#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Formula 0x04: magic gun. Casts the first, second or third spell of the gun's
 * element (60/30/10%); damage F(WP * that spell's Y) with its element; no
 * evade. */
void formula_magic_gun(void) {
    u16 weapon_power;
    u16 ability_y;

    formula_select_magic_gun_ability();
    weapon_power = g_action_current_ability.weapon_data.power;
    ability_y = g_action_current_ability.range_data.y;
    g_action_current_ability.xa = weapon_power;
    g_action_current_ability.ya = ability_y;
    formula_apply_charge();
    formula_apply_weapon_element_strengthen();
    formula_apply_magical_support_status_compatibility();
    if (formula_calculate_elemental_xa_times_ya() == 0) {
        formula_calculate_faith();
        formula_apply_elemental_absorption();
    }
}
