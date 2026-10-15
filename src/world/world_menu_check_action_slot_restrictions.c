#include "fft/battle.h"
#include "fft/world.h"

/* WORLD twin of battle_menu_check_action_slot_restrictions. */
void world_menu_check_action_slot_restrictions(s32 menu, s32 value, s32 check_unit) {
    s32 mode;
    s32 message;
    world_menu_entry_t* parameter;
    battle_stats_t* unit;

    g_world_active_menu = menu;
    parameter = world_thread_get_current_parameter_1();
    mode = 0;
    message = 0;
    unit = world_unit_get_active_turn_unit_stats();
    if (check_unit != 0) {
        if (battle_unit_get_action_block(unit) == BATTLE_UNIT_ACTION_BLOCK_SUBMERGED) {
            message = 0x1003;
            mode = 2;
        } else {
            if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FROG)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG)) {
                message = 0x1007;
                mode = 2;
            } else {
                mode = 2;
                message = g_world_menu_restriction_message_ids[menu] + 0x1000;
            }
        }
    }
    if (message == 0 && menu == ACTION_MENU_TYPE_ARITHMETICKS) {
        message = 0x8009 - value;
        mode = 1;
    }
    if (mode != 0) {
        if ((g_main_game_options.value & GAME_OPTIONS_NAVIGATION_MESSAGES_MASK)
                != GAME_OPTIONS_NAVIGATION_MESSAGES_OFF_BITS
            || (message & 0xff00) != (g_main_game_options.value & GAME_OPTIONS_NAVIGATION_MESSAGES_MASK)) {
            g_world_menu_thread_menu_data[BATTLE_MENU_ID_ACTION_RESTRICTION_MESSAGE].text_id = message;
            world_thread_start(g_world_thread_current_id - 1, world_menu_icon_linked_entry_thread);
            world_thread_set_parameters(g_world_thread_current_id - 1,
                (s32)&g_world_menu_thread_menu_data[BATTLE_MENU_ID_ACTION_RESTRICTION_MESSAGE], 0, 0);
        }
        if (mode == 2) {
            world_sound_set_effect_to_invalid();
            if ((g_main_game_options.value & GAME_OPTIONS_NAVIGATION_MESSAGES_MASK) == 0) {
                world_thread_wait_until_inactive(g_world_thread_current_id - 1);
            }
            world_thread_exit_current();
        }
    } else {
        mode = g_world_menu_action_type_modes[g_battle_action_menu_row_types
                [g_world_menu_thread_menu_data[BATTLE_MENU_ID_SKILLSET_LIST].selected_index]][0];
        if (mode < 0) {
            g_world_menu_action_slot_row_actions = mode;
            world_menu_confirm_action_silently(parameter);
            world_thread_exit_current();
        }
    }
    g_world_menu_restore_depth = 2;
}
