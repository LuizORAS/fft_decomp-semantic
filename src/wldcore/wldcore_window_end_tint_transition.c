#include "fft/wldcore.h"

/* Ends the window tint transition and restores the parent content record. */
void wldcore_window_end_tint_transition(wldcore_window_tint_request_t* request) {
    g_wldcore_window_records[request->window_a].palette = g_wldcore_window_render_records[request->render_a].palette
        = g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 2]
                                              .list_window.content_render]
              .palette = 0;
    g_wldcore_window_records[request->window_a].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
    g_wldcore_window_records[request->window_a].frame_index = g_wldcore_window_records[request->window_a].anim_counter
        = 0;
    if (g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 2]
                                            .list_window.content_render]
            .flags
        & WLDCORE_DISPLAY_OBJECT_HIDDEN) {
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 2]
                                            .list_window.content_render]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_main_system_flags &= ~MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 2]
                                            .list_window.content_render]
            .flags |= WLDCORE_DISPLAY_OBJECT_POP_IN;
    }
}
