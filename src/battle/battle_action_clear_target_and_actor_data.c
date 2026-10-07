#include "fft/battle.h"
#include "psx/types.h"

/* Reset the target's and the actor's result records (g_battle_action_target_data,
 * g_battle_action_attacker_data); the actor's starts as a miss (hit 0). */
void battle_action_clear_target_and_actor_data(void) {
    battle_action_clear_current_data(g_battle_action_target_data);
    battle_action_clear_current_data(g_battle_action_attacker_data);
    g_battle_action_attacker_data->hit = 0;
}
