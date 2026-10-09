#include "fft/wldcore.h"

/* Ends the window tint transition and restores the parent content record. */
void wldcore_window_end_tint_transition(wldcore_window_tint_request_t* request) {
    g_wldcore_window_records[request->window_a].palette = g_wldcore_window_render_records[request->render_a].palette
        = g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 2]
                                              .list_window.content_render]
              .palette = 0;
    g_wldcore_window_records[request->window_a].sequence = 2;
    g_wldcore_window_records[request->window_a].frame_index = g_wldcore_window_records[request->window_a].anim_counter
        = 0;
    if (g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 2]
                                            .list_window.content_render]
            .flags
        & 0x10) {
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 2]
                                            .list_window.content_render]
            .flags &= ~0x10;
        g_main_system_flags &= ~0x800;
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 2]
                                            .list_window.content_render]
            .flags |= 0x100;
    }
}
