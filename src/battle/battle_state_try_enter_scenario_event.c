#include "fft/battle.h"
#include "psx/types.h"

s32 battle_state_try_enter_scenario_event(battle_unit_misc_data_t* unit) {
    s32 result;

    if (g_battle_action_turn_event_type == BATTLE_TURN_EVENT_UNIT_READY
        || g_battle_action_turn_event_type == BATTLE_TURN_EVENT_NONE) {
        result = battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_SCENARIO_CHECK, 0, unit->battle_data->misc_unit_id, 0, 1);
    } else if (g_battle_action_turn_event_type == BATTLE_TURN_EVENT_ABILITY_READY
        || g_battle_action_turn_event_type == BATTLE_TURN_EVENT_ACTION_RESULT) {
        result = battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_SCENARIO_CHECK, 2, unit->battle_data->misc_unit_id, 0, 1);
        if (result == BATTLE_MENU_SYSTEM_DISPATCH_EVENT && battle_script_get_event_finish_operation() != 0) {
            if (unit->spritesheet_id < BATTLE_SPRITESHEET_ID_RAMZA_END) {
                result = battle_menu_init_system_function(
                    BATTLE_MENU_SYSTEM_COMMAND_SCENARIO_CHECK, 1, unit->battle_data->misc_unit_id, 0, 1);
            } else {
                battle_menu_init_system_function(
                    BATTLE_MENU_SYSTEM_COMMAND_SCENARIO_CHECK, 0, unit->battle_data->misc_unit_id, 0, 1);
            }
        } else {
            result = battle_menu_init_system_function(
                BATTLE_MENU_SYSTEM_COMMAND_SCENARIO_CHECK, 1, unit->battle_data->misc_unit_id, 0, 1);
        }
    } else {
        result = battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_SCENARIO_CHECK, 1, unit->battle_data->misc_unit_id, 0, 1);
    }
    if (result == BATTLE_MENU_SYSTEM_DISPATCH_EVENT) {
        g_battle_previous_game_state = g_battle_game_state;
        battle_action_set_casting_unit_id_ff();
        return 1;
    }
    return 0;
}
