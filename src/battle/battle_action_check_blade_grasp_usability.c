#include "fft/battle_action.h"

/* Blade Grasp. An ability flagged Blade Grasp eligible has its accuracy set to
 * 100 - Brave of the target; when the target passes its reaction roll during
 * execution the strike becomes a miss and the reaction is recorded. */
void battle_action_check_blade_grasp_usability(void) {
    battle_stats_t* attacker;
    battle_action_data_t* target;

    if ((g_battle_action_current_ability.range_data.flags_4 & ABILITY_SECONDARY_FLAG_4_BLADE_GRASP_ELIGIBLE) == 0)
        return;
    attacker = g_battle_action_target;
    target = g_battle_action_target_data;
    /* 16-bit store covers attack_accuracy and the pad byte after it. */
    target->attack_accuracy = 0x64 - attacker->brave;
    if (battle_action_calculate_chance_to_react(attacker) != 0)
        return;
    if (g_battle_action_state != BATTLE_ACTION_STATE_EXECUTE)
        return;
    g_battle_action_target_data->hit = 0;
    g_battle_action_target_data->miss_type = BATTLE_ACTION_MISS_TYPE_BLADE_GRASP_OR_FINGER_GUARD;
    g_battle_action_target_data->reaction_id = ABILITY_ID_REACTION_BLADE_GRASP;
}
