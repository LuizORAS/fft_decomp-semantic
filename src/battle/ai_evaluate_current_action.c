#include "fft/battle_ai.h"

/* Simulate and score the current action and keep it as the best when it
 * outranks the best so far. -1 suspends; otherwise 0. */
s32 ai_evaluate_current_action(void) {
    ai_data_t* ai = &g_ai_data_base;
    s32 result = ai_simulate_action_and_score();

    if (result == -1) {
        return -1;
    }
    if (result != 0) {
        if (ai_is_action_higher_ranked(&ai->current_action, &ai->best_action) != 0) {
            ai_copy_u16_values(&ai->best_action, &ai->current_action, sizeof(ai_action_data_t));
        }
    }
    return 0;
}
