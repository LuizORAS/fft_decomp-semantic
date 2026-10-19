#include "fft/battle_ai.h"
#include "fft/memory.h"

/* Put the best use found for one ability (best_action) into the eight ranked_actions, in order, when
 * it beats the movement baseline (only moving) and the eighth entry; then reset best_action for the
 * next ability.
 *
 * Insert through the shift loop's final index to preserve register lifetimes. */
void battle_ai_insert_ranked_action(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 i;
    s32 j;

    if (battle_ai_compare_action_rank(&ai->best_action.rank.decided, &ai->movement_baseline_action.rank.decided)
        == BATTLE_AI_RANK_ABOVE) {
        if (battle_ai_is_action_higher_ranked(
                &ai->best_action, &ai->ranked_actions[BATTLE_AI_RANKED_ACTION_COUNT - 1])) {
            for (i = 0; i < BATTLE_AI_RANKED_ACTION_COUNT; i++) {
                if (battle_ai_is_action_higher_ranked(&ai->best_action, &ai->ranked_actions[i]))
                    break;
            }
            if (i != BATTLE_AI_RANKED_ACTION_COUNT) {
                for (j = BATTLE_AI_RANKED_ACTION_COUNT - 1; i < j; j--)
                    battle_mem_copy_u16_values(
                        &ai->ranked_actions[j], &ai->ranked_actions[j - 1], sizeof(battle_ai_action_data_t));
                battle_mem_copy_u16_values(&ai->ranked_actions[j], &ai->best_action, sizeof(battle_ai_action_data_t));
            }
        }
    }
    battle_ai_reset_action_ranking_fields(&ai->best_action);
}
