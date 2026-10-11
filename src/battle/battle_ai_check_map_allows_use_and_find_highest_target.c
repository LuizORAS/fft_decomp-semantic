#include "fft/battle.h"
#include "psx/types.h"

/* Score the considered ability cast from the unit's own tile (act, then move)
 * and, when the action's destination tile differs and allows the ability, cast
 * from there (move, then act). -1 suspends. */
s32 battle_ai_check_map_allows_use_and_find_highest_target(void) {
    battle_ai_data_t* ai;

    ai = &g_battle_ai_data_base;
    if (g_battle_ai_data_base.resume_pending != 0) {
        /* Resume: re-enter the step that suspended. */
        if (g_battle_ai_data_base.find_highest_target_state != BATTLE_AI_CAST_ORIGIN_ACT_BEFORE_MOVE) {
            goto score_move_before_act;
        }
        goto score_act_before_move;
    }
    if (battle_ai_check_ability_use_at_coords(&ai->acting_unit_coords) != 0) {
        g_battle_ai_data_base.current_action.act_before_move = 1;
    score_act_before_move:
        if (battle_ai_check_set_highest_unit_priority() == -1) {
            ai->find_highest_target_state = BATTLE_AI_CAST_ORIGIN_ACT_BEFORE_MOVE;
            return -1;
        }
    }
    if (ai->current_action.coords.word == ai->acting_unit_coords.word) {
        return 0;
    }
    if (battle_ai_check_ability_use_at_coords(&ai->current_action.coords) == 0) {
        return 0;
    }
    ai->current_action.act_before_move = 0;
score_move_before_act:
    if (battle_ai_check_set_highest_unit_priority() != -1) {
        return 0;
    }
    ai->find_highest_target_state = BATTLE_AI_CAST_ORIGIN_MOVE_BEFORE_ACT;
    return -1;
}
