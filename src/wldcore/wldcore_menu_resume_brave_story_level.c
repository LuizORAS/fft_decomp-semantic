#include "fft/thread.h"
#include "fft/wldcore.h"

/* Resume handler of the Brave Story menu: its windows and texts go back to CLUT 0, the cursor to its
 * idle sequence, WORLD thread 14 shows the Brave Story's prompt (WLDCORE_TEXT_ID_BRAVE_STORY_INTRO)
 * again, the frame and the content come back (popping in) if a section hid them, and the menu returns
 * to browsing. */
void wldcore_menu_resume_brave_story_level(wldcore_menu_stack_record_t* level) {
    g_wldcore_window_records[level->list_window.main_window].palette
        = g_wldcore_window_records[level->list_window.side_window].palette
        = g_wldcore_window_render_records[level->list_window.frame_render].palette
        = g_wldcore_window_render_records[level->list_window.content_render].palette = 0;
    g_wldcore_window_records[level->list_window.upper_window].palette
        = g_wldcore_window_records[level->list_window.lower_window].palette = 0;
    g_wldcore_window_records[level->list_window.main_window].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
    g_wldcore_window_records[level->list_window.main_window].frame_index
        = g_wldcore_window_records[level->list_window.main_window].anim_counter = 0;
    thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_BRAVE_STORY_INTRO, 0);
    if (g_wldcore_window_render_records[level->list_window.frame_render].flags & WLDCORE_DISPLAY_OBJECT_HIDDEN) {
        g_wldcore_window_records[level->list_window.main_window].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[level->list_window.side_window].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_render_records[level->list_window.frame_render].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_render_records[level->list_window.frame_render].flags |= WLDCORE_DISPLAY_OBJECT_POP_IN;
        g_wldcore_window_records[level->list_window.upper_window].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[level->list_window.lower_window].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    }
    if (g_wldcore_window_render_records[level->list_window.content_render].flags & WLDCORE_DISPLAY_OBJECT_HIDDEN) {
        g_wldcore_window_render_records[level->list_window.content_render].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_main_system_flags &= ~MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
        g_wldcore_window_render_records[level->list_window.content_render].flags |= WLDCORE_DISPLAY_OBJECT_POP_IN;
    }
    level->list_window.mode = WLDCORE_LIST_WINDOW_MODE_BROWSE;
}
