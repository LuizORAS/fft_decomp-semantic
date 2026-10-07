#include "fft/battle.h"

/* volatile: the target reloads g_current_attacker before each status test
 * instead of keeping it in a register. */
extern battle_stats_t* volatile g_battle_action_attacker;

extern battle_stats_t* volatile g_battle_action_target;

/* Scale the stored amount by both Faiths: amount * target Faith * attacker Faith / 10000, with the
 * Faith status counting as 100 and Innocent as 0 (Innocent wins). The amount is the HP damage or
 * healing, or the hit chance that the accuracy steps keep in hp_damage. */
void battle_formula_calculate_faith(void) {
    battle_action_data_t* action;

    if (g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FAITH)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FAITH)) {
        g_current_ability.attacker_faith = 100;
    }
    if (g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_INNOCENT)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_INNOCENT)) {
        g_current_ability.attacker_faith = 0;
    }
    if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FAITH)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FAITH)) {
        g_current_ability.target_faith = 100;
    }
    if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_INNOCENT)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_INNOCENT)) {
        g_current_ability.target_faith = 0;
    }

    action = g_battle_action_target_data;
    action->hp_damage = action->hp_damage * g_current_ability.target_faith * g_current_ability.attacker_faith / 10000;
}
