/* LIBGPU 800263d8-80026424. */
#include "psx/libgpu.h"

void _cwc(u32* ot) {
    *g_psyq_gpu_gp1_port = (PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DMA_DIRECTION) | PSYQ_GPU_DMA_REQUEST_WRITE);
    *g_psyq_gpu_dma_madr = (u32)ot;
    *g_psyq_gpu_dma_bcr = 0;
    *g_psyq_gpu_dma_chcr = (PSYQ_GPU_DMA_BUSY | PSYQ_GPU_DMA_MODE_LINKED_LIST | PSYQ_GPU_DMA_TO_DEVICE);
}
