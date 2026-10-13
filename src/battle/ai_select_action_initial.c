#include "fft/battle_ai.h"

/*
 * Initialize and dispatch the AI action-selection passes.
 *
 * Resume the selected pass without repeating action-record initialization.
 * Return -1 to suspend and 0 when that pass completes.
 */
s32 ai_select_action_initial(void) {
    ai_data_t* ai = &g_ai_data_base;
    s32 result;

    if (g_ai_data_base.resume_pending) {
        /* Resume: re-enter the step that suspended. */
        switch (g_ai_data_base.initial_selection_stage) {
        case AI_INITIAL_SELECTION_STATUS:
            goto status_action;
        case AI_INITIAL_SELECTION_RETREAT:
            goto retreat_action;
        case AI_INITIAL_SELECTION_AUTOBATTLE:
            goto autobattle_action;
        default:
            goto general_action;
        }
    }
    for (result = 0; result < AI_RANKED_ACTION_COUNT; result++)
        ai_reset_action_ranking_fields(&ai->ranked_actions[result]);
    ai_reset_action_ranking_fields(&ai->current_action);
    ai_reset_action_ranking_fields(&ai->best_action);
    ai_reset_action_ranking_fields(&ai->inverted_priority_action);
    ai_reset_action_ranking_fields(&ai->selected_action);
    ai_fill_foe_proximity_grid();
status_action:
    result = ai_select_action_status();
    if (result == -1) {
        ai->initial_selection_stage = AI_INITIAL_SELECTION_STATUS;
        return -1;
    }
    if (result != 0)
        return 0;
    if (((ai->acting_unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_FROG)]
             & (STATUS_BYTE_MASK(STATUS_ID_FROG) | STATUS_BYTE_MASK(STATUS_ID_CRITICAL)))
            && (ai->targetability.live.target_setting_flags & AI_TARGET_SETTING_RETREAT_SUPPORT_AVAILABLE))
        || ((ai->acting_unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_DONT_ACT)]
                & STATUS_BYTE_MASK(STATUS_ID_DONT_ACT))
            && (ai->acting_unit_decision->flags & AI_DECISION_FOCUS_TARGET))) {
        ai_copy_u16_values(&ai->saved_acting_unit_decision, ai->acting_unit_decision, sizeof(ai_unit_decision_t));
    /* Resume after the save; restore the decision only when this pass ends. */
    retreat_action:
        if (ai_select_action_retreat() == -1) {
            ai->initial_selection_stage = AI_INITIAL_SELECTION_RETREAT;
            return -1;
        }
        ai_copy_u16_values(ai->acting_unit_decision, &ai->saved_acting_unit_decision, sizeof(ai_unit_decision_t));
        return 0;
    }
autobattle_action:
    result = ai_select_action_autobattle();
    if (result == -1) {
        ai->initial_selection_stage = AI_INITIAL_SELECTION_AUTOBATTLE;
        return -1;
    }
    if (result != 0)
        return 0;
    ai->resume_pending = 0;
general_action:
    if (ai_select_action_general() == -1) {
        ai->initial_selection_stage = AI_INITIAL_SELECTION_GENERAL;
        return -1;
    }
    return 0;
}
