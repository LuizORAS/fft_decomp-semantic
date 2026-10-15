/* LIBGPU 80024c38-80024cac. */
#include "psx/libetc.h"
#include "psx/libgpu.h"

/* Draw the packet chain starting at `ot`, through the queue (a linked-list DMA). The frame submitters of MAIN,
 * OPEN, BATTLE, WORLD, BUNIT and CARD, LIBGS (for WLDCORE) and FntFlush draw this way. */
void DrawOTag(u32* ot) {
    if (g_psyq_gpu_debug_level >= 2) {
        g_psyq_gpu_printf(g_psyq_gpu_draw_otag_format, ot);
    }
    g_psyq_gpu_dispatch->enqueue_four(g_psyq_gpu_dispatch->ordering_table, ot, 0, 0);
}
