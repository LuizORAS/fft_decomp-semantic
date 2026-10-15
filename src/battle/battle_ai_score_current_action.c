#include "fft/battle.h"

/* Simulate and score the current action (battle_ai_simulate_action_and_score) and keep it as
 * best_action when it outranks it (battle_ai_is_action_higher_ranked). Returns -1 to suspend, else
 * 0. */
s32 battle_ai_score_current_action(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 result = battle_ai_simulate_action_and_score();

    if (result == -1) {
        return -1;
    }
    if (result != 0) {
        if (battle_ai_is_action_higher_ranked(&ai->current_action, &ai->best_action) != 0) {
            battle_ai_transfer_halfword_values(&ai->best_action, &ai->current_action, sizeof(battle_ai_action_data_t));
        }
    }
    return 0;
}
