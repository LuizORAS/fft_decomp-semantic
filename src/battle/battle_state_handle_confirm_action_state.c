#include "fft/battle.h"

/* CONFIRM_ACTION: answers 5-7 choose how the ability aims (5 the tile; 6 or 7 the unit on it,
 * when there is one), stored with the target tile for a player-controlled unit, and enter
 * PRE_ATTACK_ANIMATION; 8 or cancel return to target selection. */
void battle_state_handle_confirm_action_state(void) {
    /* Pin: unpinned (either declaration order) GCC puts the command
     * pointer in $s0 and the unit in $s1, the reverse of the target. */
    register battle_unit_misc_data_t* unit asm("$16");
    s32* command_address;
    battle_unit_misc_data_t* target;
    s32 command;

    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
    command_address = battle_menu_get_selected_command_address();
    unit = battle_unit_get_source_misc_data();
    if ((unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) != 0) {
        command = *command_address;
        switch (command) {
        case 5:
            unit->command_state.ai.data.action.targeting_type = *command_address;
            main_util_set_svector(
                &unit->command_state.cursor.target_panel, g_battle_cursor_x, g_battle_cursor_z, g_battle_cursor_y);
            battle_state_enter_pre_attack_animation();
            return;
        case 6:
        case 7:
            target = battle_unit_get_selectable_misc_data_at_map_coords(
                g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
            if (target != 0) {
                unit->command_state.ai.data.action.targeting_type = *command_address;
                unit->command_state.ai.data.action.target_id = target->battle_data->misc_unit_id;
            } else {
                unit->command_state.ai.data.action.targeting_type = 5;
            }
            main_util_set_svector(
                &unit->command_state.cursor.target_panel, g_battle_cursor_x, g_battle_cursor_z, g_battle_cursor_y);
            battle_state_enter_pre_attack_animation();
            return;
        case 8:
        case 0xff:
            battle_target_tint_marked_tiles(BATTLE_TARGET_TINT_RESET, 3);
            battle_target_begin_tile_selection();
            return;
        default:
            break;
        }
    } else {
        command = *command_address;
        switch (command) {
        case 5:
        case 6:
        case 7:
            battle_state_enter_pre_attack_animation();
            return;
        case 8:
        case 0xff:
            battle_target_tint_marked_tiles(BATTLE_TARGET_TINT_RESET, 3);
            battle_target_begin_tile_selection();
            return;
        default:
            break;
        }
    }
}
