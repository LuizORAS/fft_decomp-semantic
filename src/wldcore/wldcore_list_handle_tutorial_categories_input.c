#include "fft/wldcore.h"
#include "psx/pad.h"

/* Per-frame step of the tutorial menu list level.
 *
 * Mode 0 runs the cursor and the four entries: formation tutorials, the
 * wldcore_list_open_tutorial_mask_1_entries list, the chapter-5 tutorial topics, and entry 3 (or cancel),
 * which starts WORLD thread 14 on text 0xb8de and moves to mode 1. Mode 1
 * waits for confirm/cancel and starts the fade out, mode 2 clears VRAM once
 * the fade flag is set and mode 3 pops the level: with system flag 0x80000 it
 * hands back to the caller through 0x10000000, otherwise it fades in and calls
 * wldcore_menu_push_entry_window_level.
 *
 * Names beyond the shared headers remain provisional. */
void wldcore_list_handle_tutorial_categories_input(wldcore_menu_list_window_level_t* level) {
    u32 buttons;
    s32* selected_entry;

    if (g_wldcore_window_render_records[level->frame_render].flags & WLDCORE_DISPLAY_OBJECT_POP_IN) {
        return;
    }
    if (g_main_system_flags & MAIN_SYSTEM_FLAG_INPUT_BLOCKED) {
        if (level->mode == WLDCORE_LIST_WINDOW_MODE_FADE_OR_HELP && !(g_main_system_flags & MAIN_SYSTEM_FLAG_FADING)) {
            level->mode = WLDCORE_LIST_WINDOW_MODE_LEAVE_WAIT;
            wldcore_gfx_clear_vram_and_scratch(1);
        }
        if (level->mode == WLDCORE_LIST_WINDOW_MODE_LEAVE_WAIT
            && world_thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE) == 0) {
            if ((g_main_system_flags & MAIN_SYSTEM_FLAG_TITLE_TUTORIAL) == 0 || g_wldcore_audio_queue.count == 0) {
                g_main_system_flags ^= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
                wldcore_window_set_render_objects_visible(1);
                g_wldcore_menu_stack_depth = g_wldcore_menu_stack_depth - 1;
                g_main_system_flags = g_main_system_flags
                    & ~(MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE | MAIN_SYSTEM_FLAG_HIDE_CALENDAR_DATE
                        | MAIN_SYSTEM_FLAG_HIDE_LOCATION_MARKER);
                wldcore_menu_dispatch_publish_handler();
                if (g_main_system_flags & MAIN_SYSTEM_FLAG_TITLE_TUTORIAL) {
                    g_main_system_flags
                        = (g_main_system_flags ^ (MAIN_SYSTEM_FLAG_RUNNING | MAIN_SYSTEM_FLAG_TITLE_TUTORIAL))
                        | MAIN_SYSTEM_FLAG_RETURN_TO_TITLE;
                    return;
                }
                wldcore_fade_start_screen(WLDCORE_SCREEN_FADE_REDRAW_MAP, 0x10);
                wldcore_menu_push_entry_window_level();
            }
        }
        return;
    }
    if (level->mode == WLDCORE_LIST_WINDOW_MODE_LEAVE_MESSAGE) {
        if (g_wldcore_new_button_presses & (PSX_PAD_CIRCLE | PSX_PAD_CROSS)) {
            level->mode = WLDCORE_LIST_WINDOW_MODE_FADE_OR_HELP;
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0, -1, 0);
            g_main_system_flags = g_main_system_flags | MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
            wldcore_fade_start_screen(WLDCORE_SCREEN_FADE_OUT, 0x10);
            if (g_main_system_flags & MAIN_SYSTEM_FLAG_TITLE_TUTORIAL) {
                wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_OUT, 0x10);
                wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_UNLOAD, 0);
            } else {
                wldcore_sound_enqueue_music_start(WLDCORE_AUDIO_MUSIC(1, MAIN_MUSIC_WORLD_MAP));
            }
            g_wldcore_tutorial_category_saved_cursor = 0;
        }
        return;
    }
    if (g_wldcore_audio_queue.count != 0) {
        return;
    }
    buttons = g_wldcore_new_button_presses;
    if (buttons & PSX_PAD_CROSS) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, 0xB8DE, 0);
        level->mode = WLDCORE_LIST_WINDOW_MODE_LEAVE_MESSAGE;
        g_wldcore_window_record_count -= 2;
        g_wldcore_window_render_record_count -= 1;
        g_wldcore_window_render_object_count -= 3;
        return;
    }
    if (buttons & PSX_PAD_SELECT) {
        wldcore_window_publish_slot_b8dd(level);
        wldcore_menu_push_message_level(level->entries[level->selected_entry] + 0x108B, 1);
        return;
    }
    if (buttons & PSX_PAD_CIRCLE) {
        wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
        selected_entry = &level->selected_entry;
        g_wldcore_tutorial_category_saved_cursor = *selected_entry;
        switch (level->entries[*selected_entry]) {
        case WLDCORE_TUTORIAL_CATEGORY_ROW_FORMATION:
            wldcore_window_publish_slot_b8dd(level);
            g_wldcore_window_records[level->main_window].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_PRESS;
            wldcore_list_open_formation_tutorials();
            return;
        case WLDCORE_TUTORIAL_CATEGORY_ROW_MASK_1_ENTRIES:
            wldcore_window_publish_slot_b8dd(level);
            g_wldcore_window_records[level->main_window].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_PRESS;
            wldcore_list_open_tutorial_mask_1_entries();
            return;
        case WLDCORE_TUTORIAL_CATEGORY_ROW_TOPICS:
            wldcore_load_message_block_if_changed(WLDCORE_MESSAGE_BLOCK_TUTORIAL_TOPICS);
            wldcore_window_publish_slot_b8dd(level);
            g_wldcore_window_records[level->main_window].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_PRESS;
            wldcore_list_open_tutorial_topics();
            return;
        case WLDCORE_TUTORIAL_CATEGORY_ROW_LEAVE:
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, 0xB8DE, 0);
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
        wldcore_sound_play_effect(MAIN_SFX_CURSOR_MOVE);
    }
    if (wldcore_input_check_repeating_directional(PSX_PAD_DOWN) != 0) {
        if (level->selected_entry < level->entry_count - 1) {
            g_wldcore_window_records[level->main_window].position.y += 0x10;
            level->selected_entry++;
        } else {
            g_wldcore_window_records[level->main_window].position.y -= (level->entry_count - 1) * 0x10;
            level->selected_entry = 0;
        }
        wldcore_sound_play_effect(MAIN_SFX_CURSOR_MOVE);
    }
}
