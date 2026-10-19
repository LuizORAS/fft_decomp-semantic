#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_target.h"
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
    battle_unit_t* unit;
    s32 result;

    if (battle_menu_get_thread_activity() == 0) {
        battle_state_enter_free_cursor();
    }
    if (g_main_input_pressed_buttons & PSX_PAD_SELECT) {
        battle_state_enter_action_help_menu();
    }
    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command = battle_menu_get_selected_command_address();
    unit = battle_unit_get_casting();
    switch ((s32)command->kind) {
    case BATTLE_AI_COMMAND_MOVE:
        if (unit->mount_state != BATTLE_UNIT_MOUNT_STATE_NONE) {
            g_battle_casting_runtime_id = battle_unit_get_by_runtime_id(unit->mount_partner_runtime_id)->runtime_id;
        } else {
            g_battle_casting_runtime_id = g_battle_unit_source_runtime_id;
        }
        battle_turn_clear_at_list_index();
        battle_state_enter_unit_move();
        break;
    case BATTLE_AI_COMMAND_MOVE_SELF:
        g_battle_casting_runtime_id = g_battle_unit_source_runtime_id;
        battle_turn_clear_at_list_index();
        battle_state_enter_unit_move();
        break;
    case BATTLE_AI_COMMAND_ACT:
        battle_turn_clear_at_list_index();
        switch (command->data.action.skillset_id) {
        case SKILLSET_ID_DEFEND:
            unit->command_state.ai = *command;
            battle_status_inflict_defending_to_battle_id(unit->stats->battle_id);
            unit->turn_action_flags |= BATTLE_UNIT_TURN_ACTION_ACTED;
            battle_unit_update_display_by_runtime_id(unit->runtime_id);
            battle_action_set_only_action_taken(unit->stats->battle_id);
            battle_state_enter_after_command();
            break;
        case SKILLSET_ID_EQUIP_CHANGE:
            unit->command_state.ai = *command;
            unit->turn_action_flags |= BATTLE_UNIT_TURN_ACTION_ACTED;
            battle_status_clear_action_state(unit->stats);
            battle_ai_init_unit_abilities(unit->stats->battle_id);
            battle_unit_update_display_by_runtime_id(unit->runtime_id);
            battle_action_set_only_action_taken(unit->stats->battle_id);
        case SKILLSET_ID_BLANK_04:
        case SKILLSET_ID_MIMIC:
            battle_state_enter_after_command();
            break;
        case SKILLSET_ID_ATTACK:
        default:
            unit->command_state.ai = *command;
            unit->command_state.ai.data.action.actor_id = unit->stats->battle_id;
            unit->command_state.ai.data.action.targeting_type = BATTLE_TARGETING_TYPE_TILE;
            battle_action_resolve_command_ability(&unit->command_state.ai.data.action);
            unit->used_ability_id = unit->command_state.ai.data.action.ability_id;
            result = battle_target_set_panels_for_action(&unit->command_state.ai.data.action);
            unit->ability_preview_phase = result;
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
        unit->target_select_command = command->kind;
        battle_state_enter_target_select_start();
        break;
    case BATTLE_AI_COMMAND_AUTOBATTLE_TARGETED_DISPLAY:
    case BATTLE_AI_COMMAND_AUTOBATTLE_RECOVERY_DISPLAY:
        unit->target_select_command = command->kind;
        battle_state_enter_target_display_start();
        break;
    case BATTLE_AI_COMMAND_AUTOBATTLE_FADING_LIFE:
    case BATTLE_AI_COMMAND_AUTOBATTLE_RETREAT:
        battle_state_enter_open_action_menus();
        break;
    case BATTLE_AI_COMMAND_NO_MENU_RECORD:
        battle_menu_clear_status_menu_state();
        battle_turn_advance();
        break;
    case BATTLE_AI_COMMAND_REFRESH_CONTROL:
        battle_unit_store_entd_flags_without_control_flag(unit);
        break;
    case BATTLE_AI_COMMAND_UNRESOLVED_FE:
    case BATTLE_AI_COMMAND_UNRESOLVED_FF:
        break;
    }
}
