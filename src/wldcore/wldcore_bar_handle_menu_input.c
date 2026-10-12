/* Input step for the world-map town menu list level.
 *
 * Waits out the screen transition (system flags 4/8), then: cancel or the
 * "leave" row (entry 4) starts the fade back to the map and announces text
 * 0xb807; Select opens the help message for the highlighted row from
 * g_wldcore_bar_row_help_text_ids; confirm dispatches on the row value through a five-entry jump
 * table at 0x80067034 - 0 rumors, 1 the proposition-location list
 * (wldcore_proposition_open_available_list), 2 wldcore_proposition_open_report_list, 3 the proposition list, 4
 * leave; and up/down wrap the cursor, moving the cursor window 16 pixels a row; a move re-announces text 0xb806 when
 * content_render is set, and clears it.
 */

#include "fft/wldcore.h"
#include "psx/pad.h"

/* Step of the Bar's menu. Once its pop-in and any fade are done: Circle opens the highlighted row
 * (the rumors, the propositions on offer, the job reports, the propositions in progress, or Leave),
 * Select shows the row's help text, and the D-pad moves the cursor, wrapping (the first move after a
 * message brings the welcome back). Cross or Leave says goodbye (WLDCORE_TEXT_ID_BAR_GOODBYE) and
 * drops the menu's records; the next press fades the screen and the music back to the map, and once
 * the message box has closed wldcore_bar_close returns to the town's menu. */
void wldcore_bar_handle_menu_input(wldcore_menu_list_window_level_t* level) {
    s32 flags;
    u32 buttons;
    s32* selected_entry;

    if (g_wldcore_window_render_records[level->frame_render].flags & WLDCORE_DISPLAY_OBJECT_POP_IN) {
        return;
    }
    flags = g_main_system_flags;
    if (flags & MAIN_SYSTEM_FLAG_FADING) {
        return;
    }
    if ((flags & MAIN_SYSTEM_FLAG_INPUT_BLOCKED) && world_thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE) == 0) {
        g_main_system_flags ^= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
        wldcore_bar_close(level);
        return;
    }

    if (level->mode != WLDCORE_LIST_WINDOW_MODE_BROWSE) {
        if (!(g_wldcore_new_button_presses & (PSX_PAD_CIRCLE | PSX_PAD_CROSS))) {
            return;
        }
        level->mode = WLDCORE_LIST_WINDOW_MODE_BROWSE;
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0, -1, 0);
        g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
        wldcore_fade_start_screen(WLDCORE_SCREEN_FADE_OUT, 0x10);
        wldcore_sound_enqueue_music_start(WLDCORE_AUDIO_MUSIC(1, MAIN_MUSIC_WORLD_MAP));
        return;
    }

    buttons = g_wldcore_new_button_presses;
    if (buttons & PSX_PAD_CROSS) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        /* Same exit as case 4 below; GCC cross-jumps the two copies. */
        g_wldcore_bar_menu_saved_cursor = 0;
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_BAR_GOODBYE, 0);
        level->mode = WLDCORE_LIST_WINDOW_MODE_LEAVE_MESSAGE;
        g_wldcore_window_record_count -= 2;
        g_wldcore_window_render_record_count -= 1;
        g_wldcore_window_render_object_count -= 3;
        return;
    }
    if (buttons & PSX_PAD_SELECT) {
        wldcore_window_publish_slot_b806(level);
        wldcore_menu_push_message_level(
            g_wldcore_bar_row_help_text_ids[level->entries[level->selected_entry]] | 0x1000, 1);
        return;
    }
    if (buttons & PSX_PAD_CIRCLE) {
        wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
        selected_entry = &level->selected_entry;
        g_wldcore_bar_menu_saved_cursor = *selected_entry;
        if (level->entries[*selected_entry] != WLDCORE_BAR_MENU_ROW_LEAVE) {
            g_wldcore_window_records[level->main_window].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_PRESS;
        }
        switch (level->entries[level->selected_entry]) {
        case WLDCORE_BAR_MENU_ROW_RUMORS:
            wldcore_load_message_block_if_changed(WLDCORE_MESSAGE_BLOCK_RUMORS);
            wldcore_window_publish_slot_b806(level);
            wldcore_bar_open_rumors();
            return;
        case WLDCORE_BAR_MENU_ROW_AVAILABLE_PROPOSITIONS:
            wldcore_load_message_block_if_changed(WLDCORE_MESSAGE_BLOCK_PROPOSITIONS);
            wldcore_window_publish_slot_b806(level);
            wldcore_proposition_open_available_list();
            return;
        case WLDCORE_BAR_MENU_ROW_COMPLETED_PROPOSITIONS:
            wldcore_window_publish_slot_b806(level);
            wldcore_proposition_open_report_list();
            return;
        case WLDCORE_BAR_MENU_ROW_PROPOSITIONS:
            wldcore_window_publish_slot_b806(level);
            wldcore_proposition_open_active_list();
            return;
        case WLDCORE_BAR_MENU_ROW_LEAVE:
            g_wldcore_bar_menu_saved_cursor = 0;
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_BAR_GOODBYE, 0);
            level->mode = WLDCORE_LIST_WINDOW_MODE_LEAVE_MESSAGE;
            g_wldcore_window_record_count -= 2;
            g_wldcore_window_render_record_count -= 1;
            g_wldcore_window_render_object_count -= 3;
            return;
        }
        return;
    }

    if (wldcore_input_check_repeating_directional(PSX_PAD_UP) != 0) {
        if (level->selected_entry != 0) {
            g_wldcore_window_records[level->main_window].position.y -= 0x10;
            level->selected_entry--;
        } else {
            g_wldcore_window_records[level->main_window].position.y += (level->entry_count - 1) * 0x10;
            level->selected_entry = level->entry_count - 1;
        }
    } else if (wldcore_input_check_repeating_directional(PSX_PAD_DOWN) != 0) {
        if (level->selected_entry < level->entry_count - 1) {
            g_wldcore_window_records[level->main_window].position.y += 0x10;
            level->selected_entry++;
        } else {
            g_wldcore_window_records[level->main_window].position.y -= (level->entry_count - 1) * 0x10;
            level->selected_entry = 0;
        }
    } else {
        return;
    }
    wldcore_sound_play_effect(MAIN_SFX_CURSOR_MOVE);
    if (level->content_render != 0) {
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_BAR_WELCOME, 0);
        level->content_render = 0;
    }
}
