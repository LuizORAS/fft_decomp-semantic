#include "fft/battle_ai.h"

/* Movement for running away: score the reachable tiles, measure the distance to the decision target
 * (the healer or crystal that battle_ai_select_retreat_action chose) and take the best-scored tile,
 * the nearest to it on ties, as the destination. The next scenario keeps only that tile and the unit's
 * own (battle_ai_set_candidate_and_actor_reachable_tiles). Returns -1 to suspend, else 0. */
s32 battle_ai_build_retreat_movement(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_ai_cowardly_movement_stage_e state;

    if (g_battle_ai_data_base.resume_pending != 0) {
        /* Resume: re-enter the step that suspended. */
        state = g_battle_ai_retreat_movement_step;
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
        g_battle_ai_retreat_movement_step = BATTLE_AI_COWARDLY_MOVEMENT_SCORE_TILES;
        return -1;
    }
fill_target_distance:
    if (battle_ai_fill_target_distance_grid(&ai->acting_unit_decision->target) == -1) {
        g_battle_ai_retreat_movement_step = BATTLE_AI_COWARDLY_MOVEMENT_TARGET_DISTANCE;
        return -1;
    }
    battle_ai_select_destination(0xff, BATTLE_AI_DESTINATION_PRIORITY_FIRST);
    ai->movement_scenario = 1;
    ai->candidate_coords[1] = ai->candidate_coords[0];
    battle_ai_set_candidate_and_actor_reachable_tiles();
    return 0;
}
