#include "fft/battle_ai.h"

/*
 * Return whether the first action outranks the second.
 *
 * Rank ties prefer lower foe proximity; equal proximity is not an improvement.
 * Reusing the working scalar for the level preserves target register lifetimes.
 */
s32 ai_is_action_higher_ranked(const ai_action_data_t* first, const ai_action_data_t* second) {
    ai_data_t* ai = &g_ai_data_base;
    ai_rank_order_e comparison;
    s32 proximity_a;
    s32 proximity_b;
    s32 value;

    comparison = ai_compare_target_priority_and_hit_rate(&first->rank, &second->rank);
    value = 1;
    if (comparison == AI_RANK_ABOVE)
        return value;
    if (comparison == AI_RANK_EQUAL) {
        value = first->coords.bytes.elevation;
        proximity_a = ai->tile_foe_proximity[value][first->coords.bytes.y][first->coords.bytes.x];
        proximity_b
            = ai->tile_foe_proximity[second->coords.bytes.elevation][second->coords.bytes.y][second->coords.bytes.x];
        return proximity_a < proximity_b;
    }
    return 0;
}
