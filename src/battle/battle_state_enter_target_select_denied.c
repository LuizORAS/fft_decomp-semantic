#include "fft/battle.h"
#include "fft/battle_camera.h"

/* Enter the target-select-denied state for the cursor tile: show the summary of a
 * unit standing there and open the refusal variant of the target-confirm prompt
 * for the casting unit.
 *
 * Option 3 is used for an empty tile, 1 for a targeted auto-battle selection and
 * 2 for a recovery selection. Any other selection command over a unit opens no
 * prompt. */
void battle_state_enter_target_select_denied(void) {
    battle_unit_misc_data_t* misc;
    battle_unit_misc_data_t* unit;
    battle_stats_t* stats;
    s32 kind;

    camera_disable_map_pan();
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
    if (kind == AI_COMMAND_AUTOBATTLE_TARGETED_SELECT) {
        battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_TARGET_CONFIRM, 1, unit->battle_data->misc_unit_id, 0, 1);
        return;
    }
    if (kind == AI_COMMAND_AUTOBATTLE_RECOVERY_SELECT) {
        battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_TARGET_CONFIRM, 2, unit->battle_data->misc_unit_id, 0, 1);
    }
}
