#include "fft/battle.h"

/* CRYSTAL_LEARN, the post-move event state: apply the crystal choice (0, 1, 2 or 4 for
 * battle_unit_learn_from_crystal; 2 also finishes the action; 7, 8 or cancel learn nothing), then,
 * once the casting unit's counter passes 31 frames and the number displays end, wait for the first
 * pending event (BATTLE_MOVE_POST_EVENT_* in g_battle_move_post_move_events) to finish (the crystal
 * or chest gone, both reward phases shown, the item animation over), clear its bit and start the
 * next (battle_move_start_next_post_move_event). */
void battle_state_handle_crystal_learn_state(void) {
    s32* choice;
    battle_unit_misc_data_t* source;
    battle_unit_misc_data_t* casting;
    u16 counter;

    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
    choice = battle_menu_get_selected_command_address();
    source = battle_unit_get_source_misc_data();
    casting = battle_unit_get_casting_misc_data();
    switch (*choice) {
    case 2:
        battle_unit_learn_from_crystal(source->battle_data, 2);
        g_battle_action_post_action = 1;
        battle_action_apply_target_result(source->battle_data->misc_unit_id);
        battle_gfx_prepare_post_action_display(source);
        battle_unit_update_display_by_misc_id(source->unit_id);
        break;
    case 0:
    case 1:
    case 4:
        battle_unit_learn_from_crystal(source->battle_data, *choice);
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
    if (source->numeric_display_active != 0) {
        return;
    }
    if (casting->numeric_display_active != 0) {
        return;
    }

    if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_CRYSTAL_OR_TREASURE) {
        if (battle_unit_get_crystal_or_treasure_at_map_coords(source->map_x, source->map_y, source->map_z) != 0) {
            return;
        }
        g_battle_move_post_move_events &= ~BATTLE_MOVE_POST_EVENT_CRYSTAL_OR_TREASURE;
        battle_ai_init_selected_action();
    } else if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_MOVEMENT_BENEFIT) {
        g_battle_action_post_action_display_phase += 1;
        if (g_battle_action_post_action_display_phase >= 2) {
            g_battle_move_post_move_events &= ~BATTLE_MOVE_POST_EVENT_MOVEMENT_BENEFIT;
        }
    } else if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_ITEM_FOUND) {
        if ((source->animation_countdown != 0) && ((u32)(source->encoded_animation >> 1) >= 0xc)) {
            return;
        }
        source->item_ability_display = 0;
        g_battle_move_post_move_events &= ~BATTLE_MOVE_POST_EVENT_ITEM_FOUND;
    } else if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_TRAP_TRIGGERED) {
        g_battle_move_post_move_events &= ~BATTLE_MOVE_POST_EVENT_TRAP_TRIGGERED;
        battle_ai_init_selected_action();
        battle_unit_update_display_by_misc_id(source->unit_id);
    } else if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_CHARGING_CANCEL) {
        g_battle_move_post_move_events &= ~BATTLE_MOVE_POST_EVENT_CHARGING_CANCEL;
    } else if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_SOURCE_DISPLAY) {
        g_battle_move_post_move_events &= ~BATTLE_MOVE_POST_EVENT_SOURCE_DISPLAY;
    } else if (g_battle_move_post_move_events & BATTLE_MOVE_POST_EVENT_MOUNT_STATUS_CHANGE) {
        g_battle_move_post_move_events &= ~BATTLE_MOVE_POST_EVENT_MOUNT_STATUS_CHANGE;
    } else {
        return;
    }
    battle_move_start_next_post_move_event();
}
