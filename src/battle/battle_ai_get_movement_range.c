#include "fft/battle.h"

/*
 * Choose the range threshold used to partition movement candidates.
 *
 * A focused enemy uses enemy_reach; a focused ally uses ally_reach.
 * Without a focused target, use Move. The caller consumes the full register.
 */
s32 battle_ai_get_movement_range(void) {
    battle_ai_unit_decision_t* acting = g_battle_ai_acting_unit_decision_ptr;
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    if (acting->flags & BATTLE_AI_DECISION_FOCUS_TARGET) {
        if (ai->unit_decisions[acting->main_target_id].enemy_flag != 0) {
            return acting->enemy_reach;
        }
        return acting->ally_reach;
    }
    return g_battle_ai_data_base.acting_unit_move;
}
