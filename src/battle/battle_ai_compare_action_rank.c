#include "fft/battle.h"

/* Compare two actions' ranking fields: decided (0 in every action the ranking compares), then
 * the score (priority, signed), then the hit chance (base_hit_percent). */
battle_ai_rank_order_e battle_ai_compare_action_rank(battle_ai_action_rank_t* first, battle_ai_action_rank_t* second) {
    u8 decided_a, decided_b;
    s16 priority_a, priority_b;
    u8 hit_a, hit_b;

    decided_a = first->decided;
    decided_b = second->decided;
    if (decided_a > decided_b)
        return BATTLE_AI_RANK_ABOVE;
    if (decided_a < decided_b)
        return BATTLE_AI_RANK_BELOW;
    priority_a = first->priority;
    priority_b = second->priority;
    if (priority_a > priority_b)
        return BATTLE_AI_RANK_ABOVE;
    if (priority_a < priority_b)
        return BATTLE_AI_RANK_BELOW;
    hit_a = first->base_hit_percent;
    hit_b = second->base_hit_percent;
    if (hit_a > hit_b)
        return BATTLE_AI_RANK_ABOVE;
    if (hit_a < hit_b)
        return BATTLE_AI_RANK_BELOW;
    return BATTLE_AI_RANK_EQUAL;
}
