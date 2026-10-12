#include "fft/battle.h"
#include "psx/pad.h"
#include "psx/types.h"

/* IDLING_ACTION_MENUS: act on the command chosen in the player's action menu. Move (0) and 0x19
 * start the move command, Act (1) runs Defend, Equip Change or an ability of the command's
 * skillset, Wait (2) chooses a facing, and the other commands enter target selection or display,
 * the between-turn event check or scripted menus.
 *
 * The kind is switched as s32: the unsigned enum type makes GCC emit sltiu
 * where the target has slti. The empty cases for the unresolved commands 0xfe
 * and 0xff keep the switch a compare tree with the target's split: a
 * two-value range above 0x19 disables the jump table (range > 10 * count). */
void battle_state_handle_idling_action_menus_state(void) {
    battle_ai_command_t* command;
    battle_unit_misc_data_t* misc;
    s32 result;

    if (battle_menu_get_thread_activity() == 0) {
        battle_state_enter_free_cursor();
    }
    if (g_main_input_pressed_buttons & PSX_PAD_SELECT) {
        battle_menu_open_help();
    }
    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command = (battle_ai_command_t*)battle_menu_get_selected_command_address();
    misc = battle_unit_get_casting_misc_data();
    switch ((s32)command->kind) {
    case BATTLE_AI_COMMAND_MOVE:
        if (misc->mount_state != BATTLE_MISC_MOUNT_STATE_NONE) {
            g_battle_casting_unit_id = battle_unit_get_misc_data_by_misc_id(misc->mount_partner_misc_id)->unit_id;
        } else {
            g_battle_casting_unit_id = g_battle_unit_source_misc_id;
        }
        battle_turn_clear_at_list_index();
        battle_state_enter_unit_move();
        break;
    case BATTLE_AI_COMMAND_MOVE_SELF:
        g_battle_casting_unit_id = g_battle_unit_source_misc_id;
        battle_turn_clear_at_list_index();
        battle_state_enter_unit_move();
        break;
    case BATTLE_AI_COMMAND_ACT:
        battle_turn_clear_at_list_index();
        switch (command->data.action.skillset) {
        case SKILLSET_ID_DEFEND:
            misc->command_state.ai = *command;
            battle_status_inflict_defending_to_battle_id(misc->battle_data->misc_unit_id);
            misc->turn_action_flags |= BATTLE_MISC_TURN_ACTION_ACTED;
            battle_unit_update_display_by_misc_id(misc->unit_id);
            battle_action_set_only_action_taken(misc->battle_data->misc_unit_id);
            battle_state_enter_after_command();
            break;
        case SKILLSET_ID_EQUIP_CHANGE:
            misc->command_state.ai = *command;
            misc->turn_action_flags |= BATTLE_MISC_TURN_ACTION_ACTED;
            battle_status_clear_action_state(misc->battle_data);
            battle_ai_init_unit_abilities(misc->battle_data->misc_unit_id);
            battle_unit_update_display_by_misc_id(misc->unit_id);
            battle_action_set_only_action_taken(misc->battle_data->misc_unit_id);
        case SKILLSET_ID_BLANK_04:
        case SKILLSET_ID_MIMIC:
            battle_state_enter_after_command();
            break;
        case SKILLSET_ID_ATTACK:
        default:
            misc->command_state.ai = *command;
            misc->command_state.ai.data.action.unit_id = misc->battle_data->misc_unit_id;
            misc->command_state.ai.data.action.targeting_type = BATTLE_TARGETING_TYPE_TILE;
            battle_action_resolve_command_ability(&misc->command_state.ai.data.action);
            misc->used_ability_id = misc->command_state.ai.data.action.ability_id;
            result = battle_target_set_panels_for_action((u8*)&misc->command_state.ai.data.action);
            misc->ability_preview_phase = result;
            switch (result) {
            case BATTLE_TARGET_PANELS_UNIT_OR_TILE:
            case BATTLE_TARGET_PANELS_TILE_ONLY:
            case BATTLE_TARGET_PANELS_NONE:
                battle_state_enter_targeting_message();
                break;
            case BATTLE_TARGET_PANELS_NO_TARGETING:
            default:
                battle_target_select_tile();
                break;
            }
            break;
        }
        break;
    case BATTLE_AI_COMMAND_END_TURN:
        battle_state_enter_wait();
        break;
    case BATTLE_AI_COMMAND_AUTOBATTLE_TARGETED_SELECT:
    case BATTLE_AI_COMMAND_AUTOBATTLE_RECOVERY_SELECT:
        misc->target_select_command = command->kind;
        battle_state_enter_target_select_start();
        break;
    case BATTLE_AI_COMMAND_AUTOBATTLE_TARGETED_DISPLAY:
    case BATTLE_AI_COMMAND_AUTOBATTLE_RECOVERY_DISPLAY:
        misc->target_select_command = command->kind;
        battle_state_enter_target_display_start();
        break;
    case BATTLE_AI_COMMAND_AUTOBATTLE_FADING_LIFE:
    case BATTLE_AI_COMMAND_AUTOBATTLE_RETREAT:
        battle_menu_set_next_script_action_menus();
        break;
    case BATTLE_AI_COMMAND_NO_MENU_RECORD:
        battle_menu_clear_status_menu_state();
        battle_turn_advance();
        break;
    case BATTLE_AI_COMMAND_REFRESH_CONTROL:
        battle_unit_store_entd_flags_without_control_flag(misc);
        break;
    case BATTLE_AI_COMMAND_UNRESOLVED_FE:
    case BATTLE_AI_COMMAND_UNRESOLVED_FF:
        break;
    }
}
