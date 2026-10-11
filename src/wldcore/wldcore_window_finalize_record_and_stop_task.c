#include "fft/wldcore.h"

/* Clear the text-task flag and move a window record to the sequence after
 * its current one, restarting the animation with the default palette. */
void wldcore_window_finalize_record_and_stop_task(s32* param) {
    s32 index = *param;
    world_animated_render_record_t* entry = &g_wldcore_window_records[index];

    g_world_thread_task_active = 0;
    entry->sequence = g_wldcore_window_records[index].sequence + 1;
    g_wldcore_window_records[index].palette = 0;
    entry->anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;
}
