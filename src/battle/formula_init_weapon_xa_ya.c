#include "fft/battle_action.h"

/* XA and YA of formula 0x64 (Jump): XA = PA, or PA * 3/2 with a polearm; YA =
 * weapon power, or bare-handed XA * Brave / 100 (at least 1). */
void formula_init_weapon_xa_ya(void) {
    u8 weapon_type = g_main_item_primary_data[g_action_current_ability.weapon_id].type;

    if (weapon_type == ITEM_TYPE_SPEAR) {
        g_action_current_ability.xa = g_action_attacker->attributes[UNIT_ATTRIBUTE_PHYSICAL_ATTACK] * 3 / 2;
    } else {
        g_action_current_ability.xa = g_action_attacker->attributes[UNIT_ATTRIBUTE_PHYSICAL_ATTACK];
    }

    if (weapon_type == ITEM_TYPE_UNARMED) {
        g_action_current_ability.ya = g_action_current_ability.xa * g_action_attacker->brave / 100;
        if (g_action_current_ability.ya == 0) {
            g_action_current_ability.ya++;
        }
    } else {
        g_action_current_ability.ya = g_action_current_ability.weapon_data.power;
    }
}
