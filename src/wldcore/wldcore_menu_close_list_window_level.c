#include "fft/wldcore.h"

/* Clears a list level's window states, restores its hidden records, and
 * queues WORLD thread 14 with message 0xb848. */
void wldcore_menu_close_list_window_level(wldcore_menu_stack_record_t* level) {
    g_wldcore_window_records[level->list_window.main_window].palette
        = g_wldcore_window_records[level->list_window.side_window].palette
        = g_wldcore_window_render_records[level->list_window.frame_render].palette
        = g_wldcore_window_render_records[level->list_window.content_render].palette = 0;
    g_wldcore_window_records[level->list_window.upper_window].palette
        = g_wldcore_window_records[level->list_window.lower_window].palette = 0;
    g_wldcore_window_records[level->list_window.main_window].sequence = 2;
    g_wldcore_window_records[level->list_window.main_window].frame_index
        = g_wldcore_window_records[level->list_window.main_window].anim_counter = 0;
    world_thread_set_parameters(14, 0x19, 0xB848, 0);
    if (g_wldcore_window_render_records[level->list_window.frame_render].flags & 0x10) {
        g_wldcore_window_records[level->list_window.main_window].flags &= ~0x10;
        g_wldcore_window_records[level->list_window.side_window].flags &= ~0x10;
        g_wldcore_window_render_records[level->list_window.frame_render].flags &= ~0x10;
        g_wldcore_window_render_records[level->list_window.frame_render].flags |= 0x100;
        g_wldcore_window_records[level->list_window.upper_window].flags &= ~0x10;
        g_wldcore_window_records[level->list_window.lower_window].flags &= ~0x10;
    }
    if (g_wldcore_window_render_records[level->list_window.content_render].flags & 0x10) {
        g_wldcore_window_render_records[level->list_window.content_render].flags &= ~0x10;
        g_main_system_flags &= ~0x800;
        g_wldcore_window_render_records[level->list_window.content_render].flags |= 0x100;
    }
    level->list_window.mode = 0;
}
