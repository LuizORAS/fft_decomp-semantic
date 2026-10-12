#include "fft/battle_ai.h"

/*
 * Compare the ranking fields of two actions.
 *
 * Compare the decided marker, signed priority, then hit percentage. Every
 * candidate holds 0 in the marker, so priority is the first key that differs.
 */
ai_rank_order_e ai_compare_target_priority_and_hit_rate(const ai_action_rank_t* first, const ai_action_rank_t* second) {
    u8 decided_a, decided_b;
    s16 priority_a, priority_b;
    u8 hit_a, hit_b;

    decided_a = first->decided;
    decided_b = second->decided;
    if (decided_a > decided_b)
        return AI_RANK_ABOVE;
    if (decided_a < decided_b)
        return AI_RANK_BELOW;
    priority_a = first->priority;
    priority_b = second->priority;
    if (priority_a > priority_b)
        return AI_RANK_ABOVE;
    if (priority_a < priority_b)
        return AI_RANK_BELOW;
    hit_a = first->base_hit_percent;
    hit_b = second->base_hit_percent;
    if (hit_a > hit_b)
        return AI_RANK_ABOVE;
    if (hit_a < hit_b)
        return AI_RANK_BELOW;
    return AI_RANK_EQUAL;
}
