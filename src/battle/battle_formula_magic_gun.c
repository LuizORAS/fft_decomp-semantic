#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x04, the magic guns (Blaze Gun, Glacier Gun, Blast Gun): cast a Fire, Ice or Bolt spell
 * of a random level (battle_formula_select_magic_gun_ability) with XA = the gun's power and YA = the
 * spell's Y: Charge's power, the gun element's Strengthen, the magical XA modifiers, XA * YA with the
 * weather and the spell's element, both Faiths and the elemental absorption. No evade or hit roll. */
void battle_formula_magic_gun(void) {
    u16 weapon_power;
    u16 ability_y;

    battle_formula_select_magic_gun_ability();
    weapon_power = g_current_ability.weapon_data.power;
    ability_y = g_current_ability.range_data.y;
    g_current_ability.xa = weapon_power;
    g_current_ability.ya = ability_y;
    battle_formula_apply_charge();
    battle_formula_apply_weapon_element_strengthen();
    battle_formula_apply_magical_xa_modifiers();
    if (battle_formula_calculate_elemental_xa_times_ya() == 0) {
        battle_formula_calculate_faith();
        battle_formula_apply_elemental_absorption();
    }
}
