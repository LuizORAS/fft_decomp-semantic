#include "fft/battle.h"
#include "psx/types.h"

/* Distribute: when healing goes past max HP, a successful Brave roll queues the reaction with the
 * excess in last_received_attack, to share among injured allies. */
void battle_reaction_try_distribute(void) {
    battle_stats_t* target = g_battle_action_target;
    battle_action_data_t* action = g_battle_action_target_data;
    s32 excess = action->hp_healing - (s32)(target->max_hp - target->hp);
    /* The target passes the unit twice to the one-parameter callee. */
    if (excess > 0
        && ((s32 (*)(const battle_stats_t*, const battle_stats_t*))battle_reaction_fails_brave_roll)(target, target)
            == 0) {
        g_battle_action_target_data->last_received_attack = (s16)excess;
        g_battle_action_target_data->reaction_id = ABILITY_ID_REACTION_DISTRIBUTE;
    }
}
