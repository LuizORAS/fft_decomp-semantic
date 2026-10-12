#include "fft/battle_action.h"
#include "psx/types.h"

/* Distribute: when the healing exceeds the target's missing HP and the reaction
 * roll passes, record the reaction with the excess as its amount. */
void action_check_distribute_usability(void) {
    battle_stats_t* target = g_action_target;
    action_data_t* action = g_action_target_data;
    /* hp_healing is read signed (lh) here. */
    s32 excess = action->hp_healing - (target->max_hp - target->hp);
    if (excess > 0 && action_calculate_chance_to_react(target) == 0) {
        g_action_target_data->last_received_attack = excess;
        g_action_target_data->reaction_id = ABILITY_ID_REACTION_DISTRIBUTE;
    }
}
