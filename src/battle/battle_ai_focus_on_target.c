#include "fft/battle_ai.h"

/* Make unit_id the acting unit's main target: set BATTLE_AI_DECISION_FOCUS_TARGET, clear
 * BATTLE_AI_DECISION_STAY_NEAR_COORDINATES, and take the target's tile as the decision target. */
void battle_ai_focus_on_target(s32 unit_id) {
    battle_ai_unit_decision_t* decision = g_battle_ai_data_base.acting_unit_decision;

    decision->main_target_id = unit_id;
    decision->flags = (decision->flags | BATTLE_AI_DECISION_FOCUS_TARGET) & ~BATTLE_AI_DECISION_STAY_NEAR_COORDINATES;
    battle_ai_transfer_unit_coordinates(unit_id, &decision->target);
}
