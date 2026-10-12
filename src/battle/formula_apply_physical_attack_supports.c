#include "fft/battle_action.h"

/* Attacker supports on XA, in order: Two Hands * 2 (active, on a weapon that
 * allows it and is not two-handed already), Attack Up * 4/3, Martial Arts *
 * 3/2 with no weapon (weapon id ITEM_ID_NOTHING). */
void formula_apply_physical_attack_supports(void) {
    u8 weapon_characteristics = g_action_current_ability.weapon_data.flags;

    if (!(weapon_characteristics & WEAPON_FLAG_FORCED_TWO_HANDS)
        && (weapon_characteristics & WEAPON_FLAG_TWO_HANDS_COMPATIBLE) && g_action_current_ability.two_hands_active) {
        g_action_current_ability.xa = g_action_current_ability.xa * 2;
    }

    if (g_action_attacker->support_abilities[1] & BATTLE_SUPPORT_SET_2_ATTACK_UP) {
        g_action_current_ability.xa = g_action_current_ability.xa * 4 / 3;
    }

    if (g_action_current_ability.weapon_id == ITEM_ID_NOTHING
        && (g_action_attacker->support_abilities[2] & BATTLE_SUPPORT_SET_3_MARTIAL_ARTS)) {
        g_action_current_ability.xa = g_action_current_ability.xa * 3 / 2;
    }
}
