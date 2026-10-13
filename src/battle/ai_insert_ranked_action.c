#include "fft/battle_ai.h"

/*
 * Insert the best action into the ranked list, then reset it.
 *
 * The candidate must beat the movement baseline and the last ranked entry.
 * Insert through the shift loop's final index to preserve register lifetimes.
 */
void ai_insert_ranked_action(void) {
    ai_data_t* ai = &g_ai_data_base;
    s32 i;
    s32 j;

    if (ai_compare_target_priority_and_hit_rate(&ai->best_action.rank, &ai->inverted_priority_action.rank)
        == AI_RANK_ABOVE) {
        if (ai_is_action_higher_ranked(&ai->best_action, &ai->ranked_actions[AI_RANKED_ACTION_COUNT - 1])) {
            for (i = 0; i < AI_RANKED_ACTION_COUNT; i++) {
                if (ai_is_action_higher_ranked(&ai->best_action, &ai->ranked_actions[i]))
                    break;
            }
            if (i != AI_RANKED_ACTION_COUNT) {
                for (j = AI_RANKED_ACTION_COUNT - 1; i < j; j--)
                    ai_copy_u16_values(&ai->ranked_actions[j], &ai->ranked_actions[j - 1], sizeof(ai_action_data_t));
                ai_copy_u16_values(&ai->ranked_actions[j], &ai->best_action, sizeof(ai_action_data_t));
            }
        }
    }
    ai_reset_action_ranking_fields(&ai->best_action);
}
