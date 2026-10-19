#include "fft/battle.h"
#include "fft/battle_target.h"

/* CONFIRM_ACTION: for a player-controlled unit, answers 5-7 choose how the ability aims (5 the
 * tile; 6 or 7 the unit on it, or the tile when none stands there), stored with the target tile,
 * and enter PRE_ATTACK_ANIMATION. An AI unit keeps the target it chose: 5-7 only enter
 * PRE_ATTACK_ANIMATION. For both, 8 or cancel clear the tile marks and return to target
 * selection. */
void battle_state_handle_confirm_action_state(void) {
    battle_unit_t* unit;
    battle_ai_command_t* command_address;
    battle_unit_t* target;
    s32 command;

    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command_address = battle_menu_get_selected_command_address();
    unit = battle_unit_get_source();
    if ((unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) != 0) {
        command = command_address->kind;
        switch (command) {
        case 5:
            unit->command_state.ai.data.action.targeting_type = command_address->kind;
            main_util_set_svector(
                &unit->command_state.cursor.target_panel, g_battle_cursor_x, g_battle_cursor_z, g_battle_cursor_y);
            battle_state_enter_pre_attack_animation();
            return;
        case 6:
        case 7:
            target = battle_unit_find_selectable_at_tile(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
            if (target != 0) {
                unit->command_state.ai.data.action.targeting_type = command_address->kind;
                unit->command_state.ai.data.action.target_id = target->stats->battle_id;
            } else {
                unit->command_state.ai.data.action.targeting_type = BATTLE_TARGETING_TYPE_TILE;
            }
            main_util_set_svector(
                &unit->command_state.cursor.target_panel, g_battle_cursor_x, g_battle_cursor_z, g_battle_cursor_y);
            battle_state_enter_pre_attack_animation();
            return;
        case 8:
        case 0xff:
            battle_target_tint_marked_tiles(BATTLE_TILE_MARK_NONE, 3);
            battle_target_begin_tile_selection();
            return;
        default:
            break;
        }
    } else {
        command = command_address->kind;
        switch (command) {
        case 5:
        case 6:
        case 7:
            battle_state_enter_pre_attack_animation();
            return;
        case 8:
        case 0xff:
            battle_target_tint_marked_tiles(BATTLE_TILE_MARK_NONE, 3);
            battle_target_begin_tile_selection();
            return;
        default:
            break;
        }
    }
}
