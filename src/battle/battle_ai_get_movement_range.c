#include "fft/battle_ai.h"

/*
 * Choose the range threshold used to partition movement candidates.
 *
 * A focused enemy uses enemy_ability_min_reach; a focused ally uses ally_ability_min_reach.
 * Without a focused target, use Move. The caller consumes the full register.
 */
s32 battle_ai_get_movement_range(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_ai_unit_decision_t* acting = g_battle_ai_data_base.acting_unit_decision;
    if (acting->flags & BATTLE_AI_DECISION_FOCUS_TARGET) {
        if (ai->unit_decisions[acting->main_target_id].enemy_flag != 0) {
            return acting->enemy_ability_min_reach;
        }
        return acting->ally_ability_min_reach;
    }
    return g_battle_ai_data_base.acting_unit_move;
}
