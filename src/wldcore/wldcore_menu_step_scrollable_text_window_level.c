#include "fft/wldcore.h"

/* Per-frame step of a location's arrival text: once no fade runs, show the frame and the text with the
 * window-open sound; when the text window has finished (wldcore_text_is_window_finished), release the
 * frame, pop the level, let the label and the animation run again, show the parent level's window and
 * resume it.
 *
 * The target keeps &g_wldcore_scrollable_text_window.render_record_index in a register and passes it
 * biased by -0x44; addressing the window through wldcore_text_scrollable_window_t materialises a second
 * base and does not match. */
void wldcore_menu_step_scrollable_text_window_level(wldcore_menu_stack_record_t* level) {
    s32* index = &g_wldcore_scrollable_text_window.render_record_index;
    s32 depth;

    if (g_wldcore_window_render_records[*index].flags & WLDCORE_DISPLAY_OBJECT_HIDDEN) {
        if (!(g_main_system_flags & MAIN_SYSTEM_FLAG_FADING)) {
            wldcore_sound_play_effect(MAIN_SFX_WINDOW_OPEN);
            g_wldcore_window_records[level->window_index].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_render_records[*index].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        }
    } else if (!(g_wldcore_window_render_records[*index].flags & WLDCORE_DISPLAY_OBJECT_POP_IN)
        && wldcore_text_is_window_finished(&g_wldcore_scrollable_text_window) == 0) {
        g_wldcore_window_record_count--;
        g_wldcore_window_render_object_count--;
        depth = g_wldcore_menu_stack_depth;
        g_wldcore_menu_stack_depth = depth - 1;
        g_main_system_flags &= ~(MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL | MAIN_SYSTEM_FLAG_FREEZE_ANIMATION);
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].window_index].flags
            &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        wldcore_menu_dispatch_resume_handler();
    }
}
