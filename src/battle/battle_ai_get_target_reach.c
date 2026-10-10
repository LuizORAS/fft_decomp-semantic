#include "fft/battle.h"

/* Return the acting unit's reach toward its focus target: enemy_reach for an enemy, ally_reach for
 * anyone else, and Move without a focus target. The caller consumes the full register. */
s32 battle_ai_get_target_reach(void) {
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
