#include "fft/battle_ai.h"
#include "psx/types.h"

/* Score the considered ability cast from the unit's own tile (act, then move)
 * and, when the action's destination tile differs and allows the ability, cast
 * from there (move, then act). -1 suspends. */
s32 ai_evaluate_cast_origins(void) {
    ai_data_t* ai;

    ai = &g_ai_data_base;
    if (g_ai_data_base.resume_pending != 0) {
        /* Resume: re-enter the step that suspended. */
        if (g_ai_data_base.find_highest_target_state != AI_CAST_ORIGIN_ACT_BEFORE_MOVE) {
            goto score_move_before_act;
        }
        goto score_act_before_move;
    }
    if (ai_is_ability_origin_allowed(&ai->acting_unit_coords) != 0) {
        g_ai_data_base.current_action.act_before_move = 1;
    score_act_before_move:
        if (ai_evaluate_current_action() == -1) {
            ai->find_highest_target_state = AI_CAST_ORIGIN_ACT_BEFORE_MOVE;
            return -1;
        }
    }
    if (ai->current_action.coords.word == ai->acting_unit_coords.word) {
        return 0;
    }
    if (ai_is_ability_origin_allowed(&ai->current_action.coords) == 0) {
        return 0;
    }
    ai->current_action.act_before_move = 0;
score_move_before_act:
    if (ai_evaluate_current_action() != -1) {
        return 0;
    }
    ai->find_highest_target_state = AI_CAST_ORIGIN_MOVE_BEFORE_ACT;
    return -1;
}
