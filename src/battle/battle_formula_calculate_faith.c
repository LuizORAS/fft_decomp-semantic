#include "fft/battle.h"

/* Faith step: HP damage * caster Faith * target Faith / 10000. The Faith
 * status counts as 100 and Innocent as 0; Innocent wins when a unit has both. */
void battle_formula_calculate_faith(void) {
    battle_action_data_t* act;

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

    act = g_battle_action_target_data;
    act->hp_damage = act->hp_damage * g_battle_action_current_ability.target_faith
        * g_battle_action_current_ability.attacker_faith / 10000;
}
