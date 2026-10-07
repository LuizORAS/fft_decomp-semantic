#include "fft/battle.h"

/* The weapon-strike supports on XA: Two Hands doubles it (a weapon that allows two hands without
 * forcing them, with Two Hands active), Attack Up multiplies it by 4 / 3, and Martial Arts by 3 / 2
 * when no weapon is held. */
void battle_formula_apply_physical_attack_supports(void) {
    u8 weapon_characteristics = g_current_ability.weapon_data.flags;

    if (!(weapon_characteristics & WEAPON_FLAG_FORCED_TWO_HANDS)
        && (weapon_characteristics & WEAPON_FLAG_TWO_HANDS_COMPATIBLE) && g_current_ability.two_hands_active) {
        g_current_ability.xa = (s16)g_current_ability.xa * 2;
    }

    if (g_battle_action_attacker->support_abilities[1] & BATTLE_SUPPORT_SET_2_ATTACK_UP) {
        s16* xa = (s16*)&g_current_ability.xa;

        *xa = *xa * 4 / 3;
    }

    if (g_current_ability.weapon_id == ITEM_ID_NOTHING
        && (g_battle_action_attacker->support_abilities[2] & BATTLE_SUPPORT_SET_3_MARTIAL_ARTS)) {
        g_current_ability.xa = (s16)g_current_ability.xa * 3 / 2;
    }
}
