#include "fft/battle.h"
#include "psx/types.h"

/* Faith Up and Absorb Used MP: against an ability that cost MP, a successful Brave roll queues
 * reaction_id with the MP cost in last_received_attack. */
void battle_reaction_try_on_mp_cost(s16 reaction_id) {
    u8* used_mp = &g_current_ability.mp_cost;
    battle_action_data_t* action;
    if (*used_mp != 0) {
        if (battle_reaction_fails_brave_roll(g_battle_action_target) == 0) {
            action = g_battle_action_target_data;
            action->reaction_id = reaction_id;
            action->last_received_attack = (s16)*used_mp;
        }
    }
}
