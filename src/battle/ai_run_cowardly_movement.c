#include "fft/battle_ai.h"

/* Score the reachable tiles, fill the distance grid for the decision's target,
 * and store the best destination as movement scenario 1. -1 suspends; the next
 * call resumes at the step that did. */
s32 ai_run_cowardly_movement(void) {
    ai_data_t* ai = &g_ai_data_base;
    ai_cowardly_movement_stage_e state;

    if (g_ai_data_base.resume_pending != 0) {
        /* Resume: re-enter the step that suspended. */
        state = g_ai_cowardly_movement_stage;
        if (state == AI_COWARDLY_MOVEMENT_SCORE_TILES) {
            goto score_tiles;
        }
        if (state == AI_COWARDLY_MOVEMENT_TARGET_DISTANCE) {
            goto fill_target_distance;
        }
    }
    g_ai_data_base.movement_scenario = 0;
score_tiles:
    if (ai_score_reachable_tiles() == -1) {
        g_ai_cowardly_movement_stage = AI_COWARDLY_MOVEMENT_SCORE_TILES;
        return -1;
    }
fill_target_distance:
    if (ai_fill_target_distance_grid(&ai->acting_unit_decision->target) == -1) {
        g_ai_cowardly_movement_stage = AI_COWARDLY_MOVEMENT_TARGET_DISTANCE;
        return -1;
    }
    ai_select_destination(0xff, AI_DESTINATION_PRIORITY_FIRST);
    ai->movement_scenario = 1;
    ai->candidate_coords[1].word = ai->candidate_coords[0].word;
    ai_set_candidate_and_actor_reachable_tiles();
    return 0;
}
