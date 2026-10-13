#include "fft/event_option.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Run the panel of the crystal or chest a dead unit left (OPTION_MENU_DEAD_UNIT) for the pickup in
 * g_battle_dead_unit_context (main_crystal_pickup_result_t). The abilities the crystal offers
 * (learned_abilities) become the rows of its ability list. Then, by the pickup's result: 4, a chest, shows
 * the item found, or message 0x101b with a quarter of its price when 99 are already held (the item goes to
 * g_battle_dead_unit_result); 1 runs the ability list (option_menu_run_list_thread, Select opens the help);
 * 2 asks to restore HP and MP (the confirm menu); 3 first offers the choice between the two (the move menu);
 * 0 shows message 0x183c. The choice goes to g_battle_dead_unit_action (0 none, 1 abilities, 2 restore,
 * 4 chest), then the previous menu record comes back and the thread exits.
 *
 * Source-order constraints proven by the byte-exact build:
 *   - `g_battle_menu_thread_menu_data` must be assigned from `&g_option_dead_unit_menu` *before*
 *     `menu`, so the shared address stays a copy (`move`) instead of being
 *     folded into a single definition.
 *   - `g_option_dead_unit_menu.menu_type` must be stored *after* both of those, or
 *     CSE bases the plain object address on `symbol + 0x3a` and derives
 *     `&g_option_dead_unit_menu` with an extra `addiu`.
 *   - `battle_thread_exit_current()` does not return, and the original relies
 *     on that: no `return` follows the early exits, so their epilogues fall
 *     through into the next case instead of being cross-jumped to the shared
 *     tail. */
void option_menu_start_dead_unit_threads(void) {
    main_crystal_pickup_result_t* context = g_battle_dead_unit_context;
    world_menu_entry_t* previous_menu_descriptor = g_battle_menu_thread_menu_data;
    dead_unit_menu_t* menu;
    s32 value_count;
    s32 byte_index;
    s32 bit_index;
    s32 index;
    s16 message_id;

    g_battle_menu_thread_menu_data = (world_menu_entry_t*)&g_option_dead_unit_menu;
    menu = &g_option_dead_unit_menu;
    g_option_dead_unit_menu.menu_type = 5;
    g_option_dead_unit_menu.entries = &g_option_dead_unit_menu_entries;

    for (index = 0x1ff; index >= 0; index--) {
        g_option_dead_unit_menu_flags[index] = 0;
    }

    value_count = 0;
    for (byte_index = 0; byte_index < 0x40; byte_index++) {
        s32 mask = 1;
        /* A pointer local, not `context->flags[byte_index]` in the test: the
         * indexed form folds 0x53 into the load displacement, while the
         * target biases the index and loads at 0(reg). */
        u8* flag_byte = &context->learned_abilities[byte_index];

        for (bit_index = 0; bit_index < 8; bit_index++) {
            if ((*flag_byte & mask) != 0) {
                /* Separate statement: written as one expression, fold
                 * reassociates the ability-name base with `byte_index * 8` and loop.c turns
                 * the pair into a strength-reduced giv. */
                s32 value = bit_index + TEXT_ID_ABILITY_NAME_BASE;

                g_option_menu_at_list_primary_values[value_count++] = byte_index * 8 + value;
            }
            mask *= 2;
        }
    }

    menu->window_x = 0xc8;
    menu->window_y = 0x40;
    menu->window_width = 0x70;
    menu->overall_width = 0x70;
    menu->inner_width = 0x70;
    if (value_count < 8) {
        menu->entries->visible_count = value_count;
        menu->entries->overflow_count = 0;
    } else {
        menu->entries->visible_count = 8;
        menu->entries->overflow_count = value_count - 8;
    }

    if (context->result == 0) {
        message_id = 0x183c;
        ((dead_unit_menu_t*)g_battle_menu_thread_menu_data)->message_id = message_id;
        battle_thread_start(g_battle_thread_current_id - 1, battle_menu_icon_linked_entry_thread);
        battle_thread_set_parameters(g_battle_thread_current_id - 1,
            (s32)((dead_unit_menu_t*)g_battle_menu_thread_menu_data)->message_menu, 0, 0);
        battle_thread_wait_until_inactive(g_battle_thread_current_id - 1);
        battle_thread_wait_frames(2);
        g_battle_dead_unit_action = 0;
        g_battle_menu_thread_menu_data = previous_menu_descriptor;
        g_battle_menu_pending_selection[0] = 0;
        battle_thread_exit_current();
    }

    g_battle_dead_unit_result = 0;
    if (context->result == 4) {
        if (main_item_get_total_equipment_quantity(context->treasure_item, 1) == 0x63) {
            g_battle_text_substitution_values[0] = context->treasure_item;
            g_battle_text_substitution_values[1] = context->treasure_item;
            message_id = 0x101b;
            g_battle_text_substitution_values[2] = main_item_get_data_pointer(context->treasure_item)->price >> 2;
            g_battle_dead_unit_result = context->treasure_item;
        } else {
            message_id = 0x101a;
            g_battle_text_substitution_values[0] = context->treasure_item;
            g_battle_dead_unit_result = context->treasure_item;
        }
        ((dead_unit_menu_t*)g_battle_menu_thread_menu_data)->message_id = message_id;
        battle_thread_start(g_battle_thread_current_id - 1, battle_menu_icon_linked_entry_thread);
        battle_thread_set_parameters(g_battle_thread_current_id - 1,
            (s32)((dead_unit_menu_t*)g_battle_menu_thread_menu_data)->message_menu, 0, 0);
        battle_thread_wait_until_inactive(g_battle_thread_current_id - 1);
        battle_thread_wait_frames(2);
        g_battle_dead_unit_action = 4;
        g_battle_menu_thread_menu_data = previous_menu_descriptor;
        g_battle_menu_pending_selection[0] = 0;
        battle_thread_exit_current();
    }

    battle_thread_set_parameters(NATIVE_THREAD_SLOT_TILE_INFO_PANEL, 0, 0, 1);
    battle_thread_set_parameters(NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL, 0, 0, 1);
    battle_thread_set_parameters(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS, 0, 0, 1);
    battle_thread_set_parameters(NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS, 0, 0, 1);
    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_TILE_INFO_PANEL);
    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL);
    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS);
    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS);

    for (;;) {
        if (context->result == 3) {
            g_option_dead_unit_secondary_result = 0xff;
            battle_thread_start(g_battle_thread_current_id - 1, battle_menu_run_icon_selection_loop);
            battle_thread_set_parameters(g_battle_thread_current_id - 1,
                (s32)((dead_unit_menu_t*)g_battle_menu_thread_menu_data)->move_menu, 0, 0);
            battle_thread_wait_until_inactive(g_battle_thread_current_id - 1);
        }

        g_option_dead_unit_primary_result = 0xff;
        if ((context->result == 3 && g_option_dead_unit_secondary_result == 0) || context->result == 1) {
            battle_thread_start(g_battle_thread_current_id - 2, option_menu_run_list_thread);
            battle_thread_set_parameters(g_battle_thread_current_id - 2, (s32)g_battle_menu_thread_menu_data, 0, 0);
            battle_thread_start(NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU, battle_menu_icon_linked_entry_thread);
            battle_thread_set_parameters(NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU,
                (s32)((dead_unit_menu_t*)g_battle_menu_thread_menu_data)->at_list_menu, 0, 0);
            for (;;) {
                battle_thread_yield();
                if (battle_thread_is_running_asm(g_battle_thread_current_id - 3) != 0) {
                    battle_thread_start(NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU_CHILD, option_thread_wait_forever);
                } else {
                    battle_thread_suspend(NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU_CHILD);
                }
                if (battle_thread_is_running_asm(g_battle_thread_current_id - 1) == 0) {
                    break;
                }
                g_option_dead_unit_controller_input = battle_script_get_controller_input_pointer(0);
                if ((*g_option_dead_unit_controller_input & PSX_PAD_SELECT) != 0) {
                    battle_menu_start_help_menu_thread();
                }
            }

            battle_thread_wait_until_inactive(g_battle_thread_current_id - 1);
            battle_thread_set_parameters(NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU, 0, 0, 1);
            battle_thread_suspend(NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU_CHILD);
            if (g_option_dead_unit_primary_result == 0) {
                g_battle_dead_unit_action = 1;
                break;
            }
            if (g_option_dead_unit_primary_result == 1 || (g_option_dead_unit_primary_result & 0xff) == 0xff) {
                if (context->result == 3) {
                    continue;
                }
            }
        }

        if ((g_option_dead_unit_secondary_result == 1 && context->result == 3) || context->result == 2) {
            g_option_dead_unit_primary_result = 0xff;
            battle_thread_start(g_battle_thread_current_id - 1, battle_menu_icon_linked_entry_thread);
            battle_thread_set_parameters(g_battle_thread_current_id - 1,
                (s32)((dead_unit_menu_t*)g_battle_menu_thread_menu_data)->confirm_menu, 0, 0);
            battle_thread_wait_until_inactive(g_battle_thread_current_id - 1);
            if (g_option_dead_unit_primary_result == 0) {
                g_battle_dead_unit_action = 2;
                break;
            }
            if (context->result != 2) {
                continue;
            }
        }
        g_battle_dead_unit_action = 0;
        break;
    }

    battle_thread_wait_frames(0x14);
    g_battle_menu_thread_menu_data = previous_menu_descriptor;
    g_battle_menu_pending_selection[0] = 0;
    battle_thread_exit_current();
}
