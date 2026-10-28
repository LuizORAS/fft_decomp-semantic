/* LIBGPU 80024510-800245bc. */
#include "psx/libgpu.h"

/* Turn the operation queue on or off, emptying it (_reset(1)) and removing the DMA callback when the setting
 * changes; returns the old setting. No code refers to it. */
int SetGraphQueue(int enabled) {
    u8 old = g_psyq_gpu_environment.queue_enabled;
    if (g_psyq_gpu_environment.debug_level >= 2)
        g_psyq_gpu_printf(g_psyq_gpu_graph_queue_format, enabled);
    if (enabled != g_psyq_gpu_environment.queue_enabled) {
        g_psyq_gpu_dispatch->reset(1);
        g_psyq_gpu_environment.queue_enabled = enabled;
        DMACallback(PSYQ_DMA_GPU_CHANNEL, 0);
    }
    return old;
}
