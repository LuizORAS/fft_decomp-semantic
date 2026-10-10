#include "fft/battle.h"

/* Return 1 when the first action outranks the second (battle_ai_compare_action_rank), or ties with
 * it from a tile with less foe proximity (tile_foe_proximity).
 *
 * Reusing the working scalar for the level preserves target register lifetimes. */
s32 battle_ai_is_action_higher_ranked(battle_ai_action_data_t* first, battle_ai_action_data_t* second) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_ai_rank_order_e comparison;
    s32 proximity_a;
    s32 proximity_b;
    s32 value;

    comparison = battle_ai_compare_action_rank(
        (battle_ai_action_rank_t*)&first->plan_chosen, (battle_ai_action_rank_t*)&second->plan_chosen);
    value = 1;
    if (comparison == BATTLE_AI_RANK_ABOVE)
        return value;
    if (comparison == BATTLE_AI_RANK_EQUAL) {
        value = first->coords.bytes.elevation;
        proximity_a = ai->tile_foe_proximity[value][first->coords.bytes.y][first->coords.bytes.x];
        proximity_b
            = ai->tile_foe_proximity[second->coords.bytes.elevation][second->coords.bytes.y][second->coords.bytes.x];
        return proximity_a < proximity_b;
    }
    return 0;
}
