/* LIBGPU 80025b5c-80025c44. */
#include "psx/libgpu.h"
int _otc(u32* ot, int count) {
    u32* position = ot;
    int final_offset;
    *g_psyq_gpu_dma_dpcr |= PSYQ_GPU_DMA_OTC_ENABLE;
    *g_psyq_gpu_otc_dma_chcr = 0;
    __asm__ volatile("" : : : "memory"); /* Keep address preparation after the DMA control clear. */
    final_offset = count * 4 - 4;
    __asm__(""
        : "=r"(final_offset)
        : "0"(final_offset)); /* Retain the original last-entry offset before adding the OT base. */
    position = (u32*)((u32)position + final_offset);
    *g_psyq_gpu_otc_dma_madr = (u32)position;
    *g_psyq_gpu_otc_dma_bcr = count;
    *g_psyq_gpu_otc_dma_chcr = (PSYQ_GPU_DMA_BUSY | PSYQ_GPU_DMA_TRIGGER | PSYQ_GPU_DMA_ADDRESS_DECREMENT);
    set_alarm();
    while (*g_psyq_gpu_otc_dma_chcr & PSYQ_GPU_DMA_BUSY) {
        if (get_alarm())
            return -1;
    }
    return count;
}
