#include "fft/battle_action.h"
#include "psx/types.h"

/* Formula 0x04: magic gun. Casts the first, second or third spell of the gun's
 * element (60/30/10%); damage F(WP * that spell's Y) with its element; no
 * evade. */
void battle_formula_magic_gun(void) {
    u16 weapon_power;
    u16 ability_y;

    battle_formula_select_magic_gun_ability();
    weapon_power = g_battle_action_current_ability.weapon_data.power;
    ability_y = g_battle_action_current_ability.range_data.y;
    g_battle_action_current_ability.xa = weapon_power;
    g_battle_action_current_ability.ya = ability_y;
    battle_formula_apply_charge();
    battle_formula_apply_weapon_element_strengthen();
    battle_formula_apply_magical_support_status_compatibility();
    if (battle_formula_calculate_elemental_xa_times_ya() == 0) {
        battle_formula_calculate_faith();
        battle_formula_apply_elemental_absorption();
    }
}
