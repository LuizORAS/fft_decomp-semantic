#include "fft/battle_ai.h"

/*
 * Evaluate target tiles from eligible movement origins.
 *
 * Return -1 with the search stage saved when time or a child suspends;
 * return 0 after restoring the temporary unit's position. The scan path
 * reloads its full row mask on resume and continues at its saved x index.
 */
s32 ai_evaluate_movement_origins(void) {
    ai_data_t* ai = &g_ai_data_base;
    s32 row;

    if (g_ai_data_base.resume_pending) {
        /* Resume: re-enter the step that suspended. */
        if (g_ai_data_base.highest_priority_state == AI_ORIGIN_EVALUATION_PREPARE)
            goto begin_evaluation;
        if (g_ai_data_base.highest_priority_state == AI_ORIGIN_EVALUATION_RANKED)
            goto evaluate_ranked_origin;
        row = ai->movable_tiles[g_ai_data_base.map_level_counter][g_ai_data_base.y_counter];
        goto evaluate_scanned_origin;
    }
    row = ai->targetability.live.unit_targetable[g_ai_data_base.acting_unit_id];
    ai->targetability.live.unit_targetable[g_ai_data_base.acting_unit_id] = 0;
    ai_build_attack_origin_masks();
    ai->targetability.live.unit_targetable[g_ai_data_base.acting_unit_id] = row;
begin_evaluation:
    if (VSync(1) >= AI_DECISION_SCANLINE_LIMIT) {
        ai->highest_priority_state = AI_ORIGIN_EVALUATION_PREPARE;
        return -1;
    }
    ai->resume_pending = 0;
    ai->ranked_origin_scan = 0;
    if (ai->considered_ability.ai_flags.word & AI_ABILITY_EXHAUSTIVE_ORIGIN_SCAN) {
        ai->targetability.live.target_setting_flags &= ~AI_TARGET_SETTING_CONSUME_EVALUATED_ORIGINS;
    } else {
        ai->max_possibilities = ai->attack_origin_tile_count;
        ai->max_possibilities *= ai->movable_tile_count;
        if (ai->max_possibilities > 10)
            ai->ranked_origin_scan = 1;
        /* The target sets this regardless of the truncated product test. */
        ai->ranked_origin_scan = 1;
    }
    if ((ai->targetability.live.target_setting_flags & AI_TARGET_SETTING_CONSUME_EVALUATED_ORIGINS)
        || ai->ranked_origin_scan) {
        while (ai->attack_origin_tile_count && ai->movable_tile_count) {
            ai_take_next_movement_origin();
            ai_finalize_action_destination();
        evaluate_ranked_origin:
            row = ai_evaluate_target_tiles();
            if (row == -1) {
                ai->highest_priority_state = AI_ORIGIN_EVALUATION_RANKED;
                return -1;
            }
        }
    } else {
        ai->map_level_counter = 0;
        do {
            for (ai->y_counter = 0; ai->y_counter < ai->map_max_y; ai->y_counter++) {
                row = ai->movable_tiles[ai->map_level_counter][ai->y_counter];
                if (row != 0) {
                    for (ai->max_possibilities = 0; ai->max_possibilities < ai->map_max_x; ai->max_possibilities++) {
                        if ((row << ai->max_possibilities) & 0x8000) {
                            ai->current_action.coords.bytes.x = ai->max_possibilities;
                            ai->current_action.coords.bytes.y = ai->y_counter;
                            ai->current_action.coords.bytes.elevation = ai->map_level_counter;
                            ai_finalize_action_destination();
                        evaluate_scanned_origin:
                            if (ai_evaluate_target_tiles() == -1) {
                                ai->highest_priority_state = AI_ORIGIN_EVALUATION_SCANNED;
                                return -1;
                            }
                        }
                    }
                }
            }
            ai->map_level_counter++;
        } while (ai->map_level_counter < MAP_TERRAIN_LAYER_COUNT);
    }
    ai_move_temp_unit_to_coords(&ai->acting_unit_coords);
    return 0;
}
