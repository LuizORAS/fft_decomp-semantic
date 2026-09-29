/* LIBGPU 80026a58-80026b94. */
#include "psx/libgpu.h"
int _reset(int mode) {
    int mask = SetIntrMask(0);
    int tail;
    g_psyq_gpu_queue_tail = 0; /* Queue indices are observed by DMA interrupt callbacks. */
    tail = g_psyq_gpu_queue_tail;
    g_psyq_gpu_reset_interrupt_mask = mask;
    g_psyq_gpu_queue_head = tail;
    switch (mode & 7) {
    case 0:
        *g_psyq_gpu_dma_chcr = (PSYQ_GPU_DMA_MODE_LINKED_LIST | PSYQ_GPU_DMA_TO_DEVICE);
        *g_psyq_gpu_dma_dpcr |= PSYQ_GPU_DMA_GPU_ENABLE;
        *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_RESET);
        memset2(g_psyq_gpu_gp1_cache, 0, 256);
        memset2(g_psyq_gpu_operation_queue, 0, 6144);
        break;
    case 1:
        *g_psyq_gpu_dma_chcr = (PSYQ_GPU_DMA_MODE_LINKED_LIST | PSYQ_GPU_DMA_TO_DEVICE);
        *g_psyq_gpu_dma_dpcr |= PSYQ_GPU_DMA_GPU_ENABLE;
        *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_ACK_IRQ);
        *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_RESET_BUFFER);
        break;
    }
    SetIntrMask(g_psyq_gpu_reset_interrupt_mask);
    if ((mode & 7) == 0)
        return psyq_gpu_detect_type(mode);
    return 0;
}
