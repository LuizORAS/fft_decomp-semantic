#include "psx/libetc.h"
#include "psx/libgpu.h"

/* LIBGPU, retail 0x8002675c-0x80026a58. DMA callback processes queued operations and completion. */
int _exeque(void) {
    extern volatile int g_psyq_gpu_queue_head;    /* Interrupt-visible head; retail retains each original load. */
    volatile psyq_gpu_operation_snapshot_t* last; /* This snapshot is observed by diagnostics/callbacks. */
    if (*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY)
        return 1;
    g_psyq_gpu_queue_interrupt_mask = SetIntrMask(0);
    while (g_psyq_gpu_queue_head != g_psyq_gpu_queue_tail && !(*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY)) {
        last = (psyq_gpu_operation_snapshot_t*)&g_psyq_gpu_last_operation;
        if (((g_psyq_gpu_queue_tail + 1) & 63) == g_psyq_gpu_queue_head) {
            void** callback = &g_psyq_gpu_draw_sync_callback;
            __asm__ volatile("" : "=r"(callback) : "0"(callback)); /* Preserve the materialized callback address. */
            if (!*callback)
                DMACallback(PSYQ_GPU_DMA_GPU_CHANNEL, 0);
        }
        {
            volatile u32* status = g_psyq_gpu_gp1_port;
            if (!(*status & PSYQ_GPU_STATUS_READY_COMMAND)) {
                volatile u32* polling_status = status;
                u32 ready = PSYQ_GPU_STATUS_READY_COMMAND;
                u32 sample;
                do {
                    sample = *polling_status;
                    sample &= ready;
                } while (!sample);
            }
        }
        {
            int operation_index = g_psyq_gpu_queue_tail;
            g_psyq_gpu_operation_queue[operation_index].operation(
                g_psyq_gpu_operation_queue[g_psyq_gpu_queue_tail].source,
                g_psyq_gpu_operation_queue[g_psyq_gpu_queue_tail].argument);
        }
        last->operation = g_psyq_gpu_operation_queue[g_psyq_gpu_queue_tail].operation;
        last->source = g_psyq_gpu_operation_queue[g_psyq_gpu_queue_tail].source;
        last->argument = g_psyq_gpu_operation_queue[g_psyq_gpu_queue_tail].argument;
        g_psyq_gpu_queue_tail = (g_psyq_gpu_queue_tail + 1) & 63;
    }
    SetIntrMask(g_psyq_gpu_queue_interrupt_mask);
    if (g_psyq_gpu_queue_head == g_psyq_gpu_queue_tail && !(*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY)) {
        int* completion_pending = &g_psyq_gpu_completion_pending;
        __asm__(""
            : "=r"(completion_pending)
            : "0"(completion_pending)); /* Preserve the original materialized flag pointer. */
        if (*completion_pending && g_psyq_gpu_draw_sync_callback) {
            *completion_pending = 0;
            ((void (*)(void))g_psyq_gpu_draw_sync_callback)();
        }
    }
    return (g_psyq_gpu_queue_head - g_psyq_gpu_queue_tail) & 63;
}
