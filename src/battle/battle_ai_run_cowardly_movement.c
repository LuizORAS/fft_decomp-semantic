#include "fft/battle.h"

/* Score the reachable tiles, fill the distance grid for the decision's target,
 * and store the best destination as movement scenario 1. -1 suspends; the next
 * call resumes at the step that did. */
s32 battle_ai_run_cowardly_movement(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 state;

    if (g_battle_ai_data_base.resume_pending != 0) {
        /* Resume: re-enter the step that suspended. */
        state = g_battle_ai_cowardly_movement_step;
        if (state == BATTLE_AI_COWARDLY_MOVEMENT_SCORE_TILES) {
            goto score_tiles;
        }
        if (state == BATTLE_AI_COWARDLY_MOVEMENT_TARGET_DISTANCE) {
            goto fill_target_distance;
        }
    }
    g_battle_ai_data_base.movement_scenario = 0;
score_tiles:
    if (battle_ai_score_reachable_tiles() == -1) {
        g_battle_ai_cowardly_movement_step = BATTLE_AI_COWARDLY_MOVEMENT_SCORE_TILES;
        return -1;
    }
fill_target_distance:
    if (battle_ai_fill_target_distance_grid(&ai->acting_unit_decision->target) == -1) {
        g_battle_ai_cowardly_movement_step = BATTLE_AI_COWARDLY_MOVEMENT_TARGET_DISTANCE;
        return -1;
    }
    battle_ai_select_destination(0xff, BATTLE_AI_DESTINATION_PRIORITY_FIRST);
    ai->movement_scenario = 1;
    ai->candidate_coords[1].word = ai->candidate_coords[0].word;
    battle_ai_set_candidate_and_actor_reachable_tiles();
    return 0;
}
