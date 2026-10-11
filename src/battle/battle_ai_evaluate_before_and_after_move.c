#include "fft/battle.h"
#include "psx/types.h"

/* Score the current action used from the unit's own tile before moving, when allowed there, then,
 * when the destination (current_action.coords) differs and allows it, used after moving there
 * (battle_ai_can_use_ability_at_coords, battle_ai_score_current_action). Returns -1 to
 * suspend, else 0. */
s32 battle_ai_evaluate_before_and_after_move(void) {
    battle_ai_data_t* ai;

    ai = &g_battle_ai_data_base;
    if (g_battle_ai_data_base.resume_pending != 0) {
        /* Resume: re-enter the step that suspended. */
        if (g_battle_ai_data_base.before_after_move_state != 0) {
            goto second;
        }
        goto first;
    }
    if (battle_ai_can_use_ability_at_coords(&ai->acting_unit_coords) != 0) {
        g_battle_ai_data_base.current_action.act_before_move = 1;
    first:
        if (battle_ai_score_current_action() == -1) {
            ai->before_after_move_state = 0;
            return -1;
        }
    }
    if (ai->current_action.coords.word == ai->acting_unit_coords.word) {
        return 0;
    }
    if (battle_ai_can_use_ability_at_coords(&ai->current_action.coords) == 0) {
        return 0;
    }
    ai->current_action.act_before_move = 0;
second:
    if (battle_ai_score_current_action() != -1) {
        return 0;
    }
    ai->before_after_move_state = 1;
    return -1;
}
