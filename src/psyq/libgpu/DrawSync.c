/* LIBGPU 800246d4-80024740. */
#include "psx/libetc.h"
#include "psx/libgpu.h"

int DrawSync(int mode) {
    if (g_psyq_gpu_debug_level >= 2) {
        g_psyq_gpu_printf(g_psyq_gpu_draw_sync_format, mode);
    }
    return g_psyq_gpu_dispatch->sync(mode);
}
