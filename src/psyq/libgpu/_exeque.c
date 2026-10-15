#include "psx/libetc.h"
#include "psx/libgpu.h"

/* Run queued GPU operations while the GPU DMA is idle (it is also the GPU DMA completion callback): each
 * waits for the GPU to take commands; the callback is removed before the last entry when no DrawSync callback
 * is set. When the queue empties and the DMA is idle, call the DrawSync callback once. Returns the entries left,
 * or 1 at once when the DMA is busy. */
int _exeque(void) {
    extern volatile psyq_gpu_environment_t g_psyq_gpu_environment; /* The driver reads the environment as volatile. */
    if (*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY)
        return 1;
    g_psyq_gpu_queue_interrupt_mask = SetIntrMask(0);
    while (g_psyq_gpu_queue_head != g_psyq_gpu_queue_tail && !(*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY)) {
        if (((g_psyq_gpu_queue_tail + 1) & 63) == g_psyq_gpu_queue_head && !g_psyq_gpu_environment.draw_sync_callback)
            DMACallback(PSYQ_GPU_DMA_GPU_CHANNEL, 0);
        while (!(*g_psyq_gpu_gp1_port & PSYQ_GPU_STATUS_READY_COMMAND))
            ;
        g_psyq_gpu_operation_queue[g_psyq_gpu_queue_tail].operation(
            g_psyq_gpu_operation_queue[g_psyq_gpu_queue_tail].source,
            g_psyq_gpu_operation_queue[g_psyq_gpu_queue_tail].argument);
        g_psyq_gpu_last_operation.operation = g_psyq_gpu_operation_queue[g_psyq_gpu_queue_tail].operation;
        g_psyq_gpu_last_operation.source = g_psyq_gpu_operation_queue[g_psyq_gpu_queue_tail].source;
        g_psyq_gpu_last_operation.argument = g_psyq_gpu_operation_queue[g_psyq_gpu_queue_tail].argument;
        g_psyq_gpu_queue_tail = (g_psyq_gpu_queue_tail + 1) & 63;
    }
    SetIntrMask(g_psyq_gpu_queue_interrupt_mask);
    if (g_psyq_gpu_queue_head == g_psyq_gpu_queue_tail && !(*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY)
        && g_psyq_gpu_environment.completion_pending && g_psyq_gpu_environment.draw_sync_callback) {
        g_psyq_gpu_environment.completion_pending = 0;
        g_psyq_gpu_environment.draw_sync_callback();
    }
    return (g_psyq_gpu_queue_head - g_psyq_gpu_queue_tail) & 63;
}
