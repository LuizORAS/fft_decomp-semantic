#include "psx/libetc.h"
#include "psx/libgpu.h"

/* Run a GPU operation now or queue it. While the 64-entry queue is full it runs queued work (_exeque),
 * giving up with -1 on a timeout. With interrupts masked: when queueing is off, or the queue is empty with no
 * DMA busy and no DrawSync callback, it waits for the GPU and runs `operation` at once; otherwise it stores the
 * operation (copying `copy_size` bytes of `source` into the entry), sets _exeque as the GPU DMA callback and
 * starts the queue. Returns 0, or the number of entries waiting. */
int _addque2(psyq_gpu_operation_t operation, void* source, int copy_size, u32 argument) {
    extern volatile psyq_gpu_environment_t g_psyq_gpu_environment; /* The driver reads the environment as volatile. */
    int i;
    set_alarm();
    while (((g_psyq_gpu_queue_head + 1) & 63) == g_psyq_gpu_queue_tail) {
        if (get_alarm())
            return -1;
        _exeque();
    }
    g_psyq_gpu_enqueue_interrupt_mask = SetIntrMask(0);
    g_psyq_gpu_environment.completion_pending = 1;
    if (!g_psyq_gpu_environment.queue_enabled
        || (g_psyq_gpu_queue_head == g_psyq_gpu_queue_tail && !(*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY)
            && !g_psyq_gpu_environment.draw_sync_callback)) {
        while (!(*g_psyq_gpu_gp1_port & PSYQ_GPU_STATUS_READY_COMMAND))
            ;
        operation(source, argument);
        g_psyq_gpu_last_operation.operation = operation;
        g_psyq_gpu_last_operation.source = source;
        g_psyq_gpu_last_operation.argument = argument;
        SetIntrMask(g_psyq_gpu_enqueue_interrupt_mask);
        return 0;
    }
    DMACallback(PSYQ_GPU_DMA_GPU_CHANNEL, _exeque);
    if (copy_size) {
        for (i = 0; i < copy_size / 4; i++)
            g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].packet_words[i] = ((u32*)source)[i];
        g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].source
            = g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].packet_words;
    } else
        g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].source = source;
    g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].argument = argument;
    g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].operation = operation;
    g_psyq_gpu_queue_head = (g_psyq_gpu_queue_head + 1) & 63;
    SetIntrMask(g_psyq_gpu_enqueue_interrupt_mask);
    _exeque();
    return (g_psyq_gpu_queue_head - g_psyq_gpu_queue_tail) & 63;
}
