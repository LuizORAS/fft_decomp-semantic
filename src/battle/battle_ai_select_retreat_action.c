#include "fft/battle_ai.h"

/* Run away, for Run like a rabbit and for the Frog, Critical or Don't Act unit that
 * battle_ai_select_action sends away. The unit heads for the nearest healer or crystal
 * (battle_ai_build_retreat_movement) and ranks the abilities it can use on itself; then it keeps a
 * charged action when that scores better; then it drops its Charge abilities and ranks the rest on
 * every other unit worth targeting; with none, it only moves (and Defends when Defend is its support
 * ability). Returns -1 to suspend, else 0. */
s32 battle_ai_select_retreat_action(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_ai_ability_entry_t* entry;
    s32 i, result;

    if (g_battle_ai_data_base.resume_pending) {
        /* Resume: re-enter the step that suspended. */
        switch (g_battle_ai_retreat_phase) {
        case BATTLE_AI_RETREAT_MOVEMENT:
            goto movement;
        case BATTLE_AI_RETREAT_SELF_ABILITIES:
            goto self_abilities;
        case BATTLE_AI_RETREAT_CHARGING:
            goto charging;
        default:
            goto other_abilities;
        }
    }
    battle_ai_focus_on_target(battle_ai_find_nearest_target(BATTLE_AI_NEAREST_HEALER_OR_CRYSTAL));
movement:
    if (battle_ai_build_retreat_movement() == -1) {
        g_battle_ai_retreat_phase = BATTLE_AI_RETREAT_MOVEMENT;
        return -1;
    }
    ai->movement_scenario = 1;
    battle_ai_set_movement_baseline();
    if (battle_ai_select_active_units(BATTLE_AI_UNIT_FILTER_SPECIFIC, ai->acting_unit_id)) {
    self_abilities:
        if (battle_ai_evaluate_ability_list() == -1) {
            g_battle_ai_retreat_phase = BATTLE_AI_RETREAT_SELF_ABILITIES;
            return -1;
        }
    }
    if (battle_ai_select_ranked_action())
        return 0;
charging:
    result = battle_ai_evaluate_charged_action();
    switch (result) {
    case -1:
        g_battle_ai_retreat_phase = BATTLE_AI_RETREAT_CHARGING;
        return -1;
    case 1:
        return 0;
    }
    for (i = 0; i < BATTLE_AI_ABILITY_LIST_LENGTH; i++) {
        entry = &ai->ability_lists[ai->acting_unit_compact_index][i];
        if (entry->skillset == SKILLSET_ID_NONE)
            break;
        battle_ai_load_ability_entry(entry);
        /* The aligned skillset/usage pair preserves the target's LHU/SH. */
        if (g_main_action_menu_types_by_skillset[entry->skillset] == ACTION_MENU_TYPE_CHARGE)
            entry->usage.policy.usable = 0;
    }
    if (battle_ai_select_active_units(BATTLE_AI_UNIT_FILTER_ANY, 255)) {
    other_abilities:
        if (battle_ai_evaluate_ability_list() == -1) {
            g_battle_ai_retreat_phase = BATTLE_AI_RETREAT_OTHER_ABILITIES;
            return -1;
        }
        if (battle_ai_select_ranked_action())
            return 0;
    }
    battle_ai_select_move_only_action();
    return 0;
}
