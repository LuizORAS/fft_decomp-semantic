/* LIBGPU 80024a88-80024b40. */
#include "psx/libgpu.h"
u32* ClearOTag(u32* ot, int count) {
    if (g_psyq_gpu_debug_level >= 2)
        g_psyq_gpu_printf(g_psyq_gpu_clear_otag_format, ot, count);
    while (--count) {
        P_TAG* entry = (P_TAG*)ot;
        u32* next = ot + 1;
        entry->len = 0;
        entry->addr = (u32)next;
        ot = next;
    }
    *ot = (u32)&g_psyq_gpu_ot_terminator & PSYQ_GPU_DMA_ADDRESS_MASK;
    return ot;
}
