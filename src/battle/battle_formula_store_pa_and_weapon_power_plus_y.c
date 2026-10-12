#include "fft/battle_action.h"

/* XA = PA, YA = the weapon's power + Y. */
void battle_formula_store_pa_and_weapon_power_plus_y(void) {
    s32 pa = g_battle_action_attacker->attributes[UNIT_ATTRIBUTE_PHYSICAL_ATTACK];
    s32 weapon_power = g_battle_action_current_ability.weapon_data.power;
    s32 y = g_battle_action_current_ability.range_data.y;

    g_battle_action_current_ability.ya = weapon_power + y;
    g_battle_action_current_ability.xa = pa;
}
