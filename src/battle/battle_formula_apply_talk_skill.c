#include "fft/battle.h"
#include "psx/types.h"

void battle_formula_apply_talk_skill(void) {
    switch (g_battle_action_current_ability.ability_id) {
    case ABILITY_ID_TALK_NEGOTIATE:
        battle_formula_calculate_stolen_gil();
        return;
    case ABILITY_ID_TALK_PERSUADE:
        g_battle_action_target_data->ct_change = BATTLE_ACTION_CT_CHANGE_ZERO;
        break;
    case ABILITY_ID_TALK_PRAISE:
        g_battle_action_target_data->brave_change
            = g_battle_action_current_ability.range_data.y | BATTLE_ACTION_STAT_CHANGE_INCREASE;
        break;
    case ABILITY_ID_TALK_THREATEN:
        g_battle_action_target_data->brave_change
            = g_battle_action_current_ability.range_data.y & BATTLE_ACTION_STAT_CHANGE_VALUE_MASK;
        break;
    case ABILITY_ID_TALK_PREACH:
        g_battle_action_target_data->faith_change
            = g_battle_action_current_ability.range_data.y | BATTLE_ACTION_STAT_CHANGE_INCREASE;
        break;
    case ABILITY_ID_TALK_SOLUTION:
        g_battle_action_target_data->faith_change
            = g_battle_action_current_ability.range_data.y & BATTLE_ACTION_STAT_CHANGE_VALUE_MASK;
        break;
    default:
        battle_formula_apply_status_to_action();
        return;
    }
    g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
}
