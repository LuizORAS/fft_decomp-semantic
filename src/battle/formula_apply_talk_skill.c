#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Effect of each talk skill: Negotiate takes gil, Persuade zeroes CT, Praise
 * and Threaten change Brave by Y, Preach and Solution change Faith by Y; the
 * rest inflict their status. */
void formula_apply_talk_skill(void) {
    switch (g_action_current_ability.ability_id) {
    case ABILITY_ID_TALK_NEGOTIATE:
        formula_calculate_stolen_gil();
        return;
    case ABILITY_ID_TALK_PERSUADE:
        g_action_target_data->ct_change = ACTION_CT_CHANGE_ZERO;
        break;
    case ABILITY_ID_TALK_PRAISE:
        g_action_target_data->brave_change = g_action_current_ability.range_data.y | ACTION_STAT_CHANGE_INCREASE;
        break;
    case ABILITY_ID_TALK_THREATEN:
        g_action_target_data->brave_change = g_action_current_ability.range_data.y & ACTION_STAT_CHANGE_VALUE_MASK;
        break;
    case ABILITY_ID_TALK_PREACH:
        g_action_target_data->faith_change = g_action_current_ability.range_data.y | ACTION_STAT_CHANGE_INCREASE;
        break;
    case ABILITY_ID_TALK_SOLUTION:
        g_action_target_data->faith_change = g_action_current_ability.range_data.y & ACTION_STAT_CHANGE_VALUE_MASK;
        break;
    default:
        formula_apply_status_to_action();
        return;
    }
    g_action_target_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
}
