#include "fft/battle_ai.h"

/*
 * Select an action for the retreat-oriented AI path.
 *
 * Try abilities targeting the actor and an existing charge before the broader
 * ability pass. Return -1 to suspend and 0 when selection completes.
 */
s32 ai_select_retreat_action(void) {
    ai_data_t* ai = &g_ai_data_base;
    ai_ability_entry_t* entry;
    s32 i, result;

    if (g_ai_data_base.resume_pending) {
        /* Resume: re-enter the step that suspended. */
        switch (g_ai_retreat_phase) {
        case AI_RETREAT_MOVEMENT:
            goto movement;
        case AI_RETREAT_SELF_ABILITIES:
            goto self_abilities;
        case AI_RETREAT_CHARGING:
            goto charging;
        default:
            goto other_abilities;
        }
    }
    ai_store_main_target_id_and_focus_on_target_flag(ai_find_nearest_target(AI_NEAREST_HEALER_OR_CRYSTAL));
movement:
    if (ai_run_cowardly_movement() == -1) {
        g_ai_retreat_phase = AI_RETREAT_MOVEMENT;
        return -1;
    }
    ai->movement_scenario = 1;
    ai_invert_target_priority();
    if (ai_select_active_units(AI_UNIT_FILTER_SPECIFIC, ai->acting_unit_id)) {
    self_abilities:
        if (ai_choose_move_from_move_list() == -1) {
            g_ai_retreat_phase = AI_RETREAT_SELF_ABILITIES;
            return -1;
        }
    }
    if (ai_select_ranked_action())
        return 0;
charging:
    result = ai_evaluate_charging_movement();
    switch (result) {
    case -1:
        g_ai_retreat_phase = AI_RETREAT_CHARGING;
        return -1;
    case 1:
        return 0;
    }
    for (i = 0; i < AI_ABILITY_LIST_COUNT; i++) {
        entry = &ai->ability_lists[ai->acting_unit_compact_index][i];
        if (entry->skillset == SKILLSET_ID_NONE)
            break;
        ai_load_ability_entry(entry);
        /* The aligned skillset/usage pair preserves the target's LHU/SH. */
        if (g_main_action_menu_types_by_skillset[entry->skillset] == ACTION_MENU_TYPE_CHARGE)
            entry->usage.policy.usable = 0;
    }
    if (ai_select_active_units(AI_UNIT_FILTER_ANY, 255)) {
    other_abilities:
        if (ai_choose_move_from_move_list() == -1) {
            g_ai_retreat_phase = AI_RETREAT_OTHER_ABILITIES;
            return -1;
        }
        if (ai_select_ranked_action())
            return 0;
    }
    ai_transfer_ability_data_and_set_defend_flag();
    return 0;
}
