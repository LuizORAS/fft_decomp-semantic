#include "fft/battle.h"

/* Enter TARGET_SELECT_DENIED for the cursor tile (no unit there, or one of the wrong team for an
 * Auto-Battle target choice): show the summary of a unit standing there and open the refusal,
 * variant 3 for an empty tile, 1 for the targeted choice (command 0xc) and 2 for the recovery
 * choice (0xe). Any other command over a unit opens none. */
void battle_state_enter_target_select_denied(void) {
    battle_unit_misc_data_t* misc;
    battle_unit_misc_data_t* unit;
    battle_stats_t* stats;
    s32 kind;

    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_TARGET_SELECT_DENIED;
    misc = battle_unit_get_selectable_misc_data_at_map_coords(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    if (misc != 0) {
        stats = misc->battle_data;
        if (stats != 0) {
            battle_unit_start_view_thread(UNIT_VIEW_MODE_UNIT_SUMMARY, stats->misc_unit_id, 0);
        }
    }
    unit = battle_unit_get_casting_misc_data();
    if (misc == 0) {
        battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_TARGET_CONFIRM, 3, unit->battle_data->misc_unit_id, 0, 1);
        return;
    }
    kind = unit->target_select_command;
    if (kind == BATTLE_AI_COMMAND_AUTOBATTLE_TARGETED_SELECT) {
        battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_TARGET_CONFIRM, 1, unit->battle_data->misc_unit_id, 0, 1);
        return;
    }
    if (kind == BATTLE_AI_COMMAND_AUTOBATTLE_RECOVERY_SELECT) {
        battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_TARGET_CONFIRM, 2, unit->battle_data->misc_unit_id, 0, 1);
    }
}
