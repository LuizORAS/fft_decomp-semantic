#include "fft/wldcore.h"
#include "psx/libgs.h"
#include "psx/pad.h"

/* Input step of the resume prompt. While the picture's dissolve runs it advances one step per frame.
 * Up and Down move between Restart and Read from start; Circle fades out and then opens the novel
 * (wldcore_menu_push_sound_novel_level), first forgetting the saved place for Read from start
 * (wldcore_sound_novel_reset_selected_record); Cross goes back to the treasure's detail
 * (wldcore_menu_push_treasure_detail_level).
 *
 * wldcore_sound_novel_reset_selected_record ignores its argument; retail still passes
 * g_wldcore_sound_novel_index. */
void wldcore_menu_step_sound_novel_resume_level(wldcore_menu_sound_novel_resume_level_t* level) {
    GsIMAGE image;
    s32 toggle;
    s32 buttons;

    if (g_wldcore_window_render_records[level->message_render].flags & WLDCORE_DISPLAY_OBJECT_POP_IN) {
        return;
    }
    if (level->dissolve_step != 0) {
        GsGetTimInfo(g_wldcore_picture_buffer + 1, &image);
        if (wldcore_gfx_step_dissolve_image_upload(&image, level->dissolve_step) == 0) {
            level->dissolve_step = 0;
            return;
        }
        level->dissolve_step++;
        return;
    }
    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_INPUT_BLOCKED) && !(g_main_system_flags & MAIN_SYSTEM_FLAG_FADING)) {
        g_main_system_flags ^= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
        g_wldcore_window_record_count--;
        g_wldcore_window_render_record_count -= 3;
        g_wldcore_window_render_object_count -= 4;
        g_wldcore_menu_stack_depth--;
        if (level->cursor_row != 0) {
            wldcore_sound_novel_reset_selected_record(g_wldcore_sound_novel_index);
        }
        wldcore_menu_push_sound_novel_level(level->mode);
        return;
    }
    buttons = g_wldcore_new_button_presses;
    if (buttons & PSX_PAD_CROSS) {
        g_wldcore_window_record_count--;
        g_wldcore_window_render_record_count -= 3;
        g_wldcore_window_render_object_count -= 4;
        g_wldcore_menu_stack_depth--;
        world_thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_BRAVE_STORY_INTRO, 0);
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        wldcore_menu_push_treasure_detail_level(level->mode);
        return;
    }
    if (buttons & PSX_PAD_CIRCLE) {
        g_wldcore_window_records[level->cursor_window].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_PRESS;
        g_wldcore_window_records[level->cursor_window].frame_index
            = g_wldcore_window_records[level->cursor_window].anim_counter = 0;
        wldcore_fade_start_screen(WLDCORE_SCREEN_FADE_OUT, 0x10);
        g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
        wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
        return;
    }
    if (wldcore_input_check_repeating_directional(PSX_PAD_UP) == 0
        && wldcore_input_check_repeating_directional(PSX_PAD_DOWN) == 0) {
        return;
    }
    level->cursor_row ^= 1;
    toggle = level->cursor_row;
    g_wldcore_window_records[level->cursor_window].position.y
        = g_wldcore_window_render_records[level->option_render].base.y + 0x3c + toggle * 0x10;
    wldcore_sound_play_effect(MAIN_SFX_CURSOR_MOVE);
}
