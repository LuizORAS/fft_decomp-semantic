#include "psx/libc.h"
#include "psx/libetc.h"
#include "psx/libgpu.h"

/* LIBGPU, retail 0x80026d10-0x80026e7c. Restores DMA/GPU state after a timeout. */
int get_alarm(void) {
    if (VSync(-1) > g_psyq_gpu_sync_deadline || g_psyq_gpu_sync_poll_count++ > 0x000f0000) {
        *g_psyq_gpu_gp1_port; /* Retail reads the status once and discards it. */
        printf(g_psyq_gpu_timeout_state_format, (g_psyq_gpu_queue_head - g_psyq_gpu_queue_tail) & 63,
            *g_psyq_gpu_gp1_port, *g_psyq_gpu_dma_chcr, *g_psyq_gpu_dma_madr);
        printf(g_psyq_gpu_timeout_operation_format, g_psyq_gpu_last_operation.operation,
            g_psyq_gpu_last_operation.source, g_psyq_gpu_last_operation.argument);
        g_psyq_gpu_reset_interrupt_mask = SetIntrMask(0);
        g_psyq_gpu_queue_head = g_psyq_gpu_queue_tail = 0;
        *g_psyq_gpu_dma_chcr = PSYQ_GPU_DMA_MODE_LINKED_LIST | PSYQ_GPU_DMA_TO_DEVICE;
        *g_psyq_gpu_dma_dpcr |= PSYQ_GPU_DMA_GPU_ENABLE;
        *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_ACK_IRQ);
        *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_RESET_BUFFER);
        SetIntrMask(g_psyq_gpu_reset_interrupt_mask);
        return -1;
    }
    return 0;
}
