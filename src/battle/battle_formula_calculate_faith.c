#include "fft/battle.h"

/* Faith step: HP damage * caster Faith * target Faith / 10000. The Faith
 * status counts as 100 and Innocent as 0; Innocent wins when a unit has both. */
/* Scale the stored amount by both Faiths: amount * target Faith * attacker Faith / 10000, with the
 * Faith status counting as 100 and Innocent as 0 (Innocent wins). The amount is the HP damage or
 * healing, or the hit chance that the accuracy steps keep in hp_damage. */
void battle_formula_calculate_faith(void) {
    battle_action_data_t* action;

    if (g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FAITH)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FAITH)) {
        g_battle_action_current_ability.attacker_faith = 100;
    }
    if (g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_INNOCENT)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_INNOCENT)) {
        g_battle_action_current_ability.attacker_faith = 0;
    }
    if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FAITH)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FAITH)) {
        g_battle_action_current_ability.target_faith = 100;
    }
    if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_INNOCENT)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_INNOCENT)) {
        g_battle_action_current_ability.target_faith = 0;
    }

    action = g_battle_action_target_data;
    action->hp_damage = action->hp_damage * g_battle_action_current_ability.target_faith
        * g_battle_action_current_ability.attacker_faith / 10000;
}
