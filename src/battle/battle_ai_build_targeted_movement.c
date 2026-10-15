#include "fft/battle.h"

/* Build the movement scenarios of Fight for life and Protect allies around the decision target (the
 * Auto-Battle target; every distance is 0 when that is the unit itself or untargetable). Scenario 0's
 * destination is the best-scored tile within reach of the target (battle_ai_get_target_reach; for
 * Protect allies, within 2 tiles when the ally is farther), else the nearest; with a reflected action,
 * the best-scored tile anywhere. Scenario 1 starts from that destination and keeps the tiles within
 * reach of the target, or only it and the unit's own tile when there are none. Scenario 2 copies
 * scenario 0's tiles; its destination is a tile nearer to the target when the unit has not acted,
 * else scenario 1's. The unit's own tile joins scenario 1. Returns -1 to suspend, else 0. */
s32 battle_ai_build_targeted_movement(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 range;
    u8(*distance_rows)[16];
    u32 distance_level_base;
    s32 limit;
    s32 distance;
    s32 within_two_tiles;

    if (!g_battle_ai_data_base.resume_pending
        && (g_battle_ai_data_base.acting_unit->auto_battle_target == g_battle_ai_data_base.acting_unit_id
            || ((ai->unit_decisions[g_battle_ai_data_base.acting_unit->auto_battle_target].targeting_flags_2 >> 4)
                & (BATTLE_AI_TARGET_UNTARGETABLE >> 4)))) {
        battle_ai_clear_words(ai->tile_target_distance, sizeof(ai->tile_target_distance));
    } else if (battle_ai_fill_target_distance_grid(&ai->acting_unit_decision->target) == -1) {
        return -1;
    }
    range = battle_ai_get_target_reach();
    /* Direct typed level indexing reverses the two address-building instructions
     * at +0xbc/+0xc0. Stage only the level base; retain the real member offset
     * and typed row/column indexing for the byte-exact match. */
    distance_level_base = ai->acting_unit_coords.bytes.elevation * sizeof(ai->tile_target_distance[0]) + (u32)ai;
    distance_rows = (u8(*)[16])(distance_level_base + (u32) & ((battle_ai_data_t*)0)->tile_target_distance);
    limit = range;
    distance = distance_rows[ai->acting_unit_coords.bytes.y][ai->acting_unit_coords.bytes.x];
    within_two_tiles = distance < 3;
    ai->movement_scenario = 0;
    if (ai->autobattle_setting == BATTLE_AUTO_BATTLE_PROTECT_ALLIES && !within_two_tiles) {
        limit = 2;
    }
    if (ai->acting_unit_decision->targeting_flags_1 & BATTLE_AI_TARGETING_FLAG_1_REFLECTED_ACTION) {
        battle_ai_select_destination(0x7fffffff, BATTLE_AI_DESTINATION_PRIORITY_FIRST);
    } else {
        battle_ai_select_destination_with_nearest_fallback(limit);
    }
    ai->movement_scenario = 1;
    ai->candidate_coords[1] = ai->candidate_coords[0];
    if (!battle_ai_filter_reachable_tiles_by_target_distance(range)) {
        battle_ai_set_candidate_and_actor_reachable_tiles();
    }
    ai->movement_scenario = 2;
    battle_ai_transfer_halfword_values(ai->reachable_tiles[2], ai->reachable_tiles[0], sizeof(ai->reachable_tiles[0]));
    if (!ai->acting_unit->action_taken) {
        s32 closer_distance = distance - 1;
        if (closer_distance < limit) {
            limit = closer_distance;
        }
        battle_ai_select_destination_with_range_floor(limit);
    } else {
        ai->candidate_coords[2] = ai->candidate_coords[1];
    }
    ai->reachable_tiles[1][ai->acting_unit_coords.bytes.elevation][ai->acting_unit_coords.bytes.y]
        |= 0x8000 >> ai->acting_unit_coords.bytes.x;
    return 0;
}
