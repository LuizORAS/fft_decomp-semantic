/* LIBGPU 80024b40-80024bd8. */
#include "psx/libgpu.h"

/* Make the `count` entries at `ot` an empty ordering table in reverse with the ordering-table DMA (_otc): each
 * links to the one below it, the first to the terminator, so drawing starts from the last entry. Returns `ot`.
 * MAIN, OPEN and LIBGS clear their tables this way. */
u32* ClearOTagR(u32* ot, int count) {
    if (g_psyq_gpu_environment.debug_level >= 2)
        g_psyq_gpu_printf(g_psyq_gpu_clear_otag_reverse_format, ot, count);
    g_psyq_gpu_dispatch->clear_ot_reverse(ot, count);
    *ot = (u32)&g_psyq_gpu_ot_terminator & PSYQ_GPU_DMA_ADDRESS_MASK;
    return ot;
}
