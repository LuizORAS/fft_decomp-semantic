#include "fft/thread.h"
#include "fft/wldcore.h"
#include "psx/pad.h"

/* Step of the list of propositions in progress: Circle or Cross closes the list panel and Select
 * shows its help text (0x1062). Once the panel's thread ends, a chosen proposition opens its call-back
 * question (wldcore_proposition_push_recall_level); Cross returns to the Bar's menu.
 *
 * The target keeps each store of the panel's result ahead of the `li $a0,5` argument of the
 * following cursor-state call, which lands in the jal delay slot; without the empty barriers GCC
 * schedules the li first. */
void wldcore_proposition_handle_active_list_input(wldcore_menu_stack_record_t* level) {
    u32 buttons;
    s32 unused[2]; /* 8-byte frame slot the target reserves */

    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_INPUT_BLOCKED)
        && thread_is_running(WLDCORE_THREAD_SLOT_WINDOW_PANEL) == 0) {
        g_wldcore_menu_ordering_table_offset = 1;
        g_wldcore_menu_blocked_buttons = 0;
        g_main_system_flags ^= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
        g_wldcore_menu_stack_depth--;
        if (level->panel.result != 0) {
            wldcore_proposition_push_recall_level(
                level->panel.entries[g_wldcore_window_panel_render_state.selected_index]);
        } else {
            wldcore_menu_dispatch_resume_handler();
        }
        return;
    }
    buttons = g_wldcore_input_new_button_presses;
    if (buttons & PSX_PAD_CROSS) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        level->panel.result = 0;
        /* Scheduling barrier: keeps the store ahead of the cursor argument. */
        __asm__ volatile("");
        wldcore_list_clear_cursor_state(WLDCORE_CURSOR_SLOT_PROPOSITIONS);
        g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
        thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
    } else if (buttons & PSX_PAD_SELECT) {
        wldcore_proposition_active_list_block_all_buttons(&level->panel);
        wldcore_menu_push_message_level(0x1062, 1);
    } else if (buttons & PSX_PAD_CIRCLE) {
        if (g_wldcore_list_row_flags[g_wldcore_window_panel_render_state.selected_index] != 0) {
            wldcore_sound_play_effect(MAIN_SFX_INVALID);
            thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_RETURN_TOMORROW, 0);
        } else {
            wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
            level->panel.result = 1;
            /* Scheduling barrier: keeps the store ahead of the cursor argument. */
            __asm__ volatile("");
            wldcore_list_store_cursor_state(WLDCORE_CURSOR_SLOT_PROPOSITIONS);
            g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
            thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
        }
    }
}
