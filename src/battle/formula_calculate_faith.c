#include "fft/battle_action.h"

/* Faith step: HP damage * caster Faith * target Faith / 10000. The Faith
 * status counts as 100 and Innocent as 0; Innocent wins when a unit has both. */
void formula_calculate_faith(void) {
    action_data_t* act;

    if (g_action_attacker->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_FAITH)]
        & STATUS_BYTE_MASK(STATUS_ID_FAITH)) {
        g_action_current_ability.attacker_faith = 100;
    }
    if (g_action_attacker->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_INNOCENT)]
        & STATUS_BYTE_MASK(STATUS_ID_INNOCENT)) {
        g_action_current_ability.attacker_faith = 0;
    }
    if (g_action_target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_FAITH)] & STATUS_BYTE_MASK(STATUS_ID_FAITH)) {
        g_action_current_ability.target_faith = 100;
    }
    if (g_action_target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_INNOCENT)]
        & STATUS_BYTE_MASK(STATUS_ID_INNOCENT)) {
        g_action_current_ability.target_faith = 0;
    }

    act = g_action_target_data;
    act->hp_damage
        = act->hp_damage * g_action_current_ability.target_faith * g_action_current_ability.attacker_faith / 10000;
}
