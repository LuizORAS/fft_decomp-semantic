#include "fft/battle.h"

/*
 * Post-action state tick: apply the selected menu command, then once the
 * caster's wait counter passes 0x1f, retire one pending move-find result.
 */
void battle_state_handle_crystal_learn_state(void) {
    battle_ai_command_t* command;
    battle_unit_misc_data_t* source;
    battle_unit_misc_data_t* casting;
    u16 counter;

    battle_state_handle_free_cursor_input();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    command = battle_menu_get_selected_command_address();
    source = battle_unit_get_source_misc_data();
    casting = battle_unit_get_casting_misc_data();
    switch ((s32)command->kind) {
    case 2:
        battle_unit_learn_from_crystal(source->battle_data, BATTLE_CRYSTAL_RESTORE_HP_MP);
        g_battle_action_post_action = 1;
        battle_action_finalize_attack_and_flag_reactions(source->battle_data->misc_unit_id);
        battle_gfx_prepare_post_action_display(source);
        battle_unit_update_display_by_misc_id(source->unit_id);
        break;
    case 0:
    case 1:
    case 4:
        battle_unit_learn_from_crystal(source->battle_data, command->kind);
        g_battle_action_post_action = 1;
        break;
    case 7:
    case 8:
    case 0xff:
        g_battle_action_post_action = 1;
        break;
    }

    if (g_battle_action_post_action == 0) {
        return;
    }
    if (g_battle_state_animation_continue_check != 0) {
        return;
    }
    counter = casting->state_frame_counter;
    casting->state_frame_counter = counter + 1;
    if (counter < 0x1f) {
        return;
    }
    if (source->numeric_display.active != 0) {
        return;
    }
    if (casting->numeric_display.active != 0) {
        return;
    }

    if (g_battle_move_find_result & BATTLE_MOVE_POST_EVENT_CRYSTAL_OR_TREASURE) {
        if (battle_unit_get_crystal_or_treasure_at_map_coords(source->map_x, source->map_y, source->map_z) != 0) {
            return;
        }
        g_battle_move_find_result &= ~BATTLE_MOVE_POST_EVENT_CRYSTAL_OR_TREASURE;
        battle_ai_init_selected_action();
    } else if (g_battle_move_find_result & BATTLE_MOVE_POST_EVENT_MOVEMENT_BENEFIT) {
        g_battle_action_post_action_display_phase += 1;
        if (g_battle_action_post_action_display_phase >= 2) {
            g_battle_move_find_result &= ~BATTLE_MOVE_POST_EVENT_MOVEMENT_BENEFIT;
        }
    } else if (g_battle_move_find_result & BATTLE_MOVE_POST_EVENT_ITEM_FOUND) {
        if ((source->animation.wait != 0) && ((source->animation.sequence_id >> 1) >= 0xc)) {
            return;
        }
        source->item_display_active = 0;
        g_battle_move_find_result &= ~BATTLE_MOVE_POST_EVENT_ITEM_FOUND;
    } else if (g_battle_move_find_result & BATTLE_MOVE_POST_EVENT_TRAP_TRIGGERED) {
        g_battle_move_find_result &= ~BATTLE_MOVE_POST_EVENT_TRAP_TRIGGERED;
        battle_ai_init_selected_action();
        battle_unit_update_display_by_misc_id(source->unit_id);
    } else if (g_battle_move_find_result & BATTLE_MOVE_POST_EVENT_CHARGING_CANCEL) {
        g_battle_move_find_result &= ~BATTLE_MOVE_POST_EVENT_CHARGING_CANCEL;
    } else if (g_battle_move_find_result & BATTLE_MOVE_POST_EVENT_SOURCE_DISPLAY) {
        g_battle_move_find_result &= ~BATTLE_MOVE_POST_EVENT_SOURCE_DISPLAY;
    } else if (g_battle_move_find_result & BATTLE_MOVE_POST_EVENT_MOUNT_STATUS_CHANGE) {
        g_battle_move_find_result &= ~BATTLE_MOVE_POST_EVENT_MOUNT_STATUS_CHANGE;
    } else {
        return;
    }
    battle_move_start_next_post_movement_step();
}
