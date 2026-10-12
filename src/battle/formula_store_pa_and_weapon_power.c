#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* XA = PA, YA = the weapon's power. */
void formula_store_pa_and_weapon_power(void) {
    s32 pa = g_action_attacker->attributes[UNIT_ATTRIBUTE_PHYSICAL_ATTACK];
    s32 weapon_power = g_action_current_ability.weapon_data.power;

    g_action_current_ability.ya = weapon_power;
    g_action_current_ability.xa = pa;
}
