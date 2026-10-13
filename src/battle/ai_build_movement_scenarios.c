#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/* Workspace aliases preserve the target's separate entry address loads. */
extern s32 g_ai_target_in_range;             /* 1 for range >= distance or charging; 0 otherwise. */
extern s32 g_ai_closest_ability_range;       /* Movement range, possibly halved before tile selection. */
extern s32 g_ai_movement_unit_index;         /* Reused unit/row loop index or Stay Near occupant ID. */
extern s32 g_ai_target_distance;             /* Target-distance byte at the acting unit's tile. */
extern s32 g_ai_stay_near_destination_fixed; /* 1 after a reachable Stay Near destination is fixed. */

/* Prepare candidate destinations and reachable movement scenarios.
 *
 * Return -1 when a child suspends and 0 after all scenarios are prepared.
 * 32-bit scratch values preserve branch decisions across suspension.
 */
s32 ai_build_movement_scenarios(void) {
    ai_unit_decision_t* decision;
    ai_data_t* ai;
    unit_stats_t* unit;
    s32 limit;
    s32 low;
    s32 high;

    ai = &g_ai_data_base;
    g_ai_data_base.movement_scenario = 1;
    decision = g_ai_data_base.acting_unit_decision;
    if (g_ai_data_base.resume_pending) {
        /* Resume: re-enter the step that suspended. */
        if (g_ai_data_base.movement_decision_state == AI_MOVEMENT_TARGET_DISTANCE)
            goto fill_target_distance;
        goto score_tiles;
    }
    if ((decision->flags & AI_DECISION_FOCUS_TARGET)
        && (decision->main_target_id == g_ai_data_base.acting_unit_id
            || ((ai->unit_decisions[decision->main_target_id].targeting_flags_2 >> 4)
                & (AI_TARGET_UNTARGETABLE >> 4)))) {
        ai_clear_words(ai->tile_target_distance, sizeof(ai->tile_target_distance));
    } else {
    fill_target_distance:
        if (ai_fill_target_distance_grid(&decision->target) == -1) {
            ai->movement_decision_state = AI_MOVEMENT_TARGET_DISTANCE;
            return -1;
        }
        if ((decision->targeting_flags_2 & (AI_TARGET_HP_BELOW_HALF | AI_TARGET_MP_CONSTRAINED))
            && !(ai->acting_unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_DEATH_SENTENCE)]
                & STATUS_BYTE_MASK(STATUS_ID_DEATH_SENTENCE))) {
            for (g_ai_movement_unit_index = 0; g_ai_movement_unit_index < BATTLE_UNIT_SLOT_COUNT;
                g_ai_movement_unit_index++) {
                unit = &g_battle_unit_stats[g_ai_movement_unit_index];
                if (unit->entd_slot != BATTLE_ENTD_SLOT_NONE
                    && (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CRYSTAL)]
                        & STATUS_BYTE_MASK(STATUS_ID_CRYSTAL))) {
                    ai_copy_unit_coordinates(g_ai_movement_unit_index, &ai->considered_unit_coords);
                    ai->tile_target_distance[ai->considered_unit_coords.fields.elevation]
                                            [ai->considered_unit_coords.fields.y][ai->considered_unit_coords.fields.x]
                        = 1;
                }
            }
        }
    }
    g_ai_target_distance = ai->tile_target_distance[ai->acting_unit_coords.fields.elevation]
                                                   [ai->acting_unit_coords.fields.y][ai->acting_unit_coords.fields.x];
    g_ai_closest_ability_range = ai_get_movement_range();
    limit = g_ai_target_distance;
    g_ai_stay_near_destination_fixed = 0;
    if (g_ai_closest_ability_range >= limit || (decision->targeting_flags_1 & AI_TARGETING_FLAG_1_REFLECTED_ACTION)) {
        g_ai_target_in_range = 1;
        if ((decision->flags & AI_DECISION_STAY_NEAR_COORDINATES)
            && ((g_ai_movement_unit_index = ai_find_unit_at_coordinates(&decision->target)) == ai->acting_unit_id
                || g_ai_movement_unit_index == BATTLE_UNIT_ID_NONE)
            && (ai->reachable_tiles[0][decision->target.fields.elevation][decision->target.fields.y]
                & (0x8000 >> decision->target.fields.x))) {
            ai->candidate_coords[ai->movement_scenario] = decision->target;
            ai_set_candidate_and_actor_reachable_tiles();
            g_ai_stay_near_destination_fixed = 1;
        } else {
            ai_filter_reachable_tiles_by_target_distance(ai->acting_unit_move + g_ai_target_distance);
        }
    } else {
        g_ai_target_in_range = 0;
        ai_filter_reachable_tiles_by_target_distance(limit - 1);
    }
score_tiles:
    if (ai_score_reachable_tiles() == -1) {
        ai->movement_decision_state = AI_MOVEMENT_SCORE_TILES;
        return -1;
    }
    if (!g_ai_stay_near_destination_fixed) {
        if (g_ai_target_in_range) {
            ai_select_destination(0x7fffffff, AI_DESTINATION_PRIORITY_FIRST);
        } else {
            if (ai->acting_unit_team && rand() % 7 == 0)
                g_ai_closest_ability_range /= 2;
            ai_select_destination_with_nearest_fallback(g_ai_closest_ability_range);
        }
    }
    /* XOR preserves the target operation without assuming scenario 1 is a subset. */
    ai->movement_scenario = 2;
    for (g_ai_movement_unit_index = 0; g_ai_movement_unit_index < ai->map_max_y; g_ai_movement_unit_index++) {
        low = ai->reachable_tiles[0][0][g_ai_movement_unit_index] ^ ai->reachable_tiles[1][0][g_ai_movement_unit_index];
        high
            = ai->reachable_tiles[0][1][g_ai_movement_unit_index] ^ ai->reachable_tiles[1][1][g_ai_movement_unit_index];
        ai->reachable_tiles[2][0][g_ai_movement_unit_index] = low;
        ai->reachable_tiles[2][1][g_ai_movement_unit_index] = high;
    }
    ai->candidate_coords[2].word = ai->candidate_coords[1].word;
    ai->movement_scenario = 0;
    if (!ai->acting_unit->action_taken && (decision->flags & AI_DECISION_FOCUS_TARGET) && g_ai_target_in_range
        && !(decision->targeting_flags_1 & AI_TARGETING_FLAG_1_REFLECTED_ACTION)) {
        ai_select_destination_with_range_floor(g_ai_target_distance - 1);
    } else {
        ai->candidate_coords[0].word = ai->candidate_coords[1].word;
    }
    ai->reachable_tiles[1][ai->acting_unit_coords.fields.elevation][ai->acting_unit_coords.fields.y]
        |= 0x8000 >> ai->acting_unit_coords.fields.x;
    return 0;
}
