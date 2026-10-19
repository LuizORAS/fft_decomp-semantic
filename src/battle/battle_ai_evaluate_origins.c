#include "fft/battle_ai.h"

/* Search the origin tiles for the considered ability (battle_ai_build_origin_and_target_tiles builds
 * them, with the acting unit left out of the targets) and evaluate the target tiles in range from each
 * (battle_ai_evaluate_target_tiles). An EXHAUSTIVE_ORIGIN_SCAN ability visits every origin in map
 * order and drops no target tile; any other takes the origins best first (battle_ai_take_best_origin)
 * until no origin or target tile is left (its 10-combination test has no effect, QUIRKS.md). An origin
 * on the unit's own tile means acting first, then moving to the scenario's destination
 * (battle_ai_finalize_action_destination). The simulation unit returns to its tile at
 * the end. Returns -1 to suspend (also past 441 horizontal blanks), else 0.
 *
 * The scan path reloads its full row mask on resume and continues at its saved x index. */
s32 battle_ai_evaluate_origins(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 row;

    if (g_battle_ai_data_base.resume_pending) {
        /* Resume: re-enter the step that suspended. */
        if (g_battle_ai_data_base.origin_search_state == BATTLE_AI_ORIGIN_EVALUATION_PREPARE)
            goto begin_evaluation;
        if (g_battle_ai_data_base.origin_search_state == BATTLE_AI_ORIGIN_EVALUATION_RANKED)
            goto evaluate_ranked_origin;
        row = ai->origin_tiles[g_battle_ai_data_base.map_level_counter][g_battle_ai_data_base.y_counter];
        goto evaluate_scanned_origin;
    }
    row = ai->targetability.live.unit_targetable[g_battle_ai_data_base.acting_unit_id];
    ai->targetability.live.unit_targetable[g_battle_ai_data_base.acting_unit_id] = 0;
    battle_ai_build_origin_and_target_tiles();
    ai->targetability.live.unit_targetable[g_battle_ai_data_base.acting_unit_id] = row;
begin_evaluation:
    if (VSync(1) >= BATTLE_AI_DECISION_SCANLINE_LIMIT) {
        ai->origin_search_state = BATTLE_AI_ORIGIN_EVALUATION_PREPARE;
        return -1;
    }
    ai->resume_pending = 0;
    ai->ranked_origin_search = 0;
    if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_EXHAUSTIVE_ORIGIN_SCAN) {
        ai->targetability.live.target_setting_flags &= ~BATTLE_AI_TARGET_SETTING_CONSUME_TARGET_TILES;
    } else {
        ai->max_possibilities = ai->target_tile_count;
        ai->max_possibilities *= ai->origin_tile_count;
        if (ai->max_possibilities > 10)
            ai->ranked_origin_search = 1;
        /* Both arms of the original test set it (QUIRKS.md). */
        ai->ranked_origin_search = 1;
    }
    if ((ai->targetability.live.target_setting_flags & BATTLE_AI_TARGET_SETTING_CONSUME_TARGET_TILES)
        || ai->ranked_origin_search) {
        while (ai->target_tile_count && ai->origin_tile_count) {
            battle_ai_take_best_origin();
            battle_ai_finalize_action_destination();
        evaluate_ranked_origin:
            row = battle_ai_evaluate_target_tiles();
            if (row == -1) {
                ai->origin_search_state = BATTLE_AI_ORIGIN_EVALUATION_RANKED;
                return -1;
            }
        }
    } else {
        ai->map_level_counter = 0;
        do {
            for (ai->y_counter = 0; ai->y_counter < ai->map_max_y; ai->y_counter++) {
                row = ai->origin_tiles[ai->map_level_counter][ai->y_counter];
                if (row != 0) {
                    for (ai->max_possibilities = 0; ai->max_possibilities < ai->map_max_x; ai->max_possibilities++) {
                        if ((row << ai->max_possibilities) & 0x8000) {
                            ai->current_action.coords.fields.x = ai->max_possibilities;
                            ai->current_action.coords.fields.y = ai->y_counter;
                            ai->current_action.coords.fields.elevation = ai->map_level_counter;
                            battle_ai_finalize_action_destination();
                        evaluate_scanned_origin:
                            if (battle_ai_evaluate_target_tiles() == -1) {
                                ai->origin_search_state = BATTLE_AI_ORIGIN_EVALUATION_SCANNED;
                                return -1;
                            }
                        }
                    }
                }
            }
            ai->map_level_counter++;
        } while (ai->map_level_counter < MAP_TERRAIN_LAYER_COUNT);
    }
    battle_ai_move_temp_unit_to_coords(&ai->acting_unit_coords);
    return 0;
}
