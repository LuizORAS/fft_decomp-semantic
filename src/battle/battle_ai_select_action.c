#include "fft/battle.h"

/* Choose the acting unit's action for battle_ai_build_command, resuming the pass that suspended. A
 * fresh choice clears the ranked actions and the movement baseline and fills the foe proximity grid. A
 * status that takes control decides first (battle_ai_choose_status_action). A Frog or Critical unit
 * with retreat support (BATTLE_AI_TARGET_SETTING_RETREAT_SUPPORT_AVAILABLE), or a Don't Act unit with a
 * focus target, runs away (battle_ai_select_retreat_action) and gets its decision record back
 * afterwards. Otherwise the unit's Auto-Battle mode decides (battle_ai_handle_autobattle), and without
 * one the general search (battle_ai_select_general_action). Returns -1 to suspend, else 0. */
s32 battle_ai_select_action(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 result;

    if (g_battle_ai_data_base.resume_pending) {
        /* Resume: re-enter the step that suspended. */
        switch (g_battle_ai_data_base.initial_targeting_state) {
        case 0:
            goto status_action;
        case 2:
            goto fallback_action;
        case 3:
            goto native_action;
        default:
            goto general_action;
        }
    }
    for (result = 0; result < 8; result++)
        battle_ai_reset_action_ranking_fields(&ai->ranked_actions[result]);
    battle_ai_reset_action_ranking_fields(&ai->current_action);
    battle_ai_reset_action_ranking_fields(&ai->best_action);
    battle_ai_reset_action_ranking_fields(&ai->movement_baseline_action);
    battle_ai_reset_action_ranking_fields(&ai->selected_action);
    battle_ai_fill_foe_proximity_grid();
status_action:
    result = battle_ai_choose_status_action();
    if (result == -1) {
        ai->initial_targeting_state = 0;
        return -1;
    }
    if (result != 0)
        return 0;
    if (((ai->acting_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FROG)]
             & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRITICAL)))
            && (ai->targetability.live.target_setting_flags & BATTLE_AI_TARGET_SETTING_RETREAT_SUPPORT_AVAILABLE))
        || ((ai->acting_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DONT_ACT)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DONT_ACT))
            && (ai->acting_unit_decision->flags & BATTLE_AI_DECISION_FOCUS_TARGET))) {
        battle_ai_transfer_halfword_values(
            &ai->saved_acting_unit_decision, ai->acting_unit_decision, sizeof(battle_ai_unit_decision_t));
    /* Resume after the save; restore the decision only when this pass ends. */
    fallback_action:
        if (battle_ai_select_retreat_action() == -1) {
            ai->initial_targeting_state = 2;
            return -1;
        }
        battle_ai_transfer_halfword_values(
            ai->acting_unit_decision, &ai->saved_acting_unit_decision, sizeof(battle_ai_unit_decision_t));
        return 0;
    }
native_action:
    result = battle_ai_handle_autobattle();
    if (result == -1) {
        ai->initial_targeting_state = 3;
        return -1;
    }
    if (result != 0)
        return 0;
    ai->resume_pending = 0;
general_action:
    if (battle_ai_select_general_action() == -1) {
        ai->initial_targeting_state = 7;
        return -1;
    }
    return 0;
}
