/* LIBGPU 80024a88-80024b40. */
#include "psx/libgpu.h"
u32* ClearOTag(void* ot, int count) {
    if (g_psyq_gpu_debug_level >= 2)
        g_psyq_gpu_printf(g_psyq_gpu_clear_otag_format, ot, count);
    while (--count) {
        P_TAG* entry = ot;
        P_TAG* next = (P_TAG*)((u32*)entry + 1);
        entry->len = 0;
        entry->addr = (u32)next;
        ot = next;
    }
    *(u32*)ot = (u32)&g_psyq_gpu_ot_terminator & PSYQ_GPU_DMA_ADDRESS_MASK;
    return ot;
}
