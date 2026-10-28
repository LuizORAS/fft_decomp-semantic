#include "fft/script.h"
#include "fft/thread.h"
#include "fft/wldcore.h"
#include "psx/pad.h"

/* Step of the list of propositions on offer. Circle takes the highlighted one, unless eight are
 * already out (WLDCORE_TEXT_ID_NO_MORE_JOBS); one tried before is first introduced as such
 * (WLDCORE_TEXT_ID_TOLD_BEFORE) and waits for a press. Once the panel's thread ends the proposition's
 * detail opens (wldcore_proposition_push_detail_level), or Cross returns to the Bar's menu. Select
 * shows the list's help text (0x105f). */
void wldcore_proposition_handle_available_list_input(wldcore_menu_panel_level_t* level) {
    s32 depth;

    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_INPUT_BLOCKED)
        && thread_is_running(WLDCORE_THREAD_SLOT_WINDOW_PANEL) == 0) {
        wldcore_proposition_available_list_close_level(level);
        if (level->result != 0) {
            wldcore_list_store_cursor_state(WLDCORE_CURSOR_SLOT_AVAILABLE_PROPOSITIONS);
            wldcore_proposition_push_detail_level(
                g_wldcore_list_entry_values[g_wldcore_window_panel_render_state.selected_index] & TEXT_ID_ENTRY_MASK);
            return;
        }
        wldcore_list_clear_cursor_state(WLDCORE_CURSOR_SLOT_AVAILABLE_PROPOSITIONS);
        wldcore_menu_dispatch_resume_handler();
        return;
    }
    if (level->result == 2) {
        if (g_wldcore_input_new_button_presses & (PSX_PAD_CIRCLE | PSX_PAD_CROSS)) {
            level->result = 1;
            g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
        }
    } else if ((g_wldcore_input_new_button_presses & PSX_PAD_CROSS) && g_main_file_still_loading == 0) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        level->result = 0;
        thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
        g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
    } else if ((g_wldcore_input_new_button_presses & PSX_PAD_SELECT) && level->entry_count != 0) {
        wldcore_proposition_available_list_block_all_buttons(level);
        wldcore_menu_push_message_level(0x105F, 1);
    } else if ((g_wldcore_input_new_button_presses & PSX_PAD_CIRCLE) && level->entry_count != 0
        && g_main_file_still_loading == 0) {
        if (g_main_save_proposition_count >= 8) {
            wldcore_sound_play_effect(MAIN_SFX_INVALID);
            thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_NO_MORE_JOBS, 0);
            return;
        }
        wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
        thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
        if (script_get_variable(
                (g_wldcore_list_entry_values[g_wldcore_window_panel_render_state.selected_index] & TEXT_ID_ENTRY_MASK)
                + EVENT_SCRIPT_VAR_PROPOSITION_FIRST)
            & WLDCORE_PROPOSITION_VARIABLE_ATTEMPTED) {
            depth = g_wldcore_menu_stack_depth;
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].window_pair_render.first_window].flags
                |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].window_pair_render.second_window]
                .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[depth - 2]
                                                .window_pair_render.render_index]
                .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
            thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_TOLD_BEFORE, 0);
            level->result = 2;
            return;
        }
        level->result = 1;
        g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
    }
}
