#include "fft/wldcore.h"
#include "psx/pad.h"

/* Input step of the battle tutorial list. A confirmed row fades out; once the message thread ends
 * it marks the tutorial watched, sets EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS, makes the row's
 * event (g_wldcore_tutorial_event_ids) the current event and leaves the world map to play it
 * (MAIN_SYSTEM_FLAG_TUTORIAL_EVENT), saving the cursor and the weather. Cross closes the list; Select
 * shows help message 0x108c. */
void wldcore_list_handle_battle_tutorials_input(wldcore_menu_panel_level_t* level) {
    s32 state;
    s16* selected;

    state = level->result;
    if (state == 2) {
        if (world_thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE) == 0) {
            selected = &g_wldcore_window_panel_render_state.selected_index;
            wldcore_set_bit_value(&g_main_tutorial_progress_masks[1], *selected, 1);
            world_script_set_variable(EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS, 1);
            world_script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, g_wldcore_tutorial_event_ids[*selected]);
            wldcore_list_store_cursor_state(WLDCORE_CURSOR_SLOT_BATTLE_TUTORIALS);
            g_main_saved_list_cursor_state = g_wldcore_tutorial_entries_cursor_state;
            g_main_system_flags = (g_main_system_flags | MAIN_SYSTEM_FLAG_TUTORIAL_EVENT) ^ MAIN_SYSTEM_FLAG_RUNNING;
            g_main_saved_weather_variable = world_script_get_variable(EVENT_SCRIPT_VAR_WEATHER);
        }
        return;
    }
    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_INPUT_BLOCKED) && !(g_main_system_flags & MAIN_SYSTEM_FLAG_FADING)) {
        if (state != 0) {
            g_main_system_flags |= MAIN_SYSTEM_FLAG_DISPLAY_OFF;
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0, -1, 0);
            level->result = 2;
            return;
        }
        g_wldcore_menu_ordering_table_offset = 1;
        g_wldcore_menu_blocked_buttons = 0;
        g_main_system_flags ^= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
        g_wldcore_menu_stack_depth--;
        wldcore_list_clear_cursor_state(WLDCORE_CURSOR_SLOT_BATTLE_TUTORIALS);
        wldcore_menu_dispatch_resume_handler();
        return;
    }
    if (g_wldcore_new_button_presses & PSX_PAD_CROSS) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        level->result = 0;
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
        g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
    } else if ((g_wldcore_new_button_presses & PSX_PAD_SELECT) && level->entry_count != 0) {
        wldcore_list_battle_tutorials_block_all_buttons(level);
        wldcore_menu_push_message_level(0x108C, 1);
    } else if ((g_wldcore_new_button_presses & PSX_PAD_CIRCLE) && level->entry_count != 0) {
        wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
        level->result = 1;
        g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
        wldcore_fade_start_screen(WLDCORE_SCREEN_FADE_OUT, 0x10);
    }
}
