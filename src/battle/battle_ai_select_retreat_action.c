#include "fft/battle.h"

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

    if (g_battle_ai_data_base.decision_state) {
        /* Resume: re-enter the step that suspended. */
        switch (g_battle_ai_retreat_phase) {
        case 0:
            goto movement;
        case 1:
            goto self_abilities;
        case 2:
            goto charging;
        default:
            goto other_abilities;
        }
    }
    battle_ai_store_main_target_id_and_focus_on_target_flag(
        battle_ai_find_nearest_target(BATTLE_AI_NEAREST_HEALER_OR_CRYSTAL));
movement:
    if (battle_ai_build_retreat_movement() == -1) {
        g_battle_ai_retreat_phase = 0;
        return -1;
    }
    ai->movement_scenario = 1;
    battle_ai_set_movement_baseline();
    if (battle_ai_select_active_units(BATTLE_AI_UNIT_FILTER_SPECIFIC, ai->acting_unit_id)) {
    self_abilities:
        if (battle_ai_evaluate_ability_list() == -1) {
            g_battle_ai_retreat_phase = 1;
            return -1;
        }
    }
    if (battle_ai_select_ranked_action())
        return 0;
charging:
    result = battle_ai_evaluate_charged_action();
    switch (result) {
    case -1:
        g_battle_ai_retreat_phase = 2;
        return -1;
    case 1:
        return 0;
    }
    for (i = 0; i < BATTLE_AI_ABILITY_LIST_LENGTH; i++) {
        entry = &ai->ability_lists[ai->acting_unit_battle_id][i];
        if (entry->skillset_flags.bytes.skillset == SKILLSET_ID_NONE)
            break;
        battle_ai_load_ability_entry(entry);
        /* The aligned skillset/usage pair preserves the target's LHU/SH. */
        if (g_main_action_menu_types_by_skillset[entry->skillset_flags.bytes.skillset] == ACTION_MENU_TYPE_CHARGE)
            entry->skillset_flags.packed &= (u16) ~(BATTLE_AI_ABILITY_ENTRY_USABLE << 8);
    }
    if (battle_ai_select_active_units(BATTLE_AI_UNIT_FILTER_ANY, 255)) {
    other_abilities:
        if (battle_ai_evaluate_ability_list() == -1) {
            g_battle_ai_retreat_phase = 3;
            return -1;
        }
        if (battle_ai_select_ranked_action())
            return 0;
    }
    battle_ai_transfer_ability_data_and_set_defend_flag();
    return 0;
}
