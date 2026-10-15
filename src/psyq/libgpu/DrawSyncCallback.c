/* LIBGPU 800245dc-80024638. */
#include "psx/libetc.h"
#include "psx/libgpu.h"

/* Set the routine called when the queue and the GPU become idle, and return the previous one.
 * main_boot_run_startup installs main_system_handle_draw_sync_callback. */
void* DrawSyncCallback(void* callback) {
    void* old;
    if (g_psyq_gpu_debug_level >= 2) {
        g_psyq_gpu_printf(g_psyq_gpu_draw_sync_callback_format, callback);
    }
    old = g_psyq_gpu_draw_sync_callback;
    g_psyq_gpu_draw_sync_callback = callback;
    return old;
}
