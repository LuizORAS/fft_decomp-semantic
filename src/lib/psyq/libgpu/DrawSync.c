/* LIBGPU 800246d4-80024740. */
#include "psx/libgpu.h"

/* Wait until the queue, the GPU DMA and the GPU are done (mode 0), or only return how much is still pending
 * (mode 1), through the dispatch table's sync. About 80 functions wait for drawing or VRAM transfers this way. */
int DrawSync(int mode) {
    if (g_psyq_gpu_environment.debug_level >= 2) {
        g_psyq_gpu_printf(g_psyq_gpu_draw_sync_format, mode);
    }
    return g_psyq_gpu_dispatch->sync(mode);
}
