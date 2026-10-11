#include "fft/world.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Runs the crystal/treasure result panel and its list and choice threads. */
void world_menu_start_dead_unit_threads(void) {
    crystal_pickup_result_t* context = g_world_thread_inner_subroutine_result;
    menu_entry_t* previous_menu_descriptor = g_world_menu_thread_menu_data;
    menu_entry_t* menu;
    s32 value_count;
    s32 byte_index;
    s32 bit_index;
    s32 index;
    s16 message_id;

    /* Publish the menu before its local copy and selector store.
     * Reordering them changes address CSE. */
    g_world_menu_thread_menu_data = g_world_dead_unit_menu;
    menu = g_world_dead_unit_menu;
    g_world_dead_unit_menu[DEAD_UNIT_MENU_LIST].select_text_table = 5;
    g_world_dead_unit_menu[DEAD_UNIT_MENU_LIST].context = &g_world_dead_unit_menu_entries;

    for (index = 0x1ff; index >= 0; index--) {
        g_world_dead_unit_menu_flags[index] = 0;
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

                g_world_menu_at_list_primary_values[value_count++] = byte_index * 8 + value;
            }
            mask *= 2;
        }
    }

    menu->window.x = 0xc8;
    menu->window.y = 0x40;
    menu->window.w = 0x70;
    menu->text_image.width = 0x70;
    menu->inner_width = 0x70;
    if (value_count < 8) {
        menu_text_layout_t* layout;
        layout = menu->context;
        layout->row_count = value_count;
        layout = menu->context;
        layout->hidden_rows = 0;
    } else {
        menu_text_layout_t* layout;
        layout = menu->context;
        layout->row_count = 8;
        layout = menu->context;
        layout->hidden_rows = value_count - 8;
    }

    if (context->result == 0) {
        message_id = 0x183c;
        g_world_menu_thread_menu_data[DEAD_UNIT_MENU_MESSAGE].text_id = message_id;
        world_thread_start(g_world_thread_current_id - 1, world_menu_icon_linked_entry_thread);
        world_thread_set_parameters(
            g_world_thread_current_id - 1, (s32)&g_world_menu_thread_menu_data[DEAD_UNIT_MENU_MESSAGE], 0, 0);
        world_thread_wait_until_inactive(g_world_thread_current_id - 1);
        world_thread_wait_frames(2);
        g_world_dead_unit_action = 0;
        g_world_menu_thread_menu_data = previous_menu_descriptor;
        g_world_menu_pending_selection[0] = 0;
        /* Thread exit does not return; a source return changes epilogue sharing. */
        world_thread_exit_current();
    }

    g_world_dead_unit_result = 0;
    if (context->result == 4) {
        if (main_item_get_total_equipment_quantity(context->treasure_item, 1) == 0x63) {
            g_world_text_substitution_values[0] = context->treasure_item;
            g_world_text_substitution_values[1] = context->treasure_item;
            message_id = 0x101b;
            g_world_text_substitution_values[2] = main_item_get_data_pointer(context->treasure_item)->price >> 2;
            g_world_dead_unit_result = context->treasure_item;
        } else {
            message_id = 0x101a;
            g_world_text_substitution_values[0] = context->treasure_item;
            g_world_dead_unit_result = context->treasure_item;
        }
        g_world_menu_thread_menu_data[DEAD_UNIT_MENU_MESSAGE].text_id = message_id;
        world_thread_start(g_world_thread_current_id - 1, world_menu_icon_linked_entry_thread);
        world_thread_set_parameters(
            g_world_thread_current_id - 1, (s32)&g_world_menu_thread_menu_data[DEAD_UNIT_MENU_MESSAGE], 0, 0);
        world_thread_wait_until_inactive(g_world_thread_current_id - 1);
        world_thread_wait_frames(2);
        g_world_dead_unit_action = 4;
        g_world_menu_thread_menu_data = previous_menu_descriptor;
        g_world_menu_pending_selection[0] = 0;
        world_thread_exit_current();
    }

    world_thread_set_parameters(NATIVE_THREAD_SLOT_TILE_INFO_PANEL, 0, 0, 1);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL, 0, 0, 1);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS, 0, 0, 1);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS, 0, 0, 1);
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_TILE_INFO_PANEL);
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL);
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS);
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS);

    for (;;) {
        if (context->result == 3) {
            g_world_dead_unit_move_result = 0xff;
            world_thread_start(g_world_thread_current_id - 1, world_menu_run_icon_selection_loop);
            world_thread_set_parameters(
                g_world_thread_current_id - 1, (s32)&g_world_menu_thread_menu_data[DEAD_UNIT_MENU_MOVE], 0, 0);
            world_thread_wait_until_inactive(g_world_thread_current_id - 1);
        }

        g_world_dead_unit_menu_result = 0xff;
        if ((context->result == 3 && g_world_dead_unit_move_result == 0) || context->result == 1) {
            world_thread_start(g_world_thread_current_id - 2, world_build_at_list);
            world_thread_set_parameters(g_world_thread_current_id - 2, (s32)g_world_menu_thread_menu_data, 0, 0);
            world_thread_start(NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU, world_menu_icon_linked_entry_thread);
            world_thread_set_parameters(NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU,
                (s32)&g_world_menu_thread_menu_data[DEAD_UNIT_MENU_AT_LIST], 0, 0);
            for (;;) {
                world_thread_yield();
                if (world_thread_is_running_asm(g_world_thread_current_id - 3) != 0) {
                    world_thread_start(NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU_CHILD, world_thread_yield_forever);
                } else {
                    world_thread_suspend(NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU_CHILD);
                }
                if (world_thread_is_running_asm(g_world_thread_current_id - 1) == 0) {
                    break;
                }
                g_world_dead_unit_controller_input = world_input_get_menu_controller(0);
                if ((*g_world_dead_unit_controller_input & PSX_PAD_SELECT) != 0) {
                    world_menu_start_building_thread();
                }
            }

            world_thread_wait_until_inactive(g_world_thread_current_id - 1);
            world_thread_set_parameters(NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU, 0, 0, 1);
            world_thread_suspend(NATIVE_BORROWED_THREAD_SLOT_AT_LIST_MENU_CHILD);
            if (g_world_dead_unit_menu_result == 0) {
                g_world_dead_unit_action = 1;
                break;
            }
            if (g_world_dead_unit_menu_result == 1 || (g_world_dead_unit_menu_result & 0xff) == 0xff) {
                if (context->result == 3) {
                    continue;
                }
            }
        }

        if ((g_world_dead_unit_move_result == 1 && context->result == 3) || context->result == 2) {
            g_world_dead_unit_menu_result = 0xff;
            world_thread_start(g_world_thread_current_id - 1, world_menu_icon_linked_entry_thread);
            world_thread_set_parameters(
                g_world_thread_current_id - 1, (s32)&g_world_menu_thread_menu_data[DEAD_UNIT_MENU_CONFIRM], 0, 0);
            world_thread_wait_until_inactive(g_world_thread_current_id - 1);
            if (g_world_dead_unit_menu_result == 0) {
                g_world_dead_unit_action = 2;
                break;
            }
            if (context->result != 2) {
                continue;
            }
        }
        g_world_dead_unit_action = 0;
        break;
    }

    world_thread_wait_frames(0x14);
    g_world_menu_thread_menu_data = previous_menu_descriptor;
    g_world_menu_pending_selection[0] = 0;
    world_thread_exit_current();
}
