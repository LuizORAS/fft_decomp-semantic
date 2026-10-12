#include "fft/wldcore.h"
#include "psx/pad.h"

/* Step of the rumor list. Circle (with a rumor listed) or Cross closes the list panel; once its
 * thread ends, a chosen rumor opens in a scrollable text window (wldcore_bar_push_rumor_detail_level)
 * and Cross returns to the Bar's menu. Select shows the list's help text (0x105e).
 *
 * The target passes the level to wldcore_bar_rumors_close_level and
 * wldcore_bar_rumors_block_all_buttons even though neither reads an argument;
 * their prototypes take it. */
void wldcore_bar_handle_rumors_input(wldcore_menu_panel_level_t* level) {
    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_INPUT_BLOCKED)
        && world_thread_is_running(WLDCORE_THREAD_SLOT_WINDOW_PANEL) == 0) {
        wldcore_bar_rumors_close_level(level);
        if (level->result != 0) {
            wldcore_list_store_cursor_state(WLDCORE_CURSOR_SLOT_RUMORS);
            wldcore_bar_push_rumor_detail_level(
                g_wldcore_list_entry_values[g_wldcore_window_panel_render_state.selected_index] & TEXT_ID_ENTRY_MASK);
            return;
        }
        wldcore_list_clear_cursor_state(WLDCORE_CURSOR_SLOT_RUMORS);
        g_main_system_flags &= ~MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
        wldcore_menu_dispatch_resume_handler();
        return;
    }
    if ((g_wldcore_new_button_presses & PSX_PAD_CROSS) && g_main_file_still_loading == 0) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        level->result = 0;
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
        g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
    } else if ((g_wldcore_new_button_presses & PSX_PAD_SELECT) && level->entry_count != 0) {
        wldcore_bar_rumors_block_all_buttons(level);
        wldcore_menu_push_message_level(0x105E, 1);
        return;
    } else if ((g_wldcore_new_button_presses & PSX_PAD_CIRCLE) && level->entry_count != 0
        && g_main_file_still_loading == 0) {
        wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
        level->result = 1;
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
        g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
    }
}
