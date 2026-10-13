#include "fft/wldcore.h"
#include "psx/pad.h"

/*
 * Per-frame step of the sound novel reader (level SOUND_NOVEL): it runs the novel's script
 * (wldcore_sound_novel_run_until_yield), L1 and R1 raise and lower the background brightness
 * (shown on a gauge for a second), and at the end it remembers the play time, resets the novel's
 * record and returns through a screen transition to the treasure the book was opened from.
 *
 * The unused array reproduces the target's 0x40-byte frame. The 0x140 state-flag test is a
 * word load (`lw`) of the u16 g_wldcore_sound_novel_record.state_flags in the target, so it
 * reads the flags through an s32 cast; the u16 declaration gives `lhu`.
 */
void wldcore_menu_step_sound_novel_level(wldcore_menu_sound_novel_level_t* level) {
    s32 unused[2];
    u16* state_flags;
    wldcore_sortbox_state_t* fade;
    s16* brightness;
    u32 buttons;
    s32 width;
    s32 window;
    u16 shade;

    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_INPUT_BLOCKED) && !(g_main_system_flags & MAIN_SYSTEM_FLAG_FADING)) {
        switch (level->phase) {
        case WLDCORE_SOUND_NOVEL_PHASE_OPEN_HISTORY:
            if (level->countdown == 0) {
                g_main_system_flags ^= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
                wldcore_menu_push_sound_novel_history_level();
                return;
            }
            level->countdown--;
            return;
        case WLDCORE_SOUND_NOVEL_PHASE_END:
            if (world_thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE) != 0) {
                return;
            }
            g_wldcore_menu_blocked_buttons = 0;
            g_main_system_flags ^= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
            wldcore_capture_state_and_play_time();
            fade = &g_wldcore_screen_fade_state;
            g_wldcore_screen_fade_state.priorities[0] = 0xF;
            g_wldcore_screen_fade_state.flags[1] = 0;
            g_wldcore_window_record_count -= 3;
            g_wldcore_window_render_record_count -= 2;
            g_wldcore_window_render_object_count -= 5;
            g_wldcore_menu_stack_depth--;
            fade->flags[0] = (fade->flags[0] & ~WLDCORE_SCREEN_FADE_BOX_VISIBLE) | WLDCORE_SCREEN_FADE_OUT;
            main_sound_stop_weather_sfx_music();
            main_sound_remove_vfx_resource(g_wldcore_sound_novel_sound_resource);
            wldcore_sound_novel_reset_selected_record(g_wldcore_sound_novel_index);
            g_wldcore_sound_novel_countdown_frames = 0;
            wldcore_menu_push_screen_transition_level(WLDCORE_MENU_LEVEL_TREASURE_DETAIL, level->message, 0);
            return;
        }
    }

    state_flags = &g_wldcore_sound_novel_record.state_flags;
    if (*state_flags & WLDCORE_SOUND_NOVEL_STATE_END) {
        return;
    }

    if (wldcore_sound_novel_run_until_yield() != 0) {
        if (g_wldcore_current_button_input & (PSX_PAD_L1 | PSX_PAD_R1)) {
            if (g_wldcore_current_button_input & PSX_PAD_L1) {
                if (g_wldcore_sound_novel_record.brightness < 0xFF) {
                    g_wldcore_sound_novel_record.brightness++;
                }
            }
            if (g_wldcore_current_button_input & PSX_PAD_R1) {
                brightness = &g_wldcore_sound_novel_record.brightness;
                if (*brightness != 0) {
                    (*brightness)--;
                }
            }
            shade = g_wldcore_sound_novel_record.brightness;
            g_wldcore_screen_fade_state.boxes[1].r = shade;
            g_wldcore_screen_fade_state.boxes[1].g = shade;
            g_wldcore_screen_fade_state.boxes[1].b = shade;
            level->fade_timer = 0x3C;
        }

        if (level->fade_timer != 0) {
            g_wldcore_window_render_records[level->render_index].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_records[level->overlay_window].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
            width = 0x100 - g_wldcore_sound_novel_record.brightness;
            g_wldcore_window_render_records[level->render_index].bounds.dimensions.x = width / 4;
            level->fade_timer--;
        } else {
            g_wldcore_window_render_records[level->render_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_records[level->overlay_window].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        }

        if (world_menu_is_message_waiting_for_confirm() != 0) {
            buttons = g_wldcore_new_button_presses;
            if (buttons & PSX_PAD_CIRCLE) {
                return;
            }
            g_wldcore_window_records[level->left_window].palette = g_wldcore_window_records[level->right_window].palette
                = 0;
            if (buttons & PSX_PAD_SELECT) {
                wldcore_menu_push_message_level(0x1036, 1);
                return;
            }
            if ((buttons & PSX_PAD_START)
                && (g_wldcore_sound_novel_record.state_flags & WLDCORE_SOUND_NOVEL_STATE_PARENT_WINDOWS_VISIBLE)
                && !(g_wldcore_sound_novel_record.state_flags & WLDCORE_SOUND_NOVEL_STATE_WHITE_OVERLAY)) {
                g_wldcore_window_records[level->left_window].sequence
                    = WLDCORE_ANIM_SEQUENCE_SOUND_NOVEL_START_BUTTON_PRESS;
                g_wldcore_window_records[level->left_window].frame_index
                    = g_wldcore_window_records[level->left_window].anim_counter = 0;
                level->fade_timer = 0;
                g_wldcore_window_render_records[level->render_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
                g_wldcore_window_records[level->overlay_window].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
                wldcore_menu_push_sound_novel_quit_level();
                return;
            }
            if ((g_wldcore_current_button_input & PSX_PAD_SQUARE)
                && (g_wldcore_sound_novel_record.state_flags & WLDCORE_SOUND_NOVEL_STATE_PARENT_WINDOWS_VISIBLE)
                && g_wldcore_sound_novel_record.text_history_3 != -1) {
                g_wldcore_window_records[level->right_window].sequence
                    = WLDCORE_ANIM_SEQUENCE_SOUND_NOVEL_SQUARE_BUTTON_PRESS;
                window = level->right_window;
                g_wldcore_window_records[window].anim_counter = 0;
                g_wldcore_window_records[window].frame_index = 0;
                level->fade_timer = 0;
                g_wldcore_window_render_records[level->render_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
                g_wldcore_window_records[level->overlay_window].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
                g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
                level->phase = WLDCORE_SOUND_NOVEL_PHASE_OPEN_HISTORY;
                level->countdown = 8;
            }
        } else {
            g_wldcore_window_records[level->left_window].palette = g_wldcore_window_records[level->right_window].palette
                = 0xA;
        }
    } else {
        if (world_thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE) != 0) {
            world_thread_request_stop(WLDCORE_THREAD_SLOT_MESSAGE);
        }
        wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_OUT, 0x20);
        main_sound_stop_weather_sfx_music();
        level->phase = WLDCORE_SOUND_NOVEL_PHASE_END;
        g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
        if (!(*state_flags & WLDCORE_SOUND_NOVEL_STATE_WHITE_OVERLAY)) {
            wldcore_fade_start_screen(WLDCORE_SCREEN_FADE_OUT, 0x10);
        }
    }
}
